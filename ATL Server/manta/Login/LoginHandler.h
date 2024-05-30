// LoginHandler.h : Defines the ATL Server request handler class
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#pragma once

#include "..\Common\Crypt.h"
#include "LoginDB.h"			// Login database classes

namespace Login		// Prevent FailureFlag name clash
{
	typedef struct _FailureFlags
	{
		bool m_bFailure;		// General failure bit
		bool m_bEmptyLogin;		// Empty login name
		bool m_bEmptyPassword;	// Empty password
		bool m_bLoginFailed;	// Login failed
	} FailureFlags;
}

[ request_handler("Login") ]
class CLoginHandler : public CMantaWebBase<CLoginHandler>
{
private:
	CStringA			m_strFirstName;			// User's first name
	CStringA			m_strLogin;				// User's login
	CStringA			m_strPassword;			// User's password
	bool				m_bPasswordRemembered;	// Set if we're remembering the password (bypassing password check)
	Login::FailureFlags m_fFailure;		// Login failure flags
	
public:
	// Default constructor
	CLoginHandler() : m_bPasswordRemembered(false)
	{
		// Set all the failure flags to false
		memset(&m_fFailure, false, sizeof(Login::FailureFlags));
	}

	HTTP_CODE ValidateAndExchange()
	{
		bool bPerformLogin = false;

		// Set the content type to html
		m_HttpResponse.SetContentType("text/html");
		m_HttpResponse.SetExpires(0);

		// Get the cached data connection
		HRESULT hr = GetDataConnection(&m_dataConnection);
		if (FAILED(hr))
			return DatabaseError("CMantaWebBase&lt;CLoginHandler&gt;::GetDataConnection()", hr);

		// Get the request param objects
		const CHttpRequestParams& FormFields = m_HttpRequest.GetFormVars();
		const CHttpRequestParams& QueryFields = m_HttpRequest.GetQueryParams();

		//Check for a logout request
		if (QueryFields.Lookup("logout") != NULL)
		{
			// Remove the session info and redirect to this page (get rid of query params)
			MantaWeb::CRemoveSessionData removeSession;
			GetSessionID(&removeSession.m_guidSessionID);
			removeSession.OpenRowset(m_dataConnection, NULL);
			m_HttpResponse.Redirect("login.srf");
			return HTTP_SUCCESS_NO_PROCESS;
		}

		if (QueryFields.Lookup("uid") != NULL &&	// Request to redirect to a message
		    QueryFields.Lookup("sid") != NULL &&	// Used with the mail service client
			QueryFields.Lookup("msgid") != NULL)
		{
			return RedirectToMessage(QueryFields);
		}

		// Check to see if all the fields are present
		if (FormFields.Lookup("login") != NULL && FormFields.Lookup("password") != NULL)
		{
			// Validate member data
			if (!ValidateFormData(FormFields))
				return HTTP_SUCCESS;

			bPerformLogin = true;
		}

		LPCSTR lpszLogin = GetLogin(true);

		if (lpszLogin != NULL)
		{
			// See if the user should have the password remembered
			// This will only work if the user has turned on "Remember Who I Am"
			CRememberPassword rememberPassword;
			if(FAILED(StringCchCopy(rememberPassword.m_szLogin, DB_MAX_STRLEN, lpszLogin)))
				return HTTP_FAIL;
			if (GetRememberID(&rememberPassword.m_guidRememberID))
			{
				hr = rememberPassword.OpenRowset(m_dataConnection, NULL);
	
				if (hr == S_OK)
				{
					hr = rememberPassword.MoveFirst();
					if (hr == S_OK && rememberPassword.m_bRememberPassword == VARIANT_TRUE)
					{
						// We are remembering the user
						m_bPasswordRemembered = true;
						// Get the login and first name from the persistant cookie too
						m_strLogin = lpszLogin;
						m_strFirstName = GetFirstName(true);
					}
				}
			}
		}

		if (!bPerformLogin && !m_bPasswordRemembered)
		{
			m_strLogin = "";
			m_strFirstName = "";
		}

		if (bPerformLogin)
			return Login();

		return HTTP_SUCCESS;
	}
	
	[ tag_name("FirstName") ]
	HTTP_CODE OnFirstName()
	{
		// Respond with the first name
		m_HttpResponse << m_strFirstName;
		return HTTP_SUCCESS;
	}

	[ tag_name("Failure") ]
	HTTP_CODE OnFailure()
	{
		// Return HTTP_SUCCESS if there was a failure
		return (m_fFailure.m_bFailure) ? HTTP_SUCCESS : HTTP_S_FALSE;
	}

