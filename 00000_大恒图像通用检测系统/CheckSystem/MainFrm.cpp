
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CheckSystem.h"
#include "CheckSystemDoc.h"
#include "CheckSystemView.h"
#include "DlgDefectStatistics.h"
#include "DlgDataStatistics.h"

#include "MainFrm.h"
#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame


extern CMainFrame*		g_pFrame; //全局指针
extern CCheckSystemDoc*		g_pDoc;
extern CCheckSystemView*	g_pView;

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

static UINT indicators[] =
{
//	ID_SEPARATOR,           // status line indicator
// 	ID_INDICATOR_CAPS,
// 	ID_INDICATOR_NUM,
// 	ID_INDICATOR_SCRL,
ID_INDICATOR_OPERATION,
ID_INDICATOR_GRAB,
ID_INDICATOR_CHECK,
ID_INDICATOR_ERROR,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	for (int i = 0; i < 256; i++)
	{
		m_pLutR[i] = min((int)(i * 0), 255);
		m_pLutG[i] = min((int)(i * 0), 255);
		m_pLutB[i] = min((int)(i * 0), 255);
	}
	CoInitialize(NULL);
}

CMainFrame::~CMainFrame()
{
	
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	//ShowWindow(SW_MAXIMIZE);
	//设置状态栏
	m_wndStatusBar.SetPaneInfo(0, ID_INDICATOR_OPERATION, SBPS_NORMAL, 300);
	m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR_GRAB, SBPS_NORMAL, 1800);  
// 	m_wndStatusBar.SetPaneInfo(2, ID_INDICATOR_CHECK, SBPS_NORMAL, 150);
// 	m_wndStatusBar.SetPaneInfo(3, ID_INDICATOR_ERROR, SBPS_NORMAL, 200);
	//Sleep(10000);
	
	//HWND hwnd1 = g_pView->m_pdlgRunning->GetDlgItem(ID_STATIC_ALGORITHM_IMAGE + 0)->GetSafeHwnd();
	//Sleep(100)
	//InitMultiObject();
	InitializeSystem();

	return 0;
}


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


// CMainFrame message handlers



BOOL CMainFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, LPCTSTR lpszMenuName, DWORD dwExStyle, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	return CFrameWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, lpszMenuName, dwExStyle, pContext);
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
	cs.hMenu = NULL;

	m_cxScreenSize = GetSystemMetrics(SM_CXSCREEN);
	m_cyScreenSize = GetSystemMetrics(SM_CYSCREEN);
	cs.style = WS_POPUP;
	cs.x = 0;
	cs.y = 0;
	cs.cx = m_cxScreenSize;
	cs.cy = m_cyScreenSize;
	CString sPathFileOfUICfg{ _T("") };
	sPathFileOfUICfg.Format(_T("%s_%d_%d.ini"), CExFn::GetCurrentAppPath() + _T("\\Config\\UIConfig"), m_cxScreenSize, m_cyScreenSize);
	CINICfg iniUICfg(sPathFileOfUICfg);
	int iWindowType = iniUICfg.GetIntEx(_T("基础参数"), _T("主窗口类型"), 0, _T("0：最大化， 1：自定义大小"));
	int iWidth = 0, iHeight = 0;
	iWidth = iniUICfg.GetIntEx(_T("基础参数"), _T("自定义宽度"), 0);
	iHeight = iniUICfg.GetIntEx(_T("基础参数"), _T("自定义高度"), 0);
	if (1 == iWindowType)
	{
		if (100 < iWidth&&iWidth<cs.cx)
			cs.cx = iWidth;
		if(100<iHeight&&iHeight<cs.cy)
			cs.cy = iHeight;
	}
	return CFrameWnd::PreCreateWindow(cs);
}


BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return CFrameWnd::OnEraseBkgnd(pDC);
}

void CMainFrame::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	m_bIsDetecting = false;
	bExit = true;
	Sleep(200);
	ReleaseMemory();
	ReleaseResource();
	WriteCfg();

	KillTimer(ID_EVENT_UPDATE_INFO);
	KillTimer(ID_EVENT_UPDATE_CHECK_SPEED);
	//exit(-1);
	CFrameWnd::OnClose();
}


bool CMainFrame::ReadCfg()
{
	bool bRet = false;
	bRet = ReadCfgOfUI();
	if(!bRet)
		return false;
	bRet = ReadCfgOfAlg();
	if (!bRet)
		return false;
	bRet = ReadCfgOfSys();
	if (!bRet)
		return false;
	return true;
}


bool CMainFrame::InitializeSystem()
{
	m_sPathOfApp = CExFn::GetCurrentAppPath();
	m_sPathOfCfg = m_sPathOfApp + _T("\\Config");
	m_sPathFileOfAlgCfg = m_sPathOfApp + _T("\\Config\\AlgConfig.ini");
	m_iniAlgConfig.Init(m_sPathFileOfAlgCfg);//**[11/16/2017 ConanSteve]**:  初始化AlgConfig.ini文件
	m_xmlAlgConfig.Init(m_sPathOfApp + _T("\\Config\\AlgConfig.xml"));
	/*************************************<>************************************[11/16/2017 ConanSteve]*/
	ReadCfg();//**[8/25/2017 ConanSteve]**:  读取配置文件
	/*************************************<开启后台初始化进程>************************************[8/25/2017 ConanSteve]*/
	//HANDLE hThred = (HANDLE)_beginthreadex(NULL, 0, (unsigned(_stdcall*)(void*))ThreadInitBackstage, this, 0, NULL);
	//HANDLE hThred = chBEGINTHREADEX(NULL, 0, ThreadInitBackstage, this, 0, NULL);
	//CloseHandle(hThred);
	//ThreadInitBackstageFunc(1);

	return true;
}


bool CMainFrame::ReadCfgOfUI()
{
	return true;
}


bool CMainFrame::ReleaseMemory()
{

	return false;
}




bool CMainFrame::ReadCfgOfAlg()
{
	/*************************************<获取运行检测模式>************************************[8/25/2017 ConanSteve]*/
	/************************************************************************/
	CString sHashKey{ _T("") }, sHashValue{ _T("") }, sAppName{_T("")};
	int iKeyValue{ 0 };
	m_sPathOfVirtualDisk = m_iniAlgConfig.GetStringEx(_T("Demo Controls"), _T("VirtualDisk"), m_sPathOfVirtualDisk, _T("M"));
	m_sPathOfVirtualDisk += _T(":");
	if (!PathFileExists(m_sPathOfVirtualDisk))
	{
		//**[8/25/2017 ConanSteve]**:  虚拟硬盘路径不存在，请重新确认
		AfxMessageBox(tr("The path of virtual disk doesn't exist, please check it again!"));
		exit(0);
	}

	bool bDisableCreate = m_iniAlgConfig.GetBoolEx(_T("DEMO Controls"), _T("DisableCreate"), true);

	/*************************************<写Demo三个按钮的名称>************************************[8/25/2017 ConanSteve]*/ 
	m_iniAlgConfig.WriteString(_T("DEMO Controls"), _T("ImgBtn1Text"), tr("Locate Grab Images"));
	m_iniAlgConfig.WriteString(_T("DEMO Controls"), _T("ImgBtn2Text"), tr("Locate Error Images"));
	m_iniAlgConfig.WriteString(_T("DEMO Controls"), _T("ImgBtn3Text"), tr("Locate Not Empty Images"));


	/*************************************<写入三个图像文件夹路径>************************************[8/25/2017 ConanSteve]*/ 
	sHashValue = m_sPathOfVirtualDisk + _T("\\SaveImages\\GrabImages");
	m_iniAlgConfig.WriteString(_T("DEMO Controls"), _T("ImgBtn1Path"), sHashValue);
	sHashValue = m_sPathOfVirtualDisk + _T("\\SaveImages\\ErrorImages");
	m_iniAlgConfig.WriteString(_T("DEMO Controls"), _T("ImgBtn2Path"), sHashValue);
	sHashValue = m_sPathOfVirtualDisk + _T("\\SaveImages\\ModelImages");
	m_iniAlgConfig.WriteString(_T("DEMO Controls"), _T("ImgBtn3Path"), sHashValue);

	/*************************************<写入点击序列图文件夹检测相关参数>************************************[8/25/2017 ConanSteve]*/
	m_iniAlgConfig.GetBoolEx(_T("DEMO Controls"), _T("EnableSequentialCheck"), FALSE, _T("是否打开序列图文件夹检测"));//**[8/24/2017 ConanSteve]**:  是否打开Demo序列图检测功能	
	/*************************************<Demo序列图检测的文件夹前缀>*************************************/ 
	m_iniAlgConfig.GetStringEx(_T("DEMO Controls"), _T("SequentialCheckFolderFilter"), sHashValue, _T(""));
	/*************************************<读取上次使用模板名称>*************************************[8/25/2017 ConanSteve]*/
	m_sModelName = m_iniAlgConfig.GetStringEx(_T("Demo Controls"), _T("DefaultModel"), m_sModelName);
	/*************************************<创建日志文件夹路径>*************************************/ 
	m_sPathOfSysLogs = m_sPathOfVirtualDisk + _T("\\LogFiles\\CheckSystemLogs");
	//CExFn::DeleteDirectory(m_sPathOfSysLogs);
	CExFn::CreateMultiDirectory(m_sPathOfSysLogs);
	m_sPathFileOfDebugInfo = m_sPathOfSysLogs + _T("\\SystemDebugInfo.txt");
	m_fileDebugInfo.Open(m_sPathFileOfDebugInfo);
	m_fileErrorInfo.Open(m_sPathOfSysLogs + _T("\\SystemErrorInfo.txt"));
	/*************************************<系统相关>************************************[9/5/2017 ConanSteve]*/
	m_bEnableDrawImg = m_iniAlgConfig.GetBoolEx(_T("SystemConfiguration"), _T("EnableDrawImage"), false, _T("系统是否画图"));
	m_bEnableInitIOCard = TRUE == GetPrivateProfileInt(_T("SystemConfiguration"), _T("EnableInitializeIOCard"), TRUE, m_sPathFileOfAlgCfg);
	CExFn::WritePrivateProfileInt(_T("SystemConfiguration"), _T("EnableInitializeIOCard"), m_bEnableInitIOCard ? TRUE : FALSE, m_sPathFileOfAlgCfg, _T("是否初始化接口卡"));
	m_bEnableInitGrabber = m_iniAlgConfig.GetBoolEx(_T("SystemConfiguration"), _T("EnableInitializeGrabber"), true, _T("是否初始化相机"));
	//CExFn::WritePrivateProfileInt(_T("SystemConfiguration"), _T("EnableInitializeGrabber"), m_bEnableInitGrabber ? TRUE : FALSE, m_sPathFileOfAlgCfg, _T("是否初始化相机"));
	m_bEnableInitMultiObjectAndCheck = m_iniAlgConfig.GetBoolEx(_T("SystemConfiguration"), _T("EnableInitMultiObjectAndCheck"), true, _T("是否执行多对象的初始化和算法检测工作"));
	/*************************************<状态栏信息显示控制>************************************[12/28/2017 ConanSteve]*/
	sAppName = _T("状态栏信息显示控制");
	m_bEnableShowPE = m_iniAlgConfig.GetBoolEx(sAppName, _T("光电输入是否显示"), true);
	m_bEnableShowAutoKick = m_iniAlgConfig.GetBoolEx(sAppName, _T("补踢计数是否显示"), true);
	m_bEnableShowGrabLostFrame= m_iniAlgConfig.GetBoolEx(sAppName, _T("采图丢帧是否显示"), true);
	/*************************************<检查算法配置文件夹是否存在>*************************************/
	m_sPathOfAlgModelInfo= m_sPathOfApp+_T("\\ModelInfo");
	if (!PathFileExists(m_sPathOfAlgModelInfo))
	{
		if (FALSE == CreateDirectory(m_sPathOfAlgModelInfo, NULL))
		{
			AfxMessageBox(tr("创建路径失败, 请确认本地算法配置文件夹是否设置正确!!!"));
			return false;
		}
	}
	m_sPathOfAlgModelInfo += _T("\\") + m_sModelName;
	/*************************************<检查ModeInfo下是否有CheckSystemConfig.ini文件>************************************[9/1/2017 ConanSteve]*/
	m_sPathFileOfCheckSysCfgInModelInfo = m_sPathOfAlgModelInfo + _T("\\CheckSystemConfig.ini");
	if (FALSE == PathFileExists(m_sPathFileOfCheckSysCfgInModelInfo))
	{
		CString sInfo{ _T("") };
		sInfo.Format(_T("%s\"%s\"%s"),tr("配方"), m_sModelName, tr("系统配置文件不存在!"));
		AfxMessageBox(sInfo);
		exit(0);
	}
	m_iniCheckSystemConfig.Init(m_sPathFileOfCheckSysCfgInModelInfo);//**[11/16/2017 ConanSteve]**:  初始化CheckSystemConfig.ini文件
// 	if (FALSE == PathFileExists(m_sPathFileOfCheckSysCfgInModelInfo))
// 	{
// 		CopyFile(m_sPathFileOfCheckSysCfgInModelInfo, m_sPathFileOfCheckSysCfgInConfig, FALSE);//**[9/1/2017 ConanSteve]**:  强制覆盖
// 	}
	/*************************************<检查模拟相机配置文件是否存在>*************************************/
	return true;
}


