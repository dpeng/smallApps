// BmpShowDlg.h : header file
//

#pragma once


// CBmpShowDlg dialog
class CBmpShowDlg : public CDialog
{
// Construction
public:
	CBmpShowDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_BMPSHOW_DIALOG };

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
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenfile();
	afx_msg void OnBnClickedShow();
public:
	CString m_sFileName;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
