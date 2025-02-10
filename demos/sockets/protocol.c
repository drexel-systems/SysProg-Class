#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "protocol.h"

proto_msg_t *build_msg(uint8_t *data, uint16_t len, 
              uint8_t *msg_buff, uint16_t msg_buff_len){

    proto_msg_t     *msg;
    int             bytes_needed;

    //see how many bytes we need
    bytes_needed = len + (int)sizeof(proto_header_t);
    if (msg_buff_len < bytes_needed)
        return NULL;

    //good idea to clear the buffer
    memset(msg_buff, 0, msg_buff_len);

    // Structure the message buffer as follows:
    // +----------------+--------------------------+
    // | PROTO_HEADER   | MESSAGE DATA.............|
    // +----------------+--------------------------+
    msg = (proto_msg_t *)msg_buff;

    //now fill out the header
    msg->proto_header.proto_id = PROTO_IDENTITY;
    msg->proto_header.proto_ver = PROTO_VERSION;
    msg->proto_header.msg_dir = PROTO_DIR_REQ;
    msg->proto_header.msg_len = len;
    
    //now copy the data
    memcpy(msg->payload, data, len);

    return msg;
}

proto_msg_t  *extract_msg(uint8_t *raw_buff, uint16_t buff_sz){
    proto_msg_t *msg;
    uint16_t    length_needed;
    msg = (proto_msg_t *)raw_buff;


    //we have to at least have the proto header
    if(buff_sz < (uint16_t)sizeof(proto_header_t))
        return NULL;

    //check length
    length_needed = msg->proto_header.msg_len + 
            (uint16_t)sizeof(proto_header_t);
    if (length_needed < buff_sz)
        return NULL;

    return msg;
}

uint16_t get_msg_len(proto_msg_t *msg){
    return msg->proto_header.msg_len + 
        (uint16_t)sizeof(proto_header_t);
}

void print_proto_msg(char *from, proto_msg_t *msg){
    printf("Debug Label: %s.  PROTOCOL INFORMATION:\n", from);
    printf("\t       ID: %d\n",msg->proto_header.proto_id);
    printf("\t  Version: %d\n",msg->proto_header.proto_ver);
    
    if (msg->proto_header.proto_id == PROTO_IDENTITY){
        printf("\t     Name: CLASS_ECHO_PROTOCOL\n");      
    }
    printf("\t  Version: %d\n",msg->proto_header.proto_ver);
    printf("\t Sim Work: %d seconds\n",msg->proto_header.proto_work_sim);
    printf("\t   Length: %d bytes\n",msg->proto_header.msg_len);
    
    switch(msg->proto_header.msg_dir){
        case PROTO_DIR_REQ:
            printf("\tDirection: %d - REQUEST\n",msg->proto_header.msg_dir);
            break;
        case PROTO_DIR_RSP:
            printf("\tDirection: %d - RESPONSE\n",msg->proto_header.msg_dir);
            break;
        default:
            printf("\tDirection: %d - UNKNOWN\n",msg->proto_header.msg_dir);
            break;
    }
    printf("PAYLOAD\n-------\n");
    printf("%.*s\n\n", msg->proto_header.msg_len, msg->payload);
}
