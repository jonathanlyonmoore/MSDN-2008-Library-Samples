// File: CincoDict.h
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#pragma once

__interface ATL_NO_VTABLE __declspec(uuid("813F3F00-3881-11d3-977B-00C04F8EE25E")) 
	ICincoDictionary : public IUnknown
{
	HRESULT  STDMETHODCALLTYPE QueryInterface(REFIID riid, void **ppv);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	HRESULT Initialize();
	HRESULT GetRandomWord(DWORD* pnIndex);
	HRESULT GetWord(DWORD nIndex, BSTR* pbstrWord);
	HRESULT IsWordValid(BSTR bstrWord);
};

class CCincoDictionary :
	public CComObjectRootEx<CComMultiThreadModel>,
	public ICincoDictionary
{
public:
	BEGIN_COM_MAP(CCincoDictionary)
		COM_INTERFACE_ENTRY(ICincoDictionary)
	END_COM_MAP()

	CAtlArray<CString> m_nodups;
	CAtlMap<CString, int, CStringElementTraits<CString> > m_dict;

	CCincoDictionary()
	{
	}

	HRESULT Initialize()
	{
		HINSTANCE hInstance = _pModule->GetModuleInstance();
		char szPath[MAX_PATH];
		GetModuleFileName(hInstance, szPath, MAX_PATH);
		char* pszFile = strrchr(szPath, '\\');
		CAtlFile file;
		ULONGLONG nLen;
		DWORD nBuffSize;
		CAutoVectorPtr<char> pData;
		char* pBegin;
		char* pEnd;

		strcpy(pszFile, "\\dict.5");
		file.Create(szPath, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING);
		file.GetSize(nLen);
		nBuffSize = (DWORD) nLen;
		pData.Allocate((size_t) nLen+1);
		file.Read(pData.m_p, nBuffSize, nBuffSize);
		pData[nLen] = 0;
		pBegin = pEnd = pData;
		while (isalpha(*pBegin))
		{
			while (isalpha(*pEnd))
				pEnd++;
			m_dict[CString(pBegin, pEnd-pBegin)] = 0;
			while (*pEnd && !isalpha(*pEnd))
				pEnd++;
			pBegin = pEnd;
		}
		file.Close();
		pData.Free();

		strcpy(pszFile, "\\nodups.5");
		file.Create(szPath, GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING);
		file.GetSize(nLen);
		nBuffSize = (DWORD) nLen;
		pData.Allocate((size_t) nLen+1);
		file.Read(pData.m_p, nBuffSize, nBuffSize);
		pData[nLen] = 0;
		pBegin = pEnd = pData;
		while (isalpha(*pBegin))
		{
			while (isalpha(*pEnd))
				pEnd++;
			m_nodups.Add(CString(pBegin, pEnd-pBegin));
			while (*pEnd && !isalpha(*pEnd))
				pEnd++;
			pBegin = pEnd;
		}
		file.Close();
		pData.Free();

		srand((unsigned int) time(NULL));

		return S_OK;
	}

	HRESULT GetRandomWord(DWORD* pnIndex)
	{
		*pnIndex = rand() % m_nodups.GetSize();
		return S_OK;
	}

	HRESULT GetWord(DWORD nIndex, BSTR* pbstrWord)
	{
		CComBSTR bstr;
		bstr = m_nodups[nIndex];
		*pbstrWord = bstr.Detach();
		return S_OK;
	}

	HRESULT IsWordValid(BSTR bstrWord)
	{
		CString str(bstrWord);
		int n;
		if (m_dict.Lookup(str, n))
			return S_OK;
		else
			return S_FALSE;
	}
};
