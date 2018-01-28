#pragma once
#include "afxwin.h"


// CDlgSwitchModel dialog

class CDlgSwitchModel : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSwitchModel)

public:
	CDlgSwitchModel(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSwitchModel();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SwitchModel };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_listboxModelName;
	vector<CString> m_vec_sPathFileOfModel;
	vector<CString> m_vec_sModelName;
private:
	CString m_sPathFileOfPlugin{_T("")};//**[1/9/2018 ConanSteve]**:  自定义创建模板的dll路径

private:
	bool InitModelList(CString sFilter = _T("*.*"));
private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonLoadmodel();
	afx_msg void OnEnChangeEditFilter();
	afx_msg void OnBnClickedButtonCopymodel();
public:
	afx_msg void OnBnClickedButtonCreatecustommodel();
	afx_msg void OnBnClickedButtonDeletemodel();
};
