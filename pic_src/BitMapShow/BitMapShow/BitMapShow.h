// BitMapShow.h : main header file for the BitMapShow application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CBitMapShowApp:
// See BitMapShow.cpp for the implementation of this class
//

class CBitMapShowApp : public CWinApp
{
public:
	CBitMapShowApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CBitMapShowApp theApp;