bool CMainFrame::ReadCfgOfSys()
{
	CString sErrorInfo{ _T("") };
	CString sModuleName{ _T("") };
	m_sWindowTitle = m_iniCheckSystemConfig.GetStringEx(_T("系统配置"), _T("窗口标题"), m_sWindowTitle);
	
	m_iPENum = m_iniCheckSystemConfig.GetIntEx(_T("IOCard Parameters"), _T("光电个数"), 0);//**[11/16/2017 ConanSteve]**:  读取产品线名称
	m_iGrabberNum = m_iniCheckSystemConfig.GetIntEx(_T("采集参数"), _T("设备个数"), 0);
	m_iniAlgConfig.WriteInt(_T("Basic Parameters"), _T("CameraNum"), m_iGrabberNum, _T("相机个数"));
	m_eProductLine = (e_ProductLine)m_iniCheckSystemConfig.GetIntEx(_T("Critical Config"), _T("ProductLineType"), e_ProductLine_NO, _T("产品线类型：1为通用算法，2为胶塞算法，3为灯检机算法， 4为采血管算法"));
	m_iniAlgConfig.WriteInt(_T("Critical Config"), _T("ProductLineType"), m_eProductLine, _T("产品线类型：1为通用算法，2为胶塞算法，3为灯检机算法， 4为采血管算法"));
	m_iAssemblyLineNum = m_iniCheckSystemConfig.GetIntEx(_T("Critical Config"), _T("AssemblyLineNumber"), 1, _T("产品线（通道）数量"));
	/*************************************<拷贝SystemConfig配置到AlgConfig文件中，供多对象初始化使用>************************************[11/24/2017 ConanSteve]*/
	/*************************************<流水线相关>************************************[12/28/2017 ConanSteve]*/
	m_iCheckSpeedUpdateInterval = m_iniCheckSystemConfig.GetIntEx(_T("Critical Config"), _T("CheckSpeedUpdateInterval"), 1000, _T("检测速度刷新周期，单位：ms"));
	/*************************************< 剔废组数>************************************[11/27/2017 ConanSteve]*/
	{
		//**[11/27/2017 ConanSteve]**: 设置每个剔废队列的超时类型
		sModuleName = _T("Basic Parameters");
		m_iKickQueueNum = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("KickQueueNum"), 1);
		m_iniAlgConfig.WriteInt(sModuleName, _T("KickQueueNum"), m_iKickQueueNum);
		for (int iKickQueueIndex = 0; iKickQueueIndex < m_iKickQueueNum; ++iKickQueueIndex)
		{
			CString sModuleName{ _T("") };
			sModuleName.Format(_T("KickGroup%d"), iKickQueueIndex + 1);
			int iTimeType = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("TimeType"), 1, _T("剔废队列1定时基准 0为时间，单位为毫秒；1为帧/行，计时单位为帧个数/编码器行频"));
			m_iniAlgConfig.WriteInt(sModuleName, _T("TimeType"), iTimeType, _T("剔废队列1定时基准 0为时间，单位为毫秒；1为帧/行，计时单位为帧个数/编码器行频"));
			int iTimeout = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("TimeOut"), 5);
			m_iniAlgConfig.WriteInt(sModuleName, _T("TimeOut"), iTimeout);
			m_arr_eKickType[iKickQueueIndex] = (e_KickType)m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("KickType"), 0, _T("0是单路剔废，1是多路垂直剔废，2多路水平剔废"));
			m_iniAlgConfig.WriteInt(sModuleName, _T("KickType"), m_arr_eKickType[iKickQueueIndex], _T("0是单路剔废，1是多路垂直剔废，2多路水平剔废"));

			int iAssemblyId = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("AssemblySN"), -1, _T("当前剔废组所属的流水线号，从0开始计数"));

			if (0 <= iAssemblyId&&iAssemblyId < m_iAssemblyLineNum)
			{
				m_map_KickSN2AssemblySN[iKickQueueIndex] = iAssemblyId;
				m_arr_veciKickSNInAssemblyLine[iAssemblyId].push_back(iKickQueueIndex);
			}
			else
			{
				sErrorInfo.Format(_T("踢废组%d流水线号不能为%d, 请重新检查配置文件!"), iKickQueueIndex, iAssemblyId);
				m_fileErrorInfo.Write(sErrorInfo);
				AfxMessageBox(sErrorInfo);
				exit(0);
			}

		}
	}
	/*************************************<检测组相关>************************************[12/28/2017 ConanSteve]*/
	{
		sModuleName = _T("Inspection Parameters");
		m_iCheckGroupNum = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("CheckNum"), 1);
		m_iniAlgConfig.WriteInt(sModuleName, _T("CheckNum"), m_iCheckGroupNum, _T("检测组数"));

		for (int i = 0; i < m_iCheckGroupNum; ++i)
		{
			//**[12/28/2017 ConanSteve]**:  拷贝动态库配置
			sModuleName = { _T("Inspection Parameters") };
			CString sDllNameKey{ _T("") }, sDllNameValue{ _T("") }, sCheckObjNumKey{ _T("") };
			int iCheckObjNumValue{ 2 };
			sDllNameKey.Format(_T("CheckDllName_%d"), i + 1);
			sDllNameValue = m_iniCheckSystemConfig.GetStringEx(sModuleName, sDllNameKey, sDllNameValue);
			m_iniAlgConfig.WriteString(sModuleName, sDllNameKey, sDllNameValue);

			sCheckObjNumKey.Format(_T("CheckObjNum_%d"), i + 1);
			iCheckObjNumValue = m_iniCheckSystemConfig.GetIntEx(sModuleName, sCheckObjNumKey, 2);
			m_iniAlgConfig.WriteInt(sModuleName, sCheckObjNumKey, iCheckObjNumValue);

			sModuleName.Format(_T("CheckGroup%d"), i + 1);
			int iKickSN = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("KickSN"), -1, _T("检测组所在踢废队列号，从0开始"));
			//m_iniAlgConfig.WriteInt(sModuleName, _T("KickSN"), iKickSN, _T("检测组所在踢废队列号"));
			if (0 <= iKickSN && iKickSN < m_iKickQueueNum)
			{
				m_arr_veciCheckSNInKickQueue[iKickSN].push_back(i);
				int iAssemblyId = m_map_KickSN2AssemblySN[iKickSN];
				m_arr_veciCheckGroupIndexOfAsseblyLine[iAssemblyId].push_back(i);
				m_map_CheckSN2KickSN[i] = iKickSN;
			}
			else
			{
				sErrorInfo.Format(_T("检测组%d踢废队列号不能为%d, 请重新检查配置文件!"), i, iKickSN);
				m_fileErrorInfo.Write(sErrorInfo);
				AfxMessageBox(sErrorInfo);
				exit(0);
			}
		}
	}
	/*************************************<相机参数设置>************************************[12/28/2017 ConanSteve]*/
	for (int i = 0; i < m_iGrabberNum; ++i)
	{
		sModuleName.Format(_T("Camera%d Parameters"), i + 1);
		int iCheckSN = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("CheckGroup"), -1, _T("相机所在检测组,从0开始"));
		if (0 <= iCheckSN && iCheckSN < m_iCheckGroupNum)
		{
			m_iniAlgConfig.WriteInt(sModuleName, _T("CheckGroup"), iCheckSN + 1, _T("相机所在检测组，从1开始"));
			int iKickSN = m_map_CheckSN2KickSN[iCheckSN];
			m_iniAlgConfig.WriteInt(sModuleName, _T("KickGroup"), iKickSN + 1, _T("相机所在剔废队列号，从1开始"));
		}
		else
		{
			sErrorInfo.Format(_T("相机%d所在检测组ID%d不正确，请重新检查配置文件！"), i + 1, iCheckSN);
			m_fileErrorInfo.Write(sErrorInfo);
			AfxMessageBox(sErrorInfo);
			exit(0);
		}

		m_arr_iSmallImgCntPerGrab[i] = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("SmallImageNumberPerGrabber"), 1, _T("相机每个图像号采集的图像小张数"));
		m_iniAlgConfig.WriteInt(sModuleName, _T("PictureNumberPerCamera"), m_arr_iSmallImgCntPerGrab[i], _T("图像张数"));
		m_arr_sCameraImageInfo[i].m_iBayerType = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("HalconBayerType"), 0, _T("算法用，Halcon Bayer变换类型,0:不变换，1:GB,2:GR,3:BG,4:RG"));
		m_arr_sCameraImageInfo[i].m_iFlipType = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("HalconFlipType"), 0, _T("算法用，系统不用 翻转类型，0不翻转，1水平翻转，2垂直翻转，3全翻转，算法使用"));
		m_arr_iSystemBayerType[i] = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("SystemBayerType"), 0, _T("系统做Bayer变换的类型。0：不变换， 1：RG，2：GB，3：GR，4：BG"));
		m_arr_iSystemFilpType[i] = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("SystemFlipType"), 0, _T("系统Bayer翻转类型. 0:NO, 1:垂直 2:水平"));
		m_arr_sCameraImageInfo[i].m_iCameraImagePixelSize = m_iniCheckSystemConfig.GetIntEx(sModuleName, _T("Byte"), 1, _T("多对象的图像数据每像素字节数大小，系统给算法Bayer变换之后的图为3，原图为1"));
		m_iniAlgConfig.WriteInt(sModuleName, _T("Byte"), m_arr_sCameraImageInfo[i].m_iCameraImagePixelSize, _T("多对象的图像数据每像素字节数大小，系统给算法Bayer变换之后的图为3，原图为1"));

		m_arr_BayerTransform[i].m_bAreaCamBayer = m_arr_iSystemBayerType[i] != 0 ? TRUE : FALSE;
		m_arr_BayerTransform[i].m_dBayerRedGain = m_iniCheckSystemConfig.GetDoubleEx(sModuleName, _T("Bayer转换Red增益"), 0);
		m_arr_BayerTransform[i].m_dBayerGreenGain = m_iniCheckSystemConfig.GetDoubleEx(sModuleName, _T("Bayer转换Green增益"), 0);
		m_arr_BayerTransform[i].m_dBayerBlueGain = m_iniCheckSystemConfig.GetDoubleEx(sModuleName, _T("Bayer转换Blue增益"), 0);
		if (m_arr_BayerTransform[i].m_bAreaCamBayer)
		{
			SetLutTable(m_arr_BayerTransform[i].m_dBayerRedGain, m_arr_BayerTransform[i].m_dBayerGreenGain,
				m_arr_BayerTransform[i].m_dBayerBlueGain);
		}
		m_arr_bWhetherImageConvertedToMultiObj[i] = m_iniCheckSystemConfig.GetBoolEx(sModuleName, _T("IsBayerConverted"), false, _T("算法接收的是否是系统Bayer变换后的图像"));

	}
	
	/*************************************<读取缺陷描述>************************************[11/24/2017 ConanSteve]*/
	for (int iCheckGroupSN = 0; iCheckGroupSN < m_iCheckGroupNum; ++iCheckGroupSN)
	{
		int iDefectSN = 0;
		CString sDefectDesc{ _T("") };
		CString sModuleName{ _T("") }, sKeyName{ _T("") };
		sModuleName.Format(_T("工位%d缺陷描述"), iCheckGroupSN+1);
		sKeyName.Format(_T("缺陷数"));
		int iDefectNum = m_iniCheckSystemConfig.GetIntEx(sModuleName, sKeyName);
		for (int iDefectIndex = 1; iDefectIndex <= iDefectNum; ++iDefectIndex)
		{
			sKeyName.Format(_T("缺陷类型%d"), iDefectIndex);
			sDefectDesc = m_iniCheckSystemConfig.GetStringEx(sModuleName, sKeyName, sDefectDesc);
			if (_T("")!= sDefectDesc)
			{
				//**[12/1/2017 ConanSteve]**:  将当前缺陷添加进系统
				m_arr_mapDefectStatistics[iCheckGroupSN].insert(make_pair(iDefectIndex, s_DefectInfo(0, sDefectDesc)));
			}
		}
		
	}
	
	CINICfg iniSet(CExFn::GetCurrentAppPath() + _T("\\set.ini"));
	iniSet.WriteString(_T("系统配置"), _T("AppName"), _T("CheckSystem"));
	//CheckCamCfgFile();
	CheckIOCardCfgFile();
	return false;
}


void CMainFrame::CheckCamCfgFile()
{
// 	CString sKeyName{ _T("") };
// 	CString sModuleName = _T("Grabber Parameters");
// 	CString sFileNameOfGrab{ _T("") };  //**[9/1/2017 ConanSteve]**:  相机配置文件名称
// 	for (int i = 0; i < m_iGrabberNum; ++i)
// 	{
// 		sKeyName.Format(_T("FileNameOfGrabberInit_%d"), i + 1);
// 		CExFn::GetPrivateProfileStringEx(sModuleName, sKeyName, sFileNameOfGrab, m_sPathFileOfCheckSysCfgInConfig);
// 		m_fileDebugInfo.Write(m_sPathFileOfCheckSysCfgInConfig);
// 		m_arr_sPathFileOfGrabInitInConfig[i] = m_sPathOfCfg +_T("\\")+ sFileNameOfGrab;
// 		m_arr_sPathFileOfGrabInitInModelInfo[i] = m_sPathOfAlgModelInfo + _T("\\")+ sFileNameOfGrab;
// 		//CopyFile(m_pPathFileOfGrabInitInModelInfo[i], m_arr_sPathFileOfGrabInitInConfig[i], FALSE);//**[9/1/2017 ConanSteve]**:  强制覆盖
// 	}
}


int CMainFrame::CheckIOCardCfgFile()
{	
	return 0;
}


CRetInfo CMainFrame::InitIOCard()
{
	CRetInfo ret;
	fill(m_arr_iIOCardType.begin(), m_arr_iIOCardType.end(), e_IOCardType_SG);
	if (!m_bEnableInitIOCard)
	{
		m_fileDebugInfo.Write(_T("警告：接口卡初始化跳过！"));
		ret.bRet = true;
		return ret;
	}
	m_iIOCardNum = m_iniCheckSystemConfig.GetIntEx(_T("IOCard Parameters"), _T("IOCardNumber"), 0);
	CString sKeyName{ _T("") };

	for (int i = 0; i < m_iIOCardNum; ++i)
	{
		sKeyName.Format(_T("IOCardType_%d"), i);
		m_arr_iIOCardType[i] = (e_IOCardType)m_iniCheckSystemConfig.GetIntEx(_T("IOCard Parameters"), sKeyName, 0,_T("0：无卡，1模拟接口卡，2：B卡，3：C卡，4：PCI6356卡，5：PCIE_6356卡， 6：RS232_XZXNYN串口继电器"));
		switch (m_arr_iIOCardType[i])
		{
		case e_IOCardType::e_IOCardType_NONE:
		{
			ret.sErrorInfo.Format(tr("接口卡%d类型不能为无"), i);
			m_fileErrorInfo.Write(ret.sErrorInfo);
			ret.bRet = false;
			return ret;
		}
			continue;
			break;
		case e_IOCardType::e_IOCardType_SG:
			m_arr_pIOCard[i] = make_shared<CDHIOCardForSG>();
			break;
		case e_IOCardType::e_IOCardType_PIOB:
			break;
		case e_IOCardType::e_IOCardType_PIOC:
			m_arr_pIOCard[i] = make_shared<CDHIOCardForPIO24C>();
			break;
		case e_IOCardType::e_IOCardType_PCI_6356:
			m_arr_pIOCard[i] = make_shared<CDHIOCardForPCI6356>();
			break;
		case e_IOCardType::e_IOCardType_PCIE_6356:
			break;
		case e_IOCardType::e_IOCardType_RS232_XZXNYN:
			m_arr_pIOCard[i] = shared_ptr<CDHIOCardForRS232_XZXNYN>(new CDHIOCardForRS232_XZXNYN());
			break;
		default:
			break;
		}
		if (nullptr == m_arr_pIOCard[i])
		{
			ret.sErrorInfo.Format(tr("接口卡%d创建失败"), i);
			m_fileErrorInfo.Write(ret.sErrorInfo);
			ret.bRet = false;
			return ret;
		}
		/*************************************<获得接口卡配置文件路径，因为不同接口卡配置文件路径不一样>************************************[9/13/2017 ConanSteve]*/
		sKeyName.Format(_T("FileNameOfIOCardInit_%d"), i);
		m_arr_sPathFileOfIOCardInitFile[i] = m_iniCheckSystemConfig.GetStringEx(_T("IOCard Parameters"), sKeyName, m_arr_sPathFileOfIOCardInitFile[i], _T("NULL"));
		m_arr_sPathFileOfIOCardInitFile[i] = CExFn::GetCurrentAppPath() +_T("\\") + m_arr_sPathFileOfIOCardInitFile[i];
		sKeyName.Format(_T("PathFileBtwGrabAndIOCard_%d"), i);
		CString sPathFileBtwGrabAndIOCard{ _T("") };
		sPathFileBtwGrabAndIOCard = m_iniCheckSystemConfig.GetStringEx(_T("IOCard Parameters"), sKeyName, sPathFileBtwGrabAndIOCard, _T("NULL"));
		sPathFileBtwGrabAndIOCard = CExFn::GetCurrentAppPath() + _T("\\") + sPathFileBtwGrabAndIOCard;
		CIOCardInitInfo initInfo;
		memcpy_s(initInfo.lpszPathFileOfInit, MAX_PATH, m_arr_sPathFileOfIOCardInitFile[i], m_arr_sPathFileOfIOCardInitFile[i].GetLength() * sizeof(TCHAR));
		memcpy_s(initInfo.lpszPathFileOfConnection, MAX_PATH, sPathFileBtwGrabAndIOCard, sPathFileBtwGrabAndIOCard.GetLength() * sizeof(TCHAR));
		m_fileDebugInfo.Write(_T("接口卡配置文件:") + (CString)initInfo.lpszPathFileOfInit);
		m_fileDebugInfo.Write(_T("接口卡连接文件:") + (CString)initInfo.lpszPathFileOfConnection);
		if (!m_arr_pIOCard[i]->InitCard(initInfo))
		{
			ret.bRet = false;
			ret.sErrorInfo = m_arr_pIOCard[i]->GetLastError();
			return ret;
		}
		if (!m_arr_pIOCard[i]->OpenCard())
		{
			ret.bRet = false;
			ret.sErrorInfo = m_arr_pIOCard[i]->GetLastError();
			return ret;
		}		
	}
	ret.bRet = true;
	return ret;
}

DWORD CMainFrame::ThreadInitBackstage(PVOID pParam)
{
	return 0;
	CMainFrame* pFrame = (CMainFrame*)pParam;
	return pFrame->ThreadInitBackstageFunc(3);
}

DWORD CMainFrame::ThreadInitBackstageFunc(int iFunc)
{
	shared_ptr<CString> pCurThredId = make_shared<CString>();
	pCurThredId->Format(_T("0x%x"), GetCurrentThreadId());
	TRACE(pCurThredId->GetBuffer());
	bool bRet{ false };
	CRetInfo ret;
	CString sHintInfo{ _T("") };


	AddMsg(tr("正在初始化接口卡......"));
	ret = InitIOCard();
	if (!ret.bRet)
	{
		sHintInfo.Format(tr("初始化接口卡失败!\n错误信息：%s"), ret.sErrorInfo);
		AddMsg(sHintInfo, true);
		m_bInitIOCardSuccess = false;
		 //::SendMessage(this->m_hWnd, WM_CLOSE, 0, 0);
		// return -1;
	}
	else
	{
		m_bInitIOCardSuccess = true;
	}

	AddMsg(tr("正在初始化相机......"));
	ret = InitGrabber();
	if (!ret.bRet)
	{
		sHintInfo.Format(tr("初始化相机失败!\n错误信息：%s"), ret.sErrorInfo);
		AddMsg(sHintInfo, true);
		//::SendMessage(this->m_hWnd, WM_CLOSE, 0, 0);
		return -1;
	}
	ResizeLayout();//**[12/5/2017 ConanSteve]**:  调整窗口布局，必须现在初始化相机之后，必须在初始化多对象之前
	//**[10/30/2017 ConanSteve]**:  初始化多对象
	AddMsg(tr("正在初始化多对象......"));
	ret = InitMultiObject();
	if (!ret.bRet)
	{
		sHintInfo.Format(tr("初始化多对象失败!\n错误信息：%s"), ret.sErrorInfo);
		AddMsg(sHintInfo, true);
		//::SendMessage(this->m_hWnd, WM_CLOSE, 0, 0);
		return -1;
	}

	AddMsg(tr("正在初始化日志系统......"));
	bRet = InitLogSys();
	if (!bRet)
	{
		//::SendMessage(this->m_hWnd, WM_CLOSE, 0, 0);
		return -1;
	}
/************************************************************************/
	bRet = AllocateMemory();
	if (!bRet)
	{
		CString sInfo = tr("初始化内存失败");
		AddMsg(sInfo, true);
		PostQuitMessage(0);
	}
	g_pView->m_xcTitleBar.EnbaleBtnOnOff(TRUE);

	/*************************************<读取使DlgParamSet从配置文件中读取参数值>************************************[12/25/2017 ConanSteve]*/
	m_pdlgParamSet = make_shared<CDlgParamSet>();
	m_pdlgParamSet->Create(IDD_DIALOG_ParamSet, this);
	m_pdlgParamSet->OnBnClickedButtonApply();
	/*************************************<>************************************[12/25/2017 ConanSteve]*/

	AddMsg(tr("系统初始化成功!"));
	ResetSystem(1);//**[12/28/2017 ConanSteve]**:  清零所有计数
	StartAllThread();
	SetTimer(ID_EVENT_UPDATE_INFO, 500, NULL);
	SetTimer(ID_EVENT_UPDATE_CHECK_SPEED, m_iCheckSpeedUpdateInterval, NULL);
	//SetTimer(ID_EVENT_START_GRABBER, 300, NULL);
	return 0;
}


