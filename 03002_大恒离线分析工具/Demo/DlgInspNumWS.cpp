// DlgInspNumWS.cpp : implementation file
//

#include "stdafx.h"
#include "DemoProject.h"
#include "DlgInspNumWS.h"
#include "DemoProjectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInspNumWS dialog

extern HMODULE g_hLanguage;
extern pFunc_GetTextFromLPK g_pGetTextFromLPK;

CDlgInspNumWS::CDlgInspNumWS(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInspNumWS::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInspNumWS)
	m_nNumWS = 0;
	//}}AFX_DATA_INIT
}


void CDlgInspNumWS::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInspNumWS)
	DDX_Text(pDX, IDC_EDIT_NUMWS, m_nNumWS);
	DDV_MinMaxInt(pDX, m_nNumWS, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInspNumWS, CDialog)
	//{{AFX_MSG_MAP(CDlgInspNumWS)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInspNumWS message handlers

void CDlgInspNumWS::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	if (m_nNumWS <= 0)
	{
		char chText[256];
		LoadString(g_hLanguage, 5037, chText, 256);
		AfxMessageBox(chText);
		return;
	}
	CDialog::OnOK();
}

BOOL CDlgInspNumWS::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(FALSE);
	
	char chText[256];
	LoadString(g_hLanguage, 5055, chText, 256);
	GetDlgItem(IDC_STATIC_1)->SetWindowText(chText);
	LoadString(g_hLanguage, 5056, chText, 256);
	GetDlgItem(IDOK)->SetWindowText(chText);
	LoadString(g_hLanguage, 5044, chText, 256);
	GetDlgItem(IDCANCEL)->SetWindowText(chText);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
