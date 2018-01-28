// AVTGrabber.h : main header file for the AVTGRABBER DLL
//

#if !defined(AFX_AVTGRABBER_H__23DB504B_6506_4FC7_8A0D_4B4BBE3CBC6F__INCLUDED_)
#define AFX_AVTGRABBER_H__23DB504B_6506_4FC7_8A0D_4B4BBE3CBC6F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CAVTGrabberApp
// See AVTGrabber.cpp for the implementation of this class
//

class CAVTGrabberApp : public CWinApp
{
public:
	CAVTGrabberApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAVTGrabberApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CAVTGrabberApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AVTGRABBER_H__23DB504B_6506_4FC7_8A0D_4B4BBE3CBC6F__INCLUDED_)
