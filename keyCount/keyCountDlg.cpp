// keyCountDlg.cpp : implementation file
//

#include "stdafx.h"
#include "keyCount.h"
#include "keyCountDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKeyCountDlg dialog

CKeyCountDlg::CKeyCountDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKeyCountDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKeyCountDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CKeyCountDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKeyCountDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CKeyCountDlg, CDialog)
	//{{AFX_MSG_MAP(CKeyCountDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKeyCountDlg message handlers
HHOOK g_hHook = NULL; 
BOOL  g_bBeep = FALSE;
HINSTANCE g_hInstance;
HWND g_hWnd;
int g_totalKeyCount = 0;
DWORD g_CurvkCode = 0;
HHOOK g_hHookMouse = NULL;
bool g_bAction = false;

int g_keyCount[VK_OEM_CLEAR+1];
LRESULT CALLBACK TaskKeyHookPro(int nCode, WPARAM wp, LPARAM lp)
{
	KBDLLHOOKSTRUCT *pkh = (KBDLLHOOKSTRUCT *) lp;
	
	if (nCode==HC_ACTION) 
	{
		g_totalKeyCount++;
		if (g_totalKeyCount%2)
		{
			g_bAction = true;
			g_keyCount[pkh->vkCode]++;
		}
		g_CurvkCode = pkh->vkCode;
		BOOL bCtrlKeyDown = GetAsyncKeyState(VK_CONTROL)>>((sizeof(SHORT) * 8) - 1);
		
		if (bCtrlKeyDown && pkh->vkCode == VK_LWIN)
		{ 
			ShowWindow(g_hWnd,SW_SHOW);
		}
	}
	return CallNextHookEx(g_hHook, nCode, wp, lp);
}

BOOL AreTaskKeysDisabled()
{
	return g_hHook != NULL;
}

BOOL CapKey(BOOL bDisable, BOOL bBeep)
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


//////////////////////////////////////////////////////////////////////////mouse
int g_leftButtonDown = 0;
int g_rightButtonDown = 0;
int g_MouseWheel = 0;
int g_MidBUttonDown = 0;
int g_Mouse = 0;
LRESULT CALLBACK MouseHookPro(int nCode, WPARAM wp, LPARAM lp)
{
	if (nCode==HC_ACTION) 
	{
		if (wp == WM_LBUTTONDOWN)
		{
			g_leftButtonDown++;
			g_Mouse = g_leftButtonDown + g_rightButtonDown + g_MouseWheel + g_MidBUttonDown;
			g_bAction = true;
		}
		if(wp == WM_RBUTTONDOWN)
		{
			g_rightButtonDown++;
			g_Mouse = g_leftButtonDown + g_rightButtonDown + g_MouseWheel + g_MidBUttonDown;
			g_bAction = true;
		}
		if(wp == WM_MOUSEWHEEL)
		{
			g_MouseWheel++;
			g_Mouse = g_leftButtonDown + g_rightButtonDown + g_MouseWheel + g_MidBUttonDown;
			g_bAction = true;
		}
		if(wp == WM_MBUTTONDOWN)
		{
			g_MidBUttonDown++;
			g_Mouse = g_leftButtonDown + g_rightButtonDown + g_MouseWheel + g_MidBUttonDown;
			g_bAction = true;
		}
	}
	return CallNextHookEx(g_hHookMouse, nCode, wp, lp);
}

