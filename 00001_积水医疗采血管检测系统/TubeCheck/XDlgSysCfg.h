#pragma once
#include "StdAfx.h"
#include "TubeCheck.h"
class XDlgSysCfg
{
public:
	XDlgSysCfg(HWND hParent);
	XDlgSysCfg(HWND hParent,wchar_t* pwszXML);
	~XDlgSysCfg();
	bool Create(wchar_t* pwszXML=NULL);
	bool Init(HWND hParent);
	int DoModal();


private:
	void RegisterEvent();
	int OnEventBtnClick(BOOL *pBool);

public:
	HWINDOW m_hWindow{ 0 };
private:
	HWND m_hWnd{0};
	HWND m_hParent{NULL};
	HELE m_hBtnExit{NULL};
//	CButton m_btnStart;



};

class CWindowShadow
{
public:
	HWINDOW m_hWindow;
	HELE  m_hSlider_shadowSize;      //阴影大小
	HELE  m_hSlider_depth;           //阴影深度
	HELE  m_hSlider_shadowAngle;     //圆角阴影内收大小
	HELE  m_hBtn_Check_rightAnge;  //是否强制直角
	HELE  m_hRichEditColor; //阴影颜色

	int   m_nShadowSize;
	int   m_nShadowDepth; //0-255
	int   m_nShadowAngleSize;
	BOOL  m_bRightAngle;
	COLORREF  m_clrShadow;

