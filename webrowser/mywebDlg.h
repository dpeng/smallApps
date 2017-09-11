// mywebDlg.h : header file
//
//{{AFX_INCLUDES()
#include "webbrowser.h"
#include "webbrowser2.h"
#include "Configuration.h"
//}}AFX_INCLUDES

#if !defined(AFX_MYWEBDLG_H__D6AF9AD4_9675_40F0_94B7_0F78683BF4DC__INCLUDED_)
#define AFX_MYWEBDLG_H__D6AF9AD4_9675_40F0_94B7_0F78683BF4DC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_PROGRESS_BAR 1000
/////////////////////////////////////////////////////////////////////////////
// CMywebDlg dialog

class CMywebDlg : public CDialog
{
// Construction
public:
	CMywebDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMywebDlg)
	enum { IDD = IDD_MYWEB_DIALOG };
	CProgressCtrl	m_Progress;
	CComboBox	m_WebAddr;
	CWebBrowser	m_Broser;
	CWebBrowser2	m_BroserSub;
	//}}AFX_DATA
	RECT m_rc;
	WINDOWPLACEMENT m_OldWndplacement;
	BOOL m_bIsFullScreen;
	CString m_strPath;
	int m_urlIndex;
	void OnWndFullScreen();
	void ReSizeWindow();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMywebDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
private:
	CConfiguration *m_cfg;
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMywebDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnGo();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNewWindow2Explorer(LPDISPATCH FAR* ppDisp, BOOL FAR* Cancel);
	afx_msg void OnBeforeNavigate2ExplorerSub(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel);
	afx_msg void OnBack();
	afx_msg void OnForword();
	afx_msg void OnRefreash();
	afx_msg void OnSelchangeComboWebaddr();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnProgressChangeExplorer(long Progress, long ProgressMax);
	afx_msg void OnTitleChangeExplorer(LPCTSTR Text);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedFavorite();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYWEBDLG_H__D6AF9AD4_9675_40F0_94B7_0F78683BF4DC__INCLUDED_)
