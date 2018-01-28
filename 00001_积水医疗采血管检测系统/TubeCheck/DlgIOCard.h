#if !defined(AFX_DLGIOCARD_H__A7CD7C0A_D088_4A8E_901C_318D42D097A1__INCLUDED_)
#define AFX_DLGIOCARD_H__A7CD7C0A_D088_4A8E_901C_318D42D097A1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgIOCard.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgIOCard dialog

class CDlgIOCard : public CDialog
{
// Construction
public:
	CDlgIOCard(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgIOCard)
	enum { IDD = IDD_DLG_IOCARD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgIOCard)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgIOCard)
	afx_msg void OnBtnCard1();
	afx_msg void OnBtnCard2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGIOCARD_H__A7CD7C0A_D088_4A8E_901C_318D42D097A1__INCLUDED_)
