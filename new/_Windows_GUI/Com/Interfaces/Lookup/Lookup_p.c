/* this ALWAYS GENERATED file contains the proxy stub code */


/* File created by MIDL compiler version 2.00.0102 */
/* at Wed Oct 30 17:25:28 1996
 */
//@@MIDL_FILE_HEADING(  )

#include "rpcproxy.h"
#include "Lookup.h"


extern const MIDL_FORMAT_STRING __MIDLFormatString;

extern const MIDL_FORMAT_STRING __MIDLProcFormatString;


extern const MIDL_STUB_DESC Object_StubDesc;


#pragma code_seg(".orpc")

HRESULT __stdcall ILookup_LookupByName_Proxy( 
    ILookup __RPC_FAR * This,
    /* [in] */ LPTSTR lpName,
    /* [string][out] */ WCHAR __RPC_FAR *__RPC_FAR *lplpNumber)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    *lplpNumber = 0;
    RpcTryExcept
        {
        RpcTryFinally
            {
            NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      3);
            
            
            
            
            _StubMsg.BufferLength = 12U;
            NdrConformantStringBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                           (unsigned char __RPC_FAR *)lpName,
                                           (PFORMAT_STRING) &__MIDLFormatString.Format[2] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrConformantStringMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                         (unsigned char __RPC_FAR *)lpName,
                                         (PFORMAT_STRING) &__MIDLFormatString.Format[2] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[0] );
            
            NdrPointerUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&lplpNumber,
                                  (PFORMAT_STRING) &__MIDLFormatString.Format[4],
                                  (unsigned char)0 );
            
            _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDLFormatString.Format[4],
                         ( void __RPC_FAR * )lplpNumber);
        _RetVal = NdrProxyErrorHandler(GetExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ILookup_LookupByName_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    WCHAR __RPC_FAR *_M4;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    LPTSTR lpName;
    WCHAR __RPC_FAR *__RPC_FAR *lplpNumber;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    lpName = 0;
    lplpNumber = 0;
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[0] );
        
        NdrConformantStringUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                       (unsigned char __RPC_FAR * __RPC_FAR *)&lpName,
                                       (PFORMAT_STRING) &__MIDLFormatString.Format[2],
                                       (unsigned char)0 );
        
        lplpNumber = &_M4;
        _M4 = 0;
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ILookup *) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> LookupByName(
                (ILookup *) ((CStdStubBuffer *)This)->pvServerObject,
                lpName,
                lplpNumber);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 16U + 10U;
        NdrPointerBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR *)lplpNumber,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[4] );
        
        _StubMsg.BufferLength += 16;
        
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        NdrPointerMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                            (unsigned char __RPC_FAR *)lplpNumber,
                            (PFORMAT_STRING) &__MIDLFormatString.Format[4] );
        
        _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrPointerFree( &_StubMsg,
                        (unsigned char __RPC_FAR *)lplpNumber,
                        &__MIDLFormatString.Format[4] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


HRESULT __stdcall ILookup_LookupByNumber_Proxy( 
    ILookup __RPC_FAR * This,
    /* [in] */ LPSTR lpNumber,
    /* [string][out] */ WCHAR __RPC_FAR *__RPC_FAR *lplpName)
{

    HRESULT _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    *lplpName = 0;
    RpcTryExcept
        {
        RpcTryFinally
            {
            NdrProxyInitialize(
                      ( void __RPC_FAR *  )This,
                      ( PRPC_MESSAGE  )&_RpcMessage,
                      ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                      ( PMIDL_STUB_DESC  )&Object_StubDesc,
                      4);
            
            
            
            
            _StubMsg.BufferLength = 12U;
            NdrConformantStringBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                           (unsigned char __RPC_FAR *)lpNumber,
                                           (PFORMAT_STRING) &__MIDLFormatString.Format[14] );
            
            NdrProxyGetBuffer(This, &_StubMsg);
            NdrConformantStringMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                                         (unsigned char __RPC_FAR *)lpNumber,
                                         (PFORMAT_STRING) &__MIDLFormatString.Format[14] );
            
            NdrProxySendReceive(This, &_StubMsg);
            
            if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
                NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[10] );
            
            NdrPointerUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                  (unsigned char __RPC_FAR * __RPC_FAR *)&lplpName,
                                  (PFORMAT_STRING) &__MIDLFormatString.Format[4],
                                  (unsigned char)0 );
            
            _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
            _RetVal = *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++;
            
            }
        RpcFinally
            {
            NdrProxyFreeBuffer(This, &_StubMsg);
            
            }
        RpcEndFinally
        
        }
    RpcExcept(_StubMsg.dwStubPhase != PROXY_SENDRECEIVE)
        {
        NdrClearOutParameters(
                         ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                         ( PFORMAT_STRING  )&__MIDLFormatString.Format[4],
                         ( void __RPC_FAR * )lplpName);
        _RetVal = NdrProxyErrorHandler(GetExceptionCode());
        }
    RpcEndExcept
    return _RetVal;
}

