/************************************************************************/
/* Copyright (c) 2013, 中国大恒集团北京图像视觉技术分公司视觉系统部  
/* All rights reserved.													
/*	




/* 文件名称： DHGrabberForMER.h	
/* 摘要： 导出采集卡类（公司水星Gige接口的相机）
/* 当前版本：1.64/32.0.4
/* 使用SDK的版本为1.1.1501.8211
/* 修改者：杨亮
/* 主要修改内容：针对新的sdk，去除曝光模式和其他参数，产生64bit的dll，整理引用头文件,增加采集卡类型返回值
/* 完成日期：2015年5月28日



/* 文件名称： DHGrabberForMER.h	
/* 摘要： 导出采集卡类（公司水星usb相机）
/* 当前版本：1.0.0.2
/* 修改者：杨亮
/* 主要修改内容：修改相机属性对话框，增加部分函数
/* 完成日期：2013年12月2日

/* 当前版本：1.0.0.1
/* 修改者：阴同
/* 主要修改内容：实现基础的初始化、连续采集、停止采集、单帧采集和参数设置界面
/* 完成日期：2013年10月12日

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
7:公司USB相机
8::公司水星系列相机*/
#pragma once

#include "Grabber.h"

#define EXDLL

#ifdef EXDLL
#define CLASS_DECLSPEC    __declspec(dllexport)  //程序编译时使用
#else
#define CLASS_DECLSPEC    __declspec(dllimport)	 //动态库应用时使用
#endif
// 水星相机专有参数
enum MERParamID
{
	MERSnapMode,				// 图像采集模式
	MERGain,						// 增益
	MERExposure,				// 曝光时间
	MERBlacklevel,			    // 黑电平
	MERBalanceRatio,             // 白平衡
	MERWidth,						//宽度	//[2017-3-24 zhy]
	MERHeight,						//高度	//[2017-3-24 zhy]
	MERXOffset,						//X方向偏移量	//[2017-3-24 zhy]
	MERYOffset						//Y方向偏移量	//[2017-3-24 zhy]
};





//采集类
class CLASS_DECLSPEC CDHGrabForMER : public CGrabber
{
	//操作
public:
	CDHGrabForMER();
	virtual ~CDHGrabForMER();
	
	//初始化
	BOOL Init(const s_GBINITSTRUCT* pInitParam);
	virtual bool Init(const CGrabInitInfo& rGrabInitInfo) override;
	
	//关闭
	BOOL Close();
	
	//开始采集
	BOOL StartGrab();
	
	//停止采集
	BOOL StopGrab();
	
	// 单帧采集 
	BOOL Snapshot();
	
	//设置参数
	BOOL SetParamInt(GBParamID Param, int nInputVal);

	bool GetParamBool(GBParamID Param, bool bRetVal);

	bool SetParamBool(GBParamID Param, bool bInputVal);
	
	//得到参数
	BOOL GetParamInt(GBParamID Param, int &nOutputVal);
	
	//调用参数对话框
	void CallParamDialog();
	
	//得到出错信息
	void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo);
	
	//////////////////////////////////////////////////////////////////////////
	// 公司相机专有接口	
	BOOL MERSetParamInt(MERParamID Param, int nInputVal1,int nInputVal2 = 0,int nInputVal3 = 0,int nInputVal4 = 0,int nInputVal5 = 0,void *sInputVal6 = NULL);
	BOOL MERGetParamInt(MERParamID Param, int &nOutputVal1,int &nOutputVal2,int &nOutputVal3);	
	int  MERSetOutputParamInt(int nOutputPort,int nOutputVal);//设置相机自定义输出内容 
	//返回值 0:成功 1:不支持自定义信号 2:设置自定义信号失败 3:打开端口失败 4:写输出值失败 

	
	void SaveParamToINI();		// 保存相机设置信息
	
	//
	//////////////////////////////////////////////////////////////////////////
	
	//属性
protected:
	CGrabber *m_pGrabber;
};
