#pragma once

//Note its better to use standard and predictable 
//size values for data types in network protocols
#include <stdint.h>




//for this demo we are just sending a sequence of characters
//but the structure can be more robust in more complicated
//protocols

//proto_header is FIXED size
typedef struct proto_header {
    uint16_t  proto_id;
    uint16_t  proto_ver;
    uint16_t  proto_work_sim;
    uint16_t  msg_dir;
    uint16_t  msg_len;
}proto_header_t;

//the msg is what will be sent over the wire, the header 
//followed by the data
typedef struct proto_msg{
    proto_header_t proto_header;
    uint8_t        payload[];
}proto_msg_t;

#define MAX_MSG_BUFF    2048
#define MAX_PAYLOAD_SZ  (MAX_MSG_BUFF - (int)sizeof(proto_header_t))

#define PROTO_IDENTITY  0x0808      //protocol identifier
#define PROTO_VERSION   1
#define PROTO_DIR_REQ   1
#define PROTO_DIR_RSP   2

#define     OK                  0
#define     ERR_MSG             -1
#define     ERR_BUFF_TO_SMALL   -2

proto_msg_t *build_msg(uint8_t *data, uint16_t len, 
              uint8_t *msg_buff, uint16_t msg_buff_len);

proto_msg_t  *extract_msg(uint8_t *raw_buff, uint16_t buff_len);
uint16_t get_msg_len(proto_msg_t *);
void print_proto_msg(char *from, proto_msg_t *msg);
