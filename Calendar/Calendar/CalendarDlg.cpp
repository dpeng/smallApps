// CalendarDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Calendar.h"
#include "CalendarDlg.h"
#include "LunarCalendar.h"
#include "ChineseEra.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define BEGINYEAR 2000
const int aiMounth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; // everymounth`s days 
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


// CCalendarDlg dialog




CCalendarDlg::CCalendarDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCalendarDlg::IDD, pParent)
	, m_iYear(0)
	, m_iMonth(0)
	, m_iDay(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCalendarDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_YEAR, m_Year);
	DDX_CBIndex(pDX, IDC_COMBO_YEAR, m_iYear);
	DDX_Control(pDX, IDC_COMBO_MONTH, m_Month);
	DDX_CBIndex(pDX, IDC_COMBO_MONTH, m_iMonth);
	DDX_Control(pDX, IDC_COMBO_DAY, m_Day);
	DDX_CBIndex(pDX, IDC_COMBO_DAY, m_iDay);
	DDX_Control(pDX, IDC_STATIC_TIME, m_DigitalClock);
}

BEGIN_MESSAGE_MAP(CCalendarDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_TODAY, &CCalendarDlg::OnClickedToday)
	ON_CBN_SELCHANGE(IDC_COMBO_YEAR, &CCalendarDlg::OnCbnSelchangeComboYear)
	ON_CBN_SELCHANGE(IDC_COMBO_MONTH, &CCalendarDlg::OnCbnSelchangeComboMonth)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_DAY, &CCalendarDlg::OnCbnSelchangeComboDay)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CCalendarDlg message handlers

BOOL CCalendarDlg::OnInitDialog()
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

	m_bGetTime = true;
	m_TickTm = 0;
	char Str[10];
	for (int i = 1; i <= 2099; i++)
	{
		memset(Str, 0, sizeof(Str));
		sprintf_s(Str, "%04d", i);
		m_Year.InsertString(i-1, (LPCSTR)Str);
		m_Year.SetItemData(i, i);
	}
	for (int i = 1; i < 13; i++)
	{
		memset(Str, 0, sizeof(Str));
		sprintf_s(Str, "%d", i);
		m_Month.InsertString(i-1, (LPCSTR)Str);
		m_Month.SetItemData(i, i);
	}
	memset(&m_tmTime, 0, sizeof(SYSTEMTIME));
	GetLocalTime(&m_tmTime);
	for(int i = 1; i < GetDays(m_tmTime.wYear, m_tmTime.wMonth) + 1; i++)
	{
		memset(Str, 0, sizeof(Str));
		sprintf_s(Str, "%d", i);
		m_Day.InsertString(i-1, (LPCSTR)Str);
		m_Day.SetItemData(i, i);
	}
	OnClickedToday();
	SetTimer(1, 40, NULL);
	m_Brush.CreateSolidBrush(RGB(0xF5, 0xF5, 0xF5));

	m_Clock.Create(IDD_CLOCKDLG, this);
	CRect rect;
	GetDlgItem(IDC_STATIC_CLOCK)->GetWindowRect(rect);
	ScreenToClient(rect);
	rect.DeflateRect(20,12,15,2);
	m_Clock.MoveWindow(rect);
	m_Clock.ShowWindow(SW_HIDE);
	SetTimeFont();
	m_DigitalClock.SetBkColor(RGB(0xF5, 0xF5, 0xF5));
	m_DigitalClock.SetTextColor(RGB(125,125,125));
	m_water.Create(352, 288);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCalendarDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CCalendarDlg::OnPaint()
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
HCURSOR CCalendarDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CCalendarDlg::OnClickedToday()
{
	((CStatic*)GetDlgItem(IDC_TODAY))->ModifyStyle(1, SS_SUNKEN, 1);
	memset(&m_tmTime, 0, sizeof(SYSTEMTIME));
	GetLocalTime(&m_tmTime);
	m_iYear = m_tmTime.wYear - 1;
	m_iMonth = m_tmTime.wMonth - 1;
	m_iDay = m_tmTime.wDay - 1;
	UpdateData(false);
	OnBnClickedInquiry();
	DispChineseEra();
	//(CStatic*)GetDlgItem(IDC_TODAY)->ModifyStyle(0, SS_SUNKEN, 0);
}
void CCalendarDlg::OnBnClickedInquiry()
{
	// TODO: Add your control notification handler code here
	int iYear, iMounth, iBeginDayOfaWeek, iDays;
	UpdateData(TRUE);
	iYear = m_iYear+1;
	iMounth = m_iMonth+1;
	// if Input a wrong num 
	if( iYear <= 0 || iMounth <= 0 || iMounth > 12 ) 
	{
		return; 
	}

	// do what you want me to do 
	iBeginDayOfaWeek = GetBeginDay( iYear, iMounth ); 
	iDays = GetDays( iYear, iMounth ); 
	PrintTitleAndDate( iBeginDayOfaWeek, iDays ); 
}

