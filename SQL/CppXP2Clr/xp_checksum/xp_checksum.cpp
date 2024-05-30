/***********************************************************************
Copyright (c) Microsoft Corporation
All Rights Reserved.
***********************************************************************/
// This is an example of an extended procedure DLL built with Open Data
// Services. The functions within the DLL can be invoked by using the extended
// stored procedures support in SQL Server.  To register the functions 
// and allow all users to use them run the ISQL script XP_DBLIB.SQL.
//
// For further information on Open Data Services refer to the Microsoft Open 
// Data Services Programmer's Reference.
//
// The extended procedure implemented in this DLL is:
//
// XP_CHECKSUM -- Used to show the creation of a new connection to SQL
// Server using DBLIB that is bound to the initial client connection; retrieves 
// all rows from the table <szTable>, which is passed as an input parameter				
#include <windows.h>
#include "srv.h"

#include <stdio.h>
#include <initguid.h>  // Include only once in your application
#include "adoid.h"     // ADO GUID's
#include "adoint.h"    // ADO Classes, enums, etc.

// Miscellaneous defines
#define XP_NOERROR                      0
#define XP_ERROR                        1
#define MAX_BINDTOKEN                   256

// Defined in odbcss.h.
#ifndef SQL_MAX_SQLSERVERNAME
#define SQL_MAX_SQLSERVERNAME           128
#endif

// Extended procedure error codes
#define REMOTE_FAIL             4002
#define SRV_MAXERROR            50000
#define GETTABLE_ERROR          SRV_MAXERROR + 1
#define GETTABLE_MSG            SRV_MAXERROR + 2

#define STRCONVERT_MAXLEN		500

unsigned short checksum(const wchar_t *buffer, int length);
OLECHAR* ConvertToUnicode(char FAR* szA);
wchar_t *CrackStrVariant(VARIANT &var, wchar_t *pszRet);
void PrintErrorMsg(SRV_PROC *srvproc, char *szMessage);

// It is highly recommended that all Microsoft® SQL Server (7.0 
// and greater) extended stored procedure DLLs implement and export 
// __GetXpVersion. For more information see SQL Server 
// Books Online
ULONG __GetXpVersion()
{
    return ODS_VERSION;
}

