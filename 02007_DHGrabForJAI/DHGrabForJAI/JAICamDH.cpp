// JAICamDH.cpp: implementation of the CJAICamDH class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JAIGrabber.h"
#include "JAICamDH.h"
#include "DlgCameraSetting.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJAICamDH::CJAICamDH()
{
	// 相机宽高位数
	m_iImageWidth=1392;			//宽
	m_iImageHeight=1040;			//高
	m_iImageByteCount=1;		//位数
	m_iImageBufferSize=1392*1040*3;		//内存大小
	m_pImageBufferAdd=NULL;	//内存地址
	m_iGain = 0;
	m_iExposureTime = 0;
	m_iMinGain = 0;
	m_iMaxGain = 0;
	m_iMinExp = 0;
	m_iMaxExp = 0;
	m_iTriggerMode = 0;

	
	// 相机基本信息
	m_iGrabberSN=0;			//相机序号
	m_iGrabberTypeSN=0;		//动态库类型号
	m_sInitFile="";			//初始化文件名称
	m_sDeviceName="";		//设备名称
	m_sDeviceMark="";		//设备标识
	m_pContext=NULL;		//存放调用初始化函数的this对象指针
	
	m_iCamCount=0;			//相机个数
	m_iFrameCount=0;
	m_bInitSucceed=FALSE;	//是否初始化成功
	m_bCameraActive=FALSE;	//初始化没有采集
	m_hGainNode = NULL;
	m_hExposureNode = NULL;

	m_pImageInfo = NULL;
	m_pImageInfo = (J_tIMAGE_INFO*)new BYTE[sizeof(J_tIMAGE_INFO)];
	m_pImageBufferAdd = new BYTE[1500*1500*3];

}

CJAICamDH::~CJAICamDH()
{
	if(m_pImageInfo!=NULL)
	{
		delete m_pImageInfo;
		m_pImageInfo = NULL;
	}

	if(m_pImageBufferAdd!=NULL)
	{
		delete m_pImageBufferAdd;
		m_pImageBufferAdd = NULL;
	}
}

//初始化
BOOL CJAICamDH::Init(const s_GBINITSTRUCT* pInitParam)
{
	//相机初始化
	s_JAI_INITSTRUCT InitCardParam;
	InitCardParam.iGrabberTypeSN = pInitParam->iGrabberTypeSN;  // 采集卡类型编号
	InitCardParam.nGrabberSN = pInitParam->nGrabberSN;          // 采集卡序号
	sprintf(InitCardParam.strDeviceName, pInitParam->strDeviceName);  // 设备名称
	sprintf(InitCardParam.strDeviceMark, pInitParam->strDeviceMark); // 设备标识
	sprintf(InitCardParam.strGrabberFile, pInitParam->strGrabberFile); // 用于初始化的文件
	InitCardParam.CallBackFunc = pInitParam->CallBackFunc;
	InitCardParam.Context = pInitParam->Context;
	
	// 
	m_iGrabberSN = pInitParam->nGrabberSN;
	m_iGrabberTypeSN = pInitParam->iGrabberTypeSN;
	m_sDeviceMark.Format("%s",pInitParam->strDeviceMark);
	m_sDeviceName.Format("%s",pInitParam->strDeviceName);
	m_sInitFile.Format("%s",pInitParam->strGrabberFile);
	m_pContext = pInitParam->Context;
	m_CallBackFunc = pInitParam->CallBackFunc;		
	
	// 寻找配置文件
	CFileFind find;
	if(FALSE==find.FindFile(m_sInitFile))
	{
		m_bInitSucceed = FALSE;
		m_LastErrorInfo.nErrorCode = ErrorInit; 
		sprintf(m_LastErrorInfo.strErrorDescription, "相机配置文件不在指定路径中");
		sprintf(m_LastErrorInfo.strErrorRemark, "Init()函数");
		return FALSE;
	}


	if(FALSE==InitFactory())
	{
		return FALSE;
	}


	// 若设备SN大于相机个数则推出
	if(pInitParam->nGrabberSN>=m_iCamCount)
	{
		m_bInitSucceed = FALSE;
		m_LastErrorInfo.nErrorCode = ErrorInit; 
		sprintf(m_LastErrorInfo.strErrorDescription, "相机编号大于实际相机个数");
		sprintf(m_LastErrorInfo.strErrorRemark, "Init()函数");
		return FALSE;
	}
	else if(pInitParam->nGrabberSN<0)
	{
		m_bInitSucceed = FALSE;
		m_LastErrorInfo.nErrorCode = ErrorInit; 
		sprintf(m_LastErrorInfo.strErrorDescription, "相机编号不能小于0");
		sprintf(m_LastErrorInfo.strErrorRemark, "Init()函数");
		return FALSE;
	}

	// 初始化相机
	InitCamera();

	m_bInitSucceed = TRUE;

	return TRUE;
}

