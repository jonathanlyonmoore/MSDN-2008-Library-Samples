﻿//Copyright (c) Microsoft Corporation.  All rights reserved.

//------------------------------------------------------------------------------
// <autogenerated>
//     This code was generated by a tool.
//     Runtime Version: 1.0.2907.0
//
//     Changes to this file may cause incorrect behavior and will be lost if 
//     the code is regenerated.
// </autogenerated>
//------------------------------------------------------------------------------

namespace OnlineAddressBookCsharpClient.OnlineAddressBookServer {
    using System.Diagnostics;
    using System.Xml.Serialization;
    using System;
    using System.Web.Services.Protocols;
    using System.Web.Services;
    
    
    [System.Web.Services.WebServiceBindingAttribute(Name="OnlineAddressBookWSServiceSoap", Namespace="urn:OnlineAddressBookWSService")]
    public class OnlineAddressBookWSService : System.Web.Services.Protocols.SoapHttpClientProtocol {
        
        public @string m_SessionID;
        
        [System.Diagnostics.DebuggerStepThroughAttribute()]
        public OnlineAddressBookWSService() {
            this.Url = "http://localhost/OnlineAddressBookWS/OnlineAddressBookWS.dll?Handler=Default";
        }
        
        [System.Web.Services.Protocols.SoapHeaderAttribute("m_SessionID")]
        [System.Diagnostics.DebuggerStepThroughAttribute()]
        [System.Web.Services.Protocols.SoapDocumentMethodAttribute("#LogOff", RequestNamespace="urn:OnlineAddressBookWSService", ResponseNamespace="urn:OnlineAddressBookWSService", Use=System.Web.Services.Description.SoapBindingUse.Literal, ParameterStyle=System.Web.Services.Protocols.SoapParameterStyle.Wrapped)]
        public void LogOff() {
            this.Invoke("LogOff", new object[0]);
        }
        
        [System.Diagnostics.DebuggerStepThroughAttribute()]
        public System.IAsyncResult BeginLogOff(System.AsyncCallback callback, object asyncState) {
            return this.BeginInvoke("LogOff", new object[0], callback, asyncState);
        }
        
        [System.Diagnostics.DebuggerStepThroughAttribute()]
        public void EndLogOff(System.IAsyncResult asyncResult) {
            this.EndInvoke(asyncResult);
        }
        
        [System.Web.Services.Protocols.SoapHeaderAttribute("m_SessionID")]
        [System.Diagnostics.DebuggerStepThroughAttribute()]
        [System.Web.Services.Protocols.SoapDocumentMethodAttribute("#DeleteAllContacts", RequestNamespace="urn:OnlineAddressBookWSService", ResponseNamespace="urn:OnlineAddressBookWSService", Use=System.Web.Services.Description.SoapBindingUse.Literal, ParameterStyle=System.Web.Services.Protocols.SoapParameterStyle.Wrapped)]
        public void DeleteAllContacts() {
            this.Invoke("DeleteAllContacts", new object[0]);
        }
        
        [System.Diagnostics.DebuggerStepThroughAttribute()]
        public System.IAsyncResult BeginDeleteAllContacts(System.AsyncCallback callback, object asyncState) {
            return this.BeginInvoke("DeleteAllContacts", new object[0], callback, asyncState);
        }
        
        [System.Diagnostics.DebuggerStepThroughAttribute()]
        public void EndDeleteAllContacts(System.IAsyncResult asyncResult) {
            this.EndInvoke(asyncResult);
        }
        
        [System.Web.Services.Protocols.SoapHeaderAttribute("m_SessionID", Direction=System.Web.Services.Protocols.SoapHeaderDirection.Out)]
        [System.Diagnostics.DebuggerStepThroughAttribute()]
        [System.Web.Services.Protocols.SoapDocumentMethodAttribute("#LogOn", RequestNamespace="urn:OnlineAddressBookWSService", ResponseNamespace="urn:OnlineAddressBookWSService", Use=System.Web.Services.Description.SoapBindingUse.Literal, ParameterStyle=System.Web.Services.Protocols.SoapParameterStyle.Wrapped)]
        public void LogOn([System.Xml.Serialization.XmlElementAttribute(IsNullable=false)] string strEmail, [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)] string strPassword) {
            this.Invoke("LogOn", new object[] {
                        strEmail,
                        strPassword});
        }
        
