// ClockDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Calendar.h"
#include "ClockDlg.h"
#include "math.h"

// CClockDlg dialog

#define MUTILPI 3.1415926*2
IMPLEMENT_DYNAMIC(CClockDlg, CDialog)

CClockDlg::CClockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClockDlg::IDD, pParent)
{

}

CClockDlg::~CClockDlg()
{
}

void CClockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CClockDlg, CDialog)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CClockDlg message handlers
unsigned int justOneTime = 1;
void CClockDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	CDC *pDC = this->GetDC();
	CRect rc;
	GetClientRect(&rc);
	int xStart = rc.right/2;
	int yStart = rc.bottom/2;
	CTime time = CTime::GetCurrentTime();
	CString strDigits;
	int x,y;
	CSize size;
	CPen Pen(PS_NULL, 0, RGB(25, 25, 25));
	CPen *pOldPen = pDC->SelectObject(&Pen);
	pDC->Ellipse(rc.left + 20, rc.top + 25, rc.right - 23, rc.bottom - 20);
	double Radians;
	if (justOneTime == 1)
	{
		justOneTime++;
		for(int i = 37; i <= 96; i++)
		{
			strDigits.Format(".");
			if (i%5)
				pDC->SetTextColor(RGB(95, 95, 95));
			else
				pDC->SetTextColor(RGB(60, 255, 121));
			size = pDC->GetTextExtent(strDigits, strDigits.GetLength());
			Radians = (double)i*MUTILPI/60;
			x = (int)(xStart- (size.cx/2) + ((double)((xStart - 5)*cos(Radians))));
			y = (int)(yStart- (size.cy/2) + ((double)((yStart - 5)*sin(Radians))));
			pDC->TextOut(x, y, strDigits);
		}
	}
	//hour
	Radians = (double)time.GetHour() + (double)time.GetMinute()/60.0 + (double)time.GetSecond()/3600.0;
	Radians *= MUTILPI/12.0;
	CPen HourPen(PS_SOLID, 5, RGB(73, 107, 91));
	pDC->SelectObject(&HourPen);
	pDC->MoveTo(xStart, yStart);
	pDC->LineTo(xStart + (int)(double)((xStart/3)*sin(Radians)), yStart - (int)(double)((yStart/3)*cos(Radians))),
	//minute
	Radians = (double)time.GetMinute() + (double)time.GetSecond()/60.0;
	Radians *= MUTILPI/60.0;
	CPen MinutePen(PS_SOLID, 3, RGB(73, 107, 91));
	pDC->SelectObject(&MinutePen);
	pDC->MoveTo(xStart, yStart);
	pDC->LineTo(xStart + (int)(double)((xStart*3/5)*sin(Radians)), yStart - (int)(double)((yStart*3/5)*cos(Radians))),
	//second
	Radians = (double)time.GetSecond();
	Radians *= MUTILPI/60.0;
	CPen SecnodPen(PS_SOLID, 2, RGB(0, 0, 0));
	pDC->SelectObject(&SecnodPen);
	pDC->MoveTo(xStart, yStart);
	pDC->LineTo(xStart + (int)(double)((xStart*0.7)*sin(Radians)), yStart - (int)(double)((yStart*0.7)*cos(Radians))),

	pDC->SelectObject(pOldPen);
	CDialog::OnTimer(nIDEvent);
}

BOOL CClockDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here

	m_Brush.CreateSolidBrush(RGB(0xF5, 0xF5, 0xF5));
	SetTimer(1, 1000, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CClockDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  Change any attributes of the DC here
	pDC->SetBkMode(TRANSPARENT);
	return m_Brush; 
	// TODO:  Return a different brush if the default is not desired
	return hbr;
}
