#pragma once
#include "Commons.h"
#include "Client.h"
#include "Server.h"



class Node : public Server{
public:
    /**
     * @brief Constructor links to all the specifed portis
     * @param link integers specifiyng the ports to connect to
     */
    Node(int id);
    virtual ~Node();

    void Connect(const std::vector<int>& currentLayer = {}, const std::vector<int>& nextLayer = {});
    void Start();  
    void Shutdown();
    void Wait();
    
protected:
    virtual void Run() = 0;
	virtual void IncommingConnection(SOCKET client);				// Incomming connection callback
protected:
    bool m_Running;
    int m_id;
    std::unordered_map<int, Client> m_Sockets;                      // Stores all the sockets of the node
    
    std::vector<int> m_CurrentLayer;                                // All the socket ids of the current layer
    std::vector<int> m_NextLayer;                                   // All the socket ids of the next layer
private:
    std::future<void> m_MainThread;
    std::mutex m_DataMutex;
};