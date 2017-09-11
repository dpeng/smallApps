// SysInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SysInfo.h"
#include "SysInfoDlg.h"

#include "AllInfo.h"

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


// CSysInfoDlg dialog




CSysInfoDlg::CSysInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSysInfoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSysInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CSysInfoDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDCANCEL, &CSysInfoDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CSysInfoDlg message handlers

BOOL CSysInfoDlg::OnInitDialog()
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
	GetLocalTime(&m_SoftStartTime);
	m_nSoftStartTime = GetTickCount();
	GetDevInfo();
	DisPlayInfo();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSysInfoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSysInfoDlg::OnPaint()
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
HCURSOR CSysInfoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CSysInfoDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
void CSysInfoDlg::GetDevInfo()
{
	memset(&_RegInfo, 0, sizeof(REGINFO));
	memcpy(_RegInfo.LicInfo.Product, GetAllMacInfo(), sizeof(_RegInfo.LicInfo.Product));
	memcpy(_RegInfo.LicInfo.Mac, GetSingleMacInfo(), sizeof(_RegInfo.LicInfo.Mac));
	GetCpuID(_RegInfo.CpuId);
	memcpy(_RegInfo.OsInfo, GetOsVersion(), sizeof(_RegInfo.OsInfo));
	GetLocalTime(&_RegInfo.SysTime);
	memcpy(_RegInfo.Mac, GetAllMacInfo(), sizeof(_RegInfo.Mac));
	memcpy(_RegInfo.MemInfo, GetMemInfo(), sizeof(_RegInfo.MemInfo));
	GetLocalTime(&_RegInfo.SysTime);
	memcpy(_RegInfo.PhysicalSerialNum, GetPhysicalSerialNum(), sizeof(_RegInfo.PhysicalSerialNum));
	memcpy(_RegInfo.BoisId, GetBoisId(), sizeof(_RegInfo.BoisId));
	//username and pc name 
	DWORD size=MAX_PATH;
	::GetUserName(_RegInfo.UserName,&size);
	::GetComputerName(_RegInfo.PcName,&size);
}

void CSysInfoDlg::DisPlayInfo()
{
	char TmpReg[32*1024];
	memset(TmpReg, 0, sizeof(TmpReg));
	sprintf_s(TmpReg, "注册文件信息：\r\nProduct:%s\r\nCurMac:%s\r\nVersion:%s\r\nProvider:%s\r\nRegister:%s\r\nFrom:%d-%d-%d\r\nEnd:%d-%d-%d\r\nBuildTime:%d-%d-%d\r\nCPUID:%s\r\nOs:%s\r\nSysTime:%d-%d-%d %d-%d-%d\r\nMac:%s\r\nMemInfo:%s\r\nPhysicalSerialNum:%s\r\nBoisId:%s\r\nUserName:%s\r\nPcName:%s\r\n\r\n",
		_RegInfo.LicInfo.Product,
		_RegInfo.LicInfo.Mac,
		_RegInfo.LicInfo.Version,
		_RegInfo.LicInfo.Provider,
		_RegInfo.LicInfo.Register,
		_RegInfo.LicInfo.vf_year,
		_RegInfo.LicInfo.vf_month,
		_RegInfo.LicInfo.vf_day,
		_RegInfo.LicInfo.vt_year,
		_RegInfo.LicInfo.vt_month,
		_RegInfo.LicInfo.vt_day,
		_RegInfo.LicInfo.build_year,
		_RegInfo.LicInfo.build_month,
		_RegInfo.LicInfo.build_day,
		_RegInfo.CpuId,
		_RegInfo.OsInfo,
		_RegInfo.SysTime.wYear,
		_RegInfo.SysTime.wMonth,
		_RegInfo.SysTime.wDay,
		_RegInfo.SysTime.wHour,
		_RegInfo.SysTime.wMinute,
		_RegInfo.SysTime.wSecond,
		_RegInfo.Mac,
		_RegInfo.MemInfo,
		_RegInfo.PhysicalSerialNum,
		_RegInfo.BoisId,
		_RegInfo.UserName,
		_RegInfo.PcName);

	SoftInfoMap SoftList;
	SoftList.clear();
	GetAllSoftInfo(SoftList);
	for (SoftInfoMap::iterator it = SoftList.begin(); it != SoftList.end(); it++)
	{
		strcat(TmpReg, "\r\n");
		strcat(TmpReg, it->second.c_str());
	}
	GetDlgItem(IDC_DISPLAY)->SetWindowText(TmpReg);
	SYSTEMTIME TmpTime;
	GetLocalTime(&TmpTime);
	int tptm = (TmpTime.wYear - m_SoftStartTime.wYear)*31536000 +
		(TmpTime.wMonth - m_SoftStartTime.wMonth)*2678400 +
		(TmpTime.wDay - m_SoftStartTime.wDay)*86400 +
		(TmpTime.wHour - m_SoftStartTime.wHour)*3600 +
		(TmpTime.wMinute - m_SoftStartTime.wMinute)*60 +
		(TmpTime.wSecond - m_SoftStartTime.wSecond);
	tptm = tptm*1000 + TmpTime.wMilliseconds - m_SoftStartTime.wMilliseconds;
	int ii = GetTickCount() - m_nSoftStartTime;
	ii = 0;
}