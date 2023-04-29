# UDP 3 Node Topology

## Prerequisites
 - C compiler (e.g. GCC)
 - Linux or Unix-based OS (e.g. Ubuntu, macOS)

## Build
To build the broadcaster, simply run the following command:

    gcc -pthread ./broadcaster.c -o broadcaster

To build the client listener program, run the following commands:
    
    gcc listener.c -o listener

## Run
To run the client listener, simply execute the listener binaries:

    ./listener <ip_address> <port> <multiple>

When a client receives a number that is multiple of \<multiple>, it sends an ACK message to the server.

You can run multiple listeners at the same time.

To run the broadcaster server, simply execute the broadcaster binary:
    
    ./broadcaster

The broadcaster sends a number starting from 1 to a random client listening on port 9435 or 9436 then increments it. It repeats this process until 100 is reached. To send the numbers again type anything in the command line then press enter.
