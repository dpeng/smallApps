// GetPointDlg.h : header file
//

#pragma once
#include "ShowMsg.h"

// CGetPointDlg dialog
class CGetPointDlg : public CDialog
{
// Construction
public:
	CGetPointDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GETPOINT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	CShowMsg m_Msg;
	HINSTANCE m_hInstanceTmp;
	void OnWndFullScreen(void);
	bool m_bIsFullScreen;
	RECT m_rc;
	WINDOWPLACEMENT m_OldWndplacement;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	BOOL SetWindowTransParent(void);
	int m_CurPointNr;
};
