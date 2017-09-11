// GetPointDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GetPoint.h"
#include "GetPointDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CGetPointDlg dialog




CGetPointDlg::CGetPointDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetPointDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGetPointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGetPointDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CGetPointDlg message handlers

BOOL CGetPointDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog. The framework does this automatically
	// when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	//SetTimer(1, 40, NULL);
	//ShowWindow(SW_SHOWMAXIMIZED);
	m_bIsFullScreen = FALSE;
	GetDlgItem(IDC_STATIC_FULLSRC)->GetWindowRect(&m_rc);
	GetWindowPlacement(&m_OldWndplacement);
	OnWndFullScreen();
	m_CurPointNr = 0;
	SetWindowTransParent();
	return TRUE; // return TRUE unless you set the focus to a control
}

void CGetPointDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
// to draw the icon. For MFC applications using the document/view model,
// this is automatically done for you by the framework.

void CGetPointDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
// the minimized window.
HCURSOR CGetPointDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//CGetPointApp theApp;
BOOL CGetPointDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_LBUTTONDBLCLK)
	{
		//OnWndFullScreen();
		FILE *fp = NULL;
		if (NULL != (fp = fopen("GetPoint.txt", "ab")))
		{
			POINT pt = {0};
			GetCursorPos(&pt);
			char TmpStr[64] = {0};
			sprintf_s(TmpStr, "%d. x:%d y:%d\n", m_CurPointNr++, pt.x, pt.y);
			fwrite(TmpStr, 1, strlen(TmpStr), fp);
			fclose(fp);
		}
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		theApp.m_pMainWnd->PostMessage(WM_CLOSE);
	}
	if (pMsg->message == 0xfff3)
	{
		SetWindowTransParent();
	}
	if (pMsg->message == WM_MOUSEMOVE && m_Msg.m_bCanShow)
	{
		POINT point = {0};
		GetCursorPos(&point);
		CString str; 
		str.Format("X:%d  Y:%d",point.x, point.y); 
		m_Msg.m_nCoordStrLen=(str.GetLength() + 1)*7; 
		m_Msg.ShowText(GetDC(), point, str);
		ReleaseDC(GetDC());
		Sleep(40);
	}
	return CDialog::PreTranslateMessage(pMsg);
}
void CGetPointDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	CDialog::OnTimer(nIDEvent);
}
void CGetPointDlg::OnWndFullScreen()
{
	m_bIsFullScreen = !m_bIsFullScreen;
	if(m_bIsFullScreen)
	{
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
		GetDlgItem(IDC_STATIC_FULLSRC)->MoveWindow(&rc,TRUE);
	}
	else
	{
		SetWindowPlacement(&m_OldWndplacement);
		GetDlgItem(IDC_STATIC_FULLSRC)->MoveWindow(&m_rc,TRUE);
	}
	this->RedrawWindow();
}
HBRUSH CGetPointDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	if(nCtlColor == CTLCOLOR_STATIC && pWnd->GetDlgCtrlID() == IDC_STATIC_FULLSRC)
	{ 
		//SetWindowTransParent();
	} 
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CGetPointDlg::SetWindowTransParent()
{
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED );
	return SetLayeredWindowAttributes(0,255/3,LWA_ALPHA);
}