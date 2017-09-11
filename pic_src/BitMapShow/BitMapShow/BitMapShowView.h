// BitMapShowView.h : interface of the CBitMapShowView class
//


#pragma once


class CBitMapShowView : public CView
{
protected: // create from serialization only
	CBitMapShowView();
	DECLARE_DYNCREATE(CBitMapShowView)

// Attributes
public:
	CBitMapShowDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CBitMapShowView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileOpen();
public:
	CString m_sFileName;
};

#ifndef _DEBUG  // debug version in BitMapShowView.cpp
inline CBitMapShowDoc* CBitMapShowView::GetDocument() const
   { return reinterpret_cast<CBitMapShowDoc*>(m_pDocument); }
#endif

