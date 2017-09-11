// EverythingDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "DataBase.h"
#include "FileMap.h"

typedef queue<FileIndex> QUEUE;
class CEverythingDlg : public CDialog
{
// Construction
public:
	CEverythingDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_EVERYTHING_DIALOG };

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
	CListCtrl m_Filelist;
	vector<FileIndex> m_filevector;
	vector<CString>m_dirvername;
	void BrowseFileRecursion(char* strFile);
	void BrowseFileUnrecursion(char* strFile);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	static void WINAPI SearchThread(ThreadPara* ThreadParameter);
	static void WINAPI Readqueue(LPVOID lPara);
	QUEUE m_queue;
	HANDLE m_ThreadId[MAX_THREAD_NUM];
	afx_msg void OnDestroy();
	HANDLE hMutex;
	CRITICAL_SECTION m_lock;
	long long m_FileNr;
	afx_msg void OnBnClickedSearch();
	CString m_Text;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnLvnItemchangedListFile(NMHDR *pNMHDR, LRESULT *pResult);
	CDataBase m_Localdb;
	//CDataBase m_Infodb;
	ItemInfo m_FileIndexItem;
	afx_msg void OnBnClickedSearchinfo();
	int m_clickCount;
#ifdef FILE_MAP_VIEW
	CFileMap m_FileMap;
#endif
};
