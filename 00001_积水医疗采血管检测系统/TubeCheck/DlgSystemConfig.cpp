// DlgSystemConfig.cpp : implementation file
//

#include "stdafx.h"
#include "TubeCheck.h"
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
	, m_iMaxNumOfConstantKick(0)
	, m_iLabelQueLen(0)
{
	//{{AFX_DATA_INIT(CDlgSystemConfig)
	m_dBlue = 0;
	m_dGreen = 0;
	m_dRed = 0;
	m_bKick1 = FALSE;
	m_bBayer = FALSE;
	//m_iMaxSaveImageCount = 0;
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
	m_bBayer4 = FALSE;
	m_bBayer5 = FALSE;
	m_bBayer6 = FALSE;
	m_bKick4 = FALSE;
	m_bKick5 = FALSE;
	m_bKick6 = FALSE;
	m_dBlue4 = 0.0;
	m_dBlue5 = 0.0;
	m_dBlue6 = 0.0;
	m_dGreen4 = 0.0;
	m_dGreen5 = 0.0;
	m_dGreen6 = 0.0;
	m_dRed4 = 0.0;
	m_dRed5 = 0.0;
	m_dRed6 = 0.0;
	m_bSaveImageSys = FALSE;
	//}}AFX_DATA_INIT

	m_iShowMode = -1;
	m_iShowDevice = 0;
}


void CDlgSystemConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSystemConfig)
	// 	DDX_Text(pDX, IDC_EDIT_BLUE, m_dBlue);
	// 	DDX_Text(pDX, IDC_EDIT_GREEN, m_dGreen);
	// 	DDX_Text(pDX, IDC_EDIT_RED, m_dRed);
	DDX_Check(pDX, IDC_CHECK_KICK1, m_bKick1);
	//	DDX_Check(pDX, IDC_CHECK_BAYER, m_bBayer);
	DDX_Text(pDX, IDC_EDIT_SHOW_TIME, m_iShowTime);
	DDX_Check(pDX, IDC_CHECK_KICK2, m_bKick2);
	// 	DDX_Check(pDX, IDC_CHECK_BAYER2, m_bBayer2);
	// 	DDX_Text(pDX, IDC_EDIT_BLUE2, m_dBlue2);
	// 	DDX_Text(pDX, IDC_EDIT_GREEN2, m_dGreen2);
	// 	DDX_Text(pDX, IDC_EDIT_RED2, m_dRed2);
	// 	DDX_Text(pDX, IDC_EDIT_RED3, m_dRed3);
	// 	DDX_Text(pDX, IDC_EDIT_GREEN3, m_dGreen3);
	// 	DDX_Text(pDX, IDC_EDIT_BLUE3, m_dBlue3);
	DDX_Check(pDX, IDC_CHECK_KICK3, m_bKick3);
	// 	DDX_Check(pDX, IDC_CHECK_BAYER3, m_bBayer3);
	// 	DDX_Check(pDX, IDC_CHECK_BAYER4, m_bBayer4);
	// 	DDX_Check(pDX, IDC_CHECK_BAYER5, m_bBayer5);
	// 	DDX_Check(pDX, IDC_CHECK_BAYER6, m_bBayer6);
	DDX_Check(pDX, IDC_CHECK_KICK4, m_bKick4);
	DDX_Check(pDX, IDC_CHECK_KICK5, m_bKick5);
	DDX_Check(pDX, IDC_CHECK_KICK6, m_bKick6);
	// 	DDX_Text(pDX, IDC_EDIT_BLUE4, m_dBlue4);
	// 	DDX_Text(pDX, IDC_EDIT_BLUE5, m_dBlue5);
	// 	DDX_Text(pDX, IDC_EDIT_BLUE6, m_dBlue6);
	// 	DDX_Text(pDX, IDC_EDIT_GREEN4, m_dGreen4);
	// 	DDX_Text(pDX, IDC_EDIT_GREEN5, m_dGreen5);
	// 	DDX_Text(pDX, IDC_EDIT_GREEN6, m_dGreen6);
	// 	DDX_Text(pDX, IDC_EDIT_RED4, m_dRed4);
	// 	DDX_Text(pDX, IDC_EDIT_RED5, m_dRed5);
	// 	DDX_Text(pDX, IDC_EDIT_RED6, m_dRed6);
	DDX_Radio(pDX, IDC_RADIO1, m_iShowMode);
	DDX_Check(pDX, IDC_CHECK_ALgCheck1, m_bOpenAlgCheck[0]);
	DDX_Check(pDX, IDC_CHECK_ALgCheck2, m_bOpenAlgCheck[1]);
	DDX_Check(pDX, IDC_CHECK_ALgCheck3, m_bOpenAlgCheck[2]);
	DDX_Check(pDX, IDC_CHECK_ALgCheck4, m_bOpenAlgCheck[3]);
	DDX_Check(pDX, IDC_CHECK_ALgCheck5, m_bOpenAlgCheck[4]);
	DDX_Check(pDX, IDC_CHECK_ALgCheck6, m_bOpenAlgCheck[5]);
	DDX_Check(pDX, IDC_CHECK_ObjStatus, m_bOpenObjStatusWnd);

	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_MaxNumOfConstantKick, m_iMaxNumOfConstantKick);
	DDV_MinMaxInt(pDX, m_iMaxNumOfConstantKick, 2, 256);
	DDX_Text(pDX, IDC_EDIT_LabelQueueLen, m_iLabelQueLen);
}


