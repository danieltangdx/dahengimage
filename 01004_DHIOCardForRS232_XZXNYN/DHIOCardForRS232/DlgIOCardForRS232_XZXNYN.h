#pragma once
#include "afxwin.h"
#include "DHIOCard.h"


// CDlgIOCardForRS232_XZXNYN 对话框

class CDlgIOCardForRS232_XZXNYN : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgIOCardForRS232_XZXNYN)

public:
	CDlgIOCardForRS232_XZXNYN(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgIOCardForRS232_XZXNYN();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RS232_XZXNYN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
public:
	CDHIOCard* m_pIOcard{nullptr};
	CComboBox m_comboComPort;
	CComboBox m_comboBoardId;
	bool InitWindow();
	virtual BOOL OnInitDialog();
	bool SetIOCardPoint(CDHIOCard* pIOCard);
	afx_msg void OnBnClickedButtonCtrl1(UINT uID);
	afx_msg void OnBnClickedButtonSwitch();
};
