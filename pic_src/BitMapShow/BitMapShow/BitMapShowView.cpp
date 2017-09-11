// BitMapShowView.cpp : implementation of the CBitMapShowView class
//

#include "stdafx.h"
#include "BitMapShow.h"

#include "BitMapShowDoc.h"
#include "BitMapShowView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBitMapShowView

IMPLEMENT_DYNCREATE(CBitMapShowView, CView)

BEGIN_MESSAGE_MAP(CBitMapShowView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_FILE_OPEN, &CBitMapShowView::OnFileOpen)
END_MESSAGE_MAP()

// CBitMapShowView construction/destruction

CBitMapShowView::CBitMapShowView()
{
	// TODO: add construction code here

}

CBitMapShowView::~CBitMapShowView()
{
}

BOOL CBitMapShowView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CBitMapShowView drawing

void CBitMapShowView::OnDraw(CDC* pDC)
{
	CBitMapShowDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	CFileFind bExit;
	if (!bExit.FindFile(m_sFileName))
	{
		//MessageBox((LPCTSTR)_T("文件不存在"), (LPCTSTR)_T("fuck"), MB_ICONQUESTION);
		return;
	}
	IStream *pStm; 
	CFileStatus fstatus; 
	CFile file; 
	LONG cb; 
	if (file.Open(m_sFileName,CFile::modeRead)&&file.GetStatus(m_sFileName,fstatus)&& ((cb = fstatus.m_size) != -1)) 
	{ 
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cb); 
		LPVOID pvData = NULL; 
		if (hGlobal != NULL) 
			if ((pvData = GlobalLock(hGlobal)) != NULL) 
			{ 
				file.Read(pvData, cb); 
				GlobalUnlock(hGlobal); 
				CreateStreamOnHGlobal(hGlobal, TRUE, &pStm); 
			} 
	} 

	IPicture *pPic; 
	if(SUCCEEDED(OleLoadPicture(pStm,fstatus.m_size,TRUE,IID_IPicture,(LPVOID*)&pPic))) 
	{ 
		OLE_XSIZE_HIMETRIC hmWidth; 
		OLE_YSIZE_HIMETRIC hmHeight; 
		pPic->get_Width(&hmWidth); 
		pPic->get_Height(&hmHeight); 
		double fX,fY; 
		fX = (double)pDC->GetDeviceCaps(HORZRES)*(double)hmWidth/((double)pDC->GetDeviceCaps(HORZSIZE)*100.0); 
		fY = (double)pDC->GetDeviceCaps(VERTRES)*(double)hmHeight/((double)pDC->GetDeviceCaps(VERTSIZE)*100.0); 
		if(FAILED(pPic->Render(*pDC,0,0,(DWORD)fX,(DWORD)fY,0,hmHeight,hmWidth,-hmHeight,NULL))) 
			MessageBox((LPCTSTR)_T("渲染图像失败"), (LPCTSTR)_T("fuck"), MB_ICONQUESTION);
		pPic->Release(); 
	} 
	else 
		MessageBox((LPCTSTR)_T("从流中加载图像失败"), (LPCTSTR)_T("fuck"), MB_ICONQUESTION);

	// TODO: add draw code for native data here
}


// CBitMapShowView printing

BOOL CBitMapShowView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBitMapShowView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBitMapShowView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CBitMapShowView diagnostics

#ifdef _DEBUG
void CBitMapShowView::AssertValid() const
{
	CView::AssertValid();
}

void CBitMapShowView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBitMapShowDoc* CBitMapShowView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBitMapShowDoc)));
	return (CBitMapShowDoc*)m_pDocument;
}
#endif //_DEBUG


// CBitMapShowView message handlers

void CBitMapShowView::OnFileOpen()
{
	// TODO: Add your command handler code here
	CFileDialog FileChooser(
		TRUE, 
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		(LPCTSTR)_T("bmp Files (*.bmp)|*.bmp|All files(*.*)|*.*||"));

	if (FileChooser.DoModal()==IDOK)
	{
		m_sFileName = FileChooser.GetPathName();
		Invalidate(TRUE);
	}
}


