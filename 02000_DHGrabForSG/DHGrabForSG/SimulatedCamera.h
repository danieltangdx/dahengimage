/************************************************************************/
/* Copyright (c) 2007, 中国大恒集团北京图像视觉技术分公司视觉系统部     
/* All rights reserved.													
/*																		
/* 文件名称： SimulateGrabber.h	
/* 摘要： 图像采集卡模拟器, 使用配置文件及已存在的图像文件实现模拟定时采图
/*
/* 当前版本： 3.0
/* 作者： 江卓佳
/* 完成日期： 2009年12月18日	// [2009-12-18 by jzj]: add
/*
/* 当前版本： 2.0
/* 作者： 江卓佳
/* 完成日期： 2007年11月13日
/************************************************************************/

#pragma once

#include "Grabber.h"

//////////////////////////////////////////////////////////////////////////
// 头文件

#include <afxmt.h>          //为了使用CEvent
//
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 声明
/************************************************************************/
/*                       [5/22/2017 ConanSteve]                         */
const bool g_bRecord = false;
/************************************************************************/

const int SGMaxImageNum = 20;		//预读图像模式时最大图像张数//[2006-08-23]

#include "ExFn.h"
typedef struct _SgSignalInfoStruct//回调信息结构
{
	PVOID Context;	//存放拥有回调函数的对象的this指针
	BOOL bBuffProcessing;	//缓冲区正在处理		
	int iFrameCount;	//已采集的帧记数
	CString strDescription; //描述信息

}s_SGSIGNALINFO;		//回调信息结构

//回调函数指针声明
typedef void (WINAPI *PSGCALLBACK)(const s_SGSIGNALINFO* SignalInfo);

typedef struct _SgInitCardStruct//图像卡参数初始化结构
{
	int CardSN;								//图像卡序号
	TCHAR sInitFile[GBMaxFileNameLen];		//用于初始化的文件
	PGBCALLBACK CallBackFunc;				//回调函数指针//new
	PVOID Context;							//存放调用初始化函数的对象的this指针

}s_SGINITCARDSTRUCT;		//图像卡参数初始化结构

typedef int SGPARAMID;	//参数ID
//typedef signed int* PINT32;// [2009-12-17 by jzj]: delete

// PARAM ID					// [2009-12-17 by jzj]: modify
//#define SG_IMAGE_WIDTH							0	//图像宽度 (单位:字节)
//#define SG_IMAGE_HEIGHT							1	//图像高度 (单位:字节)
//#define SG_IMAGE_BYTE_COUNT						2	//图像象素大小 (单位:字节)
//#define SG_IMAGE_BUFFER_SIZE					3	//图像缓冲区大小 (单位:字节)
//#define SG_IMAGE_BUFFER_ADDR					4	//图像缓冲区地址
//#define SG_IS_PLANERGB							5	//是否分通道（0：否；1：是）
//#define SG_GRAB_SPEED							6   //采集速度（单位：毫秒/张）					
#define SG_IS_PLANERGB							0	//是否分通道（0：否；1：是）
#define SG_GRAB_SPEED							1   //采集速度（单位：毫秒/张）
#define SG_IMAGE_WIDTH							2	//图像宽度 (单位:字节)
#define SG_IMAGE_HEIGHT							3	//图像高度 (单位:字节)
#define SG_IMAGE_BYTE_COUNT						4	//图像象素大小 (单位:字节)
#define SG_IMAGE_BUFFER_SIZE					5	//图像缓冲区大小 (单位:字节)
#define SG_IMAGE_BUFFER_ADDR					6	//图像缓冲区地址
	
//错误代码定义
const int SGErrorCodeInitFileNoExist = 1000;		//初始化文件不存在
const int SGErrorCodeParamIll = 1001;				//参数不合法
const int SGErrorCodeInitFileParamIll = 1002;		//初始化文件参数不合法
const int SGErrorCodeNoInit = 1003;					//对象未初始化
const int SGErrorCodeThisFuncDisable = 1004;		//此功能无效
const int SGErrorCodeReadBMPFileFail = 1200;		//读BMP文件失败
const int SGErrorCodeReadImageFromMemFail = 1201;	//从内存中读取图像失败
//
//////////////////////////////////////////////////////////////////////////

///模拟采集卡
class CSimulatedCamera : public CGrabber
{
//属性
protected:
	//////////////////////////////////////////////////////////////////////////
	// 内部变量

	int m_nCardSN;							//图像卡序号
	TCHAR m_sInitFile[GBMaxFileNameLen];		//用于初始化的格式文件
	PGBCALLBACK m_CallBackFunc;				//回调函数指针
	PVOID m_Context;						//存放调用初始化函数的对象的this指针
	s_SGSIGNALINFO m_SignalInfo;			//回调信息
	