        [System.Diagnostics.DebuggerStepThroughAttribute()]
        public System.IAsyncResult BeginLogOn(string strEmail, string strPassword, System.AsyncCallback callback, object asyncState) {
            return this.BeginInvoke("LogOn", new object[] {
                        strEmail,
                        strPassword}, callback, asyncState);
        }
        
        [System.Diagnostics.DebuggerStepThroughAttribute()]
        public void EndLogOn(System.IAsyncResult asyncResult) {
            this.EndInvoke(asyncResult);
        }
        
        [System.Web.Services.Protocols.SoapHeaderAttribute("m_SessionID", Direction=System.Web.Services.Protocols.SoapHeaderDirection.InOut)]
        [System.Diagnostics.DebuggerStepThroughAttribute()]
        [System.Web.Services.Protocols.SoapDocumentMethodAttribute("#GetAddressBookEntry", RequestNamespace="urn:OnlineAddressBookWSService", ResponseNamespace="urn:OnlineAddressBookWSService", Use=System.Web.Services.Description.SoapBindingUse.Literal, ParameterStyle=System.Web.Services.Protocols.SoapParameterStyle.Wrapped)]
        [return: System.Xml.Serialization.XmlElementAttribute("return", IsNullable=false)]
        public ContactItem GetAddressBookEntry(int lContactID) {
            object[] results = this.Invoke("GetAddressBookEntry", new object[] {
                        lContactID});
            return ((ContactItem)(results[0]));
        }
        
        [System.Diagnostics.DebuggerStepThroughAttribute()]
        public System.IAsyncResult BeginGetAddressBookEntry(int lContactID, System.AsyncCallback callback, object asyncState) {
            return this.BeginInvoke("GetAddressBookEntry", new object[] {
                        lContactID}, callback, asyncState);
        }
        
        [System.Diagnostics.DebuggerStepThroughAttribute()]
        public ContactItem EndGetAddressBookEntry(System.IAsyncResult asyncResult) {
            object[] results = this.EndInvoke(asyncResult);
            return ((ContactItem)(results[0]));
        }
        
        [System.Web.Services.Protocols.SoapHeaderAttribute("m_SessionID")]
        [System.Diagnostics.DebuggerStepThroughAttribute()]
        [System.Web.Services.Protocols.SoapDocumentMethodAttribute("#GetContactList", RequestNamespace="urn:OnlineAddressBookWSService", ResponseNamespace="urn:OnlineAddressBookWSService", Use=System.Web.Services.Description.SoapBindingUse.Literal, ParameterStyle=System.Web.Services.Protocols.SoapParameterStyle.Wrapped)]
        [return: System.Xml.Serialization.XmlArrayAttribute("NameList", IsNullable=true)]
        [return: System.Xml.Serialization.XmlArrayItemAttribute("string", IsNullable=false)]
        public string[] GetContactList([System.Xml.Serialization.XmlArrayItemAttribute("int", IsNullable=false)] out int[] IDList) {
            object[] results = this.Invoke("GetContactList", new object[0]);
            IDList = ((int[])(results[1]));
            return ((string[])(results[0]));
        }
        
        [System.Diagnostics.DebuggerStepThroughAttribute()]
        public System.IAsyncResult BeginGetContactList(System.AsyncCallback callback, object asyncState) {
            return this.BeginInvoke("GetContactList", new object[0], callback, asyncState);
        }
        
        [System.Diagnostics.DebuggerStepThroughAttribute()]
        public string[] EndGetContactList(System.IAsyncResult asyncResult, out int[] IDList) {
            object[] results = this.EndInvoke(asyncResult);
            IDList = ((int[])(results[1]));
            return ((string[])(results[0]));
        }
        
