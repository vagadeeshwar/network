# Error Detection Techniques

This project consists of a TCP Server and Client implemented in C. The server and client communicate with each other and apply various error detection techniques including Checksum, Vertical Redundancy Check (VRC), Longitudinal Redundancy Check (LRC), and Cyclic Redundancy Check (CRC).

## File Structure

- `src/tcp_server.c` : Main TCP server program.
- `src/tcp_client.c` : Main TCP client program.
- `src/server_util.c` : Utility functions for the server.
- `src/client_util.c` : Utility functions for the client.
- `include/server_util.h` : Header file containing function prototypes for server utility functions.
- `include/client_util.h` : Header file containing function prototypes for client utility functions.

## Building the Project

The `src` directory contains the source code for the server and client. The server and client are intended to be compiled separately.

- To compile the server:

  ```
  gcc -I ./include -o bin/tcp_server src/server_util.c src/tcp_server.c
  ```

- To compile the client:
  ```
  gcc -I ./include -o bin/tcp_client src/client_util.c src/tcp_client.c
  ```

## Running the Server and Client

- To run the server, navigate to the `bin` directory and execute:

  ```
  ./tcp_server
  ```

- To run the client, in a new terminal window, navigate to the `bin` directory and execute:
  ```
  ./tcp_client [ip address of server]
  ```

Replace `[ip address of server]` with the actual IP address of the server machine. Use 127.0.0.1 (self loop back address) if both the server/client are running on the same computer.

## Notes

- The server and client need to be run on different terminal windows or different machines that can communicate over a network.
- The client requires the IP address of the server machine as an argument during execution.
- When choosing the error detection method on the client, the valid options are 1 for Checksum, 2 for VRC, 3 for LRC, 4 for CRC, and 5 for Exit.
