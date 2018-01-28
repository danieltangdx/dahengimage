#if !defined(AFX_DLGSTATISTIC_H__02FD4B4B_1892_4CAC_BC98_FFE999C4EA26__INCLUDED_)
#define AFX_DLGSTATISTIC_H__02FD4B4B_1892_4CAC_BC98_FFE999C4EA26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgStatistic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgStatistic dialog

class CDlgStatistic : public CDialog
{
// Construction
public:
	CDlgStatistic(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgStatistic)
	enum { IDD = IDD_DLG_STATISTIC };
	CListCtrl	m_listData;
	CComboBox	m_Model;
	CMonthCalCtrl	m_StopTime;
	CMonthCalCtrl	m_StartTime;
	//}}AFX_DATA

	CString m_sStartTime;		//开检时间
	CString m_sStopTime;		//停检时间
	CString m_sStartYear;		//开检-年
	CString m_sStartMonth;		//开检-月
	CString m_sStartDay;		//开检-日
	CString m_sStopYear;		//停检-年
	CString m_sStopMonth;		//停检-月
	CString m_sStopDay;			//停检-日
	CString m_sModelName;		//模板名称
	CString m_sAlgModelPath;	//模板路径

	BOOL m_bCheckDate;			//是否选中日期
	BOOL m_bCheckModel;			//是否选中模板名称

	_ConnectionPtr m_Connect;	// 连接数据库
	_RecordsetPtr m_RecordSet;	// 记录集

	int m_iTotalCount;			// 检测总数
	int m_iTotalGood;			// 好品总数
	int m_iTotalBad;			// 坏品总数
	int m_iFieldCount;			// 字段个数

	CPtrList m_PtrList;			// 存放记录集内容
	CStdioFile m_stdFile;		// 用于打开，写入，读出.csv文件

	COleDateTime m_DateTime;

public:
	void InitComboProduct();	//初始化模板组合框
	void Init(_ConnectionPtr pConn, _RecordsetPtr pRecord);	//获取访问变量
	BOOL GetRecordSet(CString sSQL);		// 获得记录集
	BOOL OutputRecord();					// 输出记录集
	void CloseCsvFile();					// 关闭Excel文件
	BOOL OpenCsvFile(CString strFilePath);	// 打开Excel文件


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgStatistic)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgStatistic)
	afx_msg void OnSelectStartTime(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeStartTime(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnExcel();
	afx_msg void OnBtnQuery();
	afx_msg void OnSelchangeStopTime(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSTATISTIC_H__02FD4B4B_1892_4CAC_BC98_FFE999C4EA26__INCLUDED_)
