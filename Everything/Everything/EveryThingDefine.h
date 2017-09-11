#ifndef _EVERY_THING_DEFINE_
#define _EVERY_THING_DEFINE_

#include <vector>
#include <queue>
using namespace std;

#ifdef MAX_PATH
#undef MAX_PATH
#define MAX_PATH 260*2
#endif

//#define FILE_MAP_VIEW
#define MAX_NEW_ITEM 32
typedef struct _FILE_INDEX_
{
	char FileName[MAX_PATH];
	char FilePath[MAX_PATH];
	char FileRoot[MAX_PATH];
	char FileTitle[MAX_PATH];
	char FileURL[MAX_PATH];
	ULONGLONG FileLen;
	SYSTEMTIME CreationTime;
	SYSTEMTIME lastAccessTime;
	SYSTEMTIME LastWriteTime;
}FileIndex;

typedef struct _NEW_ITEM_INFO_
{
	vector<CString> itemName;
	vector<CString> itemMsg;
	void reset()
	{
		itemMsg.clear();
	}
	void clear()
	{
		itemName.clear();
		itemMsg.clear();
	}
}ItemInfo;
#define MAX_THREAD_NUM 10

typedef struct _THREAD_PARA_
{
	LPVOID lPara;
	vector<CString> DriverName;
}ThreadPara;
#endif