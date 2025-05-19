## Remote Drexel Shell

This work builds on your previous shell version that executed commands and supported pipes.  Except this time, we will enable the shell program to be executed over a network.  To do this we will enhance our `dsh` executable to accept some parameters.  Specifically, `dsh` should mirror the behavior of the piped version of your shell, performing local `fork/exec` to run commands, including built-in commands.  However, `dsh_cli.c` has been enhanced to support additional command line arguments:

```bash
#start dsh in server mode
./dsh -s 
#note you can override:
# -i:  Overrides default interface, e.g., -i 127.0.0.1
# -p:  Overrides default port, e.g., -p 5678
```

The starter code provides information and defaults for both the `interfaces` argument as well as a default `port`.  Note that server interfaces are specified in terms of IP addresses, and almost always we use the value of `0.0.0.0` for the server to indicate that the server should bind on all available network interfaces. 

```bash
# start dsh in client mode
./dsh -c 
#note like server-mode you can override:
# -i:  Overrides default ip address of the server, e.g., -i 129.25.203.107
# -p:  Overrides default port, e.g., -p 5678
```
In client mode, the `dsh` shell will connect to the server and send it commands.  The IP address of the server and port can be provided if the default options require changing.  Note that the defaults will work well - for example `dsh -s` to start the server and `dsh -c` to start the client if you are running in a local VM; however, the defaults might not work if you are running on tux as others might be using the default port number for this program of `1234`.  If you are running on tux an example configuration might look like:

```bash
#start the server on tux, use port 7890
./dsh -s -i 0.0.0.0 -p 7890

#start the client to connect to tux, assuming tux's IP
#address is 129.25.203.107 and the server is running
#on port number 7890
./dsh -c -i 129.25.203.107 -p 7890
```

#### A cautionary tale about network I/O
We will be using the TCP/IP protocol, which is a **stream** protocol.  This means that TCP/IP sends bytes in streams.  There is no corresponding indication of where a logical stream begins and a logical stream ends.  Consider you send the command `ls` in a single send to the server.  Its possible that the server will receive `ls\0`, `ls`, or `l` on a receive.  In other words, the network might break up sends into multiple receives.  You must handle this yourself when receiving data over the network.

There are many techniques to handle managing a logical stream.  We will consider the easiest technique that involves using a special character to mark the end of a logical stream, and then loop `recv()` calls until we receive the character that we expect.  

For the remote dsh program we will use the null byte `\0` as our end of stream delimiters for requests sent by the dsh client to the server.  On the way back we will use the ASCII code 0x04 or EOF character to indicate the end of the stream. We defined `RDSH_EOF_CHAR` in `rshlib.h` as follows:  

```c
static const char RDSH_EOF_CHAR = 0x04;  
```

**THIS MEANS THAT YOU WILL BE RESPONSIBLE FOR CORRECTLY NOT ONLY SENDING DATA BUT ENSURING THAT THE DATA YOU SEND ENDS WITH THE APPROPRIATE END OF STREAM CHARACTER**

Some pseudo-code is below for how to handle this:

```c
//Sending a stream that is null terminated
char *cmd = "ls -l"
int  send_len = strlen(cmd) + 1;    //the +1 includes the NULL byte
int  bytes_sent;

//send the command including the null byte
bytes_sent = send(sock, cmd, send_len, 0);
```

```c
//Receiving a stream that is null terminated
char *buff;
int  recv_size;         //the +1 includes the NULL byte
int  is_last_chunk;     //boolean to see if this is the last chunk
char eof_char = '\0';   //using the null character in this demo, however
                        //you can set this to RDSH_EOF_CHAR, which is
                        //0x04, or the linux EOF character.  We define
                        //RDSH_EOF_CHAR for you in rshlib.h.  For example,
                        //if all we would need to do is to change:
                        //
                        // char eof_char = '\0'; to
                        // char eof_char = RDSH_EOF_CHAR;
                        //
                        // to handle the stream of data that the server will
                        // send back to the client.

//note that RDSH_COMM_BUFF_SZ is a constant that we provide in rshlib.h
buff = malloc(RDSH_COMM_BUFF_SZ);

//send the command including the null byte
while ((recv_size= recv(socket, buff, RDSH_COMM_BUFF_SZ,0)) > 0){
    //we got recv_size bytes
    if (recv_size < 0){
        //we got an error, handle it and break out of loop or return
        //from function
    }
    if (recv_size == 0){
        //we received zero bytes, this often happens when we are waiting for
        //the other side of the connection to send, but they close the socket
        //for now lets just assume the other side went away and break out of
        //the loop or return from the function
    }

    //At this point we have some data, lets see if this is the last chunk
    is_last_chunk = ((char)buff[recv_size-1] == eof_char) ? 1 : 0;

    if (is_last_chunk){
        buff[recv_size-1] = '\0'; //remove the marker and replace with a null
                                  //this makes string processing easier
    }

    //Now the data in buff is guaranteed to be null-terminated.  Handle in,
    //in our shell client we will just be printing it out. Note that we are
    //using a special printf marker "%.*s" that will print out the characters
    //until it encounters a null byte or prints out a max of recv_size
    //characters, whatever happens first. 
    printf("%.*s", (int)recv_size, buff);

    //If we are not at the last chunk, loop back and receive some more, if it
    //is the last chunk break out of the loop
    if (is_last_chunk)
        break;
}

//NORMAL PROCESSING CONTINUES HERE
```

