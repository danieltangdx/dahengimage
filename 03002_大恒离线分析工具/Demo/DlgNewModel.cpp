// DlgNewModel.cpp : implementation file
//

#include "stdafx.h"
#include "DemoProject.h"
#include "DlgNewModel.h"
#include "DemoProjectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgNewModel dialog
extern HMODULE g_hLanguage;
extern pFunc_GetTextFromLPK g_pGetTextFromLPK;

CDlgNewModel::CDlgNewModel(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNewModel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNewModel)
	m_strNewModel = _T("");
	//}}AFX_DATA_INIT
}


void CDlgNewModel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNewModel)
	DDX_Text(pDX, IDC_EDIT_NAME, m_strNewModel);
	DDV_MaxChars(pDX, m_strNewModel, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNewModel, CDialog)
	//{{AFX_MSG_MAP(CDlgNewModel)
	ON_BN_CLICKED(IDC_BTN_CONFIRM, OnBtnConfirm)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNewModel message handlers

void CDlgNewModel::OnBtnConfirm() 
{
	UpdateData(TRUE);
	EndDialog(1);
}

BOOL CDlgNewModel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	char chText[256];
	LoadString(g_hLanguage, 5046, chText, 256);
	GetDlgItem(IDC_STATIC_N)->SetWindowText(chText);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
