// SetCameraParam.cpp : implementation file
//

#include "stdafx.h"
#include "AVTGrabber.h"
#include "SetCameraParam.h"
#include "Login.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetCameraParam dialog


CSetCameraParam::CSetCameraParam(CWnd* pParent /*=NULL*/)
	: CDialog(CSetCameraParam::IDD, pParent)
{

	//{{AFX_DATA_INIT(CSetCameraParam)
	m_nBrightness = 0;
	m_nGain = 0;
	m_nShutter = 0;
	m_nBrightnessSlider = 0;
	m_nGainSlider = 0;
	m_nShutterSlider = 0;
	m_nAOILeft = 0;
	m_nAOITop = 0;
	m_nAOIWidth = 0;
	m_nAOIHeight = 0;
	m_nCB = 0;
	m_nCR = 0;
	//}}AFX_DATA_INIT
	m_strProfileName = _T("");
	m_nOldBrightness = 0;
	m_nOldGain = 0;
	m_nOldShutter = 0;
	m_nTriggerMode = 0;
	m_nOldTriggerMode = 0;

	m_iOldCB = 0;  //修改前的白平衡UB值
	m_iOldCR = 0;  //修改前的白平衡VR值

	m_iMaxWhiteBalCB = 0;     //最大的白平衡CB分量 
	m_iMaxWhiteBalCR = 0;     //最大的白平衡CR分量

	m_iMaxBrightness = 0;     //最大亮度
	m_iMaxGain = 0;           //最大增益
	m_iMaxShutter = 0;        //最大曝光

	m_iMinBrightness = 0;     //最小亮度
	m_iMinGain = 0;           //最小增益
	m_iMinShutter = 0;        //最小曝光

	m_OldFrameCnt = 0;

	m_strLanType = _T("English");
}


void CSetCameraParam::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetCameraParam)
	DDX_Control(pDX, IDC_COMBO_TRIGGER_MODE, m_TriggerMode);
	DDX_Text(pDX, IDC_EDIT_BRIGHTNESS, m_nBrightness);
	DDX_Text(pDX, IDC_EDIT_GAIN, m_nGain);
	DDX_Text(pDX, IDC_EDIT_SHUTTER, m_nShutter);
	DDX_Slider(pDX, IDC_SLIDER_BRIGHTNESS, m_nBrightnessSlider);
	DDX_Slider(pDX, IDC_SLIDER_GAIN, m_nGainSlider);
	DDX_Slider(pDX, IDC_SLIDER_SHUTTER, m_nShutterSlider);
	DDX_Text(pDX, IDC_EDIT_AOI_LEFT, m_nAOILeft);
	DDX_Text(pDX, IDC_EDIT_AOI_TOP, m_nAOITop);
	DDX_Text(pDX, IDC_EDIT_AOI_WIDTH, m_nAOIWidth);
	DDX_Text(pDX, IDC_EDIT_AOI_HEIGHT, m_nAOIHeight);
	DDX_Text(pDX, IDC_EDIT_CB, m_nCB);
	DDX_Text(pDX, IDC_EDIT_CR, m_nCR);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetCameraParam, CDialog)
	//{{AFX_MSG_MAP(CSetCameraParam)
	ON_EN_CHANGE(IDC_EDIT_BRIGHTNESS, OnChangeEditBrightness)
	ON_EN_CHANGE(IDC_EDIT_GAIN, OnChangeEditGain)
	ON_EN_CHANGE(IDC_EDIT_SHUTTER, OnChangeEditShutter)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGER_MODE, OnSelchangeComboTriggerMode)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_APPLY, OnBtnApply)
	ON_BN_CLICKED(IDC_BUTTON_AOI_APPLY, OnButtonAoiApply)
	ON_BN_CLICKED(IDC_BUTTON_FRAMETEST, OnButtonFrametest)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, OnButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_AOI_APPLYWB, OnButtonAoiApplywb)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetCameraParam message handlers
void CSetCameraParam::SetOwner(CAVTCamDH *pGrabber, const CString &strProfileName)
{
	m_pGrabber = pGrabber;
	m_strProfileName = strProfileName;
}

