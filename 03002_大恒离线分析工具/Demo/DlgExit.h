#if !defined(AFX_DLGEXIT_H__4A317CD9_1661_4A8D_B3AF_46B2156695BD__INCLUDED_)
#define AFX_DLGEXIT_H__4A317CD9_1661_4A8D_B3AF_46B2156695BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgExit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgExit dialog

class CDlgExit : public CDialog
{
// Construction
public:
	CDlgExit(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgExit)
	enum { IDD = IDD_DLG_EXIT };
	CStatic	m_cPic1;
	CStatic	m_cPic2;
	//}}AFX_DATA
//	CStatic	m_cPic[34];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgExit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_nClass;
	BOOL m_bCancelEnable;
	BOOL m_bLoadDisable;
	UINT m_uTimer1, m_uTimer2;
	int m_nFlying1;
	int m_nFlying2;
	int /*m_nLeftLimit, */m_nRightLimit/*, m_nTopLimit*/, m_nBottomLimit;
	HICON* m_pHIcon;
protected:
	CBrush m_brush;
	// Generated message map functions
	//{{AFX_MSG(CDlgExit)
	afx_msg void OnBtnLAndE();
	afx_msg void OnBtnExit();
	afx_msg void OnBtnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGEXIT_H__4A317CD9_1661_4A8D_B3AF_46B2156695BD__INCLUDED_)
