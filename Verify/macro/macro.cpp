#include "macro.h"

namespace macro
{
	ScopeGuard *g_pScopeGuard = NULL;
#ifdef _TESTVERTION//≤‚ ‘ƒ£ Ω
	void PrintMsg(__in_opt LPCWSTR lpOutputString)
	{
		if(!g_bOutString)
		{
			return;
		}
		CString strMsg = PROJECTNAMEW;
		strMsg+=lpOutputString;
		OutputDebugStringW(strMsg);
	}
	void PrintMsg(__in_opt LPCSTR lpOutputString)
	{
		if(!g_bOutString)
		{
			return;
		}
		CStringA strMsg = PROJECTNAMEA;
		strMsg+=lpOutputString;
		OutputDebugStringA(strMsg);
	}
	void OutputDebugString(__in_opt LPCWSTR lpOutputString)
	{
		if(!g_bOutString)
		{
			return;
		}
		CString strMsg = PROJECTNAMEW;
		strMsg+=lpOutputString;
		OutputDebugStringW(strMsg);
	}
	void OutputDebugString(__in_opt LPCSTR lpOutputString)
	{
		if(!g_bOutString)
		{
//			OutputDebugStringW(_T("g_bOutString Is False"));
			return;
		}
		CStringA strMsg = PROJECTNAMEA;
		strMsg+=lpOutputString;
		OutputDebugStringA(strMsg);
	}
	void OutputDebugString(__in_opt CStringW lpOutputString)
	{
		if(!g_bOutString)
		{
//			OutputDebugStringW(_T("g_bOutString Is False"));
			return;
		}
		CString strMsg = PROJECTNAMEW;
		strMsg+=lpOutputString;
		OutputDebugStringW(strMsg);
	}
	void OutputDebugString(__in_opt CStringA lpOutputString)
	{
		if(!g_bOutString)
		{
			return;
		}
		CStringA strMsg = PROJECTNAMEA;
		strMsg+=lpOutputString;
		OutputDebugStringA(strMsg);
	}
#endif
};