#if !defined(AFX_DLGDEFECTPAGE1_H__EEB33197_BD96_4B63_9E4E_3B6A04370D10__INCLUDED_)
#define AFX_DLGDEFECTPAGE1_H__EEB33197_BD96_4B63_9E4E_3B6A04370D10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDefectPage1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDefectPage1 dialog

class CDlgDefectPage1 : public CDialog
{
// Construction
public:
	CDlgDefectPage1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDefectPage1)
	enum { IDD = IDD_DLG_DEFECT_PAGE_1 };
	CListCtrl	m_listctrl1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDefectPage1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDefectPage1)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDEFECTPAGE1_H__EEB33197_BD96_4B63_9E4E_3B6A04370D10__INCLUDED_)
