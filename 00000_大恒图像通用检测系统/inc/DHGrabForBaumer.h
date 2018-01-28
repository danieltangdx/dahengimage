/************************************************************************/
/* Copyright (c) 2012, 北京大恒图像视觉有限公司视觉系统部
/* All rights reserved.
/*
/* 文件名称： DHGrabberForBaumer.h
/* 摘要： 导出采集卡类(Baumer千兆网相机)
/* 这个动态库是针对Baumer千兆网相机，
/* 目前已经测试相机型号：
/* DXG04h
/*调用过程：(具体调用过程参看例程)
/*
/* 当前版本： 3.0.0.1 beta1
/* 修改者： 孙国强
/* 修改内容摘要：完善头文件注释
/* 完成日期： 2012年6月11日
/************************************************************************/

// #if !defined(DHGrabber_h_)
// #define DHGrabber_h_
// 
// const int GBMaxFileNameLen = 255;
// const int GBMaxTextLen = 255;
// 
// typedef struct _GBSignalInfoStruct//回调信息结构
// {
// 	PVOID Context;								// 存放拥有回调函数的对象的this指针
// 	int iGrabberTypeSN;							// 采集卡类型编号
// 	int nGrabberSN;								// 采集卡序号
// 	int nErrorCode;								// 错误代码（0：正常）
// 	int nFrameCount;							// 已采集的帧记数
// 	char strDescription[GBMaxTextLen];			// 描述信息
// 	int iReserve1;								// 备用字段1
// 	int iReserve2;								// 备用字段2	
// }s_GBSIGNALINFO;//回调信息结构
// 
// //回调函数指针声明
// typedef void (WINAPI *PGBCALLBACK)(const s_GBSIGNALINFO* SignalInfo);
// 
// typedef struct _GBInitStruct//采集卡初始化结构
// {
// 	int iGrabberTypeSN;						// 采集卡类型编号
// 	int nGrabberSN;							// 采集卡序号
//     char strDeviceName[GBMaxTextLen];		// 设备名称
// 	char strDeviceMark[GBMaxTextLen];		// 设备标识  用于标识连接的相机与此标识一致
// 	char strGrabberFile[GBMaxFileNameLen];	// 用于初始化的文件
// 	PGBCALLBACK CallBackFunc;				// 回调函数指针
// 	PVOID Context;							// 存放拥有回调函数的对象的this指针	
// 	int iReserve1;							// 备用字段1
// 	int iReserve2;							// 备用字段2
// }s_GBINITSTRUCT;//采集卡初始化结构
// 
// typedef struct _GBErrorInfoStruct//错误信息结构
// {
//    	int nErrorCode;							//错误代码（0：正常）
// 	char strErrorDescription[GBMaxTextLen];	//错误描述
// 	char strErrorRemark[GBMaxTextLen];		//附加信息
// }s_GBERRORINFO;//错误信息结构
// 
// 
// //采集卡
// class CGrabber
// {
// //操作
// public:
// 	CGrabber()
// 	{
// 	}
// 	virtual ~CGrabber()
// 	{
// 	}
// 	
// 	//初始化
// 	virtual BOOL Init(const s_GBINITSTRUCT* pInitParam) = 0;
// 
// 	//关闭
// 	virtual BOOL Close() = 0;
// 
// 	//开始采集
// 	virtual BOOL StartGrab() = 0;
// 
// 	//停止采集
// 	virtual BOOL StopGrab() = 0;
// 
// 	//单帧采集
// 	virtual BOOL Snapshot() = 0;
// 	
// 	//调用参数对话框
// 	virtual void CallParamDialog() = 0;
// 
// 	//得到出错信息
// 	virtual void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo) = 0 ;
// 
// 	//设置整型参数
// 	virtual BOOL SetParamInt(int Param, int nInputVal) = 0;
// 	
// 	//得到整型参数
// 	virtual BOOL GetParamInt(int Param, int &nOutputVal) = 0;
// 
// 	//设置浮点型参数
// 	virtual BOOL SetParamfloat(int Param, float nInputVal) = 0;
// 	
// 	//得到浮点型参数
// 	virtual BOOL GetParamfloat(int Param, float &nOutputVal) = 0;
// 
// 	//设置触发模式
// 	virtual BOOL SetTriggerParam(int ParamValInput) = 0;
// 
// 	//得到触发模式
// 	virtual BOOL GetTriggerParam(int &ParamValOutput) = 0;
// 
// 	//设置AOI区域
// 	virtual BOOL SetAOI(RECT rtAOI) = 0;
// 
// 	//得到AOI区域
// 	virtual BOOL GetAOI(RECT &rtAOI) = 0;
// 
// 	//重启设备
// 	virtual BOOL Reset() = 0;
// 
// 	//得到系统信息
// 	virtual BOOL GetSystemInfo(CString &sOutputInfo) = 0;
// 
// 	//创建默认配置文件
// 	virtual BOOL CreateDefaultConfigurationFile(CString &sInitFile) = 0;
// 
// 	//保存配置文件
// 	virtual BOOL SaveToConfigurationFile(CString &sInitFile) = 0;
// 
// 	
// };
// 
// #endif // !define(DHGrabber_h_)

