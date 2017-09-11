// beexb_gifView.h : interface of the CBeexb_gifView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BEEXB_GIFVIEW_H__194BC3FD_AAAD_471A_AFA9_138AE9CA3637__INCLUDED_)
#define AFX_BEEXB_GIFVIEW_H__194BC3FD_AAAD_471A_AFA9_138AE9CA3637__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Atlbase.h>
class CBeexb_gifView : public CView
{
protected: // create from serialization only
	CBeexb_gifView();
	DECLARE_DYNCREATE(CBeexb_gifView)

// Attributes
public:
	CBeexb_gifDoc* GetDocument();

// Operations
public:
public:
	CDC* mdc;
    CDC* m_gifDC;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBeexb_gifView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBeexb_gifView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBeexb_gifView)
	afx_msg void OnShowGif();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in beexb_gifView.cpp
inline CBeexb_gifDoc* CBeexb_gifView::GetDocument()
   { return (CBeexb_gifDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BEEXB_GIFVIEW_H__194BC3FD_AAAD_471A_AFA9_138AE9CA3637__INCLUDED_)
