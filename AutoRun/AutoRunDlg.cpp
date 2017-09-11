// AutoRunDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AutoRun.h"
#include "AutoRunDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoRunDlg dialog

CAutoRunDlg::CAutoRunDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoRunDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAutoRunDlg)
	m_hour = 0;
	m_minute = 0;
	m_second = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAutoRunDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoRunDlg)
	DDX_Text(pDX, IDC_HOUR, m_hour);
	DDV_MinMaxUInt(pDX, m_hour, 0, 24);
	DDX_Text(pDX, IDC_MINUTE, m_minute);
	DDV_MinMaxUInt(pDX, m_minute, 0, 60);
	DDX_Text(pDX, IDC_SECOND, m_second);
	DDV_MinMaxUInt(pDX, m_second, 0, 60);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAutoRunDlg, CDialog)
	//{{AFX_MSG_MAP(CAutoRunDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SIGNIN, OnSignin)
	ON_BN_CLICKED(IDC_SIGNOUT, OnSignout)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_SHUTDOWN, OnShutdown)
	ON_BN_CLICKED(IDC_HIDE, OnHide)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoRunDlg message handlers
HHOOK g_hHook = NULL; 
BOOL  g_bBeep = FALSE;
HINSTANCE g_hInstance;
HWND g_hWnd;
LRESULT CALLBACK TaskKeyHookPro(int nCode, WPARAM wp, LPARAM lp)
{
	KBDLLHOOKSTRUCT *pkh = (KBDLLHOOKSTRUCT *) lp;
	
	if (nCode==HC_ACTION) 
	{
		BOOL bCtrlKeyDown = GetAsyncKeyState(VK_CONTROL)>>((sizeof(SHORT) * 8) - 1);
		
		if (bCtrlKeyDown && pkh->vkCode == VK_LWIN)
		{ 
			ShowWindow(g_hWnd,SW_SHOW);
			return 1; 
		}
	}
	return CallNextHookEx(g_hHook, nCode, wp, lp);
}

BOOL AreTaskKeysDisabled()
{
	return g_hHook != NULL;
}

BOOL DisableTaskKey(BOOL bDisable, BOOL bBeep)
{
	if (bDisable) 
	{
		if (!g_hHook)
		{
			g_hHook = SetWindowsHookEx(WH_KEYBOARD_LL,
				TaskKeyHookPro, 
				g_hInstance, 
				NULL);
		}
		
	} 
	else if (g_hHook != NULL)
	{
		UnhookWindowsHookEx(g_hHook);
		g_hHook = NULL;
	}
	g_bBeep = bBeep;
	
	return AreTaskKeysDisabled();
}

BOOL CAutoRunDlg::OnInitDialog()
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	SetTimer(1, 40, NULL);
	SetTimer(2, 10, NULL);
	m_bin = FALSE;
	m_bout = FALSE;
	m_bshut = FALSE;
	m_CurTime = 0;
	g_hWnd = m_hWnd;
	g_hInstance = m_hInstanceTmp;
	DisableTaskKey(1, 1);
	GetDlgItem(IDC_HOUR)->SetWindowText("20");
	GetDlgItem(IDC_MINUTE)->SetWindowText("24");
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAutoRunDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CAutoRunDlg::OnPaint() 
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
HCURSOR CAutoRunDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAutoRunDlg::OnSignin() 
{
	// TODO: Add your control notification handler code here
	m_bin = TRUE;
}

void CAutoRunDlg::OnSignout() 
{
	// TODO: Add your control notification handler code here
	m_bout = TRUE;
}
//检测并修改屏幕分辨率
// 	int nFullWidth = GetSystemMetrics(SM_CXSCREEN);   
// 	int nFullHeight = GetSystemMetrics(SM_CYSCREEN);   
// 	bool bChanged = 0;   
// 	if(nFullWidth != 1024 || nFullHeight!=768)//change to 1280*1024
// 	{   
// 		DEVMODE lpDevMode;  
// 		lpDevMode.dmBitsPerPel = 16;
// 		lpDevMode.dmPelsWidth = 800;   
// 		lpDevMode.dmPelsHeight = 600;   
// 		lpDevMode.dmDisplayFrequency = 85;   
// 		lpDevMode.dmSize = sizeof(DEVMODE);   
// 		lpDevMode.dmFields = DM_PELSWIDTH|DM_PELSHEIGHT|DM_BITSPERPEL|DM_DISPLAYFREQUENCY;
// 		//int iRet = EnumDisplaySettings(NULL,NULL,&lpDevMode);
// 		//int iRet = EnumDisplaySettings(NULL,ENUM_CURRENT_SETTINGS,&lpDevMode);
// 		int iRet = ChangeDisplaySettings(&lpDevMode, 0);
// 		if(iRet == DISP_CHANGE_SUCCESSFUL)   
// 		{   
// 			iRet = ChangeDisplaySettingsEx(NULL,&lpDevMode,NULL,CDS_UPDATEREGISTRY,NULL); 
// 			bChanged = 1;   
// 		}   
// 		else   
// 		{   
// 			iRet = ChangeDisplaySettingsEx(NULL,&lpDevMode,NULL,0, NULL);   
// 		}   
// 	}
void CAutoRunDlg::OnShutdown() 
{
	// TODO: Add your control notification handler code here
	m_bshut = TRUE;

}

void CAutoRunDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 2)
	{
		KillTimer(2);
		ShowWindow(SW_HIDE);
		UpdateData();
		HKEY Key;
		DWORD dwDisp;
		CString sKeyPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
		CString softpath = "C:\\WINDOWS\\system32\\auto.exe";
		RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
			sKeyPath,
			0,
			KEY_ALL_ACCESS,
			&Key);
		if(
			RegCreateKeyEx(HKEY_LOCAL_MACHINE,
			sKeyPath,
			0,
			NULL,
			REG_OPTION_NON_VOLATILE,
			KEY_ALL_ACCESS,
			NULL,
			&Key,
			&dwDisp) == ERROR_SUCCESS
			)
		{
			RegCreateKey(HKEY_CURRENT_USER,sKeyPath,&Key);
			RegSetValueEx(Key,"AutoRun",0,REG_SZ,(CONST BYTE*)softpath.GetBuffer(0),softpath.GetLength());
			//RegDeleteValue( Key, "AutoRun");
			RegCloseKey(Key);
	}
	}
	CTime time = CTime::GetCurrentTime();
	UpdateData(TRUE);
	if (((double)time.GetMinute() - m_CurTime) >5 )
	{
		m_CurTime = (double)time.GetMinute();
		keybd_event(VK_NUMLOCK,0,0,0);
		keybd_event(VK_NUMLOCK,0,KEYEVENTF_KEYUP,0);
	}
	while (((double)time.GetHour() == m_hour)&&((double)time.GetMinute() == m_minute))
	{
//		POINT lpPoint;
#if 0   //
		SetCursorPos(117, 849);  //显示桌面
#endif
		SetCursorPos(114,1008);
		Sleep(40);
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
		Sleep(4000);
#if 0
		SetCursorPos(40, 245);  //IE
#endif
		SetCursorPos(37, 246);
		Sleep(40);
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
		Sleep(10000);
#if 0
		SetCursorPos(608, 389); //kuang
#endif
		SetCursorPos(657, 387);
		Sleep(40);
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
		Sleep(1000);
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
		Sleep(1000);
		keybd_event(VK_DOWN, 0, 0, 0); // down
		keybd_event(VK_DOWN, 0, KEYEVENTF_KEYUP, 0);
		Sleep(1000); //enter
		keybd_event(VK_RETURN, 0, 0, 0);
		keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
		Sleep(1000); //enter
		keybd_event(VK_RETURN, 0, 0, 0);
		keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
		Sleep(10000); //
#if 0
		SetCursorPos(62, 424); 
#endif
		SetCursorPos(67, 426);
		Sleep(40);
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
		Sleep(3000); //
#if 0
		SetCursorPos(88, 340); 
#endif
		SetCursorPos(87, 339);
		Sleep(40);
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
		Sleep(5000);
		if (m_bout)
		{
#if 0
			SetCursorPos(993, 219); 
#endif
			SetCursorPos(1119, 218);
			Sleep(40);
			mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
			mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
			mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
			mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
			Sleep(4000);
		}
		if (m_bin)
		{
#if 0
			SetCursorPos(947, 220); 
#endif
			SetCursorPos(1078, 220);
			Sleep(40);
			mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
			mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
			mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
			mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
			Sleep(4000);
		}
#if 0
		SetCursorPos(1140, 11); 
#endif
		SetCursorPos(1267, 12);
		Sleep(40);
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
		Sleep(1000);

		//      此处通过键盘事件实现
		if (m_bshut)
		{
			keybd_event(VK_LWIN,0,0,0); 
			keybd_event(VK_LWIN,0,KEYEVENTF_KEYUP,0); 
			Sleep(40);
			keybd_event(VK_UP, 0, 0, 0);
			keybd_event(VK_UP, 0, KEYEVENTF_KEYUP, 0);
			Sleep(40);
			keybd_event(VK_RETURN, 0, 0, 0);
			keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);
			Sleep(80);
			keybd_event(VK_TAB, 0, 0, 0);
			keybd_event(VK_TAB, 0, KEYEVENTF_KEYUP, 0);	

			Sleep(80);
			keybd_event(VK_RETURN, 0, 0, 0);
 			keybd_event(VK_RETURN, 0, KEYEVENTF_KEYUP, 0);	
 		}
	exit(-1);
	}
	
	CDialog::OnTimer(nIDEvent);
}


BOOL CAutoRunDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE) 
	{
		ShowWindow(SW_HIDE);
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CAutoRunDlg::OnHide() 
{
	// TODO: Add your control notification handler code here
	ShowWindow(SW_HIDE);
}
