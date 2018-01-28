// DlgAddModel.cpp : implementation file
//

#include "stdafx.h"
#include "CheckSystem.h"
#include "DlgCopyModel.h"
#include "afxdialogex.h"
#include "DlgSwitchModel.h"


// CDlgCopyModel dialog

IMPLEMENT_DYNAMIC(CDlgCopyModel, CDialogEx)

CDlgCopyModel::CDlgCopyModel(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CopyModel, pParent)
{

}

CDlgCopyModel::~CDlgCopyModel()
{
}

void CDlgCopyModel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ModelList, m_comboModelList);
}


BEGIN_MESSAGE_MAP(CDlgCopyModel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CDlgCopyModel::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgCopyModel message handlers


BOOL CDlgCopyModel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	CDlgSwitchModel *pParent = (CDlgSwitchModel*)GetParent();
	for (int i = 0; i < pParent->m_vec_sModelName.size(); ++i)
	{
		m_comboModelList.AddString(pParent->m_vec_sModelName[i]);
	}
	m_comboModelList.SetCurSel(pParent->m_listboxModelName.GetCurSel());
	GetDlgItem(IDC_EDIT_NewModelName)->SetFocus();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgCopyModel::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CString sNewModelName{ _T("") };
	GetDlgItemText(IDC_EDIT_NewModelName, sNewModelName);
	if (_T("") == sNewModelName)
	{
		MessageBox(_T("配方名称不能为空！"), _T("错误"), MB_OK | MB_ICONERROR);
		GetDlgItem(IDC_EDIT_NewModelName)->SetFocus();
		return;
	}
	CDlgSwitchModel *pParent = (CDlgSwitchModel*)GetParent();
	for (int i = 0; i < pParent->m_vec_sModelName.size(); ++i)
	{
		if (sNewModelName == pParent->m_vec_sModelName[i])
		{
			MessageBox(_T("新配方名称已经存在，请重新输入！"), _T("错误"), MB_OK | MB_ICONERROR);
			GetDlgItem(IDC_EDIT_NewModelName)->SetFocus();
			return;
		}
	}
	CString sPathFileOfNewModel = CExFn::GetCurrentAppPath() + _T("\\ModelInfo\\") + sNewModelName;
	CString sErrorDesc{ _T("") };
	BOOL bSuccess = CExFn::CopyFolder(pParent->m_vec_sPathFileOfModel[pParent->m_listboxModelName.GetCurSel()], sPathFileOfNewModel);
	if (!bSuccess)
	{
		sErrorDesc = tr("拷贝失败，错误描述：\n") + sErrorDesc;
		MessageBox(sErrorDesc, _T("错误"), MB_OK | MB_ICONERROR);
		CExFn::DeleteDirectory(sPathFileOfNewModel);
		return;
	}
	CDialogEx::OnOK();
}
