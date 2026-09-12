'use strict';
const {test} = require('node:test');
const assert = require('node:assert/strict');
const post = require('./clang-tidy-post.cjs');

function fixture() {
  const run = {
    id: 123, run_attempt: 2, repository: {full_name: 'owner/repo'},
    event: 'pull_request', path: '.github/workflows/clang-tidy-review.yaml',
    head_sha: 'abc123', head_repository: {id: 17}, conclusion: 'success',
    html_url: 'https://github.com/owner/repo/actions/runs/123', pull_requests: [{number: 22}],
  };
  const pr = {
    number: 22, state: 'open', base: {sha: 'base', repo: {full_name: 'owner/repo'}},
    head: {sha: run.head_sha, repo: {id: 17}},
  };
  const state = {
    run, pr, current: null, open: [pr], reviews: [], posted: [], calls: [],
    jobs: [{name: 'build', run_id: 123, run_attempt: 2, conclusion: 'success',
      check_run_url: 'https://api.github.com/repos/owner/repo/check-runs/456'}],
    annotations: [{path: 'src/main.cpp', start_line: 12, end_line: 12,
      annotation_level: 'warning', title: 'src/main.cpp:12:5 [bugprone-test]',
      message: 'Предупреждение после кириллицы'}],
    files: [{filename: 'src/main.cpp', status: 'modified', patch:
      '@@ -9,3 +9,4 @@\n // Кириллица\n int main() {\n-  old();\n+  safe();\n+  warning();'}],
  };
  const method = (name, result) => async params => {
    state.calls.push([name, params]);
    return {data: typeof result === 'function' ? result(params) : result};
  };
  let gets = 0;
  const github = {
    rest: {
      actions: {
        getWorkflowRunAttempt: method('run', () => state.run),
        listJobsForWorkflowRunAttempt: method('jobs', () => state.jobs),
      },
      checks: {listAnnotations: method('annotations', () => state.annotations)},
      pulls: {
        get: method('get', () => ++gets > 1 && state.current ? state.current : state.pr),
        list: method('open', () => state.open),
        listFiles: method('files', () => state.files),
        listReviews: method('reviews', () => state.reviews),
        createReview: method('create', params => {
          state.posted.push(params);
          state.reviews.push({...params, user: {login: 'github-actions[bot]', type: 'Bot'}});
        }),
      },
    },
    paginate: async (method, params) => (await method(params)).data,
  };
  state.execute = () => post({github, context: {
    repo: {owner: 'owner', repo: 'repo'}, payload: {workflow_run: {...run}},
  }, core: {info() {}}});
  state.github = github;
  return state;
}

test('Unicode and diff coordinates; only COMMENT on the exact SHA', async () => {
  const f = fixture();
  await f.execute();
  assert.equal(f.posted.length, 1);
  assert.equal(f.posted[0].event, 'COMMENT');
  assert.equal(f.posted[0].commit_id, 'abc123');
  assert.equal(f.posted[0].comments[0].line, 12);
  assert.equal(f.posted[0].comments[0].side, 'RIGHT');
  assert.match(f.posted[0].comments[0].body, /Предупреждение после кириллицы/);
  assert.match(f.posted[0].body, /\/runs\/123\/attempts\/2/);
  assert.equal(f.calls.find(([name]) => name === 'jobs')[1].attempt_number, 2);
  assert.equal(f.calls.find(([name]) => name === 'annotations')[1].check_run_id, 456);
});

test('hunks, deletions, context, and no-newline markers', () => {
  assert.deepEqual([...post.addedLines(
    '@@ -1,2 +1,2 @@\n-old\n+new\n context\n\\ No newline at end of file\n'
    + '@@ -10 +20,2 @@\n same\n+added')], [1, 21]);
  assert.equal(post.addedLines().size, 0);
});

test('empty report does not post LGTM', async () => {
  const f = fixture(); f.annotations = []; await f.execute();
  assert.equal(f.posted.length, 0);
});

test('reruns and duplicated annotations do not duplicate comments', async () => {
  const f = fixture(); f.annotations.push({...f.annotations[0]});
  await f.execute(); await f.execute();
  assert.equal(f.posted.length, 1);
  assert.equal(f.posted[0].comments.length, 1);
});

