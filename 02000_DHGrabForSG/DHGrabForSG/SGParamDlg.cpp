// SGParamDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DHGrabForSG.h"
#include "SGParamDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSGParamDlg dialog


CSGParamDlg::CSGParamDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSGParamDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSGParamDlg)
	m_bIsPlaneRGB = FALSE;
	m_nGrabSpeed = 0;
	m_strImagesPath = _T("");
	m_nChannelNum = 0;
	m_nImageHeight = 0;
	m_nImageWidth = 0;
	//}}AFX_DATA_INIT

	m_pGrabber = NULL;
}


void CSGParamDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSGParamDlg)
	DDX_Check(pDX, IDC_CHECK_IS_PLANERGB, m_bIsPlaneRGB);
	DDX_Text(pDX, IDC_EDIT_GRAB_SPEED, m_nGrabSpeed);
	DDV_MinMaxInt(pDX, m_nGrabSpeed, 1, 5000);
	DDX_Text(pDX, IDC_EDIT_IMAGES_PATH, m_strImagesPath);
	DDX_Text(pDX, IDC_EDIT_CHANNEL_NUM, m_nChannelNum);
	DDV_MinMaxInt(pDX, m_nChannelNum, 1, 4);
	DDX_Text(pDX, IDC_EDIT_IMAGE_HEIGHT, m_nImageHeight);
	DDV_MinMaxInt(pDX, m_nImageHeight, 1, 9999);
	DDX_Text(pDX, IDC_EDIT_IMAGE_WIDTH, m_nImageWidth);
	DDV_MinMaxInt(pDX, m_nImageWidth, 1, 9999);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSGParamDlg, CDialog)
	//{{AFX_MSG_MAP(CSGParamDlg)
	ON_BN_CLICKED(IDC_BUTTON_START_GRAB, OnButtonStartGrab)
	ON_BN_CLICKED(IDC_BUTTON_STOP_GRAB, OnButtonStopGrab)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_TO_FILE, OnButtonSaveToFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CSGParamDlg::SetOwner(CSimulatedCamera *pGrabber)
{
	m_pGrabber = pGrabber;
}

/////////////////////////////////////////////////////////////////////////////
// CSGParamDlg message handlers

void CSGParamDlg::OnOK() 
{
	ASSERT(m_pGrabber != NULL);

//	CDialog::OnOK();
	UpdateData(TRUE);//[2006-12-03]
	
	if(m_strImagesPath != m_pGrabber->m_strImageFileFolder)//[2006-12-03]
	{
		m_pGrabber->m_strImageFileFolder = m_strImagesPath;
		m_pGrabber->m_bUpdateImagesFolder = TRUE;
	}
		
	if(m_nGrabSpeed >= 1 && m_nGrabSpeed <= 5000)
	{
		m_pGrabber->SetParamInt(SG_GRAB_SPEED, m_nGrabSpeed);
	}
	m_pGrabber->SetParamInt(SG_IS_PLANERGB, m_bIsPlaneRGB);
	if(m_pGrabber->m_bGrab == TRUE)
	{
		m_pGrabber->StopGrab();
		Sleep(1000);
		
		m_pGrabber->SetParamInt(SG_IMAGE_WIDTH, m_nImageWidth);
		m_pGrabber->SetParamInt(SG_IMAGE_HEIGHT, m_nImageHeight);
		m_pGrabber->SetParamInt(SG_IMAGE_BYTE_COUNT, m_nChannelNum);

		m_pGrabber->StartGrab();
	}
	else
	{
		m_pGrabber->SetParamInt(SG_IMAGE_WIDTH, m_nImageWidth);
		m_pGrabber->SetParamInt(SG_IMAGE_HEIGHT, m_nImageHeight);
		m_pGrabber->SetParamInt(SG_IMAGE_BYTE_COUNT, m_nChannelNum);
	}
}

BOOL CSGParamDlg::OnInitDialog() 
{
	ASSERT(m_pGrabber != NULL);

	CDialog::OnInitDialog();
	
	CString str;
	GetWindowText(str);
	SetWindowText(str + _T("(") + m_pGrabber->m_strDeviceName + _T(")"));
				
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSGParamDlg::OnButtonStartGrab() 
{
	ASSERT(m_pGrabber != NULL);

	m_pGrabber->StartGrab();
}

void CSGParamDlg::OnButtonStopGrab() 
{
	ASSERT(m_pGrabber != NULL);

	m_pGrabber->StopGrab();
}

void CSGParamDlg::OnButtonSaveToFile() 
{	
	CString str;
	str.Format(_T("%d"), m_pGrabber->m_bPlaneRGB);
	WritePrivateProfileString(_T("采图参数"), _T("是否分通道"), str, m_pGrabber->m_sInitFile);
	str.Format(_T("%d"), m_pGrabber->m_nGrabSpeed);
	WritePrivateProfileString(_T("采图参数"), _T("采集速度"), str, m_pGrabber->m_sInitFile);
	WritePrivateProfileString(_T("采图参数"), _T("图像文件所在文件夹"), m_pGrabber->m_strImageFileFolder, m_pGrabber->m_sInitFile);//[2006-10-17]
	
	//[2006-11-30]
	str.Format(_T("%d"), m_pGrabber->m_nImageWidth);
	WritePrivateProfileString(_T("采图参数"), _T("图像宽度"), str, m_pGrabber->m_sInitFile);
	str.Format(_T("%d"), m_pGrabber->m_nImageHeight);
	WritePrivateProfileString(_T("采图参数"), _T("图像高度"), str, m_pGrabber->m_sInitFile);
	str.Format(_T("%d"), m_pGrabber->m_nImageByteCount);
	WritePrivateProfileString(_T("采图参数"), _T("每象素字节量"), str, m_pGrabber->m_sInitFile); 
}
