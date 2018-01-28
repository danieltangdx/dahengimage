// TubeCheckView.cpp : implementation of the CTubeCheckView class
//

#include "stdafx.h"
#include "TubeCheck.h"
#include "TubeCheckDoc.h"
#include "TubeCheckView.h"
#include "MainFrm.h"
#include "DlgTestCipher.h"
#include "DlgSelectProduct.h"
#include "DlgStatistic.h"
#include "DlgSaveImage.h"
#include "DlgSystemConfig.h"
#include "DlgCamTest.h"
#include "DlgIOCard.h"
#include "XDlgSysCfg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainFrame*		   g_pFrame; //全局指针
extern CTubeCheckDoc*		   g_pDoc;
extern CTubeCheckView*	   g_pView;


/////////////////////////////////////////////////////////////////////////////
// CTubeCheckView

IMPLEMENT_DYNCREATE(CTubeCheckView, CFormView)

BEGIN_MESSAGE_MAP(CTubeCheckView, CFormView)
	//{{AFX_MSG_MAP(CTubeCheckView)
	ON_BN_CLICKED(IDC_BTN_QUIT, OnBtnQuit)
	ON_BN_CLICKED(IDC_BTN_PARAM, OnBtnParam)
	ON_BN_CLICKED(IDC_BTN_MOULD_ADM, OnBtnMouldAdm)
	ON_BN_CLICKED(IDC_BTN_STATISTIC, OnBtnStatistic)
	ON_BN_CLICKED(IDC_BTN_START, OnBtnStart)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_RECORD, OnBtnRecord)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SAVE_IMAGE, OnSaveImage)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_IOCARD, OnIocard)
	ON_BN_CLICKED(IDC_SYSTEM_CONFIG, OnSystemConfig)
	ON_BN_CLICKED(IDC_ADV_MODE, OnAdvMode)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_BN_CLICKED(IDC_BTN_ZERO, OnBtnZero)
	ON_BN_CLICKED(IDC_BTN_DEFECT_STATISTICS, OnBtnDefectStatistics)
	ON_BN_CLICKED(IDC_STOP_SAVE_IMAGE, OnStopSaveImage)
	ON_WM_CANCELMODE()
	ON_BN_CLICKED(IDC_CAM_TEST, OnCamTest)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
	ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTubeCheckView construction/destruction

CTubeCheckView::CTubeCheckView()
	: CFormView(CTubeCheckView::IDD)
{
	//{{AFX_DATA_INIT(CTubeCheckView)
	//}}AFX_DATA_INIT
	// TODO: add construction code here

	for(int i=0;i<c_AllElementCount;i++)
	{
		m_bLabelAlarm[i] = FALSE;
		m_bAlarm_StopCount[i] = FALSE;
	}
	m_bQuitAlarmThread = FALSE;

	m_iLabelFontHeight=18;
}

CTubeCheckView::~CTubeCheckView()
{
}

void CTubeCheckView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTubeCheckView)
	DDX_Control(pDX, IDC_SPEED_DATA, m_SpeedData);
	DDX_Control(pDX, IDC_SPEED_LABEL, m_SpeedLabel);
	DDX_Control(pDX, IDC_BTN_ZERO, m_BtnZero);
	DDX_Control(pDX, IDC_BTN_DEFECT_STATISTICS, m_BtnDefectStatistic);
	DDX_Control(pDX, IDC_TAB1, m_Tab1);
	DDX_Control(pDX, IDC_BTN_MOULD_ADM, m_BtnModel);
	DDX_Control(pDX, IDC_BTN_PARAM, m_BtnParam);
	DDX_Control(pDX, IDC_BTN_QUIT, m_BtnQuit);
	DDX_Control(pDX, IDC_BTN_RECORD, m_BtnRecord);
	DDX_Control(pDX, IDC_BTN_STATISTIC, m_BtnStatistic);
	DDX_Control(pDX, IDC_BTN_START, m_BtnStart);
	DDX_Control(pDX, IDC_TOTAL_GOOD_RATIO_DATA, m_TotalGoodRatioData);
	DDX_Control(pDX, IDC_TOTAL_GOOD_RATIO_LABEL, m_TotalGoodRatioLabel);
	DDX_Control(pDX, IDC_TOTAL_GOOD_LABEL, m_TotalGoodLabel);
	DDX_Control(pDX, IDC_TOTAL_GOOD_DATA, m_TotalGoodData);
	DDX_Control(pDX, IDC_MODEL_NAME_LABEL, m_ModelNameLabel);
	DDX_Control(pDX, IDC_MODEL_NAME_DATA, m_ModelNameData);
	DDX_Control(pDX, IDC_TOTAL_DATA, m_TotalData);
	DDX_Control(pDX, IDC_TOTAL_LABEL, m_TotalLabel);
	//}}AFX_DATA_MAP
}


