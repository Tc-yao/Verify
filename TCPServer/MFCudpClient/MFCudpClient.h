
// MFCudpClient.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CMFCudpClientApp: 
// �йش����ʵ�֣������ MFCudpClient.cpp
//

class CMFCudpClientApp : public CWinApp
{
public:
	CMFCudpClientApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CMFCudpClientApp theApp;