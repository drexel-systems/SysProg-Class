#pragma once

#include "protocol.h"

static void start_server();
static void process_requests(int listen_socket);
int simulate_useful_work(proto_msg_t *proto);
int build_rsp_from_req(proto_msg_t *req_message, proto_msg_t *rsp_msg);