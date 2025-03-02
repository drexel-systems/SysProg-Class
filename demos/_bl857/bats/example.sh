#!/usr/bin/env bats

# CONCEPT: test names should be descriptive about what they do and what you expect
@test "pwd should return 0" {
    # CONCEPT: most of the time you need to use the BATS "run" command to execute something you want to make assertions about
    run pwd

    # CONCEPT: after using `run`, the $status variable contains the exit code of the command
    # CONCEPT: asserts are performed using the bash test syntax (square braces with a boolean expression)
    [ "$status" -eq 0 ]
}

@test "ls /dev/noexists returns an error" {
    run ls /dev/noexists

    # CONCEPT: echo commands are only printed on failure, so these diagnostic messages will only show if something goes wrong
    # CONCEPT: experiement with this by changing the assertion below to be false, causing test failure
    echo "captured output: $output\n"
    echo "return status: $status\n"

    [ "$status" -eq 2 ]
}

@test "ls /dev/null exists" {
    run ls /dev/null

    echo "captured output: $output\n"
    echo "return status: $status\n"

    # CONCEPT: single-line output can be tested with the `=` operator; note that BATS removes the trailing \n
    [ "$output" = "/dev/null" ]
}

@test "multiline text example" {
    run printf "1\n2\n3"

    # CONCEPT: one way to test multi-line output is to transform it to a single string with no newlines
    stripped_output=$(echo "$output" | tr -d '[:space:]')

    echo "captured output: $output\n"
    echo "stripped output: $stripped_output\n"
    echo "return status: $status\n"
    

     # CONCEPT: run printed multiline output, but we were able to still test it by removing newlines and whitespace
    [ "$stripped_output" = "123" ]
}

@test "heredoc test example" {
    # CONCEPT: commands that read from stdin can be provided multi-line input using a "heredoc"
    # CONCEPT: this is the same way in which you can test a "run" of `dsh`
    run tee <<FOO
hello,
world
FOO

    stripped_output=$(echo "$output" | tr -d '[:space:]')

    echo "captured output: $output\n"
    echo "stripped output: $stripped_output\n"
    echo "return status: $status\n"

    [ "$stripped_output" = "hello,world" ]
}