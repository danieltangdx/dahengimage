// DlgUserManage.cpp : 实现文件
//

#include "stdafx.h"
#include "ThreeTierRights.h"
#include "ThreeTierRightsInterior.h"
#include "DlgUserManage.h"
#include "afxdialogex.h"
#include "resource.h"
#include "DlgAddUser.h"
#include "ExFn.h"

#include "..\\..\\IncPublic\CGridListCtrlEx\CGridColumnTraitDateTime.h"
#include "..\\..\\IncPublic\CGridListCtrlEx\CGridColumnTraitEdit.h"
#include "..\\..\\IncPublic\CGridListCtrlEx\CGridColumnTraitCombo.h"
#include "..\\..\\IncPublic\CGridListCtrlEx\CGridColumnTraitHyperLink.h"
#include "..\\..\\IncPublic\CGridListCtrlEx\CGridRowTraitXP.h"
#include "..\\..\\IncPublic\CGridListCtrlEx\ViewConfigSection.h"


// CDlgUserManage 对话框
extern HINSTANCE g_hInstance;
IMPLEMENT_DYNAMIC(CDlgUserManage, CDialogEx)

CDlgUserManage::CDlgUserManage(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_UserManage, pParent)
{

}

CDlgUserManage::~CDlgUserManage()
{
}

void CDlgUserManage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_UserInfo, m_listCtrlUserInfo);
	DDX_Control(pDX, IDC_COMBO_UserType, m_comboUserType);
	DDX_Control(pDX, IDC_CHECK_EnableUser, m_btnEnableUser);
	DDX_Control(pDX, IDC_EDIT_UserName, m_editUserName);
}


BEGIN_MESSAGE_MAP(CDlgUserManage, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_AddUser, &CDlgUserManage::OnBnClickedButtonAdduser)
	ON_BN_CLICKED(IDC_BUTTON_DeleteUser, &CDlgUserManage::OnBnClickedButtonDeleteuser)
	ON_NOTIFY(NM_CLICK, IDC_LIST_UserInfo, &CDlgUserManage::OnNMClickListUserinfo)
	ON_WM_SETFOCUS()
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDOK, &CDlgUserManage::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgUserManage::OnBnClickedCancel)
//	ON_REGISTERED_MESSAGE(WM_XLISTCTRL_COMBO_SELECTION, OnComboSelection)
END_MESSAGE_MAP()


// CDlgUserManage 消息处理程序


void CDlgUserManage::OnBnClickedButtonAdduser()
{
	// TODO: 在此添加控件通知处理程序代码
	CDlgAddUser dlg(this);
	dlg.m_pThreeTierRightsInterior = m_pThreeTierRightsInterior;
	if (IDOK == dlg.DoModal())
	{
		UpdateUserInfoList();
	}
	UpdateUserInfoList();
}




