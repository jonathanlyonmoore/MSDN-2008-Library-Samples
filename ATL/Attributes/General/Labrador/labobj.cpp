// LabObj.cpp : Implementation of CLabradorApp and DLL registration.
//
// This is a part of the Active Template Library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Active Template Library Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Active Template Library product.

#include "prelab.h"
#include "LabObj.h"
#include <wchar.h>


/////////////////////////////////////////////////////////////////////////////
//  CLabrador Interface Implementation

CLabrador::CLabrador()
{
	wcscpy_s(m_szPetName, MAX_MY_BSTR_LEN, L"Fido");
	wcscpy_s(m_szSpeciesName, MAX_MY_BSTR_LEN, L"Warthog");
	m_bIsAlive = TRUE;
	m_bIsBarking = FALSE;
}

/////////////////////////////////////////////////////////////////////////////
//  IDog Implementation

STDMETHODIMP CLabrador::GetPetName(MY_BSTR pStr)
{
	if (pStr)
		wcscpy_s(pStr, MAX_MY_BSTR_LEN, m_szPetName);
	return (HRESULT)NOERROR;
}

STDMETHODIMP CLabrador::SetPetName(MY_BSTR pStr)
{
	if (pStr)
	{
		wcscpy_s(m_szPetName, MAX_MY_BSTR_LEN, pStr);
		return S_OK;
	}
	return E_POINTER;
}

STDMETHODIMP CLabrador::IsBarking(BOOL* pBool)
{
	if (pBool)
	{
		*pBool = m_bIsBarking;
		return S_OK;
	}
	return E_POINTER;
}

/////////////////////////////////////////////////////////////////////////////
//  IMammal Implementation

STDMETHODIMP CLabrador::GetSpeciesName(MY_BSTR pStr)
{
	if (pStr != NULL)
		wcscpy_s(pStr, MAX_MY_BSTR_LEN, m_szSpeciesName);
	return (HRESULT)NOERROR;
}

STDMETHODIMP CLabrador::IsAlive(BOOL* pBool)
{
	if (pBool)
	{
		*pBool = m_bIsAlive;
		return S_OK;
	}
	return E_POINTER;
}
