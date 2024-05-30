// SCSample.h : Defines the ATL Server request handler class
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#pragma once
#define		DEFAULT_LIFESPAN	30
#include	<time.h>
#include	<fstream>
#include	<atlfile.h>
#include	<atlpath.h>
using namespace std;


inline HTTP_CODE SendError(CHttpResponse& response, const CStringA& strError, WORD wHttpStatus = 500)
{
    // Clear any buffered headers (including cookies) and content.
    response.ClearResponse();

    // Suggest that clients and proxies do not cache this response.
    response.SetCacheControl("no-cache");
    response.SetExpires(0);

    // Set the status code in the response object.
    response.SetStatusCode(wHttpStatus);

    // Build the body of the response.
    response << "<html><head><title>Stencil Cache Sample</title></head><body><p>" << strError << "</p></body></html>";

    // Return a HTTP_CODE that tells the ATL Server code to discontinue processing of the SRF file.
    return HTTP_ERROR(wHttpStatus, SUBERR_NO_PROCESS);
}

// The default replacement handler.
[ request_handler("Default") ]
class CSCSampleHandler
{
private:
	// Put private members here

protected:
	// Put protected members here
	
public:
	// Put public members here

	HTTP_CODE ValidateAndExchange()
	{
		// TODO: Put all initialization and validation code here
		
		// Set the content-type
		m_HttpResponse.SetContentType("text/html");
		
		return HTTP_SUCCESS;
	}
	// Here is an example of how to use a replacement tag with the stencil processor
	// This replacement method will inject the local time in the response stream, whenever 
	// the "getLocalTime" replacement tag is found
	[ tag_name(name="getLocalTime") ]
	HTTP_CODE OnGetLocalTime(void)
	{
		struct tm	*stNow;
		time_t		t;	

		time( &t );

		stNow	=	localtime( &t );


		m_HttpResponse << asctime( stNow );
		return HTTP_SUCCESS;
	}
}; // class CSCSampleHandler




// Special request handler for creating, rendering and removing a temporary file that 
// is customized for the request
[ request_handler("Frame") ]
class CSCSampleFrameHandler
{
private:
	// Put private members here

protected:
	// Put protected members here
	CStringA			_szFileName;
	CStringA			_szStartTime;
	CStringA			_szLifeSpan;
	CStringA				_szUserName;
	
public:
	// Put public members here
	HTTP_CODE ValidateAndExchange()
	{
		// TODO: Put all initialization and validation code here
		POSITION	pos	=	NULL;
		DWORD		dwRet	=	VALIDATION_E_PARAMNOTFOUND;
		long		lSpan;
		LPCSTR		szUserName;



		
		// Set the content-type
		m_HttpResponse.SetContentType("text/html");

		// Use Exchange to transfer between a form variable and a string local variable
		dwRet	=	m_HttpRequest.FormVars.Exchange("name", &szUserName);

		// When the frame is loaded from the HTML, the name field is not empty, 
		// so the validation will return VALIDATION_S_OK. If the validation fails,
		// then this is a re-load
		if(  dwRet	==	VALIDATION_S_OK )
		{
			// first loading of the frame
			time_t				tNow;					//	to hold the current time
			int					iLifespan = 30;			//	the lifespan indicated by the user (Default 30 secs)
			unsigned __int64	newLifespan;			//	the lifespan indicated by the user in 100 nanoseconds units
			CComPtr<IStencilCacheControl>	stCache;	//	wrapper for the cache control interface

			
			// Create a unique, new temporary file that generates the lower frame of the response
			_szFileName.Format("t%d.srf", clock() );

			time(&tNow);
			_szStartTime.Format("%d", tNow );

			_szUserName	=	szUserName;

			// Use Exchange to transfer between a form variable and a numeric local variable
			dwRet	=	m_HttpRequest.FormVars.Exchange("lifespan", &iLifespan);
			if( dwRet != S_OK )
			{
				iLifespan	=	DEFAULT_LIFESPAN;
			}
			_szLifeSpan.Format("%d", iLifespan);

			
			// Render the temporary file
			renderTemporaryFile();


			// convert from seconds (the unit used by the user) to 100 nanosecond units, used by the cache
			newLifespan	=	iLifespan * CFileTime::Second;

			// Query the IStencilCacheControl interface
			if(FAILED(m_spServiceProvider->QueryService(__uuidof(IStencilCache), __uuidof(IStencilCacheControl), (void**)&stCache)))
				return HTTP_FAIL;

			// set the new lifespan
			stCache->SetDefaultLifespan(newLifespan);

			// Append the temporary file name, the new timespan 
			// and the request processing time as cookies to the response
			// They will be used in refresh requests
			if(m_HttpResponse.AppendCookie( "filename", _szFileName ) == FALSE)
				return SendError(m_HttpResponse, "An error occurred while creating the cookie.");
				
			if(m_HttpResponse.AppendCookie( "lifespan", _szLifeSpan ) == FALSE)
				return SendError(m_HttpResponse, "An error occurred while creating the cookie.");
			
			if(m_HttpResponse.AppendCookie( "starttime", _szStartTime) == FALSE)
				return SendError(m_HttpResponse, "An error occurred while creating the cookie.");
			
			
		}
		else
		{
			//	This is an attempt to reload the page.
			LPCSTR			szVal;
			CStringA		strVal;
			
			// Use Exchange to transfer between a frm variable and a local variable
			// "mode" is supposed to describe the required reload ("touch" or "refresh")
			dwRet	=	m_HttpRequest.FormVars.Exchange("mode", &szVal);
			
			if(szVal == NULL)
				return HTTP_FAIL;
				
			// look for the "filename" cookie, that has been set when the page was initialized
			CCookie	cookie	=	m_HttpRequest.Cookies("filename");
			
			// Get the cookie value
			cookie.GetValue( strVal );

			_szFileName	=	strVal;

			if( _stricmp( szVal, "touch" ) == 0 )
			{
				// add a new line to the temporary file, to force reloading from the disk
				touchTemporaryFile();
			}

			if( _stricmp( szVal, "reset" ) == 0 )
			{
				// reset the cache settings

				// destroy the temporary file
				reset();
				
				// redirect the client to the start page
				if(m_HttpResponse.Redirect("SCSample.htm") == FALSE)
					return HTTP_FAIL;
				
				// clear the statistics for cache activity
				CComPtr<IMemoryCacheStats>	stCache;
				if(FAILED(m_spServiceProvider->QueryService(__uuidof(IStencilCache), __uuidof(IMemoryCacheStats), (void**)&stCache)))
					return HTTP_FAIL;

				stCache->ClearStats();
			}


		}
		
		return HTTP_SUCCESS;
	}


