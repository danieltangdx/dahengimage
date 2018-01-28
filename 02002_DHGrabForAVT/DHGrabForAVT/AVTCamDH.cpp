// AVTCamDH.cpp: implementation of the CAVTCamDH class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AVTGrabber.h"
#include "AVTCamDH.h"
#include "SetCameraParam.h"
#include <afxtempl.h>		//为了使用CList

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//FILE *AVTfile1 = NULL;
//FILE *AVTfile2 = NULL;

void WINAPI FGCallBackFunc(void* Context,UINT32 wParam,void* lParam)
{
	CAVTCamDH* pAVT = (CAVTCamDH*)(lParam); 
	switch(wParam) 
	{
		case WPARAM_NODELISTCHANGED: 
	//	pAVT->OnNodelistChanged();
		break;
		case WPARAM_ERROR:
	//	pAVT->OnError(0);
		break;
		case WPARAM_ERRORFLAGSCARD0:
	//	pAVT->OnError(1);
	/*		if (AVTfile1==NULL)
			{
				AVTfile1 = fopen( _T("D:\\AVT1.txt"), _T("ab") );
				SYSTEMTIME SystemTime;
				GetLocalTime(&SystemTime);
				CString STime;
				STime.Format(_T("%ld_%ld_%ld_%ld_%ld_%ld"),
					SystemTime.wYear,
					SystemTime.wMonth,
					SystemTime.wDay,
					SystemTime.wHour,
					SystemTime.wMinute,
					SystemTime.wSecond
					);
				fprintf(AVTfile1, _T("%s__WPARAM_ERRORFLAGSCARD0,\r\n"),STime);
				fclose(AVTfile1); 
				AVTfile1 = NULL;
			}*/

		break;
		case WPARAM_ERRORFLAGSCARD1:
		//pAVT->OnError(2);
			//if (AVTfile2==NULL)
			//{
			//	AVTfile2 = fopen( _T("D:\\AVT2.txt"), _T("ab") );
			//	SYSTEMTIME SystemTime;
			//	GetLocalTime(&SystemTime);
			//	CString STime;
			//	STime.Format(_T("%ld_%ld_%ld_%ld_%ld_%ld"),
			//		SystemTime.wYear,
			//		SystemTime.wMonth,
			//		SystemTime.wDay,
			//		SystemTime.wHour,
			//		SystemTime.wMinute,
			//		SystemTime.wSecond
			//		);
			//	fprintf(AVTfile2, _T("%s__WPARAM_ERRORFLAGSCARD0,\r\n"),STime);
			//	fclose(AVTfile2); 
			//	AVTfile2 = NULL;
			//}
		break;
		case WPARAM_ERRORFLAGSCARD2:
		//pAVT->OnError(3);
		break;
		case WPARAM_ERRORFLAGSCARD3:
		//pAVT->OnError(4);
		break;
		case WPARAM_ERRORFLAGSCARD4:
		//pAVT->OnError(5);
		break;
		case WPARAM_ERRORFLAGSCARD5:
		//pAVT->OnError(6);
		break;
		case WPARAM_ERRORFLAGSCARD6:
		//pAVT->OnError(7);
		break;
		case WPARAM_ERRORFLAGSCARD7:
		//pAVT->OnError(8);
		break;
		case WPARAM_ERRORFLAGSCARD8:
		//pAVT->OnError(9);
		break;
		case WPARAM_ERRORFLAGSCARD9:
		//pAVT->OnError(10);
		break;
		case WPARAM_BUSRESET:
		//	pAVT->OnError(11);
		break;

    case WPARAM_FRAMESREADY:
		pAVT->OnNewFrame();
		break;
	}	
}

void CAVTCamDH::OnNodelistChanged()
{
//	AfxMessageBox(_T("OnNodelistChanged"));
//	if (m_bSet4)
//	{
//		m_bSet4 = FALSE;
//		m_NodelistChangedEvent.ResetEvent();
//		m_NodelistChangedEvent.SetEvent();
//	}
}

void CAVTCamDH::OnError(int nError)
{
//	CString str;
//	str.Format(_T("Error:%d"),nError);
//	AfxMessageBox(str);
//	BOOL bRet = FALSE;
//	Reset();
	//Sleep(100);
	//s_GBSIGNALINFO GrabInfo;
	//GrabInfo.Context	 = m_Context;
	//GrabInfo.nGrabberSN  = m_nGrabberSN;
	//GrabInfo.iGrabberTypeSN  = m_nGrabberTypeSN; // 加入采集卡编号
	//GrabInfo.nFrameCount = 0;
	//GrabInfo.nErrorCode  = 1;
	//GrabInfo.iReserve1 = nError;
	//GrabInfo.iReserve2 = 0;
	//_tprintf_s(GrabInfo.strDescription, _T("设备错误"));
	//m_CallBackFunc(&GrabInfo);//回调函数
}

//回调函数中调用的采集函数，此函数中包含给出外界的回调函数指针
void CAVTCamDH::OnNewFrame()
{
	FGFRAME   Frame;
	UINT32    Result=FCE_NOERROR;
	UINT32    FramesToDisplay;
	
	// If a message was pending when someone stopped the camera we don't process
	m_Camera.GetParameter(FGP_FRAMEBUFFERCOUNT,&FramesToDisplay);
	FramesToDisplay=min(16,FramesToDisplay);
	do
	{
		Result = m_Camera.GetFrame(&Frame,0);//INFINITE);		// Try to read a frame 读取一帧图像
		//Result = m_Camera.GetFrame(&Frame,0); //  等待一帧的时间0ms
		if(Result!=FCE_NOERROR)
		{ 
			TRACE(_T(".........Result=%d.........\n"), Result);

			break;
		}
		
		// Only if <pData> is valid (ignore pure frame start events)
		if(Frame.pData) //读取数据有效
		{
			m_FrameCnt++;  //采集帧数递增

	//		if (!m_bTestFramerate)
	//		{
				if(m_pbImageBufferAdd == NULL)
				{
					m_pbImageBufferAdd = new BYTE[Frame.Length];
				}
			
				memcpy(m_pbImageBufferAdd, Frame.pData,Frame.Length);
				Result = m_Camera.PutFrame(&Frame);
				if(Result!=FCE_NOERROR)
				{ 
					m_LastErrorInfo.iErrorCode = ErrorStartGrab;
					_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("返回图像帧"));
					_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("PutFrame()函数"));
					break;
				}

				FramesToDisplay--;
				s_GBSIGNALINFO GrabInfo;
				GrabInfo.Context	 = m_Context;
				GrabInfo.iGrabberSN  = m_nGrabberSN;
				GrabInfo.iGrabberTypeSN  = m_nGrabberTypeSN; // 加入采集卡编号
				GrabInfo.iFrameCount = m_FrameCnt;
				GrabInfo.iErrorCode  = 0;
				GrabInfo.iReserve1 = 0;
				GrabInfo.iReserve2 = 0;
				_tprintf_s(GrabInfo.strDescription, _T("正常"));
			
				m_CallBackFunc(&GrabInfo);//回调函数
	//		}
		}
		else
		{
			 m_Camera.PutFrame(&Frame);
		}

	}while(!(Frame.Flags&FGF_LAST));
}

BOOL CAVTCamDH::Init_Grabber_Module()
{
	UINT32     Result;
	FGINIT     Arg;
	
	memset(&Arg,0,sizeof(FGINIT));	
	// 初始化模块
	Arg.pCallback = FGCallBackFunc;
	
	Result=FGInitModule(&Arg);
	if(Result==FCE_NOERROR)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

BOOL CAVTCamDH::Exit_Grabber_Module()
{
	// Exit our frame grabber module
	FGExitModule();
	
	return TRUE;
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAVTCamDH::CAVTCamDH()
{
	m_nGaidHigh		= 0;
	m_nGaidLow		= 0;
	m_InitSucceed	= FALSE;
	m_CameraActive	= FALSE;
	m_pCameraInfo	= NULL;	

	m_nGrabberSN = 0;				// 序号
	m_nGrabberTypeSN = 0;           // 采集卡类型号 
	m_sInitFile = _T("");			// 用于初始化的文件
	m_sDeviceName = _T("");			// 设备名
	m_sDeviceMark = _T("");         // 设备标识  
	m_Context = NULL;				// 存放调用初始化函数的对象的this指针

	m_nImageWidth = 656;          //图像宽度  
	m_nImageHeight = 492;         //图像高度
	m_nImageBufferSize = 656*492;     //图像缓冲区大小
	m_nImageByteCount = 1;      //图像字节数	
	m_pbImageBufferAdd = NULL;  // 用于传出的图像内存地址

	m_bIsColorCamera = FALSE;       //是否为彩色相机

	m_pCameraInfo	= new CAMERAINFO;
	memset(&m_CamParms,0,sizeof(CAMPARMS));
	m_CamParms.Format7ModeAdress=0xF0F08000;
	m_bSet1 = FALSE;
	m_bSet2 = FALSE;
	m_bSet3 = FALSE;
	m_bSet4 = FALSE;
	m_bSet5 = FALSE;
	//m_nInitNum = 0;
	m_lFrameLengh = 0;
	m_bFirstFrame = TRUE;
	m_nMultishotNum = 14;
	m_bSnapShot = FALSE;
	m_strLanType = _T("English");
	//m_bTestFramerate = FALSE;
}

CAVTCamDH::~CAVTCamDH()
{
	if(m_pbImageBufferAdd != NULL)
	{
		delete []m_pbImageBufferAdd;
		m_pbImageBufferAdd = NULL;
	}
	
}

////////////////////////////////////////////////////////////////////////
//初始化
BOOL CAVTCamDH::Init(const s_GBINITSTRUCT* pInitParam)
{
	//只是第一个相机时进行初始化
	if (pInitParam->iGrabberSN == 0)
	{
		if (!Init_Grabber_Module())
		{
			m_InitSucceed = FALSE;
			m_LastErrorInfo.iErrorCode = ErrorInit;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("初始化FGInitModule失败，请检查驱动版本"));
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("Init()函数"));
			return FALSE;
		}
	}

	//获得最大的设备SN 用于退出
	if (pInitParam->iGrabberSN> m_iMaxGrabberSN)
	{
		m_iMaxGrabberSN = pInitParam->iGrabberSN;
	}

	//相机初始化
	s_AVT_INITSTRUCT InitCardParam;
	InitCardParam.iGrabberTypeSN = pInitParam->iGrabberTypeSN;  // 采集卡类型编号
	InitCardParam.nGrabberSN = pInitParam->iGrabberSN;          // 采集卡序号
	_tprintf_s(InitCardParam.strDeviceName, pInitParam->strDeviceName);  // 设备名称
	_tprintf_s(InitCardParam.strDeviceMark, pInitParam->strDeviceMark); // 设备标识
	_tprintf_s(InitCardParam.strGrabberFile, pInitParam->strGrabberFile); // 用于初始化的文件
	InitCardParam.CallBackFunc = pInitParam->CallBackFunc;
	InitCardParam.Context = pInitParam->Context;
	BOOL bRet = InitParm(&InitCardParam);
	if (!bRet)
	{
		return FALSE;
	}

	//TestFramerate();
	return TRUE;

}

bool CAVTCamDH::Init(const CGrabInitInfo& rGrabInitInfo)
{
	s_AVT_INITSTRUCT InitCardParam;
	InitCardParam.nGrabberSN = rGrabInitInfo.iGrabSN;          // 采集卡序号
	InitCardParam.iGrabberTypeSN = GetPrivateProfileInt(_T("相机参数"), _T("GrabberTypeSN"), -1, rGrabInitInfo.lpszPathFileOfGrab);
	GetPrivateProfileString(_T("相机参数"), _T("GrabberName"), _T(""), InitCardParam.strDeviceName, 256,rGrabInitInfo.lpszPathFileOfGrab);
	GetPrivateProfileString(_T("相机参数"), _T("GrabberMark"), _T(""), InitCardParam.strDeviceMark, 256, rGrabInitInfo.lpszPathFileOfGrab);
	_tprintf_s(InitCardParam.strGrabberFile, rGrabInitInfo.lpszPathFileOfGrab); // 用于初始化的文件
	InitCardParam.CallBackFunc = rGrabInitInfo.CallBackFunc;
	InitCardParam.Context = rGrabInitInfo.Context;
	BOOL bRet = InitParm(&InitCardParam);
	if (!bRet)
	{
		return false;
	}
	return true;
}

BOOL CAVTCamDH::InitParm(const s_AVT_INITSTRUCT* pInitParam)
{
	//////////////////////////////////////////////////////////////////////////
	m_nGrabberTypeSN = pInitParam->iGrabberTypeSN;      // 采集卡类型号
	m_nGrabberSN   = pInitParam->nGrabberSN;			// 序号
	m_CallBackFunc = pInitParam->CallBackFunc;			// 回调函数指针
	m_Context      = pInitParam->Context;				// 存放调用初始化函数的对象的this指针
	m_sInitFile	   = pInitParam->strGrabberFile;
	m_sDeviceName  = pInitParam->strDeviceName;
	m_sDeviceMark  = pInitParam->strDeviceMark;  //设备标识
	m_sInitFile = m_sInitFile.Left(m_sInitFile.GetLength()-4);
	m_sInitFile += _T(".ini");

	//检查系统配置文件是否存在
	CFileFind searcher;			
	if(!searcher.FindFile(m_sInitFile))
	{
		//////////////////////////////////////////////////////////////////////////
		//文件不存在，创建默认INI配置文件
		CreateDefaultConfigurationFile(m_sInitFile);

// 		m_InitSucceed = FALSE;
// 		m_LastErrorInfo.nErrorCode = ErrorInit; 
// 		_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("相机配置文件不在指定路径中"));
// 		_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("Init()函数"));
//  	return FALSE;
	}

	// 判断相机序号是否超出最大允许相机个数
 	if (pInitParam->nGrabberSN > iMaxCameraNum)
 	{
 		m_InitSucceed = FALSE;
 		m_LastErrorInfo.iErrorCode = ErrorInit;  // 加入出错描述
 		_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("相机序号超出最大允许相机个数"));
 		_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("InitParm()函数"));
 		return FALSE;
 	}

	//初始化相机
	//m_nInitNum = 0;
	if (!InitCamera())
	{
		m_InitSucceed = FALSE;
		m_LastErrorInfo.iErrorCode = ErrorInit;  // 加入出错描述
		_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("初始化相机失败!"));
 		_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("InitParm()函数"));
 		return FALSE;
	}

	return TRUE;
}

//开始采集
BOOL CAVTCamDH::StartGrab()
{
	UINT32  Result = FCE_NOERROR;
	
	if (!m_InitSucceed)  //是否初始化成功  
	{
		m_LastErrorInfo.iErrorCode = ErrorStartGrab;  // 加入出错描述 
		_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("相机没有初始化"));
		_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("StartGrab()函数"));
		return FALSE;
	}

	if (m_CameraActive) //如果相机正在采集，就停止采集
	{
		StopGrab();
	}

	if (m_CameraActive) // 是否正在采集
	{
		m_LastErrorInfo.iErrorCode = ErrorStartGrab;  // 加入出错描述 
		_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("相机已经在采集"));
		_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("StartGrab()函数"));
		return FALSE;
	}

	if(!m_CameraActive)	// 如果没有启动采集
	{
		
		UINT32	Address = 0xF0F0061C;
		UINT32	Value = 0x0;

		Result = m_Camera.ReadRegister(Address,&Value);
		if (Value != 0x0)
		{
			m_Camera.WriteRegister(Address,0x0);
		}

		Address = 0xF0F00614;
		Result = m_Camera.ReadRegister(Address,&Value);//ISO_EN/Continuous_Shot
		if(Result==FCE_NOERROR)
		{	
			if(Value != 0x80000000)
			{	
				Value = 0x80000000;
				Result = m_Camera.WriteRegister(Address,Value);//F7M0_BYTE_PER_PACKET
				
				if(Result==FCE_NOERROR)
				{
				}
				else
				{
					CheckResult(_T("StartGrab::WriteRegister(0xF0F00614)失败 "),Result,ErrorSetParam);
					return FALSE;
				}	
			}
		}

		Result = m_Camera.SetParameter(FGP_BURSTCOUNT,BC_INFINITE);	
		if(Result==FCE_NOERROR)
		{
		}
		else
		{
			CheckResult(_T("StartGrab::SetParameter(FGP_BURSTCOUNT)失败 "),Result,ErrorSetParam);
			return FALSE;
		}

		Result=m_Camera.SetParameter(FGP_DMAMODE,DMA_REPLACE);
		if(Result==FCE_NOERROR)
		{
		}
		else
		{
			CheckResult(_T("StartGrab::SetParameter(FGP_DMAMODE)失败 "),Result,ErrorSetParam);
			return FALSE;
		}

		m_FrameCnt		= 0;// 采集帧计数置零
		
		Result = m_Camera.OpenCapture();// 打开相机
		if(Result != FCE_NOERROR)
		{
			if(Result == FCE_PARTIAL)
			{
				Result = FCE_NOERROR;
			}
		}			
		
		if(Result==FCE_NOERROR)
		{			
			Result = m_Camera.StartDevice();// 开始采集图像
			if(Result==FCE_NOERROR)
			{				
				m_CameraActive = TRUE;// 采集启动
			}
			else
			{				
				m_Camera.StopDevice();// 关闭设备				
				m_Camera.CloseCapture();// 停止采集
			}
		}
		else
		{
			m_LastErrorInfo.iErrorCode = ErrorStartGrab;  // 加入出错描述 
			CString str;
			str.Format(_T("开始采集出错-ErrorCode=%d-"),Result);
			_tprintf_s(m_LastErrorInfo.strErrorDescription,str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("StartGrab()函数"));
			return FALSE;
		}	
	}//*/
	return TRUE;
}

