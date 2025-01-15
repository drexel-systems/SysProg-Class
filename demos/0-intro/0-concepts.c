/*******************************
 * Concept: main() entrypoint
 *  0. main() is the entrypoint for the program
 *  1. it returns an int
 *  2. the int indicates the exit status of the program
 *******************************/
#include <stdio.h>

int add_numbers(int a, int b);

int main()
{
    int sum = add_numbers(1,2);
    printf("The sum of 1 and 2 is %d.\n", sum);
    return 0;
}


/*******************************
 * Concept: main() with args
 *   0. use a different function signature to access args
 *   1. argc is the number of arguments
 *   2. argv is an array of strings
 *   3. argv[0] is the name of the program
 *   4. argv[1] is the first argument
 *******************************/

// int main(int argc, char *argv[]) {
//     if (argc >= 2) {
//         // contrived example; 
//         // return the number of args as the exit code
//         return argc;
//     }
//     return 0;
// }


/****************************************************************
 * Concept: func declarations (prototypes) vs definitions
 *   0. it fails due to missing func definition (comment out to see)
 *   1. combined declaration and definition (have to move this above main())
 *   2. declaration only moved above main - leave definition below
 ****************************************************************/

int add_numbers(int a, int b)
{
    return a + b;
}