//初始化对话框
BOOL CSetCameraParam::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	SetDiscriptionInfo(m_strLanType);
	ASSERT(m_pGrabber != NULL);

	SetSliderRange();

	UINT32 Result=0;
	UINT32 Brightness=0;
	UINT32 Gain=0;
	UINT32 Shutter=0;
	UINT32 WhiteBalanceUB=0;
	UINT32 WhiteBalanceVR=0;
	UINT32 Trigger=0;

	//////////////////////////////////////////////////////////////////////////
	//获取AOI参数
	if (m_pGrabber->m_CamParms.ImageFormat < 7)//非Format格式
	{
		GetDlgItem(IDC_EDIT_AOI_LEFT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AOI_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AOI_TOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AOI_HEIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_AOI_APPLY)->EnableWindow(FALSE);
	}
	else
	{
		m_nAOILeft = m_pGrabber->m_CamParms.XPos;
		m_nAOIWidth = m_pGrabber->m_CamParms.XSize;
		m_nAOITop = m_pGrabber->m_CamParms.YPos;
		m_nAOIHeight = m_pGrabber->m_CamParms.YSize;
	}

	if (m_pGrabber->m_CameraActive)
	{
		GetDlgItem(IDC_EDIT_AOI_LEFT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AOI_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AOI_TOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AOI_HEIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_AOI_APPLY)->EnableWindow(FALSE);
	}
	//////////////////////////////////////////////////////////////////////////
	//获取当前帧数，用来计算帧率
	m_OldFrameCnt = m_pGrabber->m_FrameCnt;

	//////////////////////////////////////////////////////////////////////////
	//获取相机当前亮度增益曝光值 触发模式
	Result = m_pGrabber->m_Camera.GetParameter(FGP_BRIGHTNESS, &Brightness); //获取相机里面的亮度值
	m_nBrightness = (int)Brightness;
	m_nBrightnessSlider = m_nBrightness;
	m_nOldBrightness = m_nBrightness;
	Result = m_pGrabber->m_Camera.GetParameter(FGP_GAIN, &Gain);  //获取相机里面的增益值
	m_nGain = (int)Gain;
	m_nGainSlider = m_nGain;
	m_nOldGain = m_nGain;
	Result = m_pGrabber->m_Camera.GetParameter(FGP_SHUTTER, &Shutter); //获取相机里面的Shutter值
	m_nShutter = (int)Shutter;
	m_nShutterSlider = m_nShutter;
	m_nOldShutter = m_nShutter;

//	Result = m_pGrabber->m_Camera.GetParameter(FGP_TRIGGER, &Trigger); //获取相机里面的触发模式
	Result = m_pGrabber->m_Camera.ReadRegister(0xF0F00830,&Trigger);
	//m_nTriggerMode = m_pGrabber->m_nTriggerMode;
	switch(Trigger) 
	{
	case 0x80100000:
		m_nTriggerMode = 0;  //AVTTriggerOff 内触发模式  
		break;
	case 0x83100000:
		m_nTriggerMode = 1;  //AVTTriggerOn  //上升沿触发//外触发开启
		break;
	case 0x82100000:
		m_nTriggerMode = 2;  //AVTTriggerMode2  //下降沿触发
		break;
	case 0x83110000:
		m_nTriggerMode = 3;  //AVTTriggerMode3 //高电平触发
		break;
	case 0x82110000:
		m_nTriggerMode = 4;  //AVTTriggerMode4  //低电平触发
		break;
	case 0x831F0000:
		m_nTriggerMode = 5;  //AVTTriggerMode5  //编程模式上升沿触发
		break;
	case 0x821F0000:
		m_nTriggerMode = 6;  //AVTTriggerMode6  //编程模式下降沿触发
		break;
	case 0x82E00000:
		m_nTriggerMode = 7;  //AVTTriggerMode6  //编程模式下降沿触发
		break;
	case 0x82E10000:
		m_nTriggerMode = 8;  //AVTTriggerMode6  //编程模式下降沿触发
		break;
	case 0x82EF0000:
		m_nTriggerMode = 9;  //AVTTriggerMode6  //编程模式下降沿触发
		break;

	default:
		m_nTriggerMode = 0;  //AVTTriggerOff 内触发模式
	}

	m_nOldTriggerMode = m_nTriggerMode;
	m_TriggerMode.SetCurSel(m_nTriggerMode);

	if (m_pGrabber->m_bIsColorCamera == TRUE) //如果是彩色相机则打开白平衡 
	{
		Result = m_pGrabber->m_Camera.GetParameter(FGP_WHITEBALCB, &WhiteBalanceUB); //获取相机里面的白平衡UB分量
		if (Result == FCE_NOERROR)
		{
			m_nCB = (int)WhiteBalanceUB;

			
		}
		else if(Result == FCE_NOTAVAILABLE)
		{
			GetDlgItem(IDC_EDIT_CB)->EnableWindow(FALSE);
		}
		else
		{
			AfxMessageBox(_T("获取白平衡CB参数失败!"));
			m_nCB = 0;
		}
	
//		m_ctrlWhiteBalUBSlider.SetPos(m_iWhiteBalanceUB);

		m_iOldCB = m_nCB;


		
		Result = m_pGrabber->m_Camera.GetParameter(FGP_WHITEBALCR, &WhiteBalanceVR); //获取相机里面的白平衡VR分量
		if (Result == FCE_NOERROR)
		{
			m_nCR = (int)WhiteBalanceVR;
		}
		else if(Result == FCE_NOTAVAILABLE)
		{
			GetDlgItem(IDC_EDIT_CR)->EnableWindow(FALSE);
		}
		else
		{
			AfxMessageBox(_T("获取白平衡CR参数失败!"));
			m_nCR = 0;
		}

		m_iOldCR = m_nCR;

//		m_iWhiteBalanceVR = (int)WhiteBalanceVR;
//		m_ctrlWhiteBalVRSlider.SetPos(m_iWhiteBalanceVR);
//		m_iOldWhiteBalanceVR = m_iWhiteBalanceVR;
	}
	else
	{
 		GetDlgItem(IDC_EDIT_CB)->EnableWindow(FALSE);
 		GetDlgItem(IDC_EDIT_CR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_AOI_APPLYWB)->EnableWindow(FALSE);
	}

	if (Result != 0)
	{
		MessageBox(_T("读取相机内参数失败,请重启程序!"));
		return FALSE;
	}
	
	UpdateData(FALSE);

	EnableShow(FALSE);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 设置语言 [4/8/2014 SJC]
