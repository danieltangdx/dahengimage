// DemoProject.h : main header file for the DEMOPROJECT application
//

#if !defined(AFX_DEMOPROJECT_H__A69F51FB_BA9C_4544_9069_9FBD9D8C2C8B__INCLUDED_)
#define AFX_DEMOPROJECT_H__A69F51FB_BA9C_4544_9069_9FBD9D8C2C8B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


#include "CustomMessages.h"
#include "GeneralFun_Yang.h"
/*************************************<>*************************************/
#include "ExFn.h"
#include "xcgui.h"
#include "CommonFiles/CmnHdr.h"
#include "IDS.h"
/////////////////////////////////////////////////////////////////////////////
// CDemoProjectApp:
// See DemoProject.cpp for the implementation of this class
//

class CDemoProjectApp : public CWinApp
{
public:
	CDemoProjectApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoProjectApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
//	HANDLE g_Event;
	void InitLanguageEx();
	BOOL ShowWindow(int nCmdShow);
	//{{AFX_MSG(CDemoProjectApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:

public:
	bool m_bEnableXCGUI{ false };
	CString m_sLanguageFilePath{ TEXT("") };
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMOPROJECT_H__A69F51FB_BA9C_4544_9069_9FBD9D8C2C8B__INCLUDED_)
