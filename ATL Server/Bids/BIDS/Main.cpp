// File: Main.cpp
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#include "stdafx.h"
#include "BidsDB.h"
#include "Bids.h"
#pragma once

HRESULT ATL_NOINLINE GetDataSourceSecure(IServiceProvider *pProvider, 
										  LPCTSTR szID, LPCOLESTR szConn, 
										  CDataConnection *pSession)
{
	CRevertThreadToken	revertThreadToken;
	HRESULT hr;
	if( revertThreadToken.Initialize() )
	{
		hr	=	GetDataSource( pProvider, szID, szConn, pSession);
		revertThreadToken.Restore();
	}
	else
		hr	=	E_FAIL;
	return hr;
}



double atofc(LPCSTR szCurrency)
{
    char szBuf[255];
    LPSTR szTo = szBuf;
    LPCSTR szFrom = szCurrency;
    while (*szFrom != '\0')
    {
        if (*szFrom != ',' && *szFrom != '$')
            *szTo++ = *szFrom++;
        else
            szFrom++;
    }

    szFrom = '\0';
    return atof(szBuf);
}

// Convert a double (DBDATE) style date to a string
void dbl2str(double dDate, LPSTR szDate, LPSTR szTime)
{
	SYSTEMTIME st;
    VariantTimeToSystemTime(dDate, &st);
	sprintf(szDate, "%d/%d/%d", st.wMonth, st.wDay, st.wYear);
	sprintf(szTime, "%02d:%02d:%02d", st.wHour, st.wMinute, st.wSecond);
}

// Convert a string date to a double (DBDATE) style date
double str2dbl(LPCSTR szDate, LPCSTR szTime)
{
	SYSTEMTIME st;
	int rval;

	rval = sscanf(szDate, "%hd/%hd/%hd", &st.wMonth, &st.wDay, &st.wYear);
	if (rval < 2)
		return NULL;
	else if (rval == 2)
	{
		SYSTEMTIME stCurrent;
		GetLocalTime(&stCurrent);
		st.wYear = stCurrent.wYear;
	}
	else
	{
		// Note this will fail in the year 2031
		if (st.wYear < 30)
			st.wYear+=2000;
		else if (st.wYear < 100)
			st.wYear+=1900;
	}

	char szBuf[255];
	rval = sscanf(szTime, "%hd:%hd:%hd.%hd %s", &st.wHour, &st.wMinute, &st.wSecond, &st.wMilliseconds, szBuf);
	if (rval < 2)
		return NULL;
	else if (rval == 2)
	{
		rval = sscanf(szTime, "%hd:%hd %s", &st.wHour, &st.wMinute, szBuf);
		if (rval == 3 && (szBuf[0] == 'p' || szBuf[0] == 'P'))
			st.wHour+=12;
		st.wSecond = 0;
		st.wMilliseconds = 0;
	}
	else if (rval == 3)
	{
		rval = sscanf(szTime, "%hd:%hd:%hd %s", &st.wHour, &st.wMinute, &st.wSecond, szBuf);
		if (rval == 4 && (szBuf[0] == 'p' || szBuf[0] == 'P'))
			st.wHour+=12;
		st.wMilliseconds = 0;
	}
	else if (rval == 5)
	{
		if (szBuf[0] == 'p' || szBuf[0] == 'P')
			st.wHour += 12;
	}

    double dDate;
	SystemTimeToVariantTime(&st, &dDate);

	return dDate;
}

class CBasic : public CRequestHandlerT<CBasic>
{
public:
    LPCSTR m_szUserID;

	CBasic() : m_szUserID(NULL)
	{
	}
	HTTP_CODE ValidateAndExchange()
	{
		const CCookie &c = m_HttpRequest.Cookies("userid");

		if (c.IsEmpty())
			m_szUserID = NULL;
		else
			m_szUserID = (LPCSTR)c.m_strValue;

        return HTTP_SUCCESS;
    }

	HTTP_CODE OnIsLoggedOn()
	{
		if (m_szUserID)
			return HTTP_SUCCESS;
		else 
			return HTTP_S_FALSE;
	}

	HTTP_CODE OnGetLoggedOnUserID()
	{
        if (m_szUserID)
            m_HttpResponse << m_szUserID;

		return HTTP_SUCCESS;
	}

	BEGIN_REPLACEMENT_METHOD_MAP(CBasic)
		REPLACEMENT_METHOD_ENTRY("GetLoggedOnUserID", OnGetLoggedOnUserID)
		REPLACEMENT_METHOD_ENTRY("IsLoggedOn", OnIsLoggedOn)
	END_REPLACEMENT_METHOD_MAP()
};
class CMain : public CRequestHandlerT<CMain>
{
private:
	char m_szUserName[200];
	CCommand< CAccessor<CUserInfo> > m_UserInfo;
    LPCSTR m_szUserID;

public:
	HTTP_CODE ValidateAndExchange()
	{
		const CCookie &c = m_HttpRequest.Cookies("userid");

		if (c.IsEmpty())
			return HTTP_SUCCESS;

		m_szUserID = (LPCSTR)c.m_strValue;

        CDataConnection DC;
        if (GetDataSourceSecure(m_spServiceProvider, DATASOURCE, CONNECTION_STRING, &DC) != S_OK)
		{
            m_HttpResponse.Redirect("dberror.htm");
			return HTTP_SUCCESS_NO_PROCESS;
		}

		CString strCommand;
		strCommand.Format("SELECT * FROM [User Information] WHERE (([User Information].[User ID] = %s))",
			m_szUserID);
			
		if (m_UserInfo.Open(DC, strCommand, NULL, NULL, DBGUID_DEFAULT, true) != S_OK)
		{
            m_HttpResponse.Redirect("dberror.htm");
			return HTTP_SUCCESS_NO_PROCESS;
		}

		if (m_UserInfo.MoveFirst() != S_OK)
		{
            m_HttpResponse.Redirect("dberror.htm");
			return HTTP_SUCCESS_NO_PROCESS;
		}
		
		return HTTP_SUCCESS;
	}

