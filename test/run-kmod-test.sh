#!/bin/bash
set -e
cd "$(dirname "$0")"

FAIL=true
for TEST_FILE in *.c; do
    TEST_NAME=${TEST_FILE:0:(-2)}
    if [[ $TEST_NAME == *.mod ]]; then
        continue
    fi
    echo "Running $TEST_NAME..."
    rmmod $TEST_NAME >/dev/null 2>&1 || true

    insmod out/$TEST_NAME/$TEST_NAME.ko

    RESULT=$(dmesg | grep "${TEST_NAME}: Test Result" | tail -1)
    echo $RESULT
    if [[ ! $RESULT == *"OK"* ]]; then
        FAIL=false
    fi
    rmmod $TEST_NAME

done
$FAIL
