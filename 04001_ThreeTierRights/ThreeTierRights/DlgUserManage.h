#pragma once
#include "afxcmn.h"
//#include "..\\..\\IncPublic\\XListCtrl\\XListCtrl.h"
//#pragma comment(lib, "..\\..\\IncPublic\\XListCtrl\\XListCtrlDDDA.lib")
#include "..\\..\\IncPublic\\CListCtrlEx\\ListCtrlEx.h"
#include "afxwin.h"

// CDlgUserManage 对话框

class CDlgUserManage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgUserManage)

public:
	CDlgUserManage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgUserManage();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_UserManage };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonAdduser();

private:
	CListCtrlEx m_listCtrlUserInfo;

public: 
	void* m_pThreeTierRightsInterior{ nullptr };
	int m_iCurIndex{ -1 };
	bool UpdateUserInfoList();
	virtual BOOL OnInitDialog();
	CStringArray m_arr_sUserTypeName;
	CImageList m_ImageList;
	afx_msg void OnBnClickedButtonDeleteuser();
	afx_msg void OnNMClickListUserinfo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg LRESULT OnComboSelection(WPARAM nItem, LPARAM nSubItem);
	int ParseUserType(CString sUserTypeDesc);
	static BOOL InitEditor(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate);
	static BOOL EndEditor(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate);
	CComboBox m_comboUserType;
	CButton m_btnEnableUser;
	CEdit m_editUserName;
};
