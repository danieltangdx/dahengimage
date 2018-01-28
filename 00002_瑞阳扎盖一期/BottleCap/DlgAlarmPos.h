#if !defined(AFX_DLGALARMPOS_H__49B08976_8CF5_450D_AC2A_C676F2A722D3__INCLUDED_)
#define AFX_DLGALARMPOS_H__49B08976_8CF5_450D_AC2A_C676F2A722D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAlarmPos.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmPos dialog

class CDlgAlarmPos : public CDialog
{
// Construction
public:
	CDlgAlarmPos(CWnd* pParent = NULL);   // standard constructor

	// 设置报警字体
	CFont m_fontAlarm;				//字体


// Dialog Data
	//{{AFX_DATA(CDlgAlarmPos)
	enum { IDD = IDD_DLG_ALRAM_POS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAlarmPos)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAlarmPos)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGALARMPOS_H__49B08976_8CF5_450D_AC2A_C676F2A722D3__INCLUDED_)
