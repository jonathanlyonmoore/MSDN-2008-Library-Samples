// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#if !defined(AFX_STDAFX_H__DE3585C2_A19B_4CE8_9EC7_12E89F6DC62E__INCLUDED_)
#define AFX_STDAFX_H__DE3585C2_A19B_4CE8_9EC7_12E89F6DC62E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Insert your headers here
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _WIN32_WINNT 0x403 
#define DATASOURCE "C:\\inetpub\\wwwroot\bids\\BidInfo.mdb"
#define OLE_DATASOURCE OLESTR("C:\\inetpub\\wwwroot\\bids\\BidInfo.mdb")
#define CONNECTION_STRING OLESTR("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=") OLE_DATASOURCE OLESTR(";Persist Security Info=False")
#include <windows.h>
#include <atlbase.h>
#include <atldbcli.h>
#include <atlisapi.h>
#include <atlstencil.h>

// This executes GetDataSource in the security context of the inetinfo.exe, therefore with access rights
// that the anonymous use cannot have. So, a connection obtained this way will allow the anonymous web user to 
// write to an Access DB . The implementation of this method is in Main.cpp
HRESULT ATL_NOINLINE GetDataSourceSecure(IServiceProvider *pProvider, 
										  LPCTSTR szID, LPCOLESTR szConn, 
										  CDataConnection *pSession);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__DE3585C2_A19B_4CE8_9EC7_12E89F6DC62E__INCLUDED_)
