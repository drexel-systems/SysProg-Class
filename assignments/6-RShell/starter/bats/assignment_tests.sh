#!/usr/bin/env bats

############################ DO NOT EDIT THIS FILE #####################################
# File: assignement_tests.sh
# 
# DO NOT EDIT THIS FILE
#
# Add/Edit Student tests in student_tests.sh
# 
# All tests in this file must pass - it is used as part of grading!
########################################################################################

# Helper function to check if output contains expected text
# Usage: assert_output_contains "expected text"
assert_output_contains() {
    local expected="$1"
    if [[ ! "$output" =~ "$expected" ]]; then
        echo "Expected output to contain: '$expected'"
        echo "Got output:"
        echo "$output"
        return 1
    fi
}

@test "Pipes" {
    run "./dsh" <<EOF                
ls | grep dshlib.c
EOF

    # Verify key elements in the output
    assert_output_contains "dshlib.c"
    assert_output_contains "cmd loop returned 0"
    
    # Verify the command executed successfully
    [ "$status" -eq 0 ]
}


