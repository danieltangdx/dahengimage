// JAICamDH.h: interface for the CJAICamDH class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JAICAMDH_H__423502D3_7E46_4956_A554_CD59E4ADFAC3__INCLUDED_)
#define AFX_JAICAMDH_H__423502D3_7E46_4956_A554_CD59E4ADFAC3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DHGrabForJAI.h"
// #include <Jai_Error.h>
// #include <Jai_Types.h>
//#include <Jai_Factory_Dynamic.h>

#include <Jai_Factory.h>

const int MaxFileNameLen = 255;			// 初始化文件名的最大长度
const int iMaxCameraNum = 20;			//采集库支持的最大相机数

#define NODE_NAME_WIDTH         "Width"
#define NODE_NAME_HEIGHT        "Height"
#define NODE_NAME_PIXELFORMAT   "PixelFormat"
#define NODE_NAME_GAIN          "GainRaw"
#define NODE_NAME_EXPOSURE      "ExposureTimeRaw"
#define NODE_NAME_ACQSTART      "AcquisitionStart"
#define NODE_NAME_ACQSTOP       "AcquisitionStop"

typedef struct _JAIInitStruct			// 参数初始化结构
{
	int iGrabberTypeSN;						// 采集卡类型编号
	int nGrabberSN;							// 采集卡序号
    char strDeviceName[GBMaxTextLen];		// 设备名称
	char strDeviceMark[GBMaxTextLen];		// 设备标识
	char strGrabberFile[GBMaxFileNameLen];	// 用于初始化的文件
	PGBCALLBACK CallBackFunc;				// 回调函数指针
	PVOID Context;							// 存放拥有回调函数的对象的this指针	
}s_JAI_INITSTRUCT;// 参数初始化结构

enum ErrorStruct
{
	ErrorNoFindCam = 0,		// 没有找到相机
	ErrorGrabberSN,			// 相机序号超出能够找到的相机数
	ErrorGetCamInfo,		// 得到相机信息失败
	ErrorValOverFlow,		// 数据溢出
	ErrorOpenCam,			// 打开相机失败
	ErrorInit,				// 初始化失败
	ErrorStartGrab,			// 开始采集失败
	ErrorStopGrab,			// 停止采集失败
	ErrorClose,				// 关闭失败
	ErrorSendBufAdd,		// 传出图像内存地址失败
	ErrorGetParam,			// 非法获取参数
	ErrorSetParam,			// 非法设置参数
	ErrorParamDlg,			// 调用参数设置对话框失败
	ErrorReadIniFile		// 读取配置文件失败
};

void WINAPI FGCallBackFunc(void* Context,UINT32 wParam,void* lParam); //采集回调函数

class CJAICamDH: public CGrabber
{
public:
	CJAICamDH();
	virtual ~CJAICamDH();

	FACTORY_HANDLE  m_hFactory;             // Factory Handle
	int8_t m_sCameraID[J_CAMERA_ID_SIZE];	// CameraIDs
	CAM_HANDLE      m_hCam;					// Camera Handles
	THRD_HANDLE     m_hThread;				// Stream handles
	J_tIMAGE_INFO*	m_pImageInfo;			// 保存采集图像信息
    NODE_HANDLE     m_hGainNode;            // Handle to "GainRaw" node
    NODE_HANDLE     m_hExposureNode;        // Handle to "ExposureTimeRaw" node


	// 相机宽高位数
	int m_iImageWidth;			//宽
	int m_iImageHeight;			//高
	int m_iImageByteCount;		//位数
	int m_iImageBufferSize;		//内存大小
	BYTE* m_pImageBufferAdd;	//内存地址
	int	 m_iGain;
	int  m_iExposureTime;
	int	 m_iMinGain;
	int	 m_iMaxGain;
	int  m_iMinExp;
	int  m_iMaxExp;
	int  m_iTriggerMode;

	// 相机基本信息
	int m_iGrabberSN;			//相机序号
	int m_iGrabberTypeSN;		//动态库类型号
	CString m_sInitFile;		//初始化文件名称
	CString m_sDeviceName;		//设备名称
	CString m_sDeviceMark;		//设备标识
	PGBCALLBACK m_CallBackFunc;		// 回调函数指针
	VOID* m_pContext;			//存放调用初始化函数的this对象指针
	s_GBERRORINFO m_LastErrorInfo;	// 错误信息


	int m_iCamCount;			//相机个数
	int m_iFrameCount;			//采集个数
	BOOL m_bInitSucceed;		//是否初始化成功
	BOOL m_bCameraActive;		//相机是否正在采集
	
