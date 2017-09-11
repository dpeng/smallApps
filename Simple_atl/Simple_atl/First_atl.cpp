// First_atl.cpp : Implementation of CFirst_atl

#include "stdafx.h"
#include "First_atl.h"


// CFirst_atl


STDMETHODIMP CFirst_atl::AddNumbers(LONG para1, LONG para2, LONG* Returnval)
{
	// TODO: Add your implementation code here
	*Returnval = para1 + para2;
	return S_OK;
}

STDMETHODIMP CFirst_atl::MultiNum(LONG para1, LONG para2, LONG* ReturnVal)
{
	// TODO: Add your implementation code here
	*ReturnVal = para1 * para2;
	return S_OK;
}
