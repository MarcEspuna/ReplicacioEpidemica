#include "nodes/LayerTwoNode.h"
#include "LayerTwoNode.h"


LayerTwoNode::~LayerTwoNode()
{
    
}

LayerTwoNode::LayerTwoNode(const std::string& name, int id)
    : Node(name, id)
{}

void LayerTwoNode::Run()
{

}

void LayerTwoNode::HandleMsg(int message, int src, Tag tag)
{
    LOG_INFO("LayerTwoNode {} received message {} from {}", m_Id, message, src);
    switch (tag)
    {
    case Tag::READ:
    case Tag::SET:
        Node::ExecuteTransaction({(TransactionType)tag, message});
        break;
    default:
        LOG_ERROR("LayerTwoNode {} received unimplemented tag. Message {} from {}", m_Id, message, src);
        break;
    }
}