// 单帧采集
BOOL CAVTCamDH::Snapshot()
{	
	UINT32    Result = FCE_NOERROR;
	if (!m_InitSucceed)  //是否初始化成功  
	{
		m_LastErrorInfo.iErrorCode = ErrorStartGrab;  // 加入出错描述 
		_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("相机没有初始化"));
		_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("Snapshot()函数"));
		return FALSE;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                           
	}
	
	
	if(m_CameraActive && m_bSnapShot)
	{	
		UINT32 Address = 0xF0F00614;
		UINT32 Value;
		Result = m_Camera.ReadRegister(Address,&Value);//ISO_EN/Continuous_Shot
		if(Result==FCE_NOERROR)
		{	
			if(Value != 0)
			{	
				Value = 0x0;
				Result = m_Camera.WriteRegister(Address,Value);//F7M0_BYTE_PER_PACKET
				
				if(Result==FCE_NOERROR)
				{
				}
				else
				{
					CheckResult(_T("Snapshot::WriteRegister(0xF0F00614)失败"),Result,ErrorSetParam);
					return FALSE;
				}	
			}

			Address = 0xF0F0061C;
			Value = 0x0;

			Result = m_Camera.ReadRegister(Address,&Value);
			if (Value == 0x40000000 || Value == 0x0)
			{

			}
			else
			{
				CheckResult(_T("Snapshot::ReadRegister(0xF0F0061C)失败"),Result,ErrorGetParam);
				return FALSE;
			}

			Value = 0x40000000+(UINT32)m_nMultishotNum;

			Result = m_Camera.WriteRegister(Address,Value);
			if(Result==FCE_NOERROR)
			{
				Address = 0xF0F0062C;
				Value = 0x0;
				Result = m_Camera.ReadRegister(Address,&Value);
				if (Result==FCE_NOERROR)
				{
					if (Value == 0x80000000)
					{
						Result = m_Camera.WriteRegister(Address,0x80000001);
					}
					else
					{
						CheckResult(_T("Snapshot::WriteRegister(0xF0F0062C)失败"),Result,ErrorSetParam);
						return FALSE;
					}
				}
				else
				{
					CheckResult(_T("Snapshot::ReadRegister(0xF0F0062C)失败"),Result,ErrorGetParam);
					return FALSE;
				}
			}
			else
			{
				CheckResult(_T("Snapshot::WriteRegister(0xF0F0061C)失败"),Result,ErrorSetParam);
				return FALSE;
			}
		}
		else
		{
			CheckResult(_T("Snapshot::ReadRegister(0xF0F00614)失败"),Result,ErrorGetParam);
			return FALSE;
		}
		
 
	}
	else
	{
		UINT32	Address = 0xF0F0061C;
		UINT32	Value = 0x0;

		Result = m_Camera.ReadRegister(Address,&Value);
		if (Value != 0x0)
		{
			m_Camera.WriteRegister(Address,0x0);
		}


		Address = 0xF0F00614;
		Result = m_Camera.ReadRegister(Address,&Value);//ISO_EN/Continuous_Shot
		if(Result==FCE_NOERROR)
		{	
			if(Value != 0x80000000)
			{	
				Value = 0x80000000;
				Result = m_Camera.WriteRegister(Address,Value);//F7M0_BYTE_PER_PACKET
				
				if(Result==FCE_NOERROR)
				{
				}
				else
				{
					CheckResult(_T("Snapshot::WriteRegister(0xF0F00614)失败"),Result,ErrorSetParam);
					return FALSE;
				}	
			}
		}


		CheckResult(_T("Snapshot::没有开采或没有触发"),0,ErrorSetParam);
		return FALSE;
	}
	return TRUE;
}

//停止采集
BOOL CAVTCamDH::StopGrab()
{
	UINT32   Result = FCE_NOERROR;	
	if (!m_InitSucceed)    // 是否初始化成功
	{
		m_LastErrorInfo.iErrorCode = ErrorStopGrab;  // 加入出错描述
		_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("相机尚未初始化成功"));
		_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("StopGrab()函数"));
		return FALSE;
	}
	
	if (!m_CameraActive)// 是否启动采集
	{
		m_LastErrorInfo.iErrorCode = ErrorStopGrab;  // 加入出错描述 
		_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("相机尚未处于采集状态"));
		_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("StopGrab()函数"));
		return FALSE;
	}

	m_CameraActive=FALSE;
	UINT32	Address = 0xF0F0061C;
	UINT32	Value = 0x0;

	Result = m_Camera.ReadRegister(Address,&Value);
	if(Result!=FCE_NOERROR)
	{
		CheckResult(_T("StopGrab::ReadRegister(0xF0F0061C)失败"),Result,ErrorGetParam);
		return FALSE;
	}

	if (Value != 0x0)
	{
		Result = m_Camera.WriteRegister(Address,0x0);
		if(Result==FCE_NOERROR)
		{
		}
		else
		{
			CheckResult(_T("StopGrab::WriteRegister(0xF0F0061C)失败"),Result,ErrorSetParam);
			return FALSE;
		}
	}
	Address = 0xF0F00614;
	Result = m_Camera.ReadRegister(Address,&Value);//ISO_EN/Continuous_Shot
	if(Result==FCE_NOERROR)
	{	
		if(Value != 0x80000000)
		{	
			Value = 0x80000000;
			Result = m_Camera.WriteRegister(Address,Value);//F7M0_BYTE_PER_PACKET
				
			if(Result==FCE_NOERROR)
			{
			}
			else
			{
				CheckResult(_T("StopGrab::WriteRegister(0xF0F00614)失败"),Result,ErrorSetParam);
				return FALSE;
			}	
		}
	}
	else
	{
		CheckResult(_T("StopGrab::ReadRegister(0xF0F00614)失败"),Result,ErrorGetParam);
		return FALSE;
	}
		
	Result = m_Camera.StopDevice();// 停止设备
	if (Result != FCE_NOERROR)
	{
		CheckResult(_T("StopGrab::StopDevice失败"),Result,ErrorStopGrab);
		m_CameraActive=FALSE;

		return FALSE;
		
	}

	Result = m_Camera.CloseCapture();	//停止采集
	if (Result != FCE_NOERROR)
	{
		CheckResult(_T("StopGrab::CloseCapture失败"),Result,ErrorStopGrab);
		m_CameraActive=FALSE;
		return FALSE;
	}	
    	
	return TRUE;
}

//关闭
BOOL CAVTCamDH::Close()
{	
	if (m_InitSucceed == TRUE)// 如果初始化成功
	{		
		if (m_CameraActive) // 如果处于采集状态
		{
			StopGrab();
		}		
		m_Camera.Disconnect();// 断开与相机的连接
		
		if(m_pCameraInfo)
		{
			delete [] m_pCameraInfo;
			m_pCameraInfo = NULL;		
		}

		if (m_nGrabberSN == m_iMaxGrabberSN) //是最后一个相机关闭时 退出
		{
			if (!Exit_Grabber_Module()) //只是退出一次
			{

			}
		}
		PostQuitMessage(0);	
	}	
	return TRUE;
}

//得到参数
BOOL CAVTCamDH::GetParamInt(GBParamID Param, int &nOutputVal)
{
	UINT32 Ret;
	switch(Param)
	{
	case 0://GBImageWidth:
		nOutputVal = m_nImageWidth;
		break;
	case 1://GBImageHeight:
		nOutputVal = m_nImageHeight;
		break;
	case 2://GBImagePixelSize:
		nOutputVal = m_nImageByteCount;
		break;
	case 3://GBImageBufferSize:
		nOutputVal = m_nImageWidth * m_nImageHeight * m_nImageByteCount;
		break;
	case 4://GBImageBufferAddr:
		nOutputVal = (int)m_pbImageBufferAdd;
		break;
	case 6://GBColor,					//相机类型  黑白和彩色 
		nOutputVal = m_bIsColorCamera; // 是否彩色相机 
		break;
	case 7://GBBrightness,				//相机亮度大小
		UINT32 OutputVal;
		Ret = m_Camera.GetParameter(FGP_BRIGHTNESS,&OutputVal);
		if (Ret == FCE_NOERROR)
		{
			nOutputVal = (int)OutputVal;
		}
		else
		{
			CheckResult(_T("GetParamInt::GetParameter(FGP_BRIGHTNESS)失败"),Ret,ErrorGetParam);
			return FALSE;
		}
		
		break;
	case 8://GBShutter,				//相机快门值
		UINT32 Shutter;
		Ret = m_Camera.GetParameter(FGP_SHUTTER,&Shutter);
		if (Ret == FCE_NOERROR)
		{
			nOutputVal = (int)Shutter;
		}
		else
		{
			CheckResult(_T("GetParamInt::GetParameter(FGP_SHUTTER)失败"),Ret,ErrorGetParam);
			return FALSE;
		}
		break;
	case 9://GBGain,					//相机增益大小
		UINT32 Gain;
		Ret = m_Camera.GetParameter(FGP_GAIN,&Gain);
		if (Ret == FCE_NOERROR)
		{
			nOutputVal = (int)Gain;
		}
		else
		{
			CheckResult(_T("GetParamInt::GetParameter(FGP_GAIN)失败"),Ret,ErrorGetParam);
			return FALSE;
		}
		break;
	case 10://GBU,						//相机U值  
		if (m_bIsColorCamera == TRUE)
		{
			UINT32 WhiteBalanceB;
			Ret = m_Camera.GetParameter(FGP_WHITEBALCB,&WhiteBalanceB);
			if (Ret == FCE_NOERROR)
			{
				nOutputVal = (int)WhiteBalanceB;
			}
			else
			{
				CheckResult(_T("GetParamInt::GetParameter(FGP_WHITEBALCB)失败"),Ret,ErrorGetParam);
				return FALSE;
			}
		}
		break;
	case 11://GBV,						//相机V值
		if (m_bIsColorCamera == TRUE)
		{
			UINT32 WhiteBalanceR;
			Ret = m_Camera.GetParameter(FGP_WHITEBALCR,&WhiteBalanceR);
			if (Ret == FCE_NOERROR)
			{
				nOutputVal = (int)WhiteBalanceR;
			}
			else
			{
				CheckResult(_T("GetParamInt::GetParameter(FGP_WHITEBALCR)失败"),Ret,ErrorGetParam);
				return FALSE;
			}
		}
		break;
	case 13://
			nOutputVal = m_nMultishotNum;
		break;

		//////////////////////////////////////////////////////////////////////////
	default:
		m_LastErrorInfo.iErrorCode = ErrorGetParam;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("得到参数出错"));
		_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("GetParamInt()函数"));

		return FALSE;
	}

	return TRUE;
}
	
