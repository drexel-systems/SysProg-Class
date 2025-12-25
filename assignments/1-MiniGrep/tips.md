# Implementation Tips and Hints

## General Approach

Start by implementing the functions in this order:
1. `str_len()` - simplest function, you'll need it for testing
2. `str_match()` - core matching logic
3. File reading and basic output (no flags)
4. Add support for each flag one at a time

## String Length Function

The `str_len()` function is straightforward. Start with a counter at 0 and a pointer to the string. Walk through the string character by character (using pointer arithmetic) until you hit the null terminator `'\0'`. Return the counter.

```c
int str_len(char *str) {
    int len = 0;
    // While we haven't hit the null terminator
    while (*str != '\0') {
        len++;
        str++;  // move pointer to next character
    }
    return len;
}
```

## Pattern Matching Function

This is the trickiest function. You need to check if the pattern appears ANYWHERE in the line, not just at the start.

**Approach:**
1. Try matching the pattern starting at each position in the line
2. For each starting position, compare pattern characters one by one with line characters
3. If all pattern characters match, return 1
4. If you reach the end of the line without finding the pattern, return 0

**Pseudocode:**
```
for each position in line:
    assume we have a match
    for each character in pattern:
        compare pattern[i] with line[position + i]
        if they don't match:
            break out and try next position
    if all characters matched:
        return 1 (found)
return 0 (not found)
```

**Case-Insensitive Matching:**
When comparing characters, convert both to lowercase (or both to uppercase):
```c
char line_char = *line_ptr;
char pattern_char = *pattern_ptr;

if (case_insensitive) {
    line_char = tolower(line_char);
    pattern_char = tolower(pattern_char);
}

if (line_char != pattern_char) {
    // no match
}
```

## File Reading Loop

Use `fgets()` to read the file line by line. It reads until it encounters a newline or reaches the buffer size limit.

```c
line_number = 0;
while (fgets(line_buffer, LINE_BUFFER_SZ, fp) != NULL) {
    line_number++;
    
    // Check if pattern matches this line
    found_match = str_match(line_buffer, pattern, case_insensitive);
    
    // Handle output based on flags
    if (found_match) {
        if (count_only) {
            match_count++;
        } else if (show_line_nums) {
            printf("%d: %s", line_number, line_buffer);
        } else {
            printf("%s", line_buffer);
        }
    }
}
```

**Note:** `fgets()` includes the newline character `\n` in the buffer (if there's room), so you don't need to add it when printing.

## Handling the Newline Character

`fgets()` includes the `\n` at the end of each line (unless the line is too long for the buffer). This is actually helpful because when you print the line with `printf("%s", line_buffer)`, it will include the newline.

However, if you need to remove it (for example, for debugging), you can do:
```c
char *ptr = line_buffer;
while (*ptr != '\0') {
    if (*ptr == '\n') {
        *ptr = '\0';  // replace newline with null terminator
        break;
    }
    ptr++;
}
```

## Combining Flags

When you have multiple flags like `-in` or `-ic`, your code should handle them together:
- `-in` means case-insensitive AND show line numbers
- `-ic` means case-insensitive AND count only

Make sure your if-statements check the flag variables, not the command line arguments directly.

## Debugging Tips

1. **Test your helper functions first:** Write a simple test in `main()` before implementing the full program:
   ```c
   printf("Length of 'hello': %d\n", str_len("hello"));  // should be 5
   printf("Match 'err' in 'error': %d\n", str_match("error", "err", 0));  // should be 1
   ```

2. **Print intermediate values:** When debugging pattern matching:
   ```c
   printf("Checking line: %s", line_buffer);
   printf("Pattern: %s\n", pattern);
   printf("Match result: %d\n", found_match);
   ```

3. **Test with simple files first:** Create a test file with just a few lines:
   ```
   echo "hello world" > test.txt
   echo "this is a test" >> test.txt
   echo "hello again" >> test.txt
   ./minigrep "hello" test.txt
   ```

4. **Use the provided test script:** Run `make test` to see which test cases pass/fail.

## Common Pitfalls

1. **Forgetting to check if malloc succeeds:**
   ```c
   line_buffer = malloc(LINE_BUFFER_SZ);
   if (line_buffer == NULL) {
       printf("Error: Memory allocation failed\n");
       exit(4);
   }
   ```

2. **Forgetting to check if fopen succeeds:**
   ```c
   fp = fopen(filename, "r");
   if (fp == NULL) {
       printf("Error: Cannot open file %s\n", filename);
       exit(3);
   }
   ```

3. **Off-by-one errors in pattern matching:** Make sure you don't read past the end of either the line or the pattern.

4. **Forgetting to free memory:** Always call `free(line_buffer)` before exiting.

## Extra Credit: Multiple Files

For the multiple file support extra credit, you'll need to:
1. Loop through `argv` starting from the filename position
2. For each file, open it, search it, and close it
3. Print the filename before each matching line: `printf("%s:%d: %s", filename, line_number, line_buffer);`

## Extra Credit: Invert Match (-v)

For invert match, you want to print lines that do NOT contain the pattern:
```c
found_match = str_match(line_buffer, pattern, case_insensitive);

// If invert_match is set, flip the logic
if (invert_match) {
    found_match = !found_match;  // invert the result
}

if (found_match) {
    // print the line
}
```
