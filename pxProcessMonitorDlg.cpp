// pxProcessMonitorDlg.cpp : 实现文件
// Created by Zhaoliang Guo

#include "stdafx.h"
#include "pxProcessMonitor.h"
#include "pxProcessMonitorDlg.h"
#include "afxdialogex.h"
#include "pxCommonDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CpxProcessMonitorDlg 对话框
CpxProcessMonitorDlg::CpxProcessMonitorDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CpxProcessMonitorDlg::IDD, pParent)
	, m_strProcessName(_T(""))
	, m_strHour(_T("0"))
	, m_strMinute(_T("0"))
	, m_strSecond(_T("30"))
	, m_strMonitorState(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	CString strState;
	strState.LoadString(IDS_STRING_READY);
	m_strMonitorState = strState;

	char szProcessName[_MAX_PATH] = {0};
	GetPrivateProfileString("Monitor", "ProcessName", "", szProcessName, sizeof(szProcessName), g_strConfFile);
	m_strProcessName.Format("%s", szProcessName);

	char szSeconds[32] = {0};
	GetPrivateProfileString("Monitor", "ScanFrequency", "30", szSeconds, sizeof(szSeconds), g_strConfFile);

	long lSec   = atoi(szSeconds);
	int nHour   = lSec / 3600;
	int nMinute = (lSec - (nHour * 3600)) / 60;
	int nSecond = lSec - (nHour * 3600) - nMinute * 60;

	m_strHour.Format("%d",   nHour);
	m_strMinute.Format("%d", nMinute);
	m_strSecond.Format("%d", nSecond);
}

void CpxProcessMonitorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PROCESSNAME, m_strProcessName);
	DDX_Text(pDX, IDC_EDIT_HOURS, m_strHour);
	DDX_Text(pDX, IDC_EDIT_MINUTES, m_strMinute);
	DDX_Text(pDX, IDC_EDIT_SECONDS, m_strSecond);
	DDX_Text(pDX, IDC_STATIC_MONITOR_STATE, m_strMonitorState);
}

BEGIN_MESSAGE_MAP(CpxProcessMonitorDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SCAN, &CpxProcessMonitorDlg::OnScan)
	ON_BN_CLICKED(IDC_BUTTON_START_MONITOR, &CpxProcessMonitorDlg::OnStartMonitor)
	ON_BN_CLICKED(IDC_BUTTON_EXIT_MONITOR, &CpxProcessMonitorDlg::OnExitMonitor)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CpxProcessMonitorDlg::OnSave)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CpxProcessMonitorDlg 消息处理程序

BOOL CpxProcessMonitorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	((CButton*)GetDlgItem(IDC_BUTTON_EXIT_MONITOR))->EnableWindow(FALSE);

	OnStartMonitor();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CpxProcessMonitorDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CpxProcessMonitorDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CpxProcessMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CpxProcessMonitorDlg::OnScan()
{
	UpdateData();

	bool bProcessExist = IsProcessExist(m_strProcessName);

	CString strMsg = "所监控的程序: " + m_strProcessName;

	if (bProcessExist)
	{
		strMsg += " 目前正在运行中...";
	}
	else
	{
		strMsg += " 目前没有运行.";
	}

	AfxMessageBox(strMsg);

	UpdateData(FALSE);
}

UINT ProcessMonitorThread(LPVOID pParam)
{
	CpxProcessMonitorDlg *pProcessMonitorDlg = (CpxProcessMonitorDlg *)pParam;

	char szProcessName[_MAX_PATH] = {0};
	GetPrivateProfileString("Monitor", "ProcessName", "", szProcessName, sizeof(szProcessName), g_strConfFile);
	CString strProcessName;
	strProcessName.Format("%s", szProcessName);

	char szSeconds[32] = {0};
	GetPrivateProfileString("Monitor", "ScanFrequency", "30", szSeconds, sizeof(szSeconds), g_strConfFile);

	long lSec   = atoi(szSeconds);

	g_bThreadActive = true;

	while (g_bThreadActive)
	{
		bool bProcessExist = IsProcessExist(strProcessName);

		if (!bProcessExist)
		{
			bool bRet = StartProcess();
			if (!bRet)
			{
				CString strState;
				strState.LoadString(IDS_STRING_STOP_MONITOR);
				pProcessMonitorDlg->m_strMonitorState = strState;
			}
		}

		for (int i = 0; i < lSec; i++)
		{
			if (!g_bThreadActive)
			{
				break;
			}

			Sleep(1000);
		}
	}

	g_bThreadActive = false;

	return 0;
}

bool StartProcess()
{
	bool bRet = false;
	char szProcessName[_MAX_PATH] = {0};
	GetPrivateProfileString("Monitor", "ProcessName", "", szProcessName, sizeof(szProcessName), g_strConfFile);
	CString strProcessName;
	strProcessName.Format("%s", szProcessName);

	if (IsFileExist(strProcessName))
	{
		ShellExecute(NULL, "open", strProcessName, NULL, NULL, SW_HIDE);
		bRet = true;
	}
	
	return bRet;
}

