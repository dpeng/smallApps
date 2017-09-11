// horrible.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <vector>  
typedef int UINT4;  
using namespace std;  

class Hack
{  

};  
Hack& operator< (Hack &a , Hack &b)  

{
	std::cerr << "小于操作符\n";
	return a;  
}  

Hack& operator> (Hack &a, Hack &b)  
{  
	std::cerr <<  "大于操作符\n";  
	return a;  
}  

void primes(int cap)
{
	int i, j, composite;

	for(i = 2; i < cap; ++i) 
	{
		composite = 0;
		for(j = 2; j * j < i; ++j)
			composite += !(i % j);
		if(!composite)
			printf("%d  ", i);
	}
}
int _tmain(int argc, _TCHAR* argv[])
{
	//test1
	//Hack vector;
	//Hack UINT4;
	//Hack foo;
	//vector<UINT4>foo;

	//test2
	//string str1 = "hello world";
	//string str2 = str1;

	//printf ("Sharing the memory:\n");
	//printf ("\tstr1's address: %x\n", str1.c_str() );
	//printf ("\tstr2's address: %x\n", str2.c_str() );

	//str1[1]='q';
	//str2[1]='w';

	//printf ("After Copy-On-Write:\n");
	//printf ("\tstr1's address: %x\n", str1.c_str() );
	//printf ("\tstr2's address: %x\n", str2.c_str() );

	//test3
	primes(100);
	system("pause");
	return(0);
}

