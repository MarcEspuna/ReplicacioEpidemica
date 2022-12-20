#pragma once
#include "Commons.h"
#include "sockets/Server.h"
#include "sockets/Client.h"
#include "sockets/Socket.h"

// Macros
#define BIND_CALLBACK(fn) std::bind(&fn, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
#define NUM_LAYERS 3
#define CORE_LAYER 0
#define LAYER_ONE 1
#define LAYER_TWO 2

// Enums
enum class Tag {
    REQUEST = 'R', 
    RELEASE = 'L', 
    OK = 'K', 
    ACK = 'A', 
    END = 'E', 
    BEGIN = 'B', 
    TERMINATE = 'T', 
    READY = 'D',
    SUM = '+', 
    SUBSTRACT = '-', 
    MULTIPLY = '*', 
    SET='s',
    READ = 'r', 
    UNKOWN = '5'
};

// Type aliases
using ServiceFunction = std::function<void(int, int, Tag)>;

// MsgHandler is a class that handles all the communication between nodes
class MsgHandler : public Server {
public:
    MsgHandler() = delete;
    MsgHandler(int id);
    virtual ~MsgHandler();

    // Sockets communications
    void SendMsg(int desc, Tag tag, int msg);
    template<typename T, int S>
    int ReceiveMsg(int src, std::array<T,S>& data);
    void RemoveClient(int id);
    int IncommingReadFrom(int id);
    void BroadcastMsg(Tag tag, int msg, int layer);

    // Connection management
    void Connect(const std::vector<int>& coreLayer = {}, 
                 const std::vector<int>& layerOne = {}, 
                 const std::vector<int>& layerTwo = {});

    // Getters
    std::vector<int> GetLayer(int layer) const;

    // Server overrides
    virtual void IncommingConnection(SOCKET client) override;

    // Abstract function
    virtual void HandleMsg(int message, int src, Tag tag) = 0;          // Function that gets called every time a message is received
protected:
    // Member variables
    int m_Id;                                                           // Owr id                      
    std::array<std::vector<int>,NUM_LAYERS> m_Layers;                   // Stores all the socket ids of each level
    std::unordered_map<int, Client> m_Sockets;                          // Stores all the sockets of the class    
private:
    // Member variables
    std::vector<std::future<void>> threads;                             // Threads for each client
    
    // Private functions
    void ClientService(int id, ServiceFunction callback);
    void EraseClient(int id);
    void StartClientService(int port);

    // Mutexes
    std::mutex mtx_CallbackWait;                // Used to sincronize the MsgHandler callback    
    std::mutex mtx_Data;                        // Making data structures threadsafe 
};

template<typename T, int S>
int MsgHandler::ReceiveMsg(int src, std::array<T,S>& data) 
{ 
    if (m_Sockets.find(src) != m_Sockets.end())
        return m_Sockets.at(src).Receive(data);    
    data = {};
    LOG_CRITICAL("Receive msg, socket not found.");
    return -1;
}