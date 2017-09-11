// EverythingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Everything.h"
#include "EverythingDlg.h"
#include <list>
#include <io.h>

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


// CEverythingDlg dialog




CEverythingDlg::CEverythingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEverythingDlg::IDD, pParent)
	, m_Text(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CEverythingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILE, m_Filelist);
	DDX_Text(pDX, IDC_TEXT, m_Text);
}

BEGIN_MESSAGE_MAP(CEverythingDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_SEARCH, &CEverythingDlg::OnBnClickedSearch)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_FILE, &CEverythingDlg::OnLvnItemchangedListFile)
//	ON_BN_CLICKED(IDC_SEARCHINFO, &CEverythingDlg::OnBnClickedSearchinfo)
END_MESSAGE_MAP()


// CEverythingDlg message handlers

BOOL CEverythingDlg::OnInitDialog()
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
	m_Filelist.InsertColumn(0, _T("FileName"), LVCFMT_IMAGE|LVCFMT_LEFT);
	m_Filelist.InsertColumn(1, _T("FilePath"));
	m_Filelist.InsertColumn(2, _T("FileRoot"));
	m_Filelist.InsertColumn(3, _T("FileTitle"));
	m_Filelist.InsertColumn(4, _T("FileURL"));
	m_Filelist.InsertColumn(5, _T("FileLen"));
	m_Filelist.InsertColumn(6, _T("CreationTime"));
	m_Filelist.InsertColumn(7, _T("lastAccessTime"));
	m_Filelist.InsertColumn(8, _T("LastWriteTime"));
	m_Filelist.SetColumnWidth(0 ,150);
	m_Filelist.SetColumnWidth(1 ,320);
	m_Filelist.SetColumnWidth(2 ,0);
	m_Filelist.SetColumnWidth(3 ,0);
	m_Filelist.SetColumnWidth(4, 0);
	m_Filelist.SetColumnWidth(5 ,75);
	m_Filelist.SetColumnWidth(6 ,150);
	m_Filelist.SetColumnWidth(7 ,150);
	m_Filelist.SetColumnWidth(8 ,150);
	for(int i = 0; i < MAX_THREAD_NUM; i++)
		m_ThreadId[i] = NULL; 

	m_filevector.clear();
	m_dirvername.clear();
	for (int i = 'A'; i < 'Z'; i++)
	{
		char tmpch = (char)i;
		CString tmpdir;
		tmpdir.Format("%c:\\", tmpch);
		switch(GetDriveType(tmpdir))
		{
		case DRIVE_UNKNOWN://未知类型
			break;
		case DRIVE_NO_ROOT_DIR://不存在的分区
			break;
		case DRIVE_REMOVABLE://可移动磁盘
			m_dirvername.push_back(tmpdir);
			break;
		case DRIVE_FIXED://固定磁盘
			m_dirvername.push_back(tmpdir);
			break;
		case DRIVE_REMOTE://网络驱动器
			break;
		case DRIVE_CDROM://CD_ROM
			break;
		case DRIVE_RAMDISK://随机存取磁盘（RAM）
			break;
		default:
			break;
		}
	}

	//hMutex=CreateMutex(NULL,TRUE,"Everything");
	InitializeCriticalSection(&m_lock);
	m_FileNr = 0;
	//打开本地文件数据库
	m_Localdb.OpenAdo("local.accdb");
	//m_Localdb.ClearAdo("DemoTable");
	m_Localdb.OpenTable("DemoTable");

	//打开收集来的数据库
	//m_Infodb.OpenAdo("info.accdb");

	m_FileIndexItem.clear();
	m_FileIndexItem.itemName.push_back("FileName");
	m_FileIndexItem.itemName.push_back("FilePath");
	m_FileIndexItem.itemName.push_back("FileRoot");
	m_FileIndexItem.itemName.push_back("FileTitle");
	m_FileIndexItem.itemName.push_back("FileURL");
	m_FileIndexItem.itemName.push_back("FileLen");
	m_FileIndexItem.itemName.push_back("CreationTime");
	m_FileIndexItem.itemName.push_back("lastAccessTime");
	m_FileIndexItem.itemName.push_back("LastWriteTime");