int CCalendarDlg::GetBeginDay(int iYear, int iMonth)
{
	int iResult; 
	int iDifference = iYear - BEGINYEAR; // i want to know if input if larger than my begin year 
	long lSumOfDays = 0; 

	if( iDifference >= 0 ) // if input is larger than my begin year 
	{ 
		for( int i = BEGINYEAR; i < iYear; i++ ) 
			lSumOfDays += GetDaysOfaYear( i ); 
		for( int k = 1; k < iMonth; k++ ) 
			if( 2 == k && ( ( iYear % 4 == 0 && iYear % 100 != 0 ) || iYear % 400 == 0 ) ) 
				lSumOfDays += 29; 
			else 
				lSumOfDays += aiMounth[k-1]; 
		iResult = ( int )( ( lSumOfDays + 6 ) % 7 ); 
	} // end the if part of if/else 
	else // if input is smaller than my begin year 
	{ 
		for( int j = iYear + 1; j < BEGINYEAR; j++ ) 
			lSumOfDays += GetDaysOfaYear( j ); 
		for( int m = iMonth; m <= 12 ; m++ ) 
			if( 2 == m && ( ( iYear % 4 == 0 && iYear % 100 != 0 ) || iYear % 400 == 0 ) ) 
				lSumOfDays += 29; 
			else 
				lSumOfDays += aiMounth[m-1]; 
		iResult = ( int )( ( lSumOfDays + 1 ) % 7 ); 

		iResult = 7 - iResult;
	} // end else part of if/else 

	return iResult; 
}

int CCalendarDlg::GetDaysOfaYear(int iYear)
{
	if( ( iYear % 4 == 0 && iYear % 100 != 0 ) || iYear % 400 == 0 ) 
		return 366; // if the year is a leap year 
	return 365; // normal year 
}

int CCalendarDlg::GetDays(int iYear, int iMonth)
{
	if( 2 == iMonth && ( ( iYear % 4 == 0 && iYear % 100 != 0 ) || iYear % 400 == 0 ) ) 
		return 29; // if the year is a leap year 
	return aiMounth[iMonth - 1]; // normal year 
}
const char *ChDay[] = {"*","初一","初二","初三","初四","初五",

"初六","初七","初八","初九","初十",

"十一","十二","十三","十四","十五",

"十六","十七","十八","十九","二十",

"廿一","廿二","廿三","廿四","廿五", 

"廿六","廿七","廿八","廿九","三十"};

const char *ChMonth[] = {"*","正","二","三","四","五","六","七","八","九","十","冬","腊"};


