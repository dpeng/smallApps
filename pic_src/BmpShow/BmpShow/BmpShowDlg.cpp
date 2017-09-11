// BmpShowDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BmpShow.h"
#include "BmpShowDlg.h"

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


// CBmpShowDlg dialog




CBmpShowDlg::CBmpShowDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBmpShowDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBmpShowDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBmpShowDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPENFILE, &CBmpShowDlg::OnBnClickedOpenfile)
	ON_BN_CLICKED(IDC_SHOW, &CBmpShowDlg::OnBnClickedShow)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CBmpShowDlg message handlers

BOOL CBmpShowDlg::OnInitDialog()
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBmpShowDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBmpShowDlg::OnPaint()
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
HCURSOR CBmpShowDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CBmpShowDlg::OnBnClickedOpenfile()
{
	// TODO: Add your control notification handler code here
	CFileDialog FileChooser(
		TRUE, 
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_T("Bmp Files (*.bmp)|*.bmp|All files(*.*)|*.*||"));

	if (FileChooser.DoModal()==IDOK)
		m_sFileName = FileChooser.GetPathName();
}

void CBmpShowDlg::OnBnClickedShow()
{
	// TODO: Add your control notification handler code here
	CFile file;
	CFileFind bExit;
	if (!bExit.FindFile(m_sFileName))
	{
		//MessageBox((LPCTSTR)_T("文件不存在"), (LPCTSTR)_T("fuck"), MB_ICONQUESTION);
		return;
	}
	file.Open(m_sFileName,CFile::modeRead,NULL);
	//if (file.GetLength() < 2)
	//{
	//	return;
	//}
	ULONGLONG ulSize = file.GetLength();
	char *pBuf = new char[ulSize];
	file.Read(pBuf,ulSize);
	file.Close();
	LONG cx,cy;
	DWORD bfOffBits;
	bfOffBits = *(DWORD*)(pBuf+10);
	cx = *(LONG*)(pBuf+18);
	cy = *(LONG*)(pBuf+22);
	delete pBuf;
	pBuf = NULL;
	CDC *pDC = GetDlgItem(IDC_STATIC_SHOW)->GetDC();
	CDC dcMemory;
	if(!dcMemory.CreateCompatibleDC(pDC))
		return;
	HBITMAP hbitmap = (HBITMAP)::LoadImage(
		0,
		m_sFileName,
		IMAGE_BITMAP,  
		cx,
		cy, 
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	CBitmap bmp;
	bmp.Attach(hbitmap);

	CBitmap *pOldBitmap = dcMemory.SelectObject(&bmp);
	pDC->BitBlt(10,10,cx,cy,&dcMemory,0,0,SRCCOPY);

	bmp.DeleteObject();
	dcMemory.SelectObject(pOldBitmap);
	dcMemory.DeleteDC();
}

void CBmpShowDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	OnBnClickedShow();
	// TODO: Add your message handler code here
}
