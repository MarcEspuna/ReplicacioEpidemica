#include "LayerTwoNode.h"


LayerTwoNode::~LayerTwoNode()
{
    
}


LayerTwoNode::LayerTwoNode(int id)
    : Node(id)
{}

void LayerTwoNode::Run()
{
    while (m_Running)
    {
        Sleep(1000);
        std::cout << "Node Two: Running.\n"; 
    }
}