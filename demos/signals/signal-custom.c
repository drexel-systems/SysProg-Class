#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#define MESSAGE "\n\nNICE TRY - BUT YOU CANT STOP ME\n\n"
#define MSG_LEN 35

static void my_wise_guy_signal_handler(int sig_num){

    //discussion, why cant we use printf?
    if (sig_num == SIGINT)
        write(STDOUT_FILENO, MESSAGE, MSG_LEN);
}

int main(){

    unsigned long counter = 0;
    
    //custom SIGINT (CTRL+C) signal
    signal(SIGINT, my_wise_guy_signal_handler);

    ////stay in this loop for 5 seconds
    while(counter < 20){
        printf("My counter value is: %ld - YOU CANT STOP ME\n",counter);
        counter++;
        usleep(250 * 1000); //250 ms
    }

    //restore the default SIGINT (CTRL+C) signal
    signal(SIGINT, SIG_DFL);
    //loop forever
    while(1){
        printf("My counter value is: %ld - CTRL-C to STOP ME\n",counter);
        counter++;
        usleep(250 * 1000); //250 ms
    }
}

