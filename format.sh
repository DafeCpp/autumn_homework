#!/bin/bash
set -euo pipefail
cd "$(git rev-parse --show-toplevel)"
mode=${1:-all}
case "$mode" in all|changed|check) ;; *) echo "Usage: $0 [all|changed|check]" >&2; exit 2 ;; esac
formatter=${CLANG_FORMAT:-clang-format}
command -v "$formatter" >/dev/null || { echo "Missing formatter: $formatter" >&2; exit 1; }
files=$(mktemp)
trap 'rm -f "$files"' EXIT
if [ "$mode" = changed ]; then
    git diff HEAD --name-only --diff-filter=ACMR -z > "$files"
    git ls-files --others --exclude-standard -z >> "$files"
else
    git ls-files --cached --others --exclude-standard -z > "$files"
fi
status=0
while IFS= read -r -d '' file; do
    case "$file" in *.cpp|*.h|*.hpp) ;; *) continue ;; esac
    [ -f "$file" ] || continue
    echo "$file"
    if [ "$mode" = check ]; then
        "$formatter" --dry-run --Werror -style=file "$file" || status=1
    else
        "$formatter" -i -style=file "$file" || status=1
    fi
done < "$files"
exit "$status"
