#if !defined(AFX_DLGALARMSTOPCOUNT_H__4FCB834F_B523_4156_86BB_01101898FB04__INCLUDED_)
#define AFX_DLGALARMSTOPCOUNT_H__4FCB834F_B523_4156_86BB_01101898FB04__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAlarmStopCount.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarmStopCount dialog

class CDlgAlarmStopCount : public CDialog
{
// Construction
public:
	CDlgAlarmStopCount(CWnd* pParent = NULL);   // standard constructor

	// 设置报警字体
	CFont m_fontAlarm;				//字体

// Dialog Data
	//{{AFX_DATA(CDlgAlarmStopCount)
	enum { IDD = IDD_DLG_ALRAM_STOP_COUNT };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAlarmStopCount)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAlarmStopCount)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGALARMSTOPCOUNT_H__4FCB834F_B523_4156_86BB_01101898FB04__INCLUDED_)
