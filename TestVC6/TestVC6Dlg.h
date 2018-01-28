// TestVC6Dlg.h : header file
//

#if !defined(AFX_TESTVC6DLG_H__C456546C_98E3_4C04_AC83_9DD50C19421E__INCLUDED_)
#define AFX_TESTVC6DLG_H__C456546C_98E3_4C04_AC83_9DD50C19421E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CTestVC6Dlg dialog

class CTestVC6Dlg : public CDialog
{
// Construction
public:
	CTestVC6Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTestVC6Dlg)
	enum { IDD = IDD_TESTVC6_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTestVC6Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTestVC6Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TESTVC6DLG_H__C456546C_98E3_4C04_AC83_9DD50C19421E__INCLUDED_)
