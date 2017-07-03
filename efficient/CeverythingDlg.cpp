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
	DDX_Control(pDX, IDC_KEYWORDCOLLECTOR, m_KeywordCollector);
	DDX_Control(pDX, IDC_SEARCHRESULTLIST, m_SearchResultList);
}


BEGIN_MESSAGE_MAP(CeverythingDlg, CDialog)
	ON_EN_CHANGE(IDC_KEYWORDCOLLECTOR, &CeverythingDlg::OnEnChangeKeywordcollector)
END_MESSAGE_MAP()


// CeverythingDlg message handlers


/*
CString strt[100];
Everything_SetSearchA("face2");
Everything_QueryA(TRUE);
DWORD i;

for (i = 0; i < Everything_GetNumResults(); i++)
{
printf("%s\n", Everything_GetResultFileNameA(i));
strt[i] = Everything_GetResultFileNameA(i);
}*/


void CeverythingDlg::OnEnChangeKeywordcollector()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	// got the keyword from user input
	CString keyWord = "";
	m_KeywordCollector.GetWindowTextA(keyWord);

	//search by everything
	Everything_SetSearchA(keyWord);
	Everything_QueryA(TRUE);
	m_searchResultStr.clear();
	int displaycount = Everything_GetNumResults();
	if (displaycount > 100)
	{
		displaycount = 100;
	}
	for (int i = 0; i < displaycount; i++)
	{
		//strt[i] = Everything_GetResultFileNameA(i);
		m_searchResultStr.push_back(Everything_GetResultFileNameA(i));
	}
	m_SearchResultList.ResetContent();
	for (int i = 0; i < m_searchResultStr.size(); i++)
	{
		m_SearchResultList.AddString(m_searchResultStr.at(i));
	}
	int abc = 0;
}
