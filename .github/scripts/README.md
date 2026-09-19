# clang-tidy reviews

`clang-tidy-review / build` checks the PR head with read-only permissions.
The CMake compilation database supplies include paths, including `lib/src`.
The preflight runs clang-tidy on changed source files without a line filter and
checks its exit status. This is necessary because cpp-linter 1.13.0 (bundled by
cpp-linter-action v2.21.0) ignores that status and can swallow process errors.
Warnings do not fail the job. Configuration errors, missing headers, failed
processes and invalid/missing compilation databases do.

The action then annotates changed lines and writes the complete Markdown report
to `build/clang-tidy-report.md`. Its artifact is named
`clang-tidy-report-<run_id>-<run_attempt>`. GitHub can truncate annotations, so the
review always links to the source run and its complete report artifact.

`clang-tidy-post` runs the publisher from the default-branch commit associated
with its own `workflow_run` event. It never checks out PR code or downloads
artifacts. It reads only the check attached to `build` in the triggering run
attempt. A failed analysis or `failure` annotation fails publication; API errors
propagate. PR matching requires the head SHA, head repository and base repository
to agree. Ambiguous or stale matches are skipped. A final check also verifies
that the base SHA has not changed while preparing the review.

Only added lines receive inline comments, in a single `COMMENT` review with at
most ten diagnostics. Paths without a patch (including oversized/binary diffs)
are skipped. No suggestions, approvals, change requests or LGTM are posted.
SHA/path/line/check/message hashes in bot review bodies prevent duplicates.
The first ten are selected deterministically before deduplication, so rerunning
a large report does not gradually post every remaining diagnostic. A changed
diagnostic or a new head SHA may produce new feedback. Publication is serialized
per head repository and SHA.

Run the checks locally:

```sh
node --test .github/scripts/clang-tidy-post.test.cjs
PYTHONDONTWRITEBYTECODE=1 python3 -m unittest discover -s .github/scripts -p 'test_*.py'
actionlint .github/workflows/clang-tidy-review.yaml .github/workflows/clang-tidy-post.yaml
```

The real-tool Python test uses `clang-tidy-19`, or the executable specified in
`CLANG_TIDY`. It skips only that test when the executable is absent. CI installs
clang-tidy-19 before running the tests.

Merge the migration PR into `main` before testing publication from a fork:
`workflow_run` uses the posting workflow from the default branch. Update the
fork branch from `main` so it includes the new analysis workflow and scripts,
then push a new commit (rerunning an old run retains its old workflow version).
Verify that a diagnostic appears on the correct added line with both jobs green,
and that rerunning it creates no duplicate review. Separately verify a missing
header makes analysis and publication red. Existing C++ test and formatting
workflows are independent of this migration.

Migration verification: cpp-linter 1.13.0 and clang-tidy 19.1.0 analyzed the three
changed C++ files from PR #22 at `7d89115844e3fae12dced27f068adfe203cebf88`.
The preflight passed; the changed-line pass produced 28 warnings and a UTF-8
Markdown report, with no missing `graphs.hpp` or decoding error. Source SHA-256
hashes were unchanged. This local check used macOS SDK/GTest paths only in its
temporary compilation database; Ubuntu CI uses the CMake database unmodified.
A direct cpp-linter probe confirmed that a tool exiting 139 returns an empty
advice list, and a missing header returns error advice without failing the
caller. The guard tests cover both cases with nonzero failures.
