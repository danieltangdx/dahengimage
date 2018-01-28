#if !defined(AFX_UIDLGMDLOPRT_H__BD0E7EB1_C8FE_4FCD_A7F8_15A777FD6FF4__INCLUDED_)
#define AFX_UIDLGMDLOPRT_H__BD0E7EB1_C8FE_4FCD_A7F8_15A777FD6FF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UIDlgMdlOprt.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUIDlgMdlOprt dialog

class CUIDlgMdlOprt : public CDialog
{
// Construction
public:
	CUIDlgMdlOprt(CWnd* pParent = NULL);   // standard constructor
	CDialog* m_pDlgMainWindow;
	void RefreshModelList();
// Dialog Data
	//{{AFX_DATA(CUIDlgMdlOprt)
	enum { IDD = IDD_DLG_MDLOP };
	CListBox	m_lbModels;
	CString	m_strModelNameUsing;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUIDlgMdlOprt)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUIDlgMdlOprt)
	virtual BOOL OnInitDialog();
	afx_msg void OnNewModel();
	afx_msg void OnSelchangeModellist();
	afx_msg void OnBtnSaveModel();
	afx_msg void OnBtnReadModel();
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UIDLGMDLOPRT_H__BD0E7EB1_C8FE_4FCD_A7F8_15A777FD6FF4__INCLUDED_)
