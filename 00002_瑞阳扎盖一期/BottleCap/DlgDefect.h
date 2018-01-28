#if !defined(AFX_DLGDEFECT_H__C75D4608_9CE7_4450_AA9A_1A93251D39E1__INCLUDED_)
#define AFX_DLGDEFECT_H__C75D4608_9CE7_4450_AA9A_1A93251D39E1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDefect.h : header file
//

#include "DlgDefectPage1.h"
#include "DlgDefectPage2.h"


/////////////////////////////////////////////////////////////////////////////
// CDlgDefect dialog

class CDlgDefect : public CDialog
{
// Construction
public:
	CDlgDefect(CWnd* pParent = NULL);   // standard constructor

public:
	CDlgDefectPage1 m_page1;
	CDlgDefectPage2 m_page2;

// Dialog Data
	//{{AFX_DATA(CDlgDefect)
	enum { IDD = IDD_DLG_DEFECT };
	CTabCtrl	m_TabCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDefect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDefect)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	bool UpdateInfo();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDEFECT_H__C75D4608_9CE7_4450_AA9A_1A93251D39E1__INCLUDED_)
