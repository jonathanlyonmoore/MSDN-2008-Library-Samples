// BidsIsapi -- the ISAPI portion of the VCBids program
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#include "stdafx.h"

#define _ATL_PERF_REGISTER


CComModule _Module;

#include <atlsync.inl>


#ifdef _DEBUG
CDebugReportHook g_ReportHook;
#endif

BEGIN_OBJECT_MAP(ObjectMap)
END_OBJECT_MAP()


/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point


extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	return _Module.DllMain(hInstance, dwReason, lpReserved, ObjectMap, NULL); 
}

class CMyIsapiWorker : public CIsapiWorker
{
public:
	typedef CDataSourceCache<> ds_cache_type; // per thread datasource cache
	CComObjectGlobal<ds_cache_type> m_dsCache;
	CMyIsapiWorker()
	{
	}
	~CMyIsapiWorker()
	{
	}
	
	virtual BOOL Initialize(void *pvParam)
	{
		BOOL bRet = CIsapiWorker::Initialize(pvParam);
		return bRet;
	}

	virtual void Terminate(void* pvParam)
	{
		CIsapiWorker::Terminate(pvParam);
	}
};

class CMyIsapiExtension : public CIsapiExtension<CThreadPool<CMyIsapiWorker> >
{
public:

    typedef CIsapiExtension<CThreadPool<CMyIsapiWorker> > parentType;

	virtual HRESULT STDMETHODCALLTYPE QueryService(REFGUID guidService,
			REFIID riid, void **ppvObject)
	{
		HRESULT hr = parentType::QueryService(guidService, riid, ppvObject);
		if (hr == E_POINTER || hr == E_NOINTERFACE)
		{
			if (InlineIsEqualGUID(guidService, __uuidof(IDataSourceCache)))
			{
				CMyIsapiWorker *pWorker = (CMyIsapiWorker *)GetThreadWorker();
				if (pWorker)
					return pWorker->m_dsCache.QueryInterface(riid, ppvObject);
			}
		}
		return hr;
	}
};

CMyIsapiExtension theExtension;

// delegate isapi exports to theExtension
extern "C" DWORD WINAPI HttpExtensionProc(LPEXTENSION_CONTROL_BLOCK lpECB)
{
	return theExtension.HttpExtensionProc(lpECB);
}

extern "C" BOOL WINAPI GetExtensionVersion(HSE_VERSION_INFO* pVer)
{
	return theExtension.GetExtensionVersion(pVer);
}

extern "C" BOOL WINAPI TerminateExtension(DWORD dwFlags)
{
	return theExtension.TerminateExtension(dwFlags);
}