void CSetCameraParam::SetDiscriptionInfo(CString strLanType)
{
	if (strLanType==_T("English"))
	{
		SetWindowText(_T("Camera Param Setting Dialog"));
		SetDlgItemText(IDC_STATIC_CAMERA_PARAM,_T("Camera Param"));
		SetDlgItemText(IDC_STATIC_LIGHT,_T("Light"));
		SetDlgItemText(IDC_STATIC_GAIN,_T("Gain"));
		SetDlgItemText(IDC_STATIC_SHUTTER,_T("Shutter"));
		SetDlgItemText(IDC_STATIC_TRIGGER_MODE,_T("Trigger Mode"));
		SetDlgItemText(IDC_STATIC_AOI_PARAM,_T("AOI Param"));
		SetDlgItemText(IDC_STATIC_LEFT,_T("Left"));
		SetDlgItemText(IDC_STATIC_WIDTH,_T("Width"));
		SetDlgItemText(IDC_STATIC_TOP,_T("Top"));
		SetDlgItemText(IDC_STATIC_HEIGHT,_T("Height"));
		SetDlgItemText(IDC_BUTTON_AOI_APPLY,_T("Apply"));
		SetDlgItemText(IDC_STATIC_AOI_INFO,_T("Stop grab first"));
		SetDlgItemText(IDC_STATIC_WB,_T("White balance(Color Camera)"));
		SetDlgItemText(IDC_BUTTON_AOI_APPLYWB,_T("Apply"));
		SetDlgItemText(IDC_STATIC_FPS_TEST,_T("FPS Test"));
		SetDlgItemText(IDC_BUTTON_FRAMETEST,_T("Test"));
		SetDlgItemText(IDC_BUTTON_LOGIN,_T("Login"));
		SetDlgItemText(IDC_BTN_APPLY,_T("Save Param"));
		SetDlgItemText(IDOK,_T("Exit"));

		m_TriggerMode.ResetContent();
		m_TriggerMode.AddString(_T("Internal Grab"));
		m_TriggerMode.AddString(_T("Rising Edge Trigger"));
		m_TriggerMode.AddString(_T("Falling Edge Trigger"));
		m_TriggerMode.AddString(_T("High Level Trigger"));
		m_TriggerMode.AddString(_T("Low Level Trigger"));
		m_TriggerMode.AddString(_T("Prog. High Level Trigger"));
		m_TriggerMode.AddString(_T("Prog. Low Level Trigger"));
		m_TriggerMode.AddString(_T("Edge Soft Trigger"));
		m_TriggerMode.AddString(_T("Level Soft Trigger"));
		m_TriggerMode.AddString(_T("Prog. Soft Trigger"));
	}
	if (strLanType==_T("Chinese"))
	{
		m_TriggerMode.ResetContent();
		m_TriggerMode.AddString(_T("内触发"));
		m_TriggerMode.AddString(_T("上升沿触发"));
		m_TriggerMode.AddString(_T("下降沿触发"));
		m_TriggerMode.AddString(_T("高电平触发"));
		m_TriggerMode.AddString(_T("低电平触发"));
		m_TriggerMode.AddString(_T("编程上升沿外触发"));
		m_TriggerMode.AddString(_T("编程下降沿外触发"));
		m_TriggerMode.AddString(_T("沿软触发"));
		m_TriggerMode.AddString(_T("电平软触发"));
		m_TriggerMode.AddString(_T("编程软触发"));
	}
}

// 设置SLIDER控件的最大最小值和滑动距离
void CSetCameraParam::SetSliderRange()
{
	CSliderCtrl *pSC = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_BRIGHTNESS);
	pSC->SetRange(m_iMinBrightness, m_iMaxBrightness, TRUE);
	pSC = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_GAIN);
	pSC->SetRange(m_iMinGain, m_iMaxGain, TRUE);
	pSC = (CSliderCtrl *)GetDlgItem(IDC_SLIDER_SHUTTER);
	pSC->SetRange(m_iMinShutter, m_iMaxShutter, TRUE);

	// 加入白平衡的控制
//	m_ctrlWhiteBalUBSlider.SetRange(0,m_iMaxWhiteBalCB);
//	m_ctrlWhiteBalVRSlider.SetRange(0,m_iMaxWhiteBalCR);
}

