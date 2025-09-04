#!/usr/bin/env bash
set -euo pipefail

# ─────────────────────  prettifiers  ──────────────────────
RED=$'\e[31m'; GREEN=$'\e[32m'; YELLOW=$'\e[33m'; DIM=$'\e[2m'; RESET=$'\e[0m'
say()   { printf "\n%b▶ %s%b\n"   "$YELLOW" "$*" "$RESET"; }
exp()   { printf "    expected : %s\n"   "$*"; }
cmd()   { printf "%b$ %s%b\n"     "$DIM" "$*" "$RESET"; "$@"; }
pass()  { printf "  %bPASS%b\n"   "$GREEN" "$RESET"; }
fail()  { printf "  %bFAIL%b\n"   "$RED"   "$RESET"; exit 1; }

# ───────────────────  shared settings  ────────────────────
SALT="1122334455667788"
PASS="424242"
ITER=10000
IV_ZEROS="0000000000000000"
SALT_ECB="0000000000004242"    # 8‑byte salt represented as 16 hex chars

MY_BIN=./ft_ssl                # rename if needed
OSSL="openssl"                 # or "openssl3" etc.
OSSL_LEG="-provider default -provider legacy"

rm -f *.cipher *.txt *.html recovered* my_* ossl_* decrypted*.html

############################################################################
say "Test 1 · DES-CBC  (OpenSSL encrypt → my_ssl decrypt)"
############################################################################
echo "Hello DES world" > plain.txt

exp "OpenSSL creates openssl.cipher"
cmd $OSSL enc -des-cbc -pbkdf2 -iter $ITER -S $SALT -md sha256 \
              -pass pass:$PASS -in plain.txt -out openssl.cipher $OSSL_LEG

exp "my_ssl reproduces the original plaintext"
cmd $MY_BIN des-cbc -d -p $PASS -s $SALT \
              -i openssl.cipher -o recovered.txt
diff -q plain.txt recovered.txt && pass || fail

############################################################################
say "Test 2 · DES-CBC  (explicit IV CAFEBABE…)  round-trip"
############################################################################
IV="CAFEBABEDEADF00D"
echo "Hello DES world with explicit IV" > plain_iv.txt

exp "OpenSSL encrypts with IV $IV → ossl_iv.cipher"
cmd $OSSL enc -des-cbc -pbkdf2 -iter $ITER -S $SALT -md sha256 \
              -pass pass:$PASS -iv $IV \
              -in plain_iv.txt -out ossl_iv.cipher $OSSL_LEG

exp "my_ssl decrypts with -v $IV"
cmd $MY_BIN des-cbc -d -p $PASS -s $SALT -v $IV \
              -i ossl_iv.cipher -o my_dec_iv.txt
diff -q plain_iv.txt my_dec_iv.txt && pass || fail

exp "my_ssl encrypts with the same IV → my_iv.cipher"
cmd $MY_BIN des-cbc -p $PASS -s $SALT -v $IV \
              -i plain_iv.txt -o my_iv.cipher

exp "OpenSSL decrypts with -iv $IV"
cmd $OSSL enc -d -des-cbc -pbkdf2 -iter $ITER -S $SALT -md sha256 \
              -pass pass:$PASS -iv $IV \
              -in my_iv.cipher -out ossl_dec_iv.txt $OSSL_LEG
diff -q plain_iv.txt ossl_dec_iv.txt && pass || fail

############################################################################
say "Test 3 · DES-ECB  (password file, salt 4242)"
############################################################################
# HTML source (use curl if network is available; fall back otherwise)
if command -v curl >/dev/null && curl -sSfL https://www.google.com -o original.html; then
    :
else
    echo '<html><body>Dummy page</body></html>' > original.html
fi
echo -n "password" > password_file

exp "my_ssl encrypts original.html with salt 4242 → ciphertext.html"
cmd $MY_BIN des-ecb -s $SALT_ECB -i original.html -o ciphertext.html \
              -p "$(cat password_file)"

exp "OpenSSL decrypts and must match the original"
cmd $OSSL enc -d -des-ecb -pbkdf2 -iter $ITER -S $SALT_ECB \
              -pass pass:$(cat password_file) \
              -in ciphertext.html -out decrypted.html $OSSL_LEG
diff -q original.html decrypted.html && pass || fail

############################################################################
say "Test 4 · CBC via pipe  (stdin → stdout)"
############################################################################
echo "Pipe test sample data" > pipe.txt

exp "Encrypt to stdout with IV zeros, decrypt with OpenSSL on the fly"
cmd bash -c "$MY_BIN des-cbc -s $SALT -v $IV_ZEROS -p des -i pipe.txt \
          | $OSSL enc -d -des-cbc -pbkdf2 -iter $ITER -S $SALT \
               -iv $IV_ZEROS -pass pass:des $OSSL_LEG \
          > pipe_recovered.txt"
diff -q pipe.txt pipe_recovered.txt && pass || fail

say "🎉  ALL TESTS PASSED"
