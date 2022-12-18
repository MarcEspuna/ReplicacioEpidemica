#pragma once
#include "Commons.h"
#include "Client.h"
#include "Server.h"
#include "Transaction.h"
#include "TransactionReader.h"

enum class Tag {
    REQUEST = 'R', RELEASE = 'L', OK = 'K', ACK = 'A', END='E', BEGIN='B', TERMINATE='T', READY='D'
};

class Node : public Server {
public:
    /**
     * @brief Constructor links to all the specifed portis
     * @param link integers specifiyng the ports to connect to
     */
    Node(const std::string& name, int id);
    virtual ~Node();

    /* Node management */
    void Connect(const std::vector<int>& currentLayer = {}, const std::vector<int>& nextLayer = {});
    void Start();  
    void Shutdown();
    void Wait();

    /* Sockets management */
    void SendMsg(int desc, Tag tag, int msg);
    template<typename T, int S>
    int ReceiveMsg(int src, std::array<T,S>& data);
    void RemoveClient(int id);
    int IncommingReadFrom(int id);

    /* Transactions */
    void ExecuteTransaction(TransactionData transaction);
protected:
    virtual void Run() = 0;                                         // Main run loop from the node
	virtual void IncommingConnection(SOCKET client) override;		// Incomming connection callback
protected:
    bool m_Running;                                                 // Boolean that keeps the run loop going
    int m_Id;                                                       // Id/Server port of the node

    std::unordered_map<int, Client> m_Sockets;                      // Stores all the sockets of the node
    std::vector<int> m_CurrentLayer;                                // All the socket ids of the current layer
    std::vector<int> m_NextLayer;                                   // All the socket ids of the next layer

    Transaction m_Transaction;
private:
    std::future<void> m_MainThread;                                
    std::mutex m_DataMutex;
};

template<typename T, int S>
int Node::ReceiveMsg(int src, std::array<T,S>& data) 
{ 
    assertm(m_Sockets.find(src) != m_Sockets.end(), "Socket not found!"); 
    return m_Sockets.at(src).Receive(data);    
}