//设置参数
BOOL CAVTCamDH::SetParamInt(GBParamID Param, int nInputVal)
{
	CString str;
	UINT32 Ret;
	switch(Param)
	{
	case 7://GBBrightness,				//相机亮度大小
		Ret = m_Camera.SetParameter(FGP_BRIGHTNESS,nInputVal);
		if (Ret == FCE_NOERROR)
		{
			str.Format(_T("%d"),nInputVal);
			WritePrivateProfileString(_T("相机参数"),_T("Brightness"),str,m_sInitFile);
		}
		else
		{
			str.Format(_T("SetParamInt::SetParameter(FGP_BRIGHTNESS,%d)失败"),nInputVal);
			CheckResult(str,Ret,ErrorSetParam);
			return FALSE;
		}
		break;
	case 8://GBShutter,				//相机快门值
		Ret = m_Camera.SetParameter(FGP_SHUTTER,nInputVal);
		if (Ret == FCE_NOERROR)
		{
			str.Format(_T("%d"),nInputVal);
			WritePrivateProfileString(_T("相机参数"),_T("Shutter"),str,m_sInitFile);
		}
		else 
		{
			str.Format(_T("SetParamInt::SetParameter(FGP_SHUTTER,%d)失败"),nInputVal);
			CheckResult(str,Ret,ErrorSetParam);
			return FALSE;
		}
		break;
	case 9://GBGain,					//相机增益大小
		Ret = m_Camera.SetParameter(FGP_GAIN,nInputVal);
		if (Ret == FCE_NOERROR)
		{
			str.Format(_T("%d"),nInputVal);
			WritePrivateProfileString(_T("相机参数"),_T("Gain"),str,m_sInitFile);
		}
		else
		{
			str.Format(_T("SetParamInt::SetParameter(FGP_GAIN,%d)失败"),nInputVal);
			CheckResult(str,Ret,ErrorSetParam);
			return FALSE;
		}
		break;
	case 10://GBU,						//相机U值 
		if (m_bIsColorCamera == TRUE)
		{
			Ret = m_Camera.SetParameter(FGP_WHITEBALCB,nInputVal);
			if (Ret == FCE_NOERROR)
			{
				str.Format(_T("%d"),nInputVal);
				WritePrivateProfileString(_T("相机参数"),_T("Whitebalcb"),str,m_sInitFile);
			}
			else
			{
				str.Format(_T("SetParamInt::SetParameter(FGP_WHITEBALCB,%d)失败"),nInputVal);
				CheckResult(str,Ret,ErrorSetParam);
				return FALSE;
			}
		}
		break;
	case 11://GBV,						//相机V值 
		if (m_bIsColorCamera == TRUE)
		{
			Ret = m_Camera.SetParameter(FGP_WHITEBALCR,nInputVal);
			if (Ret == FCE_NOERROR)
			{
				str.Format(_T("%d"),nInputVal);
				WritePrivateProfileString(_T("相机参数"),_T("Whitebalcr"),str,m_sInitFile);
			}
			else
			{
				str.Format(_T("SetParamInt::SetParameter(FGP_WHITEBALCR,%d)失败"),nInputVal);
				CheckResult(str,Ret,ErrorSetParam);
				return FALSE;
			}
		}
		break;
	case 13://设定Multishot采集桢数
		{
			m_nMultishotNum = nInputVal;
			str.Format(_T("%d"),nInputVal);
			WritePrivateProfileString(_T("相机参数"),_T("MultishotNum"),str,m_sInitFile);
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}


//得到参数
BOOL CAVTCamDH::GetParamfloat(int Param, float &nOutputVal)
{
	return TRUE;
}
	
//设置参数
BOOL CAVTCamDH::SetParamfloat(int Param, float nInputVal)
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
void CAVTCamDH::CallParamDialog()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
#ifdef _DEBUG
	// Workaround for ASSERT in WINCORE.CPP 884 (CWnd::AssertValid)
	tempRoutingFrame rframe;
#endif

	CSetCameraParam dlg;
	dlg.m_strLanType = m_strLanType;
	dlg.m_iMaxWhiteBalCB = m_CamParms.WhiteBalanceMaxCB;  //最大的白平衡CB分量 
	dlg.m_iMaxWhiteBalCR = m_CamParms.WhiteBalanceMaxCR;  //最大的白平衡CR分量

	dlg.m_iMaxBrightness = m_CamParms.BrightnessMax;
	dlg.m_iMaxGain  = m_CamParms.GainMax;
	dlg.m_iMaxShutter = m_CamParms.ShutterMax;

	dlg.m_iMinBrightness = m_CamParms.BrightnessMin;
	dlg.m_iMinGain  = m_CamParms.GainMin;
	dlg.m_iMinShutter = m_CamParms.ShutterMin;

	dlg.SetOwner(this, m_sInitFile);
	dlg.DoModal();
	delete  dlg;  
}

//得到出错信息
void CAVTCamDH::GetLastErrorInfo(s_GBERRORINFO *pErrorInfo)
{
	//   返回出错信息
// 	pErrorInfo->nErrorCode = m_LastErrorInfo.nErrorCode;
// 	pErrorInfo->strErrorDescription = m_LastErrorInfo.strErrorDescription;
// 	pErrorInfo->strErrorRemark = m_LastErrorInfo.strErrorRemark;

	if (pErrorInfo != NULL)
	{
		memcpy(pErrorInfo,&m_LastErrorInfo,sizeof(s_GBERRORINFO));
	}
}	

BOOL CAVTCamDH::GetTriggerParam(int &ParamValOutput)
{
	UINT32 TriggerValue = 0;
	UINT32 Ret = m_Camera.ReadRegister(0xF0F00830,&TriggerValue);
	if (Ret != FCE_NOERROR)
	{
		CString str;
		str.Format(_T("GetTriggerParam::ReadRegister(0xF0F00830)失败"));
		CheckResult(str,Ret,ErrorSetParam);
		return FALSE;
	}
		switch(TriggerValue) 
		{
		case 0x80100000:
			ParamValOutput = 0;//AVTTriggerOff;
			break;
		case 0x83100000:
			ParamValOutput = 1;//AVTTriggerOn;
			break;
		case 0x82100000:
			ParamValOutput = 2;//AVTTriggerMode2;
			break;
		case 0x83110000:
			ParamValOutput = 3;//AVTTriggerMode3;
			break;
		case 0x82110000:
			ParamValOutput = 4;//AVTTriggerMode4;
			break;
		case 0x831F0000:
			ParamValOutput = 5;//AVTTriggerMode5;
			break;
		case 0x821F0000:
			ParamValOutput = 6;//AVTTriggerMode6;
			break;
		case 0x82E00000:
			ParamValOutput = 7;//AVTTriggerMode7;
			break;
		case 0x82E10000:
			ParamValOutput = 8;//AVTTriggerMode8;
			break;
		case 0x82EF0000:
			ParamValOutput = 9;//AVTTriggerMode9;
			break;
		default:
			return FALSE;
		}
	
	return TRUE;
}
BOOL CAVTCamDH::SetTriggerParam(int ParamValInput)
{
//	int TriggerValue = 0;
	UINT32 Ret = 0;
	CString str;

	if(!m_InitSucceed)
		return FALSE ; 
	switch(ParamValInput) 
	{

				case 0://AVTTriggerOff:  //外触发关闭//内部触发,连续采集
				//	TriggerValue = 0;
					//	m_Camera.SetParameter(FGP_TRIGGER,TriggerValue);
					m_bSnapShot = FALSE;
					Ret = m_Camera.WriteRegister(0xF0F0061C,0x0);
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F0061C)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					Ret = m_Camera.WriteRegister(0xF0F00614,0x80000000);
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F00614)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					Ret = m_Camera.WriteRegister(0xF0F00830,0x80100000);//Internal
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F00830)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					break;
				case 1://AVTTriggerOn://上升沿触发//外触发开启
				//	TriggerValue = 50331648;
					//	m_Camera.SetParameter(FGP_TRIGGER,TriggerValue);
					m_bSnapShot = FALSE;
					Ret = m_Camera.WriteRegister(0xF0F0061C,0x0);
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F0061C)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					Ret = m_Camera.WriteRegister(0xF0F00614,0x80000000);
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F00614)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					Ret = m_Camera.WriteRegister(0xF0F00830,0x83100000);//Edge Mode(0)-Ext Rising
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F00830)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					break;
				case 2://AVTTriggerMode2://下降沿触发
				//	TriggerValue = 33554432;
					//	m_Camera.SetParameter(FGP_TRIGGER,TriggerValue);
					m_bSnapShot = FALSE;
					Ret = m_Camera.WriteRegister(0xF0F0061C,0x0);
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F0061C)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					Ret = m_Camera.WriteRegister(0xF0F00614,0x80000000);
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F00614)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					Ret = m_Camera.WriteRegister(0xF0F00830,0x82100000);//Edge Mode(0)-Ext Falling
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F00830)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					break;
				case 3://AVTTriggerMode3://高电平触发
				//	TriggerValue = 50397184;
					//	m_Camera.SetParameter(FGP_TRIGGER,TriggerValue);
					m_bSnapShot = FALSE;
					Ret = m_Camera.WriteRegister(0xF0F0061C,0x0);
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F0061C)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					Ret = m_Camera.WriteRegister(0xF0F00614,0x80000000);
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F00614)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					Ret = m_Camera.WriteRegister(0xF0F00830,0x83110000);//Level Mode(1)-Ext High act
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F00830)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					break;
				case 4://AVTTriggerMode4://低电平触发
				//	TriggerValue = 33619968;
					//	m_Camera.SetParameter(FGP_TRIGGER,TriggerValue);
					m_bSnapShot = FALSE;
					Ret = m_Camera.WriteRegister(0xF0F0061C,0x0);
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F0061C)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					Ret = m_Camera.WriteRegister(0xF0F00614,0x80000000);
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F00614)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					Ret = m_Camera.WriteRegister(0xF0F00830,0x82110000);//Level Mode(1)-Ext Low act
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F00830)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					break;
				case 5://AVTTriggerMode5://编程模式上升沿触发
				//	TriggerValue = 51214688;
					//	m_Camera.SetParameter(FGP_TRIGGER,TriggerValue);
					m_bSnapShot = FALSE;
					Ret = m_Camera.WriteRegister(0xF0F0061C,0x0);
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F0061C)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					Ret = m_Camera.WriteRegister(0xF0F00614,0x80000000);
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F00614)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					Ret = m_Camera.WriteRegister(0xF0F00830,0x831F0000);//Progr. Mode(15)-Ext Rising
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F00830)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					break;
				case 6://AVTTriggerMode6://编程模式上升沿触发
				//	TriggerValue = 34537472;
					//	m_Camera.SetParameter(FGP_TRIGGER,TriggerValue);
					m_bSnapShot = FALSE;
					Ret = m_Camera.WriteRegister(0xF0F0061C,0x0);
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F0061C)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					Ret = m_Camera.WriteRegister(0xF0F00614,0x80000000);
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F00614)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					Ret = m_Camera.WriteRegister(0xF0F00830,0x821F0000);//Progr. Mode(15)-Ext Falling
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F00830)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					break;
				case 7://AVTTriggerMode7:
				//	TriggerValue = 34537472;
					//	m_Camera.SetParameter(FGP_TRIGGER,TriggerValue);
					m_bSnapShot = TRUE;
					Ret = m_Camera.WriteRegister(0xF0F00614,0x0);
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F00614)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					Ret = m_Camera.WriteRegister(0xF0F00830,0x82E00000);//
					break;
				case 8://AVTTriggerMode8:
				//	TriggerValue = 34537472;
					//	m_Camera.SetParameter(FGP_TRIGGER,TriggerValue);
					m_bSnapShot = TRUE;
					Ret = m_Camera.WriteRegister(0xF0F00614,0x0);
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F00614)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					Ret = m_Camera.WriteRegister(0xF0F00830,0x82E10000);//
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F00830)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					break;
				case 9://AVTTriggerMode8:
				//	TriggerValue = 34537472;
					//	m_Camera.SetParameter(FGP_TRIGGER,TriggerValue);
					m_bSnapShot = TRUE;
					Ret = m_Camera.WriteRegister(0xF0F00614,0x0);
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F00614)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					Ret = m_Camera.WriteRegister(0xF0F00830,0x82EF0000);//
					if (Ret != FCE_NOERROR)
					{
						str.Format(_T("SetTriggerParam::(%d)WriteRegister(0xF0F00830)失败"),ParamValInput);
						CheckResult(str,Ret,ErrorSetParam);
						return FALSE;
					}
					break;
				default:
						return FALSE;

	}
	return TRUE;
}
BOOL CAVTCamDH::GetSystemInfo(CString &sOutputInfo)
{
	if(!m_InitSucceed)
	{
		CheckResult(_T("AVTGetCamGuid 没有初始话成功"),0,ErrorGetParam);
		return FALSE ;
	}

	sOutputInfo.Format(_T("GaidHigh:%d,GaidLow:%d"),m_nGaidHigh,m_nGaidLow);
	
	return TRUE;
}
BOOL CAVTCamDH::SetAOI(RECT rtAOI)
{
	if (!m_InitSucceed)
	{
		//如果没有初始话成功函数范围FALSE
		CheckResult(_T("AVTSetAOI 没有初始话成功"),0,ErrorSetParam);
		return FALSE;
	}

// 	if (m_CameraActive)//6.0.0.1beta3
// 	{
// 	//	AfxMessageBox(_T("正在采集,不能设置AOI区域!"));
// 		CheckResult(_T("AVTSetAOI 正在采集,不能设置AOI区域"),0,ErrorSetParam);
// 		return FALSE;
// 	}

	if (m_CamParms.ImageFormat < 7)
	{
		CheckResult(_T("AVTSetAOI 非Format7"),0,ErrorSetParam);
		return FALSE;
	}


 
	UINT32 nXPos = rtAOI.left;
	UINT32 nYPos	= rtAOI.top;
	UINT32 nXSize = rtAOI.right - rtAOI.left;
	UINT32 nYSize = rtAOI.bottom - rtAOI.top;
	
	//各个参数不能小于零
	if (nXPos < 0 || nYPos < 0 || nXSize < 0 || nYSize < 0 )
	{
		CheckResult(_T("AVTSetAOI 各个参数不能小于零"),0,ErrorSetParam);
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////

	UINT32 Result;
	CString str;

	if (nXSize > m_CamParms.XSizeMax)
	{
		nXSize = m_CamParms.XSizeMax;
	}
				
	if (nXSize < m_CamParms.XSizeMin)
	{
		nXSize = m_CamParms.XSizeMin;
	}

	if (nXSize%m_CamParms.XSizeUnit != 0)
	{
		nXSize-=nXSize % m_CamParms.XSizeUnit;
	}

	if (nXPos > m_CamParms.XPosMax)
	{
		nXPos = m_CamParms.XPosMax;
	}
				
	if (nXPos < m_CamParms.XPosMin)
	{
		nXPos = m_CamParms.XPosMin;
	}

	if (nXPos%m_CamParms.XPosUnit != 0)
	{
		nXPos-=nXPos % m_CamParms.XPosUnit;
	}
				
	if (nXSize + nXPos > m_CamParms.XSizeMax)
	{
	//	str.Format(_T("左边+宽度 > %d 请重新设定!"),m_CamParms.XSizeMax);
		CheckResult(_T("AVTSetAOI 左边+宽度 > %d 请重新设定"),m_CamParms.XSizeMax,ErrorSetParam);
		return FALSE;
	}
				


	if (nYPos > m_CamParms.YPosMax)
	{
		nYPos = m_CamParms.YPosMax;
	}
	if (nYPos < m_CamParms.YPosMin)
	{
		nYPos = m_CamParms.YPosMin;
	}
	if (nYPos%m_CamParms.YPosUnit != 0)
	{
		nYPos-=nYPos % m_CamParms.YPosUnit;
	}

	if (nYSize > m_CamParms.YSizeMax)
	{
		nYSize = m_CamParms.YSizeMax;
	}
				
	if (nYSize < m_CamParms.YSizeMin)
	{
		nYSize = m_CamParms.YSizeMin;
	}

	if (nYSize%m_CamParms.YSizeUnit != 0)
	{
		nYSize-=nYSize % m_CamParms.YSizeUnit;
	}
				
	if (nYSize + nYPos > m_CamParms.YSizeMax)
	{
		//str.Format(_T("上边+高度 > %d 请重新设定!"),m_CamParms.YSizeMax);
		CheckResult(_T("AVTSetAOI 上边+高度 > %d 请重新设定"),m_CamParms.YSizeMax,ErrorSetParam);
		return FALSE;
	}


	// Now we resize
	Result = m_Camera.SetParameter(FGP_RESIZE,1);
	if (Result != FCE_NOERROR)
	{
		str.Format(_T("SetAOI::SetParameter(FGP_RESIZE,1)失败"));
		CheckResult(str,Result,ErrorSetParam);
		return FALSE;
	}
				
	//////////////////////////////////////////////////////////////////////////
	//设置AOI区域
	if (nXSize != m_CamParms.XSize 
		|| nXPos != m_CamParms.XPos)
	{
		if(nXSize + m_CamParms.XPos <= m_CamParms.XSizeMax)
		{
			if (nXSize != m_CamParms.XSize)
			{
				Result = m_Camera.SetParameter(FGP_XSIZE,nXSize);
				if(Result==FCE_NOERROR)
				{
					m_CamParms.XSize = nXSize;			
				}
				else
				{
					str.Format(_T("设置AOI宽度%d失败!-1"),nXSize);
					CheckResult(str,Result,ErrorSetParam);
					return FALSE;
				}
			}


			if (nXPos != m_CamParms.XPos)
			{
				Result = m_Camera.SetParameter(FGP_XPOSITION,nXPos);
				if(Result==FCE_NOERROR)
				{
					m_CamParms.XPos = nXPos;
				}
				else
				{
					str.Format(_T("设置AOI左边%d失败!-1"),nXPos);
					CheckResult(str,Result,ErrorSetParam);
					return FALSE;
				}
			}

		}
		else
		{
			if (nXPos != m_CamParms.XPos)
			{
				Result = m_Camera.SetParameter(FGP_XPOSITION,nXPos);
				if(Result==FCE_NOERROR)
				{
					m_CamParms.XPos = nXPos;
				}
				else
				{
					str.Format(_T("设置AOI左边%d失败!-2"),nXPos);
					CheckResult(str,Result,ErrorSetParam);
					return FALSE;
				}
			}


			if (nXSize != m_CamParms.XSize)
			{
				Result = m_Camera.SetParameter(FGP_XSIZE,nXSize);
				if(Result==FCE_NOERROR)
				{
					m_CamParms.XSize = nXSize;			
				}
				else
				{
					str.Format(_T("设置AOI宽度%d失败!-2"),nXSize);
					CheckResult(str,Result,ErrorSetParam);
					return FALSE;
				}
			}
		}
	}

	if (nYSize != m_CamParms.YSize 
		|| nYPos != m_CamParms.YPos)
	{
		if(nYSize + m_CamParms.YPos <= m_CamParms.YSizeMax)
		{
			if (nYSize != m_CamParms.YSize)
			{
				Result = m_Camera.SetParameter(FGP_YSIZE,nYSize);
				if(Result==FCE_NOERROR)
				{
					m_CamParms.YSize = nYSize;
				}
				else
				{
					str.Format(_T("设置AOI高度%d失败!-1"),nYSize);
					CheckResult(str,Result,ErrorSetParam);
					return FALSE;
				}
			}

			if (nYPos != m_CamParms.YPos)
			{
				Result = m_Camera.SetParameter(FGP_YPOSITION,nYPos);
				if(Result==FCE_NOERROR)
				{
					m_CamParms.YPos = nYPos;
				}
				else
				{
					str.Format(_T("设置AOI上边%d失败!-1"),nYPos);
					CheckResult(str,Result,ErrorSetParam);
					return FALSE;
				}
			}			
		}
		else
		{
			if (nYPos != m_CamParms.YPos)
			{
				Result = m_Camera.SetParameter(FGP_YPOSITION,nYPos);
				if(Result==FCE_NOERROR)
				{
					m_CamParms.YPos = nYPos;
				}
				else
				{
					str.Format(_T("设置AOI上边%d失败!-2"),nYPos);
					CheckResult(str,Result,ErrorSetParam);
					return FALSE;
				}
			}


			if (nYSize != m_CamParms.YSize)
			{
				Result = m_Camera.SetParameter(FGP_YSIZE,nYSize);
				if(Result==FCE_NOERROR)
				{
					m_CamParms.YSize = nYSize;
				}
				else
				{
					str.Format(_T("设置AOI高度%d失败!-2"),nYSize);
					CheckResult(str,Result,ErrorSetParam);
					return FALSE;
				}
			}	
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//F7M0_BYTE_PER_PACKET
	UINT32 nTemp = m_CamParms.ImageFormat-7;
	UINT32 nTemp1 = nTemp<<12;
	UINT32 Address = m_CamParms.Format7ModeAdress+0x44+nTemp1;//0xF0F08044+nTemp1;
	UINT32 Value;
	Result = m_Camera.ReadRegister(Address,&Value);//F7M0_BYTE_PER_PACKET
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
			Value1 = Value4<<16;
			Value1 +=Value4;	
			Result = m_Camera.WriteRegister(Address,Value1);//F7M0_BYTE_PER_PACKET
	
			if(Result==FCE_NOERROR)
			{
			}
			else
			{
			//	CheckResult(_T("AVTSetAOI 设置包长失败"),0,ErrorSetParam);
				str.Format(_T("AVTSetAOI 设置包长失败"));
				CheckResult(str,Result,ErrorSetParam);
				return FALSE;
			}	
		}
	}
	else
	{
		CheckResult(_T("AVTSetAOI 读取包长失败"),Result,ErrorGetParam);
		return FALSE;
	}

	Result = m_Camera.SetParameter(FGP_RESIZE,0);
	if (Result != FCE_NOERROR)
	{
		str.Format(_T("SetAOI::SetParameter(FGP_RESIZE,0)失败"));
		CheckResult(str,Result,ErrorSetParam);
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	//公共设置
	if ((UINT32)(m_nImageWidth) != m_CamParms.XSize
		||(UINT32)(m_nImageHeight) != m_CamParms.YSize)
	{
		m_nImageWidth  = m_CamParms.XSize;
		m_nImageHeight = m_CamParms.YSize;	
		m_nImageBufferSize = m_nImageWidth * m_nImageHeight * m_nImageByteCount; 
		
		if(m_pbImageBufferAdd != NULL)
		{
			delete [](m_pbImageBufferAdd);
			m_pbImageBufferAdd = NULL;
		}
	//	m_pbImageBufferAdd = new BYTE[m_nImageWidth * m_nImageHeight*3];

	}

	str.Format(_T("%d"), nXPos);
	WritePrivateProfileString(_T("相机参数"),_T("Left"),str,m_sInitFile);
	str.Format(_T("%d"), nXSize);
	WritePrivateProfileString(_T("相机参数"),_T("Width"),str,m_sInitFile);
	str.Format(_T("%d"), nYPos);
	WritePrivateProfileString(_T("相机参数"),_T("Top"),str,m_sInitFile);
	str.Format(_T("%d"), nYSize);
	WritePrivateProfileString(_T("相机参数"),_T("Height"),str,m_sInitFile);


	return TRUE;
}


BOOL CAVTCamDH::GetAOI(RECT &rtAOI)
{
	if (!m_InitSucceed)
	{
		//如果没有初始话成功函数范围FALSE
		return FALSE;
	}

	rtAOI.left = m_CamParms.XPos;
	rtAOI.top  = m_CamParms.YPos;
	rtAOI.right = m_CamParms.XPos + m_CamParms.XSize;
	rtAOI.bottom = m_CamParms.YPos +m_CamParms.YSize;

	return TRUE;
}

//初始化相机
BOOL CAVTCamDH::InitCamera()
{
	UINT32     Result;
	FGNODEINFO NodeInfo[32];
	UINT32     NodeCnt = 0;
	CString str;

	// 得到节点列表
	Result = FGGetNodeList(NodeInfo,sizeof(NodeInfo)/sizeof(FGNODEINFO),&NodeCnt);
	if (Result != FCE_NOERROR)
	{
		m_InitSucceed = FALSE;
// 		m_LastErrorInfo.nErrorCode = ErrorInit;  // 加入出错描述 
// 		_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("获得相机节点失败!"));
// 		_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("Init()函数"));

		str.Format(_T("InitCamera::FGGetNodeList失败"));
		CheckResult(str,Result,ErrorInit);

		return FALSE;
	}

	if (NodeCnt == 0)
	{
		m_InitSucceed = FALSE;
//  		m_LastErrorInfo.nErrorCode = ErrorInit;  // 加入出错描述 
//  		_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("获得相机节点个数为零"));
//  		_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("Init()函数"));

		str.Format(_T("InitCamera::获得相机节点个数为零"));
		CheckResult(str,Result,ErrorInit);
		return FALSE;
	}

	//判断相机序列号合法
	int ilen = m_sDeviceMark.GetLength();
	if (ilen != 15)
	{
		if (ilen != 19) // 兼容后面带0609的模式 
		{
			m_InitSucceed = FALSE;
			m_LastErrorInfo.iErrorCode = ErrorInit;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("获得相机序列号位数不对"));
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("Init()函数"));
			return FALSE;
		}
	}

	//选择指定的相机
	int nHigh	= 6;
	int	nLow	= 9;
 	NodeInfo[m_nGrabberSN].Guid.High = _ttoi(m_sDeviceMark.Mid(0,nHigh));
	NodeInfo[m_nGrabberSN].Guid.Low = _ttoi(m_sDeviceMark.Mid(nHigh,nLow));
	m_nGaidHigh = NodeInfo[m_nGrabberSN].Guid.High;
	m_nGaidLow  = NodeInfo[m_nGrabberSN].Guid.Low;
	m_pCameraInfo->Guid = NodeInfo[m_nGrabberSN].Guid;
	m_pCameraInfo->NodeId = NodeInfo[m_nGrabberSN].NodeId;

	//连接指定相机
	Result = m_Camera.Connect(&m_pCameraInfo->Guid,this);
	if (Result == FCE_NOERROR)
	{
		// 初始化相机成功	
		m_InitSucceed = TRUE;

		//获取相机信息
		char lpszDeviceName[256];
		memset(lpszDeviceName, 0, sizeof(lpszDeviceName));

		Result = m_Camera.GetDeviceName(lpszDeviceName, sizeof(lpszDeviceName)); //获取相机设备名
#ifdef UNICODE
		wstring bwsDeviceName = L"";
		bwsDeviceName = CStringConvert::ANSI_UCS2(lpszDeviceName);
		memcpy_s(m_pCameraInfo->DeviceName, sizeof(m_pCameraInfo->DeviceName), bwsDeviceName.c_str(), bwsDeviceName.size() * sizeof(TCHAR));
#else
		memcpy_s(m_pCameraInfo->DeviceName, sizeof(m_pCameraInfo->DeviceName), lpszDeviceName, sizeof(lpszDeviceName));
#endif
		if (Result==FCE_NOERROR)
		{	
			CString  strDeviceName;
			strDeviceName.Format(_T("%s"),m_pCameraInfo->DeviceName);

			//根据最后一个字母_T("C")或_T("B")来判断相机是否为彩色或黑白相机
			if (strDeviceName.Right(1) == 'B')
			{
				m_bIsColorCamera = FALSE;
			}
			else if (strDeviceName.Right(1) == 'C')
			{
				m_bIsColorCamera = TRUE;
			}
			else
			{
				m_InitSucceed = FALSE;
				m_LastErrorInfo.iErrorCode = ErrorInit;  // 加入出错描述
				_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("SetCameraFromINI判断相机是否为彩色或黑白相机错误!"));
				_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("Init()函数"));
				return FALSE;
			}

			//根据配置文件初始化相机
			if (!SetCameraFromINI())
			{
				m_InitSucceed = FALSE;
				m_LastErrorInfo.iErrorCode = ErrorInit;  // 加入出错描述
				_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("SetCameraFromINI从相机读取参数错误!"));
				_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("Init()函数"));
				return FALSE;
			}
						
		}		
		else
		{
			m_InitSucceed = FALSE;
			m_LastErrorInfo.iErrorCode = ErrorInit;  // 加入出错描述 
			_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("GetDeviceName错误!"));
			_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("Init()函数"));
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		m_InitSucceed = FALSE;
// 		m_LastErrorInfo.nErrorCode = ErrorInit;  // 加入出错描述
// 		_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("连接相机出错, 请检测相机序列号是否正确"));
// 		_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("Init()函数"));

		str.Format(_T("InitCamera::Connect()-连接相机出错, 请检测相机序列号是否正确"));
		CheckResult(str,Result,ErrorInit);
		return FALSE;
	}

	return TRUE;
}

