// DlgShowImage.cpp : implementation file
//

#include "stdafx.h"
#include "BottleCap.h"
#include "BottleCapView.h"
#include "DlgShowImage.h"
#include "DlgTestcipher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgShowImage dialog

extern CMainFrame* g_pFrame;
extern CBottleCapView* g_pView;

CDlgShowImage::CDlgShowImage(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgShowImage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgShowImage)
	//}}AFX_DATA_INIT

	m_iWidth=0;
	m_iHeight=0;
	m_iBytes=0;
	m_iCameraSN=0;

	for(int i=0;i<CAMERA_COUNT;i++)
	{
		m_pImage[i]=new CImage;
		m_bFirstInit[i]=FALSE;
	}
}


void CDlgShowImage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgShowImage)
	DDX_Control(pDX, IDC_PIC1, m_imgPic1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgShowImage, CDialog)
	//{{AFX_MSG_MAP(CDlgShowImage)
	ON_BN_CLICKED(IDC_BTN_IMAGE, OnBtnImage)
	ON_BN_CLICKED(IDC_BTN_PARAM, OnBtnParam)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgShowImage message handlers

void CDlgShowImage::OnBtnImage() 
{
	// TODO: Add your control notification handler code here

	// 显示采集原图像
	ShowImage();

}

void CDlgShowImage::OnBtnParam() 
{
	// TODO: Add your control notification handler code here

// 	if(g_pFrame->m_bIsDetecting==TRUE)
// 	{
// 		if(IDYES!=AfxMessageBox("需停止检测后，方可进入参数设置!!!\r\n,是否停止检测?",MB_YESNO))
// 			return;
// 		else
// 		{
// 			g_pView->GetDlgItem(IDC_BTN_START)->SetWindowText(_T("开始检测"));
// 			g_pView->m_BtnStart.SetIcon(IDI_ICON_START,BS_LEFT);
// 			g_pFrame->StopCheck();
// 		}
// 		
// 	}
	
	// 设置Demo中模板为当前模板
	CWnd* pWnd = NULL;
	CString sVirtualDiskPath,strCamera,str1,str2,strDemo;

	int iImageWidth[CAMERA_COUNT]={0};
	int iImageHeight[CAMERA_COUNT]={0};
	int iImageByte[CAMERA_COUNT]={0};
	int iImageSize[CAMERA_COUNT]={0};
	
	strCamera.Format("%d",m_iCameraSN);

	for(int i=0;i<CAMERA_COUNT;i++)
	{
		iImageWidth[i] = g_pFrame->m_sCameraImageInfo[i].m_iCameraImageWidth;
		iImageHeight[i] = g_pFrame->m_sCameraImageInfo[i].m_iCameraImageHeight;
		iImageByte[i] = g_pFrame->m_sCameraImageInfo[i].m_iCameraImagePixelSize;
		iImageSize[i]=  g_pFrame->m_sCameraImageInfo[i].m_iCameraImageSize;
	}
	
	
	sVirtualDiskPath.Format("%s\\Config\\AlgConfig.ini",g_pFrame->m_sWorkPath);
// 	sVirtualDiskPath.Format("%s\\Config\\CheckSystemConfig.ini",g_pFrame->m_sWorkPath);
	
	WritePrivateProfileString("DEMO Controls","DefaultModel",g_pFrame->m_sModelName,sVirtualDiskPath);
	WritePrivateProfileString("DEMO Controls","DefaultTab",strCamera,sVirtualDiskPath);
	
	str1.Format("%s:\\SaveImages\\AlgImages\\Camera%d",g_pFrame->m_sVirtualDisk,m_iCameraSN+1);
	if(FALSE==g_pFrame->inIsPathExist(str1))
	{
		if(FALSE==g_pFrame->inCreateMultiLevelPath(str1))
		{
			str2.Format("路径%s不存在!",str1);
			AfxMessageBox(str2);
			return;
		}
	}
	
	str2.Format("%s\\OnlineImage.bmp",str1);
	g_pFrame->savebmp(str2,iImageWidth[m_iCameraSN],iImageHeight[m_iCameraSN],iImageByte[m_iCameraSN],m_pImage[m_iCameraSN]->GetImageBuffAddr());

	ShowWindow(SW_HIDE);
	
	strDemo.Format("%s\\DEMO.exe",g_pFrame->m_sWorkPath);
	
	pWnd = CWnd::FindWindow(NULL,g_pFrame->m_sDemoName);
	
	if(NULL!=pWnd)
	{
		pWnd->ShowWindow(SW_RESTORE);
		pWnd->SetForegroundWindow();
		return;
	}
	
	BOOL bSetAdv=FALSE;
	
// 	CDlgTestCipher dlg;
// 	if(dlg.DoModal() == IDOK && dlg.m_bOK == TRUE)
// 	{
// 		g_pFrame->m_bAdvMode = TRUE;
// 		g_pView->GetDlgItem(IDC_ADV_MODE)->ShowWindow(SW_SHOW);
// 		bSetAdv = TRUE;					
// 		HINSTANCE hRet= ShellExecute(NULL,"open",strDemo,NULL,NULL,SW_SHOWNORMAL);
// 	}

}

