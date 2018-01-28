// DlgSystemConfig.cpp : implementation file
//

#include "stdafx.h"
#include "BottleCap.h"
#include "MainFrm.h"
#include "DlgSystemConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSystemConfig dialog


CDlgSystemConfig::CDlgSystemConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSystemConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSystemConfig)
	m_dBlue = 0;
	m_dGreen = 0;
	m_dRed = 0;
	m_bKick1 = FALSE;
	m_bBayer = FALSE;
	m_iMaxSaveImageCount = 0;
	m_iShowTime = 0;
	m_bKick2 = FALSE;
	m_bBayer2 = FALSE;
	m_dBlue2 = 0.0;
	m_dGreen2 = 0.0;
	m_dRed2 = 0.0;
	m_dRed3 = 0.0;
	m_dGreen3 = 0.0;
	m_dBlue3 = 0.0;
	m_bKick3 = FALSE;
	m_bBayer3 = FALSE;
	//}}AFX_DATA_INIT

	m_iShowMode = -1;

}


void CDlgSystemConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSystemConfig)
	DDX_Text(pDX, IDC_EDIT_BLUE, m_dBlue);
	DDX_Text(pDX, IDC_EDIT_GREEN, m_dGreen);
	DDX_Text(pDX, IDC_EDIT_RED, m_dRed);
	DDX_Check(pDX, IDC_CHECK_KICK1, m_bKick1);
	DDX_Check(pDX, IDC_CHECK_BAYER, m_bBayer);
	DDX_Text(pDX, IDC_EDIT_SAVE_IMAGE_COUNT, m_iMaxSaveImageCount);
	DDX_Text(pDX, IDC_EDIT_SHOW_TIME, m_iShowTime);
	DDX_Check(pDX, IDC_CHECK_KICK2, m_bKick2);
	DDX_Check(pDX, IDC_CHECK_BAYER2, m_bBayer2);
	DDX_Text(pDX, IDC_EDIT_BLUE2, m_dBlue2);
	DDX_Text(pDX, IDC_EDIT_GREEN2, m_dGreen2);
	DDX_Text(pDX, IDC_EDIT_RED2, m_dRed2);
	DDX_Text(pDX, IDC_EDIT_RED3, m_dRed3);
	DDX_Text(pDX, IDC_EDIT_GREEN3, m_dGreen3);
	DDX_Text(pDX, IDC_EDIT_BLUE3, m_dBlue3);
	DDX_Check(pDX, IDC_CHECK_KICK3, m_bKick3);
	DDX_Check(pDX, IDC_CHECK_BAYER3, m_bBayer3);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_RADIO1, m_iShowMode);
}


BEGIN_MESSAGE_MAP(CDlgSystemConfig, CDialog)
	//{{AFX_MSG_MAP(CDlgSystemConfig)
	ON_BN_CLICKED(IDC_RADIO_KICK_ALL, OnRadioKickAll)
	ON_BN_CLICKED(IDC_RADIO_KICK_NO, OnRadioKickNo)
	ON_BN_CLICKED(IDC_RADIO_KICK_DIFF, OnRadioKickDiff)
	ON_BN_CLICKED(IDC_RADIO_KICK_NORMAL, OnRadioKickNormal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSystemConfig message handlers

BOOL CDlgSystemConfig::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	m_iShowTime= pFrame->m_iIntervalDisplayTime;
	m_iShowMode= pFrame->m_iImageShowMode;
	m_iMaxSaveImageCount=pFrame->m_iMaxSaveImageCount;

	m_bBayer = pFrame->m_BayerTransform[Camera1].m_bAreaCamBayer;
	m_dRed = pFrame->m_BayerTransform[Camera1].m_dBayerRedGain;
	m_dGreen= pFrame->m_BayerTransform[Camera1].m_dBayerGreenGain;
	m_dBlue = pFrame->m_BayerTransform[Camera1].m_dBayerBlueGain;

	m_bBayer2 = pFrame->m_BayerTransform[Camera2].m_bAreaCamBayer;
	m_dRed2 = pFrame->m_BayerTransform[Camera2].m_dBayerRedGain;
	m_dGreen2= pFrame->m_BayerTransform[Camera2].m_dBayerGreenGain;
	m_dBlue2 = pFrame->m_BayerTransform[Camera2].m_dBayerBlueGain;

	m_bBayer3 = pFrame->m_BayerTransform[Camera2].m_bAreaCamBayer;
	m_dRed3 = pFrame->m_BayerTransform[Camera2].m_dBayerRedGain;
	m_dGreen3= pFrame->m_BayerTransform[Camera2].m_dBayerGreenGain;
	m_dBlue3 = pFrame->m_BayerTransform[Camera2].m_dBayerBlueGain;


	m_bKick1 = pFrame->m_bIsKick[Camera1];
	m_bKick2 = pFrame->m_bIsKick[Camera2];
	m_bKick3 = pFrame->m_bIsKick[Camera3];

	m_iKickMode = pFrame->m_iKickMode;

	if(0==m_iKickMode)
	{
		((CButton*)GetDlgItem(IDC_RADIO_KICK_ALL))->SetCheck(1);
	}
	else if(1==m_iKickMode)
	{
		((CButton*)GetDlgItem(IDC_RADIO_KICK_NO))->SetCheck(1);
	}
	else if(2==m_iKickMode)
	{
		((CButton*)GetDlgItem(IDC_RADIO_KICK_DIFF))->SetCheck(1);
	}
	else if(3==m_iKickMode)
	{
		((CButton*)GetDlgItem(IDC_RADIO_KICK_NORMAL))->SetCheck(1);
	}


	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSystemConfig::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_iImageShowMode = m_iShowMode;
	UpdateData(FALSE);
	CDialog::OnOK();
}



void CDlgSystemConfig::OnRadioKickAll() 
{
	// TODO: Add your control notification handler code here
	m_iKickMode=0;
}

void CDlgSystemConfig::OnRadioKickNo() 
{
	// TODO: Add your control notification handler code here
	m_iKickMode=1;
}

void CDlgSystemConfig::OnRadioKickDiff() 
{
	// TODO: Add your control notification handler code here
	m_iKickMode=2;
}

void CDlgSystemConfig::OnRadioKickNormal() 
{
	// TODO: Add your control notification handler code here
	m_iKickMode=3;
}


