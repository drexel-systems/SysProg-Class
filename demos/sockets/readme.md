## Socket Demonstrations

This demo contains a few demonstration programs

**`client-echo` and `server-echo`**:  A simple socket client and server.  The client sends a C string (null terminated) to the server.  The server echos back what the client sent with a prefix of `THANK YOU ->`.  For example sending `./client-echo "Hi Class"` will result in the server returning `THANK YOU -> Hi Class`.  Note that `client-echo` takes an arguement to set the string being sent to the server, if one is not provided the default text `DEFAULT TEXT` will be used.

**`client-proto` and `server-proto`**: A more realistic socket client and server. This version uses a formal protocol definition documented in `protocol.h`:

```c
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
```
The protocol defined above has a structured header `proto_header_t` that provides protocol-specific information to the client and server. The format of the messages sent over the socket are specified by `proto_msg_t`.  Notice that `proto_msg_t` has a byte array called `payload` at the end.  These are the strings that will be echoed back and forth between the client and server.  To make this even more realistic there are a few other things:

1. The `payload[]` is not a C string.  It is a sequence of bytes.  In order to work with this you need to trust the `msg_len` field in the header.

2. In order to simulate useful work, the protocol header has a field called `proto_work_sim`.  This field is used by the server to simulate useful work. For now all that happens is that the server blocks/sleeps for a specified number of seconds based ont the `proto_work_sim` header.

To run the server just execute `server-proto`.  To run the client you can run `client-proto` and use the default parameters.  The `client-proto` program will provide usage information if you use the `-h` option.  You can pass alternative payload data and set the server simulated work time as well. 

**`client-proto` and `server-proto-thread`**: This pair upgrades the server to use individual threads for each client connection. It will demonstrate handling multiple clients concurrently.  For demo purposes if you have 3 terminals:

1. First terminal run server:  `./server-proto`
2. Second terminal run client 1 with 20 sec of work: `./client-proto "testing" 20`
3. Third terminal run client 2 with 0 sec of work: `./client-proto "fast"`

The above will stall the second client until the first one finishes.  If you repeat the above but use `server-proto-thread` instead you will see the second client finish immediately. 