In our final example, we will demonstrate how to just sent the `EOF` character. You will need this in your server.  Your server will do the traditional `fork/exec` pattern and you will `dup` `stdin`, `stdout`, and `stderr` to the socket you are using for communications.  This means that everything you `exec` will go back over the socket to the client until the command finishes executing.  However, the client will not know that the server is done until the server does one final send, sending the `EOF` character.  Remember this character is defined in `rshlib.h` as RDSH_EOF_CHAR.

```c
//Demo function to just send the eof
int send_eof(int socket){
    int bytes_sent;

    //send one character, the EOF character.
    bytes_sent = send(socket, &RDSH_EOF_CHAR, 1, 0);
    if (bytes_sent == 1){
        //this is what is expected, we sent a single character,
        //the EOF character, so we can return a good error code.
        //we use OK for this as defined in dshlib.h
        return OK;
    }
    

    //handle error and send back an appropriate error code
    //if bytes_sent < 0 that would indicate a network error
    //if it equals zero it indicates the character could not
    //be sent, which is also an error.  I could not imagine a
    //situation where bytes_sent > 1 since we told send to 
    //send exactly one byte, but if this happens it would also
    //be an error.

    //Ill just return a generic COMMUNICATION error we defined
    //for you in rshlib.h, but you can return different error
    //codes for different conditions if you want. 
    return ERR_RDSH_COMMUNICATION;
}
```

#### Directions

Now that we have provided an overview of what we are looking for we can turn to specific directions for this assignment including an overview of the starter code that we provide.

Files provided in the starter:

| File | Description |
| --- | --- |
| dsh_cli.c | You should not need to modify this file but you should understand it.  It accepts command line args to start `dsh` in stand-alone mode, client-mode, or server-mode. |
| dshlib.h | This file is largely unchanged from the last assignment.  The shell prompt has been bumped to `dsh4>` |
| dshlib.c | We provide an empty starter for this file.  This file should be replaced with the code you wrote in `5-ShellP3`. It assumes that local shell execution runs via the `exec_local_cmd_loop()` command. No other changes are required - reuse the submission of your last assignment as-is!|
| rshlib.h | This header file shows key constants and prototypes needed for the remote shell implementation.  There are comments in this file to highlight what they are used for |
| rsh_cli.c | The implementation of the remote shell client.  The client should start executing via the `exec_remote_cmd_loop(server_ip_address, port)` function. There is significant documentation in the starter to get you going. |
| rsh_server.c | The implementation of the remote shell server.  The server should start executing via the `start_server(server_interfaces, port)` function. There is significant documentation in the starter to get you going. |

This version of `dsh` has the following options, which can be viewed by executing `dsh -h`

```bash
Usage: ./dsh [-c | -s] [-i IP] [-p PORT] [-x] [-h]
  Default is to run ./dsh in local mode
  -c            Run as client
  -s            Run as server
  -i IP         Set IP/Interface address (only valid with -c or -s)
  -p PORT       Set port number (only valid with -c or -s)
  -x            Enable threaded mode (only valid with -s)
  -h            Show this help message
  ```
  The defaults for the interfaces to bind to on the server, the server IP address and the port number are specified in the `rshlib.h` file.  Note these might require adjustments as there is only a single port 1234 and only one student can use this port at a time.  As shown above you can adjust the port numbers and other defaults using the `-i` and `-p` command line options.  

```c
#define RDSH_DEF_PORT           1234        //Default port #
#define RDSH_DEF_SVR_INTFACE    "0.0.0.0"   //Default start all interfaces
#define RDSH_DEF_CLI_CONNECT    "127.0.0.1" //Default server is running on
                                            //localhost 127.0.0.1
```

For this assignment we are providing a lot of leadway for you to modify the design of the starter if it makes your life easier. Below we provide the high level flow of the functions provided by the starter, but **you are free to change any of these things if you desire**.

**Main Start Code Flow**
From `dsh_cli.c`:
```
┌─────────────────────────────┐
│ENTRY-POINT:                 │
│main()                       │
│  mode = initParams()        │
│  switch mode:               │
│    case LCLI                │
│      exec_local_cmd_loop()  │
│    case SCLI                │
│      exec_remote_cmd_loop() │
│    case SSVR                │
│       start_server()        │
└─────────────────────────────┘
```
**Client Starter Code Flow**
From `rsh_cli.c`:
```
┌────────────────────────┐                        
│ENTRY-POINT:            │      ┌────────────────┐
│exec_remote_cmd_loop(){ │      │start_client(){ │
│                        │      │  socket()      │
│  start_client()        ├─────▶│  connect()     │
│                        │      │}               │
│  loop:                 │      └────────────────┘
│    print_prompt        │                        
│    send-request        │                        
│    recv-response       │     ┌─────────────────┐
│                        │     │client_cleanup(){│
│  client-cleanup()      ├────▶│   free buffers  │
│}                       │     │   close socket  │
│                        │     │}                │
└────────────────────────┘     └─────────────────┘
```

