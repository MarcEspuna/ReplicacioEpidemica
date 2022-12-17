#include "Node.h"

Node::Node(int id)
    : Server(id), m_id(id), m_Running(true)
{
    StartConnectionHandling();
}

Node::~Node()
{
    
}

void Node::Connect(const std::vector<int>& currentLayer, const std::vector<int>& nextLayer)
{
    /* Connect to current layer nodes */
    for (auto port : currentLayer)
    {
        {
            std::lock_guard<std::mutex> lck(m_DataMutex);
            m_Sockets.try_emplace(port, port);
            m_CurrentLayer.push_back(port);
        }
        m_Sockets.at(port).Send(std::array<int,1>{m_id});
    }
    /* Connect to next layer nodes */
    for (auto port : nextLayer)
    {
        {
            std::lock_guard<std::mutex> lck(m_DataMutex);
            m_Sockets.try_emplace(port, port);
            m_NextLayer.push_back(port);
        }
        m_Sockets.at(port).Send(std::array<int,1>{m_id});
    }
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