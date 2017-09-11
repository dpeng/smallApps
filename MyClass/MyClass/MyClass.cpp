// MyClass.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
using namespace std;
#include "MyProperty.h"
#include "MyTemplate.h"
#include "MyStack.h"
void TestStack();
int _tmain(int argc, _TCHAR* argv[])
{
	//CMyProperty m_test;
	//cout<<"设置前："<<m_test.__getpara()<<endl;
	//cout<<"请输入一个设置的参数："<<endl;
	//int tmpPara = 0;
	//cin>>tmpPara;
	//m_test.__putpara(tmpPara);
	//cout<<"设置后："<<m_test.__getpara()<<endl;


	//double adb = MAX(4.0, 4.00999);
	TestStack();
	system("pause");
	return 0;
}

void TestStack()
{
	SqStack TmStack;
	CMyStack Stack;
	Stack.InitStack(TmStack);
	int Nr_10 = 0;
	printf("请输入一个十进制整数：");
	scanf("%d", &Nr_10);

	//十进制转成八进制
	int tmp = Nr_10;
	while(tmp)
	{
		Stack.Push(TmStack, tmp%8);
		tmp = tmp/8;
	}
	int Des = 0;
	printf("\n八进制数为：");
	while(!Stack.StackEmpty(TmStack))
	{
		Stack.Pop(TmStack, Des);
		printf("%d", Des);
	}

	//转换成二进制
	tmp = Nr_10;
	while(tmp)
	{
		Stack.Push(TmStack, tmp%2);
		tmp = tmp/2;
	}
	Des = 0;
	printf("\n二进制数为：");
	while(!Stack.StackEmpty(TmStack))
	{
		Stack.Pop(TmStack, Des);
		printf("%d", Des);
	}


	//转换成16进制
	tmp = Nr_10;
	while(tmp)
	{
		Stack.Push(TmStack, tmp%16);
		tmp = tmp/16;
	}
	Des = 0;
	printf("\n16进制数为：");
	while(!Stack.StackEmpty(TmStack))
	{
		Stack.Pop(TmStack, Des);
		printf("%x", Des);
	}
}