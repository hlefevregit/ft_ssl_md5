#!/usr/bin/env bash
set -euo pipefail

# Parameters
INPUT="Make sure they really tell you what is going on 'under the hood' when you use CBC mode, and how it is more secure than ECB"
KEY="BABE101010FACADE"
IV="77696E6B66616365"
PASSWORD="password"

# Test 1: explicit key/IV
echo "=== Test 1: DES-CBC explicit key/IV round‑trip ==="
echo "Plaintext: $INPUT"
OUTPUT=$(printf "%s" "$INPUT" \
  | openssl enc -des-cbc \
      -K "$KEY" -iv "$IV" \
      -provider default -provider legacy \
  | ./ft_ssl des-cbc -d -k "$KEY" -v "$IV")

if [[ "$OUTPUT" == "$INPUT" ]]; then
  echo "Result: ✅ PASSED"
else
  echo "Result: ❌ FAILED"
  echo "Expected: $INPUT"
  echo "Got     : $OUTPUT"
  exit 1
fi
echo

echo "=== All DES-CBC tests completed ==="