void CpxProcessMonitorDlg::OnStartMonitor()
{
	UpdateData();

	OnSave();

	if (!IsFileExist(m_strProcessName))
	{
		CString strState;
		strState.LoadString(IDS_STRING_PROCESS_NOT_FOUND);
		m_strMonitorState = strState;
		UpdateData(FALSE);
		return ;
	}

	Sleep(100);
	AfxBeginThread(ProcessMonitorThread, this, THREAD_PRIORITY_NORMAL);

	((CButton*)GetDlgItem(IDC_BUTTON_START_MONITOR))->EnableWindow(FALSE);
	((CButton*)GetDlgItem(IDC_BUTTON_EXIT_MONITOR))->EnableWindow(TRUE);

	CString strStateMonitoring;
	strStateMonitoring.LoadString(IDS_STRING_MONITORING);
	m_strMonitorState = strStateMonitoring;

	CString strStateStopMonitor;
	strStateStopMonitor.LoadString(IDS_STRING_STOP_MONITOR);
	UpdateData(FALSE);
}

void CpxProcessMonitorDlg::OnExitMonitor()
{
	g_bThreadActive = false;
	
	CString strStateStopMonitor;
	strStateStopMonitor.LoadString(IDS_STRING_STOP_MONITOR);
	m_strMonitorState = strStateStopMonitor;

	((CButton*)GetDlgItem(IDC_BUTTON_START_MONITOR))->EnableWindow(TRUE);
	((CButton*)GetDlgItem(IDC_BUTTON_EXIT_MONITOR))->EnableWindow(FALSE);
	UpdateData(FALSE);
}

void CpxProcessMonitorDlg::OnSave()
{
	UpdateData();
	/*if (m_strProcessName == "")
	{
		AfxMessageBox("进程名不能为空， 请确认.");
		((CButton *)GetDlgItem(IDC_EDIT_PROCESSNAME))->SetFocus();
		return ;
	}

	if (m_strHour == "")
	{
		AfxMessageBox("小时不能为空， 请确认.");
		((CButton *)GetDlgItem(IDC_EDIT_HOURS))->SetFocus();
		return ;
	}

	if (m_strMinute == "")
	{
		AfxMessageBox("分不能为空， 请确认.");
		((CButton *)GetDlgItem(IDC_EDIT_MINUTES))->SetFocus();
		return ;
	}

	if (m_strSecond == "")
	{
		AfxMessageBox("秒不能为空， 请确认.");
		((CButton *)GetDlgItem(IDC_EDIT_SECONDS))->SetFocus();
		return ;
	}*/

	long lSec = (_ttoi(m_strHour) * 3600) + (_ttoi(m_strMinute) * 60) + _ttoi(m_strSecond);
	CString strScanFrequency("");
	strScanFrequency.Format("%ld", lSec);
	WritePrivateProfileString ("Monitor", "ProcessName",   m_strProcessName,  g_strConfFile);
	WritePrivateProfileString ("Monitor", "ScanFrequency", strScanFrequency,  g_strConfFile);

	UpdateData(FALSE);
}


HBRUSH CpxProcessMonitorDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	STATIC_BKCOLOR static_BkColor = NULL_COLOR;
	HBRUSH return_hbr = hbr;

	CString strStateMonitoring;
	strStateMonitoring.LoadString(IDS_STRING_MONITORING);
	CString strStateStopMonitor;
	strStateStopMonitor.LoadString(IDS_STRING_STOP_MONITOR);
	CString strProcessNotFound;
	strProcessNotFound.LoadString(IDS_STRING_PROCESS_NOT_FOUND);

	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_MONITOR_STATE:
		if ((m_strMonitorState == strStateStopMonitor) 
			|| (m_strMonitorState == strProcessNotFound))
		{
			static_BkColor = RED_COLOR;
		}
		else if (m_strMonitorState == strStateMonitoring)
		{
			static_BkColor = GREEN_COLOR;
		}
		else
		{
			static_BkColor = NULL_COLOR;
		}
		break;
	default:
		static_BkColor = NULL_COLOR;
	}

	switch (static_BkColor)
	{
	case RED_COLOR:
		pDC->SetTextColor(RGB(255,255,255));
		pDC->SetBkColor(RGB(255,0,0));
		return_hbr = (HBRUSH)brush_red;
		break;
	case GREEN_COLOR:
		pDC->SetTextColor(RGB(0,0,0));
		pDC->SetBkColor(RGB(0,255,0));
		return_hbr = (HBRUSH)brush_green;
		break;
	case NULL_COLOR:
		return_hbr = hbr;
		break;
	default:
		return_hbr = hbr;
	}

	return return_hbr;
}
