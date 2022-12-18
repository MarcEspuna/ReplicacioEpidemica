#include "MsgHandler.h"
#include "Log.h"

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


MsgHandler::MsgHandler(int id, Node* sckManager)
 :  m_Running(true), 
    m_Id(id), m_ChildFinishes(0), m_Begin(false),
    m_SckManager(sckManager)
{     
}

MsgHandler::~MsgHandler()
{
    LOG_TRACE("MsgHandler, closing all clients");
    for (int id : m_CurrentComms)
        m_SckManager->RemoveClient(id);
    m_SckManager->RemoveClient(m_ParentId);
    for (auto& thread : threads)
        thread.wait();
    LOG_WARN("MsgHandler, End.");
}

/// @brief Single thread dedicated to each connected client
/// @param socket Actual client we are looking for
void MsgHandler::ClientService(int id, ServiceFunction handleMsg)
{
    bool connected = true;
    LOG_TRACE("ClientService started id: {}", id);
    while (connected)
    {
        switch (m_SckManager->IncommingReadFrom(id))
        {
        case 1:     // Incomming read - > <Tag:1 byte(char)> <data: 4 bytes(int)>s
            LOG_TRACE("ClientService, incomming read from {}", id);
            std::array<char, 5> data;                           // Reception buffer
            m_SckManager->ReceiveMsg(id, data);
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
    LOG_WARN("Removing client.");
    m_SckManager->RemoveClient(id);
}


void MsgHandler::BroadcastMsg(Tag tag, int msg)
{
    std::lock_guard<std::mutex> lck(mtx_DataLock);
    LOG_TRACE("Broadcasting msg tag, {}", (char)tag);
    for (int i = 0; i < m_CurrentComms.size(); i++){
        LOG_TRACE("Sending msg to {}", m_CurrentComms[i]);
        m_SckManager->SendMsg(m_CurrentComms[i], tag, msg); 
    }
}


void MsgHandler::AddClient(int id)
{
    std::lock_guard<std::mutex> lck(mtx_DataLock);
    m_CurrentComms.push_back(id);
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

void MsgHandler::EraseClient(int id)
{
    std::lock_guard<std::mutex> lock(mtx_DataLock);
    eraseFromVector(id, m_CurrentComms);
}