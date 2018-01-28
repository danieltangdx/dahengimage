// CameraConfigDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GrabRegularDalsa.h"
#include "CameraConfigDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCameraConfigDlg dialog


CCameraConfigDlg::CCameraConfigDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCameraConfigDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCameraConfigDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iAcqIndex = 0;
	m_pAcq = NULL;
	m_pDalsaCamDH = NULL;

}


void CCameraConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCameraConfigDlg)
	DDX_Control(pDX, IDC_COMBO_FRAME_LEVEL, m_ComboFrameLevel);
	DDX_Control(pDX, IDC_COMBO_FRAME_DETECTION, m_ComboFrameDetection);
	DDX_Control(pDX, IDC_COMBO_TRIGGER_MODE, m_ComboTriggerMode);
	DDX_Control(pDX, IDC_CHECK_FARAME_ENABLE, m_CheckFrameEnable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCameraConfigDlg, CDialog)
	//{{AFX_MSG_MAP(CCameraConfigDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_MODE, OnSelchangeComboTriggerMode)
	ON_CBN_SELCHANGE(IDC_COMBO_FRAME_LEVEL, OnSelchangeComboFrameLevel)
	ON_CBN_SELCHANGE(IDC_COMBO_FRAME_DETECTION, OnSelchangeComboFrameDetection)
	ON_BN_CLICKED(IDC_CHECK_FARAME_ENABLE, OnCheckFarameEnable)
	ON_BN_CLICKED(IDC_START_GRAB, OnStartGrab)
	ON_BN_CLICKED(IDC_STOP_GRAB, OnStopGrab)
	ON_EN_KILLFOCUS(IDC_EDIT_INTERNAL_FREQ, OnKillfocusEditInternalFreq)
	ON_EN_KILLFOCUS(IDC_EDIT_CROP_HEIGHT, OnKillfocusEditCropHeight)
	ON_BN_CLICKED(IDC_BUTTON_SAVETOCCF, OnButtonSavetoccf)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCameraConfigDlg message handlers

//DEL void CCameraConfigDlg::OnAreaConfig() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL //	CAScanDlg dlg(this,m_Acq);
//DEL //	dlg.DoModal();
//DEL }

//DEL void CCameraConfigDlg::OnLineConfig() 
//DEL {
//DEL 	// TODO: Add your control notification handler code here
//DEL 	CLScanDlg dlg(this,m_Acq);
//DEL 	dlg.DoModal();
//DEL }

void CCameraConfigDlg::GetAcquisition(CDALSACamDH* pDalsaCamDh,int iAcqIndex)
{
	m_pAcq = pDalsaCamDh->m_Acq;
	m_iAcqIndex = iAcqIndex;
	m_pDalsaCamDH = pDalsaCamDh;
}

