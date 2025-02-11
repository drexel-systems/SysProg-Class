#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "protocol.h"

#define BUFF_SZ 1024

int main(){
    uint8_t *msg_buff = malloc(BUFF_SZ);
    char *tst_msg = "TEST MESSAGE";
    proto_msg_t *send_msg, *recv_message;

    send_msg = build_msg((uint8_t *)tst_msg, strlen(tst_msg), 
              msg_buff, BUFF_SZ);

    if (send_msg == NULL){
        printf("Error building send message\n");
        exit(1);
    }

    uint16_t snd_msg_sz = get_msg_len(send_msg);

    //SIMULATE SENDING MSG_BUFF OVER NETWORK
    uint8_t *rsp_buff = malloc(BUFF_SZ);
    uint16_t recv_msg_sz = snd_msg_sz;
    memcpy(rsp_buff, msg_buff, snd_msg_sz);

    //--------------------------------------

    recv_message = extract_msg(rsp_buff, recv_msg_sz);
    uint8_t *msg_data = recv_message->payload;

    printf("The message is: %.*s\n", recv_message->proto_header.msg_len,
            msg_data);
 
    return 0;
}