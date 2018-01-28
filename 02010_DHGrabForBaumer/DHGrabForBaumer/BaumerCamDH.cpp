// DALSACamDH.cpp: implementation of the CBaumerCamDH class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "GrabberForBaumer.h"
#include "BaumerCamDH.h"
#include "SetDlg.h"




//#include "winsock2.h"
//
//#include "ws2tcpip.h"
//
//#pragma comment(lib,"IPHLPAPI.lib")
//
//
//#include "iphlpapi.h" 
//#include "stdio.h"
//#include "stdlib.h"
//
//#define MALLOC(x) HeapAlloc(GetProcessHeap(),0,(x))
//#define FREE(x) HeapFree(GetProcessHeap(),0,(x))

//#include "Wininet.h"  
//#include "Sensapi.h" 
//#pragma comment(lib, "Sensapi.lib")


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
using namespace std; 

//////////////////////////////////////////////////////////////////////////
//回调函数
BGAPI_RESULT BGAPI_CALLBACK CBaumerCamDH::imageCallback( void * callBackOwner, BGAPI::Image* pImage  )
{
	CBaumerCamDH *This = (CBaumerCamDH *)callBackOwner;
	BGAPI_RESULT res = BGAPI_RESULT_OK;
	unsigned char* imagebuffer = NULL;
	
	pImage->get( &imagebuffer );
	//Now you have the Imagebuffer and can do with it whatever you want
	This->m_pbImageBufferAdd = (void *)(imagebuffer);
	
	This->m_FrameCnt++;
//	int swc = 0;
//	int hwc = 0;
//	pImage->getNumber( &swc, &hwc );
//	TRACE("%d---Image number %d.\r\n", This->m_nGrabberSN,swc );
	s_GBSIGNALINFO GrabInfo;                    //回调信息结构   
	GrabInfo.Context = This->m_Context;
	GrabInfo.iGrabberTypeSN = 10;
	GrabInfo.iGrabberSN  = This->m_nGrabberSN;
	GrabInfo.iFrameCount = This->m_FrameCnt;
	GrabInfo.iErrorCode = NormalState;
	_tprintf_s(GrabInfo.strDescription, "正常");			
	This->m_CallBackFunc(&GrabInfo);            //回调函数

	//after you are ready with this image, return it to the camera for the next image
	res = This->m_pCamera->setImage( pImage );
	if( res != BGAPI_RESULT_OK )
		TRACE( "setImage failed with %d\n", res );
	
	return res;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CBaumerCamDH::CBaumerCamDH()
{
	m_FrameCnt    = 0;

	//-------相机设置参数------
	m_nImageWidth = 640;
	m_nImageHeight= 480;
	m_nImageBufferSize = 307200;
	m_nImageByteCount  = 1;
	m_pbImageBufferAdd = NULL; 
	m_bSet1 = FALSE;
	m_bSet2 = FALSE;
	m_bSet3 = FALSE;
	m_bGrabbing = FALSE; // 是否正在采集
	m_bInitSucceed = FALSE;
	m_bSquencerShot = FALSE;
	m_nSequencerNum = 14;
	m_nExMax = 0;
	m_nExMin = 0;
	m_nGainMax = 0;
	m_nGainMin = 0;
	m_pCamera = NULL;
	m_pSystem = NULL;

	m_ndoSequencerReset = 0;


	for (int i = 0; i < SystemMaxNum; i++)
	{
		m_ppSystem[i] = NULL;
	}

	for (int i = 0; i < CameraMaxNum; i++)
	{
		m_SystemCamera[i].m_pSystem = NULL;
		m_SystemCamera[i].m_pCamera = NULL;
	}

	for (int i = 0; i < ImageMaxNum; i++)
	{
		m_pImage[i] = NULL;
	}

	m_CameraNum = 0;

}

CBaumerCamDH::~CBaumerCamDH()
{
	BGAPI_RESULT res = BGAPI_RESULT_FAIL;

	for (int i = 0; i < CameraMaxNum; i++ )
	{
		if (m_SystemCamera[i].m_pSystem != NULL && m_SystemCamera[i].m_pCamera != NULL)
		{
			res = m_SystemCamera[i].m_pSystem->releaseCamera( m_SystemCamera[i].m_pCamera);
			if( res != BGAPI_RESULT_OK && res != BGAPI_RESULT_ALREADYDONE)
			{
				CString str;
				str.Format("相机序号:%d,releaseCamera Errorcode: %d\n",m_nGrabberSN,res);
				TRACE("releaseCamera Errorcode: %d\n", res );
				m_LastErrorInfo.iErrorCode = ErrorClose;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "Close()函数");
			}
		}
	}
	
	//
	for (int i = 0; i < SystemMaxNum; i++ )
	{
		if (m_ppSystem[i] != NULL)
		{
			m_ppSystem[i]->release();
		}
	}


	//
	for (int i = 0; i <ImageMaxNum; i++)
	{
		if (m_pImage[i] != NULL)
		{
			res = BGAPI::releaseImage( m_pImage[i] );
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,release_images Errorcode: %d\n",m_nGrabberSN,res);
				TRACE("release_images Errorcode: %d\n", res );
				m_LastErrorInfo.iErrorCode = ErrorClose;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "Close()函数");
			}
		}
	} 
}

//////////////////////////////////////////////////////////////////////////
//

BOOL CBaumerCamDH::Init(const s_GBINITSTRUCT* pInitParam)
{
	s_BAUMER_INITSTRUCT InitCardParam;
	InitCardParam.nGrabberSN = pInitParam->iGrabberSN;
	_tprintf_s(InitCardParam.strGrabberFile, pInitParam->strGrabberFile);
	_tprintf_s(InitCardParam.strDeviceName, pInitParam->strDeviceName);
	_tprintf_s(InitCardParam.strDeviceMark, pInitParam->strDeviceMark);
	InitCardParam.CallBackFunc = pInitParam->CallBackFunc;
	InitCardParam.Context = pInitParam->Context;
	
	BOOL bRet = InitParm(&InitCardParam);
	
	return bRet;
}

bool CBaumerCamDH::Init(const CGrabInitInfo & rGrabInitInfo)
{
	s_BAUMER_INITSTRUCT InitCardParam;
	InitCardParam.nGrabberSN = rGrabInitInfo.iGrabSN;
	_tprintf_s(InitCardParam.strGrabberFile, rGrabInitInfo.lpszPathFileOfGrab);
	GetPrivateProfileString(_T("Grabber Parameters"), _T("GrabberName"), _T(""), InitCardParam.strDeviceName, GBMaxTextLen, rGrabInitInfo.lpszPathFileOfGrab);
	GetPrivateProfileString(_T("Grabber Parameters"), _T("GrabberMark"), _T(""), InitCardParam.strDeviceMark, GBMaxTextLen, rGrabInitInfo.lpszPathFileOfGrab);
// 	_tprintf_s(InitCardParam.strDeviceName, rGrabInitInfo.DeviceName);
// 	_tprintf_s(InitCardParam.strDeviceMark, rGrabInitInfo.strDeviceMark);
	InitCardParam.CallBackFunc = rGrabInitInfo.CallBackFunc;
	InitCardParam.Context = rGrabInitInfo.Context;
	return true;
}

BOOL CBaumerCamDH::InitParm(const s_BAUMER_INITSTRUCT* pInitParam)
{
	m_nGrabberSN   = pInitParam->nGrabberSN;			//序号
	m_CallBackFunc = pInitParam->CallBackFunc;			//回调函数指针
	m_Context      = pInitParam->Context;				//存放初始化函数的对象的this指针
	m_sInitFile	   = pInitParam->strGrabberFile;		//采集卡配置文件(CCF文件)
	m_sDeviceName  = pInitParam->strDeviceName;			//设备名称
	m_strDeviceMark	= pInitParam->strDeviceMark;		// 设备标识
	
	m_bGrabbing = FALSE;
	
	if(m_sInitFile == "")
	{
		m_LastErrorInfo.iErrorCode = ErrorReadIniFile;
		CString str;
		str.Format("相机序号:%d,配置文件路径为空，读取配置文件失败",m_nGrabberSN);
		_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::Init()函数");
		m_bInitSucceed = FALSE;
		return FALSE; 
	}

 	if(m_nGrabberSN < 0)
 	{
		m_LastErrorInfo.iErrorCode = ErrorGrabberSN;
		CString str;
		str.Format("相机序号:%d,采集需要超过允许的范围，采集编号错误",m_nGrabberSN);
		_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
 		_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::Init()函数");
		m_bInitSucceed = FALSE;
 		return FALSE;
 	}
     
	if(m_strDeviceMark == "")
	{
		m_LastErrorInfo.iErrorCode = ErrorGetCamInfo;
		CString str;
		str.Format("相机序号:%d,得到相机信息错误，设备标识为空",m_nGrabberSN);
		_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::InitParm()函数");
		m_bInitSucceed = FALSE;
		return FALSE;
	}

	//////////////////////////////////////////////////////////////////////////
	//	
	
	int system_count = 0;
	BGAPI_RESULT res = BGAPI_RESULT_FAIL;

	//init the system like shown in bgapi_init example
	res = init_systems( &system_count, &m_pSystem ,&m_pCamera);
	if( res != BGAPI_RESULT_OK )
	{
	//	printf("init_systems Errorcode: %d\n", res);
		m_LastErrorInfo.iErrorCode = ErrorSetParam;
		CString str;
		str.Format("相机序号:%d,初始化系统失败!! Errorcode: %d",m_nGrabberSN,res);
		_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::InitParm()函数");
		m_bInitSucceed = FALSE;
		return FALSE;
	}

	if (m_pCamera == NULL)
	{
		m_LastErrorInfo.iErrorCode = ErrorNoFindCam;
		CString str;
		str.Format("相机序号:%d,相机对象为空!!",m_nGrabberSN);
		_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::InitParm()函数");
		m_bInitSucceed = FALSE;
		return FALSE;
	}

	res = init_camera();
	if ( res != BGAPI_RESULT_OK)
	{
		m_LastErrorInfo.iErrorCode = res;
		CString str;
		str.Format("相机序号:%d,相机初始化失败!!",m_nGrabberSN);
		_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::InitParm()函数");
		m_bInitSucceed = FALSE;
		return FALSE;
	}

	m_bInitSucceed = TRUE;
	return TRUE;
}


//单贞采集
BOOL CBaumerCamDH::Snapshot()
{
	if (!m_bInitSucceed)
	{
		m_LastErrorInfo.iErrorCode = ErrorInit;
		CString str;
		str.Format("相机序号:%d,系统初始化失败!!",m_nGrabberSN);
		_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::Snapshot()函数");
		return FALSE;
	}

	if (!m_bGrabbing)
	{//
		m_LastErrorInfo.iErrorCode = ErrorSetParam;
		CString str;
		str.Format("相机序号:%d,相机没有采集.",m_nGrabberSN);
		_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::Snapshot()函数");

		return TRUE;
	}

	BGAPI_RESULT res = BGAPI_RESULT_FAIL;
	res = m_pCamera->doTrigger( );
	if( res != BGAPI_RESULT_OK )
	{
		CString str;
		str.Format("相机序号:%d,BGAPI::Camera::doTrigger returned with errorcode %d\n",m_nGrabberSN,res);
		m_LastErrorInfo.iErrorCode = ErrorStartGrab;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "Snapshot()函数");
		return FALSE;
	}


	return TRUE;
}
// 开始采集
BOOL CBaumerCamDH::StartGrab()
{
	if (!m_bGrabbing && m_bInitSucceed)
	{
		BGAPI_RESULT res = BGAPI_RESULT_FAIL;

//////////////////////////////////////////////////////////////////////////
		BGAPI_FeatureState state;
		state.cbSize = sizeof( BGAPI_FeatureState );
		BGAPIX_TypeBOOL pstart;
		pstart.cbSize = sizeof(BGAPIX_TypeBOOL);

		res = m_pCamera->setStart( true );
		if( res != BGAPI_RESULT_OK )
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::Camera::setStart returned with errorcode %d\n",m_nGrabberSN,res);
			m_LastErrorInfo.iErrorCode = ErrorStartGrab;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "StartGrab()函数");
			return FALSE;
		}


		if (m_nTrigger == 0)
		{
			res = m_pCamera->getSequencer(&state,&pstart);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::getSequencer Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::StartGrab()函数");
				return FALSE;
			}
			else
			{
				if (state.bIsEnabled)
				{
					res = m_pCamera->setSequencer(false,false);
					if( res != BGAPI_RESULT_OK )
					{
						CString str;
						str.Format("相机序号:%d,BGAPI::Camera::setSequencer Errorcode: %d\n",m_nGrabberSN,res);
						m_LastErrorInfo.iErrorCode = res;
						_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
						_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::StartGrab()函数");
						return FALSE;
					}
				}
			}
		}
		else if (m_nTrigger == 9)
		{
			res = m_pCamera->getSequencer(&state,&pstart);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::getSequencer Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::StartGrab()函数");
				return FALSE;
			}
			else
			{
		//////////////////////////////////////////////////////////////////////////
				if ((state.bIsEnabled == true) && (pstart.bcur == false))
				{

				}
				else
				{
					res = m_pCamera->setSequencer(true,false);
					if( res != BGAPI_RESULT_OK )
					{
						CString str;
						str.Format("相机序号:%d,BGAPI::Camera::setSequencer Errorcode: %d\n",m_nGrabberSN,res);
						m_LastErrorInfo.iErrorCode = res;
						_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
						_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::StartGrab()函数");
						return FALSE;
					}
				}

				res = m_pCamera->setSequencerRunMode(BGAPI_SEQUENCERRUNMODE_CYCLEBYTRIGGER);
				if( res != BGAPI_RESULT_OK )
				{
					CString str;
					str.Format("相机序号:%d,BGAPI::Camera::setSequencerRunMode Errorcode: %d\n",m_nGrabberSN,res);
					m_LastErrorInfo.iErrorCode = res;
					_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
					_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::StartGrab()函数");
					return FALSE;
				}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
				res = m_pCamera->doSequencerReset();
				if( res != BGAPI_RESULT_OK )
				{
					CString str;
					str.Format("相机序号:%d,BGAPI::Camera::getExposure Errorcode: %d\n",m_nGrabberSN,res);
					AfxMessageBox(str);
					m_LastErrorInfo.iErrorCode = res;
					_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
					_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::StartGrab()函数");
					return FALSE;
				}

				BGAPIX_TypeRangeINT rangedint;					
				rangedint.cbSize = sizeof( BGAPIX_TypeRangeINT );
				res = m_pCamera->getSequencerExposure( &state, &rangedint );
				if( res != BGAPI_RESULT_OK )
				{
					CString str;
					str.Format("相机序号:%d,BGAPI::Camera::getExposure Errorcode: %d\n",m_nGrabberSN,res);
					m_LastErrorInfo.iErrorCode = res;
					_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
					_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::StartGrab()函数");
					return FALSE;
				}
				else
				{
					m_nExMax =  rangedint.maximum;
					m_nExMin =  rangedint.minimum;
				}

				int nEx = GetPrivateProfileInt("camera", "exposure",1000, m_sInitFile);
				if (nEx < m_nExMin)
				{
					nEx = m_nExMin;
					CString str;
					str.Format("%d",nEx);
					WritePrivateProfileString("camera", "exposure", str, m_sInitFile);
				}

				if (nEx > m_nExMax)
				{
					nEx = m_nExMax;
					CString str;
					str.Format("%d",nEx);
					WritePrivateProfileString("camera", "exposure", str, m_sInitFile);
				}

				if (nEx != rangedint.current)
				{
					res  = m_pCamera->setSequencerExposure(nEx);//相机快门值
					if( res != BGAPI_RESULT_OK )
					{
						CString str;
						str.Format("相机序号:%d,2 BGAPI::Camera::getExposure Errorcode: %d\n",m_nGrabberSN,res);
						AfxMessageBox(str);
						m_LastErrorInfo.iErrorCode = res;
						_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
						_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::StartGrab()函数");
						return FALSE;
					}
				}
		//////////////////////////////////////////////////////////////////////////
				BGAPIX_TypeRangeFLOAT gain;	
				gain.cbSize = sizeof( BGAPIX_TypeRangeFLOAT );
				res = m_pCamera->getSequencerGain(&state, &gain);
				if( res != BGAPI_RESULT_OK )
				{
					CString str;
					str.Format("相机序号:%d,BGAPI::Camera::getGain Errorcode: %d\n",m_nGrabberSN,res);
					m_LastErrorInfo.iErrorCode = res;
					_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
					_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::StartGrab()函数");
					return FALSE;
				}
				else
				{
					m_nGainMax =  gain.maximum;
					m_nGainMin =  gain.minimum;
				}

				CString sGain;

				GetPrivateProfileStringA("camera", "gain","1", sGain.GetBuffer(100),100,m_sInitFile);
				float nGain = (float)atof(sGain);

				if (nGain < m_nGainMin)
				{
					nGain = m_nGainMin;
					CString str;
					str.Format("%f",nGain);
					WritePrivateProfileString("camera", "gain", str, m_sInitFile);
				}
	
				if (nGain > m_nGainMax)
				{
					nGain = m_nGainMax;
					CString str;
					str.Format("%f",nGain);
					WritePrivateProfileString("camera", "gain", str, m_sInitFile);
				}

				if (abs(nGain - gain.current) > 0.05)
				{
					BOOL bRet = m_pCamera->setSequencerGain(nGain);
					if (!bRet)
					{
						CString str;
						str.Format("BGAPI::Camera::SetParamfloat Gain Errorcode: %d\n",bRet);
						m_LastErrorInfo.iErrorCode = bRet;
						_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
						_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::StartGrab()函数");
						return FALSE;
					}
				}
	

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
				int loopcounter = 1;
				int repeatcounter = m_nSequencerNum;
				int triggercounter  = m_nSequencerNum;
				res = m_pCamera->setSequencerCounter(loopcounter, repeatcounter, triggercounter);
				if( res != BGAPI_RESULT_OK )
				{
					CString str;
					str.Format("相机序号:%d,BGAPI::Camera::setSequencerCounter Errorcode: %d\n",m_nGrabberSN,res);
					m_LastErrorInfo.iErrorCode = res;
					_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
					_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::StartGrab()函数");
					return FALSE;
				}

				res = m_pCamera->setSequencer(true,true);
				if( res != BGAPI_RESULT_OK )
				{
					CString str;
					str.Format("相机序号:%d,BGAPI::Camera::setSequencer Errorcode: %d\n",m_nGrabberSN,res);
					m_LastErrorInfo.iErrorCode = res;
					_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
					_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::StartGrab()函数");
					return FALSE;
				}
			}
		}

