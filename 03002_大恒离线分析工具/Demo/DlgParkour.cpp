// DlgParkour.cpp : implementation file
//

#include "stdafx.h"
#include "DemoProject.h"
#include "DlgParkour.h"
#include "DemoProjectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgParkour dialog

extern HMODULE g_hLanguage;
extern pFunc_GetTextFromLPK g_pGetTextFromLPK;

CDlgParkour::CDlgParkour(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgParkour::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgParkour)
	m_nInterval = 10;
	m_bStopGood = FALSE;
	m_bStopExp = FALSE;
	m_bStopEmp = FALSE;
	m_bStopBad = FALSE;
	m_bUnlimLoop = FALSE;
	m_bFromSel = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgParkour::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgParkour)
	DDX_Text(pDX, IDC_EDIT_INTERVAL, m_nInterval);
	DDX_Check(pDX, IDC_CHECK_STOPGOOD, m_bStopGood);
	DDX_Check(pDX, IDC_CHECK_STOPEXP, m_bStopExp);
	DDX_Check(pDX, IDC_CHECK_STOPEMP, m_bStopEmp);
	DDX_Check(pDX, IDC_CHECK_STOPBAD, m_bStopBad);
	DDX_Check(pDX, IDC_CHECK_LOOP, m_bUnlimLoop);
	DDX_Check(pDX, IDC_CHECK_FROMSEL, m_bFromSel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgParkour, CDialog)
	//{{AFX_MSG_MAP(CDlgParkour)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgParkour message handlers

void CDlgParkour::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if (m_nInterval < 0)
	{
		AfxMessageBox("¼ä¸ôÉèÖÃÓÐÎó£¡");
		return;
	}
	CDialog::OnOK();
}

BOOL CDlgParkour::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	char chText[256];

	LoadString(g_hLanguage, 5058, chText, 256);
	GetDlgItem(IDC_STATIC_1)->SetWindowText(chText);
	LoadString(g_hLanguage, 5059, chText, 256);
	GetDlgItem(IDC_CHECK_FROMSEL)->SetWindowText(chText);
	LoadString(g_hLanguage, 5060, chText, 256);
	GetDlgItem(IDC_CHECK_LOOP)->SetWindowText(chText);
	LoadString(g_hLanguage, 5061, chText, 256);
	GetDlgItem(IDC_CHECK_STOPBAD)->SetWindowText(chText);
	LoadString(g_hLanguage, 5062, chText, 256);
	GetDlgItem(IDC_CHECK_STOPGOOD)->SetWindowText(chText);
	LoadString(g_hLanguage, 5063, chText, 256);
	GetDlgItem(IDC_CHECK_STOPEXP)->SetWindowText(chText);
	LoadString(g_hLanguage, 5064, chText, 256);
	GetDlgItem(IDC_CHECK_STOPEMP)->SetWindowText(chText);
	LoadString(g_hLanguage, 5065, chText, 256);
	GetDlgItem(IDOK)->SetWindowText(chText);
	LoadString(g_hLanguage, 5045, chText, 256);
	GetDlgItem(IDCANCEL)->SetWindowText(chText);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
