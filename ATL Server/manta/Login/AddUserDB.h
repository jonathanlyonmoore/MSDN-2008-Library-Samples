// AddUserDB.h : Defines the database classes for the AddUser handler
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#pragma once

[ 
	db_command("INSERT INTO UserInfoTable ([FirstName], [LastName], [Email], [Login], [Hint], [Password], [Salt], [RememberPassword], [RememberID]) VALUES (?,?,?,?,?,?,?,?,?)")
]
class CAddUser
{
public:
	[ db_param(ordinal="1") ]												TCHAR					m_szFirstName[DB_MAX_STRLEN + 1];
	[ db_param(ordinal="2") ]												TCHAR					m_szLastName[DB_MAX_STRLEN + 1];
	[ db_param(ordinal="3") ]												TCHAR					m_szEmail[DB_MAX_STRLEN + 1];
	[ db_param(ordinal="4") ]												TCHAR					m_szLogin[DB_MAX_STRLEN + 1];
	[ db_param(ordinal="5") ]												TCHAR					m_szHint[DB_MAX_STRLEN + 1];
	[ db_param(ordinal="6", length="m_lPassLen") ]	BYTE					m_Password[DB_MAX_STRLEN+1];
	[ db_param(ordinal="7", length="m_lSaltLen") ]	BYTE					m_Salt[DB_MAX_STRLEN+1];
	[ db_param(ordinal="8") ]												VARIANT_BOOL	m_bRememberPassword;
	[ db_param(ordinal="9") ]												GUID					m_guidRememberID;

	DBLENGTH m_lSaltLen;
	DBLENGTH m_lPassLen;
};

[ 
	db_command("UPDATE UserInfoTable SET [Confirmed]=Yes WHERE [Login]=?")
]
class CConfirmUser
{
public:
	[ db_param(ordinal="1") ] TCHAR m_szLogin[DB_MAX_STRLEN + 1];
};

[ 
	db_command("SELECT [UserID] FROM UserInfoTable WHERE [Login]=?")
]
class CFindUserID
{
public:
	[ db_column(ordinal="1") ]	LONG m_lUserID;
	[ db_param(ordinal="1") ]		TCHAR m_szLogin[DB_MAX_STRLEN + 1];
};
