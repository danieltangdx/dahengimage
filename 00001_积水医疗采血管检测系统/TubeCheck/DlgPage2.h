#if !defined(AFX_DLGPAGE2_H__A755F117_596F_466D_9B61_D73689BE663D__INCLUDED_)
#define AFX_DLGPAGE2_H__A755F117_596F_466D_9B61_D73689BE663D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPage2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPage2 dialog

class CDlgPage2 : public CDialog
{
// Construction
public:
	CDlgPage2(CWnd* pParent = NULL);   // standard constructor

	int m_iIndex;

// Dialog Data
	//{{AFX_DATA(CDlgPage2)
	enum { IDD = IDD_DLG_PAGE2 };
	CListCtrl	m_lscInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPage2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPage2)
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownList1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPAGE2_H__A755F117_596F_466D_9B61_D73689BE663D__INCLUDED_)
