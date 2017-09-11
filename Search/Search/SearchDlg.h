// SearchDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "URLEncode.h"


// CSearchDlg dialog
class CSearchDlg : public CDialog
{
// Construction
public:
	CSearchDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SEARCH_DIALOG };

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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	HINSTANCE m_hInstanceTmp;
private:
	CString m_msg;
	char * CopyClip();
	char * GetDefaultIE();
	char m_DefaultBrose[MAX_PATH];
	void Search();
public:
	afx_msg void OnCbnSelchangeDragnet();
	CComboBox m_CtrlDragNet;
	int m_nDragNet;
	enum
	{
		SEARCH_GOOGLE		= 1,
		SEARCH_BAIDU		= 2,
		SEARCH_BING			= 3,
		SEARCH_GOUGOU		= 4,
		SEARCH_DICT			= 5,
		SECRCH_CODEPLEX		= 6,
		SEARCH_CODEPROJECT	= 7,
	};
private:
	CURLEncode m_UrlEncode;
};