	// Adds a new line to the temporary file that generates the lower frame of the response,
	// to force reloading from the hard-drive
	void touchTemporaryFile()
	{
		char		modPath[MAX_PATH];
		CPathA		file;
		char		*pCh;
		ofstream	out;
		struct tm	*stNow;
		time_t		t;	

		time( &t );

		stNow	=	localtime( &t );

		

		DWORD	dwRet	=	::GetModuleFileNameA( this->m_hInstHandler, modPath, MAX_PATH);
		if( dwRet == 0 || dwRet == MAX_PATH)
		{
			ATLTRACE("GetModuleFileNameA failed -- GetLastError returned %d\n", ::GetLastError() );
			return;
		}

		CPathA		modulePath(modPath);
		if(modulePath.RemoveFileSpec())
		{
			file.Combine(LPCSTR(modulePath),_szFileName);
		}
		else
		{
			ATLTRACE("CPathA::RemoveFileSpec failed -- GetLastError returned %d\n", ::GetLastError() );
			return;
		}


		out.open(LPCSTR(file), ios::app);
		if( out.is_open() )
		{
			out	<<	"Last modified: " << asctime( stNow ) <<	"<br>"	<<	endl;
			out.close();
		}
		else
		{
			ATLTRACE("Touch: Failed in opening file %s\n", file);
		}
		
	}
	

	
	// Removes the temporary file
	void reset()
	{
		char		modPath[MAX_PATH];
		CPathA		file;
		char		*pCh;
		ofstream	out;
		struct tm	*stNow;
		time_t		t;	

		time( &t );

		stNow	=	localtime( &t );

		



		DWORD	dwRet	=	::GetModuleFileNameA( this->m_hInstHandler, modPath, MAX_PATH);
		if( dwRet == 0 || dwRet == MAX_PATH)
		{
			ATLTRACE("GetModuleFileNameA failed -- GetLastError returned %d\n", ::GetLastError() );
			return;
		}

		CPathA		modulePath(modPath);
		if(modulePath.RemoveFileSpec())
		{
			file.Combine(LPCSTR(modulePath),_szFileName);
		}
		else
		{
			ATLTRACE("CPathA::RemoveFileSpec failed -- GetLastError returned %d\n", ::GetLastError() );
			return;
		}

		::DeleteFileA( LPCSTR(file) );

	}

	
	// Renders the SRF content of the temporary file that generates the lower frame of the response
	// The temp file is customized to the user name and the current server time
	void	renderTemporaryFile()
	{
		char		modPath[MAX_PATH];
		CPathA		file;
		char		*pCh;
		ofstream	out;



		DWORD	dwRet	=	::GetModuleFileNameA( this->m_hInstHandler, modPath, MAX_PATH);
		if( dwRet == 0 || dwRet == MAX_PATH)
		{
			ATLTRACE("GetModuleFileNameA failed -- GetLastError returned %d\n", ::GetLastError() );
			return;
		}

		CPathA		modulePath(modPath);
		if(modulePath.RemoveFileSpec())
		{
			file.Combine(LPCSTR(modulePath),_szFileName);
		}
		else
		{
			ATLTRACE("CPathA::RemoveFileSpec failed -- GetLastError returned %d\n", ::GetLastError() );
			return;
		}

		out.open( LPCSTR(file) );

		if( out.is_open() )
		{
			struct tm	*stNow;
			time_t		t;	
			CStringA	szLocalTime;

			time( &t );

			stNow	=	localtime( &t );

			szLocalTime	=	asctime( stNow );

			out	<<	"{{handler SCSample.dll/Default}}"	<<	endl;
			out	<<	"<html>"	<<	endl;
			out	<<	"<title>SRF File to be cached</title>"	<<	endl;
			out	<<	"<body>"	<<	endl;
			out	<<	"Hello, "	<< (LPCSTR)_szUserName	<<	"<br>"	<<	endl;
			out	<<	"It was generated and cached first time at : "	<<	(LPCSTR)szLocalTime	<<	"(server time)<br>"	<<	endl;
			out	<<	"Now, it is : {{getLocalTime}} (server time) <br>"	<<	endl;
			out	<<	"</body>";
			out	<<	"</html>";


			out.flush();

			out.close();
		}
	}



