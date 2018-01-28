/************************************************************************/
/* Copyright (c) 2007, 中国大恒集团北京图像视觉技术分公司视觉系统部  
/* All rights reserved.													
/*																		
/* 文件名称： DHGrabberForSG.h	
/* 摘要： 导出采集卡类（模拟采集卡）
/*
/* 当前版本： 3.0
/* 修改者： 江卓佳
/* 修改内容摘要：初始化结构体加入“采集卡类型编号”和“设备标识”，采集回调结构体加入“采集卡类型编号”。
/* 完成日期： 2009年12月24日
/*
/* 以前版本： 2.0
/* 修改者： 江卓佳
/* 修改内容摘要：各类型采集对象使用不同的类名（如CDHGrabberSG），以适应一个程序用多种采集类的用法。
				 使用示例：CGrabber* pGrabber = new CDHGrabberSG; pGrabber->StartGrab(); 。
/* 完成日期： 2009年12月14日
/* 
/* 以前版本： 1.1
/* 修改者： 江卓佳
/* 修改内容摘要：加入单帧采集接口Snapshot()。
/* 完成日期： 2007年11月05日
/*
/* 当前版本： 1.0
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
6:EURESYS 采集卡*/

#if !defined(DHGrabber_h_)
#define DHGrabber_h_

const int GBMaxFileNameLen = 255;
const int GBMaxTextLen = 255;

typedef struct _GBSignalInfoStruct//回调信息结构
{
	PVOID Context;								//存放拥有回调函数的对象的this指针
	int iGrabberTypeSN;							// 采集卡类型编号
	int nGrabberSN;								//采集卡序号
	int nErrorCode;								//错误代码（0：正常）
	int nFrameCount;							//已采集的帧记数
	char strDescription[GBMaxTextLen];			//描述信息

	int iReserve1;								// 备用字段1
	int iReserve2;								// 备用字段2
	
}s_GBSIGNALINFO;//回调信息结构

//回调函数指针声明
typedef void (WINAPI *PGBCALLBACK)(const s_GBSIGNALINFO* SignalInfo);

typedef struct _GBInitStruct//采集卡初始化结构
{
	int iGrabberTypeSN;						// 采集卡类型编号
	int nGrabberSN;							//采集卡序号
    char strDeviceName[GBMaxTextLen];		//设备名称
	char strDeviceMark[GBMaxTextLen];		// 设备标识
	char strGrabberFile[GBMaxFileNameLen];	//用于初始化的文件
	PGBCALLBACK CallBackFunc;				//回调函数指针
	PVOID Context;							//存放拥有回调函数的对象的this指针
	
	int iReserve1;							// 备用字段1
	int iReserve2;							// 备用字段2

}s_GBINITSTRUCT;//采集卡初始化结构

typedef struct _GBErrorInfoStruct//错误信息结构
{
   	int nErrorCode;							//错误代码（0：正常）
	char strErrorDescription[GBMaxTextLen];	//错误描述
	char strErrorRemark[GBMaxTextLen];		//附加信息

}s_GBERRORINFO;//错误信息结构

//公共参数
enum GBParamID
{
	GBImageWidth = 0,		//图像宽度 (单位:字节)
	GBImageHeight,			//图像高度 (单位:字节)
	GBImagePixelSize,		//图像象素大小 (单位:字节)
	GBImageBufferSize,		//图像缓冲区大小 (单位:字节)
	GBImageBufferAddr,		//图像缓冲区地址
	GBGrabberTypeSN			//采集卡类型编号
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

#endif// !defined(DHGrabber_h_)

#if !defined(DHGrabberSG_h_)
#define DHGrabberSG_h_

//////////////////////////////////////////////////////////////////////////
//模拟采集卡专有参数

//模拟采集卡参数
enum SGParamID
{
	SGIsPlaneRGB = 0,		//是否分通道 (0：否，1：是)
	SGGrabSpeed,			//采图速度 (单位:毫秒/张)
	SGImageWidth,			//图像宽度 (单位:像素)//[2006-12-01]
	SGImageHeight,			//图像高度 (单位:像素)//[2006-12-01]
	SGImageByteCount,		//图像每像素字节量 (单位:字节)//[2006-12-01]
};

//错误代码定义
const int SGErrorCodeInitFileNoExist = 1000;	//初始化文件不存在
const int SGErrorCodeParamIll = 1001;			//参数不合法
const int SGErrorCodeInitFileParamIll = 1002;	//初始化文件参数不合法
const int SGErrorCodeNoInit = 1003;				//对象未初始化
const int SGErrorCodeThisFuncDisable = 1004;	//此功能无效
const int SGErrorCodeReadBMPFileFail = 1200;	//读BMP文件失败
const int SGErrorCodeReadImageFromMemFail = 1201;	//从内存中读取图像失败//[2006-08-23]
//
//////////////////////////////////////////////////////////////////////////

//采集类
class _declspec(dllimport) CDHGrabberSG : public CGrabber
{
//操作
public:
	CDHGrabberSG();
	virtual ~CDHGrabberSG();
	
	//初始化
	BOOL Init(const s_GBINITSTRUCT* pInitParam);

	//关闭
	BOOL Close();

	//开始采集
	BOOL StartGrab();

	//停止采集
	BOOL StopGrab();

	//单帧采集
	BOOL Snapshot();
	
	//设置参数
	BOOL SetParamInt(GBParamID Param, int nInputVal);
	
	//得到参数
	BOOL GetParamInt(GBParamID Param, int &nOutputVal);

	//调用参数对话框
	void CallParamDialog();

	//得到出错信息
	void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo);

	//////////////////////////////////////////////////////////////////////////
	//模拟采集卡专有接口

	BOOL SGSetParamInt(SGParamID Param, int nInputVal);
	BOOL SGGetParamInt(SGParamID Param, int &nOutputVal);
	//
	//////////////////////////////////////////////////////////////////////////
	
//属性
protected:
	CGrabber *m_pGrabber;
};

#endif// !defined(DHGrabberSG_h_)