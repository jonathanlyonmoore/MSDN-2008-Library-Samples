// ProfileHandler.h : Defines the ATL Server request handler class
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#pragma once

#include "ProfileDB.h"

namespace Profile
{
    typedef struct _FailureFlags
	{
		bool m_bFailure;			// General failure
		bool m_bInvalidFirstName;	// Invalid first name
		bool m_bInvalidLastName;	// Invalid last name
		bool m_bInvalidEmail;		// Invalid email
		bool m_bInvalidPassword;	// Invalid password
		bool m_bPasswordMismatch;	// Passwords didn't match
	} FailureFlags;
}

// class CProfileHandler
// This handler displays and updates the user profile.
// Note: POP passwords are not encrypted.
[ request_handler("Profile") ]
class CProfileHandler : public CMantaWebBase<CProfileHandler>
{
private:
	CStringA m_strFirstName;			// First name in the user's profile
	CStringA m_strLastName;				// Last name in the user's profile
	CStringA m_strEmail;				// User's email address in the user's profile
	CStringA m_strPassword1;			// First password user enters
	CStringA m_strPassword2;			// Second password user enters
	Profile::FailureFlags m_fFailure;	// Failure flags
	bool m_bProfileChanged;				// Boolean to specify if the profile update succeeded

public:
	HTTP_CODE ValidateAndExchange()
	{
		// Set the content type to html
		m_HttpResponse.SetContentType("text/html");
		m_HttpResponse.SetExpires(0);

		// Validate the session
		if (!ValidateSession())
			return ValidationError();

		// Clear the flags
		memset(&m_fFailure, false, sizeof(Profile::FailureFlags));
		m_bProfileChanged = false;

		const CHttpRequestParams& FormFields = m_HttpRequest.GetFormVars();
		// If all the form fields are present
		if (FormFields.Lookup("firstname") != NULL && FormFields.Lookup("lastname") != NULL &&
			FormFields.Lookup("email") != NULL && FormFields.Lookup("password1") != NULL &&
			FormFields.Lookup("password2") != NULL)
		{
			// Validate the form data
			if (!ValidateFormData(FormFields))
				return HTTP_SUCCESS;

			// Update the profile
			return UpdateProfile();
		}
		else
		{
			// Get the user profile
			CUserProfile m_userProfile;
			GetUserID(&m_userProfile.m_lUserID);
			HRESULT hr;
			hr = m_userProfile.OpenRowset(m_dataConnection, NULL);
			if (hr != S_OK)
				return DatabaseError("CUserProfile::OpenRowset()", hr);
			hr = m_userProfile.MoveFirst();
			if (hr != S_OK)
				return DatabaseError("CUserProfile::MoveFirst()", hr);
			
			// Set the member data
			m_strFirstName = m_userProfile.m_szFirstName;
			m_strLastName = m_userProfile.m_szLastName;
			m_strEmail = m_userProfile.m_szEmail;
		}
		return HTTP_SUCCESS;
	}

	[ tag_name("UpdateSuccessful") ]
	HTTP_CODE OnUpdateSuccessful()
	{
		// Return HTTP_SUCCESS if the profile was updated successfully
		return (m_bProfileChanged) ? HTTP_SUCCESS : HTTP_S_FALSE;
	}

	[ tag_name("Failure") ]
	HTTP_CODE OnFailure()
	{
		// Return HTTP_SUCCESS if there was a failure
		return (m_fFailure.m_bFailure) ? HTTP_SUCCESS : HTTP_S_FALSE;
	}

	[ tag_name("InvalidFirstName") ]
	HTTP_CODE OnInvalidFirstName()
	{
		// Return HTTP_SUCCESS if the first name is invalid
		return (m_fFailure.m_bInvalidFirstName) ? HTTP_SUCCESS : HTTP_S_FALSE;
	}

	[ tag_name("InvalidLastName") ]
	HTTP_CODE OnInvalidLastName()
	{
		// Return HTTP_SUCCESS if the last name is invalid
		return (m_fFailure.m_bInvalidLastName) ? HTTP_SUCCESS : HTTP_S_FALSE;
	}

	[ tag_name("InvalidEmail") ]
	HTTP_CODE OnInvalidEmail()
	{
		// Return HTTP_SUCCESS if the email address is invalid
		return (m_fFailure.m_bInvalidEmail) ? HTTP_SUCCESS : HTTP_S_FALSE;
	}

	[ tag_name("InvalidPassword") ]
	HTTP_CODE OnInvalidPassword()
	{
		// Return HTTP_SUCCESS if the password is invalid
		return (m_fFailure.m_bInvalidPassword) ? HTTP_SUCCESS : HTTP_S_FALSE;
	}

	[ tag_name("PasswordMismatch") ]
	HTTP_CODE OnPasswordMismatch()
	{
		// Return HTTP_SUCCESS if the passwords do not match
		return (m_fFailure.m_bPasswordMismatch) ? HTTP_SUCCESS : HTTP_S_FALSE;
	}

	[ tag_name("MaxStringLen") ]
	HTTP_CODE OnMaxStringLen()
	{
		// Return the maximum number of characters for a string
		m_HttpResponse << DB_MAX_STRLEN;
		return HTTP_SUCCESS;
	}

