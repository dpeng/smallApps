#ifndef _MY_PROPERTY_
#define _MY_PROPERTY_
#define PROP(T,X) __declspec(property(get= __get##X,put= __set##X))T X;
#define GETPROP(T,X) __declspec(property(get= __get##X))T X; //只读属性
#define SETPROP(T,X) __declspec(property(put= __set##X))T X; //只写属必
#define GET(T,X) T __get##X(void)
#define SET(T,X) void __put##X(T value)

class CMyProperty
{
private:
	int m_Variable;
	PROP(int, para)
public:
	GET(int, para)
	{
		return m_Variable;
	}
	SET(int, para)
	{
		if((value > 0) && (value < 60))
		{
			m_Variable = value;
		}
		else
		{
			m_Variable =20;
		}
	}
};

#endif