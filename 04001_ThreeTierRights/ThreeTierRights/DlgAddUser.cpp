// DlgAddUser.cpp : 实现文件
//

#include "stdafx.h"
#include "ThreeTierRights.h"
#include "DlgAddUser.h"
#include "ThreeTierRightsInterior.h"
#include "afxdialogex.h"
#include "resource.h"
#include "ExFn.h"


// CDlgAddUser 对话框

IMPLEMENT_DYNAMIC(CDlgAddUser, CDialogEx)

CDlgAddUser::CDlgAddUser(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_AddUser, pParent)
	, m_iUserType(0)
	, m_sUsername(_T(""))
	, m_sPassword1(_T(""))
	, m_sPassword2(_T(""))
{

}

CDlgAddUser::~CDlgAddUser()
{
}

void CDlgAddUser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_UserType, m_iUserType);
	DDX_Text(pDX, IDC_EDIT_UserName, m_sUsername);
	DDX_Text(pDX, IDC_EDIT_Password1, m_sPassword1);
	DDX_Text(pDX, IDC_EDIT_Password2, m_sPassword2);
	DDX_Control(pDX, IDC_COMBO_UserType, m_comboUserType);
}


BEGIN_MESSAGE_MAP(CDlgAddUser, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgAddUser::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDlgAddUser::OnBnClickedCancel)
END_MESSAGE_MAP()


// CDlgAddUser 消息处理程序


BOOL CDlgAddUser::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_comboUserType.Clear();
	m_comboUserType.AddString(tr("操作员"));
	m_comboUserType.AddString(tr("调试员"));
	m_comboUserType.AddString(tr("管理员"));
	m_comboUserType.SetCurSel(0);
	GetDlgItem(IDC_EDIT_UserName)->SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDlgAddUser::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (_T("") == m_sUsername)
	{
		MessageBox(tr("用户名不能为空，请重新输入！"), tr("警告"), MB_OK|MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_UserName)->SetFocus();
		return;
	}
	//int iRet = _ttoi(m_sUsername.GetAt(0));
// 	if (0 < iRet)
// 	{
// 		AfxMessageBox(tr("用户名第一个字符不能为数字，请重新输入！"));
// 		GetDlgItem(IDC_EDIT_UserName)->SetFocus();
// 		return;
// 	}
	CThreeTierRightsInterior* pThreeTierRightsInterior = (CThreeTierRightsInterior*)m_pThreeTierRightsInterior;//**[11/15/2017 ConanSteve]**:  检查用户名是否为空
	if (nullptr == pThreeTierRightsInterior)
	{
		CDialogEx::OnOK();
		return;
	}
	auto itera_find = pThreeTierRightsInterior->m_mapUserInfo.find(m_sUsername);
	if (pThreeTierRightsInterior->m_mapUserInfo.end() != itera_find)//**[11/15/2017 ConanSteve]**:  用户名已存在
	{
		MessageBox(tr("当前用户名已经存在，请输入其他用户名！"), tr("警告"), MB_OK | MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_UserName)->SetFocus();
		return;
	}
	/*************************************<检查密码是否重复>************************************[11/15/2017 ConanSteve]*/
// 	if (_T("") == m_sPassword1)
// 	{
// 		MessageBox(tr("密码1不能为空，请重新输入！"), tr("警告"), MB_OK | MB_ICONWARNING);
// 		GetDlgItem(IDC_EDIT_Password1)->SetFocus();
// 		return;
// 	}
// 	if (_T("") == m_sPassword2)
// 	{
// 		MessageBox(tr("密码2不能为空，请重新输入！"), tr("警告"), MB_OK | MB_ICONWARNING);
// 		GetDlgItem(IDC_EDIT_Password2)->SetFocus();
// 		return;
// 	}
	if (m_sPassword1 != m_sPassword2)
	{
		MessageBox(tr("密码不相同，请重新输入！"), tr("警告"), MB_OK | MB_ICONWARNING);
		GetDlgItem(IDC_EDIT_Password2)->SetFocus();
		return;
	}

	pThreeTierRightsInterior->m_mapUserInfo.insert(make_pair(m_sUsername, CUserInfo(m_sUsername, m_sPassword1, e_UserType(m_iUserType + 1))));
// 	pThreeTierRightsInterior->m_xml.AddChildElem(m_sUsername);
// 	pThreeTierRightsInterior->m_xml.AddChildAttrib(_T("password"), m_sPassword1);
// 	pThreeTierRightsInterior->m_xml.AddChildAttrib(_T("userType"), m_iUserType + 1);
	MessageBox(tr("添加用户名成功！"), tr("通知"), MB_OK | MB_ICONINFORMATION);
	CDialogEx::OnOK();
}


void CDlgAddUser::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
