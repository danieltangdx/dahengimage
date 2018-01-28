// DHIOCardForRS232.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "DHIOCardForRS232_XZXNYN.h"
#include "DHIOCardForRS232_XZXNYNInterior.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDHIOCardForRS232_XZXNYN::CDHIOCardForRS232_XZXNYN()
{
	m_pIOCard = new CDHIOCardForRS232_XZXNYNInterior();
}

CDHIOCardForRS232_XZXNYN::~CDHIOCardForRS232_XZXNYN()
{
	if (nullptr != m_pIOCard)
	{
		delete m_pIOCard;
	}
}
bool CDHIOCardForRS232_XZXNYN::InitCard(CIOCardInitInfo& rIOCardInitInfo)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->InitCard(rIOCardInitInfo);
	}
	return false;
}

bool CDHIOCardForRS232_XZXNYN::OpenCard()
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->OpenCard();
	}
	return false;
}

bool CDHIOCardForRS232_XZXNYN::OpenTools(CWnd * pParent)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->OpenTools(pParent);
	}
	return false;
}

bool CDHIOCardForRS232_XZXNYN::Enable(bool bEnable)
{
	return false;
}

bool CDHIOCardForRS232_XZXNYN::CloseCard()
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->CloseCard();
	}
	return false;//**[9/11/2017 ConanSteve]**:  
}

bool CDHIOCardForRS232_XZXNYN::GetParam(s_IOCardParaInfo& para)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetParam(para);
	}
	return true;
}

bool CDHIOCardForRS232_XZXNYN::SetParam(s_IOCardParaInfo& para)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->SetParam(para);
	}
	return true;
}

bool CDHIOCardForRS232_XZXNYN::WriteKickResult(s_sRsltInfoWritten & rslt)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->WriteKickResult(rslt);
	}
	return true;
}

bool CDHIOCardForRS232_XZXNYN::NotifyCardOfGrabCallBack(s_NotifyInfoOfGrabCallback &notifyInfo)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->NotifyCardOfGrabCallBack(notifyInfo);
	}
	return false;
}

e_IOCardType CDHIOCardForRS232_XZXNYN::GetIOCardType()
{
	return e_IOCardType::e_IOCardType_RS232_XZXNYN;
}

CString CDHIOCardForRS232_XZXNYN::GetLastError()
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetLastError();
	}
	return false;
}

bool CDHIOCardForRS232_XZXNYN::GetInputIndex(int iInputport, int& iValue)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetInputIndex(iInputport, iValue);
	}
	return false;
}

bool CDHIOCardForRS232_XZXNYN::GetGrabImgSN(int iGrabSN, int& iImageSN)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetGrabImgSN(iGrabSN, iImageSN);
	}
}

bool CDHIOCardForRS232_XZXNYN::GetKickCountInfo(int iKickSN, s_KickCountInfo& sKickCntInfo)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetKickCountInfo(iKickSN, sKickCntInfo);
	}
	return false;
}

bool CDHIOCardForRS232_XZXNYN::SetOutIOStatus(int iOutport, bool bStatus)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->SetOutIOStatus(iOutport, bStatus);
	}
}

bool CDHIOCardForRS232_XZXNYN::GetOutputCount(int iOutport, int&iValue)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetOutputCount(iOutport, iValue);
	}
}

