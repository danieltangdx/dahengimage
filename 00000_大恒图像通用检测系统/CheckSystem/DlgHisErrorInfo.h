#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CDlgHisErrorInfo 对话框

class CDlgHisErrorInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHisErrorInfo)

public:
	CDlgHisErrorInfo(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgHisErrorInfo();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_HisErrorInfo };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()


public:
	int m_iIndexOfCheckGroupSelected{ 0 };
	CComboBox m_comboCheckGroup;
	CListCtrl m_listctrlDefectStatistics;
	virtual BOOL OnInitDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	bool UpdateDefectList();
	afx_msg void OnCbnSelchangeComboCheckgourp();
};
