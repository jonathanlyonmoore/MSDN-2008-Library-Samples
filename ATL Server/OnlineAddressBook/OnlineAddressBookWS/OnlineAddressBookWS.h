// OnlineAddressBookWS.h : Defines the ATL Server request handler class
//
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Classes Reference and related electronic
// documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft C++ Libraries products.

#pragma once

static inline void BSTR2TSTR( BSTR bstr, LPTSTR lptstr, int tstrlen )
{
#ifdef _MBCS
	WideCharToMultiByte( _AtlGetConversionACP(), 0, bstr, -1, lptstr, tstrlen, NULL, NULL );
#elif defined _UNICODE
	wcsncpy( lptstr, bstr, tstrlen );
#else 
#error You have to define either _MBCS or _UNICODE to run this code
#endif
	lptstr[tstrlen - 1] = _T('\0');
}

namespace OnlineAddressBookWSService
{
const wchar_t MYDATASOURCE[] = L"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\\OnlineAddressBookWS.mdb;Persist Security Info=False";

#define OABWS_UNEXPECTED_ERROR 1
#define OABWS_ERROR_INVALID_CONTACTNAME 2
#define OABWS_ERROR_USERID_ALREADY_EXISTS 3

// all struct, enum, and typedefs for your webservice should go inside the namespace
/*
	struct SessionIDWrap{
		BSTR SessionID;
	};

*/	
	struct ContactItem{
		LONG m_ContactID;
		BSTR m_Birthday;
		BSTR m_BusinessCity;
		BSTR m_BusinessCountry;
		BSTR m_BusinessFax;
		BSTR m_BusinessPhone;
		BSTR m_BusinessPostalCode;
		BSTR m_BusinessState;
		BSTR m_BusinessStreet;
		BSTR m_Company;
		BSTR m_Department;
		BSTR m_EmailAddress;
		BSTR m_FirstName;
		BSTR m_HomeCity;
		BSTR m_HomeCountry;
		BSTR m_HomeFax;
		BSTR m_HomePhone;
		BSTR m_HomePostalCode;
		BSTR m_HomeState;
		BSTR m_HomeStreet;
		BSTR m_JobTitle;
		BSTR m_LastName;
		BSTR m_MiddleName;
		BSTR m_MobilePhone;
		BSTR m_OtherPhone;
		BSTR m_Suffix;
		BSTR m_Title;
		BSTR m_WebPage;
	};

// IOnlineAddressBookWSService - web service interface declaration
//
[
	uuid("97F8B70E-8A60-43DE-9198-23DEE5E4DB92"), 
	object
]
__interface IOnlineAddressBookWSService
{
	// HelloWorld is a sample ATL Server web service method.  It shows how to
	// declare a web service method and its in-parameters and out-parameters
 	[id(1)] HRESULT LogOn([in] BSTR email,[in] BSTR password); 
	HRESULT LogOff();
	HRESULT DeleteAllContacts();
	HRESULT GetAddressBookEntry([in] LONG lContactID,[out, retval] ContactItem *theContactItem);
	HRESULT GetContactList([out] int *arrSize,[out,size_is(*arrSize)] BSTR **NameList,[out,size_is(*arrSize)] LONG **IDList);
	HRESULT CreateAccount([in] BSTR strEmail, [in] BSTR strPassword,[in] BSTR strFirstName,[in] BSTR strLastName);
	HRESULT InsertContact([in] ContactItem newContactItem);
	// TODO: Add additional web service methods here
};


// OnlineAddressBookWSService - web service implementation
//
[
	request_handler(name="Default", sdl="GenOnlineAddressBookWSWSDL"),
	soap_handler(
		name="OnlineAddressBookWSService", 
		namespace="urn:OnlineAddressBookWSService",
		protocol="soap",style="document",use="literal"
	)
]
class COnlineAddressBookWSService : 
	public IOnlineAddressBookWSService
{
public:
	
    BSTR m_SessionID;

	// uncomment the service declaration(s) if you want to use
	// a service that was generated with your ISAPI extension
	CComPtr<ISessionStateService> m_spSessionSvc;
	CComPtr<ISession>	m_spSession;
	CDataConnection m_dc;


	HTTP_CODE InitializeHandler(AtlServerRequest *pRequestInfo, IServiceProvider *pProvider)
	{
		if (HTTP_SUCCESS != CSoapHandler<COnlineAddressBookWSService>::InitializeHandler(pRequestInfo, pProvider))
			return HTTP_FAIL;

		// Get the ISessionStateService from the ISAPI extension
		if (FAILED(pProvider->QueryService(__uuidof(ISessionStateService),
						__uuidof(ISessionStateService), (void **)&m_spSessionSvc)))
			return HTTP_FAIL;

		// Uncomment the following code to retrieve a data source
		// connection from the data source cache. Replace connection_name
		// with a string used to identify the connection and
		// connection_string with an OLEDB connection string
		// which is valid for your data source. This code assumes that
		// the service provider pointed to by m_spServiceProvider
		// can provide an IDataSourceCache pointer to a data source
		// cache service (usually implemented in the ISAPI extension).

		if (S_OK != GetDataSource(  pProvider,
									_T("onlinecatalog"),
									MYDATASOURCE ,
									&m_dc )){

			ATLTRACE(_T("GetDataSouce Failed.  Make sure MYDATASOUCE is properly defined"));
			return HTTP_FAIL;

									}
		return HTTP_SUCCESS;
	}
protected:


	[ soap_method ]
	[ soap_header(value="m_SessionID",required=true,in=true,out=false) ]
	HRESULT LogOff()
	{
		_ATLTRY {
			if(S_OK!=m_spSessionSvc->CloseSession(CW2A(m_SessionID))) {
				return E_UNEXPECTED;
			}
			else {
				return S_OK;
			}
		}
		_ATLCATCH( e ) {
			return HRESULT( e );
		}
	}

	[ soap_method ]
	[ soap_header(value="m_SessionID",required=true,in=true,out=false) ]
	HRESULT DeleteAllContacts()
	{
		CDeleteAllContacts DeleteAllContacts;
		if(S_OK!=DeleteAllContacts.Open(m_dc)) {
			return E_UNEXPECTED;
		}
		else {
			return S_OK;
		}
	}

	[ soap_method ]
	[ soap_header(value="m_SessionID",required=true,in=false,out=true) ]
	HRESULT LogOn(/*[in]*/ BSTR strEmail, /*[in]*/ BSTR strPassword)
	{
		CLoginUser loginUser;
		BSTR2TSTR(strEmail, loginUser.m_param_szEmail, DB_MAX_STRLEN);
		
		HRESULT hr = loginUser.OpenRowset(m_dc, FALSE);
		if (FAILED(hr)) {
			// Returning a special error for "user name not
			// found" allows malicious users to verify the
			// existing user names - so always return an
			// ambiguous error.
			return E_ACCESSDENIED;
		}

		hr = loginUser.MoveFirst();
		if (FAILED(hr))	{
			loginUser.Close();
			return E_ACCESSDENIED;
		}

		// Check if the passwords match ...
		hr = CompareSecret(
			(const BYTE*)  strPassword, CComBSTR(strPassword).Length(),
			loginUser.m_Salt, (DWORD)loginUser.m_lSaltLen,
			loginUser.m_Password, (DWORD)loginUser.m_lPassLen);

		if(FAILED(hr)) {
			return E_ACCESSDENIED;
		}

		//If we're here user was validated
		char newSessionID[MAX_SESSION_KEY_LEN];
		DWORD temp=MAX_SESSION_KEY_LEN;
		hr = m_spSessionSvc->CreateNewSession(newSessionID,&temp,&m_spSession);
		if(FAILED(hr)) {
			return hr;
		}

		hr = m_spSession->SetVariable("userID",CComVariant(loginUser.m_lUserID));
		if ( FAILED(hr) ) {
			return hr;
		}

		CComBSTR bstrOut(newSessionID);
		m_SessionID=bstrOut.Detach();

		if (m_SessionID == NULL) {
			return E_OUTOFMEMORY;
		}

		return S_OK;
	}
	//	TODO: Add additional web service methods here

	[ soap_method ]
	[ soap_header(value="m_SessionID",required=true,in=true,out=true) ]
	HRESULT GetAddressBookEntry(/*[in]*/LONG lContactID,/*[out, retval]*/ ContactItem *theContactItem){
		CSelectContact SelectContact;
		HRESULT hr=E_FAIL;

		_ATLTRY {
			hr=m_spSessionSvc->GetSession(CW2A(m_SessionID),&m_spSession);
		}
		_ATLCATCH( e ) {
			return HRESULT(e);
		}

		if(FAILED(hr)) {
			return E_UNEXPECTED;
		}

		CComVariant userId;
		hr=m_spSession->GetVariable("userID",&userId);
		if(FAILED(hr)) {
			return E_UNEXPECTED;
		}

		SelectContact.m_lUserID=userId.lVal;
		SelectContact.m_lContactID=lContactID;

		if(FAILED(SelectContact.Open(m_dc))) {
			return E_UNEXPECTED;
		}

		if(FAILED(SelectContact.MoveFirst())) {
			SelectContact.Close();
			return MAKE_HRESULT(1,FACILITY_ITF,OABWS_ERROR_INVALID_CONTACTNAME);
		}

		//got it
		theContactItem->m_ContactID=lContactID;

		CComBSTR temp(SelectContact.m_Birthday);
		theContactItem->m_Birthday=temp.Detach();

		temp=SelectContact.m_BusinessCity;
		theContactItem->m_BusinessCity=temp.Detach();

		temp=SelectContact.m_BusinessCountry;
		theContactItem->m_BusinessCountry=temp.Detach();

		temp=SelectContact.m_BusinessFax;
		theContactItem->m_BusinessFax=temp.Detach();

		temp=SelectContact.m_BusinessPhone;
		theContactItem->m_BusinessPhone=temp.Detach();

		temp=SelectContact.m_BusinessPostalCode;
		theContactItem->m_BusinessPostalCode=temp.Detach();

		temp=SelectContact.m_BusinessState;
		theContactItem->m_BusinessState=temp.Detach();

		temp=SelectContact.m_BusinessStreet;
		theContactItem->m_BusinessStreet=temp.Detach();

		temp=SelectContact.m_Company;
		theContactItem->m_Company=temp.Detach();

		temp=SelectContact.m_Department;
		theContactItem->m_Department=temp.Detach();

		temp=SelectContact.m_EmailAddress;
		theContactItem->m_EmailAddress=temp.Detach();

		temp=SelectContact.m_FirstName;
		theContactItem->m_FirstName=temp.Detach();

		temp=SelectContact.m_HomeCity;
		theContactItem->m_HomeCity=temp.Detach();

		temp=SelectContact.m_HomeCountry;
		theContactItem->m_HomeCountry=temp.Detach();

		temp=SelectContact.m_HomeFax;
		theContactItem->m_HomeFax=temp.Detach();

		temp=SelectContact.m_HomePhone;
		theContactItem->m_HomePhone=temp.Detach();

		temp=SelectContact.m_HomePostalCode;
		theContactItem->m_HomePostalCode=temp.Detach();

		temp=SelectContact.m_HomeState;
		theContactItem->m_HomeState=temp.Detach();

		temp=SelectContact.m_HomeStreet;
		theContactItem->m_HomeStreet=temp.Detach();

		temp=SelectContact.m_JobTitle;
		theContactItem->m_JobTitle=temp.Detach();

		temp=SelectContact.m_LastName;
		theContactItem->m_LastName=temp.Detach();

		temp=SelectContact.m_MiddleName;
		theContactItem->m_MiddleName=temp.Detach();

		temp=SelectContact.m_MobilePhone;
		theContactItem->m_MobilePhone=temp.Detach();

		temp=SelectContact.m_OtherPhone;
		theContactItem->m_OtherPhone=temp.Detach();

		temp=SelectContact.m_Suffix;
		theContactItem->m_Suffix=temp.Detach();

		temp=SelectContact.m_Title;
		theContactItem->m_Title=temp.Detach();

		temp=SelectContact.m_WebPage;
		theContactItem->m_WebPage=temp.Detach();

		SelectContact.Close();

		return S_OK;
	}

	[ soap_method ]
	[ soap_header(value="m_SessionID",required=true,in=true,out=false) ]
	HRESULT GetContactList(int *arrSize,BSTR **NameList, LONG **IDList){
		CSelectContacts SelectContacts;
		HRESULT hr;

		_ATLTRY {
			hr=m_spSessionSvc->GetSession(CW2A(m_SessionID),&m_spSession);
		}
		_ATLCATCH( e ) {
			return HRESULT(e);
		}

		if(hr!=S_OK) {
			return S_FALSE;
		}

		CComVariant userId;
		hr=m_spSession->GetVariable("userID",&userId);
		if(FAILED(hr)) {
			return E_UNEXPECTED;
		}

		SelectContacts.m_lUserID=userId.lVal;
		if(FAILED(hr=SelectContacts.Open(m_dc))) {
			return hr;
		}

		if(FAILED(hr=SelectContacts.MoveFirst())){
			SelectContacts.Close();
			return hr;
		}

		CAtlArray<CComBSTR> names;
		CAtlArray<LONG> IDs;

		CString tempName;

		_ATLTRY {
			tempName.Format(_T("%s %s"),SelectContacts.m_FirstName,SelectContacts.m_LastName);
			names.Add(tempName.GetBuffer());
			IDs.Add(SelectContacts.m_ContactID);
		}
		_ATLCATCH( e ) {
			return HRESULT(e);
		}

		while(SelectContacts.MoveNext()==S_OK){
			_ATLTRY {
				tempName.Format(_T("%s %s"),SelectContacts.m_FirstName,SelectContacts.m_LastName);
				names.Add(tempName.GetBuffer());
				IDs.Add(SelectContacts.m_ContactID);
			}
			_ATLCATCH( e ) {
				return HRESULT(e);
			}
		}

		SelectContacts.Close();

		*arrSize=(int)names.GetCount();

		if ((unsigned int)*arrSize > sizeof(BSTR)*(*arrSize) ||
		    (unsigned int)*arrSize > sizeof(LONG)*(*arrSize)) {
			return E_OUTOFMEMORY;
		}

		*NameList=(BSTR*) GetMemMgr()->Allocate(sizeof(BSTR)*(*arrSize));
		*IDList=(LONG*) GetMemMgr()->Allocate(sizeof(LONG)*(*arrSize));

		if (NameList == NULL || IDList == NULL) {
			return E_OUTOFMEMORY;
		}

		for(int i=0;i<*arrSize;i++){
			(*IDList)[i]=IDs.GetAt(i);
			(*NameList)[i]=(names.GetAt(i)).Detach();
		}

		return S_OK;
	}
	
	[soap_method]
	HRESULT CreateAccount(BSTR strEmail, BSTR strPassword,BSTR strFirstName, BSTR strLastName){
		CAddUser addUser;
		HRESULT hr;

		BSTR2TSTR(strFirstName, addUser.m_szFirstName,	DB_MAX_STRLEN);
		BSTR2TSTR(strLastName,	addUser.m_szLastName,	DB_MAX_STRLEN);
		BSTR2TSTR(strEmail,		addUser.m_szEmail,		DB_MAX_STRLEN);

		DWORD dwSaltLen = 4;
		DWORD dwPasswordLen = DB_MAX_STRLEN;
		hr = HashSecret(
			(const BYTE*) (LPCSTR) strPassword, CComBSTR(strPassword).Length(),
			addUser.m_Salt, dwSaltLen,
			addUser.m_Password, dwPasswordLen);

		if (FAILED(hr)) {
			return hr;
		}
		
		addUser.m_lSaltLen=dwSaltLen;
		addUser.m_lPassLen=dwPasswordLen;

		hr = addUser.OpenRowset(m_dc, NULL);
		if (FAILED(hr)) {
			return MAKE_HRESULT(1,FACILITY_ITF,OABWS_ERROR_USERID_ALREADY_EXISTS);
		}

		return S_OK;
	}

	[soap_method]
	[ soap_header(value="m_SessionID",required=true,in=true,out=false) ]
	HRESULT InsertContact(ContactItem newContactItem){

		HRESULT hr;
		_ATLTRY {
			hr=m_spSessionSvc->GetSession(CW2A(m_SessionID),&m_spSession);
		}
		_ATLCATCH( e ) {
			return HRESULT(e);
		}

		if(hr!=S_OK) {
			return E_UNEXPECTED;
		}

		CComVariant userId;
		hr=m_spSession->GetVariable("userID",&userId);
		if(FAILED(hr)) {
			return E_UNEXPECTED;
		}
		
		CInsertContact icSP;

		icSP.m_UserID=userId.lVal;

		BSTR2TSTR(newContactItem.m_Birthday,			icSP.m_Birthday,			DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_BusinessCity,		icSP.m_BusinessCity,		DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_BusinessCountry,		icSP.m_BusinessCountry,		DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_BusinessFax,			icSP.m_BusinessFax,			DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_BusinessPhone,		icSP.m_BusinessPhone,		DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_BusinessPostalCode,	icSP.m_BusinessPostalCode,	DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_BusinessState,		icSP.m_BusinessState,		DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_BusinessStreet,		icSP.m_BusinessStreet,		DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_Company,				icSP.m_Company,				DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_Department,			icSP.m_Department,			DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_EmailAddress,		icSP.m_EmailAddress,		DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_FirstName,			icSP.m_FirstName,			DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_HomeCity,			icSP.m_HomeCity,			DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_HomeCountry,			icSP.m_HomeCountry,			DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_HomeFax,				icSP.m_HomeFax,				DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_HomePhone,			icSP.m_HomePhone,			DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_HomePostalCode,		icSP.m_HomePostalCode,		DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_HomeState,			icSP.m_HomeState,			DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_HomeStreet,			icSP.m_HomeStreet,			DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_JobTitle,			icSP.m_JobTitle,			DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_LastName,			icSP.m_LastName,			DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_MiddleName,			icSP.m_MiddleName,			DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_MobilePhone,			icSP.m_MobilePhone,			DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_OtherPhone,			icSP.m_OtherPhone,			DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_Suffix,				icSP.m_Suffix,				DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_Title,				icSP.m_Title,				DB_MAX_CONTACT_FIELDLEN);
		BSTR2TSTR(newContactItem.m_WebPage,				icSP.m_WebPage,				DB_MAX_CONTACT_FIELDLEN);

		hr=icSP.Open(m_dc);
		if(SUCCEEDED(hr)) {
			icSP.Close();
		}
			
		return hr;
	}



	HRESULT GenerateAppError(IWriteStream *pStream, HRESULT hr){

		if (pStream == NULL)
		{
			return E_INVALIDARG;
		}

		LPWSTR pwszMessage = NULL;

		switch(hr){
			case E_UNEXPECTED:
				pwszMessage = L"An Unexpected Application Error Occurred";
				break;
			case E_ACCESSDENIED:
				pwszMessage = L"Invalid user name or password";
				break;
			case MAKE_HRESULT(1,FACILITY_ITF,OABWS_ERROR_INVALID_CONTACTNAME):
				pwszMessage = L"The contact name you're looking for doesn't exist";
				break;
			case MAKE_HRESULT(1,FACILITY_ITF,OABWS_ERROR_USERID_ALREADY_EXISTS):
				pwszMessage = L"Accout creation failed, possible causes include that email address being in use already and lack of write permissions on the DB";
				break;
			default:
				pwszMessage = L"Something quite misterious happened";
		}
			
		hr = SoapFault(SOAP_E_SERVER, pwszMessage, -1);
		return hr;
	}
	
}; // class COnlineAddressBookWSService

} // namespace OnlineAddressBookWSService