void CCalendarDlg::PrintTitleAndDate(int iBeginDayOfaWeek, int iDays)
{
	char str[26] = "";
	char TmpStr[2*1024] = {0};
	char TmpLunarStr[1024] = {0};
	strcat_s(TmpStr, "            日            一            二            三            四            五            六\n\n");
	char CurDate[64];
	for( int i = 0; i < iBeginDayOfaWeek; i++ ) 
	{
		strcat_s(TmpStr, "                ");
		if (m_iYear > 1900 && m_iYear < 2099)
			strcat_s(TmpLunarStr, "                ");
	}

	// out put the date
	for( int i = 1; i <= iDays; i++ ) 
	{ 
		memset(CurDate, 0, sizeof(CurDate));
		memset(str, 0, sizeof(str));
		if( ( 0 == ( i + iBeginDayOfaWeek - 1 ) % 7 ) && (i != 1 || iBeginDayOfaWeek == 7) ) 
		{
			strcat_s(TmpStr, "\n"); // if the day is Sunday 
			if (m_iYear > 1900 && m_iYear < 2099)
			{
				strcat_s(TmpLunarStr, "\n\n");
				strcat_s(TmpStr, TmpLunarStr);
				memset(&TmpLunarStr, 0, sizeof(TmpLunarStr));
			}
		}
		if (m_iYear > 1900 && m_iYear < 2099)
		{
			int nRet = LunarCalendar(m_iYear+1,m_iMonth+1,i);
			if ((LunarCalendarDay & 0x3F) == 1)
			{
				if (nRet)
				{
					strcat_s(str,"    闰");
					strcat_s(str,ChMonth[(LunarCalendarDay & 0x3C0) >> 6]);
				}

				else
				{
					strcat_s(str, "        ");
					strcat_s(str,ChMonth[(LunarCalendarDay & 0x3C0) >> 6]);
				}
				strcat_s(str,"月");
			}
			else
			{
				memset(&str, 0, sizeof(str));
				strcat_s(str, "        ");
				COleDateTime tmMeas;
				tmMeas.SetDateTime(m_iYear+1, m_iMonth +1, i, 1,1,1);
				m_Lunar.LunarCalendar(tmMeas);
				if (m_Lunar.m_solarTerms.GetLength() > 3)
				{
					strcat_s(str,m_Lunar.m_solarTerms);
				} 
				else
				{
					strcat_s(str,ChDay[LunarCalendarDay & 0x3F]);
				}
			}
		}
		sprintf_s(CurDate, "            %02d", i);
		if (m_iYear > 1900 && m_iYear < 2099)
			strcat_s(TmpLunarStr, str);
		strcat_s(TmpStr, CurDate); 
	} 
	if (m_iYear > 1900 && m_iYear < 2099)
	{
		strcat_s(TmpStr, "\n");
		strcat_s(TmpStr, TmpLunarStr);
	}

	//char ddd[100] = {0};
	//sprintf(ddd, "TmpStr:%d  TmpLunarStr:%d\n",strlen(TmpStr), strlen(TmpLunarStr));
	//OutputDebugString(ddd);
	GetDlgItem(IDC_STATIC_DISP)->SetWindowText(TmpStr);
}
void CCalendarDlg::OnCbnSelchangeComboYear()
{
	// TODO: Add your control notification handler code here
	OnBnClickedInquiry();
	DispChineseEra();
}

void CCalendarDlg::OnCbnSelchangeComboMonth()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int TmpDay = m_iDay + 1;
	m_Day.ResetContent();
	char Str[10];
	int nTmpDay = GetDays(m_iYear+1, m_iMonth+1);
	for(int i = 1; i <  nTmpDay + 1; i++)
	{
		memset(Str, 0, sizeof(Str));
		sprintf_s(Str, "%d", i);
		m_Day.InsertString(i-1, (LPCSTR)Str);
		m_Day.SetItemData(i, i);
	}
	if (TmpDay <= nTmpDay)
	{
		m_iDay = TmpDay - 1;
	}
	else
	{
		m_iDay = 1;
	}
	UpdateData(false);
	OnBnClickedInquiry();
	DispChineseEra();
}