	// 回调函数
	void StreamCBFunc1(J_tIMAGE_INFO * pAqImageInfo);


	// 初始化相机
	BOOL InitCamera();

	// 初始化工厂函数
	BOOL InitFactory();


	//初始化
	BOOL Init(const s_GBINITSTRUCT* pInitParam);

	//关闭
	BOOL Close();

	//开始采集
	BOOL StartGrab();

	//停止采集
	BOOL StopGrab();

	//单贞采集
	BOOL Snapshot();


	//////////////////////////////////////////////////////////////////////////
	//	输入参数定义
	//  int Param:
	// 	0:GBImageWidth,				//图像宽度 (单位:字节)
	// 	1:GBImageHeight,			//图像高度 (单位:字节)
	// 	2:GBImagePixelSize,			//图像象素大小 (单位:字节)
	// 	3:GBImageBufferSize,		//图像缓冲区大小 (单位:字节)
	// 	4:GBImageBufferAddr,		//图像缓冲区地址
	// 	5:GBGrabberTypeSN,			//采集卡类型编号
	
	// 	6:GBColor,					//相机类型  黑白和彩色 
	// 	7:GBBrightness,				//相机亮度大小
	// 	8:GBShutter,				//相机快门值
	// 	9:GBGain,					//相机增益大小
	// 	10:GBU,						//相机U值
	// 	11:GBV,						//相机V值
	// 	12:GBTemperature			//相机温度
	// 	13:Multishot				//设定Multishot采集桢数
	//设置参数
	BOOL SetParamInt(GBParamID Param, int nInputVal);
	
	//得到参数
	BOOL GetParamInt(GBParamID Param, int &nOutputVal);
	//////////////////////////////////////////////////////////////////////////

	//设置参数(未用)
	BOOL SetParamfloat(int Param, float nInputVal);
	
	//得到参数(未用)
	BOOL GetParamfloat(int Param, float &nOutputVal);

	//调用参数对话框
	void CallParamDialog();

	//得到出错信息
	void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo);

	//////////////////////////////////////////////////////////////////////////
	//	输入参数定义
	// 	0:AVTTriggerOff = 0,	//外触发关闭//内部触发,连续采集
	// 	1:AVTTriggerOn,	    	//上升沿触发//外触发开启
	// 	2:AVTTriggerMode2,		//下降沿触发
	// 	3:AVTTriggerMode3,		//高电平触发
	// 	4:AVTTriggerMode4,		//低电平触发
	// 	5:AVTTriggerMode5,		//编程模式上升沿触发
	// 	6:AVTTriggerMode6,		//编程模式下降沿触发	

	BOOL GetTriggerParam(int &ParamValOutput);

	BOOL SetTriggerParam(int ParamValInput);
	//////////////////////////////////////////////////////////////////////////


	/************************************************************************/
	/*  功能：获得采集窗口区域
	/*  输出：RECT    采集窗口大小
	/*	返回：正确:TRUE   错误:FALSE
	/************************************************************************/
	BOOL GetAOI(RECT &rtAOI);

	/************************************************************************/
	/*  功能：设置采集窗口区域
	/*  输入：RECT    采集窗口大小
	/*	返回：正确:TRUE   错误:FALSE
	/************************************************************************/
	BOOL SetAOI(RECT rtAOI);

	/************************************************************************/
	/*  功能：获得相机的序列号
	/*  输出：nGaidHigh  高六位
	/*	输出：nGaidLow   低九位
	/*	返回：正确:TRUE   错误:FALSE
	/************************************************************************/
	BOOL GetSystemInfo(CString &sOutputInfo);

	/************************************************************************/
	/*  功能：设置相机软复位，本功能在相机出现异常无回调情况下使用，
	/*  基本动作包括：
	/*  第一步:相机内部加载默认设置;
	/*  第二步: bus resets will occur;
	/*	第三步:	The FPGA will be rebooted; 
	/*	第四步:加载INI配置文件设置;
	/*  输入：
	/*	返回：正确:TRUE   错误:FALSE
	/************************************************************************/
	BOOL Reset();

	/************************************************************************/
	/*  功能：生成默认INI配置文件
	/*  输入：sInitFile 正确格式：D:\\Run\\CameraParm.ini
	/*	返回：正确:TRUE   错误:FALSE
	/************************************************************************/
	BOOL CreateDefaultConfigurationFile(CString &sInitFile);


	//保存配置文件
	BOOL SaveToConfigurationFile(CString &sInitFile);

};

#endif // !defined(AFX_JAICAMDH_H__423502D3_7E46_4956_A554_CD59E4ADFAC3__INCLUDED_)