	CWindowShadow()
	{
		m_nShadowSize = 10;
		m_nShadowDepth = 100;
		m_nShadowAngleSize = 10;
		m_clrShadow = RGB(255, 0, 0);
		m_bRightAngle = FALSE;
	}
	void Init(HWND hParent)
	{
		//m_hWindow = XWnd_Create(0, 0, 500, 400, L"炫彩界面库窗口", hParent, xc_window_style_default);
		m_hWindow = XModalWnd_Create(500, 350, L"系统设置", hParent);
		XWnd_SetTransparentAlpha(m_hWindow, 255);
		XBtn_SetType(XBtn_Create(150, 80, 200, 25, L"窗口阴影演示(关闭)", m_hWindow), button_type_close);

		XWnd_EnableDragWindow(m_hWindow, TRUE);
		XWnd_SetLayoutSize(m_hWindow, 15, 50, 15, 20);

		//	HIMAGE hImage=XImage_LoadFile(L"image\\bk.png");
		//	HBKM hBkInfoM= XWnd_GetBkManager(m_hWindow);
		//	XBkInfoM_AddImage(hBkInfoM,window_state_flag_leave,hImage);

		XWnd_SetShadowInfo(m_hWindow, m_nShadowSize, m_nShadowDepth, m_nShadowAngleSize, m_bRightAngle, m_clrShadow);
		int top = 150;
		int left = 110;
		m_hSlider_shadowSize = XSliderBar_Create(left, top, 300, 20, m_hWindow); top += 35;
		XEle_EnableBkTransparent(m_hSlider_shadowSize, TRUE);
		XSliderBar_SetRange(m_hSlider_shadowSize, 100);
		XSliderBar_SetPos(m_hSlider_shadowSize, m_nShadowAngleSize);

		m_hSlider_depth = XSliderBar_Create(left, top, 300, 20, m_hWindow); top += 35;
		XSliderBar_SetRange(m_hSlider_depth, 255);
		XEle_EnableBkTransparent(m_hSlider_depth, TRUE);
		XSliderBar_SetPos(m_hSlider_depth, m_nShadowDepth);

		m_hSlider_shadowAngle = XSliderBar_Create(left, top, 300, 20, m_hWindow); top += 35;
		XSliderBar_SetRange(m_hSlider_shadowAngle, 255);
		XEle_EnableBkTransparent(m_hSlider_shadowAngle, TRUE);
		XSliderBar_SetPos(m_hSlider_shadowAngle, m_nShadowAngleSize);

		m_hBtn_Check_rightAnge = XBtn_Create(left + 10, top, 100, 20, L" 啊啊啊 ", m_hWindow); top += 35;
		XEle_SetTextColor(m_hBtn_Check_rightAnge, RGB(255, 255, 255));
		XEle_EnableBkTransparent(m_hBtn_Check_rightAnge, TRUE);
		XBtn_SetCheck(m_hBtn_Check_rightAnge, m_bRightAngle);
		XBtn_SetTypeEx(m_hBtn_Check_rightAnge, button_type_check);

		m_hRichEditColor = XRichEditColor_Create(left + 10, top, 100, 22, m_hWindow);
		XEle_SetTextColor(m_hRichEditColor, RGB(255, 255, 255));
		XEle_EnableBkTransparent(m_hRichEditColor, TRUE);
		XRichEdit_EnableEvent_XE_RICHEDIT_CHANGE(m_hRichEditColor, TRUE);

		XWnd_SetTransparentType(m_hWindow, window_transparent_shadow);
		XEle_RegEventCPP(m_hSlider_shadowSize, XE_SLIDERBAR_CHANGE, &CWindowShadow::OnSliderBarChange_ShadowSize);
		XEle_RegEventCPP(m_hSlider_depth, XE_SLIDERBAR_CHANGE, &CWindowShadow::OnSliderBarChange_ShadowDepth);
		XEle_RegEventCPP(m_hSlider_shadowAngle, XE_SLIDERBAR_CHANGE, &CWindowShadow::OnSliderBarChange_ShadowAngleSize);

		XEle_RegEventCPP(m_hBtn_Check_rightAnge, XE_BUTTON_CHECK, &CWindowShadow::OnButtonCheck_ShadowRightAngle);
		XEle_RegEventCPP1(m_hRichEditColor, XE_RICHEDIT_CHANGE, &CWindowShadow::OnRichEditChange_ShadowColor);

		XWnd_RegEventCPP(m_hWindow, WM_PAINT, &CWindowShadow::OnWndDrawWindow);
		XWnd_AdjustLayout(m_hWindow);
		XWnd_ShowWindow(m_hWindow, SW_SHOW);
	}
	int OnSliderBarChange_ShadowSize(int pos, BOOL *pbHandled)
	{
		m_nShadowSize = pos;
		XWnd_SetShadowInfo(m_hWindow, m_nShadowSize, m_nShadowDepth, m_nShadowAngleSize, m_bRightAngle, m_clrShadow);
		XWnd_RedrawWnd(m_hWindow);
		return 0;
	}
	int OnSliderBarChange_ShadowDepth(int pos, BOOL *pbHandled)
	{
		m_nShadowDepth = pos;
		XWnd_SetShadowInfo(m_hWindow, m_nShadowSize, m_nShadowDepth, m_nShadowAngleSize, m_bRightAngle, m_clrShadow);
		XWnd_RedrawWnd(m_hWindow);
		return 0;
	}
	int OnSliderBarChange_ShadowAngleSize(int pos, BOOL *pbHandled)
	{
		m_nShadowAngleSize = pos;
		XWnd_SetShadowInfo(m_hWindow, m_nShadowSize, m_nShadowDepth, m_nShadowAngleSize, m_bRightAngle, m_clrShadow);
		XWnd_RedrawWnd(m_hWindow);
		return 0;
	}
	int  OnButtonCheck_ShadowRightAngle(BOOL bCheck, BOOL *pbHandled)
	{
		m_bRightAngle = bCheck;
		XWnd_SetShadowInfo(m_hWindow, m_nShadowSize, m_nShadowDepth, m_nShadowAngleSize, m_bRightAngle, m_clrShadow);
		XWnd_RedrawWnd(m_hWindow);
		return 0;
	}
	int  OnRichEditChange_ShadowColor(HELE hRichEditColor, BOOL *pbHandled)
	{
		m_clrShadow = XRichEditColor_GetColor(hRichEditColor);
		XWnd_SetShadowInfo(m_hWindow, m_nShadowSize, m_nShadowDepth, m_nShadowAngleSize, m_bRightAngle, m_clrShadow);
		XWnd_RedrawWnd(m_hWindow);
		return 0;
	}
	int  OnWndDrawWindow(HDRAW hDraw, BOOL *pbHandled)
	{
		*pbHandled = TRUE;
		RECT rect;
		XWnd_GetClientRect(m_hWindow, &rect);
		XDraw_SetBrushColor(hDraw, RGB(250, 250, 250));
		XDraw_FillRect(hDraw, &rect);

		RECT rcBody;
		XWnd_GetBodyRect(m_hWindow, &rcBody);
		XDraw_SetBrushColor(hDraw, RGB(200, 0, 0));
		XDraw_FillRect(hDraw, &rcBody);

		XDraw_SetBrushColor(hDraw, RGB(255, 255, 255));
		int top = 150;
		int left = 50;
		int right = 420;
		wchar_t  buf[256] = { 0 };
		XDraw_TextOutEx(hDraw, left, top + 3, L"阴影大小:");
		wsprintfW(buf, L"%d", m_nShadowSize);
		XDraw_TextOutEx(hDraw, right, top + 3, buf); top += 35;

		XDraw_TextOutEx(hDraw, left, top + 3, L"阴影深度:");
		wsprintfW(buf, L"%d", m_nShadowDepth);
		XDraw_TextOutEx(hDraw, right, top + 3, buf); top += 35;

		XDraw_TextOutEx(hDraw, left, top + 3, L"角 大 小:");
		wsprintfW(buf, L"%d", m_nShadowAngleSize);
		XDraw_TextOutEx(hDraw, right, top + 3, buf); top += 35;

		XDraw_TextOutEx(hDraw, left, top + 3, L"直    角:");
		wsprintfW(buf, L"%d", m_nShadowAngleSize);
		XDraw_TextOutEx(hDraw, right, top + 3, buf); top += 35;

		XDraw_TextOutEx(hDraw, left, top + 3, L"阴影颜色:");
		wsprintfW(buf, L"%d", m_nShadowAngleSize);
		XDraw_TextOutEx(hDraw, right, top + 3, buf); top += 35;
		return 0;
	}
};

