// File: BidsDB.h
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

class CUserInfo
{
public:

	DWORD m_dwUserID;
	char m_szFirstName[51];
	char m_szLastName[51];
	char m_szUserName[51];
	char m_szPassword[51];
	char m_szEmail[51];

	BEGIN_COLUMN_MAP(CUserInfo)
		COLUMN_ENTRY(1, m_dwUserID)
		COLUMN_ENTRY(2, m_szFirstName)
		COLUMN_ENTRY(3, m_szLastName)
		COLUMN_ENTRY(4, m_szUserName)
		COLUMN_ENTRY(5, m_szPassword)
		COLUMN_ENTRY(6, m_szEmail)
    END_COLUMN_MAP()
};

class CItemInfo
{
public:
	DWORD m_dwItemID;
	DWORD m_dwUserID;
	char m_szShortDesc[51];
	char m_szLongDesc[65535];
//	ISequentialStream * m_pLongDesc;
	CURRENCY m_cMinBid;
	double m_dStartDate;
	double m_dEndDate;
	SHORT m_sClosed;

	BEGIN_COLUMN_MAP(CItemInfo)
		COLUMN_ENTRY(1, m_dwItemID)
		COLUMN_ENTRY(2, m_dwUserID)
		COLUMN_ENTRY(3, m_szShortDesc)
		COLUMN_ENTRY(4, m_szLongDesc)
//		BLOB_ENTRY(4, IID_ISequentialStream, STGM_READ, m_pLongDesc)
		COLUMN_ENTRY_TYPE(5, DBTYPE_CY, m_cMinBid)
		COLUMN_ENTRY_TYPE(6, DBTYPE_DATE, m_dStartDate)
		COLUMN_ENTRY_TYPE(7, DBTYPE_DATE, m_dEndDate)
		COLUMN_ENTRY_TYPE(8, DBTYPE_BOOL, m_sClosed)
	END_COLUMN_MAP()
};

class CBidInfo
{
public:
	DWORD m_dwBidID;
	DWORD m_dwItemID;
	DWORD m_dwUserID;
	CURRENCY m_cBidAmount;

	BEGIN_COLUMN_MAP(CBidInfo)
		COLUMN_ENTRY(1, m_dwBidID)
		COLUMN_ENTRY(2, m_dwItemID)
		COLUMN_ENTRY(3, m_dwUserID)
		COLUMN_ENTRY_TYPE(4, DBTYPE_CY, m_cBidAmount)
	END_COLUMN_MAP()
};

class CBidInfoWithUser
{
public:
	DWORD m_dwBidID;
	DWORD m_dwItemID;
	DWORD m_dwUserID;
	CURRENCY m_cBidAmount;
	char m_szUserName[51];

	BEGIN_COLUMN_MAP(CBidInfoWithUser)
		COLUMN_ENTRY(1, m_dwBidID)
		COLUMN_ENTRY(2, m_dwItemID)
		COLUMN_ENTRY(3, m_dwUserID)
		COLUMN_ENTRY_TYPE(4, DBTYPE_CY, m_cBidAmount)
		COLUMN_ENTRY(5, m_szUserName)
	END_COLUMN_MAP()
};


class CBidInfoWithHighBidder
{
public:
	DWORD m_dwBidID;
	DWORD m_dwItemID;
	DWORD m_dwUserID;
	CURRENCY m_cBidAmount;
    DWORD m_dwMaxBidUserID;

	BEGIN_COLUMN_MAP(CBidInfoWithUser)
		COLUMN_ENTRY(1, m_dwBidID)
		COLUMN_ENTRY(2, m_dwItemID)
		COLUMN_ENTRY(3, m_dwUserID)
		COLUMN_ENTRY_TYPE(4, DBTYPE_CY, m_cBidAmount)
        COLUMN_ENTRY(5, m_dwUserID)
	END_COLUMN_MAP()
};

class CMaxBid
{
public:
	CURRENCY m_cBidAmount;

	BEGIN_COLUMN_MAP(CMaxBid)
		COLUMN_ENTRY_TYPE(1, DBTYPE_CY, m_cBidAmount)
	END_COLUMN_MAP()
};
