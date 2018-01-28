#if !defined(AFX_DLGSAVEIMAGE_H__F92B4F43_0608_4EB2_A7A2_8C2016DFA4AE__INCLUDED_)
#define AFX_DLGSAVEIMAGE_H__F92B4F43_0608_4EB2_A7A2_8C2016DFA4AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSaveImage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSaveImage dialog

class CDlgSaveImage : public CDialog
{
// Construction
public:
	CDlgSaveImage(CWnd* pParent = NULL);   // standard constructor

public:
	
	BOOL m_bSaveImage[ALLELEMENT_COUNT];

// Dialog Data
	//{{AFX_DATA(CDlgSaveImage)
	enum { IDD = IDD_DLG_SAVE_IMAGE };
	int		m_iIsCircle;		//0为不循环，1为循环
	int		m_iSaveType;		//0为连续，1为错误
	int		m_iGrabCount;
	int		m_iErrorCount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSaveImage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSaveImage)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCheck1();
	afx_msg void OnCheck2();
	afx_msg void OnCheck3();
	afx_msg void OnCircle();
	afx_msg void OnNoCircle();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_iSaveImgObj;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSAVEIMAGE_H__F92B4F43_0608_4EB2_A7A2_8C2016DFA4AE__INCLUDED_)
