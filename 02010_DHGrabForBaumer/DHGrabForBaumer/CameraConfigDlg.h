#if !defined(AFX_CAMERACONFIGDLG_H__D7C8D59F_1035_469E_9022_CDE1A4E8AC7D__INCLUDED_)
#define AFX_CAMERACONFIGDLG_H__D7C8D59F_1035_469E_9022_CDE1A4E8AC7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CameraConfigDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCameraConfigDlg dialog

#include "DALSACamDH.h"
class CCameraConfigDlg : public CDialog
{
// Construction
public:
	BOOL GetDalsaCardState();
	void GetAcquisition(CDALSACamDH* pDalsaCamDh,int iAcqIndex);
	CCameraConfigDlg(CWnd* pParent = NULL);   // standard constructor
    SapAcquisition * m_pAcq;
	int m_iAcqIndex;
	CDALSACamDH  * m_pDalsaCamDH;

// Dialog Data
	//{{AFX_DATA(CCameraConfigDlg)
	enum { IDD = IDD_CAMERA_CONFIG };
	CComboBox	m_ComboFrameLevel;
	CComboBox	m_ComboFrameDetection;
	CComboBox	m_ComboTriggerMode;
	CButton	m_CheckFrameEnable;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCameraConfigDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCameraConfigDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboTriggerMode();
	afx_msg void OnSelchangeComboFrameLevel();
	afx_msg void OnSelchangeComboFrameDetection();
	afx_msg void OnCheckFarameEnable();
	afx_msg void OnStartGrab();
	afx_msg void OnStopGrab();
	afx_msg void OnKillfocusEditInternalFreq();
	afx_msg void OnKillfocusEditCropHeight();
	afx_msg void OnButtonSavetoccf();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMERACONFIGDLG_H__D7C8D59F_1035_469E_9022_CDE1A4E8AC7D__INCLUDED_)