#ifdef FILE_MAP_VIEW
	m_FileMap.CreadFileMap("test.txt");//I love you very much, baby
#endif

	if (_access("judege.first", 0) == -1)
	{
		SetTimer(1, 10, NULL);//创建数据库
		CreateFile("judege.first", 
			GENERIC_READ,
			0, 
			NULL, 
			CREATE_NEW, 
			FILE_ATTRIBUTE_NORMAL, 
			NULL);
	}
	m_clickCount = GetTickCount();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CEverythingDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CEverythingDlg::OnPaint()
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
HCURSOR CEverythingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CEverythingDlg::BrowseFileUnrecursion(char* strFile)
{
//	FileIndex TmpIndex;
//	FILETIME tmpFiletime;
	list<CString> file_store;
	file_store.push_back(strFile);
	CFileFind filefind;
	CString list_file;
	while (file_store.size()>0)
	{
		list_file=file_store.front();
		file_store.pop_front();
		if (list_file.Right(2)!="\\")
			list_file+="\\";
		list_file+="*.*";
		BOOL res=filefind.FindFile(list_file);
		while(res)
		{
			res=filefind.FindNextFile();
			if (!filefind.IsDots())
			{
				if (filefind.IsDirectory())
				{
					file_store.push_back(filefind.GetFilePath());
					GetDlgItem(IDC_STATIC_INFO)->SetWindowText(filefind.GetFileName().GetBuffer(0));
				}

#ifdef FILE_MAP_VIEW
				if (m_FileMap.FindInMap((char*)filefind.GetFileName().GetBuffer(0)))
					continue;
				m_FileMap.AddFile(filefind.GetFileName().GetBuffer(0));
				memset(&TmpIndex, 0, sizeof(FileIndex));
#endif
				DeleteFile(filefind.GetFileName().GetBuffer(0));
				//strcpy_s(TmpIndex.FileName, sizeof(TmpIndex.FileName), filefind.GetFileName().GetBuffer(0));
				//strcpy_s(TmpIndex.FilePath, sizeof(TmpIndex.FilePath), filefind.GetFilePath().GetBuffer(0));
				//strcpy_s(TmpIndex.FileRoot, sizeof(TmpIndex.FileRoot), filefind.GetRoot().GetBuffer(0));
				//strcpy_s(TmpIndex.FileTitle, sizeof(TmpIndex.FileTitle), filefind.GetFileTitle().GetBuffer(0));
				//strcpy_s(TmpIndex.FileURL, sizeof(TmpIndex.FileURL), filefind.GetFileURL().GetBuffer(0));
				//TmpIndex.FileLen = filefind.GetLength();
				//memset(&tmpFiletime, 0, sizeof(FILETIME));
				//filefind.GetCreationTime(&tmpFiletime);
				//FileTimeToSystemTime(&tmpFiletime, &TmpIndex.CreationTime);
				//memset(&tmpFiletime, 0, sizeof(FILETIME));
				//filefind.GetLastAccessTime(&tmpFiletime);
				//FileTimeToSystemTime(&tmpFiletime, &TmpIndex.lastAccessTime);
				//memset(&tmpFiletime, 0, sizeof(FILETIME));
				//filefind.GetLastWriteTime(&tmpFiletime);
				//FileTimeToSystemTime(&tmpFiletime, &TmpIndex.LastWriteTime);

				EnterCriticalSection(&m_lock);
				//m_queue.push(TmpIndex);
				LeaveCriticalSection(&m_lock);
			}
		}
	}
	MessageBox("shit", "shit over", MB_OK);
	filefind.Close();
}
void CEverythingDlg::BrowseFileRecursion(char* strFile) //递归
{
	CFileFind ff;
	CString szDir = strFile;

	if(szDir.Right(1) != "\\")
		szDir += "\\";
	szDir += "*.*";
	BOOL res = ff.FindFile(szDir);
	while(res)
	{
		res = ff.FindNextFile();
		if(ff.IsDirectory() && !ff.IsDots()) //文件夹
		{
			BrowseFileRecursion(ff.GetFilePath().GetBuffer(0));
		}
		else if(!ff.IsDirectory() && !ff.IsDots()) //文件
		{
			//to do
		}
		CString strPath = ff.GetFilePath();
		CString strname = ff.GetFileName();
	}
	ff.Close();
}

void CEverythingDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		KillTimer( nIDEvent );
		static ThreadPara tmpPara1;
		static ThreadPara tmpPara2;
		memset(&tmpPara1, 0, sizeof(ThreadPara));
		tmpPara1.lPara = this;
		unsigned int i = 0; 
		//for(; i < m_dirvername.size(); i++)
		//tmpPara1.DriverName.push_back(m_dirvername[i]);
		tmpPara1.DriverName.push_back(_T("D:\\Code\\Chrome\\releases\\"));
		m_ThreadId[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CEverythingDlg::SearchThread, (LPVOID)&tmpPara1, 0, NULL);
		//两个线程读
		//Sleep(10);
		//memset(&tmpPara2, 0, sizeof(ThreadPara));
		//tmpPara2.lPara = this;
		//for(; i < m_dirvername.size(); i++)
		//	tmpPara2.DriverName.push_back(m_dirvername[i]);
		//m_ThreadId[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CEverythingDlg::SearchThread, (LPVOID)&tmpPara2, 0, NULL);
		Sleep(10);
		m_ThreadId[MAX_THREAD_NUM] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)CEverythingDlg::Readqueue, this, 0, NULL);
	}
	CDialog::OnTimer(nIDEvent);
}

void CEverythingDlg::SearchThread(ThreadPara* ThreadParameter)
{
	ThreadPara *pThread = (ThreadPara *)ThreadParameter;
	CEverythingDlg *pThis = (CEverythingDlg*)pThread->lPara;
	for (vector<CString>::iterator it = pThread->DriverName.begin(); it != pThread->DriverName.end(); it++)
		pThis->BrowseFileUnrecursion(it->GetBuffer());
}

void CEverythingDlg::Readqueue(LPVOID lPara)
{
	CEverythingDlg *pThis = (CEverythingDlg*)lPara;
	while(1)
	{
		if (!pThis->m_queue.empty())
		{
			EnterCriticalSection(&pThis->m_lock);

			//保存到数据库
			pThis->m_FileIndexItem.reset();
			pThis->m_FileIndexItem.itemMsg.push_back(pThis->m_queue.front().FileName);
			pThis->m_FileIndexItem.itemMsg.push_back(pThis->m_queue.front().FilePath);
			pThis->m_FileIndexItem.itemMsg.push_back(pThis->m_queue.front().FileRoot);
			pThis->m_FileIndexItem.itemMsg.push_back(pThis->m_queue.front().FileTitle);
			pThis->m_FileIndexItem.itemMsg.push_back(pThis->m_queue.front().FileURL);
			char tmpTm[256];
			memset(tmpTm, 0, sizeof(tmpTm));
			sprintf_s(tmpTm, "%llu", pThis->m_queue.front().FileLen);
			pThis->m_FileIndexItem.itemMsg.push_back(tmpTm);
			memset(tmpTm, 0, sizeof(tmpTm));
			sprintf_s(tmpTm, "%04d-%02d-%02d %02d:%02d:%02d",
				pThis->m_queue.front().CreationTime.wYear,
				pThis->m_queue.front().CreationTime.wMonth,
				pThis->m_queue.front().CreationTime.wDay,
				pThis->m_queue.front().CreationTime.wHour,
				pThis->m_queue.front().CreationTime.wMinute,
				pThis->m_queue.front().CreationTime.wSecond);
			pThis->m_FileIndexItem.itemMsg.push_back(tmpTm);

			memset(tmpTm, 0, sizeof(tmpTm));
			sprintf_s(tmpTm, "%04d-%02d-%02d %02d:%02d:%02d",
				pThis->m_queue.front().lastAccessTime.wYear,
				pThis->m_queue.front().lastAccessTime.wMonth,
				pThis->m_queue.front().lastAccessTime.wDay,
				pThis->m_queue.front().lastAccessTime.wHour,
				pThis->m_queue.front().lastAccessTime.wMinute,
				pThis->m_queue.front().lastAccessTime.wSecond);
			pThis->m_FileIndexItem.itemMsg.push_back(tmpTm);

			memset(tmpTm, 0, sizeof(tmpTm));
			sprintf_s(tmpTm, "%04d-%02d-%02d %02d:%02d:%02d",
				pThis->m_queue.front().LastWriteTime.wYear,
				pThis->m_queue.front().LastWriteTime.wMonth,
				pThis->m_queue.front().LastWriteTime.wDay,
				pThis->m_queue.front().LastWriteTime.wHour,
				pThis->m_queue.front().LastWriteTime.wMinute,
				pThis->m_queue.front().LastWriteTime.wSecond);
			pThis->m_FileIndexItem.itemMsg.push_back(tmpTm);

			pThis->m_Localdb.AddNew(pThis->m_FileIndexItem);
			pThis->GetDlgItem(IDC_STATIC_INFO)->SetWindowText(pThis->m_queue.front().FilePath);
			pThis->m_FileNr++;
			pThis->m_queue.pop();
			LeaveCriticalSection(&pThis->m_lock);
		} 
		else
		{
			DWORD ExitCode1 = 0, ExitCode2 = 0;
			GetExitCodeThread(pThis->m_ThreadId[0], &ExitCode1);
			GetExitCodeThread(pThis->m_ThreadId[0], &ExitCode2);
			if (ExitCode1 != STILL_ACTIVE && ExitCode2 != STILL_ACTIVE)
				break;
			Sleep(5);
		}
	}
	int i = GetTickCount() - pThis->m_clickCount;
	pThis->GetDlgItem(IDC_STATIC_INFO)->SetWindowText("");
}

void CEverythingDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code her
	LeaveCriticalSection(&m_lock);
	for(int i = 0; i < MAX_THREAD_NUM; i++)
	{
		CloseHandle(m_ThreadId[i]);
		m_ThreadId[i] = NULL;
	}
	while(!m_queue.empty())
		m_queue.pop();
	m_dirvername.clear();
	m_filevector.clear();
	m_Localdb.CloseTable();
	m_Localdb.CloseAdo();
	//m_Infodb.CloseAdo();
}

void CEverythingDlg::OnBnClickedSearch()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_Filelist.DeleteAllItems();
	m_FileIndexItem.reset();
	m_Localdb.ReadAdo("FileName", m_Text.GetBuffer(0), m_FileIndexItem, GetDlgItem(IDC_STATIC_INFO)->GetSafeHwnd());	
	int j = 0;
	for (unsigned int i = 0; i < m_FileIndexItem.itemMsg.size()/m_FileIndexItem.itemName.size(); i++)
	{
		m_Filelist.InsertItem (i, m_FileIndexItem.itemMsg[j++]);
		m_Filelist.SetItemText(i, 1, m_FileIndexItem.itemMsg[j++]);
		m_Filelist.SetItemText(i, 2, m_FileIndexItem.itemMsg[j++]);
		m_Filelist.SetItemText(i, 3, m_FileIndexItem.itemMsg[j++]);
		m_Filelist.SetItemText(i, 4, m_FileIndexItem.itemMsg[j++]);
		m_Filelist.SetItemText(i, 5, m_FileIndexItem.itemMsg[j++]);
		m_Filelist.SetItemText(i, 6, m_FileIndexItem.itemMsg[j++]);
		m_Filelist.SetItemText(i, 7, m_FileIndexItem.itemMsg[j++]);
		m_Filelist.SetItemText(i, 8, m_FileIndexItem.itemMsg[j++]);
	}
}


void CEverythingDlg::OnBnClickedSearchinfo()
{
	// TODO: Add your control notification handler code here
}

BOOL CEverythingDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		OnBnClickedSearch();
		return true;
	}
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
	{
		m_Text = "";
		UpdateData(false);
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CEverythingDlg::OnLvnItemchangedListFile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	//int index = m_Filelist
	*pResult = 0;
}
