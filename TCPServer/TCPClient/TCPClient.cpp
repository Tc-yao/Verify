// TCPClient.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <winsock.h>  
#include <stdio.h>  

#pragma comment(lib, "ws2_32.lib") 

int main()
{
	//初始化套接字
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}

	//创建套接字
	SOCKET sListen = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sListen == INVALID_SOCKET)
	{
		printf("Failed socket() \n");
		return 0;
	}

	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(8888);
	servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	if (::connect(sListen, (sockaddr*)&servAddr, sizeof(servAddr)) == -1)
	{
		printf("Failed connect() \n");
		return 0;
	}
	char buff[256];
	int nRecv = ::recv(sListen, buff, 256, 0);
	if (nRecv > 0)
	{
		buff[nRecv] = '\0';
		printf("jieshou%s", buff);
	}
	printf("guolai\n");
	::closesocket(sListen);
	getchar(); 
	return 0;
}

