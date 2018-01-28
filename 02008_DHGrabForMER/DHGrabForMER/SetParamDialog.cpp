// SetParamDialog.cpp : implementation file
//

#include "stdafx.h"
#include "GrabberRegular.h"
#include "SetParamDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetParamDialog dialog


CSetParamDialog::CSetParamDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSetParamDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetParamDialog)
	m_ExposureTime = 0;

	m_check=true;
	//}}AFX_DATA_INIT
}


void CSetParamDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetParamDialog)
	DDX_Text(pDX, IDC_EDIT_ExpTime, m_ExposureTime);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_SLIDER_IMAGEWID, m_slider_width);
	DDX_Control(pDX, IDC_SLIDER_IMAGEHEI, m_slider_height);
	DDX_Control(pDX, IDC_EDIT_WID, m_widthStatic);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_heightStatic);
}


BEGIN_MESSAGE_MAP(CSetParamDialog, CDialog)
	//{{AFX_MSG_MAP(CSetParamDialog)
	ON_BN_CLICKED(IDSET, OnSetParam)
	ON_BN_CLICKED(IDSAVE, OnSaveParam)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_ChangeImageSize,CSetParamDialog::OnBnClickedChangeimagesize)
	ON_BN_CLICKED(IDCANCEL, CSetParamDialog::OnBnClickedCancel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetParamDialog message handlers

void CSetParamDialog::OnSetParam() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	m_pCamera->StopGrab();
	if (((CButton *)GetDlgItem(IDC_RADIO_Tri))->GetCheck())
	{
		m_pCamera->m_bTriggerMode = true;
		m_pCamera->SetEnum(GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_ON);
	} 
	else
	{
		m_pCamera->m_bTriggerMode = false;
		m_pCamera->SetEnum(GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_OFF);
	}
	if (m_ExposureTime > 0)
	{
		m_pCamera->m_dExposureTime = m_ExposureTime;
		m_pCamera->SetFloat(GX_FLOAT_EXPOSURE_TIME,m_ExposureTime);
	}

	m_pCamera->m_nWidth=m_slider_width.GetPos();
	m_pCamera->m_nHeight=m_slider_height.GetPos();


    m_pCamera->SetInt(GX_INT_WIDTH, m_slider_width.GetPos());			
	m_pCamera->SetInt(GX_INT_HEIGHT, m_slider_height.GetPos());




// 	GX_INT_WIDTH_MAX                  = 1002 | GX_FEATURE_INT,     ///< 最大宽度
// 		GX_INT_HEIGHT_MAX                 = 1003 | GX_FEATURE_INT,     ///< 最大高度
// 		GX_INT_OFFSET_X                   = 1004 | GX_FEATURE_INT,     ///< X方向偏移
// 		GX_INT_OFFSET_Y                   = 1005 | GX_FEATURE_INT,     ///< Y方向偏移
// 		GX_INT_WIDTH                      = 1006 | GX_FEATURE_INT,     ///< 图像宽度
// 		GX_INT_HEIGHT
// 
// 
// 


	
	//m_pCamera->StartGrab();
}

void CSetParamDialog::OnSaveParam() 
{
	// TODO: Add your control notification handler code here
	OnSetParam();
	m_pCamera->SaveParamToINI();
	
}

void CSetParamDialog::OnCancel() 
{
	// TODO: Add extra cleanup here
	m_pCamera->m_pParamSetDlg = NULL ;
	CDialog::OnCancel();
	DestroyWindow();
}

BOOL CSetParamDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_pCamera->m_bTriggerMode == true)
	{
		((CButton *)GetDlgItem(IDC_RADIO_Tri))->SetCheck(TRUE);
	}
	else
	{
		((CButton *)GetDlgItem(IDC_RADIO_Con))->SetCheck(TRUE);
	}
	m_ExposureTime = (int)m_pCamera->m_dExposureTime;
	UpdateData(FALSE) ;
	SetWindowPos(&wndTopMost , 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	m_slider_height.SetRange(1,m_pCamera->GetCameraMaxHei());
	m_slider_width.SetRange(1,m_pCamera->GetCameraMaxWid());

	m_slider_height.SetPos(m_pCamera->m_nHeight);
	m_slider_width.SetPos(m_pCamera->m_nWidth);

	CString valueStr;
	valueStr.Format(_T("%d"), m_slider_width.GetPos());
	m_widthStatic.SetWindowText(valueStr);

	valueStr.Format(_T("%d"), m_slider_height.GetPos());
	m_heightStatic.SetWindowText(valueStr);

	(CWnd*)GetDlgItem(IDC_SLIDER_IMAGEWID)->EnableWindow(FALSE);
	(CWnd*)GetDlgItem(IDC_SLIDER_IMAGEHEI)->EnableWindow(FALSE);



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSetParamDialog::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	m_pCamera->m_pParamSetDlg = NULL ;
	CDialog::OnClose();
	DestroyWindow();
}


void CSetParamDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(pScrollBar == (CScrollBar*) &m_slider_width)
	{	
		//m_pCamera->SetInt(GX_INT_WIDTH, m_slider_width.GetPos());			
		CString valueStr;
		valueStr.Format(_T("%d"), m_slider_width.GetPos());
		m_widthStatic.SetWindowText(valueStr);
	}

	if(pScrollBar == (CScrollBar*) &m_slider_height)
	{
		//m_pCamera->SetInt(GX_INT_HEIGHT, m_slider_height.GetPos());
		CString valueStr;
		valueStr.Format(_T("%d"), m_slider_height.GetPos());
		m_heightStatic.SetWindowText(valueStr);
		
	}	




	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CSetParamDialog::OnBnClickedChangeimagesize()
{
	// TODO: 在此添加控件通知处理程序代码

	m_check=!m_check;

	if (m_check)
	{

		(CWnd*)GetDlgItem(IDC_SLIDER_IMAGEWID)->EnableWindow(FALSE);
		(CWnd*)GetDlgItem(IDC_SLIDER_IMAGEHEI)->EnableWindow(FALSE);
	}
	else
	{
		(CWnd*)GetDlgItem(IDC_SLIDER_IMAGEWID)->EnableWindow(TRUE);
		(CWnd*)GetDlgItem(IDC_SLIDER_IMAGEHEI)->EnableWindow(TRUE);

       AfxMessageBox(_T("改变大小后，请重新启动软件！"));

	}
}


void CSetParamDialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	OnClose();
	//CDialog::OnCancel();
}
