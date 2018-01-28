//--------------------------------------------------------------- 
/** 
\file      CameraBase.cpp
\brief     CameraBase function
\version   v1.0.13035.9301 
\date      2013-5-30 
*/ 
//--------------------------------------------------------------- 

#include "stdafx.h"
#include "CameraBase.h"
#include "ExFn.h"


CameraBase::CameraBase()
{
	m_nImageWidth = 0;
	m_nImageHeight = 0;
	m_nWndWidth = 0;
	m_nWndHeight = 0;
	m_nPayLoadSize = 0;
	m_pShowWnd = NULL;
	m_pDC = NULL;
	m_pLogWnd = NULL;
	m_hDevice = NULL;
	m_chBmpBuf = new char[2048];
	m_bIsOpen = false;
	m_bIsSnaping = false;
	m_bShowImg = true;
	m_bSaveBMP = true;
	m_bSaveAVI = false;
	m_bEnableGetImageProcess = false;
	m_pBmpInfo = NULL;
	m_pImgRGBBuffer = NULL;
	m_pImgRaw8Buffer = NULL;
	m_strBMPFolder = _T("C:\\test");
	m_strAVIFolder = _T("");
	m_pAVIFile = NULL;
	m_bCompressed = false;
	m_nTimeFrame = 0;
	m_nTotalNum = 0;
	m_bInitSuccess = false;
	m_nImageByteCount = 1;
	m_nFrameCount = 0;

	m_nWidth = 0;
	m_nHeight = 0;
	m_nOffsetX = 0;
	m_nOffsetY = 0;
	m_nExposureMode = 0;
	m_dExposureTime = 0;
	m_bExposureAuto = false;
	m_nGain = 0;
	m_bGainAuto = false;
	m_nBlacklevel = 0;
	m_bBlacklevelAuto = false;
	m_dBalanceRatio = 0;
	m_bBalanceRatioAuto = false;
	m_bTriggerMode = false;
	m_nTriggerActivation = 0;
	m_pParamSetDlg = NULL;
}

CameraBase::~CameraBase()
{
	if (m_chBmpBuf != NULL)
	{
		delete []m_chBmpBuf;
		m_chBmpBuf = NULL;
	}
}

// 初始化
BOOL CameraBase::Init(const s_GBINITSTRUCT* pInitParam)
{
	//AfxMessageBox("asd");
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	emStatus= GXInitLib();//初始化设置库------------

	BOOL bRet = false;

	s_GBINITSTRUCT InitCardParam;
	InitCardParam.iGrabberSN = pInitParam->iGrabberSN;
	//将char的函数，写入到指定的字符串中
// 	_tprintf_s(InitCardParam.strGrabberFile, pInitParam->strGrabberFile);//读取相机配置文件位置
// 
// 	_tprintf_s(InitCardParam.strDeviceName, pInitParam->strDeviceName);//读取设备的名称
// 	_tprintf_s(InitCardParam.strDeviceMark, pInitParam->strDeviceMark);//读取SN
	memcpy_s(InitCardParam.strGrabberFile, GBMaxFileNameLen, pInitParam->strGrabberFile, GBMaxFileNameLen);
	memcpy_s(InitCardParam.strDeviceName, GBMaxFileNameLen, pInitParam->strDeviceName, GBMaxFileNameLen);
	memcpy_s(InitCardParam.strDeviceMark, GBMaxFileNameLen, pInitParam->strDeviceMark, GBMaxFileNameLen);
	
	InitCardParam.CallBackFunc = pInitParam->CallBackFunc;		//设置回调函数
	InitCardParam.Context = pInitParam->Context;				//将回调函数的窗口指针传递
	InitCardParam.EventCallbackFunc = pInitParam->EventCallbackFunc;	//将触发不响应的回调函数指针传递
	InitCardParam.iGrabberTypeSN = pInitParam->iGrabberTypeSN ;// 采集卡类型编号 [10/19/2010 SJC]

	
	emStatus = Open(&InitCardParam);//打开相机设置，并且初始化

	if (emStatus == GX_STATUS_SUCCESS)
	{
		bRet = true;
	}	
	return bRet;
}

bool CameraBase::Init(const CGrabInitInfo& rGrabInitInfo)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	emStatus = GXInitLib();//初始化设置库------------

	bool bRet = false;

	s_GBINITSTRUCT InitCardParam;
	InitCardParam.iGrabberSN = rGrabInitInfo.iGrabSN;
	//将char的函数，写入到指定的字符串中
	//_tprintf_s(InitCardParam.strGrabberFile, rGrabInitInfo.lpszPathFileOfGrab);//读取相机配置文件位置
	memcpy_s(InitCardParam.strGrabberFile, GBMaxFileNameLen, rGrabInitInfo.lpszPathFileOfGrab, GBMaxFileNameLen);
	GetPrivateProfileString(_T("Grabber Parameters"), _T("GrabberName"), _T(""), InitCardParam.strDeviceName, GBMaxTextLen, rGrabInitInfo.lpszPathFileOfGrab);
	GetPrivateProfileString(_T("Grabber Parameters"), _T("GrabberMark"), _T(""), InitCardParam.strDeviceMark, GBMaxTextLen, rGrabInitInfo.lpszPathFileOfGrab);
	InitCardParam.CallBackFunc = rGrabInitInfo.CallBackFunc;		//设置回调函数
	InitCardParam.Context = rGrabInitInfo.Context;				//将回调函数的窗口指针传递
	InitCardParam.EventCallbackFunc = rGrabInitInfo.EventCallbackFunc;	//将触发不响应的回调函数指针传递
	InitCardParam.iGrabberTypeSN = GetPrivateProfileInt(_T("Grabber Parameters"), _T("GrabberTypeSN"), -1, rGrabInitInfo.lpszPathFileOfGrab);// 采集卡类型编号 [10/19/2010 SJC]


	emStatus = Open(&InitCardParam);//打开相机设置，并且初始化

	if (emStatus == GX_STATUS_SUCCESS)
	{
		bRet = true;
	}
	return bRet;
}

//开始采集
BOOL CameraBase::StartGrab()
{
	StartCallbackAcq();
	return TRUE;
} 

//停止采集
BOOL CameraBase::StopGrab()
{
	StopCallbackAcq();
	return TRUE;
} 

// 单帧采集 
BOOL CameraBase::Snapshot()
{	

// 	AfxMessageBox("此相机暂不支持单帧采集！");

	SnapCallbackAcq();

	return TRUE;
}

//设置通用参数
BOOL CameraBase::SetParamInt(GBParamID Param, int nReturnVal)
{
	switch (Param)
	{
	case GBImageWidth:
		break;
	case GBImageHeight:
		break;
	case GBImagePixelSize:
		break;
	case GBImageBufferSize:
		break;
	case GBImageBufferAddr:
		break;
	case GBGrabberTypeSN:
		break;
	case GBExposureTime:
	{
		SetFloat(GX_FLOAT_EXPOSURE_TIME, (float)nReturnVal);
		m_dExposureTime = m_iExposureTime = nReturnVal;
	}
		break;
	case GBParamID::GBGain:
	{
		m_nGain = nReturnVal;
		SetInt(GX_INT_GAIN, m_nGain);
	}
	break;
	default:
		break;
	}
	return TRUE;
}

//得到通用参数
BOOL CameraBase::GetParamInt(GBParamID Param, int &nReturnVal)
{
	switch(Param)
	{
	case GBImageWidth:
		nReturnVal = m_nWidth;
		break;
	case GBImageHeight:
		nReturnVal = m_nHeight;
		break;
	case GBImagePixelSize:
		nReturnVal = m_nImageByteCount;
		break;
	case GBImageBufferSize:
// 		nReturnVal = m_nWidth * m_nHeight * m_nImageByteCount;
		nReturnVal = m_nWidth * m_nHeight; //[2016-2-24 zhy]
		break;
	case GBImageBufferAddr:
		nReturnVal = (int)m_pImgRGBBuffer;
		break;

	case GBGrabberTypeSN:
		nReturnVal = m_nGrabberType;
		break;

	case GBExposureTime:
		nReturnVal = m_dExposureTime;
		break;
	case GBParamID::GBGain:
		nReturnVal = m_nGain;
		break;
	case GBParamID::GBIsColorFilter:
		nReturnVal =  m_bIsColorFilter;
		break;
	case GBParamID::GBBayerType:
		nReturnVal = m_iBayerType;
		break;
	case GBParamID::GBEnableFlipImage:
		nReturnVal = m_bEnableFlip;
		break;
	default:
		AfxMessageBox(_T("GetParamInt switch(Param)  default:..."));
		m_LastErrorInfo.iErrorCode = DCErrorGetParam;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("得到参数出错"));
		_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetParamInt()函数"));
		
		return FALSE;
	}
	
	return TRUE;
}

bool CameraBase::GetParamBool(GBParamID Param, bool bRetVal)
{
	switch (Param)
	{
	default:
		break;
	}
	return true;
}

//设置参数
bool CameraBase::SetParamBool(GBParamID Param, bool bInputVal)
{
	return true;
}

//调用参数对话框
void CameraBase::CallParamDialog()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if(m_pParamSetDlg != NULL )
	{
		AfxMessageBox(_T("控制窗口已打!")) ;
		return ;
	}
	m_pParamSetDlg=new CSetParamDialog();
	m_pParamSetDlg->m_pCamera = this;
	m_pParamSetDlg->Create(CSetParamDialog::IDD);
	m_pParamSetDlg->CenterWindow();
    m_pParamSetDlg->ShowWindow(SW_SHOW);

}

// 获得错误信息
void CameraBase::GetLastErrorInfo(s_GBERRORINFO *pErrorInfo)
{
}

BOOL CameraBase::MERSetParamInt(MERParamID Param,int nInputVal1,int nInputVal2,int nInputVal3,int nInputVal4,int nInputVal5,void *sInputVal6)
{
	GX_STATUS status = GX_STATUS_SUCCESS;	

	bool bRet = false;
	if (m_bIsSnaping)
	{
		StopGrab();
		bRet = true;
	}

	switch(Param)
	{
	case MERSnapMode:
		if (nInputVal1 == 0)
		{
			SetEnum(GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_OFF);
			m_bTriggerMode = false;
		} 
		else
		{
			SetEnum(GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_ON);
			m_bTriggerMode = true;
		}
		break;
	case MERExposure:
		SetFloat(GX_FLOAT_EXPOSURE_TIME,(float)nInputVal1);
		m_dExposureTime = nInputVal1;
		break;
	case MERGain:
		SetInt(GX_INT_GAIN,nInputVal1);
		m_nGain = nInputVal1;
		break;
	case MERBlacklevel:
		SetInt(GX_INT_BLACKLEVEL,nInputVal1);
		m_nBlacklevel = nInputVal1;
		break;
	case MERWidth:				//[2017-3-24 zhy]
		if((nInputVal1+m_nOffsetX)>GetCameraMaxWid())
		{
			AfxMessageBox(_T("宽度+X偏移超出最大值"));
			return FALSE;
		}
		status = SetInt(GX_INT_WIDTH, nInputVal1);	
		if(status!=GX_STATUS_SUCCESS)
		{
			AfxMessageBox(_T("设置宽度失败!"));
			return FALSE;
		}
		else
		{
			m_nWidth=nInputVal1;
		}
		break;
	case MERHeight:		//[2017-3-24 zhy]
		if((nInputVal1+m_nOffsetY)>GetCameraMaxHei())
		{
			AfxMessageBox(_T("高度+Y偏移超出最大值"));
			return FALSE;
		}
		status = SetInt(GX_INT_HEIGHT, nInputVal1);	
		if(status!=GX_STATUS_SUCCESS)
		{
			AfxMessageBox(_T("设置高度失败!"));
			return FALSE;
		}
		else
		{
			m_nHeight=nInputVal1;
		}
		break;
	case MERXOffset:		//[2017-3-24 zhy]
		if((nInputVal1+m_nWidth)>GetCameraMaxWid())
		{
			AfxMessageBox(_T("宽度+X偏移超出最大值"));
			return FALSE;
		}
		status = SetInt(GX_INT_OFFSET_X, nInputVal1);	
		if(status!=GX_STATUS_SUCCESS)
		{
			AfxMessageBox(_T("设置X偏移量失败!"));
			return FALSE;
		}
		else
		{
			m_nOffsetX=nInputVal1;
		}
		break;
	case MERYOffset:		//[2017-3-24 zhy]
		if((nInputVal1+m_nHeight)>GetCameraMaxHei())
		{
			AfxMessageBox(_T("高度+Y偏移超出最大值"));
			return FALSE;
		}
		status = SetInt(GX_INT_OFFSET_Y, nInputVal1);	
		if(status!=GX_STATUS_SUCCESS)
		{
			AfxMessageBox(_T("设置Y偏移量失败!"));
			return FALSE;
		}
		else
		{
			m_nOffsetY=nInputVal1;
		}
		break;
	}

	if (bRet)
	{
		StartGrab();
	}
	return TRUE;
}

//得到相机专有参数
BOOL CameraBase::MERGetParamInt(MERParamID Param, int &nReturnVal1,int &nReturnVal2,int &nReturnVal3)
{
	__int64 ntemp;
	double dtemp;
	switch(Param)
	{
	case MERSnapMode:
		GetEnum(GX_ENUM_TRIGGER_MODE,&ntemp);
		nReturnVal1 = ntemp;
		break;
	case MERExposure:
		GetFloat(GX_FLOAT_EXPOSURE_TIME,&dtemp);
		nReturnVal1 = (int)dtemp;
		break;
	case MERGain:
		GetInt(GX_INT_GAIN,&ntemp);
		nReturnVal1 = ntemp;
		break;
	case MERBlacklevel:
		GetInt(GX_INT_BLACKLEVEL,&ntemp);
		nReturnVal1 = ntemp;
		break;
	}
	return TRUE;
}



int  CameraBase::GetCameraMaxWid()//获得相机的最大宽度
{
	__int64 ntemp;
	GetInt(GX_INT_WIDTH_MAX,&ntemp);
	m_maxwid=ntemp;
	return ntemp;



}
int  CameraBase::GetCameraMaxHei()//获得相机的最大高度
{
	__int64 ntemp;
	double dtemp;
	GetInt(GX_INT_HEIGHT_MAX,&ntemp);
	m_maxhei=ntemp;
	return ntemp;


}




//输出端口发送信号
BOOL CameraBase::SetOutputValue(int nOutputPort,int nOutputValue)
{
	return TRUE;
}

