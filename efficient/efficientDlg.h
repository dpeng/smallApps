// efficientDlg.h : header file
//
#include "Configuration.h"
#include "./everything/Everything.h"
#include "CeverythingDlg.h"
#pragma once
#include "afxwin.h"
#define SHORT_CUT_KEY_MAX_LEN 8

enum{
	MASTER_KEY_WIN = 0,
	MASTER_KEY_ALT,
	MASTER_KEY_SHIFT
};
// CefficientDlg dialog
class CefficientDlg : public CDialog
{
// Construction
public:
	CefficientDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EFFICIENT_DIALOG };

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
	HINSTANCE m_hInstanceTmp;
	SYSTEMTIME m_sysTime;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedOpenfile();
	afx_msg void OnBnClickedOpenfolder();
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedCancle();
	CComboBox m_shortCutKeySlave;
	CComboBox m_shortCutKeyMaster;
	void initShortCutKey();
	char *m_shortCutKeyStrSlave[MAX_SUPPORT_FAST_START_SLAVE_KEY];
	afx_msg void OnCbnSelchangeComboListKey();
	afx_msg void OnClose();
	afx_msg void OnBnClickedRadioAutostart();
	afx_msg void OnCbnSelchangeComboListKeyMaster();
private:
	int m_curSelMaster;
	int m_curSelSlave;
	void saveAutoStart();
};
