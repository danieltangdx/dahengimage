// CDlgSwitchModel.cpp : implementation file
//

#include "stdafx.h"
#include "CheckSystem.h"
#include "DlgSwitchModel.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "DlgCopyModel.h"
#include "CreateCustomModel.h"
#include "DlgRunning.h"


// CDlgSwitchModel dialog

IMPLEMENT_DYNAMIC(CDlgSwitchModel, CDialogEx)

CDlgSwitchModel::CDlgSwitchModel(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SwitchModel, pParent)
{

}

CDlgSwitchModel::~CDlgSwitchModel()
{
}

void CDlgSwitchModel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ModelName, m_listboxModelName);
}


BEGIN_MESSAGE_MAP(CDlgSwitchModel, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_LoadModel, &CDlgSwitchModel::OnBnClickedButtonLoadmodel)
	ON_EN_CHANGE(IDC_EDIT_Filter, &CDlgSwitchModel::OnEnChangeEditFilter)
	ON_BN_CLICKED(IDC_BUTTON_CopyModel, &CDlgSwitchModel::OnBnClickedButtonCopymodel)
	ON_BN_CLICKED(IDC_BUTTON_CreateCustomModel, &CDlgSwitchModel::OnBnClickedButtonCreatecustommodel)
	ON_BN_CLICKED(IDC_BUTTON_DeleteModel, &CDlgSwitchModel::OnBnClickedButtonDeletemodel)
END_MESSAGE_MAP()


// CDlgSwitchModel message handlers


BOOL CDlgSwitchModel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	InitModelList(_T("*.*"));
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CINICfg iniCfgAlg(CExFn::GetCurrentAppPath() + _T("\\Config\\AlgConfig.ini"));
	//CString sPathOfPlugin = CExFn::GetCurrentAppPath() + _T("\\Plugins\\CustomModel");
	bool bCustomModel = iniCfgAlg.GetBoolEx(_T("模板管理"), _T("是否使用自定义模板插件"), false);
	m_sPathFileOfPlugin = iniCfgAlg.GetStringEx(_T("模板管理"), _T("自定义模板插件名称"), m_sPathFileOfPlugin);
	m_sPathFileOfPlugin = CExFn::GetCurrentAppPath() + _T("\\Plugins\\CustomModel\\") + m_sPathFileOfPlugin;
#ifdef DEBUG
	m_sPathFileOfPlugin += _T("D");
#endif // DEBUG
	m_sPathFileOfPlugin += _T(".dll");
	GetDlgItem(IDC_BUTTON_CreateCustomModel)->ShowWindow(bCustomModel&(TRUE == PathFileExists(m_sPathFileOfPlugin))&pFrame->m_threeTierRights.VerifyRights(e_UserType_Debugger) ? SW_SHOW : SW_HIDE);
	GetDlgItem(IDC_BUTTON_CopyModel)->ShowWindow(pFrame->m_threeTierRights.VerifyRights(e_UserType_Debugger));
	GetDlgItem(IDC_BUTTON_DeleteModel)->ShowWindow(pFrame->m_threeTierRights.VerifyRights(e_UserType_Debugger));
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDlgSwitchModel::OnBnClickedButtonLoadmodel()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	int iIndex = m_listboxModelName.GetCurSel();
	CString sSelectedModelName{ _T("") };
	m_listboxModelName.GetText(iIndex, sSelectedModelName);
	if (pFrame->LoadModel(sSelectedModelName).bRet)
	{
		AfxMessageBox(tr("加载模板成功！"));
		pFrame->m_sModelName = sSelectedModelName;
		CDlgRunning * pDlgRunning = (CDlgRunning *)GetParent();
		pDlgRunning->SetDlgItemText(IDC_BUTTON_LABEL_MODEL_NAME, sSelectedModelName);
		pDlgRunning->UpdateData(FALSE);
		pFrame->m_iniAlgConfig.WriteString(_T("Demo Controls"), _T("DefaultModel"), pFrame->m_sModelName);
	}
	else
	{
		AfxMessageBox(tr("加载模板失败！"));
	}
}


bool CDlgSwitchModel::InitModelList(CString sFilter)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CExFn::FindSpecificFiles(CExFn::GetCurrentAppPath() + _T("\\ModelInfo"), sFilter, m_vec_sPathFileOfModel, m_vec_sModelName, false, 2);
	int iIndex = 0;
	m_listboxModelName.ResetContent();
	ASSERT(m_listboxModelName.GetCount() == 0);
	for (int i = 0; i < m_vec_sModelName.size(); ++i)
	{
		m_listboxModelName.AddString(m_vec_sModelName[i]);
		if (0 == m_vec_sModelName[i].Compare(pFrame->m_sModelName))
		{
			iIndex = i;
		}
	}
	m_listboxModelName.SetCurSel(iIndex);
	UpdateData(FALSE);
	return false;
}


void CDlgSwitchModel::OnEnChangeEditFilter()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData();
	CString sFilter{ _T("") };
	GetDlgItemText(IDC_EDIT_Filter, sFilter);
	if (sFilter == _T(""))
		sFilter = _T(".");
	sFilter = _T("*") + sFilter + _T("*");	
	InitModelList(sFilter);
}


void CDlgSwitchModel::OnBnClickedButtonCopymodel()
{
	// TODO: Add your control notification handler code here

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (!pFrame->m_threeTierRights.VerifyRights(e_UserType_Debugger))
		return;
	CDlgCopyModel dlg;
	if (IDOK == dlg.DoModal())//**[1/9/2018 ConanSteve]**:  添加模板成功
	{
		AfxMessageBox(tr("添加模板成功！"));
		InitModelList();
	}

	
		
}


void CDlgSwitchModel::OnBnClickedButtonCreatecustommodel()
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (!pFrame->m_threeTierRights.VerifyRights(e_UserType_Debugger))
		return;
	HMODULE hDll = LoadLibrary(m_sPathFileOfPlugin);
	if (NULL == hDll)
	{
		return;
	}
	bool bCreateSuccess{ false };
	pCreateCustomModel p_CreateCustomModel = (pCreateCustomModel)(GetProcAddress(hDll, _T("CreateCustomModel")));
	if (NULL != p_CreateCustomModel)
	{
		TCHAR buf[1024];
		memset(buf, 0, sizeof(buf));
		p_CreateCustomModel(this->m_hWnd ,buf);
		FreeLibrary(hDll);
	}
	InitModelList();
}


void CDlgSwitchModel::OnBnClickedButtonDeletemodel()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if (!pFrame->m_threeTierRights.VerifyRights(e_UserType_Debugger))
		return;
	if (pFrame->m_sModelName == m_vec_sModelName[m_listboxModelName.GetCurSel()])
	{
		MessageBox(tr("不能删除当前正在使用的模板"), _T("错误"), MB_OK | MB_ICONERROR);
		return;
	}
	if (IDYES == MessageBox(tr("删除后将不可恢复，确定删除当前模板吗？"), _T("警告"), MB_YESNO | MB_ICONQUESTION))
	{
		CExFn::DeleteDirectory(m_vec_sPathFileOfModel[m_listboxModelName.GetCurSel()]);
		InitModelList();
	}
	// TODO: Add your control notification handler code here
}
