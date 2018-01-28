// DlgIOCard.cpp : implementation file
//

#include "stdafx.h"
#include "TubeCheck.h"
#include "DlgIOCard.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgIOCard dialog


CDlgIOCard::CDlgIOCard(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgIOCard::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgIOCard)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgIOCard::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgIOCard)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgIOCard, CDialog)
	//{{AFX_MSG_MAP(CDlgIOCard)
	ON_BN_CLICKED(IDC_BTN_CARD1, OnBtnCard1)
	ON_BN_CLICKED(IDC_BTN_CARD2, OnBtnCard2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgIOCard message handlers

void CDlgIOCard::OnBtnCard1() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	if(TRUE==pFrame->m_bIOCardValid)
	{
		pFrame->m_IOCard[Camera1].Show_PIO24B_DebugDialog(NULL);
	}
	else
	{
		AfxMessageBox("无法打开卡1，请确定有无接口卡!");
	}
}

void CDlgIOCard::OnBtnCard2() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	
	if(TRUE==pFrame->m_bIOCardValid)
	{
		pFrame->m_IOCard[Camera2].Show_PIO24B_DebugDialog(NULL);
	}
	else
	{
		AfxMessageBox("无法打开卡2，请确定有无接口卡!");
	}

}