void CSetCameraParam::OnChangeEditBrightness() //亮度
{
	UpdateData(TRUE);
	if (m_nBrightness > m_iMaxBrightness)
	{
		m_nBrightness = m_iMaxBrightness;
	}

	if (m_nBrightness < m_iMinBrightness)
	{
		m_nBrightness = m_iMinBrightness;
	}

	m_nBrightnessSlider = m_nBrightness;
	UpdateData(FALSE);
	UINT32 Result;
	Result = m_pGrabber->m_Camera.SetParameter(FGP_BRIGHTNESS, m_nBrightness);
}

void CSetCameraParam::OnChangeEditGain() //增益
{
	UpdateData(TRUE);
	if (m_nGain >m_iMaxGain)
	{
		m_nGain = m_iMaxGain;
	}
	if (m_nGain < m_iMinGain)
	{
		m_nGain = m_iMinGain;
	}

	m_nGainSlider = m_nGain;
	UpdateData(FALSE);
	UINT32 Result;
	Result = m_pGrabber->m_Camera.SetParameter(FGP_GAIN, m_nGain);
}

void CSetCameraParam::OnChangeEditShutter()  //曝光
{
	UpdateData(TRUE);
	if (m_nShutter>m_iMaxShutter)
	{
		m_nShutter = m_iMaxShutter;
	}
	if (m_nShutter < m_iMinShutter)
	{
		m_nShutter = m_iMinShutter;
	}
	m_nShutterSlider = m_nShutter;
	UpdateData(FALSE);
	UINT32 Result;
	Result = m_pGrabber->m_Camera.SetParameter(FGP_SHUTTER, m_nShutter);
}


LRESULT CSetCameraParam::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if (message == WM_HSCROLL) 
	{
		int nScrollCode = (int) LOWORD(wParam);
		int nPos = (short int) HIWORD(wParam);   
		HWND hwndScrollBar = (HWND) lParam;
		if (GetDlgItem(IDC_SLIDER_BRIGHTNESS)->GetSafeHwnd() == hwndScrollBar)  //亮度滑动条响应
		{
			UpdateData(TRUE);
			m_nBrightness = m_nBrightnessSlider;
			UpdateData(FALSE);
			UINT32 Result;
			Result = m_pGrabber->m_Camera.SetParameter(FGP_BRIGHTNESS, m_nBrightness);
		}
		if (GetDlgItem(IDC_SLIDER_GAIN)->GetSafeHwnd() == hwndScrollBar)    //增益滑动条响应
		{
			UpdateData(TRUE);
			m_nGain = m_nGainSlider;
			UpdateData(FALSE);
			UINT32 Result;
			Result = m_pGrabber->m_Camera.SetParameter(FGP_GAIN, m_nGain);
		}
		if (GetDlgItem(IDC_SLIDER_SHUTTER)->GetSafeHwnd() == hwndScrollBar)  //曝光滑动条响应
		{
			UpdateData(TRUE);
			m_nShutter = m_nShutterSlider;
			UpdateData(FALSE);
			UINT32 Result;
			Result = m_pGrabber->m_Camera.SetParameter(FGP_SHUTTER, m_nShutter);
		}
//		if (GetDlgItem(IDC_SLIDER_WHITEBAL_UB)->GetSafeHwnd() == hwndScrollBar)  //白平衡UB分量滑动条响应
//		{
//			m_iWhiteBalanceUB = m_ctrlWhiteBalUBSlider.GetPos();
//			UpdateData(FALSE);
//			UINT32 Result;
//			Result = m_pGrabber->m_Camera.SetParameter(FGP_WHITEBALCB, m_iWhiteBalanceUB);
//		}
//		if (GetDlgItem(IDC_SLIDER_WHITEBAL_VR)->GetSafeHwnd() == hwndScrollBar) //白平衡VR分量滑动条响应
//		{
//			m_iWhiteBalanceVR = m_ctrlWhiteBalVRSlider.GetPos();
//			UpdateData(FALSE);
//			UINT32 Result;
//			Result = m_pGrabber->m_Camera.SetParameter(FGP_WHITEBALCR, m_iWhiteBalanceVR);
//		}
	}	

	return CDialog::DefWindowProc(message, wParam, lParam);
}

