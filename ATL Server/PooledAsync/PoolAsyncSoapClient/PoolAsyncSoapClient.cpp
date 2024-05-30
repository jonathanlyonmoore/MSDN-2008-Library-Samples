// PoolAsyncSoapClient.cpp : Defines the entry point for the console application.
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#include "stdafx.h"
#include <windows.h>
#include <iostream>
using namespace std;
#include "SynchronousSrv.h"
#include "PooledAsyncSrv.h"

using namespace SynchronousSoapService;
using namespace PoolAsyncSoapService;
#include "CmdLineParser.h"
#define DEFAULT_CLIENT_COUNT	8

struct stClientResults
{
	bool	bSucceeded;
	DWORD	dwTimeSpan;
};


HANDLE				g_hSyncEvent;
stClientResults		*g_arResults;



#ifdef UNICODE
#define _tcout	wcout
#else
#define _tcout cout
#endif


void	displayUsage()
{
	_tcout	<<	_T("Usage : PoolAsyncSoapClient [/clients <N>]")	<<	endl;
	_tcout	<<	_T("\t <N> - the number of clients to create, default ")	<<	DEFAULT_CLIENT_COUNT	<<	endl;

}




void displayResults(unsigned int iCount, DWORD dwTimeSpan )
{
	_tcout	<<	_T("RESULTS")	<<	endl;
	_tcout	<<	_T("==================")	<<	endl;
	_tcout	<<	_T("Total time for ") << iCount	<<_T(" clients :")	<<	dwTimeSpan	<<	endl;

	for( unsigned int iIndex = 0; iIndex < iCount; iIndex ++ )
		_tcout	<<	_T("Client ")	<<	iIndex  +1	<<	_T(" : Time=")	<<	g_arResults[iIndex].dwTimeSpan	
			<<	_T(", Result=")	<<	(LPCTSTR)(g_arResults[iIndex].bSucceeded?_T("PASSED"):_T("FAILED"))	<<	endl;

}

template<class CSoapClient>
DWORD WINAPI soapClientProc(LPVOID lpvParam)
{
	CoInitialize(NULL);

	{
		CSoapClient		client;
		HRESULT			hRet;
		BSTR			bstrIn, bstrOut;
		
		// using DWORD_PTR because it is an integral type of the same size as LPVOID
		DWORD_PTR		dwIndex	=	0;
		DWORD			dwTimeStart, dwTimeEnd;		
		
		dwIndex	=	reinterpret_cast<DWORD_PTR>(lpvParam);
		::WaitForSingleObject( g_hSyncEvent, INFINITE );


		client.SetTimeout(2000000);

		bstrIn	=	::SysAllocString( L"TestValue");

		dwTimeStart	=	::GetTickCount();

		if ( bstrIn != NULL )
		{
			hRet	=	client.performStringOperation(bstrIn, &bstrOut);
			dwTimeEnd	=	::GetTickCount();
            g_arResults[dwIndex].dwTimeSpan	=	dwTimeEnd - dwTimeStart;
			g_arResults[dwIndex].bSucceeded =	false;
			if( SUCCEEDED( hRet ) )
			{
				g_arResults[dwIndex].bSucceeded =	true;
				::SysFreeString( bstrOut );
			}
			::SysFreeString( bstrIn );
		}

		

		client.Cleanup();
	}

	CoUninitialize();
	return 0;
}

template<class T>
class HandleAdapter
{
	T			*arHandle;
	HandleAdapter(const HandleAdapter&);
public:
	HandleAdapter(DWORD iNumClients)
	{
	  arHandle = new T[iNumClients];
	  ATLENSURE(arHandle);
	}
	~HandleAdapter()
	{
		if(arHandle)delete[]	arHandle;
	}
	T* get()
	{
		return arHandle;
	}
};


