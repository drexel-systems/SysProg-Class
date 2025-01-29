#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>


void echo_received_msg(char *msg, int len);

void echo_received_msg(char *msg, int len){
    //Note that the reader could have started with things in 
    //the queue or fall behind so a msg could have multiple null
    //strings inside
    int echo_amt = 0;
    int msg_len;

    while(echo_amt < len){
        printf("Received message: %s\n", msg);
        msg_len = strlen(msg) + 1; //we want to count the null byte
        echo_amt += msg_len;
        msg += msg_len;
    }
}

int main() {
    const char *fifo_path = "/tmp/myfifo";
    
    // Open the FIFO for reading
    printf("Waiting for a connection from the FIFO...\n");

    int fifo = open(fifo_path, O_RDONLY);
    if (fifo == -1) {
        perror("Error opening FIFO for reading");
        exit(1);
    }

    char buffer[256];
    ssize_t bytes_read;

    printf("Connected! Waiting messages from the FIFO...\n");

    // Read messages continuously
    while ((bytes_read = read(fifo, buffer, sizeof(buffer))) > 0) {
        buffer[bytes_read - 1] = '\0';  // Null-terminate the string
        //printf("Received message: %s\n", buffer);
        echo_received_msg(buffer, bytes_read);
    }

    if (bytes_read == -1) {
        perror("Error reading from FIFO");
    }

    close(fifo);
    printf("Reader program exiting...\n");

    return 0;
}