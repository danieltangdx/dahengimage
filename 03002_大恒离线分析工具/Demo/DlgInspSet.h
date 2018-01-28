#if !defined(AFX_DLGINSPSET_H__393A0EA8_FF57_4D32_9F89_E0CA956D84BF__INCLUDED_)
#define AFX_DLGINSPSET_H__393A0EA8_FF57_4D32_9F89_E0CA956D84BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgInspSet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgInspSet dialog

class CDlgInspSet : public CDialog
{
// Construction
public:
	CDlgInspSet(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgInspSet)
	enum { IDD = IDD_DLG_INSP };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgInspSet)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_nPreNumWS;
protected:
	int m_nNumWS;
	BOOL m_bPainted;
	BOOL TryToGetDllNameByIndex(int nIndex, CString& strDllName);
	BOOL CheckAllValid(int& nProbIndex);
	// Generated message map functions
	//{{AFX_MSG(CDlgInspSet)
	afx_msg void OnPaint();
	afx_msg void OnButton(UINT uID);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGINSPSET_H__393A0EA8_FF57_4D32_9F89_E0CA956D84BF__INCLUDED_)
