// ThreeTierRights.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "ThreeTierRights.h"
#include "ThreeTierRightsInterior.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



CThreeTierRights::CThreeTierRights(CWnd * pParent)
{
	pInteroir = nullptr;
	CThreeTierRightsInterior* pThreeTierRightsInterior = new CThreeTierRightsInterior(pParent);
	pInteroir = (CThreeTierRightsInterior*)pThreeTierRightsInterior;
}

CThreeTierRights::~CThreeTierRights()
{
	if (nullptr != pInteroir)
	{
		CThreeTierRightsInterior* pThreeTierRightsInterior = (CThreeTierRightsInterior*)pInteroir;
		delete pThreeTierRightsInterior;
		pInteroir = nullptr;
	}
}

bool CThreeTierRights::Init(e_DataStoreType dataStoreType, s_DatabaseInfo* pDatabaseInfo)
{
// 	if (nullptr != pInteroir)
// 	{
// 		CThreeTierRightsInterior* pThreeTierRightsInterior = (CThreeTierRightsInterior*)pInteroir;
// 		delete pThreeTierRightsInterior;
// 		pInteroir = nullptr;
// 	}
	return false;
}

bool CThreeTierRights::Login(CWnd * pParent, bool bEnbaleFromDisk, bool bHint)
{
	if (nullptr != pInteroir)
	{
		CThreeTierRightsInterior* pThreeTierRightsInterior = (CThreeTierRightsInterior*)pInteroir;
		return pThreeTierRightsInterior->Login(pParent, bEnbaleFromDisk, bHint);
	}
	return false;
}


bool CThreeTierRights::VerifyRights(e_UserType iUserType, bool bHint)
{
	if (nullptr != pInteroir)
	{
		CThreeTierRightsInterior* pThreeTierRightsInterior = (CThreeTierRightsInterior*)pInteroir;
		return pThreeTierRightsInterior->VerifyRights(iUserType, bHint);
	}
	return false;
}

bool CThreeTierRights::VerifyRights(CWnd* pWnd, e_RightType iRightType, bool bHint)
{
	if (nullptr != pInteroir)
	{
		CThreeTierRightsInterior* pThreeTierRightsInterior = (CThreeTierRightsInterior*)pInteroir;
		return pThreeTierRightsInterior->VerifyRights(pWnd, iRightType, bHint);
	}
	return false;
}

void CThreeTierRights::ManageUsers()
{
	if (nullptr != pInteroir)
	{
		CThreeTierRightsInterior* pThreeTierRightsInterior = (CThreeTierRightsInterior*)pInteroir;
		pThreeTierRightsInterior->ManageUsers();
	}
}

bool CThreeTierRights::GetCurUserInfo(s_UserInfoOfThreeTierRights & rUserInfo)
{
	if (nullptr != pInteroir)
	{
		CThreeTierRightsInterior* pThreeTierRightsInterior = (CThreeTierRightsInterior*)pInteroir;
		return pThreeTierRightsInterior->GetCurUserInfo(rUserInfo);
	}
	return false;
}

bool CThreeTierRights::AddCtrl(CWnd* pWnd, e_RightType iRightType)
{
	if (nullptr != pInteroir)
	{
		CThreeTierRightsInterior* pThreeTierRightsInterior = (CThreeTierRightsInterior*)pInteroir;
		pThreeTierRightsInterior->AddCtrl(pWnd, iRightType);
	}
	return false;
}
