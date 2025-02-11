#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

// Structure to hold command and its arguments
typedef struct {
    char **args;
} Command;

void execute_pipeline(Command commands[], int num_commands) {
    int pipes[num_commands - 1][2];  // Array of pipes
    pid_t pids[num_commands];        // Array to store process IDs

    // Create all necessary pipes
    for (int i = 0; i < num_commands - 1; i++) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Create processes for each command
    for (int i = 0; i < num_commands; i++) {
        pids[i] = fork();
        if (pids[i] == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0) {  // Child process
            // Set up input pipe for all except first process
            if (i > 0) {
                dup2(pipes[i-1][0], STDIN_FILENO);
            }

            // Set up output pipe for all except last process
            if (i < num_commands - 1) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Close all pipe ends in child
            for (int j = 0; j < num_commands - 1; j++) {
                close(pipes[j][0]);
                close(pipes[j][1]);
            }

            // Execute command
            execvp(commands[i].args[0], commands[i].args);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }

    // Parent process: close all pipe ends
    for (int i = 0; i < num_commands - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all children
    for (int i = 0; i < num_commands; i++) {
        waitpid(pids[i], NULL, 0);
    }
}

int main() {
    // Example usage with three commands: cat file | grep pattern | wc -l
    Command commands[] = {
        {.args = (char *[]){"cat", "./fork-pipe.c", NULL}},
        {.args = (char *[]){"grep", "//", NULL}},
        {.args = (char *[]){"wc", "-l", NULL}}
    };
    int num_commands = sizeof(commands) / sizeof(commands[0]);

    pid_t supervisor = fork();
    if (supervisor == -1) {
        perror("fork supervisor");
        exit(EXIT_FAILURE);
    }

    if (supervisor == 0) {  // Supervisor process
        execute_pipeline(commands, num_commands);
        exit(EXIT_SUCCESS);
    }

    // Main parent just waits for supervisor
    waitpid(supervisor, NULL, 0);
    return 0;
}