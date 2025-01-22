#include <stdio.h>
#include <string.h>

void print_string_length(const char *str) {
    size_t length = strlen(str);
    printf("The number of characters in the string \"%s\" is %zu.\n", str, length);
}

int main() {
    const char *str = "Hello, World!";
    print_string_length(str);
    return 0;
}