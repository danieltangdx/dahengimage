/************************************************************************/
/* Copyright (c) 2014, 北京大恒图像视觉有限公司  
/* All rights reserved.													
/*																		
/* 文件名称： DHGrabberJAI.h	
/*
/* 当前版本： 1.0.0.1Beta1
/* 修改者： 赵慧阳
/* 修改内容： 撰写新动态库
/* 完成日期： 2014-11-3
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
7:JAI千兆网相机*/


#if !defined(DHGrabber_h_)
#define DHGrabber_h_

const int GBMaxFileNameLen = 255;
const int GBMaxTextLen = 255;

typedef struct _GBSignalInfoStruct//回调信息结构
{
	PVOID Context;								// 存放拥有回调函数的对象的this指针
	int iGrabberTypeSN;							// 采集卡类型编号
	int nGrabberSN;								// 采集卡序号
	int nErrorCode;								// 错误代码（0：正常）
	int nFrameCount;							// 已采集的帧记数
	char strDescription[GBMaxTextLen];			// 描述信息
	int iReserve1;								// 备用字段1
	int iReserve2;								// 备用字段2	
}s_GBSIGNALINFO;//回调信息结构

//回调函数指针声明
typedef void (WINAPI *PGBCALLBACK)(const s_GBSIGNALINFO* SignalInfo);

typedef struct _GBInitStruct//采集卡初始化结构
{
	int iGrabberTypeSN;						// 采集卡类型编号
	int nGrabberSN;							// 采集卡序号
    char strDeviceName[GBMaxTextLen];		// 设备名称
	char strDeviceMark[GBMaxTextLen];		// 设备标识  用于标识连接的相机与此标识一致
	char strGrabberFile[GBMaxFileNameLen];	// 用于初始化的文件
	PGBCALLBACK CallBackFunc;				// 回调函数指针
	PVOID Context;							// 存放拥有回调函数的对象的this指针	
	int iReserve1;							// 备用字段1
	int iReserve2;							// 备用字段2
}s_GBINITSTRUCT;//采集卡初始化结构

typedef struct _GBErrorInfoStruct//错误信息结构
{
   	int nErrorCode;							//错误代码（0：正常）
	char strErrorDescription[GBMaxTextLen];	//错误描述
	char strErrorRemark[GBMaxTextLen];		//附加信息
}s_GBERRORINFO;//错误信息结构


//采集卡
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
	
	//调用参数对话框
	virtual void CallParamDialog() = 0;

	//得到出错信息
	virtual void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo) = 0 ;

	//设置整型参数
	virtual BOOL SetParamInt(int Param, int nInputVal) = 0;
	
	//得到整型参数
	virtual BOOL GetParamInt(int Param, int &nOutputVal) = 0;

	//设置浮点型参数
	virtual BOOL SetParamfloat(int Param, float nInputVal) = 0;
	
	//得到浮点型参数
	virtual BOOL GetParamfloat(int Param, float &nOutputVal) = 0;

	//设置触发模式
	virtual BOOL SetTriggerParam(int ParamValInput) = 0;

	//得到触发模式
	virtual BOOL GetTriggerParam(int &ParamValOutput) = 0;

	//设置AOI区域
	virtual BOOL SetAOI(RECT rtAOI) = 0;

	//得到AOI区域
	virtual BOOL GetAOI(RECT &rtAOI) = 0;

	//重启设备
	virtual BOOL Reset() = 0;

	//得到系统信息
	virtual BOOL GetSystemInfo(CString &sOutputInfo) = 0;

	//创建默认配置文件
	virtual BOOL CreateDefaultConfigurationFile(CString &sInitFile) = 0;

	//保存配置文件
	virtual BOOL SaveToConfigurationFile(CString &sInitFile) = 0;

	
};

#endif // !define(DHGrabber_h_)



// DHGrabberJAI.h: interface for the CDHGrabberJAI class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DHGRABBERJAI_H__F392C2D2_F53F_48F1_ACCA_128E9E145E26__INCLUDED_)
#define AFX_DHGRABBERJAI_H__F392C2D2_F53F_48F1_ACCA_128E9E145E26__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



//////////////////////////////////////////////////////////////////////////
//采集类
#define	_EXPORTING
#ifdef _EXPORTING
#define CLASS_DECLSPEC    __declspec(dllexport)  //程序编译时使用
#else
#define CLASS_DECLSPEC    __declspec(dllimport)	 //动态库应用时使用
#endif

class CLASS_DECLSPEC CDHGrabberJAI:CGrabber
{
public:
	CDHGrabberJAI();
	virtual ~CDHGrabberJAI();

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
	BOOL SetParamInt(int Param, int nInputVal);
	
	//得到参数
	BOOL GetParamInt(int Param, int &nOutputVal);
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

	//////////////////////////////////////////////////////////////////////////
//属性
protected:
	CGrabber *m_pGrabber;

};

#endif // !defined(AFX_DHGRABBERJAI_H__F392C2D2_F53F_48F1_ACCA_128E9E145E26__INCLUDED_)
