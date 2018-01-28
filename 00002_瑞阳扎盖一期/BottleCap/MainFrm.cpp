// MainFrm.cpp : implementation of the CMainFrame class
//
#include "stdafx.h"
#include "BottleCap.h"
#include "MainFrm.h"
#include "BottleCapDoc.h"
#include "BottleCapView.h"
#include "DlgLogin.h"
#include <io.h>
#include <MATH.H>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainFrame*		g_pFrame; //全局指针
extern CBottleCapDoc*		g_pDoc;       
extern CBottleCapView*	g_pView;


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_COPYDATA()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_INDICATOR_GRAB,
	ID_INDICATOR_CHECK,
	ID_INDICATOR_ERROR,
	ID_INDICATOR_OPERATION,
// 	ID_SEPARATOR,           // status line indicator
// 	ID_INDICATOR_CAPS,
// 	ID_INDICATOR_NUM,
// 	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	CoInitialize(NULL);

	LARGE_INTEGER iLarge;
	QueryPerformanceFrequency(&iLarge);
	m_dFreq=(double)iLarge.QuadPart;

	int i=0,j=0,k=0;

	for(i=0;i<ALLELEMENT_COUNT;i++)
	{
		m_iRC_ThreadBad[i]=0;						// 检测线程中给出坏信号时记录
		m_iRC_ThreadGood[i]=0;						// 检测线程中给出好信号时记录
		m_iRC_Good[i]=0;							// 写好信号记录
		m_iRC_AddKickImage[i]=0;					// 记录补剔图像号
		m_iRC_KickImage[i]=0;						// 记录剔废图像号
		m_iRC_ImageNum[i]=0;						// 记录重复记录
		m_iRC_Image[i]=0;							// 记录进入检测线程的图像号
		m_iRC_BeforeD[i]=0;							// 记录算法调用之前的图像号
		m_iRC_Detect[i]=0;							// 记录算法报错的图像号及检测时间
		m_iRC_Kick[i]=0;							// 记录剔废图像号
		m_iRC_Call[i]=0;							// 记录回调中图像号及回调间隔
		m_iRC_DetectTime[i]=0;						// 记录算法检测时间
		m_iRC_RecordImage[i]=0;						// 记录线程中图像号变化时的时间
		m_iRC_SetEvent[i]=0;						// 进入检测线程
		m_iRC_WriteImage[i]=0;						// 获取图像数据[2014-12-5 zhy]
		m_iRC_StopMachine[i]=0;
		m_iRC_RecordKick[i]=0;

		m_iAbnormalImage[i]=0;
		m_iStopCountDiff[i]=0;
		m_iInspectionObj[i]=0;
		m_iSaveCameraImageCount[i]=0;				
		m_iSaveCameraImageCount_Setting[i]=0;		
		m_iSaveCircleCameraImageCount[i]=0;			
		m_iSaveImageType[i]=0;					
		m_iIsCircle[i]=0;							

		m_iKickInterval[i]=0;									
		m_iGrabCountSingleCircle[i]=0;				
		m_iGrabCircleCount[i]=0;						
		m_iImageKickThread[i]=0;						
		m_iImageCheckThread[i]=0;

		m_iCheckErrorCount[i]=0;
		m_iImageCountOverFlowNumber[i]=0;		
		m_iFlowOverImageCountInPE[i]=0;				
		m_iKickFlowNumber[i]=0;						

		m_iHardImageCount[i]=0;
		m_iLastImageCount[i]=0;	
		m_iLastHardImageCount[i]=0;
		m_iImageCountInPE[i]=0;						
		m_iLastImageCountInPE[i]=0;		
		m_iHardKickCount[i]=0;						
		m_iLastKickCount[i]=0;							
		m_iLastImageID[i]=0;							
		m_iDefectTypePerWS[i]=0;
		m_iLabelErrorCount[i]=0;

		m_eSaveImageType[i]=e_SaveImgObj_Alg;

		for(j=0;j<256;j++)
		{
			m_iCheckResultTable[i][j]=-1;
			m_bBottleCapResult[i][j] = FALSE;
			m_bCheckResultOffset[i][j] = FALSE;
		}

		for(j=0;j<MAX_DEFECT_TYPE_COUNT;j++)
		{
			m_iDefectCount[i][j]=0;
		}

		m_bIsKick[i]=TRUE;							// 各路是否踢废
		m_bSaveCameraImage[i]=FALSE;				// 是否保存图像
		m_bDisplayImage[i]=TRUE;					// 三个工位是否显示图像
		m_bDisplayAlgImage[i]=TRUE;					// 三个工位是否显示算法图像
		m_bIsCheck[i]=TRUE;							// 是否检测
		m_bLabelAlarm[i]=FALSE;
		m_bAlarm_StopCount[i]=FALSE;				// 停止计数报警

		m_dAlgCheckTime[i]=0.0;						// 算法检测时间

		m_pImageAddr[i]=NULL;				// 存放图像数据

		ZeroMemory(m_chSavePath[i],512);

	}

	for(i=0;i<CAMERA_COUNT;i++)
	{
		m_iCamSN[i]=0;
		m_iPicCount[i]=0;

		m_iOriginSaveImageNum[i]=0;
		m_iListImagePathCount[i]=0;	
		m_bIsSimulateIniExist[i]=FALSE;				// 模拟采图，用于判断模拟采集时配置文件是否存在
		m_bGrabFlip[i]=FALSE;						// 是否进行翻转

		m_pAlgElement[i]=NULL;						// 专门为算法的参数设置和修改模板而开设的内存
		m_pAlgErrorElement[i]=NULL;					// 专门为算法最近一张错误图像开设的内存
		m_pbRGBImage[i]=NULL;						// 采图贝尔变换图像缓冲区

		for(j=0;j<256;j++)
		{
			for(k=0;k<PIC_COUNT;k++)
			{
				m_bWriteImage[i][j][k]=FALSE;
			}
		}
	}

	for(i=0;i<PE_COUNT;i++)
	{
		m_iRC_PE[i]=0;								// 记录光电
		m_iFrameCount[i]=0;
		m_iFrameCountOverFlowNumber[i]=0;
		m_iLastFrameCount[i]=0;
	}

	for(i=0;i<256;i++)
	{
		m_pLutR[i] = min((int)(i*0),255);
		m_pLutG[i] = min((int)(i*0),255);
		m_pLutB[i] = min((int)(i*0),255);
	}
	
	m_iSaveImage_Abnormal=0;
	m_iAlarmCardNum = 1;
	m_iAlarmOutPort = 3;
	m_iCheckErrorCountTotal=0;										// 每一次开检开始到现在的检测总数
	m_iCheckGoodCountTotal=0;										// 每一次开检开始到现在的好品总数
	m_iCheckCountTotal=0;											// 每一次开检开始到现在的坏品总数
	m_iMaxCameraImageWidth=0;									// 采集图像宽度
	m_iMaxCameraImageHeight=0;									// 采集图像高度
	m_iMaxCameraImageSize=0;									// 采集图像大小
	m_iMaxCameraImagePixelSize=0;								// 采集图像像素大小
	m_iImageShowMode=0;											// 显示图像模式 0:不显示; 1:显示所有; 2:仅显示错误
	m_iIntervalDisplayTime=0;									// 图像显示间隔时间
	m_iMaxSaveImageCount=0;										// 每个工位最大存图张数
	m_iPreAllocateSaveInfoElementCount=0;						// 预分配保存图像信息元素个数：所有信息元素×一个周期采集的个数
	m_iMaxGrabCountSingleCircle=0;								// 全部工位的一周期内的采集图像的最大值
	m_iLastCheckSpeedCount=0;

	m_bAdvMode=FALSE;
	m_bReadModel=FALSE;											// 判断读写模板是否成功
	m_bStartRecord=FALSE;										// 控制开始记录
	m_bIsDetecting=FALSE;										// 判断是否处于正在检测状态
	m_bIOCardValid=FALSE;										// 接口卡是否有效
	m_bIOCardInitSuccess=FALSE;									// 接口卡是否初始化成功
	m_bCalAlg=TRUE;												// 是否调用算法检测
	m_bSoftResetIOCard=TRUE;									// 是否软件复位接口卡
	m_bPauseSaveImageThread=FALSE;								// 是否暂停保存图像线程
	m_bKillSaveImageThread=FALSE;								// 是否停止保存图像线程
	m_bCloseCount=FALSE;										// 用于控制是否获取IO卡中的图像计数
	m_bWaitDemoMould=FALSE;
	m_bStopReadKickImageNo=FALSE;
	m_bOpenAlgCheck = TRUE;
	m_bUpdateCheckData=FALSE;

	m_dwCheckBadRatio=0.0;										// 坏品率

	m_bFirstCheck=TRUE;
	m_iKickOffset=0;
	m_iKickMode=3;
	m_iCamTestMode=0;
	m_iEmptyCount=0;


}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	m_cfgAlgCfg.Init(CExFn::GetCurrentAppPath() + _T("\\Config\\AlgConfig.ini"));
	m_eIOCardType = (e_IOCardType)m_cfgAlgCfg.GetIntEx(_T("SystemConfiguration"), _T("IOCardType"));
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	//设置状态栏
	m_wndStatusBar.SetPaneInfo(0, ID_INDICATOR_GRAB, SBPS_NORMAL, 1500);  //凌华机器参数
	m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_CHECK, SBPS_NORMAL, 150);
	m_wndStatusBar.SetPaneInfo(2, ID_INDICATOR_ERROR, SBPS_NORMAL, 200);
	//m_wndStatusBar.SetPaneInfo(3, ID_INDICATOR_OPERATION, SBPS_NORMAL, 200);


	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable

	// 隐藏标题栏和菜单栏
	ModifyStyle(WS_CAPTION,0);
	SetMenu(NULL);

	////////////////////////////////////////////////////////////////////////
	//获得"记录各文件路径的文件"的文件路径
	m_sWorkPath = GetCurrentAppPath();						// 工作路径
	m_sWorkPath.Delete(m_sWorkPath.GetLength()-1, 1);		// 删除工作路径最后的那个'\'

	CDlgLogin dlgLogin;
	if(IDOK==dlgLogin.DoModal())
	{
		m_iUserType=dlgLogin.m_iUserType;
		m_sUserName=dlgLogin.m_strUser;
		m_sPassword=dlgLogin.m_strPassword;
	}
	else
	{
		exit(0);
	}

	//检查配置文件与文件夹是否存在
	if (!FindFilePathFunc()) 
	{
		AfxMessageBox("系统配置文件不存在!");
		return -1;
	}

	try
	{
		// 读取系统配置文件
		if(!ReadSystemConfig(m_sSysConfigFileName))
		{
			AfxMessageBox("读取系统配置文件失败!");
			return -1;
		}
	}
	catch (...)
	{
		AfxMessageBox("读取系统配置文件异常!");
		return -1;
	}


#ifdef USE_IO_CARD
	
	//初始化参数	
	try
	{	
		// 初始化接口卡
		if(m_bIOCardValid)
		{	
			if(!InitIOCard())
			{
				AfxMessageBox("初始化接口卡失败!");
				return -1;
			}
			else
				m_bIOCardInitSuccess = TRUE;              // 接口卡是否初始化成功
		}
	}
	catch (...)
	{
		AfxMessageBox("初始化接口卡异常!");
		return -1;
	}
 #endif

	try
	{
		// 初始化采集对象
		if (!InitGrabber())
		{
			AfxMessageBox("初始化相机失败!");
			return -1;
		}
	}
	catch (...)
	{
		AfxMessageBox("初始化相机异常!");
		return -1;
	}
	
	try
	{
		// 初始化内存数据区
		if (!InitDataFunc())
		{
			AfxMessageBox("初始化数据区失败!");
			return -1;
		}
	}
	catch (...)
	{
		AfxMessageBox("初始化内存数据区异常!");
		return -1;
	}
	//////////////////////////////////////////////////////////////////////////

	//启动线程
	if (!StartAllThread())
	{
		AfxMessageBox("初始化线程失败!");
		return -1;
	}

	//[2012-5-9 by zhy]
	if(!LoadAllImage())
	{
		AfxMessageBox("加载M盘图像失败!");
		return -1;
	}

	// 连接数据库[2012-11-27]
	if(!ADOConnectDB())
	{
		AfxMessageBox("连接数据库失败!");
		return FALSE;
	}

	// 查看总的数据表是否存在
	if(!ExistTable("All_Check_Info"))
	{
		// 建立表
		if(!CreateTable("All_Check_Info"))
		{
			AfxMessageBox("创建数据表 All_Check_Info 失败!");
			return FALSE;
		}
	}

	CString strTemp,strMsg;
	strTemp.Format("%s","Defect_Statistic_Info");
	if(!ExistTable(strTemp))
	{
		if(!CreateTable(strTemp))
		{
			strMsg.Format("创建数据表 %s 失败!",strTemp);
			AfxMessageBox(strMsg);
			return FALSE;
		}
	}
	
	SetWindowText(m_sWindowTitle);

	//启动采集定时器
	SetTimer(StartGrabTimer,100,NULL);			
	SetTimer(IntervalDisplayTimerOne,m_iIntervalDisplayTime,NULL);    //相机1间隔显示时间
	PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,SystemStarted);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style|=WS_MAXIMIZE;
	cs.style=WS_MAXIMIZEBOX&WS_MINIMIZEBOX&WS_SYSMENU;

	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	// 窗口最大化

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::QuitPrecedure()
{
	if (m_bIsDetecting)
	{
		if(AfxMessageBox("系统正在检测，确定要停止检测吗?",MB_YESNO) !=IDYES)
		{
			return;
		}
		else
		{
			StopCheck();
		}
	}

	
	QuitCheckProgram();	
			
	CMainFrame::OnClose();
}

void CMainFrame::InitInspectionDll()
{
	CString strPath;
	strPath.Format("%s\\Config\\AlgConfig.ini",m_sWorkPath);

	s_CheckInitParam sInit;
	s_DetectInitParam pParam[ALLELEMENT_COUNT];
 	ZeroMemory(&pParam[0],sizeof(s_DetectInitParam));
 	ZeroMemory(&pParam[1],sizeof(s_DetectInitParam));
	ZeroMemory(&pParam[2],sizeof(s_DetectInitParam));

	HWND hWnd[ALLELEMENT_COUNT];
	hWnd[0]=g_pView->GetDlgItem(IDC_ALG1)->GetSafeHwnd();
	hWnd[1]=g_pView->GetDlgItem(IDC_ALG2)->GetSafeHwnd();
	hWnd[2]=g_pView->GetDlgItem(IDC_ALG3)->GetSafeHwnd();

	pParam[0].pHWnd=(int*)(&hWnd[0]);
	pParam[0].iAlgSN=0;
	pParam[0].iRunMode=0;
	ZeroMemory(pParam[0].chLanguage,MAX_LENGTH);
	ZeroMemory(pParam[0].chLanPath,MAX_LENGTH);

	pParam[1].pHWnd=(int*)(&hWnd[1]);
	pParam[1].iAlgSN=0;
	pParam[1].iRunMode=0;
	ZeroMemory(pParam[1].chLanguage,MAX_LENGTH);
	ZeroMemory(pParam[1].chLanPath,MAX_LENGTH);

	pParam[2].pHWnd=(int*)(&hWnd[2]);
	pParam[2].iAlgSN=0;
	pParam[2].iRunMode=0;
	ZeroMemory(pParam[2].chLanguage,MAX_LENGTH);
	ZeroMemory(pParam[2].chLanPath,MAX_LENGTH);

	int pAddr[ALLELEMENT_COUNT];
	int i=0;
	for(;i<ALLELEMENT_COUNT;i++)
	{
		pAddr[i]=(int)(&pParam[i]);
	}

	sInit.KickCallbackFunc=GlobalKickCallbackFunc;
	sInit.StatusCallbackFunc=GlobalStatusCallbackFunc;

#ifdef __SGGrabber_
	sInit.iCallType=2;
#else
	sInit.iCallType=0;
#endif

	sInit.pFilePath = strPath.GetBuffer(0);
	sInit.pThis=this;
	sInit.pParam = (void*)&pAddr;
	sInit.iReserve1=1;		//0表示胶囊 1表示药液

	s_Status sStutus;
	ZeroMemory(&sStutus,sizeof(sStutus));
	sStutus = m_InspectionObj.Init(sInit);
	if (STATUS_NOR != sStutus.iStatusType)
		exit(-1);

	// 需要读参数
	s_Status sRet;
	s_AlgModelInput sModelInfo;
	ZeroMemory(&sModelInfo,sizeof(s_AlgModelInput));
	ZeroMemory(&sRet,sizeof(s_Status));

	s_DetectModelInput sParam;
	ZeroMemory(&sParam,sizeof(s_DetectModelInput));

	CString strModelPath;
	strModelPath.Format("%s\\ModelInfo\\%s",m_sWorkPath,m_sModelName);
	memcpy(sParam.chPath,strModelPath.GetBuffer(0),strModelPath.GetLength());

	CString strLogPath;
	strLogPath.Format("%s\\MendParamLog",m_sLogFilePath);
	memcpy(sParam.chLog,strLogPath.GetBuffer(0),strLogPath.GetLength());

	ZeroMemory(sParam.chImageBufferPath,MAX_LENGTH);

	int* pModelParam[ALLELEMENT_COUNT];


	for(i=0;i<ALLELEMENT_COUNT;i++)
	{
		CString strAlgImages;

		strAlgImages.Format("%s:\\SaveImages\\AlgImages\\Camera%d",m_sVirtualDisk,i+1);
		memcpy(sParam.chAbnormalPath,strAlgImages.GetBuffer(0),strAlgImages.GetLength());
		sModelInfo.iCheckSN=i;
		sModelInfo.iOperSig=0;

		pModelParam[i] = (int*)&sParam;
		sModelInfo.pParam=&pModelParam[i];
		m_InspectionObj.Set(sModelInfo);
	}
}

// 获取本执行程序所在的路径
CString CMainFrame::GetCurrentAppPath()
{
	char chFile[MAX_PATH], chDrive[MAX_PATH], chDir[_MAX_DIR], chFname[_MAX_FNAME], chExt[_MAX_EXT];
	CString AppPath;
	GetModuleFileName(NULL, chFile, sizeof(chFile));
	_splitpath(chFile, chDrive, chDir, chFname, chExt);
	strcat(chDrive, chDir);
	AppPath = chDrive;
	return AppPath;
}