// XP_CHECKSUM
//    Returns the checksum value from SQL statement
//		select * from <table>
//
// Parameters:
//    srvproc - the handle to the client connection that got the SRV_CONNECT.
//
// Returns:
//    XP_NOERROR
//    XP_ERROR
//
// Side Effects:
//    Returns messages and/or a result set to client. 
RETCODE xp_checksum(SRV_PROC *srvproc)
{
    int         bImpersonated;
    // Data from the srvproc.
	DBCHAR			szTable[SQL_MAX_SQLSERVERNAME * 3];
    long			nCols;          // Data columns returned     
    int				nCol;           // Index variable            
    DBINT			nRowsFetched;   // Number of rows sent       
	char			szConnectionString[STRCONVERT_MAXLEN];
	// Data from ADO
	HRESULT         hr  = S_OK;
	ADORecordset*   Rs = NULL;
    VARIANT         Source;
    VARIANT         Connect;
    VARIANT_BOOL    bEOF = VARIANT_FALSE;
    ADOFields*      Flds = NULL;
    ADOField*       Fld  = NULL;
	ADO_LONGPTR		FieldLength;
	VARIANT			index;
	VARIANT			v;
	char 			str[STRCONVERT_MAXLEN*4];
// 
// Check number of parameters
//
    if ( srv_rpcparams(srvproc) != 1) 
	{
        // Send error message and return.
        srv_sendmsg(srvproc, SRV_MSG_ERROR, GETTABLE_ERROR,
            SRV_INFO, (DBTINYINT) 0,
            NULL, 0, 0,
            "Error executing extended stored procedure: Invalid number of parameters.",
            SRV_NULLTERM);

        // A SRV_DONE_MORE instead of a SRV_DONE_FINAL must complete the
        // result set of an Extended Stored Procedure.
        srv_senddone(srvproc, (SRV_DONE_ERROR | SRV_DONE_MORE), 0, 0);
        return(XP_ERROR);
	}
// 
// Check parameter for correct type
//
    if (srv_paramtype(srvproc, 1) != SRVVARCHAR) 
	{
        // Send error message and return.
        srv_sendmsg(srvproc, SRV_MSG_ERROR, GETTABLE_ERROR,
            SRV_INFO, (DBTINYINT) 0,
            NULL, 0, 0,
            "Error executing extended stored procedure: Invalid parameter type.",
            SRV_NULLTERM);
        // A SRV_DONE_MORE instead of a SRV_DONE_FINAL must complete the
        // result set of an Extended Stored Procedure.
        srv_senddone(srvproc, (SRV_DONE_ERROR | SRV_DONE_MORE), 0, 0);
        return(XP_ERROR);
	}
// 
// Terminate parameter string with NULL
//
    memcpy_s(szTable, srv_paramlen(srvproc, 1), srv_paramdata(srvproc, 1), srv_paramlen(srvproc, 1));
    szTable[srv_paramlen(srvproc, 1)] = '\0';
// 
// Check for integrated security and create connection string
//
    if (strcmp(srv_pfield(srvproc, SRV_LSECURE, NULL), "TRUE") == 0) 
	{
        // Client has accessed using some form of integrated security
        // Impersonate client and set DBSETLSECURE flag
        bImpersonated = srv_impersonate_client( srvproc );
		sprintf_s(szConnectionString, STRCONVERT_MAXLEN, "Provider=SQLOLEDB.1;Integrated Security=SSPI;Data Source=%s;",
				srv_pfield(srvproc, SRV_HOST, NULL));
	}
    else 
	{
        // Set the user name, password, and application name for the remote
		sprintf_s(szConnectionString, STRCONVERT_MAXLEN, "Provider=SQLOLEDB.1;Data Source=%s;User ID=%s;Password=%s;",
				srv_pfield(srvproc, SRV_HOST, NULL),
				srv_pfield(srvproc, SRV_USER, NULL),
				srv_pfield(srvproc, SRV_PWD, NULL)); 
	}

	// initialize variants
	CoInitialize(NULL);
    VariantInit( &index );
    VariantInit( &v );

	VariantInit( &Source  );
    VariantInit( &Connect );

    Source.vt = VT_BSTR;
	sprintf_s(str, STRCONVERT_MAXLEN*4, "SELECT * FROM %s", szTable);
	Source.bstrVal = SysAllocString(ConvertToUnicode(str));

	Connect.vt = VT_BSTR;	
	Connect.bstrVal = SysAllocString(ConvertToUnicode(szConnectionString));

	//get instance of ADORecordset
    hr = CoCreateInstance( CLSID_CADORecordset, NULL, CLSCTX_INPROC_SERVER, 
						IID_IADORecordset, (LPVOID *) &Rs );
	if (!SUCCEEDED(hr))
	{
		PrintErrorMsg(srvproc, "Error executing extended stored procedure: can't create ADORecordset");
		return(XP_ERROR);
	}
 
	// open connection
	hr = Rs->Open(Source, Connect, adOpenForwardOnly, adLockReadOnly, -1);
	if (!SUCCEEDED(hr))
	{
		PrintErrorMsg(srvproc, "Error executing extended stored procedure: can't open ADORecordset");
		return(XP_ERROR);	
	}
  
	// define output table
	srv_describe(srvproc, 1, "CheckSum", SRV_NULLTERM, (DBINT)SRVINT4,
            4, (DBINT)SRVINT4, 4, (BYTE *)NULL);

	// While not on EOF...
	if( SUCCEEDED ( hr ) ) hr = Rs->get_EOF(&bEOF);
	if( SUCCEEDED ( hr ) ) hr = Rs->get_Fields(&Flds);
	if( SUCCEEDED ( hr ) ) hr = Flds->get_Count(&nCols);
	index.vt = VT_I4;

	wchar_t szBuffer[STRCONVERT_MAXLEN];
	int CheckSumValue = 0;
	nRowsFetched = 0;
	while (SUCCEEDED(hr) && bEOF == VARIANT_FALSE)
	{
		// for each row, add up checksum value
		for (nCol = 0; nCol < nCols; nCol++)
		{
			index.lVal = (long )nCol;
			hr = Flds->get_Item(index, &Fld);
			hr = Fld->get_Value(&v);
			hr = Fld->get_ActualSize(&FieldLength);

			CrackStrVariant(v, szBuffer);
			CheckSumValue += checksum(szBuffer, FieldLength);
		}
		nRowsFetched++;

        // Move to next record
        if( SUCCEEDED( hr ) ) hr = Rs->MoveNext();
        if( SUCCEEDED( hr ) ) hr = Rs->get_EOF( &bEOF );
	}

	// clean up
	if( SUCCEEDED( hr ) ) hr = Rs->Close();
	if( SUCCEEDED( hr ) ) { Rs->Release();  Rs = NULL; }
	SysFreeString(Source.bstrVal);
	SysFreeString(Connect.bstrVal);
	CoUninitialize();

	// Set the column's length
	srv_setcollen(srvproc, 1, 4);

	// Set the column's data
	CheckSumValue &= 0xFFFF;
	srv_setcoldata(srvproc, 1, &CheckSumValue);

	// Send a row to client
	srv_sendrow(srvproc);
	srv_senddone(srvproc, (SRV_DONE_COUNT | SRV_DONE_MORE), 0, 1);

	// Send the final done packet for the execution of the command  
    // batch. The previous batch was one that might have returned   
    // rows, so set the DONE status accordingly.                    
    if (nRowsFetched > 0)
	{
        srv_senddone (srvproc, SRV_DONE_COUNT | SRV_DONE_MORE,
            (DBUSMALLINT) 0, nRowsFetched);
	}
    else 
	{
        srv_senddone (srvproc, SRV_DONE_MORE, (DBUSMALLINT) 0, (DBINT) 0);
	}

	// Revert back to SQL Server's user account
    if( bImpersonated )
        srv_revert_to_self(srvproc);

    return(XP_NOERROR);
}

