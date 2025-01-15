#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int);
//add additional prototypes here
void reverse_string(char *, int);
void word_print(char *, int);
void word_replace(char *, int, char *, char *, int);
int word_len (char *);

int setup_buff(char *buff, char *user_str, int buffer_sz){
    //TODO: #4:  Implement the setup buff as per the directions
    int user_str_i = 0;
    int buff_i = 0;

    while (user_str[user_str_i] != '\0') {
        // Check if the string is larger than the buffer
        if (buff_i >= buffer_sz) {
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

    while (buff_i < buffer_sz) {
        buff[buff_i] = '.';
        buff_i++;
    }
    return str_len; // return the length of the string not including the null terminator
}

void print_buff(char *buff, int len){
    printf("----------\nBuffer:  ");
    for (int i=0; i < len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int str_len){
    if (str_len == 0 || (buff[0] == ' ' && !buff[1]) ) return 0;
    
    int word_count = 0;
    for (int i = 0; i < str_len; i++) {
        if (buff[i] == ' ' && i > 0 && buff[i + 1] != '.') { // count spaces as word separators
            word_count++;
        }
    }

    return word_count + 1; // add 1 to account for the last word
}

void reverse_string(char *buff, int user_str_len) {
    if (str_len == 0 || buff[0] == ' ') {
        printf("No words to reverse!\n");
        return;
    }

    for (int i = user_str_len - 1; i >= 0; i--) {
        printf("%c", buff[i]);
    }
    printf("\n");

}

void word_print(char *buff, int str_len) {
    if (str_len == 0 || buff[0] == ' ') {
        printf("No words!\n");
        return;
    }

    printf("Word Print\n----------\n1. ");
    
    int word_i = 2;
    int word_len = 0;
    
    for (int i = 0; i < str_len; i++) {
        if (buff[i] == ' ' || buff[i] == '.') {
            if (word_len > 0) {
                printf(" (%d)\n", word_len);
                if (buff[i] == '.' || (buff[i] == ' ' && buff[i + 1] == '.')) break;
                printf("%d. ", word_i++);
                word_len = 0;
            }
            continue;
        }
        putchar(buff[i]);
        word_len++;
    }
}

int word_len (char *word) {
    int len = 0;
    while (word[len] != '\0') {
        len++;
    }
    return len;
}

void word_replace(char *buff, int str_len, char *replace_word, char *replace_with, int buffer_sz) {
    int replace_word_len = word_len(replace_word);
    int replace_with_len = word_len(replace_with);
    
    if (str_len - replace_word_len + replace_with_len > buffer_sz) {
        printf("Error: Replacement word is too long for buffer of size %d\n", buffer_sz);
        return;
    }

    for (int i = 0; i <= str_len - replace_word_len; i++) {
        int match = 1;
        
        // Check if we found a match for replace_word
        for (int j = 0; j < replace_word_len; j++) {
            if (buff[i + j] != replace_word[j]) {
                match = 0;
                break;
            }
        }

        if (match == 1) {
            // Save the rest of the string after replace_word
            char temp[buffer_sz];
            int rest_start = i + replace_word_len;
            int rest_len = str_len - rest_start;
            for (int j = 0; j < rest_len; j++) {
                temp[j] = buff[rest_start + j];
            }
            
            // Insert replace_with
            for (int j = 0; j < replace_with_len; j++) {
                buff[i + j] = replace_with[j];
            }
            
            // Copy back the rest of the string
            for (int j = 0; j < rest_len; j++) {
                buff[i + replace_with_len + j] = temp[j];
            }
            
            return;  // Exit after first replacement
        }
    }
}
//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line

    char *replace_word;     //used for -x option
    char *replace_with;     //used for -x option

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

    // -x option requires two additional arguments
    replace_word = (argv[3] == NULL) ? NULL : argv[3];
    replace_with = (argv[4] == NULL) ? NULL : argv[4];

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
        case 'c':
            rc = count_words(buff, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        case 'r':
            reverse_string(buff, user_str_len);
            break;
        case 'w':
            word_print(buff, user_str_len);
            break;
        case 'x':
            word_replace(buff, user_str_len, replace_word, replace_with, BUFFER_SZ);
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