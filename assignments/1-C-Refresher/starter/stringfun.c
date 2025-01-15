#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here


int setup_buff(char **buff, char *user_str, int len) {
    *buff = malloc(len); // Allocate memory for the buffer
    if (*buff == NULL) {
        return -1; // Allocation failed
    }

    char *temp_buff = *buff; // Pointer to iterate through the buffer
    int buffLen = 0;
    int consecSpace = 0;

    while (*user_str != '\0' && buffLen < len - 1) { // Ensure not to overflow
        if (*user_str == ' ' || *user_str == '\t' || *user_str == '\n') {
            if (consecSpace == 0) { // Allow only one space
                *temp_buff++ = ' ';
                buffLen++;
                consecSpace = 1;
            }
        } else {
            *temp_buff++ = *user_str;
            buffLen++;
            consecSpace = 0;
        }
        user_str++;
    }

    // Fill remaining space with '.' if buffer isn't full
    while (buffLen < len - 1) {
        *temp_buff++ = '.';
        buffLen++;
    }

    *temp_buff = '\0'; // Null-terminate the string
    return 0; // Success
}

void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}


// *buff - pointer to the word?
int count_words(char *buff, int len, int str_len){
    char *ptr = buff;
    char *end = buff + str_len;

    for (int i = 0; i < str_len)
    return 0;
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      This is safe because it handles the case where arv[1] does not exit and causes the program to exit. 
    //      No other processes will be ran which prevents errors
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      This checks is there are not 3 arguments inputted which is needed for the flag and string
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3


    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE