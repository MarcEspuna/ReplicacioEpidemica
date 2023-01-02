# Replicacio Epidemica
It's a distributed system arquitecture consisting of 7 nodes and 3 layers. It uses Epidemic replication between the layers to persist and backup the data. 

## Platform
Currently only supports **Windows** OS. 

## Build 
The program build sistem has been tested using Visual Studio Code with CMake Tools extension as well as CLion IDE. In each case the CMake of the main project was enough to correctly configure and build the program.

<ins>It has only been tested using **MSVC** compiler.</ins>

## Dependencies
It uses 2 git submodules: 
-   <ins>*Spdlog*</ins>: For console logging and file logging. At runtime, a directory called logs will be generated where all the node logs will be stored as the program executes. 
-   <ins>*Efsw*</ins>: File watching system. Used to tell the program when a file is beeing modified. The program applies a *Watch* on the ClientTransactions folder. In this folder you can submit read or write requests to any of the *Core Layer* nodes.

## Usage
If the user wishes to real-time monitor all the nodes using a visual interface, make sure first run the node monitor program before running this project as only on initialization tries to connect to the web socket server. Having the websocket server is not a mandatory requirement. 

Once the program is running you can submit read or write operations on any node of the core layer by writing the commands in the folders located in ClientTransactions.txt and saving the file (ctrl + s). At the moment of saving the file, the filewatch sistem will trigger an interrupt and will execute the command on the specified node. Write operations are only perimitted on Core Nodes. Read only operations are performed on the entire layer. 

Each node will generate a log of all the transactions performed and will be placed in the log folder. If that folder doesn't exist, it will be automatically created. 

## Notes
You can find the node monitor project on the following github repository: 
-   [Node Monitor](https://github.com/Singu99/Sessio4-Server-App)
