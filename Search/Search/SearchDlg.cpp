// SearchDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Search.h"
#include "SearchDlg.h"
#include <shlwapi.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CSearchDlg dialog




CSearchDlg::CSearchDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSearchDlg::IDD, pParent)
	, m_msg(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSearchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MSG, m_msg);
	DDX_Control(pDX, IDC_DRAGNET, m_CtrlDragNet);
}

BEGIN_MESSAGE_MAP(CSearchDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_DRAGNET, &CSearchDlg::OnCbnSelchangeDragnet)
END_MESSAGE_MAP()


// CSearchDlg message handlers

HHOOK g_hHook = NULL; 
HINSTANCE g_hInstance;
HWND g_hWnd;
DWORD g_LastCtrl;
KBDLLHOOKSTRUCT *pkh;

LRESULT CALLBACK TaskKeyHookPro(int nCode, WPARAM wp, LPARAM lp)
{
	if (nCode==HC_ACTION) 
	{
		pkh = (KBDLLHOOKSTRUCT *) lp;
		BOOL bCtrlKeyDown = GetAsyncKeyState(VK_CONTROL)>>((sizeof(SHORT) * 8)- 1);
		if (bCtrlKeyDown && pkh->vkCode == 0x31)
			ShowWindow(g_hWnd, SW_SHOW);
		
		//if (pkh->flags && pkh->vkCode == VK_LCONTROL)
		//	g_LastCtrl = pkh->time;
		//if (pkh->flags == 0 && 
		//	pkh->vkCode == VK_LCONTROL && 
		//	(pkh->time- g_LastCtrl) < 500 &&
		//	(pkh->time - g_LastCtrl) >100)
		//{
		//	ShowWindow(g_hWnd, SW_SHOW);
		//}
	}
	return CallNextHookEx(g_hHook, nCode, wp, lp);
}

BOOL CapKey(BOOL bDisable)
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

	return g_hHook != NULL;;
}
BOOL CSearchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	g_hWnd = m_hWnd;
	g_hInstance = m_hInstanceTmp;
	CapKey(1);
	SetTimer(1, 40, NULL);
	g_LastCtrl = 0;
	memset(m_DefaultBrose, 0, sizeof(m_DefaultBrose));
	memcpy_s(m_DefaultBrose, sizeof(m_DefaultBrose), GetDefaultIE(), sizeof(m_DefaultBrose));
	//google
	m_CtrlDragNet.InsertString(0, _T("Google"));
	m_CtrlDragNet.SetItemData(0, SEARCH_GOOGLE);
	//baidu
	m_CtrlDragNet.InsertString(1, _T("Baidu"));
	m_CtrlDragNet.SetItemData(1, SEARCH_BAIDU);
	//bing
	m_CtrlDragNet.InsertString(2, _T("Bing"));
	m_CtrlDragNet.SetItemData(2, SEARCH_BING);
	//gougou
	m_CtrlDragNet.InsertString(3, _T("Gougou"));
	m_CtrlDragNet.SetItemData(3, SEARCH_GOUGOU);
	//dict
	m_CtrlDragNet.InsertString(4, _T("Dict"));
	m_CtrlDragNet.SetItemData(4, SEARCH_DICT);
	//codeplex
	m_CtrlDragNet.InsertString(5, _T("CodePlex"));
	m_CtrlDragNet.SetItemData(5, SECRCH_CODEPLEX);
	//codeproject
	m_CtrlDragNet.InsertString(6, _T("CodeProject"));
	m_CtrlDragNet.SetItemData(6, SEARCH_CODEPROJECT);
	m_nDragNet = SEARCH_GOOGLE;
	if (::SendMessage(m_CtrlDragNet.GetSafeHwnd(), CB_SELECTSTRING, (WPARAM)-1,
		(LPARAM)(LPCTSTR)"Google") == CB_ERR)
		m_CtrlDragNet.SetWindowText(_T("Google"));
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSearchDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialog::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSearchDlg::OnPaint()
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
		int x = (rect.Width()- cxIcon + 1) / 2;
		int y = (rect.Height()- cyIcon + 1) / 2;

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
HCURSOR CSearchDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


BOOL CSearchDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		Search();
		return true;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		ShowWindow(SW_HIDE);
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CreateLink (LPSTR szPath, LPSTR szLink)

{ 
	//HRESULT hres; 
	//IShellLink * psl; 
	//IPersistFile* ppf; 
	//WORD wsz[ MAX_PATH]; 
	//hres = CoCreateInstance( CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void **)&psl); 
	//if( FAILED( hres)) 
	//	return FALSE; 
	//psl->SetPath( szPath); 
	////psl->SetHotkey( MAKEWORD( 'R', 
	////	HOTKEYF_SHIFT |HOTKEYF_CONTROL)); 
	//hres = psl->QueryInterface( IID_IPersistFile, (void**)&ppf); 
	//if( FAILED( hres)) 
	//	return FALSE; 
	//MultiByteToWideChar( CP_ACP, 0, szLink,-1, 
	//	(LPWSTR)wsz, MAX_PATH); 
	//hres = ppf->Save( (LPCOLESTR)wsz, STGM_READWRITE); 
	//ppf->Release( );
	//psl->Release( );
	return TRUE; 

} 

void CSearchDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	KillTimer(nIDEvent);
	ShowWindow(SW_HIDE);
	CFont *m_font = this->GetFont();
	if (m_font)
	{
		LOGFONT lf;
		m_font->GetLogFont(&lf);
		TRACE("Typeface name of font = %s\n", lf.lfFaceName);
		lf.lfHeight= 25;
		HFONT hfont = ::CreateFontIndirect(&lf);	
		m_font= CFont::FromHandle(hfont);
		GetDlgItem(IDC_MSG)->SetFont(m_font);
	}
	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE); //将窗口置为最前端
	//char strFilename[MAX_PATH];
	//char strLinkname[MAX_PATH];
	//memset(strLinkname, 0, sizeof(strLinkname));
	//memset(strFilename, 0, sizeof(strFilename));
	//GetModuleFileName(m_hInstanceTmp,strFilename,MAX_PATH); 
	//memcpy_s(strLinkname, MAX_PATH, "C:\\Documents and Settings\\All Users\\「开始」菜单\\程序\\启动\\search.lnk", MAX_PATH);
	//GetTempPath(MAX_PATH, strLinkname);
	//SHGetSpecialFolderPath(NULL,(LPTSTR)strLinkname,CSIDL_PERSONAL,FALSE);
	//CreateLink(strFilename, strLinkname);
	CDialog::OnTimer(nIDEvent);
}

char* CSearchDlg::CopyClip()
{
	static char buffer[512];
	memset(buffer, 0, sizeof(buffer));
	if ( OpenClipboard() )
	{
		HANDLE hData = GetClipboardData(CF_TEXT);
		memcpy_s(buffer, sizeof(buffer), (char*)GlobalLock(hData), sizeof(buffer));
		GlobalUnlock(hData);
		CloseClipboard();
	}
	return buffer;
}

char* CSearchDlg::GetDefaultIE()
{
	HKEY hkRoot,hSubKey; //定义注册表根关键字及子关键字
	TCHAR ValueName[MAX_PATH];
	memset(ValueName, 0, sizeof(ValueName));
	char DataValue[MAX_PATH];
	memset(DataValue, 0, sizeof(DataValue));
	unsigned long cbValueName=MAX_PATH;
	unsigned long cbDataValue=MAX_PATH;
	DWORD dwType;
	static char tmpFilename[MAX_PATH];
	memset(tmpFilename, 0, sizeof(tmpFilename));

	if(RegOpenKey(HKEY_CLASSES_ROOT,NULL,&hkRoot)==ERROR_SUCCESS)
	{
		if(RegOpenKeyEx(hkRoot, _T("htmlfile\\shell\\open\\command"), 0, KEY_ALL_ACCESS, &hSubKey)==ERROR_SUCCESS)
		{ 
			RegEnumValue(hSubKey, 0, ValueName, &cbValueName, NULL, &dwType, (LPBYTE)DataValue, &cbDataValue);
			char* tmpStr = "";
			tmpStr = strstr(DataValue, ".exe");
			memcpy_s(tmpFilename, sizeof(tmpFilename), DataValue, strlen(DataValue)- strlen(tmpStr));
			strcat_s(tmpFilename, sizeof(tmpFilename), ".exe");
			TCHAR tmCh[MAX_PATH];
			memset(tmCh, 0, sizeof(tmCh));
			memcpy_s(tmCh, sizeof(tmCh), strstr(tmpFilename, "\""), sizeof(tmCh));
			CString tmStr = tmCh;
			tmStr.Remove('\"');
			memcpy_s(tmpFilename, sizeof(tmpFilename), tmStr.GetBuffer(0), strlen((char*)tmStr.GetBuffer(0)) + 1);
		}
		else
		{
			strcpy_s(tmpFilename, sizeof(tmpFilename), "iexplore.exe");
		}
	}
	else
	{
		strcpy_s(tmpFilename, sizeof(tmpFilename), "iexplore.exe");
	}
	RegCloseKey(hSubKey);
	RegCloseKey(hkRoot);
	return tmpFilename;
}
void CSearchDlg::OnCbnSelchangeDragnet()
{
	// TODO: Add your control notification handler code here
	m_nDragNet = (int)m_CtrlDragNet.GetItemData(m_CtrlDragNet.GetCurSel());
}

void CSearchDlg::Search()
{
	//%E5%BD%AD%E4%B8%9C%E9%A3%8E==彭东风
	UpdateData(true);
	CString tmpMsg = _T("");
	//m_msg = m_UrlEncode.URLEncode(m_msg);
	CString ddd;
	UrlEscape(m_msg, ddd, 0, 0);
	OutputDebugString(m_msg);
	switch (m_nDragNet)
	{
	case SEARCH_GOOGLE:
		tmpMsg.Format(_T("http://www.google.cn/search?hl=zh-CN&newwindow=1&q=%s&aq=f&oq="), m_msg);
		break;
	case SEARCH_BAIDU:
		tmpMsg.Format(_T("http://www.baidu.com/s?wd=%s"), m_msg);
		break;
	case SEARCH_BING:
		tmpMsg.Format(_T("http://cn.bing.com/search?q=%s&form=QBRE&filt=all&qs=n"), m_msg);
		break;
	case SEARCH_GOUGOU:
		tmpMsg.Format(_T("http://www.gougou.com/search?search=%s&id=1"), m_msg);
		break;
	case SEARCH_DICT:
		tmpMsg.Format(_T("http://dict.cn/%s"), m_msg);
		break;
	case SECRCH_CODEPLEX:
		tmpMsg.Format(_T("http://www.codeplex.com/site/search?projectSearchText=%s"), m_msg);
		break;
	case SEARCH_CODEPROJECT:
		tmpMsg.Format(_T("http://www.codeproject.com/info/search.aspx?artkw=%s"), m_msg);
		break;
	default:
		tmpMsg.Format(_T("http://www.google.cn/search?hl=zh-CN&newwindow=1&q=%s&aq=f&oq="), m_msg);
		break;
	}
	//ShellExecute(NULL,_T("open"),m_DefaultBrose,tmpMsg,_T(""),SW_SHOWNORMAL);
	m_msg = "";
	UpdateData(false);
	ShowWindow(SW_HIDE);
}