BOOL CCameraConfigDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_ComboTriggerMode.AddString("自由运行模式");
	m_ComboTriggerMode.AddString("外部触发模式");
	m_ComboTriggerMode.AddString("内部触发模式");

	m_ComboFrameDetection.AddString("低电平");
	m_ComboFrameDetection.AddString("高电平");
	m_ComboFrameDetection.AddString("上升沿");
	m_ComboFrameDetection.AddString("下降沿");
	m_ComboFrameDetection.AddString("双沿上升沿");
	m_ComboFrameDetection.AddString("双沿下降沿");
	
    m_ComboFrameLevel.AddString("TTL电平");
	m_ComboFrameLevel.AddString("RS422电平");
	m_ComboFrameLevel.AddString("LVDS电平");
    
	int nHeight;
	CString strHeight;
	m_pDalsaCamDH->GetParamInt(GBImageHeight,nHeight);
	strHeight.Format("%d",nHeight);
	TRACE("nHeight = %d",nHeight);
	GetDlgItem(IDC_EDIT_CROP_HEIGHT)->SetWindowText(strHeight);

	if(m_pDalsaCamDH->m_bGrabbing == TRUE)
	{
		if(!GetDalsaCardState())
		{
			s_GBERRORINFO ErrorInfo;
			CString str;		
			m_pDalsaCamDH->GetLastErrorInfo(&ErrorInfo);
			str.Format("错误代码:%d,描述信息:%s,附加信息:%s",ErrorInfo.nErrorCode,ErrorInfo.strErrorDescription,
				ErrorInfo.strErrorRemark);
			AfxMessageBox(str);
			return FALSE;
		}
		m_CheckFrameEnable.EnableWindow(FALSE);
		m_ComboFrameDetection.EnableWindow(FALSE);
        m_ComboFrameLevel.EnableWindow(FALSE);
		m_ComboTriggerMode.EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_INTERNAL_FREQ)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CROP_HEIGHT)->EnableWindow(FALSE);
		SetWindowText("采集卡配置工具 采集状态:正在采集");

		GetDlgItem(IDC_START_GRAB)->EnableWindow(FALSE);
		GetDlgItem(IDC_STOP_GRAB)->EnableWindow(TRUE);
	}
	else
	{
		m_CheckFrameEnable.EnableWindow();
		m_ComboFrameDetection.EnableWindow();
        m_ComboFrameLevel.EnableWindow();
		m_ComboTriggerMode.EnableWindow();
//		GetDlgItem(IDC_EDIT_INTERNAL_FREQ)->EnableWindow();
		GetDlgItem(IDC_EDIT_CROP_HEIGHT)->EnableWindow(TRUE);
		
		GetDlgItem(IDC_START_GRAB)->EnableWindow(TRUE);
		GetDlgItem(IDC_STOP_GRAB)->EnableWindow(FALSE);

		SetWindowText("采集卡配置工具 采集状态：停止采集");
		if(!GetDalsaCardState())
		{
			s_GBERRORINFO ErrorInfo;
			CString str;		
			m_pDalsaCamDH->GetLastErrorInfo(&ErrorInfo);
			str.Format("错误代码:%d,描述信息:%s,附加信息:%s",ErrorInfo.nErrorCode,ErrorInfo.strErrorDescription,
				ErrorInfo.strErrorRemark);
			AfxMessageBox(str);
			return FALSE;
		}
	}

	return TRUE;  
}

void CCameraConfigDlg::OnSelchangeComboTriggerMode() 
{
    int iCurSelectIndex = 0;
	int iInternalTriggerFre = 0;
	iCurSelectIndex = m_ComboTriggerMode.GetCurSel();

// 	if (m_pDalsaCamDH->m_Xfer && *(m_pDalsaCamDH->m_Xfer)) 
// 		m_pDalsaCamDH->m_Xfer->Destroy();
// 	
// 	m_pDalsaCamDH->m_Buffers->Destroy();

	switch(iCurSelectIndex)
	{
	case 0:
		GetDlgItem(IDC_EDIT_INTERNAL_FREQ)->EnableWindow(FALSE);
		if(!m_pAcq->SetParameter(CORACQ_PRM_INT_LINE_TRIGGER_ENABLE,0,TRUE))
		{
			AfxMessageBox("设置内触发使能Internal Trigger无效失败!");
			return;
		}
		if(!m_pAcq->SetParameter(CORACQ_PRM_SHAFT_ENCODER_ENABLE,0,TRUE))
		{
			AfxMessageBox("设置编码器外触发Shaft Encoder Trigger使能无效失败!");
			return;
		}
		if(!m_pAcq->SetParameter(CORACQ_PRM_EXT_LINE_TRIGGER_ENABLE,0,TRUE))
		{
			AfxMessageBox("设置外部触发使能External Line Trigger无效失败!");
			return;
		}
		
		break;
	case 1:
		GetDlgItem(IDC_EDIT_INTERNAL_FREQ)->EnableWindow(FALSE);
		if(!m_pAcq->SetParameter(CORACQ_PRM_INT_LINE_TRIGGER_ENABLE,0,TRUE))
		{
			AfxMessageBox("设置内触发使能Internal Trigger无效失败!");
			return;
		}
		if(!m_pAcq->SetParameter(CORACQ_PRM_SHAFT_ENCODER_ENABLE,0,TRUE))
		{
			AfxMessageBox("设置编码器外触发Shaft Encoder Trigger使能无效失败!");
			return;
		}
		if(!m_pAcq->SetParameter(CORACQ_PRM_EXT_LINE_TRIGGER_ENABLE,1,TRUE))
		{
			AfxMessageBox("设置外部触发使能External Line Trigger有效失败!");
			return;
		}
		break;
	case 2:
		{
			if(!m_pAcq->SetParameter(CORACQ_PRM_EXT_LINE_TRIGGER_ENABLE,0,TRUE))
			{
				AfxMessageBox("设置外部触发使能External Line Trigger无效失败!");
				return;
 			}
			
			if(!m_pAcq->SetParameter(CORACQ_PRM_SHAFT_ENCODER_ENABLE,0,TRUE))
			{
				AfxMessageBox("设置编码器外触发Shaft Encoder Trigger使能无效失败!");
				return;
 			}

			if(!m_pAcq->SetParameter(CORACQ_PRM_INT_LINE_TRIGGER_ENABLE,1,TRUE))
			{
				AfxMessageBox("设置内触发使能Internal Trigger有效失败!");
				return;
			}			
			
			CString str;
			GetDlgItem(IDC_EDIT_INTERNAL_FREQ)->EnableWindow();
			if(!m_pAcq->GetParameter(CORACQ_PRM_INT_LINE_TRIGGER_FREQ,&iInternalTriggerFre))
			{
				AfxMessageBox("获取内触发频率失败!");
				return;
			}
			str.Format("%d",iInternalTriggerFre);
			GetDlgItem(IDC_EDIT_INTERNAL_FREQ)->SetWindowText(str);
			
		}		
		break;
	}	
// 	m_pDalsaCamDH->m_Buffers->Create();
// 	
// 	if (m_pDalsaCamDH->m_Xfer && !*(m_pDalsaCamDH->m_Xfer))
// 	{
// 		if( !m_pDalsaCamDH->m_Xfer->Create())
// 		{	
// 			m_pDalsaCamDH->DestroyObjects();
// 		}
// 	}
}