CRetInfo CMainFrame::InitGrabber()
{
	CRetInfo ret;
	if (!m_bEnableInitGrabber)
	{
		m_fileDebugInfo.Write(_T("警告：相机初始化跳过！"));
	}
	int iCamSN{ 0 };//**[8/29/2017 ConanSteve]**:  设备序列号
	CString sKeyName{ _T("") };
	CString sKeyValue{ _T("") };
	CString sDebugInfo{ _T("") };
	CString sCamName{ _T("") };//**[8/29/2017 ConanSteve]**:  相机名称
	CString sFileNameOfCamInit{ _T("") };//**[8/29/2017 ConanSteve]**:  相机初始化文件名称
	CString sModuleName{ _T("") };
	/*************************************<获得配置文件对应的相机类型>************************************[8/29/2017 ConanSteve]*/
	m_iUnitedGrabType = (e_GrabType)m_iniCheckSystemConfig.GetIntEx(_T("采集参数"), _T("UnitedGrabberType"), (int)e_GrabType::e_GrabType_NONE, _T("0：模拟相机，2:AVT相机，7:JAI相机， 8：MER相机"));
	if (0 != m_iGrabberNum)
	{
		m_p_iCurNumOfSaveImgOfEachGrab = make_unique<int[]>(m_iGrabberNum);
		fill(m_arr_iGrabType.begin(), m_arr_iGrabType.end(), m_iUnitedGrabType);
	}
	if (0 != m_iCheckGroupNum)
	{
		fill(m_arr_iCheckCntOfCheckGroup.begin(), m_arr_iCheckCntOfCheckGroup.end(), 0);	
		fill(m_arr_iGoodCntOfCheckGroup.begin(), m_arr_iGoodCntOfCheckGroup.end(), 0);//**[12/5/2017 ConanSteve]**:  每个检测组好品计数
		fill(m_arr_iBadCntOfCheckGroup.begin(), m_arr_iBadCntOfCheckGroup.end(), 0);//**[12/1/2017 ConanSteve]**:  每工位缺陷总数
		fill(m_arr_iAlgExceptionCntOfCheckGroup.begin(), m_arr_iAlgExceptionCntOfCheckGroup.end(), 0);//**[12/5/2017 ConanSteve]**:  每工位算法异常计数
		fill(m_arr_bEnableExecCheckOfEachGroup.begin(), m_arr_bEnableExecCheckOfEachGroup.end(), true);


	}
	if (0 != m_iKickQueueNum)
	{
		m_p_fileKickCntInfoChange = make_unique<CLogFile[]>(m_iKickQueueNum);

	}
	/*************************************<逐个初始化相机>************************************[8/29/2017 ConanSteve]*/
	array<CString, MAX_CAMER_COUNT> arr_sGrabDevName;//**[12/28/2017 ConanSteve]**:  相机设备名称
	array<CString, MAX_CAMER_COUNT> arr_sGrabDevMark;//**[12/28/2017 ConanSteve]**:  设备标识
	array<int, MAX_CAMER_COUNT> arr_GrabSN;//**[12/28/2017 ConanSteve]**:  相机序号
	for (int i = 0; i < m_iGrabberNum; ++i)
	{
//  		if (0 == i)
//  			continue;
		/*************************************<读取配置文件>************************************[8/29/2017 ConanSteve]*/
		sModuleName = _T("采集参数");
		/*************************************<设备类型>************************************[8/29/2017 ConanSteve]*/
		{
			sKeyName.Format(_T("设备%C类型"), _T('A') + i);
			if (e_GrabType::e_GrabType_NONE == m_iUnitedGrabType)
			{
				m_arr_iGrabType[i] = (e_GrabType)m_iniCheckSystemConfig.GetIntEx(sModuleName, sKeyName, (int)m_iUnitedGrabType);
			}
			else
			{
				m_arr_iGrabType[i] = m_iUnitedGrabType;
			}
			switch (m_arr_iGrabType[i])
			{
			case e_GrabType::e_GrabType_SG:
				m_arr_pGrabber[i] = make_shared<CDHGrabForSG>();
				break;
			case e_GrabType::e_GrabType_AVT:
				//m_arr_pGrabber[i] = make_shared<CDHGrabForAVT>();
				break;
			case e_GrabType::e_GrabType_BAUMER:
				//m_arr_pGrabber[i] = shared_ptr<CDHGrabForBaumer>(new CDHGrabForBaumer);
				break;
			case e_GrabType::e_GrabType_JAI:
				//m_arr_pGrabber[i] = make_shared<CDHGrabForJAI>();
				break;
			case e_GrabType::e_GrabType_MER:
				m_arr_pGrabber[i] = make_shared<CDHGrabForMER>();
				break;
			default:
				break;
			}
		}
		if (nullptr == m_arr_pGrabber[i])
		{
			ret.sErrorInfo = tr("本系统不支持当前型号相机，请联系开发人员！");
			ret.bRet = false;
			return ret;
		}
		/*************************************<设备序号>************************************[12/28/2017 ConanSteve]*/
		{
			sKeyName.Format(_T("设备%C序号"), _T('A') + i);
			arr_GrabSN[i] = m_iniCheckSystemConfig.GetIntEx(sModuleName, sKeyName, -1);
			if (iCamSN < 0 || arr_GrabSN[i] >= m_iGrabberNum)
			{
				m_fileDebugInfo.Write(CExFn::GetLastErrorMsg());
				sDebugInfo.Format(tr("初始化相机：相机%d的序号%d不正确"), i + 1, arr_GrabSN[i]);
				m_fileDebugInfo.Write(sDebugInfo);
				ret.bRet = false;
				ret.sErrorInfo = sDebugInfo;
				return ret;
			}
		}
		/*************************************<设备名称>************************************[12/28/2017 ConanSteve]*/
		{
			sKeyName.Format(_T("设备%C名称"), _T('A') + i);
			arr_sGrabDevName[i] = m_iniCheckSystemConfig.GetStringEx(sModuleName, sKeyName, arr_sGrabDevName[i]);
		}
		/*************************************<设备标识>************************************[12/28/2017 ConanSteve]*/
		{
			sKeyName.Format(_T("设备%C标识"), _T('A') + i);
			arr_sGrabDevMark[i] = m_iniCheckSystemConfig.GetStringEx(sModuleName, sKeyName, arr_sGrabDevMark[i]);
		}
		/*************************************<设备初始化文件名>************************************[12/28/2017 ConanSteve]*/
		{
			sKeyName.Format(_T("设备%C初始化文件名"), _T('A') + i);
			sFileNameOfCamInit = m_iniCheckSystemConfig.GetStringEx(sModuleName, sKeyName, sFileNameOfCamInit);
			m_arr_sPathFileOfGrabInitInModelInfo[i] = m_sPathOfAlgModelInfo + _T("\\") + sFileNameOfCamInit;
			if (!PathFileExists(m_arr_sPathFileOfGrabInitInModelInfo[i]))
			{
				sDebugInfo.Format(tr("错误描述:相机%d配置文件%s文件不在指定路径中"), i + 1, sFileNameOfCamInit);
				AfxMessageBox(sDebugInfo);
				m_fileErrorInfo.Write(sDebugInfo);
				ret.bRet = false;
				ret.sErrorInfo = sDebugInfo;
				return ret;
			}
		}
		
		bool bRet = false;
		{
// 			CGrabInitInfo GrabInfo;
// 			GrabInfo.iGrabSN = iCamSN;
// 			GrabInfo.Context = this;
// 			GrabInfo.CallBackFunc = AreaGrabOverCallback;
// 			memcpy_s(GrabInfo.lpszPathFileOfGrab, MAX_PATH * sizeof(TCHAR), m_arr_sPathFileOfGrabInitInModelInfo[i], m_arr_sPathFileOfGrabInitInModelInfo[i].GetLength() * sizeof(TCHAR));
// 			if (!m_bEnableInitGrabber)
// 				continue;
// 			bool bRet = TRUE == m_arr_pGrabber[i]->Init(GrabInfo);
		}

		{
			s_GBINITSTRUCT sAreaGrabber;
			sAreaGrabber.iGrabberTypeSN = m_arr_iGrabType[i];
			sAreaGrabber.iGrabberSN = arr_GrabSN[i];
			sAreaGrabber.Context = this;
			sAreaGrabber.CallBackFunc = AreaGrabOverCallback;
			memcpy_s(sAreaGrabber.strDeviceName, MAX_PATH * sizeof(TCHAR), arr_sGrabDevName[i].GetBuffer(), arr_sGrabDevName[i].GetLength() * sizeof(TCHAR));
			memcpy_s(sAreaGrabber.strDeviceMark, MAX_PATH * sizeof(TCHAR), arr_sGrabDevMark[i].GetBuffer(), arr_sGrabDevMark[i].GetLength() * sizeof(TCHAR));
			memcpy_s(sAreaGrabber.strGrabberFile, MAX_PATH * sizeof(TCHAR), m_arr_sPathFileOfGrabInitInModelInfo[i].GetBuffer(), m_arr_sPathFileOfGrabInitInModelInfo[i].GetLength() * sizeof(TCHAR));
			if (!m_bEnableInitGrabber)
				continue;
			bRet = m_arr_pGrabber[i]->Init(&sAreaGrabber);
		}
		if (!bRet)
		{
			s_GBERRORINFO GrabberInfo;
			m_arr_pGrabber[i]->GetLastErrorInfo(&GrabberInfo);
			sDebugInfo.Format(tr("相机%d初始化失败!"), i + 1);
			sDebugInfo += GrabberInfo.strErrorDescription;
			AddMsg(sDebugInfo, true);
			ret.bRet = false;
			ret.sErrorInfo = sDebugInfo;
			return ret;
		}
		
		sDebugInfo.Format(_T("初始化相机%d成功！"), i + 1);
		AddMsg(sDebugInfo);
		m_arr_pGrabber[i]->GetParamInt(GBImageWidth, m_arr_GrabInfo[i].m_iImageWidth);			// 面阵相机图像宽度
		m_arr_pGrabber[i]->GetParamInt(GBImageHeight, m_arr_GrabInfo[i].m_iImageHeight);			// 面阵相机图像高度
		m_arr_pGrabber[i]->GetParamInt(GBImageBufferSize, m_arr_GrabInfo[i].m_iImageSize);		// 面阵相机图像大小
		m_arr_pGrabber[i]->GetParamInt(GBImagePixelSize, m_arr_GrabInfo[i].m_iImagePixelSize);	// 面阵相机图像象素大小

		//**[1/3/2018 ConanSteve]**:	非常重要，相机库必须实现，涉及到系统图像显示的BitmapInfo是否正确 
		m_arr_pGrabber[i]->GetParamInt(GBIsColorFilter, m_arr_GrabInfo[i].m_bIsColorFilter);//**[9/11/2017 ConanSteve]**:  是否是彩图
		m_arr_pGrabber[i]->GetParamInt(GBEnableFlipImage, m_arr_GrabInfo[i].m_iFlipType);//**[9/11/2017 ConanSteve]**:  是否需要翻转


		m_arr_pGrabber[i]->GetParamInt(GBExposureTime, m_arr_GrabInfo[i].m_iExposureTime);//**[9/11/2017 ConanSteve]**:  曝光时间
		m_arr_pGrabber[i]->GetParamInt(GBGain, m_arr_GrabInfo[i].m_iGain);//**[9/11/2017 ConanSteve]**:  增益
		m_arr_pGrabber[i]->GetParamInt(GBBayerType, m_arr_GrabInfo[i].m_iBayerType);//**[9/11/2017 ConanSteve]**:  Bayer转换类型


		sModuleName.Format(_T("Camera%d Parameters"), i + 1);

		int iWidth = m_arr_sCameraImageInfo[i].m_iCameraImageWidth = m_arr_GrabInfo[i].m_iImageWidth;
		int iHeight = m_arr_sCameraImageInfo[i].m_iCameraImageHeight = m_arr_GrabInfo[i].m_iImageHeight;
		m_arr_sCameraImageInfo[i].m_iCameraImageSize = m_arr_sCameraImageInfo[i].m_iCameraImagePixelSize*iWidth*iHeight;
		/*************************************<将相机大小写入AlgConfig.ini文件>************************************[8/30/2017 ConanSteve]*/



		m_arr_iCheckGroupSNOfCurGrab[i] = m_iniAlgConfig.GetIntEx(sModuleName, _T("CheckGroup"), 1, _T("当前相机所属的工位号"))-1;
		m_arr_iKickGroupSNOfCurGrab[i] = m_iniAlgConfig.GetIntEx(sModuleName, _T("KickGroup"), 1, _T("当前相机所属的剔废队列")) - 1;
		CExFn::WritePrivateProfileInt(sModuleName, _T("Width"), iWidth*m_arr_iSmallImgCntPerGrab[i], m_sPathFileOfAlgCfg, _T("图像宽度"));
		CExFn::WritePrivateProfileInt(sModuleName, _T("PictureNumberPerCamera"), m_arr_iSmallImgCntPerGrab[i], m_sPathFileOfAlgCfg, _T("图像张数"));
		CExFn::WritePrivateProfileInt(sModuleName, _T("Height"), iHeight, m_sPathFileOfAlgCfg,_T("相机图像高度"));
		CExFn::WritePrivateProfileInt(sModuleName, _T("CheckOutNum"), 1, m_sPathFileOfAlgCfg, _T("每次算法检测返回结果个数"));
		CExFn::WritePrivateProfileInt(sModuleName, _T("KickOffset"), 0, m_sPathFileOfAlgCfg, _T(""));
		int iCheckInSize{ 0 }, iCheckOutSize{ 0 }, iCheckStatusSize{0};
		switch (m_eProductLine)
		{
		case e_ProductLine_NO:
			break;
		case e_ProductLine_General:
		{
			iCheckInSize = sizeof(General::CAlgInspInputParam);
			iCheckOutSize = sizeof(General::CAlgInspResult);
			iCheckStatusSize = sizeof(s_Status);
		}
			break;
		case e_ProductLine_Stopper:
		{
			iCheckInSize = sizeof(Stopper::s_AlgDetectInput);
			iCheckOutSize = sizeof(Stopper::s_AlgDetectOutput);
			iCheckStatusSize = sizeof(s_Status);
		}
			break;
		case e_ProductLine_LampInsp:
		{
			iCheckInSize = sizeof(LampInsp::s_DetectInParam);
			iCheckOutSize = sizeof(LampInsp::s_DetectOutParam);
			iCheckStatusSize = sizeof(s_Status);
		}
			break;
		case e_ProductLine_LableCheck:
		{
			iCheckInSize = sizeof(LabelCheck::s_AlgInspInPara);
			iCheckOutSize = sizeof(LabelCheck::s_AlgInspResult);
			iCheckStatusSize = sizeof(LabelCheck::s_Status);
		}
			break;
		default:
			break;
		}
		//检测输入特殊结构体大小
		CExFn::WritePrivateProfileInt(sModuleName, _T("CheckInSize"), iCheckInSize, m_sPathFileOfAlgCfg,_T("算法检测时结构体s_CheckInParam中 void* pParam的大小"));
		//检测输出特殊结构体大小
		CExFn::WritePrivateProfileInt(sModuleName, _T("CheckOutSize"), iCheckOutSize, m_sPathFileOfAlgCfg,_T("算法检测时结构体s_CheckInParam中 void* pParam的大小"));
		m_iniAlgConfig.WriteInt(sModuleName, _T("CheckStatusSize"), iCheckStatusSize, _T("算法检测状态结构体中的void* pParam大小（字节为单位）"));
}
	ret.bRet = true;
	return ret;
}


CRetInfo CMainFrame::InitDataBase()
{
	CRetInfo ret;
	return ret;
}


bool CMainFrame::AllocateMemory()
{
	if(0!=m_iGrabberNum)
	{	
		if (m_bEnableInitGrabber)//m_bEnableDrawImg&&
		{
			/*************************************<Bayer变换相关参数>************************************[9/6/2017 ConanSteve]*/
			/*************************************<初始化系统画图内存>************************************[9/5/2017 ConanSteve]*/
			//m_p_bEnableFlipImg = make_unique<bool[]>(m_iGrabberNum);
			for (int i = 0; i < m_iGrabberNum; ++i)
			{
				m_arr_pbtBufferOfBayerImg[i] = make_unique < BYTE[]>(m_arr_GrabInfo[i].m_iImageHeight* m_arr_GrabInfo[i].m_iImageWidth*3);
				m_arr_pbtBufferOfGrabImageCopy[i] = make_unique < BYTE[]>(m_arr_GrabInfo[i].m_iImageHeight* m_arr_GrabInfo[i].m_iImageWidth * 3);
			}
			/*************************************<初始化Bitmap信息>************************************[9/5/2017 ConanSteve]*/
			for (int i = 0; i < m_iGrabberNum; i++)
			{
				m_arr_pBitmapInfo[i] = shared_ptr<BITMAPINFO>((BITMAPINFO*)(new BYTE[sizeof(BITMAPINFO) + 256 * sizeof(RGBQUAD)]));
				m_arr_pBitmapInfo[i]->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
				m_arr_pBitmapInfo[i]->bmiHeader.biWidth = m_arr_sCameraImageInfo[i].m_iCameraImageWidth;
				m_arr_pBitmapInfo[i]->bmiHeader.biHeight = m_arr_sCameraImageInfo[i].m_iCameraImageHeight;
				m_arr_pBitmapInfo[i]->bmiHeader.biPlanes = 1;
				m_arr_pBitmapInfo[i]->bmiHeader.biBitCount = 8*(m_arr_GrabInfo[i].m_bIsColorFilter==1?3:1);
				m_arr_pBitmapInfo[i]->bmiHeader.biCompression = BI_RGB;
				m_arr_pBitmapInfo[i]->bmiHeader.biSizeImage = 0;
				m_arr_pBitmapInfo[i]->bmiHeader.biXPelsPerMeter = 0;
				m_arr_pBitmapInfo[i]->bmiHeader.biYPelsPerMeter = 0;
				m_arr_pBitmapInfo[i]->bmiHeader.biClrUsed = 0;
				m_arr_pBitmapInfo[i]->bmiHeader.biClrImportant = 0;

				//(2003-4-2)
				//对于8位位图来说，其需要调色板。对于256色地位图，调色板数组的个数位256。
				for (int k = 0; k < 256; k++)
				{
					m_arr_pBitmapInfo[i]->bmiColors[k].rgbBlue = (BYTE)k;
					m_arr_pBitmapInfo[i]->bmiColors[k].rgbGreen = (BYTE)k;
					m_arr_pBitmapInfo[i]->bmiColors[k].rgbRed = (BYTE)k;
					m_arr_pBitmapInfo[i]->bmiColors[k].rgbReserved = 0;
				}
			}
		}
		
	}
	return true;
}


