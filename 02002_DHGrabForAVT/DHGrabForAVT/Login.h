#if !defined(AFX_LOGIN_H__E38B9630_2163_4FD7_9C46_8638D22B5D63__INCLUDED_)
#define AFX_LOGIN_H__E38B9630_2163_4FD7_9C46_8638D22B5D63__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Login.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// Login dialog

class Login : public CDialog
{
// Construction
public:
	Login(CString sIniFile,CWnd* pParent = NULL);   // standard constructor

	BOOL m_bLogin;
	CString m_sIniFile;

// Dialog Data
	//{{AFX_DATA(Login)
	enum { IDD = IDD_DIALOG_LOGIN };
	CString	m_sPsw;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(Login)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(Login)
	afx_msg void OnButtonApply();
	virtual void OnOK();
	afx_msg void OnButtonNewpsw();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGIN_H__E38B9630_2163_4FD7_9C46_8638D22B5D63__INCLUDED_)
