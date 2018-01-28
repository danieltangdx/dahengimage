#if !defined(AFX_UIDLGMODELRENAME_H__86A6AEF4_DA89_4843_A9DB_0B518C6CD25B__INCLUDED_)
#define AFX_UIDLGMODELRENAME_H__86A6AEF4_DA89_4843_A9DB_0B518C6CD25B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UIDlgModelRename.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUIDlgModelRename dialog

class CUIDlgModelRename : public CDialog
{
// Construction
public:
	CUIDlgModelRename(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUIDlgModelRename)
	enum { IDD = IDD_DLG_RENAME };
	CString	m_strNewName;
	CString	m_strMsg;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUIDlgModelRename)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUIDlgModelRename)
	afx_msg void OnBtnOk();
	afx_msg void OnBtnNok();
	virtual BOOL OnInitDialog();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UIDLGMODELRENAME_H__86A6AEF4_DA89_4843_A9DB_0B518C6CD25B__INCLUDED_)
