#include "Node.h"

Node::Node(int id, const std::vector<int>& currentLayer, const std::vector<int>& nextLayer)
    : Server(id), m_id(id), m_Running(true), m_CurrentLayer(currentLayer), m_NextLayer(nextLayer)
{
    StartConnectionHandling();
    for (auto port : currentLayer)
    {
        {
            std::lock_guard<std::mutex> lck(m_DataMutex);
            m_Sockets.try_emplace(port, port);
        }
        m_Sockets.at(port).Send(std::array<int,1>{m_id});
    }
    for (auto port : nextLayer)
    {
        {
            std::lock_guard<std::mutex> lck(m_DataMutex);
            m_Sockets.try_emplace(port, port);
        }
        m_Sockets.at(port).Send(std::array<int,1>{m_id});
    }
}

Node::~Node()
{
    
}

void Node::Start()
{
    m_MainThread = std::async(std::launch::async, &Node::Run, this);
}

void Node::Shutdown()
{
    
    m_Running = false;
}

void Node::Wait()
{
    m_MainThread.wait();
}

/// @brief Callback when clients are connecting to the node
/// @param client 
void Node::IncommingConnection(SOCKET client)
{
    std::array<int,1> port;
    Socket::Receive(client, port);
    {
        std::lock_guard<std::mutex> lck(m_DataMutex);
        m_Sockets.try_emplace(port[0], client);
        m_CurrentLayer.push_back(port[0]);
    }
}