void CCameraConfigDlg::OnSelchangeComboFrameLevel() 
{
	int iCurrentPos = 0;
	iCurrentPos = m_ComboFrameLevel.GetCurSel();
	switch(iCurrentPos)
	{
	case 0:
		if(!m_pAcq->SetParameter(CORACQ_PRM_EXT_FRAME_TRIGGER_LEVEL,CORACQ_VAL_LEVEL_TTL,TRUE))
		{
			AfxMessageBox("设置帧触发电平信号为TTL失败!");
			return;
		}
		break;
	case 1:
	    if(!m_pAcq->SetParameter(CORACQ_PRM_EXT_FRAME_TRIGGER_LEVEL,CORACQ_VAL_LEVEL_422,TRUE))
		{
			AfxMessageBox("设置帧触发电平信号为RS422失败!");
			return;
		}
		break;
	case 2:
		if(!m_pAcq->SetParameter(CORACQ_PRM_EXT_FRAME_TRIGGER_LEVEL,CORACQ_VAL_LEVEL_LVDS,TRUE))
		{
			AfxMessageBox("设置帧触发电平信号为LVDS失败!");
			return;
		}
		break;
	}
}

void CCameraConfigDlg::OnSelchangeComboFrameDetection() 
{
	int iCurrentPos = 0;
	iCurrentPos = m_ComboFrameDetection.GetCurSel();
	switch(iCurrentPos)
	{
	case 0:
		if(!m_pAcq->SetParameter(CORACQ_PRM_EXT_FRAME_TRIGGER_DETECTION,CORACQ_VAL_ACTIVE_LOW,TRUE))
		{
			m_pDalsaCamDH->m_LastErrorInfo.nErrorCode = DSErrorSetParam;
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorDescription, "设置帧触发电平信号极性为低失败!");
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorRemark, "回调函数CCameraConfigDlg::OnSelchangeComboFrameDetection()");
			AfxMessageBox("设置帧触发电平信号极性为低失败!");
			return;
		}
		break;
	case 1:
		if(!m_pAcq->SetParameter(CORACQ_PRM_EXT_FRAME_TRIGGER_DETECTION,CORACQ_VAL_ACTIVE_HIGH,TRUE))
		{
			m_pDalsaCamDH->m_LastErrorInfo.nErrorCode = DSErrorSetParam;
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorDescription, "设置帧触发电平信号极性为高失败!");
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorRemark, "回调函数CCameraConfigDlg::OnSelchangeComboFrameDetection()");			
			AfxMessageBox("设置帧触发电平信号极性为高失败!");
			return;
		}
		break;
	case 2:
		if(!m_pAcq->SetParameter(CORACQ_PRM_EXT_FRAME_TRIGGER_DETECTION,CORACQ_VAL_RISING_EDGE,TRUE))
		{
			m_pDalsaCamDH->m_LastErrorInfo.nErrorCode = DSErrorSetParam;
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorDescription, "设置帧触发电平信号极性为上升沿失败!");
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorRemark, "回调函数CCameraConfigDlg::OnSelchangeComboFrameDetection()");			
			AfxMessageBox("设置帧触发电平信号极性为上升沿失败!");
			return;
		}
		break;
	case 3:
		if(!m_pAcq->SetParameter(CORACQ_PRM_EXT_FRAME_TRIGGER_DETECTION,CORACQ_VAL_FALLING_EDGE,TRUE))
		{
			m_pDalsaCamDH->m_LastErrorInfo.nErrorCode = DSErrorSetParam;
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorDescription, "设置帧触发电平信号极性为下升沿失败!");
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorRemark, "回调函数CCameraConfigDlg::OnSelchangeComboFrameDetection()");			
			AfxMessageBox("设置帧触发电平信号极性为下升沿失败!");
			return;
		}
		break;
	case 4:
        if(!m_pAcq->SetParameter(CORACQ_PRM_EXT_FRAME_TRIGGER_DETECTION,CORACQ_VAL_DOUBLE_PULSE_RISING_EDGE,TRUE))
		{
			m_pDalsaCamDH->m_LastErrorInfo.nErrorCode = DSErrorSetParam;
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorDescription, "设置帧触发电平信号极性为双上升沿失败!");
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorRemark, "回调函数CCameraConfigDlg::OnSelchangeComboFrameDetection()");			
			AfxMessageBox("设置帧触发电平信号极性为双上升沿失败!");
			return;
		}
		break;
	case 5:
		if(!m_pAcq->SetParameter(CORACQ_PRM_EXT_FRAME_TRIGGER_DETECTION,CORACQ_VAL_DOUBLE_PULSE_FALLING_EDGE,TRUE))
		{
			m_pDalsaCamDH->m_LastErrorInfo.nErrorCode = DSErrorSetParam;
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorDescription, "设置帧触发电平信号极性为双下升沿失败!");
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorRemark, "回调函数CCameraConfigDlg::OnSelchangeComboFrameDetection()");			
			AfxMessageBox("设置帧触发电平信号极性为双下升沿失败!");
			return;
		}
		break;
	}
}