//关闭
BOOL CJAICamDH::Close()
{

	J_STATUS_TYPE retval;

	if (m_bInitSucceed == TRUE)// 如果初始化成功
	{		
		if (m_bCameraActive) // 如果处于采集状态
		{
			StopGrab();
		}	
		
		if(NULL!=m_hThread)
		{
			retval = J_Image_CloseStream(m_hThread);
			m_hThread = NULL;
		}

		if(J_ST_SUCCESS!=retval)
		{
			m_LastErrorInfo.nErrorCode = ErrorClose;  // 加入出错描述 
			sprintf(m_LastErrorInfo.strErrorDescription, "Close Stream Failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "Close()函数");
			return FALSE;
		}

	}

	if (m_hCam)
	{
		// Close camera
		retval = J_Camera_Close(m_hCam);
		if(J_ST_SUCCESS!=retval)
		{
			m_LastErrorInfo.nErrorCode = ErrorClose;  // 加入出错描述 
			sprintf(m_LastErrorInfo.strErrorDescription, "Close Camera Failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "Close()函数");
			return FALSE;
		}

		m_hCam = NULL;
		TRACE("Closed camera \n");
    }

	if (m_hFactory)
    {
        // Close factory
        retval=J_Factory_Close(m_hFactory);
		if(J_ST_SUCCESS!=retval)
		{
			m_LastErrorInfo.nErrorCode = ErrorClose;  // 加入出错描述 
			sprintf(m_LastErrorInfo.strErrorDescription, "Close factory Failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "Close()函数");
			return FALSE;
		}

        m_hFactory = NULL;
        TRACE("Closed factory\n");
    }

	return TRUE;
}

//开始采集
BOOL CJAICamDH::StartGrab()
{
	if(FALSE==m_bInitSucceed || NULL==m_hCam)
	{
		m_LastErrorInfo.nErrorCode = ErrorStartGrab;  // 加入出错描述 
		sprintf(m_LastErrorInfo.strErrorDescription, "相机没有初始化");
		sprintf(m_LastErrorInfo.strErrorRemark, "StartGrab()函数");
		return FALSE;
	}

	if (m_bCameraActive) //如果相机正在采集，就停止采集
	{
		StopGrab();
	}
	
	if (m_bCameraActive) //是否正在采集
	{
		m_LastErrorInfo.nErrorCode = ErrorStartGrab;  // 加入出错描述 
		sprintf(m_LastErrorInfo.strErrorDescription, "相机已经在采集");
		sprintf(m_LastErrorInfo.strErrorRemark, "StartGrab()函数");
		return FALSE;
	}


	// Start Acquision
	if(FALSE==m_bCameraActive)
	{
		J_STATUS_TYPE retval;
		retval = J_Camera_ExecuteCommand(m_hCam, NODE_NAME_ACQSTART);

		if(J_ST_SUCCESS!=retval)
		{
			m_LastErrorInfo.nErrorCode = ErrorStartGrab;  // 加入出错描述 
			sprintf(m_LastErrorInfo.strErrorDescription, "start grab failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "StartGrab()函数");
			return FALSE;
		}
		else
		{
			m_bCameraActive = TRUE;
		}
	}


	return TRUE;
}

//停止采集
BOOL CJAICamDH::StopGrab()
{

	if(!m_bInitSucceed || NULL==m_hCam)
	{
		m_LastErrorInfo.nErrorCode = ErrorStopGrab;  // 加入出错描述 
		sprintf(m_LastErrorInfo.strErrorDescription, "相机没有初始化");
		sprintf(m_LastErrorInfo.strErrorRemark, "StopGrab()函数");
		return FALSE;
	}

	if (!m_bCameraActive)// 是否启动采集
	{
		m_LastErrorInfo.nErrorCode = ErrorStopGrab;  // 加入出错描述 
		sprintf(m_LastErrorInfo.strErrorDescription, "相机尚未处于采集状态");
		sprintf(m_LastErrorInfo.strErrorRemark, "StopGrab()函数");
		return FALSE;
	}


	J_STATUS_TYPE retval;
	retval = J_Camera_ExecuteCommand(m_hCam, NODE_NAME_ACQSTOP);

	if(J_ST_SUCCESS!=retval)
	{
		m_LastErrorInfo.nErrorCode = ErrorStopGrab;  // 加入出错描述 
		sprintf(m_LastErrorInfo.strErrorDescription, "stop grab failed!");
		sprintf(m_LastErrorInfo.strErrorRemark, "StopGrab()函数");
		return FALSE;
	}
	else
	{
		m_bCameraActive = FALSE;
	}

	return TRUE;
}
	
//得到参数
BOOL CJAICamDH::GetParamInt(GBParamID Param, int &nOutputVal)
{
	J_STATUS_TYPE retval;

	switch(Param)
	{
	case 0://GBImageWidth:
		nOutputVal = m_iImageWidth;
		break;
	case 1://GBImageHeight:
		nOutputVal = m_iImageHeight;
		break;
	case 2://GBImagePixelSize:
		nOutputVal = m_iImageByteCount;
		break;
	case 3://GBImageBufferSize:
		nOutputVal = m_iImageWidth * m_iImageHeight * m_iImageByteCount;
		break;
	case 4://GBImageBufferAddr:
		nOutputVal = (int)m_pImageBufferAdd;
		break;
	case GBIsColorFilter://GBColor,					//相机类型  黑白和彩色 
		nOutputVal = 1; // 是否彩色相机 
		break;
	case GBExposureTime://GBBrightness,				//相机亮度大小
		UINT32 OutputVal;
		OutputVal = 0;		
		break;
	case GBShutter://GBShutter,				//相机快门值
		// Get ExposureTimeRaw Node
		retval = J_Camera_GetNodeByName(m_hCam, NODE_NAME_EXPOSURE, &m_hExposureNode);
		if (retval != J_ST_SUCCESS) 
		{
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription,"GetParamInt(shutter)失败failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "GetParamInt()函数");
			return FALSE;
		}
		else
		{
			int64_t int64Val;
			retval = J_Node_GetValueInt64(m_hExposureNode, TRUE, &int64Val);
			m_iExposureTime  = (int)int64Val;
			nOutputVal = (int)int64Val;
		}
		break;
	case GBGain://GBGain,					//相机增益大小
		// Get Gain Node
		retval = J_Camera_GetNodeByName(m_hCam, NODE_NAME_GAIN, &m_hGainNode);
		if (retval != J_ST_SUCCESS) 
		{
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription,"Get gain failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "GetParamInt()函数");
			return FALSE;
		}
		else
		{
			// Get/Set Value
			int64_t int64Val;
			retval = J_Node_GetValueInt64(m_hGainNode, TRUE, &int64Val);
			m_iGain  = (int)int64Val;
			nOutputVal = (int)int64Val;
		}

		break;

		//////////////////////////////////////////////////////////////////////////
	default:
		m_LastErrorInfo.nErrorCode = ErrorGetParam;
		sprintf(m_LastErrorInfo.strErrorDescription, "得到参数出错");
		sprintf(m_LastErrorInfo.strErrorRemark, "GetParamInt()函数");

		return FALSE;
	}


	return TRUE;
}
	
//设置参数
BOOL CJAICamDH::SetParamInt(GBParamID Param, int nInputVal)
{

	J_STATUS_TYPE retval;

	switch(Param)
	{
	case GBShutter://GBShutter,				//相机快门值
		// Get ExposureTimeRaw Node
		retval = J_Camera_GetNodeByName(m_hCam, NODE_NAME_EXPOSURE, &m_hExposureNode);
		if (retval != J_ST_SUCCESS) 
		{
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription,"SetParamInt(shutter) failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "SetParamInt()函数");
			return FALSE;
		}
		else
		{
			retval = J_Node_SetValueInt64(m_hExposureNode, TRUE, nInputVal);
			if (retval != J_ST_SUCCESS) 
			{
				m_bInitSucceed = FALSE;
				m_LastErrorInfo.nErrorCode = ErrorInit; 
				sprintf(m_LastErrorInfo.strErrorDescription,"SetParamInt(shutter)2 failed!");
				sprintf(m_LastErrorInfo.strErrorRemark, "SetParamInt()函数");
				return FALSE;
			}

			m_iExposureTime = nInputVal;
		}
		break;
	case GBGain://GBGain,					//相机增益大小
		retval = J_Camera_GetNodeByName(m_hCam, NODE_NAME_GAIN, &m_hGainNode);
		if (retval != J_ST_SUCCESS) 
		{
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription,"Get gain failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "SetParamInt()函数");
			return FALSE;
		}
		else
		{
			// Get/Set Value
			retval = J_Node_SetValueInt64(m_hGainNode, TRUE, nInputVal);

			if (retval != J_ST_SUCCESS) 
			{
				m_bInitSucceed = FALSE;
				m_LastErrorInfo.nErrorCode = ErrorInit; 
				sprintf(m_LastErrorInfo.strErrorDescription,"Get gain2 failed!");
				sprintf(m_LastErrorInfo.strErrorRemark, "SetParamInt()函数");
				return FALSE;
			}
			m_iGain = nInputVal;

		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}


//得到参数
BOOL CJAICamDH::GetParamfloat(int Param, float &nOutputVal)
{
	return TRUE;
}

//设置参数
BOOL CJAICamDH::SetParamfloat(int Param, float nInputVal)
{

	return TRUE;
}

class tempRoutingFrame 
{
	CFrameWnd* m_pFrame;
	
public:
	
	tempRoutingFrame(CFrameWnd * pWnd= NULL)
	{
		// Save current value
		m_pFrame = AfxGetThreadState()->m_pRoutingFrame;
		// Set to value passed in. NULL by default.
		AfxGetThreadState()->m_pRoutingFrame = pWnd;
	}
	~tempRoutingFrame()
	{
		// Restore m_pRoutingFrame to original value.
		AfxGetThreadState()->m_pRoutingFrame = m_pFrame;
	}
};

//调用参数对话框
void CJAICamDH::CallParamDialog()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#ifdef _DEBUG
	// Workaround for ASSERT in WINCORE.CPP 884 (CWnd::AssertValid)
	tempRoutingFrame rframe;
#endif

	CDlgCameraSetting dlg;
	dlg.m_iMaxExp = m_iMaxExp;
	dlg.m_iMinExp = m_iMinExp;
	dlg.m_iMaxGain = m_iMaxGain;
	dlg.m_iMinGain = m_iMinGain;
	dlg.m_iExposureTime = m_iExposureTime;
	dlg.m_iGain = m_iGain;
	dlg.SetOwner(this,m_sInitFile);
	int iRet = dlg.DoModal();

	delete dlg;
}

//得到出错信息
void CJAICamDH::GetLastErrorInfo(s_GBERRORINFO *pErrorInfo)
{
	if (pErrorInfo != NULL)
	{
		memcpy(pErrorInfo,&m_LastErrorInfo,sizeof(s_GBERRORINFO));
	}	
}	

BOOL CJAICamDH::GetTriggerParam(int &ParamValOutput)
{
	BOOL bRet = FALSE;
	// 请添加相应函数
	return bRet;
}

BOOL CJAICamDH::SetTriggerParam(int ParamValInput)
{
	BOOL bRet = FALSE;
	return bRet;
}
BOOL CJAICamDH::GetSystemInfo(CString &sOutputInfo)
{
	BOOL bRet = FALSE;
	return bRet;
}

BOOL CJAICamDH::SetAOI(RECT rtAOI)
{
	BOOL bRet = FALSE;
	return bRet;
}

BOOL CJAICamDH::GetAOI(RECT &rtAOI)
{
	BOOL bRet = FALSE;
	return bRet;
}


BOOL CJAICamDH::Reset()
{
	BOOL bRet = FALSE;
	return bRet;
}

BOOL CJAICamDH::CreateDefaultConfigurationFile(CString &sInitFile)
{
	BOOL bRet = FALSE;
	return bRet;
}

BOOL CJAICamDH::SaveToConfigurationFile(CString &sInitFile)
{
	BOOL bRet = FALSE;

	return bRet;
}
//////////////////////////////////////////////////////////////////////////
// DirectShow专有接口 
// 单帧采集
BOOL CJAICamDH::Snapshot()
{
	return TRUE;
}

BOOL CJAICamDH::InitFactory()
{
	J_STATUS_TYPE   retval;
    uint32_t        iSize;
    uint32_t        iNumDev;
	bool8_t         bHasChange;


	// Open factory
    retval = J_Factory_Open("" , &m_hFactory);
    if (retval != J_ST_SUCCESS)
    {
		m_bInitSucceed = FALSE;
		m_LastErrorInfo.nErrorCode = ErrorInit; 
		sprintf(m_LastErrorInfo.strErrorDescription, "Open factory failed!");
		sprintf(m_LastErrorInfo.strErrorRemark, "Init()函数");
		return FALSE;
    }
    TRACE("Opening factory succeeded\n");
	
 
	
	// Update camera list
    retval = J_Factory_UpdateCameraList(m_hFactory, &bHasChange);
    if (retval != J_ST_SUCCESS)
    {
		m_bInitSucceed = FALSE;
		m_LastErrorInfo.nErrorCode = ErrorInit; 
		sprintf(m_LastErrorInfo.strErrorDescription, "Update Camera list failed!");
		sprintf(m_LastErrorInfo.strErrorRemark, "Init()函数");
		return FALSE;
    }
    TRACE("Updating camera list succeeded\n");
	
    // Get the number of Cameras
    retval = J_Factory_GetNumOfCameras(m_hFactory, &iNumDev);
    if (retval != J_ST_SUCCESS)
    {
		m_bInitSucceed = FALSE;
		m_LastErrorInfo.nErrorCode = ErrorInit; 
		sprintf(m_LastErrorInfo.strErrorDescription, "Get Number of Cameras failed!");
		sprintf(m_LastErrorInfo.strErrorRemark, "Init()函数");
		return FALSE;
    }
	
    if (iNumDev == 0)
    {
		m_bInitSucceed = FALSE;
		m_LastErrorInfo.nErrorCode = ErrorInit; 
		sprintf(m_LastErrorInfo.strErrorDescription, "no camera was found!");
		sprintf(m_LastErrorInfo.strErrorRemark, "Init()函数");
		return FALSE;
    }
	else
	{
		m_iCamCount = iNumDev;
	}
	
    TRACE("%d cameras were found\n", iNumDev);
	
	CString str1;
	str1.Format("相机%d 开始，相机个数:%d",m_iGrabberSN, m_iCamCount);
// 	AfxMessageBox(str1);

	
	// Get camera IDs
	iSize = J_CAMERA_ID_SIZE;


	BOOL bFind = FALSE;

	for(int i=0;i<m_iCamCount;i++)
	{
		m_sCameraID[0]=0;
		iSize = J_CAMERA_ID_SIZE;

		retval = J_Factory_GetCameraIDByIndex(m_hFactory, i, m_sCameraID, &iSize);
		if (retval != J_ST_SUCCESS)
		{
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription, "Could not get the camera ID!");
			sprintf(m_LastErrorInfo.strErrorRemark, "Init()函数");		
			return FALSE;
		}
		TRACE("Camera ID: %s\n", m_sCameraID);
		
		// 验证sn号
		int8_t sSerialNumber[J_CAMERA_INFO_SIZE];
		J_Factory_GetCameraInfo(m_hFactory,m_sCameraID,CAM_INFO_SERIALNUMBER,sSerialNumber,&iSize);
		
		CString str;
		str.Format("673537%s0609",sSerialNumber);
		str1.Format("相机%d 编号:%d, 序列号:%s",m_iGrabberSN, i+1, sSerialNumber);
// 		AfxMessageBox(str1);
		
		if(0==str.Compare(m_sDeviceName))
		{
			bFind = TRUE;
			break;
		}
	}

	str1.Format("相机%d结束，序列号为:%s", m_iGrabberSN, m_sDeviceName);
// 	AfxMessageBox(str1);

	if(TRUE!=bFind)
	{
		m_bInitSucceed = FALSE;
		m_LastErrorInfo.nErrorCode = ErrorInit; 
		sprintf(m_LastErrorInfo.strErrorDescription, "DeviceName is not correct!");
		sprintf(m_LastErrorInfo.strErrorRemark, "Init()函数");
		return FALSE;
	}

	return TRUE;
}

BOOL CJAICamDH::InitCamera()
{

	J_STATUS_TYPE   retval;


	// Open camera
	retval = J_Camera_Open(m_hFactory, m_sCameraID, &m_hCam);
	if (retval != J_ST_SUCCESS)
	{
		m_bInitSucceed = FALSE;
		m_LastErrorInfo.nErrorCode = ErrorInit; 
		sprintf(m_LastErrorInfo.strErrorDescription, "Open camera failed!");
		sprintf(m_LastErrorInfo.strErrorRemark, "Init()函数");
		return FALSE;
	}


	// Load camera configuration files
	retval = J_Camera_LoadSettings(m_hCam,m_sInitFile);
//	retval = J_Camera_LoadSettings(m_hCam,m_sInitFile,LOAD_FORCE_WRITE);
// 	retval = J_Camera_LoadSettings(m_hCam,m_sInitFile,LOAD_VALIDATE_ONLY);

	if(retval==J_ST_VALIDATION_ERROR || retval==J_ST_VALIDATION_WARNING)
	{
		uint32_t BufferSize = 0;
		retval = J_Camera_GetSettingsValidationErrorInfo(m_hCam,NULL,&BufferSize);

		if(retval==J_ST_SUCCESS)
		{
			// Allocate enough room for the info string!
			int8_t *buffer = (int8_t *)malloc(BufferSize);
			// And now we get the actual erro information
			retval = J_Camera_GetSettingsValidationErrorInfo(m_hCam, buffer, &BufferSize);
			OutputDebugStringA(buffer);

			CString str;
			str.Format("%s",buffer);
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription,str);
			sprintf(m_LastErrorInfo.strErrorRemark, "Init()函数");

			// Remember to free the buffer again
			free(buffer);

			return FALSE;
		}

		m_bInitSucceed = FALSE;
		m_LastErrorInfo.nErrorCode = ErrorInit; 
		sprintf(m_LastErrorInfo.strErrorDescription,"Load setting failed!");
		sprintf(m_LastErrorInfo.strErrorRemark, "Init()函数");
		return FALSE;

	}

	SIZE	ViewSize;
	int     bpp = 0;
    int64_t int64Val;
    int64_t pixelFormat;

	// 
	if(m_hCam)
	{
		// Get Width from the camera
		
		int8_t x1[255];
		uint32_t x2;
		retval = J_Camera_GetValueString(m_hCam,"PartialScan",x1,&x2);
		retval = J_Camera_GetValueInt64(m_hCam, NODE_NAME_WIDTH, &int64Val);
		if (retval != J_ST_SUCCESS) 
		{
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription,"Get Width Failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "Init()函数");
			return FALSE;
		}
		else
		{
			ViewSize.cx = (LONG)int64Val;     // Set window size cx
			m_iImageWidth = (int)int64Val;
		}

		
		// Get Height from the camera
		retval = J_Camera_GetValueInt64(m_hCam, NODE_NAME_HEIGHT, &int64Val);
		if (retval != J_ST_SUCCESS) 
		{
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription,"Get height failed");
			sprintf(m_LastErrorInfo.strErrorRemark, "Init()函数");
			return FALSE;
		}
		else
		{
			ViewSize.cy = (LONG)int64Val;     // Set window size cy
			m_iImageHeight = (int)int64Val;
		}
		
		// Get pixelformat from the camera
		retval = J_Camera_GetValueInt64(m_hCam, NODE_NAME_PIXELFORMAT, &int64Val);
		if (retval != J_ST_SUCCESS) 
		{
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription,"Get pixformat failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "Init()函数");
			return FALSE;
		}
		else
		{
			pixelFormat = int64Val;
			m_iImageByteCount = 1;
		}

		m_iImageBufferSize = m_iImageWidth*m_iImageHeight*m_iImageByteCount;
		
		// Calculate number of bits (not bytes) per pixel using macro
		bpp = J_BitsPerPixel(pixelFormat);

		// Get Gain Node
		retval = J_Camera_GetNodeByName(m_hCam, NODE_NAME_GAIN, &m_hGainNode);
		if (retval != J_ST_SUCCESS) 
		{
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription,"Get gain failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "Init()函数");
			return FALSE;
		}
		else
		{
			// Get/Set Value
			retval = J_Node_GetValueInt64(m_hGainNode, TRUE, &int64Val);
			m_iGain  = (int)int64Val;

			retval = J_Node_GetMinInt64(m_hGainNode, &int64Val);
			m_iMinGain = (int)int64Val;

			retval = J_Node_GetMaxInt64(m_hGainNode, &int64Val);
			m_iMaxGain = (int)int64Val;


		}

		// Get ExposureTimeRaw Node
		retval = J_Camera_GetNodeByName(m_hCam, NODE_NAME_EXPOSURE, &m_hExposureNode);
		if (retval != J_ST_SUCCESS) 
		{
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription,"Get exposure time failed!");
			sprintf(m_LastErrorInfo.strErrorRemark, "Init()函数");
			return FALSE;
		}
		else
		{
			// Get/Set Value
			retval = J_Node_GetValueInt64(m_hExposureNode, TRUE, &int64Val);
			m_iExposureTime  = (int)int64Val;

			retval = J_Node_GetMinInt64(m_hExposureNode, &int64Val);
			m_iMinExp = (int)int64Val;

			retval = J_Node_GetMaxInt64(m_hExposureNode, &int64Val);
			m_iMaxExp = (int)int64Val;
		}


		retval = J_Image_OpenStream(m_hCam, 0, reinterpret_cast<J_IMG_CALLBACK_OBJECT>(this), reinterpret_cast<J_IMG_CALLBACK_FUNCTION>(&CJAICamDH::StreamCBFunc1), &m_hThread, (ViewSize.cx*ViewSize.cy*bpp)/8);
		if (retval != J_ST_SUCCESS) 
		{
			m_bInitSucceed = FALSE;
			m_LastErrorInfo.nErrorCode = ErrorInit; 
			sprintf(m_LastErrorInfo.strErrorDescription,"Could not open stream!");
			sprintf(m_LastErrorInfo.strErrorRemark, "Init()函数");
			return FALSE;
		}
		TRACE("Opening stream succeeded\n");

	}

	return TRUE;
}

void CJAICamDH::StreamCBFunc1(J_tIMAGE_INFO * pAqImageInfo)
{
	if(m_hCam)
	{
		m_iFrameCount++;
		s_GBSIGNALINFO info;
		info.nGrabberSN = m_iGrabberSN;
		info.iGrabberTypeSN = m_iGrabberTypeSN;
		info.Context = m_pContext;
		info.nFrameCount = m_iFrameCount;
		info.nErrorCode = 0;
		info.iReserve1 = 0;
		info.iReserve2 = 0;
		sprintf(info.strDescription,"正常");

		memcpy(m_pImageBufferAdd,pAqImageInfo->pImageBuffer,m_iImageWidth*m_iImageHeight);
		
		m_CallBackFunc(&info);
	}
}


