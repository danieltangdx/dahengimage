// DlgExit.cpp : implementation file
//

#include "stdafx.h"
#include "DemoProject.h"
#include "DlgExit.h"
#include "DemoProjectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgExit dialog

extern HMODULE g_hLanguage;
extern pFunc_GetTextFromLPK g_pGetTextFromLPK;
extern BOOL g_bSaveEnabled;

CDlgExit::CDlgExit(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgExit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgExit)
	//}}AFX_DATA_INIT

	m_nClass = 1;
	m_nFlying1 = 0;
	m_nFlying2 = 2;
	m_bCancelEnable = TRUE;
}


void CDlgExit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgExit)
//	DDX_Control(pDX, IDC_STATIC_FLY1, m_cPic1);
//	DDX_Control(pDX, IDC_STATIC_FLY2, m_cPic2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgExit, CDialog)
	//{{AFX_MSG_MAP(CDlgExit)
	ON_BN_CLICKED(IDC_BTN_LANDE, OnBtnLAndE)
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgExit message handlers

void CDlgExit::OnBtnLAndE() 
{
	// TODO: Add your control notification handler code here
	EndDialog(1);
}

void CDlgExit::OnBtnExit() 
{
	// TODO: Add your control notification handler code here
	EndDialog(2);
}

void CDlgExit::OnBtnCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

BOOL CDlgExit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	char chText[256];
	
	if (m_nClass == 0)
	{
		LoadString(g_hLanguage, 5043, chText, 256);
		GetDlgItem(IDC_BTN_LANDE)->EnableWindow(g_bSaveEnabled);
	}
	else
	{
		LoadString(g_hLanguage, 5042, chText, 256);
	}
	
	GetDlgItem(IDC_BTN_LANDE)->SetWindowText(chText);
	if (m_nClass==1 && m_bLoadDisable)
		GetDlgItem(IDC_BTN_LANDE)->EnableWindow(FALSE);

	LoadString(g_hLanguage, 5044, chText, 256);
	GetDlgItem(IDC_BTN_EXIT)->SetWindowText(chText);

	LoadString(g_hLanguage, 5045, chText, 256);
	GetDlgItem(IDC_BTN_CANCEL)->SetWindowText(chText);
	GetDlgItem(IDC_BTN_CANCEL)->EnableWindow(m_bCancelEnable);
	GetDlgItem(IDC_BTN_CANCEL)->ShowWindow(m_bCancelEnable*5);

// 	m_pHIcon = new HICON[8];
// 	m_pHIcon[0] = AfxGetApp()->LoadIcon(IDI_ICON_FLY1);
// 	m_pHIcon[1] = AfxGetApp()->LoadIcon(IDI_ICON_FLY2);
// 	m_pHIcon[2] = AfxGetApp()->LoadIcon(IDI_ICON_FLY3);
// 	m_pHIcon[3] = AfxGetApp()->LoadIcon(IDI_ICON_FLY4);
// 	m_pHIcon[4] = AfxGetApp()->LoadIcon(IDI_ICON_FLY5);
// 	m_pHIcon[5] = AfxGetApp()->LoadIcon(IDI_ICON_FLY6);
// 	m_pHIcon[6] = AfxGetApp()->LoadIcon(IDI_ICON_FLY7);
// 	m_pHIcon[7] = AfxGetApp()->LoadIcon(IDI_ICON_FLY8);
// 
// 	m_brush.CreateSolidBrush(RGB(255,255,255));


