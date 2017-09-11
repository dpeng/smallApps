#include "StdAfx.h"
#include "ShowPicture.h"

CShowPicture::CShowPicture(void)
{
}

CShowPicture::~CShowPicture(void)
{
}

void CShowPicture::ShowPic(char* filename, HWND hWnd)
{
	CString TmpStr = _T("");
	SIZE_T len = strlen(filename) + 1;
	TmpStr = &filename[len-4];
	bool bRet = false;
	if(TmpStr == "bmp" || TmpStr == "BMP")
		bRet = ShowBmp(filename, hWnd);
	else
		bRet = ShowJpg(filename, hWnd);
	if (!bRet)
		SetWindowText(hWnd, filename);
}

bool CShowPicture::ShowBmp(char* filename, HWND hWnd)
{
	CFile file;
	CFileFind bExit;
	if (!bExit.FindFile(filename))
	{
		//MessageBox(NULL, (LPCTSTR)_T("文件不存在"), (LPCTSTR)_T("fuck"), MB_ICONQUESTION);
		return false;
	}
	file.Open(filename,CFile::modeRead,NULL);
	ULONGLONG ulSize = file.GetLength();
	char *pBuf = new char[ulSize];
	file.Read(pBuf,ulSize);
	file.Close();
	double fX,fY; 
	DWORD bfOffBits;
	RECT rc = {0};
	GetWindowRect(hWnd, &rc);
	bfOffBits = *(DWORD*)(pBuf+10);
	fX = *(LONG*)(pBuf+18);
	fY = *(LONG*)(pBuf+22);
	delete pBuf;
	pBuf = NULL;
	char str[200];
	sprintf(str, "rc:%d-%d-%d-%d  xy:%d-%d\n",rc.left, rc.right, rc.top, rc.bottom, (int)fX,(int)fY);
	OutputDebugString(str);
	if (fX <= 0 || fY <= 0)
		return false;
	CDC *pDC = new CDC;
	pDC->Attach(GetDC(hWnd));
	if (fX > rc.right - rc.left || fY > rc.bottom - rc.top)
		if ((fX/(rc.right - rc.left)) >= (fY/(rc.bottom - rc.top)))
		{
			fY = (rc.right - rc.left)*fY/fX;
			fX = rc.right - rc.left;
		}
		else
		{
			fX = (rc.bottom - rc.top)*fX/fY;
			fY = rc.bottom - rc.top;
		}
	CDC dcMemory;
	if(!dcMemory.CreateCompatibleDC(pDC))
		return false;
	HBITMAP hbitmap = (HBITMAP)::LoadImage(
		0,
		filename,
		IMAGE_BITMAP,  
		fX,
		fY, 
		LR_CREATEDIBSECTION | LR_DEFAULTSIZE | LR_LOADFROMFILE);
	CBitmap bmp;
	bmp.Attach(hbitmap);

	CBitmap *pOldBitmap = dcMemory.SelectObject(&bmp);
	pDC->BitBlt(0,0,fX,fY,&dcMemory,0,0,SRCCOPY);

	bmp.DeleteObject();
	dcMemory.SelectObject(pOldBitmap);
	dcMemory.DeleteDC();
	return true;
}

bool CShowPicture::ShowJpg(char* filename, HWND hWnd)
{
	CFileFind bExit;
	if (!bExit.FindFile(filename))
	{
		//MessageBox(NULL, (LPCTSTR)_T("文件不存在"), (LPCTSTR)_T("fuck"), MB_ICONQUESTION);
		return false;
	}
	IStream *pStm; 
	CFileStatus fstatus; 
	CFile file; 
	LONG cb; 
	LPVOID pvData = NULL; 
	if (file.Open(filename,CFile::modeRead)&&file.GetStatus(filename,fstatus)&& ((cb = fstatus.m_size) != -1)) 
	{ 
		HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, cb); 
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
		CDC *pDC = new CDC;
		pDC->Attach(GetDC(hWnd));
		double fX,fY; 
		RECT rc = {0};
		GetWindowRect(hWnd, &rc);
		fX = (double)pDC->GetDeviceCaps(HORZRES)*(double)hmWidth/((double)pDC->GetDeviceCaps(HORZSIZE)*100.0);
		fY = (double)pDC->GetDeviceCaps(VERTRES)*(double)hmHeight/((double)pDC->GetDeviceCaps(VERTSIZE)*100.0);
		if (fX <= 0 || fY <= 0)
			return false;
		if (fX > rc.right - rc.left || fY > rc.bottom - rc.top)
			if ((fX/(rc.right - rc.left)) >= (fY/(rc.bottom - rc.top)))
			{
				fY = (rc.right - rc.left)*fY/fX;
				fX = rc.right - rc.left;
			}
			else
			{
				fX = (rc.bottom - rc.top)*fX/fY;
				fY = rc.bottom - rc.top;
			}
			if(FAILED(pPic->Render(*pDC,0,0,(DWORD)fX,(DWORD)fY,0,hmHeight,hmWidth,-hmHeight,NULL))) 
			{
				//MessageBox(NULL, (LPCTSTR)_T("渲染图像失败"), (LPCTSTR)_T("fuck"), MB_ICONQUESTION);
				return false;
			}
			pPic->Release();
			pPic = NULL;
			pStm->Release();
			pStm = NULL;
	} 
	else 
		//MessageBox(NULL, (LPCTSTR)_T("从流中加载图像失败"), (LPCTSTR)_T("fuck"), MB_ICONQUESTION);
	return true;
}