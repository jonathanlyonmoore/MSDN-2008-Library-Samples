// File: Items.cpp
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

class CAddItem : public CRequestHandlerT<CAddItem>
{
protected:
	char m_szError[255];
	LPCSTR m_szItemShortDesc;
	LPCSTR m_szItemLongDesc;
	LPCSTR m_szItemEndDate;
	LPCSTR m_szItemEndTime;
	LPCSTR m_szItemMinBid;
	LPCSTR m_szUserID;
	char m_szDate[255];
	char m_szTime[255];

public:

    HTTP_CODE ValidateAndExchange()
	{

		m_szItemShortDesc = m_HttpRequest.m_QueryParams.Lookup("shortdesc");
		m_szItemLongDesc = m_HttpRequest.m_QueryParams.Lookup("longdesc");
		m_szItemEndDate = m_HttpRequest.m_QueryParams.Lookup("enddate");
		m_szItemEndTime = m_HttpRequest.m_QueryParams.Lookup("endtime");
		m_szItemMinBid = m_HttpRequest.m_QueryParams.Lookup("minbid");

		if (m_szItemShortDesc == NULL &&
			m_szItemLongDesc == NULL &&
			m_szItemEndDate == NULL &&
			m_szItemEndTime == NULL &&
			m_szItemMinBid == NULL)
		{
			m_szItemShortDesc = "";
			m_szItemLongDesc = "";
			m_szItemMinBid = "0.00";
			SYSTEMTIME st;
			GetLocalTime(&st);
			sprintf(m_szDate, "%d/%d/%d", st.wMonth, st.wDay, st.wYear);
			sprintf(m_szTime, "%d:%02d", st.wHour + 1, st.wMinute);
			m_szItemEndDate = m_szDate;
			m_szItemEndTime = m_szTime;
			strcpy(m_szError, "");
			return HTTP_SUCCESS;
		}

		const CCookie &c = m_HttpRequest.Cookies("userid");

		if (c.IsEmpty())
        {
            m_HttpResponse.Redirect("login.srf");
            return HTTP_SUCCESS_NO_PROCESS;
        }

		m_szUserID = c.m_strValue;

		if (atofc(m_szItemMinBid) < 0.0)
		{
			strcpy(m_szError, "Minimum bid must be &#8805 0");
			return HTTP_SUCCESS;
		}

		double dItemEnd = str2dbl(m_szItemEndDate, m_szItemEndTime);
		char szParsedDesc[65536];

        // Escape out single quotes so as not to kill the SQL query
		LPCSTR szOrig;
		LPSTR szCopy;
		szOrig = m_szItemLongDesc;
		szCopy = szParsedDesc;
		while (*szOrig != '\0')
		{
			*szCopy = *szOrig;
			if (*szCopy == '\'')
			{
				szCopy++;
				*szCopy = '\'';
			}
			szCopy++;
			szOrig++;
		}
		*szCopy = '\0';

		CString strCommand;
		strCommand.Format(
			"INSERT INTO [Items] ([User ID], [Short Description], [Long Description], [Minimum Bid], [Ends]) VALUES (%s, '%s', '%s', %s, %lf)",
			m_szUserID, m_szItemShortDesc, szParsedDesc, m_szItemMinBid, dItemEnd);

        CDataConnection DC;
        if (GetDataSourceSecure(m_spServiceProvider, DATASOURCE, CONNECTION_STRING, &DC) != S_OK)
		{
            m_HttpResponse.Redirect("dberror.htm");
			return HTTP_SUCCESS_NO_PROCESS;
		}

		CCommand<> command;
		if (command.Open(DC, strCommand, NULL, NULL, DBGUID_DEFAULT, true) != S_OK)
		{
			strcpy(m_szError, "Make sure the data is entered correctly");
			return HTTP_SUCCESS;
		}

        strCommand.Format("userinfo.srf?userid=%s", m_szUserID);

        m_HttpResponse.Redirect(strCommand);

		return HTTP_SUCCESS_NO_PROCESS;
	}


