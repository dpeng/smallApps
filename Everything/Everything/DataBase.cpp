#include "StdAfx.h"
#include "DataBase.h"

CDataBase::CDataBase(void)
{
	CoInitialize(NULL);
}

CDataBase::~CDataBase(void)
{
	CoUninitialize();
}

void CDataBase::OpenAdo(char *dbName)
{
	m_pConnection.CreateInstance(__uuidof(Connection));
	char filename[512];
	memset(filename, 0, sizeof(filename));
	sprintf_s(filename, "Provider=Microsoft.ACE.OLEDB.12.0;Data Source=%s", dbName);
	try 
	{ 
		m_pConnection->Open(filename,"","",adModeUnknown);
		//m_pConnection->Open("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=Demo.mdb","","",adModeUnknown);

	}
	catch(_com_error e)
	{
		AfxMessageBox(_T("数据库连接失败，确认数据库是否在当前路径下!"));
		return ;
	}

}

void CDataBase::OpenTable(char* tbName)
{
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	char filename[512];
	memset(filename, 0, sizeof(filename));
	sprintf_s(filename, "SELECT * FROM %s", tbName);
	try
	{
		m_pRecordset->Open(filename, // 查询表中所有字段
			m_pConnection.GetInterfacePtr(), // 获取库接库的IDispatch指针
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
}
void CDataBase::ReadAdo(char* itemName, char* SearchContent, ItemInfo &item, HWND hWnd)
{
	CString strName;
	try
	{
		if(!m_pRecordset->BOF)
			m_pRecordset->MoveFirst();
		else
		{
			AfxMessageBox(_T("表内数据为空"));
			return;
		}

		int i = 0;
		int j = 0;
		while(!m_pRecordset->adoEOF)
		{
			if(m_pRecordset->GetCollect(itemName).vt != VT_NULL)//"FileName"
			{
				strName = (LPCSTR)_bstr_t(m_pRecordset->GetCollect(itemName));
				if (strstr(strName.GetBuffer(0), SearchContent))
				{
					for(vector<CString>::iterator it = item.itemName.begin(); it != item.itemName.end(); it++)
					{
						item.itemMsg.push_back((LPCTSTR)_bstr_t(m_pRecordset->GetCollect(it->GetBuffer(0))));
					}
					i++;
					j = 0;
					if (i > 5000)
					{
						MessageBox(NULL, "匹配项超过5000...停止检索！", "警告", MB_OK);
						break;
					}
					SetWindowText(hWnd, (LPCSTR)_bstr_t(m_pRecordset->GetCollect("FilePath")));
				}
			}
			j++;
			if (j > 100)
			{
				j = 0;
				SetWindowText(hWnd, (LPCSTR)_bstr_t(m_pRecordset->GetCollect("FilePath")));
			}
			strName = "";

			m_pRecordset->MoveNext();
		}
		SetWindowText(hWnd, "");
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
}
void CDataBase::AddNew(ItemInfo item)
{
	try
	{
		m_pRecordset->AddNew();
		for (unsigned int i = 0; i < item.itemName.size(); i++)
			m_pRecordset->PutCollect(item.itemName[i].GetBuffer(0), _variant_t(item.itemMsg[i].GetBuffer(0)));
		m_pRecordset->Update();
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
}
void CDataBase::ModifyAdo(long long index, ItemInfo item)
{
	try
	{
		m_pRecordset->MoveFirst();
		m_pRecordset->Move((ADO_LONGPTR)index);
		for (unsigned int i = 0; i < item.itemName.size(); i++)
			m_pRecordset->PutCollect(item.itemName[i].GetBuffer(0), _variant_t(item.itemMsg[i].GetBuffer(0)));
		m_pRecordset->Update();
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
}

void CDataBase::DeleteAdo(long long index)
{
	try
	{
		m_pRecordset->MoveFirst();
		m_pRecordset->Move((ADO_LONGPTR)index);
		m_pRecordset->Delete(adAffectCurrent);
		m_pRecordset->Update();
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
}

void CDataBase::ClearAdo(char* tbName)
{
	m_pRecordset.CreateInstance(__uuidof(Recordset));
	char filename[512];
	memset(filename, 0, sizeof(filename));
	sprintf_s(filename, "delete * from %s", tbName);
	//sprintf_s(filename, "truncate table %s", tbName);
	try
	{
		 m_pRecordset->Open(filename,
			 m_pConnection.GetInterfacePtr(),
			 adOpenDynamic,
			 adLockOptimistic,
			 adCmdText);
	}
	catch(_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
}
void CDataBase::CloseTable()
{
	m_pRecordset->Close();
	m_pRecordset = NULL;
}
void CDataBase::CloseAdo()
{
	if(m_pConnection->State)
		m_pConnection->Close();
	m_pConnection= NULL; 
}

