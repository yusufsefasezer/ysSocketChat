/*
 * Author: Yusuf SEZER <yusufsezer@mail.com>
 * Copyright (c) www.yusufsezer.com
 */

#include "ysClient.h"

namespace ysSocket {

	void ysClient::setDefault() {
		inet_pton(AF_INET, this->m_serverName.c_str(), &this->m_socketInfo.sin_addr);
	}

	void ysClient::showMessage(const std::string& t_message) {
		if (this->m_onMessage != nullptr) {
			this->m_onMessage(t_message);
		}
	}

	void ysClient::_connectServer() {
		this->createSocket();

		if (connect(this->m_socketFd, reinterpret_cast<sockaddr*> (&this->m_socketInfo), this->m_addressLen) == -1) {
			throw std::runtime_error("could not connect to server");
		}
	}

	void ysClient::receiveMessage() {
		this->m_thread = std::move(std::thread([ = ]{
			int len;
			char message_buffer[this->getMessageSize()];

			while ((len = recv(this->m_socketFd, message_buffer, this->getMessageSize(), 0)) > 0) {
				message_buffer[len] = '\0';
								showMessage(message_buffer);
								std::memset(message_buffer, '\0', sizeof (message_buffer));
			}
			this->m_state = STATE::CLOSED;
		}));
	}

	void ysClient::writeMessage() {
		std::string message = "";
		while (this->m_state == STATE::OPEN) {
			std::getline(std::cin, message);
			this->sendMessage(this->m_socketFd, message);
		}
	}

	ysClient::ysClient() :
	ysNodeV4() {
		setDefault();
	}

	ysClient::ysClient(const int& t_port) :
	ysNodeV4(t_port) {
		setDefault();
	}

	ysClient::ysClient(const std::string& t_serverName, const int& t_port) :
	ysNodeV4(t_port), m_serverName(t_serverName) {
		setDefault();
	}

	void ysClient::setOnMessage(const std::function<void(const std::string&)>& t_function) {
		m_onMessage = t_function;
	}

	void ysClient::connectServer() {
		this->_connectServer();
		showMessage("Connection successfully....");
		this->receiveMessage();
		this->writeMessage();
	}

	void ysClient::closeConnection() {
		this->closeSocket();
		if (this->m_thread.joinable()) {
			this->m_thread.join();
		}
	}

	ysClient::~ysClient() {
		this->closeConnection();
	}

}