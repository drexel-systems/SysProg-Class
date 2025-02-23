#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>

// what does this do?
int main() {
    pid_t pid_supervisor;

    // Fork a supervisor process
    pid_supervisor = fork();
    if (pid_supervisor == -1) {
        perror("fork supervisor");
        exit(EXIT_FAILURE);
    }

    if (pid_supervisor == 0) {  // Supervisor process
        int pipefd[2];
        pid_t pid1, pid2;

        // Create pipe
        if (pipe(pipefd) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        // Create first child process
        pid1 = fork();
        if (pid1 == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid1 == 0) {  // First child (cat)
            close(pipefd[0]); // <---- not used in this child
            dup2(pipefd[1], STDOUT_FILENO);
            close(pipefd[1]); // <---- not needed after copy

            char *cmd1[] = {"cat", "./fork-pipe.c", NULL};
            execvp(cmd1[0], cmd1);
            perror("execvp cat");
            exit(EXIT_FAILURE);
        }

        // Create second child process
        pid2 = fork();
        if (pid2 == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid2 == 0) {  // Second child (wc)
            close(pipefd[1]); // <---- not used in this child
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]); // <---- not needed after copy

            char *cmd2[] = {"wc", "-l", NULL};
            execvp(cmd2[0], cmd2);
            perror("execvp wc");
            exit(EXIT_FAILURE);
        }

        // Supervisor process
        close(pipefd[0]);
        close(pipefd[1]);

        // Wait for both children
        waitpid(pid1, NULL, 0);
        waitpid(pid2, NULL, 0);
        exit(EXIT_SUCCESS);
    }

    // Main parent process
    // Only needs to wait for the supervisor
    waitpid(pid_supervisor, NULL, 0);
    return 0;
}