void CMainFrame::AreaGrabOverCallback(const s_GBSIGNALINFO* SignalInfo)
{
	if (SignalInfo && SignalInfo->Context)
	{
		CMainFrame* pMainFrame = (CMainFrame*)SignalInfo->Context;
		pMainFrame->AreaGrabOverCallbackFunc(SignalInfo);
	}
	
}

void CMainFrame::AreaGrabOverCallbackFunc(const s_GBSIGNALINFO* SigInfo)
{
	CString sLogInfo{ _T("") };
	if (SigInfo->iErrorCode != 0)//**[12/8/2017 ConanSteve]**:  相机采集有错
	{
		CString sErrorInfo{ _T("") };
		sErrorInfo.Format(_T("相机采集回调错误\n错误代码：%d\n错误描述：%s"), SigInfo->iErrorCode, SigInfo->strDescription);
		AddMsg(sErrorInfo, true, true);
		return;
	}
	int iGrabSN = SigInfo->iGrabberSN;
	++m_arr_iGrabCallbackCnt[iGrabSN];
	if (m_bEnableInitIOCard)
	{
		for (int i = 0; i < m_iIOCardNum; ++i)
		{
			m_arr_pIOCard[i]->NotifyCardOfGrabCallBack(s_NotifyInfoOfGrabCallback(SigInfo->iGrabberSN, m_arr_iGrabCallbackCnt[iGrabSN]));
		}
	}
	GetCount_TuXiangHao();//**[12/14/2017 ConanSteve]**:  读取图像号寄存器，保证最新
	sLogInfo.Format(_T("[相机号:%2d][采集计数:%8d][图像帧：%8d][寄存器大张号:%8d]"), SigInfo->iGrabberSN, m_arr_iGrabCallbackCnt[iGrabSN], SigInfo->iFrameCount, m_arr_iHardImageCountOfGrabInRegister[iGrabSN]);
	m_p_fileGrabCallback[SigInfo->iGrabberSN].Write(sLogInfo);
	if (!m_bIsDetecting)
	{
		return;
	}
	//**[11/21/2017 ConanSteve]**:  通知接口卡相机回调
	if (iGrabSN < 0 || iGrabSN >= m_iGrabberNum)
	{
		AddMsg(tr("相机序列号超过配置范围"));
		return;
	}
	CameraGrabFuncSingle(iGrabSN);
}

bool CMainFrame::InitLogSys()
{
	CString sPathOfLogFiles = m_sPathOfVirtualDisk + _T("\\LogFiles");
	CString sPathOfSysFolder = sPathOfLogFiles + _T("\\CheckSystemLogs");
	CString sPathFileOfCurFile{ _T("") };//**[8/31/2017 ConanSteve]**:  当前日志文件的全路径
	CExFn::CreateMultiDirectory(sPathOfSysFolder);
	if (0 != m_iGrabberNum)
	{
		m_p_fileGrabCallback = unique_ptr<CLogFile[]>(new CLogFile[m_iGrabberNum]);
		m_p_fileImgCntOfGrabInRegister = make_unique<CLogFile[]>(m_iGrabberNum);
	}
	for (int i=0;i<m_iGrabberNum;++i)
	{
		sPathFileOfCurFile.Format(_T("\\相机回调%02d.txt"), i+1);
		sPathFileOfCurFile = m_sPathOfSysLogs + sPathFileOfCurFile;
		m_p_fileGrabCallback[i].Open(sPathFileOfCurFile, &m_bEnableLog);

		sPathFileOfCurFile.Format(_T("\\相机图像寄存器%02d.txt"), i + 1);
		sPathFileOfCurFile = m_sPathOfSysLogs + sPathFileOfCurFile;
		m_p_fileImgCntOfGrabInRegister[i].Open(sPathFileOfCurFile, &m_bEnableLog);
	}

	for (int i = 0; i < m_iKickQueueNum; ++i)
	{
		sPathFileOfCurFile.Format(_T("%s\\剔废检测结果信息%02d.txt"), m_sPathOfSysLogs, i+1);
		m_arr_fileKickCallback[i].Open(sPathFileOfCurFile);
	}
	return true;
}


bool CMainFrame::StartCheck()
{
	bool bRet{ false };
	m_bIsDetecting = true;
	if (m_bEnableInitGrabber)//**[12/12/2017 ConanSteve]**:  如果初始化了相机，则打开所有相机
	{
		for (int i = 0; i < m_iGrabberNum; ++i)
		{
			bRet = TRUE == m_arr_pGrabber[i]->StartGrab();
			if (!bRet)
				AfxMessageBox(tr("打开相机失败！"));
		}
	}
	if (m_bEnableInitIOCard)//**[12/12/2017 ConanSteve]**:  如果初始化了接口卡，则启动接口卡所有队列
	{
		for (int i = 0; i < m_iIOCardNum; ++i)
		{
			m_arr_pIOCard[i]->Enable(true);
		}
	}			
	return true;
}


bool CMainFrame::StopCheck()
{
	bool bRet{ false };
	if (m_bEnableInitIOCard)//**[12/12/2017 ConanSteve]**:  如果初始化了接口卡，则启动接口卡所有队列
	{
		for (int i = 0; i < m_iIOCardNum; ++i)
		{
			m_arr_pIOCard[i]->Enable(false);
		}
	}
	if (m_bEnableInitGrabber)
	{
		for (int i = 0; i < m_iGrabberNum; ++i)
		{
// 			if (0 == i)
// 				continue;
			bRet = TRUE == m_arr_pGrabber[i]->StopGrab();
		}
	}
//	Sleep(200);
	m_bIsDetecting = false;
	return true;
}


bool CMainFrame::AddMsg(CString sMsg, bool bHint, bool bErrorInfo)
{
	m_fileDebugInfo.Write(sMsg);
	if (bErrorInfo)
		m_fileErrorInfo.Write(sMsg);
	s_HintMsg *pHintMsg = new s_HintMsg();
	pHintMsg->sMsg = sMsg;
	pHintMsg->bHint = bHint;
	SendMessage(WM_COMMAND, ID_INDICATOR_OPERATION, (LPARAM)pHintMsg);
	return false;
}


BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (wParam)
	{
	case ID_INDICATOR_OPERATION://**[9/5/2017 ConanSteve]**:  系统实时运行信息
	{
		if (NULL != lParam)
		{
			s_HintMsg* pHintMsg = (s_HintMsg*)lParam;
			m_wndStatusBar.SetPaneText(0, pHintMsg->sMsg);
			if (pHintMsg->bHint)
				AfxMessageBox(pHintMsg->sMsg);
			delete pHintMsg;
		}
	}
	return TRUE;
	default:
		break;
	}
	return CFrameWnd::OnCommand(wParam, lParam);
}


bool CMainFrame::CameraGrabFuncSingle(int iGrabSN)
{
	int iAddrOfCamImg{0};//**[9/6/2017 ConanSteve]**:  
	int iImgSN = m_arr_iHardImageCountOfGrabInRegister[iGrabSN];
	BYTE* pImageBuffer{ NULL };
	bool bRet{ false };
	int iImageWidth = 0, iImageHeight = 0, iImageByte = 0, iImageSize = 0;
	iImageWidth = m_arr_sCameraImageInfo[iGrabSN].m_iCameraImageWidth;
	iImageHeight = m_arr_sCameraImageInfo[iGrabSN].m_iCameraImageHeight;
	iImageByte = m_arr_sCameraImageInfo[iGrabSN].m_iCameraImagePixelSize;
	iImageSize = m_arr_sCameraImageInfo[iGrabSN].m_iCameraImageSize;
	int iCheckGroupSN = m_arr_iCheckGroupSNOfCurGrab[iGrabSN];
	bRet = TRUE == m_arr_pGrabber[iGrabSN]->GetParamInt(GBImageBufferAddr, iAddrOfCamImg);
	ASSERT(bRet);
	pImageBuffer = (BYTE*)iAddrOfCamImg;
	ASSERT(NULL!= pImageBuffer);
	//memcpy_s(&m_pBufferOfCamImg[iGrabSN][0], m_pCameraImageInfo[iGrabSN].m_iCameraImageSize, pImageBuffer, m_pCameraImageInfo[iGrabSN].m_iCameraImageSize);//**[9/6/2017 ConanSteve]**:  拷贝图像
	
	/*************************************<系统画图>************************************[12/14/2017 ConanSteve]*/
//	return false;
	if ((m_bEnableDrawImg || e_SaveImgType_NO != m_iSaveImgType&& e_SaveImgObj_System == m_iSaveImgObj))
	{
		//**[1/26/2018 ConanSteve]**:	无论任何相机都进行图像拷贝 
		memcpy_s(&m_arr_pbtBufferOfGrabImageCopy[iGrabSN][0], m_arr_GrabInfo[iGrabSN].m_iImageSize, pImageBuffer, m_arr_GrabInfo[iGrabSN].m_iImageSize);
		switch (m_arr_iGrabType[iGrabSN])
		{
		case e_GrabType::e_GrabType_SG:
		{
			memcpy_s(&m_arr_pbtBufferOfBayerImg[iGrabSN][0], m_arr_GrabInfo[iGrabSN].m_iImageSize, pImageBuffer, m_arr_GrabInfo[iGrabSN].m_iImageSize);
		}
		break;
		case e_GrabType::e_GrabType_AVT:
			break;
		case e_GrabType::e_GrabType_BAUMER:
			break;
		case e_GrabType::e_GrabType_JAI:
		{

			if (m_arr_BayerTransform[iGrabSN].m_bAreaCamBayer)
			{
				ConvertBayer2Rgb(&m_arr_pbtBufferOfBayerImg[iGrabSN][0],
					pImageBuffer,
					iImageWidth,
					iImageHeight,
					BAYER2RGB_NEIGHBOUR,
					&m_pLutR[0],
					&m_pLutG[0],
					&m_pLutB[0],
					FALSE,
					BAYER_RG);//**[1/3/2018 ConanSteve]**:	默认=4

			}
			else
			{
				memcpy_s(&m_arr_pbtBufferOfBayerImg[iGrabSN][0], iImageSize, pImageBuffer,
					iImageSize);
			}
			// 对采集的图像进行水平或垂直镜像
			if (0 != m_arr_iSystemFilpType[iGrabSN])
			{
				MirrorSingleImage(&m_arr_pbtBufferOfBayerImg[iGrabSN][0],
					iImageWidth, iImageHeight, iImageByte * 8, (e_ShowModeFlip)m_arr_iSystemFilpType[iGrabSN]);
			}
			break;
		}
		case e_GrabType::e_GrabType_MER://**[12/12/2017 ConanSteve]**:  水星相机
		{
			if (TRUE == m_arr_GrabInfo[iGrabSN].m_bIsColorFilter)
			{
				int iRet = DxRaw8toRGB24(pImageBuffer, &m_arr_pbtBufferOfBayerImg[iGrabSN][0],
					m_arr_sCameraImageInfo[iGrabSN].m_iCameraImageWidth, m_arr_sCameraImageInfo[iGrabSN].m_iCameraImageHeight,
					RAW2RGB_NEIGHBOUR, (DX_PIXEL_COLOR_FILTER)m_arr_GrabInfo[iGrabSN].m_iBayerType, 0 != m_arr_GrabInfo[iGrabSN].m_iFlipType);
			}
			else
			{
				for (VxUint32 i = 0; i < iImageHeight; i++)
				{
					memcpy(&m_arr_pbtBufferOfBayerImg[iGrabSN][0] + i * iImageWidth, pImageBuffer + (iImageHeight - i - 1) * iImageWidth, (size_t)iImageWidth);
				}
			}
			break;
		}
		default:
			return false;
		}
		if (m_bEnableDrawImg)
			g_pView->DisplayGrabImg(iGrabSN);
	}
	/*************************************<算法检测>************************************[12/14/2017 ConanSteve]*/
	if (m_bEnableInitMultiObjectAndCheck)//**[11/21/2017 ConanSteve]**:  如果打开检测
	{

		s_CheckInParam sCheckInParam;
		General::CAlgInspInputParam p_Param_Gerneral;
		Stopper::s_AlgDetectInput p_Param_Stopper;
		LampInsp::s_DetectInParam p_Param_LampInsp;
		LabelCheck::s_AlgInspInPara p_Param_LabelCheck;
		switch (m_eProductLine)
		{
		case e_ProductLine_NO:
			return false;
		case e_ProductLine_General:
		{
			p_Param_Gerneral.iDrawImgType = m_iDrawImgType;
			p_Param_Gerneral.bExecuteAlgCheck = m_arr_bEnableExecCheckOfEachGroup[iCheckGroupSN];
			p_Param_Gerneral.iDisplayImgMode = m_iAlgDisplayMode;//**[1/17/2018 ConanSteve]**:  算法显示模式
			p_Param_Gerneral.iSaveImageMode = (e_SaveImgObj_Alg == m_iSaveImgObj) ? m_iSaveImgType : e_SaveImgType_NO;//**[1/2/2018 ConanSteve]**:  如果存图对象是算法存图			
			p_Param_Gerneral.lpszSaveImagePath = &m_arr_lpszPathOfSaveImg[iGrabSN];

			int iFirstGrabSNInCurGroup = m_arr_iCheckGroupFirstGrabIndexInCamGroup[iGrabSN];//**[1/24/2018 ConanSteve]**:  当前检测组第一个相机号
			p_Param_Gerneral.p_iWidth = &m_arr_iWidth[0] + iFirstGrabSNInCurGroup;
			p_Param_Gerneral.p_iHeight = &m_arr_iHeight[0] + iFirstGrabSNInCurGroup;
			p_Param_Gerneral.p_iBayer = &m_arr_iHalconBayerType[0] + iFirstGrabSNInCurGroup;
			p_Param_Gerneral.p_iRevert = &m_arr_iHalconRevertType[0] + iFirstGrabSNInCurGroup;
			p_Param_Gerneral.p_iCount = &m_arr_iSmallImgCntPerGrab[0] + iFirstGrabSNInCurGroup;
			p_Param_Gerneral.p_iChannel = &m_arr_iChannel[0] + iFirstGrabSNInCurGroup;
			sCheckInParam.pParam = &p_Param_Gerneral;
		}
			break;
		case e_ProductLine_Stopper:
		{
			p_Param_Stopper.nInSig = 0;
			p_Param_Stopper.bRunCheck = 1;
			p_Param_Stopper.nImageWidth = m_arr_GrabInfo[iGrabSN].m_iImageWidth;
			p_Param_Stopper.nImageHeight = m_arr_GrabInfo[iGrabSN].m_iImageHeight;
			p_Param_Stopper.nChannel = m_arr_GrabInfo[iGrabSN].m_iImagePixelSize;
			p_Param_Stopper.nDisplayMode = 1;
			p_Param_Stopper.nSaveImageMode = 0;
			
			memset(p_Param_Stopper.chSaveImagePath, 0, sizeof(p_Param_Stopper.chSaveImagePath));
			sCheckInParam.pParam = &p_Param_Stopper;
		}
			break;
		case e_ProductLine_LampInsp:
		{
			p_Param_LampInsp.pImageInfo = m_p_CamInfo_LampInsp.get() + iCheckGroupSN;
			p_Param_LampInsp.pImageData = (int*)pImageBuffer;
			sCheckInParam.pParam = &p_Param_LampInsp;
		}
			break;
		case e_ProductLine_LableCheck:
		{
			p_Param_LabelCheck.bDsp = 1;
			p_Param_LabelCheck.bRunCheck = TRUE;
			p_Param_LabelCheck.iCircleIndex = m_arr_iHardImageCountOfGrabInRegister[iGrabSN];
			p_Param_LabelCheck.iCount = m_arr_iSmallImgCntPerGrab[iGrabSN];
			p_Param_LabelCheck.nCamIndex = iGrabSN;
			p_Param_LabelCheck.nChannel = m_arr_sCameraImageInfo[iGrabSN].m_iCameraImagePixelSize;
			//p_Param_LabelCheck.nChannel = m_arr_GrabInfo[iGrabSN].m_bIsColorFilter == 1 ? 3 : 1; //
			p_Param_LabelCheck.nDisplayMode = m_iAlgDisplayMode;
			p_Param_LabelCheck.nInSig = 0;

			//**[12/27/2017 ConanSteve]**:  如果当前工位存图打开，且存图类型不为不存图，且为算法存图
			p_Param_LabelCheck.nSaveImageMode = (e_SaveImgObj_Alg == m_iSaveImgObj) ? m_iSaveImgType : e_SaveImgType_NO;
			p_Param_LabelCheck.nHeight = iImageHeight;
			p_Param_LabelCheck.nWidth = iImageWidth;
			p_Param_LabelCheck.pcImageData = m_arr_bWhetherImageConvertedToMultiObj[iGrabSN]?
				(char*)&m_arr_pbtBufferOfBayerImg[iGrabSN][0]:(char*)pImageBuffer;
			p_Param_LabelCheck.bDisplaySys = 0;//**[12/14/2017 ConanSteve]**:  1显示系统图像，0显示算法图像
			p_Param_LabelCheck.bDetectAlg = m_arr_bEnableExecCheckOfEachGroup[iCheckGroupSN];
			p_Param_LabelCheck.nDispFirstorLast = 2;
			p_Param_LabelCheck.iRunMode = e_CallType_RealTime;
			sCheckInParam.pParam = &p_Param_LabelCheck;
		}
			break;
		default:
			break;
		}
		if (m_arr_bWhetherImageConvertedToMultiObj[iGrabSN])
		{
			sCheckInParam.pImageData = (char*)&m_arr_pbtBufferOfBayerImg[iGrabSN][0];
		}
		else
		{
			//sCheckInParam.pImageData = (char*)pImageBuffer;
			//**[1/24/2018 ConanSteve]**:  不使用相机的图像地址，而使用开辟的图像内存空间，防止checksystemconfig.ini配置文件错误，
			//**[1/24/2018 ConanSteve]**:  导致多对象拷贝图像地址越界。
			sCheckInParam.pImageData = (char*)&m_arr_pbtBufferOfGrabImageCopy[iGrabSN][0];
		}
		sCheckInParam.iImageSN = iImgSN;
		sCheckInParam.iCheckSN = iGrabSN;

 		m_arr_ExtenInfo[iCheckGroupSN].push_back(COptionalFunc(e_Fn_CheckInParam_SmallImageNumOfEachGrab).SetInt(m_arr_iSmallImgCntPerGrab[iCheckGroupSN]));
 		m_arr_ExtenInfo[iCheckGroupSN].push_back(COptionalFunc(e_Fn_CheckInParam_ImageWidth).SetInt(m_arr_sCameraImageInfo[iCheckGroupSN].m_iCameraImageWidth));
 		m_arr_ExtenInfo[iCheckGroupSN].push_back(COptionalFunc(e_Fn_CheckInParam_ImageHeight).SetInt(m_arr_sCameraImageInfo[iCheckGroupSN].m_iCameraImageHeight));
 		m_arr_ExtenInfo[iCheckGroupSN].push_back(COptionalFunc(e_Fn_CheckInParam_ImageChannel).SetInt(m_arr_sCameraImageInfo[iCheckGroupSN].m_iCameraImagePixelSize));
 		m_arr_ExtenInfo[iCheckGroupSN].push_back(COptionalFunc(e_Fn_CheckInParam_WhetherCicleSaveImage).SetBool(m_bEnableCircleSaveImg));//*[7/10/2017 ConanSteve]*:  1表示循环存图，0表示不循环存图
 		m_arr_ExtenInfo[iCheckGroupSN].push_back(COptionalFunc(e_Fn_CheckInParam_SaveImagePath).SetPointer((void*)&m_arr_lpszPathOfSaveImg[iGrabSN]));
		m_arr_ExtenInfo[iCheckGroupSN].push_back(e_Fn_CheckInParam_GrabNumPerCheckGroup).SetInt(m_arr_veciGrabSNOfEachCheckGroup[iCheckGroupSN].size());
// 		m_arr_MultipleObjSaveImgPara[iCheckGroupSN].SetImgNum(m_arr_iSmallImgCntPerGrab[iCheckGroupSN]);
// 		m_arr_MultipleObjSaveImgPara[iCheckGroupSN].SetImgWidth(m_arr_sCameraImageInfo[iCheckGroupSN].m_iCameraImageWidth);
// 		m_arr_MultipleObjSaveImgPara[iCheckGroupSN].SetImgHeight(m_arr_sCameraImageInfo[iCheckGroupSN].m_iCameraImageHeight);
// 		m_arr_MultipleObjSaveImgPara[iCheckGroupSN].SetImgChannel(m_arr_sCameraImageInfo[iCheckGroupSN].m_iCameraImagePixelSize);
// 		m_arr_MultipleObjSaveImgPara[iCheckGroupSN].SetIsCirCleSaveImg(m_bEnableCircleSaveImg);//*[7/10/2017 ConanSteve]*:  1表示循环存图，0表示不循环存图
// 		m_arr_MultipleObjSaveImgPara[iCheckGroupSN].SetSaveImgFolderPath(m_arr_lpszPathOfSaveImg[iGrabSN], MAX_PATH);

		if (e_SaveImgType_NO != m_iSaveImgType || e_SaveImgObj_System != m_iSaveImgObj)
			sCheckInParam.iReserve4 = (int)&m_arr_ExtenInfo[iCheckGroupSN];
		else
			sCheckInParam.iReserve4 = 0;

		TCHAR(*lpszPath)[MAX_PATH] = (TCHAR(*)[MAX_PATH]) m_arr_ExtenInfo[iCheckGroupSN].GetFuncStruct(e_Fn_CheckInParam_SaveImagePath).GetPointer();
		CString sTest = lpszPath[0];
		sCheckInParam.iReserve1 = e_SysDetectMode_Single;
		m_multiObject.Detect(sCheckInParam);

	}
	
	/*************************************<系统存图>************************************[12/14/2017 ConanSteve]*/  
	if (e_SaveImgType_NO != m_iSaveImgType&& e_SaveImgObj_System == m_iSaveImgObj&&
		m_p_iCurNumOfSaveImgOfEachGrab[iGrabSN]<m_iMaxNumberOfSaveImg && 
		m_arr_bEnableSaveImgOfEachCheckGroup[iCheckGroupSN])
	{
		CString sPathFileOfBMPImg{ _T("") };
		sPathFileOfBMPImg.Format(_T("%s\\SysImg_%08d.bmp"), m_arr_lpszPathOfSaveImg[iGrabSN], iImgSN);
		SaveBmp(sPathFileOfBMPImg, m_arr_sCameraImageInfo[iCheckGroupSN].m_iCameraImageWidth,
			m_arr_sCameraImageInfo[iCheckGroupSN].m_iCameraImageHeight,
			m_arr_sCameraImageInfo[iCheckGroupSN].m_iCameraImagePixelSize,
			&m_arr_pbtBufferOfBayerImg[iGrabSN][0]);
		++m_p_iCurNumOfSaveImgOfEachGrab[iGrabSN];
	}
	return true;
}


