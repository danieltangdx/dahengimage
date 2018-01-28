#if !defined(AFX_DLGNEWMODEL_H__09E7CECA_EA6F_4821_A53E_0486DDD733C7__INCLUDED_)
#define AFX_DLGNEWMODEL_H__09E7CECA_EA6F_4821_A53E_0486DDD733C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNewModel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgNewModel dialog

class CDlgNewModel : public CDialog
{
// Construction
public:
	CDlgNewModel(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgNewModel)
	enum { IDD = IDD_DLG_NEWMODEL };
	CString	m_strNewModel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNewModel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNewModel)
	afx_msg void OnBtnConfirm();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNEWMODEL_H__09E7CECA_EA6F_4821_A53E_0486DDD733C7__INCLUDED_)
