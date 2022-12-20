#include "nodes/CoreNode.h"
#include "CoreNode.h"


CoreNode::~CoreNode()
{
    
}

CoreNode::CoreNode(const std::string& name, int id)
    : Node(name, id), mtx_Lamport(id, this)
{
    
}

void CoreNode::Run()
{
    LOG_INFO("Node {} started", m_Id);

    for (int i = 0; i < 4; i++)
    {
        mtx_Lamport.requestCS();
        Sleep(1000);
        std::cout << "Here we will execute the transaction!\n\n";
        mtx_Lamport.releaseCS();
    }
    LOG_WARN("Node {} finished", m_Id);
}

void CoreNode::ExecuteTransaction(TransactionData transaction)
{
    mtx_Lamport.requestCS();
    
    /* Broadcast to current layer */
    for (int dest : m_Layers[CORE_LAYER])
        SendMsg(dest, (Tag)transaction.type, transaction.value);

    /* Execute transaction */
    Node::ExecuteTransaction(transaction);
    
    /* Wait for transactions from other nodes to finish */
    // mtx_Lamport.WaitNodes();

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
        
        break;
    /* Execute transaction request */
    case Tag::READ:
    case Tag::SUM:
    case Tag::SUBSTRACT:
    case Tag::MULTIPLY:
        ExecuteTransaction({(TransactionType)tag, message});  // Execute the transaction
        SendMsg(src, Tag::READY, message);                    // Transaction is done
        break;
    default:
        mtx_Lamport.HandleMsg(message, src, tag);
        break;
    }
}