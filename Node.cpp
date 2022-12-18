#include "Node.h"

Node::Node(const std::string& name, int id)
    : Server(id), m_Id(id), m_Running(true), m_Transaction({name, 0})
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
        m_Sockets.at(port).Send(std::array<int,1>{m_Id});
    }
    /* Connect to next layer nodes */
    for (auto port : nextLayer)
    {
        {
            std::lock_guard<std::mutex> lck(m_DataMutex);
            m_Sockets.try_emplace(port, port);
            m_NextLayer.push_back(port);
        }
        m_Sockets.at(port).Send(std::array<int,1>{m_Id});
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

void Node::SendMsg(int desc, Tag tag, int msg)
{
    //assertm(s_App->m_Sockets.find(desc) != s_App->m_Sockets.end(), "Send msg to unknown client!");
    LOG_ASSERT(m_Sockets.find(desc)!=m_Sockets.end(), "Send msg to unknown client! id: {}", desc);
    std::array<char, 5> buffer;
    buffer[0] = (char)tag;
    memcpy_s(&buffer[1], sizeof(int), &msg, sizeof(int));
    m_Sockets.at(desc).Send(buffer);
}

int Node::IncommingReadFrom(int id) 
{ 
    LOG_ASSERT(m_Sockets.find(id) != m_Sockets.end(), "Socket not found!"); 
    return m_Sockets.at(id).IncommingRead(); 
}

void Node::ExecuteTransaction(TransactionData transaction)
{
    switch (transaction.type)
    {
    case TransactionType::SUM:
        m_Transaction.Add(transaction.value);
        break;
    case TransactionType::SUBSTRACT:
        m_Transaction.Substract(transaction.value);
        break;
    case TransactionType::MULTIPLY:
        m_Transaction.Multiply(transaction.value);
        break;
    case TransactionType::READ:
        m_Transaction.Show();
        break;
    default:
        break;
    }
}

void Node::RemoveClient(int id)
{
    std::lock_guard<std::mutex> lck(m_DataMutex);
    if (m_Sockets.find(id) != m_Sockets.end()) 
        m_Sockets.erase(id);
}