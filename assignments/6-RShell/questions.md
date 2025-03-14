# Networking and TCP Questions

## 1. How does the remote client determine when a command's output is fully received from the server, and what techniques can be used to handle partial reads or ensure complete message transmission?

The remote client determines that a command's output is fully received using one of the following techniques:
- **Explicit message boundaries**: The server appends a special delimiter (e.g., `\n` or `\0`) to indicate the end of a message.
- **Fixed-length messages**: The server sends a predefined number of bytes, and the client reads exactly that amount.
- **Length-prefixed messages**: The server first sends a header specifying the message length, and the client reads the exact number of bytes.
- **Timeouts and connection closure**: The client can assume that the message is complete if the connection is closed or after a timeout.

To handle partial reads, the client can:
- Continuously read from the socket in a loop until the expected data is received.
- Use non-blocking I/O with event-driven programming.
- Implement a buffer to store incomplete messages and assemble them when more data arrives.

## 2. Since TCP does not preserve message boundaries, how should a networked shell protocol define and detect the beginning and end of a command sent over a TCP connection? What challenges arise if this is not handled correctly?

A networked shell protocol should define message boundaries using:
- **Delimiters**: Use a special character (e.g., `\n`, `\0`) to indicate the end of a command.
- **Length-prefixing**: Include the length of the message at the beginning, so the receiver knows how many bytes to read.
- **Command-response correlation**: Ensure each request has a corresponding response, using identifiers if necessary.

Challenges if not handled correctly:
- **Message fragmentation**: A single command might be split across multiple TCP packets, causing incomplete reads.
- **Message coalescence**: Multiple commands may be combined into a single TCP packet, leading to incorrect parsing.
- **Deadlocks or hangs**: If the client or server waits indefinitely for more data, the connection may hang.

## 3. Describe the general differences between stateful and stateless protocols.

- **Stateful protocols**: Maintain session or connection state across multiple requests. Each request depends on prior interactions.
  - Example: TCP (tracks connection state, sequence numbers), FTP (maintains session state).
  - Advantages: Supports complex interactions, efficiency in long-running sessions.
  - Disadvantages: Requires more resources, challenging recovery after failure.

- **Stateless protocols**: Each request is independent, with no stored context between them.
  - Example: HTTP (each request is processed independently unless using sessions).
  - Advantages: Simpler to scale, easier to recover from failures.
  - Disadvantages: Can be inefficient, requiring extra data to be sent in each request.

## 4. Our lecture this week stated that UDP is "unreliable". If that is the case, why would we ever use it?

UDP is used despite being "unreliable" because it offers advantages over TCP in specific use cases:
- **Low latency**: No connection establishment or retransmissions, making it ideal for real-time applications (e.g., video streaming, gaming).
- **Broadcast and multicast**: Supports one-to-many communication efficiently.
- **Lightweight**: Less overhead compared to TCP, making it suitable for resource-constrained environments (e.g., IoT devices).
- **Custom reliability mechanisms**: Some applications (e.g., QUIC, VoIP) implement their own reliability mechanisms on top of UDP.

## 5. What interface/abstraction is provided by the operating system to enable applications to use network communications?

The operating system provides **sockets** as an abstraction for network communication. 
- **Socket API**: Allows applications to create, bind, listen, accept, send, and receive data over a network.
- **Types of sockets**:
  - **Stream sockets (SOCK_STREAM)**: Use TCP for reliable, ordered communication.
  - **Datagram sockets (SOCK_DGRAM)**: Use UDP for connectionless, fast transmission.
- **System calls**:
  - `socket()`, `bind()`, `listen()`, `accept()`, `connect()`, `send()`, `recv()`, `close()`.
- **Higher-level abstractions**:
  - Libraries like BSD sockets, Winsock, or Java's `java.net.Socket`.

These abstractions allow developers to write networked applications without dealing directly with low-level network protocols.