BOOL CapMouse()
{
	if (!g_hHookMouse)
	{
		g_hHookMouse = SetWindowsHookEx(WH_MOUSE_LL,
			MouseHookPro, 
			g_hInstance, 
			NULL);
	}
		
	return true;
}
BOOL CKeyCountDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	g_hWnd = m_hWnd;
	g_hInstance = m_hInstanceTmp;
	memset(&g_keyCount, 0, sizeof(g_keyCount));
	SetTimer(1, 40, NULL);
	CapKey(1, 1);
	CapMouse();
	GetLocalTime(&m_sysTime);
	SetTimer(2, 100, NULL);
	SetTimer(3, 60*60*1000, NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CKeyCountDlg::OnPaint() 
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
HCURSOR CKeyCountDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CKeyCountDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE) 
		ShowWindow(SW_HIDE);
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return true;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return true;
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		ModifyStyle(WS_CAPTION,0,SWP_FRAMECHANGED);//Òþ²Ø
		MoveWindowToEage();
	}
	if (pMsg->message == WM_RBUTTONDOWN)
	{
		ModifyStyle(0, WS_CAPTION, SWP_FRAMECHANGED);//ÏÔÊ¾
		MoveTitle();
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CKeyCountDlg::ShowKeyTimes()
{
	char Str[100];
	char WindowText[50];
	CString keyNameStr;
	/* VK_0 thru VK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) */
	/* VK_A thru VK_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) */
	if ((g_CurvkCode >= 0x30 && g_CurvkCode <= 0x39) ||
		(g_CurvkCode >= 0x41 && g_CurvkCode <= 0x5a))
	{
		sprintf(Str, "Mouse:%d L:%d R:%d M:%d W:%d\nKey:%d Curr:%#x->VK_%c",
			g_Mouse,
			g_leftButtonDown,
			g_rightButtonDown,
			g_MidBUttonDown,
			g_MouseWheel,
			g_totalKeyCount/2, 
			g_CurvkCode,
			g_CurvkCode);
		sprintf(WindowText, "VK_%c",g_CurvkCode);
	}
	else
	{
		keyNameStr = FindStr(g_CurvkCode);
		sprintf(Str, "Mouse:%d L:%d R:%d M:%d W:%d\nKey:%d Curr:%#x->%s",
			g_Mouse,
			g_leftButtonDown,
			g_rightButtonDown,
			g_MidBUttonDown,
			g_MouseWheel,
			g_totalKeyCount/2, 
			g_CurvkCode,
			keyNameStr);
		sprintf(WindowText, "%s",keyNameStr);
	}
	GetDlgItem(IDC_STATIC_DISP)->SetWindowText(Str);
	SetWindowText(WindowText);
}

void CKeyCountDlg::OnTimer(UINT nIDEvent)  
{
	// TODO: Add your message handler code here and/or call default
	if (g_bAction && nIDEvent == 1)
	{
		ShowKeyTimes();
		g_bAction = false;
	}
	if (nIDEvent == 2)
	{
		KillTimer(2);
		ModifyStyle(WS_CAPTION,0,SWP_FRAMECHANGED);//Òþ²Ø
		MoveWindowToEage();
	}
	if (nIDEvent == 3)
	{
		WriteResultToFile();
	}
	CDialog::OnTimer(nIDEvent);
}

void CKeyCountDlg::MoveWindowToEage()
{
	RECT Rc = {0};
	//GetWindowRect(&Rc);
	GetDlgItem(IDC_STATIC_DISP)->GetWindowRect(&Rc);
	int SysMetrics_x = GetSystemMetrics(SM_CXSCREEN);
	Rc.bottom = Rc.bottom - Rc.top;
	Rc.top = 0;
	Rc.left = (SysMetrics_x - (Rc.right - Rc.left))/2;
	Rc.right = (SysMetrics_x + (Rc.right - Rc.left))/2;
	MoveWindow(&Rc, TRUE);
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
}

void CKeyCountDlg::MoveTitle()
{
	RECT Rc = {0};
	GetDlgItem(IDC_STATIC_DISP)->GetWindowRect(&Rc);
	int SysMetrics_x = GetSystemMetrics(SM_CXSCREEN);
	Rc.bottom = 2*(Rc.bottom - Rc.top);
	Rc.top = 0;
	Rc.left = (SysMetrics_x - (Rc.right - Rc.left))/2;
	Rc.right = (SysMetrics_x + (Rc.right - Rc.left))/2;
	MoveWindow(&Rc, TRUE);
}
CString CKeyCountDlg::FindStr(int vkCode)
{
	CString keyNameStr = _T("");
	switch (vkCode)
	{
	case VK_LBUTTON :
		keyNameStr = "VK_LBUTTON";
		break;
	case VK_RBUTTON :
		keyNameStr = "VK_RBUTTON";
		break;
	case VK_CANCEL : 
		keyNameStr = "VK_CANCEL";
		break;
	case VK_MBUTTON:
		keyNameStr = "VK_MBUTTON";
		break;
	case VK_BACK   : 
		keyNameStr = "VK_BACK";
		break;
	case VK_TAB    :  
		keyNameStr = "VK_TAB";
		break;
	case VK_CLEAR :   
		keyNameStr = "VK_CLEAR";
		break;
	case VK_RETURN  :
		keyNameStr = "VK_RETURN";
		break;
	case VK_SHIFT   : 
		keyNameStr = "VK_SHIFT";
		break;
	case VK_CONTROL :
		keyNameStr = "VK_CONTROL";
		break;
	case VK_MENU    :
		keyNameStr = "VK_MENU";
		break;
	case VK_PAUSE   : 
		keyNameStr = "VK_PAUSE";
		break;
	case VK_CAPITAL  :
		keyNameStr = "VK_CAPITAL";
		break;
	case VK_KANA    :
		keyNameStr = "VK_KANA";
		break;
	case VK_JUNJA   : 
		keyNameStr = "VK_JUNJA";
		break;
	case VK_FINAL   : 
		keyNameStr = "VK_FINAL";
		break;
	case VK_HANJA  : 
		keyNameStr = "VK_HANJA";
		break;
	case VK_ESCAPE  : 
		keyNameStr = "VK_ESCAPE";
		break;
	case VK_CONVERT :
		keyNameStr = "VK_CONVERT";
		break;
	case VK_NONCONVERT : 
		keyNameStr = "VK_NONCONVERT";
		break;
	case VK_ACCEPT  :     
		keyNameStr = "VK_ACCEPT";
		break;
	case VK_MODECHANGE :
		keyNameStr = "VK_MODECHANGE";
		break;
	case VK_SPACE   :      
		keyNameStr = "VK_SPACE";
		break;
	case VK_PRIOR   :      
		keyNameStr = "VK_PRIOR";
		break;
	case VK_NEXT   :      
		keyNameStr = "VK_NEXT";
		break;
	case VK_END   :       
		keyNameStr = "VK_END";
		break;
	case VK_HOME   :     
		keyNameStr = "VK_HOME";
		break;
	case VK_LEFT   :       
		keyNameStr = "VK_LEFT";
		break;
	case VK_UP    :       
		keyNameStr = "VK_UP";
		break;
	case VK_RIGHT  :       
		keyNameStr = "VK_RIGHT";
		break;
	case VK_DOWN   :     
		keyNameStr = "VK_DOWN";
		break;
	case VK_SELECT  :      
		keyNameStr = "VK_SELECT";
		break;
	case VK_PRINT   :     
		keyNameStr = "VK_PRINT";
		break;
	case VK_EXECUTE   :   
		keyNameStr = "VK_EXECUTE";
		break;
	case VK_SNAPSHOT  :
		keyNameStr = "VK_SNAPSHOT";
		break;
	case VK_INSERT :   
		keyNameStr = "VK_INSERT";
		break;
	case VK_DELETE  :   
		keyNameStr = "VK_DELETE";
		break;
	case VK_HELP  :     
		keyNameStr = "VK_HELP";
		break;
	case VK_LWIN   :    
		keyNameStr = "VK_LWIN";
		break;
	case VK_RWIN  :    
		keyNameStr = "VK_RWIN";
		break;
	case VK_APPS   :    
		keyNameStr = "VK_APPS";
		break;
	case VK_NUMPAD0  :
		keyNameStr = "VK_NUMPAD0";
		break;
	case VK_NUMPAD1  :
		keyNameStr = "VK_NUMPAD1";
		break;
	case VK_NUMPAD2  :
		keyNameStr = "VK_NUMPAD2";
		break;
	case VK_NUMPAD3  :
		keyNameStr = "VK_NUMPAD3";
		break;
	case VK_NUMPAD4  :
		keyNameStr = "VK_NUMPAD4";
		break;
	case VK_NUMPAD5 : 
		keyNameStr = "VK_NUMPAD5";
		break;
	case VK_NUMPAD6  :
		keyNameStr = "VK_NUMPAD6";
		break;
	case VK_NUMPAD7  :
		keyNameStr = "VK_NUMPAD7";
		break;
	case VK_NUMPAD8  :
		keyNameStr = "VK_NUMPAD8";
		break;
	case VK_NUMPAD9  :
		keyNameStr = "VK_NUMPAD9";
		break;
	case VK_MULTIPLY   :
		keyNameStr = "VK_MULTIPLY";
		break;
	case VK_ADD       :
		keyNameStr = "VK_ADD";
		break;
	case VK_SEPARATOR :
		keyNameStr = "VK_SEPARATOR";
		break;
	case VK_SUBTRACT  :
		keyNameStr = "VK_SUBTRACT";
		break;
	case VK_DECIMAL  : 
		keyNameStr = "VK_DECIMAL";
		break;
	case VK_DIVIDE  :   
		keyNameStr = "VK_DIVIDE";
		break;
	case VK_F1     :    
		keyNameStr = "VK_F1";
		break;
	case VK_F2     :    
		keyNameStr = "VK_F2";
		break;
	case VK_F3     :    
		keyNameStr = "VK_F3";
		break;
	case VK_F4      :   
		keyNameStr = "VK_F4";
		break;
	case VK_F5       :  
		keyNameStr = "VK_F5";
		break;
	case VK_F6      :   
		keyNameStr = "VK_F6";
		break;
	case VK_F7       :  
		keyNameStr = "VK_F7";
		break;
	case VK_F8     :    
		keyNameStr = "VK_F8";
		break;
	case VK_F9      :   
		keyNameStr = "VK_F9";
		break;
	case VK_F10     :   
		keyNameStr = "VK_F10";
		break;
	case VK_F11    :    
		keyNameStr = "VK_F11";
		break;
	case VK_F12     :   
		keyNameStr = "VK_F12";
		break;
	case VK_F13      :  
		keyNameStr = "VK_F13";
		break;
	case VK_F14     :   
		keyNameStr = "VK_F14";
		break;
	case VK_F15      :  
		keyNameStr = "VK_F15";
		break;
	case VK_F16     :   
		keyNameStr = "VK_F16";
		break;
	case VK_F17   :     
		keyNameStr = "VK_F17";
		break;
	case VK_F18    :    
		keyNameStr = "VK_F18";
		break;
	case VK_F19   :     
		keyNameStr = "VK_F19";
		break;
	case VK_F20    :      
		keyNameStr = "VK_F20";
		break;
	case VK_F21    :      
		keyNameStr = "VK_F21";
		break;
	case VK_F22   :       
		keyNameStr = "VK_F22";
		break;
	case VK_F23   :       
		keyNameStr = "VK_F23";
		break;
	case VK_F24     :     
		keyNameStr = "VK_F24";
		break;
	case VK_NUMLOCK  :  
		keyNameStr = "VK_NUMLOCK";
		break;
	case VK_SCROLL    :   
		keyNameStr = "VK_SCROLL";
		break;
	case VK_LSHIFT   :     
		keyNameStr = "VK_LSHIFT";
		break;
	case VK_RSHIFT  :     
		keyNameStr = "VK_RSHIFT";
		break;
	case VK_LCONTROL  :  
		keyNameStr = "VK_LCONTROL";
		break;
	case VK_RCONTROL  :  
		keyNameStr = "VK_RCONTROL";
		break;
	case VK_LMENU   :    
		keyNameStr = "VK_LMENU";
		break;
	case VK_RMENU  :     
		keyNameStr = "VK_RMENU";
		break;
	case VK_BROWSER_BACK :
		keyNameStr = "VK_BROWSER_BACK";
		break;
	case VK_BROWSER_FORWARD :
		keyNameStr = "VK_BROWSER_FORWARD";
		break;
	case VK_BROWSER_REFRESH :
		keyNameStr = "VK_BROWSER_REFRESH";
		break;
	case VK_BROWSER_STOP :
		keyNameStr = "VK_BROWSER_STOP";
		break;
	case VK_BROWSER_SEARCH :
		keyNameStr = "VK_BROWSER_SEARCH";
		break;
	case VK_BROWSER_FAVORITES :
		keyNameStr = "VK_BROWSER_FAVORITES";
		break;
	case VK_BROWSER_HOME :
		keyNameStr = "VK_BROWSER_HOME";
		break;
	case VK_VOLUME_MUTE :
		keyNameStr = "VK_VOLUME_MUTE";
		break;
	case VK_VOLUME_DOWN :
		keyNameStr = "VK_VOLUME_DOWN";
		break;
	case VK_VOLUME_UP :
		keyNameStr = "VK_VOLUME_UP";
		break;
	case VK_MEDIA_NEXT_TRACK :
		keyNameStr = "VK_MEDIA_NEXT_TRACK";
		break;
	case VK_MEDIA_PREV_TRACK :
		keyNameStr = "VK_MEDIA_PREV_TRACK";
		break;
	case VK_MEDIA_STOP :
		keyNameStr = "VK_MEDIA_STOP";
		break;
	case VK_MEDIA_PLAY_PAUSE :
		keyNameStr = "VK_MEDIA_PLAY_PAUSE";
		break;
	case VK_LAUNCH_MAIL :
		keyNameStr = "VK_LAUNCH_MAIL";
		break;
	case VK_LAUNCH_MEDIA_SELECT:
		keyNameStr = "VK_LAUNCH_MEDIA_SELECT";
		break;
	case VK_LAUNCH_APP1 :
		keyNameStr = "VK_LAUNCH_APP1";
		break;
	case VK_LAUNCH_APP2 :
		keyNameStr = "VK_LAUNCH_APP2";
		break;
	case VK_PROCESSKEY :  
		keyNameStr = "VK_PROCESSKEY";
		break;
	case VK_ATTN   :      
		keyNameStr = "VK_ATTN";
		break;
	case VK_CRSEL   :      
		keyNameStr = "VK_CRSEL";
		break;
	case VK_EXSEL  :       
		keyNameStr = "VK_EXSEL";
		break;
	case VK_EREOF :      
		keyNameStr = "VK_EREOF";
		break;
	case VK_PLAY   :       
		keyNameStr = "VK_PLAY";
		break;
	case VK_ZOOM   :     
		keyNameStr = "VK_ZOOM";
		break;
	case VK_NONAME  :    
		keyNameStr = "VK_NONAME";
		break;
	case VK_PA1    :      
		keyNameStr = "VK_PA1";
		break;
	case VK_OEM_CLEAR  :
		keyNameStr = "VK_OEM_CLEAR";
		break;
	default:
		keyNameStr.Format("UNKNOWN_%d", vkCode);
		break;
	}
	return keyNameStr;
}

HBRUSH CKeyCountDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	int r = int(rand()*0xff/0x7fff);
	int g = int(rand()*0xff/0x7fff);
	int b = int(rand()*0xff/0x7fff);
	pDC->SetTextColor(RGB(r, g, b));
	//pDC->SetBkColor(RGB(b, g, r));
	return hbr;
}

