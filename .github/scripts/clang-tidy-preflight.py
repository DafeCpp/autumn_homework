"""Catch technical failures which cpp-linter 1.13.0 does not propagate."""
import json
import os
from pathlib import Path
import subprocess
import sys

EXTENSIONS = set('c,h,C,H,cpp,hpp,cc,hh,c++,h++,cxx,hxx'.split(','))


def check(database, files, checks, tool='clang-tidy-19'):
    entries = json.loads((Path(database) / 'compile_commands.json').read_text(encoding='utf-8'))
    if not isinstance(entries, list) or not entries:
        raise ValueError('Compilation database must be a nonempty array')
    for entry in entries:
        if not all(entry.get(key) for key in ('file', 'directory')) or not (
            entry.get('command') or entry.get('arguments')
        ):
            raise ValueError('Invalid compilation database entry')
    subprocess.run([tool, '--version'], check=True, capture_output=True)
    for filename in files:
        path = Path(filename)
        if (path.suffix.lstrip('.') not in EXTENSIONS
                or any(part.startswith('.') for part in path.parts)
                or not path.is_file()):
            continue
        # Do not use a line filter: a missing include may precede changed lines.
        result = subprocess.run(
            [tool, '-p', str(database), f'--checks={checks}',
             '--warnings-as-errors=', str(path)],
            capture_output=True, encoding='utf-8', errors='replace',
        )
        if result.returncode:
            # Indent untrusted compiler output so it cannot become workflow commands.
            output = result.stdout + result.stderr
            print('\n'.join('    ' + line for line in output.splitlines()))
            raise RuntimeError(f'clang-tidy failed for {filename}: exit {result.returncode}')


def main():
    files = subprocess.check_output(
        ['git', 'diff', '--name-only', '-z', '--diff-filter=ACMR',
         f'{os.environ["BASE_SHA"]}...HEAD'],
    ).decode('utf-8').split('\0')
    check('build', files, os.environ['TIDY_CHECKS'])


if __name__ == '__main__':
    try:
        main()
    except (OSError, ValueError, RuntimeError, subprocess.SubprocessError) as error:
        print(f'Analysis preflight failed: {error}', file=sys.stderr)
        sys.exit(1)
