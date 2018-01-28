/************************************************************************/
/* Copyright (c) 2007, 中国大恒集团北京图像视觉技术分公司视觉系统部     
/* All rights reserved.													
/*																		
/* 文件名称： Grabber.h												
/* 文件标识： 见配置管理计划书
/* 摘要： 采集卡（抽象基类）
/*
/* 当前版本： 3.0
/* 修改者： 江卓佳
/* 修改内容摘要：初始化结构体及采集回调结构体加入“采集卡类型编号”。// [2009-12-18 by jzj]: add
/* 完成日期： 2009年12月18日
/*
/* 以前版本： 1.1
/* 修改者： 江卓佳
/* 修改内容摘要：加入单帧采集接口Snapshot()。
/* 完成日期： 2007年11月05日
/*
/* 历史版本： 1.0
/* 作者： 江卓佳
/* 完成日期： 2006年08月01日
/************************************************************************/

/*
采集卡类型编号定义如下
0:模拟采集卡
1:DALSA X64采集卡
2:AVT相机
3:千兆网相机
4:公司CG300模拟采集卡
5:公司1394相机
6:EURESYS 采集卡
7:JAI千兆网相机
8::公司水星系列相机*/
#pragma once

enum e_GrabType {
	e_GrabType_SG = 0,
	e_GrabType_AVT = 2,
	e_GrabType_BAUMER = 3,
	e_GrabType_JAI = 7,
	e_GrabType_MER = 8,
	e_GrabType_NONE = 666
};

const int GBMaxFileNameLen{256};
const int GBMaxTextLen{256};


class s_GBSIGNALINFO//回调信息结构
{
public:
	PVOID Context;								//存放拥有回调函数的对象的this指针
	int iGrabberTypeSN;							//采集卡类型编号
	int iGrabberSN;								//采集卡序号
	int iErrorCode;								//错误代码（0：正常）
	int iFrameCount;							//已采集的帧记数
	int &nErrorCode = iErrorCode;
	int &nFrameCount = iFrameCount;
	int &nGrabberSN = iGrabberSN;
	int &nGrabberTypeSN = iGrabberTypeSN;
	unsigned __int64 nTimestamp;				//时间戳
	TCHAR strDescription[MAX_PATH];				//描述信息
	//(TCHAR &)[MAX_PATH] strDescription = strDescription;

	int iReserve1;								// 备用字段1
	int iReserve2;								// 备用字段2
	s_GBSIGNALINFO()
	{
		Context = NULL;
		iGrabberTypeSN = 0;
		iGrabberSN = 0;
		iErrorCode = 0;
		iFrameCount = 0;
		memset(strDescription, 0, sizeof(strDescription));
	}

};//回调信息结构


// typedef struct _GBEventInfoStruct//回调信息结构
// {
// 	PVOID Context;								//存放拥有回调函数的对象的this指针
// 	int iGrabberTypeSN;							//采集卡类型编号
// 	int iGrabberSN;								//采集卡序号
// 	int iErrorCode;								//错误代码（0：正常）
// 	int iFrameCount;							//已采集的帧记数
// 	__int64 nTimestamp;							//时间戳
// 
// 	TCHAR strDescription[GBMaxTextLen];			//描述信息
// 
// 	int iReserve1;								// 备用字段1
// 	int iReserve2;								// 备用字段2
// 
// }s_GBEVENTINFO;//回调信息结构

//回调函数指针声明
typedef void (WINAPI *PGBCALLBACK)(const s_GBSIGNALINFO* SignalInfo);

// 触发不响应事件的回调函数
// typedef void (WINAPI *PGBEVENTCALLBACK)(const s_GBEVENTINFO* EventInfo);

class s_GBINITSTRUCT//采集卡初始化结构
{
public:
	int iGrabberTypeSN;						// 采集卡类型编号
	int iGrabberSN;							//采集卡序号
	int &nGrabberTypeSN = iGrabberTypeSN;
	int &nGrabberSN = iGrabberSN;
	TCHAR strDeviceName[MAX_PATH];		//设备名称
	TCHAR strDeviceMark[MAX_PATH];		//设备标识
	TCHAR strGrabberFile[MAX_PATH];	//用于初始化的文件
	PGBCALLBACK CallBackFunc;				//回调函数指针
	PGBCALLBACK EventCallbackFunc;		//触发不响应事件的回调函数

	PVOID Context;							//存放拥有回调函数的对象的this指针
	int iReserve1;							// 备用字段1
	int iReserve2;							// 备用字段2

	s_GBINITSTRUCT()
	{
		Context = NULL;
		iGrabberTypeSN = 0;
		iGrabberSN = 0;
		iReserve1 = 0;
		iReserve2 = 0;
		CallBackFunc = NULL;
		EventCallbackFunc = NULL;
		memset(strDeviceName, 0, sizeof(strDeviceName));
		memset(strDeviceMark, 0, sizeof(strDeviceMark));
		memset(strGrabberFile, 0, sizeof(strGrabberFile));
	}

};//采集卡初始化结构

class s_GBERRORINFO//错误信息结构
{
public:
	int iErrorCode;							//错误代码（0：正常）
	int &nErrorCode = iErrorCode;
	TCHAR strErrorDescription[MAX_PATH];	//错误描述
	TCHAR strErrorRemark[MAX_PATH];		//附加信息
	s_GBERRORINFO()
	{
		iErrorCode = 0;
		memset(strErrorRemark, 0, sizeof(strErrorRemark));
		memset(strErrorDescription, 0, sizeof(strErrorDescription));
	}

};//错误信息结构


			   //公共参数
