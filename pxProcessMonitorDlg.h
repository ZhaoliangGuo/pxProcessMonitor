
// pxProcessMonitorDlg.h : 头文件
//

#pragma once


// CpxProcessMonitorDlg 对话框
class CpxProcessMonitorDlg : public CDialogEx
{
// 构造
public:
	CpxProcessMonitorDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_PXPROCESSMONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_strProcessName;
	CString m_strHour;
	CString m_strMinute;
	CString m_strSecond;
	CString m_strMonitorState;

	afx_msg void OnScan();
	afx_msg void OnSave();
	afx_msg void OnStartMonitor();
	afx_msg void OnExitMonitor();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};

UINT ProcessMonitorThread(LPVOID pParam);
bool StartProcess();