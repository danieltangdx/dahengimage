// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__8927AAD0_12B1_4E3B_A541_0836B04D520C__INCLUDED_)
#define AFX_STDAFX_H__8927AAD0_12B1_4E3B_A541_0836B04D520C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#include <AFXDAO.H>
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

//////////////////////////////////
// add
#include "Afxmt.h"
#include "Afxtempl.h"

#pragma warning(disable:4146)
#import "msado15.dll" 	no_namespace rename ("EOF", "ADOEOF")\
	rename("BOF","adoBOF")\
	rename ("LockTypeEnum", "adoLockTypeEnum") \
	rename ("DataTypeEnum", "adoDataTypeEnum") \
	rename ("FieldAttributeEnum", "adoFieldAttributeEnum") \
	rename ("EditModeEnum", "adoEditModeEnum") \
	rename ("RecordStatusEnum", "adoRecordStatusEnum") \
	rename ("ParameterDirectionEnum", "adoParameterDirectionEnum")

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__8927AAD0_12B1_4E3B_A541_0836B04D520C__INCLUDED_)


#if _MSC_VER > 1000
#pragma comment(linker,"\"/manifestdependency:type='win32' \
	name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
	processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif