#include "StdAfx.h"
#include "Configuration.h"
#include "tinystr.h"
#include "tinyxml.h"

#define   TiXmlElementType(x)   tmpType##x 

CConfiguration::CConfiguration(void)
{
}

CConfiguration::~CConfiguration(void)
{
	for (int i = 0; i < MAX_SUPPORT_FAST_START_MASTER_KEY; i++)
	{
		for (int j = 0; j < MAX_SUPPORT_FAST_START_SLAVE_KEY; j++)
		{
			delete m_shellFileName[i][j];
			delete m_shellLPParameters[i][j];
			m_shellFileName[i][j] = NULL;
			m_shellLPParameters[i][j] = NULL;
		}
	}
}

void CConfiguration::initConfig()
{
	m_bAutoStart = FALSE;
	m_bDisplayMainWnd = FALSE;
	m_bNeedSaveAutoStartInformation = FALSE;
	m_strXmlFileName.Format("%sconfig.xml", GetCurPath());
	for (int i = 0; i < MAX_SUPPORT_FAST_START_MASTER_KEY; i++)
	{
		for (int j = 0; j < MAX_SUPPORT_FAST_START_SLAVE_KEY; j++)
		{
			m_shellFileName[i][j] = new char[MAX_PATH];
			m_shellLPParameters[i][j] = new char[MAX_PATH];
			memset(m_shellFileName[i][j], 0, MAX_PATH);
			memset(m_shellLPParameters[i][j], 0, MAX_PATH);
		}
	}
	if (!ReadConfig(m_strXmlFileName.GetBuffer(0)))
	{
		m_bDisplayMainWnd = TRUE;
		for (int i = 0; i < MAX_SUPPORT_FAST_START_MASTER_KEY; i++)
			for (int j = 0; j < MAX_SUPPORT_FAST_START_SLAVE_KEY; j++)
			{
				strcpy(m_shellFileName[i][j], _T("unset"));
				strcpy(m_shellLPParameters[i][j], _T("unset"));
			}
		strcpy(m_shellFileName[0][0], "notepad.exe");
		strcpy(m_shellFileName[0][1], "calc.exe");
		strcpy(m_shellFileName[0][2], GetDefaultBrowser());
		SaveConfig(m_strXmlFileName.GetBuffer(0));
	}
}

char* CConfiguration::GetCurPath()
{
	static TCHAR buf[MAX_PATH] = {0};
	HMODULE handle = GetModuleHandle("efficient.exe");
	DWORD length = ::GetModuleFileName(handle, buf, MAX_PATH);
	if (length <= 0)
		return _T("C:\\");
	--length;
	for (; length>0; --length)
		if (buf[length] == '/' || buf[length] == '\\')
		{
			buf[length + 1] = 0;
			break;
		}
		return buf;
}

char* CConfiguration::GetDefaultBrowser()
{
	HKEY hkRoot,hSubKey;
	TCHAR ValueName[MAX_PATH];
	memset(ValueName, 0, sizeof(ValueName));
	char DataValue[MAX_PATH];
	memset(DataValue, 0, sizeof(DataValue));
	unsigned long cbValueName=MAX_PATH;
	unsigned long cbDataValue=MAX_PATH;
	DWORD dwType;
	static char tmpFilename[MAX_PATH];
	memset(tmpFilename, 0, sizeof(tmpFilename));

	if(RegOpenKey(HKEY_CLASSES_ROOT,NULL,&hkRoot)==ERROR_SUCCESS)
	{
		if(RegOpenKeyEx(hkRoot, _T("htmlfile\\shell\\open\\command"), 0, KEY_ALL_ACCESS, &hSubKey)==ERROR_SUCCESS)
		{ 
			RegEnumValue(hSubKey, 0, ValueName, &cbValueName, NULL, &dwType, (LPBYTE)DataValue, &cbDataValue);
			char* tmpStr = "";
			_strlwr_s(DataValue);
			tmpStr = strstr(DataValue, ".exe");
			memcpy_s(tmpFilename, sizeof(tmpFilename), DataValue, strlen(DataValue)- strlen(tmpStr));
			strcat_s(tmpFilename, sizeof(tmpFilename), ".exe");
			TCHAR tmCh[MAX_PATH];
			memset(tmCh, 0, sizeof(tmCh));
			memcpy_s(tmCh, sizeof(tmCh), strstr(tmpFilename, "\""), sizeof(tmCh));
			CString tmStr = tmCh;
			tmStr.Remove('\"');
			memcpy_s(tmpFilename, sizeof(tmpFilename), tmStr.GetBuffer(0), strlen((char*)tmStr.GetBuffer(0)) + 1);
		}
		else
		{
			strcpy_s(tmpFilename, sizeof(tmpFilename), "iexplore.exe");
		}
	}
	else
	{
		strcpy_s(tmpFilename, sizeof(tmpFilename), "iexplore.exe");
	}
	RegCloseKey(hSubKey);
	RegCloseKey(hkRoot);
	return tmpFilename;
}

