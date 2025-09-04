#!/usr/bin/env bash
# tester_des_ecb.sh - Enhanced test suite for DES ECB
# Uses OpenSSL legacy provider so DES-ECB is available

export DES_KEY="C0FFEE69DEADBEEF"

echo "Starting DES-ECB tests..."

test_roundtrip() {
  local label="$1"
  local input="$2"
  local pipeline="$3"

  # Execute the pipeline and capture its output
  local output
  output=$(printf "%s" "$input" | eval "$pipeline")

  # Compare the round-trip result to the original input
  if [[ "$output" == "$input" ]]; then
    echo -e "${label}: ✅ PASSED"
  else
    echo -e "${label}: ❌ FAILED"
    echo -e "  Input   : '$input'"
    echo -e "  Output  : '$output'"
  fi
}

# Test 1
echo "Test 1: Encrypt with ./ft_ssl, decrypt with openssl (legacy provider)"
test_roundtrip "Test 1" "foo bar" \
  "./ft_ssl des-ecb -k \$DES_KEY | openssl enc -des-ecb -d -K \$DES_KEY -provider legacy"
echo "-------------------------------"

# Test 2
echo "Test 2: Encrypt with openssl (legacy provider), decrypt with ./ft_ssl"
test_roundtrip "Test 2" "foo bar" \
  "openssl enc -des-ecb -K \$DES_KEY -provider legacy | ./ft_ssl des-ecb -d -k \$DES_KEY"
echo "-------------------------------"

# Test 3
echo "Test 3: Encrypt with ./ft_ssl (smol), decrypt with openssl (legacy provider)"
test_roundtrip "Test 3" "smol" \
  "./ft_ssl des-ecb -k \$DES_KEY | openssl enc -des-ecb -d -K \$DES_KEY -provider legacy"
echo "-------------------------------"

# Test 4
echo "Test 4: Encrypt with openssl (legacy provider) (smol), decrypt with ./ft_ssl"
test_roundtrip "Test 4" "smol" \
  "openssl enc -des-ecb -K \$DES_KEY -provider legacy | ./ft_ssl des-ecb -d -k \$DES_KEY"
echo "-------------------------------"

# Test 5
echo "Test 5: Test with key 'ABCD' (should pad with zeros)"
test_roundtrip "Test 5" "testing the key now" \
  "openssl enc -des-ecb -K ABCD -provider legacy | ./ft_ssl des-ecb -d -k ABCD"
echo "-------------------------------"

# Test 6
echo "Test 6: Test with key 'FFFFFFFFFFFFFFFF'"
test_roundtrip "Test 6" "More key tests" \
  "openssl enc -des-ecb -K FFFFFFFFFFFFFFFF -provider legacy | ./ft_ssl des-ecb -d -k FFFFFFFFFFFFFFFF"
echo "-------------------------------"

# Test 7
echo "Test 7: Test with key '00000000' (should pad with zeros)"
test_roundtrip "Test 7" "what kind of lock takes no key?" \
  "openssl enc -des-ecb -K 00000000 -provider legacy | ./ft_ssl des-ecb -d -k 00000000"
echo "-------------------------------"

echo "All tests completed."