BOOL CTubeCheckView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CTubeCheckView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
	m_nMapMode = -1; // 设置滚动条不显示

	//////////////////////////////////////////////////////////
	// 增加代码
	InitWindowRect();

	m_fontTitle.CreateFont(
		50,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Arial");  
	GetDlgItem(IDC_WINDOW_TITLE)->SetFont(&m_fontTitle);

	m_fontTime.CreateFont(
		20,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Arial");  

	GetDlgItem(IDC_STATIC_YEAR)->SetFont(&m_fontTime);
	GetDlgItem(IDC_STATIC_HOUR)->SetFont(&m_fontTime);

	m_fontSystem.CreateFont(
		20,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Arial");  
	GetDlgItem(IDC_SAVE_IMAGE)->SetFont(&m_fontSystem);
	GetDlgItem(IDC_SYSTEM_CONFIG)->SetFont(&m_fontSystem);
	GetDlgItem(IDC_IOCARD)->SetFont(&m_fontSystem);
	GetDlgItem(IDC_STOP_SAVE_IMAGE)->SetFont(&m_fontSystem);
	GetDlgItem(IDC_CAM_TEST)->SetFont(&m_fontSystem);

	m_fontAdv.CreateFont(
		15,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Arial");  
	GetDlgItem(IDC_ADV_MODE)->SetFont(&m_fontAdv);

	// 创建显示对话框
	CRect rcTemp;
	int x_LD=0, y_LD=0, w_LD=0, h_LD=0;
	int cxScreen=0, cyScreen=0;
	
	// 获得屏幕分辨率
	cxScreen=GetSystemMetrics(SM_CXSCREEN);
	cyScreen=GetSystemMetrics(SM_CYSCREEN);

	// 显示错误图像
	m_pShowImage = NULL;
	m_pShowImage = new CDlgShowImage;
	m_pShowImage->Create(IDD_DLG_SHOW_IMAGE,this);
	m_pShowImage->GetWindowRect(&rcTemp);
	ScreenToClient(rcTemp);
	w_LD=rcTemp.Width();
	h_LD=rcTemp.Height();
	x_LD=(cxScreen-w_LD)/2;
	y_LD=(cyScreen-h_LD)/4;
	m_pShowImage->SetWindowPos(&wndTopMost, x_LD, y_LD, w_LD, h_LD,SWP_NOSIZE);
	m_pShowImage->ShowWindow(SW_HIDE);

	// 显示标签内容报警信息
	m_pdlgAlarm = NULL;
	m_pdlgAlarm = new CDlgAlarm;
	m_pdlgAlarm->Create(IDD_DLG_ALRAM,this);
	m_pdlgAlarm->GetWindowRect(&rcTemp);
	ScreenToClient(rcTemp);
	w_LD=rcTemp.Width();
	h_LD=rcTemp.Height();
	x_LD=(cxScreen-w_LD)/2;
	y_LD=(cyScreen-h_LD)/4;
	m_pdlgAlarm->SetWindowPos(&wndTopMost, x_LD, y_LD, w_LD, h_LD,SWP_NOSIZE);
	m_pdlgAlarm->ShowWindow(SW_HIDE);

	// 显示标签位置报警信息
	m_pdlgAlarmPos = NULL;
	m_pdlgAlarmPos = new CDlgAlarmPos;
	m_pdlgAlarmPos->Create(IDD_DLG_ALRAM_POS,this);
	m_pdlgAlarmPos->GetWindowRect(&rcTemp);
	ScreenToClient(rcTemp);
	w_LD=rcTemp.Width();
	h_LD=rcTemp.Height();
	x_LD=(cxScreen-w_LD)/2;
	y_LD=(cyScreen-h_LD)/4;
	m_pdlgAlarmPos->SetWindowPos(&wndTopMost, x_LD, y_LD, w_LD, h_LD,SWP_NOSIZE);
	m_pdlgAlarmPos->ShowWindow(SW_HIDE);
	
	// 显示缺陷统计信息
	m_pdlgDefect = NULL;
	m_pdlgDefect = new CDlgDefect;
	m_pdlgDefect->Create(IDD_DLG_DEFECT,this);
	m_pdlgDefect->GetWindowRect(&rcTemp);
	ScreenToClient(rcTemp);
	w_LD=rcTemp.Width();
	h_LD=rcTemp.Height();
	x_LD=(cxScreen-w_LD)/2;
	y_LD=(cyScreen-h_LD)/4;
	m_pdlgDefect->SetWindowPos(&wndTopMost, x_LD, y_LD, w_LD, h_LD,SWP_NOSIZE);
	m_pdlgDefect->ShowWindow(SW_HIDE);

	m_pdlgAlarmStop = NULL;
	m_pdlgAlarmStop = new CDlgAlarmStopCount;
	m_pdlgAlarmStop->Create(IDD_DLG_ALRAM_STOP_COUNT,this);
	m_pdlgAlarmStop->GetWindowRect(&rcTemp);
	ScreenToClient(rcTemp);
	w_LD=rcTemp.Width();
	h_LD=rcTemp.Height();
	x_LD=(cxScreen-w_LD)/2;
	y_LD=(cyScreen-h_LD)/4;
	m_pdlgAlarmStop->SetWindowPos(&wndTopMost, x_LD, y_LD, w_LD, h_LD,SWP_NOSIZE);
	m_pdlgAlarmStop->ShowWindow(SW_HIDE);

	// 开启显示错误信息对话框
	m_pShowWarmingInfoThread=NULL;
	m_pShowWarmingInfoThread = AfxBeginThread((AFX_THREADPROC)ShowWarmingInfoThread,this);
	if(NULL==m_pShowWarmingInfoThread)
	{
		return;
	}

	m_pMonitorStopCountThread=NULL;
	m_pMonitorStopCountThread=AfxBeginThread((AFX_THREADPROC)MonitorStopCountThread,this);
	if(NULL==m_pMonitorStopCountThread)
	{
		return;
	}

	// 更新时间
	SetTimer(TimeTimer,1000,NULL);
	SetTimer(ViewUpdateTimer,100,NULL);

}

/////////////////////////////////////////////////////////////////////////////
// CTubeCheckView printing

BOOL CTubeCheckView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTubeCheckView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CTubeCheckView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CTubeCheckView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

/////////////////////////////////////////////////////////////////////////////
// CTubeCheckView diagnostics

#ifdef _DEBUG
void CTubeCheckView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTubeCheckView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CTubeCheckDoc* CTubeCheckView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTubeCheckDoc)));
	return (CTubeCheckDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTubeCheckView message handlers

void CTubeCheckView::OnBtnQuit() 
{
	// TODO: Add your control notification handler code here
	//CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	g_pFrame->QuitPrecedure();
}

BOOL CTubeCheckView::InitWindowRect()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	// 显示区域宽高
	int iCx,iCy;
	int i{ 0 };

	// 图像控件高度
	int iPicH[c_AllElementCount]={0};
	int iPicW[c_AllElementCount]={0};
	int iImageW[c_AllElementCount]={0};
	int iImageH[c_AllElementCount]={0};
	int iPicL[c_AllElementCount]={0};
	int iPicT[c_AllElementCount]={0};

	// 显示框高度和宽度
	int iLabelW=0,iDataW=0,iLableH=0;

	// 列表框高度
	int iListH=0;

	// 按钮高度宽度
	int iBtnW=0,iBtnH=0;

	// 按钮图标大小
	int iBtnFontW = 18;

	// Logo宽高
	int iLogoW=0,iLogoH=0,iLogoTop=0;

	// Year,Hour高度
	int iYear=0,iHour=0;

	// 按钮间距
	int iBtnTop=0,iBtnLeft=0;

	// 列表框中各个项目宽度
	int iW1=0,iW2=0,iW3=0,iW4=0;

	// 标题部分
	CRect rcMP,rcTitle,rcSaveImage,rcStopSaveImage,rcSystem,rcIOCard,rcCamTest,rcAdvMode;
		
	// 图像区域位置信息
	CRect rcPicStatic;
	CRect rcPic[c_AllElementCount],rcAlg[c_AllElementCount];

	// 统计信息
	CRect rcInfoStatic,rcTotal1,rcTotal2,rcBad1,rcBad2;
	CRect rcBadRatio1,rcBadRatio2,rcModel1,rcModel2;
	CRect rcSpeed1,rcSpeed2;

	// [2015-5-26]
	CRect rcGoodLabel[c_AllElementCount],rcGoodData[c_AllElementCount];
	CRect rcGoodRatioLabel[c_AllElementCount],rcGoodRatioData[c_AllElementCount];
	
	// 操作信息
	CRect rcActStatic,rcBtnStart,rcBtnParam,rcBtnMoudle,rcBtnStatistic,rcDefectStatistic,rcZero;
	CRect rcRecord,rcQuit,rcTab,rcTab1,rcPage,rcLogo,rcLogo1,rcYear,rcHour;

	CTime tTime;
	CString sTime;

	iCx = GetSystemMetrics(SM_CXSCREEN);
	iCy = GetSystemMetrics(SM_CYSCREEN);

	///////////////////////////////////////////////
	// 标题部分

	// 标题图标
	rcMP.left = 20;
	rcMP.right = rcMP.left+50;
	rcMP.top=10;
	rcMP.bottom = 60;
	GetDlgItem(IDC_MP)->MoveWindow(rcMP);

	// 标题
	rcTitle.left = rcMP.right+5;
	rcTitle.right = rcTitle.left+600;
	rcTitle.top = rcMP.top;
	rcTitle.bottom = rcTitle.top+50;
	SetDlgItemText(IDC_WINDOW_TITLE,pFrame->m_sWindowTitle);
	GetDlgItem(IDC_WINDOW_TITLE)->MoveWindow(rcTitle);

	// 高级模式
	rcAdvMode.right = iCx-5;
	rcAdvMode.left = rcAdvMode.right-50;
	rcAdvMode.top = 5;
	rcAdvMode.bottom = rcAdvMode.top+20;
	GetDlgItem(IDC_ADV_MODE)->MoveWindow(rcAdvMode);
	GetDlgItem(IDC_ADV_MODE)->ShowWindow(SW_HIDE);

	// 接口卡设置
	rcIOCard.right = iCx-40;
	rcIOCard.left = rcIOCard.right-100;
	rcIOCard.top = 30;
	rcIOCard.bottom = rcIOCard.top+25;
	GetDlgItem(IDC_IOCARD)->MoveWindow(rcIOCard);

	rcCamTest.right=rcIOCard.left-15;
	rcCamTest.left=rcCamTest.right-80;
	rcCamTest.top=30;
	rcCamTest.bottom=rcCamTest.top+25;
	GetDlgItem(IDC_CAM_TEST)->MoveWindow(rcCamTest);

	// 系统配置
	rcSystem.right = rcCamTest.left-15;
	rcSystem.left = rcSystem.right-80;
	rcSystem.top = 30;
	rcSystem.bottom = rcSystem.top+25;
	GetDlgItem(IDC_SYSTEM_CONFIG)->MoveWindow(rcSystem);


	// 停止保存图像
	rcStopSaveImage.right = rcSystem.left-15;
	rcStopSaveImage.left = rcStopSaveImage.right-120;
	rcStopSaveImage.top=30;
	rcStopSaveImage.bottom=rcStopSaveImage.top+25;
	GetDlgItem(IDC_STOP_SAVE_IMAGE)->MoveWindow(rcStopSaveImage);


	// 保存图像
	rcSaveImage.right = rcStopSaveImage.left-15;
	rcSaveImage.left = rcSaveImage.right-80;
	rcSaveImage.top = 30;
	rcSaveImage.bottom = rcSaveImage.top+25;
	GetDlgItem(IDC_SAVE_IMAGE)->MoveWindow(rcSaveImage);
	
	/////////////////////////////////////////////////////////////
	// 整个窗口高度减去标题栏，菜单栏和状态栏高度
	rcPicStatic.left=5;
	rcPicStatic.right=rcPicStatic.left+iCx-10;
	rcPicStatic.top=60;
	rcPicStatic.bottom =rcPicStatic.top+(2*(iCy-60)/3);
	GetDlgItem(IDC_STATIC_PIC)->MoveWindow(rcPicStatic);

 	iPicW[Camera1] = (rcPicStatic.Width()-10)/3;
	iPicW[Camera2] = iPicW[Camera1];
	iPicW[Camera3] = iPicW[Camera1];
	iPicW[Camera4] = iPicW[Camera1];
	iPicW[Camera5] = iPicW[Camera1];
	iPicW[Camera6] = iPicW[Camera1];

	int iRealHeight = (rcPicStatic.Height()-20)/2;

	for(int i=0;i<c_AllElementCount;i++)
	{
		iImageH[i]=pFrame->m_sCameraImageInfo[i].m_iCameraImageHeight;
		iImageW[i]=pFrame->m_sCameraImageInfo[i].m_iCameraImageWidth;

		iPicH[i] = iPicW[i]*iImageH[i]/iImageW[i];

		if(iPicH[i]>iRealHeight)
		{
			iPicH[i] = iRealHeight;
			iPicW[i] = iPicH[i]*iImageW[i]/iImageH[i];
		}
	}

// 	iPicT[Camera1] = (rcPicStatic.Height()-2*iPicH[Camera1]-10)/3;
// 	iPicL[Camera1] = (rcPicStatic.Width()-iPicW[Camera1]-iPicW[Camera2]-iPicW[Camera3]-iPicW[Camera4]-iPicW[Camera5]-iPicW[Camera6]-10)/7;
// 
// 	iPicT[Camera2] = (rcPicStatic.Height()-2*iPicH[Camera2]-10)/3;
// 	iPicL[Camera2] = (rcPicStatic.Width()-iPicW[Camera1]-iPicW[Camera2]-iPicW[Camera3]-iPicW[Camera4]-iPicW[Camera5]-iPicW[Camera6]-10)/7;
// 
// 	iPicT[Camera3] = (rcPicStatic.Height()-2*iPicH[Camera3]-10)/3;
// 	iPicL[Camera3] = (rcPicStatic.Width()-iPicW[Camera1]-iPicW[Camera2]-iPicW[Camera3]-iPicW[Camera4]-iPicW[Camera5]-iPicW[Camera6]-10)/7;
// 
// 	iPicT[Camera4] = (rcPicStatic.Height()-2*iPicH[Camera4]-10)/3;
// 	iPicL[Camera4] = (rcPicStatic.Width()-iPicW[Camera1]-iPicW[Camera2]-iPicW[Camera3]-iPicW[Camera4]-iPicW[Camera5]-iPicW[Camera6]-10)/7;
// 
// 	iPicT[Camera5] = (rcPicStatic.Height()-2*iPicH[Camera5]-10)/3;
// 	iPicL[Camera5] = (rcPicStatic.Width()-iPicW[Camera1]-iPicW[Camera2]-iPicW[Camera3]-iPicW[Camera4]-iPicW[Camera5]-iPicW[Camera6]-10)/7;
// 
// 	iPicT[Camera6] = (rcPicStatic.Height()-2*iPicH[Camera6]-10)/3;
// 	iPicL[Camera6] = (rcPicStatic.Width()-iPicW[Camera1]-iPicW[Camera2]-iPicW[Camera3]-iPicW[Camera4]-iPicW[Camera5]-iPicW[Camera6]-10)/7;

	iPicT[Camera1] = ((rcPicStatic.Height()-10)/2-iPicH[Camera1])/2;
	iPicL[Camera1] = ((rcPicStatic.Width()-5)/3-iPicW[Camera1])/2;
	
	iPicT[Camera2] = ((rcPicStatic.Height()-10)/2-iPicH[Camera2])/2;
	iPicL[Camera2] = ((rcPicStatic.Width()-5)/3-iPicW[Camera2])/2;
	
	iPicT[Camera3] = ((rcPicStatic.Height()-10)/2-iPicH[Camera3])/2;
	iPicL[Camera3] = ((rcPicStatic.Width()-5)/3-iPicW[Camera3])/2;

	iPicT[Camera4] = ((rcPicStatic.Height()-10)/2-iPicH[Camera4])/2;
	iPicL[Camera4] = ((rcPicStatic.Width()-5)/3-iPicW[Camera4])/2;

	iPicT[Camera5] = ((rcPicStatic.Height()-10)/2-iPicH[Camera5])/2;
	iPicL[Camera5] = ((rcPicStatic.Width()-5)/3-iPicW[Camera5])/2;

	iPicT[Camera6] = ((rcPicStatic.Height()-10)/2-iPicH[Camera6])/2;
	iPicL[Camera6] = ((rcPicStatic.Width()-5)/3-iPicW[Camera6])/2;

	int iOffsetX = (rcPicStatic.Width()-5)/3;
	int iOffsetY =  (rcPicStatic.Height()-10)/2;

	// 采集图像1
	rcPic[Camera1].left = rcPicStatic.left+iPicL[Camera1]+5;
	rcPic[Camera1].right = rcPic[Camera1].left+iPicW[Camera1];
	rcPic[Camera1].top = rcPicStatic.top+iPicT[Camera1]+10;
	rcPic[Camera1].bottom = rcPic[Camera1].top+iPicH[Camera1];
	GetDlgItem(IDC_PIC1)->MoveWindow(rcPic[Camera1]);

	rcAlg[Camera1].left=rcPic[Camera1].left;
	rcAlg[Camera1].right =rcPic[Camera1].right;
	rcAlg[Camera1].top = rcPic[Camera1].top;
	rcAlg[Camera1].bottom = rcPic[Camera1].bottom;
	GetDlgItem(IDC_ALG1)->MoveWindow(rcAlg[Camera1]);

	// 算法图像1
// 	rcAlg[Camera1].left = rcPic[Camera1].left;
// 	rcAlg[Camera1].right = rcAlg[Camera1].left+iPicW[Camera1];
// 	rcAlg[Camera1].top = rcPic[Camera1].bottom+iPicT[Camera1];
// 	rcAlg[Camera1].bottom = rcAlg[Camera1].top+iPicH[Camera1];
// 	GetDlgItem(IDC_ALG1)->MoveWindow(rcAlg[Camera1]);

	// 采集图像2
// 	rcPic[Camera2].left = rcPic[Camera1].right+iPicL[Camera2];
// 	rcPic[Camera2].right = rcPic[Camera2].left+iPicW[Camera2];
// 	rcPic[Camera2].top =rcPicStatic.top+iPicT[Camera2]+10;
// 	rcPic[Camera2].bottom = rcPic[Camera2].top+iPicH[Camera2];
// 	GetDlgItem(IDC_PIC2)->MoveWindow(rcPic[Camera2]);
	
	// 算法图像2
// 	rcAlg[Camera2].left = rcPic[Camera2].left;
// 	rcAlg[Camera2].right = rcAlg[Camera2].left+iPicW[Camera2];
// 	rcAlg[Camera2].top = rcPic[Camera2].bottom+iPicT[Camera2];
// 	rcAlg[Camera2].bottom = rcAlg[Camera2].top+iPicH[Camera2];

	rcPic[Camera2].left = rcPicStatic.left+iOffsetX+5+iPicL[Camera2];
	rcPic[Camera2].right = rcPic[Camera2].left+iPicW[Camera2];
	rcPic[Camera2].top = rcPicStatic.top+iPicT[Camera2]+10;
	rcPic[Camera2].bottom = rcPic[Camera2].top+iPicH[Camera2];
	GetDlgItem(IDC_PIC2)->MoveWindow(rcPic[Camera2]);

	rcAlg[Camera2].left=rcPic[Camera2].left;
	rcAlg[Camera2].right =rcPic[Camera2].right;
	rcAlg[Camera2].top = rcPic[Camera2].top;
	rcAlg[Camera2].bottom = rcPic[Camera2].bottom;
	GetDlgItem(IDC_ALG2)->MoveWindow(rcAlg[Camera2]);

	// 采集图像3
// 	rcPic[Camera3].left = rcPic[Camera2].right+iPicL[Camera3];
// 	rcPic[Camera3].right = rcPic[Camera3].left+iPicW[Camera3];
// 	rcPic[Camera3].top =rcPicStatic.top+iPicT[Camera3]+10;
// 	rcPic[Camera3].bottom = rcPic[Camera3].top+iPicH[Camera3];
// 	GetDlgItem(IDC_PIC3)->MoveWindow(rcPic[Camera3]);
// 	
// 	// 算法图像3
// 	rcAlg[Camera3].left = rcPic[Camera3].left;
// 	rcAlg[Camera3].right = rcAlg[Camera3].left+iPicW[Camera3];
// 	rcAlg[Camera3].top = rcPic[Camera3].bottom+iPicT[Camera3];
// 	rcAlg[Camera3].bottom = rcAlg[Camera3].top+iPicH[Camera3];
// 	GetDlgItem(IDC_ALG3)->MoveWindow(rcAlg[Camera3]);

	rcPic[Camera3].left = rcPicStatic.left+2*iOffsetX+5+iPicL[Camera3];
	rcPic[Camera3].right = rcPic[Camera3].left+iPicW[Camera3];
	rcPic[Camera3].top = rcPicStatic.top+iPicT[Camera3]+10;
	rcPic[Camera3].bottom = rcPic[Camera3].top+iPicH[Camera3];
	GetDlgItem(IDC_PIC3)->MoveWindow(rcPic[Camera3]);
	
	rcAlg[Camera3].left=rcPic[Camera3].left;
	rcAlg[Camera3].right =rcPic[Camera3].right;
	rcAlg[Camera3].top = rcPic[Camera3].top;
	rcAlg[Camera3].bottom = rcPic[Camera3].bottom;
	GetDlgItem(IDC_ALG3)->MoveWindow(rcAlg[Camera3]);


	// 采集图像4
// 	rcPic[Camera4].left = rcPic[Camera3].right+iPicL[Camera4];
// 	rcPic[Camera4].right = rcPic[Camera4].left+iPicW[Camera4];
// 	rcPic[Camera4].top =rcPicStatic.top+iPicT[Camera4]+10;
// 	rcPic[Camera4].bottom = rcPic[Camera4].top+iPicH[Camera4];
// 	GetDlgItem(IDC_PIC4)->MoveWindow(rcPic[Camera4]);
// 	
// 	// 算法图像4
// 	rcAlg[Camera4].left = rcPic[Camera4].left;
// 	rcAlg[Camera4].right = rcAlg[Camera4].left+iPicW[Camera4];
// 	rcAlg[Camera4].top = rcPic[Camera4].bottom+iPicT[Camera4];
// 	rcAlg[Camera4].bottom = rcAlg[Camera4].top+iPicH[Camera4];
// 	GetDlgItem(IDC_ALG4)->MoveWindow(rcAlg[Camera4]);

	rcPic[Camera4].left = rcPicStatic.left+0*iOffsetX+5+iPicL[Camera4];
	rcPic[Camera4].right = rcPic[Camera4].left+iPicW[Camera4];
	rcPic[Camera4].top = rcPicStatic.top+iOffsetY+10+iPicT[Camera4];
	rcPic[Camera4].bottom = rcPic[Camera4].top+iPicH[Camera4];
	GetDlgItem(IDC_PIC4)->MoveWindow(rcPic[Camera4]);
	
	rcAlg[Camera4].left=rcPic[Camera4].left;
	rcAlg[Camera4].right =rcPic[Camera4].right;
	rcAlg[Camera4].top = rcPic[Camera4].top;
	rcAlg[Camera4].bottom = rcPic[Camera4].bottom;
	GetDlgItem(IDC_ALG4)->MoveWindow(rcAlg[Camera4]);

	// 采集图像5
// 	rcPic[Camera5].left = rcPic[Camera4].right+iPicL[Camera5];
// 	rcPic[Camera5].right = rcPic[Camera5].left+iPicW[Camera5];
// 	rcPic[Camera5].top =rcPicStatic.top+iPicT[Camera5]+10;
// 	rcPic[Camera5].bottom = rcPic[Camera5].top+iPicH[Camera5];
// 	GetDlgItem(IDC_PIC5)->MoveWindow(rcPic[Camera5]);
// 	
// 	// 算法图像5
// 	rcAlg[Camera5].left = rcPic[Camera5].left;
// 	rcAlg[Camera5].right = rcAlg[Camera5].left+iPicW[Camera5];
// 	rcAlg[Camera5].top = rcPic[Camera5].bottom+iPicT[Camera5];
// 	rcAlg[Camera5].bottom = rcAlg[Camera5].top+iPicH[Camera5];
// 	GetDlgItem(IDC_ALG5)->MoveWindow(rcAlg[Camera5]);

	rcPic[Camera5].left = rcPicStatic.left+1*iOffsetX+5+iPicL[Camera5];
	rcPic[Camera5].right = rcPic[Camera5].left+iPicW[Camera5];
	rcPic[Camera5].top = rcPicStatic.top+iOffsetY+10+iPicT[Camera5];
	rcPic[Camera5].bottom = rcPic[Camera5].top+iPicH[Camera5];
	GetDlgItem(IDC_PIC5)->MoveWindow(rcPic[Camera5]);
	
	rcAlg[Camera5].left=rcPic[Camera5].left;
	rcAlg[Camera5].right =rcPic[Camera5].right;
	rcAlg[Camera5].top = rcPic[Camera5].top;
	rcAlg[Camera5].bottom = rcPic[Camera5].bottom;
	GetDlgItem(IDC_ALG5)->MoveWindow(rcAlg[Camera5]);

	// 采集图像6
// 	rcPic[Camera6].left = rcPic[Camera5].right+iPicL[Camera6];
// 	rcPic[Camera6].right = rcPic[Camera6].left+iPicW[Camera6];
// 	rcPic[Camera6].top =rcPicStatic.top+iPicT[Camera6]+10;
// 	rcPic[Camera6].bottom = rcPic[Camera6].top+iPicH[Camera6];
// 	GetDlgItem(IDC_PIC6)->MoveWindow(rcPic[Camera6]);
// 	
// 	// 算法图像6
// 	rcAlg[Camera6].left = rcPic[Camera6].left;
// 	rcAlg[Camera6].right = rcAlg[Camera6].left+iPicW[Camera6];
// 	rcAlg[Camera6].top = rcPic[Camera6].bottom+iPicT[Camera6];
// 	rcAlg[Camera6].bottom = rcAlg[Camera6].top+iPicH[Camera6];
// 	GetDlgItem(IDC_ALG6)->MoveWindow(rcAlg[Camera6]);

	rcPic[Camera6].left = rcPicStatic.left+2*iOffsetX+5+iPicL[Camera6];
	rcPic[Camera6].right = rcPic[Camera6].left+iPicW[Camera6];
	rcPic[Camera6].top = rcPicStatic.top+iOffsetY+10+iPicT[Camera6];
	rcPic[Camera6].bottom = rcPic[Camera6].top+iPicH[Camera6];
	GetDlgItem(IDC_PIC6)->MoveWindow(rcPic[Camera6]);
	
	rcAlg[Camera6].left=rcPic[Camera6].left;
	rcAlg[Camera6].right =rcPic[Camera6].right;
	rcAlg[Camera6].top = rcPic[Camera6].top;
	rcAlg[Camera6].bottom = rcPic[Camera6].bottom;
	GetDlgItem(IDC_ALG6)->MoveWindow(rcAlg[Camera6]);

	GetDlgItem(IDC_PIC1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PIC2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PIC3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PIC4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PIC5)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PIC6)->ShowWindow(SW_HIDE);

	GetDlgItem(IDC_ALG1)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_ALG2)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_ALG3)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_ALG4)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_ALG5)->ShowWindow(SW_SHOW);
	GetDlgItem(IDC_ALG6)->ShowWindow(SW_SHOW);

	for(i=0;i<c_AllElementCount;i++)
	{
		m_rectShowImage[i].left = rcPic[i].left;
		m_rectShowImage[i].right = rcPic[i].right;
		m_rectShowImage[i].top = rcPic[i].top;
		m_rectShowImage[i].bottom = rcPic[i].bottom;
	}

