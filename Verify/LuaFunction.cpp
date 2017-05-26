#include "stdafx.h"
#include "LuaFunction.h"
#include "Common.h"
#include <TlHelp32.h>
#include <string>

void Lua_ShowError(const char* error)
{
#ifdef DEBUG
	//	MessageBoxA(NULL, error, "脚本错误", 0);
	printf("Verify:error %s\n", error);
	MyDbgPrint(error);
#else
	return;
#endif
}

int Lua_DbgPrint(lua_State *L)
{
#ifdef DEBUG
	std::string strPrint = "Debug:";
	char *str;
	for (int i = 1; i <= lua_gettop(L); ++i) {
		str = (char *)lua_tostring(L, i);
		strPrint += str ? str : "nil";
		strPrint += " ";
	}
	strPrint += "\n";
	MyDbgPrint(strPrint.c_str());
	printf(strPrint.c_str());
	return 0;
#else
	return 0;
#endif
}

int Lua_SetVerifyData(lua_State *L)
{
	std::string strconsole = "verifydata:";
	const char *fname = lua_tostring(L, 1);
	size_t sz;
	const char *data = lua_tolstring(L, 2, &sz);
	strconsole += fname;
	HANDLE hTFile = CreateFile(CString(strconsole.c_str()), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwWrite;
	WriteFile(hTFile, data, sz, &dwWrite, NULL);
	CloseHandle(hTFile);
	return 0;
}

int Lua_MessageBox(lua_State *L)
{
	auto msg = lua_tostring(L, 1);
	if (msg != nullptr)
	{
		MessageBoxA(NULL, msg, "警告", MB_OK);
	}
	return 0;
}

typedef struct 
{
	DWORD pid;
	const char * wndClass;
	HWND hwnd;
}PID;

BOOL CALLBACK EnumProcessWindowsProc(HWND hwnd, LPARAM lParam)
{
	auto ppc = (PID *)lParam;
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);
	if (pid == ppc->pid)
	{
		char classname[MAX_PATH] = { 0 };
		GetClassNameA(hwnd, classname, MAX_PATH);
		if (strcmp(ppc->wndClass, classname) == 0)
		{
			ppc->hwnd = hwnd;
			return FALSE;
		}
	}
	return TRUE;
}

int Lua_GetProcessWindow(lua_State *L)
{
	PID p;
	p.pid = lua_tointeger(L, 1);
	p.wndClass = lua_tostring(L, 2);
	p.hwnd = NULL;
	EnumWindows(EnumProcessWindowsProc, (LPARAM)&p);
	if (p.hwnd == NULL)
	{
		lua_pushnil(L);
	}
	else
	{
		lua_pushlightuserdata(L, p.hwnd);
	}
	return 1;
}

int Lua_GetProceesNum(lua_State *L)
{
	__try 
	{
		auto pnum = 0;
		auto name = lua_tostring(L, 1);
		auto hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		USES_CONVERSION;
		auto wname = A2W(name);
		if (INVALID_HANDLE_VALUE != hProcessSnap)
		{
			PROCESSENTRY32 pe32;
			pe32.dwSize = sizeof(PROCESSENTRY32);

			if (Process32First(hProcessSnap, &pe32))
			{
				do
				{
					if (_tcscmp(pe32.szExeFile, wname) == 0)
						pnum++;
				} while (Process32Next(hProcessSnap, &pe32));
			}
			CloseHandle(hProcessSnap);
		}
		lua_pushinteger(L, pnum);
	}
	__except (EXCEPTION_EXECUTE_HANDLER) {
		LogError("Lua_GetProceesNum 出现异常");
		lua_pushinteger(L, 0);
	}
	return 1;
}