BEGIN_MESSAGE_MAP(CDlgSystemConfig, CDialog)
	//{{AFX_MSG_MAP(CDlgSystemConfig)
	// 	ON_BN_CLICKED(IDC_RADIO_NONE, OnRadioNone)
	// 	ON_BN_CLICKED(IDC_RADIO_ALL, OnRadioAll)
	// 	ON_BN_CLICKED(IDC_RADIO_ERROR, OnRadioError)
	// 	ON_BN_CLICKED(IDC_RADIO_EXIST, OnRadioExist)
	ON_BN_CLICKED(IDC_RADIO_KICK_ALL, OnRadioKickAll)
	ON_BN_CLICKED(IDC_RADIO_KICK_NO, OnRadioKickNo)
	ON_BN_CLICKED(IDC_RADIO_KICK_DIFF, OnRadioKickDiff)
	ON_BN_CLICKED(IDC_RADIO_KICK_NORMAL, OnRadioKickNormal)
	ON_BN_CLICKED(IDC_RADIO_SYSTEM, OnRadioSystem)
	ON_BN_CLICKED(IDC_RADIO_ALG, OnRadioAlg)
	//ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK_STOP_MACHNIE0, IDC_CHECK_STOP_MACHNIE4, &CDlgSystemConfig::OnBnClickedCheck6)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_STOP_MACHNIE0, &CDlgSystemConfig::OnBnClickedCheckStopMachnie0)
	ON_BN_CLICKED(IDC_CHECK_STOP_MACHNIE1, &CDlgSystemConfig::OnBnClickedCheckStopMachnie1)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSystemConfig message handlers