	// Here is an example of how to use a replacement tag with the stencil processor
	[ tag_name(name="getFileName") ]
	HTTP_CODE OnGetFileName(void)
	{
		m_HttpResponse << _szFileName;
		return HTTP_SUCCESS;
	}
}; // class CSCSampleHandler








// Special request handler used in rendering statistical information related to the stencil cache
[ request_handler("Stats") ]
class CSCSampleStatsHandler
{
private:
	// Put private members here

protected:
	// Put protected members here
	
public:
	// Put public members here

	HTTP_CODE ValidateAndExchange()
	{
		// TODO: Put all initialization and validation code here
		
		// Set the content-type
		m_HttpResponse.SetContentType("text/html");
		
		return HTTP_SUCCESS;
	}
	// Here is an example of how to use a replacement tag with the stencil processor
	[ tag_name(name="getDefaultLifespan") ]
	HTTP_CODE OnGetDefaultLifespan(void)
	{
		CComPtr<IStencilCacheControl>	stCache;
		unsigned __int64		dwLifeSpan;
		

		if(FAILED(m_spServiceProvider->QueryService(__uuidof(IStencilCache), __uuidof(IStencilCacheControl), (void**)&stCache)))
			return HTTP_FAIL;
			
		stCache->GetDefaultLifespan(&dwLifeSpan);

		dwLifeSpan	=	dwLifeSpan / CFileTime::Second;

		m_HttpResponse	<<	dwLifeSpan;

		return HTTP_SUCCESS;
	}


	[ tag_name(name="getStats") ]
	HTTP_CODE OnGetStats(void)
	{
		CComPtr<IMemoryCacheStats>	stStats;
		DWORD		dwData;
		CStringA	szLine;

		// First, get a pointer to the IMemiryCacheStats interface implemented by the Stencil Cache service
		if(FAILED(m_spServiceProvider->QueryService(__uuidof(IStencilCache), __uuidof(IMemoryCacheStats), (void**)&stStats)))
			return HTTP_FAIL;

		// Use GetHitCount method to get the number of successfull cache lookups
		if(FAILED(stStats->GetHitCount(&dwData)))
			return SendError(m_HttpResponse, "An error occurred while getting the stats.");

		szLine.Format("Hits count : %d<br>", dwData);
		m_HttpResponse	<<	szLine;

		// Use GetMissCount method to get the number of insuccessfull cache lookups
		if(FAILED(stStats->GetMissCount(&dwData)))
			return SendError(m_HttpResponse, "An error occurred while getting the stats.");
			
		szLine.Format("Miss count : %d<br>", dwData);
		m_HttpResponse	<<	szLine;

		// Use GetCurrentAllocSize method to get the current cache allocated size
		if(FAILED(stStats->GetCurrentAllocSize(&dwData)))
			return SendError(m_HttpResponse, "An error occurred while getting the stats.");
			
		szLine.Format("Current Alloc Size : %d<br>", dwData);
		m_HttpResponse	<<	szLine;

		// Use GetMaxAllocSize method to get the maximum cache allocated size during this session
		if(FAILED(stStats->GetMaxAllocSize(&dwData)))
			return SendError(m_HttpResponse, "An error occurred while getting the stats.");
			
		szLine.Format("Max Alloc Size : %d<br>", dwData);
		m_HttpResponse	<<	szLine;

		// Use GetCurrentEntryCount method to get the current number of entries in the cache
		if(FAILED(stStats->GetCurrentEntryCount(&dwData)))
			return SendError(m_HttpResponse, "An error occurred while getting the stats.");
			
		szLine.Format("Current Entry Count : %d<br>", dwData);
		m_HttpResponse	<<	szLine;

		
		// Use GetCurrentAllocSize method to get the maximum number of entries in the cache during this session
		if(FAILED(stStats->GetMaxEntryCount(&dwData)))
			return SendError(m_HttpResponse, "An error occurred while getting the stats.");
			
		szLine.Format("Max Entry Count : %d<br>", dwData);
		m_HttpResponse	<<	szLine;

		return HTTP_SUCCESS;
	}

}; // class CSCSampleHandler
