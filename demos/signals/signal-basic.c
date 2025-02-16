#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

int main(){

    unsigned long counter = 0;

    //ignore the SIGINT (CTRL+C) signal
    signal(SIGINT, SIG_IGN);

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