//触发模式设置
void CSetCameraParam::OnSelchangeComboTriggerMode() 
{
	UpdateData(TRUE);
	int nval = m_TriggerMode.GetCurSel();
	m_nTriggerMode = nval;

	int  eTriggerValue; 

	switch(m_nTriggerMode) 
	{
	case 0:
		eTriggerValue = 0;  //AVTTriggerOff 内触发模式  
		break;
	case 1:
		eTriggerValue = 1;  //AVTTriggerOn  //上升沿触发//外触发开启
		break;
	case 2:
		eTriggerValue = 2;  //AVTTriggerMode2  //下降沿触发
		break;
	case 3:
		eTriggerValue = 3;  //AVTTriggerMode3 //高电平触发
		break;
	case 4:
		eTriggerValue = 4;  //AVTTriggerMode4  //低电平触发
		break;
	case 5:
		eTriggerValue = 5;  //AVTTriggerMode5  //编程模式上升沿触发
		break;
	case 6:
		eTriggerValue = 6;  //AVTTriggerMode6  //编程模式下降沿触发
		break;
	case 7:
		eTriggerValue = 7;  //AVTTriggerMode6  //编程模式下降沿触发
		break;
	case 8:
		eTriggerValue = 8;  //AVTTriggerMode6  //编程模式下降沿触发
		break;
	case 9:
		eTriggerValue = 9;  //AVTTriggerMode6  //编程模式下降沿触发
		break;
	default:
		eTriggerValue = 0;  //AVTTriggerOff 内触发模式
	}

	m_pGrabber->SetTriggerParam(eTriggerValue);
	UpdateData(FALSE);
}

void CSetCameraParam::OnBtnApply() //将当前参数保存到配置文件中
{
	CString str;
	str.Format(_T("%d"), m_nBrightness);
	WritePrivateProfileString(_T("相机参数"),_T("Brightness"),str,m_strProfileName);
	str.Format(_T("%d"), m_nShutter);
	WritePrivateProfileString(_T("相机参数"),_T("Shutter"),str,m_strProfileName);
	str.Format(_T("%d"), m_nGain);
	WritePrivateProfileString(_T("相机参数"),_T("Gain"),str,m_strProfileName);
	str.Format(_T("%d"), m_nTriggerMode);
 	WritePrivateProfileString(_T("相机参数"),_T("Trigger"),str,m_strProfileName);

	str.Format(_T("%d"), m_nAOILeft);
	WritePrivateProfileString(_T("相机参数"),_T("Left"),str,m_strProfileName);
	str.Format(_T("%d"), m_nAOIWidth);
	WritePrivateProfileString(_T("相机参数"),_T("Width"),str,m_strProfileName);
	str.Format(_T("%d"), m_nAOITop);
	WritePrivateProfileString(_T("相机参数"),_T("Top"),str,m_strProfileName);
	str.Format(_T("%d"), m_nAOIHeight);
 	WritePrivateProfileString(_T("相机参数"),_T("Height"),str,m_strProfileName);

	str.Format(_T("%d"), m_nCB);
 	WritePrivateProfileString(_T("相机参数"),_T("Whitebalcb"),str,m_strProfileName);
	str.Format(_T("%d"), m_nCR);
 	WritePrivateProfileString(_T("相机参数"),_T("Whitebalcr"),str,m_strProfileName);


}

void CSetCameraParam::OnOK() 
{
	CString str;
	
	GetDlgItem(IDC_BUTTON_FRAMETEST)->GetWindowText(str);
	
	if (str == _T("停止测试"))
	{
		KillTimer(100);
	}

	CDialog::OnOK();
}


void CSetCameraParam::OnClose() 
{
	UpdateData(TRUE);
	CDialog::OnClose();
}

