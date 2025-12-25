#!/usr/bin/env bats

# Setup function runs before each test
setup() {
    # Create test files
    cat > test1.txt << 'EOF'
This is line one
This is line two with ERROR
Line three is here
Another ERROR on line four
Line five has a warning
EOF

    cat > test2.txt << 'EOF'
Hello World
hello world
HELLO WORLD
HeLLo WoRLd
goodbye world
EOF

    cat > test3.txt << 'EOF'
TODO: implement feature
FIXME: bug here
TODO: add tests
Nothing to see here
TODO: refactor code
EOF

    cat > empty.txt << 'EOF'
EOF
}

# Cleanup function runs after each test
teardown() {
    rm -f test1.txt test2.txt test3.txt empty.txt
}

@test "no arguments shows usage" {
    run ./minigrep
    [ "$status" -eq 2 ]
    [[ "${lines[0]}" =~ "usage:" ]]
}

@test "help flag shows usage" {
    run ./minigrep -h
    [ "$status" -eq 0 ]
    [[ "${lines[0]}" =~ "usage:" ]]
}

@test "missing filename shows error" {
    run ./minigrep "test"
    [ "$status" -eq 2 ]
}

@test "nonexistent file shows error" {
    run ./minigrep "test" nonexistent.txt
    [ "$status" -eq 3 ]
}

@test "basic search finds matches" {
    run ./minigrep "ERROR" test1.txt
    [ "$status" -eq 0 ]
    [ "${#lines[@]}" -eq 2 ]
    [[ "${lines[0]}" =~ "ERROR" ]]
    [[ "${lines[1]}" =~ "ERROR" ]]
}

@test "basic search with no matches" {
    run ./minigrep "NOTFOUND" test1.txt
    [ "$status" -eq 1 ]
}

@test "search with line numbers" {
    run ./minigrep -n "ERROR" test1.txt
    [ "$status" -eq 0 ]
    [[ "${lines[0]}" =~ "2:" ]]
    [[ "${lines[1]}" =~ "4:" ]]
}

@test "case insensitive search" {
    run ./minigrep -i "hello" test2.txt
    [ "$status" -eq 0 ]
    [ "${#lines[@]}" -eq 4 ]
    [[ "${lines[0]}" =~ "Hello" ]]
    [[ "${lines[1]}" =~ "hello" ]]
    [[ "${lines[2]}" =~ "HELLO" ]]
    [[ "${lines[3]}" =~ "HeLLo" ]]
}

@test "case sensitive search" {
    run ./minigrep "hello" test2.txt
    [ "$status" -eq 0 ]
    [ "${#lines[@]}" -eq 1 ]
    [[ "${lines[0]}" =~ "hello world" ]]
}

@test "count matches" {
    run ./minigrep -c "TODO" test3.txt
    [ "$status" -eq 0 ]
    [[ "$output" =~ "Matches found: 3" ]]
}

@test "count with no matches" {
    run ./minigrep -c "NOTFOUND" test1.txt
    [ "$status" -eq 1 ]
    [[ "$output" =~ "No matches found" ]]
}

@test "combined flags -in" {
    run ./minigrep -in "error" test1.txt
    [ "$status" -eq 0 ]
    [[ "${lines[0]}" =~ "2:" ]]
    [[ "${lines[1]}" =~ "4:" ]]
}

@test "combined flags -ic" {
    run ./minigrep -ic "hello" test2.txt
    [ "$status" -eq 0 ]
    [[ "$output" =~ "Matches found: 4" ]]
}

@test "pattern at start of line" {
    run ./minigrep "This" test1.txt
    [ "$status" -eq 0 ]
    [ "${#lines[@]}" -eq 2 ]
}

@test "pattern at end of line" {
    run ./minigrep "here" test1.txt
    [ "$status" -eq 0 ]
    [ "${#lines[@]}" -eq 1 ]
}

@test "pattern in middle of line" {
    run ./minigrep "line" test1.txt
    [ "$status" -eq 0 ]
    [ "${#lines[@]}" -eq 5 ]
}

@test "empty file returns not found" {
    run ./minigrep "test" empty.txt
    [ "$status" -eq 1 ]
}

@test "single character pattern" {
    run ./minigrep "e" test1.txt
    [ "$status" -eq 0 ]
    [ "${#lines[@]}" -ge 3 ]
}

@test "EXTRA CREDIT: invert match basic" {
    run ./minigrep -v "ERROR" test1.txt
    # If not implemented, should get usage or error
    if [ "$status" -eq 0 ]; then
        # If implemented, lines without ERROR should be shown
        [ "${#lines[@]}" -eq 3 ]
        [[ ! "${lines[0]}" =~ "ERROR" ]]
        [[ ! "${lines[1]}" =~ "ERROR" ]]
        [[ ! "${lines[2]}" =~ "ERROR" ]]
    fi
}

@test "EXTRA CREDIT: invert match with count" {
    run ./minigrep -vc "ERROR" test1.txt
    if [ "$status" -eq 0 ]; then
        [[ "$output" =~ "Matches found: 3" ]]
    fi
}

# Note: Multiple file support would require additional test files
# and checking output format includes filenames