BOOL CDlgSystemConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	m_iShowTime = pFrame->m_iIntervalDisplayTime;
	m_iShowMode = pFrame->m_iImageShowMode;
	//m_iMaxSaveImageCount = pFrame->m_iMaxSaveImageCount;

	m_bBayer = pFrame->m_BayerTransform[Camera1].m_bAreaCamBayer;
	m_dRed = pFrame->m_BayerTransform[Camera1].m_dBayerRedGain;
	m_dGreen = pFrame->m_BayerTransform[Camera1].m_dBayerGreenGain;
	m_dBlue = pFrame->m_BayerTransform[Camera1].m_dBayerBlueGain;

	m_bBayer2 = pFrame->m_BayerTransform[Camera2].m_bAreaCamBayer;
	m_dRed2 = pFrame->m_BayerTransform[Camera2].m_dBayerRedGain;
	m_dGreen2 = pFrame->m_BayerTransform[Camera2].m_dBayerGreenGain;
	m_dBlue2 = pFrame->m_BayerTransform[Camera2].m_dBayerBlueGain;

	m_bBayer3 = pFrame->m_BayerTransform[Camera3].m_bAreaCamBayer;
	m_dRed3 = pFrame->m_BayerTransform[Camera3].m_dBayerRedGain;
	m_dGreen3 = pFrame->m_BayerTransform[Camera3].m_dBayerGreenGain;
	m_dBlue3 = pFrame->m_BayerTransform[Camera3].m_dBayerBlueGain;

	m_bBayer4 = pFrame->m_BayerTransform[Camera4].m_bAreaCamBayer;
	m_dRed4 = pFrame->m_BayerTransform[Camera4].m_dBayerRedGain;
	m_dGreen4 = pFrame->m_BayerTransform[Camera4].m_dBayerGreenGain;
	m_dBlue4 = pFrame->m_BayerTransform[Camera4].m_dBayerBlueGain;

	m_bBayer5 = pFrame->m_BayerTransform[Camera5].m_bAreaCamBayer;
	m_dRed5 = pFrame->m_BayerTransform[Camera5].m_dBayerRedGain;
	m_dGreen5 = pFrame->m_BayerTransform[Camera5].m_dBayerGreenGain;
	m_dBlue5 = pFrame->m_BayerTransform[Camera5].m_dBayerBlueGain;

	m_bBayer6 = pFrame->m_BayerTransform[Camera6].m_bAreaCamBayer;
	m_dRed6 = pFrame->m_BayerTransform[Camera6].m_dBayerRedGain;
	m_dGreen6 = pFrame->m_BayerTransform[Camera6].m_dBayerGreenGain;
	m_dBlue6 = pFrame->m_BayerTransform[Camera6].m_dBayerBlueGain;

	m_bKick1 = pFrame->m_bIsKick[Camera1];
	m_bKick2 = pFrame->m_bIsKick[Camera2];
	m_bKick3 = pFrame->m_bIsKick[Camera3];
	m_bKick4 = pFrame->m_bIsKick[Camera4];
	m_bKick5 = pFrame->m_bIsKick[Camera5];
	m_bKick6 = pFrame->m_bIsKick[Camera6];


	m_iKickMode = pFrame->m_iKickMode;
	m_iShowDevice = pFrame->m_iShowDevice;
	m_bSaveImageSys = pFrame->m_iSaveImageSys;
	m_iLabelQueLen = pFrame->m_iLabelQueLen;


	((CButton*)GetDlgItem(IDC_CHECK_STOP_MACHNIE0))->SetCheck(pFrame->m_arr_iAlgStopMachineCtrl[0]);
	((CButton*)GetDlgItem(IDC_CHECK_STOP_MACHNIE1))->SetCheck(pFrame->m_arr_iAlgStopMachineCtrl[1]);

	// 	if(0==m_iShowMode)
	// 	{
	// 		((CButton*)GetDlgItem(IDC_RADIO_NONE))->SetCheck(1);
	// 	}
	// 	else if(1==m_iShowMode)
	// 	{
	// 		((CButton*)GetDlgItem(IDC_RADIO_ALL))->SetCheck(1);
	// 	}
	// 	else if(4==m_iShowMode)
	// 	{
	// 		((CButton*)GetDlgItem(IDC_RADIO_ERROR))->SetCheck(1);
	// 	}
	// 	else if (2 == m_iShowMode)
	// 	{
	// 		((CButton*)GetDlgItem(IDC_RADIO_EXIST))->SetCheck(1);
	// 	}

	if (0 == m_iKickMode)
	{
		((CButton*)GetDlgItem(IDC_RADIO_KICK_ALL))->SetCheck(1);
	}
	else if (1 == m_iKickMode)
	{
		((CButton*)GetDlgItem(IDC_RADIO_KICK_NO))->SetCheck(1);
	}
	else if (2 == m_iKickMode)
	{
		((CButton*)GetDlgItem(IDC_RADIO_KICK_DIFF))->SetCheck(1);
	}
	else if (3 == m_iKickMode)
	{
		((CButton*)GetDlgItem(IDC_RADIO_KICK_NORMAL))->SetCheck(1);
	}

	// 显示系统图像还是算法图像
	if (0 == m_iShowDevice)
	{
		((CButton*)GetDlgItem(IDC_RADIO_SYSTEM))->SetCheck(1);
	}
	else if (1 == m_iShowDevice)
	{
		((CButton*)GetDlgItem(IDC_RADIO_ALG))->SetCheck(1);
	}
	for (int i = 0; i < 6; ++i)
	{
		m_bOpenAlgCheck[i] = pFrame->m_bOpenCameraAlgCheck[i];
	}
	m_bOpenObjStatusWnd = pFrame->m_bShowObjStatusWnd;
	m_iMaxNumOfConstantKick = pFrame->m_iMaxNumOfConstantKick;
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSystemConfig::OnOK()
{
	// TODO: Add extra validation here
	UpdateData();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	for (int i = 0; i < 6; ++i)
	{
		pFrame->m_bOpenCameraAlgCheck[i] = m_bOpenAlgCheck[i];
	}
	pFrame->m_bShowObjStatusWnd = TRUE == m_bOpenObjStatusWnd ? true : false;
	pFrame->m_iMaxNumOfConstantKick = m_iMaxNumOfConstantKick;
	pFrame->m_iLabelQueLen = m_iLabelQueLen;
	UpdateData(FALSE);
	CDialog::OnOK();
}

