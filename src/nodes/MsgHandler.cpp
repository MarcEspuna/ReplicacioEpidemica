#include "nodes/MsgHandler.h"
#include "Log.h"
#include "MsgHandler.h"

template<typename T>
static bool eraseFromVector(T toErase, std::vector<T>& vec)
{
    auto it = std::find(vec.begin(), vec.end(), toErase);
    if (it != vec.end())
    {
        vec.erase(it);
        return true;
    }   
    return false;
}


MsgHandler::MsgHandler(int id)
 : Server(id), m_Id(id)
{     
}

MsgHandler::~MsgHandler()
{
    LOG_TRACE("MsgHandler, closing all clients");
    m_Sockets.clear();                              // Close all clients
    for (auto& thread : threads)                    // Wait for all client threads to finish
        thread.wait();
    LOG_WARN("MsgHandler, End.");
}

/// @brief Single thread dedicated to each connected client
/// @param socket Actual client we are looking for
void MsgHandler::ClientService(int id, ServiceFunction handleMsg)
{
    bool connected = true;
    LOG_TRACE("ClientService started, my id, {} service id, {}", m_Id, id);
    while (connected)
    {
        switch (IncommingReadFrom(id))
        {
        case 1:     // Incomming read - > <Tag:1 byte(char)> <data: 4 bytes(int)>s
            LOG_TRACE("ClientService, incomming read from {}", id);
            std::array<char, 5> data;                           // Reception buffer
            ReceiveMsg(id, data);
            {
                std::lock_guard<std::mutex> lck(mtx_CallbackWait);
                handleMsg(*(int*)&data[1], id, (Tag)data[0]);   // Handle message callback
            }
            break;
        case 0:     // Reception closed
            connected = false;
            LOG_WARN("Client disconnected, id: {}, my id: {}", id, m_Id);
            break;
        default:
            if (WSAGetLastError() != WSAETIMEDOUT)
            {
                LOG_ERROR("Recv failed with code {}, and id, {}, my id {}", WSAGetLastError(), id, m_Id);
                connected = false;
            }
            break;
        } 
    }
    LOG_TRACE("Exiting client service");
}


/// @brief Callback when clients are connecting to the node
/// @param client 
void MsgHandler::IncommingConnection(SOCKET client)
{
    std::array<int,2> reception;
    Socket::Receive(client, reception);
    int id = reception[0];
    int level = reception[1];
    {
        std::lock_guard<std::mutex> lck(mtx_Data);
        m_Sockets.try_emplace(id, client);
        if (level > -1 && level < m_Layers.size()) {
            m_Layers[level].push_back(id);
            StartClientService(id);
        }
        else 
            LOG_ERROR("Incomming connection, level out of range.");
    }
    LOG_INFO("Incomming connection, id: {}, level: {}", id, level);
}

void MsgHandler::SendMsg(int desc, Tag tag, int msg)
{
    if (m_Sockets.find(desc) != m_Sockets.end())
    {
        std::array<char, 5> buffer;
        buffer[0] = (char)tag;
        memcpy_s(&buffer[1], sizeof(int), &msg, sizeof(int));
        m_Sockets.at(desc).Send(buffer);
        return;
    }
    LOG_CRITICAL("Send msg, socket not found!");
}

int MsgHandler::IncommingReadFrom(int id) 
{ 
    if (m_Sockets.find(id) != m_Sockets.end())
        return m_Sockets.at(id).IncommingRead(); 
    LOG_CRITICAL("Incomming read, socket not found.\n");
    return -1;
}

void MsgHandler::BroadcastMsg(Tag tag, int msg, int layer)
{
    std::lock_guard<std::mutex> lck(mtx_Data);
    if (layer < 0 || m_Layers.size() <= layer) {
        LOG_ERROR("Broadcast msg, level out of range.");
        return;
    }
    LOG_TRACE("Broadcast msg, layer: {}, layer size: {}, msg: {}", layer, m_Layers[layer].size(), msg);
    for (auto id : m_Layers[layer]) SendMsg(id, tag, msg);
}

void MsgHandler::Connect(const std::vector<int> &coreLayer, const std::vector<int> &layerOne, const std::vector<int> &layerTwo)
{
    /* Connect to current layer nodes */
    std::array<const std::vector<int>*, NUM_LAYERS> layers = { &coreLayer, &layerOne, &layerTwo };
    for (int i = 0; i < NUM_LAYERS; i++)
    {
        for (auto port : *layers[i])
        {
            {
                std::lock_guard<std::mutex> lck(mtx_Data);
                m_Sockets.try_emplace(port, port);
                m_Layers[i].push_back(port);
                StartClientService(port);
            }
            m_Sockets.at(port).Send(std::array<int, 2>{ m_Id, i });
            LOG_INFO("Connected to layer {}, id: {}", i, m_Id);
        }
    }
}

std::vector<int> MsgHandler::GetLayer(int layer) const
{
    if (layer < 0 || m_Layers.size() <= layer)
    {
        LOG_ERROR("Get layer, level out of range.");
        return {};
    }
    return m_Layers[layer];
}

void MsgHandler::StartClientService(int port)
{
    threads.push_back(std::async(
            std::launch::async,                     // Asyncrunous task
            &MsgHandler::ClientService,             // Client service function
            this,                                   // This class
            port,                                   // Use spcifed port
            BIND_CALLBACK(MsgHandler::HandleMsg))); // Execute handle msg reception
}