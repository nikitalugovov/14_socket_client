// 14_socket.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Winsock2.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

#define PORT 4001
#define MAX_CON 1
#define BUF_SIZE 256



int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "ru_RU");
	WSADATA wsad;
	int err;
	err = WSAStartup(MAKEWORD(2, 1), &wsad);
	if (err) {
		std::cout << "Couldn't initialize sockets or something" << std::endl;
		return 1;
	}

	sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	SOCKET sid = socket(PF_INET, SOCK_STREAM, 0);
	if (sid == INVALID_SOCKET) {
		std::cout << "Invalid socket" << std::endl;
		return 1;
	}

	err = connect(sid, (sockaddr *)&addr, sizeof addr);
	if (err == SOCKET_ERROR) {
		std::cout << "Could not connect" << std::endl;
		return 2;
	}
	long x = 22;
	std::cout << "Sending " << x << std::endl;
	send(sid, (char *)&x, sizeof x, 0);
	char buf[BUF_SIZE] = { 0 };
	int n = recv(sid, buf, sizeof buf, 0);
	std::cout << "Recieved " << buf << std::endl;
	shutdown(sid, 2);
	closesocket(sid);

	WSACleanup();
	return 0;
}