BOOL CDlgUserManage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	// Give better margin to editors


	int iCol = 0;



	//m_listCtrlUserInfo.InsertColumn(iCol++, tr("密码"), LVCFMT_LEFT, 100);
	m_arr_sUserTypeName.Add(tr("操作员"));
	m_arr_sUserTypeName.Add(tr("调试员"));
	m_arr_sUserTypeName.Add(tr("管理员"));
	m_comboUserType.AddString(tr("操作员"));
	m_comboUserType.AddString(tr("调试员"));
	m_comboUserType.AddString(tr("管理员"));
	//m_listCtrlUserInfo.SetColumnEditor(0, &CDlgUserManage::InitEditor, &CDlgUserManage::EndEditor, &m_btnEnableUser);
	//m_listCtrlUserInfo.SetColumnEditor(1, &CDlgUserManage::InitEditor, &CDlgUserManage::EndEditor, &);

	//m_ImageList.Create(1, 20, ILC_COLOR, 0, 1);
	//m_listCtrlUserInfo.SetImageList(&m_ImageList, LVSIL_SMALL);
	m_listCtrlUserInfo.SetExtendedStyle(m_listCtrlUserInfo.GetExStyle() | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_listCtrlUserInfo.InsertColumn(iCol++, tr("有效"), LVCFMT_LEFT, 50);
	m_listCtrlUserInfo.InsertColumn(iCol++, tr("用户名"), LVCFMT_LEFT, 100); //, tr("密码"), tr("用户等级")
	m_listCtrlUserInfo.InsertColumn(iCol++, tr("用户等级"), LVCFMT_LEFT, 100);
	m_listCtrlUserInfo.SetColumnReadOnly(0);
	m_listCtrlUserInfo.SetColumnEditor(2, &CDlgUserManage::InitEditor, &CDlgUserManage::EndEditor, &m_comboUserType);
	//m_listCtrlUserInfo.SetDefaultEditor(&m_editUserName);
	UpdateUserInfoList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

bool CDlgUserManage::UpdateUserInfoList()
{
	m_listCtrlUserInfo.DeleteAllItems();
	CThreeTierRightsInterior* pThreeTierRightsInterior = (CThreeTierRightsInterior*)m_pThreeTierRightsInterior;
	int i = 0;
	for (auto itera = pThreeTierRightsInterior->m_mapUserInfo.begin(); itera != pThreeTierRightsInterior->m_mapUserInfo.end(); ++itera)
	{
		int j = 0;
		if (_T("daheng") == itera->second.sUserName || _T("Admin") == itera->second.sUserName || _T("cs") == itera->second.sUserName)
			continue;
		m_listCtrlUserInfo.InsertItem(i, _T(""));
		m_listCtrlUserInfo.SetCheck(i, itera->second.bEnable);
		m_listCtrlUserInfo.SetItemText(i, j++, tr(""));
		m_listCtrlUserInfo.SetItemText(i, j++, itera->second.sUserName);
		//m_listCtrlUserInfo.SetComboBox(i, j++, TRUE, &m_arr_sUserTypeName, 5, itera->second.eUserType-1, FALSE);
		m_listCtrlUserInfo.SetItemText(i, j++, pThreeTierRightsInterior->ParseUserType(itera->second.eUserType));
		//m_listCtrlUserInfo.SetCellImage(i, j++, 1);
		//m_listCtrlUserInfo.SetItemText(i, j++, itera->second.sPassword);
		i++;
	}
// 	for (int i = 0; i < pThreeTierRightsInterior->m_mapUserInfo.size(); ++i)
// 	{
// 		if (i % 2 == 0)
// 			m_listCtrlUserInfo.SetRowColors(i, RGB(200, 0, 0), -1);
// 		if (i % 2 == 1)
// 			m_listCtrlUserInfo.SetRowColors(i, RGB(0, 0, 200), -1);
// 	}
	UpdateData(FALSE);
	return false;
}




void CDlgUserManage::OnBnClickedButtonDeleteuser()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sInfo{ _T("") }, sUserNameSelected{ _T("") };
	CThreeTierRightsInterior* pThreeTierRightsInteroir = (CThreeTierRightsInterior*)m_pThreeTierRightsInterior;
	sUserNameSelected = m_listCtrlUserInfo.GetItemText(m_iCurIndex, 1);
	if (sUserNameSelected == pThreeTierRightsInteroir->m_sCurUsername)
	{
		MessageBox(tr("无法删除当前用户！"), tr("错误"), MB_OK | MB_ICONERROR);
		return;
	}
	if (-1 == m_iCurIndex)
	{
		MessageBox(tr("请先选择要被删除的用户！"), tr("警告"), MB_OK | MB_ICONWARNING);
		return;
	}
	sInfo.Format(tr("确定要删除用户“%s”吗？"), sUserNameSelected);
	if (IDOK == MessageBox(sInfo, tr("警告"), MB_OKCANCEL | MB_ICONQUESTION))
	{
		CThreeTierRightsInterior* pThreeTierRightsInteroir = (CThreeTierRightsInterior*)m_pThreeTierRightsInterior;
		auto  itera_find = pThreeTierRightsInteroir->m_mapUserInfo.find(sUserNameSelected);
		if (pThreeTierRightsInteroir->m_mapUserInfo.end() != itera_find)
		{
			pThreeTierRightsInteroir->m_mapUserInfo.erase(itera_find);
// 			pThreeTierRightsInteroir->m_xml.FindChildElem(sUserNameSelected);
// 			pThreeTierRightsInteroir->m_xml.RemoveChildElem();
		}
		UpdateUserInfoList();
	}

}


