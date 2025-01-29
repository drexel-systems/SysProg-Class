## HW1a:  C Programming Refresher - Part 1

#### Description
The purpose of this assignment is a warm up to refresh your memory about how to program in C. 

You will be writing a simple **Text Line Processor** utility in C.  The program will allow users to provide a string and a command line operation indicating the operation that you will perform on the provided string. The command line for this program is as follows:

```bash
$ stringfun -[h|c|r|w] "sample string" 

where:
  -h    prints help about the program
  -c    counts of the number of words in the "sample string"
  -r    reverses the characters (in place) in "sample string" 
  -w    prints the individual words and their length in the "sample string"
```

#### Assignment Primer

The purpose of this assignment is to get a really simple C program working with a lot of scaffolding and directions. If C is new to you this assignment should get you ready for Part 2 where we start working with pointers and treating strings as memory buffers. 

The main concept you will be exploring in this assignment is that strings really do not exist in the C programming lanaguage.  A string in C is a sequence of characters, and **by convention** the last character in the string is a NULL or `\0` byte.

So in C, the string `char *example = "Hello class!";` would be represented in memory as:

```
example[i] is a character, see the various values based on i below:

                    1
0 1 2 3 4 5 6 7 8 9 0 1 2
|-------------------+----|
H e l l o   c l a s s ! \0
```

One of the main functions you will be using in this assignment is [strlen()](https://www.man7.org/linux/man-pages/man3/strlen.3.html). The `strlen()` function as you would expect returns the length of the C string provided as a parameter.  **NOTE: the parameter must be encoded as a C string meaning it ends with a `\0` byte.

Example:

```c
char *str = "Hello Class!";
int  slen = strlen(str);

//Given the above:
//    slen   = 12  (length of the Hello Class string)
//    str[0] = 'H' (first character)
//    str[slen-1] = '!" (last character)
//    str[slen] = str[12] = '\0' (where the null byte is located)
//
//    The total storage needed to store str is 13 bytes.  12 for
//    the string itself and one byte for the ending null character
```
For this assignment you should only really require the `strlen()` function and use array notation to access individual string characters like shown below.  You are free however to use any of the functions provided in the C runtime library like `strcpy()`, etc. 


#### What you need to do

Take a look at the starter code that was provided, note that it should compile and run correctly with the provided `makefile`.  Just run `make` and the starter code should build and be runnable. You should expect some `unused-variable` warnings from the compiler because the starter code defines a few variables that you will likely need in your implementation but I did not do anything with them. You can ignore these warnings, the starter code should run after compiling even with the warnings. 

1.  There are 7 `TODO:` activities defined in constants in the starter code.  Many of the `TODO:` activities are instructions for code changes you need to make.  **Please read the `TODO:` carefully** as some of them ask you to answer some questions by adding inline comments. 

2. The starter code has empty implementations for the `count_words()`, `reverse_string()`, and `word_print()` functions.  This is where most of your work will be done.  Please implement these functions.  Write clear and easy to follow code.  If you need help with how to implement these functions see the algorithm comment blocks.  

#### Grading Rubric

Total assignment weight: 25 points.

Your grade will be assigned based on the following:

- 0-15 pts:  Your code compiles, implements the required functionality,  and produces no warnings. Make sure you follow the directions as you have a very restricted set of functions you can use from the standard library, plus you cannot use any array notation beyond what is used in the starter package to handle the command line processing. 

- 0-5 pts: Code quality.  This includes that your code is also appropriately documented. This means just enough documentation to describe what you are doing that is not obvious from following the code itself. 

- 0-5 pts: Answer quality for the non-coding questions asked in the `TODO:` blocks in the starter code. 



