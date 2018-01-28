#if !defined(AFX_DLGSHOWIMAGE_H__513E4BAC_4BA3_4AA1_9A27_53D990295AAD__INCLUDED_)
#define AFX_DLGSHOWIMAGE_H__513E4BAC_4BA3_4AA1_9A27_53D990295AAD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgShowImage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgShowImage dialog
#include "MainFrm.h"

class CDlgShowImage : public CDialog
{
// Construction
public:
	CDlgShowImage(CWnd* pParent = NULL);   // standard constructor

public:

	int m_iWidth;								//宽
	int m_iHeight;								//高度
	int m_iBytes;								//位数
	int m_iCameraSN;							//记录当前工位号

	CString m_sSaveImagePath[CAMERA_COUNT];		//保存图像路径
	CString m_sErrorDes[CAMERA_COUNT];			//错误描述
	CImage *m_pImage[CAMERA_COUNT];				//图像内存
	CRect m_lRect[CAMERA_COUNT];				//显示图像区域
	CDC *m_pDC[CAMERA_COUNT];					//显示DC
	

	BOOL m_bFirstInit[CAMERA_COUNT];			//是否已经初始化

	// 显示原图像
	void ShowImage();

	// 打开图像
	BOOL OpenImage(CString strPath, CDC *pDC, CRect lRect, int iCameraSN);


// Dialog Data
	//{{AFX_DATA(CDlgShowImage)
	enum { IDD = IDD_DLG_SHOW_IMAGE };
	CStatic	m_imgPic1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgShowImage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgShowImage)
	afx_msg void OnBtnImage();
	afx_msg void OnBtnParam();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSHOWIMAGE_H__513E4BAC_4BA3_4AA1_9A27_53D990295AAD__INCLUDED_)
