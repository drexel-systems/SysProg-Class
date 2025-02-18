#!/usr/bin/env bats

@test "Simple Command" {
    run ./dsh <<EOF                
test_command
exit
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh>PARSEDCOMMANDLINE-TOTALCOMMANDS1<1>test_commanddsh>"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]

}

@test "Simple Command with Args" {
    run ./dsh <<EOF                
cmd -a1 -a2
exit
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output 
    expected_output="dsh>PARSEDCOMMANDLINE-TOTALCOMMANDS1<1>cmd[-a1-a2]dsh>"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]

}


@test "No command provided" {
    run ./dsh <<EOF                

exit
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh>warning:nocommandsprovideddsh>"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]

}

@test "Two commands" {
    run ./dsh <<EOF                
command_one | command_two
exit
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh>PARSEDCOMMANDLINE-TOTALCOMMANDS2<1>command_one<2>command_twodsh>"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]

}

@test "three commands with args" {
    run ./dsh <<EOF                
cmd1 a1 a2 a3 | cmd2 a4 a5 a6 | cmd3 a7 a8 a9
exit
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh>PARSEDCOMMANDLINE-TOTALCOMMANDS3<1>cmd1[a1a2a3]<2>cmd2[a4a5a6]<3>cmd3[a7a8a9]dsh>"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]

}

@test "try max (8) commands" {
    run ./dsh <<EOF                
cmd1 | cmd2 | cmd3 | cmd4 | cmd5 | cmd6 | cmd7 | cmd8
exit
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh>PARSEDCOMMANDLINE-TOTALCOMMANDS8<1>cmd1<2>cmd2<3>cmd3<4>cmd4<5>cmd5<6>cmd6<7>cmd7<8>cmd8dsh>"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]

}

@test "try too many commands" {
    run ./dsh <<EOF                
cmd1 | cmd2 | cmd3 | cmd4 | cmd5 | cmd6 | cmd7 | cmd8 | cmd9
exit
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh>error:pipinglimitedto8commandsdsh>"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]

}

@test "kitchen sink - multiple commands" {
    run ./dsh <<EOF                
cmd1
cmd2 arg arg2
p1 | p2
p3 p3a1 p3a2 | p4 p4a1 p4a2
EOF

    # Strip all whitespace (spaces, tabs, newlines) from the output
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    # Expected output with all whitespace removed for easier matching
    expected_output="dsh>PARSEDCOMMANDLINE-TOTALCOMMANDS1<1>cmd1dsh>PARSEDCOMMANDLINE-TOTALCOMMANDS1<1>cmd2[argarg2]dsh>PARSEDCOMMANDLINE-TOTALCOMMANDS2<1>p1<2>p2dsh>PARSEDCOMMANDLINE-TOTALCOMMANDS2<1>p3[p3a1p3a2]<2>p4[p4a1p4a2]dsh>"

    # These echo commands will help with debugging and will only print
    #if the test fails
    echo "Captured stdout:" 
    echo "Output: $output"
    echo "Exit Status: $status"

    # Check exact match
    [ "$stripped_output" = "$expected_output" ]

    # Assertions
    [ "$status" -eq 0 ]

}