void CCameraConfigDlg::OnCheckFarameEnable() 
{
	int iReturnValue = 0;
	if(m_CheckFrameEnable.GetCheck() == 1)
	{
		m_pAcq->SetParameter(CORACQ_PRM_EXT_FRAME_TRIGGER_ENABLE,1,TRUE);
		m_ComboFrameDetection.EnableWindow();
		m_ComboFrameLevel.EnableWindow();
		m_pAcq->GetParameter(CORACQ_PRM_EXT_FRAME_TRIGGER_LEVEL,&iReturnValue);
		switch(iReturnValue)
		{
		case CORACQ_VAL_LEVEL_TTL:
			m_ComboFrameLevel.SetCurSel(0);
			break;
		case CORACQ_VAL_LEVEL_422:
			m_ComboFrameLevel.SetCurSel(1);
			break;
		case CORACQ_VAL_LEVEL_LVDS:
			m_ComboFrameLevel.SetCurSel(2);
			break;
		}
		m_pAcq->GetParameter(CORACQ_PRM_EXT_FRAME_TRIGGER_DETECTION,&iReturnValue);
		switch(iReturnValue)
		{
		case CORACQ_VAL_ACTIVE_LOW:
			m_ComboFrameDetection.SetCurSel(0);
			break;
		case CORACQ_VAL_ACTIVE_HIGH:
			m_ComboFrameDetection.SetCurSel(1);
			break;
		case CORACQ_VAL_RISING_EDGE:
			m_ComboFrameDetection.SetCurSel(2);
			break;
		case CORACQ_VAL_FALLING_EDGE:
			m_ComboFrameDetection.SetCurSel(3);
			break;
		case CORACQ_VAL_DOUBLE_PULSE_RISING_EDGE:
			m_ComboFrameDetection.SetCurSel(4);
			break;
		case CORACQ_VAL_DOUBLE_PULSE_FALLING_EDGE:
			m_ComboFrameDetection.SetCurSel(5);
			break;
		}
	}
	else
	{
		m_pAcq->SetParameter(CORACQ_PRM_EXT_FRAME_TRIGGER_ENABLE,0,TRUE);
		m_ComboFrameDetection.EnableWindow(FALSE);
		m_ComboFrameLevel.EnableWindow(FALSE);
	}
}

