#ifndef _STR_INT_H_
#define _STR_INT_H_
void inttostr(int iValue, char *pStr, int iradix)
{
	if(NULL == pStr)
        return;
	
	sprintf_s(pStr, 10, "%d", iValue);
	
}

int strtoint( const char * pStr )
{
	if ( NULL == pStr )
		return 0;
	
	int sum = 0;
	for ( const char * p = pStr; *p != 0; p ++ )
		sum = sum * 10 + *p - '0';
	return sum;
	
}
#endif