//This is a hack, but VSCode seems to have a bug in it indicating errors
//that sigset_t is undefined unless you add the _GNU_SOURCE  #define
//Note that the code compiles file without this, but it messes up the
//tools in VSCode without it
#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

int main() {
    sigset_t mask;

    printf("Im working for the next 5 seconds to ensure that\n");
    printf("the nuclear reactor does not melt down.  Going into\n");
    printf("do not disturb mode, when I am done you can stop me\n");

    // Block all signals
    sigfillset(&mask);
    sigprocmask(SIG_BLOCK, &mask, NULL);
    printf("\nAll signals are now blocked!\n\n");

    for (int i = 5; i > 0; i--){
        printf("\tStill working, %d seconds to go\n",i);
        sleep(1);
    }


    printf("\nAll signals are now unblocked!\n");
    printf("Note any pending signals while i was blocking them will now be\n");
    printf("delivered and processed before i start busy-work...\n\n");
    // Unblock all signals
    sigprocmask(SIG_UNBLOCK, &mask, NULL);

    //loop forever
    unsigned long counter = 0;
    while(1){
        printf("Im going to loop forever now, counter = %ld\n",counter);
        counter++;
        sleep(1);
    }

    return 0;
}