BOOL CKeyCountDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	WriteResultToFile();
	return CDialog::DestroyWindow();
}
void CKeyCountDlg::WriteResultToFile(void)
{
	FILE *fp = NULL;
	CString keyNameStr;
	if ((fp = fopen("history.txt","w")) != NULL)
	{	
		char str[200] = {NULL};		
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);
		sprintf(str, "From: %d-%02d-%02d %02d:%02d:%02d.%03d To %d-%02d-%02d %02d:%02d:%02d.%03d\n",
			m_sysTime.wYear,
			m_sysTime.wMonth,
			m_sysTime.wDay,
			m_sysTime.wHour,
			m_sysTime.wMinute,
			m_sysTime.wSecond,
			m_sysTime.wMilliseconds,			
			sysTime.wYear,
			sysTime.wMonth,
			sysTime.wDay,
			sysTime.wHour,
			sysTime.wMinute,
			sysTime.wSecond,
			sysTime.wMilliseconds);
		fwrite(str, 1, strlen(str), fp);
		sprintf(str, "mouse total: %d left: %d right:%d middle: %d, wheel: %d \n",
			g_Mouse,
			g_leftButtonDown,
			g_rightButtonDown,
			g_MidBUttonDown,
			g_MouseWheel);
		fwrite(str, 1, strlen(str), fp);
		sprintf(str, "key total: %d\n",g_totalKeyCount/2);
		fwrite(str, 1, strlen(str), fp);
		for (int i =0; i < VK_OEM_CLEAR+1; i++)
		{
			if ((i >= 0x30 && i <= 0x39) ||
				(i >= 0x41 && i <= 0x5a))
			{
				sprintf(str, "%03d VK_%c: %d\n",i, i, g_keyCount[i]);
			}
			else
			{
				keyNameStr = FindStr(i);
				sprintf(str, "%03d %.22s: %d\n",i, keyNameStr.GetBuffer(0), g_keyCount[i]);
			}
			fwrite(str, 1, strlen(str), fp);
		}
		fclose(fp);
	}
}