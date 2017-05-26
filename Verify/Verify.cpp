
// Verify.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "Verify.h"
#include "VerifyDlg.h"
#include "Common.h"
#include "nomodule/LoadPE.h"
#include "LuaFunction.h"
extern "C"
{
#include "nomodule/MemoryModule.h"

}
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <memory>

// CVerifyApp

BEGIN_MESSAGE_MAP(CVerifyApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CVerifyApp 构造

CVerifyApp::CVerifyApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CVerifyApp 对象

CVerifyApp theApp;


// CVerifyApp 初始化

BOOL CVerifyApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	if (!AfxOleInit())
	{
		//AfxMessageBox(IDP_OLE_INIT_FAILED);// 初始化OLE失败
		return FALSE;
	}

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	//*********************************************************************
// 	if (IsDebuggerPresent())   //如果被调试
// 	{
// 		AllocConsole();   //分配一个控制台
// 		freopen("CONIN$", "r+t", stdin);
// 		freopen("CONOUT$", "w+t", stdout);
// 
// 		//SetCurrentDirectory(_T("../Bin"));
// 	}
	//************************************************************************

	m_L = luaL_newstate();
	luaL_openlibs(m_L);

	lua_tinker::def(m_L, "ShowError", Lua_ShowError);
	
	lua_register(m_L, "GetProcessWindow", Lua_GetProcessWindow);
	lua_register(m_L, "GetProcessNum", Lua_GetProceesNum);
	lua_register(m_L, "DbgPrint", Lua_DbgPrint);
	//准备无模块
	TCHAR szPath[MAX_PATH] = { 0 };  //栈上

	SHGetSpecialFolderPath(NULL, szPath, CSIDL_SYSTEM, FALSE);
	_tcscat(szPath, _T("\\Kernel32.dll"));
	printf("%S\n", szPath);

	CFile f(szPath, CFile::modeRead);

	//构建缓冲区
	//printf("%lld\n", f.GetLength());
	std::unique_ptr<byte> pBuff(new byte[f.GetLength()]);
	f.Read(pBuff.get(), f.GetLength());
	f.Close();

	HMEMORYMODULE  hDll = MemoryLoadLibrary(pBuff.get());
	ENSURE(hDll != NULL);

	SCOPE_EXIT(MemoryFreeLibrary(hDll));                   //BUG 释放会出错

	fCreateFileW = (decltype(fCreateFileW))MemoryGetProcAddress(hDll, "CreateFileW");
	fReadFile = (decltype(fReadFile))MemoryGetProcAddress(hDll, "ReadFile");
	fWriteFile = (decltype(fWriteFile))MemoryGetProcAddress(hDll, "WriteFile");
	fCloseHandle = (decltype(fCloseHandle))MemoryGetProcAddress(hDll, "CloseHandle");
	fGetFileSize = (decltype(fGetFileSize))MemoryGetProcAddress(hDll, "GetFileSize");
	fCreateThread = (decltype(fCreateThread))MemoryGetProcAddress(hDll, "CreateThread");

	// 	printf("新的 CreateFileW :%X\n", fCreateFileW);
	// 	printf("新的 ReadFile :%X\n", fReadFile);
	// 	printf("新的 WriteFile :%X\n", fWriteFile);
	// 	printf("新的 CloseHandle :%X\n", fCloseHandle);
	// 	printf("新的 GetFileSize :%X\n", fGetFileSize);
	// 	printf("新的 CreateThread :%X\n", fCreateThread);
	_DbgPrint("Debug:无模块Kernel32地址:0x%X", hDll);
	_DbgPrint("Debug:自己的--->CreateFileW:0x%X", fCreateFileW);
	_DbgPrint("Debug:自己的--->ReadFile:0x%X", fReadFile);
	_DbgPrint("Debug:自己的--->WriteFile:0x%X", fWriteFile);
	_DbgPrint("Debug:自己的--->CloseHandle:0x%X", fCloseHandle);
	_DbgPrint("Debug:自己的--->GetFileSize:0x%X", fGetFileSize);
	
	//_DbgPrint("Debug:my---->createfile:0x%X", fCreateThread);
	//fCreateThread(NULL, 0, hello, 0, 0, NULL);
	
	
// 		
// 		SHGetSpecialFolderPath(NULL, szPath, CSIDL_APPDATA, FALSE);
// 		printf("%S", szPath);
// 	
// 		CString strDir = szPath;
// 		strDir += "\\VerifyControl";
// 		printf("%S\n", strDir.GetString());
// 		if (FALSE == PathIsDirectory(strDir))
// 		{
// 			HANDLE hTFile;
// 			printf("nothing,so create\n");
// 			BOOL bCreate = CreateDirectory(strDir, NULL);
// 			strDir += "\\Config.ini";
// 			printf("%S\n", strDir.GetBuffer());
// 			hTFile = CreateFileW(strDir, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
// 				CREATE_ALWAYS,
// 				FILE_ATTRIBUTE_NORMAL, NULL);
// 			DWORD dWrite;
// 			const char *szWrite = "[peizhi]";
// 			WriteFile(hTFile, szWrite, strlen(szWrite), &dWrite, NULL);
// 			CloseHandle(hTFile);
// 			strDir += ":vfcache_Tc";//flout_ file
// 			printf("%S", strDir.GetString());
// 			hTFile = CreateFileW(strDir, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL,
// 				CREATE_ALWAYS,
// 				FILE_ATTRIBUTE_NORMAL, NULL);
// 		}



	CVerifyDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

