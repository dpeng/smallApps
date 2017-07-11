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
	CString tmpStr = "";
	SSearchResult tmpResult;
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
		tmpResult.fileName = Everything_GetResultFileNameA(i);
		tmpResult.filePath = Everything_GetResultPathA(i);
		m_searchResult.push_back(tmpResult);
	}

	m_searchResult.size();
	for (int i = 0; i < m_searchResult.size(); i++)
	{
		tmpResult = m_searchResult.at(i);
		//tmpStr.Format("%s\r%s", tmpResult.fileName, tmpResult.filePath);
		m_listCtrl.AppendString(tmpResult.fileName, tmpResult.filePath, RGB(53, 0, 27), RGB(236, 255, 236));
	}
}


BOOL CeverythingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
