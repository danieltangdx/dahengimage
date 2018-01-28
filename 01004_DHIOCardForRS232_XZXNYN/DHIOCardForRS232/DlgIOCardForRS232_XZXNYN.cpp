// DlgIOCardForRS232_XZXNYN.cpp : 实现文件
//

#include "stdafx.h"
#include "DHIOCardForRS232_XZXNYNInterior.h"
#include "DlgIOCardForRS232_XZXNYN.h"
#include "afxdialogex.h"
#include "resource.h"


// CDlgIOCardForRS232_XZXNYN 对话框

IMPLEMENT_DYNAMIC(CDlgIOCardForRS232_XZXNYN, CDialogEx)

CDlgIOCardForRS232_XZXNYN::CDlgIOCardForRS232_XZXNYN(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_RS232_XZXNYN, pParent)
{

}

CDlgIOCardForRS232_XZXNYN::~CDlgIOCardForRS232_XZXNYN()
{
}

void CDlgIOCardForRS232_XZXNYN::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_COM, m_comboComPort);
	DDX_Control(pDX, IDC_COMBO_BoardId, m_comboBoardId);
}


BEGIN_MESSAGE_MAP(CDlgIOCardForRS232_XZXNYN, CDialogEx)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_Ctrl1, IDC_BUTTON_Ctrl8, &CDlgIOCardForRS232_XZXNYN::OnBnClickedButtonCtrl1)
	ON_BN_CLICKED(IDC_BUTTON_Switch, &CDlgIOCardForRS232_XZXNYN::OnBnClickedButtonSwitch)
END_MESSAGE_MAP()


// CDlgIOCardForRS232_XZXNYN 消息处理程序




BOOL CDlgIOCardForRS232_XZXNYN::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString sTemp{ _T("") };
	for (int i = 0; i < 16; ++i)
	{
		sTemp.Format(_T("COM%d"), i + 1);
		m_comboComPort.AddString(sTemp);
	}
	for (int i = 0; i < 16; ++i)
	{
		sTemp.Format(_T("模块%d"), i + 1);
		m_comboBoardId.AddString(sTemp);
	}
	InitWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


bool CDlgIOCardForRS232_XZXNYN::InitWindow()
{
	CDHIOCardForRS232_XZXNYNInterior* pIOCard = (CDHIOCardForRS232_XZXNYNInterior*)m_pIOcard;
	if (nullptr != pIOCard)
	{
		m_comboComPort.SetCurSel(pIOCard->m_iPort - 1);
		m_comboBoardId.SetCurSel(pIOCard->m_iBoardId - 1);
		m_comboComPort.EnableWindow(!pIOCard->m_bOpenCardSuccess);
		m_comboBoardId.EnableWindow(!pIOCard->m_bOpenCardSuccess);
		GetDlgItem(IDC_EDIT_FrameDelay)->EnableWindow(!pIOCard->m_bOpenCardSuccess);
		SetDlgItemInt(IDC_EDIT_FrameDelay, pIOCard->m_iFrameDelayOfKick);
// 		for (int i = 0; i < c_CoilNum; ++i)
// 		{
// 			GetDlgItem(IDC_BUTTON_Ctrl1 + i)->EnableWindow(!pIOCard->m_bOpenCardSuccess);
// 		}
		if (pIOCard->m_bOpenCardSuccess)
		{
			SetDlgItemText(IDC_BUTTON_Switch, _T("关闭串口"));
		}
		else
		{
			SetDlgItemText(IDC_BUTTON_Switch, _T("打开串口"));
		}
	}
	UpdateData(FALSE);
	return false;
}


bool CDlgIOCardForRS232_XZXNYN::SetIOCardPoint(CDHIOCard* pIOCard)
{
	m_pIOcard = pIOCard;
	return true;
}


void CDlgIOCardForRS232_XZXNYN::OnBnClickedButtonCtrl1(UINT uID)
{
	// TODO: 在此添加控件通知处理程序代码
	int iCtrlId = uID - IDC_BUTTON_Ctrl1;
	CDHIOCardForRS232_XZXNYNInterior* pIOCard = (CDHIOCardForRS232_XZXNYNInterior*)m_pIOcard;
	if (0 <= iCtrlId&&iCtrlId < c_CoilNum &&nullptr != pIOCard)
	{
		pIOCard->SwitchState(iCtrlId);
	}
}


void CDlgIOCardForRS232_XZXNYN::OnBnClickedButtonSwitch()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	CDHIOCardForRS232_XZXNYNInterior* pIOCard = (CDHIOCardForRS232_XZXNYNInterior*)m_pIOcard;
	if (nullptr != pIOCard)
	{
		if (!pIOCard->m_bOpenCardSuccess)
		{
			pIOCard->m_iPort = m_comboComPort.GetCurSel() + 1;
			pIOCard->m_iBoardId = m_comboBoardId.GetCurSel() + 1;
			pIOCard->m_iFrameDelayOfKick = GetDlgItemInt(IDC_EDIT_FrameDelay);
			pIOCard->CloseCard();
			if (pIOCard->OpenCard())
			{
				AfxMessageBox(_T("打开串口成功！"));
			}
			else
			{
				AfxMessageBox(_T("打开串口失败！"));
			}
		}
		else
		{
			pIOCard->CloseCard();
		}
	}
	InitWindow();
}
