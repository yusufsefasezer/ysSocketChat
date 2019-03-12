/*
 * Author: Yusuf SEZER <yusufsezer@mail.com>
 * Copyright (c) www.yusufsezer.com
 */

#include <iostream>
#include <limits>
#include <csignal>
#include "ysServer.h"

using namespace std;
using namespace ysSocket;

ysServer *chat_server = nullptr;

void signalHandler(int);
void printMessage(const string&);

int main(int argc, char** argv) {

	signal(SIGINT, signalHandler);

	int port = 5000;
	cout << "Port: ";
	cin >> port;
	cout << "Connection: ";
	int connection_size = 16;
	cin >> connection_size;

	try {

		chat_server = new ysServer(port, connection_size);
		chat_server->setOnMessage(printMessage);
		chat_server->runServer();
		delete chat_server;

	} catch (const exception& e) {
		cerr << e.what() << endl;
	}

	return 0;
}

void signalHandler(int code) {
	char ch;
	cout << "Are you sure you want to close socket?(Y/N)";
	cin >> ch;
	if (toupper(ch) == 'Y' && chat_server != nullptr) {
		delete chat_server;
		exit(0);
	}
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void printMessage(const string& t_message) {
	std::cout << t_message << std::endl;
}