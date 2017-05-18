// TCPServer.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <stdio.h>
#include <winsock.h>
#pragma comment(lib,"ws2_32.lib")

int main()
{
	//��ʼ���׽���
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion,&data)!=0)
	{
		return 0;
	}

	//�����׽���
	SOCKET sListen = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sListen == INVALID_SOCKET)
	{
		printf("Failed socket() \n");
		return 0;
	}

	//���sockaddr_in�ṹ
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(8888);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;

	//������׽��ֵ�һ�����ص�ַ
	if (::bind(sListen,(LPSOCKADDR)&sin,sizeof(sin)) == SOCKET_ERROR)
	{
		printf("Failed bind() \n");
	}

	//�������ģʽ
	if (::listen(sListen,2) == SOCKET_ERROR)
	{
		printf("Failed listen() \n");
		return 0;
	}

	//ѭ�����ܿͻ�����������
	sockaddr_in remoteAddr;//����
	int nAddrLen = sizeof(remoteAddr);
	SOCKET sClient;
	char szText[] = "TCP Server ,hello Tc! \r\n";
	while (TRUE)
	{
		sClient = ::accept(sListen, (SOCKADDR*)&remoteAddr, &nAddrLen);//��ס
		if (sClient == INVALID_SOCKET)
		{
			printf("Failed accept()");
			continue;
		}
		printf("���ܵ�һ�����ӣ�%s  \r\n", inet_ntoa(remoteAddr.sin_addr));

		//��ͻ��˷�������
		::send(sClient, szText, strlen(szText), 0);
		//�ر�ͬ�ͻ��˵�����
		::closesocket(sClient);
	}
	//�رռ����׽���
	::closesocket(sListen);

	getchar();
    return 0;
}

