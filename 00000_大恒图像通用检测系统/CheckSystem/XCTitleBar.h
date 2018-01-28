#pragma once
#include "CheckSystem.h"
class CXCTitleBar
{
public:
	CXCTitleBar();
	~CXCTitleBar();
	bool Create(HWND hwndParent);

public:
	HWND m_hWnd{0};


private:
	HWINDOW m_hWindow{ NULL };
	CString m_sAppPath{ _T("") };
	CString m_sResourceFilePath{ _T("") };
	HELE m_btnExit;
	HELE m_btnOpenDemo;
	HELE m_btnRunning;
	HELE m_btnSetting;
	HELE m_btnOnOff;
	HELE m_btnIOCard;
public:
	void AdjustLayout();
	void RegistEvent();
	int OnBnClickExit(BOOL *pbHandled);
	// OpenDemoProgram
	int OnBnClickDemo(BOOL *pbHandled);
	// Choose tab page of Running
	int OnBnClickRunning(BOOL *pbHandled);
	// Choose tab page of Setting
	int OnBnClickSetting(BOOL *pbHandled);
	int OnBtnClickIOCard(BOOL *pbHandled);
	int OnBtnCheckOnOff(BOOL bCheck, BOOL *pbHandled);
	bool EnbaleBtnOnOff(bool bEnable);
};

