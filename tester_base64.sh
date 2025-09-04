#!/bin/bash
# test_ft_ssl.sh
# Script to test ft_ssl's base64 command implementation against expected outputs.
# Test cases:
#   1. echo toto | ./ft_ssl base64 -e         -> dG90bwo=
#   2. echo dG90bwo= | ./ft_ssl base64 -d        -> toto
#   3. echo "d G9 0bwo =" | ./ft_ssl base64 -d    -> toto
#   4. echo foobar | ./ft_ssl base64 -e           -> Zm9vYmFyCg==
#   5. echo Zm9vYmFyCg== | base64 -D              -> foobar

fail=0

# Function to run a test case.
# $1: Description
# $2: Expected output
# $3: Command to run
test_case() {
    description="$1"
    expected="$2"
    command="$3"
    output=$(eval "$command")
    
    if [ "$output" = "$expected" ]; then
        echo "PASS: $description"
    else
        echo "FAIL: $description"
        echo "   Expected: '$expected'"
        echo "   Got:      '$output'"
        fail=1
    fi
}

test_case "Encode 'toto'" "dG90bwo=" "echo toto | ./ft_ssl base64 -e"
test_case "Decode 'dG90bwo='" "toto" "echo dG90bwo= | ./ft_ssl base64 -d"
test_case "Decode 'd G9 0bwo ='" "toto" "echo 'd G9 0bwo =' | ./ft_ssl base64 -d"
test_case "Encode 'foobar'" "Zm9vYmFyCg==" "echo foobar | ./ft_ssl base64 -e"
test_case "System base64 decode 'Zm9vYmFyCg=='" "foobar" "echo Zm9vYmFyCg== | ./ft_ssl base64 -D"

if [ $fail -eq 1 ]; then
    echo "Some tests failed."
    exit 1
else
    echo "All tests passed."
    exit 0
fi