void DoSyncWork(DWORD iNumClients )
{
	HANDLE			*arThreads	=	NULL;
	DWORD			dwStart, dwEnd;
	HandleAdapter<HANDLE>   auto_thread(iNumClients);
	HandleAdapter<stClientResults> auto_client(iNumClients);

	arThreads	=	auto_thread.get();
	g_arResults	=	auto_client.get(); 
	

	
	g_hSyncEvent	=	::CreateEvent( NULL, TRUE, FALSE, _T("SoapClientProcSyncEvent"));
	ATLENSURE(g_hSyncEvent);

	_tcout	<<	endl;
	_tcout	<<	_T("Running the synchronous server:")	<<	endl;
	
	// using DWORD_PTR because it is an integral type of the same size as LPVOID
	for( DWORD_PTR dwIndex = 0; dwIndex < iNumClients; dwIndex ++)
	{
		arThreads[dwIndex]	=	::CreateThread( NULL, 0, soapClientProc<CSynchronousSoapService>, reinterpret_cast<LPVOID>(dwIndex), 0, NULL);
		ATLENSURE(arThreads[dwIndex]);
	}


	dwStart	=	::GetTickCount();
	::SetEvent( g_hSyncEvent );

	::WaitForMultipleObjects( iNumClients, arThreads, TRUE, INFINITE);
	dwEnd	=	::GetTickCount();

	// Display results
	displayResults(iNumClients, dwEnd-dwStart);

	::ResetEvent( g_hSyncEvent );
	

    for(DWORD_PTR dwIndex = 0; dwIndex < iNumClients; dwIndex++)
	{
		if(arThreads[dwIndex])
		CloseHandle(arThreads[dwIndex]);
	}

}

void DoASyncWork(DWORD iNumClients )
{
	HANDLE			*arThreads	=	NULL;
	DWORD			dwStart, dwEnd;
	
	_tcout	<<	endl;
	_tcout	<<	_T("Running the asynchronous, pool-based, server:")	<<	endl;

	HandleAdapter<HANDLE>   auto_thread(iNumClients);
	HandleAdapter<stClientResults> auto_client(iNumClients);

	arThreads	=	auto_thread.get();
	g_arResults	=	auto_client.get(); 
	


	// using DWORD_PTR because it is an integral type of the same size as LPVOID
	for( DWORD_PTR	dwIndex = 0; dwIndex < iNumClients; dwIndex ++)
	{
		arThreads[dwIndex]	=	::CreateThread( NULL, 0, soapClientProc<CPoolAsyncSoapService>, reinterpret_cast<LPVOID>(dwIndex), 0, NULL);
		ATLENSURE(arThreads[dwIndex]);
	}


	dwStart	=	::GetTickCount();
	::SetEvent( g_hSyncEvent );

	::WaitForMultipleObjects( iNumClients, arThreads, TRUE, INFINITE);
	dwEnd	=	::GetTickCount();

	// Display results
	displayResults(iNumClients, dwEnd-dwStart);

	::ResetEvent( g_hSyncEvent );
	
	
    for(DWORD_PTR dwIndex = 0; dwIndex < iNumClients; dwIndex++)
	{
		if(arThreads[dwIndex])
		CloseHandle(arThreads[dwIndex]);
	}

	
}

void main(char argc, TCHAR* argv[])
{
	CCmdLineParser	cmdLineParser;
	unsigned int	iNumClients;
	

   

    cmdLineParser.parse( argc, argv);

	if( cmdLineParser.lookup(_T("help")) || 
		cmdLineParser.lookup(_T("h")) ||
		cmdLineParser.lookup(_T("-h")) || 
		cmdLineParser.lookup(_T("?")))
	{
		displayUsage();
		return;
	}

	{
		LPCTSTR	szNumClients	=	cmdLineParser.lookup(_T("clients"));
		if( szNumClients )
			iNumClients	=	_ttoi( szNumClients );
		else
			iNumClients	=	DEFAULT_CLIENT_COUNT;
	}
	
	 DoSyncWork(iNumClients );
	 DoASyncWork(iNumClients );
	 ::CloseHandle( g_hSyncEvent );

}
