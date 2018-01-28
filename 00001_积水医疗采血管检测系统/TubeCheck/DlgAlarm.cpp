// DlgAlarm.cpp : implementation file
//

#include "stdafx.h"
#include "TubeCheck.h"
#include "DlgAlarm.h"
#include "MainFrm.h"
#include "TubeCheckVIEW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainFrame*			g_pFrame; //全局指针
extern CTubeCheckView*     g_pView;


/////////////////////////////////////////////////////////////////////////////
// CDlgAlarm dialog


CDlgAlarm::CDlgAlarm(CWnd* pParent /*=NULL*/, CString sErrorInfo)
	: CDialog(CDlgAlarm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAlarm)
	//}}AFX_DATA_INIT
	m_sErrorInfo = sErrorInfo;
}


void CDlgAlarm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAlarm)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAlarm, CDialog)
	//{{AFX_MSG_MAP(CDlgAlarm)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAlarm message handlers

BOOL CDlgAlarm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_fontAlarm.CreateFont(
		22,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Arial");  

	GetDlgItem(IDC_ALARM_INFO)->SetFont(&m_fontAlarm);
	SetDlgItemText(IDC_ALARM_INFO, m_sErrorInfo);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgAlarm::OnOK() 
{
	// TODO: Add extra validation here
	
// 	g_pFrame->m_bLabelAlarm[Camera2] = FALSE;
// 	g_pView->m_bLabelAlarm[Camera2] = FALSE;

	// 停止报警并隐藏对话框
	g_pView->m_pdlgAlarm->ShowWindow(SW_HIDE);
	g_pFrame->AlarmAction(FALSE);


	CDialog::OnOK();
}

HBRUSH CDlgAlarm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here

	switch(pWnd->GetDlgCtrlID())
	{
	case IDC_ALARM_INFO:
		{
			pDC->SetTextColor(RGB(255,0,0));
			pDC->SetBkMode(TRANSPARENT);
			break;
		}
	default:
		break;
		
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CDlgAlarm::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
//	delete this;
	CDialog::PostNcDestroy();
}
