#if !defined(AFX_DLGADDUSER_H__41EF59C3_E635_4422_BA40_9CC56DADC48A__INCLUDED_)
#define AFX_DLGADDUSER_H__41EF59C3_E635_4422_BA40_9CC56DADC48A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAddUser.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAddUser dialog

class CDlgAddUser : public CDialog
{
// Construction
public:
	CDlgAddUser(CWnd* pParent = NULL);   // standard constructor

	int m_iUserType;			//用户类型 1为管理员；2为调试员；3为普通用户

// Dialog Data
	//{{AFX_DATA(CDlgAddUser)
	enum { IDD = IDD_DLG_ADD_USER };
	CString	m_strPassword;
	CString	m_strUser;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAddUser)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAddUser)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADDUSER_H__41EF59C3_E635_4422_BA40_9CC56DADC48A__INCLUDED_)
