#pragma once
#define MAX_SUPPORT_FAVORITE_KEY 100

class CConfiguration
{
public:
	CConfiguration(void);
	~CConfiguration(void);
public:
	BOOL ReadConfig(const char* filename);
	BOOL SaveConfig(const char* filename);
	char *m_pageName[MAX_SUPPORT_FAVORITE_KEY];
	char *m_urlName[MAX_SUPPORT_FAVORITE_KEY];
	char *m_strXmlFileName;
	BOOL m_bAutoStart;
	BOOL m_bNeedSaveAutoStartInformation;
	BOOL m_bBeep;
	void initConfig();
	BOOL m_bDisplayMainWnd;
};