// 检测文件路径是否存在, 没有则建立
BOOL CMainFrame::FindFilePathFunc()
{
	// 创建检测日期
	CFileFind searcher;		// 用于搜索路径和文件是否存在的对象	
	CString strPath;
	CString strVirtualDiskPath;

	CTime CurrTime;
	CurrTime = CTime::GetCurrentTime();	

	// 询问算法demo配置文件夹是否存在
	strVirtualDiskPath.Format("%s\\Config\\AlgConfig.ini",m_sWorkPath);
	if(!inIsPathExist(strVirtualDiskPath))
	{
		CFile file;
		BOOL bRet=file.Open(strVirtualDiskPath,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		if(bRet==TRUE)
		{
			WritePrivateProfileString("DEMO Controls","VirtualDisk","M",strVirtualDiskPath);
			WritePrivateProfileString("DEMO Controls","DisableCreate","1",strVirtualDiskPath);
		}

		GetPrivateProfileString("DEMO Controls","VirtualDisk","M",m_sVirtualDisk.GetBuffer(255),255,strVirtualDiskPath);


		// 写三个按钮的名称[2014-11-7 by zhy]
		CString str1;
		GetPrivateProfileString("DEMO Controls","ImgBtn1Text","",str1.GetBuffer(255),255,strVirtualDiskPath);
		if(str1.IsEmpty())
		{
			WritePrivateProfileString("DEMO Controls","ImgBtn1Text","定位到连续图像",strVirtualDiskPath);
		}
		
		GetPrivateProfileString("DEMO Controls","ImgBtn2Text","",str1.GetBuffer(255),255,strVirtualDiskPath);
		if(str1.IsEmpty())
		{
			WritePrivateProfileString("DEMO Controls","ImgBtn2Text","定位到错误图像",strVirtualDiskPath);
		}
		
		GetPrivateProfileString("DEMO Controls","ImgBtn3Text","",str1.GetBuffer(255),255,strVirtualDiskPath);
		if(str1.IsEmpty())
		{
			WritePrivateProfileString("DEMO Controls","ImgBtn3Text","定位到模板图像",strVirtualDiskPath);
		}
		
		
		// 写入三个图像文件夹路径[2014-11-7 by zhy]
		
		str1.Format("%s:\\SaveImages\\GrabImages",m_sVirtualDisk);
		WritePrivateProfileString("DEMO Controls","ImgBtn1Path",str1,strVirtualDiskPath);
		
		str1.Format("%s:\\SaveImages\\ErrorImages",m_sVirtualDisk);
		WritePrivateProfileString("DEMO Controls","ImgBtn2Path",str1,strVirtualDiskPath);
		
		str1.Format("%s:\\SaveImages\\ModelImages",m_sVirtualDisk);
		WritePrivateProfileString("DEMO Controls","ImgBtn3Path",str1,strVirtualDiskPath);


		// 算法拷贝连续图像
		str1.Format("%s:\\SaveImages\\GrabImages\\%04d年%02d月%02d日\\Camera1",m_sVirtualDisk,CurrTime.GetYear(),
			CurrTime.GetMonth(),CurrTime.GetDay());
		WritePrivateProfileString("DEMO Controls","ImgAlgPath_1",str1,strVirtualDiskPath);

		// 写入保存循环存图时的参数
		str1.Format("%d",1);
		WritePrivateProfileString("DEMO Controls","EnableSequentialCheck",str1,strVirtualDiskPath);

		str1.Format("%s","Product");
		WritePrivateProfileString("DEMO Controls","SequentialCheckFolderFilter ",str1,strVirtualDiskPath);

	}
	else
	{
		WritePrivateProfileString("DEMO Controls","DisableCreate","1",strVirtualDiskPath);
		GetPrivateProfileString("DEMO Controls","VirtualDisk","M",m_sVirtualDisk.GetBuffer(255),255,strVirtualDiskPath);
		WritePrivateProfileString("DEMO Controls","VirtualDisk",m_sVirtualDisk,strVirtualDiskPath);

		// 写三个按钮的名称[2014-11-7 by zhy]
		CString str1;
		GetPrivateProfileString("DEMO Controls","ImgBtn1Text","",str1.GetBuffer(255),255,strVirtualDiskPath);
		if(str1.IsEmpty())
		{
			WritePrivateProfileString("DEMO Controls","ImgBtn1Text","定位到连续图像",strVirtualDiskPath);
		}

		GetPrivateProfileString("DEMO Controls","ImgBtn2Text","",str1.GetBuffer(255),255,strVirtualDiskPath);
		if(str1.IsEmpty())
		{
			WritePrivateProfileString("DEMO Controls","ImgBtn2Text","定位到错误图像",strVirtualDiskPath);
		}

		GetPrivateProfileString("DEMO Controls","ImgBtn3Text","",str1.GetBuffer(255),255,strVirtualDiskPath);
		if(str1.IsEmpty())
		{
			WritePrivateProfileString("DEMO Controls","ImgBtn3Text","定位到模板图像",strVirtualDiskPath);
		}


		// 写入三个图像文件夹路径[2014-11-7 by zhy]

		str1.Format("%s:\\SaveImages\\GrabImages",m_sVirtualDisk);
		WritePrivateProfileString("DEMO Controls","ImgBtn1Path",str1,strVirtualDiskPath);

		str1.Format("%s:\\SaveImages\\ErrorImages",m_sVirtualDisk);
		WritePrivateProfileString("DEMO Controls","ImgBtn2Path",str1,strVirtualDiskPath);

		str1.Format("%s:\\SaveImages\\ModelImages",m_sVirtualDisk);
		WritePrivateProfileString("DEMO Controls","ImgBtn3Path",str1,strVirtualDiskPath);

		// 算法拷贝连续图像
		str1.Format("%s:\\SaveImages\\GrabImages\\%04d年%02d月%02d日\\Camera1",m_sVirtualDisk,CurrTime.GetYear(),
			CurrTime.GetMonth(),CurrTime.GetDay());
		WritePrivateProfileString("DEMO Controls","ImgAlgPath_1",str1,strVirtualDiskPath);	

		// 写入保存循环存图时的参数
		str1.Format("%d",1);
		WritePrivateProfileString("DEMO Controls","EnableSequentialCheck",str1,strVirtualDiskPath);
		
		str1.Format("%s","Product");
		WritePrivateProfileString("DEMO Controls","SequentialCheckFolderFilter ",str1,strVirtualDiskPath);
		
	}


	//日志文件目录[2013-8-21 by zhy]
	CString strMPath;
	strMPath.Format("%s:\\LogFiles\\GrabLog",m_sVirtualDisk);
	if(!inIsPathExist(strMPath))
	{
		inCreateMultiLevelPath(strMPath);
	}

	int i=0;
	CString strName,strBasic;
	strBasic.Format("%s:\\LogFiles\\GrabLog",m_sVirtualDisk);

	for(i=0;i<PE_COUNT;i++)
	{
		// 记录光电信息
		strName.Format("%s\\Guangdian%d.txt",strBasic,i+1);
		m_filePE[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
	}

	for(i=0;i<ALLELEMENT_COUNT;i++)
	{
		// 记录算法检测时间[2013-8-21 by zhy]
		strName.Format("%s\\Suanfashijian%d.txt",strBasic,i+1);
		m_fileDetectTime[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// 记录相机回调时间[2013-8-21 by zhy]
		strName.Format("%s\\Huidiao%d.txt",strBasic,i+1);
		m_fileCall[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// 检测线程中算法检测返回FALSE时的图像信息[2013-8-21 by zhy]
		strName.Format("%s\\Thread_Bad%d.txt",strBasic,i+1);
		m_fileThreadBad[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// 检测线程中算法检测返回TRUE时的图像信息[2013-8-21 by zhy]
		strName.Format("%s\\Thread_Good%d.txt",strBasic,i+1);
		m_fileThreadGood[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// 剔废线程中得到好品时的图像信息
		strName.Format("%s\\Kick_Good%d.txt",strBasic,i+1);
		m_fileGood[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// 所有剔废产品的信息
		strName.Format("%s\\Tifei%d.txt",strBasic,i+1);
		m_fileKickImage[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// 所有补剔产品的信息
		strName.Format("%s\\Buti%d.txt",strBasic,i+1);
		m_fileAddKickImage[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// 回调函数中记录图像信息
		strName.Format("%s\\Call_Mid%d.txt",strBasic,i+1);
		m_fileImageNum[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// 检测过程中记录图像信息
		strName.Format("%s\\CheckThread_Begin%d.txt",strBasic,i+1);
		m_fileImage[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// 进入算法检测时图像信息
		strName.Format("%s\\Jianceqian%d.txt",strBasic,i+1);
		m_fileBeforeD[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// 记录算法检测结果
		strName.Format("%s\\Baocuo%d.txt",strBasic,i+1);
		m_fileDetect[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// 记录检测程序发送给接口卡的图像号及时间
		strName.Format("%s\\Kick_Bad%d.txt",strBasic,i+1);
		m_fileKick[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		
		// 单独一个线程中记录接口卡图像号寄存器中的图像号
		strName.Format("%s\\Tuxiangjicunqi%d.txt",strBasic,i+1);
		m_fileRecordImage[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// 记录事件发送信号信息
		strName.Format("%s\\Call_SetEvent%d.txt",strBasic,i+1);
		m_fileSetEvent[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// 记录获取图像数据信息
		strName.Format("%s\\Call_WriteImage%d.txt",strBasic,i+1);
		m_fileWriteImage[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
		
		// 停机处理
		strName.Format("%s\\StopMachine%d.txt",strBasic,i+1);
		m_fileStopMachine[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);

		strName.Format("%s\\RecordKick%d.txt",strBasic,i+1);
		m_fileRecordKick[i].Open(strName,CFile::modeCreate | CFile::modeReadWrite |CFile::shareDenyNone);
	}
	
	// 检查密码设置文件是否存在
	m_sCipherPath1.Format("%s\\Config\\cipher1.cip",m_sWorkPath);

	//检查系统配置文件是否存在
#ifdef __SGGrabber_                                // 模拟采集
	m_sSysConfigFileName.Format("%s\\Config\\CheckSystemConfig-sim.ini", m_sWorkPath);
#else
	m_sSysConfigFileName.Format("%s\\Config\\CheckSystemConfig.ini", m_sWorkPath);
#endif
	if(!searcher.FindFile(m_sSysConfigFileName))
	{
		CString strExceptionDes("");
		strExceptionDes.Format("错误描述:CheckSystemConfig.ini文件不在指定路径中!");
		m_LogManager.MyWriteLogFile("参数读取",strExceptionDes,OperationLog);
		AfxMessageBox(strExceptionDes);
	}
		
	//检查模拟采集配置文件是否存在  [4/27/2010 GZ]
	for (i=0;i<CAMERA_COUNT;i++)
	{
		CString strTmppp,strConfigName;
		strTmppp.Format("SimulateGrabberConfig%d.ini",i+1);
		strConfigName.Format("%s\\Config\\%s", m_sWorkPath,strTmppp);
		if(!searcher.FindFile(strConfigName))
		{
			m_bIsSimulateIniExist[i]=FALSE;
		}
		else
		{
			CString strTmpPath("");
			GetPrivateProfileString("采图参数","图像文件所在文件夹","",m_sSimulateGrabPath[i].GetBuffer(255),255,strConfigName);
			if (m_sSimulateGrabPath[i]!="")
			{
				CString strTmpPath;
				strTmpPath.Format("%s\\*.bmp",m_sSimulateGrabPath[i]);
				if (!searcher.FindFile(strTmpPath))
				{
					m_bIsSimulateIniExist[i]=FALSE;
				}            
				else 
					m_bIsSimulateIniExist[i]=TRUE;
			}
			
		}		
	}
	
	//检查保存图像文件夹是否存在
	m_sImageFilePath.Format("%s\\SaveImages", m_sWorkPath);
	if(!searcher.FindFile(m_sImageFilePath))
	{
		if(CreateDirectory(m_sImageFilePath, NULL) == FALSE)
		{
			AfxMessageBox("创建路径失败, 请确认本地保存图像文件夹是否设置正确!!!");
			return FALSE;
		}
	}

	CString strInterestFilePath;
	strInterestFilePath.Format("%s\\InterestImages",m_sWorkPath);
	if(!searcher.FindFile(strInterestFilePath))
	{
		if(CreateDirectory(strInterestFilePath, NULL) == FALSE)
		{
			AfxMessageBox("创建路径失败, 请确认本地保存感兴趣图像文件夹是否设置正确!!!");
			return FALSE;
		}
	}
	
	//检查日志文件夹是否存在
	m_sLogFilePath.Format("%s\\LogFiles", m_sWorkPath);
	if(!searcher.FindFile(m_sLogFilePath))
	{
		if(CreateDirectory(m_sLogFilePath, NULL) == FALSE)
		{
			AfxMessageBox("创建路径失败, 请确认本地记录日志文件夹是否设置正确!!!");
			return FALSE;
		}
	}
	
	//检查算法配置文件夹是否存在
	m_sAlgIniFilePath.Format("%s\\ModelInfo", m_sWorkPath);
	if(!searcher.FindFile(m_sAlgIniFilePath))
	{
		if(CreateDirectory(m_sAlgIniFilePath, NULL) == FALSE)
		{
			AfxMessageBox("创建路径失败, 请确认本地算法配置文件夹是否设置正确!!!");
			return FALSE;
		}
	}

	
	//创建图像相关的文件夹
	strPath.Format("%s\\GrabImages", m_sImageFilePath); //采集图像文件夹
	if(!searcher.FindFile(strPath))
	{
		if(CreateDirectory(strPath, NULL) == FALSE)
		{
			AfxMessageBox("创建采集图像文件夹路径失败!!!");
			return FALSE;
		}
	}
	
	//创建错误图像文件夹
	strPath.Format("%s\\ErrorImages", m_sImageFilePath); //错误图像文件夹
	if(!searcher.FindFile(strPath))
	{
		if(CreateDirectory(strPath, NULL) == FALSE)
		{
			AfxMessageBox("创建错误图像文件夹路径失败!!!");
			return FALSE;
		}
	}
	
	//创建异常图像文件夹
	strPath.Format("%s\\AbnormityImages", m_sImageFilePath); //异常图像文件夹
	if(!searcher.FindFile(strPath))
	{
		if(CreateDirectory(strPath, NULL) == FALSE)
		{
			AfxMessageBox("创建异常图像文件夹路径失败!!!");
			return FALSE;
		}
	}
	
	//创建正确图像文件夹
	strPath.Format("%s\\CorrectImages", m_sImageFilePath); //正确图像文件夹
	if(!searcher.FindFile(strPath))
	{
		if(CreateDirectory(strPath, NULL) == FALSE)
		{
			AfxMessageBox("创建正确图像文件夹路径失败!!!");
			return FALSE;
		}
	}
	// 异常图像保存文件夹
	CString strAbnormalPath,strAbnormalDes,strDate;
	strDate.Format("%04d年%02d月%02d日",CurrTime.GetYear(),CurrTime.GetMonth(),CurrTime.GetDay());
	
	for(i=0;i<CAMERA_COUNT;i++)
	{
		strAbnormalPath.Format("%s:\\SaveImages\\AbnormityImages\\%s\\Camera%d",m_sVirtualDisk,strDate,i+1);
		if (!inIsPathExist(strAbnormalPath))
		{
			if(!inCreateMultiLevelPath(strAbnormalPath))
			{
				strAbnormalDes.Format("创建路径 %s 失败！",strAbnormalPath);
				AfxMessageBox(strAbnormalDes);
				break;
				return FALSE;
			}
		}
	}
	
	// 为算法参数设置保存在线图像使用的文件夹
	
	CString strAlgImages,strDes;
	
	for(i=0;i<CAMERA_COUNT;i++)
	{
		strAlgImages.Format("%s:\\SaveImages\\AlgImages\\Camera%d",m_sVirtualDisk,i+1);
		if (!inIsPathExist(strAlgImages))
		{
			if(!inCreateMultiLevelPath(strAlgImages))
			{
				strDes.Format("创建路径 %s 失败！",strAlgImages);
				AfxMessageBox(strDes);
				break;
				return FALSE;
			}
		}
	}


	//创建日志相关的文件夹
	strPath.Format("%s\\OperationLog", m_sLogFilePath); //操作日志文件夹
	if(!searcher.FindFile(strPath))
	{
		if(CreateDirectory(strPath, NULL) == FALSE)
		{
			AfxMessageBox("创建操作日志文件夹路径失败!!!");
			return FALSE;
		}
	}
	
	strPath.Format("%s\\AbnormityLog", m_sLogFilePath); //异常日志文件夹
	if(!searcher.FindFile(strPath))
	{
		if(CreateDirectory(strPath, NULL) == FALSE)
		{
			AfxMessageBox("创建异常日志文件夹路径失败!!!");
			return FALSE;
		}
	}
	
	strPath.Format("%s\\CheckLog", m_sLogFilePath); //检测日志文件夹
	if(!searcher.FindFile(strPath))
	{
		if(CreateDirectory(strPath, NULL) == FALSE)
		{
			AfxMessageBox("创建检测日志文件夹路径失败!!!");
			return FALSE;
		}
	}
	m_sCheckLogFilePath=strPath;
	
	// 检测文件夹下的日期文件夹
	if(!searcher.FindFile(strPath))
	{
		if(CreateDirectory(strPath, NULL) == FALSE)
		{
			AfxMessageBox("创建检测日志文件夹-日期文件夹的路径失败!!!");
			return FALSE;
		}
	}

	//参数修改日志文件夹
	strPath.Format("%s\\MendParamLog", m_sLogFilePath); 
	if(!searcher.FindFile(strPath))
	{
		if(CreateDirectory(strPath, NULL) == FALSE)
		{
			AfxMessageBox("创建参数修改日志文件夹路径失败!!!");
			return FALSE;
		}
	}
	
	//统计检测时间文件夹 [2012-3-14 by zhy]
	strPath.Format("%s\\GrabLog", m_sLogFilePath); 
	if(!searcher.FindFile((strPath)))
	{
		if(CreateDirectory(strPath, NULL) == FALSE)
		{
			AfxMessageBox("创建记录检测时间日志文件夹路径失败!!!");
			return FALSE;
		}
	}


	// 创建数据文件[2014-12-2 zhy]
	strDes.Format("%s\\DataFiles\\%04d年%02d月%02d日",m_sWorkPath,CurrTime.GetYear(),CurrTime.GetMonth(),CurrTime.GetDay());
	if(!inIsPathExist(strDes))
	{
		if(!inCreateMultiLevelPath(strDes))
		{
			strDes.Format("创建路径 %s 失败！",strDes);
			AfxMessageBox(strDes);
			return FALSE;
		}
	}
	
	m_sDataFilePath = strDes;

	return TRUE;
}

// 文件操作 包括复制（FO_COPY），剪切（FO_CUT）和删除（FO_DELETE）
BOOL CMainFrame::OP_Directory(LPCSTR lpsrcPath, LPCSTR lpdstPath, int type, int flag)
{
	SHFILEOPSTRUCT FileOp;   
	
	FileOp.hwnd   =   NULL;   
	FileOp.wFunc  =   type;   
	FileOp.pFrom  =   lpsrcPath;   
	FileOp.pTo    =   lpdstPath;   
	FileOp.fFlags =   flag;//FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR|FOF_NOERRORUI|FOF_SILENT;   
	FileOp.hNameMappings  =  NULL;   
	FileOp.lpszProgressTitle  =  NULL;   
	
	int iRet=-1;
	iRet = SHFileOperation(&FileOp);
	
	if(0==iRet)
		return TRUE;
	else
		return FALSE;
}

BOOL CMainFrame::inCreateMultiLevelPath(const CString &strPath)
{
	//创建一个多层目录，如果存在就不创建
	//(strPath  合法: "D:\\abc\\me" 或 "me\\you" 或 ""; 
	//			不合法: "D:\\abc\\me\\" 或 "me\\you\\" 或 "\\")
	CString tmpPath;// = strPath;
	tmpPath.Format("%s",strPath);
	CList<CString,CString&> FolderStrList;
	int nStrLen = 0;
	
	for(;;)
	{
		nStrLen = tmpPath.GetLength();
		if(nStrLen <= 0)
		{
			break;//从头建文件夹
		}		
		
		BOOL bPathExist = FALSE;//该路径是否已经存在
		
		if(tmpPath.Right(1) == ":")
		{
			bPathExist = inIsPathExist(tmpPath + "\\");
		}
		else
		{
			bPathExist = inIsPathExist(tmpPath);
		}
		
		if(bPathExist)//如果路径存在
		{
			break;//从已存在的路径下建文件夹
		}
		
		//如果路径不存在		
		//每次将最后的一个文件夹名入队
		
		int nPos = 0;
		nPos = tmpPath.ReverseFind('\\');//从后往前确定文件夹名
		if(nPos == -1)//已到达第一个文件夹名
		{
			nPos = 0;//将第一个文件夹名入队
		}
		
		//将文件夹名入队
		CString tmpStr = tmpPath.Right(nStrLen - nPos);
		FolderStrList.AddHead(tmpStr);
		tmpPath.Delete(nPos, nStrLen - nPos);		
	}	
	
	//根据文件夹名队列顺序建立
	while(FolderStrList.IsEmpty() == FALSE)
	{
		tmpPath += FolderStrList.RemoveHead();
		BOOL bRet = FALSE;
		bRet = CreateDirectory(tmpPath, NULL);
		if(bRet == FALSE)
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

BOOL CMainFrame::inIsPathExist(const CString &strPath)
{
#ifdef _UNICODE
#define IsDirectoryExist(path) ::_waccess(path,0) == 0
	//_waccess的第二个参数表示仅判断存在性，返回0表示存在，返回-1表示不存在
#else
#define IsDirectoryExist(path) ::_access(path,0) == 0//_access是MBCS版本
#endif
	
	if(IsDirectoryExist(strPath))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	
}

// 读取系统配置文件
BOOL CMainFrame::ReadSystemConfig(const CString &strLoadFile)
{
	CString  strTemp;
	UINT ExitCode = 0;
	BOOL bReturn = TRUE;
	
	//读取窗口标题  //[2012-1-18 by zhy]
	GetPrivateProfileString("系统配置", "窗口标题", "大恒图像标签质量检测系统软件V1.0", m_sWindowTitle.GetBuffer(255), 255, strLoadFile);
	
	//读取上一次模板名称
	GetPrivateProfileString("系统配置","上一次使用算法模板名称","",strTemp.GetBuffer(255),255,strLoadFile);     //  [6/17/2010 ZXF]

	// 更换相机配置文件[2017-3-24]
	LoadCamConfig(strTemp);

	m_sModelName.Format("%s",strTemp);
	m_sModelPath.Format("%s\\ModelInfo\\%s",m_sWorkPath,strTemp);    

	m_iLabelErrorCount[Camera1]			= GetPrivateProfileInt("系统配置", "标签位置连续报错个数", 5, strLoadFile);			//连续错误个数
	m_bIOCardValid						= GetPrivateProfileInt("系统配置", "接口卡是否有效", 0, strLoadFile);			//接口卡是否有效
	m_iImageShowMode					= GetPrivateProfileInt("系统配置", "显示全部图像", 1, strLoadFile);				//判断图像显示模式
	m_bOpenAlgCheck						= GetPrivateProfileInt("系统配置", "开启算法检测", 1, strLoadFile);				//是否开启算法检测 
	m_iMaxSaveImageCount				= GetPrivateProfileInt("系统配置", "保存图像张数", 100, strLoadFile);			//保存图像的最大张数
	m_iPreAllocateSaveInfoElementCount	= GetPrivateProfileInt("系统配置", "预分配保存图像元素个数", 5, strLoadFile); 
	m_iIntervalDisplayTime				= GetPrivateProfileInt("系统配置", "图像显示间隔时间", 0, strLoadFile);			//图像显示间隔时间
	m_iCheckSpeedInterval				= GetPrivateProfileInt("系统配置", "检测速度时间间隔", 2000, strLoadFile);
	m_iSaveImage_Abnormal				= GetPrivateProfileInt("系统配置", "允许保存的异常图像张数", 10, strLoadFile);
	m_iRecord_Set						= GetPrivateProfileInt("系统配置", "允许保存的日志记录条数", 20000, strLoadFile);
	m_iErrorSignal						= GetPrivateProfileInt("系统配置", "过滤信号间隔", 100, strLoadFile);
	m_bOpenImageSave					= GetPrivateProfileInt("系统配置", "开启图像保存", 0, strLoadFile);				//是否开启算法检测 

	if (m_iIntervalDisplayTime < 0)
	{

		strTemp.Format("获取配置文件-系统配置-图像显示间隔时间-%d不对!",m_iIntervalDisplayTime);
		MessageBox(strTemp);
		return FALSE;
	}
	
	CString  strSession;
	int i=0,j=0;
	for (i=0;i<ALLELEMENT_COUNT;i++)
	{
		
		strSession.Format("工位%d停止计数判断条件",i+1);
		m_iStopCountDiff[i]=GetPrivateProfileInt("系统配置", strSession, 3, strLoadFile);//[2013-2-21]

		strSession.Format("工位%d算法对象个数",i+1);
		m_iInspectionObj[i]=GetPrivateProfileInt("系统配置", strSession, 3, strLoadFile);//[2013-2-21]

		strSession.Format("工位%d算法检测最长时间",i+1);
		m_iKickInterval[i]=GetPrivateProfileInt("系统配置", strSession, 120, strLoadFile);//[2013-2-21]

		strSession.Format("相机%d是否检测",i+1);
		m_bIsCheck[i] = GetPrivateProfileInt("系统配置", strSession, 0, strLoadFile); //是否检测

		strSession.Format("相机%d一个周期采集图像个数",i+1);
		m_iGrabCountSingleCircle[i] = GetPrivateProfileInt("系统配置", strSession, 1, strLoadFile);//一个周期采集图像个数

		if (i==0)// 取 3个相机周期内采集个数的最大值 [4/19/2010 GZ]
		{
			m_iMaxGrabCountSingleCircle=m_iGrabCountSingleCircle[i];
		}
		else
		{
			if (m_iGrabCountSingleCircle[i]>m_iMaxGrabCountSingleCircle)
			{
				m_iMaxGrabCountSingleCircle=m_iGrabCountSingleCircle[i];
			}
		}
	}

	//Bayer变换相关变量
	for (i=0;i<CAMERA_COUNT;i++)
	{
		// 面阵相机图像是否进行Bayer转换
		strSession.Format("相机%dBayer变换参数",i+1);
		m_BayerTransform[i].m_bAreaCamBayer = GetPrivateProfileInt(strSession, "是否进行Bayer变换", -1, strLoadFile);
		
		GetPrivateProfileString(strSession,"Bayer转换Red增益","",strTemp.GetBuffer(255),255,m_sSysConfigFileName);
		m_BayerTransform[i].m_dBayerRedGain = atof(strTemp);
		
		GetPrivateProfileString(strSession,"Bayer转换Green增益","",strTemp.GetBuffer(255),255,m_sSysConfigFileName);
		m_BayerTransform[i].m_dBayerGreenGain = atof(strTemp);
		
		GetPrivateProfileString(strSession,"Bayer转换Blue增益","",strTemp.GetBuffer(255),255,m_sSysConfigFileName);
		m_BayerTransform[i].m_dBayerBlueGain = atof(strTemp);

#ifdef __SGGrabber_
		m_BayerTransform[i].m_bAreaCamBayer = FALSE;
#endif

		if (m_BayerTransform[i].m_bAreaCamBayer)
		{
			SetLutTable(m_BayerTransform[i].m_dBayerRedGain,m_BayerTransform[i].m_dBayerGreenGain,
						m_BayerTransform[i].m_dBayerBlueGain);
		}
		
		strSession.Format("相机%d是否翻转",i+1);
		m_bGrabFlip[i] = GetPrivateProfileInt("采集图像翻转", strSession, 0, strLoadFile);	
		
		strSession.Format("相机%d翻转模式",i+1);
		int iFlip = GetPrivateProfileInt("采集图像翻转", strSession, 0, strLoadFile);	
		if (iFlip == 0)
		{
			m_eShowModeFlip[i] = NoFlip;         //无翻转
		}
		else if (iFlip == 1)
		{
			m_eShowModeFlip[i] = FlipVertical;   //垂直翻转
		}
		else if (iFlip == 2)
		{
			m_eShowModeFlip[i] = FlipHorizintal; //水平翻转
		}
	}

	// 初始化写入缺陷信息
	// 检测位置信息
	CString strInfo;
	strInfo.Format("%d",4);
	WritePrivateProfileString("工位1缺陷信息","缺陷个数",strInfo,strLoadFile);

	strInfo.Format("%s","胶塞(封口)缺陷");
	WritePrivateProfileString("工位1缺陷信息","类型1",strInfo,strLoadFile);

	strInfo.Format("%s","轧盖缺陷");
	WritePrivateProfileString("工位1缺陷信息","类型2",strInfo,strLoadFile);

	strInfo.Format("%s","无产品");
	WritePrivateProfileString("工位1缺陷信息","类型3",strInfo,strLoadFile);

	strInfo.Format("%s", "其他缺陷");
	WritePrivateProfileString("工位1缺陷信息", "类型4", strInfo, strLoadFile);

	// 工位2缺陷信息
	strInfo.Format("%d",8);
	WritePrivateProfileString("工位2缺陷信息","缺陷个数",strInfo,strLoadFile);
	
	strInfo.Format("%s","标签存在污渍");
	WritePrivateProfileString("工位2缺陷信息","类型1",strInfo,strLoadFile);
	
	strInfo.Format("%s","无法定位到标签名称");
	WritePrivateProfileString("工位2缺陷信息","类型2",strInfo,strLoadFile);
	
	strInfo.Format("%s","无法匹配到模板");
	WritePrivateProfileString("工位2缺陷信息","类型3",strInfo,strLoadFile);
	
	strInfo.Format("%s","无法正确提取到标点");
	WritePrivateProfileString("工位2缺陷信息","类型4",strInfo,strLoadFile);
	
	strInfo.Format("%s","三期部分缺少点");
	WritePrivateProfileString("工位2缺陷信息","类型5",strInfo,strLoadFile);
	
	strInfo.Format("%s","三期部分缺少横杠");
	WritePrivateProfileString("工位2缺陷信息","类型6",strInfo,strLoadFile);
	
	strInfo.Format("%s","三期区域非三行");
	WritePrivateProfileString("工位2缺陷信息","类型7",strInfo,strLoadFile);
	
	strInfo.Format("%s","三期区域模糊");
	WritePrivateProfileString("工位2缺陷信息","类型8",strInfo,strLoadFile);
	
	// 关于缺陷信息
	for(i=0;i<ALLELEMENT_COUNT;i++)
	{
		CString str1,str2;
		str1.Format("工位%d缺陷信息",i+1);
		str2.Format("%s","缺陷个数");
		m_iDefectTypePerWS[i]=GetPrivateProfileInt(str1, str2, 10, strLoadFile);	
		
		for(j=0;j<m_iDefectTypePerWS[i];j++)
		{
			str2.Format("类型%d",j+1);
			GetPrivateProfileString(str1,str2,"",m_sDefectInfo[i][j].GetBuffer(255),255,strLoadFile);
		}
	}
	
	// 算法Demo路径 [2014-7-1 by zhy]
	m_sDemoIniPath.Format("%s\\set.ini", m_sWorkPath);	
	// Demo名称
	GetPrivateProfileString("系统配置","DemoName","",m_sDemoName.GetBuffer(255),255,m_sDemoIniPath);
	WritePrivateProfileString("系统配置","AppName",m_sWindowTitle,m_sDemoIniPath);


	// 报警信号所使用的接口卡编号和输出端口号
	m_iAlarmCardNum = GetPrivateProfileInt("接口卡配置", "报警信号所使用的接口卡编号", 1, strLoadFile);
	if(m_iAlarmCardNum<1 || m_iAlarmCardNum>IOCARD_COUNT)
	{
		AfxMessageBox("报警信号所使用接口卡设置错误，请查看配置文件!");
		return FALSE;
	}

	m_iAlarmOutPort = GetPrivateProfileInt("接口卡配置", "报警信号所使用的输出端口号", 3, strLoadFile);
	if(m_iAlarmOutPort<0 || m_iAlarmOutPort>7)
	{
		AfxMessageBox("报警信号所使用接口卡输出端口设置错误，请查看配置文件!");
		return FALSE;
	}

	// 相机使用的相机编号
	for(i=0;i<CAMERA_COUNT;i++)
	{
		strInfo.Format("相机%d所使用的接口卡编号",i+1);
		m_iCamCardNum[i]=GetPrivateProfileInt("接口卡配置", strInfo, 1, strLoadFile);

		strInfo.Format("相机%d所使用的输出端口号",i+1);

		if(Camera1==i)
		{
			m_iCamOutPort[i]=GetPrivateProfileInt("接口卡配置", strInfo, 1, strLoadFile);
		}
		else if(Camera2==i)
		{
			m_iCamOutPort[i]=GetPrivateProfileInt("接口卡配置", strInfo, 2, strLoadFile);
		}
		else if(Camera3==i)
		{
			m_iCamOutPort[i]=GetPrivateProfileInt("接口卡配置", strInfo, 3, strLoadFile);
		}
	}


	// 读取第二路打标队列X延时帧个数
	CString strPIO24B;
	strPIO24B.Format("%s\\PIO24B_reg_init.txt",m_sWorkPath);
	m_iKickOffset = GetPrivateProfileInt("PIO24B", "89", 2, strPIO24B);

	CTime curTime = CTime::GetCurrentTime();

	for(i=0;i<ALLELEMENT_COUNT;i++)
	{
		m_sSaveImagePath[i].Format("%s:\\%s\\%s\\%04d年%02d月%02d日\\Camera%d",
			m_sVirtualDisk,
			"SaveImages","GrabImages",
			curTime.GetYear(),
			curTime.GetMonth(),
			curTime.GetDay(),
			i+1);

		if(!inIsPathExist(m_sSaveImagePath[i]))
		{
			if(!inCreateMultiLevelPath(m_sSaveImagePath[i]))
			{
				CString str;
				str.Format("创建路径失败:%s",m_sSaveImagePath[i]);
				AfxMessageBox(str);
				return FALSE;
			}
		}
	}

	// 读取序列图个数
	m_iPicCountPerCam[Camera1] = GetPrivateProfileInt("PIO24B", "44", 12, strPIO24B);
	m_iPicCountPerCam[Camera2] = GetPrivateProfileInt("PIO24B", "70", 12, strPIO24B);
	m_iPicCountPerCam[Camera3] = GetPrivateProfileInt("PIO24B", "68", 0, strPIO24B);

	m_iPicCountPerCam[Camera1]++;
	m_iPicCountPerCam[Camera2]++;
	m_iPicCountPerCam[Camera3]++;

	CTime CurrTime = CTime::GetCurrentTime();
	m_sStartTime.Format("%d-%d-%d %d:%d:%d",CurrTime.GetYear(),CurrTime.GetMonth(),CurrTime.GetDay(),
		CurrTime.GetHour(),CurrTime.GetMinute(),CurrTime.GetSecond());

	return TRUE;
}

// 保存系统配置文件
BOOL CMainFrame::SaveSystemConfig(const CString &strLoadFile)
{
	CString strValue;
		
	strValue.Format("%d", m_iIntervalDisplayTime);	
	WritePrivateProfileString("系统配置", "图像显示间隔时间", strValue, strLoadFile);

	strValue.Format("%d", m_iImageShowMode);
	WritePrivateProfileString("系统配置", "显示全部图像", strValue, strLoadFile);

	strValue.Format("%d", m_bOpenAlgCheck);
	WritePrivateProfileString("系统配置", "开启算法检测", strValue, strLoadFile);

	strValue.Format("%d", m_bOpenImageSave);
	WritePrivateProfileString("系统配置", "开启图像保存", strValue, strLoadFile);


	// [2014-6-10 by zhy]	
	strValue.Format("%d",m_iMaxSaveImageCount);
	WritePrivateProfileString("系统配置", "保存图像张数", strValue, strLoadFile);

	strValue.Format("%d", m_iPreAllocateSaveInfoElementCount);	
	WritePrivateProfileString("系统配置", "预分配保存图像元素个数", strValue, strLoadFile);

	strValue.Format("%d", m_iLabelErrorCount[Camera1]);	
	WritePrivateProfileString("系统配置", "标签位置连续报错个数", strValue, strLoadFile);

	strValue.Format("%d",m_iCheckSpeedInterval);
	WritePrivateProfileString("系统配置", "检测速度时间间隔", strValue, strLoadFile);

	strValue.Format("%d",m_iSaveImage_Abnormal);
	WritePrivateProfileString("系统配置", "允许保存的异常图像张数", strValue, strLoadFile);

	strValue.Format("%d",m_iRecord_Set);
	WritePrivateProfileString("系统配置", "允许保存的日志记录条数", strValue, strLoadFile);

	strValue.Format("%d",m_iErrorSignal);
	WritePrivateProfileString("系统配置", "过滤信号间隔",strValue, strLoadFile);

	strValue.Format("%d",m_iLicenseExpDate);
	WritePrivateProfileString("系统配置", "License提醒", strValue, strLoadFile);

	strValue.Format("%d",m_iAlarmCardNum);
	WritePrivateProfileString("接口卡配置","报警信号所使用的接口卡编号",strValue, strLoadFile);
	
	strValue.Format("%d",m_iAlarmOutPort);
	WritePrivateProfileString("接口卡配置","报警信号所使用的输出端口号",strValue, strLoadFile);

	CString  strSession;
	int i=0;

	for (;i<ALLELEMENT_COUNT;i++)
	{
		strSession.Format("工位%d停止计数判断条件",i+1);
		strValue.Format("%d",m_iStopCountDiff[i]);
		WritePrivateProfileString("系统配置", strSession, strValue, strLoadFile);

		strSession.Format("工位%d算法对象个数",i+1);
		strValue.Format("%d",m_iInspectionObj[i]);
		WritePrivateProfileString("系统配置", strSession, strValue, strLoadFile);

		strSession.Format("工位%d算法检测最长时间",i+1);
		strValue.Format("%d", m_iKickInterval[i]);	
		WritePrivateProfileString("系统配置", strSession, strValue, strLoadFile);

		strSession.Format("相机%d是否检测",i+1);
		strValue.Format("%d", m_bIsCheck[i]);	
		WritePrivateProfileString("系统配置", strSession, strValue, strLoadFile);
		
		strSession.Format("相机%d一个周期采集图像个数",i+1);
		strValue.Format("%d", m_iGrabCountSingleCircle[i]);	
		WritePrivateProfileString("系统配置", strSession, strValue, strLoadFile);	
	}

	//Bayer变换相关变量
	for (i=0;i<CAMERA_COUNT;i++)
	{		
		strSession.Format("相机%dBayer变换参数",i+1);		
		strValue.Format("%d", m_BayerTransform[i].m_bAreaCamBayer);	
		WritePrivateProfileString(strSession, "是否进行Bayer变换", strValue, strLoadFile);
		
		strValue.Format("%f", m_BayerTransform[i].m_dBayerRedGain);	
		WritePrivateProfileString(strSession, "Bayer转换Red增益", strValue, strLoadFile);
		
		strValue.Format("%f", m_BayerTransform[i].m_dBayerGreenGain);	
		WritePrivateProfileString(strSession, "Bayer转换Green增益", strValue, strLoadFile);
		
		strValue.Format("%f", m_BayerTransform[i].m_dBayerBlueGain);	
		WritePrivateProfileString(strSession, "Bayer转换Blue增益", strValue, strLoadFile);
		
		strSession.Format("相机%d是否翻转",i+1);
		strValue.Format("%d",m_bGrabFlip[i]);
		WritePrivateProfileString("采集图像翻转", strSession, strValue, strLoadFile);

		strSession.Format("相机%d翻转模式",i+1);
		strValue.Format("%d",m_eShowModeFlip[i]);
		WritePrivateProfileString("采集图像翻转", strSession, strValue, strLoadFile);		
	}

	// 相机使用的相机编号
	for(i=0;i<CAMERA_COUNT;i++)
	{
		strSession.Format("相机%d所使用的接口卡编号",i+1);
		strValue.Format("%d",m_iCamCardNum[i]);
		WritePrivateProfileString("接口卡配置", strSession, strValue,strLoadFile);
		
		strSession.Format("相机%d所使用的输出端口号",i+1);
		strValue.Format("%d",m_iCamOutPort[i]);
		WritePrivateProfileString("接口卡配置", strSession, strValue,strLoadFile);
	}
	return TRUE;
}

//设置Bayer参数
BOOL CMainFrame::SetLutTable(double dRatioR,double dRatioG,double dRatioB) 
{
	if(dRatioR <=0)
		return FALSE;
	if(dRatioG <=0)
		return FALSE;
	if(dRatioB <=0)
		return FALSE;
	
	for(int i=0;i<256;i++)
	{
		m_pLutR[i] = min((int)(i*dRatioR),255);
		m_pLutG[i] = min((int)(i*dRatioG),255);
		m_pLutB[i] = min((int)(i*dRatioB),255);
	}
	
	return TRUE;
}

// 初始化接口卡
inline BOOL CMainFrame::InitIOCard()
{
	BOOL  bRet = FALSE;
	try{
		if(e_IOCardType_PIOC==m_eIOCardType)
		{
			for (int i = 0; i < IOCARD_COUNT; i++)
			{
				if (Camera1 == i)
				{
					bRet = m_IOCard[i].openCard(0);

					if (bRet == FALSE)
					{
						AfxMessageBox("Open IOCard Failed!");
						return FALSE;
					}

					m_IOCard[i].hardReset();
					m_IOCard[i].initRegisters("PIO24B_reg_init.txt", "PIO24B");
					m_IOCard[i].softReset();
				}
			}
		}
		else if (e_IOCardType_NewCard == m_eIOCardType)
		{
			if (TRUE == m_NewIOCard.IsOpen())
			{
				m_NewIOCard.Close();
				m_bReadIOCardParamFile = true;
			}
			bRet = m_NewIOCard.Open(0, m_hWnd);
			if (FALSE == bRet)
				return FALSE;

		}
	}
	catch (...)
	{
		CString strExceptionDes("");
		strExceptionDes.Format("InitIOCard:接口卡初始化异常!");
		AfxMessageBox(strExceptionDes);
		return FALSE;
	}
	
	return TRUE;
}

// 释放接口卡
inline BOOL CMainFrame::ReleaseIOCard()
{
	
#ifdef USE_IO_CARD
	if(m_bIOCardValid && m_bIOCardInitSuccess)
	{
		try
		{
			if(e_IOCardType_PIOC == m_eIOCardType)
			{
				for (int i = 0; i < IOCARD_COUNT; i++)
				{
					m_IOCard[i].enable(FALSE);	      // 关闭使能接口卡 [1/18/2012 dh]
					m_IOCard[i].closeCard();
				}
			}
			else if (e_IOCardType_NewCard == m_eIOCardType)
			{
				m_NewIOCard.PauseInCntWork(-1);

				for (int j = 0; j < 24; j++)
				{
					m_NewIOCard.SetOutIOStatus(j, FALSE);
				}
				m_NewIOCard.Close();
			}
			
		}
		catch (...)
		{
			CString  strExceptionDes;  
			strExceptionDes.Format("StopCheck-设置接口卡异常!");
			m_LogManager.MyWriteLogFile("接口卡操作",strExceptionDes,AbnormityLog);
		}
	}
#endif
	
	return TRUE;
}

// 初始化采集对象
inline BOOL CMainFrame::InitGrabber()
{
	BOOL bRet = FALSE;
	int iDeviceSN  = 0;		     // 相机设备序号	
	
	CString str;				 // 字符串通用变量
	CString strCameraDeviceName; // 相机名称
	CString strDeviceMark;       // 设备标识
	CString strDeviceInitFile;	 // 设备初始化文件名
	CString strSession;         // 字段名称

	for (int i=0;i<CAMERA_COUNT;i++)
	{
		strSession.Format("设备%c序号",'A'+i);  
		iDeviceSN = GetPrivateProfileInt("采集参数", strSession, -1, m_sSysConfigFileName);//设备序号
		
		//校验相机序号
		if (iDeviceSN > CAMERA_COUNT)
		{
			CString strExceptionDes("");
			strExceptionDes.Format("InitGrabber:相机%d序号%d超过最大相机个数!",i+1,iDeviceSN);
			AfxMessageBox(strExceptionDes);
			return FALSE;
		}

		////////////////////
		// 通用版本
		strSession.Format("设备%c名称",'A'+i);
		GetPrivateProfileString("采集参数", strSession, "", strCameraDeviceName.GetBuffer(255), 255, m_sSysConfigFileName);
		
		//  [4/21/2010 GZ]
		strSession.Format("设备%c标识",'A'+i);
		GetPrivateProfileString("采集参数", strSession, "", strDeviceMark.GetBuffer(255), 255, m_sSysConfigFileName);
	
		strSession.Format("设备%c初始化文件名",'A'+i);
		GetPrivateProfileString("采集参数", strSession, "", str.GetBuffer(255), 255, m_sSysConfigFileName);
		strDeviceInitFile.Format("%s\\config\\%s", m_sWorkPath, str);
		
		// 用于搜索路径和文件是否存在的对象
		CFileFind searcher;		
		if(!searcher.FindFile(strDeviceInitFile))
		{
			CString strExceptionDes("");
			strExceptionDes.Format("错误描述:相机%d配置文件%s文件不在指定路径中",i+1,str);
			AfxMessageBox(strExceptionDes);
			return FALSE;
		}
		else
		{
			m_sCamConfigPath[i]=strDeviceInitFile;
			m_sCamConfigName[i]=str;
		}
		
		// 初始化采集对象
		s_GBINITSTRUCT AreaGraber;
		AreaGraber.iGrabberTypeSN = iDeviceSN;
		AreaGraber.nGrabberSN = iDeviceSN;						// 设备序号
#ifdef __RealGrabber_
		AreaGraber.nGrabberSN = iDeviceSN;						// 设备序号
		sprintf(AreaGraber.strDeviceMark, strDeviceMark.GetBuffer(strDeviceMark.GetLength()));	// 设备名称
#endif
		
		AreaGraber.Context	  = this;							// this指针
		sprintf(AreaGraber.strDeviceName, strCameraDeviceName.GetBuffer(strCameraDeviceName.GetLength()));	// 设备名称
		sprintf(AreaGraber.strGrabberFile, strDeviceInitFile);	// 设备的初始化文件名
		
		AreaGraber.CallBackFunc = GlobalAreaGrabOverCallback1;	// 回调函数
		
		// 初始化图像卡
		bRet = m_CameraGrabber[i].Init(&AreaGraber);
	
		if (!bRet)
		{
			// 添加初始化异常共有六种错误信息
			s_GBERRORINFO GrabberInfo;
			m_CameraGrabber[i].GetLastErrorInfo(&GrabberInfo);
			CString strExceptionDes;
			strExceptionDes.Format("相机%d初始化失败!",i+1);
			strExceptionDes += GrabberInfo.strErrorDescription;
		 	AfxMessageBox(strExceptionDes);
			return FALSE;
		}
		else
		{
			TRACE("相机%d采集对象初始化成功!\n",i+1);
		}
		
		//获取采集的图像宽度、高度、位数
		m_CameraGrabber[i].GetParamInt(GBImageWidth, m_sCameraImageInfo[i].m_iCameraImageWidth);			// 面阵相机图像宽度
		m_CameraGrabber[i].GetParamInt(GBImageHeight, m_sCameraImageInfo[i].m_iCameraImageHeight);			// 面阵相机图像高度
		m_CameraGrabber[i].GetParamInt(GBImageBufferSize, m_sCameraImageInfo[i].m_iCameraImageSize);		// 面阵相机图像大小
		m_CameraGrabber[i].GetParamInt(GBImagePixelSize, m_sCameraImageInfo[i].m_iCameraImagePixelSize);	// 面阵相机图像象素大小
#ifdef __RealGrabber_
		if(m_BayerTransform[i].m_bAreaCamBayer)
		{
			m_sCameraImageInfo[i].m_iCameraImagePixelSize = m_sCameraImageInfo[i].m_iCameraImagePixelSize*3;  // 彩色相机
			m_sCameraImageInfo[i].m_iCameraImageSize = m_sCameraImageInfo[i].m_iCameraImageSize*3;
		}
#endif
	
		if (m_sCameraImageInfo[i].m_iCameraImageWidth > m_iMaxCameraImageWidth)
		{
			m_iMaxCameraImageWidth = m_sCameraImageInfo[i].m_iCameraImageWidth;
		}
		
		if (m_sCameraImageInfo[i].m_iCameraImageHeight > m_iMaxCameraImageHeight)
		{
			m_iMaxCameraImageHeight = m_sCameraImageInfo[i].m_iCameraImageHeight;
		}
		
		if (m_sCameraImageInfo[i].m_iCameraImageSize > m_iMaxCameraImageSize)
		{
			m_iMaxCameraImageSize = m_sCameraImageInfo[i].m_iCameraImageSize;
		}
		
		if (m_sCameraImageInfo[i].m_iCameraImagePixelSize > m_iMaxCameraImagePixelSize)
		{
			m_iMaxCameraImagePixelSize = m_sCameraImageInfo[i].m_iCameraImagePixelSize;
		}

		ZeroMemory(&m_sCamInfo[i],sizeof(s_CheckCameraInfo));
		m_sCamInfo[i].iBitCount=m_sCameraImageInfo[i].m_iCameraImagePixelSize*8;
		m_sCamInfo[i].iCount=m_iPicCountPerCam[i];
		m_sCamInfo[i].iHeight= m_sCameraImageInfo[i].m_iCameraImageHeight;
		m_sCamInfo[i].iWidth= m_sCameraImageInfo[i].m_iCameraImageWidth;
		m_sCamInfo[i].iType=0;


		//将相机图像大小写入AlgCofig里面
		CString strPath;
		strPath.Format("%s\\Config\\AlgConfig.ini",m_sWorkPath);

		int iWidth = m_sCamInfo[i].iWidth*m_iPicCountPerCam[i];
		int iHeight = m_sCamInfo[i].iHeight;

		CString strApp,strValue;
		strApp.Format("Camera%d Parameters",i+1);
		strValue.Format("%d //相机图像宽度",iWidth);
		WritePrivateProfileString(strApp,"Width",strValue,strPath);
	
		strValue.Format("%d //相机图像高度",iHeight);
		WritePrivateProfileString(strApp,"Height",strValue,strPath);

		//检测结果个数，标签项目均为1
		strValue.Format("%d //每次算法检测返回结果个数",1);
		WritePrivateProfileString(strApp,"CheckOutNum",strValue,strPath);
		
		//检测输入特殊结构体大小
		strValue.Format("%d //算法检测时结构体s_CheckInParam中 void* pParam的大小",sizeof(s_DetectInParam));
		WritePrivateProfileString(strApp,"CheckInSize",strValue,strPath);
		
		//检测输出特殊结构体大小
		strValue.Format("%d //算法检测时结构体s_CheckOutParam中 void* pParam的大小",sizeof(s_DetectOutParam));
		WritePrivateProfileString(strApp,"CheckOutSize",strValue,strPath);
		
		// 写入多对象配置文件超时个数
		strValue.Format("%d",(m_iKickOffset-1));
		strApp.Format("KickGroup%d",i+1);
		WritePrivateProfileString(strApp,"TimeOut",strValue,strPath);
	}	
	//////////////////////////////////////////////////////////////////////////
	return TRUE;
}

// 释放采集对象
inline BOOL CMainFrame::ReleaseGrabber()
{
	for (int i=0;i<CAMERA_COUNT;i++)
	{
		m_CameraGrabber[i].Close();
		TRACE("关闭相机%d\r\n",i+1);
	}

	return TRUE;
}

// 初始化内存数据区
inline BOOL CMainFrame::InitDataFunc()
{	
	int i = 0;
	int j = 0;
	int k = 0;
	int iImageSize=0;

	for(i=0;i<ALLELEMENT_COUNT;i++)
	{
		for(j=0;j<DATA_ELEMENT_COUNT;j++)
		{
			m_sElement[i][j].bIsCheck=TRUE;
			m_sElement[i][j].bHaveError=FALSE;
			m_sElement[i][j].iIsUsing=0;
			m_sElement[i][j].iIsDetecting=0;
			m_sElement[i][j].iCameraSN=0;
			m_sElement[i][j].iCircleSN=0;
			m_sElement[i][j].iImageSN=0;
			m_sElement[i][j].strErrorInfo="";
		}
	}


	// 分配存储Bayer变换内存
	for (i=0;i<CAMERA_COUNT;i++)
	{
		int iWidth=0,iHeight=0,iByte=0;
		iWidth = m_sCameraImageInfo[i].m_iCameraImageWidth;
		iHeight = m_sCameraImageInfo[i].m_iCameraImageHeight;
		iByte = m_sCameraImageInfo[i].m_iCameraImagePixelSize;
		m_pbRGBImage[i] = new BYTE[iWidth*iHeight*iByte];
	}

	
	// 为算法模板设置分配内存
	for(i=0;i<CAMERA_COUNT;i++)
	{	
		try
		{
			m_pAlgElement[i]=new BYTE[m_iMaxCameraImageWidth*m_iMaxCameraImageHeight*m_iMaxCameraImagePixelSize*m_iPicCountPerCam[i]];
			memset(m_pAlgElement[i],0,m_iMaxCameraImageWidth*m_iMaxCameraImageHeight*m_iMaxCameraImagePixelSize*m_iPicCountPerCam[i]);
			
			m_pAlgErrorElement[i]=new BYTE[m_iMaxCameraImageWidth*m_iMaxCameraImageHeight*m_iMaxCameraImagePixelSize*m_iPicCountPerCam[i]];
			memset(m_pAlgErrorElement[i],0,m_iMaxCameraImageWidth*m_iMaxCameraImageHeight*m_iMaxCameraImagePixelSize*m_iPicCountPerCam[i]);	
		}
		catch (...)
		{
			CString  strExceptionDes;
			strExceptionDes.Format("InitDataFunc-相机%d分配算法模板设置所用内存出错!",i+1);
			m_LogManager.MyWriteLogFile("初始化",strExceptionDes,AbnormityLog);
			return FALSE;
		}
	}
	
	try
	{	
		for(i=0; i<ALLELEMENT_COUNT; i++)
		{
			iImageSize = m_sCameraImageInfo[i].m_iCameraImageSize;
			int iTotal = iImageSize*m_iPicCountPerCam[i]*DATA_ELEMENT_COUNT;
			m_pImageAddr[i] = NULL;
			m_pImageAddr[i] = new BYTE[iImageSize*m_iPicCountPerCam[i]*DATA_ELEMENT_COUNT];
			ZeroMemory(m_pImageAddr[i],iImageSize*m_iPicCountPerCam[i]*DATA_ELEMENT_COUNT);
		}
	}
	catch (...)
	{
		CString  strExceptionDes;
		strExceptionDes.Format("InitDataFunc-分配所有信息队列空间出错!");
		m_LogManager.MyWriteLogFile("初始化",strExceptionDes,AbnormityLog);
		return FALSE;
	}


	////////////////////////////////////////////////////////////////
	//[2012-5-2 by zhy]: add
	try
	{
		for(i=0; i<ALLELEMENT_COUNT; i++)
		{
			m_hImageInspectStart[i] = CreateEvent(NULL,FALSE,FALSE,NULL);
			if(ERROR_ALREADY_EXISTS == GetLastError())
			{
				CString strExceptionDes("");
				strExceptionDes.Format("%s","句柄已经存在--CreateEvent!");
				AfxMessageBox(strExceptionDes);
				return FALSE;
			}
			else if(NULL == m_hImageInspectStart[i])
			{
				CString strExceptionDes("");
				strExceptionDes.Format("%s","CreateEvent Fail!");
				AfxMessageBox(strExceptionDes);
				return FALSE;
			}
		}
	}
	catch (...)
	{
		CString strExceptionDes("");
		strExceptionDes.Format("%s","CreateEvent 异常!");
		AfxMessageBox(strExceptionDes);
		return FALSE;
	}



	// 初始化BITMAP信息
	for(i=0;i<CAMERA_COUNT;i++)
	{
		m_pBitmapInfo[i] = NULL;
		m_pBitmapInfo[i] = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 256*sizeof(RGBQUAD)];;
		m_pBitmapInfo[i]->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		m_pBitmapInfo[i]->bmiHeader.biWidth = m_sCameraImageInfo[i].m_iCameraImageWidth;
		m_pBitmapInfo[i]->bmiHeader.biHeight = m_sCameraImageInfo[i].m_iCameraImageHeight;
		m_pBitmapInfo[i]->bmiHeader.biPlanes=1;
		m_pBitmapInfo[i]->bmiHeader.biBitCount = m_sCameraImageInfo[i].m_iCameraImagePixelSize*8;
		m_pBitmapInfo[i]->bmiHeader.biCompression = BI_RGB;
		m_pBitmapInfo[i]->bmiHeader.biSizeImage = 0;
		m_pBitmapInfo[i]->bmiHeader.biXPelsPerMeter =0;
		m_pBitmapInfo[i]->bmiHeader.biYPelsPerMeter = 0;
		m_pBitmapInfo[i]->bmiHeader.biClrUsed = 0;
		m_pBitmapInfo[i]->bmiHeader.biClrImportant = 0;

		//(2003-4-2)
		//对于8位位图来说，其需要调色板。对于256色地位图，调色板数组的个数位256。
		for( int k = 0; k < 256; k++)
		{
			m_pBitmapInfo[i]->bmiColors[k].rgbBlue = (BYTE)k;
			m_pBitmapInfo[i]->bmiColors[k].rgbGreen = (BYTE)k;
			m_pBitmapInfo[i]->bmiColors[k].rgbRed = (BYTE)k;
			m_pBitmapInfo[i]->bmiColors[k].rgbReserved = 0;
		}
	}

	return TRUE;
}

// 释放内存数据区
inline BOOL CMainFrame::ReleaseDataFunc()
{
	int i = 0;
	int j = 0;
	int k = 0;
	int  iAllInfoElementCount=0;
	CString strinfo;

	// 释放Byter变换内存
	for (i=0;i<CAMERA_COUNT;i++)
	{
		if (m_pbRGBImage[i] != NULL)
		{
			delete []m_pbRGBImage[i];
			m_pbRGBImage[i]=NULL;
		}
	}

	TRACE("-----释放 RGB------\r\n");


	// 释放为算法参数设置和修改模板专门开出的内存
	for(i=0;i<CAMERA_COUNT;i++)
	{
		if(m_pAlgElement[i] != NULL)
		{
			delete m_pAlgElement[i];
			m_pAlgElement[i]=NULL;
		}

		if(m_pAlgErrorElement[i] != NULL)
		{
			delete m_pAlgErrorElement[i];
			m_pAlgErrorElement[i]=NULL;
		}	
	}

	TRACE("-----释放 Alg------\r\n");
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//[2012-5-2 by zhy]
	for( i=0; i<ALLELEMENT_COUNT; i++)
	{
		CloseHandle(m_hImageInspectStart[i]);
	}

	TRACE("-----释放 Handle------\r\n");

	for(i=0;i<ALLELEMENT_COUNT;i++)
	{
		if(i==Camera2)
		{
			int ii=0;
		}

		if(m_pImageAddr[i]!=NULL)
		{
			delete m_pImageAddr[i];
			m_pImageAddr[i]=NULL;
		}

		TRACE("-----释放 1------\r\n");
	}
	TRACE("-----释放 Addr------\r\n");


	// 释放图像信息
	for(i=0;i<CAMERA_COUNT;i++)
	{
		if(m_pBitmapInfo[i]!=NULL)
		{
			delete m_pBitmapInfo[i];
			m_pBitmapInfo[i]=NULL;
		}
	}

	TRACE("-----释放 Bitmap------\r\n");

	return TRUE;
}

//相机回调（1工位相机）
void CMainFrame::GlobalAreaGrabOverCallback1 (const s_GBSIGNALINFO* SigInfo)
{
	if (SigInfo && SigInfo->Context)
    {
        CMainFrame* pMF = (CMainFrame*) SigInfo->Context;
		pMF->AreaGrabOverCallback1(SigInfo);
    }
}
void CMainFrame::AreaGrabOverCallback1(const s_GBSIGNALINFO* SigInfo)
{			
	int nGrabComplete=0;
	int iGrabSN=0;
	iGrabSN=SigInfo->nGrabberSN;


// 	if (0 != iGrabSN)
// 		return;
	if(!m_bIsDetecting)
	{
		return;
	}

#ifdef __RealGrabber_
	iGrabSN=SigInfo->nGrabberSN;
#endif
	
	//相机序号不对 需要进行处理
	if(iGrabSN<Camera1)
	{
		CString  strExceptionDes;
		strExceptionDes.Format("AreaGrabOverCallback-相机序号出错!",SigInfo->nGrabberSN+1);
		m_LogManager.MyWriteLogFile("采集回调",strExceptionDes,AbnormityLog);
		return; 
	}

//实际采集使用
#ifdef __RealGrabber_  
	try
	{
		// 获得编码器计数  
		if(m_bIOCardValid)
		{
			//根据项目实际信号来定义  编码器或光电信号(本项目为光电信号)
			ReadMachineSignal(iGrabSN);
		}

		if(!CameraGrabFuncSingle(iGrabSN,iGrabSN))
		{
			CString  strExceptionDes;
			strExceptionDes.Format("CameraGrabFuncSingle-相机%d采集返回FALSE!",SigInfo->nGrabberSN+1);
			m_LogManager.MyWriteLogFile("采集回调",strExceptionDes,OperationLog);
			return;
		}

	}
	catch (...)
	{
		//**************************************还需要进一步处理**********************//
		TRACE("------------------采集回调出错!!!-----------------------\n");
		CString  strExceptionDes;
		strExceptionDes.Format("AreaGrabOverCallback-采集回调出错!");
		m_LogManager.MyWriteLogFile("采集回调",strExceptionDes,AbnormityLog);
	}

#else
	//模拟采集

	try
	{	
		//相机的采集函数
		if(Camera1==iGrabSN)
		{
			if (!CameraGrabFuncSingle(
				iGrabSN,
				Camera1
				))
			{
				CString  strExceptionDes;
				strExceptionDes.Format("CameraGrabFuncSingle-相机%d采集返回FALSE!",SigInfo->nGrabberSN+1);
				m_LogManager.MyWriteLogFile("采集回调",strExceptionDes,OperationLog);
				return;
			}
		}
		else if(Camera2==iGrabSN)
		{
			if (!CameraGrabFuncSingle(
				iGrabSN,
				Camera2
				))
			{
				CString  strExceptionDes;
				strExceptionDes.Format("CameraGrabFuncSingle-相机%d采集返回FALSE!",SigInfo->nGrabberSN+1);
				m_LogManager.MyWriteLogFile("采集回调",strExceptionDes,OperationLog);
				return;
			}
		}
		else if(Camera3==iGrabSN)
		{
			if (!CameraGrabFuncSingle(
				iGrabSN,
				Camera3
				))
			{
				CString  strExceptionDes;
				strExceptionDes.Format("CameraGrabFuncSingle-相机%d采集返回FALSE!",SigInfo->nGrabberSN+1);
				m_LogManager.MyWriteLogFile("采集回调",strExceptionDes,OperationLog);
				return;
			}
		}
	}
	catch (...)
	{
		TRACE("-----------模拟采集出错!--------------\n");
		CString  strExceptionDes;
		strExceptionDes.Format("AreaGrabOverCallback-模拟采集出错!");
		m_LogManager.MyWriteLogFile("采集回调",strExceptionDes,AbnormityLog);
	}
	
#endif
}

// 启动所有线程
inline BOOL CMainFrame::StartAllThread()
{
	// 获取CPU处理核信息 [2/17/2008 Dufeng]
	DWORD dwProcessAffinityMask = 0;
	DWORD dwSystemAffinityMask = 0;
	::GetProcessAffinityMask(GetCurrentProcess(), &dwProcessAffinityMask, &dwSystemAffinityMask) ;	

	CWinThread* pTemp = NULL;

	// 等待加载DEMO
	pTemp=NULL;
	pTemp = AfxBeginThread((AFX_THREADPROC)WaitDemoMould,this);
	if(NULL==pTemp)
	{
		return FALSE;
	}

	// 获取图像号
	pTemp=NULL;
	pTemp = AfxBeginThread((AFX_THREADPROC)GetIOCardImageNumThread,this);
	if(NULL==pTemp)
	{
		return FALSE;
	}
	return TRUE;
}

// 杀死所有线程
inline BOOL CMainFrame::KillAllThread()
{	
	DWORD  wlc = 0;

	m_bWaitDemoMould=TRUE;
	m_bStopReadKickImageNo=TRUE;

	g_pView->m_bQuitAlarmThread = TRUE;

	// 获取计数线程
	m_bCloseCount=TRUE;
	wlc = WaitForSingleObject(m_evtGetIOCardImageNum,100);
	if(wlc==WAIT_OBJECT_0)
	{
		TRACE("...获取计数线程退出成功!\n");
	}


	wlc = WaitForSingleObject(m_evtKillWait,100);
	if(wlc==WAIT_OBJECT_0)
	{
		TRACE("...等待Demo线程退出成功!\n");
	}
	
	return TRUE;
}

DWORD CMainFrame::WaitDemoMould(LPVOID lpParam)
{
	CMainFrame* This = (CMainFrame*)lpParam;
	DWORD dRet = This->WaitDemoMouldFunc();	
	return dRet;
}

DWORD CMainFrame::WaitDemoMouldFunc()
{
	DWORD dRet=0;
	
	while(FALSE==m_bWaitDemoMould)
	{
		dRet = WaitForSingleObject(m_evtTrigDemoMould,10);
		
		if(WAIT_OBJECT_0==dRet)
		{
			CBottleCapView* pView = (CBottleCapView*) g_pView;
			pView->LoadLightAndProductInfo(m_sModelName,m_sOldModelName);
		}
		
		Sleep(10);
		
	}
	
	m_evtKillWait.SetEvent();

	return 0;
}

BOOL  CMainFrame::ThreadDisplayImage( int iCamGroup, int iIndex) //显示图像
{

	if (g_pView != NULL)
	{	
		g_pView->DisplayImage(iCamGroup, iIndex);
	}

	return TRUE;
}


int CMainFrame::SaveAbnormityImageFunc(const char* pPath,int iWidth, int iHeight, int iByte, unsigned char* pAddr)
{
	// 创建文件夹
	int iImageSize;
	iImageSize = iWidth*iHeight*iByte;

	CString str1,str2,strSavePath1;
	str1.Format("%s",pPath);
	if(FALSE==inIsPathExist(str1))
	{
		if(FALSE==inCreateMultiLevelPath(str1))
		{
			str2.Format("%s 文件夹创建失败!",str1);
			return 0;
		}
	}
	
	for(int i=0;i<m_iPicCountPerCam[Camera1];i++)
	{
		strSavePath1.Format("%s\\%d.bmp",str1,i+1);
		savebmp(strSavePath1,iWidth,iHeight,iByte,pAddr+iImageSize);
	}
	
	return 1;
}

int CMainFrame::savebmp(const char *lpszPathName,int W,int H, int nByte, unsigned char *lpDest)
{
	DWORD dwSize = W*H*nByte;
	BITMAPFILEHEADER bmFH;
	int nPaletteEntries = 256;
	
	if(nByte<3)
	{
		nPaletteEntries = 256;
	}
	else
	{
		nPaletteEntries = 0;
	}
	
	bmFH.bfType = 19778;
	//	bmFH.bfType = "BM";
	
	bmFH.bfSize = (DWORD) ( sizeof(BITMAPFILEHEADER) + 
		sizeof(BITMAPINFOHEADER) +
		nPaletteEntries * sizeof(RGBQUAD)) +
		dwSize;
	bmFH.bfReserved1 = bmFH.bfReserved2 = 0;
	bmFH.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + 
		sizeof(BITMAPINFOHEADER) + 
		nPaletteEntries * sizeof(RGBQUAD);
	
	BITMAPINFO *FileBMP;
	char bmpBuf[2048];
	FileBMP = (BITMAPINFO *)bmpBuf;
	FileBMP->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	FileBMP->bmiHeader.biWidth = W;
	FileBMP->bmiHeader.biHeight = H;
	FileBMP->bmiHeader.biPlanes = 1;
	FileBMP->bmiHeader.biBitCount = nByte*8;
	FileBMP->bmiHeader.biCompression = BI_RGB;
	FileBMP->bmiHeader.biSizeImage = 0;
	FileBMP->bmiHeader.biXPelsPerMeter = 0;
	FileBMP->bmiHeader.biYPelsPerMeter = 0;
	FileBMP->bmiHeader.biClrUsed = 0;
	FileBMP->bmiHeader.biClrImportant = 0;
	
	//(2003-4-2)
	//对于8位位图来说，其需要调色板。对于256色地位图，调色板数组的个数位256。
	for( int k = 0; k < 256; k++)
	{
		FileBMP->bmiColors[k].rgbBlue = (BYTE)k;
		FileBMP->bmiColors[k].rgbGreen = (BYTE)k;
		FileBMP->bmiColors[k].rgbRed = (BYTE)k;
		FileBMP->bmiColors[k].rgbReserved = 0;
	}
	
	
	CFile ImageFile;
	DWORD dwOffset = 0;
	
	if( ImageFile.Open(lpszPathName, CFile::modeCreate | CFile::modeReadWrite) )
	{
		ImageFile.Write( &bmFH, sizeof(BITMAPFILEHEADER) ); 
		ImageFile.Write( &FileBMP->bmiHeader, sizeof(BITMAPINFOHEADER) );
		ImageFile.Write( &FileBMP->bmiColors[0], sizeof(RGBQUAD) * nPaletteEntries);
		ImageFile.Write( lpDest, dwSize );	
		ImageFile.Close();
		return 1;
	}
	else
		return -1;
	
}

inline BOOL CMainFrame::LoadAllImage()
{
	CFileFind fd;
	CString strSrcRootPath;
	strSrcRootPath.Format("%s:\\SaveImages",m_sVirtualDisk);
	BOOL bFind = fd.FindFile(strSrcRootPath+"\\*.*");
	CString strSrcPath,strCameraName;
	CString strImageType;
		
	while(bFind)
	{
		//打开子目录，读出文件夹名称
		bFind=fd.FindNextFile();
		if(!fd.IsDots())//AbnormityImages/ErrorImages/GrabImages
		{
			CFileFind fd1;
			strSrcPath.Format("%s\\%s",strSrcRootPath,fd.GetFileName());
			BOOL bFind1=fd1.FindFile(strSrcPath+"\\*.*");
			while (bFind1)
			{
				bFind1=fd1.FindNextFile();
				if (!fd1.IsDots())//2011年4月14日
				{
					CFileFind fd2;
					strSrcPath.Format("%s\\%s\\%s",strSrcRootPath,fd.GetFileName(),fd1.GetFileName());

					BOOL bFind2=fd2.FindFile(strSrcPath+"\\*.*");
					while (bFind2)
					{
						bFind2=fd2.FindNextFile();
						if (!fd2.IsDots())//Camera1
						{
							CFileFind fd3;
							strSrcPath.Format("%s\\%s\\%s\\%s",strSrcRootPath,fd.GetFileName(),fd1.GetFileName(),fd2.GetFileName());
							strCameraName.Format("%s",fd2.GetFileName());
							BOOL bFind3=fd3.FindFile(strSrcPath+"\\*.*");
							while (bFind3)
							{
								bFind3=fd3.FindNextFile();
								if (!fd3.IsDots()&&!fd3.IsDirectory())//.bmp
								{
									CString strDstRootPath;
									CString strDstPath;
									CString strErr;

									CString strBmpFilePath;
									strBmpFilePath.Format("%s",fd3.GetFilePath());
									strImageType.Format("%s",fd.GetFileName());

									strBmpFilePath+='\0';
									OP_Directory(strBmpFilePath,NULL,FO_DELETE,FOF_NOCONFIRMATION | FOF_SIMPLEPROGRESS |FOF_NOERRORUI);
								}//.bmp 的If语句结束
							}   
						}// Camera的If语句结束
					}
				}// 日期的If语句结束
			}
		}
	}

	// 向进度条对话框发命令，表面移动文件操作完成

	return TRUE;
}

void CMainFrame::SoftResetIOCard(CString strModelName)
{
	int iParam1=0,iParam2=0;
	int iNewParam1=0,iNewParam2=0;

	m_bSoftResetIOCard = TRUE;
	CBottleCapView* pView = (CBottleCapView*)GetActiveView();

	KillTimer(ID_START_CHECK_TIMER);
	KillTimer(ID_UPDATE_DEFECT_INFO_TIMER);
	
	int i=0,j=0;
	for(i=0;i<ALLELEMENT_COUNT;i++)
	{
		m_iCheckImageCount[i]=0;
		m_iGrabCircleCount[i]=0;   //将采集计数恢复为0
		m_iCheckErrorCount[i]=0;
		m_iImageCountOverFlowNumber[i]=0;		
		m_iFlowOverImageCountInPE[i]=0;				
		m_iKickFlowNumber[i]=0;						
		
		m_iLastImageCount[i]=0;	
		m_iLastHardImageCount[i]=0;
		m_iLastImageCountInPE[i]=0;	
		m_iLastKickCount[i]=0;							
		m_iLastImageID[i]=0;							

		for(j=0;j<MAX_DEFECT_TYPE_COUNT;j++)
		{
			m_iDefectCount[i][j]=0;
		}
	}

	for(i=0;i<PE_COUNT;i++)
	{
		m_iRC_PE[i]=0;								// 记录光电
		m_iFrameCount[i]=0;
		m_iFrameCountOverFlowNumber[i]=0;
		m_iLastFrameCount[i]=0;
	}

	for(i=0;i<CAMERA_COUNT;i++)
	{
		m_iCamSN[i]=0;
		m_iPicCount[i]=0;
	}

	m_iCheckCountTotal=0;										// 数据库中所用的检测总数
	m_iCheckErrorCountTotal=0;
	m_iCheckGoodCountTotal=0;
	m_iEmptyCount=0;
	m_iLastCheckSpeedCount=0;


	pView->m_TotalData.Display("0","",0,FALSE);					//合格品数归零
	pView->m_TotalGoodData.Display("0","",0,FALSE);				//不合格品计数归零
	pView->m_TotalGoodRatioData.Display("0.0%","",0,FALSE);		//合格率归零


	// 更换批次后，重新初始化数据库
	InitDataBase();
}

// 连接数据库
BOOL CMainFrame::ADOConnectDB()
{
	CString strContext;
	HRESULT hr;
	_bstr_t connStr;
	strContext.Format("Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s\\MP.mdb;Persist Security Info=False",m_sWorkPath);
	connStr=strContext;
	
	try
	{
		(m_Connect).CreateInstance(__uuidof(Connection));
		hr = m_Connect->Open(connStr,"","",adModeUnknown);
	}
	catch (_com_error e)
	{
		AfxMessageBox((char*)e.Description());
	}
	
	m_RecordSet.CreateInstance(__uuidof(Recordset));
	
	return TRUE;
}

//判断数据库中的数据表是否存在 
BOOL CMainFrame::ExistTable(CString strTable)
{
	_variant_t RecordsAffected;
	_bstr_t connStr;
	CString  strSQL;

	// 读取系统表，从系统表中查询是否含有查找的表
	strSQL.Format("select * from MSysObjects where Name='%s'",strTable);
	connStr = strSQL;
	
	(m_RecordSet).CreateInstance(__uuidof(Recordset));
	
	try
	{
		(m_RecordSet)->Open(strSQL.GetBuffer(strSQL.GetLength()),
			_variant_t((IDispatch*)(m_Connect),true),
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch(_com_error* e)
	{
		AfxMessageBox(e->ErrorMessage());
		return 0;
	}
	
	if(!m_RecordSet->ADOEOF)
	{
		(m_RecordSet)->MoveFirst();
	}
	else
	{
		m_RecordSet->Close();
		return 0;
	}

	long lCount=0;

	while(!m_RecordSet->ADOEOF)
	{
		m_RecordSet->MoveNext();
		lCount++;
	}

	if(lCount<=0)
	{
		return FALSE;
	}
	
	m_RecordSet->Close();

	return TRUE;
}

BOOL CMainFrame::CreateTable(CString strTable)
{
	_bstr_t sSql;
	VARIANT RecordAffected;

	CString strSQL;

	if(strTable=="All_Check_Info")
	{
		strSQL.Format("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
			"Create table ",
			strTable,
			"( ID int IDENTITY (1, 1) NOT NULL PRIMARY KEY,",
			"User char(255),",
			"StartTime datetime,",
			"StopTime datetime,",
			"ModelName char(255),",
			"TotalCount char(255),",
			"TotalGood char(255),",
			"TotalBad char(255),",
			"TotalBadRatio char(255),",
			"Total1 char(255),",
			"Good1 char(255),",
			"Bad1 char(255),",
			"BadRatio1 char(255),",
			"Total2 char(255),",
			"Good2 char(255),",
			"Bad2 char(255),",
			"BadRatio2 char(255))");
	}
	else
	{
		strSQL.Format("%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s%s",
			"Create table ",
			strTable,
			"( ID int IDENTITY (1, 1) NOT NULL PRIMARY KEY,",
			"User char(255),",
			"StartTime datetime,",
			"StopTime datetime,",
			"ModelName char(255),",
			"TotalCount int,",
			"TotalGood int,",
			"TotalBad int,",
			"Total1 int,",
			"Bad1 int,",
			"Total2 int,",
			"Bad2 int,",
			"Type1 int,",
			"Type2 int,",
			"Type3 int,",
			"Type4 int,",
			"Type5 int,",
			"Type6 int,",
			"Type7 int,",
			"Type8 int,",
			"Type9 int,",
			"Type10 int,",
			"Type11 int,",
			"Type12 int,",
			"Type13 int,",
			"Type14 int,",
			"Type15 int,",
			"Type16 int,",
			"Type17 int,",
			"Type18 int,",
			"Type19 int,",
			"Type20 int,",
			"Type21 int,",
			"Type22 int,",
			"Type23 int,",
			"Type24 int,",
			"Type25 int,",
			"Type26 int,",
			"Type27 int,",
			"Type28 int,",
			"Type29 int,",
			"Type30 int,",
			"Type31 int,",
			"Type32 int,",
			"Type33 int,",
			"Type34 int,",
			"Type35 int,",
			"Type36 int,",
			"Type37 int,",
			"Type38 int,",
			"Type39 int,",
			"Type40 int,",
			"Type41 int,",
			"Type42 int,",
			"Type43 int,",
			"Type44 int,",
			"Type45 int,",
			"Type46 int,",
			"Type47 int,",
			"Type48 int,",
			"Type49 int,",
			"Type50 int,",
			"Type51 int,",
			"Type52 int,",
			"Type53 int,",
			"Type54 int,",
			"Type55 int,",
			"Type56 int,",
			"Type57 int,",
			"Type58 int,",
			"Type59 int,",
			"Type60 int,",
			"Type61 int,",
			"Type62 int,",
			"Type63 int,",
			"Type64 int,",
			"Type65 int,",
			"Type66 int,",
			"Type67 int,",
			"Type68 int,",
			"Type69 int,",
			"Type70 int,",
			"Type71 int,",
			"Type72 int,",
			"Type73 int,",
			"Type74 int,",
			"Type75 int,",
			"Type76 int,",
			"Type77 int,",
			"Type78 int,",
			"Type79 int,",
			"Type80 int)");
	}


	try
	{
		sSql = strSQL;		
		_CommandPtr rCommand;
		rCommand.CreateInstance(__uuidof(Command));
		
		_variant_t vNULL;
		vNULL.vt = VT_ERROR;
		vNULL.scode = DISP_E_PARAMNOTFOUND;
		rCommand->ActiveConnection = m_Connect;
		rCommand->CommandText = sSql;
		rCommand->Execute(&vNULL,&vNULL,adCmdText);

	}
	catch (_com_error* e)
	{
		AfxMessageBox(e->ErrorMessage());
		return FALSE;
	}

	return TRUE;
}


void CMainFrame::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (nHitTest==HTCAPTION)
	{
		return;
	}

	CFrameWnd::OnNcLButtonDown(nHitTest, point);
}

void CMainFrame::InitDataBase()
{
	m_bSoftResetIOCard = FALSE;

	// 初始化总的检测信息表
	CTime CurrTime = CTime::GetCurrentTime();
	m_sStartTime.Format("%d-%d-%d %d:%d:%d",CurrTime.GetYear(),CurrTime.GetMonth(),CurrTime.GetDay(),
		CurrTime.GetHour(),CurrTime.GetMinute(),CurrTime.GetSecond());
	
	//All_Check_Info表
	CString strTotalCount,strTotalGood,strTotalBad, strTotalBadRatio;
	strTotalCount.Format("%d",m_iCheckCountTotal);
	strTotalBad.Format("%d",m_iCheckErrorCountTotal);
	strTotalGood.Format("%d",m_iCheckGoodCountTotal);
	
	if(m_iCheckErrorCountTotal<=0||m_iCheckCountTotal<=0)
	{
		strTotalBadRatio.Format("%0.3f%%",0.0);
	}
	else
	{
		strTotalBadRatio.Format("%0.3f%%",((float)100*m_iCheckErrorCountTotal)/((float)m_iCheckCountTotal));
	}
	

	s_CheckResultInfo info;
	info.strUser=m_sUserName;
	info.strStartTime=m_sStartTime;
	info.strStopTime=m_sStartTime;
	info.strModelName=m_sModelName;
	info.strTotalCount= strTotalCount;
	info.strTotalGood = strTotalGood;
	info.strTotalBad=strTotalBad;
	info.strTotalBadRatio=strTotalBadRatio;
	info.strTotal1="0";
	info.strGood1="0";
	info.strBad1="0";
	info.strBadRatio1="0";
	info.strTotal2="0";
	info.strGood2="0";
	info.strBad2="0";
	info.strBadRatio2="0";

	InitTableData("All_Check_Info");
	InsertTableData(info,"All_Check_Info");
	SetTimer(ID_START_CHECK_TIMER, 60000, NULL);

	// 缺陷信息统计表
	int i=0,j=0;
	CString strTemp;
	strTemp.Format("%s","Defect_Statistic_Info");
	s_CheckDefectInfo iDefectInfo;
	iDefectInfo.sUser=m_sUserName;
	iDefectInfo.sStartTime=m_sStartTime;
	iDefectInfo.sStopTime=m_sStartTime;
	iDefectInfo.sModelName=m_sModelName;
	iDefectInfo.iTotalCount=0;
	iDefectInfo.iTotalGood=0;
	iDefectInfo.iTotalBad=0;
	iDefectInfo.iTotal1=0;
	iDefectInfo.iBad1=0;
	iDefectInfo.iTotal2=0;
	iDefectInfo.iBad2=0;

	for(i=0;i<ALLELEMENT_COUNT;i++)
	{
		for(j=0;j<MAX_DEFECT_TYPE_COUNT;j++)
		{
			iDefectInfo.iDefectCount[i*MAX_DEFECT_TYPE_COUNT+j]=0;
		}
	}


	InitTableData(strTemp);
	InsertDefectTableData(iDefectInfo,strTemp);
	SetTimer(ID_UPDATE_DEFECT_INFO_TIMER, 1000, NULL);
}

void CMainFrame::InitTableData(CString strTableName)
{
	int nTemp=0;
	BOOL bRet=FALSE;
	CString strTemp(""), strSQL("");
	strSQL.Format("Select * from %s", strTableName);
	(m_RecordSet).CreateInstance(__uuidof(Recordset));

	try
	{
		(m_RecordSet)->Open(strSQL.GetBuffer(strSQL.GetLength()),
			_variant_t((IDispatch*)(m_Connect),true),
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch(_com_error* e)
	{
		AfxMessageBox(e->ErrorMessage());
		return;
	}

	if(!m_RecordSet->ADOEOF)
	{
		(m_RecordSet)->MoveFirst();
	}
	else
	{
		m_RecordSet->Close();
		return;
	}

	_variant_t var;
	while(!(m_RecordSet)->ADOEOF)
	{
		var=m_RecordSet->GetCollect("TotalCount");
		strTemp=(LPCSTR)_bstr_t(var);
		nTemp=atoi(strTemp);
		if(nTemp==0)
		{
			(m_RecordSet)->Delete(adAffectCurrent);
			(m_RecordSet)->Update();
		}
		m_RecordSet->MoveNext();
	}

	///////////////////////////////////////////////
	// [2017-6-14 by zhy]
	// 获得字段个数
	int iField = 0;
	iField = m_RecordSet->GetFields()->Count;
	
	int i=0;
	CString strField;
	for(i=0;i<iField;i++)
	{
		// 获取字段名称
		strField=(LPCSTR)_bstr_t(m_RecordSet->GetFields()->GetItem((long)i)->GetName());
		
		// 查询表中是否存在该字段，存在便不创建
		if(strField=="User")
		{
			bRet = TRUE;
			break;
		}
	}

	m_RecordSet->Close();

	AFX_MANAGE_STATE(AfxGetModuleState());        //以下两句一定要加
	AfxGetModuleState()->m_dwVersion = 0x0601;
	
	CString strDatabase;
	strDatabase.Format(_T("%s\\MP.MDB"),m_sWorkPath);
	
	if(FALSE==bRet)
	{
		// 增加一个字段User
		CDaoDatabase db;
		
		try
		{
			// 打开数据库
			db.Open(strDatabase);
			
			// 打开数据表
			CDaoTableDef td(&db);
			td.Open(strTableName);
			
			// 添加字段
			CDaoFieldInfo info;
			info.m_strName = "User";
			info.m_nType=dbText;
			info.m_lSize=255;
			info.m_lAttributes=dbVariableField;
			info.m_bAllowZeroLength=TRUE;
			info.m_bRequired = FALSE;
			td.CreateField(info);
		}
		catch (CException* e)
		{
			e->ReportError();
		}
		
		if(db.IsOpen())
			db.Close();
	}
	
}

void CMainFrame::InsertTableData(s_CheckResultInfo info, CString strTableName)
{
	CString strSQL("");
	strSQL.Format("Select * from %s", strTableName);
	(m_RecordSet).CreateInstance(__uuidof(Recordset));

	try
	{
		(m_RecordSet)->Open(strSQL.GetBuffer(strSQL.GetLength()),
			_variant_t((IDispatch*)(m_Connect),true),
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch (_com_error* e)
	{
		AfxMessageBox(e->ErrorMessage());
		return;
	}



	if((m_RecordSet)->ADOEOF)
	{
		(m_RecordSet)->AddNew();
		(m_RecordSet)->PutCollect("User",_variant_t(info.strUser)); //[2017-6-14 add]
		(m_RecordSet)->PutCollect("StartTime",_variant_t(info.strStartTime));
		(m_RecordSet)->PutCollect("StopTime",_variant_t(info.strStopTime));
		(m_RecordSet)->PutCollect("ModelName", _variant_t(info.strModelName));
		(m_RecordSet)->PutCollect("TotalCount", _variant_t(info.strTotalCount));
		(m_RecordSet)->PutCollect("TotalGood", _variant_t(info.strTotalGood));
		(m_RecordSet)->PutCollect("TotalBad", _variant_t(info.strTotalBad));
		(m_RecordSet)->PutCollect("TotalBadRatio", _variant_t(info.strTotalBadRatio));

		(m_RecordSet)->PutCollect("Total1", _variant_t(info.strTotal1));
		(m_RecordSet)->PutCollect("Good1", _variant_t(info.strGood1));
		(m_RecordSet)->PutCollect("Bad1", _variant_t(info.strBad1));
		(m_RecordSet)->PutCollect("BadRatio1", _variant_t(info.strBadRatio1));


		(m_RecordSet)->PutCollect("Total2", _variant_t(info.strTotal2));
		(m_RecordSet)->PutCollect("Good2", _variant_t(info.strGood2));
		(m_RecordSet)->PutCollect("Bad2", _variant_t(info.strBad2));
		(m_RecordSet)->PutCollect("BadRatio2", _variant_t(info.strBadRatio2));
		(m_RecordSet)->Update();


	}
	else
	{
		while(!(m_RecordSet)->ADOEOF)
		{

			(m_RecordSet)->MoveNext();

			if((m_RecordSet)->ADOEOF)
			{

				(m_RecordSet)->AddNew();

				(m_RecordSet)->PutCollect("User",_variant_t(info.strUser)); //[2017-6-14 add]

				(m_RecordSet)->PutCollect("StartTime",_variant_t(info.strStartTime));

				(m_RecordSet)->PutCollect("StopTime",_variant_t(info.strStopTime));
	
				(m_RecordSet)->PutCollect("ModelName", _variant_t(info.strModelName));
	
				(m_RecordSet)->PutCollect("TotalCount", _variant_t(info.strTotalCount));

				(m_RecordSet)->PutCollect("TotalGood", _variant_t(info.strTotalGood));

				(m_RecordSet)->PutCollect("TotalBad", _variant_t(info.strTotalBad));

				(m_RecordSet)->PutCollect("TotalBadRatio", _variant_t(info.strTotalBadRatio));

				(m_RecordSet)->PutCollect("Total1", _variant_t(info.strTotal1));

				(m_RecordSet)->PutCollect("Good1", _variant_t(info.strGood1));

				(m_RecordSet)->PutCollect("Bad1", _variant_t(info.strBad1));

				(m_RecordSet)->PutCollect("BadRatio1", _variant_t(info.strBadRatio1));

				(m_RecordSet)->PutCollect("Total2", _variant_t(info.strTotal2));

				(m_RecordSet)->PutCollect("Good2", _variant_t(info.strGood2));

				(m_RecordSet)->PutCollect("Bad2", _variant_t(info.strBad2));

				(m_RecordSet)->PutCollect("BadRatio2", _variant_t(info.strBadRatio2));

				(m_RecordSet)->Update();

				break;
			}
		}
	}

	(m_RecordSet)->Close();

}

void CMainFrame::UpdateTableData(s_CheckResultInfo info, CString strTableName)
{
	CString strSQL("");
	strSQL.Format("Select * from %s", strTableName);
	(m_RecordSet).CreateInstance(__uuidof(Recordset));

	try
	{
		(m_RecordSet)->Open(strSQL.GetBuffer(strSQL.GetLength()),
			_variant_t((IDispatch*)(m_Connect),true),
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch (_com_error * e)
	{
		AfxMessageBox(e->ErrorMessage());
		return;
	}

	if((m_RecordSet)->ADOEOF)
	{
		(m_RecordSet)->PutCollect("User",_variant_t(info.strUser)); //[2017-6-14 add]
		(m_RecordSet)->PutCollect("StartTime",_variant_t(info.strStartTime));
		(m_RecordSet)->PutCollect("StopTime",_variant_t(info.strStopTime));
		(m_RecordSet)->PutCollect("ModelName", _variant_t(info.strModelName));
		(m_RecordSet)->PutCollect("TotalCount", _variant_t(info.strTotalCount));
		(m_RecordSet)->PutCollect("TotalGood", _variant_t(info.strTotalGood));
		(m_RecordSet)->PutCollect("TotalBad", _variant_t(info.strTotalBad));
		(m_RecordSet)->PutCollect("TotalBadRatio", _variant_t(info.strTotalBadRatio));
		(m_RecordSet)->PutCollect("Total1", _variant_t(info.strTotal1));
		(m_RecordSet)->PutCollect("Good1", _variant_t(info.strGood1));
		(m_RecordSet)->PutCollect("Bad1", _variant_t(info.strBad1));
		(m_RecordSet)->PutCollect("BadRatio1", _variant_t(info.strBadRatio1));
		(m_RecordSet)->PutCollect("Total2", _variant_t(info.strTotal2));
		(m_RecordSet)->PutCollect("Good2", _variant_t(info.strGood2));
		(m_RecordSet)->PutCollect("Bad2", _variant_t(info.strBad2));
		(m_RecordSet)->PutCollect("BadRatio2", _variant_t(info.strBadRatio2));
		(m_RecordSet)->Update();
	}
	else
	{
		while(!(m_RecordSet)->ADOEOF)
		{
			(m_RecordSet)->MoveNext();
			if((m_RecordSet)->ADOEOF)
			{
				(m_RecordSet)->MovePrevious();
				(m_RecordSet)->PutCollect("User",_variant_t(info.strUser)); //[2017-6-14 add]
				(m_RecordSet)->PutCollect("StartTime",_variant_t(info.strStartTime));
				(m_RecordSet)->PutCollect("StopTime",_variant_t(info.strStopTime));
				(m_RecordSet)->PutCollect("ModelName", _variant_t(info.strModelName));
				(m_RecordSet)->PutCollect("TotalCount", _variant_t(info.strTotalCount));
				(m_RecordSet)->PutCollect("TotalGood", _variant_t(info.strTotalGood));
				(m_RecordSet)->PutCollect("TotalBad", _variant_t(info.strTotalBad));
				(m_RecordSet)->PutCollect("TotalBadRatio", _variant_t(info.strTotalBadRatio));
				(m_RecordSet)->PutCollect("Total1", _variant_t(info.strTotal1));
				(m_RecordSet)->PutCollect("Good1", _variant_t(info.strGood1));
				(m_RecordSet)->PutCollect("Bad1", _variant_t(info.strBad1));
				(m_RecordSet)->PutCollect("BadRatio1", _variant_t(info.strBadRatio1));
				(m_RecordSet)->PutCollect("Total2", _variant_t(info.strTotal2));
				(m_RecordSet)->PutCollect("Good2", _variant_t(info.strGood2));
				(m_RecordSet)->PutCollect("Bad2", _variant_t(info.strBad2));
				(m_RecordSet)->PutCollect("BadRatio2", _variant_t(info.strBadRatio2));
				(m_RecordSet)->Update();
				break;
			}
		}
	}

	(m_RecordSet)->Close();
}

BOOL CMainFrame::StartCheck()
{
	


	if (g_pView!=NULL)
	{
		g_pView->SetDlgItemText(IDC_BTN_START,"停止检测");
		g_pView->m_BtnStart.SetIcon(IDI_ICON_STOP,BS_LEFT);
	}
	int i=0,j=0;

	for(i=0;i<ALLELEMENT_COUNT;i++)
	{
		m_iCheckImageCount[i]=0;
		m_bDisplayAlgImage[i]=TRUE;
		m_bDisplayImage[i] = TRUE;
#ifdef __SGGrabber_
		m_iCheckErrorCount[i]=0;
		m_iGrabCircleCount[i]=0;
#endif	
	}

	for(i=0;i<CAMERA_COUNT;i++)
	{
		m_iPicCount[i]=0;
	}

	PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,Checking);

	int iParam1=0,iParam2=0;
	int iNewParam1=0,iNewParam2=0;
	
	if (m_bIOCardValid && m_bIOCardInitSuccess) //接口卡有效且初始化成功
	{
		try
		{
			if(e_IOCardType_PIOC == m_eIOCardType)
				m_IOCard[Camera1].enable(true); //[2012-2-1 by zhy]
			else if (e_IOCardType_NewCard == m_eIOCardType)
			{
				if (m_bReadIOCardParamFile)
				{
					CString sPathFileOfIOCardConfig{ _T("") };
					CExFn::GetPrivateProfileString(_T("SystemConfiguration"), _T("PathFileOfInit"), sPathFileOfIOCardConfig, CExFn::GetCurrentAppPath() + _T("\\Config\\AlgConfig.ini"));
					sPathFileOfIOCardConfig = CExFn::GetCurrentAppPath() + _T("\\") + sPathFileOfIOCardConfig;
					BOOL bRet = m_NewIOCard.ReadParameterFile(sPathFileOfIOCardConfig.GetBuffer());
					if (FALSE == bRet)
						return FALSE;
					m_bReadIOCardParamFile = false;
				}

				m_csIOCard.Lock();
				m_NewIOCard.ContinueInCntWork(-1);
				m_csIOCard.Unlock();
				
			}
			

			// 全部剔废
// 			if(TRUE==m_bIsKick[Camera1])
// 			{	
// 				iParam1 = m_IOCard[Camera1].readParam(20);  
// 				iNewParam1 = iParam1 & (65280+128+64+32+16+8+4);
// 				iNewParam1 = iNewParam1 | (2*0+1*0);
// 				
// 				m_IOCard[Camera1].writeParam(20,iNewParam1);		
// 				
// 				// 					m_IOCard[Camera1].writeParam(20,0);		// 参见Register Reference 中的address = 0x428 设置值为00000000 00000000
// 				// 					m_IOCard2.writeParam(20,0);		// 参数为 00000000 00000000			
// 			}
// 			// 1不剔
// 			else
// 			{
// 				iParam1 = m_IOCard[Camera1].readParam(20);  
// 				iNewParam1 = iParam1 & (65280+128+64+32+16+8+4);
// 				iNewParam1 = iNewParam1 | (2*1+1*0);
// 				
// 				m_IOCard[Camera1].writeParam(20,iNewParam1);					
// 			}
		}
		catch (...)
		{
			CString  strExceptionDes;
			strExceptionDes.Format("StartCheck-设置接口卡异常!");
			m_LogManager.MyWriteLogFile("接口卡操作",strExceptionDes,AbnormityLog);
		}
	}
	
	SetTimer(CheckSpeedRealTime,m_iCheckSpeedInterval,NULL);// 检测速度实时计时器计算显

	SetTimer(IO24BSTATUS,200,NULL);

	SetTimer(DisplayCount,100,NULL);

	SetTimer(SaveCount,60000,NULL);

	SetTimer(ShowDataInfo,300,NULL);


	if(TRUE==m_bSoftResetIOCard)
	{
		InitDataBase();
	}

	m_bIsDetecting=TRUE;

	return TRUE;
}

BOOL CMainFrame::StopCheck()
{
	int i=0;

	if (g_pView!=NULL)
	{
		g_pView->SetDlgItemText(IDC_BTN_START,"开始检测");
		g_pView->m_BtnStart.SetIcon(IDI_ICON_START,BS_LEFT);
	}

	m_bIsDetecting=FALSE;
	
	for(i=0;i<ALLELEMENT_COUNT;i++)
	{
		m_bDisplayImage[i] = FALSE;
		m_bDisplayAlgImage[i]=FALSE;
	}
	PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,StopChecked);

#ifdef USE_IO_CARD
	if(m_bIOCardValid && m_bIOCardInitSuccess)
	{
		try
		{
			//禁止接口卡
			if (e_IOCardType_PIOC == m_eIOCardType)
			{
				m_IOCard[Camera1].enable(false); //[2012-2-1 by zhy]
			}
			else if (e_IOCardType_NewCard == m_eIOCardType)
			{
				m_csIOCard.Lock();
				m_NewIOCard.PauseInCntWork(-1);
				m_csIOCard.Unlock();
			}

		}
		catch (...)
		{
			CString  strExceptionDes;
			strExceptionDes.Format("StopCheck-设置接口卡异常!");
			m_LogManager.MyWriteLogFile("接口卡操作",strExceptionDes,AbnormityLog);
		}
	}
#endif
	
	//  [12/3/2009 GZ]
	KillTimer(CheckSpeedRealTime);//停止检测就停止计算检测速度
	KillTimer(IO24BSTATUS);
	KillTimer(DisplayCount);
	KillTimer(SaveCount);
	KillTimer(ShowDataInfo);

	UpdateCheckData();

	return TRUE;
}

// 读取机器信号
inline BOOL CMainFrame::ReadMachineSignal(int iGrabSn)
{
	static int iHardImageCount2=0;
	int nImageNum1 = 0;//**[10/29/2017 ConanSteve]**:  接口卡的相机输出信号计数，也就是图像
	int nImageNum3 = 0;
	
	try
	{
		if(Camera1==iGrabSn)
		{
			if (e_IOCardType_PIOC == m_eIOCardType)
				nImageNum1 = m_IOCard[Camera1].readCounter(28) % 256; //获取图像计数 图像号计数x（对应第1路相机的采图回调）,Out1
			else if (e_IOCardType_NewCard == m_eIOCardType)
				nImageNum1 = m_NewIOCard.ReadOutputCount(1);
		}
		else if(Camera2==iGrabSn)
		{
			if (e_IOCardType_PIOC == m_eIOCardType)
				nImageNum1 = m_IOCard[Camera1].readCounter(31) % 256; //获取图像计数 图像号计数2（对应第2路相机的采图回调）Out2
			else if (e_IOCardType_NewCard == m_eIOCardType)
				nImageNum1 = m_NewIOCard.ReadOutputCount(2);
		}	 
		else if(Camera3==iGrabSn)
		{
			if(e_IOCardType_PIOC == m_eIOCardType)
				nImageNum1	= m_IOCard[Camera1].readCounter(29)%256; //获取图像计数 图像号计数0（对应第3路相机的采图回调）Out3
			else if(e_IOCardType_NewCard == m_eIOCardType)
				nImageNum1 = m_NewIOCard.ReadOutputCount(3);
		}

		// 图像号
		
		//第一个相机的图像软帧计数
		if (m_iLastImageCount[iGrabSn] > 200 && nImageNum1 <= 20)
		{
			m_iImageCountOverFlowNumber[iGrabSn] +=1;
		}

		m_iHardImageCount[iGrabSn] = nImageNum1 + 256*m_iImageCountOverFlowNumber[iGrabSn];
	
		if(nImageNum1!=m_iLastImageCount[iGrabSn])
		{
			m_iLastImageCount[iGrabSn] = nImageNum1;
		}

		return TRUE;
	}
	catch (...)
	{
		CString  strExceptionDes;
		strExceptionDes.Format("ReadMachineSignal-读取机器信号异常!");
		m_LogManager.MyWriteLogFile("接口卡操作",strExceptionDes,AbnormityLog);
		return FALSE;
	}

}

//新添 单个相机单入队的函数 [4/30/2010 GZ]
BOOL  CMainFrame::CameraGrabFuncSingle(int iCameraSN,
									   int iAllElementSN)//单个相机的采集函数
{
	
	if (iCameraSN < 0)
	{
		CString  strExceptionDes;
		strExceptionDes.Format("CameraGrabFuncSingle-相机序号%d错误!",iCameraSN);
		m_LogManager.MyWriteLogFile("采集回调",strExceptionDes,AbnormityLog);
		return FALSE;
	}
	
	BOOL bRet = FALSE;
	int iGrabCircle=0,iPicSN=0;

	bRet = JudgeGrabCircleNew(iCameraSN,iAllElementSN,iGrabCircle,iPicSN);

	try
	{
		if(bRet)
		{	
			CString str1;
			str1.Format("==Image: %06d=Grab: %06d=Pic:%d=%s==\r\n",
				m_iHardImageCount[iAllElementSN],iGrabCircle,iPicSN,GetCurTime());
			
			if(TRUE==m_bStartRecord)
			{
				m_iRC_ImageNum[iAllElementSN]++;
				if(m_iRC_ImageNum[iAllElementSN]>m_iRecord_Set)
				{
					m_iRC_ImageNum[iAllElementSN]=0;
					m_fileImageNum[iAllElementSN].SeekToBegin();
				}
				m_fileImageNum[iAllElementSN].Write(str1.GetBuffer(0),str1.GetLength());
			}
			
			// 若信息元素没使用则开始使用
			m_sElement[iAllElementSN][iGrabCircle%DATA_ELEMENT_COUNT].bIsCheck = m_bIsCheck[iAllElementSN];
			m_sElement[iAllElementSN][iGrabCircle%DATA_ELEMENT_COUNT].bHaveError=FALSE;
			m_sElement[iAllElementSN][iGrabCircle%DATA_ELEMENT_COUNT].iCameraSN=iAllElementSN;
			m_sElement[iAllElementSN][iGrabCircle%DATA_ELEMENT_COUNT].iCircleSN = iGrabCircle;
#ifdef __SGGrabber_
			m_sElement[iAllElementSN][iGrabCircle%DATA_ELEMENT_COUNT].iImageSN = iGrabCircle;	
#else
			m_sElement[iAllElementSN][iGrabCircle%DATA_ELEMENT_COUNT].iImageSN = m_iHardImageCount[iAllElementSN];
#endif

			// 填写其图像数据、采集信息、接口卡信息等				
			int nReturn = 0;
			BYTE* pImageBuffer = NULL;
			int iImageWidth=0, iImageHeight=0, iImageByte=0,iImageSize=0;
			iImageWidth=m_sCameraImageInfo[iCameraSN].m_iCameraImageWidth;
			iImageHeight=m_sCameraImageInfo[iCameraSN].m_iCameraImageHeight;
			iImageByte=m_sCameraImageInfo[iCameraSN].m_iCameraImagePixelSize;
			iImageSize=m_sCameraImageInfo[iCameraSN].m_iCameraImageSize;
			
			// 得到图像内存地址
			bRet = m_CameraGrabber[iCameraSN].GetParamInt(GBImageBufferAddr, nReturn);		
			ASSERT(bRet == TRUE);
			
			pImageBuffer = (BYTE*)nReturn;
			ASSERT(pImageBuffer != NULL);	

			if(iPicSN>=m_iPicCountPerCam[iAllElementSN])
			{
				return FALSE;
			}

			m_sElement[iAllElementSN][iGrabCircle%DATA_ELEMENT_COUNT].iIsUsing=1;

			if(1==m_iCamTestMode)
			{
				if (pImageBuffer != NULL)
				{	
					if(m_BayerTransform[iCameraSN].m_bAreaCamBayer)
					{
						ConvertBayer2Rgb(m_pbRGBImage[iCameraSN],
							pImageBuffer,
							iImageWidth,
							iImageHeight,
							BAYER2RGB_NEIGHBOUR,
							m_pLutR,
							m_pLutG,
							m_pLutB,
							FALSE,
							BAYER_RG);

						int iOffset = iImageSize*m_iPicCountPerCam[iAllElementSN]*(iGrabCircle%DATA_ELEMENT_COUNT);
						
						// 向内存写图像数据
						memcpy(m_pImageAddr[iAllElementSN]+iOffset,
							m_pbRGBImage[iCameraSN],
							iImageSize);
					}
					else
					{
						int iOffset = iImageSize*m_iPicCountPerCam[iAllElementSN]*(iGrabCircle%DATA_ELEMENT_COUNT);
						memcpy(m_pImageAddr[iAllElementSN]+iOffset, pImageBuffer,
							iImageSize);
					}								
					
					// 对采集的图像进行水平或垂直镜像
					if (m_bGrabFlip[iCameraSN])
					{
						int iOffset = iImageSize*m_iPicCountPerCam[iAllElementSN]*(iGrabCircle%DATA_ELEMENT_COUNT);
						MirrorSingleImage(m_pImageAddr[iAllElementSN]+iOffset,
							iImageWidth,iImageHeight,iImageByte*8,m_eShowModeFlip[iCameraSN]);
					}
				}

				ThreadDisplayImage(iAllElementSN,iGrabCircle%DATA_ELEMENT_COUNT);
			}
			else
			{
				if (pImageBuffer != NULL)
				{	
					if(m_BayerTransform[iCameraSN].m_bAreaCamBayer)
					{
						ConvertBayer2Rgb(m_pbRGBImage[iCameraSN],
							pImageBuffer,
							iImageWidth,
							iImageHeight,
							BAYER2RGB_NEIGHBOUR,
							m_pLutR,
							m_pLutG,
							m_pLutB,
							FALSE,
							BAYER_RG);
						
						int iOffset = iImageSize*m_iPicCountPerCam[iAllElementSN]*(iGrabCircle%DATA_ELEMENT_COUNT);

						// 向内存写图像数据
						memcpy(m_pImageAddr[iAllElementSN]+iOffset+iPicSN*iImageSize,
							m_pbRGBImage[iCameraSN],
							iImageSize);
						
					}
					else
					{
						int iOffset = iImageSize*m_iPicCountPerCam[iAllElementSN]*(iGrabCircle%DATA_ELEMENT_COUNT);

						if(TRUE==m_bStartRecord)
						{
							m_iRC_WriteImage[iAllElementSN]++;
							if(m_iRC_WriteImage[iAllElementSN]>m_iRecord_Set)
							{
								m_iRC_WriteImage[iAllElementSN]=0;
								m_fileWriteImage[iAllElementSN].SeekToBegin();
							}
							
							str1.Format("==Image:%d==Grab:%d==pic:%d=%s==%d=%d=%d=\r\n",m_iHardImageCount[iAllElementSN],
								iGrabCircle,iPicSN,GetCurTime(),m_pImageAddr[iAllElementSN],iOffset,iImageSize);
							m_fileWriteImage[iAllElementSN].Write(str1.GetBuffer(0),str1.GetLength());
						}

						memcpy(m_pImageAddr[iAllElementSN]+iOffset+iPicSN*iImageSize, pImageBuffer,
							iImageSize);
					}	
					
					
					// 对采集的图像进行水平或垂直镜像
					if (m_bGrabFlip[iCameraSN])
					{
						int iOffset = iImageSize*m_iPicCountPerCam[iAllElementSN]*(iGrabCircle%DATA_ELEMENT_COUNT);

						MirrorSingleImage(m_pImageAddr[iAllElementSN]+iOffset+iPicSN*iImageSize,
							iImageWidth,iImageHeight,iImageByte*8,m_eShowModeFlip[iCameraSN]);
					}

				
					m_bWriteImage[iAllElementSN][iGrabCircle%256][iPicSN]=TRUE;
										
					// saveImage
					int iTotal=0;
					for(int i=0;i<m_iPicCountPerCam[iAllElementSN];i++)
					{
						if(m_bWriteImage[iAllElementSN][iGrabCircle%256][i])
						{
							iTotal++;
						}
					}
					
					
					if(iTotal==m_iPicCountPerCam[iAllElementSN])
					{
						if(TRUE==m_bStartRecord)
						{
							m_iRC_SetEvent[iAllElementSN]++;
							if(m_iRC_SetEvent[iAllElementSN]>m_iRecord_Set)
							{
								m_iRC_SetEvent[iAllElementSN]=0;
								m_fileSetEvent[iAllElementSN].SeekToBegin();
							}

							str1.Format("==Camera:%d==Image:%d==%s==\r\n",iAllElementSN,m_iHardImageCount[iAllElementSN],GetCurTime());

							m_fileSetEvent[iAllElementSN].Write(str1.GetBuffer(0),str1.GetLength());
						}
						
						for(int i=0;i<PIC_COUNT;i++)
						{
							m_bWriteImage[iAllElementSN][iGrabCircle%256][i]=FALSE;
						}
						
						CString strSavePath;
						CTime curTime = CTime::GetCurrentTime();
						

						s_CheckInParam sIn; 
						ZeroMemory(&sIn,sizeof(s_CheckInParam));


						memcpy(m_chSavePath[iAllElementSN],m_sSaveImagePath[iAllElementSN].GetBuffer(0),m_sSaveImagePath[iAllElementSN].GetLength());
						int iOffset = iImageSize*m_iPicCountPerCam[iAllElementSN]*(iGrabCircle%DATA_ELEMENT_COUNT);

						s_DetectInParam sParam;
						sParam.iCircleIndex=m_iHardImageCount[iAllElementSN];
						sParam.iDisplayMode=m_iImageShowMode;
						sParam.iRejectTest=0;
						sParam.iSaveType=m_iSaveImageType[iAllElementSN];
						sParam.iScreenIndex=0;
						sParam.bSnapScreen=false;
						
						sParam.cSavePath=m_sSaveImagePath[iAllElementSN].GetBuffer(0);
						sParam.pImageInfo=&m_sCamInfo[iAllElementSN];
						sParam.pImageData=(int*)m_pImageAddr[iAllElementSN]+iOffset;

						
						m_arrObjSaveImgPara[iAllElementSN].SetImgWidth(m_sCameraImageInfo[iAllElementSN].m_iCameraImageWidth);
						m_arrObjSaveImgPara[iAllElementSN].SetImgHeight(m_sCameraImageInfo[iAllElementSN].m_iCameraImageHeight);
						m_arrObjSaveImgPara[iAllElementSN].SetImgChannel(m_sCameraImageInfo[iAllElementSN].m_iCameraImagePixelSize);
						m_arrObjSaveImgPara[iAllElementSN].SetImgNum(m_iPicCountPerCam[iAllElementSN]);
						m_arrObjSaveImgPara[iAllElementSN].SetIsCirCleSaveImg( TRUE==m_iIsCircle[iAllElementSN]);
						m_arrObjSaveImgPara[iAllElementSN].SetSaveImgFolderPath(m_sSaveImagePath[iAllElementSN].GetBuffer(), m_sSaveImagePath[iAllElementSN].GetLength());
						m_arrExten[iAllElementSN].clear();
 						m_arrExten[iAllElementSN].push_back((COptionalFunc*)&m_arrObjSaveImgPara[iAllElementSN]);
						sIn.iReserve4 = (int)&m_arrExten[iAllElementSN];
						
						
						sIn.iCheckSN=iAllElementSN;
						sIn.iImageSN=m_iHardImageCount[iAllElementSN];
						sIn.pImageData=(char*)m_pImageAddr[iAllElementSN]+iOffset;


						sIn.pParam=&sParam;
		
						if(m_bOpenImageSave==TRUE)
						{
							CString strSave;
							strSave.Format("%s\\TempImage\\Camera%d",m_sSaveImagePath[iAllElementSN],iAllElementSN+1);
							
							if(!inIsPathExist(strSave))
							{
								if(!inCreateMultiLevelPath(strSave))
								{
									CString str;
									str.Format("创建路径失败:%s",strSave);
									AfxMessageBox(str);
									return FALSE;
								}
							}
							
							if(m_iHardImageCount[iAllElementSN]==5)
							{
								for(int i=0;i<m_iPicCountPerCam[iAllElementSN];i++)
								{
									strSavePath.Format("%s\\%d.bmp",strSave,i+1);
									savebmp(strSavePath,iImageWidth,iImageHeight,iImageByte,
										m_pImageAddr[iAllElementSN]+iOffset+i*iImageSize);
								}
							}
						}
						
						if(TRUE==m_bOpenAlgCheck)
						{
							m_InspectionObj.Detect(sIn);
						}
						m_iCheckImageCount[iAllElementSN]++;
						
						if(m_bDisplayImage[iAllElementSN])
						{
							ThreadDisplayImage(iAllElementSN,iGrabCircle%DATA_ELEMENT_COUNT);
						}
					}
				}
				else
				{
					return FALSE;
				}
			}

			m_sElement[iAllElementSN][iGrabCircle%DATA_ELEMENT_COUNT].iIsUsing=0;
		}
	}
	catch (...)
	{
		TRACE("----------------采集回调:相机%d正常触发采集出错!---------------\n",iCameraSN+1);
		CString  strExceptionDes;
		strExceptionDes.Format("CameraGrabFuncSingle-相机%d正常触发采集出错=%d=%d=%d=!",iCameraSN+1,
			m_iHardImageCount[iAllElementSN],iGrabCircle,iPicSN);
		m_LogManager.MyWriteLogFile("采集回调",strExceptionDes,AbnormityLog);
	}	

	return TRUE;
}

inline BOOL  CMainFrame::JudgeGrabCircleNew(int  iCameraSN,
											int  iAllElementSN,	
											int& iGrabCircle,
											int& iPicSN) 
{	
	CString strCamera1;
	LARGE_INTEGER iLarge1;
	LONGLONG qt1;
	
	m_iPicCount[iCameraSN]++;

	if(TRUE==m_bStartRecord)
	{
		QueryPerformanceCounter(&iLarge1);
		qt1=iLarge1.QuadPart;
		double diffTest=0,dd=0.0;
		
		dd=abs(qt1-m_lCameraTimeInfo[iAllElementSN]);
		diffTest=1000*dd/m_dFreq;
		m_lCameraTimeInfo[iAllElementSN]=qt1;
		
		
		strCamera1.Format("==相机号:%d==图像号:%06d==间隔:%0.0f==采集:%d==Last:%d==%s==\r\n",
			iAllElementSN+1,m_iHardImageCount[iAllElementSN],diffTest,m_iGrabCircleCount[iAllElementSN],
			m_iLastHardImageCount[iAllElementSN],GetCurTime());
		
		m_iRC_Call[iAllElementSN]++;
		if(m_iRC_Call[iAllElementSN]>m_iRecord_Set)
		{
			m_iRC_Call[iAllElementSN]=0;
			m_fileCall[iAllElementSN].SeekToBegin();
		}
		m_fileCall[iAllElementSN].Write(strCamera1.GetBuffer(0),strCamera1.GetLength());
	}


#ifdef __RealGrabber_
	if(m_iHardImageCount[iAllElementSN]!=m_iLastHardImageCount[iAllElementSN])
	{
		m_iCamSN[iCameraSN]=0;
		iPicSN=0;

		m_iGrabCircleCount[iAllElementSN]++;		
		if (m_iGrabCircleCount[iAllElementSN] < 0)
		{
			m_iGrabCircleCount[iAllElementSN]=1;
		}
		iGrabCircle = m_iGrabCircleCount[iAllElementSN];
		m_iLastHardImageCount[iAllElementSN] = m_iHardImageCount[iAllElementSN];
	}
	else
	{
		m_iCamSN[iCameraSN]++;
		iPicSN=m_iCamSN[iCameraSN];
		iGrabCircle = m_iGrabCircleCount[iAllElementSN];
	}
#else
	if((m_iPicCount[iCameraSN]-1)%m_iPicCountPerCam[iCameraSN]==0)
	{
		m_iGrabCircleCount[iAllElementSN]++;
		iGrabCircle=m_iGrabCircleCount[iAllElementSN];
		m_iCamSN[iCameraSN]=(m_iPicCount[iCameraSN]-1)%m_iPicCountPerCam[iCameraSN];
		iPicSN=m_iCamSN[iCameraSN];
	}
	else
	{
		iGrabCircle=m_iGrabCircleCount[iAllElementSN];
		m_iCamSN[iCameraSN]=(m_iPicCount[iCameraSN]-1)%m_iPicCountPerCam[iCameraSN];
		iPicSN=m_iCamSN[iCameraSN];
	}
#endif	

#ifdef __SGGrabber_ 
	m_iHardImageCount[iAllElementSN] = m_iGrabCircleCount[iAllElementSN];
	m_SpendTime[iAllElementSN][m_iHardImageCount[iAllElementSN]%256].Start();
	m_iCheckResultTable[iAllElementSN][m_iHardImageCount[iAllElementSN]%256] = -1;
	m_bBottleCapResult[iAllElementSN][m_iHardImageCount[iAllElementSN]%256] = FALSE;
	m_iImageKickThread[iAllElementSN] = m_iHardImageCount[iAllElementSN];
#endif

	return TRUE;

}

//对图像数据进行镜像
int CMainFrame::MirrorSingleImage(BYTE* pImageBuff, int lImageWidth, int lImageHeight, int lImageBitCount, e_ShowModeFlip  eShowMode) //只有水平和垂直镜像
{
	if (pImageBuff == NULL)
	{
		return ImageBufferError;
	}
	if (lImageWidth<0||lImageHeight<0||lImageBitCount<0)
	{
		return ImageWHBITError;
	}
	
	if (lImageBitCount==8)
	{	
		lImageWidth=WIDTHBYTES(lImageWidth*8);
		if (eShowMode==FlipHorizintal) //水平翻转
		{
			BYTE  tem;
			for (int i=0;i<lImageHeight;i++)
			{
				for (int j=0;j<lImageWidth/2;j++)
				{
					tem=*(pImageBuff+i*lImageWidth+j);
					*(pImageBuff+i*lImageWidth+j)=*(pImageBuff+i*lImageWidth+lImageWidth-1-j);
					*(pImageBuff+i*lImageWidth+lImageWidth-1-j)=tem;
				}
			}
			return TRUE;
		}
		if (eShowMode==FlipVertical) //垂直翻转
		{
			BYTE  tem;
			for (int i=0;i<lImageHeight/2;i++)
			{
				for (int j=0;j<lImageWidth;j++)
				{
					tem=*(pImageBuff+i*lImageWidth+j);
					*(pImageBuff+i*lImageWidth+j)=*(pImageBuff+(lImageHeight-1-i)*lImageWidth+j);
					*(pImageBuff+(lImageHeight-1-i)*lImageWidth+j)=tem;
				}
			}
			return TRUE;
		}
	}
	if (lImageBitCount==24)//24位镜像
	{
		lImageWidth=WIDTHBYTES(lImageWidth*8);
		
		if (eShowMode==FlipHorizintal) //水平翻转
		{
			BYTE  tem1,tem2,tem3;
			for (int i=0;i<lImageHeight;i++)
			{
				for (int j=0;j<lImageWidth/2;j++)
				{
					tem1=*(pImageBuff+i*lImageWidth*3+3*j);
					tem2=*(pImageBuff+i*lImageWidth*3+3*j+1);
					tem3=*(pImageBuff+i*lImageWidth*3+3*j+2);
					*(pImageBuff+i*lImageWidth*3+3*j)=*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j));
					*(pImageBuff+i*lImageWidth*3+3*j+1)=*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+1);
					*(pImageBuff+i*lImageWidth*3+3*j+2)=*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+2);
					*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j))=tem1;
					*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+1)=tem2;
					*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+2)=tem3;
				}
			}
			return TRUE;
		}
		if (eShowMode==FlipVertical) //垂直翻转
		{
			BYTE  tem1,tem2,tem3;
			for (int i=0;i<lImageHeight/2;i++)
			{
				for (int j=0;j<lImageWidth;j++)
				{
					tem1=*(pImageBuff+i*lImageWidth*3+3*j);
					tem2=*(pImageBuff+i*lImageWidth*3+3*j+1);
					tem3=*(pImageBuff+i*lImageWidth*3+3*j+2);
					*(pImageBuff+i*lImageWidth*3+3*j)=*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j);
					*(pImageBuff+i*lImageWidth*3+3*j+1)=*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j+1);
					*(pImageBuff+i*lImageWidth*3+3*j+2)=*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j+2);
					*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j)=tem1;
					*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j+1)=tem2;
					*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j+2)=tem3;
				}
			}
			return TRUE;
		}
	}	
	return TRUE;
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (nIDEvent == StartGrabTimer)
	{
		for (int i=0;i<CAMERA_COUNT;i++)
		{
			if (!StartCameraGrab(i))//开始采集
			{
				KillTimer(StartGrabTimer);
				return;
			} 
		}
		KillTimer(StartGrabTimer);
	}
	else if (nIDEvent == IntervalDisplayTimerOne) // 用于控制相机1的图像显示间隔
	{
		m_bDisplayImage[Camera1] = TRUE;
		m_bDisplayAlgImage[Camera1]=TRUE;
	}
	else if (nIDEvent == IntervalDisplayTimerTwo) // 用于控制相机2的图像显示间隔
	{
		m_bDisplayImage[Camera2] = TRUE;
		m_bDisplayAlgImage[Camera2]=TRUE;
	}
	else if(nIDEvent == ID_START_CHECK_TIMER)
	{

// 		CString strStopTime;
// 		CTime CurrTime = CTime::GetCurrentTime();
// 		strStopTime.Format("%d-%d-%d %d:%d:%d",CurrTime.GetYear(),CurrTime.GetMonth(),CurrTime.GetDay(),
// 			CurrTime.GetHour(),CurrTime.GetMinute(),CurrTime.GetSecond());
// 		
// 		CString strCheckTotal,strBadTotal,strGoodTotal, strBadTotalRatio;
// 		CString strTotal[ALLELEMENT_COUNT],strGood[ALLELEMENT_COUNT];
// 		CString strBad[ALLELEMENT_COUNT],strBadRatio[ALLELEMENT_COUNT];
// 
// 		strCheckTotal.Format("%d",m_iCheckCountTotal);
// 		strBadTotal.Format("%d",m_iCheckErrorCountTotal);
// 		strGoodTotal.Format("%d",m_iCheckGoodCountTotal);
// 		
// 		int iGood=0;
// 		float fRatio=0.0;
// 		
// 		for(int i=0;i<ALLELEMENT_COUNT;i++)
// 		{
// 			strTotal[i].Format("%d",m_iFrameCount[i]);
// 			strBad[i].Format("%d",m_iHardKickCount[i]);
// 			
// 			iGood = m_iFrameCount[i]-m_iHardKickCount[i];
// 			if(iGood<=0)
// 			{
// 				strGood[i].Format("%d",0);
// 			}
// 			else
// 			{
// 				strGood[i].Format("%d",iGood);
// 			}
// 			
// 			if(m_iFrameCount[i]<=0 || m_iHardKickCount[i]<=0)
// 			{
// 				strBadRatio[i].Format("%0.3f%%",0.0);
// 			}
// 			else
// 			{
// 				fRatio = ((float)100*m_iHardKickCount[i])/m_iFrameCount[i];
// 				strBadRatio[i].Format("%0.3f%%",fRatio);
// 			}
// 		}
// 		
// 		if(m_iCheckErrorCountTotal<=0 || m_iCheckCountTotal<=0)
// 		{
// 			strBadTotalRatio.Format("%0.3f%%",0.0);
// 		}
// 		else
// 		{
// 			strBadTotalRatio.Format("%0.3f%%",((float)100*m_iCheckErrorCountTotal)/((float)m_iCheckCountTotal));
// 		}
// 		
// 		s_CheckResultInfo info;
// 		info.strStartTime=m_sStartTime;
// 		info.strStopTime=strStopTime;
// 		info.strModelName=m_sModelName;
// 		info.strTotalCount= strCheckTotal;
// 		info.strTotalGood = strGoodTotal;
// 		info.strTotalBad=strBadTotal;
// 		info.strTotalBadRatio=strBadTotalRatio;
// 		info.strTotal1 = strTotal[Camera1];
// 		info.strGood1 = strGood[Camera1];
// 		info.strBad1 = strBad[Camera1];
// 		info.strBadRatio1 = strBadRatio[Camera1];
// // 		info.strTotal2 = strTotal[Camera2];
// // 		info.strGood2 = strGood[Camera2];
// // 		info.strBad2 = strBad[Camera2];
// // 		info.strBadRatio2 = strBadRatio[Camera2];
// 
// 		
// 		UpdateTableData(info, "All_Check_Info");
	
		UpdateCheckData();
	}
	else if(nIDEvent==ID_UPDATE_DEFECT_INFO_TIMER)
	{
		CString strStopTime;
		CTime CurrTime = CTime::GetCurrentTime();
		strStopTime.Format("%d-%d-%d %d:%d:%d",CurrTime.GetYear(),CurrTime.GetMonth(),CurrTime.GetDay(),
			CurrTime.GetHour(),CurrTime.GetMinute(),CurrTime.GetSecond());

		int i=0,j=0;
		int iGood = 0;
		CString strTemp,strTable;
		s_CheckDefectInfo info;

		strTable.Format("%s","Defect_Statistic_Info");
		info.sUser=m_sUserName;
		info.sStartTime=m_sStartTime;
		info.sStopTime=strStopTime;
		info.sModelName=m_sModelName;
		info.iTotalCount=m_iFrameCount[Camera1];
 		info.iTotalGood=m_iFrameCount[Camera1]-m_iHardKickCount[Camera1];
 		info.iTotalBad=m_iHardKickCount[Camera1];
// 		info.iTotal1=m_iFrameCount[Camera1];
// 		info.iBad1=m_iHardKickCount[Camera1];
// 		info.iTotal2=m_iFrameCount[Camera2];
// 		info.iBad2=m_iHardKickCount[Camera2];
		info.iTotal1=0;
		info.iBad1=0;
		info.iTotal2=0;
		info.iBad2=0;

		for(i=0;i<ALLELEMENT_COUNT;i++)
		{
			for(j=0;j<MAX_DEFECT_TYPE_COUNT;j++)
			{
				info.iDefectCount[i*MAX_DEFECT_TYPE_COUNT+j]=m_iDefectCount[i][j];
			}
		}
		UpdateDefectTableData(info,strTable);	
	}
	else if(nIDEvent == DisplayCount)
	{
		CString strInfo("");
		int iTemp1 = 0, iTemp2=0;
		double dRatio = 0;

		m_iCheckCountTotal = (m_iFrameCount[Camera1]-m_iEmptyCount);

// 		m_iCheckGoodCountTotal = (m_iFrameCount[Camera1]-m_iHardKickCount[Camera1]-m_iEmptyCount);
// 
// 		m_iCheckErrorCountTotal = m_iHardKickCount[Camera1]+m_iEmptyCount;

		m_iCheckGoodCountTotal = (m_iFrameCount[Camera1]-m_iHardKickCount[Camera1]);
		m_iCheckErrorCountTotal = m_iHardKickCount[Camera1]-m_iEmptyCount;

		if(m_iFrameCount[Camera1]>0 && m_iHardKickCount[Camera1]>0)
		{
			m_dwCheckBadRatio = 100*((double)m_iHardKickCount[Camera1]/m_iFrameCount[Camera1]);
		}
		else
		{
			m_dwCheckBadRatio = 100;
		}

		// 设备总数
		strInfo.Format("%d",m_iCheckCountTotal);
		g_pView->m_TotalData.Display(strInfo,"",0,FALSE);
			
		//合格品数
		strInfo.Format("%d",m_iCheckGoodCountTotal);
		g_pView->m_TotalGoodData.Display(strInfo,"",0,FALSE);
		
		//合格率
		float fGoodRate{ 0.0 };
		if (m_iCheckCountTotal != 0)
		{
			fGoodRate = (float)100.0*m_iCheckGoodCountTotal / m_iCheckCountTotal;
		}
		//strInfo.Format("%0.3f%%",(100-m_dwCheckBadRatio));
		strInfo.Format("%0.3f%%", fGoodRate);
		g_pView->m_TotalGoodRatioData.Display(strInfo,"",0,FALSE);
	
	}
	else if (nIDEvent == CheckSpeedRealTime)// 实时计算检测速度
	{

		if (!m_bIsCheck[Camera1])// 如果配置设置3个都不检测则置零 [2012-2-10 by zhy]
		{
			if (g_pView!=NULL)
			{
				CString strTmp;
				strTmp.Format("%d 个/分钟",0);
 				g_pView->m_SpeedData.Display(strTmp.GetBuffer(strTmp.GetLength()),"",0,FALSE);
			}
		}
		else // 只要有检测则计算 光电计数软计数 为计算依据
		{
			CString strTmp;
			int i=0,iCount=0,iNum=0;

			if (g_pView!=NULL)
			{
#ifdef __RealGrabber_
				iCount = (m_iFrameCount[Camera1]-m_iLastCheckSpeedCount)*60000/m_iCheckSpeedInterval;
				strTmp.Format("%d 个/分钟",iCount);// 5秒一次，1分钟统计

				g_pView->m_SpeedData.Display(strTmp,"",0,FALSE);
				m_iLastCheckSpeedCount = m_iFrameCount[Camera1];
#else
				int iCamgroup=0;

				if (m_bIsCheck[Camera1])
					iCamgroup=Camera1;

				iCount = (m_iCheckImageCount[iCamgroup]-m_iLastCheckSpeedCount)*60000/m_iCheckSpeedInterval;
				strTmp.Format("%d 个/分钟", iCount);
 				g_pView->m_SpeedData.Display(strTmp.GetBuffer(strTmp.GetLength()),"",0,FALSE);
				m_iLastCheckSpeedCount = m_iCheckImageCount[iCamgroup];		
#endif
			}		
		}		
	}
	else if(nIDEvent==ShowDataInfo)
	{
		CString strGrab;

		strGrab.Format("光电-%d-采集:%d-%d-%d-剔废:%d-总采集:%d-%d-%d",
			m_iFrameCount[Camera1],
			m_iGrabCircleCount[Camera1],
			m_iGrabCircleCount[Camera2],
			m_iGrabCircleCount[Camera3],
			m_iHardKickCount[Camera1],
			m_iPicCount[Camera1],
			m_iPicCount[Camera2],
			m_iPicCount[Camera3]);

		m_wndStatusBar.SetPaneText(0, strGrab);

		strGrab.Format("检测时间:%.0fms-%.0fms-%.0fms",m_dAlgCheckTime[Camera1],m_dAlgCheckTime[Camera2],m_dAlgCheckTime[Camera3]);
		m_wndStatusBar.SetPaneText(1, strGrab);

		strGrab.Format("错误个数:%d-%d-%d",m_iCheckErrorCount[Camera1],m_iCheckErrorCount[Camera2],m_iCheckErrorCount[Camera3]);
		m_wndStatusBar.SetPaneText(2, strGrab);
	}
	CFrameWnd::OnTimer(nIDEvent);
}

// 开始采集
inline BOOL CMainFrame::StartCameraGrab(const int iCameraSN)
{
	if (iCameraSN >= CAMERA_COUNT)
	{
		CString  strExceptionDes;
		strExceptionDes.Format("StartCameraGrab-相机%d超过相机最大个数!",iCameraSN+1);
		m_LogManager.MyWriteLogFile("采集控制",strExceptionDes,OperationLog);
		return FALSE;
	}
	

	BOOL bReturnValue = FALSE;	
	bReturnValue = m_CameraGrabber[iCameraSN].StartGrab();
	
	//  添加开始采集错误处理
	if(bReturnValue == FALSE)
	{
		s_GBERRORINFO GrabberInfo;
		m_CameraGrabber[iCameraSN].GetLastErrorInfo(&GrabberInfo);
		CString strExceptionDes;
		strExceptionDes.Format("相机%d",iCameraSN+1);
		CString strExceptionContent("");
		strExceptionDes += GrabberInfo.strErrorDescription;
		strExceptionDes += GrabberInfo.strErrorRemark;
		m_LogManager.MyWriteLogFile("采集控制",strExceptionDes,OperationLog);
		return FALSE;
	}
	return TRUE;
}

// 停止采集
inline BOOL CMainFrame::StopCameraGrab(const int iCameraSN)
{
	if (iCameraSN >= CAMERA_COUNT)
	{
		CString  strExceptionDes;
		strExceptionDes.Format("StopCameraGrab-相机%d超过相机最大个数!",iCameraSN+1);
		m_LogManager.MyWriteLogFile("采集控制",strExceptionDes,OperationLog);
		return FALSE;
	}
	
	m_CameraGrabber[iCameraSN].StopGrab();
	
	return TRUE;
}

inline void CMainFrame::QuitCheckProgram()  //退出检测程序
{
	int i=0;

	int cxScreen=0, cyScreen=0;
	CRect rcTemp;
	int x_LD=0, y_LD=0, w_LD=0, h_LD=0;

	m_bCloseCount=TRUE;
	
	KillTimer(ID_START_CHECK_TIMER);
	KillTimer(ID_UPDATE_DEFECT_INFO_TIMER);
	KillTimer(ID_UPDATE_DEFECT_SHOW_INFO);
	
	SaveSystemConfig(m_sSysConfigFileName);
		
	try
	{
		for (i=0;i<CAMERA_COUNT;i++)
		{
			StopCameraGrab(i); //停止采集
		}
		ReleaseGrabber();
		TRACE("-------释放相机采集结束!---------------\n");
	}
	catch (...)
	{
		TRACE("-------释放相机采集出错!---------------\n");
	}

#ifdef USE_IO_CARD
	try
	{
		ReleaseIOCard();
		TRACE("-------释放接口卡结束!---------------\n");
		
	}
	catch (...)
	{
		TRACE("-------释放接口卡出错!---------------\n");
	}		
#endif	
	
	try
	{
		KillAllThread(); //关闭所有线程
		TRACE("-------释放线程结束!---------------\n");
	}
	catch (...)
	{
		TRACE("-------释放线程采集出错!---------------\n");
	}
	
	
	try
	{
		ReleaseDataFunc();
		TRACE("-------释放数据区内存结束------------\r\n");
	}
	catch (...)
	{
		TRACE("-------释放数据区出错!---------------\n");
	}	

	m_InspectionObj.Free();
	
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	CString str;
	switch(wParam)
	{
	case ID_INDICATOR_GRAB:
		{
			CString  strGrab;
			str.Format("采集状态:");
			switch(lParam)
			{
			case Grabbing:
				strGrab.Format("采集:%d",m_iGrabCircleCount[Camera1]);
				break;
			case IOCardInfo:
				{
					str.Format("光电:%d",m_iFrameCount[Camera1]);
					strGrab.Format("-采集:%d-剔废:%d-总采集:%d",m_iGrabCircleCount[Camera1],
						m_iHardKickCount[Camera1],m_iPicCount[Camera1]);
					str += strGrab;
					break;
				}
			default:
				str.Format("采集状态:");
				break;
			}			
		}
		m_wndStatusBar.SetPaneText(0, str);
		return TRUE;
	case ID_INDICATOR_CHECK:
		{
			str.Format("检测信息:");
			switch(lParam)
			{
			case CheckResult:
				str.Format("检测信息:%d-",m_iGrabCircleCount[Camera1]);
				break;
			case AlgCheckTime:
				str.Format("检测时间:%.0fms",m_dAlgCheckTime[Camera1]);
				break;
			default:
				str.Format("检测信息:");
				break;
			}			
		}
		m_wndStatusBar.SetPaneText(1, str);
		return TRUE;
	case ID_INDICATOR_ERROR:
		{
			switch(lParam)
			{
			case CheckResult:
				str.Format("错误个数:%d-",m_iCheckErrorCount[Camera1]);
				break;
			default:
				str.Format("错误个数:");
				break;
			}			
		}
		m_wndStatusBar.SetPaneText(2, str);
		return TRUE;
	case ID_INDICATOR_OPERATION:
		{
			str.Format("工作状态:");
			switch(lParam)
			{
			case SystemStarting:
				str += "正在启动系统...";
				break;
			case SystemStarted:
				str += "系统已启动!!!";
				break;
			case SystemClosing:
				str += "正在退出系统...";
				break;
			case Checking:
				str += "正在检测...";				
				break;
			case StopChecked:
				str += "检测已停止";
				break;
			case Grabbing:
				str.Format("开始采集...");
				break;
			case StopGrabbing:
				str.Format("停止采集");
				break;
			case ContinueImageSaving:
				if(m_iIsCircle[Camera1])
				{
					str.Format("正在循环保存连续图像:%d-%d-%d",
						m_iSaveCircleCameraImageCount[Camera1],
						m_iSaveCircleCameraImageCount[Camera2],
						m_iSaveCircleCameraImageCount[Camera3]);
				}
				else
				{		
					str.Format("正在保存连续图像:%d-%d-%d",
						m_iSaveCameraImageCount[Camera1],
						m_iSaveCameraImageCount[Camera2],
						m_iSaveCameraImageCount[Camera3]);
				}
				break;
			case ErrorImageSaving:
				if(m_iIsCircle[Camera1])
				{
					str.Format("正在循环保存错误图像:%d-%d-%d",
						m_iSaveCircleCameraImageCount[Camera1],
						m_iSaveCircleCameraImageCount[Camera2],
						m_iSaveCircleCameraImageCount[Camera3]);
				}
				else
				{
					str.Format("正在保存错误图像:%d-%d-%d",
						m_iSaveCameraImageCount[Camera1],
						m_iSaveCameraImageCount[Camera2],
						m_iSaveCameraImageCount[Camera3]);
				}
				break;
			case AbnormalImageSaving:
				str.Format("正在保存异常图像:%d",m_iSaveCameraImageCount[Camera1]);
				break;
			case ModelImageSaving:			
				str.Format("正在保存模板图像:%d",m_iSaveCameraImageCount[Camera1]);		
				break;
			case ImageSaved:
				str.Format("图像保存完毕");
				break;
			case StopSave:
				str.Format("停止保存");
				break;
			default:
				str.Format("工作状态");
				break;
			}
		}
		//m_wndStatusBar.SetPaneText(3, str);
		return TRUE;
	}
	return CFrameWnd::OnCommand(wParam, lParam);
}

CString CMainFrame::GetCurTime()
{
	LARGE_INTEGER Large;
	LONGLONG qt;
	int nHour=0,nMinute=0,nSecond=0,nMiliSecond=0;
	QueryPerformanceCounter(&Large);
	qt=Large.QuadPart;
				
	nMiliSecond=qt*1000/m_dFreq;
	nSecond=nMiliSecond/1000;
	nMinute=nSecond/60;
	nHour=nMinute/60;
	
	CString sRet;
	sRet.Format("%02d:%02d:%02d.%03d",nHour,nMinute%60,nSecond%60,nMiliSecond%1000);
	
	return sRet;
}

BOOL CMainFrame::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd* pDemoWnd = NULL;

	switch(pCopyDataStruct->dwData)
	{
	case 10:
		{
			pDemoWnd = CWnd::FindWindow(NULL, m_sDemoName);	
			break;
		}	
	case 30:
		{
			pDemoWnd = CWnd::FindWindow(NULL,m_sDemoName);
			if(NULL==pDemoWnd)
			{
				AfxMessageBox("无法找到算法DEMO窗口");
				break;
			}

			CString strOld,strNew;
			strOld = m_sModelName;
			strNew = (char*)pCopyDataStruct->lpData;
			
		
			LRESULT ret=1;
			s_Status sRet;
			s_AlgModelInput sModelInfo;
			ZeroMemory(&sModelInfo,sizeof(s_AlgModelInput));
			ZeroMemory(&sRet,sizeof(s_Status));
			
			int i=0;
			for(;i<ALLELEMENT_COUNT;i++)
			{
				sModelInfo.iCheckSN=i;
				sModelInfo.iOperSig=0;
				
			}

			if(TRUE==m_bOpenAlgCheck)
			{
				BOOL bRet = FALSE;

				// 需要读参数
				s_Status sRet;
				s_AlgModelInput sModelInfo;
				ZeroMemory(&sModelInfo,sizeof(s_AlgModelInput));
				ZeroMemory(&sRet,sizeof(s_Status));
				
				s_DetectModelInput sParam;
				ZeroMemory(&sParam,sizeof(s_DetectModelInput));
				
				CString strModelPath;
				strModelPath.Format("%s\\ModelInfo\\%s",m_sWorkPath,strNew);
				memcpy(sParam.chPath,strModelPath.GetBuffer(0),strModelPath.GetLength());
				
				CString strLogPath;
				strLogPath.Format("%s\\MendParamLog",m_sLogFilePath);
				memcpy(sParam.chLog,strLogPath.GetBuffer(0),strLogPath.GetLength());
				
				ZeroMemory(sParam.chImageBufferPath,MAX_LENGTH);
				
				int* pModelParam[ALLELEMENT_COUNT];
				
				
				for(i=0;i<ALLELEMENT_COUNT;i++)
				{
					CString strAlgImages;
					
					strAlgImages.Format("%s:\\SaveImages\\AlgImages\\Camera%d",m_sVirtualDisk,i+1);
					memcpy(sParam.chAbnormalPath,strAlgImages.GetBuffer(0),strAlgImages.GetLength());
					
					
					sModelInfo.iCheckSN=i;
					sModelInfo.iOperSig=0;
					
					pModelParam[i] = (int*)&sParam;
					
					sModelInfo.pParam=&pModelParam[i];
					
					sRet = m_InspectionObj.Set(sModelInfo);
					
					if(sRet.iStatusType!=0)
					{
						bRet = FALSE;
						break;
					}
					else
					{
						bRet = TRUE;
					}
				}	
				

				// 加载成功
				if(TRUE==bRet)
				{	
					m_bReadModel = TRUE;
					ret = pDemoWnd->PostMessage(WM_APP+4,NULL,(LPARAM)0);
					m_sModelName = strNew;
					m_sOldModelName = strOld;
					m_evtTrigDemoMould.SetEvent();
				}
				else 
				{
					// 加载失败发送失败命令[2014-11-17]
					m_bReadModel = FALSE;
					ret = pDemoWnd->PostMessage(WM_APP+4,NULL,(LPARAM)1);
					m_sModelName = strNew;
					m_sOldModelName = strOld;
					m_evtTrigDemoMould.SetEvent();
					
				}
			}

			break;
		}		
	}

	return CFrameWnd::OnCopyData(pWnd, pCopyDataStruct);
}

int CMainFrame::RotateImage(BYTE* pSrcImageBuff, int iSrcWidth, int iSrcHeight, int iImageBitCount, float fRotateAngle, 
							BYTE* pDestImageBuff, int& iDestWidth, int& iDestHeight)
{
	
	// 图像每行的字节数
	int    iLineBytes;
	
	// 旋转后图像的宽度（iNewWidth'，必须是4的倍数）
	int    iNewLineBytes;
	
	// 指向源象素的指针
	LPSTR    lpSrc;
	
	// 指向旋转图像对应象素的指针
	LPSTR    lpDst;
	
	// 循环变量（象素在新DIB中的坐标）
	int    i;
	int    j;
	
	// 象素在源DIB中的坐标
	int    i0;
	int    j0;
		
	// 旋转角度的正弦和余弦
	float    fSina, fCosa;
	
	// 源图四个角的坐标（以图像中心为坐标系原点）
	float    fSrcX1,fSrcY1,fSrcX2,fSrcY2,fSrcX3,fSrcY3,fSrcX4,fSrcY4;
	
	// 旋转后四个角的坐标（以图像中心为坐标系原点）
	float    fDstX1,fDstY1,fDstX2,fDstY2,fDstX3,fDstY3,fDstX4,fDstY4;
	
	// 两个中间常量
	float    f1,f2;
	
	// 计算图像每行的字节数
	iLineBytes = WIDTHBYTES(iSrcWidth * 8);
			
	// 计算旋转角度的正弦
	fSina = (float) sin((double)fRotateAngle);
	
	// 计算旋转角度的余弦
	fCosa = (float) cos((double)fRotateAngle);
	
	// 计算原图的四个角的坐标（以图像中心为坐标系原点）
	fSrcX1 = (float) (-(iSrcWidth - 1) / 2);
	fSrcY1 = (float) ( (iSrcHeight - 1) / 2);
	fSrcX2 = (float) ( (iSrcWidth - 1) / 2);
	fSrcY2 = (float) ( (iSrcHeight - 1) / 2);
	fSrcX3 = (float) (-(iSrcWidth - 1) / 2);
	fSrcY3 = (float) (-(iSrcHeight - 1) / 2);
	fSrcX4 = (float) ( (iSrcWidth - 1) / 2);
	fSrcY4 = (float) (-(iSrcHeight - 1) / 2);
	
	// 计算新图四个角的坐标（以图像中心为坐标系原点）
	fDstX1 = fCosa * fSrcX1 + fSina * fSrcY1;
	fDstY1 = -fSina * fSrcX1 + fCosa * fSrcY1;
	fDstX2 = fCosa * fSrcX2 + fSina * fSrcY2;
	fDstY2 = -fSina * fSrcX2 + fCosa * fSrcY2;
	fDstX3 = fCosa * fSrcX3 + fSina * fSrcY3;
	fDstY3 = -fSina * fSrcX3 + fCosa * fSrcY3;
	fDstX4 = fCosa * fSrcX4 + fSina * fSrcY4;
	fDstY4 = -fSina * fSrcX4 + fCosa * fSrcY4;
	
	// 计算旋转后的图像实际宽度
	iDestWidth = (int) ( max( fabs(fDstX4 - fDstX1), fabs(fDstX3 - fDstX2) ) + 0.5);
	
	// 计算新图像每行的字节数
	iNewLineBytes = WIDTHBYTES(iDestWidth * 8);
	
	// 计算旋转后的图像高度
	iDestHeight = (int) ( max( fabs(fDstY4 - fDstY1), fabs(fDstY3 - fDstY2) ) + 0.5);

	if(iDestHeight%4!=0)
	{
		iDestHeight = (iDestHeight/4)*4+4;
	}

	if(iDestWidth%4!=0)
	{
		iDestWidth = (iDestWidth/4)*4+4;
	}
	
	// 两个常数，这样不用以后每次都计算了
	f1 = (float) (-0.5 * (iDestWidth - 1) * fCosa - 0.5 * (iDestHeight - 1) * fSina
		+ 0.5 * (iSrcWidth - 1));
	f2 = (float) ( 0.5 * (iDestWidth - 1) * fSina - 0.5 * (iDestHeight - 1) * fCosa
		+ 0.5 * (iSrcHeight - 1));
		
	// 针对图像每行进行操作
	for(i = 0; i < iDestHeight; i++)
	{
		// 针对图像每列进行操作
		for(j = 0; j < iDestWidth; j++)
		{

			if(8==iImageBitCount)
			{
				// 指向新DIB第i行，第j个象素的指针
				// 注意此处宽度和高度是新DIB的宽度和高度
				lpDst = (char *)pDestImageBuff + iNewLineBytes * (iDestHeight - 1 - i) + j;
				
				// 计算该象素在源DIB中的坐标
				i0 = (int) (-((float) j) * fSina + ((float) i) * fCosa + f2 + 0.5);
				j0 = (int) ( ((float) j) * fCosa + ((float) i) * fSina + f1 + 0.5);
				
				// 判断是否在源图范围内
				if( (j0 >= 0) && (j0 < iSrcWidth) && (i0 >= 0) && (i0 < iSrcHeight))
				{
					// 指向源DIB第i0行，第j0个象素的指针
					lpSrc = (char *)pSrcImageBuff + iLineBytes * (iSrcHeight - 1 - i0) + j0;
					
					// 复制象素
					*lpDst = *lpSrc;
				}
				else
				{
					// 对于源图中没有的象素，直接赋值为255
					* ((unsigned char*)lpDst) = 255;
				}
			}
			else if(24==iImageBitCount)
			{
// 				tem1=*(pImageBuff+i*lImageWidth*3+3*j);
// 				tem2=*(pImageBuff+i*lImageWidth*3+3*j+1);
// 				tem3=*(pImageBuff+i*lImageWidth*3+3*j+2);
// 				*(pImageBuff+i*lImageWidth*3+3*j)=*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j));
// 				*(pImageBuff+i*lImageWidth*3+3*j+1)=*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+1);
// 				*(pImageBuff+i*lImageWidth*3+3*j+2)=*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+2);
// 				*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j))=tem1;
// 				*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+1)=tem2;
// 				*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+2)=tem3;

			}
	
		}	
	}


	return 0;
}

BOOL CMainFrame::AlarmAction(BOOL bStart)
{

#ifdef USE_IO_CARD
	if(bStart)
	{
		if(e_IOCardType_PIOC == m_eIOCardType)
			m_IOCard[m_iAlarmCardNum-1].setCardOutput(m_iAlarmOutPort,1);
	}
	else
	{
		if (e_IOCardType_PIOC == m_eIOCardType)
			m_IOCard[m_iAlarmCardNum-1].setCardOutput(m_iAlarmOutPort,0);
	}
#endif

	return TRUE;
}


// 更新缺陷信息记录
void CMainFrame::UpdateDefectTableData(s_CheckDefectInfo info, CString strTableName)
{
	int i=0,j=0;
	CString strSQL(""),strTemp("");
	strSQL.Format("Select * from %s", strTableName);
	(m_RecordSet).CreateInstance(__uuidof(Recordset));
	
	try
	{
		(m_RecordSet)->Open(strSQL.GetBuffer(strSQL.GetLength()),
			_variant_t((IDispatch*)(m_Connect),true),
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch (_com_error * e)
	{
		AfxMessageBox(e->ErrorMessage());
		return;
	}
	
	if((m_RecordSet)->ADOEOF)
	{
		(m_RecordSet)->PutCollect("User",_variant_t(info.sUser));//[2017-6-14]
		(m_RecordSet)->PutCollect("StartTime",_variant_t(info.sStartTime));
		(m_RecordSet)->PutCollect("StopTime",_variant_t(info.sStopTime));
		(m_RecordSet)->PutCollect("ModelName", _variant_t(info.sModelName));
		(m_RecordSet)->PutCollect("TotalCount", _variant_t((long)info.iTotalCount));
		(m_RecordSet)->PutCollect("TotalGood", _variant_t((long)info.iTotalGood));
		(m_RecordSet)->PutCollect("TotalBad", _variant_t((long)info.iTotalBad));
		(m_RecordSet)->PutCollect("Total1", _variant_t((long)info.iTotal1));
		(m_RecordSet)->PutCollect("Bad1", _variant_t((long)info.iBad1));
		(m_RecordSet)->PutCollect("Total2", _variant_t((long)info.iTotal2));
		(m_RecordSet)->PutCollect("Bad2", _variant_t((long)info.iBad2));

		for(i=0;i<ALLELEMENT_COUNT;i++)
		{
			for(j=0;j<MAX_DEFECT_TYPE_COUNT;j++)
			{
				strTemp.Format("Type%d",(i*MAX_DEFECT_TYPE_COUNT+j+1));
				(m_RecordSet)->PutCollect(_variant_t(strTemp), _variant_t((long)info.iDefectCount[i*MAX_DEFECT_TYPE_COUNT+j]));
			}
		}

		(m_RecordSet)->Update();
	}
	else
	{
		while(!(m_RecordSet)->ADOEOF)
		{
			(m_RecordSet)->MoveNext();
			if((m_RecordSet)->ADOEOF)
			{
				(m_RecordSet)->MovePrevious();
				(m_RecordSet)->PutCollect("User",_variant_t(info.sUser));
				(m_RecordSet)->PutCollect("StartTime",_variant_t(info.sStartTime));
				(m_RecordSet)->PutCollect("StopTime",_variant_t(info.sStopTime));
				(m_RecordSet)->PutCollect("ModelName", _variant_t(info.sModelName));
				(m_RecordSet)->PutCollect("TotalCount", _variant_t((long)info.iTotalCount));
				(m_RecordSet)->PutCollect("TotalGood", _variant_t((long)info.iTotalGood));
				(m_RecordSet)->PutCollect("TotalBad", _variant_t((long)info.iTotalBad));
				(m_RecordSet)->PutCollect("Total1", _variant_t((long)info.iTotal1));
				(m_RecordSet)->PutCollect("Bad1", _variant_t((long)info.iBad1));
				(m_RecordSet)->PutCollect("Total2", _variant_t((long)info.iTotal2));
				(m_RecordSet)->PutCollect("Bad2", _variant_t((long)info.iBad2));
				
				for(i=0;i<ALLELEMENT_COUNT;i++)
				{
					for(j=0;j<MAX_DEFECT_TYPE_COUNT;j++)
					{
						strTemp.Format("Type%d",(i*MAX_DEFECT_TYPE_COUNT+j+1));
						(m_RecordSet)->PutCollect(_variant_t(strTemp), _variant_t((long)info.iDefectCount[i*MAX_DEFECT_TYPE_COUNT+j]));
					}
				}

				(m_RecordSet)->Update();
				break;
			}
		}
	}
	
	(m_RecordSet)->Close();
}	

// 插入一条缺陷信息新记录
void CMainFrame::InsertDefectTableData(s_CheckDefectInfo info, CString strTableName)
{
	int i=0,j=0;
	CString strSQL(""),strTemp("");
	strSQL.Format("Select * from %s", strTableName);
	(m_RecordSet).CreateInstance(__uuidof(Recordset));

	try
	{
		(m_RecordSet)->Open(strSQL.GetBuffer(strSQL.GetLength()),
			_variant_t((IDispatch*)(m_Connect),true),
			adOpenDynamic,
			adLockOptimistic,
			adCmdText);
	}
	catch (_com_error* e)
	{
		AfxMessageBox(e->ErrorMessage());
		return;
	}

	if((m_RecordSet)->ADOEOF)
	{
		(m_RecordSet)->AddNew();
		(m_RecordSet)->PutCollect("User",_variant_t(info.sUser));
		(m_RecordSet)->PutCollect("StartTime",_variant_t(info.sStartTime));
		(m_RecordSet)->PutCollect("StopTime",_variant_t(info.sStopTime));
		(m_RecordSet)->PutCollect("ModelName", _variant_t(info.sModelName));
		(m_RecordSet)->PutCollect("TotalCount", _variant_t((long)info.iTotalCount));
		(m_RecordSet)->PutCollect("TotalGood", _variant_t((long)info.iTotalGood));
		(m_RecordSet)->PutCollect("TotalBad", _variant_t((long)info.iTotalBad));
		(m_RecordSet)->PutCollect("Total1", _variant_t((long)info.iTotal1));
		(m_RecordSet)->PutCollect("Bad1", _variant_t((long)info.iTotalBad));
		(m_RecordSet)->PutCollect("Total2", _variant_t((long)info.iTotalCount));
		(m_RecordSet)->PutCollect("Bad2", _variant_t((long)info.iTotalBad));
		
		for(i=0;i<ALLELEMENT_COUNT;i++)
		{
			for(j=0;j<MAX_DEFECT_TYPE_COUNT;j++)
			{
				strTemp.Format("Type%d",(i*MAX_DEFECT_TYPE_COUNT+j+1));
				(m_RecordSet)->PutCollect(_variant_t(strTemp), _variant_t((long)info.iDefectCount[i*MAX_DEFECT_TYPE_COUNT+j]));
			}
		}

		(m_RecordSet)->Update();
	}
	else
	{
		while(!(m_RecordSet)->ADOEOF)
		{
			(m_RecordSet)->MoveNext();
			if((m_RecordSet)->ADOEOF)
			{
				(m_RecordSet)->AddNew();
				(m_RecordSet)->PutCollect("User",_variant_t(info.sUser));
				(m_RecordSet)->PutCollect("StartTime",_variant_t(info.sStartTime));
				(m_RecordSet)->PutCollect("StopTime",_variant_t(info.sStopTime));
				(m_RecordSet)->PutCollect("ModelName", _variant_t(info.sModelName));
				(m_RecordSet)->PutCollect("TotalCount", _variant_t((long)info.iTotalCount));
				(m_RecordSet)->PutCollect("TotalGood", _variant_t((long)info.iTotalGood));
				(m_RecordSet)->PutCollect("TotalBad", _variant_t((long)info.iTotalBad));
				(m_RecordSet)->PutCollect("Total1", _variant_t((long)info.iTotal1));
				(m_RecordSet)->PutCollect("Bad1", _variant_t((long)info.iTotalBad));
				(m_RecordSet)->PutCollect("Total2", _variant_t((long)info.iTotalCount));
				(m_RecordSet)->PutCollect("Bad2", _variant_t((long)info.iTotalBad));
				
				for(i=0;i<ALLELEMENT_COUNT;i++)
				{
					for(j=0;j<MAX_DEFECT_TYPE_COUNT;j++)
					{
						strTemp.Format("Type%d",(i*MAX_DEFECT_TYPE_COUNT+j+1));
						(m_RecordSet)->PutCollect(_variant_t(strTemp), _variant_t((long)info.iDefectCount[i*MAX_DEFECT_TYPE_COUNT+j]));
					}
				}

				(m_RecordSet)->Update();	
				break;
			}
		}
	}

	(m_RecordSet)->Close();
}


//剔废回调
void CMainFrame::GlobalKickCallbackFunc(const s_KickParam* pParam)
{
	if (pParam && pParam->pThis)
    {
        CMainFrame* pMF = (CMainFrame*) pParam->pThis;
		pMF->KickCallbackFunc1(pParam);
    }
}


void CMainFrame::KickCallbackFunc1(const s_KickParam* sParam)
{

	BOOL bKick[ALLELEMENT_COUNT];
	BOOL bHave[ALLELEMENT_COUNT];
	ZeroMemory(bKick,ALLELEMENT_COUNT*sizeof(BOOL));
	ZeroMemory(bHave,ALLELEMENT_COUNT*sizeof(BOOL));

	s_DetectOutParam sOut[ALLELEMENT_COUNT];
	ZeroMemory(sOut,ALLELEMENT_COUNT*sizeof(s_DetectOutParam));

	memcpy(sOut,sParam->pParam,ALLELEMENT_COUNT*sizeof(s_DetectOutParam));

	int iImageSN=0,iKickSN=0;
	iImageSN = sParam->iImageSN;
	iKickSN = sParam->iKickSN;

	if(iImageSN>m_iFrameCount[Camera1])
	{
		return;
	}

	int i=0;

	for(;i<ALLELEMENT_COUNT;i++)
	{
		m_dAlgCheckTime[i] = sOut[i].iTimeConsuming;

		if(TRUE==m_bSaveCameraImage[i])
		{
			// 循环存图
			if(1==m_iIsCircle[i])
			{
				if(0==m_iSaveImageType[i])		//连续图像
				{
					m_iSaveCircleCameraImageCount[i]++;
				}
				else if(1==m_iSaveImageType[i])	//错误图像
				{
					if(0!=sOut[i].iErrorType && 1!=sOut[i].iErrorType)
					{
						m_iSaveCircleCameraImageCount[i]++;
					}
				}
				
				if(SaveGrabImage==m_iSaveImageType[i])
				{
					PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,ContinueImageSaving);
				}
				else if(SaveErrorImage==m_iSaveImageType[i])
				{
					PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,ErrorImageSaving);
				}
			}
			else
			{
				if(m_iSaveCameraImageCount_Setting[i]>0 || m_bSaveCameraImage[i])
				{
					if(m_iSaveCameraImageCount[i]<m_iSaveCameraImageCount_Setting[i] )
					{
						if(0==m_iSaveImageType[i])		//连续图像
						{
							m_iSaveCameraImageCount[i]++;
						}
						else if(1==m_iSaveImageType[i])	//错误图像
						{
							if(0!=sOut[i].iErrorType && 1!=sOut[i].iErrorType)
							{
								m_iSaveCameraImageCount[i]++;
							}
						}
					}
				}
				
				if(m_iSaveCameraImageCount[Camera1]>=m_iSaveCameraImageCount_Setting[Camera1] &&
					m_iSaveCameraImageCount[Camera2]>=m_iSaveCameraImageCount_Setting[Camera2] &&
					m_iSaveCameraImageCount[Camera3]>=m_iSaveCameraImageCount_Setting[Camera3] )
				{
					m_iSaveCameraImageCount_Setting[Camera1]=0;
					m_iSaveCameraImageCount_Setting[Camera2]=0;
					m_iSaveCameraImageCount_Setting[Camera3]=0;
					
					m_bSaveCameraImage[Camera1]=FALSE;
					m_bSaveCameraImage[Camera2]=FALSE;
					m_bSaveCameraImage[Camera3]=FALSE;
					
					PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,StopSave);			
				}
				else
				{
					if(SaveGrabImage==m_iSaveImageType[i])
					{
						PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,ContinueImageSaving);
					}
					else if(SaveErrorImage==m_iSaveImageType[i])
					{
						PostMessage(WM_COMMAND,ID_INDICATOR_OPERATION,ErrorImageSaving);
					}
				}
			}
		}	
	}

	if(0==m_iKickMode)		//全踢
	{
		m_iCheckErrorCount[iKickSN]++;
		
		if(TRUE==m_bIsKick[iKickSN])
		{
			if(TRUE==m_bIOCardValid)
			{
				if (e_IOCardType_PIOC == m_eIOCardType)
					m_IOCard[Camera1].sendChckRslt(TRUE, iImageSN % 256, Camera1);
				else if (e_IOCardType_NewCard == m_eIOCardType)
					;//**[10/29/2017 ConanSteve]**:  nothing to do
			}
		}
	}
	else if(1==m_iKickMode)	//全部不踢废
	{
//		m_iCheckErrorCount[iKickSN]++;
		
		if(TRUE==m_bIsKick[iKickSN])
		{
			if(TRUE==m_bIOCardValid)
			{
				if (e_IOCardType_PIOC == m_eIOCardType)
					m_IOCard[Camera1].sendChckRslt(FALSE, iImageSN % 256, Camera1);
				else if (e_IOCardType_NewCard == m_eIOCardType)
					m_NewIOCard.SetOutCtrlByResultEx(0, iImageSN, FALSE);
			}
		}
	}
	else if(2==m_iKickMode)	//隔张踢
	{
		if(iImageSN%2==0)
		{
			m_iCheckErrorCount[iKickSN]++;
			
			if(TRUE==m_bIsKick[iKickSN])
			{
				if(TRUE==m_bIOCardValid)
				{
					if (e_IOCardType_PIOC == m_eIOCardType)
						m_IOCard[Camera1].sendChckRslt(TRUE, iImageSN % 256, Camera1);
					else if (e_IOCardType_PIOC == m_eIOCardType)
						m_NewIOCard.SetOutCtrlByResultEx(0, iImageSN, TRUE);
				}
			}
		}
		else
		{
			if(TRUE==m_bIsKick[iKickSN])
			{
				if (e_IOCardType_PIOC == m_eIOCardType)
					m_IOCard[Camera1].sendChckRslt(FALSE, iImageSN % 256, Camera1);
				else if (e_IOCardType_PIOC == m_eIOCardType)
					m_NewIOCard.SetOutCtrlByResultEx(0, iImageSN, FALSE);
			}
		}
	}
	else if(3==m_iKickMode)		//正常剔废
	{
		if(sOut[0].iErrorType==1)
		{
			m_iEmptyCount++;
		}

		for(i=0;i<ALLELEMENT_COUNT;i++)
		{
//			if(sOut[i].iErrorType!=0 && sOut[i].iErrorType!=1)		// 报错 及无瓶
			if(sOut[i].iErrorType!=0)		//无瓶及所有其他错误均踢废[2017-7-20 zhy]
			{	
				m_iCheckErrorCount[i]++;
				
				/*************************************<>************************************[8/28/2017 ConanSteve]*/
				switch (sOut[i].iErrorType)
				{
				case 21://**[8/28/2017 ConanSteve]**:  胶塞缺陷
					++m_iDefectCount[0][0];
					break;
				case 20://**[8/28/2017 ConanSteve]**:  轧盖（封口）缺陷
					++m_iDefectCount[0][1];
					break;
				case 1://**[8/28/2017 ConanSteve]**:  无产品
					++m_iDefectCount[0][2];
					break;
				case 80://**[8/28/2017 ConanSteve]**:  其他缺陷
					++m_iDefectCount[0][3];
					break;
				default:
					break;
				}
				/************************************************************************/
				bHave[i]=TRUE;

				if(TRUE==m_bIsKick[i])
				{
					bKick[i]=TRUE;
				}
				else
				{
					bKick[i]=FALSE;
				}

				if(TRUE==m_bStartRecord)
				{
					CString str1;
					
					str1.Format("KickSN:%d-%d=Cam:%d=Image:%d=ET:%d=Time:%s=\r\n",iKickSN,m_bIsKick[iKickSN],
						i,iImageSN,sOut[i].iErrorType,GetCurTime());
					
					m_iRC_Kick[i]++;
					if(m_iRC_Kick[i]>m_iRecord_Set)
					{
						m_iRC_Kick[i]=0;
						m_fileKick[i].SeekToBegin();
					}
					m_fileKick[i].Write(str1.GetBuffer(0),str1.GetLength());
				}
			}
			else
			{
				bKick[i]=FALSE;

				if(TRUE==m_bStartRecord)
				{
					CString str1;
					
					str1.Format("KickSN:%d-%d=Cam:%d=Image:%d=ET:%d=Time:%s=\r\n",iKickSN,m_bIsKick[iKickSN],i,iImageSN,sOut[i].iErrorType,GetCurTime());
					
					m_iRC_Good[i]++;
					if(m_iRC_Good[i]>m_iRecord_Set)
					{
						m_iRC_Good[i]=0;
						m_fileGood[i].SeekToBegin();
					}
					m_fileGood[i].Write(str1.GetBuffer(0),str1.GetLength());
				}
			}
		}

		if(TRUE==bKick[Camera1] || TRUE==bKick[Camera2] || TRUE==bKick[Camera3])
		{
			if(TRUE==m_bIOCardValid)
			{
				if (e_IOCardType_PIOC == m_eIOCardType)
					m_IOCard[Camera1].sendChckRslt(TRUE, iImageSN % 256, iKickSN);
				else if (e_IOCardType_PIOC == m_eIOCardType)
					m_NewIOCard.SetOutCtrlByResultEx(0, iImageSN, TRUE);
			}
		}
		else
		{
			if(TRUE==m_bIOCardValid)
			{
				if (e_IOCardType_PIOC == m_eIOCardType)
					m_IOCard[Camera1].sendChckRslt(FALSE, iImageSN % 256, iKickSN);
				else if (e_IOCardType_PIOC == m_eIOCardType)
					m_NewIOCard.SetOutCtrlByResultEx(0, iImageSN, FALSE);
			}
		}
	}
}

//状态回调
void CMainFrame::GlobalStatusCallbackFunc(const s_Status* pParam)
{
	if (pParam && pParam->pThis)
    {
        CMainFrame* pMF = (CMainFrame*) pParam->pThis;
		pMF->StatusCallbackFunc1(pParam);
    }
}


void CMainFrame::StatusCallbackFunc1(const s_Status* sParam)
{
	
}

DWORD CMainFrame::GetIOCardImageNumThread(LPVOID lpParam)
{
	CMainFrame* This = (CMainFrame*)lpParam;
	
	DWORD dRet = This->GetIOCardImageNumThreadFunc();
	
	return dRet;
}
DWORD CMainFrame::GetIOCardImageNumThreadFunc()
{
	
	while(m_bCloseCount==FALSE)
	{
		
		if(TRUE==m_bIsDetecting)
		{
			GetCount_Guangdian();
			
			GetCount_Tifei();
			
			GetCount_TuXiangHao();
		}
		Sleep(1);
	}
	
	m_evtGetIOCardImageNum.SetEvent();

	return 0;
}

BOOL CMainFrame::GetCount_Guangdian()
{		
	int i=0;
	int iStatus = 0;		//状态寄存器
	int iIn0 =0;			//卡1 In0口状态
	int iDiffBottle=0;		//两个光电差值
		
	int nPECount[PE_COUNT];
	memset(nPECount,0,sizeof(int)*PE_COUNT);
	
	double dd1[PE_COUNT]={0.0};
	double dDiff[PE_COUNT]={0.0};
	
	LONGLONG lQt1[PE_COUNT]={0};
	LONGLONG lQt2[ALLELEMENT_COUNT]={0};
	LONGLONG lLastQt2[ALLELEMENT_COUNT]={0};
	
	LARGE_INTEGER iLarge[PE_COUNT];
	
	CString strText,strInfo;
	
	//光电计数
#ifdef USE_IO_CARD
	if (e_IOCardType_PIOC == m_eIOCardType)
	{
		nPECount[Camera1] = m_IOCard[Camera1].readCounter(0) % 256;
		nPECount[Camera2] = m_IOCard[Camera1].readCounter(1) % 256;
		nPECount[Camera3] = m_IOCard[Camera1].readCounter(0) % 256;
	}
	else if (e_IOCardType_NewCard == m_eIOCardType)
	{
		nPECount[Camera1] = nPECount[Camera2] = nPECount[Camera3] = m_NewIOCard.ReadInputIndex(0) % 256;
	}
	
#else
	nPECount[Camera1] = m_iGrabCircleCount[Camera1]%256;
	nPECount[Camera2] = m_iGrabCircleCount[Camera2]%256;
	nPECount[Camera3] = m_iGrabCircleCount[Camera3]%256;
#endif
	
	for(i=0;i<PE_COUNT;i++)
	{			
		//第n个光电溢出计数
		if(m_iLastFrameCount[i] > 200 && nPECount[i] <= 50)
		{
			m_iFrameCountOverFlowNumber[i] += 1;
		}
		
		m_iFrameCount[i] = nPECount[i] + 256*m_iFrameCountOverFlowNumber[i];
		
		if(nPECount[i] != m_iLastFrameCount[i])
		{	
			
			QueryPerformanceCounter(&iLarge[i]);
			lQt1[i]=iLarge[i].QuadPart;
			dd1[i]=lQt1[i]-m_lLastQt_PE[i];
			dDiff[i]=1000*dd1[i]/m_dFreq;
			m_lLastQt_PE[i]=lQt1[i];
			
			strInfo.Format("=图像号:%06d=%s=间隔:%0.0f=Last:%d=PE:%d\r\n",
				m_iFrameCount[i],GetCurTime(),dDiff[i],m_iLastFrameCount[i],nPECount[i]);
			
			if(TRUE==m_bStartRecord)
			{
				m_iRC_PE[i]++;
				if(m_iRC_PE[i]>m_iRecord_Set)
				{
					m_iRC_PE[i]=0;
					m_filePE[i].SeekToBegin();
				}
				m_filePE[i].Write(strInfo.GetBuffer(0),strInfo.GetLength());
			}
			m_iLastFrameCount[i] = nPECount[i];// 记录光电n上一帧的数
		}	
	}

	return TRUE;
}

BOOL CMainFrame::GetCount_Tifei()
{
	float iBadRatio=0.0;
	
	int i=0;		
	int nKickCount[ALLELEMENT_COUNT];
	memset(nKickCount,0,sizeof(int)*ALLELEMENT_COUNT);		
	CString strInfo;

#ifdef USE_IO_CARD
	nKickCount[Camera1] = m_IOCard[Camera1].readCounter(4)%256;   //获取相机1的剔废计数
	nKickCount[Camera1] = m_NewIOCard.ReadOutputCtrlCnt(0, 0);
#else
	nKickCount[Camera1] = m_iCheckErrorCount[Camera1]%256;
	nKickCount[Camera2] = m_iCheckErrorCount[Camera2]%256;
#endif
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 判断相机1的剔废计数
	// 判断上一帧的计数为255，这一帧的计数为0，则发生溢出，程序上将帧计数继续累加
	
	for(i=0;i<1;i++)
	{
		if(m_iLastKickCount[i] > 200 && nKickCount[i] <= 50)
		{
			m_iKickFlowNumber[i] += 1;
		}
		// 如果溢出次数不为零则进行累加，并加一 
		m_iHardKickCount[i] = nKickCount[i] + 256*m_iKickFlowNumber[i];	
		
		if(m_iLastKickCount[i]!=nKickCount[i])
		{
			m_iLastKickCount[i] = nKickCount[i];// 记录上一帧的数
			
						
			if(TRUE==m_bStartRecord && TRUE==m_bIsDetecting)
			{
				strInfo.Format("=Group:%d=Image:%d=%s=\r\n",i,m_iHardKickCount[i],
					GetCurTime());

				m_iRC_KickImage[i]++;
				if(m_iRC_KickImage[i]>m_iRecord_Set)
				{
					m_iRC_KickImage[i]=0;
					m_fileKickImage[i].SeekToBegin();
				}
				m_fileKickImage[i].Write(strInfo.GetBuffer(0),strInfo.GetLength());
			}
		}
	}
	
	return TRUE;
}

BOOL CMainFrame::GetCount_TuXiangHao()
{
	int i=0;
	
	int nImageCount[ALLELEMENT_COUNT];
	memset(nImageCount,0,sizeof(int)*ALLELEMENT_COUNT);
		
	int nLastRecordKick[ALLELEMENT_COUNT];
	memset(nLastRecordKick,0,sizeof(int)*ALLELEMENT_COUNT);
	
	double dd1[PE_COUNT]={0.0};
	double dDiff[PE_COUNT]={0.0};
	
	LONGLONG lQt1[PE_COUNT]={0};
	LONGLONG lQt2[ALLELEMENT_COUNT]={0};
	LONGLONG lLastQt1[PE_COUNT]={0};
	
	LARGE_INTEGER iLarge[PE_COUNT];
	
	CString strText,strInfo;

#ifdef USE_IO_CARD
	if (e_IOCardType_PIOC == m_eIOCardType)
	{
		nImageCount[Camera1] = m_IOCard[Camera1].readCounter(28) % 256;
		nImageCount[Camera2] = m_IOCard[Camera1].readCounter(31) % 256;
		nImageCount[Camera3] = m_IOCard[Camera1].readCounter(29) % 256;
	}
	else if (e_IOCardType_NewCard == m_eIOCardType)
	{
		nImageCount[Camera1] = m_NewIOCard.ReadOutputCount(1) % 256;
		nImageCount[Camera2] = m_NewIOCard.ReadOutputCount(2) % 256;
		nImageCount[Camera3] = m_NewIOCard.ReadOutputCount(3) % 256;
	}
#else
	nImageCount[Camera1]=m_iGrabCircleCount[Camera1]%256;
	nImageCount[Camera2]=m_iGrabCircleCount[Camera2]%256;
	nImageCount[Camera3]=m_iGrabCircleCount[Camera3]%256;
#endif
	
	// 图像号寄存器
	for(i=0;i<ALLELEMENT_COUNT;i++)
	{			
		if(m_iLastImageCountInPE[i]>200 && nImageCount[i]<50)
		{
			m_iFlowOverImageCountInPE[i]++;
		}	
		m_iImageCountInPE[i]=nImageCount[i]+256*m_iFlowOverImageCountInPE[i];
	
		if(nImageCount[i] != m_iLastImageCountInPE[i])
		{	
			// [2014-10-21 zhy]
			QueryPerformanceCounter(&iLarge[i]);

			lQt2[i]=iLarge[i].QuadPart;
			dd1[i]=lQt2[i]-m_lLastQt_TX[i];
			dDiff[i]=1000*dd1[i]/m_dFreq;
			m_lLastQt_TX[i]=lQt2[i];		

			strInfo.Format("=图像号:%06d=间隔:%0.0f=%s=\r\n",
				m_iImageCountInPE[i],dDiff[i],GetCurTime());
			
			if(TRUE==m_bStartRecord && TRUE==m_bIsDetecting)
			{
				m_iRC_RecordImage[i]++;
				if(m_iRC_RecordImage[i]>m_iRecord_Set)
				{
					m_iRC_RecordImage[i]=0;
					m_fileRecordImage[i].SeekToBegin();
				}
				m_fileRecordImage[i].Write(strInfo.GetBuffer(0),strInfo.GetLength());
			}
			m_iLastImageCountInPE[i]=nImageCount[i];
		}
	}
	
	return TRUE;
}

BOOL CMainFrame::LoadCamConfig(CString strModelName)
{
	int i=0;

	CString strOri,strAlg,sPathFileInModelInfo, sPathFileInConfig;
	strAlg.Format("%s\\ModelInfo\\%s",m_sWorkPath,strModelName);
//#ifdef __RealGrabber_
	for(i=0;i<CAMERA_COUNT;i++)
	{
		CString strSession{ _T("") }, strDeviceInitFile, str;
		strSession.Format("设备%c初始化文件名", 'A' + i);
		CExFn::GetPrivateProfileString("采集参数", strSession, strDeviceInitFile,m_sSysConfigFileName);
		//strDeviceInitFile.Format("%s\\config\\%s", m_sWorkPath, str);

		sPathFileInModelInfo.Format("%s\\%s",strAlg, strDeviceInitFile);
		sPathFileInConfig = CExFn::GetCurrentAppPath() + _T("\\Config\\") + strDeviceInitFile;
		if(FALSE==PathFileExists(sPathFileInModelInfo))
		{
			CopyFile(sPathFileInConfig, sPathFileInModelInfo,false);
		}
		else
		{
			CopyFile(sPathFileInModelInfo, sPathFileInConfig,false);
		}
	}
//#endif
	return TRUE;
}

void CMainFrame::UpdateCheckData()
{
	if(TRUE==m_bUpdateCheckData)
	{
		return;
	}

	m_bUpdateCheckData=TRUE;
	CString strStopTime;
	CTime CurrTime = CTime::GetCurrentTime();
	strStopTime.Format("%d-%d-%d %d:%d:%d",CurrTime.GetYear(),CurrTime.GetMonth(),CurrTime.GetDay(),
		CurrTime.GetHour(),CurrTime.GetMinute(),CurrTime.GetSecond());
	
	CString strCheckTotal,strBadTotal,strGoodTotal, strBadTotalRatio;
	CString strTotal[ALLELEMENT_COUNT],strGood[ALLELEMENT_COUNT];
	CString strBad[ALLELEMENT_COUNT],strBadRatio[ALLELEMENT_COUNT];
	
	strCheckTotal.Format("%d",m_iCheckCountTotal);
	strBadTotal.Format("%d",m_iCheckErrorCountTotal);
	strGoodTotal.Format("%d",m_iCheckGoodCountTotal);
	
	int iGood=0;
	float fRatio=0.0;
	
	for(int i=0;i<ALLELEMENT_COUNT;i++)
	{
		strTotal[i].Format("%d",m_iFrameCount[i]);
		strBad[i].Format("%d",m_iHardKickCount[i]);
		
		iGood = m_iFrameCount[i]-m_iHardKickCount[i];
		if(iGood<=0)
		{
			strGood[i].Format("%d",0);
		}
		else
		{
			strGood[i].Format("%d",iGood);
		}
		
		if(m_iFrameCount[i]<=0 || m_iHardKickCount[i]<=0)
		{
			strBadRatio[i].Format("%0.3f%%",0.0);
		}
		else
		{
			fRatio = ((float)100*m_iHardKickCount[i])/m_iFrameCount[i];
			strBadRatio[i].Format("%0.3f%%",fRatio);
		}
	}
	
	if(m_iCheckErrorCountTotal<=0 || m_iCheckCountTotal<=0)
	{
		strBadTotalRatio.Format("%0.3f%%",0.0);
	}
	else
	{
		strBadTotalRatio.Format("%0.3f%%",((float)100*m_iCheckErrorCountTotal)/((float)m_iCheckCountTotal));
	}
	

	s_CheckResultInfo info;
	info.strUser=m_sUserName;
	info.strStartTime=m_sStartTime;
	info.strStopTime=strStopTime;
	info.strModelName=m_sModelName;
	info.strTotalCount= strCheckTotal;
	info.strTotalGood = strGoodTotal;
	info.strTotalBad=strBadTotal;
	info.strTotalBadRatio=strBadTotalRatio;
	info.strTotal1 = strTotal[Camera1];
	info.strGood1 = strGood[Camera1];
	info.strBad1 = strBad[Camera1];
	info.strBadRatio1 = strBadRatio[Camera1];
	// 		info.strTotal2 = strTotal[Camera2];
	// 		info.strGood2 = strGood[Camera2];
	// 		info.strBad2 = strBad[Camera2];
	// 		info.strBadRatio2 = strBadRatio[Camera2];
	
	
		UpdateTableData(info, "All_Check_Info");

		m_bUpdateCheckData=FALSE;
}

BOOL CMainFrame::DeleteDirectory(char* sDirName)
{

	CFileFind tempFind;

	char sTempFileFind[MAX_PATH];

	sprintf(sTempFileFind, "%s/*.*", sDirName);
	BOOL IsFinded = tempFind.FindFile(sTempFileFind);
	while (IsFinded)
	{

		IsFinded = tempFind.FindNextFile();

		if (!tempFind.IsDots())
		{
			char sFoundFileName[MAX_PATH];
			strcpy(sFoundFileName, tempFind.GetFileName().GetBuffer(MAX_PATH));

			if (tempFind.IsDirectory())
			{
				char sTempDir[MAX_PATH];
				sprintf(sTempDir, "%s/%s", sDirName, sFoundFileName);
				DeleteDirectory(sTempDir);
			}	//张凡 2017/7/7 星期五 16:05:37	
			else
			{
				char sTempFileName[MAX_PATH];
				sprintf(sTempFileName, "%s/%s", sDirName, sFoundFileName);
				DeleteFile(sTempFileName);
			}
		}
	}
	tempFind.Close();
	if (!RemoveDirectory(sDirName))
	{
		return FALSE;
	}
	return TRUE;
}


