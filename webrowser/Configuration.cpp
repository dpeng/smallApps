#include "StdAfx.h"
#include "Configuration.h"
#include "tinystr.h"
#include "tinyxml.h"
#include "direct.h"

#define   TiXmlElementType(x)   tmpType##x 

CConfiguration::CConfiguration(void)
{
}

CConfiguration::~CConfiguration(void)
{
	for (int i = 0; i < MAX_SUPPORT_FAVORITE_KEY; i++)
	{
		delete m_pageName[i];
		m_pageName[i] = NULL;
		delete m_urlName[i];
		m_urlName[i] = NULL;
	}
}

void CConfiguration::initConfig()
{
	m_bAutoStart = FALSE;
	m_bBeep = FALSE;
	m_bDisplayMainWnd = FALSE;
	m_bNeedSaveAutoStartInformation = FALSE;
	m_strXmlFileName = new char[MAX_PATH];
	memset(m_strXmlFileName, 0, MAX_PATH);
	_getcwd(m_strXmlFileName, MAX_PATH);
	strcat(m_strXmlFileName, "\\webAddr.xml");
	for (int i = 0; i < MAX_SUPPORT_FAVORITE_KEY; i++)
	{
		m_pageName[i] = new char[MAX_PATH];
		memset(m_pageName[i], 0, MAX_PATH);
		m_urlName[i] = new char[MAX_PATH];
		memset(m_urlName[i], 0, MAX_PATH);
	}
	if (!ReadConfig(m_strXmlFileName))
	{
		m_bDisplayMainWnd = TRUE;
		for (int i = 0; i < MAX_SUPPORT_FAVORITE_KEY; i++)
		{
			strcpy(m_pageName[i], (char*)("+"));
			strcpy(m_urlName[i], (char*)(""));
		}
		SaveConfig(m_strXmlFileName);
	}
}

BOOL CConfiguration::ReadConfig(const char* filename)
{
	TiXmlDocument	_cfgDoc;
	if (!_cfgDoc.LoadFile(filename))
	{
		return false;
	}

	TiXmlElement* node = NULL;
	TiXmlElement* root = _cfgDoc.FirstChildElement("Parameter");

	if (NULL == root)
		return false;
	else
	{
		char * pageNameElement = new char[32];
		char * urlNameElement = new char[32];
		for (int i = 0; i < MAX_SUPPORT_FAVORITE_KEY; i++)
		{
			sprintf(pageNameElement, "pageName%03d", i);
			sprintf(urlNameElement, "urlName%03d", i);

			TiXmlElement* pageNameNode = root->FirstChildElement(pageNameElement);
			if (NULL != pageNameNode && NULL != pageNameNode->FirstChild())
				strcpy(m_pageName[i], (char*)(pageNameNode->FirstChild()->Value()));


			TiXmlElement* urlNameNode = root->FirstChildElement(urlNameElement);
			if (NULL != urlNameNode && NULL != urlNameNode->FirstChild())
				strcpy(m_urlName[i], (char*)(urlNameNode->FirstChild()->Value()));

		}
		SAFE_DELETE(pageNameElement);
		SAFE_DELETE(urlNameElement);
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
		//itemMessage.SetAttribute("encoding","UTF-8");
		//itemMessage.SetAttribute("version","2.0");
		itemMessage.SetAttribute("encoding","gb2312");
		itemMessage.SetAttribute("standalone", "yes");
		TiXmlElement masterItemParameters("Parameter");
		for (int i = 0; i < MAX_SUPPORT_FAVORITE_KEY; i++)
		{
			CString pageName = _T("");
			pageName.Format(_T("pageName%03d"), i);
			TiXmlElement pageNameParameters((char*)(pageName.GetBuffer(0)));
			TiXmlText tmpText(m_pageName[i]);
			pageNameParameters.InsertEndChild(tmpText);

			CString urlName = _T("");
			urlName.Format(_T("urlName%03d"), i);
			TiXmlElement urlNameParameters2((char*)(urlName.GetBuffer(0)));
			TiXmlText tmpText2(m_urlName[i]);
			urlNameParameters2.InsertEndChild(tmpText2);

			masterItemParameters.InsertEndChild(pageNameParameters);
			masterItemParameters.InsertEndChild(urlNameParameters2);
		}

		node = docs.InsertEndChild(itemMessage);
		node = docs.InsertEndChild(masterItemParameters);

		char xmlstr[10*1024];
		docs.DocToString(xmlstr);	
		docs.SaveFile((const char*)filename);
		return true;
	}
	else
	{
		return false;
	}
}