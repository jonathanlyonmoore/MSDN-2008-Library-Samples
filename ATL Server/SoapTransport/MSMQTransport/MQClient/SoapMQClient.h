// File: SoapMQClient.h
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#pragma once
#ifndef SOAP_MQ_CLIENT_H_INCLUDED
#define SOAP_MQ_CLIENT_H_INCLUDED

// Includes for SoapDebugClient
#include <stdio.h>


#include <iostream>
using namespace std;


class CMQSendRequest
{
protected:
	CMQQueue						m_msgQueue;

public:

	bool	Initialize( wchar_t	*lpwszQueuePath)
	{
		bool			bRet = true;
		wchar_t			lpwszQueueName[MAX_PATH];
		static	wchar_t	*lpwszLabel	=	L"SOAP Requests Queue";

        if (lpwszQueuePath == NULL)
            return false;
        if (wcslen(lpwszQueuePath) >= MAX_PATH) 
            return false;
		wcscpy_s( lpwszQueueName, _countof(lpwszQueueName), lpwszQueuePath);

		HRESULT hr = m_msgQueue.Create(lpwszQueueName, lpwszLabel);
		if (hr == MQ_ERROR_QUEUE_EXISTS)
		{
			hr = m_msgQueue.PathNameToFormatName(lpwszQueueName);
			if (FAILED(hr))
			{
				printf("Failed PathNameToFormatName%08x\n", hr);
				bRet	=	false;
			}
		}
		else if (FAILED(hr))
		{
			printf("Failed creating the queue %08x\n", hr);
			bRet	=	false;;
		}	
		
		if( bRet )
		{
			
			hr = m_msgQueue.Open(lpwszQueueName, MQ_SEND_ACCESS);
			if (FAILED(hr))
			{
				printf("Failed opening m_msgQueue, error = 0x%x\n",hr);
				bRet	=	false;
			}
		}

		return bRet;
	}


	bool	SendSOAPRequest(LPCSTR	szURL, LPCSTR	szSOAPAction, LPCSTR	szXMLBuff, wchar_t	*lpwszResponseQueue)
	{

		if( ( NULL == m_msgQueue.GetHandle() )||
			( INVALID_HANDLE_VALUE == m_msgQueue.GetHandle() ) )
			return false;
		
		CMQMessageProps props;

		CStringA	strURLAndAction;
		CA2W		wszSOAPAction( szSOAPAction );
		WCHAR		*wszLabel;
		WCHAR		wszFormat[MAX_PATH];

		// storing both the URL and the SOAP Action in the label,
		strURLAndAction	=	szURL;
		// \n is the separator, as it is not permitted in an URL
		strURLAndAction	+=	"\n";
		strURLAndAction	+=	szSOAPAction;
		wszLabel	=	new WCHAR[strURLAndAction.GetLength() + 1];
        if (wszLabel == NULL) 
            return false;


		wcscpy_s(wszLabel, strURLAndAction.GetLength() + 1, CA2W(strURLAndAction));

		
		DWORD		dwLength	=	MAX_PATH;
		HRESULT Result = ::MQPathNameToFormatName( lpwszResponseQueue, wszFormat, &dwLength);
        if (FAILED(Result)) 
            return false;

		props.Add(PROPID_M_LABEL, wszLabel);
		props.Add(PROPID_M_BODY, (unsigned char*)szXMLBuff, (int)strlen(szXMLBuff), sizeof(UCHAR));
		props.Add(PROPID_M_RESP_QUEUE, wszFormat);
		

		return SUCCEEDED(m_msgQueue.Send(&props, NULL));
	}
};





