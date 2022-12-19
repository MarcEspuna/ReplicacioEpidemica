# Replicacio Epidemica
It's a distributed system arquitecture consisting of 7 nodes and 3 layers. 
## Build 
Build using VSCode with cmake tools. There is a provided cmake that compiles the project.
### Dependencies
It uses 2 git submodules: 
-   spdlog: For logging
-   efsw: File watching system. Used to tell the program when a file is beeing modified.

## Class descriptions

-   Server: Handles everything realted with server socket. Looks for incomming connections and calls a virtual function 
every time a socket connects to the server.
-   Msg handler: Msg handler is the one that will have all clients with the unordered map. It will have all the functions for sending messages etc. It has the following virtual functions:
    -   IncommingConnection:
    -   HandleMsg:

- The node that connects will inform the server about it's id and it's layer.

- Inheritance: Node inherits from msg handler. Msg handler inherits from server.


TODOS:
-   LamportMutex. @Todo: It will take a msg handler pointer and send messages from him
-   Refactor Msg handler to inherit from node. 
-   Add layers handling on msg handler. Add sending current layer on connection.