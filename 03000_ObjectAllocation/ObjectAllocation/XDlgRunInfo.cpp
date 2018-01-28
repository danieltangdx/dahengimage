#include "stdafx.h"
#include "XDlgRunInfo.h"


XDlgRunInfo::XDlgRunInfo()
{
}


XDlgRunInfo::~XDlgRunInfo()
{
}

void XDlgRunInfo::Init()
{
	m_hWindow = (HWINDOW)XC_LoadLayout(L"res/xml/layout_sysSetting.xml");
	XC_LoadStyle(L"style1.css", L"D:/Workspace/Projects/DahengProjects/TubeCheck/bin/res/xml");
	//XC_LoadStyle(L"style2.css", L"D:/Workspace/Projects/DahengProjects/TubeCheck/bin/res/xml");
	XC_LoadResource(L"resource.res", L"D:/Workspace/Projects/DahengProjects/TubeCheck/bin/res/xml");
	XC_LoadResource(L"resource1.res", L"D:/Workspace/Projects/DahengProjects/TubeCheck/bin/res/xml");
	XC_LoadResource(L"resource2.res", L"D:/Workspace/Projects/DahengProjects/TubeCheck/bin/res/xml");
	if (m_hWindow)
	{
		XWnd_ShowWindow(m_hWindow, SW_SHOW); //ÏÔÊ¾´°¿Ú
	}
}

void XDlgRunInfo::ShowWindow(int iShow)
{
	XWnd_ShowWindow(m_hWindow, iShow);
}