#if !defined(DHGrabberBAUMER_h_)
#define DHGrabberBAUMER_h_


#include "Grabber.h"
//////////////////////////////////////////////////////////////////////////
//Baumer专用参数
enum BaumerParamID
{
	FramesPerSecondsContinuous = 0,
	Exposure,
	ExposureMin,
	ExposureMax,
	Gain,
	GainMin,
	GainMax
};

//Baumer专用参数值
enum BaumerParamVal
{
	LineTriggerOn = 0,
	LineTriggerOff
};

//错误代码定义
enum ErrorStruct
{
	NormalState = 0,    // 正常状态
	ErrorNoFindCam,	    // 没有找到相机
	ErrorValOverFlow,	// 数据溢出
	ErrorGrabberSN,		// 相机序号超出能够找到的相机数  错误处理存在 
	ErrorGetCamInfo,	// 得到相机信息失败              错误处理存在 
	ErrorOpenCam,		// 打开相机失败                  错误处理存在
	ErrorInit,			// 初始化失败                    错误处理存在
	ErrorStartGrab,		// 开始采集失败                  错误处理存在
	ErrorStopGrab,		// 停止采集失败                  错误处理存在
	ErrorClose,			// 关闭失败                      错误处理存在
	ErrorSendBufAdd,	// 传出图像内存地址失败          错误处理存在
	ErrorGetParam,		// 非法获取参数                  错误处理存在
	ErrorSetParam,		// 非法设置参数                  错误处理存在
	ErrorParamDlg,	    // 调用参数设置对话框失败        错误处理存在
	ErrorReadIniFile,	// 读取配置文件失败              错误处理存在
	ErrorLostFrame,     // 采集到TrashBuffer丢帧         错误处理存在
	ErrorSysLostFrame,  // 系统内部报出的丢帧
	ErrorTriggerIgnored,// 通常因为外触发频率过快
	ErrorSaveInitFile	//保存初始化文件失败
};

//////////////////////////////////////////////////////////////////////////
//采集类
#define	_EXPORTING

#ifdef _EXPORTING
#define CLASS_DECLSPEC    __declspec(dllexport)  //程序编译时使用
#else
#define CLASS_DECLSPEC    __declspec(dllimport)	 //动态库应用时使用
#endif

class CLASS_DECLSPEC CDHGrabForBaumer : public CGrabber
{
	//操作
public:
	CDHGrabForBaumer();
	virtual ~CDHGrabForBaumer();

	//初始化
	BOOL Init(const s_GBINITSTRUCT* pInitParam);
	virtual bool Init(const CGrabInitInfo& rGrabInitInfo) override;

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




	//设置参数
	BOOL SetParamInt(GBParamID Param, int nInputVal);

	//得到参数
	BOOL GetParamInt(GBParamID Param, int &nOutputVal);
	//////////////////////////////////////////////////////////////////////////

	// 	9:GBGain,					//相机增益大小
	//	15:GainMin,
	//	16:GainMax
	BOOL SetParamfloat(int Param, float nInputVal);

	BOOL GetParamfloat(int Param, float& nOutputVal);
	//////////////////////////////////////////////////////////////////////////

	//调用参数对话框
	void CallParamDialog();

	//得到出错信息
	void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo);


	//得到AOI区域(未用)
	BOOL GetAOI(RECT &rtAOI);

	//设置AOI区域(未用)
	BOOL SetAOI(RECT rtAOI);

	//设置触发模式(未用)
	BOOL SetTriggerParam(int ParamValInput);

	//得到触发模式(未用)
	BOOL GetTriggerParam(int &ParamValOutput);

	//重启设备(未用)
	BOOL Reset();

	//得到系统信息(未用)
	BOOL GetSystemInfo(CString &sOutputInfo);

	//创建默认配置文件(未用)
	BOOL CreateDefaultConfigurationFile(CString &sInitFile);

	//保存配置文件(未用)
	BOOL SaveToConfigurationFile(CString &sInitFile);



	//属性
protected:
	CGrabber *m_pGrabber;
};

#endif// !defined(DHGrabberBAUMER_h_)


//////////////////////////////////////////////////////////////////////////
//使用说明
/*

//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//修改记录
/*

*/
//////////////////////////////////////////////////////////////////////////

