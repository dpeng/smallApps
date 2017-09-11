// KeyMouseDlg.cpp : implementation file
//

#include "stdafx.h"
#include "KeyMouse.h"
#include "KeyMouseDlg.h"

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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

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


// CKeyMouseDlg dialog




CKeyMouseDlg::CKeyMouseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyMouseDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKeyMouseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CKeyMouseDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CKeyMouseDlg message handlers
HHOOK g_hHook = NULL; 
BOOL  g_bBeep = FALSE;
HINSTANCE g_hInstance;
HWND g_hWnd;
int g_CurMode = 5;
int cur = 0;
LRESULT CALLBACK TaskKeyHookPro(int nCode, WPARAM wp, LPARAM lp)
{
	KBDLLHOOKSTRUCT *pkh = (KBDLLHOOKSTRUCT *) lp;

	if (nCode==HC_ACTION) 
	{
		BOOL bCtrlKeyDown = GetAsyncKeyState(VK_CONTROL)>>((sizeof(SHORT) * 8) - 1);
		if (bCtrlKeyDown)
		{
			switch (pkh->vkCode)
			{
			case VK_NUMPAD0:
				g_CurMode = 100;
				break;
			case VK_NUMPAD1:
				g_CurMode = 1;
				break;
			case VK_NUMPAD2:
				g_CurMode = 2;
				break;
			case VK_NUMPAD3:
				g_CurMode = 3;
				break;
			case VK_NUMPAD4:
				g_CurMode = 5;
				break;
			case VK_NUMPAD5:
				g_CurMode = 10;
				break;
			case VK_NUMPAD6:
				g_CurMode = 15;
				break;
			case VK_NUMPAD7:
				g_CurMode = 20;
				break;
			case VK_NUMPAD8:
				g_CurMode = 30;
				break;
			case VK_NUMPAD9:
				g_CurMode = 50;
				break;
			default:
				break;
			}
		}
		if (pkh->flags == 0)
		{
			bool NeedReturn = false;
			POINT pt = {0};
			GetCursorPos(&pt);
			switch (pkh->vkCode)
			{
			case VK_NUMPAD0:
				break;
			case VK_NUMPAD1:
				//mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
				//mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
				//mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
				//mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
				//NeedReturn = true;
				break;
			case VK_NUMPAD2:
				break;
			case VK_NUMPAD3:
				//mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
				//mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0); 
				//mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
				//mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0); 
				//NeedReturn = true;
				break;
			case VK_NUMPAD4:
				pt.x-=g_CurMode;
				SetCursorPos(pt.x, pt.y);
				NeedReturn = true;
				break;
			case VK_NUMPAD5:
				pt.y+=g_CurMode;
				SetCursorPos(pt.x, pt.y);
				NeedReturn = true;
				break;
			case VK_NUMPAD6:
				pt.x+=g_CurMode;
				SetCursorPos(pt.x, pt.y);
				NeedReturn = true;
				break;
			case VK_NUMPAD7:
				mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
				mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
				cur++;
				NeedReturn = true;
				break;
			case VK_NUMPAD8:
				pt.y-=g_CurMode;
				SetCursorPos(pt.x, pt.y);
				NeedReturn = true;
				break;
			case VK_NUMPAD9:
				mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
				mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0); 
				NeedReturn = true;
				break;
			default:
				break;
			}
			if (NeedReturn)
			{
				return 1;
			}
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
BOOL CKeyMouseDlg::OnInitDialog()
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
	g_hWnd = m_hWnd;
	g_hInstance = m_hInstanceTmp;
	DisableTaskKey(1,1);
	SetTimer(1, 40, NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CKeyMouseDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CKeyMouseDlg::OnPaint()
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
//  the minimized window.
HCURSOR CKeyMouseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CKeyMouseDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	KillTimer(nIDEvent);
	ShowWindow(SW_HIDE);
	CDialog::OnTimer(nIDEvent);
}