class CMQRecvRequest
{
protected:
	CMQQueue						m_msgQueue;


public:
	// this method creates/opens the requests message queue
	bool	Initialize(wchar_t*	lpwszQueuePath)
	{
		bool		bRet = true;
		static		wchar_t	*lpwszLabel	=	L"SOAP Responses Queue";
		wchar_t		lpwszQueueName[MAX_PATH];

        if (lpwszQueuePath == NULL)
            return false;
        if (wcslen(lpwszQueuePath) >= MAX_PATH) 
            return false;
		wcscpy_s( lpwszQueueName, _countof(lpwszQueueName), lpwszQueuePath);
		
		HRESULT hr;

		hr = m_msgQueue.Create(lpwszQueueName, lpwszLabel);
		if (hr == MQ_ERROR_QUEUE_EXISTS)
		{
			
			hr = m_msgQueue.PathNameToFormatName(lpwszQueueName);
			if (FAILED(hr))
			{
				printf("Failed PathNameToFormatName%08x\n", hr);
				bRet	=	false;
			}

		}
		else if (FAILED(hr))
		{
			printf("Failed creating the queue %08x\n", hr);
			bRet	=	false;
		}	
		printf("Queue was created\n");

		if( bRet )
		{
			
			hr = m_msgQueue.Open();
			if (FAILED(hr))
			{
				printf("Failed m_msgQueue.Open() %08x\n", hr);
				bRet	=	false;
			}
		}
		return bRet;
	}



	bool	GetSoapResponse(CReadWriteStreamOnCString	&buff)
	{
		
		bool	bRet	=	QueueReceiveRequest(&m_msgQueue, buff);

		return bRet;
	}

protected:
	bool	QueueReceiveRequest(CMQQueue *pQueue, CReadWriteStreamOnCString	&buffer)
	{
		
		CMQMessageProps	msgProps;
		int	iBodySizeIndex, iLabelSizeIndex;
		bool		bRet	=	false;

        if (pQueue == NULL) return false;

		// Get the properties of the message in QUEUE
		// For this implementation, we only care about the the body size and the Label length, 
		// as these are the only fields  involve in SOAP
		
		iLabelSizeIndex	= msgProps.Add(PROPID_M_LABEL_LEN);

		iBodySizeIndex	=	msgProps.Add(PROPID_M_BODY_SIZE);


		HRESULT	hPeekRet;
		do
		{
			hPeekRet	=	pQueue->Peek(&msgProps, NULL);
			if( hPeekRet == MQ_ERROR_IO_TIMEOUT )
			{
				// No message. Sleeping...
				Sleep(100);
			}
		}
		while(hPeekRet != S_OK );
		
		
		if( SUCCEEDED(hPeekRet) )
		{
			CMQMessageProps *pProps = NULL;
			WCHAR *wszLabel = NULL;
			UCHAR *szBody = NULL;

            pProps = new CMQMessageProps;
            if (pProps == NULL) goto error_delete;
			DWORD			dwSize	=	0;
			int				iIndex	=	0;

			iIndex	=	pProps->Add(PROPID_M_LABEL_LEN);
			dwSize	=	msgProps.GetProperty(iLabelSizeIndex).ulVal;
			pProps->m_rgProps[iIndex].ulVal = dwSize;
			
			wszLabel = new WCHAR[dwSize];
            if (wszLabel == NULL) goto error_delete;
			iIndex	=	pProps->Add(PROPID_M_LABEL, wszLabel);
			

			iIndex  = pProps->Add(PROPID_M_BODY_SIZE);
			dwSize	=	msgProps.GetProperty(iBodySizeIndex).ulVal;
			pProps->m_rgProps[iIndex].ulVal = dwSize;

			szBody = new UCHAR[dwSize];
            if (szBody == NULL) goto error_delete;
			pProps->Add(PROPID_M_BODY, szBody, dwSize, sizeof(UCHAR));

			HRESULT	hRet	=	pQueue->Receive(pProps);
			if( !SUCCEEDED(hRet) )
			{
				printf("Error in Queue.Receive() %08x\n", hRet);
			}
			else
			{
				HRESULT Result = buffer.WriteStream( (LPCSTR)szBody, dwSize, NULL );
				bRet	= SUCCEEDED(Result);
			}
error_delete:
			delete	[]wszLabel;
			delete	[]szBody;
			delete	pProps;
		}

		return bRet;
	}

};



