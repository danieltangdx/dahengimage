#if !defined(AFX_SETDLG_H__2EE95436_C1A6_4F88_95D9_06487B1C9991__INCLUDED_)
#define AFX_SETDLG_H__2EE95436_C1A6_4F88_95D9_06487B1C9991__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetDlg.h : header file
//
#include "BaumerCamDH.h"
#include "afxwin.h"
/////////////////////////////////////////////////////////////////////////////
// SetDlg dialog

class SetDlg : public CDialog
{
// Construction
public:
	SetDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL GetAcquisition(CBaumerCamDH *p);

// Dialog Data
	//{{AFX_DATA(SetDlg)
	enum { IDD = IDD_DIALOG_SETCAMERA };
	CComboBox	m_TriggerMode;
	int		m_nEx;
	float	m_fGain;
	int		m_nL;
	int		m_nR;
	int		m_nT;
	int		m_nB;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SetDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CBaumerCamDH *m_p;

	int m_nExMax,m_nExMin;
	float m_nGainMax,m_nGainMin;

	// Generated message map functions
	//{{AFX_MSG(SetDlg)
	afx_msg void OnButtonA1();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonA2();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonA3();
	afx_msg void OnSelchangeComboTriggerMode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	// ͼ��תģʽ
	CComboBox m_cmbFlipType;
	afx_msg void OnSelchangeComboImageFlipType();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETDLG_H__2EE95436_C1A6_4F88_95D9_06487B1C9991__INCLUDED_)
