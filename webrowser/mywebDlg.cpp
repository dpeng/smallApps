// mywebDlg.cpp : implementation file
//

#include "stdafx.h"
#include "myweb.h"
#include "mywebDlg.h"
#include "str_int.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMywebDlg dialog

CMywebDlg::CMywebDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMywebDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMywebDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMywebDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMywebDlg)
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Control(pDX, IDC_COMBO_WEBADDR, m_WebAddr);
	DDX_Control(pDX, IDC_EXPLORER, m_Broser);
	DDX_Control(pDX, IDC_EXPLORER_SUB, m_BroserSub);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMywebDlg, CDialog)
	//{{AFX_MSG_MAP(CMywebDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_GO, OnGo)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BACK, OnBack)
	ON_BN_CLICKED(IDC_FORWORD, OnForword)
	ON_BN_CLICKED(IDC_REFREASH, OnRefreash)
	ON_CBN_SELCHANGE(IDC_COMBO_WEBADDR, OnSelchangeComboWebaddr)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_FAVORITE, &CMywebDlg::OnBnClickedFavorite)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMywebDlg message handlers

BOOL CMywebDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	m_bIsFullScreen = FALSE;
	m_cfg = new CConfiguration;
	m_cfg->initConfig();
	m_WebAddr.LimitText(MAX_PATH);

	CString tmpStr = _T("");
	for(m_urlIndex = 0; m_urlIndex <= MAX_SUPPORT_FAVORITE_KEY; m_urlIndex++)
	{
		if(strlen(m_cfg->m_urlName[m_urlIndex]) > 2)
		{
			tmpStr.Format(_T("%s  %s"),m_cfg->m_pageName[m_urlIndex],  m_cfg->m_urlName[m_urlIndex]);
			m_WebAddr.InsertString(m_urlIndex,tmpStr);
		}
		else
			break;
	}
	m_Progress.SetRange(0, MAX_PROGRESS_BAR);
	m_Progress.ShowWindow(SW_HIDE);
	ReSizeWindow();
	return TRUE;  // return TRUE  unless you set the focus to a control
}
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMywebDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMywebDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMywebDlg::OnGo() 
{
	// TODO: Add your control notification handler code here
	CString str;
	m_WebAddr.GetWindowText(str);
	if (!str.IsEmpty())
	{
		m_Broser.Navigate(str,NULL,NULL,NULL,NULL);
	}
}

BOOL CMywebDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) 
	{
		OnWndFullScreen();
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_F5) 
	{
		m_Broser.Refresh();
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_LEFT)
	{
		m_Broser.GoBack();
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RIGHT)
	{
		m_Broser.GoForward();
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		if (FromHandle(pMsg->hwnd)->GetDlgCtrlID() == IDC_COMBO_WEBADDR)
		{
			OnGo();
			return TRUE;
		}
	}
	if (pMsg->message == WM_MBUTTONDOWN)
	{
		CString str = m_Broser.GetStatusText();
	}
	return CDialog::PreTranslateMessage(pMsg);
}
void CMywebDlg::OnWndFullScreen()
{
	m_bIsFullScreen = !m_bIsFullScreen;
	CButton *pButton;
	if(m_bIsFullScreen)
	{
		GetDlgItem(IDC_EXPLORER)->GetWindowRect(&m_rc);
		pButton = (CButton *)GetDlgItem(IDC_GO);		
		pButton->ModifyStyle(WS_VISIBLE,0,0);
		pButton = (CButton *)GetDlgItem(IDC_BACK);
		pButton->ModifyStyle(WS_VISIBLE, 0, 0);
		pButton = (CButton *)GetDlgItem(IDC_FORWORD);
		pButton->ModifyStyle(WS_VISIBLE, 0, 0);
		pButton = (CButton *)GetDlgItem(IDC_REFREASH);
		pButton->ModifyStyle(WS_VISIBLE, 0, 0);
		m_WebAddr.ModifyStyle(WS_VISIBLE, 0, 0);
		GetWindowPlacement(&m_OldWndplacement);
		ModifyStyle(WS_SIZEBOX,0,0);
		CRect WindowRect, ClientRect;
		RECT m_FullScreenRect;
		GetWindowRect(&WindowRect);
		WindowRect.left+=1;
		WindowRect.right+=1;
		MoveWindow(CRect(0,0,352,288),TRUE);
		GetWindowRect(&WindowRect);
		GetClientRect(&ClientRect);
		ClientToScreen(&ClientRect);
		int x = GetSystemMetrics(SM_CXSCREEN);
		int y = GetSystemMetrics(SM_CYSCREEN);
		m_FullScreenRect.left = WindowRect.left - ClientRect.left;
		m_FullScreenRect.top = WindowRect.top - ClientRect.top;
		m_FullScreenRect.right = WindowRect.right - ClientRect.right + x;
		m_FullScreenRect.bottom = WindowRect.bottom - ClientRect.bottom + y;
		WINDOWPLACEMENT wndpl;
		wndpl.length = sizeof(WINDOWPLACEMENT);
		wndpl.flags = 0;
		wndpl.showCmd = SW_SHOWNORMAL;
		wndpl.rcNormalPosition = m_FullScreenRect;
		SetWindowPlacement(&wndpl);
		RECT rc;
		GetClientRect(&rc);
		GetDlgItem(IDC_EXPLORER)->MoveWindow(&rc,TRUE);
	}
	else
	{
		pButton = (CButton *)GetDlgItem(IDC_GO);
		pButton->ModifyStyle(0,WS_VISIBLE,0);
		pButton = (CButton *)GetDlgItem(IDC_BACK);
		pButton->ModifyStyle(0,WS_VISIBLE,0);
		pButton = (CButton *)GetDlgItem(IDC_FORWORD);
		pButton->ModifyStyle(0,WS_VISIBLE,0);
		pButton = (CButton *)GetDlgItem(IDC_REFREASH);
		pButton->ModifyStyle(0,WS_VISIBLE,0);
		m_WebAddr.ModifyStyle(0,WS_VISIBLE,0);
		SetWindowPlacement(&m_OldWndplacement);
		RECT rc = {5, 30, 5, 5};
		rc.right = m_rc.right - m_rc.left + 15;
		rc.bottom = m_rc.bottom - m_rc.top + 30;
		GetDlgItem(IDC_EXPLORER)->MoveWindow(&rc,TRUE);
	}
	this->RedrawWindow();
}


void CMywebDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	ReSizeWindow();
}

BEGIN_EVENTSINK_MAP(CMywebDlg, CDialog)
    //{{AFX_EVENTSINK_MAP(CMywebDlg)
	ON_EVENT(CMywebDlg, IDC_EXPLORER, 251 /* NewWindow2 */, OnNewWindow2Explorer, VTS_PDISPATCH VTS_PBOOL)
	ON_EVENT(CMywebDlg, IDC_EXPLORER_SUB, 250 /* BeforeNavigate2 */, OnBeforeNavigate2ExplorerSub, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	ON_EVENT(CMywebDlg, IDC_EXPLORER, 108 /* ProgressChange */, OnProgressChangeExplorer, VTS_I4 VTS_I4)
	ON_EVENT(CMywebDlg, IDC_EXPLORER, 113 /* TitleChange */, OnTitleChangeExplorer, VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CMywebDlg::OnNewWindow2Explorer(LPDISPATCH FAR* ppDisp, BOOL FAR* Cancel) 
{
	// TODO: Add your control notification handler code here
	*ppDisp = m_BroserSub.GetApplication();//将对象传递给ExplorerSub
}

void CMywebDlg::OnBeforeNavigate2ExplorerSub(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel) 
{
	// TODO: Add your control notification handler code here
	CString tmpstr = _T("");
	tmpstr = URL->bstrVal;
	m_Broser.Navigate(tmpstr,NULL,NULL,NULL,NULL);
	*Cancel = true;
}

void CMywebDlg::OnBack() 
{
	// TODO: Add your control notification handler code here
	m_Broser.GoBack();
}

void CMywebDlg::OnForword() 
{
	// TODO: Add your control notification handler code here
	m_Broser.GoForward();
}

void CMywebDlg::OnRefreash() 
{
	// TODO: Add your control notification handler code here
	m_Broser.Refresh();
}

void CMywebDlg::OnSelchangeComboWebaddr() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int index = m_WebAddr.GetCurSel();
	SetWindowText(m_cfg->m_pageName[index]);
	m_Broser.Navigate(m_cfg->m_urlName[index],NULL,NULL,NULL,NULL);	
}

void CMywebDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnTimer(nIDEvent);
}

void CMywebDlg::OnProgressChangeExplorer(long Progress, long ProgressMax) 
{
	// TODO: Add your control notification handler code here
	if (ProgressMax > 5)
	{
		m_Progress.ShowWindow(SW_SHOW);
		m_Progress.SetPos(Progress);
	}
	else
	{
		m_Progress.ShowWindow(SW_HIDE);
	}
}

void CMywebDlg::OnTitleChangeExplorer(LPCTSTR Text) 
{
	// TODO: Add your control notification handler code here
	SetWindowText(Text);
	m_WebAddr.SetWindowText(m_Broser.GetLocationURL());
}

BOOL CMywebDlg::DestroyWindow() 
{
	return CDialog::DestroyWindow();
}

void CMywebDlg::ReSizeWindow()
{
	CWnd *hWin = GetDlgItem(IDC_EXPLORER);
	if(hWin != NULL)
	{
		RECT rect;
		RECT rc;
		GetWindowRect(&rc);
		rect.top = rc.top+60;
		rect.bottom = rc.bottom - 10;
		rect.left = rc.left + 10;
		rect.right = rc.right - 10;
		ScreenToClient(&rect);
		hWin->MoveWindow(&rect);		
	}
}


void CMywebDlg::OnBnClickedFavorite()
{
	// TODO: Add your control notification handler code here
	/*
	for(int i = 0 ; i <= m_urlIndex; i++)
		if(strstr((char*)Text, (char*)(m_cfg->m_pageName[i])))
			return;
	*/
	if (m_urlIndex < MAX_SUPPORT_FAVORITE_KEY)
	{
		CString tmpStr = _T("");
		strcpy(m_cfg->m_urlName[m_urlIndex], (char*)(m_Broser.GetLocationURL().GetBuffer(0)));
		strcpy(m_cfg->m_pageName[m_urlIndex], (char*)(m_Broser.GetLocationName().GetBuffer(0)));
		tmpStr.Format(_T("%s  %s"),m_cfg->m_pageName[m_urlIndex],  m_cfg->m_urlName[m_urlIndex]);
		m_WebAddr.InsertString(m_urlIndex,tmpStr);
		m_cfg->SaveConfig(m_cfg->m_strXmlFileName);
		m_urlIndex++;
	}
	else 
	{
		m_urlIndex = 3;
	}
}
