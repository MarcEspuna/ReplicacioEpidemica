#include "nodes/CoreNode.h"
#include "CoreNode.h"


CoreNode::~CoreNode()
{
    
}

CoreNode::CoreNode(const std::string& name, int id)
    : Node(name, id), mtx_Lamport(id, this), m_FinishedNodes(0) 
{
    
}

void CoreNode::Run()
{
    LOG_INFO("Node {} started", m_Id);
    /*
    for (int i = 0; i < 4; i++)
    {
        mtx_Lamport.requestCS();
        Sleep(1000);
        std::cout << "Here we will execute the transaction!\n\n";
        mtx_Lamport.releaseCS();
    }
    */
    LOG_WARN("Node {} finished", m_Id);
}

/**
 * @brief Execute transaction and broadcast it to the current layer
 * @note This function only gets called from file system input
 * @param transaction Transaction to execute
 */
void CoreNode::ExecuteTransaction(TransactionData transaction)
{
    mtx_Lamport.requestCS();
    
    // Broadcast the update transaction to current layer
    if (transaction.type != TransactionType::READ)
    {
        for (int dest : m_Layers[CORE_LAYER])
            SendMsg(dest, (Tag)transaction.type, transaction.value);
    }

    // Execute transaction
    Node::ExecuteTransaction(transaction);
    
    // Wait for transactions from other nodes to finish
    std::unique_lock<std::mutex> lock(mtx_Transaction);
    cv_Transaction.wait(lock, [this] { return m_FinishedNodes == m_Layers[CORE_LAYER].size(); });
    m_FinishedNodes = 0;

    mtx_Lamport.releaseCS();
}

/**
 * @brief Handle incomming message from other nodes. It will be called by the connection handler thread.
 * 
 * @param message Message content
 * @param src Id of the incoming message node
 * @param tag Incomming message tag
 */
void CoreNode::HandleMsg(int message, int src, Tag tag)
{
    LOG_TRACE("Message, tag: {}, ticks {}, from {}", (char)tag, message, src);
    /* Transactions */
    switch (tag)
    {
    case Tag::READY:        // Node finished transaction        
        m_FinishedNodes++;
        cv_Transaction.notify_one();
        break;
    /* Execute transaction requests */
    case Tag::READ:
    case Tag::SUM:
    case Tag::SUBSTRACT:
    case Tag::MULTIPLY:
        Node::ExecuteTransaction({(TransactionType)tag, message});  // Proceed to execute the transaction
        SendMsg(src, Tag::READY, message);                          // Transaction is done, notify source node
        break;
    default:
        mtx_Lamport.HandleMsg(message, src, tag);
        break;
    }
}