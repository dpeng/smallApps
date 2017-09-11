// BitMapShowDoc.cpp : implementation of the CBitMapShowDoc class
//

#include "stdafx.h"
#include "BitMapShow.h"

#include "BitMapShowDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBitMapShowDoc

IMPLEMENT_DYNCREATE(CBitMapShowDoc, CDocument)

BEGIN_MESSAGE_MAP(CBitMapShowDoc, CDocument)
END_MESSAGE_MAP()


// CBitMapShowDoc construction/destruction

CBitMapShowDoc::CBitMapShowDoc()
{
	// TODO: add one-time construction code here

}

CBitMapShowDoc::~CBitMapShowDoc()
{
}

BOOL CBitMapShowDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CBitMapShowDoc serialization

void CBitMapShowDoc::Serialize(CArchive& ar)
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


// CBitMapShowDoc diagnostics

#ifdef _DEBUG
void CBitMapShowDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBitMapShowDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CBitMapShowDoc commands
