// DlgCamTest.cpp : implementation file
//

#include "stdafx.h"
#include "BottleCap.h"
#include "DlgCamTest.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgCamTest dialog


CDlgCamTest::CDlgCamTest(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgCamTest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgCamTest)
	m_iCameraSN = -1;
	//}}AFX_DATA_INIT

	m_iCamLevel=0;
}


void CDlgCamTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgCamTest)
	DDX_Radio(pDX, IDC_RADIO_CAM1, m_iCameraSN);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgCamTest, CDialog)
	//{{AFX_MSG_MAP(CDlgCamTest)
	ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgCamTest message handlers

void CDlgCamTest::OnBtnStart() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_bIsDetecting=FALSE;

	UpdateData(TRUE);

	if(-1==m_iCameraSN)
	{
		AfxMessageBox("请选择需调试的相机!");
		return;
	}
	SetTimer(CameraTest,60,NULL);

	pFrame->m_iCamTestMode=1;
	pFrame->m_bIsDetecting=TRUE;


}

void CDlgCamTest::OnBtnStop() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	KillTimer(CameraTest);
	pFrame->m_iCamTestMode=0;

	pFrame->m_bIsDetecting=FALSE;
}

void CDlgCamTest::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	KillTimer(CameraTest);
	pFrame->m_bIsDetecting=FALSE;
	pFrame->SoftResetIOCard(pFrame->m_sModelName);

	CDialog::OnClose();
}

BOOL CDlgCamTest::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();	
	m_iCameraSN=0;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgCamTest::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	if(nIDEvent==CameraTest)
	{
		if(0==m_iCamLevel)
		{
			if(TRUE==pFrame->m_bIOCardValid)
			{
				if(e_IOCardType_PIOC == pFrame->m_eIOCardType)
					pFrame->m_IOCard[pFrame->m_iCamCardNum[m_iCameraSN]-1].setCardOutput(pFrame->m_iCamOutPort[m_iCameraSN],TRUE);
				else if (e_IOCardType_NewCard == pFrame->m_eIOCardType)
				{
					pFrame->m_NewIOCard.SetOutIOStatus(pFrame->m_iCamOutPort[m_iCameraSN], TRUE);
				}
			}
			m_iCamLevel=1;
		}
		else
		{
			if(TRUE==pFrame->m_bIOCardValid)
			{
				if (e_IOCardType_PIOC == pFrame->m_eIOCardType)
					pFrame->m_IOCard[pFrame->m_iCamCardNum[m_iCameraSN] - 1].setCardOutput(pFrame->m_iCamOutPort[m_iCameraSN], FALSE);
				else if (e_IOCardType_NewCard == pFrame->m_eIOCardType)
				{
					pFrame->m_NewIOCard.SetOutIOStatus(pFrame->m_iCamOutPort[m_iCameraSN], FALSE);
				}
			}
			m_iCamLevel=0;
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}
