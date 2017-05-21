// S_broadcast.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "initsock.h"
CInitSock Iyao;
int main()
{
	SOCKET  s = ::socket(AF_INET, SOCK_DGRAM, 0);
	BOOL bBroadcast = TRUE;
	::setsockopt(s, SOL_SOCKET, SO_BROADCAST, (char*)&bBroadcast, sizeof(BOOL));
	SOCKADDR_IN bcast;
	bcast.sin_family = AF_INET;
	bcast.sin_addr.s_addr = INADDR_BROADCAST;
	bcast.sin_port = htons(8888);

	printf("broadcasting...");
	char sz[] = "521  happy \r\n";
	while (TRUE)
	{
		::sendto(s, sz, strlen(sz), 0, (sockaddr*)&bcast, sizeof(bcast));
		::Sleep(5000);
		printf("521  happy");
	}
    return 0;
}

