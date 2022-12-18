#include "sockets/Socket.h"
#include <basetsd.h>
#include <mutex>
#include <stdint.h>
#include <winsock2.h>
#include "Log.h"

Socket::Socket()
	: m_Connected(false)
{
    InitSocket();
}

Socket::Socket(SOCKET socket, const sockaddr_in& details)
	: s(socket), server(details)
{}

Socket::~Socket()
{
	if (m_Connected)
	{
		gracefulClose();
		close();
		m_Connected = false;
	}
}

bool Socket::InitSocket()
{
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		LOG_ERROR("Could not create socket : {}", WSAGetLastError());
		return false;
	}
	return true;
}

void Socket::Init()
{
	WSADATA wsa;						// Winsocket
	LOG_INFO("Initialising Winsock Client...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		LOG_ERROR("Failed. Error Code : {}", WSAGetLastError());
	}
}

int Socket::IncommingRead()
{
	char check;
	return recv(s, &check, 1, MSG_PEEK);
}

void Socket::Finit()
{
	WSACleanup();
	LOG_WARN("Finalization of windows sockets.");
}

void Socket::close()
{
	if (m_Connected)
	{
		closesocket(s);
		m_Connected = false;
		LOG_WARN("Closed socket, {}", s);
	}
}

void Socket::gracefulClose() const
{
	shutdown(s, SD_SEND);
}