void CCameraConfigDlg::OnStartGrab() 
{
	if(!m_pDalsaCamDH->StartGrab())
	{
        s_GBERRORINFO ErrorInfo;
		CString str;		
		m_pDalsaCamDH->GetLastErrorInfo(&ErrorInfo);
		str.Format("错误代码:%d,描述信息:%s,附加信息:%s",ErrorInfo.nErrorCode,ErrorInfo.strErrorDescription,
			ErrorInfo.strErrorRemark);
		AfxMessageBox(str);
		return;
	}
    SetWindowText("采集卡配置工具 采集状态:正在采集");
	m_CheckFrameEnable.EnableWindow(FALSE);
	m_ComboFrameDetection.EnableWindow(FALSE);
	m_ComboFrameLevel.EnableWindow(FALSE);
	m_ComboTriggerMode.EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_INTERNAL_FREQ)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_CROP_HEIGHT)->EnableWindow(FALSE);
	GetDlgItem(IDC_START_GRAB)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP_GRAB)->EnableWindow(TRUE);
	
}

void CCameraConfigDlg::OnStopGrab() 
{

	if(!m_pDalsaCamDH->StopGrab())
	{
        s_GBERRORINFO ErrorInfo;
		CString str;		
		m_pDalsaCamDH->GetLastErrorInfo(&ErrorInfo);
		str.Format("错误代码:%d,描述信息:%s,附加信息:%s",ErrorInfo.nErrorCode,ErrorInfo.strErrorDescription,
			ErrorInfo.strErrorRemark);
		AfxMessageBox(str);
		return;
	}

	GetDalsaCardState();
	
    SetWindowText("采集卡配置工具 采集状态：停止采集");

	m_CheckFrameEnable.EnableWindow();
	m_ComboFrameDetection.EnableWindow();
	m_ComboFrameLevel.EnableWindow();
	m_ComboTriggerMode.EnableWindow();
	GetDlgItem(IDC_EDIT_CROP_HEIGHT)->EnableWindow(TRUE);



	GetDlgItem(IDC_START_GRAB)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP_GRAB)->EnableWindow(FALSE);
}

