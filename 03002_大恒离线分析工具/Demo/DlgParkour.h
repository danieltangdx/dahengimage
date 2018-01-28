#if !defined(AFX_DLGPARKOUR_H__BFA99947_3007_466C_92AB_8EBE03F35771__INCLUDED_)
#define AFX_DLGPARKOUR_H__BFA99947_3007_466C_92AB_8EBE03F35771__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgParkour.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgParkour dialog

class CDlgParkour : public CDialog
{
// Construction
public:
	CDlgParkour(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgParkour)
	enum { IDD = IDD_DLG_PARKOUR };
	int		m_nInterval;
	BOOL	m_bStopGood;
	BOOL	m_bStopExp;
	BOOL	m_bStopEmp;
	BOOL	m_bStopBad;
	BOOL	m_bUnlimLoop;
	BOOL	m_bFromSel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgParkour)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgParkour)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPARKOUR_H__BFA99947_3007_466C_92AB_8EBE03F35771__INCLUDED_)
