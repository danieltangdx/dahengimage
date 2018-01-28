#if !defined(AFX_DLGSYSTEMCONFIG_H__45E0B736_7F42_4AD4_94C9_1FCA76C65604__INCLUDED_)
#define AFX_DLGSYSTEMCONFIG_H__45E0B736_7F42_4AD4_94C9_1FCA76C65604__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSystemConfig.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSystemConfig dialog

class CDlgSystemConfig : public CDialog
{
// Construction
public:
	CDlgSystemConfig(CWnd* pParent = NULL);   // standard constructor

	int		m_iShowMode;	//显示模式(0:不显示,1:显示所有,2:显示有瓶,3:显示好品,4:显示坏品,5:显示无瓶)
	int		m_iKickMode;	//剔废模式 0为全踢，1为全不剔，2为隔张踢，3为正常

// Dialog Data
	//{{AFX_DATA(CDlgSystemConfig)
	enum { IDD = IDD_DLG_SYSTEM_CONFIG };
	double	m_dBlue;
	double	m_dGreen;
	double	m_dRed;
	BOOL	m_bKick1;
	BOOL	m_bBayer;
	int		m_iMaxSaveImageCount;
	int		m_iShowTime;
	BOOL	m_bKick2;
	BOOL	m_bBayer2;
	double	m_dBlue2;
	double	m_dGreen2;
	double	m_dRed2;
	double	m_dRed3;
	double	m_dGreen3;
	double	m_dBlue3;
	BOOL	m_bKick3;
	BOOL	m_bBayer3;
	//}}AFX_DATA



// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSystemConfig)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSystemConfig)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnRadioKickAll();
	afx_msg void OnRadioKickNo();
	afx_msg void OnRadioKickDiff();
	afx_msg void OnRadioKickNormal();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSYSTEMCONFIG_H__45E0B736_7F42_4AD4_94C9_1FCA76C65604__INCLUDED_)