BOOL CCameraConfigDlg::GetDalsaCardState()//获取当前的板卡参数状态
{
	int extFrameTriggerCap = 0;
	int extFrameTriggerEnable = 0;
	int extFrameTriggerDetectionCap = 0;
	int extFrameTriggerDetectionPrm = 0;
	int extFrameTriggerLevelCap = 0;
	int extFrameTriggerLevelPrm = 0;
	if (m_pAcq == NULL)
	{
		AfxMessageBox("无效的acquisition参数!");
		CDialog::OnCancel();
		return FALSE;
	}
	int iReturnValue = 0;
	if (!m_pAcq->GetCapability(CORACQ_CAP_EXT_FRAME_TRIGGER, &extFrameTriggerCap))
	{
		m_pDalsaCamDH->m_LastErrorInfo.nErrorCode = DSErrorGetParam;
		sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorDescription, "获取参数CORACQ_CAP_EXT_FRAME_TRIGGER失败");
		sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorRemark, "错误位置CCameraConfigDlg::GetDalsaCardState()函数");
		return FALSE;
	}


	if(extFrameTriggerCap == 1)
	{
		
		if(!m_pAcq->GetParameter(CORACQ_PRM_EXT_FRAME_TRIGGER_ENABLE,&extFrameTriggerEnable))
		{
			m_pDalsaCamDH->m_LastErrorInfo.nErrorCode = DSErrorGetParam;
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorDescription, "获取参数CORACQ_PRM_EXT_FRAME_TRIGGER_ENABLE失败");
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorRemark, "错误位置CCameraConfigDlg::GetDalsaCardState()函数");			
			return FALSE;
		}
		if (!m_pAcq->GetCapability(CORACQ_CAP_EXT_FRAME_TRIGGER_DETECTION, &extFrameTriggerDetectionCap))
		{
			m_pDalsaCamDH->m_LastErrorInfo.nErrorCode = DSErrorGetParam;
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorDescription, "获取参数CORACQ_CAP_EXT_FRAME_TRIGGER_DETECTION失败");
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorRemark, "错误位置CCameraConfigDlg::GetDalsaCardState()函数");			
			return FALSE;
		}		
		if (!m_pAcq->GetParameter(CORACQ_PRM_EXT_FRAME_TRIGGER_DETECTION, &extFrameTriggerDetectionPrm))
		{
			m_pDalsaCamDH->m_LastErrorInfo.nErrorCode = DSErrorGetParam;
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorDescription, "获取参数CORACQ_PRM_EXT_FRAME_TRIGGER_DETECTION失败");
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorRemark, "错误位置CCameraConfigDlg::GetDalsaCardState()函数");			
			return FALSE;
		}		
		if (!m_pAcq->GetCapability(CORACQ_CAP_EXT_FRAME_TRIGGER_LEVEL, &extFrameTriggerLevelCap))
		{
			m_pDalsaCamDH->m_LastErrorInfo.nErrorCode = DSErrorGetParam;
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorDescription, "获取参数CORACQ_CAP_EXT_FRAME_TRIGGER_LEVEL失败");
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorRemark, "错误位置CCameraConfigDlg::GetDalsaCardState()函数");			
			return FALSE;
		}		
		if (!m_pAcq->GetParameter(CORACQ_PRM_EXT_FRAME_TRIGGER_LEVEL, &extFrameTriggerLevelPrm))
		{
			m_pDalsaCamDH->m_LastErrorInfo.nErrorCode = DSErrorGetParam;
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorDescription, "获取参数CORACQ_PRM_EXT_FRAME_TRIGGER_LEVEL失败");
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorRemark, "错误位置CCameraConfigDlg::GetDalsaCardState()函数");			
			return FALSE;
		}		
	}
	
	if(extFrameTriggerEnable == 1)
	{
		m_CheckFrameEnable.SetCheck(1);
		m_ComboFrameDetection.EnableWindow();
		m_ComboFrameLevel.EnableWindow();
		if(extFrameTriggerLevelCap)
		{
			switch(extFrameTriggerLevelPrm)
			{
			case CORACQ_VAL_LEVEL_TTL:
				m_ComboFrameLevel.SetCurSel(0);
				break;
			case CORACQ_VAL_LEVEL_422:
				m_ComboFrameLevel.SetCurSel(1);
				break;
			case CORACQ_VAL_LEVEL_LVDS:
				m_ComboFrameLevel.SetCurSel(2);
				break;
			}			
		}
		if(extFrameTriggerDetectionCap)
		{
			switch(extFrameTriggerDetectionPrm)
			{
			case CORACQ_VAL_ACTIVE_LOW:
				m_ComboFrameDetection.SetCurSel(0);
				break;
			case CORACQ_VAL_ACTIVE_HIGH:
				m_ComboFrameDetection.SetCurSel(1);
				break;
			case CORACQ_VAL_RISING_EDGE:
				m_ComboFrameDetection.SetCurSel(2);
				break;
			case CORACQ_VAL_FALLING_EDGE:
				m_ComboFrameDetection.SetCurSel(3);
				break;
			case CORACQ_VAL_DOUBLE_PULSE_RISING_EDGE:
				m_ComboFrameDetection.SetCurSel(4);
				break;
			case CORACQ_VAL_DOUBLE_PULSE_FALLING_EDGE:
				m_ComboFrameDetection.SetCurSel(5);
				break;
			}
		}		
	}
	else
	{
		m_CheckFrameEnable.SetCheck(0);
		m_ComboFrameDetection.EnableWindow(FALSE);
		m_ComboFrameLevel.EnableWindow(FALSE);
	}    
	int iInternalLineTriggerPram = 0;
	int iInternalLineTriggerCap = 0;
	int iInternalTriggerFre= 0;
	CString str;
    if(!m_pAcq->GetCapability(CORACQ_CAP_INT_LINE_TRIGGER,&iInternalLineTriggerCap))//是否是内部触发
	{
		m_pDalsaCamDH->m_LastErrorInfo.nErrorCode = DSErrorGetParam;
		sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorDescription, "获取参数CORACQ_CAP_INT_LINE_TRIGGER失败");
		sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorRemark, "错误位置CCameraConfigDlg::GetDalsaCardState()函数");			
		return FALSE;
	}
	if(iInternalLineTriggerCap == 1)
	{
		if(!m_pAcq->GetParameter(CORACQ_PRM_INT_LINE_TRIGGER_ENABLE,&iInternalLineTriggerPram))
		{
			m_pDalsaCamDH->m_LastErrorInfo.nErrorCode = DSErrorGetParam;
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorDescription, "获取参数CORACQ_PRM_INT_LINE_TRIGGER_ENABLE失败");
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorRemark, "错误位置CCameraConfigDlg::GetDalsaCardState()函数");			
			return FALSE;
		}
		if(iInternalLineTriggerPram == 1)
		{
			GetDlgItem(IDC_EDIT_INTERNAL_FREQ)->EnableWindow();
			m_ComboTriggerMode.SetCurSel(2);
			if(!m_pAcq->GetParameter(CORACQ_PRM_INT_LINE_TRIGGER_FREQ,&iInternalTriggerFre))
			{
				m_pDalsaCamDH->m_LastErrorInfo.nErrorCode = DSErrorGetParam;
				sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorDescription, "获取参数CORACQ_PRM_INT_LINE_TRIGGER_FREQ失败");
				sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorRemark, "错误位置CCameraConfigDlg::GetDalsaCardState()函数");			
				return FALSE;				
			}
			str.Format("%d",iInternalTriggerFre);
			GetDlgItem(IDC_EDIT_INTERNAL_FREQ)->SetWindowText(str);
		}
		else
		{
			GetDlgItem(IDC_EDIT_INTERNAL_FREQ)->EnableWindow(FALSE);
		}
	}
	
	int iExternalLineTriggerPram = 0;
	int iExternalLineTriggerCap = 0;
    if(!m_pAcq->GetCapability(CORACQ_CAP_EXT_LINE_TRIGGER ,&iExternalLineTriggerCap))//是否是外部行触发
	{
		m_pDalsaCamDH->m_LastErrorInfo.nErrorCode = DSErrorGetParam;
		sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorDescription, "获取参数CORACQ_CAP_EXT_LINE_TRIGGER失败");
		sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorRemark, "错误位置CCameraConfigDlg::GetDalsaCardState()函数");			
		return FALSE;		
	}
	if(iExternalLineTriggerCap == 1)
	{
		if(!m_pAcq->GetParameter(CORACQ_PRM_EXT_LINE_TRIGGER_ENABLE,&iExternalLineTriggerPram))
		{
			m_pDalsaCamDH->m_LastErrorInfo.nErrorCode = DSErrorGetParam;
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorDescription, "获取参数CORACQ_PRM_EXT_LINE_TRIGGER_ENABLE失败");
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorRemark, "错误位置CCameraConfigDlg::GetDalsaCardState()函数");			
			return FALSE;
		}
		if(iExternalLineTriggerPram == 1)
		{
			m_ComboTriggerMode.SetCurSel(1);
		}
	}
	int iShaftEncoderTriggerPram = 0;
	int iShaftEncoderTriggerCap = 0;	
	if(!m_pAcq->GetCapability(CORACQ_CAP_SHAFT_ENCODER,&iShaftEncoderTriggerCap))//是否是外部行触发
	{
		m_pDalsaCamDH->m_LastErrorInfo.nErrorCode = DSErrorGetParam;
		sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorDescription, "获取参数CORACQ_CAP_SHAFT_ENCODER失败");
		sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorRemark, "错误位置CCameraConfigDlg::GetDalsaCardState()函数");			
		return FALSE;
	}
	if(iExternalLineTriggerCap == 1)
	{
		if(!m_pAcq->GetParameter(CORACQ_PRM_SHAFT_ENCODER_ENABLE,&iShaftEncoderTriggerPram))
		{
			m_pDalsaCamDH->m_LastErrorInfo.nErrorCode = DSErrorGetParam;
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorDescription, "获取参数CORACQ_PRM_SHAFT_ENCODER_ENABLE失败");
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorRemark, "错误位置CCameraConfigDlg::GetDalsaCardState()函数");			
			return FALSE;
		}
	}
    if(iExternalLineTriggerPram == 0 && iInternalLineTriggerPram == 0 &&iShaftEncoderTriggerPram == 0)//自由运行模式
	{
		m_ComboTriggerMode.SetCurSel(0);
	}
