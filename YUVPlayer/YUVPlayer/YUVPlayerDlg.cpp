// YUVPlayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "YUVPlayer.h"
#include "YUVPlayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
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


// CYUVPlayerDlg dialog




CYUVPlayerDlg::CYUVPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CYUVPlayerDlg::IDD, pParent)
	, m_nWidth(0)
	, m_nHight(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CYUVPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITWIDTH, m_nWidth);
	DDX_Text(pDX, IDC_EDITHEIGHT, m_nHight);
}

BEGIN_MESSAGE_MAP(CYUVPlayerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPENFILE, &CYUVPlayerDlg::OnBnClickedOpenfile)
	ON_BN_CLICKED(IDC_PLAY, &CYUVPlayerDlg::OnBnClickedPlay)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STOP, &CYUVPlayerDlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_RADIO1, &CYUVPlayerDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CYUVPlayerDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CYUVPlayerDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CYUVPlayerDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_CLOSEFILE, &CYUVPlayerDlg::OnBnClickedClosefile)
END_MESSAGE_MAP()


// CYUVPlayerDlg message handlers

BOOL CYUVPlayerDlg::OnInitDialog()
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
	m_FileName = "";
	m_nWidth = 352;
	m_nHight = 288;
	UpdateData(false);
	m_surface = NULL;
	m_pDD = NULL;
	m_pDDSPrimary = NULL;
	DragAcceptFiles(true);
	m_buf = NULL;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CYUVPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CYUVPlayerDlg::OnPaint()
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
HCURSOR CYUVPlayerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CYUVPlayerDlg::OnBnClickedOpenfile()
{
	// TODO: Add your control notification handler code here
	CString szFile = "";
	CFileDialog FileChooser(
		TRUE, 
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		"YUV Files (*.yuv)|*.yuv|All files(*.*)|*.*||");

	if (FileChooser.DoModal()==IDOK)
	{
		szFile = FileChooser.GetPathName();
		m_FileName = szFile;
	}
}

void CYUVPlayerDlg::OnBnClickedPlay()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	initDirectDraw();
	SetTimer(1,40,NULL);
}
DDPIXELFORMAT ddpfPixelFormats[] = 
{
	{sizeof(DDPIXELFORMAT), DDPF_FOURCC,MAKEFOURCC('U','Y','V','Y'),0,0,0,0,0},   // UYVY
	{sizeof(DDPIXELFORMAT), DDPF_FOURCC,MAKEFOURCC('Y','U','Y','2'),0,0,0,0,0},   // YUY2
	{sizeof(DDPIXELFORMAT), DDPF_FOURCC,MAKEFOURCC('Y','V','1','2'),0,0,0,0,0},   // YV12	
	{sizeof(DDPIXELFORMAT), DDPF_FOURCC,MAKEFOURCC('Y','V','U','9'),0,0,0,0,0},   // YVU9
	{sizeof(DDPIXELFORMAT), DDPF_FOURCC,MAKEFOURCC('I','F','0','9'),0,0,0,0,0},   // IF09
	{sizeof(DDPIXELFORMAT), DDPF_RGB, 0, 32, 0x00FF0000,0x0000FF00,0x000000FF, 0} // RGB32
};
color_convert_func ccfunc[] = 
{
	yuv2uyvy16_mmx,
	yuv2yuyv16_mmx,	
	yuv2yv12,	
	0
};
FILE *fp = NULL;
void CYUVPlayerDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (fp == NULL)
		if ((fp = fopen(m_FileName,"rb")) != NULL)
		{
			m_buf = new unsigned char[m_nWidth*m_nHight*3/2];
			memset(m_buf , 0 , m_nWidth * m_nHight * 3 / 2);
		}
		else
		{
			KillTimer(1);
			return;
		}
	fread(m_buf, 1, m_nWidth*m_nHight*3/2, fp);
	if (feof(fp))
	{
		OnBnClickedStop();
		MessageBox("File End", "Fuck", MB_OK);
		return;
	}
	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd,sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	m_surface->Lock(NULL, &ddsd, DDLOCK_SURFACEMEMORYPTR |DDLOCK_WAIT, NULL);
	_try{
		m_colorConvert(
			(unsigned char*)m_buf,
			(unsigned char*)(m_buf + m_nHight*m_nWidth),
			(unsigned char*)(m_buf + m_nHight*m_nWidth*5/4),
			(unsigned char*)ddsd.lpSurface,
			ddsd.lPitch,
			ddsd.dwWidth,
			ddsd.dwHeight);

	}
	_except(0,1)
	{
		m_surface->Unlock(NULL);
		return ;
	}
	m_surface->Unlock(NULL);
	RECT rc;
	GetDlgItem(IDC_STATIC_PLAY)->GetWindowRect(&rc);
	m_pDDSPrimary->Blt(&rc, m_surface, NULL, DDBLT_WAIT , NULL);
	CDialog::OnTimer(nIDEvent);
}
int CYUVPlayerDlg::initDirectDraw()
{	
	HRESULT hr;
	if(FAILED(hr = DirectDrawCreateEx(NULL, (VOID**)&m_pDD,
		IID_IDirectDraw7, NULL)))
		return E_FAIL;

	//Set cooperative level
	hr = m_pDD->SetCooperativeLevel(GetDlgItem(IDC_STATIC_PLAY)->GetSafeHwnd(), DDSCL_NORMAL);
	if(FAILED(hr))
		return E_FAIL;

	//Create the primary surface

	DDSURFACEDESC2 ddsd;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize         = sizeof(ddsd);
	ddsd.dwFlags        = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	if(FAILED(m_pDD->CreateSurface(&ddsd, &m_pDDSPrimary, NULL)))
		return E_FAIL;

	if (m_pDDSPrimary == NULL)
		return E_FAIL;

	//m_hWnd = hWnd;

	m_pDD->GetDisplayMode(&ddsd);

	//m_screenWidth = ddsd.dwWidth;
	//m_screenHeight = ddsd.dwHeight;
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags        = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT; 
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN | DDSCAPS_VIDEOMEMORY;
	ddsd.dwWidth        = m_nWidth;
	ddsd.dwHeight       = m_nHight;
	int i=0;
	while(i < 3){
		ddsd.ddpfPixelFormat = ddpfPixelFormats[i];
		hr = CreateDrawSurface(&ddsd, &m_surface);
		if (hr != DD_OK);
		if(hr == DDERR_UNSUPPORTEDMODE)
		{
			m_surface = NULL;
			return -1;
		}
		if(FAILED(hr))
			i++;
		else
			break;
	}

	if(i<3 && m_surface)
		m_colorConvert = ccfunc[i];
	else
		return -1;
	return 0;
}

