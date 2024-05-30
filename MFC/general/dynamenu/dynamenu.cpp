// dynamenu.cpp
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (c) Microsoft Corporation.  All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.
//
// Purpose: implementation of the CDynaMenuApp class
//
// Functions:
//      Most of this file was generated by AppWizard.  The functions
//      which contain code specific to this sample are:
//
//      CDynaMenuApp::InitInstance()         -- per-instance initialization

#include "stdafx.h"
#include "dynamenu.h"

#include "mainfrm.h"
#include "dmdoc.h"
#include "dmview.h"

#include "mdichild.h" // Our private CMDIChildWnd class

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynaMenuApp

BEGIN_MESSAGE_MAP(CDynaMenuApp, CWinApp)
	//{{AFX_MSG_MAP(CDynaMenuApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynaMenuApp construction

CDynaMenuApp::CDynaMenuApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDynaMenuApp object

CDynaMenuApp NEAR theApp;

/////////////////////////////////////////////////////////////////////////////
// CDynaMenuApp initialization

//***********************************************************************
// Function: CDynaMenuApp::InitInstance()
//
// Purpose:
//      InitInstance is called by the framework to perform per-instance
//      initialization of the application.
//
//      This function is the code generated by AppWizard with one
//      exception - we use our own class for the frame window attached
//      to a document template, instead of using CMDIChildWnd.
//
// Parameters:
//      none
//
// Returns:
//      nonzero if successful, otherwise 0
//
// Comments:
//      see the CWinApp::InitInstance() documentation for further info.
//
//***********************************************************************
BOOL CDynaMenuApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_DYMENUTYPE,
		RUNTIME_CLASS(CDynaMenuDoc),
		RUNTIME_CLASS(CDynaMDIChildWnd),        // our private MDI child class!!
		RUNTIME_CLASS(CDynaMenuView));
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME)){
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;

	// enable file manager drag/drop and DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	m_pMainWnd->DragAcceptFiles();
	// The main window has been initialized, so show and update it.
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	return TRUE;
}

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

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
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
void CDynaMenuApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CDynaMenuApp commands
