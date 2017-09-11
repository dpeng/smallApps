#pragma once


// CClockDlg dialog

class CClockDlg : public CDialog
{
	DECLARE_DYNAMIC(CClockDlg)

public:
	CClockDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CClockDlg();

// Dialog Data
	enum { IDD = IDD_CLOCKDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CBrush m_Brush;
};
