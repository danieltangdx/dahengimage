#include "stdafx.h"
#include "XCCtrlBtnBar.h"


CXCCtrlBtnBar::CXCCtrlBtnBar()
{
}


CXCCtrlBtnBar::~CXCCtrlBtnBar()
{
}

bool CXCCtrlBtnBar::Create(HWND hwndParent)
{
	bool bRet{ false };
	/*************************************<Load XC resource>*************************************/
	m_sAppPath = CExFn::GetCurrentAppPath();
	m_sResourceFilePath = m_sAppPath + _T("\\resource\\xml");
	wstring sPathFile{ L"" }, sPathFolder{ L"" }, sResourceFilePath{L""};
#ifdef UNICODE
	sPathFile = m_sResourceFilePath + _T("\\resource.res");
#else
	sPathFile = m_sResourceFilePath + L"\\resource.res";
	sResourceFilePath = CStringConvert::ANSI_UCS2(m_sResourceFilePath.GetBuffer());
#endif
	bRet = TRUE == XC_LoadResource(sPathFile.c_str(), sResourceFilePath.c_str());
	if (!bRet)
	{
		AfxMessageBox(tr("Load Resource File Failed!"));
		return false;
	}
#ifdef UNICODE
	sPathFile = m_sResourceFilePath + _T("\\style_gray.css");
#else
	sPathFile = m_sResourceFilePath + L"\\style_gray.css";
#endif
	bRet = TRUE == XC_LoadStyle(sPathFile.c_str(), sResourceFilePath.c_str());
	if (!bRet)
	{
		AfxMessageBox(tr("Load Style File Failed!"));
		return false;
	}
#ifdef UNICODE
	sPathFile = m_sResourceFilePath + _T("\\CtrlBtnBar.xml");
#else
	sPathFile = m_sResourceFilePath + L"\\CtrlBtnBar.xml";
#endif
	m_hWindow = (HWINDOW)XC_LoadLayout(sPathFile.c_str(), hwndParent);
	if (0 == m_hWindow)
	{
		AfxMessageBox(tr("Load Layout File Failed!"));
		return false;
	}
	m_hWnd = XWnd_GetHWND(m_hWindow);
	XWnd_ShowWindow(m_hWindow, SW_SHOW);
	::SetParent(m_hWnd, hwndParent);
	//XWnd_SetTransparentType(m_hWindow, window_transparent_shadow);
	//XWnd_SetTransparentAlpha(m_hWindow, 100);

	return true;
}

void CXCCtrlBtnBar::AdjustLayout()
{
	XWnd_AdjustLayout(m_hWindow);
}
