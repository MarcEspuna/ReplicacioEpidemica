#include "sockets/Server.h"
#include <iostream>
#include "Log.h"

Server::Server() {}

Server::Server(int port)
	: m_Port(port)
{
	Bind(port);	
}

Server::~Server()
{ 
	gracefulClose();
	close();
	m_Conectivity.wait();
}

SOCKET Server::AcceptClient() const
{
	sockaddr_in cl;						// Struct used for client socket address
	int c = sizeof(struct sockaddr_in);
	SOCKET new_socket = accept(s, (struct sockaddr*)&cl, &c);
	if (new_socket == INVALID_SOCKET)
	{
		int error = WSAGetLastError();
		switch (error)
		{
		case WSAEINTR:
			LOG_WARN("Interruption function call, stopping listening for connections...");
			break;
		default:
			LOG_ERROR("Accept failed with error code: {}", error);
			break;
		}
		return 0;
	}
	LOG_INFO("Connection accepted.");
	return new_socket;
}

void Server::Bind(const unsigned int& port, const unsigned long& address)
{
	m_Port = port;
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = address;
	server.sin_port = htons(port);

	//Bind
	if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		LOG_ERROR("Bind failed, port {}, error code : {}", port, WSAGetLastError());
		assertm(false, "Failed to bind.");
		return;
	}
	m_Connected = true;
	LOG_INFO("Bind done.");
}

void Server::StartConnectionHandling()
{	
	m_Conectivity = std::async(std::launch::async, &Server::IncommingConnectionsHandler, this);
}

void Server::ListenConn(const unsigned int& connectionCount) const
{
	listen(s, connectionCount);
}

void Server::IncommingConnectionsHandler()
{
	while(m_Connected)
	{
		ListenConn(1);
		SOCKET client = AcceptClient();
		if (client)	    IncommingConnection(client);
		else m_Connected = false;
	}
	LOG_INFO("Id, {} exiting incomming connections", m_Port);
}