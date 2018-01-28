#if !defined(AFX_DLGCAMERASETTING_H__DDDBCBCE_FFAD_4798_929E_D0250F4F19AA__INCLUDED_)
#define AFX_DLGCAMERASETTING_H__DDDBCBCE_FFAD_4798_929E_D0250F4F19AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgCameraSetting.h : header file
//
#include "JAICamDH.h"

/////////////////////////////////////////////////////////////////////////////
// CDlgCameraSetting dialog

class CDlgCameraSetting : public CDialog
{
// Construction
public:
	CDlgCameraSetting(CWnd* pParent = NULL);   // standard constructor

	int m_iMinExp;				//最小曝光
	int m_iMaxExp;				//最大曝光
	int m_iMinGain;				//最小增益
	int m_iMaxGain;				//最大增益
	int m_iTriggerMode;			//触发模式 0：外触发 1：内触发

	CJAICamDH* m_pGrabber;		//采集对象
	CString m_sConfigFile;		//保存配置文件


	void SetOwner(CJAICamDH* pGrabber,CString strConfigFile); //传递指针 在domodel()之前调用



// Dialog Data
	//{{AFX_DATA(CDlgCameraSetting)
	enum { IDD = IDD_CAMERA_SETTING };
	CSliderCtrl	m_SliderGain;
	CSliderCtrl	m_SliderExp;
	CStatic	m_MinGain;
	CStatic	m_MaxGain;
	CStatic	m_MinExp;
	CStatic	m_MaxExp;
	CComboBox	m_ComboTrig;
	int		m_iExposureTime;
	int		m_iGain;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgCameraSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgCameraSetting)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCustomdrawSliderExp(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawSliderGain(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGCAMERASETTING_H__DDDBCBCE_FFAD_4798_929E_D0250F4F19AA__INCLUDED_)
