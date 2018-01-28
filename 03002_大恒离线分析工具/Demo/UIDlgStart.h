#if !defined(AFX_UIDLGSTART_H__7DBBBF21_8ADB_4F15_84C6_6EB4F9FF058C__INCLUDED_)
#define AFX_UIDLGSTART_H__7DBBBF21_8ADB_4F15_84C6_6EB4F9FF058C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UIDlgStart.h : header file
//
#include "StaticEx.h"
/////////////////////////////////////////////////////////////////////////////
// CUIDlgStart dialog
typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD);

class CUIDlgStart : public CDialog
{
// Construction
public:
	CUIDlgStart(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUIDlgStart)
	enum { IDD = IDD_DIALOG_START };
	CStaticEx	m_cStaticLoading;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUIDlgStart)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
protected:
	UINT m_uTimerT, m_uTimerR, m_uTimerD;
	MYFUNC m_fun;
	int m_nNextLimit, m_nCurPos;
	int m_nTransparent;     //Í¸Ã÷

	// Generated message map functions
	//{{AFX_MSG(CUIDlgStart)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UIDLGSTART_H__7DBBBF21_8ADB_4F15_84C6_6EB4F9FF058C__INCLUDED_)
