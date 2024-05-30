// WebDbg.cpp : Defines the class behaviors for the application.
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#include "stdafx.h"
#include "WebDbg.h"
#include "DebugDialog.h"
#include "FilterDialog.h"
#include "PipeDlg.h"

#include "MainFrm.h"
#include "atlutil.h"
#include <tchar.h>
#include <atlsecurity.h>
#include <aclui.h>

//////////////////////////////////////////////////////////////////////
//Resource (satellite) dll search and load routines. see LoadLocResDll
//an example.
//Note: Change made here must be sync with all other tools.

__inline
errno_t __cdecl DuplicateEnvString(TCHAR **ppszBuffer, size_t *pnBufferSizeInTChars, const TCHAR *pszVarName)
{	    
    /* validation section */
	if (ppszBuffer == NULL) { return  EINVAL; }
    *ppszBuffer = NULL;
    if (pnBufferSizeInTChars != NULL)
    {
        *pnBufferSizeInTChars = 0;
    }
    /* varname is already validated in getenv */
	TCHAR szDummyBuff[1] = {0};
	size_t nSizeNeeded = 0;
    errno_t ret=_tgetenv_s(&nSizeNeeded,szDummyBuff,1,pszVarName);
	if (nSizeNeeded > 0)
	{	    		
		*ppszBuffer = new TCHAR[nSizeNeeded];
		if (*ppszBuffer != NULL)
		{
			size_t nSizeNeeded2 = 0;
			ret=_tgetenv_s(&nSizeNeeded2,*ppszBuffer,nSizeNeeded,pszVarName);
			if (nSizeNeeded2!=nSizeNeeded)
			{
				ret=ERANGE;
			} else if (pnBufferSizeInTChars != NULL)
			{
				*pnBufferSizeInTChars = nSizeNeeded;
			}				
		} else
		{
			ret=ENOMEM;
		}	    		
	}
    return ret;
}

#define _TCSNLEN(sz,c) (min(_tcslen(sz), c))
#define PATHLEFT(sz) (_MAX_PATH - _TCSNLEN(sz, (_MAX_PATH-1)) - 1)

typedef LANGID (WINAPI* PFNGETUSERDEFAULTUILANGUAGE)();

