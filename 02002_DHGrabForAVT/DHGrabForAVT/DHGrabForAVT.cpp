/************************************************************************/
/* Copyright (c) 2010, 中国大恒集团北京图像视觉技术分公司视觉系统部  
/* All rights reserved.													
/*																		
/* 文件名称： DHGrabberForAVT.h	
/* 摘要： 针对AVT相机FGCamera版本(2.10)及其以前版本  如有更高版本则需要升级
/*
/* 当前版本： 3.0.0.1
/* 修改者： 杨富森
/* 修改内容摘要：初始化结构体及采集回调结构体加入“采集卡类型编号”。
/* 完成日期： 2010年1月4日
/*
/* 以前版本： 3.0.0.1
/* 以前版本编写：李庆林
/* 以前记录：适应多个相机；更新FGCamera;读取白平衡最大值;参数设置框修改触发初始化;
/* 添加亮度、增益、曝光最大值；修改触发值不对;修改黑白相机不要获得白平衡值
/* 修改者： 杨富森
/************************************************************************/

#include "stdafx.h"
#include "DHGrabForAVT.h"
#include "AVTCamDH.h"

CDHGrabForAVT::CDHGrabForAVT()
{
	m_pGrabber = new CAVTCamDH;
}

CDHGrabForAVT::~CDHGrabForAVT()
{
	if(m_pGrabber != NULL)
	{
		delete m_pGrabber;
		m_pGrabber = NULL;
	}
}

//初始化
BOOL CDHGrabForAVT::Init(const s_GBINITSTRUCT* pInitParam)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Init(pInitParam));
}

bool CDHGrabForAVT::Init(const CGrabInitInfo& rGrabInitInfo)
{
	return m_pGrabber->Init(rGrabInitInfo);
}

//关闭
BOOL CDHGrabForAVT::Close()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Close());
}

//开始采集
BOOL CDHGrabForAVT::StartGrab()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->StartGrab());
}

//停止采集
BOOL CDHGrabForAVT::StopGrab()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->StopGrab());
}
	
//得到参数
BOOL CDHGrabForAVT::GetParamInt(GBParamID Param, int &nOutputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->GetParamInt(Param, nOutputVal));
}
	
//设置参数
BOOL CDHGrabForAVT::SetParamInt(GBParamID Param, int nInputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->SetParamInt(Param, nInputVal));
}


//得到参数
BOOL CDHGrabForAVT::GetParamfloat(int Param, float &nOutputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->GetParamfloat(Param, nOutputVal));
}

//设置参数
BOOL CDHGrabForAVT::SetParamfloat(int Param, float nInputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->SetParamfloat(Param, nInputVal));
}

//调用参数对话框
void CDHGrabForAVT::CallParamDialog()
{
	ASSERT(m_pGrabber != NULL);
	m_pGrabber->CallParamDialog();
}

//得到出错信息
void CDHGrabForAVT::GetLastErrorInfo(s_GBERRORINFO *pErrorInfo)
{
	ASSERT(m_pGrabber != NULL);
	m_pGrabber->GetLastErrorInfo(pErrorInfo);
}	

BOOL CDHGrabForAVT::GetTriggerParam(int &ParamValOutput)
{
	BOOL bRet = FALSE;
	// 请添加相应函数
	bRet = m_pGrabber->GetTriggerParam(ParamValOutput);
	return bRet;
}

BOOL CDHGrabForAVT::SetTriggerParam(int ParamValInput)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->SetTriggerParam(ParamValInput);
	return bRet;
}
BOOL CDHGrabForAVT::GetSystemInfo(CString &sOutputInfo)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->GetSystemInfo(sOutputInfo);
	return bRet;
}

BOOL CDHGrabForAVT::SetAOI(RECT rtAOI)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->SetAOI(rtAOI);
	return bRet;
}

BOOL CDHGrabForAVT::GetAOI(RECT &rtAOI)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->GetAOI(rtAOI);
	return bRet;
}


BOOL CDHGrabForAVT::Reset()
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->Reset();
	return bRet;
}

BOOL CDHGrabForAVT::CreateDefaultConfigurationFile(CString &sInitFile)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->CreateDefaultConfigurationFile(sInitFile);
	return bRet;
}

BOOL CDHGrabForAVT::SaveToConfigurationFile(CString &sInitFile)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->SaveToConfigurationFile(sInitFile);
	return bRet;
}
//////////////////////////////////////////////////////////////////////////
// DirectShow专有接口 
// 单帧采集
BOOL CDHGrabForAVT::Snapshot()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Snapshot());
}





