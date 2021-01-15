# Socket-Programming
## This folder contains solutions of CS321 socket programming assignment

The folder contains a pdf describing the problem statement and testcases.

- server1 handles single client at a time. If second client tries to connect to the server while first client if connected to the server, it will get an error.
- server2 handles multiple clients.

## Environment
- Tested on ubuntu 20.04

## To run

### For server1
Compile the code:
``` gcc server1.c -o server1```

Run the code:
``` ./server1 5000```

### For server2

Compile the code:
``` gcc server2.c -o server2```

Run the code:
``` ./server2 5000```

### For Client
Keep one of the server running(either server1 or server2), open another terminal and run the client code
Compile the code:
``` gcc client.c -o client```

Run the code:
``` ./client 127.0.0.1 5000```

