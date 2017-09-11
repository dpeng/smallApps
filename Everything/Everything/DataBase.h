#pragma once
#include "EveryThingDefine.h"
class CDataBase
{
public:
	CDataBase(void);
	~CDataBase(void);
	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordset;
	void OpenAdo(char *dbName);
	void OpenTable(char *tbName);
	void ReadAdo(char* itemName, char* SearchContent, ItemInfo &item, HWND hWnd);
	void AddNew(ItemInfo item);
	void ModifyAdo(long long index, ItemInfo item);
	void DeleteAdo(long long index);
	void ClearAdo(char* tbName);
	void CloseTable();
	void CloseAdo();
};
