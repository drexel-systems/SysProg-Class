// see makefile

#include <stdio.h>

int main() {
    printf("Hello, World!\n");
    return 0;
}

/*******************************
 * Concept: compiling intermediate files
 *  0. makefile will generate all intermediate files
 *  1. .i is preprocessed source code
 *  2. .s is human-readable assembly
 *  3. .o is a binary ELF file, but not executable (not linked)
 *  4. 1-hello is the final executable ELF file
 *******************************/

/*******************************
 * Concept: 1-hello.i preprocessed source
 *  0. #include <stdio.h> is replaced with 
 *     many absolute paths from my system - ymmv
 *  1. lots of boilerplate expanded code and metadata
 *  2. preprocessed file is contextual based on the local environment!!!
 *******************************/

/*******************************
 * Concept: 1-hello.s assembly
 *  0. contains cpu instructions specific to x86_64
 *  1. assembler provides deeper insight into the code
 *  2. assembler allows direct manipulation of the CPU
 *******************************/

/*******************************
 * Concept: readelf -a 1-hello.o | more
 *  0. binary file with machine code, NOT EXECUTABLE
 *  1. lots of metadata about the binary file
 *  2. and the binary contents!
 *  3. but it's missing stuff ...
 *  4. lets refresh on binary, 2's, endianness, etc ...
 *     <SLIDES>
 *     ......
 *  5. what are sections
 *******************************/

/*******************************
 * Concept: readelf -a 1-hello | more
 *  0. this is the linked executable
 *  1. it has a lot more stuff due to linking    
 *******************************/