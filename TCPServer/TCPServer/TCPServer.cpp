// TCPServer.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <winsock.h>
#pragma comment(lib,"ws2_32.lib")

int main()
{
	//初始化套接字
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion,&data)!=0)
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

	//填充sockaddr_in结构
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;

	//绑定这个套接字到一个本地地址
	if (::bind(sListen,(LPSOCKADDR)&sin,sizeof(sin)) == SOCKET_ERROR)
	{
		printf("Failed bind() \n");
	}

	//进入监听模式
	if (::listen(sListen,2) == SOCKET_ERROR)
	{
		printf("Failed listen() \n");
		return 0;
	}

	//循环接受客户的连接请求
	sockaddr_in remoteAddr;//容器
	int nAddrLen = sizeof(remoteAddr);
	SOCKET sClient;
	char szText[] = "TCP Server ,hello Tc! \r\n";
	while (TRUE)
	{
		sClient = ::accept(sListen, (SOCKADDR*)&remoteAddr, &nAddrLen);//卡住
		if (sClient == INVALID_SOCKET)
		{
			printf("Failed accept()");
			continue;
		}
		printf("接受到一个连接：%s  \r\n", inet_ntoa(remoteAddr.sin_addr));

		//向客户端发送数据
		::send(sClient, szText, strlen(szText), 0);
		//关闭同客户端的连接
		::closesocket(sClient);
	}
	//关闭监听套接字
	::closesocket(sListen);

	getchar();
    return 0;
}

