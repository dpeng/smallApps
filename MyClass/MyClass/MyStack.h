#pragma once

#define SElemType int
typedef struct  
{
	SElemType *Base;
	SElemType *Top;
	SElemType Size;
}SqStack;
class CMyStack
{
public:
	CMyStack(void);
	~CMyStack(void);
	bool InitStack(SqStack &s);
	bool DestroyStack(SqStack &s);
	bool ClearStack(SqStack &s);
	bool StackEmpty(SqStack s);
	int StackLenght(SqStack s);
	bool GetTop(SqStack s, SElemType e);
	bool Push(SqStack &s, SElemType e);
	bool Pop(SqStack &s, SElemType &e);
};