void __RPC_STUB ILookup_LookupByNumber_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase)
{
    WCHAR __RPC_FAR *_M9;
    HRESULT _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    LPSTR lpNumber;
    WCHAR __RPC_FAR *__RPC_FAR *lplpName;
    
NdrStubInitialize(
                     _pRpcMessage,
                     &_StubMsg,
                     &Object_StubDesc,
                     _pRpcChannelBuffer);
    lpNumber = 0;
    lplpName = 0;
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDLProcFormatString.Format[10] );
        
        NdrConformantStringUnmarshall( (PMIDL_STUB_MESSAGE) &_StubMsg,
                                       (unsigned char __RPC_FAR * __RPC_FAR *)&lpNumber,
                                       (PFORMAT_STRING) &__MIDLFormatString.Format[14],
                                       (unsigned char)0 );
        
        lplpName = &_M9;
        _M9 = 0;
        
        *_pdwStubPhase = STUB_CALL_SERVER;
        _RetVal = (((ILookup *) ((CStdStubBuffer *)This)->pvServerObject)->lpVtbl) -> LookupByNumber(
                  (ILookup *) ((CStdStubBuffer *)This)->pvServerObject,
                  lpNumber,
                  lplpName);
        
        *_pdwStubPhase = STUB_MARSHAL;
        
        _StubMsg.BufferLength = 16U + 10U;
        NdrPointerBufferSize( (PMIDL_STUB_MESSAGE) &_StubMsg,
                              (unsigned char __RPC_FAR *)lplpName,
                              (PFORMAT_STRING) &__MIDLFormatString.Format[4] );
        
        _StubMsg.BufferLength += 16;
        
        NdrStubGetBuffer(This, _pRpcChannelBuffer, &_StubMsg);
        NdrPointerMarshall( (PMIDL_STUB_MESSAGE)& _StubMsg,
                            (unsigned char __RPC_FAR *)lplpName,
                            (PFORMAT_STRING) &__MIDLFormatString.Format[4] );
        
        _StubMsg.Buffer = (unsigned char __RPC_FAR *)(((long)_StubMsg.Buffer + 3) & ~ 0x3);
        *(( HRESULT __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        NdrPointerFree( &_StubMsg,
                        (unsigned char __RPC_FAR *)lplpName,
                        &__MIDLFormatString.Format[4] );
        
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


static const MIDL_STUB_DESC Object_StubDesc = 
    {
    0,
    NdrOleAllocate,
    NdrOleFree,
    0,
    0,
    0,
    0,
    0,
    __MIDLFormatString.Format,
    0, /* -error bounds_check flag */
    0x10001, /* Ndr library version */
    0,
    0x2000066, /* MIDL Version 2.0.102 */
    0
    };

const CInterfaceProxyVtbl _ILookupProxyVtbl = 
{
    &IID_ILookup,
    IUnknown_QueryInterface_Proxy,
    IUnknown_AddRef_Proxy,
    IUnknown_Release_Proxy ,
    ILookup_LookupByName_Proxy ,
    ILookup_LookupByNumber_Proxy
};


static const PRPC_STUB_FUNCTION ILookup_table[] =
{
    ILookup_LookupByName_Stub,
    ILookup_LookupByNumber_Stub
};

const CInterfaceStubVtbl _ILookupStubVtbl =
{
    &IID_ILookup,
    0,
    5,
    &ILookup_table[-3],
    CStdStubBuffer_METHODS
};

#pragma data_seg(".rdata")

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

static const MIDL_FORMAT_STRING __MIDLProcFormatString =
    {
        0,
        {
			
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/*  2 */	NdrFcShort( 0x0 ),	/* Type Offset=0 */
/*  4 */	
			0x51,		/* FC_OUT_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/*  6 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */
/*  8 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */
/* 10 */	
			0x4d,		/* FC_IN_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 12 */	NdrFcShort( 0xc ),	/* Type Offset=12 */
/* 14 */	
			0x51,		/* FC_OUT_PARAM */
#ifndef _ALPHA_
			0x1,		/* x86, MIPS & PPC Stack size = 1 */
#else
			0x2,		/* Alpha Stack size = 2 */
#endif
/* 16 */	NdrFcShort( 0x4 ),	/* Type Offset=4 */
/* 18 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0x8,		/* FC_LONG */

			0x0
        }
    };

static const MIDL_FORMAT_STRING __MIDLFormatString =
    {
        0,
        {
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/*  2 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/*  4 */	
			0x11, 0x14,	/* FC_RP [alloced_on_stack] */
/*  6 */	NdrFcShort( 0x2 ),	/* Offset= 2 (8) */
/*  8 */	
			0x13, 0x8,	/* FC_OP [simple_pointer] */
/* 10 */	
			0x25,		/* FC_C_WSTRING */
			0x5c,		/* FC_PAD */
/* 12 */	
			0x11, 0x8,	/* FC_RP [simple_pointer] */
/* 14 */	
			0x22,		/* FC_C_CSTRING */
			0x5c,		/* FC_PAD */

			0x0
        }
    };

const CInterfaceProxyVtbl * _Lookup_ProxyVtblList[] = 
{
    ( CInterfaceProxyVtbl *) &_ILookupProxyVtbl,
    0
};

const CInterfaceStubVtbl * _Lookup_StubVtblList[] = 
{
    ( CInterfaceStubVtbl *) &_ILookupStubVtbl,
    0
};

PCInterfaceName const _Lookup_InterfaceNamesList[] = 
{
    "ILookup",
    0
};


#define _Lookup_CHECK_IID(n)	IID_GENERIC_CHECK_IID( _Lookup, pIID, n)

int __stdcall _Lookup_IID_Lookup( const IID * pIID, int * pIndex )
{
    
    if(!_Lookup_CHECK_IID(0))
        {
        *pIndex = 0;
        return 1;
        }

    return 0;
}

const ExtendedProxyFileInfo Lookup_ProxyFileInfo = 
{
    (PCInterfaceProxyVtblList *) & _Lookup_ProxyVtblList,
    (PCInterfaceStubVtblList *) & _Lookup_StubVtblList,
    (const PCInterfaceName * ) & _Lookup_InterfaceNamesList,
    0, // no delegation
    & _Lookup_IID_Lookup, 
    1,
    1
};
