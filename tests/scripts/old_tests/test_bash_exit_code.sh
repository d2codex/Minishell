#!/bin/bash

echo "=== exit behavior tests ==="
echo

echo "test 1: exit (no arguments)"
bash -c "exit" 2>&1
echo "exit code: $?"
echo

echo "test 2: exit 42"
bash -c "exit 42" 2>&1
echo "exit code: $?"
echo

echo "test 3: exit -1"
bash -c "exit -1" 2>&1
echo "exit code: $?"
echo

echo "test 4: exit 999999"
bash -c "exit 999999" 2>&1
echo "exit code: $?"
echo

echo "test 5: exit hello"
bash -c "exit hello" 2>&1
echo "exit code: $?"
echo

echo "test 6: exit 1 2 3"
bash -c "exit 1 2 3" 2>&1
echo "exit code: $?"
echo

echo "test 7: exit \"\""
bash -c 'exit ""' 2>&1
echo "exit code: $?"
echo

echo "=== end tests ==="