// LOG options
class CSoapMQClient
{
protected:
	CReadWriteStreamOnCString	m_requestStream;
	CReadWriteStreamOnCString	m_responseStream;
	WCHAR						m_lpwszRequestsQueue[MAX_PATH];
	WCHAR						m_lpwszResponsesQueue[MAX_PATH];
	SOAPCLIENT_ERROR			m_errorState;
	CStringA					m_strURL;
public:
	CSoapMQClient(LPCTSTR szURL) : m_strURL(CT2A(szURL)), m_errorState(SOAPCLIENT_SUCCESS)
	{
		m_lpwszRequestsQueue[0]		=	0x00;
		m_lpwszResponsesQueue[0]	=	0x00;
	}

	~CSoapMQClient() throw()
	{
		CleanupClient();
	}

	SOAPCLIENT_ERROR GetClientError()
	{
		return m_errorState;
	}

	void SetClientError(SOAPCLIENT_ERROR errorState)
	{
		m_errorState = errorState;
	}


	void		SetQueues(LPCSTR	strReqQueue,	LPCSTR	strResQueue  = NULL)
	{
		ATLASSERT( strReqQueue != NULL );
		CA2W		wszReqQueue( strReqQueue );
		CA2W		wszResQueue( strResQueue?strResQueue:".\\SOAPResponses" );

		memset( m_lpwszRequestsQueue, 0, MAX_PATH*sizeof(WCHAR));
		memset( m_lpwszResponsesQueue, 0, MAX_PATH*sizeof(WCHAR));
		
		wcsncpy_s( m_lpwszRequestsQueue, _countof(m_lpwszRequestsQueue), wszReqQueue, _TRUNCATE);
		wcsncpy_s( m_lpwszResponsesQueue, _countof(m_lpwszResponsesQueue), wszResQueue, _TRUNCATE);

        m_lpwszRequestsQueue[MAX_PATH-1] = '\0';
        m_lpwszResponsesQueue[MAX_PATH-1] = '\0';

	}

	void CleanupClient() throw()
	{
		m_requestStream.Cleanup();
		m_responseStream.Cleanup();
	}
	
	IWriteStream * GetWriteStream() throw()
	{
		return &m_requestStream;
	}
	
	
	HRESULT GetReadStream(IStream **ppStream) throw()
	{
		if (ppStream == NULL)
		{
			return E_POINTER;
		}

		*ppStream = &m_responseStream;
		return S_OK;
	}
	
	HRESULT SendRequest(LPCTSTR tszAction) throw()
	{
		CAtlIsapiBuffer<>		buffWork;
		CMQSendRequest			mqSender;
		CMQRecvRequest			mqReceiver;
		
		
		ATLENSURE(tszAction != NULL);

		LPCSTR	szAction;

#ifdef UNICODE
		CW2A	w2aAction(tszAction);
		szAction	=	w2aAction;
		
#else
		szAction	=	tszAction;
#endif
		
		bool					bContinue	=	true;

		if( wcslen(m_lpwszRequestsQueue) == 0 )
		{
			printf("No request queue specified. Use setQueues\n");
			return E_FAIL;
		}


		if( strstr(szAction, "SOAPAction:") == szAction )
			szAction	+=	strlen("SOAPAction:");

		while( *szAction	==	' ')
			szAction++;
		

		bContinue	=	mqSender.Initialize( m_lpwszRequestsQueue );
		if( !bContinue )
			printf("Failed to initialize requests queue\n");

		if( bContinue )
		{
			bContinue	=	mqSender.SendSOAPRequest(m_strURL, szAction, (LPCSTR)m_requestStream.m_str, m_lpwszResponsesQueue);
			if( !bContinue )
				printf("Failed to send the request queue\n");

		}
		



		if( bContinue )
		{
			bContinue	=	mqReceiver.Initialize( m_lpwszResponsesQueue );
			if( !bContinue )
				printf("Failed to initialize the response queue\n");

		}

		if( bContinue )
		{
			bContinue	=	mqReceiver.GetSoapResponse(m_responseStream);
			if( !bContinue )
				printf("Failed to get the response\n");

		}
		

		return bContinue?S_OK:E_FAIL;
		
	}
};




#endif// SOAP_MQ_CLIENT_H_INCLUDED





