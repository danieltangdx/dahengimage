#pragma once


// CDlgUserManage 对话框

class CDlgUserManage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgUserManage)

public:
	CDlgUserManage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgUserManage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_UserManage };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
