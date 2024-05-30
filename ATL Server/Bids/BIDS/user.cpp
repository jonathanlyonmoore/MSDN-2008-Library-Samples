// File: user.cpp
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

class CAddUser : public CRequestHandlerT<CAddUser>
{
protected:
	char m_szError[255];
	LPCSTR m_szFirstName;
	LPCSTR m_szLastName;
	LPCSTR m_szUserName;
	LPCSTR m_szEmail;
	LPCSTR m_szPassword;
	LPCSTR m_szCPassword;

public:

    HTTP_CODE ValidateAndExchange()
	{
        CDataConnection DC;
        if (GetDataSourceSecure(m_spServiceProvider, DATASOURCE, CONNECTION_STRING, &DC) != S_OK)
		{
            m_HttpResponse.Redirect("dberror.htm");
			return HTTP_SUCCESS_NO_PROCESS;
		}

		m_szFirstName = m_HttpRequest.FormVars.Lookup("firstname");
		m_szLastName = m_HttpRequest.FormVars.Lookup("lastname");
		m_szUserName = m_HttpRequest.FormVars.Lookup("username");
		m_szEmail = m_HttpRequest.FormVars.Lookup("email");
		m_szPassword = m_HttpRequest.FormVars.Lookup("password");
		m_szCPassword = m_HttpRequest.FormVars.Lookup("cpassword");

		if (m_szFirstName == NULL ||
			m_szLastName == NULL ||
			m_szUserName == NULL ||
			m_szEmail == NULL ||
			m_szPassword == NULL ||
			m_szCPassword == NULL)
		{
			strcpy(m_szError, "");
			return HTTP_SUCCESS;
		}

		CString strCommand;
		strCommand.Format("SELECT * FROM [User Information] WHERE [User Information].[User Name]='%s'", m_szLastName);
		CCommand< CAccessor<CUserInfo> > UserInfo;

		if (UserInfo.Open(DC, strCommand, NULL, NULL, DBGUID_DEFAULT, true) != S_OK)
		{
			strcpy(m_szError, "Error in database");
			return HTTP_SUCCESS;
		}
		if (UserInfo.MoveFirst() == S_OK)
		{
			strcpy(m_szError, "Sorry, that name is already taken.  Please try another");
			return HTTP_SUCCESS;
		}

		if (strcmp(m_szPassword, m_szCPassword))
		{
			strcpy(m_szError, "Passwords don't match.  Try again");
			return HTTP_SUCCESS;
		}

		strCommand.Format(
			"INSERT INTO [User Information] ([First Name], [Last Name], [User Name], [Email], [Password]) VALUES ('%s', '%s', '%s', '%s', '%s')",
			m_szFirstName, m_szLastName, m_szUserName, m_szEmail, m_szPassword);
		CCommand<> command;
		if (command.Open(DC, strCommand, NULL, NULL, DBGUID_DEFAULT, true) != S_OK)
		{
			strcpy(m_szError, "Make sure the data is entered correctly");
			return HTTP_SUCCESS;
		}

        m_HttpResponse.Redirect("success.htm");

		return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetError()
	{	
        m_HttpResponse << m_szError;
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

    HTTP_CODE OnGetUserName()
    {
        if (m_szUserName)
            m_HttpResponse << m_szUserName;

        return HTTP_SUCCESS;
    }

    HTTP_CODE OnGetLastName()
    {
        if (m_szLastName)
            m_HttpResponse << m_szLastName;

        return HTTP_SUCCESS;
    }
    HTTP_CODE OnGetFirstName()
    {
        if (m_szFirstName)
            m_HttpResponse << m_szFirstName;

        return HTTP_SUCCESS;
    }

    HTTP_CODE OnGetEmail()
    {
        if (m_szEmail)
            m_HttpResponse << m_szEmail;

        return HTTP_SUCCESS;
    }

    BEGIN_REPLACEMENT_METHOD_MAP(CAddUser)
		REPLACEMENT_METHOD_ENTRY("GetError", OnGetError)
		REPLACEMENT_METHOD_ENTRY("GetLoggedOnUserID", OnGetLoggedOnUserID)
		REPLACEMENT_METHOD_ENTRY("IsLoggedOn", OnIsLoggedOn)
        REPLACEMENT_METHOD_ENTRY("GetFirstName", OnGetFirstName)
        REPLACEMENT_METHOD_ENTRY("GetLastName", OnGetLastName)
        REPLACEMENT_METHOD_ENTRY("GetUserName", OnGetUserName)
        REPLACEMENT_METHOD_ENTRY("GetEmail", OnGetEmail)
	END_REPLACEMENT_METHOD_MAP()
};

class CViewUser : public CRequestHandlerT<CViewUser>
{
private:
	CCommand< CAccessor<CItemInfo> > m_ItemInfo;
	LPCSTR m_szLoggedOnUserID;
	LPCSTR m_szLookedUpUserID;
	char m_szUserName[51];

public:

    HTTP_CODE ValidateAndExchange()
	{
		m_szLoggedOnUserID = (LPCSTR)m_HttpRequest.Cookies("userid").m_strValue;
		m_szLookedUpUserID = m_HttpRequest.m_QueryParams.Lookup("userid");

        CDataConnection DC;
        if (GetDataSourceSecure(m_spServiceProvider, DATASOURCE, CONNECTION_STRING, &DC) != S_OK)
		{
            m_HttpResponse.Redirect("dberror.htm");
			return HTTP_SUCCESS_NO_PROCESS;
		}

		CString strCommand;
		strCommand.Format("SELECT * FROM [Items] WHERE [Items].[User ID]=%s", m_szLookedUpUserID);
		if (m_ItemInfo.Open(DC, strCommand, NULL, NULL, DBGUID_DEFAULT, true) != S_OK)
		{
            m_HttpResponse.Redirect("dberror.htm");
			return HTTP_SUCCESS_NO_PROCESS;
		}

		CCommand< CAccessor<CUserInfo> > UserInfo;
		strCommand.Format("SELECT * FROM [User Information] WHERE [User Information].[User ID]=%s", m_szLookedUpUserID);
		if (UserInfo.Open(DC, strCommand, NULL, NULL, DBGUID_DEFAULT, true) != S_OK)
		{
            m_HttpResponse.Redirect("dberror.htm");
			return HTTP_SUCCESS_NO_PROCESS;
		}

		if (UserInfo.MoveFirst() != S_OK)
		{
            m_HttpResponse.Redirect("usererror.htm");
			return HTTP_SUCCESS_NO_PROCESS;
		}

		strncpy(m_szUserName, UserInfo.m_szUserName, sizeof(m_szUserName) - 1);

		return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetNextItem()
	{
        if (m_ItemInfo.MoveNext() != S_OK)
		    return HTTP_S_FALSE;
        else
            return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetItemName()
	{
		m_HttpResponse << m_ItemInfo.m_szShortDesc;

		return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetItemID()
	{
        m_HttpResponse << m_ItemInfo.m_dwItemID;

		return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetStartDate()
	{
		char szDate[255];
		char szTime[255];
		dbl2str(m_ItemInfo.m_dStartDate, szDate, szTime);
        m_HttpResponse << szDate << ", " << szTime;

		return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetEndDate()
	{
		char szDate[255];
		char szTime[255];
		dbl2str(m_ItemInfo.m_dEndDate, szDate, szTime);
        m_HttpResponse << szDate << ", " << szTime;
		return HTTP_SUCCESS;
	}

	HTTP_CODE OnCurrentUser()
	{
		if (!strcmp(m_szLookedUpUserID, m_szLoggedOnUserID))
			return HTTP_SUCCESS;
		else
			return HTTP_S_FALSE;
	}

	HTTP_CODE OnGetUserName()
	{
        m_HttpResponse << m_szUserName;

		return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetMaxBid()
	{
		CString strCommand;

		strCommand.Format("SELECT Max([Bids].[Bid Amount]) FROM [Bids] GROUP BY [Bids].[Item ID] HAVING [Bids].[Item ID]=%d", m_ItemInfo.m_dwItemID);
		CCommand< CAccessor<CMaxBid> > MaxBid;
        CDataConnection DC;

        if (GetDataSourceSecure(m_spServiceProvider, DATASOURCE, CONNECTION_STRING, &DC) != S_OK)
		{
			return HTTP_SUCCESS;
		}

		if (MaxBid.Open(DC, strCommand, NULL, NULL, DBGUID_DEFAULT, true) != S_OK)
		{
			return HTTP_SUCCESS;
		}

		if (MaxBid.MoveNext() == S_OK)
            m_HttpResponse << MaxBid.m_cBidAmount;
		else
        {
            CURRENCY c;
            c.int64 = 0;
            m_HttpResponse << c;
        }

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


	BEGIN_REPLACEMENT_METHOD_MAP(CViewUser)
		REPLACEMENT_METHOD_ENTRY("GetNextItem", OnGetNextItem)
		REPLACEMENT_METHOD_ENTRY("GetItemName", OnGetItemName)
		REPLACEMENT_METHOD_ENTRY("GetItemID", OnGetItemID)
		REPLACEMENT_METHOD_ENTRY("GetStartDate", OnGetStartDate)
		REPLACEMENT_METHOD_ENTRY("GetEndDate", OnGetEndDate)
		REPLACEMENT_METHOD_ENTRY("GetUserName", OnGetUserName)
		REPLACEMENT_METHOD_ENTRY("CurrentUser", OnCurrentUser)
		REPLACEMENT_METHOD_ENTRY("GetMaxBid", OnGetMaxBid)
		REPLACEMENT_METHOD_ENTRY("GetLoggedOnUserID", OnGetLoggedOnUserID)
		REPLACEMENT_METHOD_ENTRY("IsLoggedOn", OnIsLoggedOn)
	END_REPLACEMENT_METHOD_MAP()
};

BEGIN_HANDLER_MAP()
	HANDLER_ENTRY("ViewUser", CViewUser)
	HANDLER_ENTRY("AddUser", CAddUser)
END_HANDLER_MAP()