**Server Starter Code Flow**
From `rsh_server.c`:
```
                                                             ┌───────────────────────────┐   
                              ┌────────────────────────┐     │exec_cli_requests(){       │   
                              │boot_server(){          │     │ allocate-recv-buffer      │   
                              │  sock = socket()       │     │ loop:                     │   
                              │    bind()              │     │   recv()                  │   
             ┌───────────────▶│    listen()            │     │   build_cmd_list()        │   
             │                │  return sock;          │     │   rsh_execute_pipeline()  │   
             │                │}                       │     │   send_msg_eof()          │   
             │                └────────────────────────┘  ┌─▶│                           │──┐
             │                ┌────────────────────────┐  │  │   cmd==exit break         │  │
┌────────────────────────┐    │process_cli_requests(){ │  │  │   cmd==stop-server break  │  │
│ENTRY-POINT:            │    │ loop:                  │  │  │                           │  │
│start-server(){         │    │  accept()              │  │  │ free-recv-buffer          │  │
│  boot_server()         │    │  rc=exec_cli_requests()│  │  │ close socket              │  │
│  process_cli_requests()│───▶│                        │──┘  │}                          │  │
│  stop_server()         │    │  if rc==OK_EXIT        │     └───────────────────────────┘  │
│}                       │    │    break loop          │                                    │
└────────────────────────┘    │}                       │     ┌───────────────────────────┐  │
             │                └────────────────────────┘     │rsh-execute-pipeline(){    │  │
             │                 ┌───────────────────────┐     │ modified execute pipeline │  │
             │                 │stop_server(){         │     │ dup stdin, stdout, stderr │  │
             └────────────────▶│  close-server-sock    │     │ file descriptors to the   │◀─┘
                               │}                      │     │ client connection socket  │   
                               └───────────────────────┘     │}                          │   
                                                             └───────────────────────────┘   
```
#### Approaching this assignment

You are free to approach this assignment anyway you choose.  The advice here is how we did it in the solution, and represents good engineering practice of iterative software development. 

1.  Start by implementing the server.  Shell out a minimal implementation in `rsh_server.c` using the provided arguements from `dsh_cli.c`.  The goal at this point is to be able to start a socket server, accept client connections and properly send and receive messages.  Do not attempt to process any messages at this point.  Build the initial version to accept commands from the client (that are expected to be null `\0` terminated), and then simply reply by _echoing_ the this input back to the client in its expected format (messages from the server back to the client are streams of characters and terminated with the `EOF` character). Remember we provide `RDSH_EOF_CHAR` in `rshlib.h` as a `static constant char` which means you can treat it like a `#define` constant but can also use it as a variable.  In other words you can `send(socket, &RDSH_EOF_CHAR, 1, 0);

2. Next implement the basic client.  Build the logic to connect to the server with the provided arguments collected from `dsh_cli.c`. From there copy the basic command loop from your previous shell that accepts user input from the command line.  However, instead of calling any fork/exec logic, properly format the input collected from the user and send it to the server.  Remember you must send a null byte at the end as per the protocol.  Because C strings are null terminated you can easily do this by `send(socket, my_string, strlen(my_string)+1,0);`.  After sending, build a simple receive loop given that the data sent back from the server might not come all at one time.  Keep looping until the payload that you see ends with the `RDSH_EOF_CHAR` character.  We describe how to do this above.

3. Expand the rdsh_server implementation to start handling some key built-in commands.  These commands do not require any fork/exec logic.  I would recommend starting with implementing the `exit` command that simply closes the socket connection with the client and loops the server back to the next `accept()` socket API call. Correct operation would now enable a client to connect to the server, run `exit`, then allow another client to connect to the server. After that, implement the `stop-server` command that not only closes the client connection, but also gracefully shuts down the server.  This would involve freeing all allocated memory, and ultimately closing the main server socket. 

4. Implement the other required built in commands such as `cd`.  There are also other built-in commands that were extra credit earlier that you can integrate such as `dragon`.

5. Finally implement the fork/exec logic to implement remote commands.  This will largly be the same as in your previous assignment except you will be `dup2()` the `stdin` descriptor in the first process and the `stdout` and `stderr` in the last pipeline process using the socket representing the client connection.  

6. Build your tests to not only show us that your code is working, but to help you debug your code as well.  In this final assignment testing will be 100% your responsibility to implement.


#### What to hand in and Grading Rubric

Grading Rubric
- 70 points: Correct implementation of required functionality points: 
- 10 points: Code quality (how easy is your solution to follow)
- 10 points: Answering the written questions: questions.md
- 10 points: Quality and breadth of BATS unit tests
- 10 points: [EXTRA CREDIT] Make your server multi-threaded to handle concurrent client connections

Total points achievable is 110/100.