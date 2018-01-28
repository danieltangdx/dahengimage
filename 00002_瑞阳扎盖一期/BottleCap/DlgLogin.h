#if !defined(AFX_DLGLOGIN_H__CF02C7B0_DECE_4062_A899_E4C9F6BA6C86__INCLUDED_)
#define AFX_DLGLOGIN_H__CF02C7B0_DECE_4062_A899_E4C9F6BA6C86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgLogin.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLogin dialog

class CDlgLogin : public CDialog
{
// Construction
public:
	CDlgLogin(CWnd* pParent = NULL);   // standard constructor

	CString m_strConfigFile;
	int m_iUserType;			//0为非合法用户；1管理员；2为调试员；3为普通用户

	int ReadConfigFile(CString strUser, CString strPW);
	CString GetCurrentAppPath();

// Dialog Data
	//{{AFX_DATA(CDlgLogin)
	enum { IDD = IDD_DLG_LOGIN };
	CString	m_strPassword;
	CString	m_strUser;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLogin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgLogin)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLOGIN_H__CF02C7B0_DECE_4062_A899_E4C9F6BA6C86__INCLUDED_)
