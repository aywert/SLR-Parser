#!/bin/sh
set -eu

SLR_BIN="${1:-}"

if [ -z "$SLR_BIN" ]; then
  echo "ERROR: Matrix binary path not provided" >&2
  exit 1
fi

if [ ! -x "$SLR_BIN" ]; then
  echo "ERROR: Matrix binary is not executable or not found: $SLR_BIN" >&2
  exit 1
fi

SCRIPT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
DATA_DIR="$SCRIPT_DIR/data"
ANSW_DIR="$SCRIPT_DIR/answ"

echo "========================================="
echo "🔍 Starting end-to-end tests"
echo "📂 Program: $SLR_BIN"
echo "📁 Data directory: $DATA_DIR"
echo "📁 Answers directory: $ANSW_DIR"
echo "========================================="

found_any=false
passed=0
failed=0
total=0

for in_file in "$DATA_DIR"/*.txt; do
  [ -e "$in_file" ] || continue
  found_any=true

  name=$(basename "$in_file")
  exp_file="$ANSW_DIR/$name"

  if [ ! -f "$exp_file" ]; then
    echo "ERROR: expected answer file not found: $exp_file" >&2
    exit 1
  fi

  if "$SLR_BIN" "$in_file" | diff -u "$exp_file" - >/dev/null; then
    echo "✅ PASS: $name"
    passed=$((passed + 1))
  else
    echo "❌ FAIL: $name"
    failed=$((failed + 1))
    
    # Показываем детали ошибки
    echo "   └─ Input: $in_file"
    echo "   └─ Expected: $exp_file"
    echo "   └─ Difference:"
    "$SLR_BIN" "$in_file" | diff -u "$exp_file" - | sed 's/^/        /'
  fi
done

echo "========================================="
echo "📊 SUMMARY"
echo "   Total:  $total"
echo "   ✅ Passed: $passed"
echo "   ❌ Failed: $failed"
echo "========================================="

if [ "$found_any" = false ]; then
  echo "ERROR: no input files found in $DATA_DIR" >&2
  exit 1
fi

echo "all tests passed successfully!"