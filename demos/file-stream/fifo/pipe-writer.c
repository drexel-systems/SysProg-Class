#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

int main() {
    const char *fifo_path = "/tmp/myfifo";
    
    // Create the FIFO if it doesn't exist
    if (mkfifo(fifo_path, 0666) == -1 && errno != EEXIST) {
        perror("Error creating FIFO");
        exit(1);
    }

    // Open with O_RDWR to avoid blocking
    int fifo = open(fifo_path, O_RDWR | O_NONBLOCK);
    if (fifo == -1) {
        perror("Error opening FIFO");
        exit(1);
    }

    char input[256];
    printf("Enter strings to send to the FIFO (Ctrl+D to quit):\n");
    printf("> ");
    while (fgets(input, sizeof(input), stdin)) {
        input[strcspn(input, "\n")] = '\0';

        ssize_t bytes_written = write(fifo, input, strlen(input) + 1);
        if (bytes_written == -1) {
            if (errno == EAGAIN) {
                printf("No reader - message buffered in pipe\n");
                usleep(100000);  // Small delay to prevent tight looping
            } else {
                perror("Error writing to FIFO");
                break;
            }
        } else {
            printf("Message sent to FIFO: %s\n", input);
            printf("> ");
        }
    }

    close(fifo);
    printf("Writer program exiting...\n");

    return 0;
}