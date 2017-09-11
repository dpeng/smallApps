// beexb_gifDoc.cpp : implementation of the CBeexb_gifDoc class
//

#include "stdafx.h"
#include "beexb_gif.h"

#include "beexb_gifDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBeexb_gifDoc

IMPLEMENT_DYNCREATE(CBeexb_gifDoc, CDocument)

BEGIN_MESSAGE_MAP(CBeexb_gifDoc, CDocument)
	//{{AFX_MSG_MAP(CBeexb_gifDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBeexb_gifDoc construction/destruction

CBeexb_gifDoc::CBeexb_gifDoc()
{
	// TODO: add one-time construction code here

}

CBeexb_gifDoc::~CBeexb_gifDoc()
{
}

BOOL CBeexb_gifDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CBeexb_gifDoc serialization

void CBeexb_gifDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBeexb_gifDoc diagnostics

#ifdef _DEBUG
void CBeexb_gifDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBeexb_gifDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBeexb_gifDoc commands
