// ShowPicDlg.h : header file
//

#pragma once


// CShowPicDlg dialog
class CShowPicDlg : public CDialog
{
// Construction
public:
	CShowPicDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SHOWPIC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);  

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenfile();
	afx_msg void OnBnClickedShow();
public:
	CString m_sFileName;
	CBrush *m_brush;
	BOOL m_bReSizeSet;
	bool m_bIsFullScreen;
	RECT m_rc;
	bool ShowBmp(LPCTSTR FileName);
	bool ShowJpg(LPCTSTR FileName);
	void ReSizeWindow(void);
	void OnWndFullScreen(void);
	WINDOWPLACEMENT m_OldWndplacement;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedPreview();
	afx_msg void OnBnClickedNext();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedAutoshow();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedStopautoshow();
	afx_msg void OnBnClickedEnlarge();
	afx_msg void OnBnClickedReduce();
	int m_nMutil;
};
