#pragma once
#include <string>
#include <stdexcept>

#ifdef _DEBUG
	#define _DbgPrint    MyDbgPrint
#else
	#define _DbgPrint(...)
#endif

void MyDbgPrint(const char *fmt, ...);
void MyDbgPrint(char *fmt, ...);
void MyDbgPrint(wchar_t *fmt, ...);
void LogError(const char *fmt, ...);

std::string GetRandomString(int num);

#define NT_SUCCESS(Status)          ((NTSTATUS)(Status) >= 0)
#define STATUS_SUCCESS              ((NTSTATUS)0x00000000L)
#define STATUS_INVALID_INFO_CLASS   ((NTSTATUS)0xC0000003L)
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)
#define STATUS_ACCESS_DENIED        ((NTSTATUS)0xC0000022L)
#define STATUS_DEBUGGER_INACTIVE    ((NTSTATUS)0xC0000354L)
#define STATUS_NO_YIELD_PERFORMED   ((NTSTATUS)0x40000024L)
#define ENSURE_RUNTIME(cond,errmsg) ENSURE_THROW(cond, throw std::runtime_error(errmsg))
#define ENSURE_WIN32(cond) ENSURE_THROW(cond, ThrowWin32Exception() )
#define ENSURE_SUCCEEDED(hr) \
if (SUCCEEDED(hr)) \
if (SUCCEEDED(hr)) \
else ENSURE(SUCCEEDED(hr))(Win32ErrorMessage(hr))

#define ALIGN_DOWN(length, type) \
	((ULONG)(length)& ~(sizeof(type)-1))

#define ALIGN_UP(length, type) \
	(ALIGN_DOWN(((ULONG)(length)+sizeof(type)-1), type))