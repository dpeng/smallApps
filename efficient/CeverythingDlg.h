#pragma once
#include "afxwin.h"
#include "./everything/Everything.h"
#include<vector>
using namespace std;
// CeverythingDlg dialog

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
	CEdit m_KeywordCollector;
	CListBox m_SearchResultList;
	vector <CString> m_searchResultStr;
	afx_msg void OnEnChangeKeywordcollector();
};
