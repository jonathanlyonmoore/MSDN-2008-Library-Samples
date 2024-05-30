// ProfileDB.h : Defines the database classes for the Outbox
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#pragma once

[ 
	db_command("SELECT [FirstName], [LastName], [Email] FROM UserInfoTable WHERE [UserID]=?")
]
class CUserProfile
{
public:
	[ db_column(ordinal="1") ] TCHAR m_szFirstName[DB_MAX_STRLEN + 1];
	[ db_column(ordinal="2") ] TCHAR m_szLastName[DB_MAX_STRLEN + 1];
	[ db_column(ordinal="3") ] TCHAR m_szEmail[DB_MAX_STRLEN + 1];
	[ db_param("1") ] LONG m_lUserID;
};

[
	db_command("UPDATE UserInfoTable SET [FirstName]=?, [LastName]=?, [Email]=? WHERE [UserID]=?")
]
class CUpdateProfile
{
public:
	[ db_param("1") ] TCHAR m_szFirstName[DB_MAX_STRLEN + 1];
	[ db_param("2") ] TCHAR m_szLastName[DB_MAX_STRLEN + 1];
	[ db_param("3") ] TCHAR m_szEmail[DB_MAX_STRLEN + 1];
	[ db_param("4") ] LONG m_lUserID;
};

[
	db_command("UPDATE UserInfoTable SET [Password]=?, [Salt]=? WHERE [UserID]=?")
]
class CUpdatePassword
{
public:
	[ db_param(ordinal="1", length="m_lPassLen") ] BYTE m_Password[DB_MAX_STRLEN + 1];
	[ db_param(ordinal="2", length="m_lSaltLen") ] BYTE m_Salt[DB_MAX_STRLEN + 1];
	[ db_param("3") ]					   LONG m_lUserID;

	DBLENGTH m_lSaltLen;
	DBLENGTH m_lPassLen;
};