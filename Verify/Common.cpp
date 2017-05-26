#include "stdafx.h"
#include "Common.h"
#include <random>
#include <fstream>
#include <time.h>

#pragma comment(lib, "Iphlpapi.lib")

#ifndef _PRINTHEAD
	#define _PRINTHEAD "Verify_Demo:%s\n"
#endif

#ifndef _WPRINTHEAD
	#define _WPRINTHEAD L"Verify_Demo:%s\n"
#endif

class CWin32Excetion :public std::exception
{
public:
	CWin32Excetion(DWORD);
	~CWin32Excetion();
	const char *what() const;
private:
	char* m_lpMsgBuf;
};

const char *CWin32Excetion::what() const
{
	return m_lpMsgBuf;
}
CWin32Excetion::CWin32Excetion(DWORD dwError)
{
	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language 

		(char *)&m_lpMsgBuf,
		0,
		NULL
	);
}

CWin32Excetion::~CWin32Excetion()
{
	LocalFree(m_lpMsgBuf);
}

void MskThrowWin32Exception()
{
	DWORD dwError = GetLastError();
	if (0 != dwError)
		throw CWin32Excetion(dwError);
}


void MyPrint(char *msg)
{
	if (msg == 0) return;
	char newmsg[1024] = { 0 };
	sprintf_s(newmsg, _PRINTHEAD, msg);
	OutputDebugStringA(newmsg);
}

void MyDbgPrint(const char *fmt, ...)
{
	va_list	ap;
	char	sz[1024];

	va_start(ap, fmt);
	vsprintf(sz, fmt, ap);
	MyPrint(sz);
	va_end(ap);
}

void MyDbgPrint(char *fmt, ...)
{
	va_list	ap;
	char	sz[1024];

	va_start(ap, fmt);
	vsprintf(sz, fmt, ap);
	MyPrint(sz);
	va_end(ap);
}

void MyDbgPrint(wchar_t *fmt, ...)
{
	OutputDebugStringW(fmt);
	return;
	va_list	ap;
	wchar_t	sz[1024];

	va_start(ap, fmt);
	vswprintf_s(sz, 1024, fmt, ap);
	wsprintfW(sz, _WPRINTHEAD, sz);
	OutputDebugStringW(sz);
	va_end(ap);
}

void LogError(const char *fmt, ...)
{
#ifdef DEBUG
	auto fp = fopen("Òì³£¼ÇÂ¼.txt", "a");
	if (fp == nullptr) return;
	va_list	ap;
	char	sz[1024] = { 0 };
	va_start(ap, fmt);
	vsprintf(sz, fmt, ap);
	sz[strlen(sz)] = '\n';
	fwrite(sz, strlen(sz), 1, fp);
	fclose(fp);
	va_end(ap);
#endif
}

int GetRandomNum(int iMin, int iMax)
{
	static std::random_device rd;
	static std::mt19937 mt(rd());
	std::uniform_int_distribution<> dis(iMin, iMax);
	return dis(mt);
}

std::string GetRandomString(int num)
{
	if (num > 4095) num = 4095;
	char buff[4096];
	for (int i = 0; i < num; ++i) {
		buff[i] = GetRandomNum('A', 'z');
		if (buff[i] > 90 && buff[i] < 97) {
			buff[i] = '_';
		}
	}
	buff[num] = 0;
	return buff;
}

void ThrowWin32Exception()
{
	DWORD dwError = GetLastError();
	if (0 != dwError)
		throw CWin32Excetion(dwError);
}
