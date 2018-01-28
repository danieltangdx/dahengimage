#if !defined(AFX_DLGMODEL_H__703FB1F1_CA9C_4182_B6B1_36D514B856E7__INCLUDED_)
#define AFX_DLGMODEL_H__703FB1F1_CA9C_4182_B6B1_36D514B856E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgModel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgModel dialog

class CDlgModel : public CDialog
{
// Construction
public:
	CDlgModel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgModel)
	enum { IDD = IDD_DLG_MODEL };
	CString	m_strNewModelName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgModel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgModel)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGMODEL_H__703FB1F1_CA9C_4182_B6B1_36D514B856E7__INCLUDED_)
