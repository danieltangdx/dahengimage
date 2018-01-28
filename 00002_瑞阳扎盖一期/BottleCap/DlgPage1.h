#if !defined(AFX_DLGPAGE1_H__CFCFF659_80EE_44E7_AB3B_2520026FE179__INCLUDED_)
#define AFX_DLGPAGE1_H__CFCFF659_80EE_44E7_AB3B_2520026FE179__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPage1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPage1 dialog

class CDlgPage1 : public CDialog
{
// Construction
public:
	CDlgPage1(CWnd* pParent = NULL);   // standard constructor

	int m_iIndex;


// Dialog Data
	//{{AFX_DATA(CDlgPage1)
	enum { IDD = IDD_DLG_PAGE1 };
	CListCtrl	m_lscInfo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPage1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPage1)
	afx_msg void OnClickList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydownList1(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPAGE1_H__CFCFF659_80EE_44E7_AB3B_2520026FE179__INCLUDED_)
