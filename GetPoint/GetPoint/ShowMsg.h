#pragma once

class CShowMsg
{
public:
	CShowMsg(void);
	~CShowMsg(void);
public:
	CBitmap m_StoreBmp;  //´æ´¢Î»Í¼ 
	CDC m_StoreDC;    //´æ´¢DC 
	int m_nCoordStrLen;  //×ø±ê×Ö·û´®³¤¶È 
	BOOL m_bStart;
	void ShowText(CDC* pDC, CPoint Pt, LPCSTR text);
	void CopyBitmap(CDC* pDC,CPoint copyPt);
	void DrawCoord(CDC* pDC,LPCSTR coordString,CPoint drawPt);
	void DoRubberCoord(CDC* pDC);
	int m_x;
	int m_y;
	CRect m_CurRc;
	bool m_bCanShow;
};
