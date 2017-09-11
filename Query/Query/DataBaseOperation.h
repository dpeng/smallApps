#pragma once
typedef struct{
	char chengyu[64];
	char pinyin[64];
	char diangu[512];
	char chuchu[512];
	char lizi[512];
	char sPinyin[64];
}Idiom;//≥…”Ô

typedef struct{
	char Province[64];
	char DC[64];
	char YZBM[16];
	char DHQH[16];
}City;

typedef struct{
	int start;
	int end;
	char city[64];
	char city1[64];
	int grade;
	char thinks[64];
}Mobile;

typedef enum{
	IDIOM_INFO = 1,
	CITY_INFO,
	MOBILE_INFO,
};
typedef struct{
	Idiom IdiomInfo;
	City CityInfo;
	Mobile MobileInfo;
}ItemInfo;
class CDataBaseOperation
{
public:
	CDataBaseOperation(void);
	~CDataBaseOperation(void);

	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pIdiomset;
	_RecordsetPtr m_pCityset;
	_RecordsetPtr m_pMobileset;
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
