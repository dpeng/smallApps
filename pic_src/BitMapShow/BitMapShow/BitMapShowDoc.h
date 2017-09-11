// BitMapShowDoc.h : interface of the CBitMapShowDoc class
//


#pragma once


class CBitMapShowDoc : public CDocument
{
protected: // create from serialization only
	CBitMapShowDoc();
	DECLARE_DYNCREATE(CBitMapShowDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CBitMapShowDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};


