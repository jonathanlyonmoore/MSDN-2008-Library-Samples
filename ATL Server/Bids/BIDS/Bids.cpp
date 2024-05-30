// Bids.cpp : Defines the entry point for the DLL application.
//
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

BOOL APIENTRY DllMain(HANDLE, DWORD, LPVOID)
{
    return TRUE;
}

#ifdef _DEBUG
CDebugReportHook g_ReportHook;
#endif


class CAddBid : public CRequestHandlerT<CAddBid>
{
protected:
	char m_szError[255];
	CURRENCY m_cMaxBid;
	CURRENCY m_cMinNextBid;
	char m_szItemID[255];
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

		const CCookie &c = m_HttpRequest.Cookies("userid");

		if (c.IsEmpty())
        {
            m_HttpResponse.Redirect("login.srf");
            return HTTP_SUCCESS_NO_PROCESS;
        }

        LPCSTR szUserID = c.m_strValue;
		
        LPCSTR szItemID = m_HttpRequest.m_QueryParams.Lookup("itemid");
		strncpy(m_szItemID, szItemID, sizeof(m_szItemID) - 1);

		strCommand.Format("SELECT Max([Bids].[Bid Amount]) FROM [Bids] GROUP BY [Bids].[Item ID] HAVING [Bids].[Item ID]=%s", szItemID);
		CCommand< CAccessor<CMaxBid> > MaxBid;
		if (MaxBid.Open(DC, strCommand, NULL, NULL, DBGUID_DEFAULT, true) != S_OK)
		{
			strcpy(m_szError, "Error accessing database");
			return HTTP_SUCCESS;
		}

		if (MaxBid.MoveNext() != S_OK)
		{
			CCommand< CAccessor<CItemInfo> > ItemInfo;
			strCommand.Format("SELECT * FROM [Items] WHERE [Items].[Item ID]=%s", szItemID);
			if (ItemInfo.Open(DC, strCommand, NULL, NULL, DBGUID_DEFAULT, true) != S_OK)
			{
				strcpy(m_szError, "Error accessing database");
				return HTTP_SUCCESS;
			}
			if (ItemInfo.MoveFirst() != S_OK)
			{
				strcpy(m_szError, "Error accessing database");
				return HTTP_SUCCESS;
			}
			m_cMaxBid.int64 = 0;
            if (ItemInfo.m_cMinBid.int64 == 0)
                m_cMinNextBid.int64 = 20000;
            else
			    m_cMinNextBid = ItemInfo.m_cMinBid;
		}
		else
		{
			m_cMaxBid = MaxBid.m_cBidAmount;
			m_cMinNextBid.int64 = m_cMaxBid.int64 + 20000;
		}

		LPCSTR szBidAmount = m_HttpRequest.m_QueryParams.Lookup("bidamount");
		if (szBidAmount == NULL)
		{
			strcpy(m_szError, "");
			return HTTP_SUCCESS;
		}

		CURRENCY cBidAmount;
        LPCSTR szMoney = strchr(szBidAmount, '$');
        double dBidAmount;
        if (szMoney)
            dBidAmount = atofc(szMoney+1);
        else
            dBidAmount = atofc(szBidAmount);

	    cBidAmount.int64 = (__int64)(10000.0 * dBidAmount);
		if (cBidAmount.int64 < m_cMinNextBid.int64)
		{
			strcpy(m_szError, "Bid is too low");
			return HTTP_SUCCESS;
		}

		strCommand.Format(
			"INSERT INTO [Bids] ([User ID], [Item ID], [Bid Amount]) VALUES (%s, %s, %lf)",
			szUserID, szItemID, (double)cBidAmount.int64 / 10000.0);

		CCommand<> command;
		if (command.Open(DC, strCommand, NULL, NULL, DBGUID_DEFAULT, true) != S_OK)
		{
			strcpy(m_szError, "Make sure the data is entered correctly");
			return HTTP_SUCCESS;
		}

		strCommand.Format("iteminfo.srf?itemid=%s", m_szItemID);
        m_HttpResponse.Redirect(strCommand);

		return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetError()
	{	
		m_HttpResponse << m_szError;
		return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetMaxBid()
	{
        m_HttpResponse << m_cMaxBid;

        return HTTP_SUCCESS;
	}

	HTTP_CODE OnGetMinNextBid()
	{
        m_HttpResponse << m_cMinNextBid;

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


	BEGIN_REPLACEMENT_METHOD_MAP(CAddBid)
		REPLACEMENT_METHOD_ENTRY("GetError", OnGetError)
		REPLACEMENT_METHOD_ENTRY("GetMaxBid", OnGetMaxBid)
		REPLACEMENT_METHOD_ENTRY("GetMinNextBid", OnGetMinNextBid)
		REPLACEMENT_METHOD_ENTRY("GetItemID", OnGetItemID)
		REPLACEMENT_METHOD_ENTRY("GetLoggedOnUserID", OnGetLoggedOnUserID)
		REPLACEMENT_METHOD_ENTRY("IsLoggedOn", OnIsLoggedOn)
	END_REPLACEMENT_METHOD_MAP()
};

BEGIN_HANDLER_MAP()
	HANDLER_ENTRY("AddBid", CAddBid)
END_HANDLER_MAP()
