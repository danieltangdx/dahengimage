/************************************************************************/
/* Copyright (c) 2006, 中国大恒集团北京图像视觉技术分公司   
/* All rights reserved.													
/*																		
/* 文件名称： GrabberExport.cpp												
/* 文件标识： 见配置管理计划书
/* 摘要： 导出采集卡类
/*
/* 当前版本： 1.0
/* 作者： 阴同
/* 完成日期： 2013年10月12日
/************************************************************************/

#include "stdafx.h"
#include "DHGrabForMER.h"
#include "CameraBase.H"

CDHGrabForMER::CDHGrabForMER()
{
	//定义代理类的方法
	m_pGrabber = new CameraBase;
}

CDHGrabForMER::~CDHGrabForMER()
{
	if(m_pGrabber != NULL)
	{
		delete m_pGrabber;
		m_pGrabber = NULL;
	}
}
	
//初始化
BOOL CDHGrabForMER::Init(const s_GBINITSTRUCT* pInitParam)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Init(pInitParam));
}
bool CDHGrabForMER::Init(const CGrabInitInfo& rGrabInitInfo)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Init(rGrabInitInfo));
}

//关闭
BOOL CDHGrabForMER::Close()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Close());
}

//开始采集
BOOL CDHGrabForMER::StartGrab()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->StartGrab());
}

//停止采集
BOOL CDHGrabForMER::StopGrab()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->StopGrab());
}

// 单帧采集
BOOL CDHGrabForMER::Snapshot()
{
	ASSERT(m_pGrabber != NULL);
	return(((CameraBase *)m_pGrabber)->Snapshot());
}
	
//得到参数
BOOL CDHGrabForMER::GetParamInt(GBParamID Param, int &nOutputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->GetParamInt(Param, nOutputVal));
}
	
//设置参数
BOOL CDHGrabForMER::SetParamInt(GBParamID Param, int nInputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->SetParamInt(Param, nInputVal));
}

bool CDHGrabForMER::GetParamBool(GBParamID Param, bool bRetVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->GetParamBool(Param, bRetVal));
}

//设置参数
bool CDHGrabForMER::SetParamBool(GBParamID Param, bool bInputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->SetParamBool(Param, bInputVal));
}

//调用参数对话框
void CDHGrabForMER::CallParamDialog()
{
	ASSERT(m_pGrabber != NULL);
	m_pGrabber->CallParamDialog();
}

//得到出错信息
void CDHGrabForMER::GetLastErrorInfo(s_GBERRORINFO *pErrorInfo)
{
	ASSERT(m_pGrabber != NULL);
	m_pGrabber->GetLastErrorInfo(pErrorInfo);
}		

BOOL CDHGrabForMER::MERSetParamInt(MERParamID Param, int nInputVal1,int nInputVal2,int nInputVal3,int nInputVal4,int nInputVal5,void *sInputVal6)
{
	ASSERT(m_pGrabber != NULL);

	BOOL bRet = FALSE;	

	bRet = ((CameraBase *)m_pGrabber)->MERSetParamInt(Param, nInputVal1,nInputVal2,nInputVal3,nInputVal4,nInputVal5,sInputVal6);

	return bRet;
}

BOOL CDHGrabForMER::MERGetParamInt(MERParamID Param, int &nOutputVal1,int &nOutputVal2,int &nOutputVal3)
{
	ASSERT(m_pGrabber != NULL);
	
	BOOL bRet = FALSE;
	bRet = ((CameraBase *)m_pGrabber)->MERGetParamInt(Param, nOutputVal1,nOutputVal2,nOutputVal3);


	return bRet;
}

int CDHGrabForMER::MERSetOutputParamInt(int nOutputPort,int nOutputVal)
{
	ASSERT(m_pGrabber != NULL);
	BOOL bRet = FALSE;

	if(((CameraBase *)m_pGrabber)->SetOutputValue(nOutputPort,nOutputVal))
	{
		return 0;
	}//发送输出内容
	else
	{
		return 1;
	}

}


// 保存相机设置信息
void CDHGrabForMER::SaveParamToINI()
{
	((CameraBase *)m_pGrabber)->SaveParamToINI();
}

//
//////////////////////////////////////////////////////////////////////////
