
// MFCClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MFCClient.h"
#include "MFCClientDlg.h"
#include "afxdialogex.h"
#include <winsock.h>  
#include <stdio.h>  

#pragma comment(lib, "ws2_32.lib")
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCClientDlg �Ի���



CMFCClientDlg::CMFCClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFCCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCClientDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_RADIO2, &CMFCClientDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO1, &CMFCClientDlg::OnBnClickedRadio1)
END_MESSAGE_MAP()


// CMFCClientDlg ��Ϣ�������

BOOL CMFCClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	SetDlgItemText(IDC_EDIT1, "192.168.0.138");
	SetDlgItemInt(IDC_EDIT2,8888);

	CString stri;
	for (int i=0;i<7;i++)
	{
		stri.Format(_T("hao,%d"),i);
		((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(stri);
	}
	((CComboBox*)GetDlgItem(IDC_COMBO1))->AddString(stri);
	SetDlgItemText(IDC_STATIC4, _T("Tc_yao"));
	((CListBox*)GetDlgItem(IDC_LIST1))->AddString(_T("haha"));
	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(1);
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMFCClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMFCClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMFCClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


SOCKET initsocket()
{
	//��ʼ���׽���
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}

	//�����׽���
	SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == INVALID_SOCKET)
	{
		printf("Failed socket() \n");
		return 0;
	}
	return s;
}

void CMFCClientDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SOCKET s = initsocket();
	CString str;
	sockaddr_in servAddr;
	servAddr.sin_family = AF_INET;
	GetDlgItemText(IDC_EDIT1,str);
	servAddr.sin_addr.S_un.S_addr = inet_addr(str.GetString());
	int nPort = GetDlgItemInt(IDC_EDIT2);
	servAddr.sin_port = htons(nPort);

	if (::connect(s, (sockaddr*)&servAddr, sizeof(servAddr)) == -1)
	{
		printf("Failed connect() \n");
		return ;
	}
	char buff[256];
	int nRecv = ::recv(s, buff, 256, 0);
	if (nRecv > 0)
	{
		buff[nRecv] = '\0';
		AfxMessageBox("���յ�����");
	}
	//MessageBox("���������\n");
	::closesocket(s);
	getchar();
}


void CMFCClientDlg::OnBnClickedRadio1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
void CMFCClientDlg::OnBnClickedRadio2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

