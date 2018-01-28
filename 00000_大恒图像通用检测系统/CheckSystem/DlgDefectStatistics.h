#pragma once


// CDlgDefectStatistics 对话框
#include "BarChart.h"

class CDlgDefectStatistics : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDefectStatistics)

public:
	CDlgDefectStatistics(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDefectStatistics();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DefectStatistics };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	CBarChart	m_chart;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
};
