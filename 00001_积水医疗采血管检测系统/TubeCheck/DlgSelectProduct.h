#if !defined(AFX_SELECTPRODUCTDLG_H__44822648_D1DD_406D_A700_2F5DFE3EA761__INCLUDED_)
#define AFX_SELECTPRODUCTDLG_H__44822648_D1DD_406D_A700_2F5DFE3EA761__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSelectProduct.h : header file
//
// #include <vector>
/////////////////////////////////////////////////////////////////////////////
// CDlgSelectProduct dialog
#include "MainFrm.h"

#define ID_START_MODIFY_MODEL 101

class CDlgSelectProduct : public CDialog
{
// Construction
public:
	CDlgSelectProduct(CWnd* pParent = NULL);   // standard constructor

	CString m_strAlgIniPath;	//算法配置文件夹路径
	CString m_strAppPath;		//应用程序所在路径
	int     m_nCount;
	int     m_nCameraSN;
// Dialog Data
	//{{AFX_DATA(CDlgSelectProduct)
	enum { IDD = IDD_DLG_SELECT_PRODUCT };
	CListBox	m_ctrList;
	CString	m_strEditNew;
	//}}AFX_DATA

public:
	CString GetCurrentAppPath();	
	void RefreshList();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSelectProduct)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSelectProduct)
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnButtonNew();
	afx_msg void OnButtonDelete();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnBtnModify();
	bool DeleteDirectory(char* sDirName);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTPRODUCTDLG_H__44822648_D1DD_406D_A700_2F5DFE3EA761__INCLUDED_)
