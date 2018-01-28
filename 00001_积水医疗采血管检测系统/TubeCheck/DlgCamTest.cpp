// DlgCamTest.cpp : implementation file
//

#include "stdafx.h"
#include "TubeCheck.h"
#include "DlgCamTest.h"
#include "MainFrm.h"
#include "TubeCheckVIEW.h"
#ifdef __RealGrabber_
	#include "DHGrabberForMER.h"
#endif

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
	DDX_Text(pDX, IDC_EDIT_ExposureTime, m_iExposureTime);
}


BEGIN_MESSAGE_MAP(CDlgCamTest, CDialog)
	//{{AFX_MSG_MAP(CDlgCamTest)
	ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBtnStop)
	ON_BN_CLICKED(IDC_RADIO_CAM1, OnRadioGroupClick)
	ON_BN_CLICKED(IDC_RADIO_CAM2, OnRadioGroupClick)
	ON_BN_CLICKED(IDC_RADIO_CAM3, OnRadioGroupClick)
	ON_BN_CLICKED(IDC_RADIO_CAM4, OnRadioGroupClick)
	ON_BN_CLICKED(IDC_RADIO_CAM5, OnRadioGroupClick)
	ON_BN_CLICKED(IDC_RADIO_CAM6, OnRadioGroupClick)
	ON_WM_CLOSE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_ChangeExposureTime, &CDlgCamTest::OnBnClickedButtonChangeexposuretime)
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

#ifdef __RealGrabber_
	pFrame->LoadCamConfig(m_strModelName);
#endif

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

#ifdef __RealGrabber_
	pFrame->LoadCamConfig(pFrame->m_sModelName);
#endif
}

void CDlgCamTest::OnClose() 
{
	// TODO: Add your message handler code here and/or call default

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	KillTimer(CameraTest);
	//pFrame->m_bIsDetecting=FALSE;
// 	pFrame->SoftResetIOCard(pFrame->m_sModelName);
// 	CTubeCheckView* pView = (CTubeCheckView*)(pFrame->GetActiveView());
// 	pView->SetDlgItemText(IDC_BTN_START, _T("开始检测"));


	CDialog::OnClose();
}

BOOL CDlgCamTest::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	m_iCameraSN=0;
	m_iExposureTime = pFrame->m_iExposureTime[m_iCameraSN];

	pFrame->StopCheck();
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
				pFrame->m_IOCard[pFrame->m_iCamCardNum[m_iCameraSN]-1].setCardOutput(pFrame->m_iCamOutPort[m_iCameraSN],TRUE);
			}
			m_iCamLevel=1;
		}
		else
		{
			if(TRUE==pFrame->m_bIOCardValid)
			{
				pFrame->m_IOCard[pFrame->m_iCamCardNum[m_iCameraSN]-1].setCardOutput(pFrame->m_iCamOutPort[m_iCameraSN],FALSE);
			}
			m_iCamLevel=0;
		}
	}
	
	CDialog::OnTimer(nIDEvent);
}

void CDlgCamTest::OnRadioGroupClick()
{
	UpdateData();
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	m_iExposureTime = pFrame->m_iExposureTime[m_iCameraSN];
	UpdateData(FALSE);
}






void CDlgCamTest::OnBnClickedButtonChangeexposuretime()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	pFrame->m_iExposureTime[m_iCameraSN] = m_iExposureTime;
#ifdef __RealGrabber_
	pFrame->m_CameraGrabber[m_iCameraSN].MERSetParamInt(MERExposure, pFrame->m_iExposureTime[m_iCameraSN]);
#endif
	UpdateData(FALSE);
	MessageBox(TEXT("修改成功"), TEXT("通知"), MB_OK | MB_ICONINFORMATION);
}
