#include "nodes/CoreNode.h"
#include "CoreNode.h"

#define UPDATE_COUNT 10

CoreNode::~CoreNode()
{
    
}

CoreNode::CoreNode(const std::string& name, int id)
    : Node(name, id), mtx_Lamport(id, this), m_FinishedNodes(0) , m_WriteCount(0)
{
    
}

void CoreNode::Run()
{
    LOG_INFO("Node {} started", m_Id);
    // Test code for lamport mutex
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
    if (transaction.type == TransactionType::READ){
        Node::ExecuteTransaction(transaction);
        return;
    }

    // Request critical section
    mtx_Lamport.requestCS();
    
    // Broadcast the update transaction to current layer
    for (int dest : m_Layers[CORE_LAYER])
        SendMsg(dest, (Tag)transaction.type, transaction.value);

    // Execute transaction
    Node::ExecuteTransaction(transaction);
    
    // Broadcast transaction result to the next layer(layer one) if write count is above 10
    m_WriteCount++;
    if (m_WriteCount >= UPDATE_COUNT)
    {
        LOG_TRACE("Broadcasting transaction result to layer one. Version {}", m_Transaction.GetVersion());
        BroadcastMsg(Tag::SET, m_Transaction.GetVersion(), LAYER_ONE);  // Send update to the next layer(layer one)
        m_WriteCount = 0;
    }
    
    // Wait for transactions from other nodes to finish
    std::unique_lock<std::mutex> lock(mtx_Transaction);
    cv_Transaction.wait(lock, [this] { return m_FinishedNodes == m_Layers[CORE_LAYER].size(); });
    m_FinishedNodes = 0;

    // Realease critical section
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
        Node::ExecuteTransaction({(TransactionType)tag, message});      // Proceed to execute the transaction
        break;
    case Tag::SUM:
    case Tag::SUBSTRACT:
    case Tag::MULTIPLY:
        Node::ExecuteTransaction({(TransactionType)tag, message});      // Proceed to execute the transaction
        m_WriteCount++;
        if (m_WriteCount >= 10)
        {
            BroadcastMsg(Tag::SET, m_Transaction.GetVersion(), LAYER_ONE);  // Send update to the next layer(layer one)
            m_WriteCount = 0;
        }
        SendMsg(src, Tag::READY, message);                              // Transaction is done, notify source node
        break;
    default:
        mtx_Lamport.HandleMsg(message, src, tag);
        break;
    }
}