// checksum
//    C++ function to calculate checksum
//
// Parameters:
//    buffer - pointer to char string to calculate checksum
//    length - length of buffer
//
// Returns:
//    checksum value
unsigned short checksum(const wchar_t *buffer, int length)
{
	unsigned short cs = 0;
	for (int i = 0; i < length; i++)
	{
		cs += (unsigned short)buffer[i] & 0xFFFF;
	}
	return cs & 0xFFFF;
}

// ConvertToUnicode
//    convert char string to OLECHAR
//
// Parameters:
//    szA - pointer to char string
//
// Returns:
//    OLECHAR string
OLECHAR* ConvertToUnicode(char *szA)
{
  static OLECHAR achW[STRCONVERT_MAXLEN]; 
  MultiByteToWideChar(CP_ACP, 0, szA, -1, achW, STRCONVERT_MAXLEN);  
  return achW; 
}

// CrackStrVariant
//    CTaken from the CCrack::strVariant method found in the
//      DAOVIEW sample that ships with Visual C++ 4.X/5.0
//
// Parameters:
//    var - Reference to an instance of COleVariant
//    pszRet - pointer to buffer for return string
//
// Returns:
//    A string containing the value of the COleVariant 
//    (if possible).  Otherwise a string containing the type 
//    of data contained by Variant
wchar_t *CrackStrVariant(VARIANT &var, wchar_t *pszRet)
{
	swprintf_s(pszRet, STRCONVERT_MAXLEN, L"Fish");
    switch(var.vt){
        case VT_EMPTY:
        case VT_NULL:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"NULL");
            break;
        case VT_I2:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"%hd", V_I2(&var));
            break;
        case VT_I4:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"%d", V_I4(&var));
            break;
        case VT_R4:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"%e", (double)V_R4(&var));
            break;
        case VT_R8:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"%e", V_R8(&var));
            break;
        case VT_CY:			
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_CY");
            break;
        case VT_DATE:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_DATE");
            break;
        case VT_BSTR:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"%S", V_BSTR( &var ));
            break;
        case VT_DISPATCH:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_DISPATCH");
            break;
        case VT_ERROR:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_ERROR");
            break;
        case VT_BOOL:
			*((int *)pszRet ) = V_BOOL(&var) ? 1 : 0;
			return pszRet;
        case VT_VARIANT:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_VARIANT");
            break;
        case VT_UNKNOWN:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_UNKNOWN");
            break;
        case VT_I1:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_I1");
            break;
        case VT_UI1:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"0x%02hX",(unsigned short)V_UI1(&var));
            break;
        case VT_UI2:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_UI2");
            break;
        case VT_UI4:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_UI4");
            break;
        case VT_I8:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_I8");
            break;
        case VT_UI8:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_UI8");
            break;
        case VT_INT:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_INT");
            break;
        case VT_UINT:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_UINT");
            break;
        case VT_VOID:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_VOID");
            break;
        case VT_HRESULT:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_HRESULT");
            break;
        case VT_PTR:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_PTR");
            break;
        case VT_SAFEARRAY:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_SAFEARRAY");
            break;
        case VT_CARRAY:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_CARRAY");
            break;
        case VT_USERDEFINED:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_USERDEFINED");
            break;
        case VT_LPSTR:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_LPSTR");
            break;
        case VT_LPWSTR:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_LPWSTR");
            break;
        case VT_FILETIME:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_FILETIME");
            break;
        case VT_BLOB:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_BLOB");
            break;
        case VT_STREAM:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_STREAM");
            break;
        case VT_STORAGE:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_STORAGE");
            break;
        case VT_STREAMED_OBJECT:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_STREAMED_OBJECT");
            break;
        case VT_STORED_OBJECT:
			swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_STORED_OBJECT");
            break;
        case VT_BLOB_OBJECT:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_BLOB_OBJECT");
            break;
        case VT_CF:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_CF");
            break;
        case VT_CLSID:
            swprintf_s(pszRet, STRCONVERT_MAXLEN, L"VT_CLSID");
            break;
    }
    WORD vt = var.vt;
    if(vt & VT_ARRAY){
        vt = vt & ~VT_ARRAY;
        swprintf_s(pszRet, STRCONVERT_MAXLEN, L"Array of ");
    }
    if(vt & VT_BYREF){
        vt = vt & ~VT_BYREF;
        wcscat_s(pszRet, STRCONVERT_MAXLEN, L"Pointer to ");
    }
    if (vt != var.vt)
	{
        switch(vt)
		{
            case VT_EMPTY:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_EMPTY");
                break;
            case VT_NULL:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_NULL");
                break;
            case VT_I2:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_I2");
                break;
            case VT_I4:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_I4");
                break;
            case VT_R4:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_R4");
                break;
            case VT_R8:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_R8");
                break;
            case VT_CY:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_CY");
                break;
            case VT_DATE:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_DATE");
                break;
            case VT_BSTR:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_BSTR");
                break;
            case VT_DISPATCH:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_DISPATCH");
				break;
            case VT_ERROR:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_ERROR");
                break;
            case VT_BOOL:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_BOOL");
                break;
            case VT_VARIANT:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_VARIANT");
                break;
            case VT_UNKNOWN:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_UNKNOWN");
                break;
            case VT_I1:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_I1");
                break;
            case VT_UI1:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_UI1");
                break;
            case VT_UI2:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_UI2");
                break;
            case VT_UI4:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_UI4");
                break;
            case VT_I8:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_I8");
                break;
            case VT_UI8:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_UI8");
                break;
            case VT_INT:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_INT");
                break;
            case VT_UINT:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_UINT");
                break;
            case VT_VOID:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_VOID");
                break;
            case VT_HRESULT:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_HRESULT");
                break;
            case VT_PTR:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_PTR");
                break;
            case VT_SAFEARRAY:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_SAFEARRAY");
                break;
            case VT_CARRAY:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_CARRAY");
                break;
            case VT_USERDEFINED:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_USERDEFINED");
                break;
            case VT_LPSTR:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_LPSTR");
                break;
            case VT_LPWSTR:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_LPWSTR");
                break;
            case VT_FILETIME:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_FILETIME");
                break;
            case VT_BLOB:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_BLOB");
                break;
            case VT_STREAM:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_STREAM");
                break;
            case VT_STORAGE:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_STORAGE");
                break;
            case VT_STREAMED_OBJECT:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_STREAMED_OBJECT");
                break;
            case VT_STORED_OBJECT:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_STORED_OBJECT");
                break;
            case VT_BLOB_OBJECT:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_BLOB_OBJECT");
                break;
            case VT_CF:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_CF");
                break;
            case VT_CLSID:
                wcscat_s(pszRet, STRCONVERT_MAXLEN, L"VT_CLSID");
                break;
		}
    }
    return pszRet;
}

// PrintErrorMsg
//    Send a message to the client
//
// Parameters:
//    srvproc - the handle to the client connection that got the SRV_CONNECT.
//    szErrorMsg - error text
//
// Returns:
//    none
void PrintErrorMsg(SRV_PROC *srvproc, char *szErrorMsg)
{
	// Send error message and return.
	srv_sendmsg(srvproc, SRV_MSG_ERROR, GETTABLE_ERROR,
            SRV_INFO, (DBTINYINT) 0,
            NULL, 0, 0,
            szErrorMsg,
            SRV_NULLTERM);
	// A SRV_DONE_MORE instead of a SRV_DONE_FINAL must complete the
	// result set of an Extended Stored Procedure.
	srv_senddone(srvproc, (SRV_DONE_ERROR | SRV_DONE_MORE), 0, 0);
}