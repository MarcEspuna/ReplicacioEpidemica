#include "nodes/CoreNode.h"


CoreNode::~CoreNode()
{
    
}

CoreNode::CoreNode(const std::string& name, int id)
    : Node(name, id), mtx_Lamport(id, this)
{
    
}

void CoreNode::Run()
{
    /* Add clients to mtx lamport */
    for (int id : m_CurrentLayer){
        mtx_Lamport.AddClient(id);
        mtx_Lamport.StartClientService(id);
    }
}

void CoreNode::ExecuteTransaction(TransactionData transaction)
{
    mtx_Lamport.requestCS();
    
    /* Broadcast to current layer */
    for (int dest : m_CurrentLayer)
        SendMsg(dest, (Tag)transaction.type, transaction.value);

    /* Execute transaction */
    Node::ExecuteTransaction(transaction);
    
    /* Wait for transactions from other nodes to finish */
    // mtx_Lamport.WaitNodes();

    mtx_Lamport.releaseCS();
}