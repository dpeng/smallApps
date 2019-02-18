#if !defined(AFX_MULITLINELISTBOX_H__D705CB99_9FD0_424E_BD71_027547449AE5__INCLUDED_)
#define AFX_MULITLINELISTBOX_H__D705CB99_9FD0_424E_BD71_027547449AE5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MulitLineListBox.h : header file
//

#define MAX_ITEM_SHOW_IN_LIST 1024
#define TITLE_STRING_FONT_HEIGHT 24
#define SUBTITLE_STRING_FONT_HEIGHT 16
#define SEARCH_RESULT_ITEM_HEIGHT (TITLE_STRING_FONT_HEIGHT + SUBTITLE_STRING_FONT_HEIGHT + 4)
#define SEARCH_BOX_BODER_HEIGHT 5
#define SEARCH_BOX_HEIGHT (SEARCH_RESULT_ITEM_HEIGHT + SEARCH_BOX_BODER_HEIGHT * 2)
#define MAGIC_STRING_FOR_LAST_DRAW _T("abdcefhgpdffdpcasperrepsac")
/////////////////////////////////////////////////////////////////////////////
// CMulitLineListBox window
typedef struct _LISTBOX_COLOR_
{
	CString titleStr;
	CString subTitleStr;
	COLORREF fgColor;
	COLORREF bgColor;
	_LISTBOX_COLOR_()
	{
		titleStr.Empty();
		subTitleStr.Empty();
		fgColor = RGB(0, 0, 0);
		bgColor = RGB(255, 255, 255);
	}
}LISTBOX_COLOR, *PLISTBOX_COLOR;

class CMultiLineListBox : public CListBox
{
// Construction
public:
	CMultiLineListBox();

// Attributes
public:
	void AppendString(LPCSTR titleStr, LPCSTR subTitleStr, COLORREF fgColor, COLORREF bgColor);
// Operations
public:

	CFont m_titleFont;
	CFont m_subTitleFont;
	CRITICAL_SECTION m_mutexOfDraw;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMulitLineListBox)
	public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMultiLineListBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMulitLineListBox)
	afx_msg void OnDestroy();
	CDC m_dc;
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULITLINELISTBOX_H__D705CB99_9FD0_424E_BD71_027547449AE5__INCLUDED_)
