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
	
	BOOL m_bSaveImage[c_AllElementCount];

// Dialog Data
	//{{AFX_DATA(CDlgSaveImage)
	enum { IDD = IDD_DLG_SAVE_IMAGE };
	int		m_iIsCircle{0};		//0为不循环，1为循环
	int		m_iSaveType{0};		//0为连续，1为错误
	int		m_iGrabCount{0};
	int		m_iErrorCount{0};
	int		m_iExistCount{ 0 };
	int		m_iSaveImgObj{ 0 };
	int		m_iSaveImgNamingRegulation{ 0 };
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSAVEIMAGE_H__F92B4F43_0608_4EB2_A7A2_8C2016DFA4AE__INCLUDED_)
