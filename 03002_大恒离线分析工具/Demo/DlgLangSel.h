#if !defined(AFX_DLGLANGSEL_H__970730A1_7830_4D71_B2BE_3861B0193037__INCLUDED_)
#define AFX_DLGLANGSEL_H__970730A1_7830_4D71_B2BE_3861B0193037__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DemoProjectDlg.h"
// DlgLangSel.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgLangSel dialog

class CDlgLangSel : public CDialog
{
// Construction
public:
	CDlgLangSel(CWnd* pParent = NULL);   // standard constructor
	CDemoProjectDlg* m_pDlgMainWindowLan;
// Dialog Data
	//{{AFX_DATA(CDlgLangSel)
	enum { IDD = IDD_DLG_LANGUAGE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgLangSel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void GetDllName(char* chDllName, UINT uID);
protected:
	int m_nCount;
	BOOL m_bPainted;
	CRect m_rectORI;
	CStringArray m_strArrTexts;
	// Generated message map functions
	//{{AFX_MSG(CDlgLangSel)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnButton(UINT uID);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGLANGSEL_H__970730A1_7830_4D71_B2BE_3861B0193037__INCLUDED_)
