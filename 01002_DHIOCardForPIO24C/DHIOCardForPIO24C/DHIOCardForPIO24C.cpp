// DHIOCardForPIO24C.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "DHIOCardForPIO24C.h"
#include "DHIOCardForPIO24CInterior.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDHIOCardForPIO24C::CDHIOCardForPIO24C()
{
	m_pIOCard = nullptr;
	m_pIOCard = new CDHIOCardForPIO24CInterior();
}

CDHIOCardForPIO24C::~CDHIOCardForPIO24C()
{
	if (nullptr != m_pIOCard)
	{
		delete m_pIOCard;
		m_pIOCard = nullptr;
	}
}

bool CDHIOCardForPIO24C::InitCard(CIOCardInitInfo & rIOCardInitInfo)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->InitCard(rIOCardInitInfo);
	}
	return false;
}

e_IOCardType CDHIOCardForPIO24C::GetIOCardType()
{
	return e_IOCardType_PIOC;
}

bool CDHIOCardForPIO24C::OpenCard()
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->OpenCard();
	}
	return false;
}

bool CDHIOCardForPIO24C::CloseCard()
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->CloseCard();
	}
	return false;
}

bool CDHIOCardForPIO24C::ResetIOCard()
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->ResetIOCard();
	}
	return false;
}

bool CDHIOCardForPIO24C::OpenTools(CWnd * pParent)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->OpenTools(pParent);
	}
	return false;
}

bool CDHIOCardForPIO24C::Enable(bool bEnable)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->Enable(bEnable);
	}
	return false;
}

bool CDHIOCardForPIO24C::EnableQueue(bool bEnable)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->EnableQueue(bEnable);
	}
	return false;
}

bool CDHIOCardForPIO24C::GetParam(s_IOCardParaInfo & para)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetParam(para);
	}
	return false;
}

bool CDHIOCardForPIO24C::SetParam(s_IOCardParaInfo & para)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->SetParam(para);
	}
	return false;
}

bool CDHIOCardForPIO24C::WriteKickResult(s_sRsltInfoWritten & rslt)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->WriteKickResult(rslt);
	}
	return false;
}

bool CDHIOCardForPIO24C::NotifyCardOfGrabCallBack(s_NotifyInfoOfGrabCallback & notifyInfo)
{
	return true;
}

CString CDHIOCardForPIO24C::GetLastError()
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetLastError();
	}
	return CString();
}

bool CDHIOCardForPIO24C::GetInputIndex(int iInputport, int & iValue)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetInputIndex(iInputport, iValue);
	}
	return false;
}

bool CDHIOCardForPIO24C::GetGrabImgSN(int iGrabSN, int& iImageSN)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetGrabImgSN(iGrabSN, iImageSN);
	}
	return false;
}

bool CDHIOCardForPIO24C::GetKickCountInfo(int iKickSN, s_KickCountInfo & sKickCntInfo)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetKickCountInfo(iKickSN, sKickCntInfo);
	}
	return false;
}

bool CDHIOCardForPIO24C::SetOutIOStatus(int iOutport, bool bStatus)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->SetOutIOStatus(iOutport, bStatus);
	}
	return false;
}

bool CDHIOCardForPIO24C::GetOutputCount(int iOutport, int&iValue)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetOutputCount(iOutport, iValue);
	}
	return false;
}
