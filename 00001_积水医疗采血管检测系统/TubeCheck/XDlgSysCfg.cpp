#include "stdafx.h"
#include "XDlgSysCfg.h"


XDlgSysCfg::XDlgSysCfg(HWND hParent)
{
	m_hParent = hParent;
}

XDlgSysCfg::XDlgSysCfg(HWND hParent, wchar_t* pwszXML)
{
	m_hParent = hParent;
}


XDlgSysCfg::~XDlgSysCfg()
{
}

bool XDlgSysCfg::Init(HWND hParent)
{
	m_hParent = hParent;
	return true;
}

bool XDlgSysCfg::Create(wchar_t* pwszXML)
{
	//m_hWindow = XWnd_CreateEx(NULL, NULL, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 600, 350, m_hParent, xc_window_style_default); //创建窗口
	//m_hWindow =  XModalWnd_Create(500, 350, L"系统设置", m_hParent);
	bool bRet = XC_LoadResource(L"D:\\Workspace\\Projects\\DahengProjects\\TubeCheck\\bin\\resource\\xml\\resource.res", L"D:\\Workspace\\Projects\\DahengProjects\\TubeCheck\\bin\\resource\\xml");
	bRet=XC_LoadStyle(L"D:\\Workspace\\Projects\\DahengProjects\\TubeCheck\\bin\\resource\\xml\\style_gray.css", L"D:\\Workspace\\Projects\\DahengProjects\\TubeCheck\\bin\\resource\\xml");
	m_hWindow = (HWINDOW)XC_LoadLayout(L"D:\\Workspace\\Projects\\DahengProjects\\TubeCheck\\bin\\resource\\xml\\layout_SystemConfig.xml", m_hParent);
	XWnd_AdjustLayout(m_hWindow);
	if (m_hWindow)
	{
// 		m_hBtnExit = XBtn_Create(10, 10, 80, 25, L"Close", m_hWindow); //创建按钮
// 		XBtn_SetType(m_hBtnExit, button_type_close);

		//HBKM hBkmExit = XEle_GetBkManager(m_hBtnExit);
		//XBkM_Clear(hBkmExit);
//		XEle_EnableBkTransparent(m_hBtnExit, true);
		//XWnd_SetTransparentType(m_hWindow, window_transparent_shaped);
		//HIMAGE hImgSysBtnCloseHover = XImage_LoadFile(TEXT("res/sysbtn_close_hover.png", image_draw_type_stretch));
		//XBtn_AddBkImage(m_hBtnExit, button_state_stay, hImgSysBtnCloseHover);
		//XBtn_AddBkImage(m_hBtnExit, button_state_leave, XImage_LoadFile(TEXT("res/sysbtn_close_normal.png", image_draw_type_stretch)));
// 		CRect rectBody{ 0 };
// 		XWnd_GetBodyRect(m_hWindow, &rectBody);
// 		XEle_SetRectEx(m_hBtnExit, 1920 - 30, 0, 30, 27, true);
// 		hBkmExit->unused;

		//HBKM hBkmExit = XEle_GetBkManager(m_hBtnExit);
		//XBkM_AddImage(hBkmExit, button_state_stay, XImage_LoadFile(TEXT("res/sysbtn_close_hover.png", true)));


		//HBKM hBkm = XWnd_GetBkManager(m_hWindow);
		//XBkM_AddImage(hBkm, window_state_flag_leave, XImage_LoadFile(L"res/bk.jpg", image_draw_type_stretch));

		//	m_btnStart.Create(_T("Start"), WS_CHILD | WS_VISIBLE, CRect(0, 0, 100, 30), m_pParendWnd, 1000);
		//	SetParent(m_btnStart.m_hWnd, XWnd_GetHWND(m_hWindow));



		XWnd_ShowWindow(m_hWindow, SW_SHOW); //显示窗口
		RegisterEvent();
		return true;
	}
	//return NULL;
}

int XDlgSysCfg::OnEventBtnClick(BOOL *pBool)
{
	//弹出消息框
	::PostMessage(AfxGetMainWnd()->m_hWnd, WM_QUIT, 0, 0);
	//CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	//pFrame->OnClose();
	//MessageBoxW(XWnd_GetHWND(m_hWindow), L"你点击了按钮", L"提示", MB_OK);
	//*pBool = false; //对该事件进行拦截
	return 0;    //事件的返回值
}

void XDlgSysCfg::RegisterEvent()
{

	//XEle_RegEventCPP(m_hBtnExit, XE_BNCLICK, &XDlgSysCfg::OnEventBtnClick); //注册按钮点击事件
}

int XDlgSysCfg::DoModal()
{
	if(NULL !=m_hWindow)
		return XModalWnd_DoModal(m_hWindow);
}