test('fork fallback matches SHA AND head repository uniquely', async () => {
  const f = fixture(); f.run.pull_requests = [];
  f.open.push({...f.pr, number: 23, head: {sha: f.run.head_sha, repo: {id: 99}}});
  await f.execute(); assert.equal(f.posted.length, 1);
  assert.ok(f.calls.some(([name]) => name === 'open'));
  for (const open of [[], [f.pr, {...f.pr, number: 24}]]) {
    const other = fixture(); other.run.pull_requests = []; other.open = open;
    await other.execute(); assert.equal(other.posted.length, 0);
  }
});

test('stale SHA, closed PR, foreign base or fork are skipped', async () => {
  for (const mutate of [
    pr => {pr.head.sha = 'new';}, pr => {pr.state = 'closed';},
    pr => {pr.base.repo.full_name = 'elsewhere/repo';}, pr => {pr.head.repo.id = 9;},
  ]) {
    const f = fixture(); mutate(f.pr); await f.execute();
    assert.equal(f.posted.length, 0);
  }
});

test('recheck immediately before posting catches changed head or base', async () => {
  for (const mutate of [
    pr => {pr.head.sha = 'new';}, pr => {pr.base.sha = 'new-base';},
    pr => {pr.state = 'closed';},
  ]) {
    const f = fixture(); f.current = structuredClone(f.pr); mutate(f.current);
    await f.execute(); assert.equal(f.posted.length, 0);
  }
});

test('outside diff, context, removed files, missing patches and non-tidy notices skipped', async () => {
  for (const mutate of [
    f => {f.annotations[0].start_line = 9;},
    f => {f.annotations[0].path = 'elsewhere.cpp';},
    f => {f.files[0].status = 'removed';},
    f => {delete f.files[0].patch;},
    f => {f.annotations[0].annotation_level = 'notice';},
    f => {f.annotations[0].title = 'unrelated warning';},
  ]) {
    const f = fixture(); mutate(f); await f.execute(); assert.equal(f.posted.length, 0);
  }
});

test('limit ten, stable across reruns and annotation ordering', async () => {
  const f = fixture();
  f.annotations = Array.from({length: 15}, (_, i) => ({...f.annotations[0], message: `Warning ${i}`}));
  await f.execute();
  assert.equal(f.posted[0].comments.length, 10);
  f.annotations.reverse(); await f.execute();
  assert.equal(f.posted.length, 1);
});

test('failure annotations anywhere, failed analysis and API failures reject', async () => {
  const failed = fixture();
  failed.annotations.push({annotation_level: 'failure', path: 'outside.cpp'});
  await assert.rejects(failed.execute(), /technical error/);
  const unsuccessful = fixture(); unsuccessful.run.conclusion = 'failure';
  await assert.rejects(unsuccessful.execute(), /did not complete/);
  const api = fixture();
  api.github.rest.pulls.createReview = async () => {throw new Error('API 403');};
  await assert.rejects(api.execute(), /API 403/);
  const read = fixture();
  read.github.rest.checks.listAnnotations = async () => {throw new Error('API 500');};
  await assert.rejects(read.execute(), /API 500/);
});

test('reject wrong workflow, job attempt, missing or ambiguous build', async () => {
  for (const mutate of [
    f => {f.run.path = '.github/workflows/other.yaml';},
    f => {f.jobs[0].run_attempt = 1;}, f => {f.jobs[0].run_id = 999;},
    f => {f.jobs = [];}, f => {f.jobs.push({...f.jobs[0]});},
  ]) {
    const f = fixture(); mutate(f); await assert.rejects(f.execute());
    assert.equal(f.posted.length, 0);
  }
});

test('diagnostic Markdown and mentions are rendered as text', async () => {
  const f = fixture(); f.annotations[0].message = '@someone <script> ```suggestion\nevil';
  await f.execute();
  const body = f.posted[0].comments[0].body;
  assert.ok(!body.includes('@someone'));
  assert.ok(!body.includes('<script>'));
  assert.ok(!body.includes('```suggestion'));
});

test('a user cannot spoof deduplication markers', async () => {
  const f = fixture(); await f.execute();
  f.reviews[0].user = {login: 'contributor', type: 'User'};
  await f.execute(); assert.equal(f.posted.length, 2);
});
