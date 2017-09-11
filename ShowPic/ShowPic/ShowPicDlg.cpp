// ShowPicDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ShowPic.h"
#include "ShowPicDlg.h"
#include <AFXPRIV2.H>
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


// CShowPicDlg dialog




CShowPicDlg::CShowPicDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShowPicDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CShowPicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CShowPicDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_OPENFILE, &CShowPicDlg::OnBnClickedOpenfile)
	ON_BN_CLICKED(IDC_SHOW, &CShowPicDlg::OnBnClickedShow)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_PREVIEW, &CShowPicDlg::OnBnClickedPreview)
	ON_BN_CLICKED(IDC_NEXT, &CShowPicDlg::OnBnClickedNext)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_AUTOSHOW, &CShowPicDlg::OnBnClickedAutoshow)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_STOPAUTOSHOW, &CShowPicDlg::OnBnClickedStopautoshow)
	ON_BN_CLICKED(IDC_ENLARGE, &CShowPicDlg::OnBnClickedEnlarge)
	ON_BN_CLICKED(IDC_REDUCE, &CShowPicDlg::OnBnClickedReduce)
END_MESSAGE_MAP()


// CShowPicDlg message handlers
struct ivp_tag{
	int fileNr;
	char* filename[1024];
	int playnum;
};
static struct ivp_tag gManage={0};
BOOL CShowPicDlg::OnInitDialog()
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
	ReSizeWindow();
	m_brush = new CBrush;
	m_brush->CreateHatchBrush(6, RGB(0, 0, 0));
	m_bReSizeSet = false;
	m_bIsFullScreen = false;
	m_nMutil = 0;

	ShowWindow(TRUE);
	CenterWindow();
	GetDlgItem(IDC_STATIC_SHOW)->GetWindowRect(&m_rc);
	GetWindowPlacement(&m_OldWndplacement);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CShowPicDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		if(nID == SC_MINIMIZE)   
		{   
			//AfxMessageBox("最小化喽！",MB_OK,NULL);   
		}   
		else if(nID == SC_RESTORE)
		{   
			//AfxMessageBox("还原！",MB_OK,NULL);
			SetTimer(2, 100, NULL);
		}
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CShowPicDlg::OnPaint()
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
HCURSOR CShowPicDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CShowPicDlg::OnBnClickedOpenfile()
{
	// TODO: Add your control notification handler code here
	CFileDialog FileChooser(
		TRUE, 
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_T("BMP(*.BMP)|*.BMP|JPEG(*.JPG;*.JPEG;*.JPE;*.JFIF)|*.JPG;*.JPEG;*.JPE;*.JFIF|All files(*.*)|*.*||"));
	FileChooser.m_ofn.lpstrInitialDir = "F:\\娱乐\\picture\\";
	if (FileChooser.DoModal()==IDOK)
	{
		m_sFileName = FileChooser.GetPathName();
		char filename[MAX_PATH];
		memcpy(filename, m_sFileName.GetBuffer(0), strlen(m_sFileName.GetBuffer(0))+1);
		UINT len = strlen(filename) + 1;
		gManage.filename[gManage.fileNr] = (char*)malloc(len);
		memcpy(gManage.filename[gManage.fileNr], m_sFileName.GetBuffer(0), len);
		gManage.fileNr++;
		SetForegroundWindow();
	}
	OnBnClickedShow();
}

void CShowPicDlg::OnBnClickedShow()
{
	// TODO: Add your control notification handler code here
	this->RedrawWindow();
	m_sFileName = gManage.filename[gManage.playnum];
	char filename[MAX_PATH];
	CString TmpStr = _T("");
	memcpy(filename, m_sFileName.GetBuffer(0), strlen(m_sFileName.GetBuffer(0)) +1 );
	UINT len = strlen(filename) + 1;
	TmpStr = &filename[len-4];
	bool bRet = false;
	if(TmpStr == "bmp" || TmpStr == "BMP")
		bRet = ShowBmp(filename);
	else
		bRet = ShowJpg(filename);
	if (bRet)
		SetWindowText(filename);
}

