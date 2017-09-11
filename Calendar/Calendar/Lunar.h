#pragma once

class CLunar
{
public:
	CLunar(void);
	~CLunar(void);


	void LunarCalendar(COleDateTime tmMeas);//计算农历日期。

public:
	//农历
	int m_lYear; //公元年4位数字
	int m_lMonth; //农历月数字
	int m_lDay; //农历日数字
	CString m_cYearName; //年号纪年表示
	CString m_cWordDay; //文字表示的日
	CString m_cWordMonth;//文字表示的月
	CString m_cMonthSB; //月大(30) 小(29)？
	BOOL m_isLeap; //是否为农历闰月?

	//八字
	CString m_cYear; //年柱, 2个中文
	CString m_cMonth; //月柱, 2个中文
	CString m_cDay; //日柱, 2个中文
	CString m_cAnimal; //动物属相 

	//节日
	CString m_lunarFestival; //农历节日
	CString m_solarFestival; //公历节日
	CString m_solarTerms; //节气

	BOOL m_bStrong; //强调的节日 放假日

private:	
	static int getYearDays(int y);//返回农历 y年的总天数	
	static int getLeapDays(int y);//返回农历 y年闰月的天数	
	static int getLeapMonth(int y);//返回农历 y年闰哪个月 1-12 , 没闰返回 0	
	static int getMonthDays(int y,int m);//返回农历 y年m月的总天数	
	static int getSTerm(int y,int n);//某年的第n个节气为几日(从0小寒起算)	
	static CString getWordDay(int d);//得到农历天的描述	
	static CString getYearName(int y);//得到年号纪年 描述	
	static CString getGZ(LONGLONG num);//得到干支纪年	
	static void parseFtv(CString strInfo,int& nFirst,int& nLast,CString& strDesc,BOOL& bStrong);//从节日字符里解析出数字，描述，*
	static int getWeekOfMonth(int y,int m,int d);//得到当日为本月的第几周	
	static LONGLONG getUTC(int nYear, int nMonth, int nDay, int nHour=0, int nMin=0, int nSec=0);//计算指定年代与 UTC时间相差的 100ns 数
	void Init();//

	//输入阳历日期，返回星期几(1=Sun, 2=Mon, ..., 7=Sa)
	//int getDayOfWeek(myDATETIME DayWk);


public:	
	static int getWeeksOfMonth(int y,int m);//得到当月有几周	
	static int getSunMonthDays(int y,int m);//得到公历月天数

	static CString getWordMonth(int m)//得到中文月描述
	{
		ASSERT(m>0 && m<=12);
		return m_nStr1[m];
	};

	static CString getEnWordMonth(int m)//得到英文月描述
	{
		ASSERT(m>0 && m<=12);
		return m_monthName[m-1];
	};

private:
	/************************************************************************/
	/*                      农[阴]历所使用的全局常量                        */
	/************************************************************************/
	// 日期资料
	static const int m_lunarInfo[]; //农历日期
	static const CString m_TIANGAN[]; //天干
	static const CString m_DIZHI[]; //地支
	static const CString m_Animals[]; //生肖
	static const CString m_nStr1[]; //数字的中文描述
	static const CString m_nStr2[]; //数字的中文描述
	//	static const CString m_cMonName[];//数字的中文描述(月)

	//农历节日
	static const CString m_lFtv[];

	/************************************************************************/
	/*                         公[阳]历所使用的全局常量                     */
	/************************************************************************/ 
	static const int m_solarMonth[]; //月的天数
	static const CString m_solarTerm[]; //节气的中文描述
	static const int m_sTermInfo[]; //节气的日期信息
	static const CString m_monthName[]; //月的英文描述

	//公历节日 *表示放假日
	static const CString m_sFtv[];
	//某月的第几个星期几。 5,6,7,8 表示到数第 1,2,3,4 个星期几
	static const CString m_wFtv[];

	//检测时间
	COleDateTime m_tmMeas; //检测日期
};
