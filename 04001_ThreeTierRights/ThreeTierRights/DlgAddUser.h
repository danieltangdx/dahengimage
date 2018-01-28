#pragma once
#include "afxwin.h"


// CDlgAddUser 对话框

class CDlgAddUser : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgAddUser)

public:
	CDlgAddUser(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgAddUser();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_AddUser };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_iUserType;
	CString m_sUsername;
	CString m_sPassword1;
	CString m_sPassword2;
	CComboBox m_comboUserType;
	void* m_pThreeTierRightsInterior{nullptr};

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
