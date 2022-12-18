#pragma once
#include "Commons.h"
#include "sockets/Server.h"
#include "sockets/Client.h"
#include "sockets/Socket.h"
#include "nodes/Node.h"

#define BIND_CALLBACK(fn) std::bind(&fn, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

using ServiceFunction = std::function<void(int, int, Tag)>;

class MsgHandler {
public:
    MsgHandler() = delete;
    MsgHandler(int id, Node* sckManager);
    virtual ~MsgHandler();

    void BroadcastMsg(Tag tag, int msg);

    /* Pure virtual function */
    virtual void HandleMsg(int message, int src, Tag tag) = 0;          /* Used for current level processes */
    virtual void HandleChildMsg(int message, int src, Tag tag) = 0;     /* Used for child porcesses */

    void AddClient(int id);
    void StartClientService(int port);
protected:
    int m_Id;  
    int m_ParentId;                                 // Upper level connection                        
    std::vector<int> m_CurrentComms;              // Current level socket ids

    int m_ChildFinishes;                          // Number of child processes that have finished
    bool m_Begin;

    Node* m_SckManager;
private:
    /* Incomming connections thread */
    std::vector<std::future<void>> threads;
    std::mutex mtx_DataLock;                        // For data management 
    bool m_Running;
    
private:

    /* For MshHandler syncronization */
    std::mutex mtx_CallbackWait;

    void ClientService(int id, ServiceFunction callback);
    void EraseClient(int id);
};

