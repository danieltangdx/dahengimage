#if !defined(AFX_DLGINSPNUMWS_H__BA9F1275_6622_44A0_843B_0047E3E2AC65__INCLUDED_)
#define AFX_DLGINSPNUMWS_H__BA9F1275_6622_44A0_843B_0047E3E2AC65__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInspNumWS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInspNumWS dialog

class CDlgInspNumWS : public CDialog
{
// Construction
public:
	CDlgInspNumWS(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgInspNumWS)
	enum { IDD = IDD_DLG_NUMWS };
	int		m_nNumWS;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInspNumWS)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgInspNumWS)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINSPNUMWS_H__BA9F1275_6622_44A0_843B_0047E3E2AC65__INCLUDED_)
