// beexb_gifDoc.h : interface of the CBeexb_gifDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BEEXB_GIFDOC_H__BCA6E444_7A7F_43EA_BB3D_150857B20C0C__INCLUDED_)
#define AFX_BEEXB_GIFDOC_H__BCA6E444_7A7F_43EA_BB3D_150857B20C0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBeexb_gifDoc : public CDocument
{
protected: // create from serialization only
	CBeexb_gifDoc();
	DECLARE_DYNCREATE(CBeexb_gifDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBeexb_gifDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBeexb_gifDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBeexb_gifDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BEEXB_GIFDOC_H__BCA6E444_7A7F_43EA_BB3D_150857B20C0C__INCLUDED_)