//////////////////////////////////////////////////////////////////////////


		m_bGrabbing = TRUE;
	}
	else
	{
		if (m_bGrabbing)
		{
			m_LastErrorInfo.iErrorCode = ErrorStartGrab;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, "正在采集");
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "StartGrab()函数");
		}

		if (!m_bInitSucceed)
		{
			m_LastErrorInfo.iErrorCode = ErrorStartGrab;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, "开始采集失败,初始化不成功,不能开始采集");
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "StartGrab()函数");
		}
		
		return FALSE;
	}

	return TRUE;
}

// 停止采集
BOOL CBaumerCamDH::StopGrab()
{
	if (m_bGrabbing)
	{
		BGAPI_RESULT res = BGAPI_RESULT_FAIL;
		res = m_pCamera->setStart( false );
		if( res != BGAPI_RESULT_OK )
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::Camera::setStart Errorcode: %d\n",m_nGrabberSN,res);
			TRACE("BGAPI::Camera::setStart Errorcode: %d\n", res );
			m_LastErrorInfo.iErrorCode = ErrorStopGrab;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "StopGrab()函数");
			return FALSE;
		}

		m_bGrabbing = FALSE;

//////////////////////////////////////////////////////////////////////////


		BGAPI_FeatureState state;
		state.cbSize = sizeof( BGAPI_FeatureState );
		BGAPIX_TypeBOOL pstart;
		pstart.cbSize = sizeof(BGAPIX_TypeBOOL);
		res = m_pCamera->getSequencer(&state,&pstart);
		if( res != BGAPI_RESULT_OK )
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::Camera::getSequencer Errorcode: %d\n",m_nGrabberSN,res);
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::StopGrab()函数");
			return FALSE;
		}
		else
		{
			if (state.bIsEnabled || pstart.bcur)
			{
				res = m_pCamera->setSequencer(false,false);
				if( res != BGAPI_RESULT_OK )
				{
					CString str;
					str.Format("相机序号:%d,BGAPI::Camera::setSequencer Errorcode: %d\n",m_nGrabberSN,res);
					m_LastErrorInfo.iErrorCode = res;
					_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
					_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::StopGrab()函数");
					return FALSE;
				}
			}
		}
	
//////////////////////////////////////////////////////////////////////////

	}
	else
	{
		CString str;
		str.Format("相机序号:%d,采集已经停止",m_nGrabberSN);
		m_LastErrorInfo.iErrorCode = ErrorStopGrab;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "StopGrab()函数");
		return FALSE;
	}

	return TRUE;
}

//关闭图象卡
BOOL CBaumerCamDH::Close()
{
	if (m_bGrabbing)
	{
		StopGrab();
		m_bGrabbing = FALSE;
	}

	return TRUE;
}

// 得到参数
BOOL CBaumerCamDH::GetParamInt(GBParamID Param, int &nOutputVal)
{
	switch(Param)
	{
	case 0://0:GBImageWidth,				//图像宽度 (单位:字节)
		nOutputVal = m_nImageWidth;
		break;
	case 1://1:GBImageHeight,			//图像高度 (单位:字节)
		nOutputVal = m_nImageHeight;
		break;
	case 2:// 	2:GBImagePixelSize,			//图像象素大小 (单位:字节)
		nOutputVal = m_nImageByteCount;
		break;
	case 3:// 	3:GBImageBufferSize,		//图像缓冲区大小 (单位:字节)
		nOutputVal = m_nImageWidth * m_nImageHeight * m_nImageByteCount;
		break;
	case 4:// 	4:GBImageBufferAddr,		//图像缓冲区地址
		nOutputVal = (int)m_pbImageBufferAdd;
		if (m_pbImageBufferAdd == NULL)
		{
			m_LastErrorInfo.iErrorCode = ErrorGetParam;
			CString str;
			str.Format("相机序号:%d,得到图像首址出错",m_nGrabberSN);
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "GetParamInt()函数");
			return FALSE;
		}
		break;
	case GBParamID::GBShutter:// 	GBShutter,				//相机快门值
		{
			BGAPI_RESULT res = BGAPI_RESULT_FAIL;
			BGAPI_FeatureState state;
			BGAPIX_TypeRangeINT rangedint;
			
			//CHECK EXPOSURE			
			state.cbSize = sizeof( BGAPI_FeatureState );
			rangedint.cbSize = sizeof( BGAPIX_TypeRangeINT );
			
			if (m_nTrigger == 0)
			{
				//AfxMessageBox("m_nTrigger == 0");
				res = m_pCamera->getExposure( &state, &rangedint );
			}
			else if (m_nTrigger == 9)
			{
				if (m_bGrabbing == FALSE)
				{
					res = BGAPI_RESULT_OK;
					rangedint.current = GetPrivateProfileInt("camera", "exposure",4000, m_sInitFile);

				}
				else
				res = m_pCamera->getSequencerExposure( &state, &rangedint );
			}

			//	res = m_pCamera->getFramesPerSeconds( 0, &state, &framerate );
			if( res != BGAPI_RESULT_OK )
			{
				nOutputVal = -1;
			}
			else
			{
//				CString str;
//				str.Format("%d",rangedint.current);
//				AfxMessageBox(str);
				nOutputVal =  rangedint.current;
			}
		}
		break;
	case GBParamID::GBExposureMax://	14:ExposureMax,
		{
			nOutputVal = m_nExMax;
			// 			BGAPI_RESULT res = BGAPI_RESULT_FAIL;
			// 			BGAPI_FeatureState state;
			// 			BGAPIX_TypeRangeINT rangedint;
			// 			
			// 			//CHECK EXPOSURE			
			// 			state.cbSize = sizeof( BGAPI_FeatureState );
			// 			rangedint.cbSize = sizeof( BGAPIX_TypeRangeINT );
			// 			res = m_pCamera->getExposure( &state, &rangedint );
			// 			//	res = m_pCamera->getFramesPerSeconds( 0, &state, &framerate );
			// 			if( res != BGAPI_RESULT_OK )
			// 			{
			// 				nOutputVal = -1;
			// 			}
			// 			else
			// 			{
			// 				nOutputVal =  rangedint.maximum;
			// 			}
		}
		break;
	case GBParamID::GBExposureMin://	ExposureMin,
		{
			nOutputVal = m_nExMin;
			// 			BGAPI_RESULT res = BGAPI_RESULT_FAIL;
			// 			BGAPI_FeatureState state;
			// 			BGAPIX_TypeRangeINT rangedint;
			// 			
			// 			//CHECK EXPOSURE			
			// 			state.cbSize = sizeof( BGAPI_FeatureState );
			// 			rangedint.cbSize = sizeof( BGAPIX_TypeRangeINT );
			// 			res = m_pCamera->getExposure( &state, &rangedint );
			// 			//	res = m_pCamera->getFramesPerSeconds( 0, &state, &framerate );
			// 			if( res != BGAPI_RESULT_OK )
			// 			{
			// 				nOutputVal = -1;
			// 			}
			// 			else
			// 			{
			// 				nOutputVal =  rangedint.minimum;
			// 			}
		}
		break;
	case GBParamID::GBSequenceNum://	
		{
			nOutputVal = m_nSequencerNum;
		}
		break;
	default:
//		AfxMessageBox("GetParamInt switch(Param)  default:...");
		m_LastErrorInfo.iErrorCode = ErrorGetParam;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, "得到参数出错");
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "GetParamInt()函数");

		return FALSE;
	}

	return TRUE;
}

// 设置参数
BOOL CBaumerCamDH::SetParamInt(GBParamID Param, int nInputVal)
{
	BOOL bRet = TRUE;
	switch(Param)
	{		
	case GBParamID::GBShutter://GBShutter,				//相机快门值
		{
			BGAPI_RESULT res = BGAPI_RESULT_FAIL;
			//CHECK EXPOSURE
			if (m_nTrigger == 0)
			{
				res = m_pCamera->setExposure(nInputVal);
			}
			else if (m_nTrigger == 9)
			{
				if (m_bGrabbing == FALSE)
				{
					CString str;
					str.Format("相机序号:%d,sequence模式下，没有采集不能设置曝光时间.",m_nGrabberSN);
					m_LastErrorInfo.iErrorCode = res;
					_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
					_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetParamInt()函数");
					return FALSE;
				}

				// SXG10等其他相机 [4/25/2014 SJC]if(m_sDeviceName == "VLG-02M")
				{
					res = m_pCamera->setSequencer(true,false);
					if( res != BGAPI_RESULT_OK )
					{
						CString str;
						str.Format("相机序号:%d,BGAPI::Camera::setSequencer Errorcode: %d\n",m_nGrabberSN,res);
						m_LastErrorInfo.iErrorCode = res;
						_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
						_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetParamInt()函数");
						return FALSE;
					}
				}


				//res = m_pCamera->doSequencerReset();
				//if( res != BGAPI_RESULT_OK )
				//{
				//	CString str;
				//	str.Format("3 BGAPI::Camera::getExposure Errorcode: %d\n",m_nGrabberSN,res);
				//	AfxMessageBox(str);
				//	m_LastErrorInfo.iErrorCode = res;
				//	_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				//	_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
				//	return res;
				//}

				res = m_pCamera->setSequencerExposure(nInputVal);


				// SXG10等其他相机 [4/25/2014 SJC]if(m_sDeviceName == "VLG-02M")
				{
					res = m_pCamera->setSequencer(true,true);
					if( res != BGAPI_RESULT_OK )
					{
						CString str;
						str.Format("相机序号:%d,BGAPI::Camera::setSequencer Errorcode: %d\n",m_nGrabberSN,res);
						m_LastErrorInfo.iErrorCode = res;
						_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
						_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetParamInt()函数");
						return FALSE;
					}
				}
			}
			
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::setSequencer Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetParamInt()函数");
				return FALSE;
			}
			else// 设置成功保存到配置文件 [1/7/2014 SJC]
			{
				CString str;
				str.Format("%d",nInputVal);
				WritePrivateProfileString("camera", "exposure", str,m_sInitFile);
			}
		}
		break;
	case GBParamID::GBSequenceNum://SequencerNum
		{
			m_nSequencerNum = nInputVal;
			int ParamValInput = 9;
			BOOL bRet = SetTriggerParam(ParamValInput);
			if (!bRet)
			{
				CString str;
				str.Format("相机序号:%d,设置SequencerNum失败！",m_nGrabberSN);
				m_LastErrorInfo.iErrorCode = ErrorSetParam;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetParamInt()函数");
				return FALSE;
			}
		}
		break;
	default:
		//		AfxMessageBox("GetParamInt switch(Param)  default:...");
		m_LastErrorInfo.iErrorCode = ErrorSetParam;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, "设置参数出错");
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "SetParamInt()函数");
		
		return FALSE;
	}
	
	return TRUE;
}

class tempRoutingFrame {

   CFrameWnd* m_pFrame ;

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

// 参数设置对话框
void CBaumerCamDH::CallParamDialog()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
#ifdef _DEBUG
	// Workaround for ASSERT in WINCORE.CPP 884 (CWnd::AssertValid)
	tempRoutingFrame rframe;
#endif
	
