// ComparePictureDlg.h : header file
//
#include "ShowPicture.h"
#pragma once

typedef struct{
	RECT src;
	RECT dest;
	CDC* srcDc;
	CDC* destDc;
	void init()
	{
		src.left = 0;
		src.right = 0;
		src.top = 0;
		src.bottom = 0;
		dest.left = 0;
		dest.right = 0;
		dest.top = 0;
		dest.bottom = 0;
		srcDc = new CDC;
		destDc = new CDC;
	}
	void clear()
	{
	}
}ThreadPara;
// CComparePictureDlg dialog
class CComparePictureDlg : public CDialog
{
// Construction
public:
	CComparePictureDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_COMPAREPICTURE_DIALOG };

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
	afx_msg void OnBnClickedCompare();
public:
	CShowPicture m_ShowPic;
	afx_msg void OnBnClickedOpensrc();
	afx_msg void OnBnClickedOpendest();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	static void WINAPI CompareThread(LPVOID lPara);
	HANDLE m_hThreadHandle;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CBrush m_bruse;
	CString m_strSrcFileName;
	CString m_strDestFileName;
};
