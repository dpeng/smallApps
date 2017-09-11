#include "StdAfx.h"
#include "FileMap.h"

CFileMap::CFileMap(void)
{
}

CFileMap::~CFileMap(void)
{
}


void CFileMap::CreadFileMap(char *filename)
{
	m_hFile = CreateFile(filename, 
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE, 
		NULL, 
		OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
}

bool CFileMap::FindInMap(char* strName)
{
	HANDLE hFileMap = CreateFileMapping(m_hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);
	DWORD dwGran = SysInfo.dwAllocationGranularity;
	DWORD dwFileSizeHigh;
	__int64 qwFileSize = GetFileSize(m_hFile, &dwFileSizeHigh);
	qwFileSize |= (((__int64)dwFileSizeHigh) << 32);
	DWORD dwBlockBytes = 1000 * dwGran;
	if (qwFileSize < 1000 * dwGran)
		dwBlockBytes = (DWORD)qwFileSize;
	__int64 qwFileOffset = 0;
	LPBYTE lpbMapAddress = NULL;
	while (qwFileSize > 0)
	{
		lpbMapAddress = (LPBYTE)MapViewOfFile(hFileMap,FILE_MAP_ALL_ACCESS, 
			(DWORD)(qwFileOffset >> 32), (DWORD)(qwFileOffset & 0xFFFFFFFF),dwBlockBytes);
		int tmpBlock = dwBlockBytes;
		while(tmpBlock > 5*1024)
		{
			char tmpstr[5*1024];
			memset(tmpstr, 0, sizeof(tmpstr));
			memcpy(tmpstr, lpbMapAddress, sizeof(tmpstr));
			lpbMapAddress += strlen(tmpstr) - MAX_PATH;
			tmpBlock -= strlen(tmpstr) - MAX_PATH;
			if (strstr(tmpstr, strName))
			{
				UnmapViewOfFile(lpbMapAddress);
				CloseHandle(hFileMap);
				hFileMap = NULL;
				return true;
			}
		}
		qwFileOffset += dwBlockBytes;
		qwFileSize -= dwBlockBytes;
	};
	UnmapViewOfFile(lpbMapAddress);
	CloseHandle(hFileMap);
	hFileMap = NULL;
	return false;
}

void CFileMap::AddFile(char* addBuf)
{
	DWORD dwFileSizeHigh;
	__int64 qwFileSize = GetFileSize(m_hFile, &dwFileSizeHigh);
	__int64 tmpFileSize = qwFileSize;
	HANDLE hFileMap = CreateFileMapping(m_hFile, NULL, PAGE_READWRITE, 0, (DWORD)qwFileSize + (DWORD)strlen(addBuf), NULL);
	SYSTEM_INFO SysInfo;
	GetSystemInfo(&SysInfo);
	DWORD dwGran = SysInfo.dwAllocationGranularity;
	qwFileSize |= (((__int64)dwFileSizeHigh) << 32);
	DWORD dwBlockBytes = 1000 * dwGran;
	if (qwFileSize < 1000 * dwGran)
		dwBlockBytes = (DWORD)qwFileSize;
	__int64 qwFileOffset = 0;
	LPBYTE lpbMapAddress;
	while (qwFileSize > 0)
	{
		if (qwFileSize < 1000 * dwGran)
			dwBlockBytes = (DWORD)qwFileSize + strlen(addBuf);

		lpbMapAddress = (LPBYTE)MapViewOfFile(hFileMap,FILE_MAP_WRITE, 
			(DWORD)(qwFileOffset >> 32), (DWORD)(qwFileOffset & 0xFFFFFFFF),dwBlockBytes);
		qwFileOffset += dwBlockBytes;
		qwFileSize -= dwBlockBytes;
	}
	memcpy(lpbMapAddress+tmpFileSize, addBuf, strlen(addBuf));
	tmpFileSize += strlen(addBuf);
	FlushViewOfFile(lpbMapAddress, (SIZE_T)tmpFileSize);
	UnmapViewOfFile(lpbMapAddress);
	CloseHandle(hFileMap);
	hFileMap = NULL;
}
void CFileMap::CloseMap()
{
	CloseHandle(m_hFile);
	m_hFile = NULL;
}