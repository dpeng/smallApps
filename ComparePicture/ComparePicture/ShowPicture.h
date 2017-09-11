#pragma once
#include <vector>
using namespace std;
class CShowPicture
{
public:
	CShowPicture(void);
	~CShowPicture(void);
	void ShowPic(char* filename, HWND hWnd);
	bool ShowBmp(char* filename, HWND hWnd);
	bool ShowJpg(char* filename, HWND hWnd);
};
