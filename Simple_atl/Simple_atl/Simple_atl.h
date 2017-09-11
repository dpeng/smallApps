

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Wed Dec 23 11:05:10 2009
 */
/* Compiler settings for .\Simple_atl.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __Simple_atl_h__
#define __Simple_atl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IFirst_atl_FWD_DEFINED__
#define __IFirst_atl_FWD_DEFINED__
typedef interface IFirst_atl IFirst_atl;
#endif 	/* __IFirst_atl_FWD_DEFINED__ */


#ifndef __First_atl_FWD_DEFINED__
#define __First_atl_FWD_DEFINED__

#ifdef __cplusplus
typedef class First_atl First_atl;
#else
typedef struct First_atl First_atl;
#endif /* __cplusplus */

#endif 	/* __First_atl_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 

#ifndef __IFirst_atl_INTERFACE_DEFINED__
#define __IFirst_atl_INTERFACE_DEFINED__

/* interface IFirst_atl */
/* [unique][helpstring][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IFirst_atl;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F24AAE74-F97C-4000-BD4D-BAA3441C335F")
    IFirst_atl : public IDispatch
    {
    public:
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE AddNumbers( 
            /* [in] */ LONG para1,
            /* [in] */ LONG para2,
            /* [out] */ LONG *Returnval) = 0;
        
        virtual /* [helpstring][id] */ HRESULT STDMETHODCALLTYPE MultiNum( 
            /* [in] */ LONG para1,
            /* [in] */ LONG para2,
            /* [out] */ LONG *ReturnVal) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IFirst_atlVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IFirst_atl * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IFirst_atl * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IFirst_atl * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IFirst_atl * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IFirst_atl * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IFirst_atl * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IFirst_atl * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *AddNumbers )( 
            IFirst_atl * This,
            /* [in] */ LONG para1,
            /* [in] */ LONG para2,
            /* [out] */ LONG *Returnval);
        
        /* [helpstring][id] */ HRESULT ( STDMETHODCALLTYPE *MultiNum )( 
            IFirst_atl * This,
            /* [in] */ LONG para1,
            /* [in] */ LONG para2,
            /* [out] */ LONG *ReturnVal);
        
        END_INTERFACE
    } IFirst_atlVtbl;

    interface IFirst_atl
    {
        CONST_VTBL struct IFirst_atlVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IFirst_atl_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IFirst_atl_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IFirst_atl_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IFirst_atl_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IFirst_atl_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IFirst_atl_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IFirst_atl_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)


#define IFirst_atl_AddNumbers(This,para1,para2,Returnval)	\
    (This)->lpVtbl -> AddNumbers(This,para1,para2,Returnval)

#define IFirst_atl_MultiNum(This,para1,para2,ReturnVal)	\
    (This)->lpVtbl -> MultiNum(This,para1,para2,ReturnVal)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFirst_atl_AddNumbers_Proxy( 
    IFirst_atl * This,
    /* [in] */ LONG para1,
    /* [in] */ LONG para2,
    /* [out] */ LONG *Returnval);


void __RPC_STUB IFirst_atl_AddNumbers_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


/* [helpstring][id] */ HRESULT STDMETHODCALLTYPE IFirst_atl_MultiNum_Proxy( 
    IFirst_atl * This,
    /* [in] */ LONG para1,
    /* [in] */ LONG para2,
    /* [out] */ LONG *ReturnVal);


void __RPC_STUB IFirst_atl_MultiNum_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IFirst_atl_INTERFACE_DEFINED__ */



#ifndef __Simple_atlLib_LIBRARY_DEFINED__
#define __Simple_atlLib_LIBRARY_DEFINED__

/* library Simple_atlLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_Simple_atlLib;

EXTERN_C const CLSID CLSID_First_atl;

#ifdef __cplusplus

class DECLSPEC_UUID("3AAAB2F8-CB0F-445D-9057-5F6042DC4AAF")
First_atl;
#endif
#endif /* __Simple_atlLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