        [System.Diagnostics.DebuggerStepThroughAttribute()]
        [System.Web.Services.Protocols.SoapDocumentMethodAttribute("#CreateAccount", RequestNamespace="urn:OnlineAddressBookWSService", ResponseNamespace="urn:OnlineAddressBookWSService", Use=System.Web.Services.Description.SoapBindingUse.Literal, ParameterStyle=System.Web.Services.Protocols.SoapParameterStyle.Wrapped)]
        public void CreateAccount([System.Xml.Serialization.XmlElementAttribute(IsNullable=false)] string strEmail, [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)] string strPassword, [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)] string strFirstName, [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)] string strLastName) {
            this.Invoke("CreateAccount", new object[] {
                        strEmail,
                        strPassword,
                        strFirstName,
                        strLastName});
        }
        
        [System.Diagnostics.DebuggerStepThroughAttribute()]
        public System.IAsyncResult BeginCreateAccount(string strEmail, string strPassword, string strFirstName, string strLastName, System.AsyncCallback callback, object asyncState) {
            return this.BeginInvoke("CreateAccount", new object[] {
                        strEmail,
                        strPassword,
                        strFirstName,
                        strLastName}, callback, asyncState);
        }
        
        [System.Diagnostics.DebuggerStepThroughAttribute()]
        public void EndCreateAccount(System.IAsyncResult asyncResult) {
            this.EndInvoke(asyncResult);
        }
        
        [System.Web.Services.Protocols.SoapHeaderAttribute("m_SessionID")]
        [System.Diagnostics.DebuggerStepThroughAttribute()]
        [System.Web.Services.Protocols.SoapDocumentMethodAttribute("#InsertContact", RequestNamespace="urn:OnlineAddressBookWSService", ResponseNamespace="urn:OnlineAddressBookWSService", Use=System.Web.Services.Description.SoapBindingUse.Literal, ParameterStyle=System.Web.Services.Protocols.SoapParameterStyle.Wrapped)]
        public void InsertContact([System.Xml.Serialization.XmlElementAttribute(IsNullable=false)] ContactItem newContactItem) {
            this.Invoke("InsertContact", new object[] {
                        newContactItem});
        }
        
        [System.Diagnostics.DebuggerStepThroughAttribute()]
        public System.IAsyncResult BeginInsertContact(ContactItem newContactItem, System.AsyncCallback callback, object asyncState) {
            return this.BeginInvoke("InsertContact", new object[] {
                        newContactItem}, callback, asyncState);
        }
        
        [System.Diagnostics.DebuggerStepThroughAttribute()]
        public void EndInsertContact(System.IAsyncResult asyncResult) {
            this.EndInvoke(asyncResult);
        }
    }
    
    [System.Xml.Serialization.XmlTypeAttribute(Namespace="http://www.w3.org/2001/XMLSchema")]
    [System.Xml.Serialization.XmlRootAttribute("m_SessionID", Namespace="urn:OnlineAddressBookWSService", IsNullable=false)]
    public class @string : SoapHeader {
        
        [System.Xml.Serialization.XmlTextAttribute()]
        public string[] Text;
    }
    
    public class ContactItem {
        
        public int m_ContactID;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_Birthday;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_BusinessCity;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_BusinessCountry;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_BusinessFax;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_BusinessPhone;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_BusinessPostalCode;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_BusinessState;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_BusinessStreet;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_Company;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_Department;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_EmailAddress;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_FirstName;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_HomeCity;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_HomeCountry;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_HomeFax;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_HomePhone;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_HomePostalCode;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_HomeState;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_HomeStreet;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_JobTitle;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_LastName;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_MiddleName;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_MobilePhone;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_OtherPhone;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_Suffix;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_Title;
        
        [System.Xml.Serialization.XmlElementAttribute(IsNullable=false)]
        public string m_WebPage;
    }
}
