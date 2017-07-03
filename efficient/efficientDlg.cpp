// efficientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "efficient.h"
#include "efficientDlg.h"
#include "direct.h"
#include <shlwapi.h>
#include <tlhelp32.h>
#include <winuser.h>

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


// CefficientDlg dialog




CefficientDlg::CefficientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CefficientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CefficientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_LIST_KEY, m_shortCutKeySlave);
	DDX_Control(pDX, IDC_COMBO_LIST_KEY_MASTER, m_shortCutKeyMaster);
}

BEGIN_MESSAGE_MAP(CefficientDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_OPENFILE, &CefficientDlg::OnBnClickedOpenfile)
	ON_BN_CLICKED(IDC_OpenFolder, &CefficientDlg::OnBnClickedOpenfolder)
	ON_BN_CLICKED(IDC_SAVE, &CefficientDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_CANCLE, &CefficientDlg::OnBnClickedCancle)
	ON_CBN_SELCHANGE(IDC_COMBO_LIST_KEY, &CefficientDlg::OnCbnSelchangeComboListKey)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_RADIO_AUTOSTART, &CefficientDlg::OnBnClickedRadioAutostart)
	ON_CBN_SELCHANGE(IDC_COMBO_LIST_KEY_MASTER, &CefficientDlg::OnCbnSelchangeComboListKeyMaster)
END_MESSAGE_MAP()


HHOOK g_hHook = NULL; 
HINSTANCE g_hInstance;
HWND g_hWnd;
CConfiguration *g_configXml;
CeverythingDlg g_everythingDlg;

inline void openFileOrFolder(char* shellFileName, char* shellLPParameters)
{	
	if (PathIsDirectory(shellFileName))
	{
		if (strcmp(shellLPParameters, _T("unset")))
		{
			ShellExecute(g_hWnd,_T("explore"),shellFileName,shellLPParameters,_T(""),SW_SHOWNORMAL);
		} 
		else
		{
			ShellExecute(g_hWnd,_T("explore"),shellFileName,_T(""),_T(""),SW_SHOWNORMAL);
		}
	} 
	else
	{
		if(strstr(shellLPParameters, "runas"))
		{
			SHELLEXECUTEINFO shExecInfo;
			shExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
			shExecInfo.fMask = NULL;
			shExecInfo.hwnd = g_hWnd;
			shExecInfo.lpVerb = "runas";
			shExecInfo.lpFile = shellFileName;
			shExecInfo.lpParameters = NULL;
			shExecInfo.lpDirectory = NULL;
			shExecInfo.nShow = SW_SHOWNORMAL;
			shExecInfo.hInstApp = NULL;
			ShellExecuteEx(&shExecInfo);
		}
		else if (strcmp(shellLPParameters, _T("unset")))
		{
			ShellExecute(g_hWnd,_T("open"),shellFileName,shellLPParameters,_T(""),SW_SHOWNORMAL);
		} 
		else
		{
			ShellExecute(g_hWnd,_T("open"),shellFileName,_T(""),_T(""),SW_SHOWNORMAL);
		}
	}
}

inline void copyStringToClipboard(char* shellFileName, char* shellLPParameters)
{
	if(OpenClipboard(NULL))
	{  
		HGLOBAL   clipbuffer;  
		char   *   buffer;  
		EmptyClipboard();  
		clipbuffer   =   GlobalAlloc(GMEM_DDESHARE,   strlen(shellLPParameters)+1);  
		buffer   =   (char*)GlobalLock(clipbuffer);  
		strcpy_s(buffer, strlen(shellLPParameters)+1, shellLPParameters);  
		GlobalUnlock(clipbuffer);  
		SetClipboardData(CF_TEXT,clipbuffer);
		CloseClipboard();  
	}  
}
inline void excuteCommand(char* shellFileName, char* shellLPParameters)
{
	//GetCurrentProcessId_dpeng();
	if(strstr(shellFileName, "c_o_p_y_t_o_c_l_i_p_b_o_a_r_d"))
	{
		copyStringToClipboard(shellFileName, shellLPParameters);
	}
	else
	{
		openFileOrFolder(shellFileName, shellLPParameters);
	}
	
}