// void CDlgSystemConfig::OnRadioExist()
// {
// 	m_iShowDevice = 2;
// }
// 
// void CDlgSystemConfig::OnRadioNone()
// {
// 	// TODO: Add your control notification handler code here
// 	m_iShowMode = 0;
// }
// 
// void CDlgSystemConfig::OnRadioAll()
// {
// 	// TODO: Add your control notification handler code here
// 	m_iShowMode = 1;
// }
// 
// void CDlgSystemConfig::OnRadioError()
// {
// 	// TODO: Add your control notification handler code here
// 	m_iShowMode = 4;
// }

void CDlgSystemConfig::OnRadioKickAll()
{
	// TODO: Add your control notification handler code here
	m_iKickMode = 0;
}

void CDlgSystemConfig::OnRadioKickNo()
{
	// TODO: Add your control notification handler code here
	m_iKickMode = 1;
}

void CDlgSystemConfig::OnRadioKickDiff()
{
	// TODO: Add your control notification handler code here
	m_iKickMode = 2;
}

void CDlgSystemConfig::OnRadioKickNormal()
{
	// TODO: Add your control notification handler code here
	m_iKickMode = 3;
}

void CDlgSystemConfig::OnRadioSystem()
{
	// TODO: Add your control notification handler code here
	m_iShowDevice = 0;
}

void CDlgSystemConfig::OnRadioAlg()
{
	// TODO: Add your control notification handler code here
	m_iShowDevice = 1;
}




void CDlgSystemConfig::OnBnClickedCheck6(UINT uID)
{
	// TODO: 在此添加控件通知处理程序代码
// 	UpdateData();
// 	int iIndex = uID - IDC_CHECK_STOP_MACHNIE0;
// 	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
// 	pFrame->m_arr_iAlgStopMachineCtrl[iIndex] = ((CButton*)GetDlgItem(uID))->GetCheck();
}


void CDlgSystemConfig::OnBnClickedCheckStopMachnie0()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_arr_iAlgStopMachineCtrl[0] = ((CButton*)GetDlgItem(IDC_CHECK_STOP_MACHNIE0))->GetCheck();
}


void CDlgSystemConfig::OnBnClickedCheckStopMachnie1()
{
	// TODO: 在此添加控件通知处理程序代码
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_arr_iAlgStopMachineCtrl[1] = ((CButton*)GetDlgItem(IDC_CHECK_STOP_MACHNIE1))->GetCheck();
}
