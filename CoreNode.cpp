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
    /* Add clients to mtx lamport */
    for (int id : m_CurrentLayer){
        mtx_Lamport.AddClient(id);
        mtx_Lamport.StartClientService(id);
    }
    
    while (m_Running)
    {
        mtx_Lamport.requestCS();
        Sleep(1000);   
        std::cout << "My turn!\n";
        mtx_Lamport.releaseCS();
    }
}