//----------------------------------------------------------------------------------
/**
\brief  打开设备
\return 打开设备成功则返回GX_STATUS_SUCCESS，否则返回错误码
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::Open(const s_GBINITSTRUCT* pInitParam)
{
	m_nGrabberSN   = pInitParam->iGrabberSN +1;			//序号 // 公司相机序号从1开始,为了和其他相机相兼容，自加1 [9/16/2010 SJC]
	m_CallBackFunc = pInitParam->CallBackFunc;			//回调函数指针
	m_EventCallbackFunc	= pInitParam->EventCallbackFunc;//触发不响应事件回调函数
	m_Context      = pInitParam->Context;				//存放调用初始化函数的对象的this指针
	
	m_sInitFile	   = pInitParam->strGrabberFile;		//用于初始化的文件????

	m_sDeviceName  = pInitParam->strDeviceName;			//采集设备名
	m_strDeviceMark.Format(_T("%s"),pInitParam->strDeviceMark);	
	m_nGrabberType = pInitParam->iGrabberTypeSN ;		// 采集卡类型

	CFileFind fileSerch;
	if(!fileSerch.FindFile(m_sInitFile))
	{
		AfxMessageBox(_T("相机初始化文件不存在!"));
		m_LastErrorInfo.iErrorCode = DCErrorOpenCam;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("相机初始化文件不存在!"));
		_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("请查证设备初始化文件名是否正确"));
		return GX_STATUS_ERROR;
	}
	
	InitParamFromINI();// 读取相机配置文件，本身配置文件

	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	try
	{
		//枚举设备个数，超时时间1000ms[超时时间用户自行设置，没有推荐值]
		uint32_t nDeviceNum = 0;
		emStatus = GXUpdateDeviceList(&nDeviceNum, 1000);//获取设备个数
		m_nTotalNum = nDeviceNum;
		if (emStatus != GX_STATUS_SUCCESS)
		{
			AfxMessageBox(_T("获取设个个数失败!"));
			return emStatus;
		}

		if (m_nTotalNum <= 0)
		{
			AfxMessageBox(_T("设备个数小于等于0!"));

			return GX_STATUS_ERROR;
		}
		
		//根据序列号打开相机
		GX_OPEN_PARAM openParam;
		openParam.openMode = GX_OPEN_SN;
		openParam.accessMode = GX_ACCESS_EXCLUSIVE;
		char pDeviceMark[256];
#ifdef UNICODE
		string strTemp = CStringConvert::UCS2_ANSI(m_strDeviceMark.GetBuffer());
		memcpy_s(pDeviceMark, 256, strTemp.c_str(), 256);
		openParam.pszContent = pDeviceMark;
#else
		openParam.pszContent = (LPSTR)(LPCTSTR)m_strDeviceMark;
#endif
		//openParam.pszContent = (LPSTR)(LPCTSTR)m_strDeviceMark;
		emStatus = GXOpenDevice(&openParam,&m_hDevice);//设置指定的当前设备

		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("打开失败，序列号为:%s !"),m_strDeviceMark);
			AfxMessageBox(str);

			return emStatus;
		}

		m_bIsOpen = true;

		//选择触发信号被屏蔽事件
		GXSetEnum(m_hDevice,GX_ENUM_EVENT_SELECTOR,GX_ENUM_EVENT_SELECTOR_FRAMESTART_OVERTRIGGER);
		
		//开启触发信号被屏蔽事件
		GXSetEnum(m_hDevice,GX_ENUM_EVENT_NOTIFICATION,GX_ENUM_EVENT_NOTIFICATION_ON);
		
		//注册触发信号被屏蔽事件回调函数
		GXRegisterFeatureCallback(m_hDevice,this,OnFeatureCallbackFun,GX_INT_EVENT_FRAMESTART_OVERTRIGGER,&m_hCB);

//--------------------------打开相机设备完毕------------------------------------		
		if (!m_bInitSuccess)
		{
			// 设置参数
			SetInitParam();
			
			emStatus = __PrepareForShowImg();
			if (emStatus != GX_STATUS_SUCCESS)
			{
				return emStatus;
			}
			
			if (m_nImageWidth < m_nWidth || m_nImageHeight < m_nHeight)
			{
				AfxMessageBox(_T("宽度或者高度超出最大允许值"));

				m_LastErrorInfo.iErrorCode = DCErrorValOverFlow;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("宽度或者高度超出最大允许值"));
				_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("Init()函数中的HVGetDeviceInfo()"));
				return GX_STATUS_ERROR;
			}
			m_bInitSuccess = TRUE;
		}
		else
		{
			AfxMessageBox(_T("相机重复初始化"));

			m_LastErrorInfo.iErrorCode = DCErrorInit;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("相机已经初始化成功!"));
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("相机重复初始化"));
			return GX_STATUS_SUCCESS;
		}
	}
	catch(...)
	{
		AfxMessageBox(_T("Init()函数中try捕捉到的异常"));
		m_LastErrorInfo.iErrorCode = DCErrorInit;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("初始化失败"));
		_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("Init()函数中try捕捉到的异常"));
		return GX_STATUS_ERROR;
	}
	//**[9/7/2017 ConanSteve]**:  初始化通用参数
	m_iImageWidth = m_nWidth;
	m_iImageHeight = m_nHeight;
	m_iImagePixelSize = m_nImageByteCount;
	m_iOffsetX = m_nOffsetX;
	m_iOffsetY = m_nOffsetY;
	bool bIsColorFilter{ false };
	emStatus = GXIsImplemented(m_hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &bIsColorFilter);
	m_bIsColorFilter = bIsColorFilter ? TRUE : FALSE;
	VERIFY_STATUS_RET(emStatus);
	if (TRUE == m_bIsColorFilter)
	{
		emStatus = GXGetEnum(m_hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &m_nPixelColorFilter);
		m_iBayerType = m_nPixelColorFilter;
	}
	m_bEnableFlip = TRUE;
	return emStatus;
}

//读取配置文件
void CameraBase::InitParamFromINI()
{
	try
	{
		m_nWidth = GetPrivateProfileInt(_T("Camera"), _T("Width"), GetCameraMaxWid(), m_sInitFile);//默认值为相机的最大值
		m_nHeight = GetPrivateProfileInt(_T("Camera"), _T("Height"), GetCameraMaxHei(), m_sInitFile);
		m_nOffsetX = GetPrivateProfileInt(_T("Camera"), _T("OffsetX"), 0, m_sInitFile);
		m_nOffsetY = GetPrivateProfileInt(_T("Camera"), _T("OffsetY"), 0, m_sInitFile);
		m_nExposureMode = GetPrivateProfileInt(_T("Camera"), _T("ExposureMode"), 0, m_sInitFile);
		m_dExposureTime = GetPrivateProfileInt(_T("Camera"), _T("ExposureTime"), 10000, m_sInitFile);
		m_bExposureAuto = GetPrivateProfileInt(_T("Camera"), _T("ExposureAuto"), 0, m_sInitFile);
		m_nGain = GetPrivateProfileInt(_T("Camera"), _T("Gain"), 0, m_sInitFile);
		m_bGainAuto = GetPrivateProfileInt(_T("Camera"), _T("GainAuto"), 0, m_sInitFile);
		m_nBlacklevel = GetPrivateProfileInt(_T("Camera"), _T("Blacklevel"), 0, m_sInitFile);
		m_bBlacklevelAuto = GetPrivateProfileInt(_T("Camera"), _T("BlacklevelAuto"), 0, m_sInitFile);
		m_dBalanceRatio = GetPrivateProfileInt(_T("Camera"), _T("BalanceRatio"), 0, m_sInitFile);
		m_bBalanceRatioAuto = GetPrivateProfileInt(_T("Camera"), _T("BalanceRatioAuto"), 0, m_sInitFile);
		m_bTriggerMode = GetPrivateProfileInt(_T("Camera"), _T("TriggerMode"), 0, m_sInitFile);
	    m_nTriggerActivation = GetPrivateProfileInt(_T("Camera"), _T("TriggerActivation"), 0, m_sInitFile);
		m_nTriggerSource = GetPrivateProfileInt(_T("Camera"),_T("TriggerSource"),0,m_sInitFile); //[2016-11-18 zhy] 
	}
	catch (...) 
	{
		m_LastErrorInfo.iErrorCode = DCErrorReadIniFile;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, "读取配置文件出错");
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "InitParamFromINI()函数");
	}
}

//保存相机设置信息
void CameraBase::SaveParamToINI()
{
	CString str;
	str.Format(_T("%d"), m_nWidth);
	WritePrivateProfileString(_T("Camera"), _T("Width"), str, m_sInitFile);
	str.Format(_T("%d"), m_nHeight);
	WritePrivateProfileString(_T("Camera"), _T("Height"), str, m_sInitFile);
	str.Format(_T("%d"), m_nOffsetX);
	WritePrivateProfileString(_T("Camera"), _T("OffsetX"), str, m_sInitFile);
	str.Format(_T("%d"), m_nOffsetY);
	WritePrivateProfileString(_T("Camera"), _T("OffsetY"), str, m_sInitFile);
	str.Format(_T("%d"), m_nExposureMode);
	WritePrivateProfileString(_T("Camera"), _T("ExposureMode"), str, m_sInitFile);
	str.Format(_T("%d"), (int)m_dExposureTime);
	WritePrivateProfileString(_T("Camera"), _T("ExposureTime"), str, m_sInitFile);
	str.Format(_T("%d"), (int)m_bExposureAuto);
	WritePrivateProfileString(_T("Camera"), _T("ExposureAuto"), str, m_sInitFile);
	str.Format(_T("%d"), m_nGain);
	WritePrivateProfileString(_T("Camera"), _T("Gain"), str, m_sInitFile);
	str.Format(_T("%d"), (int)m_bGainAuto);
	WritePrivateProfileString(_T("Camera"), _T("GainAuto"), str, m_sInitFile);
	str.Format(_T("%d"), m_nBlacklevel);
	WritePrivateProfileString(_T("Camera"), _T("Blacklevel"), str, m_sInitFile);
	str.Format(_T("%d"), (int)m_bBlacklevelAuto);
	WritePrivateProfileString(_T("Camera"), _T("BlacklevelAuto"), str, m_sInitFile);
	str.Format(_T("%d"), m_dBalanceRatio);
	WritePrivateProfileString(_T("Camera"), _T("BalanceRatio"), str, m_sInitFile);
	str.Format(_T("%d"), (int)m_bBalanceRatioAuto);
	WritePrivateProfileString(_T("Camera"), _T("BalanceRatioAuto"), str, m_sInitFile);
	str.Format(_T("%d"), (int)m_bTriggerMode);
	WritePrivateProfileString(_T("Camera"), _T("TriggerMode"), str, m_sInitFile);
	str.Format(_T("%d"), m_nTriggerActivation);
	WritePrivateProfileString(_T("Camera"), _T("TriggerActivation"), str, m_sInitFile);
	str.Format(_T("%d"), m_nTriggerSource);
	WritePrivateProfileString(_T("Camera"), _T("TriggerSource"), str, m_sInitFile);

}

void CameraBase::SetInitParam()
{
	//设置感兴趣区域
	GX_STATUS status = GX_STATUS_SUCCESS;	
	status = SetInt(GX_INT_WIDTH, m_nWidth);	
	status = SetInt(GX_INT_HEIGHT, m_nHeight);
	status = SetInt(GX_INT_OFFSET_X, m_nOffsetX);
	status = SetInt(GX_INT_OFFSET_Y, m_nOffsetY);

	status = SetEnum(GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);
	status = SetEnum(GX_ENUM_GAIN_AUTO,GX_GAIN_AUTO_CONTINUOUS);
	//status=SetEnum(GX_ENUM_PIXEL_FORMAT,mono)

	//设置触发模式
	if (m_bTriggerMode)
	{
		status = SetEnum(GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_ON);
	}
	else
	{
		status = SetEnum(GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_OFF);
	}
	//--------------------------------------------------------------------------------------------------
	
	//设置触发激活方式
	switch(m_nTriggerActivation)
	{
	case 0:
		status = SetEnum(GX_ENUM_TRIGGER_ACTIVATION,GX_TRIGGER_ACTIVATION_RISINGEDGE);
		break;
	case 1:
		status = SetEnum(GX_ENUM_TRIGGER_ACTIVATION,GX_TRIGGER_ACTIVATION_FALLINGEDGE);
		break;
	}
	
	// 设置触发源
	status = GXSetEnum(m_hDevice, GX_ENUM_TRIGGER_SOURCE, m_nTriggerSource);

	//设置曝光模式
	switch(m_nExposureMode)
	{
	case 0:
//		status = SetEnum(GX_ENUM_EXPOSURE_MODE,GX_EXPOSURE_MODE_OFF);//由于水星Gige接口的SDK没有这个结构体的标示，所以就删除了
		break;
	case 1:
		status = SetEnum(GX_ENUM_EXPOSURE_MODE,GX_EXPOSURE_MODE_TIMED);
		break;
	case 2:
		status = SetEnum(GX_ENUM_EXPOSURE_MODE,GX_EXPOSURE_MODE_TRIGGERWIDTH);
		break;
	default:
		//status = SetEnum(GX_ENUM_EXPOSURE_MODE,GX_EXPOSURE_MODE_OFF);
		break;
	}
	//设置自动曝光
	if (m_bExposureAuto)
	{
		status = SetEnum(GX_ENUM_EXPOSURE_AUTO,GX_EXPOSURE_AUTO_CONTINUOUS);
	}
	else
	{
        status = SetEnum(GX_ENUM_EXPOSURE_AUTO,GX_EXPOSURE_AUTO_OFF);
		//设置曝光时间
	    status = SetFloat(GX_FLOAT_EXPOSURE_TIME,m_dExposureTime);
	}

	//设置自动增益
	if (m_bGainAuto)
	{
		status = SetEnum(GX_ENUM_GAIN_AUTO,GX_GAIN_AUTO_CONTINUOUS);
	}
	else
	{
		status = SetEnum(GX_ENUM_GAIN_AUTO,GX_GAIN_AUTO_OFF);
		//设置增益
	    status = SetInt(GX_INT_GAIN,m_nGain);
	}

	if (m_bBlacklevelAuto)
	{
		status = SetEnum(GX_ENUM_BLACKLEVEL_AUTO,GX_BLACKLEVEL_AUTO_CONTINUOUS);
	}
	else
	{
		status = SetEnum(GX_ENUM_BLACKLEVEL_AUTO,GX_BLACKLEVEL_AUTO_OFF);
		//设置黑电平
	    status = SetInt(GX_INT_BLACKLEVEL,m_nBlacklevel);
	}
	
//------------------------------------------------------------------------------------------------------
	
// 	//设置自动白平衡
// 	if (m_bBalanceRatioAuto)
// 	{
// 		status = SetEnum(GX_ENUM_BALANCE_WHITE_AUTO,GX_BALANCE_WHITE_AUTO_CONTINUOUS);
// 	}
// 	else
// 	{
// 		status = SetEnum(GX_ENUM_BALANCE_WHITE_AUTO,GX_BALANCE_WHITE_AUTO_OFF);
// 		//设置白平衡
//      	status = SetFloat(GX_FLOAT_BALANCE_RATIO,m_dBalanceRatio);
// 	}
	

}

//----------------------------------------------------------------------------------
/**
\brief  关闭设备
\return 关闭设备成功则返回GX_STATUS_SUCCESS，否则返回错误码
*/
//----------------------------------------------------------------------------------
BOOL CameraBase::Close()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	BOOL bRet = FALSE;
	//**[9/11/2017 ConanSteve]**:  先保存参数
	CExFn::WritePrivateProfileInt(_T("Camera"), _T("Gain"), m_nGain, m_sInitFile);
	CExFn::WritePrivateProfileInt(_T("Camera"), _T("ExposureTime"), m_dExposureTime, m_sInitFile);
	if (IsOpen())
	{
		if (IsSnaping())
		{
			emStatus = StopCallbackAcq();
			if (emStatus != GX_STATUS_SUCCESS)
			{
				return FALSE;
			}
		}

		__UnPrepareForShowImg();


		//注销触发信号被屏蔽事件回调
		//GXUnregisterFeatureCallback(m_hDevice, GX_INT_EVENT_FRAMESTART_OVERTRIGGER, m_hCB);
		
		emStatus = GXCloseDevice(m_hDevice);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			return FALSE;
		}
		bRet = TRUE;
		m_bIsOpen = false;

	}
	return bRet;
}

