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
		24,                        // nHeight
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
		16,                        // nHeight
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

	pInfo->titleStr.Format(_T("%s"), titleStr);
	pInfo->subTitleStr.Format(_T("%s"), subTitleStr);
	pInfo->fgColor = fgColor; 
	pInfo->bgColor = bgColor;

	SetItemDataPtr(AddString(pInfo->titleStr), pInfo);
}

void CMultiLineListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your code to determine the size of specified item
	ASSERT(lpMeasureItemStruct->CtlType == ODT_LISTBOX);
	
	CString strText(_T(""));
	GetText(lpMeasureItemStruct->itemID, strText);
	ASSERT(TRUE != strText.IsEmpty());

	CRect rect;
	GetItemRect(lpMeasureItemStruct->itemID, &rect);
	
	CDC* pDC = GetDC(); 
	//lpMeasureItemStruct->itemHeight = pDC->DrawText(strText, -1, rect, /*DT_WORDBREAK | */DT_CALCRECT);
	lpMeasureItemStruct->itemHeight = 48;// fix the line height
	ReleaseDC(pDC);
}

void CMultiLineListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);

	LISTBOX_COLOR* pListBox = (LISTBOX_COLOR*)GetItemDataPtr(lpDrawItemStruct->itemID);
	ASSERT(NULL != pListBox);

	CDC dc;
	
	dc.Attach(lpDrawItemStruct->hDC);
	
	// Save these value to restore them when done drawing.
	COLORREF crOldTextColor = dc.GetTextColor();
	COLORREF crOldBkColor = dc.GetBkColor();
	
	// If this item is selected, set the background color 
	// and the text color to appropriate values. Also, erase
	// rect by filling it with the background color.
	if ((lpDrawItemStruct->itemAction | ODA_SELECT) &&
		(lpDrawItemStruct->itemState & ODS_SELECTED))
	{
		dc.SetTextColor(pListBox->bgColor);
		dc.SetBkColor(pListBox->fgColor);
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, pListBox->fgColor);
	}
	else
	{
		dc.SetTextColor(pListBox->fgColor);
		dc.SetBkColor(pListBox->bgColor);
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, pListBox->bgColor);
	}
	
	lpDrawItemStruct->rcItem.left += 5;
	// Draw the text.
	//////////////////////////////////////


	dc.SelectObject(&m_titleFont);
	//pDC->DrawText(_T("test"), rect, /*DT_WORDBREAK | */DT_CALCRECT| DT_SINGLELINE);
	dc.DrawText(pListBox->titleStr, pListBox->titleStr.GetLength(), &lpDrawItemStruct->rcItem, DT_WORDBREAK| DT_SINGLELINE);

	dc.SelectObject(&m_subTitleFont);
	lpDrawItemStruct->rcItem.top += 24;
	dc.DrawText(pListBox->subTitleStr, pListBox->subTitleStr.GetLength(), &lpDrawItemStruct->rcItem, DT_WORDBREAK| DT_SINGLELINE);

	//////////////////////////////////////
	
	// Reset the background color and the text color back to their
	// original values.
	dc.SetTextColor(crOldTextColor);
	dc.SetBkColor(crOldBkColor);
	
	dc.Detach();	
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