	if (m_nTrigger == 9)
	{
		if (m_bGrabbing == FALSE)
		{
			CString str;
			str.Format("相机序号:%d,采集处于multishot模式，必须开始采集才能设置相机参数！",m_nGrabberSN);
			AfxMessageBox(str);
			return;
		}
	}

	SetDlg dlg;
	dlg.GetAcquisition(this);
	if(dlg.DoModal() == -1)
	{
		m_LastErrorInfo.iErrorCode = ErrorParamDlg;
		CString str;
		str.Format("相机序号:%d,调用参数设置对话框错误！",m_nGrabberSN);
		_tprintf_s(m_LastErrorInfo.strErrorDescription,str);
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::CallParamDialog()函数");
		return;
	}

}

// 得到错误信息
void CBaumerCamDH::GetLastErrorInfo(s_GBERRORINFO *pErrorInfo)
{
	//BGAPI_RESULT res = BGAPI_RESULT_FAIL;
	//BGAPI_FeatureState state;
	//BGAPIX_CameraStatistic statistic; 

	//state.cbSize = sizeof( BGAPI_FeatureState );
	//statistic.cbSize = sizeof( BGAPIX_CameraStatistic );
	//res =  m_pCamera->getStatistic( &state, &statistic );
		/**
	\brief Definition of statistic array for type = BGAPIX_DEVICETYPE_BAUMERCAM_GIGE
	\brief statistic[0] Received Frames Good
	\brief statistic[1] Received Frames Corrupted
	\brief statistic[2] Lost Frames
	\brief statistic[3] Resend Requests
	\brief statistic[4] Resend Packets
	\brief statistic[5] Lost Packest
	\brief statistic[6] Bandwidth
	\brief statistic[7-31] Reserved
	\brief Definition of statistic array for type = BGAPIX_DEVICETYPE_BAUMERTRIGGERDEVICE_GIGE 
	\brief statistic[0] action command send counter for input 1
	\brief statistic[1] action command send counter for input 2
	\brief statistic[2] action command send counter for input 3
	\brief statistic[3] action command send counter for input 4
	\brief statistic[4] action command send counter for input 5
	\brief statistic[5] action command send counter for input 6
	\brief statistic[6] action command send counter for input 7
	\brief statistic[7] action command send counter for input 8
	\brief statistic[8] action command receive counter for input 1
	\brief statistic[9] action command receive counter for input 2
	\brief statistic[10] action command receive counter for input 3
	\brief statistic[11] action command receive counter for input 4
	\brief statistic[12] action command receive counter for input 5
	\brief statistic[13] action command receive counter for input 6
	\brief statistic[14] action command receive counter for input 7
	\brief statistic[15] action command receive counter for input 8
	\brief statistic[16] action command receive to late counter for output 1
	\brief statistic[17] action command receive to late counter for output 2
	\brief statistic[18] action command receive to late counter for output 3
	\brief statistic[19] action command receive to late counter for output 4
	\brief statistic[20] action command receive to late counter for output 5
	\brief statistic[21] action command receive to late counter for output 6
	\brief statistic[22] action command receive to late counter for output 7
	\brief statistic[23] action command receive to late counter for output 8
	\brief statistic[24] line counter for input 1 
	\brief statistic[25] line counter for input 2 
	\brief statistic[26] line counter for input 3 
	\brief statistic[27] line counter for input 4 
	\brief statistic[28] line counter for input 5 
	\brief statistic[29] line counter for input 6 
	\brief statistic[30] line counter for input 7 
	\brief statistic[31] line counter for input 8 
	\brief Definition of statistic array for type = BGAPIX_DEVICETYPE_BAUMERCAM_FCAMX or BGAPIX_DEVICETYPE_BAUMERCAM_DCAM
	\brief statistic[0-31] Reserved
	*/

	//statistic.statistic
	//CString str;
	//str.Format("Good:%d---Corrupted:%d--- Lost:%d----Resend Requests:%d---Resend Packets:%d---Lost Packest:%d---Bandwidth:%d\n",
	//	statistic.statistic[0],
	//	statistic.statistic[1],
	//	statistic.statistic[2],
	//	statistic.statistic[3],
	//	statistic.statistic[4],
	//	statistic.statistic[5],
	//	statistic.statistic[6]);

	//AfxMessageBox(str);

//	_tprintf_s(m_LastErrorInfo.strErrorDescription,str);

	pErrorInfo->iErrorCode = m_LastErrorInfo.iErrorCode;
	_tprintf_s(pErrorInfo->strErrorDescription,m_LastErrorInfo.strErrorDescription);
	_tprintf_s(pErrorInfo->strErrorRemark,m_LastErrorInfo.strErrorRemark);
}

//////////////////////////////////////////////////////////////////////////
//


BOOL CBaumerCamDH::SetParamfloat(int Param, float nInputVal)
{
	BOOL bRet = TRUE;
	switch(Param)
	{		
	case 9://GBGain,					//相机增益大小
		{
			BGAPI_RESULT res = BGAPI_RESULT_FAIL;	
			
			if (m_nTrigger == 0)
			{
				res = m_pCamera->setGain(nInputVal);
			}
			else if (m_nTrigger == 9)
			{
				if (m_bGrabbing == FALSE)
				{
					CString str;
					str.Format("相机序号:%d，sequence模式下，没有采集不能设置增益.",m_nGrabberSN);
					m_LastErrorInfo.iErrorCode = res;
					_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
					_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetParamfloat()函数");
					return FALSE;
				}

				//  [4/25/2014 SJC]if(m_sDeviceName == "VLG-02M")
				{
					res = m_pCamera->setSequencer(true,false);
					if( res != BGAPI_RESULT_OK )
					{
						CString str;
						str.Format("相机序号:%d,BGAPI::Camera::setSequencer Errorcode: %d\n",m_nGrabberSN,res);
						m_LastErrorInfo.iErrorCode = res;
						_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
						_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetParamfloat()函数");
						return FALSE;
					}
				}
				res = m_pCamera->setSequencerGain(nInputVal);

				//  [4/25/2014 SJC]if(m_sDeviceName == "VLG-02M")
				{
					res = m_pCamera->setSequencer(true,true);
					if( res != BGAPI_RESULT_OK )
					{
						CString str;
						str.Format("相机序号:%d,BGAPI::Camera::setSequencer Errorcode: %d\n",m_nGrabberSN,res);
						m_LastErrorInfo.iErrorCode = res;
						_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
						_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetParamfloat()函数");
						return FALSE;
					}
				}


			}
			
			if( res != BGAPI_RESULT_OK )
			{
				return FALSE;
			}
		}
		break;
	default:
//		AfxMessageBox("GetParamInt switch(Param)  default:...");
		m_LastErrorInfo.iErrorCode = ErrorSetParam;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, "设置参数出错");
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "SetParamfloat()函数");
		
		return FALSE;
	}
	
	return TRUE;
}

BOOL CBaumerCamDH::GetParamfloat(int Param, float& nOutputVal)
{
	switch(Param)
	{
	case 9:// 	9:GBGain,					//相机增益大小
		{
			BGAPI_RESULT res = BGAPI_RESULT_FAIL;
			BGAPI_FeatureState state;
			BGAPIX_TypeRangeFLOAT gain;
			
			state.cbSize = sizeof( BGAPI_FeatureState );
			gain.cbSize = sizeof( BGAPIX_TypeRangeFLOAT );
			if (m_nTrigger == 0)
			{
				res = m_pCamera->getGain(&state, &gain);
			}
			else if (m_nTrigger == 9)
			{
				if (m_bGrabbing == FALSE)
				{
					res = BGAPI_RESULT_OK;
					CString sGain;
					GetPrivateProfileStringA("camera", "gain","1", sGain.GetBuffer(100),100,m_sInitFile);
					gain.current = (float)atof(sGain);
				}
				res = m_pCamera->getSequencerGain(&state, &gain);
			}
			
			//	res = m_pCamera->getFramesPerSeconds( 0, &state, &framerate );
			if( res != BGAPI_RESULT_OK )
			{
				nOutputVal = -1;
			}
			else
			{
				nOutputVal =  gain.current;
			}
		}
		break;
	case 16://	16:GainMax
		{
			nOutputVal = m_nGainMax;
// 			BGAPI_RESULT res = BGAPI_RESULT_FAIL;
// 			BGAPI_FeatureState state;
// 			BGAPIX_TypeRangeFLOAT gain;
// 			
// 			state.cbSize = sizeof( BGAPI_FeatureState );
// 			gain.cbSize = sizeof( BGAPIX_TypeRangeFLOAT );
// 			res = m_pCamera->getGain(&state, &gain);
// 			//	res = m_pCamera->getFramesPerSeconds( 0, &state, &framerate );
// 			if( res != BGAPI_RESULT_OK )
// 			{
// 				nOutputVal = -1;
// 			}
// 			else
// 			{
// 				nOutputVal =  gain.maximum;
// 			}
		}
		break;
	case 15://	15:GainMin
		{
			nOutputVal = m_nGainMin;
// 			BGAPI_RESULT res = BGAPI_RESULT_FAIL;
// 			BGAPI_FeatureState state;
// 			BGAPIX_TypeRangeFLOAT gain;
// 			
// 			state.cbSize = sizeof( BGAPI_FeatureState );
// 			gain.cbSize = sizeof( BGAPIX_TypeRangeFLOAT );
// 			res = m_pCamera->getGain(&state, &gain);
// 			//	res = m_pCamera->getFramesPerSeconds( 0, &state, &framerate );
// 			if( res != BGAPI_RESULT_OK )
// 			{
// 				nOutputVal = -1;
// 			}
// 			else
// 			{
// 				nOutputVal =  gain.minimum;
// 			}
		}
		break;

	default:
//		AfxMessageBox("GetParamInt switch(Param)  default:...");
		m_LastErrorInfo.iErrorCode = ErrorGetParam;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, "得到参数出错");
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "GetParamfloat()函数");
		
		return FALSE;
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// 增加图像翻转操作 [4/25/2014 SJC]
BOOL CBaumerCamDH::SetImageFlipMode(int iType)
{
	if(!m_bInitSucceed)
		return FALSE ;
	BGAPI_RESULT res = BGAPI_RESULT_FAIL;
	BGAPI_FlipType bFT = (BGAPI_FlipType)iType;
	res = m_pCamera->setFlipType(bFT);
	if( res != BGAPI_RESULT_OK )
	{
		CString str;
		str.Format("相机序号:%d,图像翻转失败-错误代码: %d\n",m_nGrabberSN,res);
		m_LastErrorInfo.iErrorCode = res;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::setFlipType()函数");
		return FALSE;
	}
	return TRUE;
}
BOOL CBaumerCamDH::SetTriggerParam(int ParamValInput)
{
	if(!m_bInitSucceed)
		return FALSE ;
	BGAPI_RESULT res = BGAPI_RESULT_FAIL;
	
	switch(ParamValInput) 
	{

	case 0://Internal:  //外触发关闭//内部触发,连续采集
		{
			if (m_nTrigger == 9)
			{
				m_nTrigger = 0;
				if (m_bGrabbing)
				{
					res = m_pCamera->setSequencer(false,false);
					if( res != BGAPI_RESULT_OK )
					{
						CString str;
						str.Format("相机序号:%d,BGAPI::Camera::setSequencer Errorcode: %d\n",m_nGrabberSN,res);
						m_LastErrorInfo.iErrorCode = res;
						_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
						_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetTriggerParam()函数");
						return FALSE;
					}
				}
			}

			res = m_pCamera->setTrigger(false);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::getTrigger Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetTriggerParam()函数");
				return FALSE;
			}

			int nEx = GetPrivateProfileInt("camera", "exposure",1000, m_sInitFile);
			BOOL bRet = SetParamInt(GBParamID::GBShutter, nEx);//:GBShutter,	//相机快门值
			if (!bRet)
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::BaumerSetParamPro Exposure Errorcode: %d\n",m_nGrabberSN,bRet);
				m_LastErrorInfo.iErrorCode = bRet;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetTriggerParam()函数");
				return FALSE;
			}

			CString sGain;

			GetPrivateProfileStringA("camera", "gain","1", sGain.GetBuffer(100),100,m_sInitFile);
			float nGain = (float)atof(sGain);
			bRet = SetParamfloat(9, nGain);
			if (!bRet)
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::SetParamfloat Gain Errorcode: %d\n",m_nGrabberSN,bRet);
				m_LastErrorInfo.iErrorCode = bRet;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetTriggerParam()函数");
				return FALSE;
			}
		}
		break;
	case 1:
		{
			BGAPI_FeatureState state;
			res = m_pCamera->setTrigger(true);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::getTrigger Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetTriggerParam()函数");
				return FALSE;
			}
			m_nTrigger=1;
			//////////////////////////////////////////////////////////////////////////
			int nTriggerSource = GetPrivateProfileInt("camera", "triggersource",-1,m_sInitFile);
			if (nTriggerSource == -1)
			{
				WritePrivateProfileString("camera", "triggersource", "0", m_sInitFile);
				nTriggerSource = 0;
			}

			BGAPIX_TypeListINT TriggerSourceList;
			TriggerSourceList.cbSize = sizeof ( BGAPIX_TypeListINT );

			res = m_pCamera->getTriggerSource( &state, &TriggerSourceList );
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::getTriggerSource Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
				return res;
			}

			if ( TriggerSourceList.array[TriggerSourceList.current] !=  nTriggerSource)
			{
				for ( int t = 0; t<TriggerSourceList.length; t++ )
				{
					if (nTriggerSource == TriggerSourceList.array[t])
					{
						m_bSet3 = TRUE;
						res = m_pCamera->setTriggerSource( BGAPI_TriggerSource(TriggerSourceList.array[t]) );
						if( res != BGAPI_RESULT_OK )
						{
							CString str;
							str.Format("相机序号:%d,BGAPI::Camera::setTriggerSource Errorcode: %d\n",m_nGrabberSN,res);
							m_LastErrorInfo.iErrorCode = res;
							_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
							_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
							return res;
						}

						break;
					}
					else
					{
						if (t == TriggerSourceList.length-1)
						{
							CString str;
							str.Format("相机序号:%d,非法触发源参数!",m_nGrabberSN);
							m_LastErrorInfo.iErrorCode = ErrorSetParam;
							_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
							_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
							return res;
						}
					}
				}
			}
			//////////////////////////////////////////////////////////////////////////
			if (nTriggerSource != 0)
			{
				int nTriggerActivation = GetPrivateProfileInt("camera", "triggeract",-1,m_sInitFile);
				if (nTriggerActivation == -1)
				{
					WritePrivateProfileString("camera", "triggeract", "2", m_sInitFile);
					nTriggerActivation = 2;
				}

				BGAPIX_TypeListINT TriggerActivation;
				TriggerActivation.cbSize = sizeof ( BGAPIX_TypeListINT );
				res = m_pCamera->getTriggerActivation( &state, &TriggerActivation );
				if( res != BGAPI_RESULT_OK )
				{
					CString str;
					str.Format("相机序号:%d,BGAPI::Camera::getTriggerActivation Errorcode: %d\n",m_nGrabberSN,res);
					m_LastErrorInfo.iErrorCode = res;
					_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
					_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
					return res;
				}

				if (BGAPI_Activation(TriggerActivation.array[TriggerActivation.current]) != BGAPI_Activation(nTriggerActivation))
				{

					for ( int i = 0; i<TriggerActivation.length; i++ )
					{
						if (nTriggerActivation == TriggerActivation.array[i])
						{
							m_bSet3 = TRUE;
							res = m_pCamera->setTriggerActivation(BGAPI_Activation(TriggerActivation.array[i]));
							if( res != BGAPI_RESULT_OK )
							{
								CString str;
								str.Format("相机序号:%d,BGAPI::Camera::setTriggerActivation Errorcode: %d\n",m_nGrabberSN,res);
								m_LastErrorInfo.iErrorCode = res;
								_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
								_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
								return res;
							}
							break;
						}
						else
						{
							if (i == TriggerActivation.length-1)
							{
								CString str;
								str.Format("相机序号:%d,非法触发波形参数!",m_nGrabberSN);
								m_LastErrorInfo.iErrorCode = ErrorSetParam;
								_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
								_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
								return res;
							}
						}
					}	
				}
			}

		}
	case 9:
		{
			res = m_pCamera->setTrigger(true);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::getTrigger Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetTriggerParam()函数");
				return FALSE;
			}

			res = m_pCamera->setTriggerSource(BGAPI_TRIGGERSOURCE_SOFTWARE);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::setTriggerSource Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetTriggerParam()函数");
				return FALSE;
			}

			m_nTrigger = 9;

			if (m_bGrabbing == FALSE)
			{
				CString str;
				str.Format(" 相机序号:%d,sequence模式下，必须在采集状态下才能设置参数",m_nGrabberSN);
				m_LastErrorInfo.iErrorCode = 0;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetTriggerParam函数");
				return TRUE;
			}

			//	if (m_nTrigger != 0)
			//	{
			//	if (m_bGrabbing)
			//	{
			res = m_pCamera->setSequencer(true,false);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::setSequencer Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetTriggerParam()函数");
				return FALSE;
			}

			res = m_pCamera->setSequencerRunMode(BGAPI_SEQUENCERRUNMODE_CYCLEBYTRIGGER);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::setSequencerRunMode Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetTriggerParam()函数");
				return FALSE;
			}

			int nEx = GetPrivateProfileInt("camera", "exposure",1000, m_sInitFile);
			res = m_pCamera->setSequencerExposure(nEx);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::setSequencer Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetTriggerParam()函数");
				return FALSE;
			}

			CString sGain;

			GetPrivateProfileStringA("camera", "gain","1", sGain.GetBuffer(100),100,m_sInitFile);
			float nGain = (float)atof(sGain);
			res = m_pCamera->setSequencerGain(nGain);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::setSequencer Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetTriggerParam()函数");
				return FALSE;
			}
			//////////////////////////////////////////////////////////////////////////


			int loopcounter = 1;
			int repeatcounter = m_nSequencerNum;
			int triggercounter  = m_nSequencerNum;
			res = m_pCamera->setSequencerCounter(loopcounter, repeatcounter, triggercounter);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::setSequencerCounter Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetTriggerParam()函数");
				return FALSE;
			}
			//////////////////////////////////////////////////////////////////////////
			res = m_pCamera->setSequencer(true,true);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::setSequencer Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetTriggerParam()函数");
				return FALSE;
			}
			//	}
			//	}						
		}
		break;
	default:
		return FALSE;

	}
	return TRUE;
}

