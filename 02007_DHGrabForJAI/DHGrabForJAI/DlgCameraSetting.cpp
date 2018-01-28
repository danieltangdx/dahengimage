// DlgCameraSetting.cpp : implementation file
//

#include "stdafx.h"
#include "JAIGrabber.h"
#include "DlgCameraSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCameraSetting dialog


CDlgCameraSetting::CDlgCameraSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCameraSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCameraSetting)
	m_iExposureTime = 0;
	m_iGain = 0;
	//}}AFX_DATA_INIT

	m_iMinExp = 0;
	m_iMaxExp = 0;
	m_iMinGain = 0;
	m_iMaxGain = 0;
	m_iTriggerMode = 0;

	m_pGrabber = NULL;


}


void CDlgCameraSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCameraSetting)
	DDX_Control(pDX, IDC_SLIDER_GAIN, m_SliderGain);
	DDX_Control(pDX, IDC_SLIDER_EXP, m_SliderExp);
	DDX_Control(pDX, IDC_GAIN_MIN, m_MinGain);
	DDX_Control(pDX, IDC_GAIN_MAX, m_MaxGain);
	DDX_Control(pDX, IDC_EXP_MIN, m_MinExp);
	DDX_Control(pDX, IDC_EXP_MAX, m_MaxExp);
	DDX_Control(pDX, IDC_COMBO_TRIG, m_ComboTrig);
	DDX_Text(pDX, IDC_EDIT_EXP, m_iExposureTime);
	DDX_Text(pDX, IDC_EDIT_GAIN, m_iGain);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCameraSetting, CDialog)
	//{{AFX_MSG_MAP(CDlgCameraSetting)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_EXP, OnCustomdrawSliderExp)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_GAIN, OnCustomdrawSliderGain)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCameraSetting message handlers

BOOL CDlgCameraSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	ASSERT(m_pGrabber!=NULL);

	// 设置最大最小值
	CString str;
	str.Format("%d",m_iMaxExp);
	m_MaxExp.SetWindowText(str);

	str.Format("%d",m_iMinExp);
	m_MinExp.SetWindowText(str);

	str.Format("%d",m_iMaxGain);
	m_MaxGain.SetWindowText(str);

	str.Format("%d",m_iMinGain);
	m_MinGain.SetWindowText(str);


	m_SliderGain.SetRange(m_iMinGain,m_iMaxGain);
	m_SliderExp.SetRange(m_iMinExp,m_iMaxExp);

	m_SliderGain.SetPos(m_iGain);
	m_SliderExp.SetPos(m_iExposureTime);


	// 触发方式
	m_ComboTrig.AddString("外触发");
	m_ComboTrig.AddString("内触发");

	int8_t valuestr[J_CAMERA_INFO_SIZE];
	uint32_t iSize;

	if(J_ST_SUCCESS != J_Camera_GetValueString(m_pGrabber->m_hCam, "TriggerMode", valuestr,&iSize))
	{
		AfxMessageBox("Get trigger mode failed!");
	}
	else
	{
		CString str;
		str.Format("%s",valuestr);


		if(0==str.Compare("On"))
		{
			m_ComboTrig.SetCurSel(0);
		}
		else
		{
			m_ComboTrig.SetCurSel(1);
		}
	}
	
	

	UpdateData(FALSE);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCameraSetting::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	
	// 设置增益
	m_pGrabber->SetParamInt((GBParamID)9,m_iGain);

	// 设置曝光
	m_pGrabber->SetParamInt((GBParamID)8,m_iExposureTime);

	// 设置触发方式
	m_iTriggerMode = m_ComboTrig.GetCurSel();

	J_STATUS_TYPE status;
	NODE_HANDLE hNode = NULL;
	status = J_Camera_GetNodeByName(m_pGrabber->m_hCam, "TriggerSelector", &hNode);

	if(status==J_ST_SUCCESS && hNode!=NULL)
	{
		if(0==m_iTriggerMode)
		{
			// Here we assume that this is GenICam SFNC trigger so we do the following:
			// TriggerSelector=FrameStart
			// TriggerMode=On
			// TriggerSource=Line5 - Optical In 1
			
			if(J_ST_SUCCESS != J_Camera_SetValueString(m_pGrabber->m_hCam, "TriggerSelector", "FrameStart"))
			{
				AfxMessageBox(_T("Failed to set the value to TriggerSelector!"), MB_OK | MB_ICONINFORMATION);
			}
			
			if(J_ST_SUCCESS != J_Camera_SetValueString(m_pGrabber->m_hCam, "TriggerMode", "On"))
			{
				AfxMessageBox(_T("Failed to set the value to TriggerMode!"), MB_OK | MB_ICONINFORMATION);
			}
			
			if(J_ST_SUCCESS != J_Camera_SetValueString(m_pGrabber->m_hCam, "TriggerSource", "Line5 - Optical In 1"))
			{
				AfxMessageBox(_T("Failed to set the value to TriggerSource!"), MB_OK | MB_ICONINFORMATION);
			}
		}
		else
		{
			// Here we assume that this is GenICam SFNC trigger so we do the following:
			// TriggerSelector=FrameStart
			// TriggerMode=Off
			if(J_ST_SUCCESS != J_Camera_SetValueString(m_pGrabber->m_hCam, "TriggerSelector", "FrameStart"))
			{
				AfxMessageBox(_T("Failed to set the value to ExposureMode!"), MB_OK | MB_ICONINFORMATION);
			}
			
			if(J_ST_SUCCESS != J_Camera_SetValueString(m_pGrabber->m_hCam, "TriggerMode", "Off"))
			{
				AfxMessageBox(_T("Failed to set the value to ExposureMode!"), MB_OK | MB_ICONINFORMATION);
            }
		}
	}


	// 保存配置文件
	status=J_Camera_SaveSettings(m_pGrabber->m_hCam,m_sConfigFile);
	if(J_ST_SUCCESS!=status)
	{
		AfxMessageBox("Save setting failed!");
	}

	CDialog::OnOK();
}

void CDlgCameraSetting::SetOwner(CJAICamDH* pGrabber,CString strConfigFile)
{
	m_pGrabber = pGrabber;
	m_sConfigFile = strConfigFile;
}

void CDlgCameraSetting::OnCustomdrawSliderExp(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_iExposureTime = m_SliderExp.GetPos();

	UpdateData(FALSE);
	*pResult = 0;
}

void CDlgCameraSetting::OnCustomdrawSliderGain(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_iGain = m_SliderGain.GetPos();
	
	UpdateData(FALSE);

	*pResult = 0;
}