bool CShowPicDlg::ShowBmp(LPCTSTR FileName)
{
	CFile file;
	CFileFind bExit;
	if (!bExit.FindFile(FileName))
	{
		//MessageBox((LPCTSTR)_T("文件不存在"), (LPCTSTR)_T("fuck"), MB_ICONQUESTION);
		return false;
	}
	file.Open(FileName,CFile::modeRead,NULL);
	ULONGLONG ulSize = file.GetLength();
	char *pBuf = new char[ulSize];
	file.Read(pBuf,ulSize);
	file.Close();
	double fX,fY; 
	DWORD bfOffBits;
	RECT rc = {0};
	GetDlgItem(IDC_STATIC_SHOW)->GetWindowRect(&rc);
	bfOffBits = *(DWORD*)(pBuf+10);
	fX = *(LONG*)(pBuf+18);
	fY = *(LONG*)(pBuf+22);
	delete pBuf;
	pBuf = NULL;
	if (m_nMutil != 0)
	{
		//rc.bottom += rc.bottom*m_nMutil/10;
		//rc.left += rc.left*m_nMutil/10;
		//rc.right += rc.right*m_nMutil/10;
		//rc.top += rc.top*m_nMutil/10;
		fX += fX*m_nMutil/10;
		fY += fY*m_nMutil/10;
	}
	char str[200];
	sprintf(str, "rc:%d-%d-%d-%d  xy:%d-%d\n",rc.left, rc.right, rc.top, rc.bottom, (int)fX,(int)fY);
	OutputDebugString(str);
	if (fX <= 0 || fY <= 0)
		return false;
	CDC *pDC = GetDlgItem(IDC_STATIC_SHOW)->GetDC();
	int x = 5;
	int y = 5;
	if (fX > rc.right - rc.left || fY > rc.bottom - rc.top)
		if ((fX/(rc.right - rc.left)) >= (fY/(rc.bottom - rc.top)))
		{
			fY = (rc.right - rc.left)*fY/fX;
			fX = rc.right - rc.left;
		}
		else
		{
			fX = (rc.bottom - rc.top)*fX/fY;
			fY = rc.bottom - rc.top;
		}
	if (((rc.bottom - rc.top) - fY)/2 > 0)
		y = ((rc.bottom - rc.top) - fY)/2;
	if (((rc.right - rc.left) - fX)/2 > 0)
		x = ((rc.right - rc.left) - fX)/2;
	CDC dcMemory;
	if(!dcMemory.CreateCompatibleDC(pDC))
		return false;
	HBITMAP hbitmap = (HBITMAP)::LoadImage(
		0,
		FileName,
		IMAGE_BITMAP,  
		fX,
		fY, 
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	CBitmap bmp;
	bmp.Attach(hbitmap);

	CBitmap *pOldBitmap = dcMemory.SelectObject(&bmp);
	pDC->BitBlt(x,y,fX,fY,&dcMemory,0,0,SRCCOPY);

	//pDC->DeleteDC();
	//pDC = NULL;
	bmp.DeleteObject();
	dcMemory.SelectObject(pOldBitmap);
	dcMemory.DeleteDC();
	return true;
}
bool CShowPicDlg::ShowJpg(LPCTSTR FileName)
{
	CFileFind bExit;
	if (!bExit.FindFile(FileName))
	{
		//MessageBox((LPCTSTR)_T("文件不存在"), (LPCTSTR)_T("fuck"), MB_ICONQUESTION);
		return false;
	}
	IStream *pStm; 
	CFileStatus fstatus; 
	CFile file; 
	LONG cb; 
	LPVOID pvData = NULL; 
	if (file.Open(FileName,CFile::modeRead)&&file.GetStatus(FileName,fstatus)&& ((cb = fstatus.m_size) != -1)) 
	{ 
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cb); 
		if (hGlobal != NULL) 
			if ((pvData = GlobalLock(hGlobal)) != NULL) 
			{ 
				file.Read(pvData, cb); 
				GlobalUnlock(hGlobal); 
				CreateStreamOnHGlobal(hGlobal, TRUE, &pStm); 
			} 
	} 

	IPicture *pPic; 
	if(SUCCEEDED(OleLoadPicture(pStm,fstatus.m_size,TRUE,IID_IPicture,(LPVOID*)&pPic))) 
	{ 
		OLE_XSIZE_HIMETRIC hmWidth; 
		OLE_YSIZE_HIMETRIC hmHeight; 
		pPic->get_Width(&hmWidth); 
		pPic->get_Height(&hmHeight); 
		CDC *pDC = GetDlgItem(IDC_STATIC_SHOW)->GetDC();
		double fX,fY; 
		RECT rc = {0};
		GetDlgItem(IDC_STATIC_SHOW)->GetWindowRect(&rc);
		fX = (double)pDC->GetDeviceCaps(HORZRES)*(double)hmWidth/((double)pDC->GetDeviceCaps(HORZSIZE)*100.0);
		fY = (double)pDC->GetDeviceCaps(VERTRES)*(double)hmHeight/((double)pDC->GetDeviceCaps(VERTSIZE)*100.0);
		if (m_nMutil != 0)
		{
			//rc.bottom += rc.bottom*m_nMutil/10;
			//rc.left += rc.left*m_nMutil/10;
			//rc.right += rc.right*m_nMutil/10;
			//rc.top += rc.top*m_nMutil/10;
			fX += fX*m_nMutil/10;
			fY += fY*m_nMutil/10;
		}
		if (fX <= 0 || fY <= 0)
			return false;
		if (rc.right < 0 || rc.left < 0 || rc.bottom < 0, rc.top < 0)
			rc = m_rc;
		int x = 5;
		int y = 5;
		if (fX > rc.right - rc.left || fY > rc.bottom - rc.top)
			if ((fX/(rc.right - rc.left)) >= (fY/(rc.bottom - rc.top)))
			{
				fY = (rc.right - rc.left)*fY/fX;
				fX = rc.right - rc.left;
			}
			else
			{
				fX = (rc.bottom - rc.top)*fX/fY;
				fY = rc.bottom - rc.top;
			}
		if (((rc.bottom - rc.top) - fY)/2 > 0)
			y = ((rc.bottom - rc.top) - fY)/2;
		if (((rc.right - rc.left) - fX)/2 > 0)
			x = ((rc.right - rc.left) - fX)/2;
		if(FAILED(pPic->Render(*pDC,x,y,(DWORD)fX,(DWORD)fY,0,hmHeight,hmWidth,-hmHeight,NULL))) 
			MessageBox((LPCTSTR)_T("渲染图像失败"), (LPCTSTR)_T("fuck"), MB_ICONQUESTION);
		pPic->Release();
		pPic = NULL;
		pStm->Release();
		pStm = NULL;
	} 
	else 
		MessageBox((LPCTSTR)_T("从流中加载图像失败"), (LPCTSTR)_T("fuck"), MB_ICONQUESTION);
	return true;
}
void CShowPicDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if(GetDlgItem(IDC_STATIC_SHOW) != NULL/* && m_bReSizeSet*/)
	{
		m_bReSizeSet = false;
		ReSizeWindow();
		OnBnClickedShow();
	}
	// TODO: Add your message handler code here
}
void CShowPicDlg::ReSizeWindow()
{
	RECT rect;
	RECT rc;
	GetWindowRect(&rc);
	rect.top = rc.top+55;
	rect.bottom = rc.bottom - 5;
	rect.left = rc.left + 5;
	rect.right = rc.right - 5;
	ScreenToClient(&rect);
	GetDlgItem(IDC_STATIC_SHOW)->MoveWindow(&rect);
}
void CShowPicDlg::OnBnClickedPreview()
{
	// TODO: Add your control notification handler code here
	if (gManage.playnum > 0)
		gManage.playnum--;
	m_sFileName = gManage.filename[gManage.playnum];
	OnBnClickedShow();
}