BOOL CBaumerCamDH::SetAOI(RECT rtAOI)
{
	if (!m_bInitSucceed)
	{
		CString str;
		str.Format("相机序号:%d,初始化没有成功!",m_nGrabberSN);
		m_LastErrorInfo.iErrorCode = ErrorSetParam;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetAOI()函数");
		return FALSE;
		
	}

	if (m_nTrigger == 0)
	{
		if (m_bGrabbing)
		{
			CString str;
			str.Format("相机序号:%d,正在采集!",m_nGrabberSN);
			m_LastErrorInfo.iErrorCode = ErrorSetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetAOI()函数");
			return FALSE;
		}
	}
	else if (m_nTrigger == 9)
	{
		if (m_bGrabbing)
		{
			CString str;
			str.Format("相机序号:%d,正在采集!",m_nGrabberSN);
			m_LastErrorInfo.iErrorCode = ErrorSetParam;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetAOI()函数");
			return FALSE;
		}
	}


	BGAPI_RESULT res = BGAPI_RESULT_FAIL;
	BGAPI_FeatureState state;
	BGAPIX_TypeROI roi; 
	state.cbSize = sizeof( BGAPI_FeatureState );
	roi.cbSize = sizeof( BGAPIX_TypeROI );
	CString str1,str2;

	int nleft,ntop,nright,nbottom;
	res = m_pCamera->getPartialScan(&state,&roi);
	if( res != BGAPI_RESULT_OK )
	{
		CString str;
		str.Format("相机序号:%d,BGAPI::Camera::getPartialScan Errorcode: %d\n",m_nGrabberSN,res);
		m_LastErrorInfo.iErrorCode = res;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetAOI()函数");
		return FALSE;
	}
	else
	{

		nleft = rtAOI.left;
		if (nleft%4!=0)
		{
			nleft = nleft-nleft%4;
		}
		if (nleft < roi.minleft)
		{
			nleft = roi.minleft;
		}
		
		if (nleft > roi.maxleft)
		{
			nleft = roi.maxleft;
		}
		
		ntop = rtAOI.top;
		if (ntop < roi.mintop)
		{
			ntop = roi.mintop;
		}
		if (ntop > roi.maxtop)
		{
			ntop = roi.maxtop;
		}
		
		nright = rtAOI.right;
		if (nright%4!=0)
		{
			nright = nright-nright%4;
		}
		if (nright < roi.minright)
		{
			nright = roi.minright;
		}
		if (nright > roi.maxright)
		{
			nright = roi.maxright;
		}
		
		nbottom = rtAOI.bottom;
		if (nbottom < roi.minbottom)
		{
			nbottom = roi.minbottom;
		}
		if (nbottom > roi.maxbottom)
		{
			nbottom = roi.maxbottom;
		}


		if (state.bIsEnabled != true 
			|| roi.curleft != nleft 
			|| roi.curtop != ntop 
			|| roi.curright != nright 
			|| roi.curbottom != nbottom)
		{
			m_bSet3 = TRUE;
			res = m_pCamera->setPartialScan(true,nleft,ntop,nright,nbottom);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,setPartialScan returned with errorcode %d\n",m_nGrabberSN, res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::SetAOI()函数");
				return FALSE;
			}
			else// 设置AOI成功保存到配置文件 [1/7/2014 SJC]
			{
				CString str;
				str.Format("%d",nleft);
				WritePrivateProfileString("camera", "roileft", str,m_sInitFile);
				str.Format("%d",nright);
				WritePrivateProfileString("camera", "roiright", str,m_sInitFile);
				str.Format("%d",ntop);
				WritePrivateProfileString("camera", "roitop", str,m_sInitFile);
				str.Format("%d",nbottom);
				WritePrivateProfileString("camera", "roibottom", str,m_sInitFile);
			}
		}

		m_nImageWidth = nright-nleft;
		m_nImageHeight = nbottom-ntop;
		m_nImageBufferSize = m_nImageWidth*m_nImageHeight;
	}


//////////////////////////////////////////////////////////////////////////

	return TRUE;
}

BOOL CBaumerCamDH::GetAOI(RECT &rtAOI)
{
	if (!m_bInitSucceed)
	{
		CString str;
		str.Format("相机序号:%d,初始化没有成功!",m_nGrabberSN);
		m_LastErrorInfo.iErrorCode = ErrorSetParam;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::GetAOI()函数");
		return FALSE;
		
	}

	BGAPI_RESULT res = BGAPI_RESULT_FAIL;
	BGAPI_FeatureState state;
	BGAPIX_TypeROI roi; 
	state.cbSize = sizeof( BGAPI_FeatureState );
	roi.cbSize = sizeof( BGAPIX_TypeROI );
	CString str1,str2;
	
	
	res = m_pCamera->getPartialScan(&state,&roi);
	if( res != BGAPI_RESULT_OK )
	{
		CString str;
		str.Format("相机序号:%d,BGAPI::Camera::getPartialScan Errorcode: %d\n",m_nGrabberSN,res);
		m_LastErrorInfo.iErrorCode = res;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::GetAOI()函数");
		return FALSE;
	}
	else
	{
		rtAOI.left = roi.curleft;
		rtAOI.top = roi.curtop;
		rtAOI.right = roi.curright;
		rtAOI.bottom = roi.curbottom;

	}
	return TRUE;
}


