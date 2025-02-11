/*
 * THE MOST BASIC SOCKET SERVER
 *
 */
 
#include "server-echo.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>

#define BUFF_SZ 512

#define PORT_NUM    1090

static uint8_t send_buffer[BUFF_SZ];
static uint8_t recv_buffer[BUFF_SZ];


/*
 *  This function accepts a socket and processes requests from clients
 *  the server runs until stopped manually with a CTRL+C
 */
static void process_requests(int listen_socket){
    int data_socket;
    int ret;

    //again, not the best approach, need ctrl-c to exit
    while(1){
        //Do some cleaning
        memset(send_buffer,0,sizeof(send_buffer));
        memset(recv_buffer,0,sizeof(recv_buffer));

        //Establish a connection
        data_socket = accept(listen_socket, NULL, NULL);
        if (data_socket == -1) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        printf("\t RECEIVED REQ...\n");

        /* Wait for next data packet. */
        ret = recv(data_socket, recv_buffer, sizeof(recv_buffer),0);
        if (ret == -1) {
            perror("read error");
            exit(EXIT_FAILURE);
        }
 
        int buff_len = snprintf((char *)send_buffer, 
                sizeof(send_buffer), "THANK YOU -> %s", recv_buffer);

        //now string out buffer has the length
        send (data_socket, send_buffer, buff_len, 0);
        close(data_socket);
    }
}

/*
 *  This function starts the server, basically creating the socket
 *  it will listen on INADDR_ANY which is basically all local
 *  interfaces, eg., 0.0.0.0
 */
static void start_server(){
    int listen_socket;
    int ret;
    
    struct sockaddr_in addr;

    /* Create local socket. */
    listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (listen_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /*
     * NOTE this is good for development as sometimes port numbers
     * get held up, this forces the port to be bound, do not use
     * in a real application
     */
    int enable=1;
    setsockopt(listen_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int));

    /* Bind socket to socket name. */
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(PORT_NUM);

    ret = bind(listen_socket, (const struct sockaddr *) &addr,
               sizeof(struct sockaddr_in));
    if (ret == -1) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    /*
     * Prepare for accepting connections. The backlog size is set
     * to 20. So while one request is being processed other requests
     * can be waiting.
     */
    ret = listen(listen_socket, 20);
    if (ret == -1) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    //Now process requests, this will never return so its bad coding
    //but ok for purposes of demo
    process_requests(listen_socket);

    close(listen_socket);
}

int main()
{
    printf("STARTING SERVER - CTRL+C to EXIT \n");
    start_server();
}