#if 0
int m_nState = 3;
if (m_nState == 1)   //第一种方式，通过最基本的读取方式来完成
{
	CFile file;               //表示欲打开的bmp文件
	char *pColor;
	int i,j;
	file.Open(m_sFileName,CFile::modeRead,NULL);
	ULONGLONG ulSize = file.GetLength();
	char *pBuf = new char[ulSize];                         //定义一个同位图文件相同大小的缓冲区
	UINT uiTemp = file.Read(pBuf,ulSize);            //读取位图数据到缓冲区
	file.Close();

	LONG cx,cy;

	DWORD bfOffBits;
	WORD biBitCount;
	bfOffBits = *(DWORD*)(pBuf+10);                //移动10位处，取出DWORD   bfOffBits;

	cx = *(LONG*)(pBuf+18);                              //分别取出x、y方向象素值，移动18和22(实际 
	cy = *(LONG*)(pBuf+22);                              //上距离bfOffBits 40）
	biBitCount = *(WORD*)(pBuf+28);                //继续取biBitCount
	BYTE byBlue,byGreen,byRed;
	if (biBitCount == 24)//如果是真彩色24位图片
	{
		char *pTemp;

		pTemp = pBuf+54;//54位后处，是每个象素的具体颜色值
		for(j=cy-1;j>=0;j--)
		{
			for(i=0;i<cx;i++)
			{
				byBlue = *pTemp;   //因为是真彩色24位，所以刚好8位是一种颜色,++就可以取下一颜色
				//值了
				pTemp++;
				byGreen = *pTemp;
				pTemp++;
				byRed = *pTemp;
				pTemp++;
				SetPixel(pDC->m_hDC,i,j,RGB(byRed,byGreen,byBlue));  //在屏幕上相应位置画上颜色
			}
			if ((cx*3)%4 != 0)
			{
				pTemp += (4-(cx*3)%4);
			}
		}
	}
	else if(biBitCount == 8) //256色图片
	{
		unsigned char *pTemp = (unsigned char*)(void*)(pBuf+bfOffBits);
		pColor = pBuf+54;
		int nIndex;
		for(j=cy-1;j>=0;j--)
		{
			for(i=0;i<cx;i++)
			{
				nIndex = (UINT)*pTemp;//注意现在是256色图片，颜色值取法有所不同
				byBlue = *(pColor+nIndex*4);
				byGreen = *(pColor+nIndex*4+1);
				byRed = *(pColor+nIndex*4+2);
				SetPixel(pDC->m_hDC,i,j,RGB(byRed,byGreen,byBlue));    
				pTemp++;
			}
			pTemp += (4-cx%4);
		}
	}
	else;//剩下的16色图片和单色图片，读者自己可以自行试验
	//AfxMessageBox("Init Decvice failed!");;//单色与16色图片打开功能暂未提供!
	delete []pBuf;
	pBuf = NULL;
}
if (m_nState == 2)//第二种方法使用SetDIBitsToDevice函数来完成
{
	CFile file;
	file.Open(m_sFileName,CFile::modeRead,NULL);
	ULONGLONG ulSize = file.GetLength();
	char *pBuf = new char[ulSize];
	file.Read(pBuf,ulSize);
	file.Close();
	LONG cx,cy;
	DWORD bfOffBits;
	bfOffBits = *(DWORD*)(pBuf+10);
	cx = *(LONG*)(pBuf+18);
	cy = *(LONG*)(pBuf+22);
	void *pBmpInfo,*lpvBufBmp;
	lpvBufBmp = pBuf+bfOffBits;
	pBmpInfo = pBuf+14;
	BITMAPINFO bmpInfo;
	memcpy(&bmpInfo.bmiHeader,pBmpInfo,40);
	SetDIBitsToDevice(pDC->m_hDC,0,0,cx,cy,0,0,0,cy,lpvBufBmp,
		&bmpInfo,DIB_PAL_COLORS);  
	delete [] pBuf;
	pBuf = NULL;
}
if (m_nState == 3)//第三种方法使用BitBlt函数来完成，这种方法最为简便实用。
{
	//CDC *pDC = GetDC();
	CDC dcMemory;
	CFile file;
	file.Open(m_sFileName,CFile::modeRead,NULL);
	ULONGLONG ulSize = file.GetLength();
	char *pBuf = new char[ulSize];
	file.Read(pBuf,ulSize);
	file.Close();
	LONG cx,cy;
	DWORD bfOffBits;
	bfOffBits = *(DWORD*)(pBuf+10);
	cx = *(LONG*)(pBuf+18);
	cy = *(LONG*)(pBuf+22);
	dcMemory.CreateCompatibleDC(pDC);
	HBITMAP hbitmap = (HBITMAP)::LoadImage(
		0,
		m_sFileName,
		IMAGE_BITMAP,  
		cx,
		cy, 
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	CBitmap bmp;
	bmp.Attach(hbitmap);

	CBitmap *pOldBitmap = dcMemory.SelectObject(&bmp);
	pDC->BitBlt(10,10,cx,cy,&dcMemory,0,0,SRCCOPY);
}

// 本文来自CSDN博客，转载请标明出处：http://blog.csdn.net/welcome_ck/archive/2004/12/24/228185.aspx
#endif