// CalendarDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "Lunar.h"
#include "afxdtctl.h"
#include "ClockDlg.h"
#include "Water.h"
#include "DigitalClock.h"


// CCalendarDlg dialog
class CCalendarDlg : public CDialog
{
// Construction
public:
	CCalendarDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CALENDAR_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedInquiry();
	afx_msg void OnClickedToday();
	int GetBeginDay(int iYear, int iMonth);
	int GetDaysOfaYear(int iYear);
	int GetDays(int iYear, int iMonth);
	void PrintTitleAndDate(int iBeginDayOfaWeek, int iDays);
	CComboBox m_Year;
	int m_iYear;
	CComboBox m_Month;
	int m_iMonth;
	afx_msg void OnCbnSelchangeComboYear();
	afx_msg void OnCbnSelchangeComboMonth();
	void DispChineseEra();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CLunar m_Lunar;
	CComboBox m_Day;
	int m_iDay;
	afx_msg void OnCbnSelchangeComboDay();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CBrush m_Brush;
	CClockDlg m_Clock;
	bool SetTimeFont(void);
	char m_tmStr[512];
	SYSTEMTIME m_tmTime;
	bool m_bGetTime;
	DWORD m_TickTm;
	CString m_CStr;
private:
	CWater m_water;
	CDigitalClock m_DigitalClock;
};
