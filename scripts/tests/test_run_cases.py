"""CLI regressions using isolated tasks; never run or modify student solutions."""
import hashlib
import json
from pathlib import Path
import shutil
import subprocess
import sys
import tempfile
import unittest

RUNNER = Path(__file__).resolve().parents[1] / 'run_cases.py'


class RunnerTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory(prefix='runner tests ')
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        (self.root / 'scripts').mkdir()
        shutil.copyfile(RUNNER, self.root / 'scripts/run_cases.py')
        self.tests = self.root / 'task_03/tests'
        self.tests.mkdir(parents=True)
        self.exe = self.root / 'build/task_03/task_03'
        self.exe.parent.mkdir(parents=True)
        self.traces = self.root / 'traces/task_03'

    def program(self, body):
        self.exe.write_text(f'#!{sys.executable}\n' + body, encoding='utf-8')
        self.exe.chmod(0o755)

    def case(self, name, input_text='1 0\n', expected='ok\n'):
        path = self.tests / (name + '.in')
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(input_text, encoding='utf-8')
        if expected is not None:
            path.with_suffix('.out').write_text(expected, encoding='utf-8')
        return path

    def run_cases(self, *args, trace=True):
        command = [sys.executable, str(self.root / 'scripts/run_cases.py'), '--tasks', 'task_03']
        if trace:
            command += ['--trace-dir', str(self.traces.parent)]
        return subprocess.run(command + list(args), cwd=self.root, text=True, capture_output=True, timeout=15)

    def report(self):
        return json.loads((self.traces / 'results.json').read_text())

    def test_all_verdicts_and_exit_code_124_is_not_timeout(self):
        self.program('import sys,time\ns=sys.stdin.read().strip()\n'
                     'if s=="timeout": time.sleep(30)\n'
                     'if s=="error": print("ok"); print("crash",file=sys.stderr); sys.exit(124)\n'
                     'print("wrong" if s=="wrong" else "ok")\n')
        for name in ['ok', 'wrong', 'error', 'timeout']:
            self.case(name, name+'\n')
        # Allow interpreter startup under concurrent C++ builds on CI machines.
        result = self.run_cases('--timeout', '2', '--save-actual')
        self.assertEqual(result.returncode, 1, result.stderr)
        statuses = {Path(r['id']).stem: r['status'] for r in self.report()}
        self.assertEqual(statuses, {'ok':'ok','wrong':'fail','error':'error','timeout':'timeout'})
        for text in ['Passed: 1/4', 'Wrong Answer', 'Runtime Error', 'Time Limit Exceeded',
                     'exit code 124', 'First difference at line 1', 'Run manually:']:
            self.assertIn(text, result.stdout)
        self.assertEqual((self.tests/'wrong.out.actual').read_text(), 'wrong\n')
        self.assertFalse((self.tests/'error.out.actual').exists())
        self.program('print("ok")\n')
        self.assertEqual(self.run_cases().returncode, 0)
        self.assertFalse((self.tests/'wrong.out.actual').exists())

    def test_crash_never_creates_or_overwrites_expected(self):
        self.program('import sys\nprint("bad")\nsys.exit(2)\n')
        self.case('existing')
        self.case('missing', expected=None)
        result = self.run_cases('--update-expected', '--write-missing')
        self.assertEqual(result.returncode, 1)
        self.assertEqual((self.tests/'existing.out').read_text(), 'ok\n')
        self.assertFalse((self.tests/'missing.out').exists())
        self.assertTrue(all(r['status']=='error' for r in self.report()))

    def test_reports_hash_the_compared_snapshot_even_if_solution_changes_files(self):
        path = self.case('changed', 'original\n', 'old\n')
        self.program('from pathlib import Path\n'
                     'Path("task_03/tests/changed.in").write_text("new input\\n")\n'
                     'Path("task_03/tests/changed.out").write_text("new\\n")\nprint("new")\n')
        result = self.run_cases()
        self.assertEqual(result.returncode, 1)
        record = self.report()[0]
        self.assertEqual(record['status'], 'fail')
        self.assertEqual(record['inputHash'], hashlib.sha256(b'original\n').hexdigest())
        self.assertEqual(record['expectedHash'], hashlib.sha256(b'old\n').hexdigest())
        self.assertNotEqual(record['inputHash'], hashlib.sha256(path.read_bytes()).hexdigest())

    def test_equal_names_in_custom_builtin_and_root_keep_separate_traces(self):
        self.program('import os,sys\nfrom pathlib import Path\ns=sys.stdin.read()\n'
                     'Path(os.environ["GRAPH_TRACE"]).write_text(s)\nprint("ok")\n')
        self.case('same', 'builtin\n')
        self.case('custom/same', 'custom\n')
        self.case('../same', 'root\n')
        result = self.run_cases()
        self.assertEqual(result.returncode, 0, result.stdout+result.stderr)
        self.assertEqual(len({r['id'] for r in self.report()}), 3)
        for group, text in [('', 'builtin\n'), ('custom','custom\n'), ('root','root\n')]:
            self.assertEqual((self.traces/group/'same.jsonl').read_text(), text)

    def test_missing_executable_clears_old_trace_and_saved_mismatch(self):
        self.case('one')
        self.traces.mkdir(parents=True)
        (self.traces/'one.jsonl').write_text('old trace')
        (self.tests/'one.out.actual').write_text('old answer')
        result = self.run_cases('--save-actual')
        self.assertEqual(result.returncode, 1)
        self.assertEqual(self.report()[0]['status'], 'exec_missing')
        self.assertFalse((self.traces/'one.jsonl').exists())
        self.assertFalse((self.tests/'one.out.actual').exists())

    def test_standalone_runner_works_without_recordings(self):
        self.program('print("ok")\n')
        self.case('one')
        result = self.run_cases(trace=False)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertIn('Test 1 (one): OK', result.stdout)
        self.assertFalse(self.traces.exists())

    def test_write_missing_hashes_new_expected_and_missing_without_flag_fails(self):
        self.program('print("ok")\n')
        self.case('one', expected=None)
        self.assertEqual(self.run_cases().returncode, 1)
        self.assertEqual(self.report()[0]['status'], 'no_expected')
        self.assertEqual(self.run_cases('--write-missing').returncode, 0)
        record = self.report()[0]
        self.assertEqual(record['status'], 'ok')
        self.assertEqual(record['expectedHash'], hashlib.sha256(b'ok\n').hexdigest())


if __name__ == '__main__':
    unittest.main()
