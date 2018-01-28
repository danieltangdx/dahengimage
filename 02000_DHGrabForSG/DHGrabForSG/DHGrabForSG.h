/************************************************************************/
/* Copyright (c) 2007, 中国大恒集团北京图像视觉技术分公司视觉系统部  
/* All rights reserved.													
/*																		
/* 文件名称： GrabberExportForSG.h	
/* 摘要： 导出采集卡类
/*
/* 当前版本： 3.0
/* 修改者： 江卓佳
/* 修改内容摘要：初始化结构体及采集回调结构体加入“采集卡类型编号”。// [2009-12-18 by jzj]: add
/* 完成日期： 2009年12月18日
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
/* 完成日期： 2006年07月27日
/************************************************************************/


#pragma once
#include "resource.h"

#include "Grabber.h"

//////////////////////////////////////////////////////////////////////////
//模拟采集卡专有参数

// [2009-12-17 by jzj]: add 
//模拟采集卡参数
enum SGParamID	// 修改时注意与SimulateGrabber.h中的“PARAM ID”定义相一致
{
	SGIsPlaneRGB = 0,		//是否分通道 (0：否，1：是)
	SGGrabSpeed,			//采图速度 (单位:毫秒/张)
	SGImageWidth,			//图像宽度 (单位:像素)//[2006-12-01]
	SGImageHeight,			//图像高度 (单位:像素)//[2006-12-01]
	SGImageByteCount,		//图像每像素字节量 (单位:字节)//[2006-12-01]
};
//
//////////////////////////////////////////////////////////////////////////

//采集类
class _declspec(dllexport) CDHGrabForSG : public CGrabber
{
//操作
public:
	CDHGrabForSG();
	virtual ~CDHGrabForSG();
	
	//初始化
	BOOL Init(const s_GBINITSTRUCT* pInitParam);
	virtual bool Init(const CGrabInitInfo& rGrabInitInfo);

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

	// [2009-12-17 by jzj]: add
	BOOL SGSetParamInt(SGParamID Param, int nInputVal);
	BOOL SGGetParamInt(SGParamID Param, int &nOutputVal);
	//
	//////////////////////////////////////////////////////////////////////////
	
//属性
protected:
	CGrabber *m_pGrabber;
};

