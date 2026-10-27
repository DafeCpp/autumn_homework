'use strict';

const {createHash} = require('node:crypto');

// GitHub patch hunk coordinates already refer to the new (RIGHT) side.
function addedLines(patch = '') {
  const lines = new Set();
  let next = null;
  for (const text of patch.split('\n')) {
    const hunk = /^@@ -\d+(?:,\d+)? \+(\d+)(?:,\d+)? @@/.exec(text);
    if (hunk) next = Number(hunk[1]);
    else if (next !== null && text.startsWith('+')) lines.add(next++);
    else if (next !== null && text.startsWith(' ')) next++;
  }
  return lines;
}

function fingerprint(sha, annotation, rule) {
  return createHash('sha256').update(JSON.stringify([
    sha, annotation.path, annotation.start_line, rule, annotation.message,
  ])).digest('hex');
}

// Treat diagnostics as text, including any Markdown or mentions from PR source.
function escapeText(text) {
  return String(text).replace(/&/g, '&amp;').replace(/</g, '&lt;')
    .replace(/>/g, '&gt;').replace(/@/g, '&#64;')
    .replace(/[\\`*_{}\[\]()#+.!|~-]/g, '\\$&');
}

async function post({github, context, core}) {
  const repo = context.repo;
  const event = context.payload.workflow_run;
  if (event.event !== 'pull_request') return;
  const {data: run} = await github.rest.actions.getWorkflowRunAttempt({
    ...repo, run_id: event.id, attempt_number: event.run_attempt,
  });
  if (run.id !== event.id || run.run_attempt !== event.run_attempt
      || run.repository?.full_name !== `${repo.owner}/${repo.repo}`
      || run.event !== 'pull_request'
      || run.path !== '.github/workflows/clang-tidy-review.yaml'
      || run.head_sha !== event.head_sha
      || run.head_repository?.id !== event.head_repository?.id) {
    throw new Error('Unexpected source workflow run');
  }

  const jobs = await github.paginate(github.rest.actions.listJobsForWorkflowRunAttempt, {
    ...repo, run_id: run.id, attempt_number: run.run_attempt, per_page: 100,
  });
  const builds = jobs.filter(job => job.name === 'build');
  if (builds.length !== 1) throw new Error('Expected exactly one build job');
  const job = builds[0];
  // Use only the check attached to this job, not all checks for a SHA.
  const checkId = /\/check-runs\/(\d+)$/.exec(job.check_run_url || '')?.[1];
  if (!checkId || job.run_id !== run.id || job.run_attempt !== run.run_attempt) {
    throw new Error('Build job does not belong to the triggering attempt');
  }
  const annotations = await github.paginate(github.rest.checks.listAnnotations, {
    ...repo, check_run_id: Number(checkId), per_page: 100,
  });
  if (annotations.some(a => a.annotation_level === 'failure')) {
    throw new Error(`Analysis reported a technical error; see ${run.html_url}`);
  }
  if (run.conclusion === 'cancelled') return;
  if (run.conclusion !== 'success' || job.conclusion !== 'success') {
    throw new Error(`Analysis did not complete successfully; see ${run.html_url}`);
  }

  function matches(pr) {
    return pr.state === 'open' && pr.base?.repo?.full_name === `${repo.owner}/${repo.repo}`
      && pr.head?.sha === run.head_sha && Boolean(run.head_repository?.id)
      && pr.head?.repo?.id === run.head_repository.id;
  }
  const linked = run.pull_requests || [];
  let candidates;
  if (linked.length) {
    candidates = await Promise.all([...new Set(linked.map(pr => pr.number))].map(async number => {
      const {data} = await github.rest.pulls.get({...repo, pull_number: number});
      return data;
    }));
  } else {
    // Fork runs can have no associated PR in workflow_run (e.g. PR #22).
    candidates = await github.paginate(github.rest.pulls.list, {
      ...repo, state: 'open', per_page: 100,
    });
  }
  candidates = candidates.filter(matches);
  if (candidates.length !== 1) {
    core.info('No unique open PR for the analyzed head SHA and repository; skipping.');
    return;
  }
  const pr = candidates[0];
  const params = {...repo, pull_number: pr.number, per_page: 100};
  const files = await github.paginate(github.rest.pulls.listFiles, params);
  const added = new Map(files.filter(f => f.status !== 'removed')
    .map(f => [f.filename, addedLines(f.patch)]));
  const reviews = await github.paginate(github.rest.pulls.listReviews, params);
  const seen = new Set();
  for (const review of reviews) {
    if (review.user?.login !== 'github-actions[bot]' || review.user?.type !== 'Bot') continue;
    for (const match of (review.body || '').matchAll(/<!-- cpp-linter:([a-f0-9]{64}) -->/g)) {
      seen.add(match[1]);
    }
  }
  const unique = new Map();
  for (const annotation of annotations) {
    if (annotation.annotation_level !== 'warning'
        || !added.get(annotation.path)?.has(annotation.start_line)) continue;
    // cpp-linter 1.13.0 titles: path:line:column [check-name].
    const rule = /\[([a-zA-Z0-9.,-]+)\]$/.exec(annotation.title || '')?.[1];
    if (!rule) continue;
    const key = fingerprint(run.head_sha, annotation, rule);
    unique.set(key, {
      path: annotation.path, line: annotation.start_line, side: 'RIGHT',
      body: `**${escapeText(rule)}**\n\n${escapeText(annotation.message)}`,
    });
  }
  // Choose a stable first ten before deduplication: reruns must not drip-feed
  // another ten comments for the same diagnostics on the same commit.
  const selected = [...unique.entries()].sort(([a], [b]) => a.localeCompare(b))
    .slice(0, 10).filter(([key]) => !seen.has(key));
  if (!selected.length) return;

  const {data: current} = await github.rest.pulls.get({...repo, pull_number: pr.number});
  if (!matches(current) || current.base.sha !== pr.base.sha) {
    core.info('PR closed or changed while preparing review; skipping.');
    return;
  }
  await github.rest.pulls.createReview({
    ...repo, pull_number: pr.number, commit_id: run.head_sha, event: 'COMMENT',
    body: `Рекомендации clang-tidy: ${selected.length} (не более 10). `
      + `Аннотации GitHub могут быть усечены. `
      + `[Полный отчёт CI и артефакт clang-tidy-report](${run.html_url}/attempts/${run.run_attempt}).\n\n`
      + selected.map(([key]) => `<!-- cpp-linter:${key} -->`).join('\n'),
    comments: selected.map(([, comment]) => comment),
  });
}

module.exports = post;
module.exports.addedLines = addedLines;