//////////////////////////////////////////////////////////////////////////
	// 统计信息框
	rcInfoStatic.left = rcPicStatic.left+5;
	rcInfoStatic.right = rcInfoStatic.left+(iCx-5)/2;
	rcInfoStatic.top = rcPicStatic.bottom+5;
	rcInfoStatic.bottom = iCy-10;
	GetDlgItem(IDC_STATIC_INFO)->MoveWindow(rcInfoStatic);

	iLabelW = (rcInfoStatic.Width()-15)/3;
	iDataW = (rcInfoStatic.Width()-15-iLabelW);
	iLableH = (rcInfoStatic.Height()-35)/5;

	// 检测总数
	rcTotal1.left = rcInfoStatic.left+5;
	rcTotal1.right = rcTotal1.left+iLabelW;
	rcTotal1.top = rcInfoStatic.top+10;
	rcTotal1.bottom = rcTotal1.top+iLableH;
	GetDlgItem(IDC_TOTAL_LABEL)->MoveWindow(rcTotal1);

	rcTotal2.left = rcTotal1.right+5;
	rcTotal2.right = rcTotal2.left+iDataW;
	rcTotal2.top = rcTotal1.top;
	rcTotal2.bottom = rcTotal2.top+iLableH;
	GetDlgItem(IDC_TOTAL_DATA)->MoveWindow(rcTotal2);

	// 坏品数
	rcBad1.left = rcInfoStatic.left+5;
	rcBad1.right = rcBad1.left+iLabelW;
	rcBad1.top = rcTotal1.bottom+5;
	rcBad1.bottom = rcBad1.top+iLableH;
	GetDlgItem(IDC_TOTAL_GOOD_LABEL)->MoveWindow(rcBad1);
	
	rcBad2.left = rcBad1.right+5;
	rcBad2.right = rcBad2.left+iDataW;
	rcBad2.top = rcBad1.top;
	rcBad2.bottom = rcBad2.top+iLableH;
	GetDlgItem(IDC_TOTAL_GOOD_DATA)->MoveWindow(rcBad2);

	// 坏品率
	rcBadRatio1.left = rcInfoStatic.left+5;
	rcBadRatio1.right = rcBadRatio1.left+iLabelW;
	rcBadRatio1.top = rcBad1.bottom+5;
	rcBadRatio1.bottom = rcBadRatio1.top+iLableH;
	GetDlgItem(IDC_TOTAL_GOOD_RATIO_LABEL)->MoveWindow(rcBadRatio1);
	
	rcBadRatio2.left = rcBadRatio1.right+5;
	rcBadRatio2.right = rcBadRatio2.left+iDataW;
	rcBadRatio2.top = rcBadRatio1.top;
	rcBadRatio2.bottom = rcBadRatio2.top+iLableH;
	GetDlgItem(IDC_TOTAL_GOOD_RATIO_DATA)->MoveWindow(rcBadRatio2);

	// 模板名称
	rcModel1.left = rcInfoStatic.left+5;
	rcModel1.right = rcModel1.left+iLabelW;
	rcModel1.top = rcBadRatio1.bottom+5;
	rcModel1.bottom = rcModel1.top+iLableH;
	GetDlgItem(IDC_MODEL_NAME_LABEL)->MoveWindow(rcModel1);
	
	rcModel2.left = rcModel1.right+5;
	rcModel2.right = rcModel2.left+iDataW;
	rcModel2.top = rcModel1.top;
	rcModel2.bottom = rcModel2.top+iLableH;
	GetDlgItem(IDC_MODEL_NAME_DATA)->MoveWindow(rcModel2);


	// 检测速度[2015-9-15]
	rcSpeed1.left = rcInfoStatic.left+5;
	rcSpeed1.right = rcSpeed1.left+iLabelW;
	rcSpeed1.top = rcModel1.bottom+5;
	rcSpeed1.bottom = rcSpeed1.top+iLableH;
	GetDlgItem(IDC_SPEED_LABEL)->MoveWindow(rcSpeed1);
	
	rcSpeed2.left = rcSpeed1.right+5;
	rcSpeed2.right = rcSpeed2.left+iDataW;
	rcSpeed2.top = rcSpeed1.top;
	rcSpeed2.bottom = rcSpeed2.top+iLableH;
	GetDlgItem(IDC_SPEED_DATA)->MoveWindow(rcSpeed2);

	///////////////////////////////////////////////////////////////////
	// 操作信息
	rcActStatic.left = rcInfoStatic.right+5;
	rcActStatic.right = iCx-5;
	rcActStatic.top=rcInfoStatic.top;
	rcActStatic.bottom = rcInfoStatic.bottom;
	GetDlgItem(IDC_STATIC_ACT)->MoveWindow(rcActStatic);

	iListH = rcActStatic.Height()/2;
	iBtnH = (rcActStatic.Height()-40)/4;

