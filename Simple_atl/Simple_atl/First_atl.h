// First_atl.h : Declaration of the CFirst_atl

#pragma once
#include "resource.h"       // main symbols

#include "Simple_atl.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Single-threaded COM objects are not properly supported on Windows CE platform, such as the Windows Mobile platforms that do not include full DCOM support. Define _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA to force ATL to support creating single-thread COM object's and allow use of it's single-threaded COM object implementations. The threading model in your rgs file was set to 'Free' as that is the only threading model supported in non DCOM Windows CE platforms."
#endif



// CFirst_atl

class ATL_NO_VTABLE CFirst_atl :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CFirst_atl, &CLSID_First_atl>,
	public IDispatchImpl<IFirst_atl, &IID_IFirst_atl, &LIBID_Simple_atlLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CFirst_atl()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_FIRST_ATL)

DECLARE_NOT_AGGREGATABLE(CFirst_atl)

BEGIN_COM_MAP(CFirst_atl)
	COM_INTERFACE_ENTRY(IFirst_atl)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

	STDMETHOD(AddNumbers)(LONG para1, LONG para2, LONG* Returnval);
	STDMETHOD(MultiNum)(LONG para1, LONG para2, LONG* ReturnVal);
};

OBJECT_ENTRY_AUTO(__uuidof(First_atl), CFirst_atl)