	HTTP_CODE OnGetError()
	{	
		m_HttpResponse << m_szError;
		return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetShortDesc()
	{
		m_HttpResponse << m_szItemShortDesc;
		return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetLongDesc()
	{
		m_HttpResponse << m_szItemLongDesc;
		return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetEndDate()
	{
		m_HttpResponse << m_szItemEndDate;
		return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetEndTime()
	{
		m_HttpResponse << m_szItemEndTime;
		return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetMinBid()
	{
		m_HttpResponse << m_szItemMinBid;
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


	BEGIN_REPLACEMENT_METHOD_MAP(CAddItem)
		REPLACEMENT_METHOD_ENTRY("GetError", OnGetError)
		REPLACEMENT_METHOD_ENTRY("GetShortDesc", OnGetShortDesc)
		REPLACEMENT_METHOD_ENTRY("GetLongDesc", OnGetLongDesc)
		REPLACEMENT_METHOD_ENTRY("GetEndDate", OnGetEndDate)
		REPLACEMENT_METHOD_ENTRY("GetEndTime", OnGetEndTime)
		REPLACEMENT_METHOD_ENTRY("GetMinBid", OnGetMinBid)
		REPLACEMENT_METHOD_ENTRY("GetLoggedOnUserID", OnGetLoggedOnUserID)
		REPLACEMENT_METHOD_ENTRY("IsLoggedOn", OnIsLoggedOn)
	END_REPLACEMENT_METHOD_MAP()
};
class CEditItem : public CRequestHandlerT<CEditItem>
{
protected:
	char m_szError[255];
	LPCSTR m_szItemShortDesc;
	LPCSTR m_szItemLongDesc;
	LPCSTR m_szItemEndDate;
	LPCSTR m_szItemEndTime;
	LPCSTR m_szItemMinBid;
	LPCSTR m_szUserID;
	LPCSTR m_szItemID;
	char m_szDate[255];
	char m_szTime[255];
	CCommand< CAccessor<CItemInfo> > m_ItemInfo;

public:

	HTTP_CODE ValidateAndExchange()
    {
        strcpy(m_szError, "");

        CDataConnection DC;
        if (GetDataSourceSecure(m_spServiceProvider, DATASOURCE, CONNECTION_STRING, &DC) != S_OK)
		{
            m_HttpResponse.Redirect("dberror.htm");
			return HTTP_SUCCESS_NO_PROCESS;
		}
		m_szItemID = m_HttpRequest.m_QueryParams.Lookup("itemid");
		const CCookie &c = m_HttpRequest.Cookies("userid");

		if (c.IsEmpty())
        {
            m_HttpResponse.Redirect("login.srf");
            return HTTP_SUCCESS_NO_PROCESS;
        }
		m_szUserID = (LPCSTR)c.m_strValue;

		m_szItemShortDesc = m_HttpRequest.m_QueryParams.Lookup("shortdesc");
		m_szItemLongDesc = m_HttpRequest.m_QueryParams.Lookup("longdesc");

		CString strCommand;

		if (m_szItemShortDesc == NULL ||
			m_szItemLongDesc == NULL)
		{

			strCommand.Format("SELECT * FROM [Items] WHERE [Items].[Item ID] = %s", m_szItemID);

			if (m_ItemInfo.Open(DC, strCommand, NULL, NULL, DBGUID_DEFAULT, true) != S_OK)
			{
				strcpy(m_szError, "Unexpected error opening database");
				return HTTP_SUCCESS;
			}

			m_ItemInfo.MoveFirst();
			if (m_szUserID == NULL || m_ItemInfo.m_dwUserID != (DWORD)atoi(m_szUserID))
			{
                m_HttpResponse.Redirect("login.srf");
                return HTTP_SUCCESS_NO_PROCESS;
            }

			return HTTP_SUCCESS;
		}
		else
		{
            char szParsedDesc[65536];
            // Escape out single quotes so as not to kill the SQL query
		    LPCSTR szOrig;
		    LPSTR szCopy;
		    szOrig = m_szItemLongDesc;
		    szCopy = szParsedDesc;
		    while (*szOrig != '\0')
		    {
			    *szCopy = *szOrig;
			    if (*szCopy == '\'')
			    {
				    szCopy++;
				    *szCopy = '\'';
			    }
			    szCopy++;
			    szOrig++;
		    }
		    *szCopy = '\0';

		    CString strCommand;
		    strCommand.Format(
			    "UPDATE [Items] SET [Items].[Short Description] = '%s', [Items].[Long Description] = '%s' WHERE [Items].[Item ID] = %s",
			    m_szItemShortDesc, szParsedDesc, m_szItemID);

		    CCommand<> command;
		    if (command.Open(DC, strCommand, NULL, NULL, DBGUID_DEFAULT, true) != S_OK)
		    {
			    strcpy(m_szError, "Make sure the data is entered correctly");
			    return HTTP_SUCCESS;
		    }
            
            strCommand.Format("userinfo.srf?userid=%s", m_szUserID);
            m_HttpResponse.Redirect(strCommand);
		}

		return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetError()
	{	
		m_HttpResponse << m_szError;
		return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetShortDesc()
	{
		m_HttpResponse << m_ItemInfo.m_szShortDesc;
		return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetLongDesc()
	{
    	m_HttpResponse << m_ItemInfo.m_szLongDesc;
		return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetItemID()
	{
		m_HttpResponse << m_szItemID;
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

	BEGIN_REPLACEMENT_METHOD_MAP(CEditItem)
		REPLACEMENT_METHOD_ENTRY("GetError", OnGetError)
		REPLACEMENT_METHOD_ENTRY("GetShortDesc", OnGetShortDesc)
		REPLACEMENT_METHOD_ENTRY("GetLongDesc", OnGetLongDesc)
		REPLACEMENT_METHOD_ENTRY("GetItemID", OnGetItemID)
		REPLACEMENT_METHOD_ENTRY("GetLoggedOnUserID", OnGetLoggedOnUserID)
		REPLACEMENT_METHOD_ENTRY("IsLoggedOn", OnIsLoggedOn)
	END_REPLACEMENT_METHOD_MAP()
};
class CViewItem : public CRequestHandlerT<CViewItem>
{
private:
	CCommand< CAccessor<CBidInfoWithUser> > m_BidInfo;
	char m_szItemName[51];
	char m_szItemDesc[65536];
	char m_szItemUserName[51];
	char m_szItemUserID[20];
	char m_szItemID[51];
	bool m_bDisplayAll;

public:
    HTTP_CODE ValidateAndExchange()
	{
		strncpy(m_szItemID, m_HttpRequest.m_QueryParams.Lookup("itemid"), sizeof(m_szItemID) - 1);
		LPCSTR view = m_HttpRequest.m_QueryParams.Lookup("view");

        if (view == NULL || !strcmp(view, "short"))
			m_bDisplayAll = false;
		else
			m_bDisplayAll = true;

        CDataConnection DC;
        if (GetDataSourceSecure(m_spServiceProvider, DATASOURCE, CONNECTION_STRING, &DC) != S_OK)
		{
            m_HttpResponse.Redirect("dberror.htm");
			return HTTP_SUCCESS_NO_PROCESS;
		}

		CString strCommand;
		strCommand.Format("SELECT [Bids].*, [User Information].[User Name] FROM [Bids], [User Information] WHERE ([Bids].[Item ID]=%s And [User Information].[User ID]=[Bids].[User ID]) ORDER BY [Bids].[Bid Amount] DESC", m_szItemID);
			
		if (m_BidInfo.Open(DC, strCommand, NULL, NULL, DBGUID_DEFAULT, true) != S_OK)
		{
            m_HttpResponse.Redirect("dberror.htm");
			return HTTP_SUCCESS_NO_PROCESS;
		}

		CCommand< CAccessor<CItemInfo> > ItemInfo;
		strCommand.Format("SELECT * FROM [Items] WHERE [Items].[Item ID]=%s", m_szItemID);
		if (ItemInfo.Open(DC, strCommand, NULL, NULL, DBGUID_DEFAULT, true) != S_OK)
		{
            m_HttpResponse.Redirect("dberror.htm");
			return HTTP_SUCCESS_NO_PROCESS;
		}

		if (ItemInfo.MoveFirst() != S_OK)
		{
            m_HttpResponse.Redirect("dberror.htm");
			return HTTP_SUCCESS_NO_PROCESS;
		}
		strncpy(m_szItemName, ItemInfo.m_szShortDesc, sizeof(m_szItemName) - 1);
        strncpy(m_szItemDesc, ItemInfo.m_szLongDesc,sizeof(m_szItemDesc) - 1);

		CCommand< CAccessor<CUserInfo> > UserInfo;
		strCommand.Format("SELECT * FROM [User Information] WHERE [User Information].[User ID]=%d", ItemInfo.m_dwUserID);
		if (UserInfo.Open(DC, strCommand, NULL, NULL, DBGUID_DEFAULT, true) != S_OK)
		{
            m_HttpResponse.Redirect("dberror.htm");
			return HTTP_SUCCESS_NO_PROCESS;
		}

		if (UserInfo.MoveFirst() != S_OK)
		{
            m_HttpResponse.Redirect("dberror.htm");
			return HTTP_SUCCESS_NO_PROCESS;
		}
		strncpy(m_szItemUserName, UserInfo.m_szUserName, sizeof(m_szItemUserName) - 1);
		sprintf(m_szItemUserID, "%d", ItemInfo.m_dwUserID);

		return HTTP_SUCCESS;
	}

	HTTP_CODE OnItemName()
	{
		m_HttpResponse << m_szItemName;
		return HTTP_SUCCESS;
	}

	HTTP_CODE OnItemDesc()
	{
		m_HttpResponse << m_szItemDesc;
		return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetItemID()
	{
		m_HttpResponse << m_szItemID;
		return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetNextBid()
	{
		if (m_BidInfo.MoveNext() != S_OK)
            return HTTP_S_FALSE;
        else
            return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetBidderID()
	{
        m_HttpResponse << m_BidInfo.m_dwUserID;
		return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetBidderName()
	{
        m_HttpResponse << m_BidInfo.m_szUserName;
		return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetBidAmount()
	{
        m_HttpResponse << m_BidInfo.m_cBidAmount;
		return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetUserID()
	{
		m_HttpResponse << m_szItemUserID;
		return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetUserName()
	{
		m_HttpResponse << m_szItemUserName;
		return HTTP_SUCCESS;
	}

	HTTP_CODE OnIsFullDisplay()
	{
		if (m_bDisplayAll) 
            return HTTP_SUCCESS;
		else 
            return HTTP_S_FALSE;
	}

	HTTP_CODE OnIsNotFullDisplay()
	{
		if (m_bDisplayAll) 
            return HTTP_S_FALSE;
		else 
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

	BEGIN_REPLACEMENT_METHOD_MAP(CViewItem)
		REPLACEMENT_METHOD_ENTRY("ItemName", OnItemName)
		REPLACEMENT_METHOD_ENTRY("ItemDescription", OnItemDesc)
		REPLACEMENT_METHOD_ENTRY("GetItemID", OnGetItemID)
		REPLACEMENT_METHOD_ENTRY("GetNextBid", OnGetNextBid)
		REPLACEMENT_METHOD_ENTRY("GetBidderID", OnGetBidderID)
		REPLACEMENT_METHOD_ENTRY("GetBidderName", OnGetBidderName)
		REPLACEMENT_METHOD_ENTRY("GetBidAmount", OnGetBidAmount)
		REPLACEMENT_METHOD_ENTRY("GetUserID", OnGetUserID)
		REPLACEMENT_METHOD_ENTRY("GetUserName", OnGetUserName)
		REPLACEMENT_METHOD_ENTRY("IsFullDisplay", OnIsFullDisplay)
		REPLACEMENT_METHOD_ENTRY("IsNotFullDisplay", OnIsNotFullDisplay)
		REPLACEMENT_METHOD_ENTRY("GetLoggedOnUserID", OnGetLoggedOnUserID)
		REPLACEMENT_METHOD_ENTRY("IsLoggedOn", OnIsLoggedOn)
	END_REPLACEMENT_METHOD_MAP()
};
class CBrowse : public CRequestHandlerT<CBrowse>
{
private:
	CCommand< CAccessor<CItemInfo> > m_ItemInfo;

public:
    HTTP_CODE ValidateAndExchange()
    {
        CDataConnection DC;
        if (GetDataSourceSecure(m_spServiceProvider, DATASOURCE, CONNECTION_STRING, &DC) != S_OK)
		{
            m_HttpResponse.Redirect("dberror.htm");
			return HTTP_SUCCESS_NO_PROCESS;
		}

		CString strCommand;
		strCommand.Format("SELECT * FROM [Items]");
		if (m_ItemInfo.Open(DC, strCommand, NULL, NULL, DBGUID_DEFAULT, true) != S_OK)
		{
            m_HttpResponse.Redirect("dberror.htm");
			return HTTP_SUCCESS_NO_PROCESS;
		}

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

	HTTP_CODE OnGetMaxBid()
	{
		CString strCommand;

		strCommand.Format("SELECT Max([Bids].[Bid Amount]) FROM [Bids] GROUP BY [Bids].[Item ID] HAVING [Bids].[Item ID]=%d", m_ItemInfo.m_dwItemID);
		CCommand< CAccessor<CMaxBid> > MaxBid;
        CDataConnection DC;
        if (GetDataSourceSecure(m_spServiceProvider, DATASOURCE, CONNECTION_STRING, &DC) != S_OK)
		{
            m_HttpResponse.Redirect("dberror.htm");
			return HTTP_SUCCESS_NO_PROCESS;
		}

		if (MaxBid.Open(DC, strCommand, NULL, NULL, DBGUID_DEFAULT, true) != S_OK)
		{
			return HTTP_SUCCESS;
		}

		if (MaxBid.MoveNext() == S_OK)
		{
            m_HttpResponse << MaxBid.m_cBidAmount;
		}
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


	BEGIN_REPLACEMENT_METHOD_MAP(CBrowse)
		REPLACEMENT_METHOD_ENTRY("GetNextItem", OnGetNextItem)
		REPLACEMENT_METHOD_ENTRY("GetItemName", OnGetItemName)
		REPLACEMENT_METHOD_ENTRY("GetItemID", OnGetItemID)
		REPLACEMENT_METHOD_ENTRY("GetStartDate", OnGetStartDate)
		REPLACEMENT_METHOD_ENTRY("GetEndDate", OnGetEndDate)
		REPLACEMENT_METHOD_ENTRY("GetMaxBid", OnGetMaxBid)
		REPLACEMENT_METHOD_ENTRY("IsLoggedOn", OnIsLoggedOn)
		REPLACEMENT_METHOD_ENTRY("GetLoggedOnUserID", OnGetLoggedOnUserID)
	END_REPLACEMENT_METHOD_MAP()
};

BEGIN_HANDLER_MAP()
	HANDLER_ENTRY("Browse", CBrowse)
	HANDLER_ENTRY("ViewItem", CViewItem)
	HANDLER_ENTRY("AddItem", CAddItem)
	HANDLER_ENTRY("EditItem", CEditItem)
END_HANDLER_MAP()