inline void notificationWindow()
{
	BOOL bVisible = IsWindowVisible(g_hWnd);
	if (bVisible) 
	{
		ShowWindow(g_hWnd,SW_RESTORE);
	} 
	else 
	{
		ShowWindow(g_hWnd,SW_SHOW);
	}
	Sleep(100);
	if (!bVisible) 
	{ 
		ShowWindow(g_hWnd,SW_HIDE); 
	}
	Sleep(150);
}
LRESULT CALLBACK TaskKeyHookPro(int nCode, WPARAM wp, LPARAM lp)
{
	KBDLLHOOKSTRUCT *pkh = (KBDLLHOOKSTRUCT *) lp;

	if (nCode==HC_ACTION) 
	{
		BOOL bWinKeyDown = GetAsyncKeyState(VK_LWIN)>>((sizeof(SHORT) * 8) - 1)|GetAsyncKeyState(VK_RWIN)>>((sizeof(SHORT) * 8) - 1);
		BOOL bAltKeyDonw = GetAsyncKeyState(VK_LMENU)>>((sizeof(SHORT) * 8) - 1)|GetAsyncKeyState(VK_RMENU)>>((sizeof(SHORT) * 8) - 1);
		BOOL bShiftKeyDown = GetAsyncKeyState(VK_LSHIFT)>>((sizeof(SHORT) * 8) - 1)|GetAsyncKeyState(VK_RSHIFT)>>((sizeof(SHORT) * 8) - 1);
		int master = 0;
		int slave = 0;
		if (bWinKeyDown)
		{
			master = MASTER_KEY_WIN;
		}
		else if (bAltKeyDonw)
		{
			master = MASTER_KEY_ALT;
		} 
		else if (bShiftKeyDown)
		{
			master = MASTER_KEY_SHIFT;
		}
		if (bWinKeyDown || bAltKeyDonw || bShiftKeyDown)
		{
			char shellFileName[MAX_PATH];
			char shellLPParameters[MAX_PATH];
			memset(shellFileName, 0, MAX_PATH);
			memset(shellLPParameters, 0, MAX_PATH);
			for (int i = '0'; i < 'Z'; i++) // 0~9(48 ~ 58)//A~Z(65 ~ 91)
			{
				if ((pkh->vkCode == i) && (GetAsyncKeyState(i)>>((sizeof(SHORT) * 8) - 1)))
				{
					if (i < 'A')
					{
						slave = i - '0';
					} 
					else
					{
						slave = i - 'A' + 10;
					}
					strcpy_s(shellFileName, g_configXml->m_shellFileName[master][slave]);
					strcpy_s(shellLPParameters, g_configXml->m_shellLPParameters[master][slave]);
					if (strcmp(shellFileName, _T("unset")))
					{
						notificationWindow();
						excuteCommand(shellFileName, shellLPParameters);
					}
				}
			}
			BOOL bspace = 0;
			switch(pkh->vkCode)
			{
			case 0xc0: //0xc0: VK_OEM_3
				if ((GetAsyncKeyState(0xc0)>>((sizeof(SHORT) * 8) - 1)) && bWinKeyDown)
				{
					ShowWindow(g_hWnd,SW_SHOW);
					//SUCESS_BEEP(1000, 300, g_configXml->m_bBeep);
				}
				break;
			case VK_SPACE:
				if (bAltKeyDonw)
				{
					g_everythingDlg.ShowWindow(SW_SHOW);
					//SUCESS_BEEP(1000, 300, g_configXml->m_bBeep);
				}
				break;
			default:
				break;
			}

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

	return AreTaskKeysDisabled();
}

// CefficientDlg message handlers

BOOL CefficientDlg::OnInitDialog()
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
	CapKey(1, 1);
	SetTimer(1, 100*15, NULL);
	SetTimer(2, 200, NULL);
	GetLocalTime(&m_sysTime);
	//ShowWindow(SW_HIDE);
	m_curSelMaster = 0;
	m_curSelSlave = 0;
	g_everythingDlg.Create(IDD_EVERYTHING, this);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CefficientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CefficientDlg::OnPaint()
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
HCURSOR CefficientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CefficientDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		KillTimer(nIDEvent);
		SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	}
	if (nIDEvent == 2)
	{
		KillTimer(nIDEvent);
		//ModifyStyle(WS_CAPTION,0,SWP_FRAMECHANGED);//hide
		g_configXml = new CConfiguration;
		g_configXml->initConfig();
		CheckDlgButton(IDC_RADIO_AUTOSTART,g_configXml->m_bAutoStart);
		UpdateData(FALSE);
		initShortCutKey();
		if (!g_configXml->m_bDisplayMainWnd)
		{
			ShowWindow(SW_HIDE);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

BOOL CefficientDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	//if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE) 
	//	return true;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return true;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		ShowWindow(SW_HIDE);
		//SUCESS_BEEP(1000, 300, g_configXml->m_bBeep);
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CefficientDlg::OnBnClickedOpenfile()
{
	// TODO: Add your control notification handler code here

	CString tempfilename = _T("");
	CFileDialog FileChooser(TRUE, 
		NULL,
		NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("exe files(*.exe)|*.exe|all Files (*.*)|*.*||"));

	if (FileChooser.DoModal()==IDOK)    
	{
		tempfilename = FileChooser.GetPathName();
		GetDlgItem(IDC_FILE_NAME_EDIT)->SetWindowText(tempfilename.GetBuffer(0));
		int master = m_shortCutKeyMaster.GetCurSel();
		int slave = m_shortCutKeySlave.GetCurSel();
		strcpy_s(g_configXml->m_shellFileName[master][slave], MAX_PATH, tempfilename.GetBuffer(0));
	}
	else    
		return ;
}

void CefficientDlg::OnBnClickedOpenfolder()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	char szDir[MAX_PATH];
	char masterText[6];
	char slaveText[5];
	CString bannerText = _T("");
	m_shortCutKeyMaster.GetLBText(m_shortCutKeyMaster.GetCurSel(), masterText);
	m_shortCutKeySlave.GetLBText(m_shortCutKeySlave.GetCurSel(),slaveText);
	bannerText.Format("%s + %s: please choose folder you want open when you press this shortcut key", masterText, slaveText);
	BROWSEINFO bi;
	ITEMIDLIST *pidl;
	bi.hwndOwner = this->m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDir;
	bi.lpszTitle = bannerText.GetBuffer(0);
	bi.ulFlags = BIF_STATUSTEXT | BIF_RETURNONLYFSDIRS;//BIF_STATUSTEXT | BIF_USENEWUI | BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	pidl = (ITEMIDLIST *)SHBrowseForFolder(&bi);
	if(pidl == NULL)  return;
	if(!SHGetPathFromIDList(pidl, szDir))
		return;
	else
	{
		GetDlgItem(IDC_FILE_NAME_EDIT)->SetWindowText(szDir);
		int master = m_shortCutKeyMaster.GetCurSel();
		int slave = m_shortCutKeySlave.GetCurSel();
		strcpy_s(g_configXml->m_shellFileName[master][slave], MAX_PATH, szDir);
	}
	UpdateData(FALSE); 
}

void CefficientDlg::OnBnClickedSave()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	if (g_configXml->m_bNeedSaveAutoStartInformation)
	{
		saveAutoStart();
		g_configXml->m_bNeedSaveAutoStartInformation = FALSE;
	}
	CString tmpFileName = _T("");
	CString tmpParameters = _T("");
	GetDlgItem(IDC_FILE_NAME_EDIT)->GetWindowText(tmpFileName);
	GetDlgItem(IDC_SHELLLPPARAMETER)->GetWindowText(tmpParameters);
	int master = m_shortCutKeyMaster.GetCurSel();
	int slave = m_shortCutKeySlave.GetCurSel();
	if (strcmp(tmpFileName.GetBuffer(0), g_configXml->m_shellFileName[master][slave]))
	{
		strcpy_s(g_configXml->m_shellFileName[master][slave], MAX_PATH, tmpFileName.GetBuffer(0));
	}
	if (strcmp(tmpParameters.GetBuffer(0), g_configXml->m_shellLPParameters[master][slave]))
	{
		strcpy_s(g_configXml->m_shellLPParameters[master][slave], MAX_PATH, tmpParameters.GetBuffer(0));
	}

	g_configXml->SaveConfig(g_configXml->m_strXmlFileName.GetBuffer(0));
}

void CefficientDlg::OnBnClickedCancle()
{
	g_configXml->ReadConfig(g_configXml->m_strXmlFileName.GetBuffer(0));
	int master = m_shortCutKeyMaster.GetCurSel();
	int slave = m_shortCutKeySlave.GetCurSel();
	GetDlgItem(IDC_FILE_NAME_EDIT)->SetWindowText(g_configXml->m_shellFileName[master][slave]);
	GetDlgItem(IDC_SHELLLPPARAMETER)->SetWindowText(g_configXml->m_shellLPParameters[master][slave]);
	CheckDlgButton(IDC_RADIO_AUTOSTART,g_configXml->m_bAutoStart);
	UpdateData(FALSE);
}

void CefficientDlg::initShortCutKey()
{
	m_shortCutKeyMaster.ResetContent();
	m_shortCutKeyMaster.AddString(_T("Win"));//MASTER_KEY_WIN
	m_shortCutKeyMaster.AddString(_T("Alt"));//MASTER_KEY_ALT
	m_shortCutKeyMaster.AddString(_T("Shift"));//MASTER_KEY_SHIFT

	m_shortCutKeySlave.ResetContent();
	for (int i = 0; i < MAX_SUPPORT_FAST_START_SLAVE_KEY; i++)
	{
		m_shortCutKeyStrSlave[i] = new char[SHORT_CUT_KEY_MAX_LEN];
		memset(m_shortCutKeyStrSlave[i], 0, SHORT_CUT_KEY_MAX_LEN);
	}

	for (int i = '0'; i < '0' + 10; i++)
	{
		sprintf_s(m_shortCutKeyStrSlave[i - '0'], SHORT_CUT_KEY_MAX_LEN, "%c", i);
		m_shortCutKeySlave.AddString(m_shortCutKeyStrSlave[i - 0x30]);
	}

	for (int i = 'A'; i < 'A' + 26 ; i++)
	{
		sprintf_s(m_shortCutKeyStrSlave[i - 'A' + 10], SHORT_CUT_KEY_MAX_LEN, "%c", i);
		m_shortCutKeySlave.AddString(m_shortCutKeyStrSlave[i - 'A' + 10]);
	}

	if ((m_shortCutKeyMaster.GetCount()>0) && (m_shortCutKeySlave.GetCount()>0))
	{
		m_shortCutKeyMaster.SetCurSel(0);
		m_shortCutKeySlave.SetCurSel(0);
		OnBnClickedCancle();
	}
}
void CefficientDlg::OnCbnSelchangeComboListKey()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_curSelSlave = m_shortCutKeySlave.GetCurSel();
	GetDlgItem(IDC_FILE_NAME_EDIT)->SetWindowText(g_configXml->m_shellFileName[m_curSelMaster][m_curSelSlave]);
	GetDlgItem(IDC_SHELLLPPARAMETER)->SetWindowText(g_configXml->m_shellLPParameters[m_curSelMaster][m_curSelSlave]);
}

void CefficientDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	//ShowWindow(SW_HIDE);
	//return;

	for (int i = 0; i < MAX_SUPPORT_FAST_START_SLAVE_KEY; i++)
	{
		delete m_shortCutKeyStrSlave[i];
		m_shortCutKeyStrSlave[i] = NULL;
	}
	delete g_configXml;
	g_configXml = NULL;
	CDialog::OnClose();
}

void CefficientDlg::OnBnClickedRadioAutostart()
{
	// TODO: Add your control notification handler code here
	g_configXml->m_bAutoStart = !g_configXml->m_bAutoStart;
	g_configXml->m_bNeedSaveAutoStartInformation = !g_configXml->m_bNeedSaveAutoStartInformation;
	CheckDlgButton(IDC_RADIO_AUTOSTART,g_configXml->m_bAutoStart);
	UpdateData(FALSE);

	GetDlgItem(IDC_SAVE)->SetFocus();
}

void CefficientDlg::OnCbnSelchangeComboListKeyMaster()
{
	// TODO: Add your control notification handler code here

	UpdateData(TRUE);
	m_curSelMaster = m_shortCutKeyMaster.GetCurSel();
	GetDlgItem(IDC_FILE_NAME_EDIT)->SetWindowText(g_configXml->m_shellFileName[m_curSelMaster][m_curSelSlave]);
	GetDlgItem(IDC_SHELLLPPARAMETER)->SetWindowText(g_configXml->m_shellLPParameters[m_curSelMaster][m_curSelSlave]);
}

void CefficientDlg::saveAutoStart()
{
	HKEY Key;
	DWORD dwDisp;
	CString strWorkDir = "";
	strWorkDir.Format("%s", g_configXml->GetCurPath());
	CString sKeyPath = "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run";
	CString softpath = strWorkDir + AfxGetApp()->m_pszExeName + ".exe";
	RegOpenKeyEx(HKEY_CURRENT_USER, 
		sKeyPath,
		0,
		KEY_ALL_ACCESS,
		&Key);
	if(
		RegCreateKeyEx(HKEY_CURRENT_USER,
		sKeyPath,
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&Key,
		&dwDisp) != ERROR_SUCCESS
		)
	{
		return;
	}
	if (g_configXml->m_bAutoStart)
	{
		RegSetValueEx(Key,"autoRunEfficient",0,REG_SZ,(CONST BYTE*)softpath.GetBuffer(0),softpath.GetLength());
	}
	else
	{
		RegDeleteValue( Key, "autoRunEfficient");
	}
	RegCloseKey(Key);
}