// 	// 列表框
// 	rcTab.left = rcActStatic.left+5;
// 	rcTab.right = rcActStatic.right-5;
// 	rcTab.top = rcActStatic.top+10;
// 	rcTab.bottom = rcTab.top+iListH;
// 	GetDlgItem(IDC_TAB1)->MoveWindow(rcTab);
// 
// 	m_Tab1.GetWindowRect(rcTab1);
// 	m_Tab1.GetClientRect(rcTab1);
// 
// 	rcPage.left = rcTab1.left;
// 	rcPage.right = rcTab1.right;
// 	rcPage.top = rcTab1.top;
// 	rcPage.bottom = rcTab1.bottom-25;
// 
// 	m_Tab1.InsertItem(0,"工位1");
// 
// 	m_dlgPage1.Create(IDD_DLG_PAGE1,GetDlgItem(IDC_TAB1));
// 
// 	m_dlgPage1.MoveWindow(&rcPage);
// 
// 	m_dlgPage1.GetDlgItem(IDC_LIST1)->MoveWindow(&rcPage);
// 
// 	iW1= 60;
// 	iW2 = 100;
// 	iW3 = 200;
// 	iW4 = rcTab.Width()-iW1-iW2-iW3;
// 
// 	m_dlgPage1.m_lscInfo.InsertColumn(0, "序号",	LVCFMT_LEFT, iW1);
// 	m_dlgPage1.m_lscInfo.InsertColumn(1, "图像大张号", LVCFMT_LEFT, iW2);
// 	m_dlgPage1.m_lscInfo.InsertColumn(2, "错误描述", LVCFMT_LEFT, iW3);
// 	m_dlgPage1.m_lscInfo.InsertColumn(3, "存图时间", LVCFMT_LEFT, iW4);
// 	m_dlgPage1.m_lscInfo.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
// 	
// 	m_dlgPage1.ShowWindow(true);
// 	m_Tab1.SetCurSel(0);


	// Logo大小
	CWnd  *pWnd = GetDlgItem(IDC_LOGO); //获取LOGO显示框位置
	WINDOWPLACEMENT  *winPlacement;
	winPlacement=new WINDOWPLACEMENT;

	pWnd->GetWindowPlacement(winPlacement);
	rcLogo.left = winPlacement->rcNormalPosition.left;
	rcLogo.right = winPlacement->rcNormalPosition.right;
	rcLogo.top = winPlacement->rcNormalPosition.top;
	rcLogo.bottom = winPlacement->rcNormalPosition.bottom;
	delete winPlacement;

	// LOGO

	iYear=20;
	iHour=20;
	iLogoTop = rcActStatic.Height()  - rcLogo.Height()-10-5-iYear-iHour;
	iLogoTop = iLogoTop/2;

	rcLogo1.right = rcActStatic.right-20;
	rcLogo1.left = rcActStatic.right-rcLogo.Width();
	rcLogo1.top = rcActStatic.top+iLogoTop;
	rcLogo1.bottom = rcLogo1.top+rcLogo.Height();
	GetDlgItem(IDC_LOGO)->MoveWindow(rcLogo1);

	rcYear.left = rcLogo1.left;
	rcYear.right = rcLogo1.right;
	rcYear.top = rcLogo1.bottom+5;
	rcYear.bottom = rcYear.top+20;
	GetDlgItem(IDC_STATIC_YEAR)->MoveWindow(rcYear);

	tTime = CTime::GetCurrentTime();
	sTime.Format("%d-%d-%d",tTime.GetYear(),tTime.GetMonth(),tTime.GetDay());
	SetDlgItemText(IDC_STATIC_YEAR,sTime);

	rcHour.left = rcLogo1.left;
	rcHour.right = rcLogo1.right;
	rcHour.top = rcYear.bottom+5;
	rcHour.bottom = rcHour.top+20;
	GetDlgItem(IDC_STATIC_HOUR)->MoveWindow(rcHour);
	sTime.Format("%02d:%02d:%02d",tTime.GetHour(),tTime.GetMinute(),tTime.GetSecond());
	SetDlgItemText(IDC_STATIC_HOUR,sTime);


	iBtnTop = 10;
	iBtnLeft = 10;
	iBtnH = (rcActStatic.Height()-iBtnTop*5)/4;
	iBtnW = (rcLogo1.left - rcActStatic.left - 3*iBtnLeft)/2;
	
	// 开始检测
	rcBtnStart.left = rcActStatic.left+iBtnLeft;
	rcBtnStart.right = rcBtnStart.left+iBtnW;
	rcBtnStart.top = rcActStatic.top+iBtnTop;
	rcBtnStart.bottom = rcBtnStart.top+iBtnH;
	m_BtnStart.MoveWindow(rcBtnStart);
	m_BtnStart.SetShade(SHS_HARDBUMP,8,20,5,RGB(0,0,255));
	m_BtnStart.SetIcon(IDI_ICON_START,BS_LEFT);
	m_BtnStart.SetFont("黑体",iBtnFontW,400,FALSE,FALSE);
	m_BtnStart.SetTextColor(RGB(0x0,0x72,0xc6));

	
	// 参数设置
	rcBtnParam.left = rcBtnStart.right+iBtnLeft;
	rcBtnParam.right = rcBtnParam.left+iBtnW;
	rcBtnParam.top = rcBtnStart.top;
	rcBtnParam.bottom = rcBtnParam.top+iBtnH;
	m_BtnParam.MoveWindow(rcBtnParam);
	m_BtnParam.SetShade(SHS_HARDBUMP,8,20,5,RGB(0,0,255));
	m_BtnParam.SetIcon(IDI_ICON_PARAM,BS_LEFT);
	m_BtnParam.SetFont("黑体",iBtnFontW,400,FALSE,FALSE);
	m_BtnParam.SetTextColor(RGB(0x0,0x72,0xc6));


	// 模板管理
	rcBtnMoudle.left = rcBtnStart.left;
	rcBtnMoudle.right = rcBtnStart.right;
	rcBtnMoudle.top = rcBtnStart.bottom+iBtnTop;
	rcBtnMoudle.bottom = rcBtnMoudle.top+iBtnH;
	m_BtnModel.MoveWindow(rcBtnMoudle);
	m_BtnModel.SetShade(SHS_HARDBUMP,8,20,5,RGB(0,0,255));
	m_BtnModel.SetIcon(IDI_ICON_MODEL,BS_LEFT);
	m_BtnModel.SetFont("黑体",iBtnFontW,400,FALSE,FALSE);
	m_BtnModel.SetTextColor(RGB(0x0,0x72,0xc6));


	// 检测统计
	rcBtnStatistic.left = rcBtnMoudle.right+iBtnLeft;
	rcBtnStatistic.right = rcBtnStatistic.left+iBtnW;
	rcBtnStatistic.top = rcBtnMoudle.top;
	rcBtnStatistic.bottom = rcBtnStatistic.top+iBtnH;
	m_BtnStatistic.MoveWindow(rcBtnStatistic);
	m_BtnStatistic.SetShade(SHS_HARDBUMP,8,20,5,RGB(0,0,255));
	m_BtnStatistic.SetIcon(IDI_ICON_STATICTIC,BS_LEFT);
	m_BtnStatistic.SetFont("黑体",iBtnFontW,400,FALSE,FALSE);
	m_BtnStatistic.SetTextColor(RGB(0x0,0x72,0xc6));


	// 开始记录
	rcRecord.left = rcBtnStart.left;
	rcRecord.right = rcBtnStart.right;
	rcRecord.top = rcBtnStatistic.bottom+iBtnTop;
	rcRecord.bottom = rcRecord.top+iBtnH;
	m_BtnRecord.MoveWindow(rcRecord);
	m_BtnRecord.SetShade(SHS_HARDBUMP,8,20,5,RGB(0,0,255));
	m_BtnRecord.SetIcon(IDI_ICON_RECORD,BS_LEFT);
	m_BtnRecord.SetFont("黑体",iBtnFontW,400,FALSE,FALSE);
	m_BtnRecord.SetTextColor(RGB(0x0,0x72,0xc6));


	// 缺陷统计
	rcDefectStatistic.left = rcRecord.right+iBtnLeft;
	rcDefectStatistic.right = rcDefectStatistic.left+iBtnW;
	rcDefectStatistic.top = rcRecord.top;
	rcDefectStatistic.bottom = rcRecord.bottom;
	m_BtnDefectStatistic.MoveWindow(rcDefectStatistic);
	m_BtnDefectStatistic.SetShade(SHS_HARDBUMP,8,20,5,RGB(0,0,255));
	m_BtnDefectStatistic.SetIcon(IDI_ICON_DEFECT_STATISTIC,BS_LEFT);
	m_BtnDefectStatistic.SetFont("黑体",iBtnFontW,400,FALSE,FALSE);
	m_BtnDefectStatistic.SetTextColor(RGB(0x0,0x72,0xc6));


	// 计数清零
	rcZero.left = rcBtnStart.left;
	rcZero.right = rcBtnStart.right;
	rcZero.top = rcRecord.bottom+iBtnTop;
	rcZero.bottom = rcZero.top+iBtnH;
	m_BtnZero.MoveWindow(rcZero);
	m_BtnZero.SetShade(SHS_HARDBUMP,8,20,5,RGB(0,0,255));
	m_BtnZero.SetIcon(IDI_ICON_ZERO,BS_LEFT);
	m_BtnZero.SetFont("黑体",iBtnFontW,400,FALSE,FALSE);
	m_BtnZero.SetTextColor(RGB(0x0,0x72,0xc6));


	// 退出程序
	rcQuit.left = rcZero.right+iBtnLeft;
	rcQuit.right = rcQuit.left+iBtnW;
	rcQuit.top = rcZero.top;
	rcQuit.bottom = rcQuit.top+iBtnH;
	m_BtnQuit.MoveWindow(rcQuit);
	m_BtnQuit.SetShade(SHS_HARDBUMP,8,20,5,RGB(0,0,255));
	m_BtnQuit.SetIcon(IDI_ICON_QUIT,BS_LEFT);
	m_BtnQuit.SetFont("黑体",iBtnFontW,400,FALSE,FALSE);
	m_BtnQuit.SetTextColor(RGB(0x0,0x72,0xc6));


	m_TotalLabel.m_pMainFont->SetHeight(m_iLabelFontHeight);
	m_TotalLabel.SetMainTextColour(RGB(0,0,0));
	m_TotalLabel.Display("检测总数:","",0,FALSE);
	m_TotalData.m_pMainFont->SetHeight(m_iLabelFontHeight);
	m_TotalData.SetMainTextColour(RGB(0,0,255));
	m_TotalData.Display("0","",0,FALSE);

	m_TotalGoodLabel.m_pMainFont->SetHeight(m_iLabelFontHeight);
 	m_TotalGoodLabel.SetMainTextColour(RGB(0,0,0));
	m_TotalGoodLabel.Display("合格品数:","",0,FALSE);
	m_TotalGoodData.m_pMainFont->SetHeight(m_iLabelFontHeight);
	m_TotalGoodData.SetMainTextColour(RGB(0,0,255));
	m_TotalGoodData.Display("0","",0,FALSE);

	m_TotalGoodRatioLabel.m_pMainFont->SetHeight(m_iLabelFontHeight);
	m_TotalGoodRatioLabel.SetMainTextColour(RGB(0,0,0));
	m_TotalGoodRatioLabel.Display("合格品率:","",0,FALSE);
	m_TotalGoodRatioData.m_pMainFont->SetHeight(m_iLabelFontHeight);
	m_TotalGoodRatioData.SetMainTextColour(RGB(0,0,255));
	m_TotalGoodRatioData.Display("0.00%","",0,FALSE);

	m_ModelNameLabel.m_pMainFont->SetHeight(m_iLabelFontHeight);
	m_ModelNameLabel.SetMainTextColour(RGB(0,0,0));
	m_ModelNameLabel.Display("模板名称:","",0,FALSE);
	m_ModelNameData.m_pMainFont->SetHeight(m_iLabelFontHeight);
	m_ModelNameData.SetMainTextColour(RGB(0,0,255));
	m_ModelNameData.Display(pFrame->m_sModelName.GetBuffer(pFrame->m_sModelName.GetLength()),"",0,FALSE);

	//*[7/6/2017 ConanSteve]*:  
	CString sAppPath{ TEXT("") };
	GetModuleFileName(AfxGetInstanceHandle(), sAppPath.GetBuffer(256), 256);
	sAppPath.ReleaseBuffer();
	sAppPath = sAppPath.Mid(0, sAppPath.ReverseFind(TEXT('\\')));
	CString sAlgPath = sAppPath + TEXT("\\Config\\AlgConfig.ini");
	CString sLanguage{ TEXT("") };
	if (PathFileExists(sAlgPath))
	{
		GetPrivateProfileString(TEXT("Demo Controls"), TEXT("Language"), TEXT(""), sLanguage.GetBuffer(256), 256, sAlgPath);
		sLanguage.ReleaseBuffer();
		//InitStringTable(sLanguage);
	}

	m_SpeedLabel.m_pMainFont->SetHeight(m_iLabelFontHeight);
	m_SpeedLabel.SetMainTextColour(RGB(0,0,0));
	m_SpeedLabel.Display(tr("检测速度:"),"",0,FALSE);
	m_SpeedData.m_pMainFont->SetHeight(m_iLabelFontHeight);
	m_SpeedData.SetMainTextColour(RGB(0,0,255));
	m_SpeedData.Display("0个/分","",0,FALSE);

	return TRUE;
}

