import importlib.util
import json
from pathlib import Path
import subprocess
import tempfile
import unittest
from unittest.mock import patch

spec = importlib.util.spec_from_file_location(
    'preflight', Path(__file__).with_name('clang-tidy-preflight.py'))
preflight = importlib.util.module_from_spec(spec)
spec.loader.exec_module(preflight)


class PreflightTest(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)
        self.source = self.root / 'main.cpp'
        self.source.write_text('// Кириллица перед диагностикой\nint main() {}\n')
        self.database = self.root / 'compile_commands.json'
        self.database.write_text(json.dumps([{
            'file': str(self.source), 'directory': str(self.root),
            'command': f'c++ -c {self.source}',
        }]))

    def check(self):
        preflight.check(self.root, [str(self.source)], '-*,bugprone-*')

    def test_missing_database(self):
        self.database.unlink()
        with self.assertRaises(FileNotFoundError):
            self.check()

    def test_empty_or_invalid_database(self):
        for value in ('', '[]', '{}', '[{}]'):
            self.database.write_text(value)
            with self.assertRaises(ValueError):
                self.check()

    @patch.object(preflight.subprocess, 'run')
    def test_warnings_are_advisory(self, run):
        run.return_value = subprocess.CompletedProcess([], 0, 'warning: замечание', '')
        self.check()
        args = run.call_args.args[0]
        self.assertIn('--warnings-as-errors=', args)
        self.assertFalse(any('line-filter' in arg for arg in args))
        self.assertEqual(run.call_args.kwargs['encoding'], 'utf-8')

    @patch.object(preflight.subprocess, 'run')
    def test_missing_header_and_tool_crash(self, run):
        for code, message in ((1, "error: 'graphs.hpp' file not found"),
                              (139, 'Segmentation fault'), (1, 'invalid configuration')):
            run.side_effect = [subprocess.CompletedProcess([], 0),
                               subprocess.CompletedProcess([], code, message, '')]
            with self.assertRaises(RuntimeError):
                self.check()

    @patch.object(preflight.subprocess, 'run', side_effect=FileNotFoundError('clang-tidy'))
    def test_missing_tool(self, run):
        with self.assertRaises(FileNotFoundError):
            self.check()


class RealToolTest(unittest.TestCase):
    """Exercise actual exit codes as well as the mocked API above."""
    setUp = PreflightTest.setUp

    def test_real_clang_tidy(self):
        import os
        import shutil
        tool = os.environ.get('CLANG_TIDY', shutil.which('clang-tidy-19'))
        if not tool:
            self.skipTest('clang-tidy-19 is not installed')
        self.source.write_text('// Русский комментарий\nint answer() { return 42; }\n', encoding='utf-8')
        preflight.check(self.root, [str(self.source)],
                        '-*,cppcoreguidelines-avoid-magic-numbers', tool)
        self.source.write_text('#include "nonexistent-header.hpp"\nint answer();\n')
        with self.assertRaises(RuntimeError):
            preflight.check(self.root, [str(self.source)], '-*,bugprone-*', tool)

    def test_process_crash(self):
        tool = self.root / 'crashing-clang-tidy'
        tool.write_text('#!/bin/sh\nif [ "$1" = "--version" ]; then exit 0; fi\nexit 139\n')
        tool.chmod(0o700)
        with self.assertRaises(RuntimeError):
            preflight.check(self.root, [str(self.source)], '-*,bugprone-*', str(tool))


if __name__ == '__main__':
    unittest.main()
