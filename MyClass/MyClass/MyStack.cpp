#include "StdAfx.h"
#include "MyStack.h"
#include "malloc.h"
#include "stdlib.h"

#define STACK_INIT_SIZE 100
#define STACK_INCREMENT 10
#define _OVER_FLOW_ -1
CMyStack::CMyStack(void)
{
}

CMyStack::~CMyStack(void)
{
}
//初始化栈
bool CMyStack::InitStack(SqStack &s)
{
	s.Base = (SElemType*)malloc(STACK_INIT_SIZE*sizeof(SElemType));
	if (!s.Base)
		exit(_OVER_FLOW_);
	s.Top = s.Base;
	s.Size = STACK_INIT_SIZE;
	return true;
}

//取出栈顶元素
bool CMyStack::GetTop(SqStack s, SElemType e)
{
	if (s.Top == s.Base)
		return false;
	e = *(s.Top - 1);
	return true;
}

//将元素压入栈顶
bool CMyStack::Push(SqStack &s, SElemType e)
{
	if (s.Top - s.Base >= s.Size)
	{
		s.Base = (SElemType*)realloc(s.Base, (s.Size + STACK_INCREMENT) * sizeof(SElemType));
		if (!s.Base)
			exit(_OVER_FLOW_);
		s.Top = s.Base + s.Size;
		s.Size += STACK_INCREMENT;
	}
	*s.Top++ = e;
	return true;
}

//将栈顶元素取出，并删除栈顶元素
bool CMyStack::Pop(SqStack &s, SElemType &e)
{
	if (s.Top == s.Base)
		return false;
	e = * --s.Top;
	return true;
}


//判断栈是否为空
bool CMyStack::StackEmpty(SqStack s)
{
	if (s.Base == s.Top)
		return true;
	else
		return false;
}