BOOL CConfiguration::ReadConfig(const char* filename)
{
	TiXmlDocument	_cfgDoc;
	if (!_cfgDoc.LoadFile(filename))
	{
		return false;
	}

	TiXmlElement* elementNode = NULL;
	TiXmlElement* node = NULL;
	TiXmlElement* root = _cfgDoc.FirstChildElement("Parameter");

	if (NULL == root)
		return false;
	else
	{
		char * masterElement = new char[32];
		char * slaveElement = new char[32];
		for (int i = 0; i < MAX_SUPPORT_FAST_START_MASTER_KEY; i++)
		{
			sprintf(masterElement, "MasterKey%03d", i);
			TiXmlElement* masterNode = root->FirstChildElement(masterElement);

			for (int j = 0; j < MAX_SUPPORT_FAST_START_SLAVE_KEY; j++)
			{
				memset(slaveElement, 0, sizeof(slaveElement));
				sprintf(slaveElement, "element%03d", j);
				elementNode = masterNode->FirstChildElement(slaveElement);
				node = elementNode->FirstChildElement("shellFileName");
				if (NULL != node && NULL != node->FirstChild())
					strcpy(m_shellFileName[i][j], (char*)(node->FirstChild()->Value()));
				node = elementNode->FirstChildElement("shellLPParameters");
				if (NULL != node && NULL != node->FirstChild())
					strcpy(m_shellLPParameters[i][j], (char*)(node->FirstChild()->Value()));
			}
		}
		SAFE_DELETE(masterElement);
		SAFE_DELETE(slaveElement);
		node = root->FirstChildElement("AutoReStartEn");
		if (NULL != node && NULL != node->FirstChild())
			m_bAutoStart = atoi(node->FirstChild()->Value());
	}
	return true;
}

BOOL CConfiguration::SaveConfig(const char* filename)
{
	TiXmlDocument docs;
	TiXmlNode* node = 0;
	char Vstr[256] = {0};
	if (!docs.Error())
	{
		TiXmlElement itemMessage("xml");
		itemMessage.SetAttribute("version","1.0");
		itemMessage.SetAttribute("encoding","UTF-8");

		TiXmlElement masterItemParameters("Parameter");
		for (int i = 0; i < MAX_SUPPORT_FAST_START_MASTER_KEY; i++)
		{
			CString tmpStr = _T("");
			tmpStr.Format("MasterKey%03d", i);
			TiXmlElement slaveItemParameters(tmpStr.GetBuffer(0));
			char *TiXmlElementName[MAX_SUPPORT_FAST_START_SLAVE_KEY];
			for (int j = 0; j < MAX_SUPPORT_FAST_START_SLAVE_KEY; j++)
			{
				TiXmlElementName[j] = new char[MAX_PATH];
				memset(TiXmlElementName[j], 0, sizeof(TiXmlElementName[j]));
				sprintf(TiXmlElementName[j], "element%03d", j);
			}

			for (int j = 0; j < MAX_SUPPORT_FAST_START_SLAVE_KEY; j++)
			{
				TiXmlElement TiXmlElementType(j)(TiXmlElementName[j]);
				//TiXmlElementType(j).InsertEndChild(tmpText);
				//slaveItemParameters.InsertEndChild(TiXmlElementType(j));
				
				TiXmlText shellFileName(m_shellFileName[i][j]);
				TiXmlElement shellFileNameElementName(_T("shellFileName"));
				shellFileNameElementName.InsertEndChild(shellFileName);

				TiXmlText shellLPParameters(m_shellLPParameters[i][j]);
				TiXmlElement shellLPParametersElementName(_T("shellLPParameters"));
				shellLPParametersElementName.InsertEndChild(shellLPParameters);

				TiXmlElementType(j).InsertEndChild(shellFileNameElementName);
				TiXmlElementType(j).InsertEndChild(shellLPParametersElementName);
				slaveItemParameters.InsertEndChild(TiXmlElementType(j));
			}
			masterItemParameters.InsertEndChild(slaveItemParameters);
		}
		//auto start at windows startup
		char Vstr[256] = {0};
		sprintf(Vstr, "%d", m_bAutoStart);
		TiXmlText textK1(Vstr);
		TiXmlElement AutoReStartEn("AutoReStartEn");
		AutoReStartEn.InsertEndChild(textK1);
		masterItemParameters.InsertEndChild(AutoReStartEn);

		node = docs.InsertEndChild(itemMessage);
		node = docs.InsertEndChild(masterItemParameters);

		char xmlstr[64*1024];
		docs.DocToString(xmlstr);	
		docs.SaveFile((const char*)filename);
		return true;
	}
	else
	{
		return false;
	}
}