void CShowPicDlg::OnBnClickedNext()
{
	// TODO: Add your control notification handler code here
	if (gManage.playnum < gManage.fileNr - 1)
		gManage.playnum++;
	m_sFileName = gManage.filename[gManage.playnum];
	OnBnClickedShow();
}
BOOL CShowPicDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) 
		return TRUE;
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) 
		return TRUE;
	if (pMsg->message == WM_SIZE)
		m_bReSizeSet = true;
	if(pMsg->message == WM_LBUTTONDBLCLK)
	{
		RECT rc = { 0 };
		GetDlgItem(IDC_STATIC_SHOW)->GetWindowRect(&rc);
		POINT lpPoint = { 0 };
		GetCursorPos(&lpPoint);
		if ((lpPoint.x >= rc.left)  &&
			(lpPoint.x <= rc.right) && 
			(lpPoint.y <= rc.bottom)&&
			(lpPoint.y >= rc.top))
		{
			OnWndFullScreen();
			OnBnClickedShow();
		}
	}
	if (pMsg->message == WM_DROPFILES)
	{
		char fName[256];
		CString Str = "";
		UINT nCount = DragQueryFile((HDROP)pMsg->wParam, 0xFFFFFFFF, NULL, 0);
		for (UINT i = 0; i < nCount; i++)
		{
			DragQueryFile ((HDROP)pMsg->wParam,i,fName, 256);
			m_sFileName = fName;
			DWORD value = GetFileAttributes(fName);   
			if(value == FILE_ATTRIBUTE_DIRECTORY ) //路径是文件夹
			{
				CFileFind  FindFile;
				if(m_sFileName.Right(1) != "\\")
					m_sFileName += "\\";
				m_sFileName += "*.*";
				BOOL res = FindFile.FindFile( m_sFileName );
				while(res)
				{
					res = FindFile.FindNextFile();
					if(!FindFile.IsDirectory() && !FindFile.IsDots() )
					{
						m_sFileName = FindFile.GetFilePath();
						CString TmpStr = "";
						char filename[MAX_PATH];
						memcpy(filename, m_sFileName.GetBuffer(0), strlen(m_sFileName.GetBuffer(0))+1);
						UINT tmplen = strlen(filename) + 1;
						TmpStr = &filename[tmplen-4];
						if ((TmpStr != "bmp") && (TmpStr != "BMP") && 
							(TmpStr != "jpg") && (TmpStr != "JPG") &&
							(TmpStr != "peg") && (TmpStr != "PEG") &&
							(TmpStr != "jpe") && (TmpStr != "JPE") &&
							(TmpStr != "fif") && (TmpStr != "FIF"))
						{
							//TmpStr.Format("%s\n可能是不支持的文件格式!继续打开文件？",m_sFileName);
							//if (MessageBox(TmpStr, "confirm", MB_ICONQUESTION | MB_YESNO) != IDYES)
							continue;
						}
						memcpy(filename, m_sFileName.GetBuffer(0), strlen(m_sFileName.GetBuffer(0))+1);
						UINT len = strlen(filename) + 1;
						gManage.filename[gManage.fileNr] = (char*)malloc(len);
						memcpy(gManage.filename[gManage.fileNr], m_sFileName.GetBuffer(0), len);
						gManage.fileNr++;
					}
				}
				FindFile.Close();
			}
			else  //路径是文件
			{
				CString TmpStr = "";
				char filename[MAX_PATH];
				memcpy(filename, m_sFileName.GetBuffer(0), strlen(m_sFileName.GetBuffer(0))+1);
				UINT tmplen = strlen(filename) + 1;
				TmpStr = &filename[tmplen-4];
				if ((TmpStr != "bmp") && (TmpStr != "BMP") && 
					(TmpStr != "jpg") && (TmpStr != "JPG") &&
					(TmpStr != "peg") && (TmpStr != "PEG") &&
					(TmpStr != "jpe") && (TmpStr != "JPE") &&
					(TmpStr != "fif") && (TmpStr != "FIF"))
				{
					//TmpStr.Format("%s\n可能是不支持的文件格式!继续打开文件？",m_sFileName);
					//if (MessageBox(TmpStr, "confirm", MB_ICONQUESTION | MB_YESNO) != IDYES)
						continue;
				}
				memcpy(filename, m_sFileName.GetBuffer(0), strlen(m_sFileName.GetBuffer(0))+1);
				UINT len = strlen(filename) + 1;
				gManage.filename[gManage.fileNr] = (char*)malloc(len);
				memcpy(gManage.filename[gManage.fileNr], m_sFileName.GetBuffer(0), len);
				gManage.fileNr++;
			}
			SetForegroundWindow();
		}
		DragFinish((HDROP)pMsg->wParam);
	}
	return CDialog::PreTranslateMessage(pMsg);
}
HBRUSH CShowPicDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
		delete m_brush;
		m_brush = NULL;
		m_brush = new CBrush;
		//int r = int(rand()*0xff/0x7fff);
		//int g = int(rand()*0xff/0x7fff);
		//int b = int(rand()*0xff/0x7fff);
		if(nCtlColor != CTLCOLOR_DLG)
		{
			m_brush->CreateHatchBrush(6, RGB(90, 100, 110));
			//OnBnClickedShow();
			//m_brush->CreateHatchBrush(4, RGB(100, 100, 100));
			return *m_brush;
		}
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

