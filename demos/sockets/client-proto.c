
#include "client-proto.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>

#include "protocol.h"

#define PORT_NUM    1090

static uint8_t send_buffer[MAX_MSG_BUFF] = {0};
static uint8_t recv_buffer[MAX_MSG_BUFF] = {0};


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
static void start_client(proto_msg_t *send_msg, proto_msg_t *recv_message){
    struct sockaddr_in addr;
    int data_socket;
    int data_len;
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

    print_proto_msg("client-send", send_msg);
    data_len = get_msg_len(send_msg);
    ret = send(data_socket, send_msg, data_len, 0);
    if (ret == -1) {
        perror("header write error");
        exit(EXIT_FAILURE);
    }
    if (ret != data_len){
        printf("error, did not send entire message, sent %d bytes\n", ret);
        exit(EXIT_FAILURE);
    }

    printf("\nREQUEST SENT.... WAITING FOR CLIENT RESPONSE...\n\n");

    //NOW READ RESPONSES BACK - 2 READS, HEADER AND DATA
    ret = recv(data_socket, recv_message, MAX_MSG_BUFF,0);
    if (ret == -1) {
        perror("read error");
        exit(EXIT_FAILURE);
    }

    print_proto_msg("client-recv", recv_message);

    close(data_socket);

}

void usage(char *exe_name){
    printf("usage:  %s [-h] | \"message\" [work_sim_amount]\n", exe_name);
    printf("where:\n\n");
    printf("               -h:   Print this usage message\n");
    printf("        \"message\":   Set the message to send\n");
    printf("  work_sim_amount:   Simulates work on server (in seconds)\n");
    printf("\nIf message not provided, a default message will be sent!\n");
}

int main(int argc, char *argv[])
{
    proto_msg_t *send_msg, *recv_msg;
    char *default_msg = "DEFAULT MESSAGE - USE ARGV[1] TO CHANGE";
    send_msg = (proto_msg_t *)send_buffer;
    recv_msg = (proto_msg_t *)recv_buffer;

    if ((argc == 2) && (strcmp(argv[1], "-h")==0)){
        usage(argv[0]);
        exit(0);
    }

    send_msg->proto_header.msg_dir = PROTO_DIR_REQ;
    send_msg->proto_header.proto_ver = PROTO_VERSION;
    send_msg->proto_header.proto_id = PROTO_IDENTITY;
    send_msg->proto_header.proto_work_sim = 0;

    if (argc >= 2) {
        strcpy((char *)send_msg->payload, argv[1]);
        send_msg->proto_header.msg_len = strlen(argv[1]);
    } else {
        strcpy((char *)send_msg->payload, default_msg);
        send_msg->proto_header.msg_len = strlen(default_msg);
    }

    if (argc == 3){
        send_msg->proto_header.proto_work_sim = atoi(argv[2]);
    }

    //start the client
    start_client( send_msg, recv_msg);
}