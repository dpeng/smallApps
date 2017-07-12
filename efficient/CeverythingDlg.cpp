// CeverythingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "efficient.h"
#include "CeverythingDlg.h"
#include "afxdialogex.h"


// CeverythingDlg dialog

IMPLEMENT_DYNAMIC(CeverythingDlg, CDialog)

CeverythingDlg::CeverythingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_EVERYTHING, pParent)
{
}

CeverythingDlg::~CeverythingDlg()
{
}

void CeverythingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_KEYWORDCOLLECTOR, m_EditBox);
	DDX_Control(pDX, IDC_SEARCHRESULTLIST, m_listCtrl);
}


BEGIN_MESSAGE_MAP(CeverythingDlg, CDialog)
	ON_EN_CHANGE(IDC_KEYWORDCOLLECTOR, &CeverythingDlg::OnEnChangeKeywordcollector)
	ON_LBN_DBLCLK(IDC_SEARCHRESULTLIST, &CeverythingDlg::OnLbnDblclkSearchresultlist)
END_MESSAGE_MAP()


// CeverythingDlg message handlers

void CeverythingDlg::OnEnChangeKeywordcollector()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	// got the keyword from user input
	CString keyWord = "";
	m_EditBox.GetWindowTextA(keyWord);

	//search by everything
	Everything_SetSearchA(keyWord);
	Everything_QueryA(TRUE);
	m_searchResult.clear();
	m_listCtrl.ResetContent();
	int displaycount = Everything_GetNumResults();
	if (displaycount > 100)
	{
		displaycount = 100;
	}
	for (int i = 0; i < displaycount; i++)
	{
		m_tmpSearchResult.fileName = Everything_GetResultFileNameA(i);
		m_tmpSearchResult.filePath = Everything_GetResultPathA(i);
		m_searchResult.push_back(m_tmpSearchResult);
	}

	m_searchResult.size();
	for (int i = 0; i < m_searchResult.size(); i++)
	{
		m_tmpSearchResult = m_searchResult.at(i);
		m_listCtrl.AppendString(m_tmpSearchResult.fileName, m_tmpSearchResult.filePath, RGB(53, 0, 27), RGB(236, 255, 236));
	}
}


BOOL CeverythingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

extern HWND g_hWnd;
void CeverythingDlg::OnLbnDblclkSearchresultlist()
{
	// TODO: Add your control notification handler code here
	int nSel;
	nSel = m_listCtrl.GetCurSel();
	CString s;
	m_listCtrl.GetText(nSel, s);
	releaseResources();
	ShellExecute(g_hWnd, _T("open"), s, _T(""), _T(""), SW_SHOWNORMAL);
}


BOOL CeverythingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return true;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		releaseResources();
		//SUCESS_BEEP(1000, 300, g_configXml->m_bBeep);
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
void CeverythingDlg::releaseResources()
{
	ShowWindow(SW_HIDE);
	m_searchResult.clear();
	m_listCtrl.ResetContent();
	Everything_CleanUp();
}