void CSetCameraParam::OnButtonAoiApply() 
{
	if (m_pGrabber->m_CameraActive)
	{
		AfxMessageBox(_T("正在采集,不能设置AOI区域!"));
		return;
	}

	if (m_pGrabber->m_CamParms.ImageFormat < 7)
	{
		AfxMessageBox(_T("AVTSetAOI 非Format7模式"));
		return;
	}

	UINT32 Result;
	CString str;
	UpdateData(TRUE);

	if (m_nAOIWidth > m_pGrabber->m_CamParms.XSizeMax)
	{
		m_nAOIWidth = m_pGrabber->m_CamParms.XSizeMax;
	}
				
	if (m_nAOIWidth < m_pGrabber->m_CamParms.XSizeMin)
	{
		m_nAOIWidth = m_pGrabber->m_CamParms.XSizeMin;
	}

	if (m_nAOIWidth%m_pGrabber->m_CamParms.XSizeUnit != 0)
	{
		m_nAOIWidth-=m_nAOIWidth % m_pGrabber->m_CamParms.XSizeUnit;
	}

	if (m_nAOILeft > m_pGrabber->m_CamParms.XPosMax)
	{
		m_nAOILeft = m_pGrabber->m_CamParms.XPosMax;
	}
				
	if (m_nAOILeft < m_pGrabber->m_CamParms.XPosMin)
	{
		m_nAOILeft = m_pGrabber->m_CamParms.XPosMin;
	}

	if (m_nAOILeft%m_pGrabber->m_CamParms.XPosUnit != 0)
	{
		m_nAOILeft-=m_nAOILeft % m_pGrabber->m_CamParms.XPosUnit;
	}
				
	if (m_nAOIWidth + m_nAOILeft > m_pGrabber->m_CamParms.XSizeMax)
	{
		str.Format(_T("左边+宽度 > %d 请重新设定!"),m_pGrabber->m_CamParms.XSizeMax);
		AfxMessageBox(str);
		return;
	}
				


	if (m_nAOITop > m_pGrabber->m_CamParms.YPosMax)
	{
		m_nAOITop = m_pGrabber->m_CamParms.YPosMax;
	}
	if (m_nAOITop < m_pGrabber->m_CamParms.YPosMin)
	{
		m_nAOITop = m_pGrabber->m_CamParms.YPosMin;
	}
	if (m_nAOITop%m_pGrabber->m_CamParms.YPosUnit != 0)
	{
		m_nAOITop-=m_nAOITop % m_pGrabber->m_CamParms.YPosUnit;
	}

	if (m_nAOIHeight > m_pGrabber->m_CamParms.YSizeMax)
	{
		m_nAOIHeight = m_pGrabber->m_CamParms.YSizeMax;
	}
				
	if (m_nAOIHeight < m_pGrabber->m_CamParms.YSizeMin)
	{
		m_nAOIHeight = m_pGrabber->m_CamParms.YSizeMin;
	}

	if (m_nAOIHeight%m_pGrabber->m_CamParms.YSizeUnit != 0)
	{
		m_nAOIHeight-=m_nAOIHeight % m_pGrabber->m_CamParms.YSizeUnit;
	}
				
	if (m_nAOIHeight + m_nAOITop > m_pGrabber->m_CamParms.YSizeMax)
	{
		str.Format(_T("上边+高度 > %d 请重新设定!"),m_pGrabber->m_CamParms.YSizeMax);
		AfxMessageBox(str);
		return;
	}


	// Now we resize
	m_pGrabber->m_Camera.SetParameter(FGP_RESIZE,1);
				
	//////////////////////////////////////////////////////////////////////////
	//设置AOI区域
	if (m_nAOIWidth != m_pGrabber->m_CamParms.XSize 
		|| m_nAOILeft != m_pGrabber->m_CamParms.XPos)
	{
		if(m_nAOIWidth + m_pGrabber->m_CamParms.XPos <= m_pGrabber->m_CamParms.XSizeMax)
		{
			if (m_nAOIWidth != m_pGrabber->m_CamParms.XSize)
			{
				Result = m_pGrabber->m_Camera.SetParameter(FGP_XSIZE,m_nAOIWidth);
				if(Result==FCE_NOERROR)
				{
					m_pGrabber->m_CamParms.XSize = m_nAOIWidth;			
				}
				else
				{
					str.Format(_T("设置AOI宽度%d失败!"),m_nAOIWidth);
					AfxMessageBox(str);
					return;
				}
			}


			if (m_nAOILeft != m_pGrabber->m_CamParms.XPos)
			{
				Result = m_pGrabber->m_Camera.SetParameter(FGP_XPOSITION,m_nAOILeft);
				if(Result==FCE_NOERROR)
				{
					m_pGrabber->m_CamParms.XPos = m_nAOILeft;
				}
				else
				{
					str.Format(_T("设置AOI左边%d失败!"),m_nAOILeft);
					AfxMessageBox(str);
					return;
				}
			}

		}
		else
		{
			if (m_nAOILeft != m_pGrabber->m_CamParms.XPos)
			{
				Result = m_pGrabber->m_Camera.SetParameter(FGP_XPOSITION,m_nAOILeft);
				if(Result==FCE_NOERROR)
				{
					m_pGrabber->m_CamParms.XPos = m_nAOILeft;
				}
				else
				{
					str.Format(_T("设置AOI左边%d失败!"),m_nAOILeft);
					AfxMessageBox(str);
					return;
				}
			}


			if (m_nAOIWidth != m_pGrabber->m_CamParms.XSize)
			{
				Result = m_pGrabber->m_Camera.SetParameter(FGP_XSIZE,m_nAOIWidth);
				if(Result==FCE_NOERROR)
				{
					m_pGrabber->m_CamParms.XSize = m_nAOIWidth;			
				}
				else
				{
					str.Format(_T("设置AOI宽度%d失败!"),m_nAOIWidth);
					AfxMessageBox(str);
					return;
				}
			}
		}
	}

	if (m_nAOIHeight != m_pGrabber->m_CamParms.YSize 
		|| m_nAOITop != m_pGrabber->m_CamParms.YPos)
	{
		if(m_nAOIHeight + m_pGrabber->m_CamParms.YPos <= m_pGrabber->m_CamParms.YSizeMax)
		{

			if (m_nAOIHeight != m_pGrabber->m_CamParms.YSize)
			{
				Result = m_pGrabber->m_Camera.SetParameter(FGP_YSIZE,m_nAOIHeight);
				if(Result==FCE_NOERROR)
				{
					m_pGrabber->m_CamParms.YSize = m_nAOIHeight;
				}
				else
				{
					str.Format(_T("设置AOI高度%d失败!"),m_nAOIHeight);
					AfxMessageBox(str);
					return;
				}
			}

			
			if (m_nAOITop != m_pGrabber->m_CamParms.YPos)
			{
				Result = m_pGrabber->m_Camera.SetParameter(FGP_YPOSITION,m_nAOITop);
				if(Result==FCE_NOERROR)
				{
					m_pGrabber->m_CamParms.YPos = m_nAOITop;
				}
				else
				{
					str.Format(_T("设置AOI上边%d失败!"),m_nAOITop);
					AfxMessageBox(str);
					return;
				}
			}			
		}
		else
		{
			if (m_nAOITop != m_pGrabber->m_CamParms.YPos)
			{
				Result = m_pGrabber->m_Camera.SetParameter(FGP_YPOSITION,m_nAOITop);
				if(Result==FCE_NOERROR)
				{
					m_pGrabber->m_CamParms.YPos = m_nAOITop;
				}
				else
				{
					str.Format(_T("设置AOI上边%d失败!"),m_nAOITop);
					AfxMessageBox(str);
					return;
				}
			}


			if (m_nAOIHeight != m_pGrabber->m_CamParms.YSize)
			{
				Result = m_pGrabber->m_Camera.SetParameter(FGP_YSIZE,m_nAOIHeight);
				if(Result==FCE_NOERROR)
				{
					m_pGrabber->m_CamParms.YSize = m_nAOIHeight;
				}
				else
				{
					str.Format(_T("设置AOI高度%d失败!"),m_nAOIHeight);
					AfxMessageBox(str);
					return;
				}
			}		
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//F7M0_BYTE_PER_PACKET

	UINT32 nTemp = m_pGrabber->m_CamParms.ImageFormat-7;
	UINT32 nTemp1 = nTemp<<12;
	UINT32 Address = m_pGrabber->m_CamParms.Format7ModeAdress+0x44+nTemp1;//0xF0F08044+nTemp1;
	
	UINT32 Value;
	Result = m_pGrabber->m_Camera.ReadRegister(Address,&Value);//F7M0_BYTE_PER_PACKET
	if(Result==FCE_NOERROR)
	{
		//////////////////////////////////////////////////////////////////////////
		UINT32  Value1 = 0xffff0000;
		UINT32  Value2 = 0x0;
		Value2 = Value&Value1;
		Value1 = Value2>>16;
		
		UINT32 Value3 = 0xffff;
		UINT32 Value4 = 0x0;
		Value4= Value&Value3;

		if(Value1 != Value4)
		{
			//////////////////////////////////////////////////////////////////////////
			Value1 = Value4<<16;
			Value1 +=Value4;	
			Result = m_pGrabber->m_Camera.WriteRegister(Address,Value1);//F7M0_BYTE_PER_PACKET
	
			if(Result==FCE_NOERROR)
			{
			}
			else
			{
				AfxMessageBox(_T("设置包长失败!"));
				return;
			}	
		}
	}
	else
	{
		AfxMessageBox(_T("读取包长失败!"));
		return;
	}


	m_pGrabber->m_Camera.SetParameter(FGP_RESIZE,0);
	//////////////////////////////////////////////////////////////////////////
	//公共设置
	if ((UINT32)(m_pGrabber->m_nImageWidth) != m_pGrabber->m_CamParms.XSize
		||(UINT32)(m_pGrabber->m_nImageHeight) != m_pGrabber->m_CamParms.YSize)
	{
		m_pGrabber->m_nImageWidth  = m_pGrabber->m_CamParms.XSize;
		m_pGrabber->m_nImageHeight = m_pGrabber->m_CamParms.YSize;	
		m_pGrabber->m_nImageBufferSize = m_pGrabber->m_nImageWidth * m_pGrabber->m_nImageHeight * m_pGrabber->m_nImageByteCount; 
		
		if(m_pGrabber->m_pbImageBufferAdd != NULL)
		{
			delete [](m_pGrabber->m_pbImageBufferAdd);
			m_pGrabber->m_pbImageBufferAdd = NULL;
		}
//		m_pGrabber->m_pbImageBufferAdd = new BYTE[m_pGrabber->m_nImageWidth*m_pGrabber->m_nImageHeight*3];

	}

// 	str.Format(_T("%d"), m_nAOILeft);
// 	WritePrivateProfileString(_T("相机参数"),_T("Left"),str,m_strProfileName);
// 	str.Format(_T("%d"), m_nAOIWidth);
// 	WritePrivateProfileString(_T("相机参数"),_T("Width"),str,m_strProfileName);
// 	str.Format(_T("%d"), m_nAOITop);
// 	WritePrivateProfileString(_T("相机参数"),_T("Top"),str,m_strProfileName);
// 	str.Format(_T("%d"), m_nAOIHeight);
//  WritePrivateProfileString(_T("相机参数"),_T("Height"),str,m_strProfileName);

	UpdateData(FALSE);

}

void CSetCameraParam::OnButtonFrametest() 
{
	CString str;

	GetDlgItem(IDC_BUTTON_FRAMETEST)->GetWindowText(str);

	if (str == _T("开始测试"))
	{
		//////////////////////////////////////////////////////////////////////////
		//获取当前帧数，用来计算帧率
		m_OldFrameCnt = m_pGrabber->m_FrameCnt;
		SetTimer(100,1000,NULL);

		GetDlgItem(IDC_BUTTON_FRAMETEST)->SetWindowText(_T("停止测试"));
	}
	else if (str == _T("停止测试"))
	{
		KillTimer(100);
		GetDlgItem(IDC_STATIC_FRAME)->SetWindowText(_T(""));
		GetDlgItem(IDC_BUTTON_FRAMETEST)->SetWindowText(_T("开始测试"));
	}
	
}

void CSetCameraParam::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == 100)
	{
		CString str;
		str.Format(_T("%d帧/秒"),m_pGrabber->m_FrameCnt-m_OldFrameCnt);
		m_OldFrameCnt = m_pGrabber->m_FrameCnt;
		GetDlgItem(IDC_STATIC_FRAME)->SetWindowText(str);
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CSetCameraParam::OnButtonLogin() 
{
	// TODO: Add your control notification handler code here
//	UpdateData(TRUE);
//	Login login(m_strProfileName);
//	login.DoModal();

//	if (login.m_bLogin)
//	{
		EnableShow(TRUE);
		GetDlgItem(IDC_BUTTON_LOGIN)->EnableWindow(FALSE);
//	}

}

void CSetCameraParam::EnableShow(BOOL bEnable)
{
	if (bEnable)
	{
		GetDlgItem(IDC_SLIDER_BRIGHTNESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_BRIGHTNESS)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLIDER_GAIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(TRUE);
		GetDlgItem(IDC_SLIDER_SHUTTER)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SHUTTER)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_TRIGGER_MODE)->EnableWindow(TRUE);
		if (m_pGrabber->m_CameraActive)
		{
			GetDlgItem(IDC_EDIT_AOI_HEIGHT)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_AOI_WIDTH)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_AOI_LEFT)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_AOI_TOP)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_AOI_APPLY)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_EDIT_AOI_HEIGHT)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_AOI_WIDTH)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_AOI_LEFT)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_AOI_TOP)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_AOI_APPLY)->EnableWindow(TRUE);
		}

	
		GetDlgItem(IDC_BUTTON_FRAMETEST)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_APPLY)->EnableWindow(TRUE);


