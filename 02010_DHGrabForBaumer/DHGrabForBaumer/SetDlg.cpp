// SetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GrabberForBaumer.h"
#include "SetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SetDlg dialog


SetDlg::SetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(SetDlg)
	m_nEx = 0;
	m_fGain = 0.0f;
	m_nL = 0;
	m_nR = 0;
	m_nT = 0;
	m_nB = 0;
	//}}AFX_DATA_INIT
	m_p = NULL;
}


void SetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SetDlg)
	DDX_Control(pDX, IDC_COMBO_TRIGGER_MODE, m_TriggerMode);
	DDX_Text(pDX, IDC_EDIT_EX, m_nEx);
	DDX_Text(pDX, IDC_EDIT_GAIN, m_fGain);
	DDX_Text(pDX, IDC_EDIT_L, m_nL);
	DDX_Text(pDX, IDC_EDIT_R, m_nR);
	DDX_Text(pDX, IDC_EDIT_T, m_nT);
	DDX_Text(pDX, IDC_EDIT_B, m_nB);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_IMAGE_FLIP_TYPE, m_cmbFlipType);
}


BEGIN_MESSAGE_MAP(SetDlg, CDialog)
	//{{AFX_MSG_MAP(SetDlg)
	ON_BN_CLICKED(IDC_BUTTON_A1, OnButtonA1)
	ON_BN_CLICKED(IDC_BUTTON_A2, OnButtonA2)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_A3, OnButtonA3)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_MODE, OnSelchangeComboTriggerMode)
	//}}AFX_MSG_MAP
	ON_CBN_SELCHANGE(IDC_COMBO_IMAGE_FLIP_TYPE, &SetDlg::OnSelchangeComboImageFlipType)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SetDlg message handlers
BOOL SetDlg::GetAcquisition(CBaumerCamDH *p)
{
	m_p = p;
	return TRUE;
}

void SetDlg::OnButtonA1() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_A1)->EnableWindow(FALSE);
	UpdateData(TRUE);
	if (m_nEx < m_nExMin)
	{
		m_nEx = m_nExMin;
	}
	
	if (m_nEx > m_nExMax)
	{
		m_nEx = m_nExMax;
	}

	m_p->SetParamInt(GBParamID::GBShutter,m_nEx);//:GBShutter,				//相机快门值

	m_p->GetParamInt(GBParamID::GBShutter,m_nEx);

	UpdateData(FALSE);
	GetDlgItem(IDC_BUTTON_A1)->EnableWindow(TRUE);

}

BOOL SetDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_p->GetParamInt(GBParamID::GBShutter,m_nEx);
	m_p->GetParamInt(GBParamID::GBExposureMax,m_nExMax);
	m_p->GetParamInt(GBParamID::GBSequenceNum,m_nExMin);
	m_p->GetParamfloat(9,m_fGain);
	m_p->GetParamfloat(16,m_nGainMax);
	m_p->GetParamfloat(15,m_nGainMin);
	RECT rAOI;
	m_p->GetAOI(rAOI);
	m_nL = rAOI.left;
	m_nR = rAOI.right;
	m_nT = rAOI.top;
	m_nB = rAOI.bottom;

	int ParamValOutput = -1;
	m_p->GetTriggerParam(ParamValOutput);
	if (ParamValOutput == 0)
	{
		m_TriggerMode.SetCurSel(0);
	}
	else if (ParamValOutput == 9)
	{
		m_TriggerMode.SetCurSel(1);
	}
	else
	{
		AfxMessageBox("触发模式非法!");
	}

	// 获取图像翻转模式 [4/25/2014 SJC
	int iFlipmode = 0;
	GetPrivateProfileInt("camera","flipmode",iFlipmode, m_p->m_sInitFile);
	m_cmbFlipType.SetCurSel(iFlipmode);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void SetDlg::OnButtonA2() 
{
	// TODO: Add your control notification handler code here

	GetDlgItem(IDC_BUTTON_A2)->EnableWindow(FALSE);
	UpdateData(TRUE);
	if (m_fGain < m_nGainMin)
	{
		m_fGain = m_nGainMin;
	}

	if (m_fGain > m_nGainMax)
	{
		m_fGain = m_nGainMax;
	}

	m_p->SetParamfloat(9,m_fGain);//

	m_p->GetParamfloat(9,m_fGain);
	UpdateData(FALSE);
	GetDlgItem(IDC_BUTTON_A2)->EnableWindow(TRUE);

}

void SetDlg::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
	CString str;
	str.Format("%d",m_nEx);
	WritePrivateProfileString("camera", "exposure", str, m_p->m_sInitFile);
	str.Format("%f",m_fGain);
	WritePrivateProfileString("camera", "gain", str, m_p->m_sInitFile);
	str.Format("%d",m_nL);
	WritePrivateProfileString("camera", "roileft", str, m_p->m_sInitFile);
	str.Format("%d",m_nR);
	WritePrivateProfileString("camera", "roiright", str, m_p->m_sInitFile);
	str.Format("%d",m_nT);
	WritePrivateProfileString("camera", "roitop", str, m_p->m_sInitFile);
	str.Format("%d",m_nB);
	WritePrivateProfileString("camera", "roibottom", str, m_p->m_sInitFile);

	int ParamValOutput = -1;
	m_p->GetTriggerParam(ParamValOutput);
	str.Format("%d",ParamValOutput);
	WritePrivateProfileString("camera", "trigger", str, m_p->m_sInitFile);

	// 增加图像翻转模式 [4/25/2014 SJC]
	str.Format("%d",m_cmbFlipType.GetCurSel());
	WritePrivateProfileString("camera","flipmode",str,m_p->m_sInitFile);

	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
}

void SetDlg::OnButtonA3() 
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON_A3)->EnableWindow(FALSE);
	UpdateData(TRUE);
	RECT rAOI;
	rAOI.left = m_nL;
	rAOI.right = m_nR;
	rAOI.top = m_nT;
	rAOI.bottom = m_nB;
	BOOL b = m_p->SetAOI(rAOI);//
	if (!b)
	{
		AfxMessageBox("设置AOI区域失败!");
	}

	m_p->GetAOI(rAOI);
	m_nL = rAOI.left;
	m_nR = rAOI.right;
	m_nT = rAOI.top;
	m_nB = rAOI.bottom;

	UpdateData(FALSE);
	GetDlgItem(IDC_BUTTON_A3)->EnableWindow(TRUE);
}

void SetDlg::OnSelchangeComboTriggerMode() 
{
	UpdateData(TRUE);
	int nval = m_TriggerMode.GetCurSel();

	int  eTriggerValue; 
	
	switch(nval) 
	{
	case 0:
		eTriggerValue = 0;  //AVTTriggerOff 内触发模式  
		break;
	case 1:
		eTriggerValue = 9;  //AVTTriggerOn  //上升沿触发//外触发开启
		break;
	default:
		eTriggerValue = 0;  //AVTTriggerOff 内触发模式
	}
	
	m_p->SetTriggerParam(eTriggerValue);
	UpdateData(FALSE);
	
}


void SetDlg::OnSelchangeComboImageFlipType()
{
	// TODO: 在此添加控件通知处理程序代码
	m_p->SetImageFlipMode(m_cmbFlipType.GetCurSel());
}