//根据配置文件初始化相机
BOOL CAVTCamDH::SetCameraFromINI()
{
	if (m_CameraActive)
	{
		CheckResult(_T("SetCameraFromINI::正在采集,不能根据配置文件初始化相机。"),0,ErrorGetParam);
 		return FALSE;	
	}

	UINT32  Result;
	UINT32  Value,Value1,Value2,Value3,Value4,Value5;
	FGPINFO Info;
	CString str;

// 	//////////////////////////////////////////////////////////////////////////
// 	//判断相机是否有掉电保存参数的flash，即USERSET
// 	Result = m_Camera.ReadRegister(0xF0F00400,&Value);// USERSET num //IIDC
// 	if(Result==FCE_NOERROR)
// 	{
// 		//////////////////////////////////////////////////////////////////////////
// 		Value1 = 0xf;
// 		Value2 = 0x0;
// 		Value2 = Value&Value1;
// 		if (Value2 != 0)
// 		{
// 			m_bSet2 = TRUE;
// 		}
// 		//////////////////////////////////////////////////////////////////////////
// 	}
// 	else
// 	{
// 		CheckResult(_T("ReadRegister(0xF0F00400)"),Result,ErrorGetParam);
// 		return FALSE;
// 	}
	
// 	//////////////////////////////////////////////////////////////////////////
// 	//加载USERSET1
// 	if (m_bSet2)
// 	{
// 		//////////////////////////////////////////////////////////////////////////
// 		//判断相机userset默认是否为USERSET1，如不是，首先加载USERSET1，设为默认
// 		Result = m_Camera.ReadRegister(0xF0F00624,&Value);//Cur_Mem_Ch //(0xF1000550,&Value);//ADV_USERSETS
// 		if(Result==FCE_NOERROR)
// 		{
// 
// 			Value1 = Value&0xf0000000;
// 			Value2 = Value1>>28;
// 
// 			
// 			//如果当前不是USERSET1，首先加载USERSET1
// 			if (Value2 != 0x1)//USERSET1
// 			{
// 				AfxMessageBox(_T("Value2 != 0x1 USERSET1"));
// 				m_Camera.WriteRegister(0xF0F00620,0x10000000);//Mem_Save_Ch
// 				//////////////////////////////////////////////////////////////////////////
// 				Result = m_Camera.WriteRegister(0xF0F00624,0x10000000);//(0xF1000550,0x80000001);//ADV_USERSETS
// 				if(Result==FCE_NOERROR)
// 				{
// 					//////////////////////////////////////////////////////////////////////////
// 				}//////////////////////////////////
// 				else
// 				{
// 					CheckResult(_T("WriteRegister(0xF1000550 0x80000001)"),Result,ErrorGetParam);
// 					return FALSE;
// 				}
// 				//////////////////////////////////////////////////////////////////////////
// 			}///////
// 		}
// 		else
// 		{
// 			CheckResult(_T("ReadRegister(0xF1000550)"),Result,ErrorGetParam);
// 			return FALSE;
// 		}
// 		
// 		//////////////////////////////////////////////////////////////////////////
// 	}


	//////////////////////////////////////////////////////////////////////////
	//获取配置文件中ImageFormat值
		// ImageFormat=7
		//0:160*120
		//1:320*240
		//2:640*480
		//3:800*600
		//4:1024*768
		//5:1280*960
		//6:1600*1200
		//7:Format7Mode0
		//8:Format7Mode1
		//9:Format7Mode2
		//10:Format7Mode3
		//11:Format7Mode4
		//12:Format7Mode5
		//13:Format7Mode6
		//14:Format7Mode7
	UINT32 nImageFormat = GetPrivateProfileInt(_T("相机参数"),_T("ImageFormat"),8,m_sInitFile);
	if (nImageFormat < 0 || nImageFormat > 14)
	{
		CheckResult(_T("SetCameraFromINI::配置文件中 ImageFormat项越界"),nImageFormat,ErrorGetParam);
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	//获取配置文件中Color值
		//	Color=0
		//0:Mono8        CM_Y8
		//1:YUV411       CM_YUV411
		//2:YUV422       CM_YUV422
		//3:YUV444       CM_YUV444
		//4:RGB8         CM_RGB8
		//5:Mono16       CM_Y16
		//6:RGB16        CM_RGB16
		//7:SY16         CM_SY16
		//8:SRGB16       CM_SRGB16
		//9:RAW8         CM_RAW8
		//10:RAW16       CM_RAW16

	UINT32 nColor = GetPrivateProfileInt(_T("相机参数"),_T("Color"),1,m_sInitFile);
	if (nColor < 0 || nColor > 10)
	{
		CheckResult(_T("SetCameraFromINI::配置文件中 Color项越界"),nColor,ErrorGetParam);
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	//获取配置文件中Framerate值
		//Framerate=0
		//0:1_875
		//1:3_75
		//2:7_5
		//3:15
		//4:30
		//5:60
		//6:120
		//7:240
	UINT32 nFramerate = GetPrivateProfileInt(_T("相机参数"),_T("Framerate"),5,m_sInitFile);
	if (nFramerate < 0 || nFramerate > 7)
	{
		CheckResult(_T("SetCameraFromINI::配置文件中 Framerate项越界"),nFramerate,ErrorGetParam);
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	//从配置文件获取相机亮度值		
	UINT32 Brightness=GetPrivateProfileInt(_T("相机参数"),_T("Brightness"),16,m_sInitFile);
	if (Brightness < 0)
	{
		CheckResult(_T("SetCameraFromINI::配置文件中 Brightness项越界"),Brightness,ErrorGetParam);
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	//从配置文件获取相机曝光值
	UINT32	shutter=GetPrivateProfileInt(_T("相机参数"),_T("Shutter"),80,m_sInitFile);
	if (shutter < 0)
	{
		CheckResult(_T("SetCameraFromINI::配置文件中 Shutter项越界"),shutter,ErrorGetParam);
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	//从配置文件获取相机增益值
	UINT32 gain=GetPrivateProfileInt(_T("相机参数"),_T("Gain"),0,m_sInitFile);
	if (gain < 0)
	{
		CheckResult(_T("SetCameraFromINI::配置文件中 gain项越界"),gain,ErrorGetParam);
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	//获取配置文件中left值
	UINT32 nleft = GetPrivateProfileInt(_T("相机参数"),_T("Left"),0,m_sInitFile);
	if (nleft < 0)
	{
		CheckResult(_T("SetCameraFromINI::配置文件中 Left项越界"),nleft,ErrorGetParam);
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	//获取配置文件中width值
	UINT32 nwidth = GetPrivateProfileInt(_T("相机参数"),_T("Width"),640,m_sInitFile);
	if (nwidth < 0)
	{
		CheckResult(_T("SetCameraFromINI::配置文件中 Width项越界"),nwidth,ErrorGetParam);
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	//获取配置文件中top值
	UINT32 ntop = GetPrivateProfileInt(_T("相机参数"),_T("Top"),0,m_sInitFile);
	if (ntop < 0)
	{
		CheckResult(_T("SetCameraFromINI::配置文件中 Top项越界"),ntop,ErrorGetParam);
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	//获取配置文件中height值
	UINT32 nheight = GetPrivateProfileInt(_T("相机参数"),_T("Height"),480,m_sInitFile);
	if (nheight < 0)
	{
		CheckResult(_T("SetCameraFromINI::配置文件中 Height项越界"),nheight,ErrorGetParam);
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	//获取配置文件中Whitebalcb值
	UINT32 nWhitebalcb = GetPrivateProfileInt(_T("相机参数"),_T("Whitebalcb"),0,m_sInitFile);
	if (nWhitebalcb < 0)
	{
		CheckResult(_T("SetCameraFromINI::配置文件中 Whitebalcb项越界"),nWhitebalcb,ErrorGetParam);
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	//获取配置文件中Whitebalcr值
	UINT32 nWhitebalcr = GetPrivateProfileInt(_T("相机参数"),_T("Whitebalcr"),0,m_sInitFile);
	if (nWhitebalcr < 0)
	{
		CheckResult(_T("SetCameraFromINI::配置文件中 Whitebalcr项越界"),nWhitebalcr,ErrorGetParam);
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	//获取配置文件中Packet值
	UINT32 nPacket = GetPrivateProfileInt(_T("相机参数"),_T("Packet"),0,m_sInitFile);
	if (nPacket < 0)
	{
		CheckResult(_T("SetCameraFromINI::配置文件中Packet项越界"),nPacket,ErrorGetParam);
		return FALSE;
	}
	/////////////////////////////////////////////////////////////////////////
	//获取配置文件中Packet值
	m_nMultishotNum = GetPrivateProfileInt(_T("相机参数"),_T("MultishotNum"),14,m_sInitFile);
	//////////////////////////////////////////////////////////////////////////
	//获取相机当前图像格式
	Result=m_Camera.GetParameter(FGP_IMAGEFORMAT,&Value);
	if(Result==FCE_NOERROR)
	{
		UINT32 nRes = IMGRES(Value);
		UINT32 nCol = IMGCOL(Value);
		UINT32 nRate = IMGRATE(Value);

		m_CamParms.ImageFormat = nImageFormat;
		if (nImageFormat < 7)//非format7模式
		{
 			if (nImageFormat != nRes
				||nColor != nCol
				||nFramerate != nRate)
 			{
				UINT32 vTemp = (((UINT32)(UINT8)nImageFormat<<16)|((UINT32)(UINT8)nColor<<8)|((UINT32)(UINT8)nFramerate));
				Result=m_Camera.SetParameter(FGP_IMAGEFORMAT,vTemp);
				if(Result==FCE_NOERROR)
				{
					m_bSet1 = TRUE;
				}
				else
				{
					CheckResult(_T("SetCameraFromINI::SetParameter(FGP_IMAGEFORMAT)"),Result,ErrorSetParam);
					return FALSE;
				}
 			}

			//////////////////////////////////////////////////////////////////////////
			Result=m_Camera.GetParameterInfo(FGP_XSIZE,&Info);
			if(Result==FCE_NOERROR)
			{
				m_CamParms.XSize=Info.IsValue;
				m_CamParms.XSizeMax=Info.MaxValue;
				m_CamParms.XSizeUnit=Info.Unit;
			}
			else if (Result==FCE_NOTAVAILABLE)
			{
				CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_XSIZE)-1"),Result,ErrorGetParam);
			}
			else
			{
				CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_XSIZE)-2"),Result,ErrorGetParam);
				return FALSE;
			}
			
			
			Result=m_Camera.GetParameterInfo(FGP_YSIZE,&Info);
			if(Result==FCE_NOERROR)
			{
				m_CamParms.YSize=Info.IsValue;
				m_CamParms.YSizeMax=Info.MaxValue;
				m_CamParms.YSizeUnit=Info.Unit;
			}
			else if (Result==FCE_NOTAVAILABLE)
			{
				CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_YSIZE)-1"),Result,ErrorGetParam);
			}
			else
			{
				CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_YSIZE)-2"),Result,ErrorGetParam);
				return FALSE;
			}

			//////////////////////////////////////////////////////////////////////////
			
			Result=m_Camera.GetParameterInfo(FGP_XPOSITION,&Info);
			if(Result==FCE_NOERROR)
			{
				m_CamParms.XPos=Info.IsValue;
				m_CamParms.XPosMax=Info.MaxValue;
				m_CamParms.XPosUnit=Info.Unit;
			}
			else if (Result==FCE_NOTAVAILABLE)
			{
				CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_XPOSITION)-1"),Result,ErrorGetParam);
			}
			else
			{
				CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_XPOSITION)-2"),Result,ErrorGetParam);
				return FALSE;
			}
			
			
			Result=m_Camera.GetParameterInfo(FGP_YPOSITION,&Info);
			if(Result==FCE_NOERROR)
			{
				m_CamParms.YPos=Info.IsValue;
				m_CamParms.YPosMax=Info.MaxValue;
				m_CamParms.YPosUnit=Info.Unit;
			}
			else if (Result==FCE_NOTAVAILABLE)
			{
				CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_YPOSITION)-1"),Result,ErrorGetParam);
			}
			else
			{
				CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_YPOSITION)-2"),Result,ErrorGetParam);
				return FALSE;
			}

			//////////////////////////////////////////////////////////////////////////
			//ISO_CHANNEL
			Result = m_Camera.ReadRegister(0xF0F0060C,&Value);//ISO_CHANNEL
			if(Result==FCE_NOERROR)
			{
				//////////////////////////////////////////////////////////////////////////
				if(Value != 0x3008003)//ISO Speed:S800 ISO_CHANNEL:0
				{
					Result = m_Camera.WriteRegister(0xF0F0060C,0x3008003);//ISO_CHANNEL
					if(Result==FCE_NOERROR)
					{
						m_bSet1 = TRUE;
					}
					else
					{
						CheckResult(_T("SetCameraFromINI::WriteRegister(0xF0F0060C,0x3008003)"),Result,ErrorSetParam);
						return FALSE;
					}
				}
				//////////////////////////////////////////////////////////////////////////
			}
			else
			{
				CheckResult(_T("SetCameraFromINI::ReadRegister(0xF0F0060C)"),Result,ErrorGetParam);
				return FALSE;
			}
			//////////////////////////////////////////////////////////////////////////

		}
		else 
		{
			//////////////////////////////////////////////////////////////////////////
			//判断相机是否为format7，如不是，设为format7
			//CUR_V_FORMAT F0F00608
			Result = m_Camera.ReadRegister(0xF0F00608,&Value);// CUR_V_FORMAT //IIDC
			if(Result==FCE_NOERROR)
			{
				//////////////////////////////////////////////////////////////////////////
				Value1 = 0xe0000000;
				Value2 = 0x0;
				Value2 = Value&Value1;
				if (Value2 != 0xe0000000)//not format7
				{
					//A//fxMessageBox(_T("1"));
					Result = m_Camera.WriteRegister(0xF0F00608,0xe0000000);// CUR_V_FORMAT format7
					if(Result==FCE_NOERROR)
					{
						m_bSet1 = TRUE;
					}
					else
					{
						CheckResult(_T("SetCameraFromINI::WriteRegister(0xF0F00608,0xe0000000)"),Result,ErrorSetParam);
						return FALSE;
					}
				}
			}
			else
			{
				CheckResult(_T("SetCameraFromINI::ReadRegister(0xF0F00608)"),Result,ErrorGetParam);
				return FALSE;
			}
			//////////////////////////////////////////////////////////////////////////
			//根据配置文件读取mode(nImageFormat-7)，计算首地址
			//Format7Mode0: F0F002E0h
			//Format7Mode1: F0F002E4h
			//Format7Mode2: F0F002E8h
			//Format7Mode3: F0F002ECh
			//Format7Mode4: F0F002F0h
			//Format7Mode5: F0F002F4h
			//Format7Mode6: F0F002F8h
			//Format7Mode7: F0F002FCh
			//
			UINT32  uFormat7ModeAdr = (nImageFormat-7)*4+0xF0F002E0;
			Result = m_Camera.ReadRegister(uFormat7ModeAdr,&Value);//
			if(Result==FCE_NOERROR)
			{
				m_CamParms.Format7ModeAdress = Value*4+0xF0000000;

// 				CString str;
// 				str.Format(_T("%x"),m_CamParms.Format7ModeAdress);
// 				AfxMessageBox(str);
			}
			else
			{
				CheckResult(_T("SetCameraFromINI::ReadRegister(uFormat7ModeAdr)"),Result,ErrorGetParam);
				return FALSE;
			}

			//////////////////////////////////////////////////////////////////////////
			//判断相机是否为mode(nImageFormat-7)，如不是，设为mode(nImageFormat-7)
			//CUR_V_MODE F0F00604
			Result = m_Camera.ReadRegister(0xF0F00604,&Value);// CUR_V_MODE //IIDC
			if(Result==FCE_NOERROR)
			{
				//////////////////////////////////////////////////////////////////////////
				Value1 = 0xe0000000;
				Value2 = 0x0;
				Value2 = Value&Value1;
				Value1 = Value2>>29;
				Value3 = nImageFormat-7;

				if (Value3 != Value1)//not MODE(nImageFormat-7)
				{
					//AfxMessageBox(_T("2"));
					Value4 = Value3<<29;
					Result = m_Camera.WriteRegister(0xF0F00604,Value4);// CUR_V_MODE MODE0
					if(Result==FCE_NOERROR)
					{
						m_bSet1 = TRUE;
					}
					else
					{
						CheckResult(_T("SetCameraFromINI::WriteRegister(0xF0F00604)"),Result,ErrorSetParam);
						return FALSE;
					}
				}
			}
			else
			{
				CheckResult(_T("SetCameraFromINI::ReadRegister(0xF0F00604)"),Result,ErrorGetParam);
				return FALSE;
			}
			//////////////////////////////////////////////////////////////////////////
			if (nColor != nCol)
			{
				UINT32 uAdr = m_CamParms.Format7ModeAdress+0x10;
				if (nColor == 0)//0:Mono8
				{
					//////////////////////////////////////////////////////////////////////////
					
					Result = m_Camera.ReadRegister(uAdr,&Value);//F7M0_COLOR_CODING_ID
					if(Result==FCE_NOERROR)
					{
						//////////////////////////////////////////////////////////////////////////
						if(Value != 0x0)//Mono8
						{
							//AfxMessageBox(_T("3"));
							Result = m_Camera.WriteRegister(uAdr,0x0);//F7M0_COLOR_CODING_ID
							if(Result==FCE_NOERROR)
							{
								m_bSet1 = TRUE;
							}
							else
							{
								CheckResult(_T("SetCameraFromINI::WriteRegister(uAdr,0x0)"),Result,ErrorSetParam);
								return FALSE;
							}
						}
						//////////////////////////////////////////////////////////////////////////
					}
					else
					{
						CheckResult(_T("SetCameraFromINI::ReadRegister(uAdr)"),Result,ErrorGetParam);
						return FALSE;
					}
				}
				else if (nColor == 1 && m_bIsColorCamera)//1:YUV411
				{
					//////////////////////////////////////////////////////////////////////////
					Result = m_Camera.ReadRegister(uAdr,&Value);//F7M0_COLOR_CODING_ID
					if(Result==FCE_NOERROR)
					{
						if(Value != 0x1000000)//YUV411
						{
							Result = m_Camera.WriteRegister(uAdr,0x1000000);//F7M0_COLOR_CODING_ID
							if(Result==FCE_NOERROR)
							{
								m_bSet1 = TRUE;
							}
							else
							{
								CheckResult(_T("SetCameraFromINI::WriteRegister(uAdr,0x1000000)"),Result,ErrorSetParam);
								return FALSE;
							}
						}
					}
					else
					{
						CheckResult(_T("SetCameraFromINI::ReadRegister(uAdr)-1"),Result,ErrorGetParam);
						return FALSE;
					}
				}
				else if (nColor == 2 && m_bIsColorCamera)//2:YUV422
				{
					//////////////////////////////////////////////////////////////////////////
					Result = m_Camera.ReadRegister(uAdr,&Value);//F7M0_COLOR_CODING_ID
					if(Result==FCE_NOERROR)
					{
						if(Value != 0x2000000)//2:YUV422
						{
							Result = m_Camera.WriteRegister(uAdr,0x2000000);//F7M0_COLOR_CODING_ID
							if(Result==FCE_NOERROR)
							{
								m_bSet1 = TRUE;
							}
							else
							{
								CheckResult(_T("SetCameraFromINI::WriteRegister(uAdr,0x2000000)"),Result,ErrorSetParam);
								return FALSE;
							}
						}
					}
					else
					{
						CheckResult(_T("SetCameraFromINI::ReadRegister(uAdr)-2"),Result,ErrorGetParam);
						return FALSE;
					}
				}
				else if (nColor == 3 && m_bIsColorCamera)//3:YUV444
				{
					//////////////////////////////////////////////////////////////////////////
					Result = m_Camera.ReadRegister(uAdr,&Value);//F7M0_COLOR_CODING_ID
					if(Result==FCE_NOERROR)
					{
						if(Value != 0x3000000)//3:YUV444
						{
							Result = m_Camera.WriteRegister(uAdr,0x3000000);//F7M0_COLOR_CODING_ID
							if(Result==FCE_NOERROR)
							{
								m_bSet1 = TRUE;
							}
							else
							{
								CheckResult(_T("SetCameraFromINI::ReadRegister(0xF0F08010)"),Result,ErrorSetParam);
								return FALSE;
							}
						}
					}
					else
					{
						CheckResult(_T("SetCameraFromINI::ReadRegister(uAdr)-3"),Result,ErrorGetParam);
						return FALSE;
					}
				}
				else if (nColor == 4 && m_bIsColorCamera)//4:RGB8
				{
					//////////////////////////////////////////////////////////////////////////
					Result = m_Camera.ReadRegister(uAdr,&Value);//F7M0_COLOR_CODING_ID
					if(Result==FCE_NOERROR)
					{
						if(Value != 0x4000000)//4:RGB8
						{
							Result = m_Camera.WriteRegister(uAdr,0x4000000);//F7M0_COLOR_CODING_ID
							if(Result==FCE_NOERROR)
							{
								m_bSet1 = TRUE;
							}
							else
							{
								CheckResult(_T("SetCameraFromINI::WriteRegister(uAdr,0x4000000)"),Result,ErrorSetParam);
								return FALSE;
							}
						}
					}
					else
					{
						CheckResult(_T("SetCameraFromINI::ReadRegister(uAdr)-4"),Result,ErrorGetParam);
						return FALSE;
					}
				}
				else if (nColor == 5)// && m_bIsColorCamera)//5:Mono16
				{
					//////////////////////////////////////////////////////////////////////////
					Result = m_Camera.ReadRegister(uAdr,&Value);//F7M0_COLOR_CODING_ID
					if(Result==FCE_NOERROR)
					{
						if(Value != 0x5000000)//5:Mono16
						{
							Result = m_Camera.WriteRegister(uAdr,0x5000000);//F7M0_COLOR_CODING_ID
							if(Result==FCE_NOERROR)
							{
								m_bSet1 = TRUE;
							}
							else
							{
								CheckResult(_T("SetCameraFromINI::WriteRegister(uAdr,0x5000000)"),Result,ErrorSetParam);
								return FALSE;
							}
						}
					}
					else
					{
						CheckResult(_T("SetCameraFromINI::ReadRegister(uAdr)-5"),Result,ErrorGetParam);
						return FALSE;
					}
				}
				else if (nColor == 6 && m_bIsColorCamera)//6:RGB16
				{
					//////////////////////////////////////////////////////////////////////////
					Result = m_Camera.ReadRegister(uAdr,&Value);//F7M0_COLOR_CODING_ID
					if(Result==FCE_NOERROR)
					{
						if(Value != 0x6000000)//6:RGB16
						{
							Result = m_Camera.WriteRegister(uAdr,0x6000000);//F7M0_COLOR_CODING_ID
							if(Result==FCE_NOERROR)
							{
								m_bSet1 = TRUE;
							}
							else
							{
								CheckResult(_T("SetCameraFromINI::WriteRegister(uAdr,0x6000000)"),Result,ErrorSetParam);
								return FALSE;
							}
						}
					}
					else
					{
						CheckResult(_T("SetCameraFromINI::ReadRegister(uAdr)-6"),Result,ErrorGetParam);
						return FALSE;
					}
				}
				else if (nColor == 7 )//&& m_bIsColorCamera)//7:SY16
				{
					//////////////////////////////////////////////////////////////////////////
					Result = m_Camera.ReadRegister(uAdr,&Value);//F7M0_COLOR_CODING_ID
					if(Result==FCE_NOERROR)
					{
						if(Value != 0x7000000)//7:SY16
						{
							Result = m_Camera.WriteRegister(uAdr,0x7000000);//F7M0_COLOR_CODING_ID
							if(Result==FCE_NOERROR)
							{
								m_bSet1 = TRUE;
							}
							else
							{
								CheckResult(_T("SetCameraFromINI::WriteRegister(uAdr,0x7000000)"),Result,ErrorSetParam);
								return FALSE;
							}
						}
					}
					else
					{
						CheckResult(_T("SetCameraFromINI::ReadRegister(uAdr)-7"),Result,ErrorGetParam);
						return FALSE;
					}
				}
				else if (nColor == 8 && m_bIsColorCamera)//8:SRGB16
				{
					//////////////////////////////////////////////////////////////////////////
					Result = m_Camera.ReadRegister(uAdr,&Value);//F7M0_COLOR_CODING_ID
					if(Result==FCE_NOERROR)
					{
						if(Value != 0x8000000)//8:SRGB16
						{
							Result = m_Camera.WriteRegister(uAdr,0x8000000);//F7M0_COLOR_CODING_ID
							if(Result==FCE_NOERROR)
							{
								m_bSet1 = TRUE;
							}
							else
							{
								CheckResult(_T("SetCameraFromINI::WriteRegister(uAdr,0x8000000)"),Result,ErrorSetParam);
								return FALSE;
							}
						}
					}
					else
					{
						CheckResult(_T("SetCameraFromINI::ReadRegister(uAdr)-8"),Result,ErrorGetParam);
						return FALSE;
					}
				}
				else if (nColor == 9 && m_bIsColorCamera)//9:RAW8
				{
					//////////////////////////////////////////////////////////////////////////
					Result = m_Camera.ReadRegister(uAdr,&Value);//F7M0_COLOR_CODING_ID
					if(Result==FCE_NOERROR)
					{
						if(Value != 0x9000000)//RAW8
						{
							Result = m_Camera.WriteRegister(uAdr,0x9000000);//F7M0_COLOR_CODING_ID
							if(Result==FCE_NOERROR)
							{
								m_bSet1 = TRUE;
							}
							else
							{
								CheckResult(_T("SetCameraFromINI::WriteRegister(uAdr,0x9000000)"),Result,ErrorSetParam);
								return FALSE;
							}
						}
					}
					else
					{
						CheckResult(_T("SetCameraFromINI::ReadRegister(uAdr)-9"),Result,ErrorGetParam);
						return FALSE;
					}
				}
				else if (nColor == 10 && m_bIsColorCamera)//10:RAW16
				{
					//////////////////////////////////////////////////////////////////////////
					Result = m_Camera.ReadRegister(uAdr,&Value);//F7M0_COLOR_CODING_ID
					if(Result==FCE_NOERROR)
					{
						if(Value != 0xa000000)//10:RAW16
						{
							Result = m_Camera.WriteRegister(uAdr,0xa000000);//F7M0_COLOR_CODING_ID
							if(Result==FCE_NOERROR)
							{
								m_bSet1 = TRUE;
							}
							else
							{
								CheckResult(_T("SetCameraFromINI::WriteRegister(uAdr,0xa000000)"),Result,ErrorSetParam);
								return FALSE;
							}
						}
					}
					else
					{
						CheckResult(_T("SetCameraFromINI::ReadRegister(uAdr)-10"),Result,ErrorGetParam);
						return FALSE;
					}
				}
				else
				{
					CheckResult(_T("目前采集库不支持这种图像格式"),Result,ErrorGetParam);
					return FALSE;
				}
			}

			//////////////////////////////////////////////////////////////////////////
			//////////////////////////////////////////////////////////////////////////
			//ISO_CHANNEL
			Result = m_Camera.ReadRegister(0xF0F0060C,&Value);//ISO_CHANNEL ISO Speed //IIDC
			if(Result==FCE_NOERROR)
			{
				//////////////////////////////////////////////////////////////////////////
				if(Value != 0x3008003)//ISO Speed:S800 ISO_CHANNEL:0
				{
					//AfxMessageBox(_T("10"));
					Result = m_Camera.WriteRegister(0xF0F0060C,0x3008003);//ISO_CHANNEL
					if(Result==FCE_NOERROR)
					{
						m_bSet1 = TRUE;
					}
					else
					{
						CheckResult(_T("SetCameraFromINI::WriteRegister(0xF0F0060C,0x3008003)"),Result,ErrorSetParam);
						return FALSE;
					}
				}
				//////////////////////////////////////////////////////////////////////////
			}
			else
			{
				CheckResult(_T("SetCameraFromINI::ReadRegister(0xF0F0060C)"),Result,ErrorGetParam);
				return FALSE;
			}
			//////////////////////////////////////////////////////////////////////////


			if (nRes == 7)
			{
				for (int i = 0; i < 2; i++)
				{
				//////////////////////////////////////////////////////////////////////////
					//获取图像位置信息
					Result=m_Camera.GetParameterInfo(FGP_XPOSITION,&Info);
					if(Result==FCE_NOERROR && Info.Unit != 0)
					{
						m_CamParms.XPosMax=Info.MaxValue;
						m_CamParms.XPosMin=Info.MinValue;
						m_CamParms.XPosUnit=Info.Unit;

					
						if (nleft > m_CamParms.XPosMax)
						{
							nleft = m_CamParms.XPosMax;
						}
						if (nleft < m_CamParms.XPosMin)
						{
							nleft = m_CamParms.XPosMin;
						}
						if (nleft%m_CamParms.XPosUnit != 0)
						{
							nleft-=nleft % m_CamParms.XPosUnit;
						}
					
					}
					else if (Result==FCE_NOTAVAILABLE || Info.Unit == 0)
					{
						CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_XPOSITION)-1"),Result,ErrorGetParam);
					}
					else
					{
						CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_XPOSITION)-2"),Result,ErrorGetParam);
						return FALSE;
					}
				
				
					Result=m_Camera.GetParameterInfo(FGP_YPOSITION,&Info);
					if(Result==FCE_NOERROR&& Info.Unit != 0)
					{
						m_CamParms.YPosMax=Info.MaxValue;
						m_CamParms.YPosMin=Info.MinValue;
						m_CamParms.YPosUnit=Info.Unit;
					
						if (ntop > m_CamParms.YPosMax)
						{
							ntop = m_CamParms.YPosMax;
						}
						if (ntop < m_CamParms.YPosMin)
						{
							ntop = m_CamParms.YPosMin;
						}
						if (ntop%m_CamParms.YPosUnit != 0)
						{
							ntop-=ntop % m_CamParms.YPosUnit;
						}
					
					}
					else if (Result==FCE_NOTAVAILABLE || Info.Unit == 0)
					{
						CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_YPOSITION)-1"),Result,ErrorGetParam);
					}
					else
					{
						CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_YPOSITION)-2"),Result,ErrorGetParam);
						return FALSE;
					}
					//////////////////////////////////////////////////////////////////////////

					//获取相机图像尺寸信息
					Result=m_Camera.GetParameterInfo(FGP_XSIZE,&Info);
					if(Result==FCE_NOERROR)
					{
						m_CamParms.XSizeMax=Info.MaxValue;
						m_CamParms.XSizeMin=Info.MinValue;
						m_CamParms.XSizeUnit=Info.Unit;

						if (nwidth > m_CamParms.XSizeMax)
						{
							nwidth = m_CamParms.XSizeMax;
						}

						if (nwidth < m_CamParms.XSizeMin)
						{
							nwidth = m_CamParms.XSizeMin;
						}

						if (nwidth%m_CamParms.XSizeUnit != 0)
						{
							nwidth-=nwidth % m_CamParms.XSizeUnit;
						}

						if (nwidth + nleft > m_CamParms.XSizeMax)
						{
							CheckResult(_T("SetCameraFromINI::nwidth + nleft > m_CamParms.XSizeMax"),Result,ErrorGetParam);
							return FALSE;
						}
					}
					else if (Result==FCE_NOTAVAILABLE)
					{
						CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_XSIZE)-1"),Result,ErrorGetParam);
					}
					else
					{
						CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_XSIZE)-2"),Result,ErrorGetParam);
						return FALSE;
					}

					Result=m_Camera.GetParameterInfo(FGP_YSIZE,&Info);
					if(Result==FCE_NOERROR)
					{
						m_CamParms.YSizeMax=Info.MaxValue;
						m_CamParms.YSizeMin=Info.MinValue;
						m_CamParms.YSizeUnit=Info.Unit;

						if (nheight > m_CamParms.YSizeMax)
						{
							nheight = m_CamParms.YSizeMax;
						}
					
						if (nheight < m_CamParms.YSizeMin)
						{
							nheight = m_CamParms.YSizeMin;
						}

						if (nheight%m_CamParms.YSizeUnit != 0)
						{
							nheight-=nheight % m_CamParms.YSizeUnit;
						}


						if (nheight + ntop > m_CamParms.YSizeMax)
						{
							CheckResult(_T("SetCameraFromINI::nheight + ntop > m_CamParms.YSizeMax"),Result,ErrorGetParam);
							return FALSE;
						}
					}
					else if (Result==FCE_NOTAVAILABLE)
					{
						CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_YSIZE)-1"),Result,ErrorGetParam);
					}
					else
					{
						CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_YSIZE)-2"),Result,ErrorGetParam);
						return FALSE;
					}

					//////////////////////////////////////////////////////////////////////////
					////获取 IMAGE_SIZE
					UINT32  nTemp = nImageFormat-7;
					UINT32  nTemp1 = nTemp<<12;
					UINT32 Address = m_CamParms.Format7ModeAdress+0xC+nTemp1;//0xF0F0800C+nTemp1;
			
					Result = m_Camera.ReadRegister(Address,&Value);//IMAGE_SIZE
					if(Result==FCE_NOERROR)
					{
						Value1 = 0xffff0000;
						Value2 = 0x0;
						Value2 = Value&Value1;
						Value1 = Value2>>16;
						m_CamParms.XSize=Value1;
					
						Value1 = 0xffff;
						Value2 = 0x0;
						Value2 = Value&Value1;
						m_CamParms.YSize=Value2;
					}
					else
					{
						CheckResult(_T("SetCameraFromINI::ReadRegister(Address)-1"),Result,ErrorGetParam);
						return FALSE;
					}

					//////////////////////////////////////////////////////////////////////////
					//获取F7M0_IMAGE_POSITION
					nTemp = nImageFormat-7;
					nTemp1 = nTemp<<12;
					Address = m_CamParms.Format7ModeAdress+0x8+nTemp1;//0xF0F08008+nTemp1;
				
					Result = m_Camera.ReadRegister(Address,&Value);//F7M0_IMAGE_POSITION
					if(Result==FCE_NOERROR)
					{
						Value1 = 0xffff0000;
						Value2 = 0x0;
						Value2 = Value&Value1;
						Value1 = Value2>>16;
						m_CamParms.XPos=Value1;
			
						Value1 = 0xffff;
						Value2 = 0x0;
						Value2 = Value&Value1;
						m_CamParms.YPos=Value2;

						//////////////////////////////////////////////////////////////////////////
						///根据配置文件设置AOI区域
						if (nwidth != m_CamParms.XSize || nleft != m_CamParms.XPos)
						{
							if(nwidth + m_CamParms.XPos <= m_CamParms.XSizeMax)
							{
								if (nwidth != m_CamParms.XSize)
								{
									//AfxMessageBox(_T("11"));
									Result = m_Camera.SetParameter(FGP_XSIZE,nwidth);
									if(Result==FCE_NOERROR)
									{
										m_CamParms.XSize = nwidth;
										//m_bSet1 = TRUE;
									}
									else
									{
										str.Format(_T("SetCameraFromINI::SetCameraFromINI::SetParameter(FGP_XSIZE,%d)"),nwidth);
										CheckResult(str,Result,ErrorGetParam);
										return FALSE;
									}
								}


								if (nleft != m_CamParms.XPos)
								{
									//AfxMessageBox(_T("12"));
									Result = m_Camera.SetParameter(FGP_XPOSITION,nleft);
									if(Result==FCE_NOERROR)
									{
										m_CamParms.XPos = nleft;
										//m_bSet1 = TRUE;
									}
									else
									{
										str.Format(_T("SetCameraFromINI::SetParameter(FGP_XPOSITION,%d)"),nleft);
										CheckResult(str,Result,ErrorGetParam);
										return FALSE;
									}
								}
							}
							else
							{
								if (nleft != m_CamParms.XPos)
								{
									//AfxMessageBox(_T("13"));
									Result = m_Camera.SetParameter(FGP_XPOSITION,nleft);
									if(Result==FCE_NOERROR)
									{
										m_CamParms.XPos = nleft;
										//m_bSet1 = TRUE;
									}
									else
									{
									//	CheckResult(_T("WriteRegister(IMAGE_SIZE)"),Result,ErrorGetParam);
										str.Format(_T("SetCameraFromINI::SetParameter(FGP_XPOSITION,%d)"),nleft);
										CheckResult(str,Result,ErrorGetParam);
										return FALSE;
									}
								}


								if (nwidth != m_CamParms.XSize)
								{
									//AfxMessageBox(_T("14"));
									Result = m_Camera.SetParameter(FGP_XSIZE,nwidth);
									if(Result==FCE_NOERROR)
									{
										m_CamParms.XSize = nwidth;
										//m_bSet1 = TRUE;
									}
									else
									{
									//	CheckResult(_T("WriteRegister(IMAGE_SIZE)"),Result,ErrorGetParam);
										str.Format(_T("SetCameraFromINI::SetParameter(FGP_XSIZE,%d)"),nwidth);
										CheckResult(str,Result,ErrorGetParam);
										return FALSE;
									}
								}
							}
						}


						if (nheight != m_CamParms.YSize || ntop != m_CamParms.YPos)
						{
							if(nheight + m_CamParms.YPos <= m_CamParms.YSizeMax)
							{
								if (nheight != m_CamParms.YSize)
								{
									//CString str11;
									//str11.Format(_T("15:  %d,%d"),nheight,m_CamParms.YSize);
									//AfxMessageBox(str11);
									Result = m_Camera.SetParameter(FGP_YSIZE,nheight);
									if(Result==FCE_NOERROR)
									{
										m_CamParms.YSize = nheight;
										//m_bSet1 = TRUE;
									}
									else
									{
									//	CheckResult(_T("WriteRegister(IMAGE_SIZE)"),Result,ErrorGetParam);
										str.Format(_T("SetCameraFromINI::SetParameter(FGP_YSIZE,%d)"),nheight);
										CheckResult(str,Result,ErrorGetParam);
										return FALSE;
									}
								}

								if (ntop != m_CamParms.YPos)
								{
									//CString str11;
									//str11.Format(_T("16:  %d,%d"),ntop,m_CamParms.YPos);
									//AfxMessageBox(str11);
									//AfxMessageBox(_T("16"));
									Result = m_Camera.SetParameter(FGP_YPOSITION,ntop);
									if(Result==FCE_NOERROR)
									{
										m_CamParms.YPos = ntop;
										//m_bSet1 = TRUE;
									}
									else
									{
									//	CheckResult(_T("WriteRegister(IMAGE_SIZE)"),Result,ErrorGetParam);
										str.Format(_T("SetCameraFromINI::SetParameter(FGP_YPOSITION,%d)"),ntop);
										CheckResult(str,Result,ErrorGetParam);
										return FALSE;
									}
								}
							}
							else
							{
								if (ntop != m_CamParms.YPos)
								{
									//CString str11;
									//str11.Format(_T("17:  %d,%d"),ntop,m_CamParms.YPos);
									//AfxMessageBox(str11);
									Result = m_Camera.SetParameter(FGP_YPOSITION,ntop);
									if(Result==FCE_NOERROR)
									{
										m_CamParms.YPos = ntop;
										//m_bSet1 = TRUE;
									}
									else
									{
									//	CheckResult(_T("WriteRegister(IMAGE_SIZE)"),Result,ErrorGetParam);
										str.Format(_T("SetCameraFromINI::SetParameter(FGP_YPOSITION,%d)"),ntop);
										CheckResult(str,Result,ErrorSetParam);
										return FALSE;
									}
								}

								if (nheight != m_CamParms.YSize)
								{
									//CString str11;
									//str11.Format(_T("18:  %d,%d"),nheight,m_CamParms.YSize);
									//AfxMessageBox(str11);
									Result = m_Camera.SetParameter(FGP_YSIZE,nheight);
									if(Result==FCE_NOERROR)
									{
										m_CamParms.YSize = nheight;
										//m_bSet1 = TRUE;
									}
									else
									{
									//	CheckResult(_T("WriteRegister(IMAGE_SIZE)"),Result,ErrorGetParam);
										str.Format(_T("SetCameraFromINI::SetParameter(FGP_YSIZE,%d)"),nheight);
										CheckResult(str,Result,ErrorSetParam);
										return FALSE;
									}
								}
							}
						}
					}
					else
					{
		//				AfxMessageBox(_T("ReadRegister(0xF0F08008)"));
						CheckResult(_T("SetCameraFromINI::ReadRegister(Address)-2"),Result,ErrorGetParam);
						return FALSE;
					}

					//////////////////////////////////////////////////////////////////////////
					//F7M0_BYTE_PER_PACKET
					nTemp = nImageFormat-7;
					nTemp1 = nTemp<<12;
					Address = m_CamParms.Format7ModeAdress+0x44+nTemp1;//0xF0F08044+nTemp1;
					
					Result = m_Camera.ReadRegister(Address,&Value);//F7M0_BYTE_PER_PACKET
					if(Result==FCE_NOERROR)
					{
						//////////////////////////////////////////////////////////////////////////
						Value1 = 0xffff0000;
						Value2 = 0x0;
						Value2 = Value&Value1;
						Value1 = Value2>>16;
			
						Value3 = 0xffff;
						Value4 = 0x0;
						Value4= Value&Value3;

						//////////////////////////////////////////////////////////////////////////
						//
						if (nPacket == 0)
						{
							if(Value1 != Value4)
							{
								//AfxMessageBox(_T("20"));
								Value1 = Value4<<16;
								Value1 +=Value4;
								//	UINT32 Value5 = 0x40000+Value4;
								//	Result = m_Camera.WriteRegister(0xF0F08048,0x102);
								//	Result = m_Camera.WriteRegister(0xF0F08040,Value5);
								Result = m_Camera.WriteRegister(Address,Value1);//F7M0_BYTE_PER_PACKET
								//	Result = m_Camera.SetParameter(FGP_PACKETSIZE,Value4);
						
								if(Result==FCE_NOERROR)
								{
									//////////////////////////////////////////////////////////////////////////
									//m_bSet1 = TRUE;
									//////////////////////////////////////////////////////////////////////////
								}
								else
								{
									CheckResult(_T("SetCameraFromINI::WriteRegister(Address)-1"),Result,ErrorSetParam);
									return FALSE;
								}	
								//////////////////////////////////////////////////////////////////////////
							}
						}
						else
						{
							if(Value1 != nPacket)
							{
								Value1 = nPacket<<16;
								Value1 +=Value4;
								//	UINT32 Value5 = 0x40000+Value4;
								//AfxMessageBox(_T("21"));
							
								//	Result = m_Camera.WriteRegister(0xF0F08048,0x102);
								//	Result = m_Camera.WriteRegister(0xF0F08040,Value5);
								Result = m_Camera.WriteRegister(Address,Value1);//F7M0_BYTE_PER_PACKET
								//	Result = m_Camera.SetParameter(FGP_PACKETSIZE,Value4);
							
								if(Result==FCE_NOERROR)
								{
									//////////////////////////////////////////////////////////////////////////
									//m_bSet1 = TRUE;
									//////////////////////////////////////////////////////////////////////////
								}
								else
								{
									//	AfxMessageBox(_T("WriteRegister(0xF0F08044)"));
									CheckResult(_T("SetCameraFromINI::WriteRegister(Address)-2"),Result,ErrorSetParam);
									return FALSE;
								}	
							}	
						}
					}
					else
					{
		//				AfxMessageBox(_T("ReadRegister(0xF0F08044)"));
						CheckResult(_T("SetCameraFromINI::ReadRegister(Address)-3"),Result,ErrorGetParam);
						return FALSE;
					}
				}
			}//m_nInitNum == 1
			//////////////////////////////////////////////////////////////////////////
		}//Format7模式

		//////////////////////////////////////////////////////////////////////////
		//公共设置
		m_nImageWidth  = m_CamParms.XSize;
		m_nImageHeight = m_CamParms.YSize;

