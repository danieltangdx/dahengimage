#if !defined(AFX_UIDLGPROCESSTEST_H__1A875B78_0C36_4D23_BDF5_A05BDC24BB03__INCLUDED_)
#define AFX_UIDLGPROCESSTEST_H__1A875B78_0C36_4D23_BDF5_A05BDC24BB03__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UIDlgProcessTest.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUIDlgProcessTest dialog

class CUIDlgProcessTest : public CDialog
{
// Construction
public:
	CUIDlgProcessTest(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUIDlgProcessTest)
	enum { IDD = IDD_DLG_PROCESS_TEST };
	CListBox	m_lProcessList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUIDlgProcessTest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUIDlgProcessTest)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UIDLGPROCESSTEST_H__1A875B78_0C36_4D23_BDF5_A05BDC24BB03__INCLUDED_)
