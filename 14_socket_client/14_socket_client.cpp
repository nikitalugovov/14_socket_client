﻿// 14_socket.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <Winsock2.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

#define PORT 4001
#define MAX_CON 1
#define BUF_SIZE 256

VOID ReportError(LPCTSTR UserMessage, DWORD ExitCode, BOOL PrintErrorMsg);
BOOL PrintMsg(HANDLE hOut, LPCTSTR pMsg);
BOOL PrintStrings(HANDLE hOut, ...);
BOOL PrintFormat(HANDLE hOut, LPCTSTR pFormat, ...);

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

VOID ReportError(LPCTSTR UserMessage, DWORD ExitCode, BOOL PrintErrorMsg)
{
	DWORD eMsgLen, LastErr = GetLastError();
	LPTSTR lpvSysMsg;
	HANDLE hStdErr = GetStdHandle(STD_ERROR_HANDLE);
	PrintMsg(hStdErr, UserMessage);
	if (PrintErrorMsg)
	{
		eMsgLen = FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL, LastErr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpvSysMsg, 0, NULL);
		MessageBox(NULL, lpvSysMsg, _T("Ошибка"), MB_OK);
		//PrintStrings(hStdErr, _T("\n"), lpvSysMsg, _T("\n"), NULL);
		LocalFree(lpvSysMsg);
	}
	if (ExitCode > 0)
	{
		getchar();
		ExitProcess(ExitCode);
	}
	else
		return;
}

BOOL PrintFormat(HANDLE hOut, LPCTSTR pFormat, ...)
{
	LPTSTR lpBuf;
	va_list vl;
	va_start(vl, pFormat);
	//_vstprintf(buf, pFormat, vl);
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_STRING,
		pFormat, 0, 0, (LPTSTR)&lpBuf, 0, &vl);
	va_end(vl);
	BOOL result = PrintMsg(hOut, lpBuf);
	LocalFree(lpBuf);
	return result;
}

BOOL PrintMsg(HANDLE hOut, LPCTSTR pMsg)
{
	return PrintStrings(hOut, pMsg, NULL);
}

BOOL PrintStrings(HANDLE hOut, ...)
{
	DWORD MsgLen, Count;
	LPCTSTR pMsg;
	va_list pMsgList;
	va_start(pMsgList, hOut);
	while ((pMsg = va_arg(pMsgList, LPCTSTR)) != NULL)
	{
		MsgLen = _tcslen(pMsg);
		if (
			!WriteConsole(hOut, pMsg, MsgLen, &Count, NULL) &&
			!WriteFile(hOut, pMsg, MsgLen * sizeof(TCHAR), &Count, NULL
			)) return FALSE;
	}
	va_end(pMsgList);
	return TRUE;
}