//----------------------------------------------------------------------------------
/**
\brief  设备打开状态
\return 设备打开则返回true，否则返回false
*/
//----------------------------------------------------------------------------------
bool CameraBase::IsOpen()
{
	return m_bIsOpen;
}



//----------------------------------------------------------------------------------
/**
\brief  注册图像显示窗口
\param  pWnd  指向图像显示窗口指针
\return 图像显示窗口指针不为NULL，返回GX_STATUS_SUCCESS；否则返回GX_STATUS_INVALID_PARAMETER
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::RegisterShowWnd(void* pWnd)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	if (pWnd == NULL)
	{
		return GX_STATUS_INVALID_PARAMETER;
	}

	m_pShowWnd = pWnd;

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  注册日志显示窗口
\param  pWnd  指向日志显示窗口指针
\return 日志显示窗口指针不为NULL，返回GX_STATUS_SUCCESS；否则返回GX_STATUS_INVALID_PARAMETER
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::RegisterLogWnd(void* pWnd)
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;

	if (pWnd == NULL)
	{
		return GX_STATUS_INVALID_PARAMETER;
	}

	m_pLogWnd = pWnd;

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  开始注册回调
\return 当接口调用成功，则返回GX_STATUS_SUCCESS，否则返回GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::StartCallbackAcq()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;

	if (IsOpen())
	{
		emStatus = GXRegisterCaptureCallback(m_hDevice, this, __OnFrameCallbackFun);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			__UnPrepareForShowImg();
			return emStatus;
		}
	//	__PrintInfotoWnd(MsgComposer("注册采集回调函数"));
		emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_START);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			__UnPrepareForShowImg();
			return emStatus;
		}
	//	__PrintInfotoWnd(MsgComposer("发送开采命令"));
		m_bIsSnaping = true;
	}	
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  停止注册回调
\return 当接口调用成功，则返回GX_STATUS_SUCCESS，否则返回GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::StopCallbackAcq()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen() && IsSnaping())
	{
		//如果当前正在录制视频流，先停止录制
		if (m_bSaveAVI)
		{
			EnableSaveAVI(false);
		}

		emStatus = GXSendCommand(m_hDevice, GX_COMMAND_ACQUISITION_STOP);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			return emStatus;
		}
		//__PrintInfotoWnd(MsgComposer("发送停采命令"));
		emStatus = GXUnregisterCaptureCallback(m_hDevice);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			return emStatus;
		}
		//__PrintInfotoWnd(MsgComposer("注销采集回调函数"));

		m_bIsSnaping = false;		
	}	
	return emStatus;
}

GX_STATUS CameraBase::SnapCallbackAcq()
{
	GX_STATUS  emStatus = GX_STATUS_SUCCESS;
	
	// 发送软触发命令
	emStatus = GXSendCommand(m_hDevice,GX_COMMAND_TRIGGER_SOFTWARE);

	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  使能显示图像功能
\param  bShowImg  true表示显示图像；false表示不显示图像
\return 无
*/
//----------------------------------------------------------------------------------
void CameraBase::EnableShowImg(bool bShowImg)
{
	m_bShowImg = bShowImg;
}