static BOOL CALLBACK _EnumResLangProc(HMODULE /*hModule*/, LPCTSTR /*pszType*/, 
	LPCTSTR /*pszName*/, WORD langid, LONG_PTR lParam)
{
	if(lParam == NULL)
	{
		return FALSE;
	}
		
	LANGID* plangid = reinterpret_cast< LANGID* >( lParam );
	*plangid = langid;

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//Purpose: GetUserDefaultUILanguage for downlevel platforms (Win9x, NT4).
//Input: szDllName - the string resource dll name to search. Ex: ToolUI.dll 
//Output: TCHAR *szPathOut - filled with absolute path to dll, if found.
//		  size_t sizeInCharacters - buffer size in characters
//Returns: Success - HMODULE of found dll, Failure - NULL
//////////////////////////////////////////////////////////////////////////
HRESULT GetUserDefaultUILanguageLegacyCompat(LANGID* pLangid)
{
	HRESULT hr=E_FAIL;	
	if (pLangid == NULL) { return E_POINTER; }
	PFNGETUSERDEFAULTUILANGUAGE pfnGetUserDefaultUILanguage;	
	HINSTANCE hKernel32 = ::GetModuleHandle(_T("kernel32.dll"));
	pfnGetUserDefaultUILanguage = (PFNGETUSERDEFAULTUILANGUAGE)::GetProcAddress(hKernel32, "GetUserDefaultUILanguage");
	if(pfnGetUserDefaultUILanguage != NULL)
	{
		*pLangid = pfnGetUserDefaultUILanguage();
		hr = S_OK;
	} else
	{
		// We're not on an MUI-capable system.
		OSVERSIONINFO version;
		memset(&version, 0, sizeof(version));
		version.dwOSVersionInfoSize = sizeof(version);
		::GetVersionEx(&version);
		if( version.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
		{
			// We're on Windows 9x, so look in the registry for the UI language
			HKEY hKey = NULL;
			LONG nResult = ::RegOpenKeyEx(HKEY_CURRENT_USER, 
				_T( "Control Panel\\Desktop\\ResourceLocale" ), 0, KEY_READ, &hKey);
			if (nResult == ERROR_SUCCESS)
			{
				DWORD dwType;
				TCHAR szValue[16];
				ULONG nBytes = sizeof( szValue );
				nResult = ::RegQueryValueEx(hKey, NULL, NULL, &dwType, LPBYTE( szValue ), 
					&nBytes );
				if ((nResult == ERROR_SUCCESS) && (dwType == REG_SZ))
				{
					DWORD dwLangID;
					int nFields = _stscanf_s( szValue, _T( "%x" ), &dwLangID );
					if( nFields == 1 )
					{
						*pLangid = LANGID( dwLangID );
						hr = S_OK;
					}
				}

				::RegCloseKey(hKey);
			}
		}
		else
		{
			// We're on NT 4.  The UI language is the same as the language of the version
			// resource in ntdll.dll
			HMODULE hNTDLL = ::GetModuleHandle( _T( "ntdll.dll" ) );
			if (hNTDLL != NULL)
			{
				*pLangid = 0;
				::EnumResourceLanguages( hNTDLL, RT_VERSION, MAKEINTRESOURCE( 1 ), 
					_EnumResLangProc, reinterpret_cast< LONG_PTR >( pLangid ) );
				if (*pLangid != 0)
				{
					hr = S_OK;
				}
			}
		}
	}
	return hr;
}

//////////////////////////////////////////////////////////////////////////
//Purpose: Searches for a resource dll in sub directories using a search order
//		   based on szPath - a directory to search res dll below.
//		   see example at .
//Input: szDllName - the string resource dll name to search. Ex: ToolUI.dll 
//Output: TCHAR *szPathOut - filled with absolute path to dll, if found.
//		  size_t sizeInCharacters - buffer size in characters
//Returns: Success (found dll) - S_OK , Failure - E_FAIL or E_UNEXPECTED
//////////////////////////////////////////////////////////////////////////
HRESULT LoadUILibrary(LPCTSTR szPath, LPCTSTR szDllName, DWORD dwExFlags, 
                      HINSTANCE *phinstOut, LPTSTR szFullPathOut,size_t sizeInCharacters,
                      LCID *plcidOut)
{
    TCHAR szPathTemp[_MAX_PATH + 1] = _T("");
    HRESULT hr = E_FAIL;
    LCID lcidFound = (LCID)-1;
    size_t nPathEnd = 0;

    // Gotta have this stuff!
	if (szPath==NULL || *szPath == '\0')	   
	{ 
		return E_POINTER; 
	}

    if (szDllName==NULL || *szDllName == '\0') 
	{ 
		return E_POINTER; 
	}

    if (!szPath || !*szPath || !szDllName || !*szDllName)
	{
        return E_INVALIDARG;
	}

    if (phinstOut != NULL)
    {
        *phinstOut = NULL;
    }

    szPathTemp[_MAX_PATH-1] = L'\0';

    // Add \ to the end if necessary
    _tcsncpy_s(szPathTemp,_countof(szPathTemp), szPath, _TRUNCATE);
    if (szPathTemp[_TCSNLEN(szPathTemp, _MAX_PATH-1) - 1] != L'\\')
    {
        _tcsncat_s(szPathTemp,_countof(szPathTemp), _T("\\"), PATHLEFT(szPathTemp));
    }

    // Check if given path even exists
    if (GetFileAttributes(szPathTemp) == 0xFFFFFFFF)
	{
        return E_FAIL;
	}

    nPathEnd = _TCSNLEN(szPathTemp, _MAX_PATH-1);
    
    {	        
		LANGID langid=0;
		if (FAILED(GetUserDefaultUILanguageLegacyCompat(&langid)))
		{
			return E_UNEXPECTED;
		}
        const LCID lcidUser = MAKELCID(langid, SORT_DEFAULT);
        
        LCID rglcid[3];
        rglcid[0] = lcidUser;
        rglcid[1] = MAKELCID(MAKELANGID(PRIMARYLANGID(lcidUser), SUBLANG_DEFAULT), SORTIDFROMLCID(lcidUser));
        rglcid[2] = 0x409;
        for (unsigned int i = 0; i < _countof(rglcid); i++)
        {
            TCHAR szNumBuf[10];
            
            // Check if it's the same as any LCID already checked,
            // which is very possible
            unsigned int n = 0;
            for (n = 0; n < i; n++)
            {
                if (rglcid[n] == rglcid[i])
                    break;
            }

            if (n < i)
			{
                continue;
			}
            
            szPathTemp[nPathEnd] = L'\0';
			_itot_s(rglcid[i], szNumBuf,_countof(szNumBuf), 10);
            _tcsncat_s(szPathTemp, _countof(szPathTemp),szNumBuf , PATHLEFT(szPathTemp));
            _tcsncat_s(szPathTemp,_countof(szPathTemp), _T("\\"), PATHLEFT(szPathTemp));
            _tcsncat_s(szPathTemp,_countof(szPathTemp), szDllName, PATHLEFT(szPathTemp));

            if (GetFileAttributes(szPathTemp) != 0xFFFFFFFF)
            {
                lcidFound = rglcid[i];

                hr = S_OK;
                goto Done;
            }
        }
    }

    // None of the default choices exists, so now look for the dll in a folder below
	//the given path (szPath)
    {        
        szPathTemp[nPathEnd] = L'\0';
        _tcsncat_s(szPathTemp,_countof(szPathTemp), _T("*.*"), PATHLEFT(szPathTemp));

		WIN32_FIND_DATA wfdw;
        HANDLE hDirs = FindFirstFile(szPathTemp, &wfdw);
        nPathEnd = _TCSNLEN(szPathTemp, _MAX_PATH-1)-3;
        if (hDirs != INVALID_HANDLE_VALUE)
        {
            while (FindNextFile(hDirs, &wfdw))
            {
                // We are only interested in directories, since at this level, that should
                // be the only thing in this directory, i.e, LCID sub dirs
                if (wfdw.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                {
                    // Skip current and previous dirs, "." and ".."
                    if (!_tcscmp(wfdw.cFileName, _T(".")) || !_tcscmp(wfdw.cFileName, _T("..")))
                        continue;

                    // Does this dir have a copy of the dll?
                    szPathTemp[nPathEnd] = L'\0';
                    _tcsncat_s(szPathTemp,_countof(szPathTemp), wfdw.cFileName, PATHLEFT(szPathTemp));
                    _tcsncat_s(szPathTemp,_countof(szPathTemp), _T("\\"), PATHLEFT(szPathTemp));
                    _tcsncat_s(szPathTemp,_countof(szPathTemp), szDllName, PATHLEFT(szPathTemp));

                    if (GetFileAttributes(szPathTemp) != 0xFFFFFFFF)
                    {
                        // Got it!
                        lcidFound = (LCID)_tstol(wfdw.cFileName);

                        hr = S_OK;
                        break;
                    }
                }
            }

            FindClose(hDirs);
        }
    }

Done:
    if (SUCCEEDED(hr))
    {
        // Set the default LCID
        if (plcidOut)
        {
			if (lcidFound == (LCID)-1) 
			{ 
				return E_UNEXPECTED; 
			}
            *plcidOut = lcidFound;
        }

        // Finally, attempt to load the library
        // Beware!  A dll loaded with LOAD_LIBRARY_AS_DATAFILE won't
        // let you use LoadIcon and things like that (only general calls like
        // FindResource and LoadResource).
        if (phinstOut != NULL)
        {
            *phinstOut = LoadLibraryEx(szPathTemp, NULL, dwExFlags);
            hr = (*phinstOut) ? S_OK : E_FAIL;
        }
        if ( szFullPathOut )
		{
            _tcsncpy_s(szFullPathOut,sizeInCharacters, szPathTemp, _MAX_PATH-1);
		}
    }
 
    return hr;
}
//////////////////////////////////////////////////////////////////////////
//Purpose: Iterates env("PATH") directories to try to find (using LoadUILibrary)
//		   resource dll a directory below PATH dirs. Ex: if PATH="c:\bin;d:\win"
//		   and szDllName="ToolUI.dll", then the first of c:\bin\1033\ToolUI.dll 
//		   and d:\win\SomeFolder\ToolUI.dll will be loaded.
//		   See LoadLocResDll doc (below) for example.
//Input: szDllName - the string resource dll name to search. Ex: ToolUI.dll 
//Output: TCHAR *szPathOut - filled with absolute path to dll, if found.
//		  size_t sizeInCharacters - buffer size in characters
//Returns: Success - HMODULE of found dll, Failure - NULL
//////////////////////////////////////////////////////////////////////////
HMODULE LoadSearchPath(LPCTSTR szDllName,TCHAR *szPathOut, size_t sizeInCharacters)
{
    TCHAR * szEnvPATH = NULL;
	TCHAR * szEnvPATHBuff = NULL;    
    int nPathLen = 0;
    int nPathIndex = 0;
    HMODULE hmod = NULL;	
    if (DuplicateEnvString(&szEnvPATHBuff,NULL,_T("PATH"))==0 && (szEnvPATH=szEnvPATHBuff) != NULL) 
	{
        while (*szEnvPATH) 
		{
            /* skip leading white space and nop semicolons */
            for (; *szEnvPATH == L' ' || *szEnvPATH == L';'; ++szEnvPATH)
            {} /* NOTHING */

            if (*szEnvPATH == L'\0')
			{
                break;
			}

            ++nPathIndex;

            /* copy this chunk of the path into our trypath */
            nPathLen = 0;
			TCHAR szPath[_MAX_PATH+1];
			TCHAR * pszTry = NULL;
            for (pszTry = szPath; *szEnvPATH != L'\0' && *szEnvPATH != L';'; ++szEnvPATH) 
			{
				++nPathLen;
                if (nPathLen < _MAX_PATH) 
				{                    
                    *pszTry++ = *szEnvPATH;
                } else 
				{
                    break;
                }
            }
            *pszTry = L'\0';

            if (nPathLen == 0 || nPathLen >= _MAX_PATH)
			{
                continue;
			}

            LoadUILibrary(szPath, szDllName, LOAD_LIBRARY_AS_DATAFILE, 
                          &hmod, szPathOut,sizeInCharacters, NULL);
            if ( hmod )
			{
                break;
			}
        }
    }
	if (szEnvPATHBuff!=NULL)
	{
		delete [] szEnvPATHBuff;
	}
    return hmod;
}
//Example: Say PATH="c:\bin;d:\win", resource dll name (szDllName) is "ToolUI.dll",
//		   user locale is 936, and the .exe calling LoadLocResDll is c:\MyTools\Tool.exe
//			Search order:
//			a) c:\MyTools\936\ToolUI.dll (exe path + user default UI lang)			   
//			b) c:\MyTools\1033 (same with eng)
//			c) c:\MyTools\*\ToolUI.dll (where * is sub folder).
//			d) c:\bin\936\ToolUI.dll (first in path)
//			e) c:\bin\1033\ToolUI.dll (first in path + eng)
//			f) c:\bin\*\ToolUI.dll
//			g) d:\win\936\ToolUI.dll  (second in path)
//			h) d:\win\1033\ToolUI.dll (second in path + eng)
//			i) d:\win\*\ToolUI.dll (second in path + eng)
//			j) if bExeDefaultModule and not found, return exe HINSTANCE.
//			Note: The primary lang (without the sublang) is tested after the user ui lang.
// Main Input: szDllName - the name of the resource dll <ToolName>ui.dll. Ex: vcdeployUI.dll
// Main Output: HMODULE of resource dll or NULL - if not found (see bExeDefaultModule).
HMODULE LoadLocResDll(LPCTSTR szDllName,BOOL bExeDefaultModule=TRUE,DWORD dwExFlags=LOAD_LIBRARY_AS_DATAFILE,LPTSTR pszPathOut = NULL,size_t sizeInCharacters = 0  )
{
    HMODULE hmod = NULL;
    TCHAR driverpath[_MAX_PATH + 1], exepath[_MAX_PATH + 1];
    LPTSTR p = NULL;
    
    memset(driverpath,0,_MAX_PATH + 1);
    GetModuleFileName(GetModuleHandle(NULL), driverpath, _MAX_PATH);
    // find path of tool
    p = driverpath + _TCSNLEN(driverpath, _MAX_PATH-1)-1;
    while ( *p != L'\\' && p != driverpath)
	{
        p--;
	}
    *p = '\0';

    LoadUILibrary(driverpath, szDllName, dwExFlags, 
                  &hmod, exepath,_countof(exepath), NULL);

    if ( hmod == NULL ) 
	{
        // search PATH\<lcid> for <ToolName>ui.dll
        hmod = LoadSearchPath(szDllName,exepath,_countof(exepath));
    }

    if ( hmod && pszPathOut )
	{
        _tcsncpy_s(pszPathOut,sizeInCharacters, exepath, _MAX_PATH-1);
	}
	//Not found dll, return the exe HINSTANCE as a fallback.
	if (hmod == NULL && bExeDefaultModule)
	{
		hmod=GetModuleHandle(NULL);
	}
    return hmod;
}
//End loc routines
////////////////////////////////////////////////////////////////////
const TCHAR* szWebDbgUIDll = _T("WebDbgUI.dll");

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWebDbgApp

BEGIN_MESSAGE_MAP(CWebDbgApp, CWinApp)
	//{{AFX_MSG_MAP(CWebDbgApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_VIEW_STACKTRACE, OnViewStacktrace)
	ON_COMMAND(ID_APP_PIPE, OnAppPipe)
	ON_COMMAND(ID_FILE_PERMISSIONS, OnAppPermissions)
	ON_UPDATE_COMMAND_UI(ID_VIEW_STACKTRACE, OnUpdateViewStacktrace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// The one and only CWebDbgApp object

CWebDbgApp theApp;


class CPipeSecurityInformation : public CComObjectRootEx<CComSingleThreadModel>,
		public ISecurityInformation
{
public:
	HANDLE m_hObject;
	CStringW m_strObjectName;
	CSecurityDesc m_secDesc;
	CSecurityDesc m_secOriginal;

	SI_ACCESS m_access[1];

	CPipeSecurityInformation()
	{
		m_hObject = INVALID_HANDLE_VALUE;
	}

	HRESULT Init(HANDLE hObject, LPCWSTR wszObjectName)
	{
		memset(m_access, 0x00, sizeof(m_access));
		m_access[0].dwFlags = SI_ACCESS_GENERAL;
		m_access[0].pszName = MAKEINTRESOURCEW(IDS_ACCESS_PIPE);
		m_access[0].mask = FILE_GENERIC_WRITE|FILE_GENERIC_READ;

		m_hObject = hObject;
		try
		{
			if (!AtlGetSecurityDescriptor(m_hObject, SE_KERNEL_OBJECT, &m_secDesc, DACL_SECURITY_INFORMATION, true))
				return AtlHresultFromLastError();
		}
		catch (COleException *pE)
		{
			HRESULT hr = pE->m_sc;
			pE->Delete();
			return hr;
		}

		m_secOriginal = m_secDesc;
		m_strObjectName = wszObjectName;
		return S_OK;
	}

	BEGIN_COM_MAP(CPipeSecurityInformation)
		COM_INTERFACE_ENTRY_IID(IID_ISecurityInformation, ISecurityInformation)
	END_COM_MAP()

    // *** ISecurityInformation methods ***
    STDMETHOD(GetObjectInformation)(PSI_OBJECT_INFO pObjectInfo)
	{
		pObjectInfo->dwFlags = SI_EDIT_PERMS|SI_OWNER_READONLY;
		pObjectInfo->hInstance = AfxGetResourceHandle();
		pObjectInfo->pszServerName = NULL;
		pObjectInfo->pszObjectName = const_cast<LPWSTR>(static_cast<LPCWSTR>(m_strObjectName));
		return S_OK;
	}

    STDMETHOD(GetSecurity)(SECURITY_INFORMATION /*RequestedInformation*/,
                            PSECURITY_DESCRIPTOR *ppSecurityDescriptor,
                            BOOL fDefault)
	{
		HRESULT hr;

		if (!ppSecurityDescriptor)
			return E_POINTER;
		*ppSecurityDescriptor = NULL;

		CSecurityDesc *pSecDescToCopy = &m_secDesc;

		if (fDefault)
			pSecDescToCopy = &m_secOriginal;

		DWORD dwSize = 0;
		SECURITY_DESCRIPTOR *pSecDesc = NULL;
		try
		{
			pSecDescToCopy->GetSECURITY_DESCRIPTOR(NULL, &dwSize);
			pSecDesc = (SECURITY_DESCRIPTOR *) LocalAlloc(LHND, dwSize);
			if (!pSecDesc)
				return E_OUTOFMEMORY;
			pSecDescToCopy->GetSECURITY_DESCRIPTOR(pSecDesc, &dwSize);
		}
		catch (COleException *pE)
		{
			LocalFree(pSecDesc);
			hr = pE->m_sc;
			pE->Delete();
			return hr;
		}
		*ppSecurityDescriptor = pSecDesc;
		return S_OK;
	}

    STDMETHOD(SetSecurity)(SECURITY_INFORMATION SecurityInformation,
                            PSECURITY_DESCRIPTOR pSecurityDescriptor)
	{
		if (!pSecurityDescriptor)
			return E_POINTER;

		try
		{
			CSecurityDesc secDesc(*static_cast<SECURITY_DESCRIPTOR*>(pSecurityDescriptor));
			
			secDesc.MakeAbsolute();

			const SECURITY_DESCRIPTOR *pTempSecDesc = secDesc.GetPSECURITY_DESCRIPTOR();

			DWORD dwErr = SetSecurityInfo(m_hObject, SE_KERNEL_OBJECT, SecurityInformation,
				pTempSecDesc->Owner, pTempSecDesc->Group, pTempSecDesc->Dacl,
				pTempSecDesc->Sacl);
			if (dwErr != ERROR_SUCCESS)
				return AtlHresultFromWin32(dwErr);

			if (!AtlGetSecurityDescriptor(m_hObject, SE_KERNEL_OBJECT, &m_secDesc, DACL_SECURITY_INFORMATION, true))
				return AtlHresultFromLastError();
		}
		catch (COleException *pE)
		{
			HRESULT hr = pE->m_sc;
			pE->Delete();
			return hr;
		}
		return S_OK;
	}

    STDMETHOD(GetAccessRights) (const GUID* pguidObjectType,
                                DWORD /*dwFlags*/,
                                PSI_ACCESS *ppAccess,
                                ULONG *pcAccesses,
                                ULONG *piDefaultAccess)
	{
		if (pguidObjectType && !InlineIsEqualGUID(*pguidObjectType, GUID_NULL))
			return E_INVALIDARG;
		
		*ppAccess = m_access;
		*pcAccesses = sizeof(m_access)/sizeof(m_access[0]);
		*piDefaultAccess = 0;
		return S_OK;
	}

    STDMETHOD(MapGeneric) (const GUID *pguidObjectType,
                           UCHAR * /*pAceFlags*/,
                           ACCESS_MASK *pMask)
	{
		if (pguidObjectType && !InlineIsEqualGUID(*pguidObjectType, GUID_NULL))
			return E_INVALIDARG;
		GENERIC_MAPPING mapping;
		mapping.GenericAll = FILE_WRITE_DATA|FILE_READ_DATA|FILE_CREATE_PIPE_INSTANCE;
		mapping.GenericWrite = FILE_WRITE_DATA|FILE_CREATE_PIPE_INSTANCE;
		mapping.GenericRead = FILE_READ_DATA|FILE_CREATE_PIPE_INSTANCE;
		mapping.GenericExecute = 0;
		::MapGenericMask(pMask, &mapping);
		return S_OK;
	}
    
	STDMETHOD(GetInheritTypes) (PSI_INHERIT_TYPE *ppInheritTypes,
                                ULONG *pcInheritTypes)
	{
		if (!ppInheritTypes)
			return E_POINTER;

		*ppInheritTypes = NULL;
		
		if (!pcInheritTypes)
			return E_POINTER;

		*pcInheritTypes = 0;
		return S_OK;
	}

    STDMETHOD(PropertySheetPageCallback)(HWND /*hwnd*/, UINT /*uMsg*/, SI_PAGE_TYPE /*uPage*/)
	{
		return S_OK;
	}
};

UINT PipeServerThread(void* /* pvContext */)
{
	DWORD dwRead;

	HANDLE hPipe = theApp.GetPipe();
	if (hPipe == INVALID_HANDLE_VALUE)
		return 1;

	DEBUG_SERVER_MESSAGE Message;

	//Get the debugger
	TCHAR lpszDebugger[2048];
	TCHAR lpszCommandLine[500];
	TCHAR lpszExe[256];
	ULONG cbSize = 2048;
	HKEY hKey;
	RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\AeDebug"), 0, KEY_READ, &hKey);
	RegQueryValueEx(hKey, _T("Debugger"), NULL, NULL, (unsigned char*) lpszDebugger, &cbSize);
    lpszDebugger[2047] = 0;


	//Extract the name of the executable and the command line parameters
	//NOTE: I don't think this is portable

	//Skip the "
	int nDi = 0; 
	int nEi = 0;
	int nCi = 0;
	BOOL bBreakOnSpace = TRUE;

	//If the string starts with a ", don't break on space
	if (lpszDebugger[nDi] == _T('\"')) 
	{
		bBreakOnSpace = FALSE;
		nDi++;
	}	

	//Get the executable string
	while (nEi < 255)
	{
		TCHAR ch = lpszDebugger[nDi++];
		if (bBreakOnSpace && ch == ' ')
			break;
		else if (ch == _T('\"'))
			break;
		lpszExe[nEi++] = ch;
	}
	
	//Make sure it's null terminated
	lpszExe[nEi] = '\0';

	while (nCi < 499)
	{
		lpszCommandLine[nCi] = lpszDebugger[nDi++];

		//when we reach the end of the string recovered from the registry, break
		if (lpszCommandLine[nCi++] == '\0')
			break;
	}
    // Null terminate, just in case
    lpszCommandLine[nCi] = '\0';

	//intialize STARTUPINFO structure for CreateProcess
	STARTUPINFO startInfo;
	startInfo.cb = sizeof(STARTUPINFO);
	startInfo.lpReserved = NULL;
	startInfo.lpDesktop = NULL;
	startInfo.lpTitle = NULL;
	startInfo.dwFlags = 0;
	startInfo.cbReserved2 = 0;
	startInfo.lpReserved2 = NULL;


	ThreadMessageParam threadMessage;

	//Create an event to synchronize on for TRACE and ASSERT messages
	threadMessage.hEvent = CreateEvent(NULL, FALSE, NULL, NULL);
	if (threadMessage.hEvent == INVALID_HANDLE_VALUE)
	{
		return 1;
	}
	LPTSTR szMessage;
	LPTSTR szClientName;
	TCHAR szStackInfo[1025];
	CString strStackTrace;
	TCHAR szComputerName[MAX_COMPUTERNAME_LENGTH+1];
	DWORD dwComputerNameLen = sizeof(szComputerName)/sizeof(TCHAR);
	GetComputerName(szComputerName, &dwComputerNameLen);
	
	while (1)
	{
		strStackTrace.Empty();
		ConnectNamedPipe(hPipe, NULL);
		
		//Read the DEBUG_SERVER_MESSAGE structure from the pipe 
		BOOL bRet = ReadFile(hPipe, &Message, sizeof(Message), &dwRead, NULL);

		if (!bRet)
		{
//			TCHAR szErr[256];
//			sprintf(szErr, _T("Cannot Read From Pipe\r\nGetLastError() returned %d"), GetLastError());
//			theApp.m_pMainWnd->MessageBox(szErr, _T("Error"), MB_OK);
			DisconnectNamedPipe(hPipe);
			continue;
		}

		//If it is a quit Message from ExitInstance, break out of the loop
		if (Message.dwType == DEBUG_SERVER_MESSAGE_QUIT)
		{
			DisconnectNamedPipe(hPipe);
			break;
		}

		//Attempt to read the assert or trace text from the pipe
		szClientName = (LPTSTR)malloc(sizeof(TCHAR)*Message.dwClientNameLen);
                ATLENSURE(szClientName != NULL);

		bRet = ReadFile(hPipe, szClientName, Message.dwClientNameLen, &dwRead, NULL);
		if (!bRet)
		{
			DisconnectNamedPipe(hPipe);
            free(szClientName);
			continue;
		}

		//Attempt to read the assert or trace text from the pipe
		szMessage = (LPTSTR)malloc(sizeof(TCHAR)*Message.dwTextLen);
		ATLENSURE(szMessage != NULL);
		bRet = ReadFile(hPipe, szMessage, Message.dwTextLen, &dwRead, NULL);
		if (!bRet)
		{
//			TCHAR szErr[256];
//			sprintf(szErr, _T("Cannot Read From Pipe\r\nGetLastError() returned %d"), GetLastError());
//			theApp.m_pMainWnd->MessageBox(szErr, _T("Error"), MB_OK);
			DisconnectNamedPipe(hPipe);
            free(szClientName);
            free(szMessage);
			continue;
		}

		//If it is a trace message
		if (Message.dwType == DEBUG_SERVER_MESSAGE_TRACE)
		{
			threadMessage.nMsgType = HPS_TRACE_MESSAGE;
		}
		else
		{
			threadMessage.nMsgType = HPS_ASSERT_MESSAGE;
		}
		
		theApp.m_pMainWnd->SendMessage(WM_USER, (WPARAM)szMessage, (LPARAM)&threadMessage);
		int nRet = threadMessage.nRet;

		//If we want stack trace information
		if (nRet == IDOK)
		{
			if (theApp.WantStackTrace())
			{
				int nStack = 1;
				WriteFile(hPipe, &nStack, sizeof(nStack), &dwRead, NULL);
				do
				{
					nRet = ReadFile(hPipe, szStackInfo, 1024, &dwRead, NULL);
					if (!nRet && GetLastError() != ERROR_MORE_DATA)
					{
						//Can't properly recover from this type of error
						//So clean up and return
						DisconnectNamedPipe(hPipe);
						CloseHandle(threadMessage.hEvent);
                        free(szClientName);
                        free(szMessage);
                        return 0;
					}
					//concatenate
					strStackTrace += szStackInfo;
				} while (!nRet); //Repeat loop while ERROR_MORE_DATA
			}
			else //Else we just want to read the assert/trace message text
			{
				int nStack = 0;
				WriteFile(hPipe, &nStack, sizeof(nStack), &dwRead, NULL);
				strStackTrace = _T("");
			}
			CDebugDialog dlg(theApp.m_pMainWnd, theApp.WantStackTrace(), (_tcscmp(szClientName, szComputerName) == 0));
			dlg.InitData(threadMessage.nMsgType, Message.dwProcessId, szMessage, strStackTrace);
			nRet = dlg.DoModal();
		}
        free(szClientName);
        free(szMessage);

		switch (nRet)
		{
			case IDRETRY:
			{
				if (!Message.bIsDebuggerAttached)
				{
					SECURITY_ATTRIBUTES sa;
					sa.nLength = sizeof(sa);
					sa.lpSecurityDescriptor = NULL;
					sa.bInheritHandle = TRUE;
					HANDLE hEvent = CreateEvent(&sa, TRUE, FALSE, NULL);

					//Build the command line with the processId and event
					TCHAR lpszCmdLine[550];
					_stprintf_s(lpszCmdLine, sizeof(lpszCmdLine)/sizeof(TCHAR), lpszCommandLine, Message.dwProcessId, hEvent);

					PROCESS_INFORMATION processInfo;
					bRet = CreateProcess(lpszExe, lpszCmdLine, NULL, NULL, TRUE, 
						CREATE_DEFAULT_ERROR_MODE |CREATE_NEW_PROCESS_GROUP | NORMAL_PRIORITY_CLASS, 
						NULL, NULL, &startInfo, &processInfo);

					//We don't need these
					CloseHandle(processInfo.hProcess);
					CloseHandle(processInfo.hThread);

					theApp.m_pMainWnd->SetForegroundWindow();
					WaitForSingleObject(hEvent, 60000);

					CloseHandle(hEvent);
				}
				nRet = 1;
				break;
			}
			case IDABORT:
			{
				nRet = 2;
				break;
			}
			case IDCANCEL:
			case IDIGNORE:
			{
				nRet = 0;
				break;
			}
		}
		
		//Write the return value back to the pipe
		bRet = WriteFile(hPipe, &nRet, sizeof(nRet), &dwRead, NULL);
		if (!bRet)
		{
//			TCHAR szErr[256];
//			sprintf(szErr, _T("Cannot Write To Pipe\r\nGetLastError() returned %d"), GetLastError());
//			theApp.m_pMainWnd->MessageBox(szErr, _T("Error"), MB_OK);
			DisconnectNamedPipe(hPipe);
			continue;
		}
		FlushFileBuffers(hPipe);
		DisconnectNamedPipe(hPipe);
	}
	CloseHandle(threadMessage.hEvent);

	return 0;
}


/////////////////////////////////////////////////////////////////////////////
// CWebDbgApp construction

CWebDbgApp::CWebDbgApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_nCmdShow = SW_SHOW;
	m_strPipeName = _T("\\\\.\\pipe\\AtlsDbgPipe");
	m_hPipe = INVALID_HANDLE_VALUE;
}

CWebDbgApp::~CWebDbgApp()
{
	if (m_hPipe != INVALID_HANDLE_VALUE)
		CloseHandle(m_hPipe);
}

/////////////////////////////////////////////////////////////////////////////
// CWebDbgApp initialization

BOOL CWebDbgApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("HPS"));

	
	BOOL bFound = FALSE;
    m_hMutexOneInstance = CreateMutex(NULL,TRUE,_T("WebDbgOneInstanceOnly"));
    if(GetLastError() == ERROR_ALREADY_EXISTS)
        bFound = TRUE;
    if(m_hMutexOneInstance) 
        ReleaseMutex(m_hMutexOneInstance);
	

	if (!bFound)
	{
		HINSTANCE hInstRes=LoadLocResDll(szWebDbgUIDll,TRUE,0); //Do not load resource dll as data file (bitmaps,icons).
		AfxSetResourceHandle(hInstRes);
		// To create the main window, this code creates a new frame window
		// object and then sets it as the application's main window object.
		CMainFrame* pFrame = new CMainFrame;
		m_pMainWnd = pFrame;

		m_bWantStackTrace = GetProfileInt(_T("Settings"), _T("StackTrace"), 0);

		// create and load the frame with its resources

		pFrame->LoadFrame(IDR_MAINFRAME,
			WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
			NULL);

		// The one and only window has been initialized, so show and update it.
		pFrame->ShowWindow(m_nCmdShow);
		pFrame->UpdateWindow();

		// create the named pipe
		if (!CreatePipe())
			return FALSE;
		BeginPipeThread();
	}

	return (bFound == TRUE ? FALSE : TRUE);
}

BOOL CWebDbgApp::CreatePipe()
{
	m_hPipe = CreateNamedPipe(theApp.GetPipeName(),
		PIPE_ACCESS_DUPLEX | WRITE_DAC, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, 
		PIPE_UNLIMITED_INSTANCES, 1024, 4096, 20000, NULL);
	
	if (m_hPipe == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(IDS_FAILED_CREATING_PIPE, MB_ICONSTOP);
		return FALSE;
	}
	else if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		CloseHandle(m_hPipe);
		m_hPipe = INVALID_HANDLE_VALUE;
		AfxMessageBox(IDS_PIPEALREADYEXISTS, MB_ICONSTOP);
		return FALSE;
	}
	return TRUE;
}