//0:Mono8
//1:YUV411
//2:YUV422
//3:YUV444
//4:RGB8
//5:Mono16
//6:RGB16
//7:SY16
//8:SRGB16
//9:RAW8
//10:RAW16
//		if(nColor == 0
//			||nColor == 9
//			|| nColor == 4
//			)
//		{
//			m_nImageByteCount = 1;
//		}
//		else if(nColor == 1
//			|| nColor == 2
//			|| nColor == 5
//			)
//		{
//			m_nImageByteCount = 2;
//		}
		if (m_bIsColorCamera == FALSE)
		{
			m_nImageByteCount = 1;
		}
		else if (m_bIsColorCamera == TRUE)
		{
			m_nImageByteCount = 3;
		}
		
		m_nImageBufferSize = m_nImageWidth * m_nImageHeight * m_nImageByteCount; 

		if(m_pbImageBufferAdd != NULL)
		{
			delete [](m_pbImageBufferAdd);
			m_pbImageBufferAdd = NULL;
		}

	//	m_pbImageBufferAdd = new BYTE[m_nImageWidth * m_nImageHeight*3];

	}
	else if (Result==FCE_NOTAVAILABLE)
	{
		CheckResult(_T("SetCameraFromINI::GetParameter(FGP_IMAGEFORMAT)-1"),Result,ErrorGetParam);
	}
	else
	{
		CheckResult(_T("SetCameraFromINI::GetParameter(FGP_IMAGEFORMAT)-2"),Result,ErrorGetParam);
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	
	if (m_bIsColorCamera == TRUE) // 黑白相机则不需要读取 
	{

		//获得相机里面的白平衡参数
		//获取白平衡CB分量
		Result=m_Camera.GetParameterInfo(FGP_WHITEBALCB,&Info);
		if(Result==FCE_NOERROR)
		{
			m_CamParms.WhiteBalanceCB=Info.IsValue;
			m_CamParms.WhiteBalanceMaxCB=Info.MaxValue;
			m_CamParms.WhiteBalanceMinCB=Info.MinValue;

			if (nWhitebalcb < m_CamParms.WhiteBalanceMinCB)
			{
				nWhitebalcb = m_CamParms.WhiteBalanceMinCB;
			}
			else if (nWhitebalcb > m_CamParms.WhiteBalanceMaxCB)
			{
				nWhitebalcb = m_CamParms.WhiteBalanceMaxCB;
			}

			if(nWhitebalcb != m_CamParms.WhiteBalanceCB)
			{
				Result=m_Camera.SetParameter(FGP_WHITEBALCB,nWhitebalcb);
				if (Result==FCE_NOERROR)
				{
					m_CamParms.WhiteBalanceCB=nWhitebalcb;
				}
				else
				{
					CheckResult(_T("SetCameraFromINI::SetParameter(FGP_WHITEBALCB)"),Result,ErrorSetParam);
					return FALSE;
				}
			}

		}
		else if (Result==FCE_NOTAVAILABLE)
		{
			CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_WHITEBALCB)-1"),Result,ErrorGetParam);
		}
		else
		{
			CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_WHITEBALCB)-2"),Result,ErrorGetParam);
			return FALSE;
		}

		
		 //获取白平衡CR分量
	
		Result=m_Camera.GetParameterInfo(FGP_WHITEBALCR,&Info);
		if(Result==FCE_NOERROR)
		{
			m_CamParms.WhiteBalanceCR=Info.IsValue;
			m_CamParms.WhiteBalanceMaxCR=Info.MaxValue;
			m_CamParms.WhiteBalanceMinCR=Info.MinValue;

			if (nWhitebalcr < m_CamParms.WhiteBalanceMinCR)
			{
				nWhitebalcr = m_CamParms.WhiteBalanceMinCR;
			}
			else if (nWhitebalcr > m_CamParms.WhiteBalanceMaxCR)
			{
				nWhitebalcr = m_CamParms.WhiteBalanceMaxCR;
			}

			if(nWhitebalcr != m_CamParms.WhiteBalanceCR)
			{
				Result=m_Camera.SetParameter(FGP_WHITEBALCR,nWhitebalcr);
				if (Result==FCE_NOERROR)
				{
					m_CamParms.WhiteBalanceCR=nWhitebalcr;
				}
				else
				{
					CheckResult(_T("SetCameraFromINI::SetParameter(FGP_WHITEBALCR)"),Result,ErrorSetParam);
					return FALSE;
				}
			}
		}
		else if (Result==FCE_NOTAVAILABLE)
		{
			CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_WHITEBALCR)-1"),Result,ErrorGetParam);
		}
		else
		{
			CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_WHITEBALCR)-2"),Result,ErrorGetParam);
			return FALSE;
		}

	}	

	//////////////////////////////////////////////////////////////////////////
	//从配置文件获取相机触发模式
	m_CamParms.Trigger	= GetPrivateProfileInt(_T("相机参数"),_T("Trigger"),0,m_sInitFile);
	if (m_CamParms.Trigger < 0 || m_CamParms.Trigger > 9)
	{
		CheckResult(_T("SetCameraFromINI::m_CamParms->Trigger < 0 || m_CamParms->Trigger > 6"),m_CamParms.Trigger,ErrorGetParam);
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////
	//获取配置文件中Debounce值
	UINT32 nDebounce = GetPrivateProfileInt(_T("相机参数"),_T("Debounce"),3,m_sInitFile);
	if (nDebounce < 0)
	{
		CheckResult(_T("SetCameraFromINI::配置文件中Debounce项越界"),nDebounce,ErrorGetParam);
		return FALSE;
	}
	//////////////////////////////////////////////////////////////////////////

	//获取相机当前触发模式
	Result = m_Camera.ReadRegister(0xF0F00830,&Value);
	if(Result!=FCE_NOERROR)
	{
		CheckResult(_T("SetCameraFromINI::ReadRegister(0xF0F00830"),Result,ErrorGetParam);
		return FALSE;
	}

	//判断相机当前触发模式是否与配置文件一致，不一致，按配置文件值设置
	switch(m_CamParms.Trigger) 
	{
	case 0://AVTTriggerOff: 
		{
			if (Value != 0x80100000)
			{
				Result = m_Camera.WriteRegister(0xF0F00830,0x80100000);//Internal
				if(Result!=FCE_NOERROR)
				{
					CheckResult(_T("SetCameraFromINI::WriteRegister(0xF0F00830,0x80100000)"),Result,ErrorSetParam);
					return FALSE;
				}
				else
				{
					m_bSet1 = TRUE;
				}
			}

			m_bSnapShot = FALSE;
		}
		break;
	case 1://AVTTriggerOn:
		{
			if (Value != 0x83100000)
			{
				Result = m_Camera.WriteRegister(0xF0F00830,0x83100000);//Edge Mode(0)-Ext Rising
				if(Result!=FCE_NOERROR)
				{
					CheckResult(_T("SetCameraFromINI::WriteRegister(0xF0F00830,0x83100000)"),Result,ErrorSetParam);
					return FALSE;
				}
				else
				{
					m_bSet1 = TRUE;
				}
			}

			m_bSnapShot = FALSE;
		}
		break;
	case 2://AVTTriggerMode2:
		{
			if (Value != 0x82100000)
			{
				Result = m_Camera.WriteRegister(0xF0F00830,0x82100000);//Edge Mode(0)-Ext Falling
				if(Result!=FCE_NOERROR)
				{
					CheckResult(_T("SetCameraFromINI::WriteRegister(0xF0F00830,0x82100000)"),Result,ErrorSetParam);
					return FALSE;
				}
				else
				{
					m_bSet1 = TRUE;
				}
			}

			m_bSnapShot = FALSE;
		}
		break;
	case 3://AVTTriggerMode3:
		{
			if (Value != 0x83110000)
			{
				Result = m_Camera.WriteRegister(0xF0F00830,0x83110000);//Level Mode(1)-Ext High act
				if(Result!=FCE_NOERROR)
				{
					CheckResult(_T("SetCameraFromINI::WriteRegister(0xF0F00830,0x83110000)"),Result,ErrorSetParam);
					return FALSE;
				}
				else
				{
					m_bSet1 = TRUE;
				}
			}

			m_bSnapShot = FALSE;
		}
		break;
	case 4://AVTTriggerMode4:
		{
			if (Value != 0x82110000)
			{
				Result = m_Camera.WriteRegister(0xF0F00830,0x82110000);//Level Mode(1)-Ext Low act
				if(Result!=FCE_NOERROR)
				{
					CheckResult(_T("SetCameraFromINI::SetParameter(0xF0F00830,0x82110000)"),Result,ErrorSetParam);
					return FALSE;
				}
				else
				{
					m_bSet1 = TRUE;
				}
			}

			m_bSnapShot = FALSE;
		}
		break;
	case 5://AVTTriggerMode5:
		{
			if (Value != 0x831F0000)
			{
				Result = m_Camera.WriteRegister(0xF0F00830,0x831F0000);//Progr. Mode(15)-Ext Rising
				if(Result!=FCE_NOERROR)
				{
					CheckResult(_T("SetCameraFromINI::SetParameter(0xF0F00830,0x831F0000)"),Result,ErrorSetParam);
					return FALSE;
				}
				else
				{
					m_bSet1 = TRUE;
				}
			}

			m_bSnapShot = FALSE;
		}
		break;
	case 6://AVTTriggerMode6:
		{
			if (Value != 0x821F0000)
			{
				Result = m_Camera.WriteRegister(0xF0F00830,0x821F0000);//Progr. Mode(15)-Ext Falling
				if(Result!=FCE_NOERROR)
				{
					CheckResult(_T("SetCameraFromINI::SetParameter(0xF0F00830,0x821F0000)"),Result,ErrorSetParam);
					return FALSE;
				}
				else
				{
					m_bSet1 = TRUE;
					
				}
			}

			m_bSnapShot = FALSE;
		}
		break;
	case 7://AVTTriggerMode7:
		{
			if (Value != 0x82E00000)
			{
				Result = m_Camera.WriteRegister(0xF0F00830,0x82E00000);//Edge Mode(0)-SW
				if(Result!=FCE_NOERROR)
				{
					CheckResult(_T("SetCameraFromINI::SetParameter(0xF0F00830,0x82E00000)"),Result,ErrorSetParam);
					return FALSE;
				}
				else
				{
					m_bSet1 = TRUE;
					
				}
			}

			m_bSnapShot = TRUE;
		}
		break;
	case 8://AVTTriggerMode8:
		{
			if (Value != 0x82E10000)
			{
				Result = m_Camera.WriteRegister(0xF0F00830,0x82E10000);//Level Mode(1)-SW
				if(Result!=FCE_NOERROR)
				{
					CheckResult(_T("SetCameraFromINI::SetParameter(0xF0F00830,0x82E10000)"),Result,ErrorSetParam);
					return FALSE;
				}
				else
				{
					m_bSet1 = TRUE;
					
				}
			}

			m_bSnapShot = TRUE;
		}
		break;
	case 9://AVTTriggerMode9:
		{
			if (Value != 0x82EF0000)
			{
				Result = m_Camera.WriteRegister(0xF0F00830,0x82EF0000);//Progr. Mode(15)-SW
				if(Result!=FCE_NOERROR)
				{
					CheckResult(_T("SetCameraFromINI::SetParameter(0xF0F00830,0x82EF0000)"),Result,ErrorSetParam);
					return FALSE;
				}
				else
				{
					m_bSet1 = TRUE;
				}
			}

			m_bSnapShot = TRUE;
		}
		break;
	default:
		{
			Result = m_Camera.WriteRegister(0xF0F00830,0x80100000);//Internal
			if(Result!=FCE_NOERROR)
			{
				CheckResult(_T("SetCameraFromINI::SetParameter(0xF0F00830,0x80100000)"),Result,ErrorSetParam);
				return FALSE;
			}
			m_bSnapShot = FALSE;
		}
		break;
	}


	//获取相机当前DebounceMin
	Result = m_Camera.ReadRegister(0xF1000844,&Value);
	if(Result!=FCE_NOERROR)
	{
		CheckResult(_T("SetCameraFromINI::ReadRegister(0xF1000844"),Result,ErrorSetParam);
		return FALSE;
	}
	else
	{
		m_CamParms.DebounceMin = Value;
	}

	//获取相机当前DebounceMax
	Result = m_Camera.ReadRegister(0xF1000848,&Value);
	if(Result!=FCE_NOERROR)
	{
		CheckResult(_T("SetCameraFromINI::ReadRegister(0xF1000848"),Result,ErrorSetParam);
		return FALSE;
	}
	else
	{
		m_CamParms.DebounceMax = Value;
	}

	if (nDebounce < m_CamParms.DebounceMin)
	{
		nDebounce = m_CamParms.DebounceMin;
	}

	if (nDebounce > m_CamParms.DebounceMax)
	{
		nDebounce = m_CamParms.DebounceMin;
	}

	//获取相机当前Debounce
	Result = m_Camera.ReadRegister(0xF1000840,&Value);
	if(Result!=FCE_NOERROR)
	{
		CheckResult(_T("SetCameraFromINI::ReadRegister(0xF1000840"),Result,ErrorSetParam);
		return FALSE;
	}
	else
	{
		Value5 = Value&0xffffff;
		m_CamParms.Debounce = nDebounce;
		if (nDebounce != Value5)
		{
			//AfxMessageBox(_T("30"));
			Value5 = 0x80000000+nDebounce;
			Result = m_Camera.WriteRegister(0xF1000840,Value5);
			if(Result!=FCE_NOERROR)
			{
				CheckResult(_T("SetCameraFromINI::WriteRegister(0xF1000840)"),Result,ErrorSetParam);
				return FALSE;
			}
			else
			{
				m_bSet1 = TRUE;
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	//从相机获取亮度信息
	Result=m_Camera.GetParameterInfo(FGP_BRIGHTNESS,&Info);
	if(Result==FCE_NOERROR)
	{
		m_CamParms.BrightnessMin=Info.MinValue;
		m_CamParms.BrightnessMax=Info.MaxValue;

		//获取相机当前亮度值
		Result = m_Camera.ReadRegister(0xF0F00800,&Value);
		if(Result!=FCE_NOERROR)
		{
			CheckResult(_T("SetCameraFromINI::ReadRegister(0xF0F00800"),Result,ErrorSetParam);
			return FALSE;
		}
		m_CamParms.Brightness = Value&0xffff;

		if (Brightness < m_CamParms.BrightnessMin)
		{
			Brightness = m_CamParms.BrightnessMin;
		}
		
		if (Brightness > m_CamParms.BrightnessMax)
		{
			Brightness = m_CamParms.BrightnessMax;
		}
		
		if (Brightness != m_CamParms.Brightness)
		{
			//AfxMessageBox(_T("31"));
			UINT32 uBrightness = 0x82000000 + Brightness;
			Result = m_Camera.WriteRegister(0xF0F00800,uBrightness);
			if(Result!=FCE_NOERROR)
			{
				CheckResult(_T("SetCameraFromINI::WriteRegister(0xF0F00800"),Result,ErrorSetParam);
				return FALSE;
			}
			else
			{
				m_bSet1 = TRUE;
				m_CamParms.Brightness = Brightness;
			}


// 			Result = m_Camera.SetParameter(FGP_BRIGHTNESS,Brightness);
// 			if(Result!=FCE_NOERROR)
// 			{
// 				CheckResult(_T("SetParameter(FGP_BRIGHTNESS)"),Result,ErrorSetParam);
// 				return FALSE;
// 			}
// 			else
// 			{
// 				m_bSet1 = TRUE;
// 			}
		}
	}
	else if (Result==FCE_NOTAVAILABLE)
	{
		CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_BRIGHTNESS)"),Result,ErrorGetParam);
	}
	else
	{
		CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_BRIGHTNESS)"),Result,ErrorGetParam);
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	//从相机获取相机曝光信息
	Result=m_Camera.GetParameterInfo(FGP_SHUTTER,&Info);
	if(Result==FCE_NOERROR)
	{
		m_CamParms.ShutterMin=Info.MinValue;
		m_CamParms.ShutterMax=Info.MaxValue;

		//获取相机当前曝光值
		Result = m_Camera.ReadRegister(0xF0F0081C,&Value);
		if(Result!=FCE_NOERROR)
		{
			CheckResult(_T("SetCameraFromINI::ReadRegister(0xF0F0081C"),Result,ErrorSetParam);
			return FALSE;
		}
		m_CamParms.Shutter = Value&0xffff;


		if (shutter < m_CamParms.ShutterMin)
		{
			shutter = m_CamParms.ShutterMin;
		}
		
		if (shutter > m_CamParms.ShutterMax)
		{
			shutter = m_CamParms.ShutterMax;
		}
		
		if (shutter != m_CamParms.Shutter)
		{
			//AfxMessageBox(_T("32"));
			UINT32 uShutter = 0x82000000 + shutter;
			Result = m_Camera.WriteRegister(0xF0F0081C,uShutter);
			if(Result!=FCE_NOERROR)
			{
				CheckResult(_T("SetCameraFromINI::WriteRegister(0xF0F0081C"),Result,ErrorSetParam);
				return FALSE;
			}
			else
			{
				m_bSet1 = TRUE;
				m_CamParms.Shutter = shutter;
			}

// 			Result = m_Camera.SetParameter(FGP_SHUTTER,shutter);
// 			if(Result!=FCE_NOERROR)
// 			{
// 				CheckResult(_T("SetParameter(FGP_SHUTTER)"),Result,ErrorSetParam);
// 				return FALSE;
// 			}
// 			else
// 			{
// 				m_bSet1 = TRUE;
// 			}
		}
	} 
	else if (Result==FCE_NOTAVAILABLE)
	{
		CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_SHUTTER)"),Result,ErrorGetParam);
	}
	else
	{
		CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_SHUTTER)"),Result,ErrorGetParam);
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	//从相机获取相机增益信息
	Result=m_Camera.GetParameterInfo(FGP_GAIN,&Info);
	if(Result==FCE_NOERROR)
	{
		m_CamParms.GainMin=Info.MinValue;
		m_CamParms.GainMax=Info.MaxValue;

		//获取相机当前增益值
		Result = m_Camera.ReadRegister(0xF0F00820,&Value);
		if(Result!=FCE_NOERROR)
		{
			CheckResult(_T("SetCameraFromINI::ReadRegister(0xF0F00820"),Result,ErrorSetParam);
			return FALSE;
		}
		m_CamParms.Gain = Value&0xffff;


		if (gain < m_CamParms.GainMin)
		{
			gain = m_CamParms.GainMin;
		}
		
		if (gain > m_CamParms.GainMax)
		{
			gain = m_CamParms.GainMax;
		}
		
		if (gain != m_CamParms.Gain)
		{
			//AfxMessageBox(_T("33"));
			UINT32 uGain = 0x82000000 + gain;
			Result = m_Camera.WriteRegister(0xF0F00820,uGain);
			if(Result!=FCE_NOERROR)
			{
				CheckResult(_T("SetCameraFromINI::WriteRegister(0xF0F00820"),Result,ErrorSetParam);
				return FALSE;
			}
			else
			{
				m_bSet1 = TRUE;
				m_CamParms.Gain=gain;
			}

// 			Result = m_Camera.SetParameter(FGP_GAIN,gain);
// 			if(Result!=FCE_NOERROR)
// 			{
// 				CheckResult(_T("SetParameter(FGP_GAIN)"),Result,ErrorSetParam);
// 				return FALSE;
// 			}
// 			else
// 			{
// 				m_bSet1 = TRUE;
// 			}
		}
	} 
	else if (Result==FCE_NOTAVAILABLE)
	{
		CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_GAIN)-1"),Result,ErrorGetParam);
	}
	else
	{
		CheckResult(_T("SetCameraFromINI::GetParameterInfo(FGP_GAIN)-2"),Result,ErrorGetParam);
		return FALSE;
	}


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//设置图像队列数
	UINT32 uBufferCount = 0;
	Result=m_Camera.GetParameter(FGP_FRAMEBUFFERCOUNT,&uBufferCount);
	if (uBufferCount != 16)
	{
		//AfxMessageBox(_T("35"));
		Result=m_Camera.SetParameter(FGP_FRAMEBUFFERCOUNT,16);
		if(Result==FCE_NOERROR)
		{
			m_bSet1 = TRUE;
		}
		else if (Result==FCE_NOTAVAILABLE)
		{
			CheckResult(_T("SetCameraFromINI::SetParameter(FGP_FRAMEBUFFERCOUNT)-1"),Result,ErrorGetParam);
		}
		else
		{
			CheckResult(_T("SetCameraFromINI::SetParameter(FGP_FRAMEBUFFERCOUNT)-2"),Result,ErrorGetParam);
			return FALSE;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//设置连续采集
	UINT32 uBurstCount = -1;
	Result=m_Camera.GetParameter(FGP_BURSTCOUNT,&uBurstCount);
	if (uBurstCount != BC_INFINITE)
	{
		//AfxMessageBox(_T("34"));
		Result = m_Camera.SetParameter(FGP_BURSTCOUNT,BC_INFINITE);	
		if(Result==FCE_NOERROR)
		{
		}
		else if (Result==FCE_NOTAVAILABLE)
		{
			CheckResult(_T("SetCameraFromINI::SetParameter(FGP_BURSTCOUNT)-1"),Result,ErrorGetParam);
		}
		else
		{
			CheckResult(_T("SetCameraFromINI::SetParameter(FGP_BURSTCOUNT)-2"),Result,ErrorSetParam);
			return FALSE;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//设置 DMA模式为REPLACE
	// Set DMA mode to replace so we don't halt our DMA
	UINT32 uDMAMode = -1;
	Result=m_Camera.GetParameter(FGP_DMAMODE,&uDMAMode);
	if (uDMAMode != DMA_REPLACE)
	{
		//AfxMessageBox(_T("36"));
		Result=m_Camera.SetParameter(FGP_DMAMODE,DMA_REPLACE);
		if(Result!=FCE_NOERROR)
		{
			CheckResult(_T("SetCameraFromINI::SetParameter(FGP_DMAMODE)"),Result,ErrorSetParam);
			return FALSE;
		}
	}
		//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//判断相机是否有掉电保存参数的flash，即USERSET
	/*Result = m_Camera.ReadRegister(0xF0F00400,&Value);// USERSET num 
	if(Result==FCE_NOERROR)
	{
		//////////////////////////////////////////////////////////////////////////
		Value1 = 0xf;
		Value2 = 0x0;
		Value2 = Value&Value1;
		if (Value2 != 0)
		{
			m_bSet2 = TRUE;
		}
		//////////////////////////////////////////////////////////////////////////
	}
	else
	{
		CheckResult(_T("SetCameraFromINI::ReadRegister(0xF0F00400)"),Result,ErrorGetParam);
		return FALSE;
	}*/
	

	//////////////////////////////////////////////////////////////////////////
	//参数有变化，保存到USERSET1
	/*if (m_bSet2 && m_bSet1)
	{
		m_bSet2 = FALSE;
		m_bSet1 = FALSE;

	//	AfxMessageBox(_T("123"));


		//////////////////////////////////////////////////////////////////////////
		//判断相机userset默认是否为USERSET1，如不是，首先加载USERSET1，设为默认
		Result = m_Camera.ReadRegister(0xF1000550,&Value);//ADV_USERSETS
		if(Result==FCE_NOERROR)
		{

			//如果当前不是USERSET1，首先加载USERSET1
			if (Value != 0x80000001)//USERSET1
			{
				//AfxMessageBox(_T("38"));
				//////////////////////////////////////////////////////////////////////////
				Result = m_Camera.WriteRegister(0xF1000550,0x80000001);//ADV_USERSETS
				if(Result==FCE_NOERROR)
				{
					//////////////////////////////////////////////////////////////////////////
				}//////////////////////////////////
				else
				{
					CheckResult(_T("SetCameraFromINI::WriteRegister(0xF1000550 0x80000001)"),Result,ErrorGetParam);
					return FALSE;
				}
				//////////////////////////////////////////////////////////////////////////
			}///////
			
			//
			//AfxMessageBox(_T("39"));
			//////////////////////////////////////////////////////////////////////////
			Result = m_Camera.WriteRegister(0xF1000550,0x80800001);//save USERSET1 0x80400001);//load USERSET1
			//参数有变化，保存到USERSET1
			if(Result==FCE_NOERROR)
			{
				//AfxMessageBox(_T("40"));
				//////////////////////////////////////////////////////////////////////////
				Result = m_Camera.WriteRegister(0xF1000550,0x80200001);//set USERSET1 default
				if(Result==FCE_NOERROR)
				{
					//////////////////////////////////////////////////////////////////////////
					//	AfxMessageBox(_T("123456"));
					//////////////////////////////////////////////////////////////////////////
				}
				else
				{
					CheckResult(_T("SetCameraFromINI::WriteRegister(0xF1000550 0x80200001)"),Result,ErrorGetParam);
					return FALSE;
				}
				//////////////////////////////////////////////////////////////////////////
			}
			else
			{
				CheckResult(_T("SetCameraFromINI::WriteRegister(0xF1000550 0x80400001)"),Result,ErrorGetParam);
				return FALSE;
			}
			//////////////////////////////////////////////////////////////////////////
		}
		else
		{
			CheckResult(_T("SetCameraFromINI::ReadRegister(0xF1000550)"),Result,ErrorGetParam);
			return FALSE;
		}

//        m_nInitNum++;
//		if(m_nInitNum == 1)
//		{
//				m_Camera.Disconnect();// 断开与相机的连接
////
//				if (!InitCamera())
//				{
//					m_InitSucceed = FALSE;
//// 					m_LastErrorInfo.nErrorCode = ErrorInit;  // 加入出错描述
//// 					_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("初始化相机失败!"));
//// 					_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("Reset()函数"));
//					CheckResult(_T("SetCameraFromINI::InitCamera"),0,ErrorSetParam);
//					return FALSE;
//				}
//		}
		
		//////////////////////////////////////////////////////////////////////////
	}*/


// 	if(Result!=FCE_NOERROR)
// 	{
// 		return FALSE;
// 	}	
	return TRUE;
}


void CAVTCamDH::CheckResult(CString pFunction,DWORD Result,int n)
{
    TCHAR cbuf[300];
	
    wsprintf(cbuf,_T("Eerror:%s=%d"),pFunction,Result);
	
	m_LastErrorInfo.iErrorCode = n;
	_tprintf_s(m_LastErrorInfo.strErrorDescription, cbuf);
    _tprintf_s(m_LastErrorInfo.strErrorRemark, _T("CheckResult"));	
}


BOOL CAVTCamDH::CreateDefaultConfigurationFile(CString &sInitFile)
{
	BOOL b = FALSE;
	b = CreateMultiLevelPath(sInitFile);
	if (!b)
	{
		return FALSE;
	}

	CString str;
	str.Format(_T("7"));
	b = WritePrivateProfileString(_T("相机参数"),_T("ImageFormat"),str,sInitFile);
	if (!b)
	{
		return FALSE;
	}

	str.Format(_T("0"));
	b = WritePrivateProfileString(_T("相机参数"),_T("Color"),str,sInitFile);
	if (!b)
	{
		return FALSE;
	}

	str.Format(_T("0"));
	b = WritePrivateProfileString(_T("相机参数"),_T("Framerate"),str,sInitFile);
	if (!b)
	{
		return FALSE;
	}

	str.Format(_T("0"));
	b = WritePrivateProfileString(_T("相机参数"),_T("Trigger"),str,sInitFile);
	if (!b)
	{
		return FALSE;
	}

	str.Format(_T("16"));
	b = WritePrivateProfileString(_T("相机参数"),_T("Brightness"),str,sInitFile);
	if (!b)
	{
		return FALSE;
	}

	str.Format(_T("90"));
	b = WritePrivateProfileString(_T("相机参数"),_T("Shutter"),str,sInitFile);
	if (!b)
	{
		return FALSE;
	}

	str.Format(_T("0"));
	b = WritePrivateProfileString(_T("相机参数"),_T("Gain"),str,sInitFile);	
	if (!b)
	{
		return FALSE;
	}

	str.Format(_T("0"));
	b = WritePrivateProfileString(_T("相机参数"),_T("Left"),str,sInitFile);
	if (!b)
	{
		return FALSE;
	}

	str.Format(_T("656"));
	b = WritePrivateProfileString(_T("相机参数"),_T("Width"),str,sInitFile);
	if (!b)
	{
		return FALSE;
	}

	str.Format(_T("0"));
	b = WritePrivateProfileString(_T("相机参数"),_T("Top"),str,sInitFile);
	if (!b)
	{
		return FALSE;
	}

	str.Format(_T("492"));
	b = WritePrivateProfileString(_T("相机参数"),_T("Height"),str,sInitFile);
	if (!b)
	{
		return FALSE;
	}

	str.Format(_T("0"));
	b = WritePrivateProfileString(_T("相机参数"),_T("Whitebalcb"),str,sInitFile);
	if (!b)
	{
		return FALSE;
	}

	str.Format(_T("0"));
	b = WritePrivateProfileString(_T("相机参数"),_T("Whitebalcr"),str,sInitFile);
	if (!b)
	{
		return FALSE;
	}

	str.Format(_T("0"));
	b = WritePrivateProfileString(_T("相机参数"),_T("Packet"),str,sInitFile);
	if (!b)
	{
		return FALSE;
	}

	str.Format(_T("3"));
	b = WritePrivateProfileString(_T("相机参数"),_T("Debounce"),str,sInitFile);
	if (!b)
	{
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
// 	CString sNewPsw;
// 	char cc[100];
// 	sNewPsw.Format(_T("123"));
// 	int n2 = sNewPsw.GetLength();
// 	for (int j = 0; j <n2; j++)
// 	{
// 		cc[j] = sNewPsw.GetAt(j);
// 		if (isalnum(cc[j])  == 0)
// 		{
// 			return FALSE;
// 		}
// 		else
// 		{
// 			cc[j] -=8;//加密
// 		}
// 	}
// 	cc[n2] = '\0';
// 	sNewPsw.Format(_T("%s"),cc);
// 	b = WritePrivateProfileString(_T("相机参数"),_T("Psw"),sNewPsw,sInitFile);
// 	if (!b)
// 	{
// 		return FALSE;
// 	}

	return TRUE;
}

BOOL CAVTCamDH::SaveToConfigurationFile(CString &sInitFile)
{
	return TRUE;
}


BOOL CAVTCamDH::Reset()
{
	if (m_bSet5)
	{
		return TRUE;
	}
	m_bSet5 = TRUE;

	BOOL bGrabbering = FALSE;
	if (m_CameraActive)
	{
	//	CheckResult(_T("Reset::正在采集,不能根据配置文件初始化相机。"),0,ErrorGetParam);
	//	return FALSE;	
		StopGrab();
		bGrabbering = TRUE;
	}

	UINT32 Result;
	UINT32 Value;
	UINT32 Value1 = 0x80000000,Value2;

	Result = m_Camera.Disconnect();// 断开与相机的连接
// 	if (Result!=FCE_NOERROR)
// 	{
// 		CheckResult(_T("Reset::Disconnect-1"),Result,ErrorGetParam);
// 		return FALSE;
// 	}
	Result = m_Camera.Connect(&m_pCameraInfo->Guid,this);
// 	if (Result!=FCE_NOERROR)
// 	{
// 		CheckResult(_T("Reset::Connect-1"),Result,ErrorGetParam);
// 		return FALSE;
// 	}
	
	//判断相机是否支持SOFT_RESET功能 
	Result = m_Camera.ReadRegister(0xF1000510,&Value);
	if(Result==FCE_NOERROR)
	{
		Value2 = Value&Value1;
		if (Value2 != 0x80000000)
		{
			CheckResult(_T("Reset::ReadRegister(0xF1000510 0x80000000)"),Result,ErrorGetParam);
			return FALSE;
		}
	}
	else
	{
		CheckResult(_T("Reset::ReadRegister(0xF1000510)"),Result,ErrorGetParam);
		return FALSE;
	}

	

	//启动SOFT_RESET 
	Result = m_Camera.WriteRegister(0xF1000510,0x82000014);//SOFT_RESET
	if(Result==FCE_NOERROR)
	{
	}
	else
	{
		CheckResult(_T("Reset::WriteRegister(0xF1000510 0x82000014)"),Result,ErrorSetParam);
		return FALSE;
	}

	//等待SOFT_RESET完成，一般10秒内完成


	int nTemp = 0;
	while(nTemp < 10)//最大等待时间是10*3000=30秒
	{
		Sleep(3000);
		nTemp++;
		Result = m_Camera.Disconnect();// 断开与相机的连接
// 		if (Result!=FCE_NOERROR)
// 		{
// 			CheckResult(_T("Reset::Disconnect-2"),Result,ErrorGetParam);
// 			return FALSE;
// 		}
		Result = m_Camera.Connect(&m_pCameraInfo->Guid,this);
		if (Result==FCE_NOERROR)
		{
			m_Camera.Disconnect();
			break;
		}
	}

//	m_NodelistChangedEvent.ResetEvent();
//	m_bSet4 = TRUE;
//	::WaitForSingleObject(m_NodelistChangedEvent, INFINITE);//等待相机列表变化
//	m_NodelistChangedEvent.ResetEvent();
	

	//根据配置文件初始化相机
	//初始化相机
	//m_nInitNum = 0;
	if (!InitCamera())
	{
		m_InitSucceed = FALSE;
	//	m_LastErrorInfo.nErrorCode = ErrorInit;  // 加入出错描述
	//	_tprintf_s(m_LastErrorInfo.strErrorDescription, _T("初始化相机失败!"));
	//	_tprintf_s(m_LastErrorInfo.strErrorRemark, _T("Reset()函数"));

		CheckResult(_T("Reset::InitCamera"),0,ErrorInit);
		return FALSE;
	}

	if (bGrabbering)
	{
		bGrabbering = FALSE;
		StartGrab();
	}

	m_bSet5 = FALSE;
	return TRUE;
}



//判断路径是否存在
BOOL CAVTCamDH::IsPathExist(const CString &strPath)
{
	WIN32_FIND_DATA fd; 
	HANDLE hFind = FindFirstFile(strPath, &fd); 
	if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{ 
		FindClose(hFind);
		return TRUE; 
	} 
	else
	{
		FindClose(hFind); 
		return FALSE;
	}
}

//判断文件是否存在
BOOL CAVTCamDH::IsFileExist(const CString &strFileName)
{
	CFileFind searcher;
	
	if(searcher.FindFile(strFileName))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//创建一个多层目录，如果存在就不创建
//(strPath  合法: _T("D:\\abc\\me") 或 _T("me\\you") 或 _T(""); 
//			不合法: _T("D:\\abc\\me\\") 或 _T("me\\you\\") 或 _T("\\"))
BOOL CAVTCamDH::CreateMultiLevelPath(const CString &strPath)
{
	CString tmpPath = strPath;
	CList<CString,CString&> FolderStrList;
	int nStrLen = 0;

	int nPos = 0;
	nPos = tmpPath.ReverseFind('\\');//从后往前确定文件夹名
	if(nPos == -1)//已到达第一个文件夹名
	{
		nPos = 0;//将第一个文件夹名入队
	}
	
	tmpPath = tmpPath.Left(nPos);
	
	for(;;)
	{
		nStrLen = tmpPath.GetLength();
		if(nStrLen <= 0)
		{
			break;//从头建文件夹
		}		
		
		BOOL bPathExist = FALSE;//该路径是否已经存在
		bPathExist = IsPathExist(tmpPath);
		if(bPathExist)//如果路径存在
		{
			break;//从已存在的路径下建文件夹
		}
		
		//如果路径不存在		
		//每次将最后的一个文件夹名入队
		
		int nPos = 0;
		nPos = tmpPath.ReverseFind('\\');//从后往前确定文件夹名
		if(nPos == -1)//已到达第一个文件夹名
		{
			nPos = 0;//将第一个文件夹名入队
		}
		
		//将文件夹名入队
		CString tmpStr = tmpPath.Right(nStrLen - nPos);
		FolderStrList.AddHead(tmpStr);
		tmpPath.Delete(nPos, nStrLen - nPos);		
	}	
	
	//根据文件夹名队列顺序建立
	while(FolderStrList.IsEmpty() == FALSE)
	{
		tmpPath += FolderStrList.RemoveHead();
		BOOL bRet = FALSE;
		bRet = CreateDirectory(tmpPath, NULL);
		if(bRet == FALSE)
		{
			int n = tmpPath.Find(_T("\\"));
			if (n != -1)
			{
				return FALSE;
			}

		}
	}
	
	return TRUE;
}


//int CAVTCamDH::TestFramerate()
//{
//	UINT32 Value = 0x0;
//	//获取相机当前触发模式
//	UINT32 Result = m_Camera.ReadRegister(0xF0F00830,&Value);
//	if(Result!=FCE_NOERROR)
//	{
//		CheckResult(_T("SetCameraFromINI::ReadRegister(0xF0F00830"),Result,ErrorGetParam);
//		return 1;
//	}
//
//	if (Value != 0x80100000)
//	{
//		Result = m_Camera.WriteRegister(0xF0F00830,0x80100000);//Internal
//		if(Result!=FCE_NOERROR)
//		{
//			CheckResult(_T("SetCameraFromINI::WriteRegister(0xF0F00830,0x80100000)"),Result,ErrorSetParam);
//			return 2;
//		}
//	}
//
//	m_bTestFramerate = TRUE;
//	StartGrab();
//	Sleep(1000);
//	StopGrab();
//	m_bTestFramerate = FALSE;
//	if (m_FrameCnt == 0)
//	{
//		Reset();
//	}
//
//	if (Value != 0x80100000)
//	{
//		Result = m_Camera.WriteRegister(0xF0F00830,Value);
//		if(Result!=FCE_NOERROR)
//		{
//			CheckResult(_T("SetCameraFromINI::WriteRegister(0xF0F00830,0x80100000)"),Result,ErrorSetParam);
//			return FALSE;
//		}
//	}
//
//	return 0;
//}


//////////////////////////////////////////////////////////////////////////
// end of all