bool CMainFrame::WriteCfg()
{
	/*************************************<将系统和相机配置文件拷贝回去>************************************[9/6/2017 ConanSteve]*/
	//CopyFile(m_sPathFileOfCheckSysCfgInConfig, m_sPathFileOfCheckSysCfgInModelInfo, FALSE);//**[9/1/2017 ConanSteve]**:  强制覆盖
	for (int i = 0; i < m_iGrabberNum; ++i)
	{
		//CopyFile(m_arr_sPathFileOfGrabInitInConfig[i], m_arr_sPathFileOfGrabInitInModelInfo[i], FALSE);//**[9/1/2017 ConanSteve]**:  强制覆盖
	}
	return false;
}


bool CMainFrame::OnBtnClickOpenDemo()
{

	if (!m_threeTierRights.VerifyRights(e_UserType_Debugger))
		return false;
	//CString sCmd = _T("notepad.exe ") + m_sPathFileOfDebugInfo;
	CString sCmd{ _T("") };
#ifdef DEBUG
	sCmd.Format(_T("%s\\DemoD.exe Save:Y"), m_sPathOfApp);
#else
	sCmd.Format(_T("%s\\Demo.exe Save:Y"), m_sPathOfApp);
#endif // DEBUG


#ifdef UNICODE
	WinExec(CStringConvert::UCS2_ANSI(sCmd.GetBuffer()).c_str(), SW_NORMAL);
#else
	WinExec(sCmd, SW_NORMAL);
#endif
	return false;
}


bool CMainFrame::OnBnClickOpenParamSet()
{
	m_pdlgParamSet = make_shared<CDlgParamSet>();
	m_pdlgParamSet->Create(IDD_DIALOG_ParamSet, NULL);
	//m_pdlgParamSet->ShowWindow(SW_SHOWMAXIMIZED);
	m_pdlgParamSet->ShowWindow(SW_SHOW);
	return false;
}


bool CMainFrame::ReleaseResource()
{
	if(m_bEnableInitGrabber)
	{
		for (int i = 0; i < m_iGrabberNum; ++i)
		{
			m_arr_pGrabber[i]->Close();
		}
	}
	if (m_bEnableInitIOCard)
	{
		for (int i = 0; i < m_iIOCardNum; ++i)
		{
			m_arr_pIOCard[i]->CloseCard();
		}
	}
	if (m_bEnableInitMultiObjectAndCheck)//**[11/21/2017 ConanSteve]**:  如果打开多对象检测
	{
		m_multiObject.Free();
	}
	return true;
}




bool CMainFrame::JudgeGrabCircleNew(int  iCameraSN,
	int  iAllElementSN,
	int& iGrabCircle,
	int& iPicSN)
{
	return true;
}


CRetInfo CMainFrame::InitMultiObject()
{
	CRetInfo ret;
	/*************************************<必要参数初始化>************************************[11/23/2017 ConanSteve]*/
	if (!m_bEnableInitMultiObjectAndCheck)
	{
		m_fileDebugInfo.Write(_T("警告：多对象初始化跳过！"));
		ret.bRet = true;
		return ret;
	}
	/*************************************<>************************************[11/16/2017 ConanSteve]*/
	unique_ptr<HWND[]> p_arrCamImg = make_unique<HWND[]>(m_iGrabberNum);
	unique_ptr<HWND[]> p_arrAlgImg = make_unique<HWND[]>(m_iGrabberNum);
	//**[11/27/2017 ConanSteve]**:  统计每个剔废队列的检测组数
	{
		unique_ptr<bool[]> p_bHasBeenSetOfCheckGroupForFirstGrabIndex = make_unique<bool[]>(m_iCheckGroupNum);//**[11/27/2017 ConanSteve]**:  每个检测组的第一个相机的索引是否被设置了
		
		memset(p_bHasBeenSetOfCheckGroupForFirstGrabIndex.get(), 0, sizeof(bool)*m_iCheckGroupNum);
		//memset(p_bHasBeenSetOfCheckGroupForKickQueue.get(), 0, sizeof(bool)*m_iCheckGroupNum);
		
		for (int iGrabIndex = 0; iGrabIndex < m_iGrabberNum; ++iGrabIndex)
		{
			CString sModuleName{ _T("") };
			sModuleName.Format(_T("Camera%d Parameters"), iGrabIndex + 1);
			int iCheckGroupSN = m_iniAlgConfig.GetIntEx(sModuleName, _T("CheckGroup"))-1;
			int iKickQueueSN = m_iniAlgConfig.GetIntEx(sModuleName, _T("KickGroup"))-1;
			if (iCheckGroupSN<0 || iCheckGroupSN>m_iCheckGroupNum)
			{
				ret.bRet = false;
				ret.sErrorInfo = tr("检测组索引不正确，请重新确认配置文件！");
				return ret;
			}
			if(iKickQueueSN<0||iKickQueueSN>m_iKickQueueNum)
			{
				ret.bRet = false;
				ret.sErrorInfo = tr("剔废队列索引不正确，请重新确认配置文件！");
				return ret;
			}
			m_arr_veciGrabSNOfEachCheckGroup[iCheckGroupSN].push_back(iGrabIndex);

			
		}
	}
	/*************************************<通用算法>************************************[11/16/2017 ConanSteve]*/
	unique_ptr<General::CAlgSingleInitParam[]> p_arrParam_General = make_unique<General::CAlgSingleInitParam[]>(m_iCheckGroupNum);//**[11/16/2017 ConanSteve]**:  通用新系统结构体
	/*************************************<胶塞算法>************************************[11/16/2017 ConanSteve]*/
	unique_ptr<Stopper::s_AlgInitParam[]> p_arrParam_Stopper = make_unique<Stopper::s_AlgInitParam[]>(m_iCheckGroupNum);//**[11/16/2017 ConanSteve]**:  胶塞系统程序
	CDC* pDC[31];//**[11/16/2017 ConanSteve]**:  胶塞/采血管,最大支持30个相机
	memset(pDC, 0, sizeof(pDC));
	/*************************************<灯检机算法>************************************[11/24/2017 ConanSteve]*/
	unique_ptr<LampInsp::s_DetectInitParam[]> p_arrParam_LampInsp = make_unique<LampInsp::s_DetectInitParam[]>(m_iCheckGroupNum);
	unique_ptr<LampInsp::s_DetectInitParam*[]> pp_arrParam_LampInsp = make_unique<LampInsp::s_DetectInitParam*[]>(m_iCheckGroupNum);
	int pWnd[50];
	/*************************************<采血管算法>************************************[12/12/2017 ConanSteve]*/
	unique_ptr<LabelCheck::s_AlgSingleInit[]> p_arrParam_LabelCheck = make_unique<LabelCheck::s_AlgSingleInit[]>(m_iCheckGroupNum);
	int pAddr_Label[31];
	int pAddr_General[51];
	memset(pAddr_Label, 0, sizeof(pAddr_Label));
	/*************************************<设置窗口句柄>************************************[11/16/2017 ConanSteve]*/
	for (int i = 0; i < m_iGrabberNum; ++i)
	{
		//**[12/14/2017 ConanSteve]**:  通用算法
		p_arrCamImg[i] = g_pView->m_pdlgRunning->GetDlgItem(ID_STATIC_CAMERA_IMAGE+i)->GetSafeHwnd();
		p_arrAlgImg[i] = g_pView->m_pdlgRunning->GetDlgItem(ID_STATIC_ALGORITHM_IMAGE + i)->GetSafeHwnd();//**[11/21/2017 ConanSteve]**:  GetDlgItem(ID_STATIC_ALGORITHM_IMAGE + i)->
		m_arr_iWidth[i] = m_arr_GrabInfo[i].m_iImageWidth;
		m_arr_iHeight[i] = m_arr_GrabInfo[i].m_iImageHeight;
		m_arr_iHalconBayerType[i] =m_arr_sCameraImageInfo[i].m_iBayerType;
		m_arr_iHalconRevertType[i] = m_arr_sCameraImageInfo[i].m_iFlipType;
		m_arr_iChannel[i] = m_arr_GrabInfo[i].m_bIsColorFilter ? 3 : 1;

		//胶塞算法
		pDC[i] = (CDC*)p_arrAlgImg[i];

		//**[11/24/2017 ConanSteve]**:  灯检机算法
		pWnd[i] = (int)p_arrAlgImg[i];

		//**[12/14/2017 ConanSteve]**:  采血管算法

	}
	/*************************************<设置私有结构体变量>************************************[11/16/2017 ConanSteve]*/
	for (int i = 0; i < m_iCheckGroupNum; ++i)
	{
		//**[11/16/2017 ConanSteve]**:  通用算法
		if(m_arr_veciGrabSNOfEachCheckGroup[i].size()>0)//**[11/27/2017 ConanSteve]**:  防止用户设置错误，导致某个检测组没有相机
			m_arr_iCheckGroupFirstGrabIndexInCamGroup[i] = m_arr_veciGrabSNOfEachCheckGroup[i][0];
		else
		{
			ret.bRet = false;
			ret.sErrorInfo.Format(_T("工位(检测组)%d相机数为0，请检查配置文件！"), i+1);
			return ret;
		}
		int iFirstGrabSNInCurGroup = m_arr_iCheckGroupFirstGrabIndexInCamGroup[i];//**[1/24/2018 ConanSteve]**:  当前检测组第一个相机号
		p_arrParam_General[i].iStation = i;
		p_arrParam_General[i].iWindowNum = m_arr_veciGrabSNOfEachCheckGroup[i].size();
		p_arrParam_General[i].iCallType = e_CallType_RealTime;
		p_arrParam_General[i].p_iWidth = &m_arr_iWidth[0] + iFirstGrabSNInCurGroup;
		p_arrParam_General[i].p_iHeight = &m_arr_iHeight[0] + iFirstGrabSNInCurGroup;
		p_arrParam_General[i].p_iBayer = &m_arr_iHalconBayerType[0] + iFirstGrabSNInCurGroup;
		p_arrParam_General[i].p_iRevert = &m_arr_iHalconRevertType[0] + iFirstGrabSNInCurGroup;
		p_arrParam_General[i].p_iCount = &m_arr_iSmallImgCntPerGrab[0]+iFirstGrabSNInCurGroup;
		p_arrParam_General[i].p_iChannel = &m_arr_iChannel[0]+iFirstGrabSNInCurGroup;
		p_arrParam_General[i].hWndCamImg = p_arrCamImg.get() + i;
		p_arrParam_General[i].hWndAlgImg = p_arrAlgImg.get() + i;
		pAddr_General[i] = (int)&p_arrParam_General[i];

		//**[11/16/2017 ConanSteve]**:胶塞算法
		p_arrParam_Stopper[i].pDC = pDC;
		p_arrParam_Stopper[i].nBayer[0] = m_arr_GrabInfo[i].m_iBayerType;
		p_arrParam_Stopper[i].nRevert[0] = m_arr_GrabInfo[i].m_iFlipType;

		//**[11/24/2017 ConanSteve]**:  灯检机算法
		p_arrParam_LampInsp[i].pHWnd = pWnd;
		p_arrParam_LampInsp[i].iRunMode = 0;//**[11/24/2017 ConanSteve]**:  在线检测
		p_arrParam_LampInsp[i].iAlgSN = i+1;
		pp_arrParam_LampInsp[i] = &p_arrParam_LampInsp[i];

		//**[12/12/2017 ConanSteve]**:  采血管算法
		p_arrParam_LabelCheck[i].pDC = pDC;
		p_arrParam_LabelCheck[i].nCameraIndex = iFirstGrabSNInCurGroup;
		p_arrParam_LabelCheck[i].nWidth = m_arr_sCameraImageInfo[iFirstGrabSNInCurGroup].m_iCameraImageWidth;
		p_arrParam_LabelCheck[i].nHeight = m_arr_sCameraImageInfo[iFirstGrabSNInCurGroup].m_iCameraImageHeight;
		if (m_arr_bWhetherImageConvertedToMultiObj[i])
		{
			p_arrParam_LabelCheck[i].nBayer= 0;
		}
		else
		{
			p_arrParam_LabelCheck[i].nBayer = m_arr_sCameraImageInfo[iFirstGrabSNInCurGroup].m_iBayerType;
		}
		p_arrParam_LabelCheck[i].nRevert = m_arr_sCameraImageInfo[iFirstGrabSNInCurGroup].m_iFlipType;
		pAddr_Label[i] = (int)&p_arrParam_LabelCheck[i];
	}
	/*************************************<初始化算法库>************************************[11/16/2017 ConanSteve]*/
	s_CheckInitParam sInit;
	switch (m_eProductLine)
	{
	case e_ProductLine_NO:
		m_fileErrorInfo.Write(tr("多对象初始化失败,产品线不能为未设置！"));
		ret.bRet = false;
		ret.sErrorInfo = tr("多对象初始化失败,产品线不能为未设置！");
		return ret;
		break;
	case e_ProductLine_General:
	{
		sInit.pParam = (void*)pAddr_General;
// 		void* ppp = (void*)pAddr_General;
// 		sInit.pParam = (void*)p_arrParam_General.get();
		break;
	}
	case e_ProductLine_Stopper:
		sInit.pParam = p_arrParam_Stopper.get();
		break;
	case e_ProductLine_LampInsp:
		sInit.pParam = pp_arrParam_LampInsp.get();

		m_p_CamInfo_LampInsp = make_unique<LampInsp::s_CheckCameraInfo[]>(m_iCheckGroupNum);
		for (int i = 0; i < m_iCheckGroupNum; ++i)
		{
			m_p_CamInfo_LampInsp[i].iBitCount = m_arr_sCameraImageInfo[i].m_iCameraImagePixelSize*8;
			m_p_CamInfo_LampInsp[i].iCount = m_arr_iSmallImgCntPerGrab[i];
			m_p_CamInfo_LampInsp[i].iHeight = m_arr_sCameraImageInfo[i].m_iCameraImageHeight;
			m_p_CamInfo_LampInsp[i].iWidth = m_arr_sCameraImageInfo[i].m_iCameraImageWidth;
			m_p_CamInfo_LampInsp[i].iType = 1;
		}
		break;
	case e_ProductLine_LableCheck:
	{
		sInit.pParam = (void*)pAddr_Label;
	}
		break;
	default:
		break;
	}
	sInit.KickCallbackFunc = GlobalKickCallbackFunc;
	sInit.StatusCallbackFunc = GlobalStatusCallbackFunc;
	sInit.pFilePath = m_sPathFileOfAlgCfg.GetBuffer();
	sInit.pThis = this;
	sInit.iCallType = e_CallType_RealTime;
	sInit.iReserve1 = e_MachineType_Liquor;

	s_Status sStatus;
	if (m_bEnableInitMultiObjectAndCheck&&0<m_iCheckGroupNum)
	{
		sStatus = m_multiObject.Init(sInit);
		if (STATUS_NOR != sStatus.iStatusType)
		{
			ret.bRet = false;
			ret.sErrorInfo = sStatus.pErrorInfo;
			return ret;
		}
	}

	LoadModel(m_sModelName);
	/*************************************<>************************************[11/22/2017 ConanSteve]*/
	ret.bRet = true;
	return ret;
}

