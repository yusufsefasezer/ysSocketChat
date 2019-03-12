/*
 * Author: Yusuf SEZER <yusufsezer@mail.com>
 * Copyright (c) www.yusufsezer.com
 */

#ifndef YSCLIENT_H
#define YSCLIENT_H

#include "ysNodeV4.h"
#include <iostream>
#include <functional>
#include <thread>
#include <mutex>

namespace ysSocket {

	class ysClient : protected ysNodeV4 {
	protected:
		void setDefault();

		// message event function
		std::function<void(const std::string& t_message) > m_onMessage = nullptr;
		void showMessage(const std::string& t_message);

		// server
		std::string m_serverName = "localhost";

		// thread
		std::thread m_thread;
		std::mutex m_mu;
		
		void _connectServer();
		void receiveMessage();
		void writeMessage();

	public:
		ysClient();
		ysClient(const int& t_port);
		ysClient(const std::string& t_serverName, const int& t_port);

		void setOnMessage(const std::function<void(const std::string&) >& t_function);

		void connectServer();
		void closeConnection();

		virtual ~ysClient();
	};

}

#endif /* YSCLIENT_H */