int CBaumerCamDH::init_systems(int * system_count, BGAPI::System ** externppSystem ,BGAPI::Camera ** externppCamera)
{
	//////////////////////////////////////////////////////////////////////////

	BGAPI_RESULT res = BGAPI_RESULT_FAIL;
	int i = 0;
	BGAPI_FeatureState state;
	BGAPIX_SystemInfo sysdeviceinfo;
	BGAPIX_CameraInfo cameradeviceinfo;
	
	//this is the base call to find the bgapi_system modules which are necessary to perform any further action
	//every BGPAPI_Function returns a BGAPI_RESULT
	*system_count = 0;

	res = BGAPI::countSystems( system_count );
	
	//You should always check the result to make sure everything works fine
	if( res != BGAPI_RESULT_OK &&  res != BGAPI_RESULT_ALREADYDONE)
	{
		//in case of error you will get a result different from BGAPI_RESULT_OK 
		//all resultcodes are defined in bgapiresult.h and are returned for special reasons
		CString str;
		str.Format("相机序号:%d,BGAPI_CountSystems Errorcode: %d\n",m_nGrabberSN,res);
		m_LastErrorInfo.iErrorCode = res;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "init_systems()函数");
		*externppCamera = NULL;
		return res;
	}

	//CString str;
	//str.Format("system_count:%d",*system_count);
	//AfxMessageBox(str);



	
	for( i = 0; i < *system_count; i++ )
	{
		res = BGAPI::createSystem( i, externppSystem );
		if( res != BGAPI_RESULT_OK &&  res != BGAPI_RESULT_ALREADYDONE)
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::CreateSystem Errorcode: %d Systemnumber %d SysPointer 0x%p\n",m_nGrabberSN,res, i, (void*)(*externppSystem));
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "init_systems()函数");
			*externppCamera = NULL;
			return res;
		}

		state.cbSize = sizeof( BGAPI_FeatureState );
		sysdeviceinfo.cbSize = sizeof( BGAPIX_SystemInfo );
		res = (*externppSystem)->getDeviceInformation( &state, &sysdeviceinfo );
		if( res != BGAPI_RESULT_OK )
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::System::getDeviceInformation Systemnumber %d Errorcode: %d\n",m_nGrabberSN,i, res) ;
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "init_systems()函数");
			*externppCamera = NULL;
			return res;
	
		}
		else
		{
			if(sysdeviceinfo.type == BGAPIX_DEVICETYPE_COMMONPCI_NIC)
			{
				//AfxMessageBox("NIC_OK");

				res = (*externppSystem)->open();		
				if( res != BGAPI_RESULT_OK &&  res != BGAPI_RESULT_ALREADYDONE)
				{
					CString str;
					str.Format("相机序号:%d,BGAPI::System::open Systemnumber %d Errorcode: %d\n",m_nGrabberSN,i, res );
					m_LastErrorInfo.iErrorCode = res;
					_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
					_tprintf_s(m_LastErrorInfo.strErrorRemark, "init_systems()函数");
					*externppCamera = NULL;
					return res;
				}

				if (res == BGAPI_RESULT_OK)
				{
					m_ppSystem[i] = *externppSystem;
				}


				//////////////////////////////////////////////////////////////////////////
				int count = 0;
				//this is an example how to count available cameras for all available systems
				res = (*externppSystem)->countCameras( &count);		
				if( res != BGAPI_RESULT_OK &&  res != 2)
				{
					CString str;
					str.Format("相机序号:%d,BGAPI::System::countCameras Systemnumber %d Errorcode: %d\n",m_nGrabberSN,i, res );
					m_LastErrorInfo.iErrorCode = res;
					_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
					_tprintf_s(m_LastErrorInfo.strErrorRemark, "init_systems()函数");
					*externppCamera = NULL;
					(*externppSystem)->release();
					return res;
				}


				
				int cam = 0;
				for( cam = 0; cam < count; cam++ )
				{	
					res = (*externppSystem)->createCamera( cam, externppCamera);
					if( res != BGAPI_RESULT_OK &&  res != BGAPI_RESULT_ALREADYDONE)
					{
					//	printf("\n");
					//	printf( "BGAPI::System::createCamera Systemnumber %d Errorcode: %d\n", systemIter - externppSystem->begin(), res );
						*externppCamera = NULL;
						(*externppSystem)->release();

						CString str;
						str.Format("相机序号:%d,BGAPI::Camera::createCamera Errorcode: %d\n", m_nGrabberSN,res);
						m_LastErrorInfo.iErrorCode = res;
						_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
						_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_systems()函数");

						return res;
					}
					if (res == BGAPI_RESULT_OK)
					{
						m_SystemCamera[m_CameraNum].m_pSystem = *externppSystem;
						m_SystemCamera[m_CameraNum].m_pCamera = *externppCamera;
						m_CameraNum++;
					}
					
					//this is an example how to get the device information for a camera
					state.cbSize = sizeof( BGAPI_FeatureState );
					cameradeviceinfo.cbSize = sizeof( BGAPIX_CameraInfo );
					res = (*externppCamera)->getDeviceInformation( &state, &cameradeviceinfo );
					if( res != BGAPI_RESULT_OK )
					{	
						*externppCamera = NULL;
						(*externppSystem)->release();

						CString str;
						str.Format("相机序号:%d,BGAPI::Camera::getDeviceInformation Errorcode: %d\n",m_nGrabberSN, res);
						m_LastErrorInfo.iErrorCode = res;
						_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
						_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_systems()函数");

						return res;
					}
					
					CString str,str1;
					str.Format("%s",cameradeviceinfo.modelName);
					str1.Format("%s",cameradeviceinfo.serialNumber);

					if (str == m_sDeviceName && str1 == m_strDeviceMark)
					{
						res = (*externppCamera)->open();
						if( res != BGAPI_RESULT_OK )
						{
						//	printf("\n");
						//	printf( "BGAPI::Camera::open Systemnumber %d Errorcode: %d\n", *pCurrSystem, res );
							*externppCamera = NULL;
							(*externppSystem)->release();

							CString str;
							str.Format("相机序号:%d,BGAPI::Camera::open  Errorcode: %d\n",m_nGrabberSN, res);
							m_LastErrorInfo.iErrorCode = res;
							_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
							_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_systems()函数");
							return res;
						}
						else
						{
							//////////////////////////////////////////////////////////////////////////
							int j = 0;
							BGAPIX_TypeListINT listint;
							BGAPI_FeatureState state;
							BGAPIX_TypeINT intNumberofbuffer;
							BGAPIX_TypeRangeINT dataLen;
							BGAPIX_TypeListINT  tDefaultUserSet;
							BGAPIX_TypeArrayINT  tUserSet;
							listint.cbSize = sizeof( BGAPIX_TypeListINT );
							state.cbSize = sizeof( BGAPI_FeatureState );
							intNumberofbuffer.cbSize = sizeof( BGAPIX_TypeINT );
							dataLen.cbSize = sizeof ( BGAPIX_TypeRangeINT );
							tDefaultUserSet.cbSize = sizeof ( BGAPIX_TypeListINT);
							tUserSet.cbSize = sizeof ( BGAPIX_TypeArrayINT);
							//////////////////////////////////////////////////////////////////////////
						//	BGAPI_RESULT getUserSet( BGAPI_FeatureState * pstate, BGAPIX_TypeArrayINT * tUserSet );
// 							res = (*externppCamera)->getUserSet( &state, &tUserSet);
// 							if( res != BGAPI_RESULT_OK )
// 							{
// 								CString str;
// 								str.Format("getUserSetDefault returned with errorcode %d\n", res);
// 								m_LastErrorInfo.iErrorCode = res;
// 								_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
// 								_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_systems()函数");
// 								return res;
// 							}

							res = (*externppCamera)->getUserSetDefault( &state, &tDefaultUserSet);
							if( res != BGAPI_RESULT_OK )
							{
								CString str;
								str.Format("相机序号:%d,getUserSetDefault returned with errorcode %d\n", m_nGrabberSN,res);
								m_LastErrorInfo.iErrorCode = res;
								_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
								_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_systems()函数");
								return res;
							}
							
							if((BGAPI_UserSet)tDefaultUserSet.array[tDefaultUserSet.current] != BGAPI_USERSET_1)
							{
								res = (*externppCamera)->setUserSet( BGAPI_USERSET_1 );
								if( res != BGAPI_RESULT_OK &&  res != BGAPI_RESULT_FEATURE_LOCKED )//1
								{
									CString str;
									str.Format("相机序号:%d,BGAPI::Camera::setUserSet Errorcode: %d\n",m_nGrabberSN,res);
									m_LastErrorInfo.iErrorCode = res;
									_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
									_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_systems()函数");
									return res;
								}

								if(res != BGAPI_RESULT_FEATURE_LOCKED)//1
								{
									res = (*externppCamera)->setUserSetAsDefault( BGAPI_USERSET_1 );
									if( res != BGAPI_RESULT_OK )
									{
										CString str;
										str.Format("相机序号:%d,BGAPI::Camera::setUserSetAsDefault Errorcode: %d\n",m_nGrabberSN,res);
										m_LastErrorInfo.iErrorCode = res;
										_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
										_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_systems()函数");
										return res;
									}	
								}
							}

							//////////////////////////////////////////////////////////////////////////
							res = (*externppCamera)->getDataAccessMode( &state, &listint, &intNumberofbuffer );
							if( res != BGAPI_RESULT_OK )
							{
								CString str;
								str.Format("相机序号:%d,getDataAccessMode returned with errorcode %d\n", m_nGrabberSN,res);
								m_LastErrorInfo.iErrorCode = res;
								_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
								_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_systems()函数");
								return res;
							}

							if (((BGAPI_DataAccessMode)listint.array[listint.current] != BGAPI_DATAACCESSMODE_NORMALINTERN)//BGAPI_DATAACCESSMODE_NORMALINTERN)//BGAPI_DATAACCESSMODE_QUEUEDINTERN)
								|| (intNumberofbuffer.current != ImageMaxNum))
							{
					//			m_bSet3 = TRUE;
								res = (*externppCamera)->setDataAccessMode(BGAPI_DATAACCESSMODE_NORMALINTERN, ImageMaxNum);
								if (res != BGAPI_RESULT_OK)
								{
									CString str;
									str.Format("相机序号:%d,setDataAccessMode returned with errorcode %d\n", m_nGrabberSN,res);
									m_LastErrorInfo.iErrorCode = res;
									_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
									_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_systems()函数");
									return res;
								}

								res = (*externppCamera)->getDataAccessMode( &state, &listint, &intNumberofbuffer );
								if( res != BGAPI_RESULT_OK )
								{
									CString str;
									str.Format("相机序号:%d,getDataAccessMode returned with errorcode %d\n", m_nGrabberSN,res);
									m_LastErrorInfo.iErrorCode = res;
									_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
									_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_systems()函数");
									return res;
								}

							}
	
							if( (BGAPI_DataAccessMode)listint.array[listint.current] == BGAPI_DATAACCESSMODE_NORMALINTERN)//BGAPI_DATAACCESSMODE_QUEUEDINTERN)// || (BGAPI_DataAccessMode)listint.array[listint.current] == BGAPI_DATAACCESSMODE_QUEUEDEXTERN )
							{
								//create image objects
								for(j = 0; j < intNumberofbuffer.current; j++)
								{
									res = BGAPI::createImage( &m_pImage[j] ); 
									if( res != BGAPI_RESULT_OK )
									{
										CString str;
										str.Format("相机序号:%d,BGAPI::createImage for Image %d returned with errorcode %d\n",m_nGrabberSN, j, res );
										m_LastErrorInfo.iErrorCode = res;
										_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
										_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_systems()函数");
										return res;
									}
		
									res = (*externppCamera)->setImage( m_pImage[j] );
									if( res != BGAPI_RESULT_OK )
									{
										CString str;
										str.Format("相机序号:%d,BGAPI::Camera::setImage for Image %d returned with errorcode %d\n",m_nGrabberSN, i, res );
										m_LastErrorInfo.iErrorCode = res;
										_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
										_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_systems()函数");
										return res;
									}
								}
							}
							else
							{
								res = BGAPI_RESULT_FAIL;
								CString str;
								str.Format("相机序号:%d,Error %d DataAccessMode fail\n",m_nGrabberSN, res );
								m_LastErrorInfo.iErrorCode = res;
								_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
								_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_systems()函数");
								return res;
							}

						
							res = (*externppCamera)->registerNotifyCallback( this, (BGAPI::BGAPI_NOTIFY_CALLBACK)&imageCallback );
							if( res != BGAPI_RESULT_OK )
							{
								CString str;
								str.Format("相机序号:%d,Error %d while register NotifyCallback.\n",m_nGrabberSN, res );
								m_LastErrorInfo.iErrorCode = res;
								_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
								_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_systems()函数");
								return res;
							}

							//////////////////////////////////////////////////////////////////////////
							return res;
						}	
					}
				}//cam

			}//sysdeviceinfo.type
			else
			{
				(*externppSystem)->release();
			}
		}//getDeviceInformation
	}//system_count
	*externppCamera = NULL;
	return BGAPI_RESULT_FAIL;
}

int CBaumerCamDH::init_camera()
{
	BGAPI_RESULT res = BGAPI_RESULT_FAIL;
	BGAPI_FeatureState state;
	BGAPIX_TypeListINT listint;
	BGAPIX_TypeListINT tDefaultUserSet;
	BGAPIX_TypeListINT TestPatternlist;
	BGAPIX_CameraImageFormat pImageformat;
	BGAPIX_CameraPixelFormat pPixelformat;
	BGAPIX_TypeListINT pixellist;
	BGAPIX_TypeRangeFLOAT tframerate;
	BGAPIX_TypeROI roi; 
	state.cbSize = sizeof( BGAPI_FeatureState );
	tDefaultUserSet.cbSize = sizeof( BGAPIX_TypeListINT );
	TestPatternlist.cbSize = sizeof( BGAPIX_TypeListINT );
	pixellist.cbSize = sizeof( BGAPIX_TypeListINT );
	listint.cbSize = sizeof( BGAPIX_TypeListINT );
	pImageformat.cbSize = sizeof( BGAPIX_CameraImageFormat );
	pPixelformat.cbSize = sizeof( BGAPIX_CameraPixelFormat );
	tframerate.cbSize = sizeof( BGAPIX_TypeRangeFLOAT );
	roi.cbSize = sizeof( BGAPIX_TypeROI );
	CString str1,str2;
	int formatindex = -1;

	//////////////////////////////////////////////////////////////////////////
	m_nSequencerNum = GetPrivateProfileInt("camera", "sequencernum",14, m_sInitFile);
	m_nTrigger = GetPrivateProfileInt("camera", "trigger",-1,m_sInitFile);
	if (m_nTrigger == -1)
	{
		WritePrivateProfileString("camera", "trigger", "0", m_sInitFile);
		m_nTrigger = 0;
	}
	//////////////////////////////////////////////////////////////////////////
	//GET IMAGE FORMAT
	res = m_pCamera->getImageFormat( &state, &listint );
	if( res != BGAPI_RESULT_OK )
	{
		CString str;
		str.Format("相机序号:%d,BGAPI::Camera::getImageFormat1 Errorcode: %d\n",m_nGrabberSN,res);
	//	AfxMessageBox(str);
		m_LastErrorInfo.iErrorCode = res;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
		return res;
	}
		
	for( int k = 0; k < listint.length; k++ )
	{
		res = m_pCamera->getImageFormatDescription(listint.array[k], &pImageformat );
		if( res != BGAPI_RESULT_OK )
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::Camera::getImageFormatDescription Errorcode: %d\n",m_nGrabberSN,res);
		//	AfxMessageBox(str);
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
			return res;	
		}
			
		str1.Format("%s",(char*)pImageformat.sName);
		if (str1 == "Full Frame")
		{
			if (listint.array[listint.current] != pImageformat.iFormat)
			{
				m_bSet3 = TRUE;
				res = m_pCamera->setImageFormat(pImageformat.iFormat);
				if( res != BGAPI_RESULT_OK )
				{
					CString str;
					str.Format("相机序号:%d,BGAPI::Camera::setPixelFormat Errorcode: %d\n",m_nGrabberSN,res);
				//	AfxMessageBox(str);
					m_LastErrorInfo.iErrorCode = res;
					_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
					_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
					return res;
				}

				res = m_pCamera->getImageFormat( &state, &listint );
				if( res != BGAPI_RESULT_OK )
				{
					CString str;
					str.Format("相机序号:%d,BGAPI::Camera::getImageFormat2 Errorcode: %d\n",m_nGrabberSN,res);
					//	AfxMessageBox(str);
					m_LastErrorInfo.iErrorCode = res;
					_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
					_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
					return res;
				}


			}
			m_nImageWidth = pImageformat.iSizeX;
			m_nImageHeight = pImageformat.iSizeY;
			formatindex = pImageformat.iFormat;
			m_nImageBufferSize = m_nImageWidth*m_nImageHeight;
			m_bSet1 = TRUE;
			break;
		}		
	}

	if (m_bSet1 == FALSE)
	{
		m_LastErrorInfo.iErrorCode = BGAPI_RESULT_FAIL;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, "m_pCamera == NULL 1");
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
		return BGAPI_RESULT_FAIL;
	}

	//////////////////////////////////////////////////////////////////////////
	res = m_pCamera->getChunckMode(&state);
	if( res != BGAPI_RESULT_OK )
	{
		if (res != BGAPI_RESULT_FEATURE_NOTAVAILABLE)
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::Camera::getChunckMode Errorcode: %d\n",m_nGrabberSN,res);
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
			return res;
		}
	}
	else
	{
		if (!state.bIsEnabled)
		{
			m_bSet3 = TRUE;
			res = m_pCamera->setChunckMode(true);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::setChunckMode Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
				return res;
			}
		}	
	}

	//////////////////////////////////////////////////////////////////////////
	//GET PIXEL FORMAT
	res = m_pCamera->getPixelFormat( formatindex, &state, &pixellist );
	if( res != BGAPI_RESULT_OK )
	{
		CString str;
		str.Format("相机序号:%d,BGAPI::Camera::getPixelFormat Errorcode: %d\n",m_nGrabberSN,res);
	//	AfxMessageBox(str);
		m_LastErrorInfo.iErrorCode = res;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
		return res;
	}
	//GET PIXEL FORMAT DESCRIPTION FOR ALL AVAILABLE PIXEL FORMATS AND MARK CURRENT	
	for( int p = 0; p<pixellist.length; p++ )
	{
		res = m_pCamera->getPixelFormatDescription( listint.current, pixellist.array[p], &pPixelformat );
		if( res != BGAPI_RESULT_OK )
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::Camera::getPixelFormatDescription Errorcode: %d\n",m_nGrabberSN,res);
		//	AfxMessageBox(str);
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
			return res;
		}
		
		str1.Format("%s",(char*)pPixelformat.sName);
		str2.Format("%d",pPixelformat.iPixelBytes);
		if (str1 == "Mono8" && str2 == "1")
		{
			if (pixellist.array[pixellist.current] != pPixelformat.iPixelFormat)
			{
				m_bSet3 = TRUE;

				res = m_pCamera->setPixelFormat(pPixelformat.iPixelFormat);
				if( res != BGAPI_RESULT_OK )
				{
					CString str;
					str.Format("相机序号:%d,BGAPI::Camera::setPixelFormat Errorcode: %d\n",m_nGrabberSN,res);
				//	AfxMessageBox(str);
					m_LastErrorInfo.iErrorCode = res;
					_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
					_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
					return res;
				}
			}
			m_nImageByteCount =pPixelformat.iPixelBytes;
			m_bSet2 = TRUE;
			break;
		}		
	}
	if (m_bSet2 == FALSE)
	{
		m_LastErrorInfo.iErrorCode = BGAPI_RESULT_FAIL;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, "m_pCamera == NULL 2");
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
		return BGAPI_RESULT_FAIL;
	}
	//////////////////////////////////////////////////////////////////////////
	BGAPIX_TypeRangeINT PacketSize;
	PacketSize.cbSize = sizeof( BGAPIX_TypeRangeINT );
	res = m_pCamera->getPacketSize( &state, &PacketSize );
	if( res != BGAPI_RESULT_OK )
	{
		if (res != BGAPI_RESULT_FEATURE_NOTAVAILABLE)
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::Camera::getPacketSize Errorcode: %d\n",m_nGrabberSN,res);
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
			return res;
		}
	}
	else
	{
		int nTemp = (int)(PacketSize.maximum*0.8);

		if (PacketSize.current < nTemp)
		{
			m_bSet3 = TRUE;
			res = m_pCamera->setPacketSize(nTemp);
			if( res != BGAPI_RESULT_OK && res != BGAPI_RESULT_FEATURE_NOTIMPLEMENTED)
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::setPacketSize Errorcode: %d\n",m_nGrabberSN,res);
			//	AfxMessageBox(str);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
				return res;
			}
		}	
	}
	
	//////////////////////////////////////////////////////////////////////////

