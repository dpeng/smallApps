// myweb.h : main header file for the MYWEB application
//

#if !defined(AFX_MYWEB_H__00D3E392_351F_4F14_AC8C_C3FFE89F9F9A__INCLUDED_)
#define AFX_MYWEB_H__00D3E392_351F_4F14_AC8C_C3FFE89F9F9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMywebApp:
// See myweb.cpp for the implementation of this class
//

class CMywebApp : public CWinApp
{
public:
	CMywebApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMywebApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMywebApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYWEB_H__00D3E392_351F_4F14_AC8C_C3FFE89F9F9A__INCLUDED_)
