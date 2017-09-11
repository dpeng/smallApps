#ifndef _CHINESE_ERA_H_
#define _CHINESE_ERA_H_
/*

函数名称：int GetChineseEra(int year,int flag)

函数功能: 返回某年对应的天干和地支。

flag = 0 返回天干 flag = 1 返回 地支。

函数参数：year 要查询的年；如：1984年，

天干：GetChineseEra(1984,0) 

地支：GetChineseEra(1984,1)

*/

int GetLunarYear(int year,int month,int day)

{

	int Spring_NY,Sun_NY;


	//Spring_NY 记录春节离当年元旦的天数。

	//Sun_NY 记录阳历日离当年元旦的天数。

	if( ((LunarCalendarTable[year-1901] & 0x0060) >> 5) == 1)



		Spring_NY = (LunarCalendarTable[year-1901] & 0x001F) - 1;

	else

		Spring_NY = (LunarCalendarTable[year-1901] & 0x001F) - 1 + 31;



	Sun_NY = MonthAdd[month-1] + day - 1;



	if( (!(year % 4)) && (month > 2))



		Sun_NY++;



	if(Sun_NY >= Spring_NY)

		return year;

	else

		return year - 1;

}

int GetChineseEra(int year,int flag)

{

	if(flag == 0)

		return ((year - 4) % 60) % 10;

	else

		return ((year - 4) % 60) % 12;

}


#endif