const char *AnimalSign[] = {"鼠","牛","虎","兔","龙","蛇","马","羊","猴","鸡","狗","猪"};
const char *TianGan[] = {"甲","乙","丙","丁","戊","己","庚","辛","壬","癸"};
const char *DiZhi[] = {"子","丑","寅","卯","辰","巳","午","未","申","酉","戌","亥"};

void CCalendarDlg::DispChineseEra()
{
	if (m_iYear >= 1900 && m_iYear < 2099)
	{
		char Era[512];
		int LunarYear = GetLunarYear(m_iYear+1, m_iMonth+1, m_iDay+1);

		COleDateTime tmMeas;
		tmMeas.SetDateTime(m_iYear+1, m_iMonth +1, m_iDay+1, 1,1,1);
		m_Lunar.LunarCalendar(tmMeas);


		sprintf_s(Era, "%d 年 %d 月 %d 日(%s %s月 %s)\n",
			m_iYear+1, 
			m_iMonth+1, 
			m_iDay+1, 
			m_Lunar.m_cYearName,
			m_Lunar.m_cWordMonth,
			m_Lunar.m_cWordDay);
		strcat_s(Era,"生肖：");
		strcat_s(Era,AnimalSign[GetChineseEra(LunarYear,1)]);
		strcat_s(Era,"\n年柱：");
		strcat_s(Era, m_Lunar.m_cYear);
		strcat_s(Era,"\n月柱：");
		strcat_s(Era, m_Lunar.m_cMonth);
		strcat_s(Era,"\n日柱：");
		strcat_s(Era, m_Lunar.m_cDay);
		strcat_s(Era,"\n农历节日：");
		strcat_s(Era, m_Lunar.m_lunarFestival);
		strcat_s(Era, "\n公历节日：");
		strcat_s(Era, m_Lunar.m_solarFestival);
		strcat_s(Era, "\n节气：");
		strcat_s(Era, m_Lunar.m_solarTerms);
		GetDlgItem(IDC_STATIC_ERA)->SetWindowText(Era);
	}
	else
	{
		GetDlgItem(IDC_STATIC_ERA)->SetWindowText("超出农历计算范围：1900～2099");
	}

}
void CCalendarDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == 1)
	{
		memset(&m_tmTime, 0, sizeof(SYSTEMTIME));
		memset(m_tmStr, 0, sizeof(m_tmStr));
		GetLocalTime(&m_tmTime);
		sprintf_s(m_tmStr, "%04d 年 %02d 月 %02d 日   %02d 时 %02d 分 %02d 秒 %03d 毫秒",
			m_tmTime.wYear,
			m_tmTime.wMonth,
			m_tmTime.wDay,
			m_tmTime.wHour,
			m_tmTime.wMinute,
			m_tmTime.wSecond,
			m_tmTime.wMilliseconds);
		//GetDlgItem(IDC_STATIC_SEC)->SetWindowText(m_tmStr);
		m_DigitalClock.SetClock(m_tmTime.wHour, m_tmTime.wMinute, m_tmTime.wSecond, m_tmTime.wMilliseconds);
	}
	if (nIDEvent == 2)
	{
		memset(m_tmStr, 0, sizeof(m_tmStr));
		int CurTm = GetTickCount() - m_TickTm;
		int h = CurTm/3600000;
		CurTm -= h*3600000;
		int m = CurTm/60000;
		CurTm -= m*60000;
		int s = CurTm/1000;
		CurTm -= s*1000;
		sprintf_s(m_tmStr, "%02d ：%02d ：%02d：%03d", h, m, s, CurTm);
		GetDlgItem(IDC_STATIC_SEC)->SetWindowText(m_tmStr);
	}
	CDialog::OnTimer(nIDEvent);
}

void CCalendarDlg::OnCbnSelchangeComboDay()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	DispChineseEra();
}


