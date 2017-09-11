// keyCount.h : main header file for the KEYCOUNT application
//

#if !defined(AFX_KEYCOUNT_H__035602CD_44E8_4408_9E2E_1F7E18E4F44C__INCLUDED_)
#define AFX_KEYCOUNT_H__035602CD_44E8_4408_9E2E_1F7E18E4F44C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CKeyCountApp:
// See keyCount.cpp for the implementation of this class
//

class CKeyCountApp : public CWinApp
{
public:
	CKeyCountApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKeyCountApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CKeyCountApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KEYCOUNT_H__035602CD_44E8_4408_9E2E_1F7E18E4F44C__INCLUDED_)
