#if !defined(AFX_SETCIPHERDLG_H__20CDFF01_A306_11D5_9302_E6DCFE97F30D__INCLUDED_)
#define AFX_SETCIPHERDLG_H__20CDFF01_A306_11D5_9302_E6DCFE97F30D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetCipher.h : header file

#include "resource.h"

//
/********************************************************************
	created:	2001/09/06
	created:	6:9:2001   21:00
	filename: 	d:\liufenglong\moneycode\setciphedrlg.h
	file path:	d:\liufenglong\moneycode
	file base:	setcipherdlg
	file ext:	h
	author:		lfl
	
	purpose:	ÃÜÂëÐÞ¸Ä¶Ô»°¿ò
	 
*********************************************************************/

/////////////////////////////////////////////////////////////////////////////
// CDlgSetCipher dialog

class CDlgSetCipher : public CDialog
{
// Construction
public:
	CDlgSetCipher(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSetCipher)
	enum { IDD = IDD_DLG_SETCIPHER };
	CString	m_new1;
	CString	m_new2;
	CString	m_old;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSetCipher)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSetCipher)
	afx_msg void OnBtnsetcipher();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETCIPHERDLG_H__20CDFF01_A306_11D5_9302_E6DCFE97F30D__INCLUDED_)
