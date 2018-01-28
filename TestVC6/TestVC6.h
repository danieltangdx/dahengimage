// TestVC6.h : main header file for the TESTVC6 application
//

#if !defined(AFX_TESTVC6_H__697FC271_6101_42BE_896F_1E84CE69D09A__INCLUDED_)
#define AFX_TESTVC6_H__697FC271_6101_42BE_896F_1E84CE69D09A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTestVC6App:
// See TestVC6.cpp for the implementation of this class
//

class CTestVC6App : public CWinApp
{
public:
	CTestVC6App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestVC6App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTestVC6App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTVC6_H__697FC271_6101_42BE_896F_1E84CE69D09A__INCLUDED_)