/*	int iExternalTriggerPram = 0;
	int iExternalTriggerCap = 0;
	m_pAcq->GetCapability(CORACQ_CAP_EXT_TRIGGER ,&iExternalLineTriggerCap);//是否是外部触发模式
	if(iExternalLineTriggerCap == 1)
	{
		m_pAcq->GetParameter(CORACQ_PRM_EXT_TRIGGER_ENABLE,&iExternalLineTriggerPram);
		if(iExternalLineTriggerPram == 1)
		{
			m_ComboTriggerMode.SetCurSel(1);
		}
	}*/
	return TRUE;
}

void CCameraConfigDlg::OnKillfocusEditInternalFreq() 
{
	int iInternalTriggerMax = 0;
	int iInternalTriggerMin = 0;
	CString strFrcen,strInfo;
	if(!m_pAcq->GetParameter(CORACQ_PRM_INT_LINE_TRIGGER_FREQ_MIN,&iInternalTriggerMin))
	{
		m_pDalsaCamDH->m_LastErrorInfo.nErrorCode = DSErrorGetParam;
		sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorDescription, "获取参数CORACQ_PRM_INT_LINE_TRIGGER_FREQ_MIN失败");
		sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorRemark, "错误位置CCameraConfigDlg::OnKillfocusEditInternalFreq()函数");
		AfxMessageBox("获取内触发最小频率失败!");
		return;
	}
	if(!m_pAcq->GetParameter(CORACQ_PRM_INT_LINE_TRIGGER_FREQ_MAX,&iInternalTriggerMax))
	{
		m_pDalsaCamDH->m_LastErrorInfo.nErrorCode = DSErrorGetParam;
		sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorDescription, "获取参数CORACQ_PRM_INT_LINE_TRIGGER_FREQ_MAX失败");
		sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorRemark, "错误位置CCameraConfigDlg::OnKillfocusEditInternalFreq()函数");
		AfxMessageBox("获取内触发最大频率失败!");
		return;
	}
	GetDlgItem(IDC_EDIT_INTERNAL_FREQ)->GetWindowText(strFrcen);
	if(atoi(strFrcen) < iInternalTriggerMin || atoi(strFrcen) > iInternalTriggerMax)
	{
		strInfo.Format("输入的内触发频率超过了允许范围%d-%d",iInternalTriggerMin,iInternalTriggerMax);
		AfxMessageBox(strInfo);
	}
	else
	{
		if(!m_pAcq->SetParameter(CORACQ_PRM_INT_LINE_TRIGGER_FREQ,atoi(strFrcen),TRUE))
		{
			m_pDalsaCamDH->m_LastErrorInfo.nErrorCode = DSErrorSetParam;
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorDescription, "获取参数CORACQ_PRM_INT_LINE_TRIGGER_FREQ失败");
			sprintf(m_pDalsaCamDH->m_LastErrorInfo.strErrorRemark, "错误位置CCameraConfigDlg::OnKillfocusEditInternalFreq()函数");
			AfxMessageBox("获取内触发最大频率失败!");
			return;			
		}
	}
}

