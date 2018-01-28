// DlgHisErrorInfo.cpp : 实现文件
//

#include "stdafx.h"
#include "CheckSystem.h"
#include "MainFrm.h"
#include "DlgHisErrorInfo.h"
#include "afxdialogex.h"
#include "ExFn.h"


// CDlgHisErrorInfo 对话框

IMPLEMENT_DYNAMIC(CDlgHisErrorInfo, CDialogEx)

CDlgHisErrorInfo::CDlgHisErrorInfo(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_HisErrorInfo, pParent)
{

}

CDlgHisErrorInfo::~CDlgHisErrorInfo()
{
}

void CDlgHisErrorInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CheckGourp, m_comboCheckGroup);
	DDX_Control(pDX, IDC_LIST_DefectStatistics, m_listctrlDefectStatistics);
}


BEGIN_MESSAGE_MAP(CDlgHisErrorInfo, CDialogEx)
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_COMBO_CheckGourp, &CDlgHisErrorInfo::OnCbnSelchangeComboCheckgourp)
END_MESSAGE_MAP()


// CDlgHisErrorInfo 消息处理程序


BOOL CDlgHisErrorInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CINICfg iniAlgCfg(CExFn::GetCurrentAppPath() + _T("\\Config\\AlgConfig.ini"));
	CString sModuleName{ _T("") };
	sModuleName = iniAlgCfg.GetStringEx(_T("Demo Controls"), _T("DefaultModel"), sModuleName);
	CINICfg iniCheckSysCfg(CExFn::GetCurrentAppPath() + _T("\\ModelInfo\\") + sModuleName +_T("\\CheckSystemConfig.ini"));
	int iCheckGroupNum = iniCheckSysCfg.GetIntEx(_T("Inspection Parameters"), _T("CheckNum"));


	m_comboCheckGroup.AddString(tr("全工位"));
	for (int i = 0; i < iCheckGroupNum; ++i)
	{
		CString sGroupName{ _T("") };
		sGroupName.Format(tr("工位%d"), i + 1);
		m_comboCheckGroup.AddString(sGroupName);
	}
	m_comboCheckGroup.SetCurSel(0);
	CLayout layout(CExFn::GetCurrentAppPath()+_T("\\Config\\LayoutRunDlg.xml"), CRect(0,0,0,0));
	int iWidthOfDefectDesc = layout.GetAttrib(_T("HistoryErrorInfo"), _T("widthOfDefectDesc"), 100);//**[12/4/2017 ConanSteve]**:  缺陷描述的列宽度
	int iWidthOfDefectCnt = layout.GetAttrib(_T("HistoryErrorInfo"), _T("widthOfDefectCount"), 60);//**[12/4/2017 ConanSteve]**:  缺陷计数的列宽度
	m_listctrlDefectStatistics.InsertColumn(0, tr("缺陷描述"), LVCFMT_LEFT, iWidthOfDefectDesc);
	m_listctrlDefectStatistics.InsertColumn(1, tr("缺陷计数"), LVCFMT_LEFT, iWidthOfDefectCnt);
	m_listctrlDefectStatistics.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	//UpdateDefectList();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


int CDlgHisErrorInfo::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	
	return 0;
}


bool CDlgHisErrorInfo::UpdateDefectList()
{
	UpdateData();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	m_listctrlDefectStatistics.DeleteAllItems();
	m_iIndexOfCheckGroupSelected = m_comboCheckGroup.GetCurSel();
	if(0== m_iIndexOfCheckGroupSelected)
	{
		for (int iCheckGroupSN = 0; iCheckGroupSN < pFrame->m_iCheckGroupNum; ++iCheckGroupSN)
		{
			map<int, s_DefectInfo> mapDefect = pFrame->m_arr_mapDefectStatistics[iCheckGroupSN];//**[12/1/2017 ConanSteve]**:  这是个map
			int i = 0;
			for (auto itera = mapDefect.begin(); itera != mapDefect.end(); ++itera)
			{
				m_listctrlDefectStatistics.InsertItem(i, _T(""));
				m_listctrlDefectStatistics.SetItemText(i, 0, itera->second.sDesc);
				CString sCnt{ _T("") };
				sCnt.Format(_T("%d"), itera->second.iCnt);
				m_listctrlDefectStatistics.SetItemText(i, 1, sCnt);
				++i;
			}
		}
	}
	else if(m_iIndexOfCheckGroupSelected>0 &&m_iIndexOfCheckGroupSelected<= pFrame->m_iCheckGroupNum)
	{
		map<int, s_DefectInfo> mapDefect = pFrame->m_arr_mapDefectStatistics[m_iIndexOfCheckGroupSelected - 1];//**[12/1/2017 ConanSteve]**:  这是个map
		int i = 0;
		for (auto itera = mapDefect.begin(); itera != mapDefect.end(); ++itera)
		{
			m_listctrlDefectStatistics.InsertItem(i, _T(""));
			m_listctrlDefectStatistics.SetItemText(i, 0, itera->second.sDesc);
			CString sCnt{ _T("") };
			sCnt.Format(_T("%d"), itera->second.iCnt);
			m_listctrlDefectStatistics.SetItemText(i, 1, sCnt);
			++i;
		}
	}
	UpdateData(FALSE);
	return false;
}


void CDlgHisErrorInfo::OnCbnSelchangeComboCheckgourp()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateDefectList();
}
