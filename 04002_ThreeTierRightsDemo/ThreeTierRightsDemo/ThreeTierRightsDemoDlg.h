
// ThreeTierRightsDemoDlg.h : 头文件
//

#pragma once
#include "ThreeTierRights.h"

// CThreeTierRightsDemoDlg 对话框
class CThreeTierRightsDemoDlg : public CDialogEx
{
// 构造
public:
	CThreeTierRightsDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_THREETIERRIGHTSDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLogin();
	afx_msg void OnBnClickedButtonUsermanage();
	afx_msg void OnBnClickedButtonOperator();
	afx_msg void OnBnClickedButtonDebugger();

private:
	CThreeTierRights m_threeTierRights;
public:
	afx_msg void OnBnClickedButtonAdmin();
	afx_msg void OnBnClickedButtonAdmin2();
	afx_msg void OnBnClickedButtonDebugger2();
	afx_msg void OnBnClickedButtonOperator2();
	afx_msg void OnBnClickedButtonUsermanage2();
	afx_msg void OnBnClickedButtonSuperadmin();
};
