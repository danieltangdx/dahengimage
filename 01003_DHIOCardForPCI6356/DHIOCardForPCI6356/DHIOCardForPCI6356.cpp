// DHIOCardForPCI6356.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "DHIOCardForPCI6356.h"
#include "DHIOCardForPCI6356Interior.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDHIOCardForPCI6356::CDHIOCardForPCI6356()
{
	m_pIOCard = nullptr;
	m_pIOCard = new CDHIOCardForPCI6356Interior();
}

CDHIOCardForPCI6356::~CDHIOCardForPCI6356()
{
// 	if (nullptr != m_pIOCard)
// 		delete m_pIOCard;
}

bool CDHIOCardForPCI6356::InitCard(CIOCardInitInfo& rIOCardInitInfo)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->InitCard(rIOCardInitInfo);
	}
	return false;		
}

bool CDHIOCardForPCI6356::OpenCard()
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->OpenCard();
	}
	return false;
}

bool CDHIOCardForPCI6356::CloseCard()
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->CloseCard();
	}
	return false;
}

bool CDHIOCardForPCI6356::ResetIOCard()
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->ResetIOCard();
	}
	return false;
}

bool CDHIOCardForPCI6356::OpenTools(CWnd * pParent)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->OpenTools(pParent);
	}
	return false;
}

bool CDHIOCardForPCI6356::Enable(bool bEnable)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->Enable(bEnable);
	}
	return false;
}

bool CDHIOCardForPCI6356::EnableQueue(bool bEnable)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->EnableQueue(bEnable);
	}
	return false;
}

bool CDHIOCardForPCI6356::GetParam(s_IOCardParaInfo & para)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetParam(para);
	}
	return false;
}

bool CDHIOCardForPCI6356::SetParam(s_IOCardParaInfo & para)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->SetParam(para);
	}
	return false;
}

bool CDHIOCardForPCI6356::WriteKickResult(s_sRsltInfoWritten & rslt)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->WriteKickResult(rslt);
	}
	return false;
}

bool CDHIOCardForPCI6356::NotifyCardOfGrabCallBack(s_NotifyInfoOfGrabCallback & notifyInfo)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->NotifyCardOfGrabCallBack(notifyInfo);
	}
	return false;
}

e_IOCardType CDHIOCardForPCI6356::GetIOCardType()
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetIOCardType();
	}
	return e_IOCardType();
}

CString CDHIOCardForPCI6356::GetLastError()
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetLastError();
	}
	return CString();
}

bool CDHIOCardForPCI6356::GetInputIndex(int iInputport, int &iValue)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetInputIndex(iInputport, iValue);
	}
	return 0;
}

bool CDHIOCardForPCI6356::GetGrabImgSN(int iGrabSN, int& iImageSN)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetGrabImgSN(iGrabSN, iImageSN);
	}
	return 0;
}

bool CDHIOCardForPCI6356::GetKickCountInfo(int iKickSN, s_KickCountInfo& sKickCntInfo)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetKickCountInfo(iKickSN, sKickCntInfo);
	}
	return false;
}

bool CDHIOCardForPCI6356::SetOutIOStatus(int iOutport, bool bStatus)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->SetOutIOStatus(iOutport, bStatus);
	}
	return false;
}

bool CDHIOCardForPCI6356::GetOutputCount(int iOutport, int&iValue)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetOutputCount(iOutport, iValue);
	}
	return 0;
}
