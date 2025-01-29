#!/usr/bin/env bats

@test "no args shows usage" {
    run ./stringfun
    [ "$status" -eq 1 ]
    [ "${lines[0]}" = "usage: ./stringfun [-h|c|r|w|x] \"string\" [other args]" ]
}

@test "bad args shows usage" {
    run ./stringfun -z "Bad arg usage"  
    [ "$status" -eq 1 ]
    [ "${lines[0]}" = "usage: ./stringfun [-h|c|r|w|x] \"string\" [other args]" ]
}

@test "check -h" {
    run ./stringfun -h
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "usage: ./stringfun [-h|c|r|w|x] \"string\" [other args]" ]
}

@test "wordcount" {
    run ./stringfun -c "There should be eight words in this sentence"
    [ "$status" -eq 0 ]
    [ "$output" = "Word Count: 8
Buffer:  [There should be eight words in this sentence......]" ]
}

@test "remove extra spaces" {
    run ./stringfun -c "   The   strange    spaces    should   be     removed   from this    "
    [ "$status" -eq 0 ]
    [ "$output" = "Word Count: 8
Buffer:  [The strange spaces should be removed from this....]" ]
}

@test "reverse" {
    run ./stringfun -r "Reversed sentences look very weird"
    [ "$status" -eq 0 ]
    [ "$output" = "Buffer:  [driew yrev kool secnetnes desreveR................]" ]
}

@test "print words" {
    run ./stringfun -w "Lets get a lot of words to test"
    [ "$status" -eq 0 ]
    [ "$output" = "Word Print
----------
1. Lets(4)
2. get(3)
3. a(1)
4. lot(3)
5. of(2)
6. words(5)
7. to(2)
8. test(4)

Number of words returned: 8
Buffer:  [Lets get a lot of words to test...................]" ]
}

@test "check max length" {
    run ./stringfun -r "This is the maximum length string that should work"
    [ "$status" -eq 0 ]
    [ "$output" = "Buffer:  [krow dluohs taht gnirts htgnel mumixam eht si sihT]" ]
}

@test "check over max length" {
    run ./stringfun -w "This is a string that does not work as it is too long"
    [ "$status" -ne 0 ]
}



@test "basic string search replace" {
    run ./stringfun -x "This is a bad test" bad  great
    [ "$output" = "Buffer:  [This is a great test..............................]" ] ||
    [ "$output" = "Not Implemented!" ]
}

@test "search replace not found" {
    run ./stringfun -x "This is a a long string for testing" bad  great
    [ "$status" -ne 0 ] || 
    [ "$output" = "Not Implemented!" ]
}

@test "basic overflow search replace" {
    run ./stringfun -x "This is a super long string for testing my program" testing  validating
    [ "$output" = "Buffer:  [This is a super long string for validating my prog]" ] ||
    [ "$output" = "Not Implemented!" ]
}

@test "test overflow string replace" {
    run ./stringfun -x "This is a super long string for testing my program" testing  validating
    [ "$output" = "Buffer:  [This is a super long string for validating my prog]" ] ||
    [ "$output" = "Not Implemented!" ]
}

@test "test shorter string replace" {
    run ./stringfun -x "This is a super long string for testing my program" program  app
    [ "$output" = "Buffer:  [This is a super long string for testing my app....]" ] || 
    [ "$output" = "Not Implemented!" ]
}
