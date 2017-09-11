// beexb_gifView.cpp : implementation of the CBeexb_gifView class
//

#include "stdafx.h"
#include "beexb_gif.h"

#include "beexb_gifDoc.h"
#include "beexb_gifView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBeexb_gifView

IMPLEMENT_DYNCREATE(CBeexb_gifView, CView)

BEGIN_MESSAGE_MAP(CBeexb_gifView, CView)
	//{{AFX_MSG_MAP(CBeexb_gifView)
	ON_COMMAND(ID_SHOW_GIF, OnShowGif)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBeexb_gifView construction/destruction

CBeexb_gifView::CBeexb_gifView()
{
	// TODO: add construction code here

}

CBeexb_gifView::~CBeexb_gifView()
{
}

BOOL CBeexb_gifView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBeexb_gifView drawing

void CBeexb_gifView::OnDraw(CDC* pDC)
{
	CBeexb_gifDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CBeexb_gifView printing

BOOL CBeexb_gifView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBeexb_gifView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBeexb_gifView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CBeexb_gifView diagnostics

#ifdef _DEBUG
void CBeexb_gifView::AssertValid() const
{
	CView::AssertValid();
}

void CBeexb_gifView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBeexb_gifDoc* CBeexb_gifView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBeexb_gifDoc)));
	return (CBeexb_gifDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBeexb_gifView message handlers
typedef struct gifImage{
	WORD lWidth;
	WORD lHeight;
	WORD width;
	WORD height;
	struct flag{
		BYTE d:3;
		BYTE c:1;
		BYTE b:3;
        BYTE a:1;
	}Flag;
}GifImage,*PGifImage;
#include <math.h>

static UINT GifThread(LPVOID gifDC)
{
	CDC* pdc = (CDC*)gifDC;
	HINSTANCE handle = ::AfxGetResourceHandle();
	HRSRC hrsrc = ::FindResource(handle,MAKEINTRESOURCE(IDR_IMAGE2),"IMAGE");
    DWORD word = ::SizeofResource(handle,hrsrc);
    BYTE* lpBy = (BYTE*)LoadResource(handle,hrsrc);
	BYTE* pByte[20];
	DWORD nu[20];
	int num = 0;
	DWORD firstLocation = 0;
	for(DWORD j=0;j<word;j++)
	{
		if(lpBy[j]==0x2c)
		{
			if(lpBy[j-1]==0x00)
			{
				
				if(num==0)
				{
                    firstLocation = j;
				}
                PGifImage nowImage = (PGifImage)&lpBy[j+1];
				if(nowImage->Flag.a==0)
				{
					DWORD number = 1+sizeof(GifImage);
					while(lpBy[j+number]!=0)
					{
					    number = number+(DWORD)lpBy[j+number]+1;
					}
					number++;
					pByte[num] = new BYTE[number];
					for(DWORD n=0;n<number;n++)
					{
						*(BYTE*)(pByte[num]+n) = lpBy[j+n];
					}
					nu[num] = number;
					j = j+number-1;
					num++;
				}
				else
				{
					int number = 1+sizeof(GifImage)+1+3*(int)floor(pow(2,nowImage->Flag.d));
					while(lpBy[j+number]!=0)
					{
					    number = number+lpBy[j+number];
					}
					pByte[num] = new BYTE[number];
					for(int n=0;n<number;n++)
					{
						*(BYTE*)(pByte[num]+n) = lpBy[j+n];
					}
					nu[num] = number;
					j = j+number-1;
					num++;
				}
			}
		}
	}
	int i = 1;
	while(i)
	{
		for(int m=0;m<num;m++)
		{
			CBrush brush(RGB(255,255,255));
			pdc->FillRect(CRect(0,0,500,500),&brush);
			DWORD DDD;
			VirtualProtect(lpBy,word,PAGE_READWRITE,&DDD);
            for(DWORD n=0;n<nu[m];n++)
			{
				lpBy[firstLocation+n] = *(BYTE*)(pByte[m]+n);
			}
			VirtualProtect(lpBy,word,DDD,NULL);



	        CMemFile file(lpBy,word);
            CArchive ar(&file,CArchive::load|CArchive::bNoFlushOnDelete);
            CArchiveStream arcstream(&ar);
            CComQIPtr<IPicture> m_picture;
			HRESULT hr = OleLoadPicture((LPSTREAM)&arcstream,0,false,IID_IPicture,(void**)&m_picture);
			long a,b;
			m_picture->get_Width(&a);
			m_picture->get_Height(&b);
			CSize sz(a,b);
			pdc->HIMETRICtoDP(&sz);
			CRect rect;
			m_picture->Render(*pdc,0,0,sz.cx,sz.cy,0,b,a,-b,&rect);
			Sleep(100);
		}
	}
	return 1;
}

void CBeexb_gifView::OnShowGif() 
{
	static BOOL bIsRunning = FALSE;
	if(!bIsRunning)
	{
		CDC* pdc = GetDC();
		m_gifDC = new CDC;
		m_gifDC->Attach(pdc->GetSafeHdc());
		AfxBeginThread(GifThread,(LPVOID)m_gifDC);
		bIsRunning = TRUE;
	}
}