void CWebDbgApp::BeginPipeThread()
{
	AfxBeginThread(PipeServerThread, (void*) GetCurrentThreadId());
}

void CWebDbgApp::QuitPipeThread()
{
	DEBUG_SERVER_MESSAGE Message;
	memset(&Message, 0x00, sizeof(Message));

	//Send a quit message to the PipeServerThread
	Message.dwType = DEBUG_SERVER_MESSAGE_QUIT;

	SendPipeMessage(&Message);
}

BOOL CWebDbgApp::ExitInstance()
{
	QuitPipeThread();

	WriteProfileInt(_T("Settings"), _T("StackTrace"), m_bWantStackTrace);

	ReleaseMutex(m_hMutexOneInstance);

	return CWinApp::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// CWebDbgApp message handlers





/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CWebDbgApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CWebDbgApp message handlers

void CWebDbgApp::OnViewStacktrace() 
{
	m_bWantStackTrace = !m_bWantStackTrace;
}


BOOL CWebDbgApp::WantStackTrace()
{
	return m_bWantStackTrace;
}

void CWebDbgApp::OnUpdateViewStacktrace(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_bWantStackTrace ? 1 : 0);
}

void CWebDbgApp::OnAppPipe()
{
	CPipeDlg dlg;
	int nItem = m_strPipeName.ReverseFind(_T('\\'));
	CString strPipeName = m_strPipeName.Right(m_strPipeName.GetLength()-nItem-1);
	dlg.m_PipeName = strPipeName;
	int nRet = dlg.DoModal();
	if (nRet == IDOK)
	{
		// update the pipe name
		if ((dlg.m_PipeName.GetLength()) && (dlg.m_PipeName.GetLength() < MAX_PATH-10))
		{
			CString strNewPipeName(dlg.m_PipeName);
			strNewPipeName.MakeLower();
			strPipeName.MakeLower();
			// Name did not change don't recreate the pipe
			if (strPipeName == strNewPipeName)
			{
				return;
			}
			QuitPipeThread();
			CloseHandle(m_hPipe);

			m_strPipeName.Format(_T("\\\\.\\pipe\\%s"), (LPCTSTR)dlg.m_PipeName);

			if (!CreatePipe())
			{
				AfxPostQuitMessage(1);
				return;
			}

			BeginPipeThread();
		}
	}
}

typedef BOOL (__stdcall *PFNEDITSECURITY)(HWND, LPSECURITYINFO);

void CWebDbgApp::OnAppPermissions()
{
#pragma warning(suppress:6321)	// relative path warning OK for constant
        HMODULE hDll = LoadLibrary(_T("aclui.dll"));
	if (hDll == NULL)
	{
		AfxMessageBox(IDS_ACLUI_REQUIRED, MB_ICONEXCLAMATION);
		return;
	}

	PFNEDITSECURITY pfnEditSecurity = (PFNEDITSECURITY) GetProcAddress(hDll, "EditSecurity");
	if (!pfnEditSecurity)
	{
		FreeLibrary(hDll);
		AfxMessageBox(IDS_ACLUI_REQUIRED, MB_ICONEXCLAMATION);
		return;
	}
	
	// turn of the pipe thread
	QuitPipeThread();

	do
	{

                CComObjectNoLock<CPipeSecurityInformation> *psecInfo = new CComObjectNoLock<CPipeSecurityInformation>();
		ATLENSURE(psecInfo != NULL);
		psecInfo->AddRef();
		HRESULT hr = psecInfo->Init(m_hPipe, CT2W(GetPipeName()));
		if (FAILED(hr))
		{
			LPVOID lpMsgBuf;
			if (FormatMessage( 
				FORMAT_MESSAGE_ALLOCATE_BUFFER | 
				FORMAT_MESSAGE_FROM_SYSTEM | 
				FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL,
				hr,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL 
			) != 0)
			{
				// Process any inserts in lpMsgBuf.
				// ...
				// Display the string.
				AfxMessageBox((LPCTSTR)lpMsgBuf, NULL, MB_ICONSTOP);
				// Free the buffer.
				LocalFree( lpMsgBuf );
			}
			else
				AfxMessageBox(IDS_UNKNOWN_ERROR, MB_ICONSTOP);
		}
		else
		{
			// We're ignoring the error here since if the user
			// canceled or an error occurs, we should have
			// the previous permissions
			(*pfnEditSecurity)(AfxGetMainWnd()->m_hWnd, psecInfo);
		}

		// before we recreate the pipe thread
		// make sure that the current user
		// has access to read/write from/to the pipe
		// since we use the pipe to control
		// the thread.  If the user cannot access
		// the pipe, then we have no way of shutting down
		// the pipe thread

		hr = TestAccessToPipe();
		psecInfo->Release();
		delete psecInfo;

		if (hr == E_ACCESSDENIED)
		{
			AfxMessageBox(IDS_ACCESSDENIED, MB_ICONSTOP);
		}
		else
		{
			if (FAILED(hr) && hr != HRESULT_FROM_WIN32(ERROR_PIPE_BUSY))
				AfxMessageBox(IDS_UNKNOWN_ERROR, MB_ICONSTOP);
			break;
		}
	} while (TRUE);

	BeginPipeThread();

	FreeLibrary(hDll);
}

