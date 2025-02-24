#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}
@test "Test background process with sleep" {
    run ./dsh <<EOF
sleep 2 &
EOF

    [ "$status" -eq 0 ]
}

@test "Test multiple commands with semicolon" {
    run ./dsh <<EOF
echo Hello; echo World
EOF

    [ "$status" -eq 0 ]
    
    [[ "$output" == *"Hello"* ]] && [[ "$output" == *"World"* ]]
}

@test "Test command with an invalid argument" {
    run ./dsh <<EOF
ls -nonexistentoption
EOF

    [ "$status" -eq 0 ]
    
    [[ "$output" == *"invalid option"* ]] || [[ "$output" == *"not found"* ]]
}

@test "Test handling of invalid command with multiple spaces" {
    run ./dsh <<EOF
    ls     -l     /nonexistentdirectory
EOF

    [ "$status" -eq 0 ]
    
    [[ "$output" == *"No such file or directory"* ]] || [[ "$output" == *"cannot"* ]] || [[ "$output" == *"error"* ]]
}

@test "Test command with special characters" {
    run ./dsh <<EOF
echo "Hello! How are you?"
EOF

    [ "$status" -eq 0 ]
    
    [[ "$output" == *"Hello!"* ]] && [[ "$output" == *"How are you?"* ]]
}

@test "Test command with multiple pipes" {
    run ./dsh <<EOF
echo "apple orange banana" | tr ' ' '\n' | sort
EOF

    [ "$status" -eq 0 ]
    
    [[ "$output" == *"apple"* ]] && [[ "$output" == *"orange"* ]] && [[ "$output" == *"banana"* ]]
}


@test "Test nonexistent command handling" {
    run ./dsh <<EOF
nonexistentcommand
EOF

    # The command should fail but the shell should continue
    [ "$status" -eq 0 ]
    
    # Check for common error patterns without relying on specific CMD_ERR_EXECUTE variable
    [[ "$output" == *"failed to execute"* ]] || [[ "$output" == *"not found"* ]] || [[ "$output" == *"No such file"* ]] || [[ "$output" == *"cannot"* ]] || [[ "$output" == *"failed"* ]]
}