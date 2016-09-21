
// pxProcessMonitorDlg.h : ͷ�ļ�
//

#pragma once


// CpxProcessMonitorDlg �Ի���
class CpxProcessMonitorDlg : public CDialogEx
{
// ����
public:
	CpxProcessMonitorDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_PXPROCESSMONITOR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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