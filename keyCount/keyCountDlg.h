// keyCountDlg.h : header file
//

#if !defined(AFX_KEYCOUNTDLG_H__D1825063_97BF_4FF7_BD79_9D85ACDAB0C5__INCLUDED_)
#define AFX_KEYCOUNTDLG_H__D1825063_97BF_4FF7_BD79_9D85ACDAB0C5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CKeyCountDlg dialog

class CKeyCountDlg : public CDialog
{
// Construction
public:
	CKeyCountDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CKeyCountDlg)
	enum { IDD = IDD_KEYCOUNT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyCountDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CKeyCountDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
public:
	HINSTANCE m_hInstanceTmp;
	SYSTEMTIME m_sysTime;
	void ShowKeyTimes(void);
	void MoveWindowToEage(void);
	void MoveTitle(void);
	CString FindStr(int vkCode);
	void WriteResultToFile(void);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYCOUNTDLG_H__D1825063_97BF_4FF7_BD79_9D85ACDAB0C5__INCLUDED_)
