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
	ON_WM_CTLCOLOR()
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

				displaycount = displaycount < 10 ? displaycount : 10;

				pThis->MoveWindow(CRect(pThis->m_rc.left, pThis->m_rc.top, pThis->m_rc.right, (LONG)(pThis->m_rc.bottom + SEARCHRESULTITEMHEIGHT * displaycount)));
				pThis->m_listCtrl.MoveWindow(CRect(0, (pThis->m_rc.bottom - pThis->m_rc.top), (pThis->m_rc.right - pThis->m_rc.left), (LONG)(pThis->m_rc.bottom - pThis->m_rc.top + SEARCHRESULTITEMHEIGHT * displaycount)));

				for (int i = 0; i < pThis->m_searchResult.size(); i++)
				{
					pThis->m_tmpSearchResult = pThis->m_searchResult.at(i);
					pThis->m_listCtrl.AppendString(pThis->m_tmpSearchResult.fileName, pThis->m_tmpSearchResult.filePath, RGB(192, 192, 192), RGB(0, 43, 54));
				}
				pThis->m_listCtrl.AppendString(MAGIC_STRING_FOR_LAST_DRAW, MAGIC_STRING_FOR_LAST_DRAW, RGB(192, 192, 192), RGB(0, 43, 54));
			}
		}
		else
		{
			Sleep(1);
		}
	}
	return 0;
}

BOOL CeverythingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	//set the main window place
	m_screenx = GetSystemMetrics(SM_CXSCREEN);
	m_screeny = GetSystemMetrics(SM_CYSCREEN);
	GetClientRect(&m_rc);
	m_rc.top = (LONG)(m_screeny * 0.191);
	m_rc.bottom += (LONG)(m_screeny * 0.191);
	m_rc.left = (LONG)(m_screenx * 0.191);//(1-0.618)/2
	m_rc.right = (LONG)(m_screenx * 0.809);
	MoveWindow(m_rc);
	m_listCtrl.MoveWindow(CRect(0, 40, 0, 0));

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
	int nSel = m_listCtrl.GetCurSel();
	CString s;
	m_listCtrl.GetText(nSel, s);
	releaseResources();
	ShellExecute(g_hWnd, _T("open"), s, _T(""), _T(""), SW_SHOWNORMAL);
}


BOOL CeverythingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (IDC_SEARCHRESULTLIST == (CWnd::GetFocus())->GetDlgCtrlID())
		{
			OnLbnDblclkSearchresultlist();
		}
		return true;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		releaseResources();
		return true;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_DOWN)
	{
		if (IDC_KEYWORDCOLLECTOR == (CWnd::GetFocus())->GetDlgCtrlID())
		{
			m_listCtrl.SetFocus();
			m_listCtrl.SetCurSel(0);
			return true;
		}
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
	//move the window to default place
	MoveWindow(m_rc);
	m_listCtrl.MoveWindow(CRect (0, 40, 0, 0));

	Everything_CleanUp();
	CloseHandle(m_queryAndDisplayProcessHandler);
	m_queryAndDisplayProcessHandler = NULL;
}

HBRUSH CeverythingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if ((CTLCOLOR_EDIT == nCtlColor) && (IDC_KEYWORDCOLLECTOR == pWnd->GetDlgCtrlID()) )
	{
		pDC->SetBkColor(RGB(0, 35, 46));
		pDC->SetTextColor(RGB(192, 192, 192));
		hbr = CreateSolidBrush(RGB(0, 35, 46));
	}
	if (CTLCOLOR_DLG == nCtlColor)
	{
		pDC->SetBkColor(RGB(0, 43, 54));
		pDC->SetTextColor(RGB(192, 192, 192));
		hbr = CreateSolidBrush(RGB(0, 43, 54));
	}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}
