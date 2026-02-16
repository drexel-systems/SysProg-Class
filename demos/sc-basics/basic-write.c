#include <unistd.h>
#include <stdio.h>

int main(){

    int bytes_written = 0;
    // int rc = 0;

    // while (bytes_written < 12) {
    //     rc = write(STDOUT_FILENO, "Hello there\n" + bytes_written, 12 - bytes_written);
    //     if (rc < 0) {
    //         perror("write failed");
    //         return 1;
    //     }
    //     bytes_written += rc;
    // }
    
    
    //int rc = write(STDOUT_FILENO, "Hello there\n", 12);

    char *mystring = "Hello there\n";

    //int rc = write(1, mystring, 12);
    int rc = write(STDOUT_FILENO, mystring, 12);

    printf("Mystring is at memory location %p\n", (void*)mystring);
    
    printf("write rc = %d\n",rc);

    //printf("The number is: %04x\n", 42);
}