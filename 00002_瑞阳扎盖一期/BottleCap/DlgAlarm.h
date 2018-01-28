#if !defined(AFX_DLGALARM_H__8318E783_B781_4BBA_8224_7DA69A1B7758__INCLUDED_)
#define AFX_DLGALARM_H__8318E783_B781_4BBA_8224_7DA69A1B7758__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAlarm.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CDlgAlarm dialog

class CDlgAlarm : public CDialog
{
// Construction
public:
	CDlgAlarm(CWnd* pParent = NULL);   // standard constructor

	// 设置报警字体
	CFont m_fontAlarm;				//字体



// Dialog Data
	//{{AFX_DATA(CDlgAlarm)
	enum { IDD = IDD_DLG_ALRAM };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAlarm)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAlarm)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGALARM_H__8318E783_B781_4BBA_8224_7DA69A1B7758__INCLUDED_)
