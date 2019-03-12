/*
 * Author: Yusuf SEZER <yusufsezer@mail.com>
 * Copyright (c) www.yusufsezer.com
 */

#ifndef NODEV4_H
#define NODEV4_H

#include <cstring>
#include <stdexcept>

// cross platform
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
typedef int socklen_t;
static WSAData wsaData;
#else
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

namespace ysSocket {

	enum class STATE {
		OPEN,
		CLOSED
	};

	class ysNodeV4 {
	protected:
		// Socket
		int m_socketFd = -1;
		struct sockaddr_in m_socketInfo;
		int m_port = 5000;
		int m_addressLen = 0;
		int m_messageSize = 4096;
		STATE m_state;

		// socket
		void setSocketInfo();
		void createSocket();
		void sendMessage(const int& t_socketFd, const std::string& t_message);
		void closeSocket();

	public:
		ysNodeV4();
		ysNodeV4(const int& t_port);

		// Port
		int getPort() const;
		void setPort(const int& t_port);

		// Socket file descriptor
		int getSocketFd() const;
		void setSocketFd(const int& t_socketFd);

		// Socket information
		sockaddr_in getSocketInfo() const;
		void setSocketInfo(const sockaddr_in& t_socketInfo);

		// Connection status
		STATE getState() const;
		void setState(const STATE& t_state);

		// Message size
		int getMessageSize() const;
		void setMessageSize(const int& t_messageSize);

		virtual ~ysNodeV4();
	};

}

#endif /* NODEV4_H */