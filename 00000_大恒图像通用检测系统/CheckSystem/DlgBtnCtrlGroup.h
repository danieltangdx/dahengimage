#pragma once


// CDlgBtnCtrlGroup 对话框

class CDlgBtnCtrlGroup : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgBtnCtrlGroup)

public:
	CDlgBtnCtrlGroup(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgBtnCtrlGroup();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_BtnCtrlGroup };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