HRESULT CYUVPlayerDlg::CreateDrawSurface(DDSURFACEDESC2 * pDdsd, LPDIRECTDRAWSURFACE7 * pSurface)
{
	HRESULT hr;
	LPDIRECTDRAWSURFACE7 _pSurface = NULL;

	/**/
	if (FAILED(hr = m_pDD->CreateSurface(pDdsd, &_pSurface, NULL)))
		return hr;

	*pSurface = _pSurface;

	LPDIRECTDRAWCLIPPER pClipper = NULL;

	if(FAILED(hr = m_pDD->CreateClipper(0, &pClipper, NULL))) 
		return hr;

	if(FAILED(hr = pClipper->SetHWnd(0, m_hWnd)))
		return hr;
	if(FAILED( hr = m_pDDSPrimary->SetClipper(pClipper))) 
		return hr;

	return	S_OK;
}
void CYUVPlayerDlg::OnBnClickedStop()
{
	// TODO: Add your control notification handler code here
	KillTimer(1);
	if (fp)
	{
		fclose(fp);
		fp = NULL;
	}
	if (m_buf)
	{
		delete m_buf;
		m_buf = NULL;
	}
	destroyDDObjects();
	GetDlgItem(IDC_STATIC_PLAY)->RedrawWindow();
}

void CYUVPlayerDlg::OnBnClickedRadio1()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDITWIDTH)->EnableWindow(false);
	GetDlgItem(IDC_EDITHEIGHT)->EnableWindow(false);
	m_nWidth = 176;
	m_nHight = 144;
	UpdateData(false);
}

void CYUVPlayerDlg::OnBnClickedRadio2()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDITWIDTH)->EnableWindow(false);
	GetDlgItem(IDC_EDITHEIGHT)->EnableWindow(false);
	m_nWidth = 352;
	m_nHight = 288;
	UpdateData(false);
}

void CYUVPlayerDlg::OnBnClickedRadio3()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDITWIDTH)->EnableWindow(false);
	GetDlgItem(IDC_EDITHEIGHT)->EnableWindow(false);
	m_nWidth = 704;
	m_nHight = 576;
	UpdateData(false);
}

void CYUVPlayerDlg::OnBnClickedRadio4()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_EDITWIDTH)->EnableWindow(true);
	GetDlgItem(IDC_EDITHEIGHT)->EnableWindow(true);
}
HRESULT CYUVPlayerDlg::destroyDDObjects()
{
	SAFE_RELEASE(m_surface);
	SAFE_RELEASE(m_pDDSPrimary);
	if(m_pDD)
		m_pDD->SetCooperativeLevel(m_hWnd, DDSCL_NORMAL);
	SAFE_RELEASE(m_pDD);
	return S_OK;
}
BOOL CYUVPlayerDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return true;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return true;
	if (pMsg->message == WM_DROPFILES)
	{
		char fName[256];
		//多文件拖放暂不支持，目前仅取第一个文件
		//UINT nCount = DragQueryFile((HDROP)pMsg->wParam, 0xFFFFFFFF, NULL, 0);
		//for (unsigned int i=0; i<nCount; i++)
		//{
		//	DragQueryFile ((HDROP)pMsg->wParam,i,fName, 256);
		//	SetForegroundWindow();
		//}
		DragQueryFile((HDROP)pMsg->wParam,0,fName, 256);
		SetForegroundWindow();
		DragFinish((HDROP)pMsg->wParam);
		m_FileName = fName;
		return TRUE;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
void CYUVPlayerDlg::OnBnClickedClosefile()
{
	// TODO: Add your control notification handler code here
	OnBnClickedStop();
	m_FileName = "";
}
