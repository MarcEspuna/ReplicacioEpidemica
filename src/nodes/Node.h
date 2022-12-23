#pragma once
#include "Commons.h"
#include "nodes/MsgHandler.h"
#include "sockets/Client.h"
#include "sockets/Server.h"
#include "transactions/Transaction.h"
#include "transactions/TransactionReader.h"
#include "web_socket/WsClient.h"

// Node abstract class is the base class for all nodes in the system
class Node : public MsgHandler {
public:
    /**
     * @brief Constructor links to all the specifed portis
     * @param link integers specifiyng the ports to connect to
     */
    Node(const std::string& name, int id);
    virtual ~Node();

    /* Node management */
    void Start();  
    void Shutdown();
    void Wait() const;

    /* Transactions */
    virtual void ExecuteTransaction(TransactionData transaction);
protected:
    virtual void Run() = 0;                                         // Main run loop from the node
protected:
    bool m_Running;                                                 // Boolean that keeps the run loop going
    Transaction m_Transaction;
    WebSocketClient m_WsClient;
private:
    std::future<void> m_MainThread;                                
    std::mutex m_DataMutex;
};