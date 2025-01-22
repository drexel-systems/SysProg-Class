#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFF_SIZE 50

void loop_print_ints(int *input_array, int array_size);
void loop_print_string(char *str_buff, int buff_size);

int main()
{
    // let's start with a simple literal int array - the compiler allocates exactly the right amount of memory
    // because this array was defined as a literal, it is stored in the read-only section of the memory
    int digits[] = {100, 101, 102, 103};

    // CONCEPT: show "decay" of digits var
    printf("%x\n", digits);

    loop_print_ints(digits, 4);

    // CONCEPT: char arrays (there are no strings!)
    char *mystr = "hello, world"; // has the null term '\0'

    char *buff = malloc(BUFF_SIZE * sizeof(char));
    if (buff == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        return -1;
    }

    strcpy(buff, mystr);

    loop_print_string(buff, BUFF_SIZE);

    return 0;
}

void loop_print_string(char *str_buff, int buff_size)
{
    for (int i = 0; i < buff_size; i++)
    {
        if (*str_buff == '\0')
        {
            return;
        }
        printf("%c\n", *str_buff);
        str_buff++;

        //? what if this loop goes beyond the buff size?
        //! that would be undefined
    }
}

void loop_print_ints(int *input_array, int array_size)
{
    for (int i = 0; i < array_size; i++)
    {
        // CONCEPT: offset addressing
        printf("%d %d\n", *(input_array + i), *input_array);

        // CONCEPT: array index addressing
        printf("%d\n", input_array[i]);

        // CONCEPT: pointer ARGUMENT increment addressing
        // TODO
        //? why is this ok? does it have any side effects outside the func? (hint: in C, args are passed by _____?)
        // printf("%d\n", *input_array);
        // input_array++;
    }

    // CONCEPT: create local pointer addressing
    // CONCEPT: 1-line pointer increment and use (only because it's common in the wild)
    // TODO
    //? when is this useful? what does it provide?

    int *p_myptr = input_array;

    for (int i = 0; i < array_size; i++)
    {
        printf("%d %d\n", *p_myptr, *input_array);
        p_myptr++;
    }
}