void CDlgUserManage::OnNMClickListUserinfo(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	m_iCurIndex = pNMItemActivate->iItem;
	CThreeTierRightsInterior* pThreeTierRightsInteroir = (CThreeTierRightsInterior*)m_pThreeTierRightsInterior;
	if (m_iCurIndex >= pThreeTierRightsInteroir->m_mapUserInfo.size())
		m_iCurIndex = -1;
	*pResult = 0;
}


void CDlgUserManage::OnSetFocus(CWnd* pOldWnd)
{
	CDialogEx::OnSetFocus(pOldWnd);
	m_listCtrlUserInfo.SetFocus();
	m_listCtrlUserInfo.SetItemState(m_iCurIndex, LVIS_SELECTED, LVIS_SELECTED);
	UpdateData(FALSE);
	// TODO: 在此处添加消息处理程序代码
}


void CDlgUserManage::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码
	if (m_iCurIndex < 0)
		m_iCurIndex = 0;
	CThreeTierRightsInterior* pThreeTierRightsInteroir = (CThreeTierRightsInterior*)m_pThreeTierRightsInterior;
	if (m_iCurIndex >= pThreeTierRightsInteroir->m_mapUserInfo.size())
		m_iCurIndex = pThreeTierRightsInteroir->m_mapUserInfo.size() - 1;
	m_listCtrlUserInfo.SetItemState(m_iCurIndex, LVIS_SELECTED, LVIS_SELECTED);
}


void CDlgUserManage::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CThreeTierRightsInterior* pThreeTierRightsInteroir = (CThreeTierRightsInterior*)m_pThreeTierRightsInterior;
	pThreeTierRightsInteroir->m_xml.ResetMainPos();
	pThreeTierRightsInteroir->m_xml.FindElem(_T("UserInfo"));
	pThreeTierRightsInteroir->m_xml.RemoveElem();
	pThreeTierRightsInteroir->m_xml.AddElem(_T("UserInfo"));
	pThreeTierRightsInteroir->m_xml.AddAttrib(_T("enable"), pThreeTierRightsInteroir->m_bEnable ? _T("true") : _T("false"));
	pThreeTierRightsInteroir->m_xml.AddAttrib(_T("lastLoginUserName"), 0 == pThreeTierRightsInteroir->m_sCurUsername.Compare(_T("cs"))?_T(""): pThreeTierRightsInteroir->m_sCurUsername);
	CString sCtrlLabel{ _T("") };
	for (int i = 0; i < m_listCtrlUserInfo.GetItemCount(); ++i)
	{
		CString sUserName = _T("");
		sUserName = m_listCtrlUserInfo.GetItemText(i, 1);
		auto itera_find = pThreeTierRightsInteroir->m_mapUserInfo.find(sUserName);
		if (pThreeTierRightsInteroir->m_mapUserInfo.end() != itera_find)
		{
			itera_find->second.bEnable = m_listCtrlUserInfo.GetCheck(i);
		}
	}
	int i = 0;
	for (auto a: pThreeTierRightsInteroir->m_mapUserInfo)
	{
		if(_T("daheng") == a.first|| _T("Admin") == a.first ||_T("cs") == a.first)
			continue;
		sCtrlLabel.Format(_T("User_%d"), i++);
		pThreeTierRightsInteroir->m_xml.AddChildElem(sCtrlLabel);
		pThreeTierRightsInteroir->m_xml.AddChildAttrib(_T("username"),a.second.sUserName);
		pThreeTierRightsInteroir->m_xml.AddChildAttrib(_T("password"), a.second.sPassword);
		pThreeTierRightsInteroir->m_xml.AddChildAttrib(_T("userType"), a.second.eUserType);
		pThreeTierRightsInteroir->m_xml.AddChildAttrib(_T("enable"), a.second.bEnable?_T("true"):_T("false"));
	}
	pThreeTierRightsInteroir->SaveConfigFile();
	CDialogEx::OnOK();
}


void CDlgUserManage::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CThreeTierRightsInterior* pThreeTierRightsInteroir = (CThreeTierRightsInterior*)m_pThreeTierRightsInterior;
	pThreeTierRightsInteroir->SaveConfigFile();
	pThreeTierRightsInteroir->ReadConfigFile();
	CDialogEx::OnCancel();
}


LRESULT CDlgUserManage::OnComboSelection(WPARAM nItem, LPARAM nSubItem)
{
	CString strText = m_listCtrlUserInfo.GetItemText(nItem, nSubItem);
	//AfxMessageBox(strText);
	return 0;
}


