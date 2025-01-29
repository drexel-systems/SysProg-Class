## HW1:  C Programming Refresher

#### Description
The purpose of this assignment is a warm up to refresh your memory about how to program in C. 

You will be writing a simple **Text Line Processor** utility in C.  The program will allow users to provide a string and a command line operation indicating the operation that you will perform on the provided string. The command line for this program is as follows:

```bash
$ stringfun -[h|c|r|w|x] "sample string" [other_options]

where:
  -h    prints help about the program
  -c    counts of the number of words in the "sample string"
  -r    reverses the characters (in place) in "sample string" 
  -w    prints the individual words and their length in the "sample string"
  -x    takes sample string and 2 other strings, replaces the first with second
```

#### Assignment Requirements

The purpose of this assignment is to practice / refresh your memory working with memory, pointers, creating functions, etc.  So your implementation has to adhere to the following requirements:

1. You will not be able to use any string processing functions from the standard library (e.g., no `strcpy()`, no `strlen()`, etc).  You will be operating on a string buffer using pointers only.  This also means you cannot use array notation for processing any strings, **you must operate on individual characters using pointer notation only!**

2. For the purpose of this assignment, you may use the `memset()` and `memcpy()`, `printf()`, and `exit()` functions from the C standard library.  The provided template already includes appropriate `#include` statements for that will import the proper prototypes for these functions. 

3.  Since we will be composing utilities in the shell during this term, its common that most utilities return a value.  For this assignment you may use the `exit()` function to return a value to the shell.  The values that should be used are `0 = success`, `1 = command line problem`, `2 = memory allocation failure`, and `3 = error with one of your provided services`.  For example, if the provided input string is too long you would print an error message `error: Provided input string is too long` and then exit the program using code 3.


#### What you need to do

Take a look at the starter code that was provided, note that it should compile and run correctly with the provided `makefile`.  You should expect some warnings in the output of the provided code because the starter code defines a few variables like `char *buff;` that you will be using to manage an internal buffer (see below for the details), but the starter code provided does not do anything with these variables so the compiler will throw a few warnings prior to you implementing the following: 

1.  You will allocate an internal buffer using the `malloc()` function. Use the provided `buff` variable as a pointer to the storage that you will be allocating.

2. This buffer must be exactly 50 bytes.  Instead of hard coding 50, use the `#define BUFFER_SZ` provided in the starter code.


3. Implement and comment the `setup_buff()` function.  This function accepts your internal buffer `buff` as the first argument, the user supplied string from the command line as the second argument and the size of `buff` as the third argument.  This function **must** accomplish following behavior:

    * Copy over every non-white space character from the user supplied string to the internal buffer.  A white space character could be a space or a tab.

    * When **one or more** whitespace character(s) are encountered in the user supplied it is ignored.  For example, if the user string was `hello there` `buff` would contain `hello there`.  Note if multiple whitespace characters are included in a row they will be compressed.  For example "`why`&nbsp;&nbsp;&nbsp;`so`&nbsp;&nbsp;&nbsp;&nbsp;`many`&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`spaces`" would be processed as "`why so many spaces`" by this function.  Compressing spaces also includes leading and trailing spaces. 
  
    * Whitespace in the output may only be the space char `' '`; other whitespace type chars should be replaced with the space.
    
    * After the user supplied buffer is copied over, the remainder of the internal buffer should be filled with `'.'` characters.  For example if the user supplied string is `'hi class'`.  The `buff` would end up being populated with `'hi class..............'` where the dots extend to the end of the buffer defined by the `BUFFER_SZ`.  In this example since the user input `hi class` contains 8 characters, the resultant buffer would have this string followed by 42 dots since `BUFFER_SZ` is set to 50.
    
    * Note that the user supplied string variable `user_str` is a C string, and therefore it is null terminated `'\0'`.  You should use this ending marker when processing the `user_str`.  Remember you cannot use any string functions from the standard library, you will be processing these strings as characters using pointers.  Also the buffer you are building in the `buff` variable should not have any null termination bytes, it is just the user supplied string filled with dots to the end of the buffer. 
    
    * **IMPORTANT** What this function should return.  The `setup_buff()` function returns an integer.  The proper return values from this function are `-1 = The user supplied string is too large`.  In other words the user supplied string > `BUFFER_SZ`. `-2 = Any other error you want to report`.  For a `-2` error code, document in your code with a comment what the error condition is you are reporting.  If there are no errors, the `setup_buff()` function should return the length of the user supplied string.  For example if the user provided an input string of `hi there` the `setup_buff()` function should return 8.  You will likely find having this length helpful to implement the remaining functionality described next. 

