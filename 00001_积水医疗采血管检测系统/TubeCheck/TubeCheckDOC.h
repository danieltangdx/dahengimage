// TubeCheckDoc.h : interface of the CTubeCheckDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TubeCheckDOC_H__03097DD0_5178_4B86_B5E7_CFF00FB8C2A9__INCLUDED_)
#define AFX_TubeCheckDOC_H__03097DD0_5178_4B86_B5E7_CFF00FB8C2A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\\Inc\\ParamDefine.h"

class CTubeCheckDoc : public CDocument
{
protected: // create from serialization only
	CTubeCheckDoc();
	DECLARE_DYNCREATE(CTubeCheckDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTubeCheckDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTubeCheckDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTubeCheckDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TubeCheckDOC_H__03097DD0_5178_4B86_B5E7_CFF00FB8C2A9__INCLUDED_)
