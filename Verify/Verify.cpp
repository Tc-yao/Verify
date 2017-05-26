
// Verify.cpp : ����Ӧ�ó��������Ϊ��
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


// CVerifyApp ����

CVerifyApp::CVerifyApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CVerifyApp ����

CVerifyApp theApp;


// CVerifyApp ��ʼ��

BOOL CVerifyApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	if (!AfxOleInit())
	{
		//AfxMessageBox(IDP_OLE_INIT_FAILED);// ��ʼ��OLEʧ��
		return FALSE;
	}

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	//*********************************************************************
// 	if (IsDebuggerPresent())   //���������
// 	{
// 		AllocConsole();   //����һ������̨
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
	//׼����ģ��
	TCHAR szPath[MAX_PATH] = { 0 };  //ջ��

	SHGetSpecialFolderPath(NULL, szPath, CSIDL_SYSTEM, FALSE);
	_tcscat(szPath, _T("\\Kernel32.dll"));
	printf("%S\n", szPath);

	CFile f(szPath, CFile::modeRead);

	//����������
	//printf("%lld\n", f.GetLength());
	std::unique_ptr<byte> pBuff(new byte[f.GetLength()]);
	f.Read(pBuff.get(), f.GetLength());
	f.Close();

	HMEMORYMODULE  hDll = MemoryLoadLibrary(pBuff.get());
	ENSURE(hDll != NULL);

	SCOPE_EXIT(MemoryFreeLibrary(hDll));                   //BUG �ͷŻ����

	fCreateFileW = (decltype(fCreateFileW))MemoryGetProcAddress(hDll, "CreateFileW");
	fReadFile = (decltype(fReadFile))MemoryGetProcAddress(hDll, "ReadFile");
	fWriteFile = (decltype(fWriteFile))MemoryGetProcAddress(hDll, "WriteFile");
	fCloseHandle = (decltype(fCloseHandle))MemoryGetProcAddress(hDll, "CloseHandle");
	fGetFileSize = (decltype(fGetFileSize))MemoryGetProcAddress(hDll, "GetFileSize");
	fCreateThread = (decltype(fCreateThread))MemoryGetProcAddress(hDll, "CreateThread");

	// 	printf("�µ� CreateFileW :%X\n", fCreateFileW);
	// 	printf("�µ� ReadFile :%X\n", fReadFile);
	// 	printf("�µ� WriteFile :%X\n", fWriteFile);
	// 	printf("�µ� CloseHandle :%X\n", fCloseHandle);
	// 	printf("�µ� GetFileSize :%X\n", fGetFileSize);
	// 	printf("�µ� CreateThread :%X\n", fCreateThread);
	_DbgPrint("Debug:��ģ��Kernel32��ַ:0x%X", hDll);
	_DbgPrint("Debug:�Լ���--->CreateFileW:0x%X", fCreateFileW);
	_DbgPrint("Debug:�Լ���--->ReadFile:0x%X", fReadFile);
	_DbgPrint("Debug:�Լ���--->WriteFile:0x%X", fWriteFile);
	_DbgPrint("Debug:�Լ���--->CloseHandle:0x%X", fCloseHandle);
	_DbgPrint("Debug:�Լ���--->GetFileSize:0x%X", fGetFileSize);
	
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
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