	HTTP_CODE OnUserName()
	{
        m_HttpResponse << m_UserInfo.m_szUserName;

		return HTTP_SUCCESS;
	}

	HTTP_CODE OnFirstName()
	{
		m_HttpResponse << m_UserInfo.m_szFirstName;

		return HTTP_SUCCESS;
	}
	
	HTTP_CODE OnLastName()
	{
		m_HttpResponse << m_UserInfo.m_szLastName;

		return HTTP_SUCCESS;
	}

	HTTP_CODE OnUserID()
	{
        m_HttpResponse << m_UserInfo.m_dwUserID;
		return HTTP_SUCCESS;
	}

	HTTP_CODE OnIsLoggedOn()
	{
		if (m_szUserID)
			return HTTP_SUCCESS;
		else 
			return HTTP_S_FALSE;
	}


	HTTP_CODE OnGetLoggedOnUserID()
	{
        m_HttpResponse << m_szUserID;

		return HTTP_SUCCESS;
	}

	BEGIN_REPLACEMENT_METHOD_MAP(CMain)
		REPLACEMENT_METHOD_ENTRY("UserName", OnUserName)
		REPLACEMENT_METHOD_ENTRY("FirstName", OnFirstName)
		REPLACEMENT_METHOD_ENTRY("LastName", OnLastName)
		REPLACEMENT_METHOD_ENTRY("GetUserID", OnUserID)
		REPLACEMENT_METHOD_ENTRY("GetLoggedOnUserID", OnGetLoggedOnUserID)
		REPLACEMENT_METHOD_ENTRY("IsLoggedOn", OnIsLoggedOn)
	END_REPLACEMENT_METHOD_MAP()
};

class CLogin : public CRequestHandlerT<CLogin>
{
protected:
	char szError[255];

public:
	HTTP_CODE ValidateAndExchange()
	{
		CValidateContext c;
		LPCSTR szUserName;
		LPCSTR szPassword;
		
		m_HttpRequest.FormVars.Exchange("username", &szUserName, &c);
		m_HttpRequest.FormVars.Exchange("password", &szPassword, &c);
        
        strcpy(szError, "");

        if (!c.ParamsOK())
        {
            return HTTP_SUCCESS;
        }

		CString strCommand;
		CCommand< CAccessor<CUserInfo> > UserInfo;

        CDataConnection DC;
        if (GetDataSourceSecure(m_spServiceProvider, DATASOURCE, CONNECTION_STRING, &DC) != S_OK)
        {
			strcpy(szError, "Failed to open connection to database");
			return HTTP_SUCCESS;
        }

		strCommand.Format("SELECT * FROM [User Information] WHERE ((([User Information].[User Name])='%s' And ([User Information].[Password])='%s'))",
			szUserName, szPassword);

		if (UserInfo.Open(DC, strCommand, NULL, NULL, DBGUID_DEFAULT, true) != S_OK)
		{
    		strcpy(szError, "Failed to get data from database");
			return HTTP_SUCCESS;
		}

		if (UserInfo.MoveFirst() != S_OK)
		{
			strcpy(szError, "Make sure you entered the correct name and password");
			return HTTP_SUCCESS;
		}

		strCommand.Format("%u", UserInfo.m_dwUserID);
		CCookie cookie;
		cookie.SetName("userid");
		cookie.SetValue(strCommand);
		cookie.SetPath("/");
		m_HttpResponse.AppendCookie(&cookie);

		m_HttpResponse.Redirect("mainmenu.srf");

        return HTTP_SUCCESS_NO_PROCESS;
	}

	HTTP_CODE OnGetLastError()
	{
        m_HttpResponse << szError;
		return HTTP_SUCCESS;
	}
	
	HTTP_CODE OnIsLoggedOn()
	{
		const CCookie &c = m_HttpRequest.Cookies("userid");

		if (c.IsEmpty())
			return HTTP_S_FALSE;
		else 
			return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetLoggedOnUserID()
	{
		LPCSTR szUID = (LPCSTR)m_HttpRequest.Cookies("userid").m_strValue;
        m_HttpResponse << szUID;

		return HTTP_SUCCESS;
	}


	BEGIN_REPLACEMENT_METHOD_MAP(CLogin)
		REPLACEMENT_METHOD_ENTRY("GetLastError", OnGetLastError)
		REPLACEMENT_METHOD_ENTRY("GetLoggedOnUserID", OnGetLoggedOnUserID)
		REPLACEMENT_METHOD_ENTRY("IsLoggedOn", OnIsLoggedOn)
	END_REPLACEMENT_METHOD_MAP()
};

BEGIN_HANDLER_MAP()
	HANDLER_ENTRY("Default", CLogin)
	HANDLER_ENTRY("Login", CLogin)
    HANDLER_ENTRY("Basic", CBasic)
	HANDLER_ENTRY("Main", CMain)
END_HANDLER_MAP()
