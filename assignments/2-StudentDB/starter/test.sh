#!/usr/bin/env bats

# The setup function runs before every test
setup_file() {
    # Delete the student.db file if it exists
    if [ -f "student.db" ]; then
        rm "student.db"
    fi
}

@test "Check if database is empty to start" {
    run ./sdbsc -p
    [ "$status" -eq 0 ]
    [ "$output" = "Database contains no student records." ]
}

@test "Add a student 1 to db" {
    run ./sdbsc -a 1      john doe 3.45
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "Student 1 added to database." ]
}

@test "Add more students to db" {
    run ./sdbsc -a 3      jane  doe  3.90
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "Student 3 added to database." ] || {
        echo "Failed Output:  $output"
        return 1
    }

    run ./sdbsc -a 63     jim   doe  2.85
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "Student 63 added to database." ] || {
        echo "Failed Output:  $output"
        return 1
    }

    run ./sdbsc -a 64     janet doe  3.10
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "Student 64 added to database." ] || {
        echo "Failed Output:  $output"
        return 1
    }

    run ./sdbsc -a 99999  big   dude 2.05
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "Student 99999 added to database." ] || {
        echo "Failed Output:  $output"
        return 1
    }
}

@test "Check student count" {
    run ./sdbsc -c
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "Database contains 5 student record(s)." ] || {
        echo "Failed Output:  $output"
        return 1
    }
}

@test "Make sure adding duplicate student fails" {
    run ./sdbsc -a 63 dup student 300
    [ "$status" -eq 1 ]  || {
        echo "Expecting status of 1, got:  $status"
        return 1
    }
    [ "${lines[0]}" = "Cant add student with ID=63, already exists in db." ] || {
        echo "Failed Output:  $output"
        return 1
    }
}

@test "Make sure the file size is correct at this time" {
    run stat --format="%s" ./student.db
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "6400000" ] || {
        echo "Failed Output:  $output"
        echo "Expected: 64000000"
        return 1
    }
}

#@test "Make sure the file storage is correct at this time" {
#    run du -h ./student.db
#   [ "$status" -eq 0 ]
#    #note du -h puts a tab between the 2 fields need to match on that
#    [ "$output" = "12K$(echo -e '\t')./student.db" ] || {
#        echo "Failed Output:  $output"
#        echo "12K     ./student.db"
#        return 1
#    }
#}

@test "Find student 3 in db" {
    run ./sdbsc -f 3
    
    # Ensure the command ran successfully
    [ "$status" -eq 0 ]
    
    # Use echo with -n to avoid adding extra newline and normalize spaces
    normalized_output=$(echo -n "${lines[1]}" | tr -s '[:space:]' ' ')

    # Define the expected output
    expected_output="3 jane doe 0.03"

    # Compare the normalized output with the expected output
    [ "$normalized_output" = "$expected_output" ] || {
        echo "Failed Output:  $normalized_output"
        echo "Expected: $expected_output"
        return 1
    }
}

@test "Try looking up non-existent student" {
    run ./sdbsc -f 4
    [ "$status" -eq 1 ]  || {
        echo "Expecting status of 1, got:  $status"
        return 1
    }
    [ "${lines[0]}" = "Student 4 was not found in database." ] || {
        echo "Failed Output:  $output"
        return 1
    }
}

@test "Delete student 64 in db" {
    run ./sdbsc -d 64
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "Student 64 was deleted from database." ] || {
        echo "Failed Output:  $output"
        return 1
    }
}

@test "Try deleting non-existent student" {
    run ./sdbsc -d 65
    [ "$status" -eq 1 ]  || {
        echo "Expecting status of 1, got:  $status"
        return 1
    }
    [ "${lines[0]}" = "Student 65 was not found in database." ] || {
        echo "Failed Output:  $output"
        return 1
    }
}

@test "Check student count again, should be 4 now" {
    run ./sdbsc -c
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "Database contains 4 student record(s)." ] || {
        echo "Failed Output:  $output"
        return 1
    }
}

@test "Print student records" {
    # Run the command
    run ./sdbsc -p

    # Ensure the command ran successfully
    [ "$status" -eq 0 ]

    # Normalize the output by replacing multiple spaces with a single space
    normalized_output=$(echo -n "$output" | tr -s '[:space:]' ' ')

    # Define the expected output (normalized)
    expected_output="ID FIRST NAME LAST_NAME GPA 1 john doe 0.03 3 jane doe 0.03 63 jim doe 0.02 99999 big dude 0.02"

    # Compare the normalized output
    [ "$normalized_output" = "$expected_output" ] || {
        echo "Failed Output: $normalized_output"
        echo "Expected Output: $expected_output"
        return 1
    }
}

#if you implemented the compress db function remove the 
#skip from the tests below

#@test "Double check storage at this point" {
#    run du -h ./student.db
#    [ "$status" -eq 0 ]
#    #note du -h puts a tab between the 2 fields need to match on that
#    [ "$output" = "12K$(echo -e '\t')./student.db" ] || {
#        echo "Failed Output:  $output"
#        echo "12K     ./student.db"
#        return 1
#    }
#}

@test "Compress db - try 1" {
    skip
    run ./sdbsc -x
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "Database successfully compressed!" ] || {
        echo "Failed Output:  $output"
        return 1
    }
}

#@test "One block should be gone" {
#    run du -h ./student.db
#    [ "$status" -eq 0 ]
#    #note du -h puts a tab between the 2 fields need to match on that
#    [ "$output" = "8.0K$(echo -e '\t')./student.db" ] || {
#        echo "Failed Output:  $output"
#        echo "8.0K     ./student.db"
#        return 1
#    }
#}

@test "Delete student 99999 in db" {
    skip
    run ./sdbsc -d 99999
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "Student 99999 was deleted from database." ] || {
        echo "Failed Output:  $output"
        return 1
    }
}

@test "Compress db again - try 2" {
    skip
    run ./sdbsc -x
    [ "$status" -eq 0 ]
    [ "${lines[0]}" = "Database successfully compressed!" ] || {
        echo "Failed Output:  $output"
        return 1
    }
}

#@test "Should be down to 1 block" {
#    run du -h ./student.db
#    [ "$status" -eq 0 ]
#    #note du -h puts a tab between the 2 fields need to match on that
#    [ "$output" = "4.0K$(echo -e '\t')./student.db" ] || {
#        echo "Failed Output:  $output"
#        echo "4.0K     ./student.db"
#        return 1
#    }
#}






