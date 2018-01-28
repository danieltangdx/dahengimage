// ObjectAllocation.h : main header file for the OBJECTALLOCATION DLL
//

#if !defined(AFX_OBJECTALLOCATION_H__057525AD_6A6C_4309_81F7_C6432CA5DADB__INCLUDED_)
#define AFX_OBJECTALLOCATION_H__057525AD_6A6C_4309_81F7_C6432CA5DADB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "MultiObjectDef.h"
#include "ImageDef.h"
#include "IDS.h"
#include "ExtenInfo.h"
#include "StringConvert.h"
#include "ExFn.h"
#include "xcgui.h"
#include "array"
#include "vector"
#include "queue"
#include "memory"
#include "algorithm"
#include "numeric"
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CObjectAllocationApp
// See ObjectAllocation.cpp for the implementation of this class
//

class CObjectAllocationApp : public CWinApp
{
public:
	CObjectAllocationApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CObjectAllocationApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CObjectAllocationApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OBJECTALLOCATION_H__057525AD_6A6C_4309_81F7_C6432CA5DADB__INCLUDED_)