enum GBParamID
{
	GBImageWidth = 0,		//图像宽度 (单位:字节)
	GBImageHeight=1,			//图像高度 (单位:字节)
	GBImagePixelSize=2,		//图像象素大小 (单位:字节)
	GBImageBufferSize=3,		//图像缓冲区大小 (单位:字节)
	GBImageBufferAddr=4,		//图像缓冲区地址
	GBGrabberTypeSN=5,			//采集卡类型编号

	//**[12/28/2017 ConanSteve]**:  可能不支持，
	GBExposureTime=6, //**[9/11/2017 ConanSteve]**:  曝光时间
	GBGain=7,//**[9/11/2017 ConanSteve]**:  增益
	GBIsColorFilter=8,//**[11/16/2017 ConanSteve]**:  是否彩图
	GBBayerType=9,//**[11/16/2017 ConanSteve]**:  Bayer变换类型
	GBEnableFlipImage=10,//**[11/16/2017 ConanSteve]**:  相机原图是否需要翻转
	GBShutter=11,//**[11/16/2017 ConanSteve]**:  相机快门值
	GBSequenceNum=12,//**[11/16/2017 ConanSteve]**:  序列图张数
	GBExposureMin=13,//**[11/16/2017 ConanSteve]**:  最小曝光值
	GBExposureMax=14//**[11/16/2017 ConanSteve]**:  最大曝光值

};
//**[9/7/2017 ConanSteve]**:  相机通用信息
class CGrabInfo
{
public:
	int m_iImageWidth;//**[9/7/2017 ConanSteve]**:  相机当前图像宽
	int m_iImageHeight;//**[9/7/2017 ConanSteve]**:  相机当前图像高
	int m_iImageSize;//**[1/2/2018 ConanSteve]**:  相机原图尺寸大小
	int m_iOffsetX;
	int m_iOffsetY;
	int m_iImagePixelSize;//**[9/7/2017 ConanSteve]**:  相机采集原图每个图素的字节数
	int m_iExposureTime;//**[9/11/2017 ConanSteve]**:  曝光时间
	int m_iGain;//**[9/11/2017 ConanSteve]**:  增益
	int m_bIsColorFilter;//**[9/7/2017 ConanSteve]**:  相机是否是彩色相机
	int  m_iBayerType;//**[9/11/2017 ConanSteve]**:  Bayer转换类型，值与DX_PIXEL_COLOR_FILTER类型一致
	int	 m_iFlipType;//**[9/7/2017 ConanSteve]**:  翻转类型
	CGrabInfo()
	{
		m_iImageWidth = 0;
		m_iImageHeight = 0;
		m_iOffsetX = 0;
		m_iOffsetY = 0;
		m_iImagePixelSize = 0;
		m_iExposureTime = 1000;
		m_iGain = 0;
		m_bIsColorFilter = FALSE;
		m_iBayerType = 0;
		m_iFlipType = 0;
	}
};

//错误代码定义
const int GBOK = 0;


//**[8/29/2017 ConanSteve]**:  初始化相机类
class CGrabInitInfo {
public:
	int iGrabSN{ -1 };
	TCHAR lpszPathFileOfGrab[GBMaxFileNameLen];	//用于初始化的文件
	PGBCALLBACK CallBackFunc;				//回调函数指针
	PGBCALLBACK EventCallbackFunc;	//将触发不响应的回调函数指针传递
	PVOID Context;							//存放拥有回调函数的对象的this指针
public:
	CGrabInitInfo()
	{
		iGrabSN = -1;
		memset(lpszPathFileOfGrab, 0, sizeof(lpszPathFileOfGrab));
		CallBackFunc = EventCallbackFunc = NULL;
		Context = NULL;
	}
};


class CGrabber
{
//操作
public:
	CGrabber()
	{
	}
	virtual ~CGrabber()
	{
	}
	
	//初始化
	virtual BOOL Init(const s_GBINITSTRUCT* pInitParam) = 0;
	virtual bool Init(const CGrabInitInfo& rGrabInitInfo) {return false;}

	//关闭
	virtual BOOL Close() = 0;

	//开始采集
	virtual BOOL StartGrab() = 0;

	//停止采集
	virtual BOOL StopGrab() = 0;

	//单帧采集
	virtual BOOL Snapshot() = 0;
	
	//设置参数
	virtual BOOL SetParamInt(GBParamID Param, int nInputVal) = 0;
	
	//得到参数
	virtual BOOL GetParamInt(GBParamID Param, int &nOutputVal) = 0;

	virtual bool SetParamBool(GBParamID Param, bool bInputVal) {return false;};
	virtual bool GetParamBool(GBParamID Param, bool bRetVal) { return false; };
	
	//调用参数对话框
	virtual void CallParamDialog() = 0;
	
	//得到出错信息
	virtual void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo) = 0;



	//设置浮点型参数
	virtual BOOL SetParamfloat(int Param, float nInputVal) {return false;};

	//得到浮点型参数
	virtual BOOL GetParamfloat(int Param, float &nOutputVal) { return false; };

	//设置触发模式
	virtual BOOL SetTriggerParam(int ParamValInput) { return false; };

	//得到触发模式
	virtual BOOL GetTriggerParam(int &ParamValOutput) { return false; };

	//设置AOI区域
	virtual BOOL SetAOI(RECT rtAOI) { return false; };

	//得到AOI区域
	virtual BOOL GetAOI(RECT &rtAOI) { return false; };

	//重启设备
	virtual BOOL Reset() { return false; };

	//得到系统信息
	virtual BOOL GetSystemInfo(CString &sOutputInfo) { return false; };

	//创建默认配置文件
	virtual BOOL CreateDefaultConfigurationFile(CString &sInitFile) { return false; };

	//保存配置文件
	virtual BOOL SaveToConfigurationFile(CString &sInitFile) { return false; };
};