5. Now you are ready to implement the remaining functionality.  The starter code stubs out the *count words* functionality.  Notice how there is a condition for it coded into the `switch` statement in `main()` and a handler function called `count_words()`.  This function accepts 3 arguments:  (a)a pointer to the buffer, (b) the length of the buffer, and (c)the length of the user supplied string. This function should return the number of words in the user supplied string and then print out `Word Count: ###`.  With this as your starter, implement all of the required code for the reverse string (option `-r`) and word print (option `-w`).  The output for these functions should be an error message similar to the one in the template for count words if an error occurs. If an error does not occur then the output should follow:

    *  **For reverse string [-r]:**  `Reversed String: xxxxx` where `xxxxx` is the user supplied string where all of the characters are reversed.

    * **For word print [-w]:**  Consider the user provided the input `Systems Programming is Great!` the output should be:
      ```
      Word Print
      ----------
      1. Systems (7)
      2. Programming (11)
      3. is (2)
      4. Great! (6)
      ```
      Note that each word should be printed on its own line followed by the number of characters in each word. 

    * **For word search and replace [-x]:**  Your basic implementation is to just ensure that all of the arguments are provide.  In this case argc should be 5.  A sample run of this command would be `./stringfun -x "Systems Programming is Great" Great Awesome`.  For the primary part of the assignment you would ensure that the user provided the 3 required arguments for the `-x` flag.  From there you would just print out "Not Implemented!" and return an error code.  Note that if you want to challenge yourself, there is some significant extra credit being offered for implementing this feature.  See below.

6. Finally see `TODO: #7` in the starter code.  If no errors occur, the final thing your program should do is print the entire `buff`.  This is handled by the `print_buff()` that is already implemented for you in your starter code.  You will likely find this helpful for debugging when you are implementing your solution.


#### Grading Rubric

Your grade will be assigned based on the following:

- 65%:  Your code compiles, implements the required functionality,  and produces no warnings. Make sure you follow the directions as you have a very restricted set of functions you can use from the standard library, plus you cannot use any array notation beyond what is used in the starter package to handle the command line processing. 

- 25%: Code quality.  This includes that your code is also appropriately documented. This means just enough documentation to describe what you are doing that is not obvious from following the code itself. 

- 10%: Answer quality for the non-coding questions asked in the `TODO:` blocks in the starter code. 

#### Extra Credit (+5)

Implement your own helper function to calculate string length; use it in the same way you would have used strlen() from stdlib.

#### Extra Credit (+20)

You may take on some extra credit to get more practice.  The extra credit involves adding a string replace function.  If you choose to do this assign it to the `-x` flag.  This enhancement should work as follows.  Consider the example:

```
stringfun -x "Replacing words in strings is not fun!" not super
```
The above would result in the program printing out:
```
Modified String: Replacing words in strings is super fun!
```
To keep things easier your replacement should be case sensitive, in other words `fun` will not match `Fun`. Also if there are multiple matches you can just replace the first occurrence and not deal with matching all occurrences. Of course if you replace them all this would not be considered incorrect.   

Also watch out for the situation where your replacement can go beyond the length of `buff`.  If this happens you can handle it by reporting some sort of buffer overflow error, or you can handle it by truncating the right side of the string.  In either event, you can not overrun your buffer.
