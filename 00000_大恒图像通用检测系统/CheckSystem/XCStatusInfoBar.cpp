#include "stdafx.h"
#include "XCStatusInfoBar.h"


CXCStatusInfoBar::CXCStatusInfoBar()
{
}


CXCStatusInfoBar::~CXCStatusInfoBar()
{
}

bool CXCStatusInfoBar::Create(HWND hwndParent)
{
// 	bool bRet{ false };
// 	/*************************************<Load XC resource>*************************************/
// 	m_sAppPath = CExFn::GetCurrentAppPath();
// 	m_sResourceFilePath = m_sAppPath + _T("\\resource\\xml");
// 	bRet = TRUE == XC_LoadResource(m_sResourceFilePath + _T("\\resource.res"), m_sResourceFilePath.GetBuffer());
// 	if (!bRet)
// 	{
// 		AfxMessageBox(tr("Load Resource File Failed!"));
// 		return false;
// 	}
// 	bRet = TRUE == XC_LoadStyle(m_sResourceFilePath + _T("\\style_gray.css"), m_sResourceFilePath.GetBuffer());
// 	if (!bRet)
// 	{
// 		AfxMessageBox(tr("Load Style File Failed!"));
// 		return false;
// 	}
// 	m_hWindow = (HWINDOW)XC_LoadLayout(m_sResourceFilePath + _T("\\StatusInfoBar.xml"), hwndParent);
// 	if (0 == m_hWindow)
// 	{
// 		AfxMessageBox(tr("Load Layout File Failed!"));
// 		return false;
// 	}
// 	m_hWnd = XWnd_GetHWND(m_hWindow);
// 	XWnd_ShowWindow(m_hWindow, SW_SHOW);
// 	::SetParent(m_hWnd, hwndParent);
// 	//XWnd_SetTransparentType(m_hWindow, window_transparent_shadow);
// 	//XWnd_SetTransparentAlpha(m_hWindow, 100);

	return true;
}

void CXCStatusInfoBar::AdjustLayout()
{
	XWnd_AdjustLayout(m_hWindow);
}