// Try to open a handle to the named pipe.
// This function should only be called when
// the PipeServerThread is not running
HRESULT CWebDbgApp::TestAccessToPipe()
{
	HRESULT hr = S_OK;
	HANDLE hPipe = CreateFile(m_strPipeName, GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		DWORD dwError = GetLastError();
		// we ignore ERROR_PIPE_BUSY since
		// we know no one is listining
		// on the pipe
		if (dwError != ERROR_PIPE_BUSY)
		{
			hr = HRESULT_FROM_WIN32(dwError);
		}
	}
	else
	{
		if (GetFileType(hPipe) != FILE_TYPE_PIPE)
		{
			hr = E_INVALIDARG;
		}
		CloseHandle(hPipe);
	}

	return hr;
}

// Send a message to the pipe.  The message cannot require getting
// a response from the server
HRESULT CWebDbgApp::SendPipeMessage(const DEBUG_SERVER_MESSAGE *pMsg)
{
	HRESULT hr = S_OK;
	DWORD dwWritten;

	//Open the pipe for writing
	HANDLE hPipe = CreateFile(m_strPipeName, GENERIC_WRITE | GENERIC_READ,
		FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
		
	if (hPipe == INVALID_HANDLE_VALUE)
	{
		hr = AtlHresultFromLastError();
	}
	else
	{
		if (GetFileType(hPipe) != FILE_TYPE_PIPE)
		{
			hr = E_INVALIDARG;
		}
		else
		{
			if (!WriteFile(hPipe, pMsg, sizeof(DEBUG_SERVER_MESSAGE), &dwWritten, NULL) ||
					dwWritten != sizeof(DEBUG_SERVER_MESSAGE))
			hr = AtlHresultFromLastError();
			DisconnectNamedPipe(hPipe);
		}

		CloseHandle(hPipe);
	}
	return hr;
}

