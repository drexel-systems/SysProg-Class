#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <duration_in_seconds>\n", argv[0]);
        return 1;
    }

    // Convert command line argument to integer
    int target_duration = atoi(argv[1]);
    if (target_duration <= 0) {
        fprintf(stderr, "Please provide a positive number of seconds\n");
        return 1;
    }

    // Get start time
    time_t start_time = time(NULL);
    time_t current_time;
    int elapsed_time = 0;
    int iteration = 0;

    printf("Starting loop that will run for %d seconds\n", target_duration);

    while (1) {
        // Get current time and calculate elapsed time
        current_time = time(NULL);
        elapsed_time = current_time - start_time;

        // Check if we've exceeded the target duration
        if (elapsed_time >= target_duration) {
            break;
        }

        iteration++;
        printf("Iteration %d - Elapsed time: %d seconds [pid:%d, ppid:%d]\n", 
                iteration, elapsed_time, getpid(), getppid());
        
        // Sleep for 5 seconds
        sleep(2);
    }

    printf("Program finished after %d seconds\n", elapsed_time);
    return 0;
}