// SysInfoDlg.h : header file
//

#pragma once

// CSysInfoDlg dialog
struct LICENSEINFO
{
	enum {
		ProductSize		= 256,
		VersionSize		= 16,
		ProviderSize	= 64,
		RegisterSize	= 64,
	};

	char Product[ProductSize]; // 加密用的mac，所有的mac地址
	char Mac[ProductSize/2];   //校验的时候只取其中一个mac进行校验
	char Version[VersionSize];

	char Provider[ProviderSize];
	char Register[RegisterSize];

	//	有效期起始时间
	unsigned short  vf_year;
	unsigned short	vf_month;
	unsigned short	vf_day;

	// 有效期结束时间
	unsigned short vt_year;
	unsigned short	vt_month;
	unsigned short	vt_day;

	unsigned short max_device;

	// 安装程序起始时间
	unsigned short vif_year;
	unsigned short vif_month;
	unsigned short vif_day;

	// 安装程序结束时间
	unsigned short vit_year;
	unsigned short vit_month;
	unsigned short vit_day;	

	// 程序编译时间
	unsigned short build_year;
	unsigned short build_month;
	unsigned short build_day;

};

struct REGINFO
{
	LICENSEINFO LicInfo;
	char CpuId[256];
	char OsInfo[256];
	SYSTEMTIME SysTime;
	char Mac[256];
	char MemInfo[256];
	char PhysicalSerialNum[256];
	char BoisId[256];
	char UserName[256];
	char PcName[256];
//	SoftInfoMap SoftList;
};
class CSysInfoDlg : public CDialog
{
// Construction
public:
	CSysInfoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_SYSINFO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	void GetDevInfo();
	void DisPlayInfo();
	REGINFO _RegInfo;
	SYSTEMTIME m_SoftStartTime;
	int m_nSoftStartTime;
	//vector<char*> m_SoftList;
};
