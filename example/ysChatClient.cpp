/*
 * Author: Yusuf SEZER <yusufsezer@mail.com>
 * Copyright (c) www.yusufsezer.com
 */

#include <iostream>
#include <limits>
#include <csignal>
#include "ysClient.h"

using namespace std;
using namespace ysSocket;

ysClient *chat_client = nullptr;

void signalHandler(int);
void printMessage(const string&);

int main(int argc, char** argv) {

	signal(SIGINT, signalHandler);

	string server = "localhost";
	cout << "Server: ";
	std::getline(std::cin, server);
	cout << "Port: ";
	int port = 5000;
	cin >> port;

	try {

		chat_client = new ysClient(server, port);
		chat_client->setOnMessage(printMessage);
		chat_client->connectServer();
		delete chat_client;

	} catch (const exception& e) {
		cerr << e.what() << endl;
	}

	return 0;
}

void signalHandler(int code) {
	char ch;
	cout << "Are you sure you want to close socket?(Y/N)";
	cin >> ch;
	if (toupper(ch) == 'Y' && chat_client != nullptr) {
		delete chat_client;
		exit(0);
	}
	cin.clear();
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void printMessage(const string& t_message) {
	std::cout << t_message << std::endl;
}