// 	BGAPIX_TypeListINT ExposureMode;
// 	ExposureMode.cbSize = sizeof( BGAPIX_TypeListINT );
// 	res = m_pCamera->getExposureMode( &state, &ExposureMode );
// 	if( res != BGAPI_RESULT_OK )
// 	{
// 		AfxMessageBox("m_pCamera11 == NULL 2");
// 		return 0;
// 	}
// 	else
// 	{
// 		if (ExposureMode.current != BGAPI_EXPOSUREMODE_TIMED)
// 		{
// 			res = m_pCamera->setExposureMode(BGAPI_EXPOSUREMODE_TIMED);
// 			if( res != BGAPI_RESULT_OK && res != BGAPI_RESULT_FEATURE_NOTIMPLEMENTED)
// 			{
// 				CString str;
// 				str.Format("BGAPI::Camera::setExposureMode Errorcode: %d\n",m_nGrabberSN,res);
// 				AfxMessageBox(str);
// 				return 0;
// 			}
// 		}
// 	}

	/**
	\brief Function category: BO Extended API.
	\brief Use this function to select the automatic mode for an automatic operation. 
	\param 'automode' (IN) It can be one of the following modes: off, once or continuous mode.
	\retval 'BGAPI_RESULT_OK'	No error.
	*/	
//	BGAPI_RESULT setExposureAutoMode( BGAPI_AutomaticMode automode );

	/**
	\brief Function category: BO Extended API.
	\brief This function returns information about the curent and available automatic modes for auto exposure control.
	\param 'pstate' (OUT) Current feature state.
	\param 'tAutoMode' (OUT) Current automatic mode and a list with available modes.
	\retval 'BGAPI_RESULT_OK'	No error.
	*/	
//	BGAPI_RESULT getExposureAutoMode( BGAPI_FeatureState * pstate, BGAPIX_TypeListINT * tAutoMode );

	if (m_nTrigger == 0)
	{
		BGAPIX_TypeListINT tAutoMode;
		tAutoMode.cbSize = sizeof(BGAPIX_TypeListINT); 
		res = m_pCamera->getExposureAutoMode( &state, &tAutoMode);
		if( res != BGAPI_RESULT_OK)
		{
			if (res != BGAPI_RESULT_FEATURE_NOTAVAILABLE)
			{
				CString str;
				str.Format("相机序号:%d,getExposureAutoMode returned with errorcode %d\n",m_nGrabberSN, res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
				return res;
			}

		}
		else
		{
			if (((BGAPI_AutomaticMode)tAutoMode.array[tAutoMode.current]) != BGAPI_AUTOMATICMODE_OFF)
			{
				m_bSet3 = TRUE;
				res = m_pCamera->setExposureAutoMode(BGAPI_AUTOMATICMODE_OFF);
				if( res != BGAPI_RESULT_OK )
				{
					CString str;
					str.Format("相机序号:%d,setExposureAutoMode returned with errorcode %d\n", m_nGrabberSN,res);
					m_LastErrorInfo.iErrorCode = res;
					_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
					_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
					return res;
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		BGAPIX_TypeRangeINT rangedint;					
		rangedint.cbSize = sizeof( BGAPIX_TypeRangeINT );
		res = m_pCamera->getExposure( &state, &rangedint );
		if( res != BGAPI_RESULT_OK )
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::Camera::getExposure Errorcode: %d\n",m_nGrabberSN,res);
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
			return res;
		}
		else
		{
			m_nExMax =  rangedint.maximum;
			m_nExMin =  rangedint.minimum;
		}

		int nEx = GetPrivateProfileInt("camera", "exposure",1000, m_sInitFile);
		if (nEx < m_nExMin)
		{
			nEx = m_nExMin;
			CString str;
			str.Format("%d",nEx);
			WritePrivateProfileString("camera", "exposure", str, m_sInitFile);
		}

		if (nEx > m_nExMax)
		{
			nEx = m_nExMax;
			CString str;
			str.Format("%d",nEx);
			WritePrivateProfileString("camera", "exposure", str, m_sInitFile);
		}

		if (nEx != rangedint.current)
		{
			m_bSet3 = TRUE;
			BOOL bRet = SetParamInt(GBParamID::GBSequenceNum, nEx);//:GBShutter,	//相机快门值
			if (!bRet)
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::BaumerSetParamPro Exposure Errorcode: %d\n",m_nGrabberSN,bRet);
				m_LastErrorInfo.iErrorCode = bRet;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
				return BGAPI_RESULT_FAIL;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		BGAPIX_TypeRangeFLOAT gain;	
		gain.cbSize = sizeof( BGAPIX_TypeRangeFLOAT );
		res = m_pCamera->getGain(&state, &gain);
		if( res != BGAPI_RESULT_OK )
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::Camera::getGain Errorcode: %d\n",m_nGrabberSN,res);
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
			return res;
		}
		else
		{
			m_nGainMax =  gain.maximum;
			m_nGainMin =  gain.minimum;
		}

		CString sGain;

		GetPrivateProfileStringA("camera", "gain","1", sGain.GetBuffer(100),100,m_sInitFile);
		float nGain = (float)atof(sGain);

		if (nGain < m_nGainMin)
		{
			nGain = m_nGainMin;
			CString str;
			str.Format("%f",nGain);
			WritePrivateProfileString("camera", "gain", str, m_sInitFile);
		}
	
		if (nGain > m_nGainMax)
		{
			nGain = m_nGainMax;
			CString str;
			str.Format("%f",nGain);
			WritePrivateProfileString("camera", "gain", str, m_sInitFile);
		}

		if (abs(nGain - gain.current) > 0.05)
		{
			m_bSet3 = TRUE;
			BOOL bRet = SetParamfloat(9, nGain);
			if (!bRet)
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::SetParamfloat Gain Errorcode: %d\n",m_nGrabberSN,bRet);
				m_LastErrorInfo.iErrorCode = bRet;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
				return 0;
			}
		}
	}
//////////////////////////////////////////////////////////////////////////
	res = m_pCamera->getDefectPixelCorrection(&state);
	if( res != BGAPI_RESULT_OK )
	{
		if (res != BGAPI_RESULT_FEATURE_NOTAVAILABLE)
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::Camera::getDefectPixelCorrection Errorcode: %d\n",m_nGrabberSN,res);
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
			return res;
		}
	}
	else
	{
		if (!state.bIsEnabled)
		{
			m_bSet3 = TRUE;
			res = m_pCamera->setDefectPixelCorrection(true);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::setDefectPixelCorrection Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
				return res;
			}
		}	
	}
//////////////////////////////////////////////////////////////////////////
	res = m_pCamera->getFlash(&state);
	if( res != BGAPI_RESULT_OK )
	{
		if (res != BGAPI_RESULT_FEATURE_NOTAVAILABLE)
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::Camera::getFlash Errorcode: %d\n",m_nGrabberSN,res);
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
			return res;
		}
	}
	else
	{
		if (state.bIsEnabled)
		{
			m_bSet3 = TRUE;
			res = m_pCamera->setFlash(false);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::setFlash Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
				return res;
			}
		}	
	}

//////////////////////////////////////////////////////////////////////////

	res = m_pCamera->getFramesPerSecondsContinuous(&state,&tframerate);
	if( res != BGAPI_RESULT_OK )
	{
		if (res != BGAPI_RESULT_FEATURE_NOTAVAILABLE)
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::Camera::getFramesPerSecondsContinuous Errorcode: %d\n",m_nGrabberSN,res);
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
			return res;
		}
	}
	else
	{
		if (tframerate.current != 0)
		{
			m_bSet3 = TRUE;
			float framerate  = 0;
			res = m_pCamera->setFramesPerSecondsContinuous(framerate);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,setFramesPerSecondsContinuous returned with errorcode %d\n",m_nGrabberSN, res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
				return res;
			}
		}
	}
//////////////////////////////////////////////////////////////////////////
	res = m_pCamera->getTestPattern( &state, &TestPatternlist);
	if( res != BGAPI_RESULT_OK )
	{
		if (res != BGAPI_RESULT_FEATURE_NOTAVAILABLE)
		{
			CString str;
			str.Format("相机序号:%d,getTestPattern returned with errorcode %d\n",m_nGrabberSN, res);
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
			return res;
		}
	}

	if (state.bIsEnabled == true)
	{
		m_bSet3=TRUE;
		res = m_pCamera->setTestPattern( false, BGAPI_TESTPATTERN_HORI);
		if( res != BGAPI_RESULT_OK )
		{
			CString str;
			str.Format("相机序号:%d,setTestPattern returned with errorcode %d\n",m_nGrabberSN, res);
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
			return res;
		}
	}


//////////////////////////////////////////////////////////////////////////
	res = m_pCamera->getGVSHeartBeatEnable(&state);
	if( res != BGAPI_RESULT_OK )
	{
		if (res != BGAPI_RESULT_FEATURE_NOTAVAILABLE)
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::Camera::getGVSHeartBeatEnable Errorcode: %d\n",m_nGrabberSN,res);
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
			return res;
		}
	}
	else
	{
		if (!state.bIsEnabled)
		{
 			m_bSet3 = TRUE;
			res = m_pCamera->setGVSHeartBeatEnable(true);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::setGVSHeartBeatEnable Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
				return res;
			}
		}
	}

 	BGAPIX_TypeRangeINT timeinms;
	timeinms.cbSize = sizeof(BGAPIX_TypeRangeINT); 
 	res = m_pCamera->getGVSHeartBeatTimeout( &state, &timeinms);
	if( res != BGAPI_RESULT_OK )
	{
		if (res != BGAPI_RESULT_FEATURE_NOTAVAILABLE)
		{
			CString str;
			str.Format("相机序号:%d,getGVSHeartBeatTimeout returned with errorcode %d\n", m_nGrabberSN,res);
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
			return res;
		}
	}

	if (timeinms.current != 3000)
	{
		m_bSet3 = TRUE;
		res = m_pCamera->setGVSHeartBeatTimeout(3000);
		if( res != BGAPI_RESULT_OK )
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::Camera::setGVSHeartBeatTimeout Errorcode: %d\n",m_nGrabberSN,res);
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
			return res;
		}
	}

//////////////////////////////////////////////////////////////////////////
 	BGAPIX_TypeINT tPacketDelay;//1
 	tPacketDelay.cbSize = sizeof(BGAPIX_TypeINT); 
 	res = m_pCamera->getGVSPacketDelay( &state, &tPacketDelay);
 	if( res != BGAPI_RESULT_OK )
 	{
 		if (res != BGAPI_RESULT_FEATURE_NOTAVAILABLE)
 		{
 			CString str;
 			str.Format("相机序号:%d,getGVSPacketDelay returned with errorcode %d\n",m_nGrabberSN, res);
 			m_LastErrorInfo.iErrorCode = res;
 			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
 			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
 			return res;
 		}
 	}

//  [4/25/2014 SJC]
// 	if(m_sDeviceName != "VLG-02M")
// 	{
// 		if (tPacketDelay.current != 50)
// 		{
// 			m_bSet3 = TRUE;
// 			res = m_pCamera->setGVSPacketDelay(50);
// 			if( res != BGAPI_RESULT_OK )
// 			{
// 				CString str;
// 				str.Format("相机序号:%d,BGAPI::Camera::setGVSPacketDelay Errorcode: %d\n",m_nGrabberSN,res);
// 				m_LastErrorInfo.iErrorCode = res;
// 				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
// 				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
// 				return res;
// 			}
// 		}
// 	}
// 	else
// 	{
// 		if (tPacketDelay.current != 2282)
// 		{
// 			m_bSet3 = TRUE;
// 			res = m_pCamera->setGVSPacketDelay(2282);
// 			if( res != BGAPI_RESULT_OK )
// 			{
// 				CString str;
// 				str.Format("相机序号:%d,BGAPI::Camera::setGVSPacketDelay Errorcode: %d\n",m_nGrabberSN,res);
// 				m_LastErrorInfo.iErrorCode = res;
// 				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
// 				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
// 				return res;
// 			}
// 		}
// 	}
 	


//////////////////////////////////////////////////////////////////////////
	BGAPIX_TypeListINT tSource;
	tSource.cbSize = sizeof(BGAPIX_TypeListINT);
	res = m_pCamera->getTimerTriggerSource(BGAPI_TIMER_1,&state,&tSource);
	if( res != BGAPI_RESULT_OK )
	{
		if (res != BGAPI_RESULT_FEATURE_NOTAVAILABLE)
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::Camera::getTimerTriggerSource Errorcode: %d\n",m_nGrabberSN,res);
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
			return res;
		}
	}
	else
	{
		if ((BGAPI_TriggerSource)(tSource.array[tSource.current]) != BGAPI_TRIGGERSOURCE_OFF)
		{
			m_bSet3 = TRUE;
			res = m_pCamera->setTimerTriggerSource(BGAPI_TIMER_1,BGAPI_TRIGGERSOURCE_OFF);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::setTimerTriggerSource Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
				return res;
			}
		}
	}

//////////////////////////////////////////////////////////////////////////
	BGAPIX_TypeRangeINT tDelay;
	tDelay.cbSize = sizeof(BGAPIX_TypeRangeINT);
	res = m_pCamera->getTransmissionDelay(&state,&tDelay);
	if( res != BGAPI_RESULT_OK )
	{
		if (res != BGAPI_RESULT_FEATURE_NOTAVAILABLE)
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::Camera::getTransmissionDelay Errorcode: %d\n",m_nGrabberSN,res);
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
			return res;
		}
	}
	else
	{
		if (tDelay.current != 0)
		{
			m_bSet3 = TRUE;
			res = m_pCamera->setTransmissionDelay(0);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::setTransmissionDelay Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
				return res;
			}
		}
	}

//////////////////////////////////////////////////////////////////////////
	BGAPIX_TypeRangeINT sint;
	sint.cbSize = sizeof(BGAPIX_TypeRangeINT);
	res = m_pCamera->getOffset(&state,&sint);
	if( res != BGAPI_RESULT_OK )
	{
		CString str;
		str.Format("相机序号:%d,BGAPI::Camera::getOffset Errorcode: %d\n",m_nGrabberSN,res);
		m_LastErrorInfo.iErrorCode = res;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
		return res;
	}
	else
	{
		if (sint.current != 0)
		{
			m_bSet3 = TRUE;
			res = m_pCamera->setOffset(0);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::setOffset Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
				return res;
			}
		}
	}

