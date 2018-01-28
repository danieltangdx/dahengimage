#if !defined(AFX_DLGMODIFYCIPHER_H__5AA9D60E_9A0C_4AB2_BF82_97030162C18D__INCLUDED_)
#define AFX_DLGMODIFYCIPHER_H__5AA9D60E_9A0C_4AB2_BF82_97030162C18D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgModifyCipher.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgModifyCipher dialog

class CDlgModifyCipher : public CDialog
{
// Construction
public:
	CDlgModifyCipher(CWnd* pParent = NULL);   // standard constructor

	int m_iUserType;		//用户类型 0为非法，1为管理员，2为调试员，3为普通用户
	int m_iCallType;		//1为 管理员调用，2为其他用户

// Dialog Data
	//{{AFX_DATA(CDlgModifyCipher)
	enum { IDD = IDD_DLG_MODIFY_CIPHER };
	CString	m_strUser;
	CString	m_strOldPW;
	CString	m_strNewPW;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgModifyCipher)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgModifyCipher)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMODIFYCIPHER_H__5AA9D60E_9A0C_4AB2_BF82_97030162C18D__INCLUDED_)
