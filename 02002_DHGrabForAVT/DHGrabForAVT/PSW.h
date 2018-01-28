#if !defined(AFX_PSW_H__0D83E1E2_6E98_428A_B162_FA1DE726E50B__INCLUDED_)
#define AFX_PSW_H__0D83E1E2_6E98_428A_B162_FA1DE726E50B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PSW.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PSW dialog

class PSW : public CDialog
{
// Construction
public:
	PSW(CString str,CWnd* pParent = NULL);   // standard constructor
	CString m_sIniFile;
// Dialog Data
	//{{AFX_DATA(PSW)
	enum { IDD = IDD_DIALOG_PSW };
	CString	m_sOldPsw;
	CString	m_sNewPsw;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PSW)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(PSW)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PSW_H__0D83E1E2_6E98_428A_B162_FA1DE726E50B__INCLUDED_)
