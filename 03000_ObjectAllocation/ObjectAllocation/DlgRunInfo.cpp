// DlgRunInfo.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectAllocation.h"
#include "DlgRunInfo.h"
#include "afxdialogex.h"


// CDlgRunInfo dialog

IMPLEMENT_DYNAMIC(CDlgRunInfo, CDialogEx)

CDlgRunInfo::CDlgRunInfo(CWnd* pParent /*=NULL*/, bool bEnableXCGUI)
	: CDialogEx(IDD_DIALOG_RunInfo, pParent)
{
	m_bEnableXCGUI = bEnableXCGUI;
}


CDlgRunInfo::~CDlgRunInfo()
{
}

void CDlgRunInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_RunInfo, m_listRunInfo);
	DDX_Control(pDX, IDC_EDIT_RunInfo, m_editRunInfo);
}


BEGIN_MESSAGE_MAP(CDlgRunInfo, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgRunInfo message handlers


BOOL CDlgRunInfo::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	if (!m_bEnableXCGUI)
		return true;
	m_sAppPath = CExFn::GetCurrentAppPath();
	m_wsResPath = CStringConvert::ANSI_UCS2(m_sAppPath.GetBuffer()) + L"/resource/xml/";
	bool bRet{ false }, bRet1{ false }, bRet2{false};

	if (!PathFileExistsW(m_wsResPath.c_str()))
		return true;
	wstring wsResFilePath = m_wsResPath + L"resource.res";
	if (!PathFileExistsW(wsResFilePath.c_str()))
		return true;
	bRet = XC_LoadResource(wsResFilePath.c_str());
	wsResFilePath = m_wsResPath + L"resource1.res";
	if (!PathFileExistsW(wsResFilePath.c_str()))
		return true;
	bRet1 = XC_LoadResource(wsResFilePath.c_str());
	wsResFilePath = m_wsResPath + L"resource2.res";
	if (!PathFileExistsW(wsResFilePath.c_str()))
		return true;
	bRet2 = XC_LoadResource(wsResFilePath.c_str());
	if (!bRet||!bRet1|!bRet2)
		return true;
	m_bOpenXCWnd = true;
//	bRet = XC_LoadStyle((m_wsResPath + L"style1.css").c_str());
	//bRet = XC_LoadStyle((m_wsResPath + L"style2.css").c_str());
	//m_hWindow = XWnd_Create(0, 0, 500, 400, L"炫彩界面库窗口", m_hWnd, xc_window_style_default);
	//m_hWindow = XWnd_CreateEx(NULL, NULL, NULL, WS_CHILD | WS_VISIBLE, 0, 0, 600, 350, m_hWnd, xc_window_style_default);
	m_hWindow = (HWINDOW)XC_LoadLayout((m_wsResPath + L"/XRunInfo.xml").c_str());
	m_hShapeTextRunInfo = XC_GetObjectByID(m_hWindow, 1001);
	m_hBtnOK = (HELE)XC_GetObjectByID(m_hWindow, 106);
	XEle_RegEventCPP(m_hBtnOK, XE_BNCLICK, &CDlgRunInfo::OnEventBtnClick);
	::SetWindowPos(XWnd_GetHWND(m_hWindow), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	CExFn::ShowInTaskbar(XWnd_GetHWND(m_hWindow), FALSE);
	XWnd_AdjustLayout(m_hWindow);

	//::SetParent(XWnd_GetHWND(m_hWindow), m_hWnd);
	//XWnd_ShowWindow(m_hWindow, SW_SHOW);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgRunInfo::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages

	CRect rect;
	GetClientRect(&rect);
	m_editRunInfo.MoveWindow(rect, TRUE);
// 	if (NULL != m_hWindow)
// 		::MoveWindow(XWnd_GetHWND(m_hWindow), 0, 0, rect.Width(), rect.Height(), true);
}

void CDlgRunInfo::UpdateRunInfo(CString & sInfo)
{
	
	if(m_bOpenXCWnd)
	{
		XShapeText_SetText(m_hShapeTextRunInfo, CStringConvert::ANSI_UCS2(sInfo.GetBuffer()).c_str());
		XWnd_RedrawWnd(m_hWindow);
	}
	else
	{
		m_editRunInfo.SetWindowText(sInfo);
	}
//	UpdateData(FALSE);
}

// CString CDlgRunInfo::GetCurrentAppPath()
// {
// 	char chFile[MAX_PATH]{0}, chDrive[MAX_PATH], chDir[_MAX_DIR], chFname[_MAX_FNAME], chExt[_MAX_EXT];
// 	CString AppPath;
// 	GetModuleFileName(NULL, chFile, sizeof(chFile));
// 	_splitpath(chFile, chDrive, chDir, chFname, chExt);
// 	strcat(chDrive, chDir);
// 	AppPath = chDrive;
// 	return AppPath;
// }

int CDlgRunInfo::OnEventBtnClick(BOOL *pBool) //按钮点击事件响应
{
	//弹出消息框
	//MessageBoxW(XWnd_GetHWND(m_hWindow), L"你点击了按钮", L"提示", MB_OK);
	XWnd_ShowWindow(m_hWindow, SW_HIDE);
	*pBool = TRUE; //对该事件进行拦截
	return 0;    //事件的返回值
}

void CDlgRunInfo::ShowWindowEx(int iShowMode)
{
	if (m_bOpenXCWnd)
	{
		XWnd_ShowWindow(m_hWindow, iShowMode);
	}
	else
	{
		ShowWindow(iShowMode);
	}
}