void CMainFrame::GlobalKickCallbackFunc(const s_KickParam * pKickParam)
{
	if (pKickParam && pKickParam->pThis)
	{
		CMainFrame* pMF = (CMainFrame*)pKickParam->pThis;
		pMF->KickCallbackFunc1(pKickParam);
	}
}

void CMainFrame::KickCallbackFunc1(const s_KickParam * pKickParam)
{
	int iKickSN = pKickParam->iKickSN;
	int iImgSN = pKickParam->iImageSN;
	int iCheckRsltNum = m_arr_veciCheckSNInKickQueue[iKickSN].size();//**[11/27/2017 ConanSteve]**:  当前剔废组检测结果的数目
	unique_ptr<General::CAlgInspResult[]> p_sOut_General = make_unique<General::CAlgInspResult[]>(iCheckRsltNum);
	unique_ptr<Stopper::s_AlgDetectOutput []> p_sOut_Stopper = make_unique<Stopper::s_AlgDetectOutput[]>(iCheckRsltNum);
	unique_ptr<LampInsp::s_DetectOutParam[]> p_sOut_LampInsp = make_unique<LampInsp::s_DetectOutParam[]>(iCheckRsltNum);
	unique_ptr<LabelCheck::s_AlgInspResult[]> p_sOut_LabelCheck = make_unique<LabelCheck::s_AlgInspResult[]>(iCheckRsltNum);
	s_sRsltInfoWritten sRstlInfoWritten;//**[11/27/2017 ConanSteve]**:  写接口卡信息
	unique_ptr<bool[]> p_bKick{ nullptr };//**[11/27/2017 ConanSteve]**:  是否剔废
	sRstlInfoWritten.iImgSN = iImgSN;
	sRstlInfoWritten.iKickSN = iKickSN;
	sRstlInfoWritten.iRsltNum = 0;
	CString sLogInfo{ _T("") }, sTemp{ _T("") };
	sLogInfo.Format(_T("[剔废号:%2d][图像号:%8d]["), iKickSN, iImgSN);
	switch (m_eProductLine)
	{
	case e_ProductLine_NO:
		return;
	case e_ProductLine_General:
	{
		memcpy_s(p_sOut_General.get(), sizeof(General::CAlgInspResult)*iCheckRsltNum, pKickParam->pParam, sizeof(General::CAlgInspResult)*iCheckRsltNum);
		//**[12/4/2017 ConanSteve]**:遍历每个检测组的检测结果
		bool bEmpty{ false };//**[12/4/2017 ConanSteve]**:  空产品
		bool bKickValueFromAlgStatus{ false };//**[12/14/2017 ConanSteve]**:  每个检测结果对应的剔废结果
		for (int i = 0; i < iCheckRsltNum; ++i)  
		{
			//**[12/4/2017 ConanSteve]**:  查询当前检测结果的的检测组号

			int iCheckSN = m_arr_veciCheckSNInKickQueue[iKickSN][i];
			if (1 == p_sOut_General[i].iRsltNum)
				p_sOut_General[i].arrAlgStatus[0] = p_sOut_General[i].iAlgStatus;
			for (int j = 0; j < p_sOut_General[i].iRsltNum; ++j)
			{
				if (nullptr == p_bKick)
				{
					p_bKick = make_unique<bool[]>(p_sOut_General[0].iRsltNum);
					sRstlInfoWritten.iRsltNum = p_sOut_General[0].iRsltNum;
				}
				int iAlgStatus = p_sOut_General[i].arrAlgStatus[j];
				sTemp.Format(_T("%d-"), iAlgStatus);
				sLogInfo += sTemp;
				switch (iAlgStatus)
				{
				case e_AlgStatus_Good:
				{
					++m_arr_iCheckCntOfCheckGroup[iCheckSN];
					++m_arr_iGoodCntOfCheckGroup[iCheckSN];
					++m_arr_iGoodCntOfKickQueue[iKickSN];
					bKickValueFromAlgStatus = false;					
					bEmpty = false;
					break;
				}
				case e_AlgStatus_AlgException:
				{
					bKickValueFromAlgStatus = true;
					break;
				}
				case e_AlgStatus_Empty:
				{
					bKickValueFromAlgStatus = m_bEnableEmptyKick;
					bEmpty = true;
					break;
				}
				case e_AlgStatus_Bad:
				{
					bKickValueFromAlgStatus = true;
					++m_arr_iCheckCntOfCheckGroup[iCheckSN];
					++m_arr_iBadCntOfCheckGroup[iCheckSN];
					bEmpty = false;
					break;
				}
				case e_AlgStatus_LostImg:
				{
					bKickValueFromAlgStatus = bKickValueFromAlgStatus;
					break;
				}
				case e_AlgStatus_Overtime:
				{
					bKickValueFromAlgStatus = true;
				}
					break;
				default:
					break;
				}
				
				sRstlInfoWritten.arr_bKick[j] |= bKickValueFromAlgStatus;
				
			}

			//**[12/5/2017 ConanSteve]**:  读取缺陷描述统计信息
			for (int iDefectIndex = 0; iDefectIndex < p_sOut_General[i].iLenOfDefect;++iDefectIndex)
			{
				int iDefectTypeSN = p_sOut_General[i].arrHeadOfDefectDesc[iDefectIndex];
				auto itera_find = m_arr_mapDefectStatistics[iCheckSN].find(iDefectTypeSN);
				if (itera_find != m_arr_mapDefectStatistics[iCheckSN].end())//**[12/4/2017 ConanSteve]**:  如果当前返回结果符合从配置文件中的缺陷类型描述
				{
					itera_find->second.iCnt++;//**[12/4/2017 ConanSteve]**:  缺陷统计计数加1
				}
			}
		}
		//**[12/4/2017 ConanSteve]**:  如果检测结果非空且剔废组为0
		if (!bEmpty&& e_CheckTotleCntType_Rlst == m_iCheckTotleCntType&& 0 == iKickSN)
		{
			m_iCheckCntTotle++;
		}
	}
		break;
	case e_ProductLine_Stopper:
		break;
	case e_ProductLine_LampInsp:
		break;
	case e_ProductLine_LableCheck:
	{
		if (e_KickType_Single != m_arr_eKickType[iKickSN])//**[12/14/2017 ConanSteve]**:  采血管算法不支持多路剔废
			return;
		memcpy_s(p_sOut_LabelCheck.get(), sizeof(LabelCheck::s_AlgInspResult)*iCheckRsltNum, pKickParam->pParam, sizeof(LabelCheck::s_AlgInspResult)*iCheckRsltNum);
		sRstlInfoWritten.iRsltNum = 1;
		bool bKick{ false };
		for (int i = 0; i < iCheckRsltNum; ++i)
		{
			int iCheckSN = m_arr_veciCheckSNInKickQueue[iKickSN][i];
			e_AlgStatus eAlgStatus = (e_AlgStatus)(p_sOut_LabelCheck[i].nAlgStatus);
			//**[12/28/2017 ConanSteve]**:  算计数
			if (e_AlgStatus_Good == eAlgStatus || e_AlgStatus_Bad == eAlgStatus || e_AlgStatus_AlgException)
			{
				m_arr_iCheckCntOfCheckGroup[iCheckSN]++;//**[12/28/2017 ConanSteve]**:  产品计数
			}
			if (e_AlgStatus_Bad == eAlgStatus || e_AlgStatus_AlgException == eAlgStatus)
			{
				m_arr_iBadCntOfCheckGroup[iCheckSN]++;//**[12/28/2017 ConanSteve]**:  坏品计数
			}

			//**[12/28/2017 ConanSteve]**:  算剔废结果
			if (e_AlgStatus_Good != eAlgStatus)
			{
				bKick = true;
			}
			else
			{
				bKick = false;
			}
			sTemp.Format(_T("%2d-"), eAlgStatus);
			sLogInfo += sTemp;
			sRstlInfoWritten.arr_bKick[0] |= bKick;
		}
	}
		break;
	default:
		break;
	}
	sLogInfo += _T("]");

	//**[1/26/2018 ConanSteve]**:	如果不执行踢废，则所有都为false 
	if (!m_arr_bEnableKickOfKickQueue[iKickSN])
	{
		fill_n(sRstlInfoWritten.arr_bKick, 30, false);
	}
	m_arr_fileKickCallback[iKickSN].Write(sLogInfo);
	if (m_bEnableInitIOCard)
	{
		for (int i = 0; i < m_iIOCardNum; ++i)
		{
			if (!m_arr_pIOCard[i]->WriteKickResult(sRstlInfoWritten))
			{
				m_fileErrorInfo.Write(m_arr_pIOCard[i]->GetLastError());
			}
		}
	}
}

void CMainFrame::GlobalStatusCallbackFunc(const s_Status * sStatusParam)
{
}

void CMainFrame::StatusCallbackFunc1(const s_Status * sStatusParam)
{
}

int CMainFrame::SaveBmp(const TCHAR* pPathName, int iWidth, int iHeight, int iByte, TBYTE *pDest)
{
	BITMAPFILEHEADER bmFH;
	int nPaletteEntries = 256;
	DWORD dwSize = iWidth*iHeight*iByte;

	if (iByte < 3)
	{
		nPaletteEntries = 256;
	}
	else
	{
		nPaletteEntries = 0;
	}

	bmFH.bfType = 19778;
	//	bmFH.bfType = "BM";

	bmFH.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
		sizeof(BITMAPINFOHEADER) +
		nPaletteEntries * sizeof(RGBQUAD)) +
		dwSize;
	bmFH.bfReserved1 = bmFH.bfReserved2 = 0;
	bmFH.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) +
		sizeof(BITMAPINFOHEADER) +
		nPaletteEntries * sizeof(RGBQUAD);

	BITMAPINFO *FileBMP;
	char chBmpBuf[2048];
	FileBMP = (BITMAPINFO *)chBmpBuf;
	FileBMP->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	FileBMP->bmiHeader.biWidth = iWidth;
	FileBMP->bmiHeader.biHeight = iHeight;
	FileBMP->bmiHeader.biPlanes = 1;
	FileBMP->bmiHeader.biBitCount = iByte * 8;
	FileBMP->bmiHeader.biCompression = BI_RGB;
	FileBMP->bmiHeader.biSizeImage = 0;
	FileBMP->bmiHeader.biXPelsPerMeter = 0;
	FileBMP->bmiHeader.biYPelsPerMeter = 0;
	FileBMP->bmiHeader.biClrUsed = 0;
	FileBMP->bmiHeader.biClrImportant = 0;

	//(2003-4-2)
	//对于8位位图来说，其需要调色板。对于256色地位图，调色板数组的个数位256。
	for (int k = 0; k < 256; k++)
	{
		FileBMP->bmiColors[k].rgbBlue = (BYTE)k;
		FileBMP->bmiColors[k].rgbGreen = (BYTE)k;
		FileBMP->bmiColors[k].rgbRed = (BYTE)k;
		FileBMP->bmiColors[k].rgbReserved = 0;
	}


	CFile ImageFile;

	if (ImageFile.Open(pPathName, CFile::modeCreate | CFile::modeReadWrite))
	{
		ImageFile.Write(&bmFH, sizeof(BITMAPFILEHEADER));
		ImageFile.Write(&FileBMP->bmiHeader, sizeof(BITMAPINFOHEADER));
		ImageFile.Write(&FileBMP->bmiColors[0], sizeof(RGBQUAD) * nPaletteEntries);
		ImageFile.Write(pDest, dwSize);
		ImageFile.Close();
		return 1;
	}
	else
		return -1;

}


e_BayerHalcon CMainFrame::TranslateToHalconBayer(int iBayerType)
{
	e_BayerHalcon eRet = e_BayerHalcon_NONE;
	switch (iBayerType)
	{
	case DX_PIXEL_COLOR_FILTER::NONE:
		eRet = e_BayerHalcon_NONE;
		break;
	case DX_PIXEL_COLOR_FILTER::BAYERRG:
		eRet = e_BayerHalcon_RG;
		break;
	case DX_PIXEL_COLOR_FILTER::BAYERGB:
		eRet = e_BayerHalcon_GB;
		break;
	case DX_PIXEL_COLOR_FILTER::BAYERGR:
		eRet = e_BayerHalcon_GR;
		break;
	case DX_PIXEL_COLOR_FILTER::BAYERBG:
		eRet = e_BayerHalcon_BG;
		break;
	default:
		break;
	}
	return eRet;
}



