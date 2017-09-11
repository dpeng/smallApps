// YUVPlayerDlg.h : header file
//

#pragma once
#include "colorspace.h"
#include <ddraw.h>
// CYUVPlayerDlg dialog
class CYUVPlayerDlg : public CDialog
{
// Construction
public:
	CYUVPlayerDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_YUVPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOpenfile();
	CString m_FileName;
	afx_msg void OnBnClickedPlay();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int m_nWidth;
	int m_nHight;
	color_convert_func m_colorConvert;
	LPDIRECTDRAWSURFACE7 m_surface;
	int initDirectDraw(void);
	LPDIRECTDRAW7 m_pDD;
	LPDIRECTDRAWSURFACE7 m_pDDSPrimary;
	HRESULT CreateDrawSurface(DDSURFACEDESC2 * pDdsd, LPDIRECTDRAWSURFACE7 * pSurface);
	unsigned char* m_buf;
	unsigned char* m_py; 
	unsigned char* m_pu;
	unsigned char* m_pv;

	afx_msg void OnBnClickedStop();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	HRESULT destroyDDObjects();
	afx_msg void OnBnClickedClosefile();
};
