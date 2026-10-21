#!/bin/sh
# Shell script deliberately does not install compilers or modify source files.
set -eu
HERE="$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)"
TMPBIN="$(mktemp "${TMPDIR:-/tmp}/msp430-l11-test.XXXXXX")"
trap 'rm -f "$TMPBIN"' EXIT INT TERM
cc -std=c99 -Wall -Wextra -Werror -o "$TMPBIN" "$HERE/test_logic.c"
"$TMPBIN"
