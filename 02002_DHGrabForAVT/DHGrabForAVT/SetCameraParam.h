#if !defined(AFX_SETCAMERAPARAM_H__81FEB935_2DA9_4AE2_8D02_5CCF886B3247__INCLUDED_)
#define AFX_SETCAMERAPARAM_H__81FEB935_2DA9_4AE2_8D02_5CCF886B3247__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetCameraParam.h : header file
//
#include "AVTCamDH.h"
#include "ExFn.h"
/////////////////////////////////////////////////////////////////////////////
// CSetCameraParam dialog

class CSetCameraParam : public CDialog
{
// Construction
public:
	CString m_strLanType;		 // 语言文件类型 [4/8/2014 SJC]
	void SetDiscriptionInfo(CString strLanType);//根据语言类型更新描述信息

	UINT   m_iMaxWhiteBalCB;     // 最大的白平衡CB分量 
	UINT   m_iMaxWhiteBalCR;     // 最大的白平衡CR分量
	UINT   m_iMaxBrightness;     // 最大亮度
	UINT   m_iMaxGain;           // 最大增益
	UINT   m_iMaxShutter;        // 最大曝光
	UINT   m_iMinBrightness;     // 最小亮度
	UINT   m_iMinGain;           // 最小增益
	UINT   m_iMinShutter;        // 最小曝光
	CSetCameraParam(CWnd* pParent = NULL);   // standard constructor
	void SetOwner(CAVTCamDH *pGrabber, const CString &strProfileName);
	void EnableShow(BOOL bEnable);

// Dialog Data
	//{{AFX_DATA(CSetCameraParam)
	enum { IDD = IDD_SET_CAMERA_DIALOG };
	CComboBox	m_TriggerMode;
	UINT		m_nTriggerMode;
	UINT		m_nBrightness;
	UINT		m_nGain;
	UINT		m_nShutter;
	int			m_nBrightnessSlider;
	int			m_nGainSlider;
	int			m_nShutterSlider;
	UINT	m_nAOILeft;
	UINT	m_nAOITop;
	UINT	m_nAOIWidth;
	UINT	m_nAOIHeight;
	UINT	m_nCB;
	UINT	m_nCR;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetCameraParam)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	DWORD           m_OldFrameCnt; 

	// Generated message map functions
	//{{AFX_MSG(CSetCameraParam)
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeEditBrightness();
	afx_msg void OnChangeEditGain();
	afx_msg void OnChangeEditShutter();
	virtual void OnOK();
	afx_msg void OnSelchangeComboTriggerMode();
	afx_msg void OnClose();
	afx_msg void OnBtnApply();
	afx_msg void OnButtonAoiApply();
	afx_msg void OnButtonFrametest();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnButtonLogin();
	afx_msg void OnButtonAoiApplywb();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CAVTCamDH *m_pGrabber;
	CString m_strProfileName;

	UINT m_nOldBrightness;      //修改前的亮度
	UINT m_nOldGain;            //修改前的增益
	UINT m_nOldShutter;         //修改前的Shutter
	UINT m_nOldTriggerMode;     //修改前的触发模式

	UINT m_iOldCB;  //修改前的白平衡UB值
	UINT m_iOldCR;  //修改前的白平衡VR值

	// 设置SLIDER控件的最大最小值和滑动距离
	void SetSliderRange();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETCAMERAPARAM_H__81FEB935_2DA9_4AE2_8D02_5CCF886B3247__INCLUDED_)
