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
	TerminateThread(m_queryAndDisplayProcessHandler, 0); 
	releaseResources();
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
	m_EditBox.GetWindowTextA(m_keyWordInEditBox);
	m_editBoxTextChange = TRUE;
}

DWORD CeverythingDlg::queryAndDisplayProcess(LPVOID pParam)
{
	CeverythingDlg* pThis = (CeverythingDlg*)pParam;
	while (1)
	{
		if (pThis->m_editBoxTextChange)
		{
			pThis->m_editBoxTextChange = FALSE;
			CString tmpKeyWord = Everything_GetSearchA();
			if (tmpKeyWord.Compare(pThis->m_keyWordInEditBox))
			{
				//search by everything
				Everything_SetSearchA(pThis->m_keyWordInEditBox);
				Everything_QueryA(TRUE);
				CString tmpErrorMsg = "";
				int everythingErrorCode = Everything_GetLastError();
				switch (everythingErrorCode)
				{
				case EVERYTHING_OK:
					tmpErrorMsg = "EVERYTHING: everthing is ok";
					break;
				case EVERYTHING_ERROR_MEMORY:
					tmpErrorMsg = "EVERYTHING: out of memory";
					break;
				case EVERYTHING_ERROR_IPC:
					tmpErrorMsg = "EVERYTHING: search client is not running";
					break;
				case EVERYTHING_ERROR_REGISTERCLASSEX:
					tmpErrorMsg = "EVERYTHING: unable to register window class";
					break;
				case EVERYTHING_ERROR_CREATEWINDOW:
					tmpErrorMsg = "EVERYTHING: unable to create listening window";
					break;
				case EVERYTHING_ERROR_CREATETHREAD:
					tmpErrorMsg = "EVERYTHING: unable to create listening thread";
					break;
				case EVERYTHING_ERROR_INVALIDINDEX:
					tmpErrorMsg = "EVERYTHING: invalid index";
					break;
				case EVERYTHING_ERROR_INVALIDCALL:
					tmpErrorMsg = "EVERYTHING: invalid call";
					break;
				case EVERYTHING_ERROR_INVALIDREQUEST:
					tmpErrorMsg = "EVERYTHING: invalid request data, request data first";
					break;
				case EVERYTHING_ERROR_INVALIDPARAMETER:
					tmpErrorMsg = "EVERYTHING: bad parameter";
					break;
				default:
					break;
				}
				if (everythingErrorCode != EVERYTHING_OK)
				{
					pThis->MessageBox(tmpErrorMsg);
				}
				pThis->m_searchResult.clear();
				pThis->m_listCtrl.ResetContent();
				int displaycount = Everything_GetNumResults();
				if (displaycount > MAX_ITEM_SHOW_IN_LIST)
				{
					displaycount = MAX_ITEM_SHOW_IN_LIST;
				}
				for (int i = 0; i < displaycount; i++)
				{
					pThis->m_tmpSearchResult.fileName = Everything_GetResultFileNameA(i);
					pThis->m_tmpSearchResult.filePath = Everything_GetResultPathA(i);
					pThis->m_searchResult.push_back(pThis->m_tmpSearchResult);
				}

				pThis->m_searchResult.size();
				for (int i = 0; i < pThis->m_searchResult.size(); i++)
				{
					pThis->m_tmpSearchResult = pThis->m_searchResult.at(i);
					pThis->m_listCtrl.AppendString(pThis->m_tmpSearchResult.fileName, pThis->m_tmpSearchResult.filePath, RGB(53, 0, 27), RGB(236, 255, 236));
				}
			}
		}
	}
	return 0;
}

BOOL CeverythingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	DWORD threadID;
	m_keyWordInEditBox = "";
	m_editBoxTextChange = FALSE;
	m_queryAndDisplayProcessHandler = CreateThread(NULL, 0, CeverythingDlg::queryAndDisplayProcess, this, 0, &threadID);
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
	m_EditBox.SetSel(0, -1);
	m_EditBox.Clear();
	Everything_CleanUp();
	CloseHandle(m_queryAndDisplayProcessHandler);
	m_queryAndDisplayProcessHandler = NULL;
}