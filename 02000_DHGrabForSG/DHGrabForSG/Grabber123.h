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

#if !defined(Grabber_h_)
#define Grabber_h_

const int GBMaxFileNameLen = 255;
const int GBMaxTextLen = 255;

typedef struct _GBSignalInfoStruct//回调信息结构
{
	PVOID Context;								//存放拥有回调函数的对象的this指针
	int iGrabberTypeSN;							// 采集卡类型编号	// [2009-12-18 by jzj]: add
	int nGrabberSN;								//采集卡序号
	int nErrorCode;								//错误代码（0：正常）
	int nFrameCount;							//已采集的帧记数
	TCHAR strDescription[GBMaxTextLen];			//描述信息

	int iReserve1;								// 备用字段1	// [2009-12-18 by jzj]: add
	int iReserve2;								// 备用字段2	// [2009-12-18 by jzj]: add
	
}s_GBSIGNALINFO;//回调信息结构

//回调函数指针声明
typedef void (WINAPI *PGBCALLBACK)(const s_GBSIGNALINFO* SignalInfo);


typedef struct _GBInitStruct//采集卡初始化结构
{
	int iGrabberTypeSN;						// 采集卡类型编号	// [2009-12-18 by jzj]: add
	int nGrabberSN;							//采集卡序号
    char strDeviceName[GBMaxFileNameLen];	//设备名称
	char strGrabberFile[GBMaxFileNameLen];	//用于初始化的文件
	PGBCALLBACK CallBackFunc;				//回调函数指针
	PVOID Context;							//存放拥有回调函数的对象的this指针
	
	int iReserve1;							// 备用字段1	// [2009-12-18 by jzj]: add
	int iReserve2;							// 备用字段2	// [2009-12-18 by jzj]: add

}s_GBINITSTRUCT;//采集卡初始化结构

typedef struct _GBErrorInfoStruct//错误信息结构
{
   	int nErrorCode;							//错误代码（0：正常）
	TCHAR strErrorDescription[GBMaxTextLen];	//错误描述
	TCHAR strErrorRemark[GBMaxTextLen];		//附加信息

}s_GBERRORINFO;//错误信息结构

//公共参数
enum GBParamID
{
	GBImageWidth = 0,		//图像宽度 (单位:字节)
	GBImageHeight,			//图像高度 (单位:字节)
	GBImagePixelSize,		//图像象素大小 (单位:字节)
	GBImageBufferSize,		//图像缓冲区大小 (单位:字节)
	GBImageBufferAddr,		//图像缓冲区地址
	GBGrabberTypeSN			// 采集卡类型编号	// [2009-12-18 by jzj]: add
};

//错误代码定义
const int GBOK = 0;

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
	
	//调用参数对话框
	virtual void CallParamDialog() = 0;
	
	//得到出错信息
	virtual void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo) = 0;
};

#endif // !define(Grabber_h_)