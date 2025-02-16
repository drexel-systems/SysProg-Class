#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define MSG3 "\n\nIT WILL TAKE 3 MORE ATTEMPTS TO STOP ME\n\n"
#define MSG2 "\n\nIT WILL TAKE 2 MORE ATTEMPTS TO STOP ME\n\n"
#define MSG1 "\n\nIT WILL TAKE 1 MORE ATTEMPT TO STOP ME\n\n"
#define MSG0 "\n\nI GUESS YOU REALLY WANT TO STOP ME\n\n"
#define MSG_ERR "\n\nTHIS CANT REALLY BE HAPPENING\n\n"
#define MSG3_LEN 43
#define MSG2_LEN 43
#define MSG1_LEN 42
#define MSG0_LEN 38
#define MSG_ERR_LEN 34

static volatile sig_atomic_t attempt_counter = 3;

static void my_wise_guy_signal_handler(int sig_num){

    if (sig_num == SIGINT){
        switch(attempt_counter){
            case 3:
                write(STDOUT_FILENO, MSG3, MSG3_LEN);
                attempt_counter = 2;
                break;
            case 2:
                write(STDOUT_FILENO, MSG2, MSG2_LEN);
                attempt_counter = 1;
                break;
            case 1:
                write(STDOUT_FILENO, MSG1, MSG1_LEN);
                attempt_counter = 0;
                break;
            case 0:
                write(STDOUT_FILENO, MSG0, MSG0_LEN);
                exit(0);
            default:
                write(STDOUT_FILENO, MSG_ERR, MSG_ERR_LEN);
                exit(1);
        }
    }
}

int main(){

    unsigned long counter = 0;

    //custom SIGINT (CTRL+C) signal
    signal(SIGINT, my_wise_guy_signal_handler);

    
    //loop forever
    while(1){
        printf("My counter value is: %ld - CTRL-C to STOP ME\n",counter);
        counter++;
        usleep(250 * 1000); //250 ms
    }
}