// 参数设置
void CTubeCheckView::OnBtnParam() 
{
	// TODO: Add your control notification handler code here
// 	if(g_pFrame->m_bIsDetecting==TRUE)
// 	{
// 		if(IDYES!=AfxMessageBox("停止检测后,才可进入参数设置!\r\n是否停止检测?",MB_YESNO))
// 			return;
// 		else
// 		{
// 			GetDlgItem(IDC_BTN_START)->SetWindowText(_T("开始检测"));
// 			m_BtnStart.SetIcon(IDI_ICON_START,BS_LEFT);
// 			g_pFrame->StopCheck();
// 		}
// 	}
	if (!g_pFrame->m_threeTierRights.Login())
		return;
	int i{ 0 };
	CString strVirtualDiskPath;
	strVirtualDiskPath.Format("%s\\Config\\AlgConfig.ini",g_pFrame->m_sWorkPath);
	WritePrivateProfileString("DEMO Controls","DefaultModel",g_pFrame->m_sModelName,strVirtualDiskPath);
// 	strVirtualDiskPath.Format("%s\\Config\\CheckSystemConfig.ini",g_pFrame->m_sWorkPath);
// 	WritePrivateProfileString("系统配置","上一次使用算法模板名称",g_pFrame->m_sModelName,strVirtualDiskPath);

	CString strDemo;
#ifdef DEBUG
	strDemo.Format("%s\\DEMOD.exe", g_pFrame->m_sWorkPath);
#else
	
	strDemo.Format("%s\\DEMO.exe", g_pFrame->m_sWorkPath);
#endif

	if (g_pFrame->m_threeTierRights.VerifyRights(e_UserType_Debugger))
		strDemo += _T(" Save:Y");

	CWnd* pWnd = NULL;
	pWnd = CWnd::FindWindow(NULL,g_pFrame->m_sDemoName);
	
	if(NULL!=pWnd)
	{
		pWnd->SetForegroundWindow();
		return;
	}

	int iImageWidth[CAMERA_COUNT]={0};
	int iImageHeight[CAMERA_COUNT]={0};
	int iImageByte[CAMERA_COUNT]={0};
	int iImageSize[CAMERA_COUNT]={0};
	
	for(int i=0;i<CAMERA_COUNT;i++)
	{
		iImageWidth[i] = g_pFrame->m_sCameraImageInfo[i].m_iCameraImageWidth;
		iImageHeight[i] = g_pFrame->m_sCameraImageInfo[i].m_iCameraImageHeight;
		iImageByte[i] = g_pFrame->m_sCameraImageInfo[i].m_iCameraImagePixelSize;
		iImageSize[i]=  g_pFrame->m_sCameraImageInfo[i].m_iCameraImageSize;
	}
	
	
// 	if(FALSE==g_pFrame->m_bAdvMode)
// 	{
// 		CDlgTestCipher dlg;
// 		if(dlg.DoModal() == IDOK && dlg.m_bOK == TRUE)
// 		{
// 			g_pFrame->m_bAdvMode = TRUE;
// 			GetDlgItem(IDC_ADV_MODE)->ShowWindow(SW_SHOW);
// 		}
// 		else
// 		{
// 			return;
// 		}
// 	}

	CString str1,str2;
//	HINSTANCE hRet= ShellExecute(NULL,"open",strDemo,TEXT("Save:Y"),NULL,SW_SHOWNORMAL);	

	WinExec(strDemo, SW_SHOW);

	// 保存图像
	for(i=0;i<CAMERA_COUNT;i++)
	{
		str1.Format("%s:\\SaveImages\\AlgImages\\Camera%d",g_pFrame->m_sVirtualDisk,i+1);
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
//		g_pFrame->savebmp(str2,iImageWidth[i],iImageHeight[i],iImageByte[i],g_pFrame->m_pAlgElement[i]);
	}
}
// 模板管理
void CTubeCheckView::OnBtnMouldAdm() 
{
	// TODO: Add your control notification handler code here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	if(pFrame->m_bIsDetecting==TRUE)
	{
		if(IDYES!=AfxMessageBox("停止检测后,才可进入模板管理!\r\n是否停止检测？",MB_YESNO))
		{
			return;
		}
		else
		{
			GetDlgItem(IDC_BTN_START)->SetWindowText(_T("开始检测"));
			m_BtnStart.SetIcon(IDI_ICON_START,BS_LEFT);
			g_pFrame->StopCheck();
		}
	}

// 	if(FALSE==g_pFrame->m_bAdvMode)
// 	{
// 		CDlgTestCipher dlg;
// 		if(dlg.DoModal() == IDOK && dlg.m_bOK == TRUE)
// 		{
// 			g_pFrame->m_bAdvMode = TRUE;
// 			GetDlgItem(IDC_ADV_MODE)->ShowWindow(SW_SHOW);
// 		}
// 		else
// 		{
// 			return;
// 		}
// 	}

	CDlgSelectProduct dlg;
	dlg.m_strAlgIniPath=g_pFrame->m_sAlgIniFilePath;
	dlg.DoModal();

}
// 检测统计
void CTubeCheckView::OnBtnStatistic() 
{
	// TODO: Add your control notification handler code here
	CDlgStatistic dlg;
	dlg.Init(g_pFrame->m_Connect,g_pFrame->m_RecordSet);
	dlg.DoModal();

}

// 开始检测
void CTubeCheckView::OnBtnStart() 
{
	// TODO: Add your control notification handler code here
	CString strText=_T("");
	GetDlgItem(IDC_BTN_START)->GetWindowText(strText);
	
	if (strText==_T("开始检测"))
	{
		GetDlgItem(IDC_BTN_START)->SetWindowText(_T("停止检测"));
		m_BtnStart.SetIcon(IDI_ICON_STOP,BS_LEFT);
		g_pFrame->StartCheck();
	}
	else
	{
		if (AfxMessageBox("确定要停止检测吗?", MB_YESNO) != IDYES) 
		{
			return ;
		}
		
		GetDlgItem(IDC_BTN_START)->SetWindowText(_T("开始检测"));
		m_BtnStart.SetIcon(IDI_ICON_START,BS_LEFT);
		g_pFrame->StopCheck();
	}
	
	UpdateData(FALSE);
}

