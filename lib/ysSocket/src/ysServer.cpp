/*
 * Author: Yusuf SEZER <yusufsezer@mail.com>
 * Copyright (c) www.yusufsezer.com
 */

#include "ysServer.h"

namespace ysSocket {

	void ysServer::setDefault() {
		this->m_socketInfo.sin_addr.s_addr = htonl(INADDR_ANY);
	}

	void ysServer::showMessage(const std::string& t_message) {
		if (this->m_onMessage != nullptr) {
			std::lock_guard<std::mutex> locker(this->m_mu);
			this->m_onMessage(t_message);
		}
	}

	ysServer::ysServer() : ysNodeV4() {
		setDefault();
	}

	ysServer::ysServer(const int& t_port) : ysNodeV4(t_port) {
		setDefault();
	}

	ysServer::ysServer(const int& t_port, const int& t_connectionSize) : ysNodeV4(t_port), m_connectionSize(t_connectionSize) {
		setDefault();
	}

	void ysServer::setOnMessage(const std::function<void(const std::string&)>& t_function) {
		m_onMessage = t_function;
	}

	void ysServer::runServer() {
		this->createServer();
		this->bindServer();
		this->listenServer();
		this->handleRequest();
	}

	void ysServer::createServer() {
		const int opt = 1;
		this->createSocket();
		if (setsockopt(this->m_socketFd, SOL_SOCKET, SO_REUSEADDR, (const char*) &opt, sizeof (opt))) {
			throw std::runtime_error("setsockopt");
		}
	}

	void ysServer::bindServer() {
		if (bind(this->m_socketFd, reinterpret_cast<sockaddr*> (&this->m_socketInfo), this->m_addressLen) == -1) {
			throw std::runtime_error("Could not bind socket");
		}
	}

	void ysServer::listenServer() {
		if (listen(this->m_socketFd, this->m_connectionSize) == -1) {
			throw std::runtime_error("Could not open socket for listening");
		}
	}

	void ysServer::handleRequest() {
		showMessage("ysServer is running...");
		showMessage(std::string(inet_ntoa(this->m_socketInfo.sin_addr)) + ":" + std::to_string(ntohs(this->m_socketInfo.sin_port)));

		while (1) {
			struct sockaddr_in temp_addr;
			socklen_t temp_len = sizeof (temp_addr);
			int temp_socket = accept(this->m_socketFd, reinterpret_cast<sockaddr*> (&temp_addr), &temp_len);

			// check connection limit
			if (this->m_nodeSize + 1 > this->m_connectionSize) {
				sendMessage(temp_socket, "Server is full.");
				close(temp_socket);
				continue;
			}
			this->m_nodeSize += 1;

			// create client
			ysNodeV4 * new_client = new ysNodeV4();
			new_client->setSocketInfo(temp_addr);
			new_client->setSocketFd(temp_socket);
			new_client->setState(STATE::OPEN);

			std::string client_ip(inet_ntoa(temp_addr.sin_addr));
			std::string client_port(std::to_string(ntohs(temp_addr.sin_port)));

			// create thread
			this->v_thread.push_back(std::thread([ = ]{
				int len;
				char message_buffer[this->getMessageSize()];
				while ((len = recv(new_client->getSocketFd(), message_buffer, this->getMessageSize(), 0)) > 0) {
					message_buffer[len] = '\0';
									std::string message(client_ip + ":" + client_port + "> " + message_buffer);
									this->sendMessageAll(message, new_client->getSocketFd());
									std::memset(message_buffer, '\0', sizeof (message_buffer));
				}
				// connection closed.
				this->v_client.erase(std::remove(this->v_client.begin(), this->v_client.end(), new_client));
				this->showMessage(client_ip + ":" + client_port + " disconnected.");
			}));

			this->showMessage(client_ip + ":" + client_port + " connected.");
			this->v_client.push_back(new_client);
		}
	}

	void ysServer::sendMessageClients(const std::string& t_message) {
		for (auto &client : v_client) {
			sendMessage(client->getSocketFd(), t_message);
		}
	}

	void ysServer::sendMessageAll(const std::string& t_message, const int& t_socket) {
		for (auto &client : v_client) {
			if (client->getSocketFd() != t_socket) {
				sendMessage(client->getSocketFd(), t_message);
			}
		}
	}

	void ysServer::closeClient() {
		for (auto &client : v_client) {
			delete client;
		}
	}

	void ysServer::joinThread() {
		for (auto &thread : v_thread) {
			if (thread.joinable()) {
				thread.join();
			}
		}
	}

	void ysServer::closeServer() {
		sendMessageClients("Server closed.");
		this->closeClient();
		this->closeSocket();
		this->joinThread();
		this->v_client.clear();
		this->v_thread.clear();
	}

	ysServer::~ysServer() {
		this->closeServer();
	}

}