	[ tag_name("EmptyLogin") ]
	HTTP_CODE OnEmptyLogin()
	{
		// Return HTTP_SUCCESS if the login was invalid
		return (m_fFailure.m_bEmptyLogin) ? HTTP_SUCCESS : HTTP_S_FALSE;
	}
	
	[ tag_name("LoginFailed") ]
	HTTP_CODE OnLoginFailed()
	{
		// Return HTTP_SUCCESS if the login failed
		return (m_fFailure.m_bLoginFailed) ? HTTP_SUCCESS : HTTP_S_FALSE;
	}

	[ tag_name("EmptyPassword") ]
	HTTP_CODE OnEmptyPassword()
	{
		// Return HTTP_SUCCESS if the password was invalid
		return (m_fFailure.m_bEmptyPassword) ? HTTP_SUCCESS : HTTP_S_FALSE;
	}

	[ tag_name("LoginName") ]
	HTTP_CODE OnLoginName()
	{
		// Respond with the login
		m_HttpResponse << m_strLogin;
		return HTTP_SUCCESS;
	}

	[ tag_name("PasswordRemembered") ]
	HTTP_CODE OnPasswordRemembered()
	{
		// Respond if we're remembering the password
		return m_bPasswordRemembered ? HTTP_SUCCESS : HTTP_S_FALSE;
	}

protected:
	bool ValidateFormData(const CHttpRequestParams& FormFields)
	{
		// Validate the login
		if (FormFields.Validate("login", &m_strLogin, 1, DB_MAX_STRLEN) != VALIDATION_S_OK ||
			!IsStringValid(m_strLogin))
		{
			m_fFailure.m_bFailure = true;
			m_fFailure.m_bEmptyLogin = true;
		}
		// Validate the password
		if (FormFields.Validate("password", &m_strPassword, 1, DB_MAX_STRLEN) != VALIDATION_S_OK ||
			!IsStringValid(m_strPassword, true, true, " "))
		{
			m_fFailure.m_bFailure = true;
			m_fFailure.m_bEmptyPassword = true;
		}
		
		return !m_fFailure.m_bFailure;
	}

	HTTP_CODE Login()
	{
		bool bRememberMe = (m_HttpRequest.GetFormVars().Lookup("rememberme") != NULL) ? true : false;

		// Get the login information
		CLoginUser loginUser;
		if(FAILED(StringCchCopy(loginUser.m_szLogin, DB_MAX_STRLEN, m_strLogin)))
			return HTTP_FAIL;
		
		HRESULT hr = loginUser.OpenRowset(m_dataConnection, FALSE);
		if (hr != S_OK)
			return DatabaseError("CLoginUser::OpenRowset()", hr);

		hr = loginUser.MoveFirst();
		if (hr != S_OK && hr != DB_S_ENDOFROWSET)
			return DatabaseError("CLoginUser::MoveFirst()", hr);
		
		// If we're comparing passwords, do it secretly
		if (hr == S_OK && (!m_bPasswordRemembered || !bRememberMe))
		{
			hr = CompareSecret((const BYTE*) (LPCSTR) m_strPassword, m_strPassword.GetLength(), loginUser.m_Salt, 
								loginUser.m_lSaltLen, loginUser.m_Password, loginUser.m_lPassLen);
		}

		// If hr is anything but S_OK, there's a failure
		if (hr != S_OK)
		{
			// Login failed
			m_fFailure.m_bLoginFailed = true;
			m_fFailure.m_bFailure = true;
		}

		// If the user authenticated or they disabled the remember me feature
		if (!m_fFailure.m_bLoginFailed || (m_bPasswordRemembered && !bRememberMe))
		{
			// Update the remember password flag and identifier
			CUpdateRememberPass updateRemember;
			if(FAILED(StringCchCopy(updateRemember.m_szLogin, DB_MAX_STRLEN, m_strLogin)))
				return HTTP_FAIL;
			CoCreateGuid(&updateRemember.m_guidRememberID);
			updateRemember.m_bRememberPassword = (bRememberMe) ? VARIANT_TRUE : VARIANT_FALSE;
			hr = updateRemember.OpenRowset(m_dataConnection, NULL);
			if (hr != S_OK)
				return DatabaseError("CUpdateRememberPass::OpenRowset()", hr);
			
			// Re-write the persistant cookie
			if (bRememberMe)
				WritePersistCookie(m_strLogin, loginUser.m_szFirstName, updateRemember.m_guidRememberID);
			else
				WritePersistCookie("!!!!", "!!!!", GUID_NULL);
		}

		// We're no longer remembering password (because of failure, or we're redirecting because of success)
		m_bPasswordRemembered = false;

		// If the user authenticated properly, lets create a session and redirect
		if (!m_fFailure.m_bLoginFailed)
		{
			// Give the user a session
			HTTP_CODE hc = UpdateUserSession(m_strLogin, loginUser.m_szFirstName, loginUser.m_szLastName, loginUser.m_lUserID);
			if (hc != HTTP_SUCCESS)
				return hc;
					
			// Redirect to the main page
			m_HttpResponse.Redirect("main.srf");
			return HTTP_SUCCESS;
		}
		
		return HTTP_SUCCESS;
	}

