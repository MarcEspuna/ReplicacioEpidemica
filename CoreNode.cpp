#include "CoreNode.h"


CoreNode::~CoreNode()
{
    
}


CoreNode::CoreNode(int id)
    : Node(id)
{
    
}

void CoreNode::Run()
{
    while (m_Running)
    {
        std::cout << "Running.\n";
        Sleep(1000);   
    }
}