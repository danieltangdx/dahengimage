#if !defined(AFX_DLGNEWMODEL_H__1FE194E1_3B52_4698_9E6C_5255C31C5B81__INCLUDED_)
#define AFX_DLGNEWMODEL_H__1FE194E1_3B52_4698_9E6C_5255C31C5B81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgNewModel.h : header file
//
#define MAX_MEDICINE_TYPE 200  //药品类型最大数目

/////////////////////////////////////////////////////////////////////////////
// CDlgNewModel dialog

class CDlgNewModel : public CDialog
{
// Construction
public:
	CDlgNewModel(CWnd* pParent = NULL);   // standard constructor


	CString m_sModelName;	//模板名称

	CString m_sNewName;		//产品名称[2015-5-6]
	CString m_sNewSpec;		//产品规格
	CString m_sNewPercent;		//产品百分比

	CString m_sGroupName[MAX_MEDICINE_TYPE];	//产品名称集合总数
	CString m_sGroupSpec[MAX_MEDICINE_TYPE];	//产品规格集合总数
	CString m_sGroupPercent[MAX_MEDICINE_TYPE];	//产品百分比集合总数

	int m_iName; //产品名称[2015-5-6]
	int m_iSpec; //产品规格[2015-5-6]
	int m_iPercent; //产品百分比[2015-5-6]

	int m_iNameIndex; //产品名称
	int m_iSpecIndex; //产品规格
	int m_iPercentIndex; //产品百分比

	int m_iLabelType;		//标签类型（命名方式根据算法系统通信协议规定）

	int m_iBodyShape; // 100为塑料瓶 101为可立袋
	int m_iHeadShape; // 100为拉环 101为双阀


// Dialog Data
	//{{AFX_DATA(CDlgNewModel)
	enum { IDD = IDD_DLG_NEW_MODEL };
	CComboBox	m_cmbSpec;
	CComboBox	m_cmbPercent;
	CComboBox	m_cmbName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgNewModel)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgNewModel)
	virtual void OnOK();
	afx_msg void OnRadioBodyShape1();
	afx_msg void OnRadioBodyShape2();
	afx_msg void OnRadioHeadShape1();
	afx_msg void OnRadioHeadShape2();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboName();
	afx_msg void OnSelchangeComboPercent();
	afx_msg void OnSelchangeComboSpec();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGNEWMODEL_H__1FE194E1_3B52_4698_9E6C_5255C31C5B81__INCLUDED_)