//----------------------------------------------------------------------------------
/**
\brief  使能保存BMP功能
\param  bSaveBMP  true表示保存；false表示不保存
\return 无
*/
//----------------------------------------------------------------------------------
void CameraBase::EnableSaveBMP(bool bSaveBMP)
{
	m_bSaveBMP = bSaveBMP;
}

void CameraBase::CompressedAVI(bool bCompressed)
{
	m_bCompressed = bCompressed;
}
//----------------------------------------------------------------------------------
/**
\brief  使能保存AVI功能
\param  bSaveAVI  true表示保存；false表示不保存
\return 无
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::EnableSaveAVI(bool bSaveAVI)
{
//	m_SaverCriSec.Enter();
// 	m_bSaveAVI = bSaveAVI;
// 
 	GX_STATUS emStatus = GX_STATUS_SUCCESS;
// 	if (m_bSaveAVI)
// 	{
// 		emStatus = __PrepareForSaveAVI();
// 		if (emStatus != GX_STATUS_SUCCESS)
// 		{
// 			m_SaverCriSec.Leave();
// 			__UnPrepareForSaveAVI();
// 			return emStatus;
// 		}
// 	}
// 	else
// 	{
// 		__UnPrepareForSaveAVI();
// 	}
// 	m_SaverCriSec.Leave();
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  设置保存BMP图片的文件夹路径
\param  pBMPFolder  路径，尾部不带斜杠
\return 无
*/
//----------------------------------------------------------------------------------
void CameraBase::SetBMPFolder(TCHAR* pBMPFolder)
{
	m_strBMPFolder = CString(pBMPFolder);
}

//----------------------------------------------------------------------------------
/**
\brief  设置保存AVI视频的文件夹路径
\param  szAVIFolder  路径，尾部不带斜杠
\return 无
*/
//----------------------------------------------------------------------------------
void CameraBase::SetAVIFolder(TCHAR* pAVIFolder)
{
	m_strAVIFolder = CString(pAVIFolder);
}

//----------------------------------------------------------------------------------
/**
\brief  设备开始采集状态
\return 设备开始采集则返回true，否则返回false
*/
//----------------------------------------------------------------------------------
bool CameraBase::IsSnaping()
{
	return m_bIsSnaping;
}

