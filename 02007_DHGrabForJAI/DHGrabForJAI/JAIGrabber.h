// JAIGrabber.h : main header file for the JAIGRABBER DLL
//

#if !defined(AFX_JAIGRABBER_H__31CD98A0_AD87_4971_899A_1E67A2B7B593__INCLUDED_)
#define AFX_JAIGRABBER_H__31CD98A0_AD87_4971_899A_1E67A2B7B593__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CJAIGrabberApp
// See JAIGrabber.cpp for the implementation of this class
//

class CJAIGrabberApp : public CWinApp
{
public:
	CJAIGrabberApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CJAIGrabberApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CJAIGrabberApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_JAIGRABBER_H__31CD98A0_AD87_4971_899A_1E67A2B7B593__INCLUDED_)
