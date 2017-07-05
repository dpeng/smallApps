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
	SSearchResult tmpResult;
	m_EditBox.GetWindowTextA(keyWord);

	//search by everything
	Everything_SetSearchA(keyWord);
	Everything_QueryA(TRUE);
	m_searchResult.clear();
	int displaycount = Everything_GetNumResults();
	if (displaycount > 100)
	{
		displaycount = 100;
	}
	for (int i = 0; i < displaycount; i++)
	{
		tmpResult.fileName = Everything_GetResultFileNameA(i);
		tmpResult.fileName = Everything_GetResultPathA(i);
		m_searchResult.push_back(tmpResult);
	}
	//m_listCtrl.ResetContent();
	for (int i = 0; i < m_searchResult.size(); i++)
	{
		tmpResult = m_searchResult.at(i);
		//m_listCtrl.AddString(tmpResult.fileName);
	}
}


BOOL CeverythingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	CRect rect;
	m_listCtrl.GetClientRect(&rect);
	m_listCtrl.SetExtendedStyle(m_listCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	int ret = m_listCtrl.InsertColumn(0, _T("fileName"), LVCFMT_LEFT, rect.Width()/4, 0);
	ret = m_listCtrl.InsertColumn(1, _T("filePath"), LVCFMT_LEFT, 3* rect.Width() / 4, 1);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