	[ tag_name("FirstName") ]
	HTTP_CODE OnFirstName()
	{
		// Respond with the first name
		m_HttpResponse << m_strFirstName;
		return HTTP_SUCCESS;
	}

	[ tag_name("LastName") ]
	HTTP_CODE OnLastName()
	{
		// Respond with the last name
		m_HttpResponse << m_strLastName;
		return HTTP_SUCCESS;
	}

	[ tag_name("Email") ]
	HTTP_CODE OnEmail()
	{
		// Respond with the email address
		m_HttpResponse << m_strEmail;
		return HTTP_SUCCESS;
	}

	[ tag_name("Password1") ]
	HTTP_CODE OnPassword1()
	{
		// Respond with the first password
		m_HttpResponse << m_strPassword1;
		return HTTP_SUCCESS;
	}

	[ tag_name("Password2") ]
	HTTP_CODE OnPassword2()
	{
		// Respond with the second password
		m_HttpResponse << m_strPassword2;
		return HTTP_SUCCESS;
	}

protected:
	bool ValidateFormData(const CHttpRequestParams& FormFields)
	{
		// Validate the first name
		if (FormFields.Validate("firstname", &m_strFirstName, 1, DB_MAX_STRLEN) != VALIDATION_S_OK ||
			!IsStringValid(m_strFirstName, true, false, " "))
		{
			m_fFailure.m_bFailure = true;
			m_fFailure.m_bInvalidFirstName = true;
		}
		// Validate the last name
		if (FormFields.Validate("lastname", &m_strLastName, 1, DB_MAX_STRLEN) != VALIDATION_S_OK ||
			!IsStringValid(m_strLastName, true, false, " "))
		{
			m_fFailure.m_bFailure = true;
			m_fFailure.m_bInvalidLastName = true;
		}
		// Validate the email address
		if (FormFields.Validate("email", &m_strEmail, 1, DB_MAX_STRLEN) != VALIDATION_S_OK ||
			m_strEmail.Find("@", 0) == -1 || !IsStringValid(m_strEmail, true, true, ".@_"))
		{
			m_fFailure.m_bFailure = true;
			m_fFailure.m_bInvalidEmail = true;
		}
		// Validate the first password
		DWORD dwRet = FormFields.Validate("password1", &m_strPassword1, 0, DB_MAX_STRLEN);
		if ( (dwRet != VALIDATION_S_OK && dwRet != VALIDATION_S_EMPTY) || !IsStringValid(m_strPassword1, true, true, " "))
		{
			m_fFailure.m_bFailure = true;
			m_fFailure.m_bInvalidPassword = true;
		}
		// Get the second password, and if the first password is there, match against it
		dwRet = FormFields.Exchange("password2", &m_strPassword2);
		if ( ( (dwRet != VALIDATION_S_OK && dwRet != VALIDATION_S_EMPTY) || m_strPassword1 != m_strPassword2 ) && 
			!m_fFailure.m_bInvalidPassword)
		{
			m_fFailure.m_bFailure = true;
			m_fFailure.m_bPasswordMismatch = true;
		}
		return !m_fFailure.m_bFailure;
	}

	HTTP_CODE UpdateProfile()
	{
		CUpdateProfile updateProfile;
		HRESULT hr;

		// Copy the data into the profile
		if(FAILED(StringCchCopy(updateProfile.m_szFirstName, DB_MAX_STRLEN, m_strFirstName)))
			return HTTP_FAIL;
		if(FAILED(StringCchCopy(updateProfile.m_szLastName, DB_MAX_STRLEN, m_strLastName)))
			return HTTP_FAIL;
		if(FAILED(StringCchCopy(updateProfile.m_szEmail, DB_MAX_STRLEN, m_strEmail)))
			return HTTP_FAIL;
		GetUserID(&updateProfile.m_lUserID);

		// Update the user's profile
		hr = updateProfile.OpenRowset(m_dataConnection, NULL);
		if (hr != S_OK)
			return DatabaseError("CUpdateProfile::OpenRowset()", hr);
		updateProfile.Close();

		// If we need to update the password
		if (m_strPassword1 != "")
		{
			CUpdatePassword updatePassword;
			
			updatePassword.m_lUserID = updateProfile.m_lUserID;

			// Don't store the real password. only store a one-way encrypted hash
			updatePassword.m_lSaltLen = 4;
			updatePassword.m_lPassLen = DB_MAX_STRLEN;

			hr = HashSecret((const BYTE*) (LPCSTR) m_strPassword1, m_strPassword1.GetLength(),
							updatePassword.m_Salt, (DWORD &) updatePassword.m_lSaltLen, updatePassword.m_Password, 
							(DWORD &) updatePassword.m_lPassLen);
		
			// Update the password
			hr = updatePassword.OpenRowset(m_dataConnection, NULL);
			if (hr != S_OK)
				return DatabaseError("CUpdatePassword::OpenRowset()", hr);

			// Reset the passwords
			m_strPassword1 = m_strPassword2 = "";
		}
		// Rewrite the cookies, and set changed to true
		m_bProfileChanged = true;

		// Note: the GetLogin(), GetUserID(), and GetSessionID() will be succesful on a valid session (session already validated)
		WriteSessionCookie(GetLogin(), m_strFirstName, m_strLastName, GetUserID(), GetSessionID());
		return HTTP_SUCCESS;
	}


}; // class CSessionHandler
