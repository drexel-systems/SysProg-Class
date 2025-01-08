#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int, int);
//add additional prototypes here
void reverse_string(char *, int);

int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    int user_str_i = 0;
    int buff_i = 0;

    while (user_str[user_str_i] != '\0') {
        // Check if the string is larger than the buffer
        if (buff_i >= len) {
            return -1;
        }
    
        // Skip consecutive white spaces
        if (user_str[user_str_i] == ' ' && user_str_i > 0 && buff[buff_i - 1] == ' ') {
            user_str_i++;
            continue;
        }

        // Replace tabs with spaces, else copy the character
        buff[buff_i] = (user_str[user_str_i] == '\t') ? ' ' : user_str[user_str_i];
        user_str_i++;
        buff_i++;
    }
    int str_len = buff_i; // save the length of the string

    while (buff_i < len) {
        buff[buff_i] = '.';
        buff_i++;
    }
    return str_len; // return the length of the string not including the null terminator
}

void print_buff(char *buff, int len){
    printf("---\nBuffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int len, int str_len){
    int word_count = 0;
    for (int i = 0; i < str_len; i++) {
        if (buff[i] == ' ') { // count spaces as word separators
            word_count++;
        }
    }

    return word_count + 1; // add 1 to account for the last word
}

void reverse_string(char *buff, int user_str_len) {
    for (int i = user_str_len - 1; i >= 0; i--) {
        printf("%c", buff[i]);
    }
    printf("\n");

}

void word_print(char *buff, int word_count, int str_len){
    printf("Word Print\n----------\n1. ");

    int current_word_i = 0;
    int current_word_len = 0;

    for (int i = 0; i <= str_len; i++) {
        if (buff[i] == ' ') {
            printf(" (%d) ", current_word_len);

            current_word_i++;
            current_word_len = 0;
            printf("\n%d. ", current_word_i);
            continue;
        } else {
            printf("%c", buff[i]);
        }
    }
    printf("\n");
    
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      This is safe because the program will exit gracefully if arv[1] does not exist.
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
    //      The if statement below checks if there are not enough arguments to run the program
    //      and exits gracefully if there are not enough arguments.
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    buff = malloc(BUFFER_SZ);
    if (buff == NULL && BUFFER_SZ > 0){
        printf("Error allocating buffer\n");
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case ('c' || 'w'):
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            if (opt == 'c'){
                printf("Word Count: %d\n", rc);
            } else {
                word_print(buff, rc, user_str_len);
            }
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            reverse_string(buff, user_str_len);
            break;

        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE