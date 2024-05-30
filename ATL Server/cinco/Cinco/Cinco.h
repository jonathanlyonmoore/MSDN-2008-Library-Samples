// Cinco.h : Defines the ATL Server request handler class
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#pragma once

[ request_handler("Default") ]
class CCincoHandler
{
private:
	// Put private members here

protected:
	// Put protected members here
	CComPtr<ICincoDictionary> m_spDictionary;
	CString m_strGuess;
	CString m_strTarget;
	int m_nMatches;
	int m_nExact;
	DWORD m_nWord;
	
public:
	// Put public members here

	HTTP_CODE ValidateAndExchange()
	{
#if 1
		// get (or create) a dictionary object
		CComPtr<ICincoDictionary> m_spDictionary;
		HRESULT hr;
		hr = m_spServiceProvider->QueryService(
			__uuidof(ICincoDictionary),
			__uuidof(ICincoDictionary),
			(void**) &m_spDictionary);
		if (FAILED(hr))
		{
			CComQIPtr<IIsapiExtension> spExtension = m_spServiceProvider;
			if (!spExtension)
				return HTTP_FAIL;
			else
			{
				m_spDictionary = new CComObjectNoLock<CCincoDictionary>;
				hr = m_spDictionary->Initialize();
				if (FAILED(hr))
					return HTTP_FAIL;
				else
				{
					hr = spExtension->AddService(
						__uuidof(ICincoDictionary),
						__uuidof(ICincoDictionary),
						m_spDictionary,
						_pModule->GetModuleInstance());
					if (FAILED(hr))
						return HTTP_FAIL;
				}
			}
		}

		m_strGuess = m_HttpRequest.FormVars.Lookup("guess");
		m_strGuess.MakeLower();
		m_nMatches = 0;
		m_nExact = 0;

		LPCSTR szWord = m_HttpRequest.FormVars.Lookup("word");
		if (szWord && szWord[0])
			m_nWord = atoi(szWord);
		else
			m_spDictionary->GetRandomWord(&m_nWord);

		CComBSTR bstr;
		m_spDictionary->GetWord(m_nWord, &bstr);
		m_strTarget = CString(bstr);
#else
		m_strGuess = m_HttpRequest.FormVars.Lookup("guess");
		m_strGuess.MakeLower();
		m_nMatches = 0;
		m_nExact = 0;

		static LPCSTR s_aszWord[] =
		{
			"whole",
			"zebra",
			"shaft",
			"sword",
		};
		static int s_nWords = sizeof(s_aszWord)/sizeof(s_aszWord[0]);
		LPCSTR szWord = m_HttpRequest.FormVars.Lookup("word");
		if (szWord && szWord[0])
			m_nWord = atoi(szWord);
		else
			m_nWord = rand() % s_nWords;
		m_strTarget = s_aszWord[m_nWord];
#endif

		if (m_strGuess.GetLength() == m_strTarget.GetLength())
		{
			for (int i=0; i<m_strGuess.GetLength(); i++)
			{
				// assumes target has no duplicate letters
				if (m_strTarget[i] == m_strGuess[i])
				{
					m_nMatches++;
					m_nExact++;
				}
				else if (m_strGuess.Find(m_strTarget[i]) != -1)
				{
					m_nMatches++;
				}
			}
		}

		// Set the content-type
		m_HttpResponse.SetContentType("text/html");

		return HTTP_SUCCESS;
	}

	[ tag_name(name="guess") ]
	HTTP_CODE OnGuess(void)
	{
		m_HttpResponse << m_strGuess;
		return HTTP_SUCCESS;
	}

	[ tag_name(name="exact") ]
	HTTP_CODE OnExact(void)
	{
		m_HttpResponse << m_nExact;
		return HTTP_SUCCESS;
	}

	[ tag_name(name="matches") ]
	HTTP_CODE OnMatches(void)
	{
		m_HttpResponse << m_nMatches;
		return HTTP_SUCCESS;
	}

	[ tag_name(name="length") ]
	HTTP_CODE OnLength(void)
	{
		m_HttpResponse << m_strTarget.GetLength();
		return HTTP_SUCCESS;
	}

	[ tag_name(name="word") ]
	HTTP_CODE OnWord(void)
	{
		m_HttpResponse << m_nWord;
		return HTTP_SUCCESS;
	}

	[ tag_name(name="guessvalid") ]
	HTTP_CODE OnGuessValid(void)
	{
		if (m_strGuess.GetLength() > 0)
		{
#if 1
			CComPtr<ICincoDictionary> m_spDictionary;
			HRESULT hr;
			hr = m_spServiceProvider->QueryService(
				__uuidof(ICincoDictionary),
				__uuidof(ICincoDictionary),
				(void**) &m_spDictionary);
			if (FAILED(hr))
				return HTTP_FAIL;

			hr = m_spDictionary->IsWordValid(CComBSTR(m_strGuess));
			if (hr == S_OK)
				m_HttpResponse << "true";
			else
				m_HttpResponse << "false";
#else
			if (m_strGuess == "screw")
				m_HttpResponse << "false";
			else
				m_HttpResponse << "true";
#endif
		}
		else
			m_HttpResponse << "true";
		return HTTP_SUCCESS;
	}
}; // class CCincoHandler