BOOL CDlgShowImage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	
	int iTemp1=0, iTemp2 = 0;
	int iBtnWidth=0,iBtnHeight=0;
	int iLeft1=0,iLeft2=0;
	int iLeft[CAMERA_COUNT]={0};
	int iTop[CAMERA_COUNT] ={0};
	int iWidth[CAMERA_COUNT]={0};
	int iHeight[CAMERA_COUNT]={0};
	int iWidth_Pic[CAMERA_COUNT]={0};
	int iHeight_Pic[CAMERA_COUNT]={0};
	
	float fRatio1[CAMERA_COUNT]={0};
	float fRatio2[CAMERA_COUNT]={0};
	
	CRect rcClient,rcBtn1,rcBtn2;
	CRect rcPic[CAMERA_COUNT];
	GetClientRect(rcClient);
	
	iTemp1 = 60;
	iTemp2 = rcClient.Height()-iTemp1;
	
	for(int i=0;i<CAMERA_COUNT;i++)
	{
		iWidth[i] = pFrame->m_sCameraImageInfo[i].m_iCameraImageWidth;
		iHeight[i] = pFrame->m_sCameraImageInfo[i].m_iCameraImageHeight;
		
		fRatio1[i] = ((float)iTemp2)/rcClient.Width();
		fRatio2[i] = ((float)iHeight[i])/iWidth[i];
		
		if(fRatio1[i]>fRatio2[i])
		{
			iWidth_Pic[i] = rcClient.Width();
			iHeight_Pic[i] = (int)(rcClient.Width()*fRatio2[i]);
		}
		else
		{
			iWidth_Pic[i] = (int)(iTemp2/fRatio2[i]);
			iHeight_Pic[i] = iTemp2;
		}
		
		iLeft[i] = (rcClient.Width()-iWidth_Pic[i])/2;
		iTop[i] = (iTemp2-iHeight_Pic[i])/2;
		
		rcPic[i].left = iLeft[i];
		rcPic[i].right = rcPic[i].left+iWidth_Pic[i];
		rcPic[i].top = iTop[i]+5;
		rcPic[i].bottom = rcPic[i].top+iHeight_Pic[i];
		
	}
	
	GetDlgItem(IDC_PIC1)->MoveWindow(rcPic[Camera1]);
// 	GetDlgItem(IDC_PIC2)->MoveWindow(rcPic[Camera2]);

	GetDlgItem(IDC_PIC1)->ShowWindow(SW_SHOW);
// 	GetDlgItem(IDC_PIC2)->ShowWindow(SW_HIDE);

	iBtnWidth = rcClient.Width()/6;
	iBtnHeight = iTemp1-20;
	
	iLeft1 = iBtnWidth;
	iLeft2 = 4*iLeft1;
	
	rcBtn1.left = iLeft1;
	rcBtn1.right = rcBtn1.left+iBtnWidth;
	rcBtn1.top = iTemp2+10;
	rcBtn1.bottom = rcBtn1.top+iBtnHeight;
	GetDlgItem(IDC_BTN_IMAGE)->MoveWindow(rcBtn1);
	
	rcBtn2.left = iLeft2;
	rcBtn2.right = rcBtn2.left+iBtnWidth;
	rcBtn2.top = rcBtn1.top;
	rcBtn2.bottom = rcBtn1.bottom;
	GetDlgItem(IDC_BTN_PARAM)->MoveWindow(rcBtn2);

	
	m_imgPic1.GetClientRect(m_lRect[Camera1]);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgShowImage::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialog::PostNcDestroy();
}

void CDlgShowImage::ShowImage()
{

	if (Camera1==m_iCameraSN)
	{
		m_pDC[m_iCameraSN]=m_imgPic1.GetDC();
		m_imgPic1.GetClientRect(m_lRect[m_iCameraSN]);
		m_imgPic1.ShowWindow(SW_SHOW);
	}

	m_lRect[m_iCameraSN].NormalizeRect();
	
	if (m_sSaveImagePath[m_iCameraSN] !="")
	{
		if (!OpenImage(m_sSaveImagePath[m_iCameraSN],m_pDC[m_iCameraSN],m_lRect[m_iCameraSN],m_iCameraSN))
		{
			AfxMessageBox("open false!");
			return;
		}
	}
	
	m_pImage[m_iCameraSN]->ShowDIBImage(m_pDC[m_iCameraSN],
		m_lRect[m_iCameraSN],
		NoFlip,
		Stretch,
		m_pImage[m_iCameraSN]->GetImageBuffAddr(),
		m_iWidth,
		m_iHeight,
		m_pImage[m_iCameraSN]->GetImageBitCount());
	
	m_pDC[m_iCameraSN]->TextOut(m_lRect[m_iCameraSN].left,m_lRect[m_iCameraSN].top,m_sErrorDes[m_iCameraSN]);


}	

BOOL CDlgShowImage::OpenImage(CString strPath, CDC *pDC, CRect lRect, int iCameraSN)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	if (strPath==_T(""))
	{
		return FALSE;
	}
	
	m_iWidth=pFrame->m_sCameraImageInfo[iCameraSN].m_iCameraImageWidth;
	m_iHeight=pFrame->m_sCameraImageInfo[iCameraSN].m_iCameraImageHeight;
	m_iBytes=pFrame->m_sCameraImageInfo[iCameraSN].m_iCameraImagePixelSize;
	
	if(m_bFirstInit[iCameraSN]==FALSE)
	{
		m_pImage[iCameraSN]->Init(m_iWidth*m_iHeight*m_iBytes,pFrame->m_iPicCountPerCam[iCameraSN]);
		m_bFirstInit[iCameraSN]=TRUE;
	}
	m_pImage[iCameraSN]->OpenBMPFileX(strPath,pFrame->m_iPicCountPerCam[iCameraSN]);
	
	return TRUE;
}

void CDlgShowImage::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	for(int i=0;i<CAMERA_COUNT;i++)
	{
		if(m_pImage[i]!=NULL)
		{
			delete m_pImage[i];
			m_pImage[i] = NULL;
		}
	}
}
