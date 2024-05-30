//
// sproxy.exe generated file
// do not modify this file
//
// Created: 07/03/2002@12:43:37
//

#pragma once

#include <atlsoap.h>

namespace MailService
{

struct _MSGDATA
{
	BSTR m_bstrFrom;
	double m_DateSent;
	BSTR m_bstrSubject;
	int m_lMessageID;
	int m_bMarkedRead;
};

template <typename TClient = CSoapSocketClientT<> >
class CMailServiceT : 
	public TClient, 
	public CSoapRootHandler
{
protected:

	const _soapmap ** GetFunctionMap();
	const _soapmap ** GetHeaderMap();
	void * GetHeaderValue();
	const wchar_t * GetNamespaceUri();
	const char * GetServiceName();
	const char * GetNamespaceUriA();
	HRESULT CallFunction(
		void *pvParam, 
		const wchar_t *wszLocalName, int cchLocalName,
		size_t nItem);
	HRESULT GetClientReader(ISAXXMLReader **ppReader);

public:

	HRESULT __stdcall QueryInterface(REFIID riid, void **ppv)
	{
		if (ppv == NULL)
		{
			return E_POINTER;
		}

		*ppv = NULL;

		if (InlineIsEqualGUID(riid, IID_IUnknown) ||
			InlineIsEqualGUID(riid, IID_ISAXContentHandler))
		{
			*ppv = static_cast<ISAXContentHandler *>(this);
			return S_OK;
		}

		return E_NOINTERFACE;
	}

	ULONG __stdcall AddRef()
	{
		return 1;
	}

	ULONG __stdcall Release()
	{
		return 1;
	}

	CMailServiceT(ISAXXMLReader *pReader = NULL)
		:TClient(_T("http://localhost/mantaweb/cgi-bin/MailService.dll\?Handler=Default"))
	{
		SetClient(true);
		SetReader(pReader);
	}
	
	~CMailServiceT()
	{
		Uninitialize();
	}
	
	void Uninitialize()
	{
		UninitializeSOAP();
	}	

	HRESULT GetUnreadCount(
		int lUserID, 
		BSTR bstrSessionID, 
		int* lUnreadCount
	);

	HRESULT GetLaunchURL(
		int lUserID, 
		BSTR bstrSessionID, 
		int lMessageID, 
		BSTR* bstrURL
	);

	HRESULT GetMessageCount(
		int lUserID, 
		BSTR bstrSessionID, 
		int* lMessageCount
	);

	HRESULT Login(
		BSTR bstrLogin, 
		BSTR bstrPassword, 
		int* lUserID, 
		BSTR* bstrSessionID
	);

	HRESULT GetMessages(
		int lUserID, 
		BSTR bstrSessionID, 
		int lMax, 
		_MSGDATA** msgData, int* msgData_nSizeIs
	);
};

typedef CMailServiceT<> CMailService;

__if_not_exists(___MSGDATA_entries)
{
extern __declspec(selectany) const _soapmapentry ___MSGDATA_entries[] =
{
	{ 
		0x0F182BDB, 
		"m_bstrFrom", 
		L"m_bstrFrom", 
		sizeof("m_bstrFrom")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(_MSGDATA, m_bstrFrom),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xF4E49EE4, 
		"m_DateSent", 
		L"m_DateSent", 
		sizeof("m_DateSent")-1, 
		SOAPTYPE_DOUBLE, 
		SOAPFLAG_FIELD, 
		offsetof(_MSGDATA, m_DateSent),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xDEEDC8D7, 
		"m_bstrSubject", 
		L"m_bstrSubject", 
		sizeof("m_bstrSubject")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_FIELD | SOAPFLAG_NULLABLE, 
		offsetof(_MSGDATA, m_bstrSubject),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xF0A23FAA, 
		"m_lMessageID", 
		L"m_lMessageID", 
		sizeof("m_lMessageID")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_FIELD, 
		offsetof(_MSGDATA, m_lMessageID),
		NULL, 
		NULL, 
		-1 
	},
	{ 
		0xB25A0A7E, 
		"m_bMarkedRead", 
		L"m_bMarkedRead", 
		sizeof("m_bMarkedRead")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_FIELD, 
		offsetof(_MSGDATA, m_bMarkedRead),
		NULL, 
		NULL, 
		-1 
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap ___MSGDATA_map =
{
	0x9A79A6E0,
	"_MSGDATA",
	L"_MSGDATA",
	sizeof("_MSGDATA")-1,
	sizeof("_MSGDATA")-1,
	SOAPMAP_STRUCT,
	___MSGDATA_entries,
	sizeof(_MSGDATA),
	5,
	-1,
	SOAPFLAG_NONE,
	0x34F7DEC3,
	"urn:MailService",
	L"urn:MailService",
	sizeof("urn:MailService")-1
};
}

struct __CMailService_GetUnreadCount_struct
{
	int lUserID;
	BSTR bstrSessionID;
	int lUnreadCount;
};

extern __declspec(selectany) const _soapmapentry __CMailService_GetUnreadCount_entries[] =
{

	{
		0x483271B8, 
		"lUserID", 
		L"lUserID", 
		sizeof("lUserID")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		offsetof(__CMailService_GetUnreadCount_struct, lUserID),
		NULL,
		NULL,
		-1,
	},
	{
		0x3D124BCC, 
		"bstrSessionID", 
		L"bstrSessionID", 
		sizeof("bstrSessionID")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CMailService_GetUnreadCount_struct, bstrSessionID),
		NULL,
		NULL,
		-1,
	},
	{
		0xD9716B14, 
		"lUnreadCount", 
		L"lUnreadCount", 
		sizeof("lUnreadCount")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		offsetof(__CMailService_GetUnreadCount_struct, lUnreadCount),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CMailService_GetUnreadCount_map =
{
	0x205B11A8,
	"GetUnreadCount",
	L"GetUnreadCount",
	sizeof("GetUnreadCount")-1,
	sizeof("GetUnreadCount")-1,
	SOAPMAP_FUNC,
	__CMailService_GetUnreadCount_entries,
	sizeof(__CMailService_GetUnreadCount_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x34F7DEC3,
	"urn:MailService",
	L"urn:MailService",
	sizeof("urn:MailService")-1
};


struct __CMailService_GetLaunchURL_struct
{
	int lUserID;
	BSTR bstrSessionID;
	int lMessageID;
	BSTR bstrURL;
};

extern __declspec(selectany) const _soapmapentry __CMailService_GetLaunchURL_entries[] =
{

	{
		0x483271B8, 
		"lUserID", 
		L"lUserID", 
		sizeof("lUserID")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		offsetof(__CMailService_GetLaunchURL_struct, lUserID),
		NULL,
		NULL,
		-1,
	},
	{
		0x3D124BCC, 
		"bstrSessionID", 
		L"bstrSessionID", 
		sizeof("bstrSessionID")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CMailService_GetLaunchURL_struct, bstrSessionID),
		NULL,
		NULL,
		-1,
	},
	{
		0x9B9C3A3E, 
		"lMessageID", 
		L"lMessageID", 
		sizeof("lMessageID")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		offsetof(__CMailService_GetLaunchURL_struct, lMessageID),
		NULL,
		NULL,
		-1,
	},
	{
		0x8C7F944E, 
		"bstrURL", 
		L"bstrURL", 
		sizeof("bstrURL")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CMailService_GetLaunchURL_struct, bstrURL),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CMailService_GetLaunchURL_map =
{
	0x55EE902E,
	"GetLaunchURL",
	L"GetLaunchURL",
	sizeof("GetLaunchURL")-1,
	sizeof("GetLaunchURL")-1,
	SOAPMAP_FUNC,
	__CMailService_GetLaunchURL_entries,
	sizeof(__CMailService_GetLaunchURL_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x34F7DEC3,
	"urn:MailService",
	L"urn:MailService",
	sizeof("urn:MailService")-1
};


struct __CMailService_GetMessageCount_struct
{
	int lUserID;
	BSTR bstrSessionID;
	int lMessageCount;
};

extern __declspec(selectany) const _soapmapentry __CMailService_GetMessageCount_entries[] =
{

	{
		0x483271B8, 
		"lUserID", 
		L"lUserID", 
		sizeof("lUserID")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		offsetof(__CMailService_GetMessageCount_struct, lUserID),
		NULL,
		NULL,
		-1,
	},
	{
		0x3D124BCC, 
		"bstrSessionID", 
		L"bstrSessionID", 
		sizeof("bstrSessionID")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CMailService_GetMessageCount_struct, bstrSessionID),
		NULL,
		NULL,
		-1,
	},
	{
		0x65B8FB1A, 
		"lMessageCount", 
		L"lMessageCount", 
		sizeof("lMessageCount")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		offsetof(__CMailService_GetMessageCount_struct, lMessageCount),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CMailService_GetMessageCount_map =
{
	0x89D7742E,
	"GetMessageCount",
	L"GetMessageCount",
	sizeof("GetMessageCount")-1,
	sizeof("GetMessageCount")-1,
	SOAPMAP_FUNC,
	__CMailService_GetMessageCount_entries,
	sizeof(__CMailService_GetMessageCount_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x34F7DEC3,
	"urn:MailService",
	L"urn:MailService",
	sizeof("urn:MailService")-1
};


struct __CMailService_Login_struct
{
	BSTR bstrLogin;
	BSTR bstrPassword;
	int lUserID;
	BSTR bstrSessionID;
};

extern __declspec(selectany) const _soapmapentry __CMailService_Login_entries[] =
{

	{
		0xAA236B14, 
		"bstrLogin", 
		L"bstrLogin", 
		sizeof("bstrLogin")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CMailService_Login_struct, bstrLogin),
		NULL,
		NULL,
		-1,
	},
	{
		0x6E0C36CE, 
		"bstrPassword", 
		L"bstrPassword", 
		sizeof("bstrPassword")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CMailService_Login_struct, bstrPassword),
		NULL,
		NULL,
		-1,
	},
	{
		0x483271B8, 
		"lUserID", 
		L"lUserID", 
		sizeof("lUserID")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		offsetof(__CMailService_Login_struct, lUserID),
		NULL,
		NULL,
		-1,
	},
	{
		0x3D124BCC, 
		"bstrSessionID", 
		L"bstrSessionID", 
		sizeof("bstrSessionID")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CMailService_Login_struct, bstrSessionID),
		NULL,
		NULL,
		-1,
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CMailService_Login_map =
{
	0x059DE879,
	"Login",
	L"Login",
	sizeof("Login")-1,
	sizeof("Login")-1,
	SOAPMAP_FUNC,
	__CMailService_Login_entries,
	sizeof(__CMailService_Login_struct),
	2,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x34F7DEC3,
	"urn:MailService",
	L"urn:MailService",
	sizeof("urn:MailService")-1
};


struct __CMailService_GetMessages_struct
{
	int lUserID;
	BSTR bstrSessionID;
	int lMax;
	_MSGDATA *msgData;
	int __msgData_nSizeIs;
};

extern __declspec(selectany) const _soapmapentry __CMailService_GetMessages_entries[] =
{

	{
		0x483271B8, 
		"lUserID", 
		L"lUserID", 
		sizeof("lUserID")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		offsetof(__CMailService_GetMessages_struct, lUserID),
		NULL,
		NULL,
		-1,
	},
	{
		0x3D124BCC, 
		"bstrSessionID", 
		L"bstrSessionID", 
		sizeof("bstrSessionID")-1, 
		SOAPTYPE_STRING, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CMailService_GetMessages_struct, bstrSessionID),
		NULL,
		NULL,
		-1,
	},
	{
		0x003C8D72, 
		"lMax", 
		L"lMax", 
		sizeof("lMax")-1, 
		SOAPTYPE_INT, 
		SOAPFLAG_NONE | SOAPFLAG_IN | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		offsetof(__CMailService_GetMessages_struct, lMax),
		NULL,
		NULL,
		-1,
	},
	{
		0xDA3BF021, 
		"msgData", 
		L"msgData", 
		sizeof("msgData")-1, 
		SOAPTYPE_UNK, 
		SOAPFLAG_NONE | SOAPFLAG_OUT | SOAPFLAG_DYNARR | SOAPFLAG_RPC | SOAPFLAG_ENCODED | SOAPFLAG_NULLABLE,
		offsetof(__CMailService_GetMessages_struct, msgData),
		NULL,
		&___MSGDATA_map,
		3+1,
	},
	{
		0xDA3BF021,
		"__msgData_nSizeIs",
		L"__msgData_nSizeIs",
		sizeof("__msgData_nSizeIs")-1,
		SOAPTYPE_INT,
		SOAPFLAG_NOMARSHAL,
		offsetof(__CMailService_GetMessages_struct, __msgData_nSizeIs),
		NULL,
		NULL,
		-1
	},
	{ 0x00000000 }
};

extern __declspec(selectany) const _soapmap __CMailService_GetMessages_map =
{
	0xE064F1F8,
	"GetMessages",
	L"GetMessages",
	sizeof("GetMessages")-1,
	sizeof("GetMessages")-1,
	SOAPMAP_FUNC,
	__CMailService_GetMessages_entries,
	sizeof(__CMailService_GetMessages_struct),
	1,
	-1,
	SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
	0x34F7DEC3,
	"urn:MailService",
	L"urn:MailService",
	sizeof("urn:MailService")-1
};

extern __declspec(selectany) const _soapmap * __CMailService_funcs[] =
{
	&__CMailService_GetUnreadCount_map,
	&__CMailService_GetLaunchURL_map,
	&__CMailService_GetMessageCount_map,
	&__CMailService_Login_map,
	&__CMailService_GetMessages_map,
	NULL
};

template <typename TClient>
inline HRESULT CMailServiceT<TClient>::GetUnreadCount(
		int lUserID, 
		BSTR bstrSessionID, 
		int* lUnreadCount
	)
{
    if ( lUnreadCount == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CMailService_GetUnreadCount_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.lUserID = lUserID;
	__params.bstrSessionID = bstrSessionID;

	__atlsoap_hr = SetClientStruct(&__params, 0);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"#GetUnreadCount\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*lUnreadCount = __params.lUnreadCount;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CMailServiceT<TClient>::GetLaunchURL(
		int lUserID, 
		BSTR bstrSessionID, 
		int lMessageID, 
		BSTR* bstrURL
	)
{
    if ( bstrURL == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CMailService_GetLaunchURL_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.lUserID = lUserID;
	__params.bstrSessionID = bstrSessionID;
	__params.lMessageID = lMessageID;

	__atlsoap_hr = SetClientStruct(&__params, 1);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"#GetLaunchURL\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*bstrURL = __params.bstrURL;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CMailServiceT<TClient>::GetMessageCount(
		int lUserID, 
		BSTR bstrSessionID, 
		int* lMessageCount
	)
{
    if ( lMessageCount == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CMailService_GetMessageCount_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.lUserID = lUserID;
	__params.bstrSessionID = bstrSessionID;

	__atlsoap_hr = SetClientStruct(&__params, 2);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"#GetMessageCount\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*lMessageCount = __params.lMessageCount;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CMailServiceT<TClient>::Login(
		BSTR bstrLogin, 
		BSTR bstrPassword, 
		int* lUserID, 
		BSTR* bstrSessionID
	)
{
    if ( lUserID == NULL )
		return E_POINTER;
    if ( bstrSessionID == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CMailService_Login_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.bstrLogin = bstrLogin;
	__params.bstrPassword = bstrPassword;

	__atlsoap_hr = SetClientStruct(&__params, 3);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"#Login\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*lUserID = __params.lUserID;
	*bstrSessionID = __params.bstrSessionID;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
inline HRESULT CMailServiceT<TClient>::GetMessages(
		int lUserID, 
		BSTR bstrSessionID, 
		int lMax, 
		_MSGDATA** msgData, int* __msgData_nSizeIs
	)
{
    if ( msgData == NULL )
		return E_POINTER;
    if( __msgData_nSizeIs == NULL )
		return E_POINTER;

	HRESULT __atlsoap_hr = InitializeSOAP(NULL);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_INITIALIZE_ERROR);
		return __atlsoap_hr;
	}
	
	CleanupClient();

	CComPtr<IStream> __atlsoap_spReadStream;
	__CMailService_GetMessages_struct __params;
	memset(&__params, 0x00, sizeof(__params));
	__params.lUserID = lUserID;
	__params.bstrSessionID = bstrSessionID;
	__params.lMax = lMax;

	__atlsoap_hr = SetClientStruct(&__params, 4);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_OUTOFMEMORY);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = GenerateResponse(GetWriteStream());
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_GENERATE_ERROR);
		goto __skip_cleanup;
	}
	
	__atlsoap_hr = SendRequest(_T("SOAPAction: \"#GetMessages\"\r\n"));
	if (FAILED(__atlsoap_hr))
	{
		goto __skip_cleanup;
	}
	__atlsoap_hr = GetReadStream(&__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_READ_ERROR);
		goto __skip_cleanup;
	}
	
	// cleanup any in/out-params and out-headers from previous calls
	Cleanup();
	__atlsoap_hr = BeginParse(__atlsoap_spReadStream);
	if (FAILED(__atlsoap_hr))
	{
		SetClientError(SOAPCLIENT_PARSE_ERROR);
		goto __cleanup;
	}

	*msgData = __params.msgData;
	*__msgData_nSizeIs = __params.__msgData_nSizeIs;
	goto __skip_cleanup;
	
__cleanup:
	Cleanup();
__skip_cleanup:
	ResetClientState(true);
	memset(&__params, 0x00, sizeof(__params));
	return __atlsoap_hr;
}

template <typename TClient>
ATL_NOINLINE inline const _soapmap ** CMailServiceT<TClient>::GetFunctionMap()
{
	return __CMailService_funcs;
}

template <typename TClient>
ATL_NOINLINE inline const _soapmap ** CMailServiceT<TClient>::GetHeaderMap()
{
	static const _soapmapentry __CMailService_GetUnreadCount_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CMailService_GetUnreadCount_atlsoapheader_map = 
	{
		0x205B11A8,
		"GetUnreadCount",
		L"GetUnreadCount",
		sizeof("GetUnreadCount")-1,
		sizeof("GetUnreadCount")-1,
		SOAPMAP_HEADER,
		__CMailService_GetUnreadCount_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		0x34F7DEC3,
		"urn:MailService",
		L"urn:MailService",
		sizeof("urn:MailService")-1
	};

	static const _soapmapentry __CMailService_GetLaunchURL_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CMailService_GetLaunchURL_atlsoapheader_map = 
	{
		0x55EE902E,
		"GetLaunchURL",
		L"GetLaunchURL",
		sizeof("GetLaunchURL")-1,
		sizeof("GetLaunchURL")-1,
		SOAPMAP_HEADER,
		__CMailService_GetLaunchURL_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		0x34F7DEC3,
		"urn:MailService",
		L"urn:MailService",
		sizeof("urn:MailService")-1
	};

	static const _soapmapentry __CMailService_GetMessageCount_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CMailService_GetMessageCount_atlsoapheader_map = 
	{
		0x89D7742E,
		"GetMessageCount",
		L"GetMessageCount",
		sizeof("GetMessageCount")-1,
		sizeof("GetMessageCount")-1,
		SOAPMAP_HEADER,
		__CMailService_GetMessageCount_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		0x34F7DEC3,
		"urn:MailService",
		L"urn:MailService",
		sizeof("urn:MailService")-1
	};

	static const _soapmapentry __CMailService_Login_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CMailService_Login_atlsoapheader_map = 
	{
		0x059DE879,
		"Login",
		L"Login",
		sizeof("Login")-1,
		sizeof("Login")-1,
		SOAPMAP_HEADER,
		__CMailService_Login_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		0x34F7DEC3,
		"urn:MailService",
		L"urn:MailService",
		sizeof("urn:MailService")-1
	};

	static const _soapmapentry __CMailService_GetMessages_atlsoapheader_entries[] =
	{
		{ 0x00000000 }
	};

	static const _soapmap __CMailService_GetMessages_atlsoapheader_map = 
	{
		0xE064F1F8,
		"GetMessages",
		L"GetMessages",
		sizeof("GetMessages")-1,
		sizeof("GetMessages")-1,
		SOAPMAP_HEADER,
		__CMailService_GetMessages_atlsoapheader_entries,
		0,
		0,
		-1,
		SOAPFLAG_NONE | SOAPFLAG_RPC | SOAPFLAG_ENCODED,
		0x34F7DEC3,
		"urn:MailService",
		L"urn:MailService",
		sizeof("urn:MailService")-1
	};


	static const _soapmap * __CMailService_headers[] =
	{
		&__CMailService_GetUnreadCount_atlsoapheader_map,
		&__CMailService_GetLaunchURL_atlsoapheader_map,
		&__CMailService_GetMessageCount_atlsoapheader_map,
		&__CMailService_Login_atlsoapheader_map,
		&__CMailService_GetMessages_atlsoapheader_map,
		NULL
	};
	
	return __CMailService_headers;
}

template <typename TClient>
ATL_NOINLINE inline void * CMailServiceT<TClient>::GetHeaderValue()
{
	return this;
}

template <typename TClient>
ATL_NOINLINE inline const wchar_t * CMailServiceT<TClient>::GetNamespaceUri()
{
	return L"urn:MailService";
}

template <typename TClient>
ATL_NOINLINE inline const char * CMailServiceT<TClient>::GetServiceName()
{
	return NULL;
}

template <typename TClient>
ATL_NOINLINE inline const char * CMailServiceT<TClient>::GetNamespaceUriA()
{
	return "urn:MailService";
}

template <typename TClient>
ATL_NOINLINE inline HRESULT CMailServiceT<TClient>::CallFunction(
	void *, 
	const wchar_t *, int,
	size_t)
{
	return E_NOTIMPL;
}

template <typename TClient>
ATL_NOINLINE inline HRESULT CMailServiceT<TClient>::GetClientReader(ISAXXMLReader **ppReader)
{
	if (ppReader == NULL)
	{
		return E_INVALIDARG;
	}
	
	CComPtr<ISAXXMLReader> spReader = GetReader();
	if (spReader.p != NULL)
	{
		*ppReader = spReader.Detach();
		return S_OK;
	}
	return TClient::GetClientReader(ppReader);
}

} // namespace MailService

template<>
inline HRESULT AtlCleanupValue<MailService::_MSGDATA>(MailService::_MSGDATA *pVal)
{
	pVal;
	AtlCleanupValue(&pVal->m_bstrFrom);
	AtlCleanupValue(&pVal->m_DateSent);
	AtlCleanupValue(&pVal->m_bstrSubject);
	AtlCleanupValue(&pVal->m_lMessageID);
	AtlCleanupValue(&pVal->m_bMarkedRead);
	return S_OK;
}

template<>
inline HRESULT AtlCleanupValueEx<MailService::_MSGDATA>(MailService::_MSGDATA *pVal, IAtlMemMgr *pMemMgr)
{
	pVal;
	pMemMgr;
	
	AtlCleanupValueEx(&pVal->m_bstrFrom, pMemMgr);
	AtlCleanupValueEx(&pVal->m_DateSent, pMemMgr);
	AtlCleanupValueEx(&pVal->m_bstrSubject, pMemMgr);
	AtlCleanupValueEx(&pVal->m_lMessageID, pMemMgr);
	AtlCleanupValueEx(&pVal->m_bMarkedRead, pMemMgr);
	return S_OK;
}
