#include "stdafx.h"
#include "XCTitleBar.h"
#include "CheckSystemDoc.h"
#include "CheckSystemView.h"


CXCTitleBar::CXCTitleBar()
{
}


CXCTitleBar::~CXCTitleBar()
{
}
bool CXCTitleBar::Create(HWND hwndParent)
{
	bool bRet{ false };
	/*************************************<Load XC resource>*************************************/
	m_sAppPath = CExFn::GetCurrentAppPath();
	m_sResourceFilePath = m_sAppPath + _T("\\resource\\xml");
	wstring sPathFile{ L"" }, sPathFolder{ L"" }, sResourceFilePath{ L"" };
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
	sPathFile = m_sResourceFilePath + _T("\\TitleBar.xml");
#else
	sPathFile = m_sResourceFilePath + L"\\TitleBar.xml";
#endif
	m_hWindow = (HWINDOW)XC_LoadLayout(sPathFile.c_str(), hwndParent);
	if (0 == m_hWindow)
	{
		AfxMessageBox(tr("Load Layout File Failed!"));
			return false;
	}
	m_hWnd = XWnd_GetHWND(m_hWindow);
	XWnd_ShowWindow(m_hWindow ,SW_SHOW);
	::SetParent(m_hWnd, hwndParent);
	//XWnd_SetTransparentType(m_hWindow, window_transparent_shadow);
	//XWnd_SetTransparentAlpha(m_hWindow, 100);
	RegistEvent();
	return true;
}


void CXCTitleBar::AdjustLayout()
{
	XWnd_AdjustLayout(m_hWindow);
}


void CXCTitleBar::RegistEvent()
{
	m_btnExit = (HELE)XC_GetObjectByID(m_hWindow, ID_Button_Exit);
	m_btnRunning = (HELE)XC_GetObjectByID(m_hWindow, ID_TabButton_Running);
	m_btnSetting = (HELE)XC_GetObjectByID(m_hWindow, ID_TabButton_Setting);
	m_btnOnOff = (HELE)XC_GetObjectByID(m_hWindow, ID_Button_OnOff);
	m_btnOpenDemo = (HELE)XC_GetObjectByID(m_hWindow, ID_Button_Demo);
	m_btnIOCard = (HELE)XC_GetObjectByID(m_hWindow, ID_Button_IOCard);
	XEle_Enable(m_btnOnOff, FALSE);
	XEle_RegEventCPP(m_btnExit, XE_BNCLICK, &CXCTitleBar::OnBnClickExit);
	//XEle_RegEventCPP(m_btnRunning, XE_BNCLICK, &CXCTitleBar::OnBnClickRunning);
	//XEle_RegEventCPP(m_btnSetting, XE_BNCLICK, &CXCTitleBar::OnBnClickSetting);
	XEle_RegEventCPP(m_btnOpenDemo, XE_BNCLICK, &CXCTitleBar::OnBnClickDemo);
	XEle_RegEventCPP(m_btnIOCard, XE_BNCLICK, &CXCTitleBar::OnBtnClickIOCard);
	XEle_RegEventCPP(m_btnOnOff, XE_BUTTON_CHECK, &CXCTitleBar::OnBtnCheckOnOff);
}


int CXCTitleBar::OnBnClickExit(BOOL *pbHandled)
{
	::PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);
	//m_pMainFrame
	*pbHandled = TRUE;
	return 0;
}


// OpenDemoProgram
int CXCTitleBar::OnBnClickDemo(BOOL *pbHandled)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCheckSystemView* pView = (CCheckSystemView*)(pFrame->GetActiveView());
	pFrame->OnBtnClickOpenDemo();
	return 0;
}


// Choose tab page of Running
int CXCTitleBar::OnBnClickRunning(BOOL *pbHandled)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCheckSystemView* pView = (CCheckSystemView*)(pFrame->GetActiveView());
	pView->OnBnClickTabPageChange(0);
	return 0;
}


// Choose tab page of Setting
int CXCTitleBar::OnBnClickSetting(BOOL *pbHandled)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CCheckSystemView* pView = (CCheckSystemView*)(pFrame->GetActiveView());
	pView->OnBnClickTabPageChange(1);
	return 0;
}


int CXCTitleBar::OnBtnCheckOnOff(BOOL bCheck, BOOL *pbHandled)
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CCheckSystemView* pView = (CCheckSystemView*)(pFrame->GetActiveView()); 
	if (TRUE == bCheck)
		pFrame->StartCheck();
	else
		pFrame->StopCheck();
	return 0;
}

int CXCTitleBar::OnBtnClickIOCard(BOOL *pbHandled)
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	CCheckSystemView* pView = (CCheckSystemView*)(pFrame->GetActiveView());
	pFrame->OnBnClickOpenParamSet();
	return 0;
}



bool CXCTitleBar::EnbaleBtnOnOff(bool bEnable)
{
	XEle_Enable(m_btnOnOff, bEnable ? TRUE : FALSE);
	return true;
}
