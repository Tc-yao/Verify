// C_broadcast.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "new.h"
#include <windows.h>


CInitSock theSock;
int main()
{
	SOCKET s = ::socket(AF_INET, SOCK_DGRAM,0);
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	sin.sin_port = ::ntohs(8888);
	if (::bind(s,(sockaddr*)&sin,sizeof(sin)) == SOCKET_ERROR)
	{
		printf("bind() failed \n");
		return 0;
	}
	printf("接收中……\n");
	SOCKADDR_IN addrRemote;
	int nLen = sizeof(addrRemote);
	char sz[256];
	while (TRUE)
	{
		int nRet = ::recvfrom(s, sz, 256, 0, (sockaddr*)&addrRemote, &nLen);
		if (nRet > 0)
		{
			sz[nRet] = '\0';
			printf(sz);
		}
	}
    return 0;
}

