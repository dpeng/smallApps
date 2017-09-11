// beexb_gif.h : main header file for the BEEXB_GIF application
//

#if !defined(AFX_BEEXB_GIF_H__69C69FF1_7FD4_474F_8749_7674AF930C9D__INCLUDED_)
#define AFX_BEEXB_GIF_H__69C69FF1_7FD4_474F_8749_7674AF930C9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CBeexb_gifApp:
// See beexb_gif.cpp for the implementation of this class
//

class CBeexb_gifApp : public CWinApp
{
public:
	CBeexb_gifApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBeexb_gifApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CBeexb_gifApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BEEXB_GIF_H__69C69FF1_7FD4_474F_8749_7674AF930C9D__INCLUDED_)