void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case ID_EVENT_UPDATE_INFO://**[12/1/2017 ConanSteve]**:  更新显示信息
	{
		/*************************************<标签信息>************************************[12/1/2017 ConanSteve]*/
		CString sText{ _T("") };
		CString sTemp{ _T("") };
		/*************************************<检测产品总数>************************************[12/28/2017 ConanSteve]*/
		m_iCheckCntTotle = 0;	
		for (int i = 0; i < m_iAssemblyLineNum; ++i)//**[12/28/2017 ConanSteve]**:  检测总数等于每道流水线的第一个检测组的产品计数之和
		{
			if (0 == m_arr_veciCheckGroupIndexOfAsseblyLine[i].size())
			{
				KillTimer(ID_EVENT_UPDATE_INFO);
				sTemp.Format(tr("系统异常，即将退出\n错误原因：流水线%d包含的检测组个数为0，请重新检查系统配置文件"), i);
				AfxMessageBox(sTemp);
				m_fileErrorInfo.Write(sTemp);
				exit(0);
			}
			int iFirstCheckSNInCurAsbly = m_arr_veciCheckGroupIndexOfAsseblyLine[i][0];//**[12/28/2017 ConanSteve]**:  当前流水线的第一个检测组号
			m_arr_iCheckCntOfAssembly[i] = m_arr_iCheckCntOfCheckGroup[iFirstCheckSNInCurAsbly];
			m_iCheckCntTotle += m_arr_iCheckCntOfAssembly[i];
		}
		sText.Format(_T("%s: %d"),tr("检测总数"), m_iCheckCntTotle);
		g_pView->m_pdlgRunning->m_staticCheckCntLabel.SetWindowText(sText);
		g_pView->m_pdlgRunning->SetDlgItemText(IDC_BUTTON_LABEL_CHECK_COUNT_TOTLE, sText);

		/*************************************<整个系统的坏品计数>************************************[12/28/2017 ConanSteve]*/
		m_iCheckErrorCntTotle = 0;
		for (int i = 0; i < m_iAssemblyLineNum; ++i)//**[12/28/2017 ConanSteve]**:  整个系统的坏品计数等于每个流水线最后一个检测组的产品计数减去这个流水线的最后一个剔废队列的剔废个数之和
		{
			int iLastCheckSNInCurAsbly = *m_arr_veciCheckGroupIndexOfAsseblyLine[i].rbegin();//**[12/28/2017 ConanSteve]**:  当前流水线的最后一个检测组的组号
			int iLastKickSNInCurAsbly = *m_arr_veciKickSNInAssemblyLine[i].rbegin();//**[12/28/2017 ConanSteve]**:  当前流水线的最后一个剔废队列的编号
			s_KickCountInfo & rKickCntInfo = m_arr_sKickCntInfoOfKickQueue[iLastKickSNInCurAsbly];
			int iBadCntOfCurKickQueue = 0;//**[12/28/2017 ConanSteve]**:  当前踢废队列所踢的坏品之和。 瓶盖分色踢废的并非都是坏品
			for (int j = 0; j < rKickCntInfo.iKickPortNum; ++j)
			{
				if (rKickCntInfo.arr_bIsBadKick[j])//**[12/28/2017 ConanSteve]**:  如果是坏品踢废
					iBadCntOfCurKickQueue += rKickCntInfo.arr_iKickPortCount[j];
			}
			//m_arr_iCheckGoodCntOfAssembly[i] = m_arr_iCheckCntOfCheckGroup[iLastCheckSNInCurAsbly] - iBadCntOfCurKickQueue;
			m_arr_iCheckGoodCntOfAssembly[i] = m_arr_iGoodCntOfKickQueue[iLastKickSNInCurAsbly];
			m_arr_iCheckErrorCntOfAssembly[i] = m_arr_iCheckCntOfAssembly[i] - m_arr_iCheckGoodCntOfAssembly[i];
			if (0 != m_arr_iCheckCntOfAssembly[i])
				m_arr_lfGoodRatioOfAssembly[i] = m_arr_iCheckGoodCntOfAssembly[i] / (double)m_arr_iCheckCntOfAssembly[i] * 100.0;
			else
				m_arr_lfGoodRatioOfAssembly[i] = 0;
			m_iCheckErrorCntTotle += m_arr_iCheckErrorCntOfAssembly[i];
			sText.Format(_T("%s%d%s: %d"), tr("流水线"), i + 1, tr("检测总数"), m_arr_iCheckCntOfAssembly[i]);
			g_pView->m_pdlgRunning->SetDlgItemText(IDC_BUTTON_LABEL_TOTLE_COUNT_ASSEMBLY + i, sText);
			sText.Format(_T("%s%d%s: %d"), tr("流水线"), i + 1, tr("合格总数"), m_arr_iCheckGoodCntOfAssembly[i]);
			g_pView->m_pdlgRunning->SetDlgItemText(IDC_BUTTON_LABEL_GOOD_COUNT_ASSEMBLY + i, sText);
			sText.Format(_T("%s%d%s: %d"), tr("流水线"), i + 1, tr("不合格总数"), m_arr_iCheckErrorCntOfAssembly[i]);
			g_pView->m_pdlgRunning->SetDlgItemText(IDC_BUTTON_LABEL_BAD_COUNT_ASSEMBLY + i, sText);
			sText.Format(_T("%s%d%s: %.3f%%"), tr("流水线"), i + 1, tr("合格率"), m_arr_lfGoodRatioOfAssembly[i]);
			g_pView->m_pdlgRunning->SetDlgItemText(IDC_BUTTON_LABEL_GOOD_RATIO_ASSEMBLY + i, sText);
		}
		sText.Format(_T("%s: %d"),tr("不合格总数"), m_iCheckErrorCntTotle);
		g_pView->m_pdlgRunning->m_staticErrorCntLabel.SetWindowText(sText);
		g_pView->m_pdlgRunning->SetDlgItemText(IDC_BUTTON_LABEL_ERROR_COUNT_TOTLE, sText);
		/*************************************<>************************************[12/28/2017 ConanSteve]*/
		int iGoodCntTotle = m_iCheckCntTotle - m_iCheckErrorCntTotle;
		sText.Format(_T("%s: %d"), tr("合格总数"),iGoodCntTotle);
		g_pView->m_pdlgRunning->m_staticGoodCntLabel.SetWindowText(sText);
		g_pView->m_pdlgRunning->SetDlgItemText(IDC_BUTTON_LABEL_GOOD_COUNT_TOTLE, sText);

		double lfGoodRate = 0;
		if (0 < iGoodCntTotle)
		{
			lfGoodRate = (double)(iGoodCntTotle) * 100.0 / m_iCheckCntTotle;
		}
		sText.Format(_T("%s: %.3f%%"), tr("合格率"),lfGoodRate);
		g_pView->m_pdlgRunning->m_staticGoodRateLabel.SetWindowText(sText);
		g_pView->m_pdlgRunning->SetDlgItemText(IDC_BUTTON_LABEL_GOOD_RATE_TOTLE, sText);
		/*************************************<状态栏信息>************************************[12/1/2017 ConanSteve]*/
		sText = _T("");

		if (m_bEnableShowPE)
		{
			sText += tr("光电:");
			for (int i = 0; i < m_iPENum; ++i)
			{
				sTemp.Format(_T("[%d]"), m_arr_iPECount[i]);
				sText += sTemp;
			}
		}
		
		sText += (CString)_T(" ")+tr("采集")+(CString)_T(":");
		for (int i = 0; i < m_iGrabberNum; ++i)
		{
			sTemp.Format(_T("[%d]"), m_arr_iGrabCallbackCnt[i]);
			sText += sTemp;
		}
		sTemp = (CString)_T(" ")+ tr("图像号") + (CString)_T(":");
		sText += sTemp;
		for (int i=0;i<m_iGrabberNum;++i)
		{
			sTemp.Format(_T("[%d]"), m_arr_iHardImageCountOfGrabInRegister[i]);
			sText += sTemp;
		}
		/*****************************************************************************************/
		if (m_bEnableShowGrabLostFrame)
		{
			sText += (CString)_T(" ")+tr("丢帧") + (CString)_T(":");
			for (int i = 0; i < m_iGrabberNum; ++i)
			{
				sTemp.Format(_T("[%d]"), m_arr_iLostFrameEachGrab[i]);
				sText += sTemp;
			}
		}
		/*****************************************************************************************/
		{
			sText += (CString)_T(" ")+ tr("产品数") + (CString)_T(":");
			for (int i = 0; i < m_iCheckGroupNum; ++i)
			{
				sTemp.Format(_T("[%d]"), m_arr_iCheckCntOfCheckGroup[i]);
				sText += sTemp;
			}
		}
		/*****************************************************************************************/
		{
			sTemp = (CString)_T(" ") +tr("废品") + (CString)_T(":");
			sText += sTemp;
			for (int i = 0; i < m_iCheckGroupNum; ++i)
			{
				sTemp.Format(_T("[%d]"), m_arr_iBadCntOfCheckGroup[i]);
				sText += sTemp;
			}
		}
		/*****************************************************************************************/
		{
			sTemp = (CString)_T(" ") +tr("剔废") + (CString)_T(":");
			sText += sTemp;
			for (int i = 0; i < m_iKickQueueNum; ++i)
			{
				sText += _T("[");
				for (int j = 0; j < m_arr_sKickCntInfoOfKickQueue[i].iKickPortNum; ++j)
				{
					CString sFormat{ _T("") };
					sFormat = 0 == j ? _T("%d") : _T("-%d");
					sTemp.Format(sFormat, m_arr_sKickCntInfoOfKickQueue[i].arr_iKickPortCount[j]);
					sText += sTemp;
				}
				sText += _T("]");
			}
		}
		/*****************************************************************************************/
		if (m_bEnableShowAutoKick)
		{
			sTemp = tr("补踢:");
			sText += sTemp;
			for (int i = 0; i < m_iKickQueueNum; ++i)
			{
				sText += _T("[");
				for (int j = 0; j < m_arr_sKickCntInfoOfKickQueue[i].iKickPortNum; ++j)
				{
					CString sFormat{ _T("") };
					sFormat = 0 == j ? _T("%d") : _T("-%d");
					sTemp.Format(sFormat, m_arr_sKickCntInfoOfKickQueue[i].arr_iAutoKickCnt[j]);
					sText += sTemp;
				}
				sText += _T("]");
			}
		}

		m_wndStatusBar.SetPaneText(1,sText);
		/*************************************<>************************************[12/1/2017 ConanSteve]*/
		g_pView->m_pdlgRunning->m_pdlgHisErrorInfo->UpdateDefectList();
		break;
	}
	case ID_EVENT_UPDATE_CHECK_SPEED://**[12/28/2017 ConanSteve]**:  更新检测速度
	{
		int iCheckSpeedTotle{ 0 };//**[12/28/2017 ConanSteve]**:  所有流水线的检测速度之和
		CString sInfo{ _T("") };
		for (int i = 0; i < m_iAssemblyLineNum; ++i)
		{
			if (m_arr_queCheckCntPerInterval[i].size() > 60)
			{
				m_arr_queCheckCntPerInterval[i].pop();
			}
			m_arr_queCheckCntPerInterval[i].push(m_arr_iCheckCntOfAssembly[i]);
			//**[12/28/2017 ConanSteve]**:  
			m_arr_iCheckSpeedOfAssembly[i] = (m_arr_iCheckCntOfAssembly[i] - m_arr_queCheckCntPerInterval[i].front())*((m_arr_queCheckCntPerInterval[i].size()-1)* m_iCheckSpeedUpdateInterval) / 60000;
			sInfo.Format(_T("%s%d%s: %d%s"), tr("流水线"), i + 1, tr("检测速度"), m_arr_iCheckSpeedOfAssembly[i], tr("个/分"));
			g_pView->m_pdlgRunning->SetDlgItemText(IDC_BUTTON_LABEL_CHECK_SPEED_ASSEMBLY+i, sInfo);
			iCheckSpeedTotle += m_arr_iCheckSpeedOfAssembly[i];
		}
		sInfo.Format(_T("%s: %d%s"), tr("检测速度"),iCheckSpeedTotle, tr("个/分"));
		g_pView->m_pdlgRunning->SetDlgItemText(IDC_BUTTON_LABEL_CHECK_SPEED, sInfo);
		break;
	}
	case ID_EVENT_START_GRABBER:
	{
		for (int i = 0; i < m_iGrabberNum; ++i)
		{
			if (i == 0)
				continue;
			if (!StartCameraGrab(i))
			{
				KillTimer(ID_EVENT_START_GRABBER);
				return;
			}
		}
		KillTimer(ID_EVENT_START_GRABBER);
		break;
	}
	default:
			break;
	}
	CFrameWnd::OnTimer(nIDEvent);
}


bool CMainFrame::ResizeLayout()
{
	/*************************************<调整图像窗口>************************************[12/27/2017 ConanSteve]*/
	if (NULL != g_pFrame&&NULL!=g_pView)
	{
		CRect rectDlgRunning;
		g_pView->m_pdlgRunning->GetClientRect(rectDlgRunning);
		CString sPathFileOfUICfg{ _T("") };
		sPathFileOfUICfg.Format(_T("%s_%d_%d.ini"), CExFn::GetCurrentAppPath() + _T("\\Config\\UIConfig"), m_cxScreenSize, m_cyScreenSize);
		CINICfg iniCfgUI(sPathFileOfUICfg);
		int iWindowLayoutType = iniCfgUI.GetIntEx(_T("基础参数"), _T("图像窗口布局类型"), 0, _T("0:是自适应, 1是固定位置"));
		if (1 == iWindowLayoutType)//**[12/27/2017 ConanSteve]**:  固定位置
		{
			for (int i = 0; i < m_iGrabberNum; ++i)
			{
				CString sModuleName{ _T("") };
				sModuleName.Format(_T("CameraImageParameters%d"), i + 1);
				g_pView->m_pdlgRunning->m_p_rectCamImg[i].left = iniCfgUI.GetIntEx(sModuleName, _T("Left"), 0);
				g_pView->m_pdlgRunning->m_p_rectCamImg[i].top = iniCfgUI.GetIntEx(sModuleName, _T("Top"), 0);
				g_pView->m_pdlgRunning->m_p_rectCamImg[i].right = iniCfgUI.GetIntEx(sModuleName, _T("Width"), 0) + g_pView->m_pdlgRunning->m_p_rectCamImg[i].left;
				g_pView->m_pdlgRunning->m_p_rectCamImg[i].bottom = iniCfgUI.GetIntEx(sModuleName, _T("Height"), 0) + g_pView->m_pdlgRunning->m_p_rectCamImg[i].top;

				sModuleName.Format(_T("AlgorithmImageParameters%d"), i + 1);
				g_pView->m_pdlgRunning->m_p_rectAlgImg[i].left = iniCfgUI.GetIntEx(sModuleName, _T("Left"), 0);
				g_pView->m_pdlgRunning->m_p_rectAlgImg[i].top = iniCfgUI.GetIntEx(sModuleName, _T("Top"), 0);
				g_pView->m_pdlgRunning->m_p_rectAlgImg[i].right = iniCfgUI.GetIntEx(sModuleName, _T("Width"), 0) + g_pView->m_pdlgRunning->m_p_rectAlgImg[i].left;
				g_pView->m_pdlgRunning->m_p_rectAlgImg[i].bottom = iniCfgUI.GetIntEx(sModuleName, _T("Height"), 0) + g_pView->m_pdlgRunning->m_p_rectAlgImg[i].top;
			}
		}
		else if (0 == iWindowLayoutType)//**[12/27/2017 ConanSteve]**:  自适应
		{
			//**[12/27/2017 ConanSteve]**:  系统图像
			int iGap_X = 5, iGap_Y = 5;//**[12/27/2017 ConanSteve]**:  相邻窗口之间的距离
			int iCnt_X = iniCfgUI.GetIntEx(_T("系统图像窗口"), _T("X轴最大个数"), int(sqrt(m_iGrabberNum)+1));
			int iCnt_Y = iniCfgUI.GetIntEx(_T("系统图像窗口"), _T("Y轴最大个数"), int(sqrt(m_iGrabberNum) + 1));
			int iWindowWidth = iniCfgUI.GetIntEx(_T("系统图像窗口"), _T("X轴宽度"), rectDlgRunning.Width());
			int iWindowHeight = iniCfgUI.GetIntEx(_T("系统图像窗口"), _T("Y轴高度"), rectDlgRunning.Height()/3);
			int iWindowLeft = iniCfgUI.GetIntEx(_T("系统图像窗口"), _T("X轴Left"), 0);
			int iWindowTop = iniCfgUI.GetIntEx(_T("系统图像窗口"), _T("Y轴Top"), 0);
			int iBaseCoordination = iniCfgUI.GetIntEx(_T("系统图像窗口"), _T("基准轴"), 0, _T(" 0表示基本轴为水平， 1为垂直"));
			CExFn::AutoLayoutRect(CRect(iWindowLeft, iWindowTop, iWindowLeft + iWindowWidth, iWindowTop + iWindowHeight),
				iCnt_X, iCnt_Y, iGap_X, iGap_Y, m_iGrabberNum, iBaseCoordination, g_pView->m_pdlgRunning->m_p_rectCamImg.get());
			//**[12/27/2017 ConanSteve]**:  算法图像
			iCnt_X = iniCfgUI.GetIntEx(_T("算法图像窗口"), _T("X轴最大个数"), int(sqrt(m_iGrabberNum) + 1));
			iCnt_Y = iniCfgUI.GetIntEx(_T("算法图像窗口"), _T("Y轴最大个数"), int(sqrt(m_iGrabberNum) + 1));
			iWindowWidth = iniCfgUI.GetIntEx(_T("算法图像窗口"), _T("X轴宽度"), rectDlgRunning.Width());
			iWindowHeight = iniCfgUI.GetIntEx(_T("算法图像窗口"), _T("Y轴高度"), rectDlgRunning.Height()/3);
			iWindowLeft = iniCfgUI.GetIntEx(_T("算法图像窗口"), _T("X轴Left"), 0);
			iWindowTop = iniCfgUI.GetIntEx(_T("算法图像窗口"), _T("Y轴Top"), rectDlgRunning.Height() / 3);
			iBaseCoordination = iniCfgUI.GetIntEx(_T("算法图像窗口"), _T("基准轴"), 0, _T(" 0表示基本轴为水平， 1为垂直"));
			CExFn::AutoLayoutRect(CRect(iWindowLeft, iWindowTop, iWindowLeft + iWindowWidth, iWindowTop + iWindowHeight),
				iCnt_X, iCnt_Y, iGap_X, iGap_Y, m_iGrabberNum, iBaseCoordination, g_pView->m_pdlgRunning->m_p_rectAlgImg.get());

		}
		/*************************************<根据相机尺寸调整布局>************************************[12/27/2017 ConanSteve]*/
		for (int i = 0; i < m_iGrabberNum; ++i)
		{
			if (m_bEnableInitGrabber)
			{
				int iPicHeight = m_arr_sCameraImageInfo[i].m_iCameraImageHeight;
				int iPicWidth = m_arr_sCameraImageInfo[i].m_iCameraImageWidth;
				g_pView->m_pdlgRunning->m_p_rectCamImg[i] = CExFn::AutoResizeRect(iPicWidth, iPicHeight, g_pView->m_pdlgRunning->m_p_rectCamImg[i]);
				g_pView->m_pdlgRunning->m_p_rectAlgImg[i] = CExFn::AutoResizeRect(iPicWidth, iPicHeight, g_pView->m_pdlgRunning->m_p_rectAlgImg[i]);
			}
			g_pView->m_pdlgRunning->m_pStaticCamImg[i].MoveWindow(g_pView->m_pdlgRunning->m_p_rectCamImg[i]);
			g_pView->m_pdlgRunning->m_pStaticAlgImg[i].MoveWindow(g_pView->m_pdlgRunning->m_p_rectAlgImg[i]);
		}
	}
	/*************************************<调整按钮和信息显示>************************************[12/27/2017 ConanSteve]*/
	if (NULL != g_pFrame)
	{
		g_pView->m_pdlgRunning->ResizeLayout();
	}
	return true;
}