// class CWindowShadow
// {
// public:
// 	HWINDOW m_hWindow;
// 	HELE  m_hSlider_shadowSize;      //阴影大小
// 	HELE  m_hSlider_depth;           //阴影深度
// 	HELE  m_hSlider_shadowAngle;     //圆角阴影内收大小
// 	HELE  m_hBtn_Check_rightAnge;  //是否强制直角
// 	HELE  m_hRichEditColor; //阴影颜色
// 
// 	int   m_nShadowSize;
// 	int   m_nShadowDepth; //0-255
// 	int   m_nShadowAngleSize;
// 	BOOL  m_bRightAngle;
// 	COLORREF  m_clrShadow;
// 
// 	CWindowShadow()
// 	{
// 		m_nShadowSize = 10;
// 		m_nShadowDepth = 100;
// 		m_nShadowAngleSize = 10;
// 		m_clrShadow = RGB(255, 0, 0);
// 		m_bRightAngle = FALSE;
// 		//Init();
// 	}
// 	void Init(HWND hParent)
// 	{
// 		//m_hWindow = XWnd_Create(0, 0, 500, 400, L"炫彩界面库窗口", NULL, xc_window_style_default);
// 		m_hWindow = XModalWnd_Create(500, 350, L"系统设置", hParent);
// 		XBtn_SetType(XBtn_Create(150, 80, 200, 25, L"炫彩界面库-窗口阴影演示(关闭)", m_hWindow), button_type_close);
// 
// 		XWnd_EnableDragWindow(m_hWindow, TRUE);
// 		XWnd_SetLayoutSize(m_hWindow, 0, 0, 0, 0);
// 
// 		//	HIMAGE hImage=XImage_LoadFile(L"image\\bk.png");
// 		//	HBKM hBkInfoM= XWnd_GetBkManager(m_hWindow);
// 		//	XBkInfoM_AddImage(hBkInfoM,window_state_flag_leave,hImage);
// 
// 		XWnd_SetShadowInfo(m_hWindow, m_nShadowSize, m_nShadowDepth, m_nShadowAngleSize, m_bRightAngle, m_clrShadow);
// 		int top = 150;
// 		int left = 110;
// 		m_hSlider_shadowSize = XSliderBar_Create(left, top, 300, 20, m_hWindow);
// 		top += 35;
// 		XEle_EnableBkTransparent(m_hSlider_shadowSize, true);
// 		XSliderBar_SetRange(m_hSlider_shadowSize, 100);
// 		XSliderBar_SetPos(m_hSlider_shadowSize, m_nShadowAngleSize);
// 
// 		m_hSlider_depth = XSliderBar_Create(left, top, 300, 20, m_hWindow);
// 		top += 35;
// 		XSliderBar_SetRange(m_hSlider_depth, 255);
// 		XEle_EnableBkTransparent(m_hSlider_depth, TRUE);
// 		XSliderBar_SetPos(m_hSlider_depth, m_nShadowDepth);
// 
// 		m_hSlider_shadowAngle = XSliderBar_Create(left, top, 300, 20, m_hWindow);
// 		top += 35;
// 		XSliderBar_SetRange(m_hSlider_shadowAngle, 255);
// 		XEle_EnableBkTransparent(m_hSlider_shadowAngle, TRUE);
// 		XSliderBar_SetPos(m_hSlider_shadowAngle, m_nShadowAngleSize);
// 
// 		m_hBtn_Check_rightAnge = XBtn_Create(left + 10, top, 100, 20, L" 啊啊啊 ", m_hWindow); top += 35;
// 		XEle_SetTextColor(m_hBtn_Check_rightAnge, RGB(255, 255, 255));
// 		XEle_EnableBkTransparent(m_hBtn_Check_rightAnge, TRUE);
// 		XBtn_SetCheck(m_hBtn_Check_rightAnge, m_bRightAngle);
// 		XBtn_SetTypeEx(m_hBtn_Check_rightAnge, button_type_check);
// 
// 		m_hRichEditColor = XRichEditColor_Create(left + 10, top, 100, 22, m_hWindow);
// 		XEle_SetTextColor(m_hRichEditColor, RGB(255, 255, 255));
// 		XEle_EnableBkTransparent(m_hRichEditColor, TRUE);
// 		XRichEdit_EnableEvent_XE_RICHEDIT_CHANGE(m_hRichEditColor, TRUE);
// 
// 		XWnd_SetTransparentType(m_hWindow, window_transparent_shaped);
// 		XEle_RegEventCPP(m_hSlider_shadowSize, XE_SLIDERBAR_CHANGE, &CWindowShadow::OnSliderBarChange_ShadowSize);
// 		XEle_RegEventCPP(m_hSlider_depth, XE_SLIDERBAR_CHANGE, &CWindowShadow::OnSliderBarChange_ShadowDepth);
// 		XEle_RegEventCPP(m_hSlider_shadowAngle, XE_SLIDERBAR_CHANGE, &CWindowShadow::OnSliderBarChange_ShadowAngleSize);
// 
// 		XEle_RegEventCPP(m_hBtn_Check_rightAnge, XE_BUTTON_CHECK, &CWindowShadow::OnButtonCheck_ShadowRightAngle);
// 		XEle_RegEventCPP1(m_hRichEditColor, XE_RICHEDIT_CHANGE, &CWindowShadow::OnRichEditChange_ShadowColor);
// 
// 		XWnd_RegEventCPP(m_hWindow, WM_PAINT, &CWindowShadow::OnWndDrawWindow);
// 		XWnd_AdjustLayout(m_hWindow);
// 		XWnd_ShowWindow(m_hWindow, SW_SHOW);
// 	}
// 	int OnSliderBarChange_ShadowSize(int pos, BOOL *pbHandled)
// 	{
// 		m_nShadowSize = pos;
// 		XWnd_SetShadowInfo(m_hWindow, m_nShadowSize, m_nShadowDepth, m_nShadowAngleSize, m_bRightAngle, m_clrShadow);
// 		XWnd_RedrawWnd(m_hWindow);
// 		return 0;
// 	}
// 	int OnSliderBarChange_ShadowDepth(int pos, BOOL *pbHandled)
// 	{
// 		m_nShadowDepth = pos;
// 		XWnd_SetShadowInfo(m_hWindow, m_nShadowSize, m_nShadowDepth, m_nShadowAngleSize, m_bRightAngle, m_clrShadow);
// 		XWnd_RedrawWnd(m_hWindow);
// 		return 0;
// 	}
// 	int OnSliderBarChange_ShadowAngleSize(int pos, BOOL *pbHandled)
// 	{
// 		m_nShadowAngleSize = pos;
// 		XWnd_SetShadowInfo(m_hWindow, m_nShadowSize, m_nShadowDepth, m_nShadowAngleSize, m_bRightAngle, m_clrShadow);
// 		XWnd_RedrawWnd(m_hWindow);
// 		return 0;
// 	}
// 	int  OnButtonCheck_ShadowRightAngle(BOOL bCheck, BOOL *pbHandled)
// 	{
// 		m_bRightAngle = bCheck;
// 		XWnd_SetShadowInfo(m_hWindow, m_nShadowSize, m_nShadowDepth, m_nShadowAngleSize, m_bRightAngle, m_clrShadow);
// 		XWnd_RedrawWnd(m_hWindow);
// 		return 0;
// 	}
// 	int  OnRichEditChange_ShadowColor(HELE hRichEditColor, BOOL *pbHandled)
// 	{
// 		m_clrShadow = XRichEditColor_GetColor(hRichEditColor);
// 		XWnd_SetShadowInfo(m_hWindow, m_nShadowSize, m_nShadowDepth, m_nShadowAngleSize, m_bRightAngle, m_clrShadow);
// 		XWnd_RedrawWnd(m_hWindow);
// 		return 0;
// 	}
// 	int  OnWndDrawWindow(HDRAW hDraw, BOOL *pbHandled)
// 	{
// 		*pbHandled = TRUE;
// 		RECT rect;
// 		XWnd_GetClientRect(m_hWindow, &rect);
// 		GetClientRect(XWnd_GetHWND(m_hWindow), &rect);
// 		XDraw_SetBrushColor(hDraw, RGB(250, 250, 250));
// 		XDraw_FillRect(hDraw, &rect);
// 
// 		RECT rcBody;
// 		XWnd_GetBodyRect(m_hWindow, &rcBody);
// 		XDraw_SetBrushColor(hDraw, RGB(200, 0, 0));
// 		XDraw_FillRect(hDraw, &rcBody);
// 
// 		XDraw_SetBrushColor(hDraw, RGB(255, 255, 255));
// 		int top = 150;
// 		int left = 50;
// 		int right = 420;
// 		wchar_t  buf[256] = { 0 };
// 		XDraw_TextOutEx(hDraw, left, top + 3, L"阴影大小:");
// 		wsprintfW(buf, L"%d", m_nShadowSize);
// 		XDraw_TextOutEx(hDraw, right, top + 3, buf); top += 35;
// 
// 		XDraw_TextOutEx(hDraw, left, top + 3, L"阴影深度:");
// 		wsprintfW(buf, L"%d", m_nShadowDepth);
// 		XDraw_TextOutEx(hDraw, right, top + 3, buf); top += 35;
// 
// 		XDraw_TextOutEx(hDraw, left, top + 3, L"角 大 小:");
// 		wsprintfW(buf, L"%d", m_nShadowAngleSize);
// 		XDraw_TextOutEx(hDraw, right, top + 3, buf); top += 35;
// 
// 		XDraw_TextOutEx(hDraw, left, top + 3, L"直    角:");
// 		wsprintfW(buf, L"%d", m_nShadowAngleSize);
// 		XDraw_TextOutEx(hDraw, right, top + 3, buf); top += 35;
// 
// 		XDraw_TextOutEx(hDraw, left, top + 3, L"阴影颜色:");
// 		wsprintfW(buf, L"%d", m_nShadowAngleSize);
// 		XDraw_TextOutEx(hDraw, right, top + 3, buf); top += 35;
// 		return 0;
// 	}
// };