//////////////////////////////////////////////////////////////////////////
		if (m_pGrabber->m_bIsColorCamera == FALSE) //如果是彩色相机则打开白平衡 
		{
			GetDlgItem(IDC_EDIT_CB)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_CR)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_AOI_APPLYWB)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_EDIT_CB)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_CR)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_AOI_APPLYWB)->EnableWindow(TRUE);
		}

//////////////////////////////////////////////////////////////////////////

	}
	else
	{
		GetDlgItem(IDC_SLIDER_BRIGHTNESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_BRIGHTNESS)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_GAIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_SLIDER_SHUTTER)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_SHUTTER)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_TRIGGER_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AOI_LEFT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AOI_WIDTH)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AOI_TOP)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_AOI_HEIGHT)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_AOI_APPLY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_FRAMETEST)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_APPLY)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CB)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CR)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_AOI_APPLYWB)->EnableWindow(FALSE);


	}
}



void CSetCameraParam::OnButtonAoiApplywb() 
{
	UINT32 Result;
	CString str;
	UpdateData(TRUE);


	if (m_nCB > m_pGrabber->m_CamParms.WhiteBalanceMaxCB)
	{
		m_nCB = m_pGrabber->m_CamParms.WhiteBalanceMaxCB;
	}
				
	if (m_nCB < m_pGrabber->m_CamParms.WhiteBalanceMinCB)
	{
		m_nCB = m_pGrabber->m_CamParms.WhiteBalanceMinCB;
	}

	if(m_nCB != m_iOldCB)
	{
		Result=m_pGrabber->m_Camera.SetParameter(FGP_WHITEBALCB,m_nCB);
		if (Result==FCE_NOERROR)
		{
			m_pGrabber->m_CamParms.WhiteBalanceCB=m_nCB;
			m_iOldCB = m_nCB;
		}
		else
		{
			AfxMessageBox(_T("设置白平衡CB失败!"));
		}
	}


	if (m_nCR > m_pGrabber->m_CamParms.WhiteBalanceMaxCR)
	{
		m_nCR = m_pGrabber->m_CamParms.WhiteBalanceMaxCR;
	}
				
	if (m_nCR < m_pGrabber->m_CamParms.WhiteBalanceMinCR)
	{
		m_nCR = m_pGrabber->m_CamParms.WhiteBalanceMinCR;
	}

	if(m_nCR != m_iOldCR)
	{
		Result=m_pGrabber->m_Camera.SetParameter(FGP_WHITEBALCR,m_nCR);
		if (Result==FCE_NOERROR)
		{
			m_pGrabber->m_CamParms.WhiteBalanceCR=m_nCR;
			m_iOldCR = m_nCR;
		}
		else
		{
			AfxMessageBox(_T("设置白平衡CR失败!"));
		}
	}

	UpdateData(FALSE);

}
