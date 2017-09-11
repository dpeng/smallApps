#pragma once

class CFileMap
{
public:
	CFileMap(void);
	~CFileMap(void);
public:
	void CreadFileMap(char *filename);
	bool FindInMap(char* strName);
	void AddFile(char* addBuf);
	void CloseMap();
	HANDLE m_hFile;
};
