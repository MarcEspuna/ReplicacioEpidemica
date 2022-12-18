#include "LayerTwoNode.h"


LayerTwoNode::~LayerTwoNode()
{
    
}


LayerTwoNode::LayerTwoNode(const std::string& name, int id)
    : Node(name, id)
{}

void LayerTwoNode::Run()
{
    while (m_Running)
    {
        Sleep(1000);
        std::cout << "Node Two: Running.\n"; 
    }
}