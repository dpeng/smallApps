// AutoRunDlg.h : header file
//

#if !defined(AFX_AutoRunDLG_H__2C23EB64_4D86_47C4_B6EB_2859C090CE38__INCLUDED_)
#define AFX_AutoRunDLG_H__2C23EB64_4D86_47C4_B6EB_2859C090CE38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAutoRunDlg dialog

class CAutoRunDlg : public CDialog
{
// Construction
public:
	CAutoRunDlg(CWnd* pParent = NULL);	// standard constructor
	BOOL m_bin;
	BOOL m_bout;
	BOOL m_bshut;
// Dialog Data
	//{{AFX_DATA(CAutoRunDlg)
	enum { IDD = IDD_AUTORUN_DIALOG };
	UINT	m_hour;
	UINT	m_minute;
	UINT	m_second;
	double    m_CurTime;
	HINSTANCE m_hInstanceTmp;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoRunDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAutoRunDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSignin();
	afx_msg void OnSignout();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShutdown();
	afx_msg void OnHide();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AutoRunDLG_H__2C23EB64_4D86_47C4_B6EB_2859C090CE38__INCLUDED_)
