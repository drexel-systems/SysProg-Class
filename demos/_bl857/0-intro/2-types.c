#include <stdio.h>
#include <inttypes.h>

int main() {
    printf("The size of a char is: %zu bytes\n", sizeof(char));
    printf("The size of an int is: %zu bytes\n", sizeof(int));
    printf("The size of a long is: %zu bytes\n", sizeof(long));
    printf("The size of an unsigned int is: %zu bytes\n", sizeof(unsigned int));

    // Look here!! int16_t will always be 2 bytes!!!!!!!!
    printf("The size of an int16_t is ALWAYS: %zu bytes\n", sizeof(int16_t));
    return 0;
}