	HTTP_CODE UpdateUserSession(LPCTSTR lpszLogin, LPCTSTR lpszFirstName, LPCTSTR lpszLastNameName, long lUserID)
	{
		HRESULT hr;

		// Attempt to find a current session
		MantaWeb::CSessionData sessionData;

		sessionData.m_lUserID = lUserID;
		hr = sessionData.OpenRowset(m_dataConnection, NULL);
		if (hr != S_OK)
			return DatabaseError("CSessionData::OpenRowset()", hr);

		hr = sessionData.MoveFirst();
		if (hr != S_OK && hr != DB_S_ENDOFROWSET)
			return DatabaseError("CSessionData::MoveFirst()", hr);

		// If there is an existing session, remove it
		if (hr != DB_S_ENDOFROWSET)
		{
			// Remove the session
			MantaWeb::CRemoveSessionData removeSession;
			memcpy(&removeSession.m_guidSessionID, &sessionData.m_guidSessionID, sizeof(GUID));

			// Close the session data so we can update the table
			sessionData.Close();

			hr = removeSession.OpenRowset(m_dataConnection, NULL);
			if (hr != S_OK)
				return DatabaseError("CRemoveSessionData::OpenRowset()", hr);
		}
		else
			// Close the session data so we can update the table
			sessionData.Close();
		

		// Insert a new session
		MantaWeb::CInsertSession insertSession;
		insertSession.m_lUserID = lUserID;

		hr = insertSession.OpenRowset(m_dataConnection, NULL);
		if (hr != S_OK)
			return DatabaseError("CInsertSession::OpenRowset()", hr);

		insertSession.Close();
		
		// Get the generated session id (GUID)
		hr = sessionData.OpenRowset(m_dataConnection, NULL);
		if (hr != S_OK)
			return DatabaseError("CSessionData::OpenRowset()", hr);

		hr = sessionData.MoveFirst();
		if (hr != S_OK)
			return DatabaseError("CSessionData::MoveFirst()", hr);				
		
		// Write the session cookie
		WriteSessionCookie(lpszLogin, lpszFirstName, lpszLastNameName, lUserID, sessionData.m_guidSessionID);
		return HTTP_SUCCESS;		
	}

	HTTP_CODE RedirectToMessage(const CHttpRequestParams& QueryFields)
	{
		LONG userID;
		GUID sessionID;
		QueryFields.Exchange("uid", &userID);
		QueryFields.Exchange("sid", &sessionID);

		// Validate the session
		if (ValidateSession(userID, sessionID))
		{
			// Get the user data for the session cookie
			CUserSessionData sessionData;
			sessionData.m_lUserID = userID;
			HRESULT hr = sessionData.OpenRowset(m_dataConnection, NULL);
			if (hr != S_OK)
				return DatabaseError("CUserSessionData::OpenRowset()", hr);
			hr = sessionData.MoveFirst();
			if (hr != S_OK)
				return DatabaseError("CUserSessionData::MoveFirst()", hr);
			
			// Create the session cookie
			WriteSessionCookie(sessionData.m_szLogin, sessionData.m_szFirstName, sessionData.m_szLastName, userID, sessionID);
			// Load the html from resource
			CString strHTML;
			LoadHtmlFromResource(MAKEINTRESOURCE(IDR_MAILREDIRECT), strHTML);

			// Replace the message id and send back the response
			strHTML.Replace("[MSGID]", QueryFields.Lookup("msgid"));
			m_HttpResponse << strHTML;
			return HTTP_SUCCESS_NO_PROCESS;
		}
			
		// Session not valid, send them to the login page (get rid of query params)
		m_HttpResponse.Redirect("login.srf");
		return HTTP_SUCCESS_NO_PROCESS;
	}

}; // class CLoginHandler
