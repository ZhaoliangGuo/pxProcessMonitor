// pxProcessMonitorDlg.cpp : ʵ���ļ�
// Created by Zhaoliang Guo

#include "stdafx.h"
#include "pxProcessMonitor.h"
#include "pxProcessMonitorDlg.h"
#include "afxdialogex.h"
#include "pxCommonDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CpxProcessMonitorDlg �Ի���
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


// CpxProcessMonitorDlg ��Ϣ�������

BOOL CpxProcessMonitorDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	((CButton*)GetDlgItem(IDC_BUTTON_EXIT_MONITOR))->EnableWindow(FALSE);

	OnStartMonitor();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CpxProcessMonitorDlg::OnPaint()
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
HCURSOR CpxProcessMonitorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CpxProcessMonitorDlg::OnScan()
{
	UpdateData();

	bool bProcessExist = IsProcessExist(m_strProcessName);

	CString strMsg = "����صĳ���: " + m_strProcessName;

	if (bProcessExist)
	{
		strMsg += " Ŀǰ����������...";
	}
	else
	{
		strMsg += " Ŀǰû������.";
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
		AfxMessageBox("����������Ϊ�գ� ��ȷ��.");
		((CButton *)GetDlgItem(IDC_EDIT_PROCESSNAME))->SetFocus();
		return ;
	}

	if (m_strHour == "")
	{
		AfxMessageBox("Сʱ����Ϊ�գ� ��ȷ��.");
		((CButton *)GetDlgItem(IDC_EDIT_HOURS))->SetFocus();
		return ;
	}

	if (m_strMinute == "")
	{
		AfxMessageBox("�ֲ���Ϊ�գ� ��ȷ��.");
		((CButton *)GetDlgItem(IDC_EDIT_MINUTES))->SetFocus();
		return ;
	}

	if (m_strSecond == "")
	{
		AfxMessageBox("�벻��Ϊ�գ� ��ȷ��.");
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
