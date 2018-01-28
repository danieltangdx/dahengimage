// DlgSaveImage.cpp : implementation file
//

#include "stdafx.h"
#include "TubeCheck.h"
#include "MainFrm.h"
#include "DlgSaveImage.h"
#include "TubeCheckVIEW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainFrame*			g_pFrame; //È«¾ÖÖ¸Õë
extern CTubeCheckView*     g_pView;


/////////////////////////////////////////////////////////////////////////////
// CDlgSaveImage dialog


CDlgSaveImage::CDlgSaveImage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSaveImage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSaveImage)
	m_iIsCircle = -1;
	m_iSaveType = -1;
	m_iGrabCount = 0;
	m_iErrorCount = 0;
	//}}AFX_DATA_INIT

	for(int i=0;i<c_AllElementCount;i++)
	{
		m_bSaveImage[i]=FALSE;
	}
}

void CDlgSaveImage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSaveImage)
	DDX_Radio(pDX, IDC_NO_CIRCLE, m_iIsCircle);
	DDX_Radio(pDX, IDC_RADIO_GRAB, m_iSaveType);
	DDX_Radio(pDX, IDC_RADIO_SysSave, m_iSaveImgObj);
	DDX_Radio(pDX, IDC_RADIO_SaveImgNamingRegulation_ImgSN, m_iSaveImgNamingRegulation);
	DDX_Text(pDX, IDC_EDIT_GRAB, m_iGrabCount);
	DDX_Text(pDX, IDC_EDIT_ERROR, m_iErrorCount);
	DDX_Text(pDX, IDC_EDIT_EXIST, m_iExistCount);
	DDX_Check(pDX, IDC_CHECK1, m_bSaveImage[0]);
	DDX_Check(pDX, IDC_CHECK2, m_bSaveImage[1]);
	DDX_Check(pDX, IDC_CHECK3, m_bSaveImage[2]);
	DDX_Check(pDX, IDC_CHECK4, m_bSaveImage[3]);
	DDX_Check(pDX, IDC_CHECK5, m_bSaveImage[4]);
	DDX_Check(pDX, IDC_CHECK6, m_bSaveImage[5]);

	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgSaveImage, CDialog)
	//{{AFX_MSG_MAP(CDlgSaveImage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSaveImage message handlers

BOOL CDlgSaveImage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	m_iSaveType = g_pFrame->m_iSaveImageType;
	m_iIsCircle = g_pFrame->m_iIsCircle;

	m_iErrorCount = g_pFrame->m_iSaveErrCnt;
	m_iGrabCount = pFrame->m_iSaveSeqCnt;
	m_iExistCount = g_pFrame->m_iSaveExistCnt;
	m_iSaveImgObj = g_pFrame->m_iSaveImgObj;
	m_iSaveImgNamingRegulation = g_pFrame->m_iSaveImgNamingRegulation;
	for (int i = 0; i < c_AllElementCount; ++i)
	{
		m_bSaveImage[i] = g_pFrame->m_bSaveCameraImage[i];
	}
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSaveImage::OnOK() 
{
	// TODO: Add extra validation here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	UpdateData(TRUE);
	g_pFrame->m_iIsCircle = m_iIsCircle;
	g_pFrame->m_iSaveImageType = m_iSaveType;
	g_pFrame->m_iSaveSeqCnt = m_iGrabCount;
	g_pFrame->m_iSaveErrCnt = m_iErrorCount;
	g_pFrame->m_iSaveExistCnt = m_iExistCount;
	g_pFrame->m_iSaveImgObj = m_iSaveImgObj;
	g_pFrame->m_iSaveImgNamingRegulation = m_iSaveImgNamingRegulation;
	for (int i = 0; i < c_AllElementCount; ++i)
	{
		g_pFrame->m_bSaveCameraImage[i] = m_bSaveImage[i];
	}
	CDialog::OnOK();
}

