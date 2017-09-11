// efficient.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "efficient.h"
#include "efficientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CefficientApp

BEGIN_MESSAGE_MAP(CefficientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CefficientApp construction

CefficientApp::CefficientApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CefficientApp object

CefficientApp theApp;


// CefficientApp initialization

BOOL CefficientApp::InitInstance()
{
	CWinApp::InitInstance();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));		
	m_hMutex = CreateMutex(NULL, FALSE, _T("EFFICIENT_INSTANCE"));
	DWORD dwLastError = GetLastError();
	if (m_hMutex == NULL || dwLastError == ERROR_ALREADY_EXISTS)
	{
		if (dwLastError == ERROR_ALREADY_EXISTS)
		{
			MessageBox(NULL, _T("efficient has been started"), _T("Waring"), MB_OK|MB_ICONINFORMATION);
		}
		CloseHandle(m_hMutex);	
		ReleaseMutex(m_hMutex);	
		m_hMutex = NULL;
		return FALSE;
	}


	CefficientDlg dlg;
	m_pMainWnd = &dlg;
	dlg.m_hInstanceTmp = m_hInstance;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
