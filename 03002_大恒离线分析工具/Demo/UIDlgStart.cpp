// UIDlgStart.cpp : implementation file
//

#include "stdafx.h"
#include "DemoProject.h"
#include "UIDlgStart.h"
#include "DemoProjectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern int g_nPainted;
extern CDemoProjectApp theApp;
extern HMODULE g_hLanguage;
extern pFunc_GetTextFromLPK g_pGetTextFromLPK;
/////////////////////////////////////////////////////////////////////////////
// CUIDlgStart dialog

CUIDlgStart::CUIDlgStart(CWnd* pParent /*=NULL*/)
	: CDialog(CUIDlgStart::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUIDlgStart)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nCurPos = 0;
	m_nNextLimit = 19;
	m_nTransparent = 40;
}


void CUIDlgStart::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUIDlgStart)
	DDX_Control(pDX, IDC_STATIC_LOADING, m_cStaticLoading);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUIDlgStart, CDialog)
	//{{AFX_MSG_MAP(CUIDlgStart)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUIDlgStart message handlers

BOOL CUIDlgStart::OnInitDialog() 
{
	CDialog::OnInitDialog();

// 	m_nTransparent = 0;
// 	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
// 	HINSTANCE hInst = LoadLibrary("User32.DLL"); 	
// 	if(hInst) 
// 	{
// 		m_fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
// 		if (m_fun)
// 			m_uTimerT = SetTimer(1, 5, NULL);
// 	}
// 	FreeLibrary(hInst);
//	m_uTimerR = SetTimer(2, 1, NULL);
//	m_uTimerD = SetTimer(3, 10, NULL);

//	m_cProgLoad.SetRange(0, 100);
//	m_cProgLoad.SetStep(1);
	char chText[256];
	LoadString(g_hLanguage, 5067, chText, 256);
	GetDlgItem(IDC_STATIC_LOADING)->SetWindowText(chText);
	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);
	SetTimer(4, 50, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUIDlgStart::OnTimer(UINT nIDEvent) 
{
// 	if (nIDEvent == 1)
// 	{
// 		m_fun(this->GetSafeHwnd(), 0, m_nTransparent, 2);
// 		m_nTransparent+=5;
// 		if (m_nTransparent > 255)
// 		{	
// 			KillTimer(m_uTimerT);
// 		}
// 	}
// 	if (nIDEvent == 2)
// 	{
// 		if (m_nCurPos == 100)
// 		{
// 			CString strTitle;
// 			strTitle.Format("初始化中 请稍后.....  %d%", m_nCurPos);
// 			m_cStaticLoading.SetWindowText(strTitle);
// 			KillTimer(m_uTimerR);
// 		}
// 		else
// 		{
// 			int nTemp = m_nCurPos + 1;
// 			if (nTemp <= m_nNextLimit)
// 			{
// 				m_nCurPos++;
// 				m_cProgLoad.SetPos(m_nCurPos);
// 				CString strTitle;
// 				strTitle.Format("初始化中 请稍后.....  %d%", m_nCurPos);
// 				m_cStaticLoading.SetWindowText(strTitle);
// 			}	
// 		}
// 	}
// 	if (nIDEvent == 3)
// 	{
// 		KillTimer(m_uTimerD);
// 	}
	if (nIDEvent == 4)
	{
		if (g_nPainted == 2)
		{
			KillTimer(4);
			SetTimer(5, 10, NULL);
//			this->ShowWindow(SW_HIDE);
//			EndDialog(IDOK);
		}
		if (m_nTransparent <= 255)
		{
			HINSTANCE hInst = LoadLibrary("User32.DLL"); 	
			if(hInst) 
			{
				typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD);
				MYFUNC fun = NULL;
				fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
				if(fun)fun(this->GetSafeHwnd(),0,m_nTransparent,2);
				FreeLibrary(hInst);
				m_nTransparent += 10;
			}
		}
	}
	if (nIDEvent == 5)
	{
		if (m_nTransparent >= 10)
		{
			HINSTANCE hInst = LoadLibrary("User32.DLL"); 	
			if(hInst) 
			{
				typedef BOOL (WINAPI *MYFUNC)(HWND,COLORREF,BYTE,DWORD);
				MYFUNC fun = NULL;
				fun=(MYFUNC)GetProcAddress(hInst, "SetLayeredWindowAttributes");
				if(fun)fun(this->GetSafeHwnd(),0,m_nTransparent,2);
				FreeLibrary(hInst);
				m_nTransparent -= 10;
			}
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}