void CShowPicDlg::OnWndFullScreen()
{
	m_bIsFullScreen = !m_bIsFullScreen;
	CButton *pButton;
	if(m_bIsFullScreen)
	{
		pButton = (CButton *)GetDlgItem(IDC_OPENFILE);		
		pButton->ModifyStyle(WS_VISIBLE,0,0);
		pButton = (CButton *)GetDlgItem(IDC_SHOW);		
		pButton->ModifyStyle(WS_VISIBLE,0,0);
		pButton = (CButton*)GetDlgItem(IDC_PREVIEW);
		pButton->ModifyStyle(WS_VISIBLE,0,0);
		pButton = (CButton*)GetDlgItem(IDC_NEXT);
		pButton->ModifyStyle(WS_VISIBLE, 0, 0);

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
		GetDlgItem(IDC_STATIC_SHOW)->MoveWindow(&rc,TRUE);
	}
	else
	{
		pButton = (CButton *)GetDlgItem(IDC_OPENFILE);		
		pButton->ModifyStyle(0,WS_VISIBLE,0);
		pButton = (CButton *)GetDlgItem(IDC_SHOW);		
		pButton->ModifyStyle(0,WS_VISIBLE,0);
		pButton = (CButton*)GetDlgItem(IDC_PREVIEW);
		pButton->ModifyStyle(0,WS_VISIBLE,0);
		pButton = (CButton*)GetDlgItem(IDC_NEXT);
		pButton->ModifyStyle(0, WS_VISIBLE, 0);


		SetWindowPlacement(&m_OldWndplacement);
		RECT rc = { 0 };
		rc.right = m_rc.right - m_rc.left;
		rc.bottom = m_rc.bottom - m_rc.top;
		GetDlgItem(IDC_STATIC_SHOW)->MoveWindow(&m_rc,TRUE);
		ReSizeWindow();
	}
	//this->RedrawWindow();
}
void CShowPicDlg::OnBnClickedAutoshow()
{
	// TODO: Add your control notification handler code here
	SetTimer(1, 3000, NULL);
}

void CShowPicDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	switch(nIDEvent)
	{
	case 1:
		if (gManage.playnum < gManage.fileNr - 1)
			gManage.playnum++;
		else
			gManage.playnum = 0;
		m_sFileName = gManage.filename[gManage.playnum];
		OnBnClickedShow();
		break;
	case 2:
		KillTimer(2);
		OnBnClickedShow();
	}
	CDialog::OnTimer(nIDEvent);
}

void CShowPicDlg::OnBnClickedStopautoshow()
{
	KillTimer(1);
	// TODO: Add your control notification handler code here
}

void CShowPicDlg::OnBnClickedEnlarge()
{
	// TODO: Add your control notification handler code here
	m_nMutil++;
	OnBnClickedShow();
}

void CShowPicDlg::OnBnClickedReduce()
{
	// TODO: Add your control notification handler code here
	m_nMutil--;
	OnBnClickedShow();
}
