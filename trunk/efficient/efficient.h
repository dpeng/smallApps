// efficient.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CefficientApp:
// See efficient.cpp for the implementation of this class
//

class CefficientApp : public CWinApp
{
public:
	CefficientApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation
private:		
	HANDLE	m_hMutex;
	DECLARE_MESSAGE_MAP()
};

extern CefficientApp theApp;