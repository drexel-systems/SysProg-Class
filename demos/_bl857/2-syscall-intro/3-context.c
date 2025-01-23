// CONCEPT: dont worry about the logic of the code; this just demonstrates the cost of context switching
// CONCEPT: that boundary is also a trust boundary; the kernel is the only thing that can switch between processes

#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define ITERATIONS 1000000  // Number of context switches

int main() {
    int pipe1[2], pipe2[2]; // Two pipes for bidirectional communication
    pipe(pipe1); // Parent to child
    pipe(pipe2); // Child to parent

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        // Child process
        char buffer[1];
        for (int i = 0; i < ITERATIONS; i++) {
            read(pipe1[0], buffer, 1);  // Read from parent
            write(pipe2[1], "c", 1);   // Respond to parent
        }
    } else {
        // Parent process
        char buffer[1];
        struct timespec start, end;

        // Start timing
        clock_gettime(CLOCK_MONOTONIC, &start);

        for (int i = 0; i < ITERATIONS; i++) {
            write(pipe1[1], "p", 1);   // Send to child
            read(pipe2[0], buffer, 1); // Receive from child
        }

        // End timing
        clock_gettime(CLOCK_MONOTONIC, &end);

        // Calculate elapsed time
        double elapsed = (end.tv_sec - start.tv_sec) + 
                         (end.tv_nsec - start.tv_nsec) / 1e9;
        printf("Elapsed time for %d context switches: %.6f seconds\n", ITERATIONS, elapsed);

        // Wait for child to finish
        wait(NULL);
    }

    return 0;
}
