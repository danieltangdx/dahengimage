#if !defined(AFX_DLGDEFECTPAGE2_H__E994ACC0_3277_4211_939A_6225DA9F676D__INCLUDED_)
#define AFX_DLGDEFECTPAGE2_H__E994ACC0_3277_4211_939A_6225DA9F676D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgDefectPage2.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgDefectPage2 dialog

class CDlgDefectPage2 : public CDialog
{
// Construction
public:
	CDlgDefectPage2(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgDefectPage2)
	enum { IDD = IDD_DLG_DEFECT_PAGE_2 };
	CListCtrl	m_listData;
	CMonthCalCtrl	m_StopDate;
	CMonthCalCtrl	m_StartDate;
	CComboBox	m_cmbModelName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgDefectPage2)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgDefectPage2)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnInquiry();
	afx_msg void OnBtnExcel();
	afx_msg void OnSelchangeStartDate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeStopDate(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	// 变量
	BOOL m_bCheckDate;			//是否选中日期
	BOOL m_bCheckModel;			//是否选中模板名称
	
	_ConnectionPtr m_Connect;	// 连接数据库
	_RecordsetPtr m_RecordSet;	// 记录集
	
	int m_iTotalCount;			// 检测总数
	int m_iTotalGood;			// 好品总数
	int m_iTotalBad;			// 坏品总数
	int m_iFieldCount;			// 字段个数
	
	CString m_sModelName;		// 模板名称
	CString m_sAlgModelPath;	// 算法文件路径
	CString m_sStartTime;		//开检时间
	CString m_sStopTime;		//停检时间
	CString m_sStartYear;		//开检-年
	CString m_sStartMonth;		//开检-月
	CString m_sStartDay;		//开检-日
	CString m_sStopYear;		//停检-年
	CString m_sStopMonth;		//停检-月
	CString m_sStopDay;			//停检-日

	CPtrList m_PtrList;			// 存放记录集内容
	CStdioFile m_stdFile;		// 用于打开，写入，读出.csv文件
	CStringList m_listColName;	// 列表框每列名称
	
	// 函数
	void InitComboProduct();				// 初始化模板组合框
	BOOL GetRecordSet(CString sSQL);		// 获得记录集
	BOOL OutputRecord();					// 输出记录集
	BOOL CloseCsvFile();					// 关闭Excel文件
	BOOL OpenCsvFile(CString strFilePath);	// 打开Excel文件


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGDEFECTPAGE2_H__E994ACC0_3277_4211_939A_6225DA9F676D__INCLUDED_)
