#pragma once
#include "Commons.h"
#include "Client.h"
#include "Server.h"

enum class Tag {
    REQUEST = 'R', RELEASE = 'L', OK = 'K', ACK = 'A', END='E', BEGIN='B', TERMINATE='T', READY='D'
};


class Node : public Server {
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
    
    void SendMsg(int desc, Tag tag, int msg);
    template<typename T, int S>
    int ReceiveMsg(int src, std::array<T,S>& data);
    void RemoveClient(int id);
    int IncommingReadFrom(int id);

protected:
    virtual void Run() = 0;
	virtual void IncommingConnection(SOCKET client);				// Incomming connection callback
protected:
    bool m_Running;
    int m_Id;
    std::unordered_map<int, Client> m_Sockets;                      // Stores all the sockets of the node
    
    std::vector<int> m_CurrentLayer;                                // All the socket ids of the current layer
    std::vector<int> m_NextLayer;                                   // All the socket ids of the next layer
private:
    std::future<void> m_MainThread;
    std::mutex m_DataMutex;
};

template<typename T, int S>
int Node::ReceiveMsg(int src, std::array<T,S>& data) 
{ 
    assertm(m_Sockets.find(src) != m_Sockets.end(), "Socket not found!"); 
    return m_Sockets.at(src).Receive(data);    
}