
#include "client-echo.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>

#define PORT_NUM    1090
#define EOF_CHAR_STR "\x05"   //CTRL+D is EOF in general ASCII 5

#define BUFF_SZ 512
static uint8_t send_buffer[BUFF_SZ] = {0};
static uint8_t recv_buffer[BUFF_SZ] = {0};


/*
 *  This function "starts the client".  It takes a header structure
 *  and a properly formed packet.  The packet is basically the
 *  header with a message at the end of it in the case of the
 *  command being CMD_PING_PONG.  The length in the header field
 *  indicates the total size to send to the server, thus it will
 *  be sizeof(cs472_proto_header_t) in the case that the cmd is
 *  CMD_COURSE_INFO, or it will be 
 *  sizeof(cs472_proto_header_t) + strlen(MESSAGE) + 1 in the case
 *  we are doing a ping to the server.  We add the extra 1 to send
 *  over the null terminator for the string
 */
static void start_client(uint8_t *packet){
    struct sockaddr_in addr;
    int data_socket;
    int ret;

    /* Create local socket. */

    data_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (data_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    /*
     * For portability clear the whole structure, since some
     * implementations have additional (nonstandard) fields in
     * the structure.
     */

    memset(&addr, 0, sizeof(struct sockaddr_in));

    /* Connect socket to socket address */

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(PORT_NUM);

    ret = connect (data_socket, (const struct sockaddr *) &addr,
                   sizeof(struct sockaddr_in));
    if (ret == -1) {
        fprintf(stderr, "The server is down.\n");
        exit(EXIT_FAILURE);
    }

    ret = send(data_socket, packet, strlen((char *)packet), 0);
    if (ret == -1) {
        perror("header write error");
        exit(EXIT_FAILURE);
    }

    //NOW READ RESPONSES BACK - 2 READS, HEADER AND DATA
    ret = recv(data_socket, recv_buffer, sizeof(recv_buffer),0);
    if (ret == -1) {
        perror("read error");
        exit(EXIT_FAILURE);
    }

    printf("RECV FROM SERVER -> %s\n",recv_buffer);

    close(data_socket);

}


int main(int argc, char *argv[])
{
    strcpy((char *)send_buffer, "DEFAULT TEXT");

    if (argc == 2)
        strcpy((char *)send_buffer, argv[1]);

    //start the client
    start_client( send_buffer);
}