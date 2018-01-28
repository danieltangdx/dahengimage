#if !defined(AFX_DLGUSERADM_H__A5F94F9A_4B0F_4CEB_BB1B_5FDFF38A4432__INCLUDED_)
#define AFX_DLGUSERADM_H__A5F94F9A_4B0F_4CEB_BB1B_5FDFF38A4432__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgUserAdm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgUserAdm dialog

class CDlgUserAdm : public CDialog
{
// Construction
public:
	CDlgUserAdm(CWnd* pParent = NULL);   // standard constructor
	
	int m_iUserType;		//1为管理员；2为调试员；3为普通用户

	void LoadUser();

// Dialog Data
	//{{AFX_DATA(CDlgUserAdm)
	enum { IDD = IDD_DLG_USER_ADM };
	CListBox	m_listUser;
	CListBox	m_listDebug;
	CListBox	m_listAdm;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgUserAdm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgUserAdm)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAdmAdd();
	afx_msg void OnBtnAdmDelete();
	afx_msg void OnBtnAdmModify();
	afx_msg void OnBtnDebugAdd();
	afx_msg void OnBtnDebugDelete();
	afx_msg void OnBtnDebugModify();
	afx_msg void OnBtnUserAdd();
	afx_msg void OnBtnUserDelete();
	afx_msg void OnBtnUserModify();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGUSERADM_H__A5F94F9A_4B0F_4CEB_BB1B_5FDFF38A4432__INCLUDED_)
