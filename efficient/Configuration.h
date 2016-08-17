#pragma once
#define MAX_SUPPORT_FAST_START_MASTER_KEY 4      //Current support win, alt and shift. add one for special keyboard
#define MAX_SUPPORT_FAST_START_SLAVE_KEY 10 + 26 // ten number and 26 alphabet 

class CConfiguration
{
public:
	CConfiguration(void);
	~CConfiguration(void);
public:
	BOOL ReadConfig(const char* filename);
	BOOL SaveConfig(const char* filename);
	char *m_shellFileName[MAX_SUPPORT_FAST_START_MASTER_KEY][MAX_SUPPORT_FAST_START_SLAVE_KEY];
	char *m_shellLPParameters[MAX_SUPPORT_FAST_START_MASTER_KEY][MAX_SUPPORT_FAST_START_SLAVE_KEY];
	CString m_strXmlFileName;
	BOOL m_bAutoStart;
	BOOL m_bNeedSaveAutoStartInformation;
	void initConfig();
	BOOL m_bDisplayMainWnd;
	char* GetCurPath();
private:
	char * GetDefaultBrowser();
};
