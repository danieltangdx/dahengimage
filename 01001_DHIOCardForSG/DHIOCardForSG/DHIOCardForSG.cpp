// DHIOCardForSG.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "DHIOCardForSG.h"
#include "DHIOCardForSGInterior.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CDHIOCardForSG::CDHIOCardForSG()
{
	m_pIOCard = nullptr;
	m_pIOCard = new CDHIOCardForSGInterior();
}

CDHIOCardForSG::~CDHIOCardForSG()
{
	if (nullptr != m_pIOCard)
		delete m_pIOCard;
}

bool CDHIOCardForSG::OpenTools(CWnd * pParent)
{
	if(nullptr != m_pIOCard)
		return m_pIOCard->OpenTools(pParent);
	return true;
}

bool CDHIOCardForSG::InitCard(CIOCardInitInfo& rIOCardInitInfo)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->InitCard(rIOCardInitInfo);
	}
	return true;
}

bool CDHIOCardForSG::OpenCard()
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->OpenCard();
	}
	return true;
}

bool CDHIOCardForSG::CloseCard()
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->CloseCard();
	}
	return true;
}

bool CDHIOCardForSG::ResetIOCard()
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->ResetIOCard();
	}
	return false;
}

bool CDHIOCardForSG::Enable(bool bEnable)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->Enable(bEnable);
	}
	return false;
}

bool CDHIOCardForSG::EnableQueue(bool bEnable)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->EnableQueue(bEnable);
	}
	return false;
}

bool CDHIOCardForSG::GetParam(s_IOCardParaInfo & para)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetParam(para);
	}
	return false;
}

bool CDHIOCardForSG::SetParam(s_IOCardParaInfo & para)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->SetParam(para);
	}
	return false;
}

bool CDHIOCardForSG::WriteKickResult(s_sRsltInfoWritten & rslt)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->WriteKickResult(rslt);
	}
	return false;
}

bool CDHIOCardForSG::NotifyCardOfGrabCallBack(s_NotifyInfoOfGrabCallback & notifyInfo)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->NotifyCardOfGrabCallBack(notifyInfo);
	}
	return false;
}

e_IOCardType CDHIOCardForSG::GetIOCardType()
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetIOCardType();
	}
	return e_IOCardType();
}

CString CDHIOCardForSG::GetLastError()
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetLastError();
	}
	return CString();
}

bool CDHIOCardForSG::GetInputIndex(int iInputport, int &iValue)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetInputIndex(iInputport, iValue);
	}
	return 0;
}

bool CDHIOCardForSG::GetGrabImgSN(int iGrabSN, int& iImageSN)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetGrabImgSN(iGrabSN, iImageSN);
	}
	return 0;
}

bool CDHIOCardForSG::GetKickCountInfo(int iKickSN, s_KickCountInfo& sKickCntInfo)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetKickCountInfo(iKickSN, sKickCntInfo);
	}
	return false;
}

bool CDHIOCardForSG::SetOutIOStatus(int nOutport, bool bStatus)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->SetOutIOStatus(nOutport, bStatus);
	}
	return false;
}

bool CDHIOCardForSG::GetOutputCount(int iOutport, int &iValue)
{
	if (nullptr != m_pIOCard)
	{
		return m_pIOCard->GetOutputCount(iOutport, iValue);
	}
	return 0;
}


