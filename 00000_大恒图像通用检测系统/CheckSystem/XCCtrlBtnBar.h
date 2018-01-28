#pragma once
#include "CheckSystem.h"
class CXCCtrlBtnBar
{
public:
	CXCCtrlBtnBar();
	~CXCCtrlBtnBar();
	bool Create(HWND hwndParent);

public:
	HWND m_hWnd{ 0 };


private:
	HWINDOW m_hWindow{ NULL };
	CString m_sAppPath{ _T("") };
	CString m_sResourceFilePath{ _T("") };

public:
	void AdjustLayout();

};

