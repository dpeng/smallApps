#pragma once
#include "afxwin.h"
#include "./everything/Everything.h"
#include "./multiLineListBox/MultiLineListBox.h"
#include<vector>
#include "afxcmn.h"
using namespace std;
// CeverythingDlg dialog
typedef struct
{
	CString fileName;
	CString filePath;
} SSearchResult;
class CeverythingDlg : public CDialog
{
	DECLARE_DYNAMIC(CeverythingDlg)

public:
	CeverythingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CeverythingDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EVERYTHING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditBox;
	CString m_keyWordInEditBox;
	BOOL m_editBoxTextChange;
	vector <SSearchResult> m_searchResult;
	SSearchResult m_tmpSearchResult;
	afx_msg void OnEnChangeKeywordcollector();
	virtual BOOL OnInitDialog();
	CMultiLineListBox m_listCtrl;
	afx_msg void OnLbnDblclkSearchresultlist();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void releaseResources();
	HANDLE m_queryAndDisplayProcessHandler;
	static DWORD WINAPI queryAndDisplayProcess(LPVOID pParam);
};