int CDlgUserManage::ParseUserType(CString sUserTypeDesc)
{
	if (sUserTypeDesc == tr("操作员"))
		return e_UserType_Operator;
	if (sUserTypeDesc = tr("调试员"))
		return e_UserType_Debugger;
	if (sUserTypeDesc = tr("管理员"))
		return e_UserType_Admin;
	return e_UserType_Illegal;
}

BOOL CDlgUserManage::InitEditor(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate)
{
	ASSERT(*pWnd);
	switch (nColumn)
	{
// 	case 1:
// 	case 5:
// 	{
// 		CDateTimeCtrl *pDTC = dynamic_cast<CDateTimeCtrl *>(*pWnd);
// 		COleDateTime dt;
// 		if (dt.ParseDateTime(strSubItemText)) pDTC->SetTime(dt);
// 	}
// 	break;
	case 0:
	{
		CButton * pBtn = dynamic_cast<CButton *>(*pWnd);
		pBtn->SetCheck(TRUE);
		break;
	}
	case 2:
	{
		CComboBox *pCmb = dynamic_cast<CComboBox *>(*pWnd);
		pCmb->SelectString(0, strSubItemText);
	}
	break;


// 	case 3:
// 	{
// 		CDlgColor *pDlg = dynamic_cast<CDlgColor *>(*pWnd);
// 		pDlg->m_nColor = strSubItemText.CompareNoCase("green") ? (strSubItemText.CompareNoCase("blue") ? 0 : 2) : 1;
// 		pDlg->Create(CDlgColor::IDD, (CWnd*)pThis);
// 		pDlg->UpdateData(FALSE);
// 	}
// 	break;
	}
	return TRUE;
}
BOOL CDlgUserManage::EndEditor(CWnd** pWnd, int nRow, int nColumn, CString& strSubItemText, DWORD_PTR dwItemData, void* pThis, BOOL bUpdate)
{
	ASSERT(pWnd);
	switch (nColumn)
	{
// 	case 1:
// 	case 5:
// 	{
// 		CDateTimeCtrl *pDTC = dynamic_cast<CDateTimeCtrl *>(*pWnd);
// 		COleDateTime dt;
// 		pDTC->GetTime(dt);
// 		strSubItemText = dt.Format();
// 	}
// 	break;
	case 0:
	{
		CButton* pBtn = dynamic_cast<CButton *>(*pWnd);
		break;
	}
	case 2:
	{
		CComboBox *pCmb = dynamic_cast<CComboBox *>(*pWnd);
		int index = pCmb->GetCurSel();
		if (index >= 0)
		{
			CString sUserName{ _T("") };
			CListCtrlEx *pList = reinterpret_cast<CListCtrlEx *>(pThis);
			CDlgUserManage* pUserManage = (CDlgUserManage*)pList->GetParent();
			sUserName = pList->GetItemText(nRow, 1);
			CThreeTierRightsInterior* pThreeTier = (CThreeTierRightsInterior*)pUserManage->m_pThreeTierRightsInterior;
			auto itera_find = pThreeTier->m_mapUserInfo.find(sUserName);
			if (pThreeTier->m_mapUserInfo.end() != itera_find)
			{
				itera_find->second.eUserType = (e_UserType)(index + 1);
				pThreeTier->SaveConfigFile();
			}
		}
	}
	break;
// 	case 3:
// 	{
// 		CDlgColor *pDlg = dynamic_cast<CDlgColor *>(*pWnd);
// 		CListCtrlEx *pList = reinterpret_cast<CListCtrlEx *>(pThis);
// 		pDlg->UpdateData(TRUE);
// 		switch (pDlg->m_nColor)
// 		{
// 		case 1:
// 			strSubItemText = "Green";
// 			pList->SetCellColors(nRow, nColumn, RGB(0, 255, 0), -1);
// 			break;
// 		case 2:
// 			strSubItemText = "Blue";
// 			pList->SetCellColors(nRow, nColumn, RGB(0, 0, 255), -1);
// 			break;
// 		default:
// 			strSubItemText = "Red";
// 			pList->SetCellColors(nRow, nColumn, RGB(255, 0, 0), -1);
// 			break;
// 		}
// 		pDlg->DestroyWindow();
// 	}
// 	break;
	}
	return TRUE;
}