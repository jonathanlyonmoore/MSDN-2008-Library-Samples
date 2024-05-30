// LoginDB.h : Defines the database classes for the login handler
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#pragma once
[
	db_command("SELECT [Password], [Salt], [FirstName], [LastName], [UserID], [RememberID] FROM UserInfoTable WHERE [Login]=? AND [Confirmed]=Yes")
]
class CLoginUser
{
public:
	[ db_column(ordinal="1", length="m_lPassLen") ] BYTE m_Password[DB_MAX_STRLEN+1];
	[ db_column(ordinal="2", length="m_lSaltLen") ] BYTE m_Salt[DB_MAX_STRLEN+1];
	[ db_column(ordinal="3") ] TCHAR m_szFirstName[DB_MAX_STRLEN + 1];
	[ db_column(ordinal="4") ] TCHAR m_szLastName[DB_MAX_STRLEN + 1];
	[ db_column(ordinal="5") ] LONG m_lUserID;
	[ db_column(ordinal="6") ] GUID m_guidRememberID;
	[ db_param("1") ]  TCHAR m_szLogin[DB_MAX_STRLEN + 1];

	DBLENGTH m_lPassLen;
	DBLENGTH m_lSaltLen;
};

[ db_command("UPDATE UserInfoTable SET [RememberPassword]=?, [RememberID]=? WHERE [Login]=?") ]
class CUpdateRememberPass
{
public:
	[ db_param("1") ] VARIANT_BOOL m_bRememberPassword;
	[ db_param("2") ] GUID m_guidRememberID;
	[ db_param("3") ] TCHAR m_szLogin[DB_MAX_STRLEN + 1];
};

[
	db_command("SELECT [FirstName], [LastName], [Login] FROM UserInfoTable WHERE [UserID]=?")
]
class CUserSessionData
{
public:
	[ db_column(ordinal="1") ] TCHAR m_szFirstName[DB_MAX_STRLEN + 1];
	[ db_column(ordinal="2") ] TCHAR m_szLastName[DB_MAX_STRLEN + 1];
	[ db_column(ordinal="3") ] TCHAR m_szLogin[DB_MAX_STRLEN + 1];
	[ db_param("1") ] LONG m_lUserID;
	
};

[
	db_command("SELECT [RememberPassword] FROM UserInfoTable WHERE [Login]=? AND [RememberID]=?")
]
class CRememberPassword
{
public:
	[ db_column(ordinal="1") ] VARIANT_BOOL m_bRememberPassword;
	[ db_param("1") ] TCHAR m_szLogin[DB_MAX_STRLEN + 1];
	[ db_param("2") ] GUID m_guidRememberID;
};
