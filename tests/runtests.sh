#!/bin/bash

# If "VALGRIND" env variable is not unset, run tests with valgrind
if [ -n "$VALGRIND" ]; then
    RUNNER="valgrind --quiet --leak-check=full --show-leak-kinds=all --error-exitcode=1"
else
    RUNNER=""
fi

function say_green {
	echo -e "\033[0;32m$1\033[0m"
}

function say_red {
	echo -e "\033[0;31m$1\033[0m"
}

function say_yellow {
    	echo -e "\033[0;33m$1\033[0m"
}
say_yellow "Running unit tests:"

ALL_PASSED=1
FAILURES=""
for i in tests/*tests; do
    say_yellow "Running $i"
    $RUNNER $i 
    if [ $? -ne 0 ]; then
        say_red "ERROR in test $i"
	ALL_PASSED=0
	FAILURES="$FAILURES $i"
    else
	say_green "PASS"
    fi
done

if [ $ALL_PASSED -eq 1 ]; then
	say_green "ALL TESTS PASSED"
else
	say_red "SOME TESTS FAILED"
	say_red "Failed tests: $FAILURES"
fi
