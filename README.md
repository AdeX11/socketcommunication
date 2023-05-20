# socketcommunication
client/server application using the sockets support available in C

implementation of client program, the accompanying server program, and the application protocol that will allow both ends to communicate using netster framework.

# protocol details

Messages MUST be the only content printed to stdout or stderr and should appear alone without any special formatting.
Port numbers < 1024 are for system use; when testing you should use a port number >= 1024.

### Welcome messages
When started, the client should print Hello, I am a client (This is already part of the netster framework)

### Normal messages
The client takes characters from the user over stdin
Full messages are transmitted one line at a time
The client waits for a response before taking a new input
The client prints each message it receives from the server without 
modification.

On receipt of a message of farewell or ok, the client terminates
All messages end with LF (line feed, or \n) character.
maximum message size is 256 bytes, including the termination character.

### How to run
Implementation uses the netster framework 
https://github.iu.edu/SICE-Networks/Net-Fall22/wiki/Netster
You can run locally and globally but i include a local example

### Local example
Change directory in both terminals until your working directory is the netster/ directory.
Build your netster application after each change with make
In the server-terminal run netster with ./netster -i 0.0.0.0
In the client-terminal run netster with ./netster 127.0.0.1
Test various combinations of “hello”, “goodbye”, “exit”, and other text and compare the results to our described protocol.
Repeat the above test with different combinations of the --port flag (both same port and different ports).
Repeat 5 and 6 with the --udp flag to run using udp.





