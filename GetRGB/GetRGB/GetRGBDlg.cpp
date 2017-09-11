// GetRGBDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GetRGB.h"
#include "GetRGBDlg.h"

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


// CGetRGBDlg dialog




CGetRGBDlg::CGetRGBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetRGBDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGetRGBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDERR, m_r);
	DDX_Control(pDX, IDC_SLIDERG, m_g);
	DDX_Control(pDX, IDC_SLIDERB, m_b);
}

BEGIN_MESSAGE_MAP(CGetRGBDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_VSCROLL()
	ON_WM_CTLCOLOR()
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CGetRGBDlg message handlers

BOOL CGetRGBDlg::OnInitDialog()
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
	m_r.SetRange(0, 255, true);
	m_g.SetRange(0, 255, true);
	m_b.SetRange(0, 255, true);
	m_brush = new CBrush;
	m_brush->CreateHatchBrush(6, RGB(0, 0, 0));
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGetRGBDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGetRGBDlg::OnPaint()
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
		//CRect rect;
		//CPaintDC dc(GetDlgItem(IDC_STATIC_SHOW));
		//GetDlgItem(IDC_STATIC_SHOW)->GetClientRect(&rect);
		//int r = m_r.GetPos();
		//int g = m_g.GetPos();
		//int b = m_b.GetPos();
		//dc.FillSolidRect(rect,RGB(r,g,b)); 
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGetRGBDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGetRGBDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}


void CGetRGBDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: Add your message handler code here and/or call default
	switch(GetWindowLong(pScrollBar->m_hWnd, GWL_ID))
	{
	case IDC_SLIDERR:
	case IDC_SLIDERG:
	case IDC_SLIDERB:
		int r = m_r.GetPos();
		int g = m_g.GetPos();
		int b = m_b.GetPos();
		CString Str = "";
		Str.Format("R:%d\n\nG:%d\n\nB:%d", r, g, b);
		GetDlgItem(IDC_STATIC_TEXT)->SetWindowText(Str);
		//GetDlgItem(IDC_STATIC_SHOW)->RedrawWindow();
		this->RedrawWindow();
		break;
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

HBRUSH CGetRGBDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	if(nCtlColor == CTLCOLOR_STATIC && pWnd->GetDlgCtrlID() == IDC_STATIC_SHOW)
	{
		delete m_brush;
		m_brush = NULL;
		m_brush = new CBrush;
		int r = m_r.GetPos();
		int g = m_g.GetPos();
		int b = m_b.GetPos();
		m_brush->CreateHatchBrush(6, RGB(r, g, b));
		return *m_brush;
	}
	//CPaintDC dc(GetDlgItem(IDC_STATIC_SHOW));
	//int r = m_r.GetPos();
	//int g = m_g.GetPos();
	//int b = m_b.GetPos();
	//dc.FillSolidRect(rect,RGB(r,g,b)); 
	//return dc;
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