// 	CRect rect;
// 	GetDlgItem(IDC_STATIC_FLY2)->GetWindowRect(rect);
// 	ScreenToClient(rect);
// 	m_nRightLimit = rect.left;
// 	m_nBottomLimit = rect.top;
// 
// 	m_uTimer1 = SetTimer(1, 50, NULL);
// 	m_uTimer2 = SetTimer(2, 50, NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgExit::OnTimer(UINT nIDEvent) 
{
// 	int nStep = 8;
// 	CString strWrite;
// 	if (nIDEvent == 1)
// 	{
// 		if (m_nFlying1 == 0) // Right
// 		{
// 			m_cPic1.SetIcon(m_pHIcon[2]);
// 			CRect rect;
// 			m_cPic1.GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			rect.left += nStep;
// 			rect.right += nStep;
// 			if (rect.left >= m_nRightLimit)
// 			{
// 				m_nFlying1 = 1;	
// 				m_cPic1.SetIcon(m_pHIcon[3]);
// 			}
// 			m_cPic1.MoveWindow(rect);
// 			strWrite.Format("left:%d\r\n", rect.left);
// 		}
// 		else if (m_nFlying1 == 1) // Down
// 		{
// 			m_cPic1.SetIcon(m_pHIcon[4]);
// 			CRect rect;
// 			m_cPic1.GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			rect.top += nStep;
// 			rect.bottom += nStep;
// 			if (rect.top >= m_nBottomLimit)
// 			{
// 				m_nFlying1 = 2;
// 				m_cPic1.SetIcon(m_pHIcon[5]);
// 			}
// 			m_cPic1.MoveWindow(rect);
// 			strWrite.Format("top:%d\r\n", rect.top);
// 		}
// 		else if (m_nFlying1 == 2) // Left
// 		{
// 			m_cPic1.SetIcon(m_pHIcon[6]);
// 			CRect rect;
// 			m_cPic1.GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			rect.left -= nStep;
// 			rect.right -= nStep;
// 			if (rect.left <= 2)
// 			{
// 				m_nFlying1 = 3;
// 				m_cPic1.SetIcon(m_pHIcon[7]);
// 			}
// 			m_cPic1.MoveWindow(rect);
// 			strWrite.Format("left:%d\r\n", rect.left);
// 		}
// 		else if (m_nFlying1 == 3) // Up
// 		{
// 			m_cPic1.SetIcon(m_pHIcon[0]);
// 			CRect rect;
// 			m_cPic1.GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			rect.top -= nStep;
// 			rect.bottom -= nStep;
// 			if (rect.top <= 2)
// 			{
// 				m_nFlying1 = 0;
// 				m_cPic1.SetIcon(m_pHIcon[1]);
// 			}
// 			m_cPic1.MoveWindow(rect);
// 			strWrite.Format("top:%d\r\n", rect.top);
// 		}
// 	}
// 	
// 	if (nIDEvent == 2)
// 	{
// 		if (m_nFlying2 == 0) // Right
// 		{
// 			m_cPic2.SetIcon(m_pHIcon[2]);
// 			CRect rect;
// 			m_cPic2.GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			rect.left += nStep;
// 			rect.right += nStep;
// 			if (rect.left >= m_nRightLimit)
// 			{
// 				m_nFlying2 = 1;	
// 				m_cPic2.SetIcon(m_pHIcon[3]);
// 			}
// 			m_cPic2.MoveWindow(rect);
// 			
// 		}
// 		else if (m_nFlying2 == 1) // Down
// 		{
// 			m_cPic2.SetIcon(m_pHIcon[4]);
// 			CRect rect;
// 			m_cPic2.GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			rect.top += nStep;
// 			rect.bottom += nStep;
// 			if (rect.top >= m_nBottomLimit)
// 			{
// 				m_nFlying2 = 2;
// 				m_cPic2.SetIcon(m_pHIcon[5]);
// 			}
// 			m_cPic2.MoveWindow(rect);
// 		}
// 		else if (m_nFlying2 == 2) // Left
// 		{
// 			m_cPic2.SetIcon(m_pHIcon[6]);
// 			CRect rect;
// 			m_cPic2.GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			rect.left -= nStep;
// 			rect.right -= nStep;
// 			if (rect.left <= 2)
// 			{
// 				m_nFlying2 = 3;
// 				m_cPic2.SetIcon(m_pHIcon[7]);
// 			}
// 			m_cPic2.MoveWindow(rect);
// 		}
// 		else if (m_nFlying2 == 3) // Up
// 		{
// 			m_cPic2.SetIcon(m_pHIcon[0]);
// 			CRect rect;
// 			m_cPic2.GetWindowRect(rect);
// 			ScreenToClient(rect);
// 			rect.top -= nStep;
// 			rect.bottom -= nStep;
// 			if (rect.top <= 2)
// 			{
// 				m_nFlying2 = 0;
// 				m_cPic2.SetIcon(m_pHIcon[1]);
// 			}
// 			m_cPic2.MoveWindow(rect);
// 		}
// 	}

	CDialog::OnTimer(nIDEvent);
}

HBRUSH CDlgExit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
