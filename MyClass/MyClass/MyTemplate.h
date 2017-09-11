#ifndef _MY_TEMPLATE_
#define _MY_TEMPLATE_
template<typename T>
inline T const& MAX(T const& a, T const& b)
{
	return a < b ? b:a;
}
#endif