void CCameraConfigDlg::OnKillfocusEditCropHeight() 
{
	// TODO: Add your control notification handler code here
	int nHeight;
	CString strHeight;
	GetDlgItem(IDC_EDIT_CROP_HEIGHT)->GetWindowText(strHeight);
	nHeight = atoi(strHeight);

	if(m_pDalsaCamDH->m_nMaxImageHeight < nHeight )
	{
		nHeight = m_pDalsaCamDH->m_nMaxImageHeight;
	}
	else if (nHeight <= 20)
	{
		nHeight = 20;
	}
	strHeight.Format("%d",nHeight);
	
	GetDlgItem(IDC_EDIT_CROP_HEIGHT)->SetWindowText(strHeight);

	if (!m_pDalsaCamDH->DSSetParamPro(DSLineCropHeight,nHeight))
	{
		if (m_pDalsaCamDH->m_bGrabbing)
		{
			AfxMessageBox("正在采集不能设置高度");
		}
		AfxMessageBox("设置高度失败");
	}
	else
	{
		m_pAcq = m_pDalsaCamDH->m_Acq;
	}
}

void CCameraConfigDlg::OnButtonSavetoccf() 
{
	if (!m_pDalsaCamDH->m_Acq->SaveParameters(m_pDalsaCamDH->m_sInitFile))
	{
		AfxMessageBox("保存初始化文件失败");
	}
}
