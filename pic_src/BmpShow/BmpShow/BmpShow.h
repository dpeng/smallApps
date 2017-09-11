// BmpShow.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CBmpShowApp:
// See BmpShow.cpp for the implementation of this class
//

class CBmpShowApp : public CWinApp
{
public:
	CBmpShowApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CBmpShowApp theApp;