void CTubeCheckView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == ViewUpdateTimer)
	{
		//获取全局类指针
		g_pView = this;
		g_pDoc  = GetDocument();
		Invalidate(TRUE);	
		KillTimer(ViewUpdateTimer); //关闭定时器	
	}
	else if (nIDEvent == TimeTimer)
	{
		CTime  curTime = CTime::GetCurrentTime();
		int  iYear = curTime.GetYear();
		int  iMonth = curTime.GetMonth();
		int  iDay = curTime.GetDay();
		int  iHour = curTime.GetHour();
		int  iMinute = curTime.GetMinute();
		int  iSecond = curTime.GetSecond();
		CString strTime;
		strTime.Format("%d-%d-%d",iYear,iMonth,iDay);
		SetDlgItemText(IDC_STATIC_YEAR,strTime);
		strTime.Format("%02d:%02d:%02d",iHour,iMinute,iSecond);
		SetDlgItemText(IDC_STATIC_HOUR,strTime);
	}


	CFormView::OnTimer(nIDEvent);
}


void CTubeCheckView::AddRecordToListCtrl(int iCamGroup, int iImageSN, int iCircleSN, CString strSaveTime)
{
	CString strShow;
	CString strIndex;
	CString strCamGroup;
	CString strCircleNum;

	strIndex.Format("%d",m_iIndex+1);
	strCamGroup.Format("%d",iCamGroup+1);
	strCircleNum.Format("%d",iImageSN);
	
	strShow = g_pFrame->m_strErrorDesc[iCamGroup][iCircleSN%256];
	g_pFrame->m_strErrorDesc[iCamGroup][iCircleSN%256]="";
	
	// 目前列表框控件内规定最大记录个数，若记录数少于最大值，直接加到列表控件
	
	if(Camera1==iCamGroup)
	{
		if(m_dlgPage1.m_iIndex/(g_pFrame->m_iMaxSaveImageCount) == 0)
		{
			strIndex.Format("%d",m_dlgPage1.m_iIndex+1);
			m_dlgPage1.m_lscInfo.InsertItem(m_dlgPage1.m_iIndex,strIndex);
			m_dlgPage1.m_lscInfo.SetItemText(m_dlgPage1.m_iIndex,1,strCircleNum);
			m_dlgPage1.m_lscInfo.SetItemText(m_dlgPage1.m_iIndex,2,strShow);
			m_dlgPage1.m_lscInfo.SetItemText(m_dlgPage1.m_iIndex,3,strSaveTime);
			m_dlgPage1.m_lscInfo.EnsureVisible(m_dlgPage1.m_iIndex,TRUE);
			m_dlgPage1.m_iIndex++;
		} 
		else if(m_dlgPage1.m_iIndex/(g_pFrame->m_iMaxSaveImageCount) > 0) // 若记录数大于允许的最大保存张数，则删除最先存入的记录，加入最新记录
		{
			int nTemp = m_dlgPage1.m_iIndex%(g_pFrame->m_iMaxSaveImageCount);
			CString strTemp;
			strTemp.Format("%d",m_dlgPage1.m_iIndex+1);
			m_dlgPage1.m_lscInfo.DeleteItem(nTemp);
			m_dlgPage1.m_lscInfo.InsertItem(nTemp,strTemp);
			m_dlgPage1.m_lscInfo.SetItemText(nTemp,0,strTemp);
			m_dlgPage1.m_lscInfo.SetItemText(nTemp,1,strCircleNum);
			m_dlgPage1.m_lscInfo.SetItemText(nTemp,2,strShow);		
			m_dlgPage1.m_lscInfo.SetItemText(nTemp,3,strSaveTime);
			m_dlgPage1.m_lscInfo.EnsureVisible(nTemp,TRUE);
			m_dlgPage1.m_iIndex++;
		}
	}
}


BOOL CTubeCheckView::LoadLightAndProductInfo(CString sModelName,CString sOldModelName)
{
	CMainFrame* pFrame = g_pFrame;

	CString strBatchModify,strOriBatchCode,strCurrBatchCode;
	CString strCode,strSize,strFormat,strBrand,strColor,strAluminumMaterial,strMouldNum;
	CString strModelInfoIniFile;
	CFileFind find;


	strOriBatchCode="无";
	strModelInfoIniFile.Format("%s\\%s\\1\\ModelParas.ini",pFrame->m_sAlgIniFilePath,sModelName);
	strCurrBatchCode=strOriBatchCode;

	
	// 获取各光源亮度
	int i1=0,i2=0,i3=0,i4=0;
	CString str1,str2,str3,str4;

	if(pFrame->m_bReadModel)
	{

 		WritePrivateProfileString("系统配置", "上一次使用算法模板名称", sModelName, pFrame->m_sSysConfigFileName);
		pFrame->m_bCalAlg=TRUE;

	}
	else
	{
		pFrame->m_bCalAlg=FALSE;

	}



	// 更新界面模板名称
	pFrame->m_sModelName = sModelName;
	m_ModelNameData.Display(sModelName.GetBuffer(sModelName.GetLength()),"",0,FALSE);

	if(sModelName!=sOldModelName)
	{
		pFrame->SoftResetIOCard(sOldModelName);
	}
	else
	{
		if(strOriBatchCode!=strCurrBatchCode)
		{
			pFrame->SoftResetIOCard(sOldModelName);
		}
	}

	if(FALSE==pFrame->m_bReadModel)
	{
		str1.Format("主程序加载模板'%s'失败!\n目前只能采集图像,不能检测图像!",sModelName);
		AfxMessageBox(str1);
	}

	return TRUE;
}

//显示图像
void CTubeCheckView::DisplayImage(const int iCamGroup, int iIndex)
{

	CDC* pDC = GetDC();
	
	if (g_pFrame!=NULL || g_pFrame->m_bIsDetecting==FALSE)
	{
		if(0==g_pFrame->m_iShowDevice)
			DisplayCameraImage(pDC,iCamGroup,iIndex, NoFlip);
	}
	ReleaseDC(pDC);
	
}

void CTubeCheckView::DisplayCameraImage(CDC *pDC,  const int iCamGroup, int iIndex, e_ShowModeFlip eShowMode)
{
	int i=0,j=0;
	int iImageWidth=0, iImageHeight=0, iImageByte=0, iImageSize=0;
	int  iDisplayLeft = 0, iDisplayTop = 0;
	CString  strShow;

	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	pDC->SelectObject(pBrush);
	pDC->SetTextColor(RGB(0,255,255));  //设置文本颜色
	pDC->SetBkMode(TRANSPARENT);        //将文本背景设为透明的

	CMainFrame * pFrame = g_pFrame;	


	if (pFrame == NULL)
	{
		return;
	}

	try
	{								
		iImageWidth = pFrame->m_sCameraImageInfo[iCamGroup].m_iCameraImageWidth;
		iImageHeight = pFrame->m_sCameraImageInfo[iCamGroup].m_iCameraImageHeight;
		iImageByte = pFrame->m_sCameraImageInfo[iCamGroup].m_iCameraImagePixelSize;
		iImageSize = pFrame->m_sCameraImageInfo[iCamGroup].m_iCameraImageSize;
		int iOffset = iIndex*iImageSize*pFrame->m_iPicCountPerCam[iCamGroup];
		
		ShowDIBImage(pDC,m_rectShowImage[iCamGroup],eShowMode,Stretch,
			pFrame->m_pImageAddr[iCamGroup].get() +iOffset,
			iImageWidth,iImageHeight,iImageByte*8,iCamGroup);

		
		iDisplayLeft = m_rectShowImage[iCamGroup].left;
		iDisplayTop = m_rectShowImage[iCamGroup].top;
		
		strShow.Format("第%d工位相机",iCamGroup+1);
		pDC->TextOut(iDisplayLeft,iDisplayTop,strShow);
		
		strShow.Format("第%d路",iCamGroup+1);
		pDC->TextOut(iDisplayLeft,iDisplayTop+20,strShow);
		
		strShow.Format("采集大张数-%d",pFrame->m_sElement[iCamGroup][iIndex].iImageSN);		
		pDC->TextOut(iDisplayLeft,iDisplayTop+40,strShow);
		
		pDC->SetTextColor(RGB(255,0,0));  //设置文本颜色
		strShow.Format("%s",pFrame->m_sElement[iCamGroup][iIndex].strErrorInfo);
		pDC->TextOut(iDisplayLeft,iDisplayTop+60,strShow);			

	}
	catch (...)
	{
		CString  strExceptionDes;
		strExceptionDes.Format("DisplayCameraImageOne-相机%d出错!",iCamGroup+1);
		pFrame->m_LogManager.MyWriteLogFile("图像显示",strExceptionDes,AbnormityLog);
	}
}
void CTubeCheckView::ShowDIBImage(CDC* pDC, CRect Rect, e_ShowModeFlip ShowModeFlip, e_ShowModeStretch ShowModeStretch,
								   const BYTE* pImageBuff, long lImageWidth, long lImageHeight, long lImageBitCount, int iCameraSN)
{
	int XDest = 0;
	int YDest = 0;
	int nDestWidth = 0;
	int nDestHeight = 0;
	
	ASSERT(pImageBuff != NULL && lImageWidth >= 0 && lImageHeight >= 0 && lImageBitCount > 0);
	
	m_csShowBitmapInfoSection.Lock();
	
	ScreenToClient(&Rect);
	
	XDest = Rect.left;
	YDest = Rect.top;
	nDestWidth = Rect.Width();
	nDestHeight = Rect.Height();
	
	if(ShowModeStretch == NoStretch)
	{		
		SetDIBitsToDevice(pDC->GetSafeHdc(), 
			XDest, // destx 
			YDest, // desty 
			nDestWidth,				//显示窗口宽度
			nDestHeight,			//显示窗口高度
			0, // srcx 
			0, // srcy 
			0, // nstartscan 
			lImageHeight, // nnumscans 
			pImageBuff, // lpbits 
			g_pFrame->m_pBitmapInfo[iCameraSN], // lpbitsinfo 
			DIB_RGB_COLORS); // wusage 
	}
	else
	{
		switch(ShowModeFlip) 
		{
		case NoFlip:			//不翻转
			break;
		case FlipHorizintal:	//水平翻转
			XDest = Rect.right;
			nDestWidth = -nDestWidth;
			break;
		case FlipVertical:		//垂直翻转
			YDest = Rect.bottom;
			nDestHeight = -nDestHeight;
			break;
		case Flip:				//全翻转
			XDest = Rect.right;
			YDest = Rect.bottom;
			nDestWidth = -nDestWidth;
			nDestHeight = -nDestHeight;
			break;
		default:				//不翻转
			XDest = Rect.left;
			YDest = Rect.top;
		}
		
		SetStretchBltMode(pDC->m_hDC, COLORONCOLOR);
		
		StretchDIBits(pDC->GetSafeHdc(),
			XDest,
			YDest,	
			nDestWidth,				//显示窗口宽度
			nDestHeight,			//显示窗口高度
			0,
			0,
			lImageWidth,			//图像宽度
			lImageHeight,			//图像高度
			pImageBuff,				//图像缓冲区
			g_pFrame->m_pBitmapInfo[iCameraSN],			//BMP图像描述信息
			DIB_RGB_COLORS,
			SRCCOPY
			);
	}
	
	m_csShowBitmapInfoSection.Unlock();
}