GX_STATUS CameraBase::IsImplemented(GX_FEATURE_ID emFeatureID, bool* pbIsImplemented)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXIsImplemented(m_hDevice, emFeatureID, pbIsImplemented);
		//打印日志信息
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("失败:GXIsImplemented 功能名称:%s 错误码:%d"),chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
	     	_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("IsImplemented()函数"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  获取功能码名称
\param  emFeatureID  功能码ID
\param  pszName    指向返回功能码名称的指针
\param  pnSize     功能码的字符串长度
\return            当接口调用成功，则返回GX_STATUS_SUCCESS，否则返回GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetFeatureName(GX_FEATURE_ID emFeatureID, char* pszName, size_t* pnSize)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetFeatureName(m_hDevice, emFeatureID, pszName, pnSize);
		//打印日志信息
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("失败:GXGetFeatureName 功能名称:%s 错误码:%d"),pszName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
	     	_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetFeatureName()函数"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  获取Int类型参数最小值、最大值、步长
\param  emFeatureID  功能码ID
\param  pIntRange  指向返回int型结构体的指针
\return            当接口调用成功，则返回GX_STATUS_SUCCESS，否则返回GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetIntRange(GX_FEATURE_ID emFeatureID, GX_INT_RANGE* pIntRange)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetIntRange(m_hDevice, emFeatureID, pIntRange);
		//打印日志信息
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("失败:GXGetIntRange 功能名称:%s 错误码:%d"),chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
	     	_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetIntRange()函数"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  获取int型参数当前值
\param  emFeatureID  功能码ID
\param  pnValue    指向返回当前值的指针
\return            当接口调用成功，则返回GX_STATUS_SUCCESS，否则返回GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetInt(GX_FEATURE_ID emFeatureID, int64_t* pnValue)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetInt(m_hDevice, emFeatureID, pnValue);
		//打印日志信息
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("失败:GXGetInt 功能名称:%s 错误码:%d"),chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
	     	_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetInt()函数"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  设置Int类型值功能参数
\param  emFeatureID  功能码ID
\param  pnValue    用户将要设置的值
\return            当接口调用成功，则返回GX_STATUS_SUCCESS，否则返回GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::SetInt(GX_FEATURE_ID emFeatureID, int64_t nValue)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXSetInt(m_hDevice, emFeatureID, nValue);
		//打印日志信息
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("失败:GXSetInt 功能名称:%s 错误码:%d"),chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorSetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
	     	_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("SetInt()函数"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  获取Float类型参数的最小值、最大值、步长、单位等描述信息
\param  emFeatureID    功能码ID
\param  pFloatRange  指向返回Float类型结构体的指针
\return              当接口调用成功，则返回GX_STATUS_SUCCESS，否则返回GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetFloatRange(GX_FEATURE_ID emFeatureID, GX_FLOAT_RANGE* pFloatRange)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetFloatRange(m_hDevice, emFeatureID, pFloatRange);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		emStatus = GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("失败:GXGetFloatRange 功能名称:%s 错误码:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetFloatRange()函数"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  获取Float类型当前参数值
\param  emFeatureID    功能码ID
\param  pdValue      指向返回Float类型参数值的指针
\return              当接口调用成功，则返回GX_STATUS_SUCCESS，否则返回GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetFloat(GX_FEATURE_ID emFeatureID, double* pdValue)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetFloat(m_hDevice, emFeatureID, pdValue);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("失败:GXGetFloat 功能名称:%s 错误码:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetFloat()函数"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  设置Float类型参数值
\param  emFeatureID    功能码ID
\param  dValue       用户将要设置的值
\return              当接口调用成功，则返回GX_STATUS_SUCCESS，否则返回GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::SetFloat(GX_FEATURE_ID emFeatureID, double dValue)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXSetFloat(m_hDevice, emFeatureID, dValue);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("失败:GXSetFloat 功能名称:%s 错误码:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorSetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("SetFloat()函数"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  获取枚举项的可选项个数
\param  emFeatureID    功能码ID
\param  pnEntryNums  指向返回可选项个数的指针
\return              当接口调用成功，则返回GX_STATUS_SUCCESS，否则返回GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetEnumEntryNums(GX_FEATURE_ID emFeatureID, uint32_t* pnEntryNums)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetEnumEntryNums(m_hDevice, emFeatureID, pnEntryNums);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("失败:GXGetEnumEntryNums 功能名称:%s 错误码:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetEnumEntryNums()函数"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  获取枚举类型值的描述信息
\param  emFeatureID         功能码ID
\param  pEnumDescription  指向枚举项的描述信息的数组指针
\param  pBufferSize       指向所有枚举项描述信息的数组大小
\return                   当接口调用成功，则返回GX_STATUS_SUCCESS，否则返回GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetEnumDescription(GX_FEATURE_ID emFeatureID, GX_ENUM_DESCRIPTION* pEnumDescription, size_t* pBufferSize)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetEnumDescription(m_hDevice, emFeatureID, pEnumDescription, pBufferSize);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("失败:GXGetEnumDescription 功能名称:%s 错误码:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetEnumDescription()函数"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  获取当前枚举值
\param  emFeatureID  功能码ID
\param  pnValue    指向返回的枚举值的指针
\return            当接口调用成功，则返回GX_STATUS_SUCCESS，否则返回GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetEnum(GX_FEATURE_ID emFeatureID, int64_t* pnValue)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetEnum(m_hDevice, emFeatureID, pnValue);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("失败:GXGetEnum 功能名称:%s 错误码:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetEnum()函数"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  设置枚举值
\param  emFeatureID  功能码ID
\param  nValue     用户将要设置的枚举值
\return            当接口调用成功，则返回GX_STATUS_SUCCESS，否则返回GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::SetEnum(GX_FEATURE_ID emFeatureID, int64_t nValue)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXSetEnum(m_hDevice, emFeatureID, nValue);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("失败:GXGetEnum 功能名称:%s 错误码:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorSetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("SetEnum()函数"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  获取bool类型值
\param  emFeatureID  功能码ID
\param  nValue     指向返回的布尔值的指针
\return            当接口调用成功，则返回GX_STATUS_SUCCESS，否则返回GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetBool(GX_FEATURE_ID emFeatureID, bool* pbValue)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetBool(m_hDevice, emFeatureID, pbValue);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("失败:GXGetBool 功能名称:%s 错误码:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetBool()函数"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  设置bool类型值
\param  emFeatureID  功能码ID
\param  bValue     用户将要设置的布尔值
\return            当接口调用成功，则返回GX_STATUS_SUCCESS，否则返回GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::SetBool(GX_FEATURE_ID emFeatureID, bool bValue)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXSetBool(m_hDevice, emFeatureID, bValue);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("失败:GXSetBool 功能名称:%s 错误码:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorSetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("SetBool()函数"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  获取字符串类型值的长度
\param  emFeatureID  功能码ID
\param  bValue     指向返回的长度值的指针，长度值带末位'\0'，长度单位字节
\return            当接口调用成功，则返回GX_STATUS_SUCCESS，否则返回GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetStringLength(GX_FEATURE_ID emFeatureID, size_t* pnSize)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetStringLength(m_hDevice, emFeatureID, pnSize);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("失败:GXGetStringLength 功能名称:%s 错误码:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetStringLength()函数"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  获取字符串类型值的内容
\param  emFeatureID  功能码ID
\param  pszContent 指向用户申请的字符串缓存地址
\param  pnSize     表示用户输入的字符串缓冲区地址的长度
\return            当接口调用成功，则返回GX_STATUS_SUCCESS，否则返回GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetString(GX_FEATURE_ID emFeatureID, char* pszContent, size_t* pnSize)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetString(m_hDevice, emFeatureID, pszContent, pnSize);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("失败:GXGetString 功能名称:%s 错误码:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetString()函数"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  设置字符串值内容
\param  emFeatureID  功能码ID
\param  pszContent 指向用户将要设置的字符串地址
\return            当接口调用成功，则返回GX_STATUS_SUCCESS，否则返回GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::SetString(GX_FEATURE_ID emFeatureID, char* pszContent)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXSetString(m_hDevice, emFeatureID, pszContent);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("失败:GXSetString 功能名称:%s 错误码:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorSetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("SetString()函数"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  获取块儿数据长度
\param  emFeatureID  功能码ID
\param  pnSize     指向返回的长度值的指针，长度单位字节
\return            当接口调用成功，则返回GX_STATUS_SUCCESS，否则返回GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetBufferLength(GX_FEATURE_ID emFeatureID, size_t* pnSize)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetBufferLength(m_hDevice, emFeatureID, pnSize);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("失败:GXGetBufferLength 功能名称:%s 错误码:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetBufferLength()函数"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  获取块儿数据
\param  emFeatureID  功能码ID
\param  pBuffer    指向用户申请的块儿数据内存地址指针
\param  pnSize     表示用户输入的缓冲区地址的长度
\return            当接口调用成功，则返回GX_STATUS_SUCCESS，否则返回GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetBuffer(GX_FEATURE_ID emFeatureID, uint8_t* pBuffer, size_t* pnSize)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXGetBuffer(m_hDevice, emFeatureID, pBuffer, pnSize);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("失败:GXGetBuffer 功能名称:%s 错误码:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorGetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetBuffer()函数"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  设置块儿数据
\param  emFeatureID  功能码ID
\param  pBuffer    指向用户申请的块儿数据内存地址指针
\param  nSize      用户输入的缓冲区地址的长度
\return            当接口调用成功，则返回GX_STATUS_SUCCESS，否则返回GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::SetBuffer(GX_FEATURE_ID emFeatureID, uint8_t* pBuffer, size_t nSize)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXSetBuffer(m_hDevice, emFeatureID, pBuffer, nSize);
		char chFeatureName[64] = {0};
		size_t nSizeValue = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSizeValue);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			CString str;
			str.Format(_T("失败:GXSetBuffer 功能名称:%s 错误码:%d"), chFeatureName, emStatus);
			m_LastErrorInfo.iErrorCode = DCErrorSetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("SetBuffer()函数"));
		}
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  发送命令码
\param  emFeatureID  功能码ID
\return            当接口调用成功，则返回GX_STATUS_SUCCESS，否则返回GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::SendCommand(GX_FEATURE_ID emFeatureID)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	if (IsOpen())
	{
		emStatus = GXSendCommand(m_hDevice, emFeatureID);
		char chFeatureName[64] = {0};
		size_t nSize = 64;
		GXGetFeatureName(m_hDevice, emFeatureID, chFeatureName, &nSize);
	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  发送开采命令后,此接口可以直接获取图像（注意此接口不能与回调采集方式混用）
\param  pFrameData  指向用户传入的用来接收图像数据的地址指针
\param  nTimeout    取图的超时时间
\return             当接口调用成功，则返回GX_STATUS_SUCCESS，否则返回GX_STATUS_ERROR
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::GetImage(GX_FRAME_DATA* pFrameData, int32_t nTimeout)
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
// 	if (IsOpen())
// 	{
// 		emStatus = GXGetImage(m_hDevice, pFrameData, nTimeout);
// 		if (emStatus != GX_STATUS_SUCCESS)
// 		{
// 			__PrintInfotoWnd(MsgComposer("失败:GXGetImage 错误码:%d", emStatus));
// 		}
// 		else
// 		{
// 			if (m_bEnableGetImageProcess)
// 			{
// 				//首先将回调中输出的图像数据，处理成RGB数据，以备后面的显示和存储
// 				__ProcessData((BYTE*)pFrameData->pImgBuf,m_pImgRGBBuffer, m_nImageWidth, m_nImageHeight);
// 				//在指定窗口上一帧一帧的画图
// 				__DrawImg(m_pImgRGBBuffer, m_nImageWidth, m_nImageHeight);
// 				//在指定存储目录下一帧一帧的存储bmp图
// 				__SaveBMP(m_pImgRGBBuffer, m_nImageWidth, m_nImageHeight);
// 			}
// 		}
// 		
// 	}
	return emStatus;
}

//----------------------------------------------------------------------------------
/**
\brief  使能GetImage接口的内部处理过程，包括图片信息的显示及保存
\param  bEnableGetImageProcess  true表示使能；false表示禁用
\return 无
*/
//----------------------------------------------------------------------------------
void CameraBase::EnableGetImageProcess(bool bEnableGetImageProcess)
{
	m_bEnableGetImageProcess = bEnableGetImageProcess;
}



//----------------------------------------------------------------------------------
/**
\brief  为图像显示准备资源
\return 成功返回GX_STATUS_SUCCESS;否则返回相应错误码，具体查询GX_STATUS_LIST
*/
//----------------------------------------------------------------------------------
GX_STATUS CameraBase::__PrepareForShowImg()
{      
	GX_STATUS emStatus = GX_STATUS_ERROR;
	//获取图象数据大小
	emStatus = GXGetInt(m_hDevice, GX_INT_PAYLOAD_SIZE, &m_nPayLoadSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		AfxMessageBox(_T("获取数据大小出错!"));

		return emStatus;
	}
	//获得图像格式
	emStatus = GXGetEnum(m_hDevice, GX_ENUM_PIXEL_FORMAT, &m_nPixelFormat);//?????????????????

	if (emStatus != GX_STATUS_SUCCESS)
	{
		AfxMessageBox(_T("获取像素格式出错!"));

		return emStatus;
	}
	switch(m_nPixelFormat)
	{
	case GX_PIXEL_FORMAT_MONO8:
	case GX_PIXEL_FORMAT_MONO10:
	case GX_PIXEL_FORMAT_MONO12:
	    m_nImageByteCount = 1 ;
		break;
	default:
		m_nImageByteCount = 1 ;
		break;
	}

	//查询当前相机是否支持GX_ENUM_PIXEL_COLOR_FILTER
	bool bIsImplemented = false;
	GXIsImplemented(m_hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &bIsImplemented);
	if(bIsImplemented)
	{
		emStatus = GXGetEnum(m_hDevice,GX_ENUM_PIXEL_COLOR_FILTER,&m_nPixelColorFilter);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			AfxMessageBox(_T("获取Bayer格式出错!"));

			return emStatus;
		}
	}

	//获取宽度
	emStatus = GXGetInt(m_hDevice, GX_INT_WIDTH, &m_nImageWidth);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		AfxMessageBox(_T("获取图像宽度出错!"));

		return emStatus;
	}
	//获取高度
	emStatus = GXGetInt(m_hDevice, GX_INT_HEIGHT, &m_nImageHeight);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		AfxMessageBox(_T("获取图像高度出错!"));

		return emStatus;
	}

	if ((m_nImageWidth != 0) && (m_nImageHeight != 0))
	{
		//为存储Raw8数据开辟空间
		m_pImgRaw8Buffer = new BYTE[size_t(m_nImageWidth*m_nImageHeight)];
		if (m_pImgRaw8Buffer == NULL)
		{
			return GX_STATUS_INVALID_HANDLE;
		}
		//为存储RGB数据开辟空间
		m_pImgRGBBuffer = new BYTE[size_t(m_nImageWidth*m_nImageHeight*3)];
		if (m_pImgRGBBuffer == NULL)
		{
			if (m_pImgRaw8Buffer != NULL)
			{
				delete[]m_pImgRaw8Buffer;
				m_pImgRaw8Buffer = NULL;
			}
			return GX_STATUS_INVALID_HANDLE;
		}
	}
	else
	{
		AfxMessageBox(_T("图像宽度或高度为0!"));

		return GX_STATUS_INVALID_HANDLE;
	}

	return emStatus;
}


//----------------------------------------------------------------------------------
/**
\brief  释放为图像显示准备资源
\return 无返回值
*/
//----------------------------------------------------------------------------------
void CameraBase::__UnPrepareForShowImg()
{
	if (m_pImgRGBBuffer != NULL)
	{
		delete []m_pImgRGBBuffer;
		m_pImgRGBBuffer = NULL;
	}
	if (m_pImgRaw8Buffer != NULL)
	{
		delete []m_pImgRaw8Buffer;
		m_pImgRaw8Buffer = NULL;
	}
}

//----------------------------------------------------------------------------------
/**
\brief  将相机输出的原始数据转换为RGB数据
\param  pImageBuf  指向图像缓冲区的指针
\param  pImageRGBBuf  指向RGB数据缓冲区的指针
\param  nImageWidth 图像宽
\param  nImageHeight 图像高
\return 无返回值
*/
//----------------------------------------------------------------------------------
void CameraBase::__ProcessData(BYTE * pImageBuf, BYTE * pImageRGBBuf, int64_t nImageWidth, int64_t nImageHeight)
{
	switch(m_nPixelFormat)
	{
		//当数据格式为12位时，位数转换为4-11
	case GX_PIXEL_FORMAT_BAYER_GR12:
	case GX_PIXEL_FORMAT_BAYER_RG12:
	case GX_PIXEL_FORMAT_BAYER_GB12:
	case GX_PIXEL_FORMAT_BAYER_BG12:
		//将12位格式的图像转换为8位格式
		DxRaw16toRaw8(pImageBuf, m_pImgRaw8Buffer, (VxUint32)nImageWidth, (VxUint32)nImageHeight, DX_BIT_4_11);	

		//将Raw8图像转换为RGB图像以供显示
		DxRaw8toRGB24(m_pImgRaw8Buffer, pImageRGBBuf,(VxUint32)nImageWidth, (VxUint32)nImageHeight, RAW2RGB_NEIGHBOUR,
		DX_PIXEL_COLOR_FILTER(m_nPixelColorFilter), TRUE);		        
		break;

		//当数据格式为12位时，位数转换为2-9
	case GX_PIXEL_FORMAT_BAYER_GR10:
	case GX_PIXEL_FORMAT_BAYER_RG10:
	case GX_PIXEL_FORMAT_BAYER_GB10:
	case GX_PIXEL_FORMAT_BAYER_BG10:
		////将12位格式的图像转换为8位格式,有效位数2-9
		DxRaw16toRaw8(pImageBuf, m_pImgRaw8Buffer, (VxUint32)nImageWidth, (VxUint32)nImageHeight, DX_BIT_2_9);

		//将Raw8图像转换为RGB图像以供显示
		DxRaw8toRGB24(m_pImgRaw8Buffer, pImageRGBBuf, (VxUint32)nImageWidth, (VxUint32)nImageHeight, RAW2RGB_NEIGHBOUR,
		DX_PIXEL_COLOR_FILTER(m_nPixelColorFilter), TRUE);	
		break;

	case GX_PIXEL_FORMAT_BAYER_GR8:
	case GX_PIXEL_FORMAT_BAYER_RG8:
	case GX_PIXEL_FORMAT_BAYER_GB8:
	case GX_PIXEL_FORMAT_BAYER_BG8:
		//将Raw8图像转换为RGB图像以供显示
		DxRaw8toRGB24(pImageBuf,pImageRGBBuf, (VxUint32)nImageWidth, (VxUint32)nImageHeight,RAW2RGB_NEIGHBOUR,
			DX_PIXEL_COLOR_FILTER(m_nPixelColorFilter),TRUE);	
		break;

	case GX_PIXEL_FORMAT_MONO12:
		//将12位格式的图像转换为8位格式
		DxRaw16toRaw8(pImageBuf, m_pImgRaw8Buffer, (VxUint32)nImageWidth, (VxUint32)nImageHeight, DX_BIT_4_11);	
		//将Raw8图像转换为RGB图像以供显示
		DxRaw8toRGB24(m_pImgRaw8Buffer, pImageRGBBuf, (VxUint32)nImageWidth, (VxUint32)nImageHeight, RAW2RGB_NEIGHBOUR,
		DX_PIXEL_COLOR_FILTER(NONE), TRUE);		        
		break;

	case GX_PIXEL_FORMAT_MONO10:
		//将10位格式的图像转换为8位格式
		DxRaw16toRaw8(pImageBuf, m_pImgRaw8Buffer, (VxUint32)nImageWidth, (VxUint32)nImageHeight, DX_BIT_4_11);	
		//将Raw8图像转换为RGB图像以供显示
		DxRaw8toRGB24(m_pImgRaw8Buffer,pImageRGBBuf, (VxUint32)nImageWidth, (VxUint32)nImageHeight,RAW2RGB_NEIGHBOUR,
			DX_PIXEL_COLOR_FILTER(NONE), TRUE);		        
		break;

	case GX_PIXEL_FORMAT_MONO8:
		//将Raw8图像转换为RGB图像以供显示
		DxRaw8toRGB24(pImageBuf,pImageRGBBuf, (VxUint32)nImageWidth, (VxUint32)nImageHeight,RAW2RGB_NEIGHBOUR,
			DX_PIXEL_COLOR_FILTER(NONE), TRUE);	


	default:
		break;
	}
}


//----------------------------------------------------------------------------------
/**
\brief  回调函数
\param  pFrame  指向采集回调的数据结构指针
\return 无返回值
*/
//----------------------------------------------------------------------------------
void __stdcall CameraBase::__OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame)
{
	CameraBase *pCamera = (CameraBase *)(pFrame->pUserParam);
	if (pFrame->status == 0)
	{
		BOOL bRet = FALSE;
		try
		{
			//首先将回调中输出的图像数据，处理成RGB数据，以备后面的显示和存储
		  //  pCamera->__ProcessData((BYTE*)pFrame->pImgBuf, pCamera->m_pImgRGBBuffer, pCamera->m_nImageWidth, pCamera->m_nImageHeight);
            // memcpy(pCamera->m_pImgRGBBuffer,(BYTE*)pFrame->pImgBuf,pCamera->m_nImageWidth*pCamera->m_nImageHeight);
			//将数据进行垂直方向镜像
			int i=0;
			
			//	break;for( i=0;i<pCamera->m_nImageHeight;i++)
			{
				//memcpy(pCamera->m_pImgRGBBuffer+pCamera->m_nImageWidth*i,(BYTE*)pFrame->pImgBuf+pCamera->m_nImageWidth*(pCamera->m_nImageHeight-i-1), pCamera->m_nImageWidth);
				memcpy(pCamera->m_pImgRGBBuffer,(BYTE*)pFrame->pImgBuf, pCamera->m_nImageWidth*pCamera->m_nImageHeight);
			}
			//pCamera->__SaveBMP(pCamera->m_pImgRGBBuffer, pCamera->m_nImageWidth, pCamera->m_nImageHeight);
		}
		catch (...)
		{
			bRet = TRUE;
			pCamera->m_LastErrorInfo.iErrorCode = DCErrorSendBufAdd;
			_tprintf_s(pCamera->m_LastErrorInfo.strErrorDescription, "传出图像内存地址失败");
			_tprintf_s(pCamera->m_LastErrorInfo.strErrorRemark, "__OnFrameCallbackFun()函数");
		}
		
		s_GBSIGNALINFO GrabInfo;
		GrabInfo.Context = pCamera->m_Context;
		GrabInfo.iGrabberTypeSN = pCamera->m_nGrabberType; // 采集卡类型编号
		GrabInfo.iGrabberSN  = pCamera->m_nGrabberSN - 1; // 相机序号从0开始,保证与其他相机一样
		GrabInfo.iErrorCode = GBOK;
		GrabInfo.iFrameCount = pFrame->nFrameID;
		GrabInfo.nTimestamp = pFrame->nTimestamp;
		if (bRet)
		{
			GrabInfo.iErrorCode = pCamera->m_LastErrorInfo.iErrorCode;
			_tprintf_s(GrabInfo.strDescription, pCamera->m_LastErrorInfo.strErrorDescription);
		}
    	pCamera->m_CallBackFunc(&GrabInfo);
	}
}
//----------------------------------------------------------------------------------
/**
\brief  保存bmp图像
\param  pImageBuf  指向图像缓冲区的指针
\param  nImageWidth 图像宽
\param  nImageHeight 图像高
\return 无返回值
*/
//----------------------------------------------------------------------------------
void CameraBase::__SaveBMP(BYTE * pImageBuf, int64_t nImageWidth, int64_t nImageHeight)
{
	if (!m_bSaveBMP || m_strBMPFolder.GetLength() == 0)
	{
		return;
	}
	
	BITMAPFILEHEADER	bfh	= {0};
	DWORD				dwBytesRead	= 0;
	DWORD               dwImageSize = DWORD(nImageWidth * nImageHeight * 3);
	
	bfh.bfType		= (WORD)'M' << 8 | 'B';			                        //定义文件类型
	bfh.bfOffBits	= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);	//定义文件头大小彩色
	bfh.bfSize		= bfh.bfOffBits + dwImageSize;		                    //文件大小
	
	SYSTEMTIME system;
	GetLocalTime(&system);
	CString strBMPFileName = MsgComposer(_T("\\%02d%02d%02d%03d.bmp"),
		system.wHour, 
		system.wMinute, 
		system.wSecond, 
		system.wMilliseconds);
	
	CString strBMPFilePath = m_strBMPFolder + strBMPFileName;
	
	HANDLE hFile = ::CreateFile((LPCTSTR)strBMPFilePath,
		GENERIC_WRITE ,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL
		);
	if (hFile != INVALID_HANDLE_VALUE) 
	{
		::WriteFile(hFile, &bfh, sizeof(BITMAPFILEHEADER), &dwBytesRead, NULL );
		::WriteFile(hFile, m_pBmpInfo, sizeof(BITMAPINFOHEADER), &dwBytesRead, NULL );
		::WriteFile(hFile, pImageBuf, dwImageSize, &dwBytesRead, NULL );				
		CloseHandle(hFile);
	}
}


//远端设备事件回调处理函数
void GX_STDC CameraBase::OnFeatureCallbackFun(GX_FEATURE_ID_CMD emFeatureID, void* pUserParam)
{
	CameraBase *pCamera = (CameraBase *)(pUserParam);
    if (emFeatureID == GX_INT_EVENT_FRAMESTART_OVERTRIGGER)
	{
		//获取触发被屏蔽事件的ID等事件数据	
		int64_t nFrameID=0;	
		GXGetInt(pCamera->m_hDevice, GX_INT_EVENT_FRAMESTART_OVERTRIGGER, &nFrameID);

		int64_t nTimestamp=0;
		GXGetInt(pCamera->m_hDevice, GX_INT_EVENT_FRAMESTART_OVERTRIGGER_TIMESTAMP, &nTimestamp);

		//s_GBEVENTINFO EventInfo;
		s_GBSIGNALINFO EventInfo;
		ZeroMemory(&EventInfo,sizeof(s_GBSIGNALINFO));

		EventInfo.Context = pCamera->m_Context;
		EventInfo.iGrabberTypeSN = pCamera->m_nGrabberType; // 采集卡类型编号
		EventInfo.iGrabberSN  = pCamera->m_nGrabberSN - 1; // 相机序号从0开始,保证与其他相机一样
		EventInfo.iErrorCode = GBOK;
		EventInfo.iFrameCount = nFrameID;
		EventInfo.nTimestamp = nTimestamp;

    	pCamera->m_EventCallbackFunc(&EventInfo);
	}
	return;    
}