//////////////////////////////////////////////////////////////////////////
	BGAPIX_TypeArrayBYTE tMulticastIP;
	tMulticastIP.cbSize = sizeof(BGAPIX_TypeArrayBYTE);
	res = m_pCamera->getMulticast(&state,&tMulticastIP);
	if( res != BGAPI_RESULT_OK )
	{
		if (res != BGAPI_RESULT_FEATURE_NOTAVAILABLE)
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::Camera::getMulticast Errorcode: %d\n",m_nGrabberSN,res);
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
			return res;
		}
	}
	else
	{
		if (state.bIsEnabled)
		{
			m_bSet3 = TRUE;
			res = m_pCamera->setMulticast(false,tMulticastIP.array);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::setMulticast Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
				return res;
			}
		}
	}

	BGAPIX_TypeArrayBYTE tMulticastMessageIP;
	tMulticastMessageIP.cbSize = sizeof(BGAPIX_TypeArrayBYTE);
	res = m_pCamera->getMulticastMessage(&state,&tMulticastMessageIP);
	if( res != BGAPI_RESULT_OK )
	{
		if (res != BGAPI_RESULT_FEATURE_NOTAVAILABLE)
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::Camera::getMulticastMessage Errorcode: %d\n",m_nGrabberSN,res);
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
			return res;
		}
	}
	else
	{
		if (state.bIsEnabled)
		{
			m_bSet3 = TRUE;
			res = m_pCamera->setMulticastMessage(false,tMulticastMessageIP.array);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::setMulticastMessage Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
				return res;
			}
		}
	}

//////////////////////////////////////////////////////////////////////////
	res = m_pCamera->getLookUpTableState(BGAPI_LOOKUPTABLE_LUMINANCE,&state);
	if( res != BGAPI_RESULT_OK )
	{
		if (res != BGAPI_RESULT_FEATURE_NOTAVAILABLE)
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::Camera::getLookUpTableState Errorcode: %d\n",m_nGrabberSN,res);
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
			return res;
		}
	}
	else
	{
		if (state.bIsEnabled)
		{
			m_bSet3 = TRUE;
			res = m_pCamera->setLookUpTableState(BGAPI_LOOKUPTABLE_LUMINANCE,false);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::setLookUpTableState Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
				return res;
			}
		}
	}
//////////////////////////////////////////////////////////////////////////
	int nleft,ntop,nright,nbottom;
	res = m_pCamera->getPartialScan(&state,&roi);
	if( res != BGAPI_RESULT_OK )
	{
		if (res != BGAPI_RESULT_FEATURE_NOTAVAILABLE)
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::Camera::getPartialScan Errorcode: %d\n",m_nGrabberSN,res);
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
			return res;
		}
	}
	else
	{
		
		nleft = GetPrivateProfileInt("camera", "roileft",9999,m_sInitFile);
		if (nleft == 9999)
		{
			nleft = 0;
			CString str;
			str.Format("%d",nleft);
			WritePrivateProfileString("camera", "roileft", str, m_sInitFile);
		}


		if (nleft%4!=0)
		{
			nleft = nleft-nleft%4;
		}
		if (nleft < roi.minleft)
		{
			nleft = roi.minleft;
			CString str;
			str.Format("%d",nleft);
			WritePrivateProfileString("camera", "roileft", str, m_sInitFile);
		}
		
		if (nleft > roi.maxleft)
		{
			nleft = roi.maxleft;
			CString str;
			str.Format("%d",nleft);
			WritePrivateProfileString("camera", "roileft", str, m_sInitFile);
		}
		
		ntop = GetPrivateProfileInt("camera", "roitop",9999,m_sInitFile);
		if (ntop == 9999)
		{
			ntop = 0;
			CString str;
			str.Format("%d",ntop);
			WritePrivateProfileString("camera", "roitop", str, m_sInitFile);
		}




		if (ntop < roi.mintop)
		{
			ntop = roi.mintop;
			CString str;
			str.Format("%d",ntop);
			WritePrivateProfileString("camera", "roitop", str, m_sInitFile);
		}
		if (ntop > roi.maxtop)
		{
			ntop = roi.maxtop;
			CString str;
			str.Format("%d",ntop);
			WritePrivateProfileString("camera", "roitop", str, m_sInitFile);
		}
		
		nright = GetPrivateProfileInt("camera", "roiright",9999,m_sInitFile);
		if (nright%4!=0)
		{
			nright = nright-nright%4;
		}
		if (nright < roi.minright)
		{
			nright = roi.minright;
			CString str;
			str.Format("%d",nright);
			WritePrivateProfileString("camera", "roiright", str, m_sInitFile);
		}
		if (nright > roi.maxright)
		{
			nright = roi.maxright;
			CString str;
			str.Format("%d",nright);
			WritePrivateProfileString("camera", "roiright", str, m_sInitFile);
		}
		
		nbottom = GetPrivateProfileInt("camera", "roibottom",9999,m_sInitFile);
		if (nbottom < roi.minbottom)
		{
			nbottom = roi.minbottom;
			CString str;
			str.Format("%d",nbottom);
			WritePrivateProfileString("camera", "roibottom", str, m_sInitFile);
		}
		if (nbottom > roi.maxbottom)
		{
			nbottom = roi.maxbottom;
			CString str;
			str.Format("%d",nbottom);
			WritePrivateProfileString("camera", "roibottom", str, m_sInitFile);
		}


		if (state.bIsEnabled != true 
			|| roi.curleft != nleft 
			|| roi.curtop != ntop 
			|| roi.curright != nright 
			|| roi.curbottom != nbottom)
		{
			m_bSet3 = TRUE;
			res = m_pCamera->setPartialScan(true,nleft,ntop,nright,nbottom);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,setPartialScan returned with errorcode %d\n", m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
				return res;
			}
		}

		m_nImageWidth = nright-nleft;
		m_nImageHeight = nbottom-ntop;
		m_nImageBufferSize = m_nImageWidth*m_nImageHeight;
	}

	//////////////////////////////////////////////////////////////////////////
	res = m_pCamera->getTrigger(&state);
	if( res != BGAPI_RESULT_OK )
	{
		CString str;
		str.Format("相机序号:%d,BGAPI::Camera::getTrigger Errorcode: %d\n",m_nGrabberSN,res);
		m_LastErrorInfo.iErrorCode = res;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
		return res;
	}

