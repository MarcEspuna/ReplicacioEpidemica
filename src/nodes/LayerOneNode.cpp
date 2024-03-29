#include "nodes/LayerOneNode.h"
#include "LayerOneNode.h"

LayerOneNode::~LayerOneNode()
{
    m_Running = false;
    m_WaitCondition.notify_all();
}


LayerOneNode::LayerOneNode(const std::string& name, int id)
    : Node(name, id)
{
    
}

// This node will broadcast an update to layer 2 every 10 seconds
void LayerOneNode::Run()
{
    LOG_INFO("Starting LayerOneNode {}", m_Id);
    while (m_Running)
    {
        LOG_WARN("LayerOneNode {} sending update to layer 2", m_Id);
        BroadcastMsg(Tag::SET, m_Transaction.GetVersion(), LAYER_TWO);       // Broadcast an update to layer 2 every 10 seconds
        
        std::unique_lock<std::mutex> lock(m_WaitMutex);
        m_WaitCondition.wait_for(lock, std::chrono::seconds(10), [this] { return !m_Running; });
    }
    LOG_INFO("LayerOneNode {} finished", m_Id);
}

void LayerOneNode::HandleMsg(int message, int src, Tag tag)
{
    LOG_INFO("LayerOneNode {} received message {} from {}", m_Id, message, src);
    switch (tag)
    {
    case Tag::READ:
    case Tag::SET:
        Node::ExecuteTransaction({(TransactionType)tag, message});
        break;
    default:
        LOG_ERROR("LayerOneNode {} received unimplemented tag. Message {} from {}", m_Id, message, src);
        break;
    }
}

void LayerOneNode::Shutdown()
{
    m_Running = false;
    m_WaitCondition.notify_all();
}
