#if !defined(AFX_SETPARAMDIALOG_H__EACE81E9_5A1A_4DB3_8B1D_727DBB438015__INCLUDED_)
#define AFX_SETPARAMDIALOG_H__EACE81E9_5A1A_4DB3_8B1D_727DBB438015__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SetParamDialog.h : header file
//

#include "GrabberRegular.h"
#include "CameraBase.h"
#include "afxcmn.h"
#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CSetParamDialog dialog
class CameraBase;
class CSetParamDialog : public CDialog
{
// Construction
public:
	CSetParamDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSetParamDialog)
	enum { IDD = IDD_SetParamDialog };
	int		m_ExposureTime;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetParamDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

	public:
		CameraBase* m_pCamera;
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetParamDialog)
	afx_msg void OnSetParam();
	afx_msg void OnSaveParam();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	// 设置宽度
	CSliderCtrl m_slider_width;
	// 设置高度
	CSliderCtrl m_slider_height;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	// //宽度显示
	CEdit m_widthStatic;
	// 高度
	CEdit m_heightStatic;

	bool m_check;
	afx_msg void OnBnClickedChangeimagesize();
	afx_msg void OnBnClickedCancel();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETPARAMDIALOG_H__EACE81E9_5A1A_4DB3_8B1D_727DBB438015__INCLUDED_)