// 	int nBeTriggerEnable = GetPrivateProfileInt("camera", "triggerenable",9999,m_sInitFile);
// 	if (nBeTriggerEnable == 9999)
// 	{
// 		WritePrivateProfileString("camera", "triggerenable", "0", m_sInitFile);
// 		nBeTriggerEnable = 0;
// 	}

	if (m_nTrigger == 0)
	{
		if (state.bIsEnabled)
		{
			m_bSet3 = TRUE;
			res = m_pCamera->setTrigger(false);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::getTrigger Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
				return res;
			}
		}	
	}
	if (m_nTrigger == 1)
	{
		if (state.bIsEnabled == false)
		{
			m_bSet3 = TRUE;
			res = m_pCamera->setTrigger(true);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::getTrigger Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
				return res;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		int nTriggerSource = GetPrivateProfileInt("camera", "triggersource",-1,m_sInitFile);
		if (nTriggerSource == -1)
		{
			WritePrivateProfileString("camera", "triggersource", "0", m_sInitFile);
			nTriggerSource = 0;
		}

		BGAPIX_TypeListINT TriggerSourceList;
		TriggerSourceList.cbSize = sizeof ( BGAPIX_TypeListINT );

		res = m_pCamera->getTriggerSource( &state, &TriggerSourceList );
		if( res != BGAPI_RESULT_OK )
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::Camera::getTriggerSource Errorcode: %d\n",m_nGrabberSN,res);
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
			return res;
		}

		if ( TriggerSourceList.array[TriggerSourceList.current] !=  nTriggerSource)
		{
			for ( int t = 0; t<TriggerSourceList.length; t++ )
			{
				if (nTriggerSource == TriggerSourceList.array[t])
				{
					m_bSet3 = TRUE;
					res = m_pCamera->setTriggerSource( BGAPI_TriggerSource(TriggerSourceList.array[t]) );
					if( res != BGAPI_RESULT_OK )
					{
						CString str;
						str.Format("相机序号:%d,BGAPI::Camera::setTriggerSource Errorcode: %d\n",m_nGrabberSN,res);
						m_LastErrorInfo.iErrorCode = res;
						_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
						_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
						return res;
					}

					break;
				}
				else
				{
					if (t == TriggerSourceList.length-1)
					{
						CString str;
						str.Format("相机序号:%d,非法触发源参数!",m_nGrabberSN);
						m_LastErrorInfo.iErrorCode = ErrorSetParam;
						_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
						_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
						return res;
					}
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		if (nTriggerSource != 0)
		{
			int nTriggerActivation = GetPrivateProfileInt("camera", "triggeract",-1,m_sInitFile);
			if (nTriggerActivation == -1)
			{
				WritePrivateProfileString("camera", "triggeract", "2", m_sInitFile);
				nTriggerActivation = 2;
			}

			BGAPIX_TypeListINT TriggerActivation;
			TriggerActivation.cbSize = sizeof ( BGAPIX_TypeListINT );
			res = m_pCamera->getTriggerActivation( &state, &TriggerActivation );
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::getTriggerActivation Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
				return res;
			}

			if (BGAPI_Activation(TriggerActivation.array[TriggerActivation.current]) != BGAPI_Activation(nTriggerActivation))
			{

				for ( int i = 0; i<TriggerActivation.length; i++ )
				{
					if (nTriggerActivation == TriggerActivation.array[i])
					{
						m_bSet3 = TRUE;
						res = m_pCamera->setTriggerActivation(BGAPI_Activation(TriggerActivation.array[i]));
						if( res != BGAPI_RESULT_OK )
						{
							CString str;
							str.Format("相机序号:%d,BGAPI::Camera::setTriggerActivation Errorcode: %d\n",m_nGrabberSN,res);
							m_LastErrorInfo.iErrorCode = res;
							_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
							_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
							return res;
						}
						break;
					}
					else
					{
						if (i == TriggerActivation.length-1)
						{
							CString str;
							str.Format("相机序号:%d,非法触发波形参数!",m_nGrabberSN);
							m_LastErrorInfo.iErrorCode = ErrorSetParam;
							_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
							_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
							return res;
						}
					}
				}	
			}
		}
	}
	else if (m_nTrigger == 9)
	{
		if (state.bIsEnabled == false)
		{
			m_bSet3 = TRUE;
			res = m_pCamera->setTrigger(true);
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::getTrigger Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
				return res;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		int nTriggerSource = GetPrivateProfileInt("camera", "triggersource",9999,m_sInitFile);
		if (nTriggerSource == 9999)
		{
			WritePrivateProfileString("camera", "triggersource", "0", m_sInitFile);
			nTriggerSource = 0;
		}

		BGAPIX_TypeListINT TriggerSourceList;
		TriggerSourceList.cbSize = sizeof ( BGAPIX_TypeListINT );

		res = m_pCamera->getTriggerSource( &state, &TriggerSourceList );
		if( res != BGAPI_RESULT_OK )
		{
			CString str;
			str.Format("相机序号:%d,BGAPI::Camera::getTriggerSource Errorcode: %d\n",m_nGrabberSN,res);
			m_LastErrorInfo.iErrorCode = res;
			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
			return res;
		}

		if ( TriggerSourceList.array[TriggerSourceList.current] !=  nTriggerSource)
		{
			for ( int t = 0; t<TriggerSourceList.length; t++ )
			{
				if (nTriggerSource == TriggerSourceList.array[t])
				{
					m_bSet3 = TRUE;
					res = m_pCamera->setTriggerSource( BGAPI_TriggerSource(TriggerSourceList.array[t]) );
					if( res != BGAPI_RESULT_OK )
					{
						CString str;
						str.Format("相机序号:%d,BGAPI::Camera::setTriggerSource Errorcode: %d\n",m_nGrabberSN,res);
						m_LastErrorInfo.iErrorCode = res;
						_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
						_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
						return res;
					}
					
					break;
				}
				else
				{
					if (t == TriggerSourceList.length-1)
					{
						CString str;
						str.Format("相机序号:%d,非法触发源参数!",m_nGrabberSN);
						m_LastErrorInfo.iErrorCode = ErrorSetParam;
						_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
						_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
						return res;
					}
				}
			}
		}
		//////////////////////////////////////////////////////////////////////////
		if (nTriggerSource != 0)
		{
			int nTriggerActivation = GetPrivateProfileInt("camera", "triggeract",9999,m_sInitFile);
			if (nTriggerActivation == 9999)
			{
				WritePrivateProfileString("camera", "triggeract", "2", m_sInitFile);
				nTriggerActivation = 2;
			}

			BGAPIX_TypeListINT TriggerActivation;
			TriggerActivation.cbSize = sizeof ( BGAPIX_TypeListINT );
			res = m_pCamera->getTriggerActivation( &state, &TriggerActivation );
			if( res != BGAPI_RESULT_OK )
			{
				CString str;
				str.Format("相机序号:%d,BGAPI::Camera::getTriggerActivation Errorcode: %d\n",m_nGrabberSN,res);
				m_LastErrorInfo.iErrorCode = res;
				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
				return res;
			}

			if (BGAPI_Activation(TriggerActivation.array[TriggerActivation.current]) != BGAPI_Activation(nTriggerActivation))
			{

				for ( int i = 0; i<TriggerActivation.length; i++ )
				{
					if (nTriggerActivation == TriggerActivation.array[i])
					{
						m_bSet3 = TRUE;
						res = m_pCamera->setTriggerActivation(BGAPI_Activation(TriggerActivation.array[i]));
						if( res != BGAPI_RESULT_OK )
						{
							CString str;
							str.Format("相机序号:%d,BGAPI::Camera::setTriggerActivation Errorcode: %d\n",m_nGrabberSN,res);
							m_LastErrorInfo.iErrorCode = res;
							_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
							_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
							return res;
						}
						break;
					}
					else
					{
						if (i == TriggerActivation.length-1)
						{
							CString str;
							str.Format("相机序号:%d,非法触发波形参数!",m_nGrabberSN);
							m_LastErrorInfo.iErrorCode = ErrorSetParam;
							_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
							_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
							return res;
						}
					}
				}	
			}
		}
	}
	else
	{
		CString str;
		str.Format("相机序号:%d,非法触发参数!",m_nGrabberSN);
		m_LastErrorInfo.iErrorCode = ErrorSetParam;
		_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
		_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
		return res;
	}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//  [4/25/2014 SJC]
// 	if(m_sDeviceName != "VLG-02M")//1
// 	{
// 		if (m_bSet3 == TRUE)
// 		{
// 			res = m_pCamera->doUserSetStore(BGAPI_USERSET_1);
// 			if( res != BGAPI_RESULT_OK )
// 			{
// 				CString str;
// 				str.Format("相机序号:%d,doUserSetStore returned with errorcode %d\n", m_nGrabberSN,res);
// 				m_LastErrorInfo.iErrorCode = res;
// 				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
// 				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
// 				return res;
// 			}
// 		}
// 		
// 		res = m_pCamera->getUserSetDefault( &state, &tDefaultUserSet);
// 		if( res != BGAPI_RESULT_OK )
// 		{
// 			CString str;
// 			str.Format("相机序号:%d,getUserSetDefault returned with errorcode %d\n",m_nGrabberSN, res);
// 			m_LastErrorInfo.iErrorCode = res;
// 			_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
// 			_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
// 			return res;
// 		}
// 		
// 		if((BGAPI_UserSet)tDefaultUserSet.array[tDefaultUserSet.current] != BGAPI_USERSET_1)
// 		{
// 			res = m_pCamera->setUserSet( BGAPI_USERSET_1 );
// 			if( res != BGAPI_RESULT_OK )
// 			{
// 				CString str;
// 				str.Format("相机序号:%d,BGAPI::Camera::setUserSet Errorcode: %d\n",m_nGrabberSN,res);
// 				m_LastErrorInfo.iErrorCode = res;
// 				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
// 				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
// 				return res;
// 			}
// 			
// 			res = m_pCamera->setUserSetAsDefault( BGAPI_USERSET_1 );
// 			if( res != BGAPI_RESULT_OK )
// 			{
// 				CString str;
// 				str.Format("相机序号:%d,BGAPI::Camera::setUserSetAsDefault Errorcode: %d\n",m_nGrabberSN,res);
// 				m_LastErrorInfo.iErrorCode = res;
// 				_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
// 				_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::init_camera()函数");
// 				return res;
// 			}						
// 		}
// 	}
	int iFlipMode =0;
	GetPrivateProfileInt("camera","flipmode",iFlipMode,m_sInitFile);
	res = SetImageFlipMode(iFlipMode);
	if (res != BGAPI_RESULT_OK)
	{
		return res;
	}
//////////////////////////////////////////////////////////////////////////
	return BGAPI_RESULT_OK;
}


//得到触发模式
BOOL CBaumerCamDH::GetTriggerParam(int &ParamValOutput)
{
	BOOL bRet = FALSE;
	ParamValOutput = m_nTrigger;
	return TRUE;
}

//重启设备
BOOL CBaumerCamDH::Reset()
{
	BOOL bRet = FALSE;

	CString str;
	str.Format("相机序号:%d,不支持此功能!\n",m_nGrabberSN);
	m_LastErrorInfo.iErrorCode = 0;
	_tprintf_s(m_LastErrorInfo.strErrorDescription, str);
	_tprintf_s(m_LastErrorInfo.strErrorRemark, "CBaumerCamDH::Reset()函数");

	return bRet;
}

//得到系统信息
BOOL CBaumerCamDH::GetSystemInfo(CString &sOutputInfo)
{
	BOOL bRet = FALSE;
	return bRet;
}

//创建默认配置文件
BOOL CBaumerCamDH::CreateDefaultConfigurationFile(CString &sInitFile)
{
	BOOL bRet = FALSE;
	return bRet;
}

//保存配置文件
BOOL CBaumerCamDH::SaveToConfigurationFile(CString &sInitFile)
{
	BOOL bRet = FALSE;
	return bRet;
}



//void CBaumerCamDH::Fun1()  
//{  
//
//	//#define INTERNET_CONNECTION_MODEM           1  
//	//#define INTERNET_CONNECTION_LAN             2  
//	//#define INTERNET_CONNECTION_PROXY           4  
//	//#define INTERNET_CONNECTION_MODEM_BUSY      8  
//	//#define INTERNET_RAS_INSTALLED              0x10
//	//#define INTERNET_CONNECTION_OFFLINE         0x20
//	//#define INTERNET_CONNECTION_CONFIGURED      0x40
//
//	DWORD   flags;//上网方式   
//	BOOL   m_bOnline=TRUE;//是否在线    
//
//	m_bOnline=InternetGetConnectedState(&flags,0);     
//	if(m_bOnline)//在线     
//	{     
//		if ((flags & INTERNET_CONNECTION_MODEM) ==INTERNET_CONNECTION_MODEM)  
//		{  
//			AfxMessageBox("在线：拨号上网\n");
//		}  
//		else if ((flags & INTERNET_CONNECTION_LAN) ==INTERNET_CONNECTION_LAN)  
//		{  
//			AfxMessageBox("在线：通过局域网\n");
//		}  
//		else if ((flags & INTERNET_CONNECTION_PROXY) ==INTERNET_CONNECTION_PROXY)  
//		{  
//			AfxMessageBox("在线：代理\n");
//		}  
//		else if ((flags & INTERNET_CONNECTION_MODEM_BUSY) ==INTERNET_CONNECTION_MODEM_BUSY)  
//		{  
//			AfxMessageBox("MODEM被其他非INTERNET连接占用\n");
//		}  
//		else if ((flags & INTERNET_RAS_INSTALLED) ==INTERNET_RAS_INSTALLED)  
//		{  
//			AfxMessageBox("INTERNET_RAS_INSTALLED\n");
//		}  
//		else if ((flags & INTERNET_CONNECTION_OFFLINE) ==INTERNET_CONNECTION_OFFLINE)  
//		{  
//			AfxMessageBox("INTERNET_CONNECTION_OFFLINE\n");
//		} 
//		else if ((flags & INTERNET_CONNECTION_CONFIGURED) ==INTERNET_CONNECTION_CONFIGURED)  
//		{  
//			AfxMessageBox("INTERNET_CONNECTION_CONFIGURED\n");
//		} 
//		else
//		{
//			AfxMessageBox("fun1 no");
//		}
//
//
//
//	}  
//	else  
//		AfxMessageBox("不在线1\n");
//}  
//
//int CBaumerCamDH::Fun2()  
//{  
//
//	DWORD   flags;//上网方式   
//	BOOL   m_bOnline=TRUE;//是否在线    
//
//	m_bOnline=IsNetworkAlive(&flags);     
//	if(m_bOnline)//在线     
//	{     
//		if ((flags & NETWORK_ALIVE_LAN) ==NETWORK_ALIVE_LAN)  
//		{  
//			AfxMessageBox("在线：NETWORK_ALIVE_LAN\n");
//			return 1;
//		}  
//		else if ((flags & NETWORK_ALIVE_WAN) ==NETWORK_ALIVE_WAN)  
//		{  
//			AfxMessageBox("在线：NETWORK_ALIVE_WAN\n");
//			return 2;
//		}  
//		else if ((flags & NETWORK_ALIVE_AOL) ==NETWORK_ALIVE_AOL)  
//		{  
//			AfxMessageBox("在线：NETWORK_ALIVE_AOL\n");
//			return 3;
//		}  
//		else if ((flags & NETWORK_ALIVE_INTERNET) ==NETWORK_ALIVE_INTERNET)  
//		{  
//			AfxMessageBox("在线：NETWORK_ALIVE_INTERNET\n");
//			return 4;
//		}
//		else
//		{
//			AfxMessageBox("fun2 no");
//			return 5;
//		}
//
//
//	}  
//	else 
//	{
//		AfxMessageBox("不在线2\n");
//		return 0;
//	}
//
//}  
//



////////////////////////////////////////////////////////////////////////////
	//BOOL bNICSetUp = FALSE;
	//int nCircle = 60;
	/////* variables used for GetIfTable and GetIfEntry */
	//MIB_IFTABLE *pIfTable = NULL;
	//MIB_IFROW *pIfRow = NULL;
	//CString str;
	//DWORD dwSize = 0;
	//DWORD dwRetVal = 0;

	////// Allocate memory for our pointers.
	//pIfTable = (MIB_IFTABLE *) MALLOC(sizeof(MIB_IFTABLE));
	//if (pIfTable == NULL)
	//{
	//	AfxMessageBox("Error allocating memory needed to call GetIfTable");
	//	return BGAPI_RESULT_FAIL;
	//}

	//pIfRow = (MIB_IFROW *) MALLOC(sizeof(MIB_IFROW));
	//if(pIfRow == NULL)
	//{
	//	AfxMessageBox("Error allocating memory\n");
	//	if(pIfTable != NULL)
	//	{
	//		FREE(pIfTable);
	//		pIfTable = NULL;
	//	}
	//	return BGAPI_RESULT_FAIL;
	//}

	//while(!bNICSetUp && nCircle > 0)
	//{
	//	//////////////////////////////////////////////////////////////////////////
	//	// Before calling GetIfEntry, we call GetIfTable to make
	//	// sure there are entries to get and retrieve the interface index.					  
	//	// Make an initial call to GetIfTable to get the
	//	// necessary size into dwSize
	//	dwSize = sizeof(MIB_IFTABLE);
	//	if(GetIfTable(pIfTable, &dwSize, 0) == ERROR_INSUFFICIENT_BUFFER)
	//	{
	//		FREE(pIfTable);
	//		pIfTable = (MIB_IFTABLE *) MALLOC(dwSize);
	//		if(pIfTable == NULL)
	//		{
	//			AfxMessageBox("Error allocating memory\n");
	//			return BGAPI_RESULT_FAIL;
	//		}
	//	}

	//	// Make a second call to GetIfTable to get the actual  data we want.
	//	if((dwRetVal = GetIfTable(pIfTable, &dwSize, 0)) == NO_ERROR)
	//	{
	//		if(pIfTable->dwNumEntries > 0)
	//		{
	//			if(pIfRow == NULL)
	//			{
	//				pIfRow = (MIB_IFROW *) MALLOC(sizeof(MIB_IFROW));
	//			}

	//			str.Format("\tNum Entries: %ld\n\n", pIfTable->dwNumEntries);
	//			AfxMessageBox(str);
	//			for(int i = 0; i < pIfTable->dwNumEntries; i++)
	//			{
	//				pIfRow->dwIndex = pIfTable->table[i].dwIndex;
	//				if((dwRetVal = GetIfEntry(pIfRow)) == NO_ERROR)
	//				{
	//					//str.Format("\tIndex:\t %ld\n", pIfRow->dwIndex);
	//					//str.Format("\tInterfaceName[%d]:\t ", i);
	//					//if(pIfRow->wszName != NULL)
	//						//str.Format("%ws", pIfRow->wszName);
	//					//str.Format("\tDescription[%d]:\t ", i);
	//					//for(j = 0; j < pIfRow->dwDescrLen; j++)
	//						//str.Format("%c", pIfRow->bDescr[j]);
	//					//str.Format("\tIndex[%d]:\t\t %d\n", i, pIfRow->dwIndex);
	//					//str.Format("\tType[%d]:\t\t ", i);
	//					if (pIfRow->dwType == IF_TYPE_ETHERNET_CSMACD && pIfRow->dwOperStatus == IF_OPER_STATUS_OPERATIONAL)
	//					{
	//						bNICSetUp = TRUE;
	//						AfxMessageBox("OK");
	//						break;;
	//					}
	//					//switch(pIfRow->dwType)
	//					//{
	//					//case IF_TYPE_OTHER:
	//					//	str.Format("Other\n");
	//					//	AfxMessageBox(str);
	//					//	break;
	//					//case IF_TYPE_ETHERNET_CSMACD:
	//					//	str.Format("Ethernet\n");
	//					//	AfxMessageBox(str);
	//					//	break;
	//					//case IF_TYPE_ISO88025_TOKENRING:
	//					//	str.Format("Token Ring\n");
	//					//	AfxMessageBox(str);
	//					//	break;
	//					//case IF_TYPE_PPP:
	//					//	str.Format("PPP\n");
	//					//	AfxMessageBox(str);
	//					//	break;
	//					//case IF_TYPE_SOFTWARE_LOOPBACK:
	//					//	str.Format("Software Lookback\n");
	//					//	AfxMessageBox(str);
	//					//	break;
	//					//case IF_TYPE_ATM:
	//					//	str.Format("ATM\n");
	//					//	AfxMessageBox(str);
	//					//	break;
	//					//case IF_TYPE_IEEE80211:
	//					//	str.Format("IEEE 802.11 Wireless\n");
	//					//	AfxMessageBox(str);
	//					//	break;
	//					//case IF_TYPE_TUNNEL:
	//					//	str.Format("Tunnel type encapsulation\n");
	//					//	AfxMessageBox(str);
	//					//	break;
	//					//case IF_TYPE_IEEE1394:
	//					//	str.Format("IEEE 1394 Firewire\n");
	//					//	AfxMessageBox(str);
	//					//	break;
	//					//default:
	//					//	str.Format("Unknown type %ld\n", pIfRow->dwType);
	//					//	AfxMessageBox(str);
	//					//	break;
	//					//}
	//					//str.Format("\tMtu[%d]:\t\t %ld\n", i, pIfRow->dwMtu);
	//					//str.Format("\tSpeed[%d]:\t\t %ld\n", i, pIfRow->dwSpeed);
	//					//str.Format("\tPhysical Addr:\t\t ");
	//					//if(pIfRow->dwPhysAddrLen == 0)
	//					//	str.Format("\n");
	//					////                    for (j = 0; j < (int) pIfRow->dwPhysAddrLen; j++) {
	//					//for(j = 0; j < pIfRow->dwPhysAddrLen; j++)
	//					//{
	//					//	if(j == (pIfRow->dwPhysAddrLen - 1))
	//					//		str.Format("%.2X\n", (int) pIfRow->bPhysAddr[j]);
	//					//	else
	//					//		str.Format("%.2X-", (int) pIfRow->bPhysAddr[j]);
	//					//}
	//					//str.Format("\tAdmin Status[%d]:\t %ld\n", i,  pIfRow->dwAdminStatus);
	//					//str.Format("\tOper Status[%d]:\t ", i);

	//					/*					switch(pIfRow->dwOperStatus)
	//					{
	//					case IF_OPER_STATUS_NON_OPERATIONAL:
	//					AfxMessageBox("Non Operational\n");
	//					break;
	//					case IF_OPER_STATUS_UNREACHABLE:
	//					AfxMessageBox("Unreasonable\n");
	//					break;
	//					case IF_OPER_STATUS_DISCONNECTED:
	//					AfxMessageBox("Disconnected\n");
	//					break;
	//					case IF_OPER_STATUS_CONNECTING:
	//					AfxMessageBox("Connecting\n");
	//					break;
	//					case IF_OPER_STATUS_CONNECTED:
	//					AfxMessageBox("Connected\n");
	//					break;
	//					case IF_OPER_STATUS_OPERATIONAL:
	//					AfxMessageBox("Operational\n");
	//					break;
	//					default:
	//					AfxMessageBox("Unknown status %ld\n", pIfRow->dwAdminStatus);
	//					break;
	//					}
	//					str.Format("\n");*/
	//				}
	//			}

	//			Sleep(1000);
	//			nCircle--;

	//		}
	//		else
	//		{
	//			Sleep(1000);
	//			nCircle--;
	//		}

	//	}
	//	else
	//	{
	//		Sleep(1000);
	//		nCircle--;
	//	}
////////////////////////////////////////////////////////////////////////////
// end of all