BOOL CCalendarDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
		return true;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE)
	{
		POINT pt = {0};
		CRect Rc;
		GetDlgItem(IDC_STATIC_SEC)->GetWindowRect(&Rc);
		HRGN hRgn = CreateRoundRectRgn(Rc.left, Rc.top, Rc.right, Rc.bottom, Rc.Width(), Rc.Height());
		GetCursorPos(&pt);
		if (PtInRegion(hRgn, pt.x, pt.y))
		{
			memset(m_tmStr, 0, sizeof(m_tmStr));
			if(m_bGetTime)
			{
				m_TickTm = 0;
				sprintf_s(m_tmStr, "%d", m_TickTm);
				m_TickTm = GetTickCount();
				SetTimer(2, 10, NULL);
			}
			else
			{
				int CurTm = GetTickCount() - m_TickTm;
				int h = CurTm/3600000;
				CurTm -= h*3600000;
				int m = CurTm/60000;
				CurTm -= m*60000;
				int s = CurTm/1000;
				CurTm -= s*1000;
				sprintf_s(m_tmStr, "%02d ：%02d ：%02d：%03d", h, m, s, CurTm);
				KillTimer(2);
			}
			GetDlgItem(IDC_STATIC_SEC)->SetWindowText(m_tmStr);
			m_bGetTime = !m_bGetTime;
		}
	}
	if (m_bGetTime && pMsg->message == WM_MOUSEMOVE)
	{
		POINT pt = {0};
		CRect Rc;
		GetDlgItem(IDC_STATIC_SEC)->GetWindowRect(&Rc);
		HRGN hRgn = CreateRoundRectRgn(Rc.left, Rc.top, Rc.right, Rc.bottom, Rc.Width(), Rc.Height());
		GetCursorPos(&pt);
		if (PtInRegion(hRgn, pt.x, pt.y))
		{
			GetDlgItem(IDC_STATIC_SEC)->SetWindowText("00 ：00 ：00：000");
		}
		else
		{
			m_CStr = "";
			GetDlgItem(IDC_STATIC_SEC)->GetWindowText(m_CStr);
			if (strlen(m_CStr) != 38)
				GetDlgItem(IDC_STATIC_SEC)->SetWindowText("停秒:鼠标放在此区域上,按空格开始或停止");
		}
	}
	if (pMsg->message == WM_LBUTTONDOWN)
	{
		POINT pt = {0};
		CRect Rc;
		GetDlgItem(IDC_TODAY)->GetWindowRect(&Rc);
		HRGN hRgn = CreateRoundRectRgn(Rc.left, Rc.top, Rc.right, Rc.bottom, Rc.Width(), Rc.Height());
		GetCursorPos(&pt);
		if (PtInRegion(hRgn, pt.x, pt.y))
			OnClickedToday();
	}
	//if (pMsg->message == WM_LBUTTONDBLCLK)
	//{
	//	m_Clock.ModifyStyle(WS_CHILD, WS_POPUP|WS_DLGFRAME|DS_MODALFRAME);
	//	m_Clock.SetParent(NULL);
	//	LONG lStyle = ::GetWindowLong(m_Clock.GetSafeHwnd(), GWL_STYLE);
	//	lStyle = lStyle | WS_THICKFRAME | WS_CAPTION/* | WS_SYSMENU*/;
	//	::SetWindowLong(m_Clock.GetSafeHwnd(), GWL_STYLE, lStyle);
	//	m_Clock.SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);
	//}
	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CCalendarDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	pDC->SetBkMode(TRANSPARENT);
	return m_Brush; 
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}

bool CCalendarDlg::SetTimeFont()
{
	CFont *pfont;
	pfont = new CFont;
	pfont->CreateFont(20,          // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("宋体"));    // lpszFac
	GetDlgItem(IDC_STATIC_TIME)->SetFont(pfont);
	delete pfont;
	pfont = NULL;
	return true;
}
