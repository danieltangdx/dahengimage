/************************************************************************/
/* Copyright (c) 2009, 中国大恒集团北京图像视觉技术分公司视觉系统部  
/* All rights reserved.													
/*																		
/* 文件名称： DHGrabberForBaumer.CPP	
/* 摘要： 导出采集卡类（Baumer千兆网相机）
/************************************************************************/

#include "stdafx.h"
#include "DHGrabForBaumer.h"
#include "BaumerCamDH.h"

CDHGrabForBaumer::CDHGrabForBaumer()
{
	m_pGrabber = new CBaumerCamDH;
}

CDHGrabForBaumer::~CDHGrabForBaumer()
{
	if(m_pGrabber != NULL)
	{
		delete m_pGrabber;
		m_pGrabber = NULL;
	}
}
	
//初始化
BOOL CDHGrabForBaumer::Init(const s_GBINITSTRUCT* pInitParam)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Init(pInitParam));
}

bool CDHGrabForBaumer::Init(const CGrabInitInfo & rGrabInitInfo)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Init(rGrabInitInfo));
}

//关闭
BOOL CDHGrabForBaumer::Close()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Close());
}

//开始采集
BOOL CDHGrabForBaumer::StartGrab()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->StartGrab());
}

//停止采集
BOOL CDHGrabForBaumer::StopGrab()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->StopGrab());
}
	
//得到参数
BOOL CDHGrabForBaumer::GetParamInt(GBParamID Param, int &nOutputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->GetParamInt((GBParamID)Param, nOutputVal));
}
	
//设置参数
BOOL CDHGrabForBaumer::SetParamInt(GBParamID Param, int nInputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->SetParamInt((GBParamID)Param, nInputVal));
}

//调用参数对话框
void CDHGrabForBaumer::CallParamDialog()
{
	ASSERT(m_pGrabber != NULL);
	m_pGrabber->CallParamDialog();
}

//得到出错信息
void CDHGrabForBaumer::GetLastErrorInfo(s_GBERRORINFO *pErrorInfo)
{
	ASSERT(m_pGrabber != NULL);
	m_pGrabber->GetLastErrorInfo(pErrorInfo);
}		

BOOL CDHGrabForBaumer::SetParamfloat(int Param, float nInputVal)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->SetParamfloat(Param,nInputVal);
	return bRet;
}	

BOOL CDHGrabForBaumer::GetParamfloat(int Param, float& nOutputVal)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->GetParamfloat(Param, nOutputVal);
	return bRet;
}	

BOOL CDHGrabForBaumer::SetTriggerParam(int ParamValInput)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->SetTriggerParam(ParamValInput);
	return bRet;
}

BOOL CDHGrabForBaumer::SetAOI(RECT rtAOI)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->SetAOI(rtAOI);
	return bRet;
}

BOOL CDHGrabForBaumer::GetAOI(RECT &rtAOI)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->GetAOI(rtAOI);
	return bRet;
}

BOOL CDHGrabForBaumer::Snapshot()
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->Snapshot();
	return bRet;
}



//得到触发模式
BOOL CDHGrabForBaumer::GetTriggerParam(int &ParamValOutput)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->GetTriggerParam(ParamValOutput);
	return bRet;
}

//重启设备
BOOL CDHGrabForBaumer::Reset()
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->Reset();
	return bRet;
}

//得到系统信息
BOOL CDHGrabForBaumer::GetSystemInfo(CString &sOutputInfo)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->GetSystemInfo(sOutputInfo);
	return bRet;
}

//创建默认配置文件
BOOL CDHGrabForBaumer::CreateDefaultConfigurationFile(CString &sInitFile)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->CreateDefaultConfigurationFile(sInitFile);
	return bRet;
}

//保存配置文件
BOOL CDHGrabForBaumer::SaveToConfigurationFile(CString &sInitFile)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->SaveToConfigurationFile(sInitFile);
	return bRet;
}