void CTubeCheckView::OnBtnRecord() 
{
	// TODO: Add your control notification handler code here
	int i=0;
	CString strName,strBasic;
	strBasic.Format("%s:\\LogFiles\\GrabLog",g_pFrame->m_sVirtualDisk);
	
	if(FALSE==g_pFrame->m_bStartRecord)
	{

		for(i=0;i<c_AllElementCount;i++)
		{
			g_pFrame->m_filePE[i].Close();
			strName.Format("%s\\Guangdian%d.txt",strBasic,i+1);
			g_pFrame->m_filePE[i].Open(strName,CFile::modeCreate|CFile::modeReadWrite|CFile::shareDenyNone);
			g_pFrame->m_iRC_PE[i]=0;
		}
				
		for(i=0;i<c_AllElementCount;i++)
		{
			
			strName.Format("%s\\Suanfashijian%d.txt",strBasic,i+1);
			g_pFrame->m_fileDetectTime[i].Close();
			g_pFrame->m_fileDetectTime[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
			g_pFrame->m_iRC_DetectTime[i]=0;
			
			strName.Format("%s\\Huidiao%d.txt",strBasic,i+1);
			g_pFrame->m_fileCall[i].Close();
			g_pFrame->m_fileCall[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
			g_pFrame->m_iRC_Call[i]=0;
			
			strName.Format("%s\\Thread_Bad%d.txt",strBasic,i+1);
			g_pFrame->m_fileThreadBad[i].Close();
			g_pFrame->m_fileThreadBad[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
			g_pFrame->m_iRC_ThreadBad[i]=0;
			
			strName.Format("%s\\Thread_Good%d.txt",strBasic,i+1);
			g_pFrame->m_fileThreadGood[i].Close();
			g_pFrame->m_fileThreadGood[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
			g_pFrame->m_iRC_ThreadGood[i]=0;
			
			strName.Format("%s\\Kick_Good%d.txt",strBasic,i+1);
			g_pFrame->m_fileGood[i].Close();
			g_pFrame->m_fileGood[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
			g_pFrame->m_iRC_Good[i]=0;
			
			strName.Format("%s\\Tifei%d.txt",strBasic,i+1);
			g_pFrame->m_fileKickImage[i].Close();
			g_pFrame->m_fileKickImage[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
			g_pFrame->m_iRC_KickImage[i]=0;
			
			strName.Format("%s\\Buti%d.txt",strBasic,i+1);
			g_pFrame->m_fileAddKickImage[i].Close();
			g_pFrame->m_fileAddKickImage[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
			g_pFrame->m_iRC_AddKickImage[i]=0;
			
			strName.Format("%s\\Call_Mid%d.txt",strBasic,i+1);
			g_pFrame->m_fileImageNum[i].Close();
			g_pFrame->m_fileImageNum[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
			g_pFrame->m_iRC_ImageNum[i]=0;
			
			strName.Format("%s\\CheckThread_Begin%d.txt",strBasic,i+1);
			g_pFrame->m_fileImage[i].Close();
			g_pFrame->m_fileImage[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
			g_pFrame->m_iRC_Image[i]=0;
			
			strName.Format("%s\\Jianceqian%d.txt",strBasic,i+1);
			g_pFrame->m_fileBeforeD[i].Close();
			g_pFrame->m_fileBeforeD[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
			g_pFrame->m_iRC_BeforeD[i]=0;
			
			strName.Format("%s\\Baocuo%d.txt",strBasic,i+1);
			g_pFrame->m_fileDetect[i].Close();
			g_pFrame->m_fileDetect[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
			g_pFrame->m_iRC_Detect[i]=0;
			
			strName.Format("%s\\Kick_Bad%d.txt",strBasic,i+1);
			g_pFrame->m_fileKick[i].Open(strName,&g_pFrame->m_bStartRecord, CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
			g_pFrame->m_iRC_Kick[i]=0;
			
			strName.Format("%s\\Tuxiangjicunqi%d.txt",strBasic,i+1);
			g_pFrame->m_fileRecordImage[i].Close();
			g_pFrame->m_fileRecordImage[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
			g_pFrame->m_iRC_RecordImage[i]=0;
			
			
			strName.Format("%s\\Call_SetEvent%d.txt",strBasic,i+1);
			g_pFrame->m_fileSetEvent[i].Close();
			g_pFrame->m_fileSetEvent[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
			g_pFrame->m_iRC_SetEvent[i]=0;
			
			strName.Format("%s\\Call_WriteImage%d.txt",strBasic,i+1);
			g_pFrame->m_fileWriteImage[i].Close();
			g_pFrame->m_fileWriteImage[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
			g_pFrame->m_iRC_WriteImage[i]=0;


			strName.Format("%s\\StopMachine%d.txt",strBasic,i+1);
			g_pFrame->m_fileStopMachine[i].Close();
			g_pFrame->m_fileStopMachine[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
			g_pFrame->m_iRC_StopMachine[i]=0;

			strName.Format("%s\\RecordKick%d.txt",strBasic,i+1);
			g_pFrame->m_fileRecordKick[i].Close();
			g_pFrame->m_fileRecordKick[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
			g_pFrame->m_iRC_RecordKick[i]=0;

			SetDlgItemText(IDC_BTN_RECORD, "停止记录");

			g_pFrame->m_bStartRecord = TRUE;
		}
	}
	else
	{
		SetDlgItemText(IDC_BTN_RECORD,"开始记录");
		g_pFrame->m_bStartRecord=FALSE;
	}
}

HBRUSH CTubeCheckView::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);


	// TODO: Change any attributes of the DC here

	switch(pWnd->GetDlgCtrlID())
	{
		case IDC_WINDOW_TITLE:
		{
			pDC->SetTextColor(RGB(0,0,255));
			pDC->SetBkMode(TRANSPARENT);
			break;
		}
		case IDC_SAVE_IMAGE:
		{
			pDC->SetTextColor(RGB(33,209,209));
			pDC->SetBkMode(TRANSPARENT);
			break;
		}
		case IDC_STOP_SAVE_IMAGE:
			{
				pDC->SetTextColor(RGB(33,209,209));
				pDC->SetBkMode(TRANSPARENT);
				break;
		}
		case IDC_SYSTEM_CONFIG:
		{
			pDC->SetTextColor(RGB(33,209,209));
			pDC->SetBkMode(TRANSPARENT);
			break;
		}
		case IDC_CAM_TEST:
		{
			pDC->SetTextColor(RGB(33,209,209));
			pDC->SetBkMode(TRANSPARENT);
			break;
		}
		case IDC_IOCARD:
		{
			pDC->SetTextColor(RGB(33,209,209));
			pDC->SetBkMode(TRANSPARENT);
			break;
		}
		case IDC_ADV_MODE:
		{
			pDC->SetTextColor(RGB(255,0,0));
			pDC->SetBkMode(TRANSPARENT);
			break;
		}
		default:
		{
			pDC->SetTextColor(RGB(0,0,0));
			pDC->SetBkMode(TRANSPARENT);
		}
			
	}

	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CTubeCheckView::OnSaveImage() 
{
	// TODO: Add your control notification handler code here

	// 判断M盘下是否有图像
	BOOL bHaveImage=FALSE;

	CDlgSaveImage dlg;
	if(IDOK==dlg.DoModal())
	{
		m_iIsCircle = dlg.m_iIsCircle;
		m_iSaveImageType = dlg.m_iSaveType;
		m_iSaveImageCount = m_iSaveImageType?dlg.m_iErrorCount:dlg.m_iGrabCount;

		CTime curTime=CTime::GetCurrentTime();

		if(m_iSaveImageCount>0)
		{
			m_iIndex = 0;

			// 删除图像
			CString str;
			BOOL bRet = FALSE;
			str.Format("%s:\\SaveImages",g_pFrame->m_sVirtualDisk);
			str+='\0';
			
			if(TRUE==g_pFrame->inIsPathExist(str))
			{
				//bRet = g_pFrame->OP_Directory(str,NULL,FO_DELETE,FOF_NOCONFIRMATION | FOF_SIMPLEPROGRESS |FOF_NOERRORUI);
				bRet =CExFn::DeleteDirectory(str.GetBuffer(256));
			}

			for(int i=0;i<CAMERA_COUNT;i++)
			{
				g_pFrame->m_iOriginSaveImageNum[i]=0;
				g_pFrame->m_iSaveCameraImageCount[i] = 0;
				g_pFrame->m_iSaveCameraImageCount_Setting[i] = m_iSaveImageCount;
				g_pFrame->m_iSaveCircleCameraImageCount[i]=0;
				g_pFrame->m_iSaveImageType = m_iSaveImageType;
				
				if(SaveGrabImage==m_iSaveImageType)
				{
					g_pFrame->PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,ContinueImageSaving);
				}
				else
				{
					g_pFrame->PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,ErrorImageSaving);
				}


				switch (g_pFrame->m_iSaveImageType)
				{
				case 0:
				{
					g_pFrame->m_sSaveImagePath[i].Format("%s:\\%s\\%s\\%04d-%02d-%02d\\Camera%d",
						g_pFrame->m_sVirtualDisk,
						"SaveImages", "GrabImages",
						curTime.GetYear(),
						curTime.GetMonth(),
						curTime.GetDay(),
						i + 1);
				}
					break;
				case 1:
				{
					g_pFrame->m_sSaveImagePath[i].Format("%s:\\%s\\%s\\%04d-%02d-%02d\\Camera%d",
						g_pFrame->m_sVirtualDisk,
						"SaveImages", "ErrorImages",
						curTime.GetYear(),
						curTime.GetMonth(),
						curTime.GetDay(),
						i + 1);
				}
					break;
				case 2:
				{
					g_pFrame->m_sSaveImagePath[i].Format("%s:\\%s\\%s\\%04d-%02d-%02d\\Camera%d",
						g_pFrame->m_sVirtualDisk,
						"SaveImages", "ModelImages",
						curTime.GetYear(),
						curTime.GetMonth(),
						curTime.GetDay(),
						i + 1);
				}
					break;
				default:
					break;
				}
				// 创建文件夹
				if(!g_pFrame->inIsPathExist(g_pFrame->m_sSaveImagePath[i]))
				{
					//if(!g_pFrame->inCreateMultiLevelPath(g_pFrame->m_sSaveImagePath[i]))
					if (!CExFn::CreateMultiDirectory(g_pFrame->m_sSaveImagePath[i]))
					{
						CString str;
						str.Format("创建文件夹失败:%s",g_pFrame->m_sSaveImagePath[i]);
						AfxMessageBox(str);
					}
				}

				// 设置保存图像
				s_AlgModelInput sIn;
				sIn.iCheckSN = i;
				sIn.iOperSig = 301;
				sIn.iReserve1=m_iSaveImageType;
				sIn.iReserve2=m_iSaveImageCount;
				sIn.iReserve3 = g_pFrame->m_iSaveImgObj;
				sIn.iReserve4 = 0;
				g_pFrame->m_InspectionObj.Set(sIn);

			}
		}
	}

}

void CTubeCheckView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd* pWnd = GetDlgItem(IDC_SAVE_IMAGE);

	if(NULL!=pWnd)
	{
		CRect rcTemp;
		pWnd->GetWindowRect(&rcTemp);
		ScreenToClient(&rcTemp);
		BOOL bIn = FALSE;
		bIn = rcTemp.PtInRect(point);

		if(TRUE==bIn)
		{
			pWnd->GetDC()->SetTextColor(RGB(255,0,0));
			pWnd->Invalidate();
			pWnd->UpdateWindow();
		}
	}
	CFormView::OnMouseMove(nFlags, point);
}

void CTubeCheckView::OnIocard() 
{
	// TODO: Add your control notification handler code here
	CDlgIOCard dlg;
	dlg.DoModal();
}

void CTubeCheckView::OnSystemConfig() 
{
	// TODO: Add your control notification handler code here

	int iParam1=0,iNewParam1=0;
	BOOL bKick[c_AllElementCount];
	bKick[Camera1]= g_pFrame->m_bIsKick[Camera1];

	
//  	CWindowShadow xdlg;
// 	xdlg.Init(this->m_hWnd);
// 	int nResult = XModalWnd_DoModal(xdlg.m_hWindow); 
// 	XDlgSysCfg xdlgCfg(this->m_hWnd);
// 	xdlgCfg.Create(NULL);
// 	xdlgCfg.DoModal();
//  	return;
// 	if (!g_pFrame->m_bOpenXCWnd)
// 		return;
// 	wstring wsPath = g_pFrame->m_wsResPath + L"layout_SystemConfig.xml";

	CDlgSystemConfig dlg;

	if(IDOK==dlg.DoModal())
	{
		g_pFrame->m_BayerTransform[Camera1].m_bAreaCamBayer = dlg.m_bBayer;
		g_pFrame->m_BayerTransform[Camera1].m_dBayerRedGain = dlg.m_dRed;
		g_pFrame->m_BayerTransform[Camera1].m_dBayerGreenGain= dlg.m_dGreen;
		g_pFrame->m_BayerTransform[Camera1].m_dBayerBlueGain=dlg.m_dBlue;

		g_pFrame->m_BayerTransform[Camera2].m_bAreaCamBayer = dlg.m_bBayer2;
		g_pFrame->m_BayerTransform[Camera2].m_dBayerRedGain = dlg.m_dRed2;
		g_pFrame->m_BayerTransform[Camera2].m_dBayerGreenGain= dlg.m_dGreen2;
		g_pFrame->m_BayerTransform[Camera2].m_dBayerBlueGain=dlg.m_dBlue2;

		g_pFrame->m_BayerTransform[Camera3].m_bAreaCamBayer = dlg.m_bBayer3;
		g_pFrame->m_BayerTransform[Camera3].m_dBayerRedGain = dlg.m_dRed3;
		g_pFrame->m_BayerTransform[Camera3].m_dBayerGreenGain= dlg.m_dGreen3;
		g_pFrame->m_BayerTransform[Camera3].m_dBayerBlueGain=dlg.m_dBlue3;

		g_pFrame->m_BayerTransform[Camera4].m_bAreaCamBayer = dlg.m_bBayer4;
		g_pFrame->m_BayerTransform[Camera4].m_dBayerRedGain = dlg.m_dRed4;
		g_pFrame->m_BayerTransform[Camera4].m_dBayerGreenGain= dlg.m_dGreen4;
		g_pFrame->m_BayerTransform[Camera4].m_dBayerBlueGain=dlg.m_dBlue4;

		g_pFrame->m_BayerTransform[Camera5].m_bAreaCamBayer = dlg.m_bBayer5;
		g_pFrame->m_BayerTransform[Camera5].m_dBayerRedGain = dlg.m_dRed5;
		g_pFrame->m_BayerTransform[Camera5].m_dBayerGreenGain= dlg.m_dGreen5;
		g_pFrame->m_BayerTransform[Camera5].m_dBayerBlueGain=dlg.m_dBlue5;

		g_pFrame->m_BayerTransform[Camera6].m_bAreaCamBayer = dlg.m_bBayer6;
		g_pFrame->m_BayerTransform[Camera6].m_dBayerRedGain = dlg.m_dRed6;
		g_pFrame->m_BayerTransform[Camera6].m_dBayerGreenGain= dlg.m_dGreen6;
		g_pFrame->m_BayerTransform[Camera6].m_dBayerBlueGain=dlg.m_dBlue6;
		
		g_pFrame->m_bIsKick[Camera1] = dlg.m_bKick1;
		g_pFrame->m_bIsKick[Camera2] = dlg.m_bKick2;
		g_pFrame->m_bIsKick[Camera3] = dlg.m_bKick3;
		g_pFrame->m_bIsKick[Camera4] = dlg.m_bKick4;
		g_pFrame->m_bIsKick[Camera5] = dlg.m_bKick5;
		g_pFrame->m_bIsKick[Camera6] = dlg.m_bKick6;

//		g_pFrame->m_iIntervalDisplayTime = dlg.m_iShowTime;
		g_pFrame->m_iImageShowMode= dlg.m_iShowMode;
		g_pFrame->m_iMaxSaveImageCount = dlg.m_iMaxSaveImageCount;

		g_pFrame->m_iKickMode = dlg.m_iKickMode;
		g_pFrame->m_iShowDevice = dlg.m_iShowDevice;
		g_pFrame->m_iSaveImageSys = dlg.m_bSaveImageSys;


// 		if(0==g_pFrame->m_iShowDevice)
// 		{
// 			GetDlgItem(IDC_PIC1)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_PIC2)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_PIC3)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_PIC4)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_PIC5)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_PIC6)->ShowWindow(SW_SHOW);
// 
// 			GetDlgItem(IDC_ALG1)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_ALG2)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_ALG3)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_ALG4)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_ALG5)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_ALG6)->ShowWindow(SW_HIDE);
// 		}
// 		else if(1==g_pFrame->m_iShowDevice)
// 		{
// 			GetDlgItem(IDC_PIC1)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_PIC2)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_PIC3)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_PIC4)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_PIC5)->ShowWindow(SW_HIDE);
// 			GetDlgItem(IDC_PIC6)->ShowWindow(SW_HIDE);
// 
// 			GetDlgItem(IDC_ALG1)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_ALG2)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_ALG3)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_ALG4)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_ALG5)->ShowWindow(SW_SHOW);
// 			GetDlgItem(IDC_ALG6)->ShowWindow(SW_SHOW);
// 
// 		}
	
		//**[7/26/2017 ConanSteve]**:  设置是否显示多对象实时检测状态
		CExtenInfo extenInfo;
		s_AlgModelInput sIn;
		extenInfo.push_back(COptionalFunc(e_Fn_AlgModelInput_WhetherShowObjectRunInfo).SetBool(dlg.m_bOpenObjStatusWnd));
		sIn.iOperSig = e_AlgModelInputOper_UseExtenInfoFunc;
		sIn.iReserve1 = 0;
		sIn.iReserve2 = 0;
		sIn.iReserve3 = 0;
		sIn.iReserve4 = (int)&extenInfo;
		g_pFrame->m_InspectionObj.Set(sIn);
	}
}


void CTubeCheckView::OnAdvMode() 
{
	// TODO: Add your control notification handler code here
	g_pFrame->m_bAdvMode = FALSE;
	GetDlgItem(IDC_ADV_MODE)->ShowWindow(SW_HIDE);
}

void CTubeCheckView::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	int CurSel=m_Tab1.GetCurSel();
	m_dlgPage1.ShowWindow(FALSE);
	
	switch(CurSel)
	{
	case 0:
		m_dlgPage1.ShowWindow(TRUE);
		break;
	default:
		break;
	}
	*pResult = 0;
}


DWORD CTubeCheckView::ShowWarmingInfoThread(LPVOID lpParam)
{
	CTubeCheckView* This = (CTubeCheckView*)lpParam;
	
	DWORD dRet = This->ShowWarmingInfoThreadFunc();
	
	return dRet;
}
DWORD CTubeCheckView::ShowWarmingInfoThreadFunc()
{
	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);

	while(FALSE==m_bQuitAlarmThread)
	{
		for(int i=0;i<c_AllElementCount;i++)
		{
			// 若当前未显示报警信息
			if(FALSE==m_bLabelAlarm[i])
			{
				// 若标签检测报错
				if(TRUE==pFrame->m_bLabelAlarm[i])
				{
					if(Camera1==i)
					{
						m_pdlgAlarmPos->ShowWindow(SW_SHOW);
						m_pdlgAlarmPos->SetForegroundWindow();
						m_pdlgAlarmPos->GetDlgItem(IDOK)->SetFocus();

					}
// 					else if(Camera2==i)
// 					{
// 						// 执行报警动作，显示报警信息
// 						m_pdlgAlarm->ShowWindow(SW_SHOW);
// 						m_pdlgAlarm->SetForegroundWindow();
// 						m_pdlgAlarm->GetDlgItem(IDOK)->SetFocus();
// 
// 					}
					
					pFrame->AlarmAction(TRUE);
					m_bLabelAlarm[i] = TRUE;
				}
			}
		}

		Sleep(10);
	}

	return 0;
}

void CTubeCheckView::OnBtnZero() 
{
	// TODO: Add your control notification handler code here
	if(g_pFrame->m_bIsDetecting==TRUE)
	{
		if(IDYES!=AfxMessageBox("正在检测,停止检测后才可清零!\r\n是否停止检测？",MB_YESNO))
		{
			return;
		}
		else
		{
			GetDlgItem(IDC_BTN_START)->SetWindowText(_T("开始检测"));
			g_pFrame->StopCheck();
		}
	}

	if(IDYES==AfxMessageBox("确定要清零吗?",MB_YESNO))
	{
		g_pFrame->SoftResetIOCard(g_pFrame->m_sModelName);
	}
}

void CTubeCheckView::OnBtnDefectStatistics() 
{
	// TODO: Add your control notification handler code here

	m_pdlgDefect->ShowWindow(SW_SHOW);
}


DWORD CTubeCheckView::MonitorStopCountThread(LPVOID lpParam)
{
	CTubeCheckView* This = (CTubeCheckView*)lpParam;
	
	DWORD dRet = This->MonitorStopCountThreadFunc();
	
	return dRet;
}

DWORD CTubeCheckView::MonitorStopCountThreadFunc()
{

	CMainFrame* pFrame = (CMainFrame*)(AfxGetApp()->m_pMainWnd);

	while(FALSE==m_bQuitAlarmThread)
	{
		// 若当前未报警

		if(FALSE==m_bAlarm_StopCount[Camera1])
		{
			if(TRUE==pFrame->m_bAlarm_StopCount[Camera1])
			{
				m_pdlgAlarmStop->ShowWindow(SW_SHOW);
				m_pdlgAlarmStop->SetForegroundWindow();
				m_pdlgAlarmStop->GetDlgItem(IDOK)->SetFocus();
				pFrame->AlarmAction(TRUE);
				m_bAlarm_StopCount[Camera1] = TRUE;
			}
		}
		Sleep(10);
	}

	return 0;
}

void CTubeCheckView::OnStopSaveImage() 
{
	// TODO: Add your control notification handler code here
	if(g_pFrame->m_bSaveCameraImage[Camera1])
	{
		if(IDYES==AfxMessageBox("正在保存图像，是否停止保存图像!",MB_YESNO))
		{
			for(int i=0;i<CAMERA_COUNT;i++)
			{
				g_pFrame->m_bSaveCameraImage[i] = FALSE;
				g_pFrame->PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,StopSave);		
			}
		}
	}
}

void CTubeCheckView::OnCamTest() 
{
	// TODO: Add your control notification handler code here
	//**[1/16/2018 ConanSteve]**:  改成了用户管理
	if(!g_pFrame->m_threeTierRights.Login(this))
		return;
	g_pFrame->m_threeTierRights.ManageUsers();
// 	for (int i = 0; i < 2; ++i)
// 	{
// 		g_pFrame->m_arr_iCurNumOfConstantKick[i] = 0;
// 	}
// 	g_pFrame->m_IOCard[e_IOCard_2].setCardOutput(g_pFrame->m_iPortOfAlarm, 0);
// 	return;
// 	if(g_pFrame->m_bCheckPushed)
// 		OnBtnStart();
// 	if (g_pFrame->m_bCheckPushed)
// 		return;
// 	CDlgCamTest dlg;
// 	dlg.DoModal();
}


int CTubeCheckView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	return 0;
}
