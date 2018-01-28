// DHGrabberJAI.cpp: implementation of the CDHGrabForJAI class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JAIGrabber.h"
#include "DHGrabForJAI.h"
#include "JAICamDH.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDHGrabForJAI::CDHGrabForJAI()
{
	m_pGrabber = new CJAICamDH;
}

CDHGrabForJAI::~CDHGrabForJAI()
{
	if(m_pGrabber != NULL)
	{
		delete m_pGrabber;
		m_pGrabber = NULL;
	}
}

//初始化
BOOL CDHGrabForJAI::Init(const s_GBINITSTRUCT* pInitParam)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Init(pInitParam));
}

//关闭
BOOL CDHGrabForJAI::Close()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Close());
}

//开始采集
BOOL CDHGrabForJAI::StartGrab()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->StartGrab());
}

//停止采集
BOOL CDHGrabForJAI::StopGrab()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->StopGrab());
}
	
//得到参数
BOOL CDHGrabForJAI::GetParamInt(GBParamID Param, int &nOutputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->GetParamInt(Param, nOutputVal));
}
	
//设置参数
BOOL CDHGrabForJAI::SetParamInt(GBParamID Param, int nInputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->SetParamInt(Param, nInputVal));
}


//得到参数
BOOL CDHGrabForJAI::GetParamfloat(int Param, float &nOutputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->GetParamfloat(Param, nOutputVal));
}

//设置参数
BOOL CDHGrabForJAI::SetParamfloat(int Param, float nInputVal)
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->SetParamfloat(Param, nInputVal));
}

//调用参数对话框
void CDHGrabForJAI::CallParamDialog()
{
	ASSERT(m_pGrabber != NULL);
	m_pGrabber->CallParamDialog();
}

//得到出错信息
void CDHGrabForJAI::GetLastErrorInfo(s_GBERRORINFO *pErrorInfo)
{
	ASSERT(m_pGrabber != NULL);
	m_pGrabber->GetLastErrorInfo(pErrorInfo);
}	

BOOL CDHGrabForJAI::GetTriggerParam(int &ParamValOutput)
{
	BOOL bRet = FALSE;
	// 请添加相应函数
	bRet = m_pGrabber->GetTriggerParam(ParamValOutput);
	return bRet;
}

BOOL CDHGrabForJAI::SetTriggerParam(int ParamValInput)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->SetTriggerParam(ParamValInput);
	return bRet;
}
BOOL CDHGrabForJAI::GetSystemInfo(CString &sOutputInfo)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->GetSystemInfo(sOutputInfo);
	return bRet;
}

BOOL CDHGrabForJAI::SetAOI(RECT rtAOI)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->SetAOI(rtAOI);
	return bRet;
}

BOOL CDHGrabForJAI::GetAOI(RECT &rtAOI)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->GetAOI(rtAOI);
	return bRet;
}


BOOL CDHGrabForJAI::Reset()
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->Reset();
	return bRet;
}

BOOL CDHGrabForJAI::CreateDefaultConfigurationFile(CString &sInitFile)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->CreateDefaultConfigurationFile(sInitFile);
	return bRet;
}

BOOL CDHGrabForJAI::SaveToConfigurationFile(CString &sInitFile)
{
	BOOL bRet = FALSE;
	bRet = m_pGrabber->SaveToConfigurationFile(sInitFile);
	return bRet;
}
//////////////////////////////////////////////////////////////////////////
// DirectShow专有接口 
// 单帧采集
BOOL CDHGrabForJAI::Snapshot()
{
	ASSERT(m_pGrabber != NULL);
	return(m_pGrabber->Snapshot());
}