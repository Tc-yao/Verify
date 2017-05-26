#pragma once
#include <windows.h>
#include <TlHelp32.h>

// #ifdef DEBUG
// 	#define fCreateFileW  CreateFileW
// 	#define fReadFile  ReadFile
// 	#define fWriteFile  WriteFile
// 	#define fCloseHandle  CloseHandle
// 	#define fGetFileSize  GetFileSize
// #else
// 	extern decltype(CreateFileW)* fCreateFileW;
// 	extern decltype(ReadFile)* fReadFile;
// 	extern decltype(WriteFile)* fWriteFile;
// 	extern decltype(CloseHandle)* fCloseHandle;
// 	extern decltype(GetFileSize)* fGetFileSize;
// #endif

#if 1
	extern decltype(CreateFileW)* fCreateFileW;
	extern decltype(ReadFile)* fReadFile;
	extern decltype(WriteFile)* fWriteFile;
	extern decltype(CloseHandle)* fCloseHandle;
	extern decltype(GetFileSize)* fGetFileSize;
	extern decltype(CreateThread)* fCreateThread;
#endif


BOOL LaunchDll(char *strName,DWORD dwReason);