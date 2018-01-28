// SplitMerge.h : main header file for the SPLITMERGE DLL
//

#if !defined(AFX_SPLITMERGE_H__228AC346_DACC_403A_ABE6_6FD45C45E8B3__INCLUDED_)
#define AFX_SPLITMERGE_H__228AC346_DACC_403A_ABE6_6FD45C45E8B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


/////////////////////////////////////////////////////////////////////////////
// CSplitMergeApp
// See SplitMerge.cpp for the implementation of this class
//

class CSplitMergeApp : public CWinApp
{
public:
	CSplitMergeApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSplitMergeApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CSplitMergeApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

_declspec(dllexport) PixToPlane(const char *srcRGB, char *dstR, char *dstG, 
								char *dstB, int nWidth, int nHeight);

_declspec(dllexport) PlaneToPix(const char *srcR, const char *srcG, 
								const char *srcB, char *dstRGB,  int nWidth,
								int nHeight);


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPLITMERGE_H__228AC346_DACC_403A_ABE6_6FD45C45E8B3__INCLUDED_)