	BYTE* m_pImageBuffer;					//供外面处理用的图像缓冲区
	int m_nImageWidth;						//图像宽度 (单位:字节)
	int m_nImageHeight;						//图像高度 (单位:字节)
	int m_nImageByteCount;					//图像象素大小 (单位:字节)
	int m_nImageBuffSize;					//图像缓冲区大小 (单位:字节)
	BOOL m_bPlaneRGB;						//是否分通道
	BYTE* m_pPlaneRGBBuffer;				//供外面处理用的分通道图像缓冲区

	BOOL m_bInited;							//图像卡是否已经初始化

	BOOL m_bGrab;							//是否采集图像
	BOOL m_bGrabbing;						//是否正在采集图像
	int m_nGrabSpeed;						//采集速度(单位: 毫秒/张)
	CString m_strImageFileFolder;			//图像文件所在文件夹
	
	//位图信息指针
	BITMAPINFO *m_pBmpInfo;
	
	DWORD   m_dwThreadId;						//采集服务线程ID
	static DWORD ServiceThread(LPVOID lpParam);	//采集服务线程
	DWORD ServiceThreadFunc();					//采集服务线程函数
	DWORD PreReadImageServiceThreadFunc();		//预读图像的采集服务线程函数
	BOOL m_bKillServiceThread;
	CEvent m_ServiceThreadDeadEvent;

	CFileFind m_FileFinder;
	CFileFind m_BMPFileFinder;

	int m_nCapturedFieldCount;//采集帧记数
	
	//CEvent m_evtReset;// [2008-1-22 by jzj]

	CString m_strDeviceName;//设备名称

	s_GBERRORINFO m_LastErrorInfo;//出错信息

	BOOL m_bPreReadImage;//是否预先读图像到内存中
	int m_nImageNum;//图像张数
	BYTE* m_PreReadImageBuffArray[SGMaxImageNum];//预读图像缓冲区指针数组
	CString m_strImageFileNameArray[SGMaxImageNum];//预读图像文件名数组

	BOOL m_bImagesOK;//图像是否已准备好
	BOOL m_bUpdateImagesFolder;//更新图像所在文件夹
	
	int m_nMaxImageWidth;
	int m_nMaxImageHeight;
	int m_nMaxImageByteCount;

	BOOL m_bLoopGrab;// 是否循环采集// [2008-10-23 by jzj]

	BOOL m_bResetPosition;// 是否重置预读位置// [2008-12-4 by jzj]: add

	int m_iGrabberTypeSN;	// 采集卡类型编号// [2009-12-18 by jzj]: add

	int m_iPicNumOfEachGrab;//**[12/9/2017 ConanSteve]**:  每次采集的图像张数，1时所有图像在同一文件夹下，大于1时，相同图像号的文件都在同一文件夹下
	CINICfg m_iniCfg;
	int m_iTriggerPort{ 0 };//**[12/9/2017 ConanSteve]**:  相机受控触发的Out口编号
	//
	//////////////////////////////////////////////////////////////////////////
	
//操作
public:
	CSimulatedCamera();
	virtual ~CSimulatedCamera();
	
	//初始化图像卡
	virtual BOOL Init(const s_GBINITSTRUCT* GrabInit);
	virtual bool Init(const CGrabInitInfo& rGrabInitInfo);

	//关闭图像卡
	virtual BOOL Close();

	//开始采集
	virtual BOOL StartGrab();

	//停止采集
	virtual BOOL StopGrab();

	//单帧采集
	virtual BOOL Snapshot();

	//得到参数
	virtual BOOL GetParamInt(GBParamID Param, int &nOutputVal);

	//设置参数
	virtual BOOL SetParamInt(GBParamID Param, int nInputVal);
	
	//调用参数对话框
	virtual void CallParamDialog();

	//得到出错信息
	virtual void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo);

	//////////////////////////////////////////////////////////////////////////
	//模拟采集卡专用接口

	BOOL InitCard(const s_SGINITCARDSTRUCT* pInitCardParam);
	BOOL CloseCard();
	//BOOL GetParamInt(SGPARAMID Param, PINT32 nOutputVal);
	BOOL GetParamInt(SGPARAMID Param, int& nOutputVal);// [2009-12-17 by jzj]: modify
	BOOL SetParamInt(SGPARAMID Param, int nInputVal);
	//
	//////////////////////////////////////////////////////////////////////////
	
	friend class CSGParamDlg;
	/************************************************************************/
	/*                       [5/22/2017 ConanSteve]                         */
	CStdioFile m_fileRecordSingleTime;
	CStdioFile m_fileRecordIntervalTime;
	double m_dFrequency{ 0 };
	DWORD m_dwLastStartCnt{ 0 };
	
	/************************************************************************/
};
