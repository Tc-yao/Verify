
// Verify.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CVerifyApp: 
// �йش����ʵ�֣������ Verify.cpp
//

class CVerifyApp : public CWinApp
{
public:
	CVerifyApp();

// ��д
public:
	virtual BOOL InitInstance();
public: 
	lua_State * m_L;
// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CVerifyApp theApp;