#!/usr/bin/env bash

set -euo pipefail

FILES=$(git diff --cached --name-only --diff-filter=ACMR \
    | grep -E '\.(c|cpp|cc|cxx|h|hpp)$' || true)

[ -z "$FILES" ] && exit 0

for file in $FILES; do
    clang-tidy \
        -p build \
        "$file"
done
