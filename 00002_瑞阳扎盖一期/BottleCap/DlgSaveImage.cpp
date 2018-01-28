// DlgSaveImage.cpp : implementation file
//

#include "stdafx.h"
#include "BottleCap.h"
#include "MainFrm.h"
#include "DlgSaveImage.h"
#include "BottleCapVIEW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainFrame*			g_pFrame; //全局指针
extern CBottleCapView*     g_pView;


/////////////////////////////////////////////////////////////////////////////
// CDlgSaveImage dialog


CDlgSaveImage::CDlgSaveImage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSaveImage::IDD, pParent)
	, m_iSaveImgObj(0)
{
	//{{AFX_DATA_INIT(CDlgSaveImage)
	m_iIsCircle = -1;
	m_iSaveType = -1;
	m_iGrabCount = 0;
	m_iErrorCount = 0;
	//}}AFX_DATA_INIT

	for(int i=0;i<ALLELEMENT_COUNT;i++)
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
	DDX_Text(pDX, IDC_EDIT_GRAB, m_iGrabCount);
	DDX_Text(pDX, IDC_EDIT_ERROR, m_iErrorCount);
	//}}AFX_DATA_MAP
	DDX_Radio(pDX, IDC_RADIO_SaveImgObj_Alg, m_iSaveImgObj);
}

BEGIN_MESSAGE_MAP(CDlgSaveImage, CDialog)
	//{{AFX_MSG_MAP(CDlgSaveImage)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHECK3, OnCheck3)
	ON_BN_CLICKED(IDC_CIRCLE, OnCircle)
	ON_BN_CLICKED(IDC_NO_CIRCLE, OnNoCircle)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSaveImage message handlers

BOOL CDlgSaveImage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	m_bSaveImage[Camera1]=TRUE;
	m_bSaveImage[Camera2]=TRUE;
	m_bSaveImage[Camera3]=TRUE;
	
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(1);;
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(1);;
	((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(1);;

	m_iSaveType = 1;
	m_iIsCircle = 1;

	m_iErrorCount = pFrame->m_iMaxSaveImageCount;
	m_iGrabCount = pFrame->m_iMaxSaveImageCount;
	m_iSaveImgObj = g_pFrame->m_iSaveImgObj;
	UpdateData(FALSE);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgSaveImage::OnOK() 
{
	// TODO: Add extra validation here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	UpdateData(TRUE);

	if(FALSE==m_bSaveImage[Camera1] && FALSE==m_bSaveImage[Camera2]&&FALSE==m_bSaveImage[Camera3])
	{
		AfxMessageBox("请选择工位号");
		return;
	}

	if(m_iSaveType==0)
	{
		if(m_iGrabCount>pFrame->m_iMaxSaveImageCount)
		{
			AfxMessageBox("保存图像张数超过最大值，请重新输入!");
			return;
		}
	}

	if(m_iSaveType==1)
	{
		if(m_iErrorCount>pFrame->m_iMaxSaveImageCount)
		{
			AfxMessageBox("保存图像张数超过最大值，请重新输入!");
			return;
		}
	}
	/*************************************<>*************************************/
	g_pFrame->m_iSaveImgObj = m_iSaveImgObj;
	CDialog::OnOK();
}

void CDlgSaveImage::OnCheck1() 
{
	// TODO: Add your control notification handler code here

	if(FALSE==m_bSaveImage[Camera1])
	{
		m_bSaveImage[Camera1]=TRUE;
	}
	else
	{
		m_bSaveImage[Camera1]=FALSE;
	}
}

void CDlgSaveImage::OnCheck2() 
{
	// TODO: Add your control notification handler code here

	if(FALSE==m_bSaveImage[Camera2])
	{
		m_bSaveImage[Camera2]=TRUE;
	}
	else
	{
		m_bSaveImage[Camera2]=FALSE;
	}
}

void CDlgSaveImage::OnCheck3() 
{
	// TODO: Add your control notification handler code here
	if(FALSE==m_bSaveImage[Camera3])
	{
		m_bSaveImage[Camera3]=TRUE;
	}
	else
	{
		m_bSaveImage[Camera3]=FALSE;
	}

}

void CDlgSaveImage::OnCircle() 
{
	// TODO: Add your control notification handler code here
// 	GetDlgItem(IDC_EDIT_GRAB)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_EDIT_ERROR)->EnableWindow(FALSE);

	CMainFrame* pFrame=(CMainFrame*)AfxGetMainWnd();
	
	CString sVirtualDiskPath,str1;
	sVirtualDiskPath.Format("%s\\Config\\AlgConfig.ini",pFrame->m_sWorkPath);
	
// 	// 写入保存循环存图时的参数
	str1.Format("%d",1);
	WritePrivateProfileString("DEMO Controls","EnableSequentialCheck",str1,sVirtualDiskPath);
	
	str1.Format("%s","Product");
	WritePrivateProfileString("DEMO Controls","SequentialCheckFolderFilter ",str1,sVirtualDiskPath);

}

void CDlgSaveImage::OnNoCircle() 
{
	// TODO: Add your control notification handler code here
// 	GetDlgItem(IDC_EDIT_GRAB)->EnableWindow(FALSE);
// 	GetDlgItem(IDC_EDIT_ERROR)->EnableWindow(FALSE);

	CMainFrame* pFrame=(CMainFrame*)AfxGetMainWnd();

	CString sVirtualDiskPath,str1;
	sVirtualDiskPath.Format("%s\\Config\\AlgConfig.ini",pFrame->m_sWorkPath);

	// 写入保存循环存图时的参数
	str1.Format("%d",1);
	WritePrivateProfileString("DEMO Controls","EnableSequentialCheck",str1,sVirtualDiskPath);
	
	str1.Format("%s","Product");
	WritePrivateProfileString("DEMO Controls","SequentialCheckFolderFilter ",str1,sVirtualDiskPath);

}