bool CMainFrame::ResetSystem(int iType)
{
	if (1 == (iType & 1))//**[12/28/2017 ConanSteve]**:  重置所有计数
	{
		//**[1/5/2018 ConanSteve]**:  光电
		fill(m_arr_iPECount.begin(), m_arr_iPECount.end(), 0);
		//**[12/13/2017 ConanSteve]**:  相机组
		fill(m_arr_iGrabCallbackCnt.begin(), m_arr_iGrabCallbackCnt.end(), 0);
		fill(m_arr_iLastHardImageCountOfGrabInRegister.begin(), m_arr_iLastHardImageCountOfGrabInRegister.end(), 0);
		fill(m_arr_iHardImageCountOfGrabInRegister.begin(), m_arr_iHardImageCountOfGrabInRegister.end(), 0);
		fill(m_arr_iLostFrameEachGrab.begin(), m_arr_iLostFrameEachGrab.end(), 0);
		fill(m_arr_iCurSmallCntEachGrab.begin(), m_arr_iCurSmallCntEachGrab.end(), 0);

		//**[12/13/2017 ConanSteve]**:  检测组
		fill(m_arr_iCheckCntOfCheckGroup.begin(), m_arr_iCheckCntOfCheckGroup.end(), 0);
		fill(m_arr_iGoodCntOfCheckGroup.begin(), m_arr_iGoodCntOfCheckGroup.end(), 0);
		fill(m_arr_iBadCntOfCheckGroup.begin(), m_arr_iBadCntOfCheckGroup.end(), 0);
		fill(m_arr_iAlgExceptionCntOfCheckGroup.begin(), m_arr_iAlgExceptionCntOfCheckGroup.end(), 0);

		//**[12/13/2017 ConanSteve]**:  剔废组
		fill(m_arr_sKickCntInfoOfKickQueue.begin(), m_arr_sKickCntInfoOfKickQueue.end(), s_KickCountInfo());
		fill(m_arr_iGoodCntOfKickQueue.begin(), m_arr_iGoodCntOfKickQueue.end(), 0);
		//**[12/28/2017 ConanSteve]**:  流水线组
		fill(m_arr_iCheckCntOfAssembly.begin(), m_arr_iCheckCntOfAssembly.end(), 0);
		//**[12/5/2017 ConanSteve]**:  重置缺陷描述统计
		for (int i = 0; i < m_iCheckGroupNum; ++i)
		{
			for (auto itera = m_arr_mapDefectStatistics[i].begin(); itera != m_arr_mapDefectStatistics[i].end(); ++itera)
			{
				itera->second.iCnt = 0;
			}
		}
		for (int i=0; i<m_iAssemblyLineNum;++i)
		{
			while (!m_arr_queCheckCntPerInterval[i].empty())//**[12/28/2017 ConanSteve]**:  弹出所有数据
				m_arr_queCheckCntPerInterval[i].pop();
			for (int j = 0; j < 60; ++j)
			{
				m_arr_queCheckCntPerInterval[i].push(0);
			}
		}
	}
	//**[12/13/2017 ConanSteve]**:  重置接口卡
	if (2 == (iType & 2))
	{
		for (int i = 0; i < m_iIOCardNum; ++i)
		{
			m_arr_pIOCard[i]->ResetIOCard();
		}
	}
	return true;
}


bool CMainFrame::OnBtnClickOpenIOCardTools()
{
	if (m_bEnableInitIOCard)//**[12/8/2017 ConanSteve]**:  如果打开了接口卡
	{
		for (int i = 0; i < m_iIOCardNum; ++i)
		{
			m_arr_pIOCard[i]->OpenTools(this);
		}
	}
	return true;
}

inline bool CMainFrame::StartAllThread()
{
	CWinThread* pTemp = NULL;
	// 获取图像号
	pTemp = NULL;
	pTemp = AfxBeginThread((AFX_THREADPROC)GetIOCardImageNumThread, this);
	if (NULL == pTemp)
		return false;
	pTemp = AfxBeginThread((AFX_THREADPROC)WaitLoadModelThread, this);
	if (NULL == pTemp)
		return false;
	return true;
}

DWORD CMainFrame::GetIOCardImageNumThread(LPVOID lpParam)
{
	CMainFrame* This = (CMainFrame*)lpParam;
	DWORD dRet = This->GetIOCardImageNumThreadFunc();
	return dRet;
}

DWORD CMainFrame::GetIOCardImageNumThreadFunc()
{
	//if(m_bInitIOCardSuccess)
	{
		while (!bExit)//**[12/13/2017 ConanSteve]**:  没有退出系统
		{
			if (m_bEnableInitIOCard)//**[12/13/2017 ConanSteve]**:  如果正在检测，且初始化了接口卡
			{
				GetCount_Guangdian();

				GetCount_Tifei();

				GetCount_TuXiangHao();
			}
			Sleep(1);
		}
	}
	return 0;
}

DWORD CMainFrame::WaitLoadModelThread(LPVOID lpParam)
{
	CMainFrame* This = (CMainFrame*)lpParam;
	DWORD dRet = This->WaitLoadModelThreadFunc();
	return dRet;
}

DWORD CMainFrame::WaitLoadModelThreadFunc()
{
	m_hEvent_DemoMessage_SendCommandOfLoadModel = m_hEvent_DemoMessage_ReceiveSuccessOfLoadModel = m_hEvent_DemoMessage_ReveiveFailureOfLoadModel = NULL;
	m_hEvent_DemoMessage_SendCommandOfLoadModel = CreateEvent(NULL, FALSE, FALSE, TEXT("DemoMessage_SendCommandOfLoadModel"));
	m_hEvent_DemoMessage_ReceiveSuccessOfLoadModel = CreateEvent(NULL, FALSE, FALSE, TEXT("DemoMessage_ReceiveSuccessOfLoadModel"));
	m_hEvent_DemoMessage_ReveiveFailureOfLoadModel = CreateEvent(NULL, FALSE, FALSE, TEXT("DemoMessage_ReveiveFailureOfLoadModel"));
	
	if (NULL!= m_hEvent_DemoMessage_ReveiveFailureOfLoadModel)
	{
		while (!bExit)
		{
			DWORD dwRet = WaitForSingleObject(m_hEvent_DemoMessage_SendCommandOfLoadModel, 100);
			switch (dwRet)
			{
			case WAIT_OBJECT_0:
				if (LoadModel(m_sModelName).bRet)
					SetEvent(m_hEvent_DemoMessage_ReceiveSuccessOfLoadModel);
				else
					SetEvent(m_hEvent_DemoMessage_ReveiveFailureOfLoadModel);
				break;
			default:
				break;
			}
		}
		CloseHandle(m_hEvent_DemoMessage_SendCommandOfLoadModel);
		CloseHandle(m_hEvent_DemoMessage_ReceiveSuccessOfLoadModel);
		CloseHandle(m_hEvent_DemoMessage_ReveiveFailureOfLoadModel);
	}
	return 0;
}

BOOL CMainFrame::GetCount_Guangdian()
{
	for (int i = 0; i < m_iPENum; ++i)
	{
		for (int j = 0; j < m_iIOCardNum; ++j)
			m_arr_pIOCard[j]->GetInputIndex(i, m_arr_iPECount[i]);
	}
	return 0;
}

BOOL CMainFrame::GetCount_Tifei()
{
	for (int i = 0; i < m_iKickQueueNum; ++i)
	{
		for (int j = 0; j < m_iIOCardNum; ++j)
			m_arr_pIOCard[j]->GetKickCountInfo(i, m_arr_sKickCntInfoOfKickQueue[i]);
	}
	return 0;
}

BOOL CMainFrame::GetCount_TuXiangHao()
{
	m_csGetCount_TuXiangHao.Lock();
	for (int i = 0; i < m_iGrabberNum; ++i)
	{
		for (int j = 0; j < m_iIOCardNum; ++j)
			m_arr_pIOCard[j]->GetGrabImgSN(i, m_arr_iHardImageCountOfGrabInRegister[i]);
		if (m_arr_iHardImageCountOfGrabInRegister[i] != m_arr_iLastHardImageCountOfGrabInRegister[i])
		{
			m_arr_iLastHardImageCountOfGrabInRegister[i] = m_arr_iHardImageCountOfGrabInRegister[i];
			CString sInfo{ _T("") };
			sInfo.Format(_T("[当前图像号:%8d][上一图像号:%8d]"), m_arr_iHardImageCountOfGrabInRegister[i], m_arr_iLastHardImageCountOfGrabInRegister[i]);
			m_p_fileImgCntOfGrabInRegister[i].Write(sInfo);
		}
	}
	m_csGetCount_TuXiangHao.Unlock();
	return 0;
}


bool CMainFrame::OnBtnClickOpenDefectStatistics()
{
	CDlgDefectStatistics dlg;
	dlg.DoModal();
	return true;
}


BOOL CMainFrame::StartCameraGrab(int iGrabSN)
{
	BOOL bRet = FALSE;
	bRet = m_arr_pGrabber[iGrabSN]->StartGrab();
	if (FALSE == bRet)
	{
		CString sInfo{ _T() };
		sInfo.Format(_T("打开相机%d失败!"), iGrabSN + 1);
		AfxMessageBox(sInfo);
		return FALSE;
	}
	return TRUE;
}


CRetInfo CMainFrame::LoadModel(CString sModelName)
{
	/*************************************<加载模板>************************************[11/24/2017 ConanSteve]*/
	s_Status sStatus;
	CRetInfo ret;
	s_AlgModelInput sAlgModel;
	s_DemoAlgSetParam sAlgSet;
	CString sInfo{ _T("") };
	m_fileDebugInfo.Write(_T("开始加载模板"));
	for (int i = 0; i < m_iCheckGroupNum; i++)
	{
		sAlgSet.bTopWin = TRUE;
		sAlgSet.pParent = NULL;
		sAlgSet.iCheckSN = i;
		switch (m_eProductLine)
		{
		case e_ProductLine_NO:
			break;
		case e_ProductLine_General:
		case e_ProductLine_LableCheck:
		{
			memcpy_s(sAlgSet.chModelName, MAX_PATH, sModelName.GetBuffer(0), sModelName.GetLength());
		}
		break;
		case e_ProductLine_Stopper:
			break;
		case e_ProductLine_LampInsp:
		{
			memcpy_s(sAlgSet.chModelName, MAX_PATH, m_sPathOfAlgModelInfo.GetBuffer(0), m_sPathOfAlgModelInfo.GetLength());
		}
		break;
		default:
			break;
		}

		sAlgModel.iCheckSN = i;
		sAlgModel.iOperSig = e_AlgModelInputOper_LoadModel;
		sAlgModel.pParam = &sAlgSet;
		if (m_bEnableInitMultiObjectAndCheck)
		{
			sInfo.Format(_T("开始加载工位%d模板"), i + 1);
			m_fileDebugInfo.Write(sInfo);
			sStatus = m_multiObject.Set(sAlgModel);
			sInfo.Format(_T("完成加载工位%d模板"), i + 1);
			m_fileDebugInfo.Write(sInfo);
			if (STATUS_NOR != sStatus.iStatusType)
			{
				ret.bRet = false;
				ret.sErrorInfo = sStatus.pErrorInfo;
				return ret;
			}
		}
	}
	m_fileDebugInfo.Write(_T("完成加载模板"));
	ret.bRet = true;
	return ret;
}

BOOL CMainFrame::SetLutTable(double dRatioR, double dRatioG, double dRatioB)
{
	if (dRatioR <= 0)
		return FALSE;
	if (dRatioG <= 0)
		return FALSE;
	if (dRatioB <= 0)
		return FALSE;

	for (int i = 0; i < 256; i++)
	{
		m_pLutR[i] = min((int)(i*dRatioR), 255);
		m_pLutG[i] = min((int)(i*dRatioG), 255);
		m_pLutB[i] = min((int)(i*dRatioB), 255);
	}

	return TRUE;
}

int CMainFrame::MirrorSingleImage(BYTE* pImageBuff, int lImageWidth, int lImageHeight, int lImageBitCount, e_ShowModeFlip  eShowMode) //只有水平和垂直镜像
{
	if (pImageBuff == NULL)
	{
		return ImageBufferError;
	}
	if (lImageWidth < 0 || lImageHeight < 0 || lImageBitCount < 0)
	{
		return ImageWHBITError;
	}

	if (lImageBitCount == 8)
	{
		lImageWidth = WIDTHBYTES(lImageWidth * 8);
		if (eShowMode == FlipHorizintal) //**[1/3/2018 ConanSteve]**:	 水平翻转
		{
			BYTE  tem;
			for (int i = 0; i < lImageHeight; i++)
			{
				for (int j = 0; j < lImageWidth / 2; j++)
				{
					tem = *(pImageBuff + i*lImageWidth + j);
					*(pImageBuff + i*lImageWidth + j) = *(pImageBuff + i*lImageWidth + lImageWidth - 1 - j);
					*(pImageBuff + i*lImageWidth + lImageWidth - 1 - j) = tem;
				}
			}
			return TRUE;
		}
		if (eShowMode == FlipVertical) //**[1/3/2018 ConanSteve]**:	 垂直翻转
		{
			BYTE  tem;
			for (int i = 0; i < lImageHeight / 2; i++)
			{
				for (int j = 0; j < lImageWidth; j++)
				{
					tem = *(pImageBuff + i*lImageWidth + j);
					*(pImageBuff + i*lImageWidth + j) = *(pImageBuff + (lImageHeight - 1 - i)*lImageWidth + j);
					*(pImageBuff + (lImageHeight - 1 - i)*lImageWidth + j) = tem;
				}
			}
			return TRUE;
		}
	}
	if (lImageBitCount == 24)//24位镜像
	{
		lImageWidth = WIDTHBYTES(lImageWidth * 8);

		if (eShowMode == FlipHorizintal) //**[1/3/2018 ConanSteve]**:	 水平翻转
		{
			BYTE  tem1, tem2, tem3;
			for (int i = 0; i < lImageHeight; i++)
			{
				for (int j = 0; j < lImageWidth / 2; j++)
				{
					tem1 = *(pImageBuff + i*lImageWidth * 3 + 3 * j);
					tem2 = *(pImageBuff + i*lImageWidth * 3 + 3 * j + 1);
					tem3 = *(pImageBuff + i*lImageWidth * 3 + 3 * j + 2);
					*(pImageBuff + i*lImageWidth * 3 + 3 * j) = *(pImageBuff + i*lImageWidth * 3 + 3 * (lImageWidth - 1 - j));
					*(pImageBuff + i*lImageWidth * 3 + 3 * j + 1) = *(pImageBuff + i*lImageWidth * 3 + 3 * (lImageWidth - 1 - j) + 1);
					*(pImageBuff + i*lImageWidth * 3 + 3 * j + 2) = *(pImageBuff + i*lImageWidth * 3 + 3 * (lImageWidth - 1 - j) + 2);
					*(pImageBuff + i*lImageWidth * 3 + 3 * (lImageWidth - 1 - j)) = tem1;
					*(pImageBuff + i*lImageWidth * 3 + 3 * (lImageWidth - 1 - j) + 1) = tem2;
					*(pImageBuff + i*lImageWidth * 3 + 3 * (lImageWidth - 1 - j) + 2) = tem3;
				}
			}
			return TRUE;
		}
		if (eShowMode == FlipVertical) //**[1/3/2018 ConanSteve]**:	 垂直翻转
		{
			BYTE  tem1, tem2, tem3;
			for (int i = 0; i < lImageHeight / 2; i++)
			{
				for (int j = 0; j < lImageWidth; j++)
				{
					tem1 = *(pImageBuff + i*lImageWidth * 3 + 3 * j);
					tem2 = *(pImageBuff + i*lImageWidth * 3 + 3 * j + 1);
					tem3 = *(pImageBuff + i*lImageWidth * 3 + 3 * j + 2);
					*(pImageBuff + i*lImageWidth * 3 + 3 * j) = *(pImageBuff + (lImageHeight - 1 - i)*lImageWidth * 3 + 3 * j);
					*(pImageBuff + i*lImageWidth * 3 + 3 * j + 1) = *(pImageBuff + (lImageHeight - 1 - i)*lImageWidth * 3 + 3 * j + 1);
					*(pImageBuff + i*lImageWidth * 3 + 3 * j + 2) = *(pImageBuff + (lImageHeight - 1 - i)*lImageWidth * 3 + 3 * j + 2);
					*(pImageBuff + (lImageHeight - 1 - i)*lImageWidth * 3 + 3 * j) = tem1;
					*(pImageBuff + (lImageHeight - 1 - i)*lImageWidth * 3 + 3 * j + 1) = tem2;
					*(pImageBuff + (lImageHeight - 1 - i)*lImageWidth * 3 + 3 * j + 2) = tem3;
				}
			}
			return TRUE;
		}
	}
	return TRUE;
}


bool CMainFrame::OnBtnClickOpenDataStatistics()
{
	CDlgDataStatistics dlg;
	dlg.DoModal();
	return false;
}
