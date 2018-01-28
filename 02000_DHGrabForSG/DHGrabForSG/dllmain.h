// GrabberRegularForSG.h : main header file for the GRABBERREGULARFORSG DLL
//

#if !defined(AFX_GRABBERREGULARFORSG_H__84CB388E_711B_4E27_9F4B_E7AF25841EE1__INCLUDED_)
#define AFX_GRABBERREGULARFORSG_H__84CB388E_711B_4E27_9F4B_E7AF25841EE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "ExFn.h"

/////////////////////////////////////////////////////////////////////////////
// CGrabberRegularForSGApp
// See GrabberRegularForSG.cpp for the implementation of this class
//

class CDHGrabForSGApp : public CWinApp
{
public:
	CDHGrabForSGApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGrabberRegularForSGApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CGrabberRegularForSGApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRABBERREGULARFORSG_H__84CB388E_711B_4E27_9F4B_E7AF25841EE1__INCLUDED_)
