// MulitLineListBox.cpp : implementation file
//

#include "../stdafx.h"
#include "MultiLineListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMulitLineListBox

CMultiLineListBox::CMultiLineListBox()
{
	VERIFY(m_titleFont.CreateFont(
		TITLE_STRING_FONT_HEIGHT,  // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));                 // lpszFacename
	VERIFY(m_subTitleFont.CreateFont(
		SUBTITLE_STRING_FONT_HEIGHT,// nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		TRUE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("Arial")));                 // lpszFacename
}

CMultiLineListBox::~CMultiLineListBox()
{
	m_titleFont.DeleteObject();
	m_subTitleFont.DeleteObject();
}


BEGIN_MESSAGE_MAP(CMultiLineListBox, CListBox)
	//{{AFX_MSG_MAP(CMultiLineListBox)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMulitLineListBox message handlers
void CMultiLineListBox::AppendString(LPCSTR titleStr, LPCSTR subTitleStr, COLORREF fgColor, COLORREF bgColor)
{
	LISTBOX_COLOR* pInfo = new LISTBOX_COLOR;
	CString tmpStr = "";

	pInfo->titleStr.Format(_T("%s"), titleStr);
	pInfo->subTitleStr.Format(_T("%s"), subTitleStr);
	pInfo->fgColor = fgColor; 
	pInfo->bgColor = bgColor;

	tmpStr.Format("%s\\%s", subTitleStr, titleStr);
	SetItemDataPtr(AddString(tmpStr), pInfo);
}

void CMultiLineListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your code to determine the size of specified item
	ASSERT(lpMeasureItemStruct->CtlType == ODT_LISTBOX);
	if (lpMeasureItemStruct->itemID > MAX_ITEM_SHOW_IN_LIST)
		return;
	CString strText(_T(""));
	GetText(lpMeasureItemStruct->itemID, strText);
	if (strstr(strText, MAGIC_STRING_FOR_LAST_DRAW))
	{
		lpMeasureItemStruct->itemHeight = 0;
	}
	else
	{
		lpMeasureItemStruct->itemHeight = SEARCHRESULTITEMHEIGHT;// fix the line height
	}
}

void CMultiLineListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	if (NULL == lpDrawItemStruct) return;
	ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);
	if (lpDrawItemStruct->itemID > MAX_ITEM_SHOW_IN_LIST)  return;
	LISTBOX_COLOR* pListBox = (LISTBOX_COLOR*)GetItemDataPtr(lpDrawItemStruct->itemID);
	if (pListBox == NULL)  return;
	if (strstr(pListBox->titleStr, MAGIC_STRING_FOR_LAST_DRAW)) return;
		
	m_dc.Attach(lpDrawItemStruct->hDC);
	
	// Save these value to restore them when done drawing.
	COLORREF crOldTextColor = m_dc.GetTextColor();
	COLORREF crOldBkColor = m_dc.GetBkColor();
	
	// If this item is selected, set the background color 
	// and the text color to appropriate values. Also, erase
	// rect by filling it with the background color.
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
		(lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		m_dc.SetTextColor(pListBox->bgColor);
		m_dc.SetBkColor(pListBox->fgColor);
		m_dc.FillSolidRect(&lpDrawItemStruct->rcItem, pListBox->fgColor);
	}
	else
	{
		m_dc.SetTextColor(pListBox->fgColor);
		m_dc.SetBkColor(pListBox->bgColor);
		m_dc.FillSolidRect(&lpDrawItemStruct->rcItem, pListBox->bgColor);
	}
	
	lpDrawItemStruct->rcItem.left += 5;
	// Draw the text.
	m_dc.SelectObject(&m_titleFont);
	m_dc.DrawText(pListBox->titleStr, pListBox->titleStr.GetLength(), &lpDrawItemStruct->rcItem, DT_WORDBREAK| DT_SINGLELINE);

	m_dc.SelectObject(&m_subTitleFont);
	lpDrawItemStruct->rcItem.top += TITLE_STRING_FONT_HEIGHT;
	m_dc.DrawText(pListBox->subTitleStr, pListBox->subTitleStr.GetLength(), &lpDrawItemStruct->rcItem, DT_WORDBREAK| DT_SINGLELINE);
	
	// Reset the background color and the text color back to their
	// original values.
	m_dc.SetTextColor(crOldTextColor);
	m_dc.SetBkColor(crOldBkColor);
	m_dc.Detach();
}


void CMultiLineListBox::OnDestroy() 
{
	CListBox::OnDestroy();
	
	// TODO: Add your message handler code here	
	int nCount = GetCount();
	for(int i=0; i<nCount; i++)
	{
		LISTBOX_COLOR* pList = (LISTBOX_COLOR*)GetItemDataPtr(i);
		delete pList;
		pList = NULL;
	}
}
