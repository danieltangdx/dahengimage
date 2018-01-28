// DemoProjectDlg.cpp : implementation file

#include "stdafx.h"
#include "DemoProject.h"
#include "DemoProjectDlg.h"
#include "DlgExit.h"
#include "DlgLangSel.h"
#include "DlgInspSet.h"
#include "DlgNewModel.h"
#include "DlgParkour.h"
#include "UIDlgStart.h"
#include "MyProgressCtrl.h"
#include "CHINESE.H"
#include "winver.h"
#include <dbt.h>
#include <afxmt.h>
#include <io.h>
#include "DHAlgInterface.h"
#include <sys/stat.h>

#ifdef _DEBUG
	#define new DEBUG_NEW
	#undef THIS_FILE
	static char THIS_FILE[] = __FILE__;
#endif

#define EVENT_SEARCHBM			1000
#define EVENT_MOVE_SIDEBAR		1001
#define PATH_DISPLAY_PER_LINE	33
#define IDM_LANGUAGE_SETTINGS	0x0020
#define IDM_WINDOW_SIZING		0x0030


//#define NEEDLOGVERSION
BOOL g_bSaveEnabled = TRUE;


#ifdef COMMON_DEMO_VERSION
CInspBase** g_ppCheck = NULL;
#endif

#ifdef CAPSULE_DEMO_VERSION

#include "..\\Inc\\Capsule\\Ftop.h"
#pragma comment(lib, "..\\Lib\\Capsule\\CapsuleFtop.lib")

#include "..\\Inc\\Capsule\\Fbody.h"
#pragma comment(lib, "..\\Lib\\Capsule\\CapsuleFbody.lib")

#include "..\\Inc\\Capsule\\Bbody.h"
#pragma comment(lib, "..\\Lib\\Capsule\\CapsuleBbody.lib")

void* g_ppCheck[5] = {NULL};

#endif


typedef struct _DEMOParkourStruct
{
	int Interval;
	BOOL FromSel, EndlessLoop, StopBad, StopGood, StopEmp, StopExp;
	CDHFileBox* pList;
	int Workstation;
}DEMOPARKOURDATA, *PDEMOPARKOURDATA;

CFile g_file;

CCriticalSection g_CS1;
CProgressCtrl* g_pProgCtrl = NULL;
DEMOPARKOURDATA g_DemoParkData;
BOOL g_bParkourRunning = FALSE;
int	g_nPainted = 0;
extern HMODULE g_hLanguage;
extern pFunc_GetTextFromLPK g_pGetTextFromLPK;
extern CUIDlgStart* theStart;
extern int g_nLangType;
extern CString g_strAppDir, g_strLogDir,g_strPresDir,g_strConfigDir,g_strLanguageDir,g_strCurSet, g_strIniPath, g_strAllModelPath, g_strCheckIni, g_strLanguage,g_strCheckPath;

extern CDemoProjectApp theApp;

UINT ThreadFuncParkour(LPVOID);

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CStaticEx	m_DemoVer;
	CStaticEx	m_Comp;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	CString m_strDemoVer;
protected:
	CBrush m_bkBrush;
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnAbout();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
	m_bkBrush.CreateSolidBrush(RGB(255,255,255));
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, IDC_STATIC_DEMOV, m_DemoVer);
	DDX_Control(pDX, IDC_STATIC_COMP, m_Comp);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDOK_ABOUT, OnAbout)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//Demo
	char chText[256];
	g_pGetTextFromLPK(chText, 20, m_strDemoVer.GetBuffer(0));
	GetDlgItem(IDC_STATIC_DEMOV)->SetWindowText(chText);

	//Title
	g_pGetTextFromLPK(chText, 21);
	SetWindowText(chText);

	//Company
	g_pGetTextFromLPK(chText, 22);
	GetDlgItem(IDC_STATIC_COMP)->SetWindowText(chText);

	//Confirm
	g_pGetTextFromLPK(chText, 1);
	GetDlgItem(IDOK_ABOUT)->SetWindowText(chText);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnAbout() 
{
	// TODO: Add your control notification handler code here
	OnOK();
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if (nCtlColor == CTLCOLOR_DLG)
		return m_bkBrush;
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
/////////////////////////////////////////////////////////////////////////////
// CDemoProjectDlg dialog

CDemoProjectDlg::CDemoProjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDemoProjectDlg::IDD, pParent)
	, m_iShowModelLog(0)
{
	//{{AFX_DATA_INIT(CDemoProjectDlg)
	m_strModelNameUsing = _T("");
	m_strNewModelName = _T("");
	m_strCModel = _T("");
	m_bNewInProcess = false;
	m_bShowSide = FALSE;
	m_strVersion = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_DefaultTab = -1;
	m_bModelChanged = false;
	m_bAskingSystem = false;

	m_bStopperMode = FALSE;
	m_nStopperType = -1;

	g_pProgCtrl = NULL;
	m_nbrush = 0;
	
	m_bMouseIsDoing = false;
	m_bNewCreateModel = false;

	m_nInitTopIndex = NULL;
	m_plDisk = NULL;
	m_plFile = NULL;
	m_nNumWS = -1;

	m_nLastPage = -1;
	m_bIsWaiting = FALSE;

	m_bInitSuccess = TRUE;
	m_bChangeType = false;
	m_bFirstCreate = TRUE;

	m_bMoveSize = FALSE;
	m_bF11Pressed = FALSE;
	m_bLBtnDown = FALSE;
	m_pMainCtrlPR = NULL;
	m_pSideCtrlPR = NULL;
	m_bSidebarHide = FALSE;
	m_nSideWidthTemp = 0;
	m_bNeedRemap = FALSE;

	m_bCtrlIsDown = FALSE;

	if(_access("D:\\tmp.vbs", 0) == 0)
		CGeneralFunc_Yang::DeleteProtectionFile("D:\\tmp.vbs");
}

void CDemoProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDemoProjectDlg)
	DDX_Control(pDX, IDC_BTN_SAVESINGLEMODEL, m_SAVESINGLE);
	DDX_Control(pDX, IDC_LIST_RESULT, m_lResult);
	DDX_Control(pDX, IDC_STATIC_NEWNAME, m_NewName);
	DDX_Control(pDX, IDC_STATIC_MLIST, m_MList);
	DDX_Control(pDX, IDC_MODELLIST, m_lModelNames);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
	DDX_Text(pDX, IDC_EDIT1, m_strNewModelName);
	DDX_Text(pDX, IDC_STATIC_CMODEL, m_strCModel);
	DDX_Text(pDX, IDC_STATIC_VERSION, m_strVersion);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_EDIT_ModelLog, m_editModelLog);
	DDX_Check(pDX, IDC_CHECK_ShowModelLog, m_iShowModelLog);
}

BEGIN_MESSAGE_MAP(CDemoProjectDlg, CDialog)
	//{{AFX_MSG_MAP(CDemoProjectDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()	
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab)
	ON_BN_CLICKED(IDC_BTN_SAVEMODEL, OnBtnSave)
	ON_BN_CLICKED(IDC_BTN_MLIST, OnButtonMlist)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_NEWMODEL, OnBtnCreate)
	ON_BN_CLICKED(IDC_BTN_DELETE, OnBtnDelete)
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_SETD, OnBtnSetd)
	ON_BN_CLICKED(IDC_BTN_CLRST, OnBtnClrst)
	ON_BN_CLICKED(IDC_BTN_ERRIMG, OnBtnErrimg)
	ON_BN_CLICKED(IDC_BTN_GRABIMG, OnBtnGrabimg)
	ON_BN_CLICKED(IDC_BTN_CAPIMG, OnBtnCapimg)
	ON_BN_CLICKED(IDC_BTN_CTEST, OnBtnImgTest)
	ON_BN_CLICKED(IDC_BTN_UPDMDL, OnBtnUpdmdl)
	ON_WM_MOUSEWHEEL()
	ON_BN_CLICKED(IDC_BTN_SAVESINGLEMODEL, OnBtnSaveSingle)
	ON_BN_CLICKED(IDC_BTN_PARKOUR, OnBtnParkour)
	ON_BN_CLICKED(IDC_BTN_LANGUAGE, OnBtnLanguage)
	ON_BN_CLICKED(IDC_BTN_INSP, OnBtnInsp)
	ON_BN_CLICKED(IDC_BTN_EXIT2, OnBtnExit2)
	ON_CBN_SELCHANGE(IDC_COMBO_STOPPERTYPE, OnSelchangeComboStoppertype)
	//}}AFX_MSG_MAP
	ON_MESSAGE(DHLISTMSG, OnMsgFromListBox)
	ON_MESSAGE(DEMO_APP_LOADM, OnMsgFromSysReadModel)
	ON_WM_DEVICECHANGE()
	ON_MESSAGE(WM_BM_COPIED, OnMsgFromBackupM)
	ON_MESSAGE(THREAD_CHECK_IMAGE, OnMsgFromInternalThread)
	ON_MESSAGE(WM_ENTERSIZEMOVE, OnEnterSizeMove)
	ON_MESSAGE(WM_EXITSIZEMOVE, OnExitSizeMove)
	ON_MESSAGE(MSG_APP_LOGIN, OnAppLogin)
	ON_EN_CHANGE(IDC_EDIT_ModelLog, &CDemoProjectDlg::OnEnChangeEditModellog)
	ON_LBN_SELCHANGE(IDC_MODELLIST, &CDemoProjectDlg::OnLbnSelchangeModellist)
	ON_BN_CLICKED(IDC_CHECK_ShowModelLog, &CDemoProjectDlg::OnBnClickedCheckShowmodellog)
	ON_BN_CLICKED(IDC_BUTTON_FoldPage, &OnBnClickedButtonFoldpage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoProjectDlg message handlers

BOOL CDemoProjectDlg::OnInitDialog()
{
	m_bOpenFileOfErrorLog = TRUE== m_fileErrorLog.Open(CExFn::GetCurrentAppPath() + TEXT("\\ErrorLog.txt"), CFile::modeCreate | CFile::shareDenyNone | CFile::modeWrite);
	ReadSystemConfig();
	CDialog::OnInitDialog();
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	this->ShowWindow(SW_HIDE);
//	this->ShowWindow(SW_SHOW);
	ModifyStyle(WS_THICKFRAME, 0); // WS_THICKFRAME 的意思是具有可调边框  修改风格？ 

	//给后台运行程序发送消息
// 	CWnd *pBMWnd = CWnd::FindWindow(NULL, "BackupManager");
// 	if (pBMWnd)
// 		pBMWnd->PostMessage(WM_DEMO_ONOFF, WPARAM(1), LPARAM(this));
// 	else
// 		m_uSearchBM = SetTimer(EVENT_SEARCHBM, 500, NULL);
	
	// 需要登录的版本
#ifdef NEEDLOGVERSION
	if (atoi(theApp.m_lpCmdLine) > 0)
	{
		GetDlgItem(IDC_BTN_SAVESINGLEMODEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_SAVEMODEL)->EnableWindow(TRUE);
		g_bSaveEnabled = TRUE;
	}
	else
	{
		GetDlgItem(IDC_BTN_SAVESINGLEMODEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SAVEMODEL)->EnableWindow(FALSE);
		g_bSaveEnabled = FALSE;
	}
#endif

	
	//语言界面刷入
	LoadControlStrings();
	
	//加载配置文件，初始化检测对象，加载图标，加载字体
#ifdef COMMON_DEMO_VERSION
	if(!LoadConfigs())
	{
		CDlgInspSet dlg;
		dlg.m_nPreNumWS = m_nNumWS;
		if (IDOK == dlg.DoModal())
		{
			if(!LoadConfigs())
			{			
				m_bInitSuccess = FALSE;
				return TRUE;
			}
		}
		else
		{
			m_bInitSuccess = FALSE;
			return TRUE;
		}
	}
#endif


	// 字体
	char chFont[256];
	LoadString(g_hLanguage, 5002, chFont, 256);
	m_cModelNameFont.CreatePointFont(120, chFont, NULL);
	m_cVersionFont.CreatePointFont(110, chFont, NULL);
	GetDlgItem(IDC_STATIC_CMODEL)->SetFont(&m_cModelNameFont);
	GetDlgItem(IDC_STATIC_VERSION)->SetFont(&m_cVersionFont);
	

	//默认模板
	//给系统程序发送启动完毕的消息
	{
		//先修改Title
		char chTitle[256];
		LoadString(g_hLanguage, 5001, chTitle, 256);
		SetWindowText(chTitle);
		WritePrivateProfileString(_T("系统配置"), "DemoName", chTitle, g_strCurSet.GetBuffer(0));
		
		
		LRESULT copyDataResult;
// #ifdef DEBUG
// 		m_strSystemName += _T("D");
// #endif
		m_bOpenViaSystem = false;
		CWnd *pOtherWnd = CWnd::FindWindow(NULL, m_strSystemName.GetBuffer(0));
		if (pOtherWnd != NULL)
			m_bOpenViaSystem = true;
		HANDLE hCheckSystem = OpenEvent(EVENT_ALL_ACCESS, FALSE, _T("{8D264CC6-8633-45DC-A616-4597E2BA97F6}"));
		if (hCheckSystem != NULL)
			m_bOpenViaSystem = true;
		//CWnd *pOtherWnd = CWnd::FindWindow(NULL, _T("[Guid(5FB06D01 - 839C - 4CE0 - 9072 - FAFB8E084676)]"));//**[8/28/2017 ConanSteve]**:  瑞阳定制版
		// 通过系统程序打开
		if (m_bOpenViaSystem)
		{
// 			COPYDATASTRUCT cpd;
// 			cpd.dwData = DEMO_OPEN_SUCCESS;
// 			cpd.cbData = 0;
// 			cpd.lpData = (void*)this;
// 			copyDataResult = pOtherWnd->SendMessage(WM_COPYDATA,
// 				(WPARAM)AfxGetApp()->m_pMainWnd->GetSafeHwnd(),
// 				(LPARAM)&cpd);
			//if (m_strSysModelName.GetLength() < 1)
			{
				char chBuf[256];
				GetPrivateProfileString("DEMO Controls", "DefaultModel", "", chBuf, 256, g_strIniPath.GetBuffer(0));
				m_strModelNameUsing = chBuf;
			}
// 			else
// 			{
// 				m_strModelNameUsing = m_strSysModelName;
// 			}


			m_nOpenMode = 1;
			// 判断命令行参数是否允许保存模板参数
			CString strCommand;
			strCommand = theApp.m_lpCmdLine;

		//	AfxMessageBox(strCommand);
			if(m_bEnableVerificationOfSaveModel)
			{
				if (0 == strCommand.Compare("Save:Y"))
				{
					GetDlgItem(IDC_BTN_SAVESINGLEMODEL)->EnableWindow(TRUE);
					GetDlgItem(IDC_BTN_SAVEMODEL)->EnableWindow(TRUE);
					g_bSaveEnabled = TRUE;
				}
				else if (0 == strCommand.Compare("Save:N"))
				{
					GetDlgItem(IDC_BTN_SAVESINGLEMODEL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BTN_SAVEMODEL)->EnableWindow(FALSE);
					g_bSaveEnabled = FALSE;
				}
				else
				{
					GetDlgItem(IDC_BTN_SAVESINGLEMODEL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BTN_SAVEMODEL)->EnableWindow(FALSE);
					g_bSaveEnabled = FALSE;
				}
			}
		}
		//**[8/14/2017 ConanSteve]**:  资源管理器打开Demo
		else 
		{
			TCHAR chBuf[256];
			GetPrivateProfileString(TEXT("DEMO Controls"), TEXT("DefaultModel"), TEXT("DefaultModel"), chBuf, 256, g_strIniPath.GetBuffer(0));
			m_strModelNameUsing = chBuf;
			m_nOpenMode = 0;
			GetDlgItem(IDC_BTN_UPDMDL)->EnableWindow(FALSE);
			char chFile[MAX_PATH];
			BOOL bEnable=FALSE;

 			sprintf(chFile,"C:\\windows\\dhnewbee\\alg.ver");

			if(_access(chFile,0) == 0)//如果文件存在，返回0，不存在，返回-1
			{
				struct stat buf;	
				if(stat(chFile, &buf)==0)
				{
					int a = buf.st_size;
					if(a == 43998)
						bEnable = TRUE;
					else
						bEnable = FALSE;
				}			
			}

			if(m_bEnableVerificationOfSaveModel)
			{
				if (TRUE == bEnable)
				{
					GetDlgItem(IDC_BTN_SAVESINGLEMODEL)->EnableWindow(TRUE);
					GetDlgItem(IDC_BTN_SAVEMODEL)->EnableWindow(TRUE);
					g_bSaveEnabled = TRUE;
				}
				else
				{
					GetDlgItem(IDC_BTN_SAVESINGLEMODEL)->EnableWindow(FALSE);
					GetDlgItem(IDC_BTN_SAVEMODEL)->EnableWindow(FALSE);
					g_bSaveEnabled = FALSE;
				}
			}
		}
	}

	//各种控件
	ConfigInterCtrls();
	//添加Tab页
	{
		m_TabImgList.Create(48, 30, ILC_COLOR24, 1, 1);
		CBitmap bitCamera;
		//bitCamera.LoadBitmap(IDB_BITMAP_CAMERA2);
		bitCamera.LoadBitmap(IDB_BITMAP_CAMERA);

		m_TabImgList.Add(&bitCamera, RGB(0,0,0));
		m_Tab.SetImageList(&m_TabImgList);

		char chTabTitle[256];
		for (int n=0; n<m_nNumWS; ++n)
		{
			g_pGetTextFromLPK(chTabTitle, 2, n+1);
			m_Tab.InsertItem(n, chTabTitle, 0);
		}
	}


	//初始化模板控件
	InitModel();

	UpdateData(FALSE);
	/************************************************************************/
	/*                       [5/31/2017 ConanSteve]                         */
	InitModelLog();
	/************************************************************************/
	/************************************************************************/
	/*动态修改图标                       [6/5/2017 ConanSteve]                         */
	HICON hIcon{ 0 };
	hIcon = (HICON)::LoadImage(NULL, g_strAppDir + TEXT("\\res\\Demo.ico"), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
	SetIcon(hIcon, false);
	/*************************************<创建加载模板内核对象>*************************************/
	m_hEvent_DemoMessage_SendCommandOfLoadModel = CreateEvent(NULL, FALSE, FALSE, TEXT("DemoMessage_SendCommandOfLoadModel"));
	m_hEvent_DemoMessage_ReceiveSuccessOfLoadModel = CreateEvent(NULL, FALSE, FALSE, TEXT("DemoMessage_ReceiveSuccessOfLoadModel"));
	m_hEvent_DemoMessage_ReceiveFailureOfLoadModel = CreateEvent(NULL, FALSE, FALSE, TEXT("DemoMessage_ReveiveFailureOfLoadModel"));
	if (NULL== m_hEvent_DemoMessage_SendCommandOfLoadModel||NULL== m_hEvent_DemoMessage_ReceiveSuccessOfLoadModel||NULL== m_hEvent_DemoMessage_ReceiveFailureOfLoadModel)
	{
		if(m_bOpenFileOfErrorLog)
		{m_fileErrorLog.WriteString(TEXT("创建加载模板内核事件对象失败\n"));}
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}


void CDemoProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.m_strDemoVer = m_strArrVersion.GetAt(m_strArrVersion.GetSize()-1);
		dlgAbout.DoModal();
	}
	else if ((nID & 0xFFF0) == IDM_LANGUAGE_SETTINGS)
	{
		OnBtnLanguage();
	}
	else if ((nID & 0xFFF0) == IDM_WINDOW_SIZING)
	{
		m_bMoveSize = !m_bMoveSize;

		CMenu* pSysMenu = GetSystemMenu(FALSE);
		if (pSysMenu)
		{
			UINT check_flag;
			if (m_bMoveSize)
				check_flag = MF_UNCHECKED|MF_BYCOMMAND;
			else
				check_flag = MF_CHECKED|MF_BYCOMMAND;
			pSysMenu->CheckMenuItem(IDM_WINDOW_SIZING, check_flag);
		}
#ifdef COMMON_DEMO_VERSION
		if (m_bMoveSize)
			ModifyStyle(0, WS_THICKFRAME);
		else
			ModifyStyle(WS_THICKFRAME, 0);
#endif
	}
	else if (nID == SC_CLOSE)
	{
		OnBtnExit2();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

void CDemoProjectDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CView::OnPaint() for painting messages
					   /************************************************************************/
					   /*                       [6/3/2017 ConanSteve]                         */
// 	CRect rect{ 0 };
// 	GetClientRect(&rect);
// 	CBrush brush;
// 	//brush.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
// 	brush.CreateSolidBrush(COLORREF(RGB(255, 255, 255)));
// 	dc.FillRect(&rect, &brush);
// 	brush.DeleteObject();
	/************************************************************************/

	if (g_nPainted == 0)
	{
		g_nPainted = 1;

		if (m_bInitSuccess == FALSE)
		{
			FreeMemories();
			PostMessage(WM_CLOSE);

			return;
		}

		//创建文件浏览器
		CRect rectFile, rectDisk;
		GetDlgItem(IDC_STATIC_FILE)->GetWindowRect(rectFile);	ScreenToClient(rectFile);
		GetDlgItem(IDC_STATIC_DISK)->GetWindowRect(rectDisk);	ScreenToClient(rectDisk);
		for (int n=0; n<m_nNumWS; ++n)
		{
			m_plFile[n].Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL|LBS_HASSTRINGS|LBS_NOINTEGRALHEIGHT|LBS_NOTIFY|LBS_OWNERDRAWFIXED
				, rectFile, this, 0);
			m_plFile[n].ModifyStyleEx(DWORD(0), WS_EX_CLIENTEDGE);
			
			m_plDisk[n].Create(WS_CHILD|WS_VISIBLE|WS_VSCROLL|WS_HSCROLL|LBS_HASSTRINGS|LBS_NOINTEGRALHEIGHT|LBS_NOTIFY|LBS_OWNERDRAWFIXED
				, rectDisk, this, 0);
			m_plDisk[n].ModifyStyleEx(DWORD(0), WS_EX_CLIENTEDGE);
			m_plFile[n].ShowWindow(SW_HIDE);
			m_plDisk[n].ShowWindow(SW_HIDE);
			CFont fnt;
			char chFont[256];
			LoadString(g_hLanguage, 5002, chFont, 256);
			fnt.CreatePointFont(120, chFont, NULL);
			m_plDisk[n].Init(&m_plFile[n], this, n+1, &fnt, g_nLangType);

			m_plFile[n].EnterDir(m_strListPath[n], TRUE, FALSE);
			if (m_nInitTopIndex[n]>=0 || m_nInitTopIndex[n]<m_plFile[n].GetCount())
				m_plFile[n].SetTopIndex(m_nInitTopIndex[n]);
		}

		//隐藏控件
		DisEnMdlList(0);
		
		//挪动显示页面的父窗口Static
		CRect rcTabPage, rcBTNOK, rc;
		GetDlgItem(IDC_TAB1)->GetClientRect(m_TabRect);
		GetClientRect(rcTabPage);
		rcTabPage.top += m_TabRect.Height();rcTabPage.right = m_TabRect.Width()+2;
		m_StaticRect = rcTabPage;
		GetDlgItem(IDC_BTN_MLIST)->GetWindowRect(rcTabPage);	ScreenToClient(rcTabPage);
		m_StaticRect.bottom = rcTabPage.top-1;
		GetDlgItem(IDC_STATIC_1)->MoveWindow(m_StaticRect);
		CRect rr;
		GetDlgItem(IDC_STATIC_1)->GetWindowRect(rr);

		// 创建界面
		CreateTabWins(0);

		// 搞控件
		IntegrateCtrls();
		GetWindowRect(m_WndInitSize);
#ifdef COMMON_DEMO_VERSION
		CRect rectSide, rectCWnd;
		GetClientRect(rectCWnd);
		ClientToScreen(rectCWnd);
		GetDlgItem(IDC_STATIC_BCK)->GetWindowRect(rectSide);
		m_nSideWidth = rectCWnd.right - rectSide.left;
		int nSetWndHeight = GetPrivateProfileInt("DEMO Controls", "WindowHeight", m_WndInitSize.Height(), g_strIniPath);
		int nSetWndWidth = GetPrivateProfileInt("DEMO Controls", "WindowWidth", m_WndInitSize.Width(), g_strIniPath);
		// 获取比例
		CRect rectMain;
		GetDlgItem(IDC_TAB1)->GetWindowRect(rectMain);
		GetAllRatios();
		// 到目前为止窗口还是原始大小！
		int nCenterRow = (m_WndInitSize.top+m_WndInitSize.bottom)/2.0+0.5;
		int nCenterCol = (m_WndInitSize.left+m_WndInitSize.right)/2.0+0.5;
		CRect rectNew;
		rectNew.left = nCenterCol - nSetWndWidth/2.0;
		rectNew.right= nCenterCol + nSetWndWidth/2.0;
		rectNew.top = nCenterRow - nSetWndHeight/2.0;
		rectNew.bottom = nCenterRow + nSetWndHeight/2.0;
		MoveWindow(rectNew);
		ScaleMoveWindow();
		m_nSideWidth = GetPrivateProfileInt("DEMO Controls", "SidebarWidth", m_WndInitSize.right-rectSide.left, g_strIniPath);
		m_bSidebarHide = GetPrivateProfileInt("DEMO Controls", "SidebarHide",0/* m_WndInitSize.right-rectSide.left*/, g_strIniPath);
		if (m_bSidebarHide)	//隐藏了
		{
			m_nSideWidthTemp = m_nSideWidth;
			ModifySidebarWidth(10);
			VisualSidebar();
		}
		else
		{
			ModifySidebarWidth(m_nSideWidth);
		}
#endif
		
		//载入默认工位页面
		int nDefaultTab = GetPrivateProfileInt("DEMO Controls", "DefaultTab", 0, g_strIniPath);
		if (nDefaultTab<0 || nDefaultTab>m_nNumWS-1)
			nDefaultTab = 0;		
		if (nDefaultTab == 0)
		{
			m_DefaultTab = 0;
			ShowTabWins(m_DefaultTab+1);
			char chPath[MAX_PATH];
			m_plFile[m_DefaultTab].GetCurrentPath(&chPath[0]);
			GetDlgItem(IDC_EDIT_CURPATH)->SetWindowText(&chPath[0]);
			m_nLastPage = nDefaultTab;
		}
		else
		{
			m_DefaultTab = nDefaultTab - 100;
			m_Tab.SetCurFocus(nDefaultTab);
		}


		UpdateData(FALSE);
		g_nPainted = 2;
		SetTimer(3, 300, NULL);

		ShowWindow(SW_SHOW);
	}

	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		if (m_bSidebarHide)
		{
			CPaintDC dc(this);

			CRect wndRect; GetClientRect(wndRect);
			CRect wndTab;  GetDlgItem(IDC_TAB1)->GetClientRect(wndTab);

			COLORREF bkColor = dc.GetBkColor();
			int nr = GetRValue(bkColor);
			int ng = GetGValue(bkColor);
			int nb = GetBValue(bkColor);

			CPen pen;
			pen.CreatePen(PS_SOLID, 1, RGB(nr*0.8,ng*0.8,nb*0.8));
			CPen* pOldPen =dc.SelectObject(&pen);
			dc.MoveTo(wndRect.right-5, wndTab.Height());
			dc.LineTo(wndRect.right-5, wndRect.bottom);
			pen.DeleteObject();
			pen.CreatePen(PS_SOLID, 1, RGB(min(nr*1.1, 255),min(ng*1.1, 255),min(nb*1.1, 255)));
			dc.SelectObject(&pen);
			dc.MoveTo(wndRect.right-4, wndTab.Height());
			dc.LineTo(wndRect.right-4, wndRect.bottom);
			dc.SelectObject(pOldPen);
		}

		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDemoProjectDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDemoProjectDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{	
	::SetFocus(this->m_hWnd);
	if (GetIfDrawing())
	{
		char chText[256];
		LoadString(g_hLanguage, 5020, chText, 256);
		MessageBox(chText, "DEMO", MB_ICONWARNING);
		return;
	}
	
	int nPage;
	if (m_DefaultTab >= 0)
	{
		nPage = m_Tab.GetCurSel()+1;
		ShowTabWins(nPage);
	}
	else	//初次进来 0 1 2 3 4    0 1 3 4
	{
		m_DefaultTab += 100;
		nPage = m_DefaultTab+1;
		ShowTabWins(nPage);
	}
	m_nLastPage = m_Tab.GetCurSel();

	char chPath[MAX_PATH];
	m_plFile[m_nLastPage].GetCurrentPath(&chPath[0]);
	GetDlgItem(IDC_EDIT_CURPATH)->SetWindowText(&chPath[0]);

	*pResult = 0;
}

void CDemoProjectDlg::ShowTabWins(int nWinNum)
{
	//所有窗口全部隐藏
	s_AlgModelInput sIn;
	sIn.iOperSig = -1;//-1隐藏模板窗口(外部点击tab页切换时使用)
	
	if (m_nLastPage>=0 && m_nLastPage<m_nNumWS)
	{
		sIn.iCheckSN = m_nLastPage;
#ifdef COMMON_DEMO_VERSION
		sIn.pParam = NULL;
		m_multiObjManager.Set(sIn);
#endif

		m_plFile[m_nLastPage].ShowWindow(SW_HIDE);
		m_plDisk[m_nLastPage].ShowWindow(SW_HIDE);
	}
		
	//显示对应窗口
	sIn.iOperSig = -2;//	-2显示模板窗口
#ifdef COMMON_DEMO_VERSION
	sIn.iCheckSN = nWinNum-1;
	sIn.pParam= NULL;
	m_multiObjManager.Set(sIn);
#endif

	if(m_bFileIsShow && !m_bSidebarHide)
	{
		m_plFile[nWinNum-1].EnableWindow(TRUE);
		m_plFile[nWinNum-1].ShowWindow(SW_SHOW);
		m_plDisk[nWinNum-1].EnableWindow(TRUE);
		m_plDisk[nWinNum-1].ShowWindow(SW_SHOW);
	}
	m_strVersion.Format("DLL Version:%s", m_strArrVersion.GetAt(nWinNum-1));
	UpdateData(FALSE);
}

void CDemoProjectDlg::CreateTabWins(int nWinTop)
{
	//进度条控件
	if (m_bFirstCreate)
	{
		m_bFirstCreate = FALSE;
	}
	else
	{	
		g_pProgCtrl = new CMyProgressCtrl(1, 1);
		CRect ProRect;
		ProRect.left=486;
		ProRect.right=ProRect.left+200;
		ProRect.top=330;
		ProRect.bottom=ProRect.top+30;
		g_pProgCtrl->Create(WS_CHILD|WS_BORDER, ProRect, this, 0);
		g_pProgCtrl->SetRange(0, m_nNumWS);
		g_pProgCtrl->SetStep(1);
		g_pProgCtrl->ShowWindow(SW_SHOW);
	}


	//背景Static位置
	CRect rc;
	GetDlgItem(IDC_STATIC_1)->GetWindowRect(rc);
	ScreenToClient(rc);

	//SetModel函数
	/************************************************************************/
	/* 	typedef struct _AlgModelInput
		{
			int		nOperSig;   //多对象与算法，0读参数，300将虚窗口copyWindow到实窗口
			int     iCheckSN;   //当前检测第几路。检测端传给多对象用于调用该路set。与算法无关。
	
			void*   pParam;      //检测端与算法
	}s_AlgModelInput;                                                                     */
	/************************************************************************/
	s_AlgModelInput sModelIn;
	ZeroMemory(&sModelIn, sizeof(s_AlgModelInput));

	s_Status sRst;
	ZeroMemory(&sRst,sizeof(s_Status));
	sRst.iStatusType = STATUS_NOR;

	sModelIn.iOperSig = 2;//2表示双击应用程序打开Demo窗口

	bool bEnableMixedModeOfInitAlg = 1 == GetPrivateProfileInt(TEXT("MixedModeOfInitAlg"), TEXT("EnableMixedModeOfInitAlg"), 0, CExFn::GetCurrentAppPath() + TEXT("\\Config\\AlgConfig.ini"));
	for (int n=0; n<m_nNumWS; ++n)
	{
		s_DemoAlgSetParam sDemo;
		s_Status sRst2;
		sDemo.pParent = this;
		sDemo.rect = rc;
		sDemo.bTopWin = false;
		if (nWinTop == n+1)
			sDemo.bTopWin = true;

		sModelIn.iCheckSN = n;
		sDemo.iCheckSN = n;
		sModelIn.pParam = &sDemo;
		/*************************************<针对灯检机加载2次模板>*************************************/
		//**[8/8/2017 ConanSteve]**:  
		//bool bUseReadModel = 1==GetPrivateProfileIntA(TEXT("SystemConfiguration"), TEXT("EnableReadModelTwice"), 0, CExFn::GetCurrentAppPath()+TEXT("\\Config\\AlgConfig.ini"));

		//if (bUseReadModel)
		if(bEnableMixedModeOfInitAlg)
		{
			bool bOldAlg{ false };
			CString sHashKey{ TEXT("") };
			sHashKey.Format(TEXT("%d"), n + 1);
			bOldAlg = 1 == GetPrivateProfileInt(TEXT("MixedModeOfInitAlg"), sHashKey, 0, CExFn::GetCurrentAppPath() + TEXT("\\Config\\AlgConfig.ini"));
			if(bOldAlg)
			{
				CString sEntireModelName = CExFn::GetCurrentAppPath() + TEXT("\\ModelInfo\\") + m_strModelNameUsing;
				sModelIn.iOperSig = 0;//2表示双击应用程序打开Demo窗口
				strcpy(sDemo.chModelName, sEntireModelName);
				sRst2 = m_multiObjManager.Set(sModelIn);
			}
		}
		/************************************************************************/
		sModelIn.iOperSig = 2;//2表示双击应用程序打开Demo窗口
		strcpy(sDemo.chModelName, m_strModelNameUsing);
		sRst2 = m_multiObjManager.Set(sModelIn);

		if (sRst2.iStatusType != STATUS_NOR)
		{
			char chInfo[256];
			ZeroMemory(chInfo,256);
			memcpy(chInfo,sRst2.pErrorInfo,strlen(sRst2.pErrorInfo));

			AfxMessageBox(chInfo);
		}
		if(g_pProgCtrl)g_pProgCtrl->StepIt();
	}

	if (nWinTop == 0)
		m_lModelNames.ApplyModel(m_strModelNameUsing.GetBuffer(0));

	//完毕
	Sleep(300);
	if(g_pProgCtrl)delete g_pProgCtrl;
	g_pProgCtrl = NULL;
}

BOOL CDemoProjectDlg::PreTranslateMessage(MSG* pMsg) 
{
 	if (WM_KEYDOWN == pMsg->message)
 	{
		UINT nKey = (int)pMsg->wParam;
		if (VK_CONTROL == nKey)
		{	//按住Ctrl键
			if (m_bCtrlIsDown == TRUE)
				return FALSE;
			m_bCtrlIsDown = TRUE;

			if (GetIfDrawing())
			{
				char chText[256];
				LoadString(g_hLanguage, 5020, chText, 256);
				MessageBox(chText, "DEMO", MB_ICONWARNING);
				return FALSE;
			}
			
 			int nWorkStn = m_Tab.GetCurSel();
			//获取路径
			char chPath[MAX_PATH];
			ZeroMemory(chPath, MAX_PATH);
			if (m_plFile[nWorkStn].GetSelectedPath(chPath))
				return FALSE;
			if (strlen(chPath) == 0)
				return FALSE;

			//检测
			s_AlgModelInput sInput;
			s_DemoAlgSetParam sDemo;
			strcpy(&sDemo.chModelName[0], &chPath[0]);
			sDemo.bTopWin = false;
			sInput.iOperSig = 102;
			sDemo.pParent = NULL;
			sDemo.rect = NULL;
			sInput.iCheckSN = nWorkStn;
			sDemo.iCheckSN = nWorkStn;
			sInput.pParam = &sDemo;
#ifdef COMMON_DEMO_VERSION
			s_Status sError = m_multiObjManager.Set(sInput);
#endif

			int nImg = sError.iStatusType;
			if (nImg == -1)
				nImg = 2;
			else if (nImg == -2)
				nImg = 3;
			m_lResult.InsertString(0, sError.pErrorInfo, nImg);
			
			return FALSE;
		}
		else if (VK_ESCAPE==nKey)
		{
			return TRUE;
		}
		else if (VK_RETURN==nKey)
		{
			if (GetDlgItem(IDC_EDIT_CURPATH) == GetFocus())
			{
				UpdateData(TRUE);
				CString strPath;
				GetDlgItem(IDC_EDIT_CURPATH)->GetWindowText(strPath);
				m_plFile[m_nLastPage].EnterDir(strPath);			
			}
			return FALSE;
		}
		else if (VK_F11 == nKey)
		{
			m_bF11Pressed = !m_bF11Pressed;

			CRect rect;
			if (m_bF11Pressed)	//第一次按下
			{
				CRect rect2;
				GetWindowRect(m_CurWndSize);
				GetClientRect(rect2);
				
				int nSide2 = (m_CurWndSize.Width() - rect2.Width()) / 2;
				int nSide1 = m_CurWndSize.Height() - rect2.Height() - nSide2;
				
				SystemParametersInfo(SPI_GETWORKAREA, 0, PVOID(&rect), 0);
				
				rect.top -= nSide1;
				rect.bottom += nSide2;
				rect.left -= nSide2;
				rect.right += nSide2;
			}
			else
			{
				rect = m_CurWndSize;
			}
			
			MoveWindow(rect);
			ScaleMoveWindow();
			
			RedrawWindow();
		}
		else if (VK_F12 == nKey)
		{
			if (m_bMoveSize)
			{
				CRect rect;
				if (GetKeyState(VK_CONTROL) < 0)	//按下ctrl
				{
					rect = m_WndInitSize;
				}
				else
				{
					SystemParametersInfo(SPI_GETWORKAREA, 0, PVOID(&rect), 0);
				}
				
				MoveWindow(rect);
				ScaleMoveWindow();
				RedrawWindow();
			}
		}
		else if ('i'==nKey || 'I'==nKey)
		{
			if (GetKeyState(VK_MENU) && GetKeyState(VK_CONTROL))
			{
				GetDlgItem(IDC_BTN_INSP)->EnableWindow(TRUE);
				GetDlgItem(IDC_BTN_INSP)->ShowWindow(SW_SHOW);
			}
		}
 	}
	if (pMsg->message == WM_KEYUP)
	{
		UINT nKey = pMsg->wParam;
		if (VK_CONTROL == nKey)
		{	//松开Ctrl键
			if (m_bCtrlIsDown == FALSE)
				return FALSE;
			m_bCtrlIsDown = FALSE;

			if (GetIfDrawing())
			{
				char chText[256];
				LoadString(g_hLanguage, 5020, chText, 256);
				MessageBox(chText, "DEMO", MB_ICONWARNING);
				return LRESULT(1);
			}
			
			int nWorkStn = m_Tab.GetCurSel();
			//获取路径
			char chPath[MAX_PATH];
			ZeroMemory(chPath, MAX_PATH);
			m_plFile[nWorkStn].GetSelectedPath(chPath);
			if (strlen(chPath) == 0)
				return FALSE;
			
			//检测
			s_AlgModelInput sInput;
			s_DemoAlgSetParam sDemo;
			ZeroMemory(&sInput,sizeof(sInput));
			ZeroMemory(&sDemo,sizeof(sDemo));

			strcpy(&sDemo.chModelName[0], &chPath[0]);
			sDemo.bTopWin = false;
			sInput.iOperSig = 101;
			sDemo.pParent = NULL;
			sDemo.rect = NULL;
			sDemo.iCheckSN = nWorkStn;
			sInput.pParam = &sDemo;
			sInput.iCheckSN = nWorkStn;
#ifdef COMMON_DEMO_VERSION
			s_Status sError = m_multiObjManager.Set(sInput);
#endif

			int nImg = sError.iStatusType;
			if (nImg == -1)
				nImg = 2;
			else if (nImg == -2)
				nImg = 3;
			m_lResult.InsertString(0, sError.pErrorInfo, nImg);
			
			return FALSE;
		}
	}

	if (pMsg->message == DEMO_APP_LOGIN)
	{
		if (pMsg->wParam > 0)
		{
			GetDlgItem(IDC_BTN_SAVESINGLEMODEL)->EnableWindow(TRUE);
			GetDlgItem(IDC_BTN_SAVEMODEL)->EnableWindow(TRUE);
			g_bSaveEnabled = TRUE;
		}
		else
		{
			GetDlgItem(IDC_BTN_SAVESINGLEMODEL)->EnableWindow(FALSE);
			GetDlgItem(IDC_BTN_SAVEMODEL)->EnableWindow(FALSE);
			g_bSaveEnabled = FALSE;
		}
	}

	// Mouse Operation
	if (!m_bIsWaiting && !m_bLBtnDown)
	{

		CRect rectDlg;
		GetDlgItem(IDC_STATIC_1)->GetWindowRect(rectDlg);
		if (pMsg->pt.x>=rectDlg.left && pMsg->pt.x<=rectDlg.right &&\
			pMsg->pt.y>=rectDlg.top && pMsg->pt.y<=rectDlg.bottom)
		{
			s_AlgModelInput sIn;
			s_DemoAlgSetParam sDemo;
			memset(&sIn,0,sizeof(s_AlgModelInput));
			sIn.iOperSig = 52;
			sDemo.rect.top = pMsg->pt.x;
			sDemo.rect.left = pMsg->pt.y;
			sprintf(sDemo.chModelName, "%d", pMsg->message);
#ifdef COMMON_DEMO_VERSION
			sIn.iCheckSN = m_Tab.GetCurSel();
			sDemo.iCheckSN = m_Tab.GetCurSel();
			TRACE("=====iCheckSN=%d========\n\n",sIn.iCheckSN);
			sIn.pParam = &sDemo;
			m_multiObjManager.Set(sIn);
#endif
		}
	}
	if (pMsg->message == WM_LBUTTONDOWN)
	{	//左键点击halcon窗口
		if (m_bIsWaiting)
			return TRUE;

		if (g_bParkourRunning)
		{
			POINT MousePos;
			GetCursorPos(&MousePos);
			CRect BtnRect;
			GetDlgItem(IDC_BTN_PARKOUR)->GetWindowRect(BtnRect);

			if (MousePos.x<=BtnRect.left || MousePos.x>=BtnRect.right ||\
				MousePos.y<=BtnRect.top || MousePos.y>=BtnRect.bottom)
				return TRUE;
		}
		else
		{
#ifdef COMMON_DEMO_VERSION
			if (!m_bSidebarHide)
			{
				CRect wndRect;	GetWindowRect(wndRect);
				CPoint pp;		GetCursorPos(&pp);
				CRect rectBk;	GetDlgItem(IDC_STATIC_BCK)->GetWindowRect(rectBk);
				if (pp.x>=rectBk.left-3 && pp.x<=rectBk.left+3 &&
					pp.y>=wndRect.top && pp.y<=wndRect.bottom)
				{	
					m_bLBtnDown = TRUE;
					SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_SIZEWE));
				}
			}
#endif
		}
	}


	if (pMsg->message == WM_MOUSEMOVE)
	{
#ifdef COMMON_DEMO_VERSION
		CRect wndRect;	GetClientRect(wndRect); ClientToScreen(wndRect);
		CPoint pp;		GetCursorPos(&pp);

		if (!m_bSidebarHide)
		{
			CRect rectBk;
			GetDlgItem(IDC_STATIC_BCK)->GetWindowRect(rectBk);
			
			int nSideBarCol1 = rectBk.left;
			if (pp.x>=nSideBarCol1-3 && pp.x<=nSideBarCol1+3)
			{
				SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_SIZEWE));
			}
			else
			{
				if (!m_bLBtnDown)
				{
					SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));
				}
			}
			if (m_bLBtnDown)
			{
				m_nSideWidth = wndRect.right - pp.x;
				SetTimer(EVENT_MOVE_SIDEBAR, 1, NULL);
			}
		}
		else	//隐藏侧边栏了
		{
			if (pp.x>=wndRect.right-5-3 && pp.x<=wndRect.right-5+3)
			{
				SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_SIZEWE));
			}
			else
			{
				SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));
			}
		}
#endif
	}

	if (pMsg->message == WM_LBUTTONDBLCLK)
	{
		if (m_bIsWaiting)
		{
			return TRUE;
		}
#ifdef COMMON_DEMO_VERSION
		CRect wndRect;	GetClientRect(wndRect);	ClientToScreen(wndRect);
		CPoint pp;		GetCursorPos(&pp);

		if (m_bSidebarHide)	// 要展开
		{
			if (pp.x>=wndRect.right-5-3 && pp.x<=wndRect.right-5+3)
			{
				m_bSidebarHide = FALSE;
				VisualSidebar();
				ModifySidebarWidth(m_nSideWidthTemp);
			}
		}
		else				// 要隐藏
		{
			CRect rectBk;
			GetDlgItem(IDC_STATIC_BCK)->GetWindowRect(rectBk);
			if (pp.x>=rectBk.left-3 && pp.x<=rectBk.left+3)
			{
				m_bSidebarHide = TRUE;
				VisualSidebar();
				m_nSideWidthTemp = m_nSideWidth;
				ModifySidebarWidth(10);
			}
		}
#endif
	}

#ifdef COMMON_DEMO_VERSION
	if (pMsg->message == WM_LBUTTONUP)
	{

		if (m_bLBtnDown)
		{
			SetClassLong(this->GetSafeHwnd(), GCL_HCURSOR, (LONG)LoadCursor(NULL, IDC_ARROW));
			m_bLBtnDown = FALSE;
			ModifySidebarWidth(m_nSideWidth);
		}
	}
#endif


#ifdef COMMON_DEMO_VERSION
	if (pMsg->message == WM_NCLBUTTONDBLCLK)
	{


// 		CRect rect;
// 		SystemParametersInfo(SPI_GETWORKAREA, 0, PVOID(&rect), 0);
// 		MoveWindow(rect);
// 		ScaleMoveWindow();
// 		RedrawWindow();
	}
#endif

	return CDialog::PreTranslateMessage(pMsg);
}

LRESULT CDemoProjectDlg::OnEnterSizeMove(WPARAM wParam, LPARAM lParam)
{
	m_bIsWaiting = TRUE;
	return 1;
}

LRESULT CDemoProjectDlg::OnExitSizeMove(WPARAM wParam, LPARAM lParam)
{
 	m_bIsWaiting = FALSE;
 
#ifdef COMMON_DEMO_VERSION
	CRect wndSize;
	GetWindowRect(wndSize);

	CPoint mouse_point;
	GetCursorPos(&mouse_point);

	BOOL bResizeWnd = FALSE;
	if (wndSize.Width() < 400)
	{
		bResizeWnd = TRUE;
		if (mouse_point.x > wndSize.CenterPoint().x)	//右边
		{
			wndSize.right = wndSize.left + 400;
		}
		else
		{
			wndSize.left = wndSize.right - 400;
		}
	}

	if (wndSize.Height() < 300)
	{
		bResizeWnd = TRUE;
		if (mouse_point.y > wndSize.CenterPoint().y)	//下边
		{
			wndSize.bottom = wndSize.top + 300;
		}
		else
		{
			wndSize.top = wndSize.bottom - 300;
		}
	}

	if (bResizeWnd)
	{
		MoveWindow(wndSize);
	}
	m_bNeedRemap = TRUE;

	ScaleMoveWindow();
	RedrawWindow();

#endif
	return 1;
}

LRESULT CDemoProjectDlg::OnAppLogin(WPARAM wParam, LPARAM lParam)
{
	if(1==wParam)
	{
		GetDlgItem(IDC_BTN_SAVESINGLEMODEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_SAVEMODEL)->EnableWindow(TRUE);
		g_bSaveEnabled = TRUE;
	}
	else if(0==wParam)
	{
		GetDlgItem(IDC_BTN_SAVESINGLEMODEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SAVEMODEL)->EnableWindow(FALSE);
		g_bSaveEnabled = FALSE;
	}

	return 1;
}

void CDemoProjectDlg::OnCancel()
{
	return;
}

void CDemoProjectDlg::OnOK() 
{
	if (m_pMainCtrlPR)
	{
		delete [] m_pMainCtrlPR;
		m_pMainCtrlPR = NULL;
	}

	if (m_pSideCtrlPR)
	{
		delete [] m_pSideCtrlPR;
		m_pSideCtrlPR = NULL;
	}

	m_MainCtrl.clear();
	m_SideCtrl.clear();

	if (m_bOpenFileOfErrorLog)
		m_fileErrorLog.Close();
	if (NULL!=m_hEvent_DemoMessage_SendCommandOfLoadModel)
		CloseHandle(m_hEvent_DemoMessage_SendCommandOfLoadModel);
	if (NULL != m_hEvent_DemoMessage_ReceiveSuccessOfLoadModel)
		CloseHandle(m_hEvent_DemoMessage_ReceiveSuccessOfLoadModel);
	if (NULL != m_hEvent_DemoMessage_ReceiveFailureOfLoadModel)
		CloseHandle(m_hEvent_DemoMessage_ReceiveFailureOfLoadModel);

	CDialog::OnOK();
	return;
}

void CDemoProjectDlg::FreeMemories()
{

	s_Status sError = m_multiObjManager.Free();
	if (sError.iStatusType != STATUS_NOR)
	{
		char chInfo[256];
		ZeroMemory(chInfo,256);
		memcpy(chInfo,sError.pErrorInfo,strlen(sError.pErrorInfo));

		CString strError;
		strError.Format("多对象释放失败-Free函数-%s",chInfo);
		AfxMessageBox(strError);
	}
	
	FreeLibrary(g_hLanguage);
	g_hLanguage = NULL;
	g_pGetTextFromLPK = NULL;

	if (g_pProgCtrl)
	{
		delete g_pProgCtrl;
		g_pProgCtrl = NULL;
	}

	if (m_nInitTopIndex)
		delete [] m_nInitTopIndex;
	if (m_plFile)
		delete [] m_plFile;
	if (m_plDisk)
		delete [] m_plDisk;
}

void CDemoProjectDlg::SaveModelBeforeQuit(BOOL bPopMsgBox)
{
	if (GetIfDrawing())	
	{
		char chTest[256];
		LoadString(g_hLanguage, 5020, chTest, 256);
		MessageBox(chTest, "DEMO", MB_ICONWARNING);
		return;
	}

	m_bIsWaiting = TRUE;

	//进度条
	char chText[256];
	LoadString(g_hLanguage, 5023, chText, 256);
	g_pProgCtrl = new CMyProgressCtrl(1, 1);
	CRect ProRect;
	ProRect.left=486;
	ProRect.right=ProRect.left+200;
	ProRect.top=330;
	ProRect.bottom=ProRect.top+30;
	g_pProgCtrl->Create(WS_CHILD|WS_BORDER, ProRect, this, 0);
	g_pProgCtrl->SetRange(0, m_nNumWS*2);
	g_pProgCtrl->SetStep(m_nNumWS);
	g_pProgCtrl->SetWindowText(chText);
	g_pProgCtrl->ShowWindow(SW_SHOW);
	
	
	s_AlgModelInput sModelIn;


	s_Status sRst;
	for (int n=0; n<m_nNumWS; ++n)
	{
		s_DemoAlgSetParam sDemo;
		strcpy(sDemo.chModelName, m_strModelNameUsing);
		if (m_bNewCreateModel)
			sModelIn.iOperSig = 41;
		else
			sModelIn.iOperSig = 4;
#ifdef COMMON_DEMO_VERSION
		sDemo.iCheckSN = n;
		sModelIn.iCheckSN = n;
		sModelIn.pParam = &sDemo;
		sRst = m_multiObjManager.Set(sModelIn);
#endif

		g_pProgCtrl->StepIt();

		sModelIn.iOperSig = 5;
#ifdef COMMON_DEMO_VERSION
		sModelIn.iCheckSN = n ;
		sRst = m_multiObjManager.Set(sModelIn);
#endif

		g_pProgCtrl->StepIt();
	}
	
	if (bPopMsgBox)
	{
		char chText2[256];
		LoadString(g_hLanguage, 5024, chText2, 256);
		((CMyProgressCtrl*)g_pProgCtrl)->SetWindowText(chText2);
		((CMyProgressCtrl*)g_pProgCtrl)->ShowPercent(FALSE);
		Sleep(300);
		LoadString(g_hLanguage, 5025, chText2, 256);
		AfxMessageBox(chText2);
		
		g_pProgCtrl->ShowWindow(SW_HIDE);
		delete g_pProgCtrl;
		g_pProgCtrl = NULL;
	}
	m_bIsWaiting = FALSE;
	m_bNewCreateModel = false;
}

void CDemoProjectDlg::OnBtnSaveSingle()
{
	 //m_SAVESINGLE.SetIcon(IDR_MAINFRAME);
	 
	if (GetIfDrawing())
	{
		char chText[256];
		LoadString(g_hLanguage, 5020, chText, 256);
		MessageBox(chText, "DEMO", MB_ICONWARNING);
		return;
	}

	//不允许其他操作
	m_bIsWaiting = TRUE;
	
	//控件
	char chText[256];
	LoadString(g_hLanguage, 5023, chText, 256);
	g_pProgCtrl = new CMyProgressCtrl(1, 1);
	CRect ProRect;
	ProRect.left=486;
	ProRect.right=ProRect.left+200;
	ProRect.top=330;
	ProRect.bottom=ProRect.top+30;
	g_pProgCtrl->Create(WS_CHILD|WS_BORDER|PBS_SMOOTH, ProRect, this, 0);
	g_pProgCtrl->SetRange(0, 100);
	g_pProgCtrl->SetStep(50);
	
	g_pProgCtrl->SetWindowText(chText);
	g_pProgCtrl->ShowWindow(SW_SHOW);
	
	
	s_AlgModelInput sModelIn;
	s_DemoAlgSetParam sDemo;
	s_Status sRst;
	if (m_bNewCreateModel)
		sModelIn.iOperSig = 41;
	else
		sModelIn.iOperSig = 4;
	strcpy(sDemo.chModelName, m_strModelNameUsing);
	
	int nPage = m_Tab.GetCurSel();
	FnWriteLogOfSaveModel(nPage + 1);
	//写模板
#ifdef COMMON_DEMO_VERSION
	sDemo.iCheckSN = nPage;
	sModelIn.iCheckSN = nPage;
	sModelIn.pParam = &sDemo;
	sRst = m_multiObjManager.Set(sModelIn);
#endif

	g_pProgCtrl->StepIt();
	BOOL bSuccess = !(BOOL)sRst.iStatusType;
	//写配置文件
	sModelIn.iOperSig = 5;
#ifdef COMMON_DEMO_VERSION
	sModelIn.iCheckSN = nPage;
	sRst = m_multiObjManager.Set(sModelIn);
#endif


	if (bSuccess && !(BOOL)sRst.iStatusType)
	{
		LoadString(g_hLanguage, 5024, chText, 256);
		((CMyProgressCtrl*)g_pProgCtrl)->SetWindowText(chText);
		((CMyProgressCtrl*)g_pProgCtrl)->ShowPercent(FALSE);

 		m_bIsWaiting = FALSE;

		g_pGetTextFromLPK(chText, 6, m_strModelNameUsing.GetBuffer(0), nPage+1);
		GetDlgItem(IDC_STATIC_INFO)->SetWindowText(chText);

		Sleep(300);
		AfxMessageBox(tr("Save Succeed!"));
	}
	else
	{
		g_pGetTextFromLPK(chText, 7, m_strModelNameUsing.GetBuffer(0), nPage+1);
		GetDlgItem(IDC_STATIC_INFO)->SetWindowText(chText);

		LoadString(g_hLanguage, 5026, chText, 256);
		((CMyProgressCtrl*)g_pProgCtrl)->SetWindowText(chText);
		m_bIsWaiting = FALSE;
		
		AfxMessageBox(chText);
	}
	
	g_pProgCtrl->ShowWindow(SW_HIDE);
	delete g_pProgCtrl;
	g_pProgCtrl = NULL;
	
	m_bNewCreateModel = false;
	return;
}

void CDemoProjectDlg::OnBtnSave() 
{
	FnWriteLogOfSaveModel(0);
	if (GetIfDrawing())
	{
		char chText[256];
		LoadString(g_hLanguage, 5020, chText, 256);
		MessageBox(chText, "DEMO", MB_ICONWARNING);
		return;
	}

	//不允许有其他动作
	m_bIsWaiting = TRUE;

	//进度条
	char chText[256];
	LoadString(g_hLanguage, 5023, chText, 256);
	g_pProgCtrl = new CMyProgressCtrl(1, 1);
	CRect ProRect;
	ProRect.left=486;
	ProRect.right=ProRect.left+200;
	ProRect.top=330;
	ProRect.bottom=ProRect.top+30;
	g_pProgCtrl->Create(WS_CHILD|WS_BORDER, ProRect, this, 0);
	g_pProgCtrl->SetRange(0,m_nNumWS*2);
	g_pProgCtrl->SetStep(1);
	g_pProgCtrl->SetWindowText(chText);
	g_pProgCtrl->ShowWindow(SW_SHOW);

	//开始保存
	s_AlgModelInput sModelIn;

	s_Status sRst;

	int* nSuccess = new int[m_nNumWS];
	ZeroMemory(nSuccess, sizeof(int)*m_nNumWS);
	for (int n=0; n<m_nNumWS; ++n)
	{
		s_DemoAlgSetParam sDemo;
		strcpy(sDemo.chModelName, m_strModelNameUsing);
//		AfxMessageBox(sDemo.chModelName);
		if (m_bNewCreateModel)
			sModelIn.iOperSig = 41;
		else
		sModelIn.iOperSig = 4;
#ifdef COMMON_DEMO_VERSION
		sDemo.iCheckSN = n;
		sModelIn.iCheckSN = n;
		sModelIn.pParam = &sDemo;
		sRst = m_multiObjManager.Set(sModelIn);
#endif

		g_pProgCtrl->StepIt();
		nSuccess[n] = nSuccess[n]|(!sRst.iStatusType);

		sModelIn.iOperSig = 5;
#ifdef COMMON_DEMO_VERSION
		sModelIn.iCheckSN = n;
		sRst = m_multiObjManager.Set(sModelIn);
#endif

		g_pProgCtrl->StepIt();
		nSuccess[n] = nSuccess[n]|((!sRst.iStatusType)*2);
	}

	CString strFailNum;
	BOOL bSus = TRUE;
	for (int n=0; n<m_nNumWS; ++n)
	{
		if (nSuccess[n]&2==0 || nSuccess[n]&1==0)//失败啦 收衣服哇
		{	
			CString strThisNum;
			strThisNum.Format("%d,", n+1);
			strFailNum += strThisNum;
			bSus = FALSE;
		}
	}
	if (m_nNumWS == 1)
		delete nSuccess;
	else
		delete [] nSuccess;

	if (bSus)	//成功
	{
		LoadString(g_hLanguage, 5024, chText, 256);
		((CMyProgressCtrl*)g_pProgCtrl)->SetWindowText(chText);
		((CMyProgressCtrl*)g_pProgCtrl)->ShowPercent(FALSE);


		m_bIsWaiting = FALSE;	//无其他工作了

		g_pGetTextFromLPK(chText, 8, m_strModelNameUsing.GetBuffer(0));
		GetDlgItem(IDC_STATIC_INFO)->SetWindowText(chText);
		Sleep(300);
		AfxMessageBox(tr("Save Succeed!"));
	}
	else	//失败
	{
		LoadString(g_hLanguage, 5026, chText, 256);
		((CMyProgressCtrl*)g_pProgCtrl)->SetWindowText(chText);
		
		g_pGetTextFromLPK(chText, 9, m_strModelNameUsing.GetBuffer(0));
		GetDlgItem(IDC_STATIC_INFO)->SetWindowText(chText);

		g_pGetTextFromLPK(chText, 10, strFailNum.Left(strFailNum.GetLength()-1).GetBuffer(0));
		m_bIsWaiting = FALSE;

		AfxMessageBox(chText);
	}

	g_pProgCtrl->ShowWindow(SW_HIDE);
	delete g_pProgCtrl;
	g_pProgCtrl = NULL;

	m_bNewCreateModel = false;
}

void CDemoProjectDlg::DisEnMdlList(int nFlag)
{
	if (nFlag == 1)			//显示模板
	{
		m_bFileIsShow = false;
		//模板
		/************************************************************************/
		/*                       [5/31/2017 ConanSteve]                         */
		if (m_bEnableModelSearch)
		{
			GetDlgItem(IDC_EDIT_ModelLog)->ShowWindow(SW_SHOW);
		}
		if (m_bEnableShowModelLog)
		{
			GetDlgItem(IDC_CHECK_ShowModelLog)->ShowWindow(SW_SHOW);
		}
// 		CRect rectEditSearch{ 0 }, rectCheckBox{ 0 };
// 		GetDlgItem(IDC_EDIT_ModelLog)->GetClientRect(rectEditSearch);
// 		GetDlgItem(IDC_CHECK_ShowModelLog)->GetClientRect(rectEditSearch);
		/************************************************************************/
		GetDlgItem(IDC_STATIC_BCK)->ShowWindow(SW_SHOW);	GetDlgItem(IDC_STATIC_MLIST)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_MLIST)->ShowWindow(SW_SHOW);	GetDlgItem(IDC_STATIC_MLIST)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_NEWNAME)->ShowWindow(SW_SHOW);GetDlgItem(IDC_STATIC_NEWNAME)->EnableWindow(TRUE);
		GetDlgItem(IDC_MODELLIST)->ShowWindow(SW_SHOW);		GetDlgItem(IDC_MODELLIST)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_KJ)->ShowWindow(SW_SHOW);		GetDlgItem(IDC_STATIC_KJ)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT1)->ShowWindow(SW_SHOW);			if(!m_bDisableCreate)GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		GetDlgItem(IDC_NEWMODEL)->ShowWindow(SW_SHOW);		if(!m_bDisableCreate)GetDlgItem(IDC_NEWMODEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_DELETE)->ShowWindow(SW_SHOW);	if(!m_bDisableDelete)GetDlgItem(IDC_BTN_DELETE)->EnableWindow(TRUE);
		if (!m_nOpenMode)	//双击打开
		{	
			GetDlgItem(IDC_BTN_SETD)->ShowWindow(SW_SHOW); if(!m_bDisableSetDef)GetDlgItem(IDC_BTN_SETD)->EnableWindow(TRUE);
		}
		if(m_bStopperMode){GetDlgItem(IDC_COMBO_STOPPERTYPE)->ShowWindow(SW_SHOW);GetDlgItem(IDC_COMBO_STOPPERTYPE)->EnableWindow(TRUE);}

		//浏览器
		GetDlgItem(IDC_STATIC_FILEBACK)->ShowWindow(SW_HIDE);GetDlgItem(IDC_STATIC_FILEBACK)->EnableWindow(FALSE);
		for (int n=0; n<m_nNumWS; ++n)
		{
			m_plFile[n].EnableWindow(FALSE);	m_plFile[n].ShowWindow(SW_HIDE);
			m_plDisk[n].EnableWindow(FALSE);	m_plDisk[n].ShowWindow(SW_HIDE);
		}
		GetDlgItem(IDC_BTN_ERRIMG)->ShowWindow(SW_HIDE);	GetDlgItem(IDC_BTN_ERRIMG)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_GRABIMG)->ShowWindow(SW_HIDE);	GetDlgItem(IDC_BTN_GRABIMG)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CAPIMG)->ShowWindow(SW_HIDE);	GetDlgItem(IDC_BTN_CAPIMG)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CLRST)->ShowWindow(SW_HIDE);		GetDlgItem(IDC_BTN_CLRST)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_RESULT)->ShowWindow(SW_HIDE);	GetDlgItem(IDC_LIST_RESULT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CURPATH)->ShowWindow(SW_HIDE);	GetDlgItem(IDC_EDIT_CURPATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_VERSION)->ShowWindow(SW_HIDE);GetDlgItem(IDC_STATIC_VERSION)->EnableWindow(FALSE);
	}
	else if (nFlag == 0)	//显示图像文件
	{
		m_bFileIsShow = true;
		//模板
		/************************************************************************/
		/*                       [5/31/2017 ConanSteve]                         */
		GetDlgItem(IDC_EDIT_ModelLog)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK_ShowModelLog)->ShowWindow(SW_HIDE);
		/************************************************************************/
		GetDlgItem(IDC_STATIC_BCK)->ShowWindow(SW_HIDE);	GetDlgItem(IDC_STATIC_BCK)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_MLIST)->ShowWindow(SW_HIDE);	GetDlgItem(IDC_STATIC_MLIST)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_NEWNAME)->ShowWindow(SW_HIDE);GetDlgItem(IDC_STATIC_NEWNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_MODELLIST)->ShowWindow(SW_HIDE);		GetDlgItem(IDC_MODELLIST)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_KJ)->ShowWindow(SW_HIDE);		GetDlgItem(IDC_STATIC_KJ)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);			GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_NEWMODEL)->ShowWindow(SW_HIDE);		GetDlgItem(IDC_NEWMODEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DELETE)->ShowWindow(SW_HIDE);	GetDlgItem(IDC_BTN_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SETD)->ShowWindow(SW_HIDE);		GetDlgItem(IDC_BTN_SETD)->EnableWindow(FALSE);
		if(m_bStopperMode){GetDlgItem(IDC_COMBO_STOPPERTYPE)->ShowWindow(SW_HIDE);GetDlgItem(IDC_COMBO_STOPPERTYPE)->EnableWindow(FALSE);}

		//图像文件浏览器
		GetDlgItem(IDC_STATIC_FILEBACK)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_BTN_ERRIMG)->ShowWindow(SW_SHOW);	GetDlgItem(IDC_BTN_ERRIMG)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_GRABIMG)->ShowWindow(SW_SHOW);	GetDlgItem(IDC_BTN_GRABIMG)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_CAPIMG)->ShowWindow(SW_SHOW);	GetDlgItem(IDC_BTN_CAPIMG)->EnableWindow(TRUE);
		GetDlgItem(IDC_BTN_CLRST)->ShowWindow(SW_SHOW);		GetDlgItem(IDC_BTN_CLRST)->EnableWindow(TRUE);
		GetDlgItem(IDC_LIST_RESULT)->ShowWindow(SW_SHOW);	GetDlgItem(IDC_LIST_RESULT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_CURPATH)->ShowWindow(SW_SHOW);	GetDlgItem(IDC_EDIT_CURPATH)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_VERSION)->ShowWindow(SW_SHOW);GetDlgItem(IDC_STATIC_VERSION)->EnableWindow(TRUE);


		if (m_nLastPage>=0 && m_nLastPage<m_nNumWS)
		{
			m_plFile[m_nLastPage].EnableWindow(TRUE);	m_plFile[m_nLastPage].ShowWindow(SW_SHOW);
			m_plDisk[m_nLastPage].EnableWindow(TRUE);	m_plDisk[m_nLastPage].ShowWindow(SW_SHOW);
		}
	}
	return;
}

void CDemoProjectDlg::OnButtonMlist() 
{
	//切换最右侧列表
	m_bShowSide = !m_bShowSide;
	DisEnMdlList(m_bShowSide);
	char chText[256];
	if (m_bShowSide)
		LoadString(g_hLanguage, 5018, chText, 256);
	else
		LoadString(g_hLanguage, 5003, chText, 256);
	GetDlgItem(IDC_BTN_MLIST)->SetWindowText(chText);
}

HBRUSH CDemoProjectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if   ((pWnd-> GetDlgCtrlID()   ==   IDC_STATIC_BCK)   &&   (nCtlColor   ==   CTLCOLOR_STATIC) && m_nbrush==0) 
	{ 
		CDC* pWnd = GetDC();
		COLORREF bkcolor = pWnd->GetBkColor();
		pDC-> SetBkColor(bkcolor);           //设置黑色的背景 
		m_brush.CreateSolidBrush(bkcolor); 
		m_nbrush = 1;
		return   m_brush;     //作为约定，返回背景色对应的刷子句柄 
    } 
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

LRESULT CDemoProjectDlg::OnMsgFromBackupM(WPARAM wParam, LPARAM lParam)
{
	m_bIsWaiting = FALSE;
	return 0;
}

LRESULT CDemoProjectDlg::OnMsgFromInternalThread(WPARAM wParam, LPARAM lParam)
{	// 一键检测
	if (g_bParkourRunning)
	{
		s_AlgModelInput sIn;
		s_DemoAlgSetParam sDemo;
		ZeroMemory(&sIn,sizeof(sIn));	
		ZeroMemory(&sDemo,sizeof(sDemo));

		strcpy(sDemo.chModelName, ((s_Status*)lParam)->pErrorInfo);
		sDemo.bTopWin = false;
		sIn.iOperSig = 101;
		sDemo.pParent = NULL;
		sDemo.rect = NULL;
		int nWS = ((s_Status*)lParam)->iStatusType;
		sDemo.iCheckSN = nWS;
		((s_Status*)lParam)->iStatusType = 0;
		ZeroMemory(((s_Status*)lParam)->pErrorInfo, STATUS_STRING_MAXLENGTH);
#ifdef COMMON_DEMO_VERSION
		sDemo.iCheckSN = nWS;
		sIn.iCheckSN = nWS;
		sIn.pParam = &sDemo;
		*((s_Status*)lParam) = m_multiObjManager.Set(sIn);
#endif

		int nImg = ((s_Status*)lParam)->iStatusType;
// 		if (nImg == -1)
// 			nImg = 2;
// 		else if (nImg == -2)
// 			nImg = 3;
		if (nImg != 0)
			nImg = 1;
		m_lResult.InsertString(0, ((s_Status*)lParam)->pErrorInfo, nImg);
		m_lResult.Refresh();
		
		char chInfo[256];
		ZeroMemory(chInfo, 256);
		g_pGetTextFromLPK(chInfo, 19, nWS+1);
		GetDlgItem(IDC_STATIC_INFO)->SetWindowText(chInfo);
	}
	return 1;
}

LRESULT CDemoProjectDlg::OnMsgFromSysReadModel(WPARAM wParam, LPARAM lParam)
{
	int nMsg = (int)lParam;

	m_csProgressCtrl.Lock();
	if(!m_bHandledLoadModelMsg)
	{
		if (g_pProgCtrl)
		{
			char chProgText[256];
			LoadString(g_hLanguage, 5027, chProgText, 256);
			KillTimer(2);
			g_pProgCtrl->SetPos(100);
			((CMyProgressCtrl*)g_pProgCtrl)->ChangeColor(1);
			g_pProgCtrl->SetWindowText(chProgText);
		}

		m_bIsWaiting = FALSE;
		if (nMsg == 0)
		{
			char chText[256];
			LoadString(g_hLanguage, 5028, chText, 256);
			AfxMessageBox(chText);
		}
		else
		{
			char chText[256];
			LoadString(g_hLanguage, 5029, chText, 256);
			AfxMessageBox(chText);
		}


		if (g_pProgCtrl)
		{
			delete g_pProgCtrl;
			g_pProgCtrl = NULL;
		}
		if (m_bAskingSystem)
		{
			FreeMemories();
			if (m_bChangeType)	//如果是修改检测类型导致的读取模板 需要重新开一个DEMO
				ShellExecute(NULL, "open", "D:\\tmp.vbs", NULL, NULL, SW_SHOW);
			OnOK();
			return (LRESULT)0;
		}

		char chText[256];
		g_pGetTextFromLPK(chText, 11, m_strModelNameUsing.GetBuffer(0));
		GetDlgItem(IDC_STATIC_INFO)->SetWindowText(chText);

		m_bHandledLoadModelMsg = true;
	}

	m_csProgressCtrl.Unlock();
	return (LRESULT)lParam;
}

void CDemoProjectDlg::OnBtnCreate()
{
	UpdateData(TRUE);

	if (GetIfDrawing())
	{
		char chText[256];
		LoadString(g_hLanguage, 5020, chText, 256);
		MessageBox(chText, "DEMO", MB_ICONWARNING);
		return;
	}
	
	//检测模板名称
	if (m_strNewModelName.GetLength()<=0)
	{
		char chText[256];
		LoadString(g_hLanguage, 5030, chText, 256);
		AfxMessageBox(chText);
		return;
	}
	if (m_strNewModelName.GetLength() > 254)
	{
		char chText[256];
		LoadString(g_hLanguage, 5031, chText, 256);
		AfxMessageBox(chText);
		return;
	}
	
	//复制哪个模板
	CString modelname;
	int nIndex = m_lModelNames.GetCaretIndex();
	if (nIndex < 0)
		modelname = m_strModelNameUsing;	//复制当前的
	else
		m_lModelNames.GetText(nIndex, modelname);	//复制选中的

	CString strModelDir, strNewModelDir;
	strModelDir = g_strAllModelPath + "\\" + modelname;				//源模板路径
	strNewModelDir = g_strAllModelPath + "\\" + m_strNewModelName;	//要新建的模板路径

	if(_access(strNewModelDir.GetBuffer(0), 0) == 0)
	{
		char chText[256];
		LoadString(g_hLanguage, 5032, chText, 256);
		AfxMessageBox(chText);
		return;
	}

	bool bret = CopyDir(strModelDir, strNewModelDir);
	if (bret)
	{

		CreateModelLog(m_strNewModelName);
		//保存一下当前模板
		if(g_bSaveEnabled && GetIfNeedSave())
		{	
			char chText[256];
			g_pGetTextFromLPK(chText, 13, m_strModelNameUsing.GetBuffer(0));
			if (IDYES==MessageBox(chText, "DEMO", MB_ICONQUESTION|MB_YESNO))
			{
				OnBtnSave();
			}
		}

		char chText[256];
		LoadString(g_hLanguage, 5033, chText, 256);
		AfxMessageBox(chText);

		m_bNewCreateModel = true;
		m_lModelNames.Refresh();
		int nFind = m_lModelNames.FindString(0, m_strNewModelName);
 		m_lModelNames.SetCurSel(nFind);
		
		m_strModelNameUsing = m_strNewModelName;
		CreateTabWins(m_Tab.GetCurSel()+1);
		
		m_strCModel.Format("%s", m_strModelNameUsing.GetBuffer(0));
		m_strNewModelName = "";
		UpdateData(FALSE);

		g_pGetTextFromLPK(chText, 14, m_strModelNameUsing.GetBuffer(0));
		GetDlgItem(IDC_STATIC_INFO)->SetWindowText(chText);


	}
	else
	{
		char chText[256];
		LoadString(g_hLanguage, 5034, chText, 256);
		AfxMessageBox(chText);
		DeleteDir(strNewModelDir);
	}
	InitModel();
	return;
}

bool CDemoProjectDlg::CopyDir(CString strSource, CString strDest)
{
	if (!CreateDirectory(strDest.GetBuffer(0), NULL))
	{
		char chText[256];
		LoadString(g_hLanguage, 5035, chText, 256);
		AfxMessageBox(chText);
		return false;
	}
	
	CString strSearch(strSource);
	strSearch += "\\*.*";
	
	CFileFind finder;
	BOOL Ret = finder.FindFile(strSearch);
	while(Ret)
	{
		Ret = finder.FindNextFile();
		if (finder.IsDots())
			continue;
		
		CString strSrcFile = finder.GetFilePath();
		CString strDesFile(strDest);
		strDesFile += "\\"; 
		strDesFile += finder.GetFileName();
		if (finder.IsDirectory())
		{
			if (!CopyDir(strSrcFile, strDesFile))
			{
				finder.Close();
				return false;
			}
		}
		else
		{
			if (!CopyFile(strSrcFile, strDesFile, FALSE))
			{
				finder.Close();
				return FALSE;
			}
		}
	}

	finder.Close();
	return TRUE;
}

void CDemoProjectDlg::OnBtnDelete()
{	
	if (GetIfDrawing())
	{
		char chText[256];
		LoadString(g_hLanguage, 5020, chText, 256);
		MessageBox(chText, "DEMO", MB_ICONWARNING);
		return;
	}

	//获取选中的名称
	int nIndex = m_lModelNames.GetCaretIndex();
	if (m_lModelNames.GetCount()<=0 || nIndex<0)
		return;
	CString strModelSelected;
	m_lModelNames.GetText(nIndex, strModelSelected);

	//能否删除？
	if (strModelSelected.Compare(m_strSysModelName) == 0)
	{
		char chText[256];
		LoadString(g_hLanguage, 5047, chText, 256);
		AfxMessageBox(chText);
		return;
	}
	if (strModelSelected.Compare(m_strModelNameUsing) == 0)
	{
		char chText[256];
		LoadString(g_hLanguage, 5048, chText, 256);
		AfxMessageBox(chText);
		return;
	}
	char chText[256];
	g_pGetTextFromLPK(chText, 30, strModelSelected.GetBuffer(0));
	int nChoice = MessageBox(chText, "DEMO", MB_YESNO|MB_ICONQUESTION);
	if (nChoice == IDNO)
		return;

	//删除
	CString strModelPath = g_strAllModelPath + "\\" + strModelSelected;
	if(_access(strModelPath.GetBuffer(0), 0) == -1)
	{
		char chText[256];
		LoadString(g_hLanguage, 5049, chText, 256);
		AfxMessageBox(chText);
		return;
	}
	BOOL bret = DeleteDir(strModelPath);
	if (bret)
	{
		char chText[256];
		LoadString(g_hLanguage, 5050, chText, 256);
		AfxMessageBox(chText);

		g_pGetTextFromLPK(chText, 29, strModelSelected.GetBuffer(0));
		GetDlgItem(IDC_STATIC_INFO)->SetWindowText(chText);

		//控件中删除
		int nFind = m_lModelNames.FindString(0, strModelSelected);
		m_lModelNames.DeleteString(nFind);
		if (nFind == 0)
			nFind++;
		m_lModelNames.SetCaretIndex(nFind-1);
	}
}

BOOL CDemoProjectDlg::DeleteDir(CString strDir)
{
	CString strSearch;
	strSearch = strDir + "\\*.*";
	
	CFileFind finder;
	BOOL Ret = finder.FindFile(strSearch);

	while(Ret)
	{
		Ret = finder.FindNextFile();
		if (finder.IsDots())
			continue;

		CString strSubFile = finder.GetFilePath();

		if (finder.IsDirectory())
		{
			if (!DeleteDir(strSubFile))
			{
				finder.Close();
				return FALSE;
			}
		}
		else
		{
			if (!DeleteFile(strSubFile))
			{
				finder.Close();
				return FALSE;
			}
		}
	}

	finder.Close();

	return RemoveDirectory(strDir.GetBuffer(0));
}

void CDemoProjectDlg::OnTimer(UINT nIDEvent) 
{


	//进度条循环走
	if (nIDEvent == 2)
	{
		if (g_pProgCtrl)
		{
			if (g_pProgCtrl->GetPos()==100)
				g_pProgCtrl->SetPos(10);
			else
				g_pProgCtrl->StepIt();
		}
	}

	//寻找后台程序
	if (nIDEvent == EVENT_SEARCHBM)
	{
// 		CWnd *pBMWnd = CWnd::FindWindow(NULL, "BackupManager");
// 		if (pBMWnd)
// 		{
// 			KillTimer(m_uSearchBM);
// 			pBMWnd->SendMessage(WM_DEMO_ONOFF, WPARAM(1), LPARAM(this));
// 		}		
	}

	if (nIDEvent == EVENT_MOVE_SIDEBAR)
	{
		ModifySidebarWidth(m_nSideWidth, FALSE);
		KillTimer(nIDEvent);
	}

	if (nIDEvent == 3)
	{
		KillTimer(3);
		theStart->ShowWindow(SW_HIDE);
		theStart->SendMessage(WM_CLOSE);
		this->SetForegroundWindow();
		::SetFocus(this->m_hWnd);
	}

	CDialog::OnTimer(nIDEvent);
}

void CDemoProjectDlg::OnClose() 
{
	CDialog::OnClose();
}

void CDemoProjectDlg::OnBtnSetd() 
{
	// TODO: Add your control notification handler code here
	CString strModelSelected;
	m_lModelNames.GetText(m_lModelNames.GetCaretIndex(), strModelSelected);
	char chMsg[256];
	g_pGetTextFromLPK(chMsg, 16, strModelSelected.GetBuffer(0));
	if (MessageBox(chMsg, "DEMO", MB_YESNO|MB_ICONQUESTION)==IDYES)
		WritePrivateProfileString("系统配置", "模板名称", strModelSelected.GetBuffer(0), g_strCheckIni);
		WritePrivateProfileString("系统配置", "模板名称", strModelSelected.GetBuffer(0), g_strCheckIni);
	
	g_pGetTextFromLPK(chMsg, 17, strModelSelected.GetBuffer(0));
	GetDlgItem(IDC_STATIC_INFO)->SetWindowText(chMsg);
}

LRESULT CDemoProjectDlg::OnMsgFromListBox(WPARAM wParam, LPARAM lParam)
{
	if ((HWND)wParam == GetSafeHwnd())
	{
		//切换文件夹
		if (UINT(((COPYDATASTRUCT*)lParam)->dwData) == DHLIST_DIRCHANGE)
		{
			int nWorkStn = int(((COPYDATASTRUCT*)lParam)->cbData);
			char chPath[MAX_PATH];
			strcpy(chPath, (char*)(((COPYDATASTRUCT*)lParam)->lpData));
			GetDlgItem(IDC_EDIT_CURPATH)->SetWindowText(chPath);
			m_strListPath[nWorkStn-1] = chPath;
			char chInfo[256];
			g_pGetTextFromLPK(chInfo, 18, nWorkStn);
			GetDlgItem(IDC_STATIC_INFO)->SetWindowText(chInfo);
			
			///////////////////////////  new
// 			CFileFind finder;
// 			CString strSearch = chPath;
// 			strSearch += "\\*.bmp";
// 			BOOL Ret = finder.FindFile(strSearch.GetBuffer(0));
// 			CStringArray strArrSuperImg;
// 			while(Ret)
// 			{
// 				Ret = finder.FindNextFile();
// 				if (      ((finder.GetFileName()).Right(4)==".bmp")&&((finder.GetFileName()).Left(7)=="Needle_")     ||     ((finder.GetFileName()).Right(4)==".BMP")&&((finder.GetFileName()).Left(7)=="Needle_")    )
// 				{
// 					//			AfxMessageBox(finder.GetFileName());
// 					//strArrSuperImg.Add(finder.GetFileName());
// 					s_AlgModelInput sInput;
// 					s_DemoAlgSetParam sDemo;
// 					ZeroMemory(&sInput,sizeof(sInput));
// 					ZeroMemory(&sDemo,sizeof(sDemo));
// 					CString strCurrentDir;
// 					strCurrentDir=m_strListPath[nWorkStn-1]+"\\"+finder.GetFileName();
// 					strcpy(&sDemo.chModelName[0],strCurrentDir.GetBuffer(0));
// 					
// 					sDemo.bTopWin = false;
// 					if (UINT(((COPYDATASTRUCT*)lParam)->dwData)==DHLIST_SELIMGFILECTRL)
// 						sInput.iOperSig = 102;
// 					else
// 						sInput.iOperSig = 101;//101正常检测图，102只显示图，103当做模板图像，104相机取图测试，105询问是否在画
// 					sDemo.pParent = NULL;
// 					sDemo.rect = NULL;
// 					sDemo.iCheckSN = nWorkStn-1;
// 					sInput.iCheckSN = nWorkStn-1;
// #ifdef COMMON_DEMO_VERSION
// 					sInput.pParam = &sDemo;
// 					s_Status sError = m_multiObjManager.Set(sInput);
// #endif
// 					
// 					int nImg = sError.iStatusType;
// 					if (nImg == -1)
// 						nImg = 2;
// 					else if (nImg == -2)
// 						nImg = 3;
// 					m_lResult.InsertString(0, sError.pErrorInfo, nImg);
// 			
// 
// 				}
// 				
// 			}
			

			return LRESULT(1);
		}
		if (UINT(((COPYDATASTRUCT*)lParam)->dwData)==DHLIST_SELIMGFILE || UINT(((COPYDATASTRUCT*)lParam)->dwData)==DHLIST_SELIMGFILECTRL)
		{	//点击图了
			if (g_bParkourRunning)
				return LRESULT(1);

			if (GetIfDrawing())
			{
				char chText[256];
				LoadString(g_hLanguage, 5020, chText, 256);
				MessageBox(chText, "DEMO", MB_ICONWARNING);
				return LRESULT(1);
			}

			int nWorkStn = int(((COPYDATASTRUCT*)lParam)->cbData);
			char chPath[MAX_PATH];
			strcpy(chPath, (TCHAR*)(((COPYDATASTRUCT*)lParam)->lpData));

			CFileFind finder;
			CString strSearch = chPath;
			strSearch += "\\*.bmp";
			BOOL Ret = finder.FindFile(strSearch.GetBuffer(0));
			CStringArray strArrSuperImg;
			CString strListPath;
			if (Ret == 0)
			{
				s_AlgModelInput sInput;
				s_DemoAlgSetParam sDemo;
				ZeroMemory(&sInput,sizeof(sInput));
				ZeroMemory(&sDemo,sizeof(sDemo));
				
				strcpy(&sDemo.chModelName[0], &chPath[0]);
				sDemo.bTopWin = false;
				if (UINT(((COPYDATASTRUCT*)lParam)->dwData)==DHLIST_SELIMGFILECTRL)
					sInput.iOperSig = 102;
				else
					sInput.iOperSig = 101;//101正常检测图，102只显示图，103当做模板图像，104相机取图测试，105询问是否在画
				sDemo.pParent = NULL;
				sDemo.rect = NULL;
				sDemo.iCheckSN = nWorkStn-1;
				sInput.iCheckSN = nWorkStn-1;
#ifdef COMMON_DEMO_VERSION
				sInput.pParam = &sDemo;
				s_Status sError = m_multiObjManager.Set(sInput);
#endif
				
				int nImg = sError.iStatusType;
// 				if (nImg == -1)
// 					nImg = 2;
// 				else if (nImg == -2)
// 					nImg = 3;
				if (0 != nImg)
					nImg = 1;
				m_lResult.InsertString(0, sError.pErrorInfo, nImg);
			}
			while(Ret)
			{
				
				Ret = finder.FindNextFile();
				Ret=0;
				if (  ((finder.GetFileName()).Right(4)==".bmp")   ||     ((finder.GetFileName()).Right(4)==".BMP")  )
				//if (      ((finder.GetFileName()).Right(4)==".bmp")&&((finder.GetFileName()).Left(7)=="Needle_")     ||     ((finder.GetFileName()).Right(4)==".BMP")&&((finder.GetFileName()).Left(7)=="Needle_")    )
				{
					//			AfxMessageBox(finder.GetFileName());
					//strArrSuperImg.Add(finder.GetFileName());
					s_AlgModelInput sInput;
					s_DemoAlgSetParam sDemo;
					ZeroMemory(&sInput,sizeof(sInput));
					ZeroMemory(&sDemo,sizeof(sDemo));
					CString strCurrentDir;
					strcpy(chPath, (char*)(((COPYDATASTRUCT*)lParam)->lpData));

					strListPath= chPath;
					//strCurrentDir=strListPath+"\\"+finder.GetFileName();
					strCurrentDir=strListPath;
					strcpy(&sDemo.chModelName[0],strCurrentDir.GetBuffer(0));
					
					sDemo.bTopWin = false;
					if (UINT(((COPYDATASTRUCT*)lParam)->dwData)==DHLIST_SELIMGFILECTRL)
						sInput.iOperSig = 102;
					else
						sInput.iOperSig = 101;//101正常检测图，102只显示图，103当做模板图像，104相机取图测试，105询问是否在画
					sDemo.pParent = NULL;
					sDemo.rect = NULL;
					sDemo.iCheckSN = nWorkStn-1;
					sInput.iCheckSN = nWorkStn-1;
#ifdef COMMON_DEMO_VERSION
					sInput.pParam = &sDemo;
					s_Status sError = m_multiObjManager.Set(sInput);
#endif
					
					int nImg = sError.iStatusType;
// 					if (nImg == -1)
// 						nImg = 2;
// 					else if (nImg == -2)
// 						nImg = 3;
					if (0 != nImg)
						nImg = 1;
					m_lResult.InsertString(0, sError.pErrorInfo, nImg);
					
					
				}
				
			}
			
			char chInfo[256];
			ZeroMemory(chInfo, 256);
			g_pGetTextFromLPK(chInfo, 19, nWorkStn);
			GetDlgItem(IDC_STATIC_INFO)->SetWindowText(chInfo);

			return LRESULT(1);
		}
		if (UINT(((COPYDATASTRUCT*)lParam)->dwData) == DHLIST_CHNMODEL)//切换模板
		{	
			if (GetIfDrawing())
			{
				char chText[256];
				LoadString(g_hLanguage, 5020, chText, 256);
				MessageBox(chText, "DEMO", MB_ICONWARNING);
				return LRESULT(0);
			}

			char chInfo[256];
			g_pGetTextFromLPK(chInfo, 23, m_strModelNameUsing.GetBuffer(0), (char*)((COPYDATASTRUCT*)lParam)->lpData);

 			m_strModelNameUsing = (char*)((COPYDATASTRUCT*)lParam)->lpData;
 			int nPage = m_Tab.GetCurSel()+1;
 			CreateTabWins(nPage);
			
			m_strCModel = m_strModelNameUsing;
			UpdateData(FALSE);
			m_bModelChanged = true;
			
			GetDlgItem(IDC_STATIC_INFO)->SetWindowText(chInfo);
			/*************************************<重新移动窗口布局>************************************[12/18/2017 ConanSteve]*/
			ModifySidebarWidth(m_nSideWidth);
			/*************************************<>************************************[12/18/2017 ConanSteve]*/
			return LRESULT(1);
		}
		if (UINT(((COPYDATASTRUCT*)lParam)->dwData) == DHLIST_RENAMEMDL)
		{	//重命名
			if (GetIfDrawing())
			{
				char chText[256];
				LoadString(g_hLanguage, 5020, chText, 256);
				MessageBox(chText, "DEMO", MB_ICONWARNING);
				return LRESULT(0);
			}

			char chInfo[256];
			g_pGetTextFromLPK(chInfo, 24, m_strModelNameUsing.GetBuffer(0), (char*)((COPYDATASTRUCT*)lParam)->lpData);

			m_strModelNameUsing = (char*)((COPYDATASTRUCT*)lParam)->lpData;
			// 再读一下模板
			int nPage = m_Tab.GetCurSel()+1;
			CreateTabWins(nPage);

			m_strCModel = m_strModelNameUsing;
			UpdateData(FALSE);

			GetDlgItem(IDC_STATIC_INFO)->SetWindowText(chInfo);

			return LRESULT(1);
		}
		if (UINT(((COPYDATASTRUCT*)lParam)->dwData) == DHLIST_REFRESHMDL)
		{	//刷新模板
			if (m_bStopperMode)
			{
				CString strMt;
				CString strStandard;
				strStandard.Format("Type%d_", m_nStopperType);
				while (1)
				{
					BOOL bFoundBad = FALSE;
					for (int ni=0; ni<m_lModelNames.GetCount(); ++ni)
					{
						m_lModelNames.GetText(ni, strMt);
						if ( strStandard.CompareNoCase(strMt.Left(strStandard.GetLength())) )
						{
							m_lModelNames.DeleteString(ni);
							bFoundBad = TRUE;
							break;
						}
					}
					if (bFoundBad == FALSE)
						break;
				}
			}
			
			return LRESULT(1);
		}
		return LRESULT(0);
	}
	return LRESULT(0);
}

void CDemoProjectDlg::OnBtnClrst() 
{
	m_lResult.ResetContent();
}

void CDemoProjectDlg::OnBtnErrimg() 
{
	if (GetIfDrawing())
	{
		char chText[256];
		LoadString(g_hLanguage, 5020, chText, 256);
		MessageBox(chText, "DEMO", MB_ICONWARNING);
		return;
	}

	int nCurPage = m_Tab.GetCurSel();
	CTime cTime = CTime::GetCurrentTime();
	char chTime[256];
	g_pGetTextFromLPK(chTime, 25, cTime);
	CString strThisDir;
	strThisDir.Format("%s%s\\%s", m_strImgPath[1], chTime, m_strImgFolder.GetAt(nCurPage));
//	AfxMessageBox(strThisDir);return;
	m_plFile[nCurPage].EnterDir(strThisDir);
}

void CDemoProjectDlg::OnBtnGrabimg() 
{
	if (GetIfDrawing())
	{
		char chText[256];
		LoadString(g_hLanguage, 5020, chText, 256);
		MessageBox(chText, "DEMO", MB_ICONWARNING);
		return;
	}

	int nCurPage = m_Tab.GetCurSel();
	CTime cTime = CTime::GetCurrentTime();
	char chTime[256];
	g_pGetTextFromLPK(chTime, 25, cTime);
	CString strThisDir;
	strThisDir.Format("%s%s\\%s", m_strImgPath[0], chTime, m_strImgFolder.GetAt(nCurPage));
//	AfxMessageBox(strThisDir);return;
	m_plFile[nCurPage].EnterDir(strThisDir);
}

void CDemoProjectDlg::OnBtnCapimg() 
{
	if (GetIfDrawing())
	{
		char chText[256];
		LoadString(g_hLanguage, 5020, chText, 256);
		MessageBox(chText, "DEMO", MB_ICONWARNING);
		return;
	}
	
	int nCurPage = m_Tab.GetCurSel();
	CTime cTime = CTime::GetCurrentTime();
	char chTime[256];
	g_pGetTextFromLPK(chTime, 25, cTime);
	CString strThisDir;
	strThisDir.Format("%s%s\\%s", m_strImgPath[2], chTime, m_strImgFolder.GetAt(nCurPage));
	m_plFile[nCurPage].EnterDir(strThisDir);
}

void CDemoProjectDlg::OnBtnImgTest() 
{
	if (GetIfDrawing())
	{
		char chText[256];
		LoadString(g_hLanguage, 5020, chText, 256);
		MessageBox(chText, "DEMO", MB_ICONWARNING);
		return;
	}

	s_AlgModelInput sInput;
	
	s_Status sResult;
	s_DemoAlgSetParam sDemo;
	sDemo.bTopWin = false;
	sInput.iOperSig = 104;
	sDemo.pParent = NULL;
	sDemo.rect = NULL;
	sDemo.iCheckSN = m_Tab.GetCurSel();

#ifdef COMMON_DEMO_VERSION
	sInput.pParam = &sDemo;
	sInput.iCheckSN = m_Tab.GetCurSel();
	sResult = m_multiObjManager.Set(sInput);
#endif

	int nImg = sResult.iStatusType;
	if (nImg == -1)
		nImg = 2;
	else if (nImg == -2)
		nImg = 3;
	m_lResult.InsertString(0, sResult.pErrorInfo, nImg);
}

void CDemoProjectDlg::OnBtnUpdmdl() //加载当前模板
{
 	if (GetIfDrawing())
	{
		char chText[256];
		LoadString(g_hLanguage, 5020, chText, 256);
		MessageBox(chText, "DEMO", MB_ICONWARNING);
		return;
	}

	char chMsg[256];
	g_pGetTextFromLPK(chMsg, 26, m_strModelNameUsing.GetBuffer(0));
	if (MessageBox(chMsg, "DEMO", MB_YESNO|MB_ICONQUESTION)==IDYES)
	{
		//先保存模板？
		bool bNeedSave = GetIfNeedSave() && g_bSaveEnabled;
		if (bNeedSave)
			SaveModelBeforeQuit(0);

		m_bIsWaiting = TRUE;
		WritePrivateProfileString(TEXT("Demo Controls"), TEXT("DefaultModel"), m_strModelNameUsing, m_sAlgConfigPath);
		CWnd *pOtherWnd = CWnd::FindWindow(NULL, m_strSystemName.GetBuffer(0));
		//CWnd *pOtherWnd = CWnd::FindWindow(NULL, _T("[Guid(5FB06D01 - 839C - 4CE0 - 9072 - FAFB8E084676)]"));//**[8/28/2017 ConanSteve]**:  瑞阳定制版
		if (!bNeedSave)
		{
			//不保存 则创建一下
			char chProgText[256];
			LoadString(g_hLanguage, 5021, chProgText, 256);
			g_pProgCtrl = new CMyProgressCtrl(0, 0);
			CRect ProRect;
			ProRect.left=486;
			ProRect.right=ProRect.left+200;
			ProRect.top=330;
			ProRect.bottom=ProRect.top+30;
			g_pProgCtrl->Create(WS_CHILD|WS_BORDER, ProRect, this, 0);
			g_pProgCtrl->SetRange(0,100);
			g_pProgCtrl->SetStep(10);
			g_pProgCtrl->SetWindowText(_T(chProgText));
		}
		else
		{
			char chProgText[256];
			LoadString(g_hLanguage, 5022, chProgText, 256);
			((CMyProgressCtrl*)g_pProgCtrl)->ShowPercent(FALSE);
			((CMyProgressCtrl*)g_pProgCtrl)->ChangeColor(FALSE);
			g_pProgCtrl->SetWindowText(_T(chProgText));
		}
		g_pProgCtrl->SetPos(0);
		g_pProgCtrl->ShowWindow(SW_SHOW);
		
		LRESULT copyDataResult;	
		if (pOtherWnd) 
		{
			COPYDATASTRUCT cpd;
			cpd.dwData = DEMO_CHANGE;//告诉系统读取模板
			cpd.cbData = m_strModelNameUsing.GetLength()+1;
			cpd.lpData = m_strModelNameUsing.GetBuffer(0);
			copyDataResult = pOtherWnd->SendMessage(WM_COPYDATA,
				(WPARAM)AfxGetApp()->m_pMainWnd->GetSafeHwnd(),
				(LPARAM)&cpd);
			HANDLE hThread = chBEGINTHREADEX(NULL, 0, ThreadLoadModel, this, 0, NULL);
			if (NULL != hThread)
				CloseHandle(hThread);
			m_bHandledLoadModelMsg = false;
		}

		m_uTimerWaiting = SetTimer(2, 50, NULL);
	}
}

bool CDemoProjectDlg::GetIfDrawing()
{
	s_Status sRst;
	s_AlgModelInput sIn;
	ZeroMemory(&sIn, sizeof(s_AlgModelInput));
	ZeroMemory(&sRst, sizeof(s_Status));
	sIn.iOperSig = 105;

	for (int i=0; i<m_nNumWS; i++)
	{
#ifdef COMMON_DEMO_VERSION
		sIn.iCheckSN = i;
		sRst = m_multiObjManager.Set(sIn);
#endif
		if (sRst.iStatusType==1)
			return true;
	}
	return false;
}

void CDemoProjectDlg::GetFileVersion(CString strPath, CString& strVersion)
{
	if (_access(strPath.GetBuffer(0), 0)==-1)
	{
		strVersion.Empty();
		return;
	}
	int cbInfoSize=GetFileVersionInfoSize(strPath.GetBuffer(0), NULL);   
	LPVOID Version = new char[cbInfoSize];  
	UINT cbVerSize;
	GetFileVersionInfo(strPath.GetBuffer(0), NULL, cbInfoSize, Version);
    int ma, mj, r, b;
    VS_FIXEDFILEINFO *VInfo;
    if(VerQueryValue(Version, "\\", ((void**)&VInfo), (PUINT)&cbVerSize))
	{
		ma= HIWORD(VInfo->dwFileVersionMS);
		mj= LOWORD(VInfo->dwFileVersionMS);
		r=	HIWORD(VInfo->dwFileVersionLS);
		b=  LOWORD(VInfo->dwFileVersionLS);
	}
	strVersion.Format("%d.%d.%d.%d", ma, mj, r, b);
	
	delete []Version;
}

BOOL CDemoProjectDlg::OnDeviceChange(UINT nEventType, DWORD dwData)
{
	if (nEventType==DBT_DEVICEARRIVAL || nEventType==DBT_DEVICEREMOVECOMPLETE)
	{
		for (int n=0; n<m_nNumWS; ++n)
			m_plDisk[n].DeviceChanged(2001-0.25*(int)nEventType);

		if (nEventType == DBT_DEVICEREMOVECOMPLETE)	//移除的时候
		{
			char chCurPath[MAX_PATH];
			m_plFile[m_nLastPage].GetCurrentPath(&chCurPath[0]);
			GetDlgItem(IDC_EDIT_CURPATH)->SetWindowText(&chCurPath[0]);
		}
	}

	return CWnd::OnDeviceChange(nEventType, dwData);
}

BOOL CDemoProjectDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{	
// 	CRect rectt;
// 	GetDlgItem(IDC_STATIC_1)->GetWindowRect(rectt);
// 	ScreenToClient(rectt);
// 	CPoint pp;
// 	pp.x = pt.x;
// 	pp.y = pt.y;
// 	ScreenToClient(&pp);
// 	if (pp.x>=rectt.left && pp.x<=rectt.right &&
// 		pp.y>=rectt.top && pp.y<=rectt.bottom)
// 	{
// 		s_AlgModelInput sIn;
// 		s_Status sRet;
// 		sIn.iOperSig = 52;
// 		sIn.rect.top = pt.x;
// 		sIn.rect.left = pt.y;
// 		sIn.rect.bottom = zDelta;
// 		sprintf(&sIn.chModelName[0], "%d", 4);
// 
// 		sRet = g_ppCheck[m_Tab.GetCurSel()][0].SetModel(sIn);
// 
// 		return (bool)sRet.nStatusType;
// 	}
// 	else
// 	{
// 		return FALSE;
// 	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

bool CDemoProjectDlg::GetIfNeedSave()
{
	s_Status sRet;
	s_AlgModelInput sIn;
	ZeroMemory(&sIn, sizeof(s_AlgModelInput));
	sIn.iOperSig = 202;

	for (int n=0; n<m_nNumWS; ++n)
	{
#ifdef COMMON_DEMO_VERSION
		sIn.iCheckSN = n ;
		sRet = m_multiObjManager.Set(sIn);
#endif
		if (sRet.iStatusType==1) 
			return true;
	}
	return false;
}

void CDemoProjectDlg::StopParkour()
{
	g_bParkourRunning = FALSE;
	char chText[256];
	LoadString(g_hLanguage, 5057, chText, 256);
	GetDlgItem(IDC_BTN_PARKOUR)->SetWindowText(chText);
}

void CDemoProjectDlg::OnBtnParkour()
{
	if (g_bParkourRunning)
	{
		g_bParkourRunning = FALSE;
		char chText[256];
		LoadString(g_hLanguage, 5057, chText, 256);
		GetDlgItem(IDC_BTN_PARKOUR)->SetWindowText(chText);
	}
	else
	{
		CDlgParkour dlg;
		if (dlg.DoModal() == IDOK)
		{
			int nPage = m_Tab.GetCurSel();
			g_DemoParkData.Interval = dlg.m_nInterval;
			g_DemoParkData.FromSel = dlg.m_bFromSel;
			g_DemoParkData.EndlessLoop = dlg.m_bUnlimLoop;
			g_DemoParkData.StopBad = dlg.m_bStopBad;
			g_DemoParkData.StopGood = dlg.m_bStopGood;
			g_DemoParkData.StopEmp = dlg.m_bStopEmp;
			g_DemoParkData.StopExp = dlg.m_bStopExp;
			g_DemoParkData.pList = &m_plFile[nPage];
			g_DemoParkData.Workstation = nPage;

			g_bParkourRunning = TRUE;
			AfxBeginThread(ThreadFuncParkour, (void*)this);
			char chText[256];
			LoadString(g_hLanguage, 5066, chText, 256);
			GetDlgItem(IDC_BTN_PARKOUR)->SetWindowText(chText);
		}
	}

}

#ifdef COMMON_DEMO_VERSION
BOOL CDemoProjectDlg::LoadConfigs()
{
	char chbuf[256], chText[256];

	//检测动态库文件名
	m_nNumWS = GetPrivateProfileInt("Inspection Parameters", "CheckNum", 5, g_strIniPath.GetBuffer(0));
	if (m_nNumWS <= 0)
	{
		LoadString(g_hLanguage, 5037, chText, 256);
		MessageBox(_T(chText), "DEMO", MB_ICONERROR);
		return FALSE;
	}

	// 多对象初始化 [8/14/2015 sjc]
	s_Status sRet;
	s_CheckInitParam sInit;
	ZeroMemory(&sInit,sizeof(s_CheckInitParam));
	ZeroMemory(&sRet,sizeof(s_Status));

	char chFile[256];
	sInit.pFilePath = chFile;
	sInit.pFilePath=g_strIniPath.GetBuffer(0);

	//////////////////////////////////////////////////
	s_DetectInitParam sDetectInitParam[20];
	
 	int InitParam[20];
 	for (int i = 0 ; i< m_nNumWS; i++)
 	{
 
 		memset(sDetectInitParam[i].chLanPath,0,MAX_PATH);
 		memset(sDetectInitParam[i].chLanguage,0,MAX_PATH);
 
 		sprintf(sDetectInitParam[i].chLanguage,g_strLanguage);
 
 		sprintf(sDetectInitParam[i].chLanPath,g_strLanguageDir);
 		sDetectInitParam[i].iRunMode = 1;
 		sDetectInitParam[i].pHWnd = 0;
 		
 		InitParam[i] = int(&sDetectInitParam[i]);
 	}
 
 	sInit.pParam = InitParam;
	sInit.iCallType = 1;

	//////////////////////////////////////////////////

	sRet = m_multiObjManager.Init(sInit);
	if (sRet.iStatusType != STATUS_NOR)
	{
		char chInfo[256];
		ZeroMemory(chInfo,256);
		memcpy(chInfo,sRet.pErrorInfo,strlen(sRet.pErrorInfo));

		CString str;
		str.Format("多对象初始化失败-%s",chInfo);
		AfxMessageBox(str);
		exit(-1);
		return FALSE;
	}

	//文件浏览器有关控件
	m_nInitTopIndex = new int[m_nNumWS];
	m_plFile = new CDHFileBox[m_nNumWS];
	m_plDisk = new CDHDiskBox[m_nNumWS];

	CString strDllKeyName, strDllPath, strFolderKeyName, strDefaultFolderName, strCamPath;
	char chDefaultFolderName[256], chkey[32];
	for (int n=0; n<m_nNumWS; ++n)
	{
		//先读入图像文件夹名称
		strFolderKeyName.Format("ImgFolder_%d", n+1);
		sprintf(chDefaultFolderName, "Camera%d", n+1);
		GetPrivateProfileString("DEMO Controls", strFolderKeyName, chDefaultFolderName, chbuf, 256, g_strIniPath.GetBuffer(0));
		m_strImgFolder.InsertAt(n, chbuf);

		//各工位看图路径
		sprintf(chkey, "Cam%dPath", n+1);
		GetPrivateProfileString("DEMO Controls", chkey, "", chbuf, 256, g_strIniPath.GetBuffer(0));
		strCamPath = chbuf;	
		m_nInitTopIndex[n] = atoi(strCamPath.Right(3).GetBuffer(0));
		strCamPath = strCamPath.Left(strCamPath.GetLength()-3);
		m_strListPath.InsertAt(n, strCamPath);
	}

	
	//检测系统的标题
	GetPrivateProfileString("系统配置", "AppName", "检测系统", chbuf, 256, g_strCurSet.GetBuffer(0));
	m_strSystemName = chbuf;

	//系统默认模板
	GetPrivateProfileString("系统配置", "模板名称", "", chbuf, 256, g_strCheckIni.GetBuffer(0));

	m_strSysModelName = chbuf;

	char chAppChars[64];
	CString strCamCfgIni;
	GetPrivateProfileString("DEMO Controls", "GrabTitle", "采集参数", chAppChars, 64, g_strIniPath.GetBuffer(0));

 	for (int n=0; n<m_nNumWS; ++n)
 	{
 		//获取版本号
 		s_AlgModelInput sIn;// 获取第n路检测的程序版本 [8/14/2015 sjc]
 		ZeroMemory(&sIn, sizeof(s_AlgModelInput));
 		s_Status sRet;
 		sIn.iOperSig = 203;
 		sIn.iCheckSN = n;
 		sRet = m_multiObjManager.Set(sIn);
 	
 		m_strArrVersion.InsertAt(n, sRet.pErrorInfo);
 	}

	//DEMO版本号
	char chPath[MAX_PATH];
	CString strVersion;
	GetModuleFileName(NULL, chPath, MAX_PATH);
	GetFileVersion((CString)chPath, strVersion);
	m_strArrVersion.InsertAt(m_nNumWS, strVersion);
	
	return TRUE;
}
#endif



void CDemoProjectDlg::LoadControlStrings()
{
	char chText[256];

	//About
	CMenu* pSysMenu = GetSystemMenu(FALSE);  //获取系统菜单
	if (pSysMenu)
	{

		UINT uCount = pSysMenu->GetMenuItemCount();
		int nDelIndex = 0;

		if(g_nLangType==0)//g_nLangType  是语言  //1:中文  0:英文  
		{
			SetDlgItemText(IDC_CHECK_ShowModelLog, TEXT("ShowModelLog"));
			for (int n=0; n<uCount; ++n)
			{
				UINT uID = pSysMenu->GetMenuItemID(nDelIndex);
				
				if (pSysMenu->GetMenuItemID(nDelIndex)==SC_SIZE || pSysMenu->GetMenuItemID(nDelIndex)==SC_MOVE)
				{
					if(pSysMenu->GetMenuItemID(nDelIndex)==SC_SIZE)
					{
						pSysMenu->ModifyMenu(nDelIndex,MF_BYPOSITION,SC_SIZE,"Size");
					}
					
					if(pSysMenu->GetMenuItemID(nDelIndex)==SC_MOVE)
					{
						pSysMenu->ModifyMenu(nDelIndex,MF_BYPOSITION,SC_MOVE,"Move");
					}

					nDelIndex++;
				}
				else
					pSysMenu->DeleteMenu(nDelIndex, MF_BYPOSITION);
			}
		}
		else if(g_nLangType==1)
		{
			SetDlgItemText(IDC_CHECK_ShowModelLog, TEXT("显示日志"));
			for (int n=0; n<uCount; ++n)
			{
				UINT uID = pSysMenu->GetMenuItemID(nDelIndex);
				
				if (pSysMenu->GetMenuItemID(nDelIndex)==SC_SIZE || pSysMenu->GetMenuItemID(nDelIndex)==SC_MOVE)
				{
					if(pSysMenu->GetMenuItemID(nDelIndex)==SC_SIZE)
					{
						pSysMenu->ModifyMenu(nDelIndex,MF_BYPOSITION,SC_SIZE,"位置");
					}
					
					if(pSysMenu->GetMenuItemID(nDelIndex)==SC_MOVE)
					{
						pSysMenu->ModifyMenu(nDelIndex,MF_BYPOSITION,SC_MOVE,"移动");
					}
					nDelIndex++;
				}
				else
					pSysMenu->DeleteMenu(nDelIndex, MF_BYPOSITION);
			}
		}
		
		pSysMenu->AppendMenu(MF_SEPARATOR);     // 添加一条水平线
 		LoadString(g_hLanguage, 5070, chText, 256);		//锁定窗口大小    5070 是要转换成16进制？
		pSysMenu->AppendMenu(MF_STRING, IDM_WINDOW_SIZING, chText);
		pSysMenu->CheckMenuItem(IDM_WINDOW_SIZING, MF_CHECKED|MF_BYCOMMAND);
		
		LoadString(g_hLanguage, 5051, chText, 256);		//语言设置菜单
		pSysMenu->AppendMenu(MF_STRING, IDM_LANGUAGE_SETTINGS, chText);
		
		pSysMenu->AppendMenu(MF_SEPARATOR);
		LoadString(g_hLanguage, 5019, chText, 256);		//关于菜单
		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, chText);

	}


	//模板列表
	LoadString(g_hLanguage, 5003, chText, 256);
	GetDlgItem(IDC_STATIC_MLIST)->SetWindowText(chText);
	//本地图像测试区
	LoadString(g_hLanguage, 5004, chText, 256);
	GetDlgItem(IDC_STATIC_FILEBACK)->SetWindowText(chText);
	//四个按钮
	LoadString(g_hLanguage, 5005, chText, 256);
	GetDlgItem(IDC_BTN_GRABIMG)->SetWindowText(chText);

	LoadString(g_hLanguage, 5006, chText, 256);
	GetDlgItem(IDC_BTN_ERRIMG)->SetWindowText(chText);

	LoadString(g_hLanguage, 5007, chText, 256);
	GetDlgItem(IDC_BTN_CAPIMG)->SetWindowText(chText);

	LoadString(g_hLanguage, 5008, chText, 256);
	GetDlgItem(IDC_BTN_CLRST)->SetWindowText(chText);
	//新建模板
	LoadString(g_hLanguage, 5009, chText, 256);
	GetDlgItem(IDC_STATIC_KJ)->SetWindowText(chText);
	LoadString(g_hLanguage, 5010, chText, 256);
	GetDlgItem(IDC_STATIC_NEWNAME)->SetWindowText(chText);
	LoadString(g_hLanguage, 5017, chText, 256);
	GetDlgItem(IDC_NEWMODEL)->SetWindowText(chText);
	//删除模板
	LoadString(g_hLanguage, 5011, chText, 256);
	GetDlgItem(IDC_BTN_DELETE)->SetWindowText(chText);
	//设为默认
	LoadString(g_hLanguage, 5012, chText, 256);
	GetDlgItem(IDC_BTN_SETD)->SetWindowText(chText);
	//保存模板
	LoadString(g_hLanguage, 5013, chText, 256);
	GetDlgItem(IDC_BTN_SAVESINGLEMODEL)->SetWindowText(chText);
	LoadString(g_hLanguage, 5014, chText, 256);
	GetDlgItem(IDC_BTN_SAVEMODEL)->SetWindowText(chText);
	//退出
	LoadString(g_hLanguage, 5015, chText, 256);
	GetDlgItem(IDC_BTN_EXIT2)->SetWindowText(chText);
	//加载模板
	LoadString(g_hLanguage, 5016, chText, 256);
	GetDlgItem(IDC_BTN_UPDMDL)->SetWindowText(chText);
	//模板列表
	LoadString(g_hLanguage, 5003, chText, 256);
	GetDlgItem(IDC_BTN_MLIST)->SetWindowText(chText);
	//一键检测
	LoadString(g_hLanguage, 5057, chText, 256);
	GetDlgItem(IDC_BTN_PARKOUR)->SetWindowText(chText);

	//胶塞类型  我这暂时用不到
	int nStopperTypeCount = ((CComboBox*)GetDlgItem(IDC_COMBO_STOPPERTYPE))->GetCount();
	int nCurSel = ((CComboBox*)GetDlgItem(IDC_COMBO_STOPPERTYPE))->GetCurSel();
	LoadString(g_hLanguage, 5069, chText, 256);
	int nMidIndex = strlen(chText);
	if (nStopperTypeCount > 0)
	{
		int *pType = new int[nStopperTypeCount+1];
		CString strType;
		for (int n=0; n<nStopperTypeCount; ++n)
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_STOPPERTYPE))->GetLBText(n, strType);
			pType[n] = atoi(strType.Mid(nMidIndex).GetBuffer(0));
		}
		((CComboBox*)GetDlgItem(IDC_COMBO_STOPPERTYPE))->ResetContent();

		CString strNewType;
		for (int n=0; n<nStopperTypeCount; ++n)
		{
			strNewType.Format("%s%d", chText, pType[n]);
			((CComboBox*)GetDlgItem(IDC_COMBO_STOPPERTYPE))->InsertString(n, strNewType.GetBuffer(0));
		}
		((CComboBox*)GetDlgItem(IDC_COMBO_STOPPERTYPE))->SetCurSel(nCurSel);

		delete [] pType;
	}


}

void CDemoProjectDlg::OnBtnLanguage() 
{
	if (GetIfDrawing())
	{
		char chText[256];
		LoadString(g_hLanguage, 5020, chText, 256);
		MessageBox(chText, "DEMO", MB_ICONWARNING);
		return;
	}

	CDlgLangSel dlg_ls;
	dlg_ls.m_pDlgMainWindowLan = this; // 子窗口类的指针变量 指向父窗口
	int nID = dlg_ls.DoModal();
	if (nID > 1000)
	{
		char chDllName[MAX_PATH];
		dlg_ls.GetDllName(chDllName, nID);
		CString strLangDll = chDllName;

		if (strLangDll.Compare("DEFAULT") != 0)//英文
		{
			if (strLangDll.GetLength() > 4)
			{
				if (strLangDll.Right(4).Compare(".dlk")!=0)
					strLangDll += ".dlk";
			}
			else
			{
				strLangDll += ".dlk";
			}
			
			CString strLangDllPath;
			strLangDllPath.Format("%s\\%s", g_strAppDir, strLangDll);
			if (_access(strLangDllPath.GetBuffer(0), 0) == -1)
			{
				if (g_nLangType == 1)
					MessageBox("设置失败\r\n找不到语言文件！", "DEMO", MB_ICONWARNING);
				else
					MessageBox("Failed!\r\nCan't Find Language File!", "DEMO", MB_ICONWARNING);
			}
			else
			{
				HMODULE hTemp = g_hLanguage;
				pFunc_GetTextFromLPK pFuncTemp = g_pGetTextFromLPK;
				CString strLanguage = g_strLanguage;
				int nLangType = g_nLangType;

				g_hLanguage = LoadLibrary(strLangDll);
				if (g_hLanguage)
				{
					char chInfo[256];
					int nRet = LoadString(g_hLanguage, 5000, chInfo, 256);
					if (nRet > 0)
					{
						g_strLanguage = chInfo;
						if (g_strLanguage.Compare("中文") == 0)
							g_nLangType = 1;
						else if (g_strLanguage.Compare("English") == 0)
							g_nLangType = 0;
						else
							g_nLangType = 1;
						
						g_pGetTextFromLPK = (pFunc_GetTextFromLPK)(GetProcAddress(g_hLanguage, "GetText"));
						if (!g_pGetTextFromLPK)
						{
							if (g_nLangType == 1)
								MessageBox("非法的语言文件！\r\n将使用默认语言(中文)", "DEMO", MB_ICONWARNING);
							else
								MessageBox("Illegal Language File!\r\nThe Default Language (Chinese) Will Be Used", "DEMO", MB_ICONWARNING);

							FreeLibrary(g_hLanguage);
							g_hLanguage = hTemp;
							g_pGetTextFromLPK = pFuncTemp;
							g_strLanguage = strLanguage;
							g_nLangType = nLangType;
						}
						else	//成功的唯一途径
						{
							WritePrivateProfileString("DEMO Controls", "Language", strLangDll.GetBuffer(0), g_strIniPath.GetBuffer(0));
							LoadControlStrings();

							char chTitle[256];
							LoadString(g_hLanguage, 5001, chTitle, 256);
							SetWindowText(chTitle);
							WritePrivateProfileString(_T("系统配置"), "DemoName", chTitle, g_strCurSet.GetBuffer(0));

							LoadString(g_hLanguage, 5072, chTitle, 256);
							SetWindowText(chTitle);
							WritePrivateProfileString(_T("系统配置"), "AppName", chTitle, g_strCurSet.GetBuffer(0));
							WritePrivateProfileString(_T("系统配置"), "窗口标题", chTitle, g_strCheckPath.GetBuffer(0));

							for (int n=0; n<m_nNumWS; ++n)
							{
								m_plDisk[n].Init(NULL, NULL, -1, NULL, g_nLangType);
							}
							m_lModelNames.Init(NULL, g_nLangType);
						}
					}
					else
					{
						if (g_nLangType == 1)
							MessageBox("非法的语言文件！\r\n将使用默认语言(中文)", "DEMO", MB_ICONWARNING);
						else
							MessageBox("Illegal Language File!\r\nThe Default Language (Chinese) Will Be Used", "DEMO", MB_ICONWARNING);

						FreeLibrary(g_hLanguage);
						g_hLanguage = hTemp;
					}
					
				}
				else
				{
					DWORD dwError = GetLastError();
					CString strMsg;
					if (g_nLangType == 1)
						strMsg.Format("加载语言文件失败！\r\n错误代码：%d", (int)dwError);
					else
						strMsg.Format("Can't Load Language File!\r\nError Code: %d", (int)dwError);
					MessageBox(strMsg.GetBuffer(0), "DEMO", MB_ICONWARNING);

					g_hLanguage = hTemp;
				}
			}
		}
		else
		{
			g_hLanguage = NULL;
			g_pGetTextFromLPK = (pFunc_GetTextFromLPK)(GetProcAddress(g_hLanguage, "GetText"));
			g_nLangType = 1;
			g_strLanguage = "中文";
			WritePrivateProfileString("DEMO Controls", "Language", strLangDll.GetBuffer(0), g_strIniPath.GetBuffer(0));

			char chTitle[256];
			LoadString(g_hLanguage, 5001, chTitle, 256);
			SetWindowText(chTitle);
			WritePrivateProfileString(_T("系统配置"), "DemoName", chTitle, g_strCurSet.GetBuffer(0));

			LoadString(g_hLanguage, 5072, chTitle, 256);
			SetWindowText(chTitle);
			WritePrivateProfileString(_T("系统配置"), "AppName", chTitle, g_strCurSet.GetBuffer(0));
			WritePrivateProfileString(_T("系统配置"), "窗口标题", chTitle, g_strCheckPath.GetBuffer(0));


			LoadControlStrings();
			for (int n=0; n<m_nNumWS; ++n)
			{
				m_plDisk[n].Init(NULL, NULL, -1, NULL, g_nLangType);
			}
			m_lModelNames.Init(NULL, g_nLangType);
		}
	}
}

void CDemoProjectDlg::OnBtnInsp() 
{
	if (GetIfDrawing())
	{
		char chText[256];
		LoadString(g_hLanguage, 5020, chText, 256);
		MessageBox(chText, "DEMO", MB_ICONWARNING);
		return;
	}

 	CDlgInspSet dlg;
	dlg.m_nPreNumWS = m_nNumWS;
	if (IDOK == dlg.DoModal())
	{
		m_bChangeType = true;
		CFile file;
		file.Open("D:\\tmp.vbs", CFile::modeCreate|CFile::modeWrite);
		char chPath[MAX_PATH];
		GetModuleFileName(NULL, chPath, MAX_PATH);
		CString strCommand;
		strCommand.Format("Set WshShell = WScript.CreateObject(\"WScript.Shell\")\r\nWScript.Sleep 2000\r\nWshShell.Run \"\"\"%s\"\"\",,false", chPath);
		file.Write(strCommand.GetBuffer(0), strCommand.GetLength()+1);
		file.Close();
		CGeneralFunc_Yang::EnableProtectionFile("D:\\tmp.vbs");

 		OnOK();
	}
}

UINT ThreadFuncParkour(LPVOID pParam)
{
	CDemoProjectDlg* pDlg = (CDemoProjectDlg*)pParam;
	if (g_DemoParkData.pList->GetCount() < 1)
	{
		pDlg->StopParkour();
		return 0;
	}

	//Loop1
	int nStart = 0;
	if (g_DemoParkData.FromSel)
	{
		nStart = g_DemoParkData.pList->GetCaretIndex();
		if (nStart < 0)
			nStart = 0;
	}
	for (int n=nStart; n<g_DemoParkData.pList->GetCount(); ++n)
	{
		if (!g_bParkourRunning)
			break;

		g_DemoParkData.pList->SetCurSel(n);
		char chCurPath[MAX_PATH];
		if (g_DemoParkData.pList->GetSelectedPath(chCurPath))
		{
			if (n==g_DemoParkData.pList->GetCount()-1 && g_DemoParkData.EndlessLoop)
				n = -1;
			continue;
		}


		s_Status sError;
		sError.iStatusType = g_DemoParkData.Workstation;
		sError.pErrorInfo = chCurPath;
		LRESULT Ret = SendMessage(AfxGetApp()->GetMainWnd()->GetSafeHwnd(), THREAD_CHECK_IMAGE, WPARAM(0), LPARAM(&sError));

		
		//0:Good 1:Bad -1:Emp -2:Exp
		if (sError.iStatusType==0 && g_DemoParkData.StopGood)
			break;
		if (sError.iStatusType==1 && g_DemoParkData.StopBad)
			break;
		if (sError.iStatusType==-1 && g_DemoParkData.StopEmp)
			break;
		if (sError.iStatusType==-2 && g_DemoParkData.StopExp)
			break;
		
		if (g_DemoParkData.Interval > 0)
			Sleep(g_DemoParkData.Interval);

		if (n==g_DemoParkData.pList->GetCount()-1 && g_DemoParkData.EndlessLoop)
			n = -1;
	}
	
	pDlg->StopParkour();
	return 1;
}

void CDemoProjectDlg::OnBtnExit2() 
{
	SaveSystemConfig();
	if (GetIfDrawing())
	{
		char chText[256];
		LoadString(g_hLanguage, 5020, chText, 256);
		MessageBox(chText, "DEMO", MB_ICONWARNING);
		return;
	}

	BOOL bNeedSave = TRUE;
	try
	{	
		bNeedSave = GetIfNeedSave();
	}
	catch(...)
	{
		;
	}
	bNeedSave = bNeedSave && g_bSaveEnabled;
	CString sMsg;
	sMsg.Format(_T("%d  %d "), m_nOpenMode, bNeedSave);
	//MessageBox(sMsg);
	if ((m_nOpenMode==1 || bNeedSave)&&m_bShowLoadModalDlg)	//通过系统打开或者需要保存  弹出退出对话框
	{
		CDlgExit dlgExit;
		dlgExit.m_nClass = m_nOpenMode;
		dlgExit.m_bLoadDisable = m_bDisableLoad;
		if (m_bChangeType)
			dlgExit.m_bCancelEnable = FALSE;

		int nSel = dlgExit.DoModal();
		
		int nFlg = 0;
		if (nSel == 0)	//取消
		{
			return;
		}
		else if (nSel == 1)	//加载 保存
		{
			if (bNeedSave)
			{
				SaveModelBeforeQuit(1-m_nOpenMode);	//系统打开的 无需弹出加载成功的对话框 因为还需等待系统那边的回复
			}
			if (m_nOpenMode==1)	//from system
			{
				m_bIsWaiting = TRUE;
				if (!bNeedSave)
				{
					char chText[256];
					g_pProgCtrl = new CMyProgressCtrl(0, 0);
					CRect ProRect;
					ProRect.left=486;
					ProRect.right=ProRect.left+200;
					ProRect.top=330;
					ProRect.bottom=ProRect.top+30;
					g_pProgCtrl->Create(WS_CHILD|WS_BORDER, ProRect, this, 0);
					g_pProgCtrl->SetRange(0,100);
					g_pProgCtrl->SetStep(10);
					LoadString(g_hLanguage, 5021, chText, 256);
					g_pProgCtrl->SetWindowText(chText);
				}
				else
				{
					char chText[256];
					((CMyProgressCtrl*)g_pProgCtrl)->ShowPercent(FALSE);
					((CMyProgressCtrl*)g_pProgCtrl)->ChangeColor(FALSE);
					LoadString(g_hLanguage, 5022, chText, 256);
					g_pProgCtrl->SetWindowText(chText);
				}
				g_pProgCtrl->SetPos(0);
				g_pProgCtrl->ShowWindow(SW_SHOW);

				nFlg = 1;
				m_bAskingSystem = true;
				LRESULT copyDataResult;	
				CWnd *pOtherWnd = CWnd::FindWindow(NULL, m_strSystemName.GetBuffer(0));
				//CWnd *pOtherWnd = CWnd::FindWindow(NULL, _T("[Guid(5FB06D01 - 839C - 4CE0 - 9072 - FAFB8E084676)]"));//**[8/28/2017 ConanSteve]**:  瑞阳定制版
				if (pOtherWnd) 
				{					
					COPYDATASTRUCT cpd;
					cpd.dwData = DEMO_CHANGE;//告诉系统读取模板
					cpd.cbData = m_strModelNameUsing.GetLength()+1;
					cpd.lpData = m_strModelNameUsing.GetBuffer(0);
						copyDataResult = pOtherWnd->SendMessage(WM_COPYDATA,
							(WPARAM)AfxGetApp()->m_pMainWnd->GetSafeHwnd(),
							(LPARAM)&cpd);
						HANDLE hThread = chBEGINTHREADEX(NULL, 0, ThreadLoadModel, this, 0, NULL);
						if (NULL != hThread)
							CloseHandle(hThread);
					m_bHandledLoadModelMsg = false;
				}
				m_uTimerWaiting = SetTimer(2, 50, NULL);
			}
		}
		else if (nSel == 2)	//直接退出
		{
			;
		}
		
		//写入四件事！////////////////////////////////////////////////////////////
		WriteOpenParas();
		
		if (nFlg == 1)
			return;
	}
	else	//双击打开且无需保存
	{
		WriteOpenParas();
	}

	FreeMemories();
	if (m_bChangeType)
		ShellExecute(NULL, "open", "D:\\tmp.vbs", NULL, NULL, SW_SHOW);
	OnOK();
}

void CDemoProjectDlg::WriteOpenParas()
{
	char chBuf[256];

	//写默认模板
	if (m_nOpenMode==0)
		WritePrivateProfileString("DEMO Controls", "DefaultModel", m_strModelNameUsing.GetBuffer(0), g_strIniPath);

	//写入尺寸
	CRect rectWnd;
	if (m_bF11Pressed)
		rectWnd = m_CurWndSize;
	else
		GetWindowRect(rectWnd);
	sprintf(chBuf, "%d", rectWnd.Width());
	WritePrivateProfileString("DEMO Controls", "WindowWidth", chBuf, g_strIniPath);
	sprintf(chBuf, "%d", rectWnd.Height());
	WritePrivateProfileString("DEMO Controls", "WindowHeight", chBuf, g_strIniPath);
	sprintf(chBuf, "%d", m_bSidebarHide);
	WritePrivateProfileString("DEMO Controls", "SidebarHide", chBuf, g_strIniPath);
	if (m_bSidebarHide)
		sprintf(chBuf, "%d", m_nSideWidthTemp);
	else
		sprintf(chBuf, "%d", m_nSideWidth);
	WritePrivateProfileString("DEMO Controls", "SidebarWidth", chBuf, g_strIniPath);

	

	//写默认页面
	CString strWrite;
	strWrite.Format("%d",m_nLastPage);
	WritePrivateProfileString("DEMO Controls", "DefaultTab", strWrite.GetBuffer(0), g_strIniPath);
	
	//写默认路径
	for (int n=0; n<m_nNumWS; ++n)
	{
		char chPathtemp[MAX_PATH],  chTemp[MAX_PATH+50];
		m_plFile[n].GetCurrentPath(&chPathtemp[0]);
		int nIndex = m_plFile[n].GetTopIndex();
		sprintf(chTemp, "%s%3d", chPathtemp, nIndex);
		CString strKeyName;
		strKeyName.Format("Cam%dPath", n+1);
		WritePrivateProfileString("DEMO Controls", strKeyName.GetBuffer(0), &chTemp[0], g_strIniPath);
	}
	
	//写胶塞模式
// 	char chSM[8];
// 	sprintf(chSM, "%d", m_bStopperMode);
// 	WritePrivateProfileString("DEMO Controls", "StopperMode", chSM, g_strIniPath);
}

void CDemoProjectDlg::ConfigInterCtrls()
{
	char chBuf[256];
	m_NewName.m_TextColor = RGB(255,0,0);
	m_bDisableCreate = GetPrivateProfileInt("DEMO Controls", "DisableCreate", 0, g_strIniPath.GetBuffer(0));
	if (m_bDisableCreate)
	{
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_NEWMODEL)->EnableWindow(FALSE);
	}
	m_bDisableSetDef = GetPrivateProfileInt("DEMO Controls", "DisableSetDef", 0, g_strIniPath.GetBuffer(0));
	if (m_bDisableSetDef)
	{
		GetDlgItem(IDC_BTN_SETD)->EnableWindow(FALSE);
	}
	m_bDisableDelete = GetPrivateProfileInt("DEMO Controls", "DisableDelete", 0, g_strIniPath.GetBuffer(0));
	if (m_bDisableDelete)
	{
		GetDlgItem(IDC_BTN_DELETE)->EnableWindow(FALSE);
	}
	m_bDisableLoad = GetPrivateProfileInt("DEMO Controls", "DisableLoad", 0, g_strIniPath.GetBuffer(0));
	if (m_bDisableLoad)
	{
		GetDlgItem(IDC_BTN_UPDMDL)->EnableWindow(FALSE);
	}
	m_bDisableSave = GetPrivateProfileInt("DEMO Controls", "DisableSave", 0, g_strIniPath.GetBuffer(0));
	if (m_bDisableSave)
	{
		GetDlgItem(IDC_BTN_SAVEMODEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SAVESINGLEMODEL)->EnableWindow(FALSE);
	}
	m_bDisableImgBtn[0] = GetPrivateProfileInt("DEMO Controls", "DisableImgBtn1", 0, g_strIniPath.GetBuffer(0));
	if (m_bDisableImgBtn[0])
	{
		GetDlgItem(IDC_BTN_GRABIMG)->EnableWindow(FALSE);
	}
	m_bDisableImgBtn[1] = GetPrivateProfileInt("DEMO Controls", "DisableImgBtn2", 0, g_strIniPath.GetBuffer(0));
	if (m_bDisableImgBtn[1])
	{
		GetDlgItem(IDC_BTN_ERRIMG)->EnableWindow(FALSE);
	}
	m_bDisableImgBtn[2] = GetPrivateProfileInt("DEMO Controls", "DisableImgBtn3", 0, g_strIniPath.GetBuffer(0));
	if (m_bDisableImgBtn[2])
	{
		GetDlgItem(IDC_BTN_CAPIMG)->EnableWindow(FALSE);
	}

	//图像按钮控件
	GetPrivateProfileString("DEMO Controls", "ImgBtn1Text", _T("定位到连续图像目录"), chBuf, 256, g_strIniPath.GetBuffer(0));
//	GetDlgItem(IDC_BTN_GRABIMG)->SetWindowText(chBuf);
	GetPrivateProfileString("DEMO Controls", "ImgBtn1Path", _T("F:\\SaveImages\\GrabImages"), chBuf, 256, g_strIniPath.GetBuffer(0));
	m_strImgPath[0] = chBuf;
	if (m_strImgPath[0].Right(1) != "\\")
		m_strImgPath[0] += "\\";
	GetPrivateProfileString("DEMO Controls", "ImgBtn2Text", _T("定位到错误图像目录"), chBuf, 256, g_strIniPath.GetBuffer(0));
//	GetDlgItem(IDC_BTN_ERRIMG)->SetWindowText(chBuf);
	GetPrivateProfileString("DEMO Controls", "ImgBtn2Path", _T("F:\\SaveImages\\ErrorImages"), chBuf, 256, g_strIniPath.GetBuffer(0));
	m_strImgPath[1] = chBuf;
	if (m_strImgPath[1].Right(1) != "\\")
		m_strImgPath[1] += "\\";
	GetPrivateProfileString("DEMO Controls", "ImgBtn3Text", _T("定位到有胶囊图像目录"), chBuf, 256, g_strIniPath.GetBuffer(0));
//	GetDlgItem(IDC_BTN_CAPIMG)->SetWindowText(chBuf);
	GetPrivateProfileString("DEMO Controls", "ImgBtn3Path", _T("F:\\SaveImages\\CapsuleImages"), chBuf, 256, g_strIniPath.GetBuffer(0));
	m_strImgPath[2] = chBuf;
	if (m_strImgPath[2].Right(1) != "\\")
		m_strImgPath[2] += "\\";


	//胶塞的Combo
	if (m_bStopperMode)
	{
		GetDlgItem(IDC_COMBO_STOPPERTYPE)->ShowWindow(SW_SHOW);
		int nCount = 0;
		CFileFind finder;
		BOOL bRet = finder.FindFile(g_strAppDir+"\\Type*.*");

		int nSel = 0;
		char chText[256]; ZeroMemory(&chText, 256);

		while (bRet)
		{
			bRet = finder.FindNextFile();
			if (finder.IsDirectory())
			{
				if (finder.GetFileName().Left(4)=="Type" && finder.GetFileName().GetLength()>4)
				{
					CString strType;
					int nType = atoi(finder.GetFileName().Mid(4));
					LoadString(g_hLanguage, 5069, chText, 256);
					strType.Format("%s%d", chText, nType);
					((CComboBox*)GetDlgItem(IDC_COMBO_STOPPERTYPE))->InsertString(nCount, strType.GetBuffer(0));

					if (m_nStopperType == nType)
						nSel = nCount;

					nCount++;
				}
			}
		}

		if (nCount > 0)
		{
			((CComboBox*)GetDlgItem(IDC_COMBO_STOPPERTYPE))->SetCurSel(nSel);
		}
	}
}

void CDemoProjectDlg::OnSelchangeComboStoppertype() 
{
#ifdef COMMON_DEMO_VERSION
	if (GetIfDrawing())
	{
		char chText[256];
		LoadString(g_hLanguage, 5020, chText, 256);
		MessageBox(chText, "DEMO", MB_ICONWARNING);
		return;
	}

	char chText[256]; ZeroMemory(&chText, 256);
	LoadString(g_hLanguage, 5069, chText, 256);
	int nMidIndex = strlen(chText);

	int nIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_STOPPERTYPE))->GetCurSel();
	if (nIndex >= 0)
	{
		CString strType;
		((CComboBox*)GetDlgItem(IDC_COMBO_STOPPERTYPE))->GetLBText(nIndex, strType);
		int nNewType = atoi(strType.Mid(nMidIndex));
		/************************************************************************/
		/*                       [6/5/2017 ConanSteve]                         */
		CString sNewType{ TEXT("") };
		sNewType.Format(TEXT("%d"), nNewType);
		WritePrivateProfileString(TEXT("Demo Controls"), TEXT("StopperType"), sNewType, g_strAppDir + TEXT("\\Config\\AlgConfig.ini"));
		/************************************************************************/
		/*修改默认模板名称                       [6/5/2017 ConanSteve]                         */
		CFileFind fileFind;
		CString sFindModelName{ TEXT("") };
		sFindModelName.Format(TEXT("Type%d_*.*"), nNewType);
		bool bRet = fileFind.FindFile(g_strAppDir + TEXT("\\ModelInfo\\") + sFindModelName);
		if (bRet)
		{
			fileFind.FindNextFile();
			if (fileFind.IsDirectory())
			{
				CString sNewModelName = fileFind.GetFileName();
				WritePrivateProfileString(TEXT("Demo Controls"), TEXT("DefaultModel"), sNewModelName, g_strAppDir + TEXT("\\Config\\AlgConfig.ini"));
			}
		}

		/************************************************************************/
		/************************************************************************/
		if (m_nStopperType != nNewType)
		{
			// 询问是否保存模板
			char chText[256];
			LoadString(g_hLanguage, 5068, chText, 256);
			int nAnswer = MessageBox(chText, NULL, MB_ICONQUESTION|MB_YESNOCANCEL);
			if (nAnswer == IDCANCEL)
			{
				LoadString(g_hLanguage, 5069, chText, 256);
				CString strStopperType;
				strStopperType.Format("%s%d", chText, m_nStopperType);
				int nOrigIndex = ((CComboBox*)GetDlgItem(IDC_COMBO_STOPPERTYPE))->FindStringExact(0, strStopperType.GetBuffer(0));
				if(nOrigIndex>=0)((CComboBox*)GetDlgItem(IDC_COMBO_STOPPERTYPE))->SetCurSel(nOrigIndex);
				return;
			}
			else if (nAnswer == IDYES)
			{
				//不允许有其他动作
				m_bIsWaiting = TRUE;

				//进度条
				char chText[256];
				LoadString(g_hLanguage, 5023, chText, 256);
				g_pProgCtrl = new CMyProgressCtrl(1, 1);
				CRect ProRect;
				ProRect.left=486;
				ProRect.right=ProRect.left+200;
				ProRect.top=330;
				ProRect.bottom=ProRect.top+30;
				g_pProgCtrl->Create(WS_CHILD|WS_BORDER, ProRect, this, 0);
				g_pProgCtrl->SetRange(0,m_nNumWS*2);
				g_pProgCtrl->SetStep(1);
				g_pProgCtrl->SetWindowText(chText);
				g_pProgCtrl->ShowWindow(SW_SHOW);

				//开始保存
				s_AlgModelInput sModelIn;
			
				s_Status sRst;
			
				int* nSuccess = new int[m_nNumWS];
				ZeroMemory(nSuccess, sizeof(int)*m_nNumWS);
				for (int n=0; n<m_nNumWS; ++n)
				{
					s_DemoAlgSetParam sDemo;	
					strcpy(sDemo.chModelName, m_strModelNameUsing);
					if (m_bNewCreateModel)
						sModelIn.iOperSig = 41;
					else
					sModelIn.iOperSig = 4;
					sDemo.iCheckSN = n;
					sModelIn.iCheckSN = n;
					sModelIn.pParam = &sDemo;
					sRst = m_multiObjManager.Set(sModelIn);
					g_pProgCtrl->StepIt();
					nSuccess[n] = nSuccess[n]|(!sRst.iStatusType);

					sModelIn.iOperSig = 5;
					sRst = m_multiObjManager.Set(sModelIn);
					g_pProgCtrl->StepIt();
					nSuccess[n] = nSuccess[n]|((!sRst.iStatusType)*2);
				}

				CString strFailNum;
				BOOL bSus = TRUE;
				for (int n=0; n<m_nNumWS; ++n)
				{
					if (nSuccess[n]&2==0 || nSuccess[n]&1==0)//失败啦 收衣服哇
					{
						CString strThisNum;
						strThisNum.Format("%d,", n+1);
						strFailNum += strThisNum;
						bSus = FALSE;
					}
				}
				if (m_nNumWS == 1)
					delete nSuccess;
				else
					delete [] nSuccess;

			//	CWnd *pBMWnd = CWnd::FindWindow(NULL, "BackupManager");
				if (bSus)	//成功
				{
					LoadString(g_hLanguage, 5024, chText, 256);
					((CMyProgressCtrl*)g_pProgCtrl)->SetWindowText(chText);
					((CMyProgressCtrl*)g_pProgCtrl)->ShowPercent(FALSE);

			// 		if (pBMWnd)
			// 			pBMWnd->PostMessage(WM_DEMO_MSG, WPARAM(1), LPARAM(this));
			// 		else
						m_bIsWaiting = FALSE;	//无其他工作了

					g_pGetTextFromLPK(chText, 8, m_strModelNameUsing.GetBuffer(0));
					GetDlgItem(IDC_STATIC_INFO)->SetWindowText(chText);

					Sleep(300);
				}
				else	//失败
				{
					LoadString(g_hLanguage, 5026, chText, 256);
					((CMyProgressCtrl*)g_pProgCtrl)->SetWindowText(chText);
					
					g_pGetTextFromLPK(chText, 9, m_strModelNameUsing.GetBuffer(0));
					GetDlgItem(IDC_STATIC_INFO)->SetWindowText(chText);

					g_pGetTextFromLPK(chText, 10, strFailNum.Left(strFailNum.GetLength()-1).GetBuffer(0));
					m_bIsWaiting = FALSE;

					AfxMessageBox(chText);
				}

				g_pProgCtrl->ShowWindow(SW_HIDE);
				delete g_pProgCtrl;
				g_pProgCtrl = NULL;

				m_bNewCreateModel = false;
			}

			//告知要修改
			s_AlgModelInput sIn;
			s_DemoAlgSetParam sDemo;
			ZeroMemory(&sIn, sizeof(s_AlgModelInput));
			sIn.iOperSig = 400;
			sprintf(sDemo.chModelName, "%d", nNewType);
			sDemo.iCheckSN = 0;
			sIn.iCheckSN = 0;
			sIn.pParam = &sDemo;
			m_multiObjManager.Set(sIn);


			
			//重启
			WriteOpenParas();
			FreeMemories();
			CFile file;
			file.Open("D:\\tmp.vbs", CFile::modeCreate|CFile::modeWrite);
			char chPath[MAX_PATH];
			GetModuleFileName(NULL, chPath, MAX_PATH);
			CString strCommand;
			strCommand.Format("Set WshShell = WScript.CreateObject(\"WScript.Shell\")\r\nWScript.Sleep 2000\r\nWshShell.Run \"\"\"%s\"\"\",,false", chPath);
			file.Write(strCommand.GetBuffer(0), strCommand.GetLength()+1);
			file.Close();
			CGeneralFunc_Yang::EnableProtectionFile("D:\\tmp.vbs");
			ShellExecute(NULL, "open", "D:\\tmp.vbs", NULL, NULL, SW_SHOW);
			OnOK();
		}
	}
#endif
}

void CDemoProjectDlg::GetAllRatios()
{
	CRect rectCWnd;
	GetClientRect(rectCWnd);

	int nMainWndWidth = rectCWnd.Width() - m_nSideWidth;
	m_fSideWidth = m_nSideWidth*1.0/rectCWnd.Width();
	
	CRect CtrlRect;
	// Main Window
	for (int n1=0; n1<m_MainCtrl.size(); ++n1)
	{
		GetDlgItem(m_MainCtrl[n1])->GetWindowRect(CtrlRect);	ScreenToClient(CtrlRect);
		m_pMainCtrlPR[n1].fTop = (CtrlRect.top)*1.0/rectCWnd.Height();
		m_pMainCtrlPR[n1].fBottom = (CtrlRect.bottom)*1.0/rectCWnd.Height();
		m_pMainCtrlPR[n1].fLeft = (CtrlRect.left)*1.0/nMainWndWidth;
		m_pMainCtrlPR[n1].fRight = (CtrlRect.right)*1.0/nMainWndWidth;
	}
	
	// Sidebar
	for (int n2=0; n2<m_SideCtrl.size(); ++n2)
	{
		GetDlgItem(m_SideCtrl[n2])->GetWindowRect(CtrlRect);	ScreenToClient(CtrlRect);
		m_pSideCtrlPR[n2].fTop = (CtrlRect.top)*1.0/rectCWnd.Height();
		m_pSideCtrlPR[n2].fBottom = (CtrlRect.bottom)*1.0/rectCWnd.Height();
		m_pSideCtrlPR[n2].fLeft = (CtrlRect.left-nMainWndWidth)*1.0/m_nSideWidth;
		m_pSideCtrlPR[n2].fRight = (CtrlRect.right-nMainWndWidth)*1.0/m_nSideWidth;
	}
}

void CDemoProjectDlg::ScaleMoveWindow()
{
	CRect rectWnd;
	GetClientRect(rectWnd);

	int nMainWndWidth;

	if (m_bSidebarHide)
	{
		m_nSideWidthTemp = rectWnd.Width()*m_fSideWidth;
		nMainWndWidth = rectWnd.Width() - m_nSideWidth;
	}
	else
	{
		m_nSideWidth = rectWnd.Width()*m_fSideWidth;
		nMainWndWidth = rectWnd.Width() - m_nSideWidth;
	}

	
	CRect CtrlRect;
	// 主窗口
	CRect rectDlg;
	for (int n1=0; n1<m_MainCtrl.size(); ++n1)
	{
// 		if (n1 = 2)
// 			continue;
		CtrlRect.top = (rectWnd.Height()*m_pMainCtrlPR[n1].fTop);
		CtrlRect.bottom = (rectWnd.Height()*m_pMainCtrlPR[n1].fBottom);
		CtrlRect.left = nMainWndWidth*m_pMainCtrlPR[n1].fLeft;
		CtrlRect.right = nMainWndWidth*m_pMainCtrlPR[n1].fRight;
		GetDlgItem(m_MainCtrl[n1])->MoveWindow(CtrlRect);
		
		if (m_MainCtrl[n1] == IDC_STATIC_1)
		{
			rectDlg = CtrlRect;
		}
	}
	
	// 侧边栏
	for (int n2=0; n2<m_SideCtrl.size(); ++n2)
	{
		CtrlRect.top = (rectWnd.Height()*m_pSideCtrlPR[n2].fTop);
		CtrlRect.bottom = (rectWnd.Height()*m_pSideCtrlPR[n2].fBottom);
		CtrlRect.left = m_nSideWidth*m_pSideCtrlPR[n2].fLeft + nMainWndWidth;
		CtrlRect.right = m_nSideWidth*m_pSideCtrlPR[n2].fRight + nMainWndWidth;
		GetDlgItem(m_SideCtrl[n2])->MoveWindow(CtrlRect);
		
		if (m_SideCtrl[n2] == IDC_STATIC_FILE)
		{
			for (int n=0; n<m_nNumWS; ++n)
				m_plFile[n].MoveWindow(CtrlRect);
		}
		else if (m_SideCtrl[n2] == IDC_STATIC_DISK)
		{
			for (int n=0; n<m_nNumWS; ++n)
				m_plDisk[n].MoveWindow(CtrlRect);
		}
	}
	
	// MoveWindow
	s_AlgModelInput sIn;

	ZeroMemory(&sIn, sizeof(s_AlgModelInput));
	sIn.iOperSig = 6;

	for (int n=0; n<m_nNumWS; ++n)
	{
	s_DemoAlgSetParam sDemo;
	sDemo.rect = rectDlg;
#ifdef COMMON_DEMO_VERSION
		sDemo.iCheckSN = n;
		sIn.iCheckSN = n;
		sIn.pParam = &sDemo;
		m_multiObjManager.Set(sIn);
#endif
// #ifdef CAPSULE_DEMO_VERSION
// 		switch (n)
// 		{
// 		case 0:
// 		case 1:
// 			((CFtop*)g_ppCheck[n])->SetModel(sIn);
// 			break;
// 		case 2:
// 			((CFbody*)g_ppCheck[n])->SetModel(sIn);
// 			break;
// 		case 3:
// 		case 4:
// 			((CBbody*)g_ppCheck[n])->SetModel(sIn);
// 			break;
// 		}
// #endif
	}
}

void CDemoProjectDlg::ModifySidebarWidth(int nNewWidth, BOOL bMoveAll)
{
	CRect WndRect, rectCWnd;
	GetWindowRect(WndRect);
	GetClientRect(rectCWnd);

	m_nSideWidth = nNewWidth;
	if(!m_bSidebarHide)m_fSideWidth = m_nSideWidth*1.0 / rectCWnd.Width();
	int nMainWndWidth = rectCWnd.Width() - m_nSideWidth;
	

	CRect CtrlRect;
	// 侧边栏
	for (int n2=0; n2<m_SideCtrl.size(); ++n2)
	{
		GetDlgItem(m_SideCtrl[n2])->GetWindowRect(CtrlRect);
		ScreenToClient(CtrlRect);
		CtrlRect.left = m_nSideWidth*m_pSideCtrlPR[n2].fLeft + 0.5 + nMainWndWidth;
		CtrlRect.right = m_nSideWidth*m_pSideCtrlPR[n2].fRight + 0.5 + nMainWndWidth;
		GetDlgItem(m_SideCtrl[n2])->MoveWindow(CtrlRect);
		
		if (m_SideCtrl[n2] == IDC_STATIC_FILE)
		{
			for (int n=0; n<m_nNumWS; ++n)
				m_plFile[n].MoveWindow(CtrlRect);
		}
		else if (m_SideCtrl[n2] == IDC_STATIC_DISK)
		{
			for (int n=0; n<m_nNumWS; ++n)
				m_plDisk[n].MoveWindow(CtrlRect);
		}
	}
	
	// 主窗口
	if (bMoveAll)
	{
		CRect rectDlg;
		for (int n1=0; n1<m_MainCtrl.size(); ++n1)
		{
			GetDlgItem(m_MainCtrl[n1])->GetWindowRect(CtrlRect);
			ScreenToClient(CtrlRect);
			CtrlRect.left = nMainWndWidth*m_pMainCtrlPR[n1].fLeft;
			CtrlRect.right = nMainWndWidth*m_pMainCtrlPR[n1].fRight;
			GetDlgItem(m_MainCtrl[n1])->MoveWindow(CtrlRect);
			
			if (m_MainCtrl[n1] == IDC_STATIC_1)
				rectDlg = CtrlRect;
		}

		// MoveWindow
		s_AlgModelInput sIn;
		
		ZeroMemory(&sIn,sizeof(s_AlgModelInput));
		sIn.iOperSig = 6;
	
		for (int n=0; n<m_nNumWS; ++n)
		{
			s_DemoAlgSetParam sDemo;
			sDemo.rect = rectDlg;
#ifdef COMMON_DEMO_VERSION
			sDemo.iCheckSN = n;
			sIn.iCheckSN = n;
			sIn.pParam = &sDemo;
			m_multiObjManager.Set(sIn);
#endif
		}

		RedrawWindow();
	}
}

void CDemoProjectDlg::VisualSidebar()
{
	if (m_bSidebarHide)	//要隐藏
	{
		//模板
		GetDlgItem(IDC_STATIC_BCK)->ShowWindow(SW_HIDE);		GetDlgItem(IDC_STATIC_BCK)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_MLIST)->ShowWindow(SW_HIDE);		GetDlgItem(IDC_STATIC_MLIST)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_NEWNAME)->ShowWindow(SW_HIDE);	GetDlgItem(IDC_STATIC_NEWNAME)->EnableWindow(FALSE);
		GetDlgItem(IDC_MODELLIST)->ShowWindow(SW_HIDE);			GetDlgItem(IDC_MODELLIST)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_KJ)->ShowWindow(SW_HIDE);			GetDlgItem(IDC_STATIC_KJ)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT1)->ShowWindow(SW_HIDE);				GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_NEWMODEL)->ShowWindow(SW_HIDE);			GetDlgItem(IDC_NEWMODEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_DELETE)->ShowWindow(SW_HIDE);		GetDlgItem(IDC_BTN_DELETE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_SETD)->ShowWindow(SW_HIDE);			GetDlgItem(IDC_BTN_SETD)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_STOPPERTYPE)->ShowWindow(SW_HIDE);	GetDlgItem(IDC_COMBO_STOPPERTYPE)->EnableWindow(FALSE);
	
		//浏览器
		GetDlgItem(IDC_STATIC_FILEBACK)->ShowWindow(SW_HIDE);	GetDlgItem(IDC_STATIC_FILEBACK)->EnableWindow(FALSE);
		for (int n=0; n<m_nNumWS; ++n)
		{
			m_plFile[n].EnableWindow(FALSE);	m_plFile[n].ShowWindow(SW_HIDE);
			m_plDisk[n].EnableWindow(FALSE);	m_plDisk[n].ShowWindow(SW_HIDE);
		}
		GetDlgItem(IDC_BTN_ERRIMG)->ShowWindow(SW_HIDE);		GetDlgItem(IDC_BTN_ERRIMG)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_GRABIMG)->ShowWindow(SW_HIDE);		GetDlgItem(IDC_BTN_GRABIMG)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CAPIMG)->ShowWindow(SW_HIDE);		GetDlgItem(IDC_BTN_CAPIMG)->EnableWindow(FALSE);
		GetDlgItem(IDC_BTN_CLRST)->ShowWindow(SW_HIDE);			GetDlgItem(IDC_BTN_CLRST)->EnableWindow(FALSE);
		GetDlgItem(IDC_LIST_RESULT)->ShowWindow(SW_HIDE);		GetDlgItem(IDC_LIST_RESULT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_CURPATH)->ShowWindow(SW_HIDE);		GetDlgItem(IDC_EDIT_CURPATH)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_VERSION)->ShowWindow(SW_HIDE);	GetDlgItem(IDC_STATIC_VERSION)->EnableWindow(FALSE);
	}
	else				//要展开
	{
		if (m_bFileIsShow)
		{
			DisEnMdlList(0);
		}
		else
		{
			DisEnMdlList(1);
		}
	}
}



void CDemoProjectDlg::OnEnChangeEditModellog()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
 	CString sSearchName{ TEXT("") };
 	GetDlgItemText(IDC_EDIT_ModelLog, sSearchName);
	InitModel(TEXT("*")+sSearchName+TEXT("*.*"));
	
}

void CDemoProjectDlg::InitModelLog()
{
	CFileFind fileFind;
	bool bRet= fileFind.FindFile(g_strAllModelPath+TEXT("\\*.*"));
	m_mapModelLog.clear();/** 清空日志模板数据              [6/20/2017 ConanSteve]**/
	while (bRet)
	{
		bRet = fileFind.FindNextFile();
		CString sModelName = fileFind.GetFileName();
		CString sModelLogPath = g_strAllModelPath + TEXT("\\") + sModelName + TEXT("\\LoadModelLog.ini");
		int iTimes = GetPrivateProfileInt(TEXT("MainConfiguration"), TEXT("LoadModelTimes"), 0, sModelLogPath);
		CString sValue{ TEXT("") }, sKey;
		vector<CString> vecModelLog;
		for (int i = iTimes-1; i>=0; --i)
		{
			sValue = { TEXT("") };
			sKey.Format(TEXT("%d"), i + 1);
			GetPrivateProfileString(TEXT("LoadModelTime"), sKey, TEXT(""), sValue.GetBuffer(255), 255, sModelLogPath);
			if (TEXT("") != sValue)
			{
				vecModelLog.push_back(sValue);
			}
		}
		if(0!=vecModelLog.size())
			m_mapModelLog.insert(make_pair(sModelName, vecModelLog));
	}
	
}


bool CDemoProjectDlg::InitModel(CString sFilter)
{
	m_lModelNames.Init(this, g_nLangType, sFilter);
	int nModelCnt = m_lModelNames.GetCount();
	if (nModelCnt <= 0 && m_bFirstInitModel)
	{
		return false;
		CDlgNewModel dlg;
		CString strNewName, strModelPath;
		while (1)
		{
			dlg.DoModal();
			strNewName = dlg.m_strNewModel;
			if (strNewName.GetLength() > 0)
				break;
		}
		strModelPath.Format("%s\\%s", g_strAllModelPath, strNewName);
		CreateDirectory(strModelPath.GetBuffer(0), NULL);
		m_lModelNames.Refresh();
		m_lModelNames.SetCurSel(0);

		char chText[256];
		g_pGetTextFromLPK(chText, 3, strNewName.GetBuffer(0));
		m_strModelNameUsing = strNewName;
		GetDlgItem(IDC_STATIC_INFO)->SetWindowText(chText);
	}
	else
	{
		if (m_bStopperMode)	//筛选模板名称
		{
			CString strMt;
			CString strStandard;
			strStandard.Format("Type%d_", m_nStopperType);
			while (1)
			{
				BOOL bFoundBad = FALSE;
				for (int ni = 0; ni < m_lModelNames.GetCount(); ++ni)
				{
					m_lModelNames.GetText(ni, strMt);
					if (strStandard.CompareNoCase(strMt.Left(strStandard.GetLength())))
					{
						m_lModelNames.DeleteString(ni);
						bFoundBad = TRUE;
						break;
					}
				}
				if (bFoundBad == FALSE)
					break;
			}
		}

		int nIndex = m_lModelNames.FindStringExact(0, m_strModelNameUsing.GetBuffer(0));
		if (nIndex < 0)	//没找到模板
		{
// 			CString strSubModel;
// 			m_lModelNames.GetText(0, strSubModel);
// 			CString strInfo;
// 			char chText[256];
// 			g_pGetTextFromLPK(chText, 4, m_strModelNameUsing.GetBuffer(0), strSubModel.GetBuffer(0));
// 			m_strModelNameUsing = strSubModel;
// 			GetDlgItem(IDC_STATIC_INFO)->SetWindowText(chText);
// 			m_lModelNames.SetCurSel(0);
		}
		else
		{
			m_lModelNames.SetCurSel(nIndex);
			char chText[256];
			g_pGetTextFromLPK(chText, 5, m_strModelNameUsing.GetBuffer(0));
			GetDlgItem(IDC_STATIC_INFO)->SetWindowText(chText);
		}
	}
	
	
	m_strCModel.Format("%s", m_strModelNameUsing.GetBuffer(0));
	m_bFirstInitModel = false;
	return true;
}


void CDemoProjectDlg::OnLbnSelchangeModellist()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	int nIndex = m_lModelNames.GetCurSel();
	CString sModelName{ TEXT("") };
	m_lModelNames.GetText(nIndex, sModelName);
	if (0!=m_iShowModelLog&& m_bEnableShowModelLog)
	{
		if (nullptr == m_pDlgModelLog)
		{
			m_pDlgModelLog = make_shared<CDlgModelLog>();
			m_pDlgModelLog->Create(IDD_ModelLog, this);
		}
		if (NULL != g_hLanguage)
			m_pDlgModelLog->SetWindowText(TEXT("Load Model Logs:"));
		else
			m_pDlgModelLog->SetWindowText(TEXT("模板加载日志："));
		m_pDlgModelLog->ShowWindow(SW_SHOW);
		auto vecModelLog = m_mapModelLog[sModelName];
		m_pDlgModelLog->m_listModelLog.ResetContent();
		for (auto a: vecModelLog)
		{
			m_pDlgModelLog->m_listModelLog.AddString(a);
		}
	}
}


void CDemoProjectDlg::OnBnClickedCheckShowmodellog()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	m_iShowModelLog = ((CButton*)GetDlgItem(IDC_CHECK_ShowModelLog))->GetCheck();
	UpdateData(FALSE);

}
bool CDemoProjectDlg::ReadSystemConfig()
{
	CString sFilePath{ TEXT("") };
	sFilePath = g_strAppDir + TEXT("\\Config\\AlgConfig.ini");
	m_sAlgConfigPath = sFilePath;
	/************************************************************************/
	/*                       [6/2/2017 ConanSteve]                         */
	m_bStopperMode = 1 == GetPrivateProfileInt("DEMO Controls", "StopperMode", 0, sFilePath);
	/************************************************************************/
	m_bEnableModelSearch = 1 == GetPrivateProfileInt(TEXT("DEMO Controls"), TEXT("EnableModelSearch"), 0, sFilePath);
	m_bEnableShowModelLog = 1 == GetPrivateProfileInt(TEXT("DEMO Controls"), TEXT("EnableModelLog"), 0, sFilePath);
	/************************************************************************/
	/*                       [6/5/2017 ConanSteve]                         */
	m_nStopperType = GetPrivateProfileInt(TEXT("Demo Controls"), TEXT("StopperType"), 0, sFilePath);
	m_sStopperType.Format(TEXT("Type_%d"), m_nStopperType);
	/*************************************<Demo保存功能验证>*************************************/
	//**[8/14/2017 ConanSteve]**:  需要打开此功能，需要在AlgConfig.ini里面设置EnableVerificationOfSaveModel参数为1
	m_bEnableVerificationOfSaveModel = 1 == GetPrivateProfileInt(TEXT("Demo Controls"), TEXT("EnableVerificationOfSaveModel"), 0, g_strIniPath.GetBuffer());
	/************************************************************************/
	m_bShowLoadModalDlg = 1 == GetPrivateProfileInt(_T("Demo Controls"), _T("EnbaleShowLoadModelDlg"), 1, g_strIniPath);
	WritePrivateProfileString(_T("Demo Controls"), _T("EnbaleShowLoadModelDlg"), m_bShowLoadModalDlg ? _T("1") : _T("0"), g_strIniPath);
	return true;
}

bool CDemoProjectDlg::SaveSystemConfig()
{
	CString sVal{ TEXT("") }; 
	CString sFilePath{ TEXT("") };
	sFilePath = g_strAppDir + TEXT("\\Config\\AlgConfig.ini");
	sVal.Format(TEXT("%d"), m_bStopperMode ? 1 : 0);
	WritePrivateProfileString(TEXT("Demo Controls"), TEXT("StopperMode"), sVal, sFilePath);
	sVal.Format(TEXT("%d"), m_bEnableModelSearch ? 1 : 0);
	WritePrivateProfileString(TEXT("Demo Controls"), TEXT("EnableModelSearch"), sVal, sFilePath);
	sVal.Format(TEXT("%d"), m_bEnableShowModelLog ? 1 : 0);
	WritePrivateProfileString(TEXT("Demo Controls"), TEXT("EnableModelLog"), sVal, sFilePath);
	sVal.Format(TEXT("%d"), m_nStopperType);
	WritePrivateProfileString(TEXT("Demo Controls"), TEXT("StopperType"), sVal, sFilePath);
	sVal.Format(TEXT("%d"), m_bEnableVerificationOfSaveModel ? 1 : 0);
	WritePrivateProfileString(TEXT("Demo Controls"), TEXT("EnableVerificationOfSaveModel"), sVal, sFilePath);
	return true;
}


void CDemoProjectDlg::CreateModelLog(CString& sModelName)
{
	CString sLogFilePath = g_strAllModelPath + TEXT("\\") + sModelName + TEXT("\\LoadModelLog.ini");
	WritePrivateProfileString(TEXT("MainConfiguration"), TEXT("LoadModelTimes"), TEXT("1"), sLogFilePath);
	SYSTEMTIME sysTime{ 0 };
	GetLocalTime(&sysTime);
	CString sLoadInfo{ TEXT("") };
	sLoadInfo.Format(TEXT("%04d%02d%02d_%02d:%02d:%02d    Create Model Succeed!"), sysTime.wYear, sysTime.wMonth, sysTime.wDay
		, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	WritePrivateProfileString(TEXT("LoadModelTime"), TEXT("1"), sLoadInfo, sLogFilePath);
	InitModelLog();
}


void CDemoProjectDlg::FnWriteLogOfSaveModel(int iIndex)
{
	SYSTEMTIME sysTime{ 0 };
	GetLocalTime(&sysTime);
	CString sLoadInfo{ TEXT("") };
	switch (iIndex)
	{
	case 0:
	{
		sLoadInfo.Format(TEXT("%04d%02d%02d_%02d:%02d:%02d    Save All Model Information!"), sysTime.wYear, sysTime.wMonth, sysTime.wDay
			, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
	}
		break;
	default:
		sLoadInfo.Format(TEXT("%04d%02d%02d_%02d:%02d:%02d    Save Model Information Of %d!"), sysTime.wYear, sysTime.wMonth, sysTime.wDay
			, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, iIndex);
		break;
	}
	CString sLogFilePath = g_strAllModelPath + TEXT("\\") + m_strModelNameUsing + TEXT("\\LoadModelLog.ini");
	int iModelTimes = GetPrivateProfileInt(TEXT("MainConfiguration"), TEXT("LoadModelTimes"), 1, sLogFilePath);
	++iModelTimes;
	CString sVal{ TEXT("") };
	sVal.Format(TEXT("%d"), iModelTimes);
	WritePrivateProfileString(TEXT("MainConfiguration"), TEXT("LoadModelTimes"), sVal, sLogFilePath);
	WritePrivateProfileString(TEXT("LoadModelTime"), sVal, sLoadInfo, sLogFilePath);
	InitModelLog();
	if (m_pDlgModelLog)
	{
		m_pDlgModelLog->m_listModelLog.ResetContent();
		auto vecModelLog = m_mapModelLog[m_strModelNameUsing];
		for (auto a : vecModelLog)
		{
			m_pDlgModelLog->m_listModelLog.AddString(a);
		}
	}
}

DWORD WINAPI CDemoProjectDlg::ThreadLoadModel(LPVOID pPara)
{
	CDemoProjectDlg* pDlg = (CDemoProjectDlg*)pPara;
	return pDlg->ThreadFnLoadModel();
}

DWORD CDemoProjectDlg::ThreadFnLoadModel()
{
	/*************************************<通过内核事件来通知系统加载模板>*************************************/
	{
		SetEvent(m_hEvent_DemoMessage_SendCommandOfLoadModel);
		HANDLE hEvent_Temp[2];
		hEvent_Temp[0] = m_hEvent_DemoMessage_ReceiveSuccessOfLoadModel;
		hEvent_Temp[1] = m_hEvent_DemoMessage_ReceiveFailureOfLoadModel; 
		int iCnt{ 0 };
		DWORD dwRet = 0;
		dwRet = WaitForMultipleObjects(2, hEvent_Temp, FALSE, 2000);
		bool bLoadSuccess{ false };
		switch (dwRet)
		{
		case WAIT_OBJECT_0:
		{
			bLoadSuccess = true;
		}
		break;
		default:
			break;
		}
		m_csProgressCtrl.Lock();
		//if (!m_bEnableVerificationOfSaveModel)
			PostMessage(DEMO_APP_LOADM, 0, bLoadSuccess ? 0 : 1);
		m_csProgressCtrl.Unlock();
	}
	/************************************************************************/
	return 0;
}

bool CDemoProjectDlg::UpdateUILanguage(CString sLanguageFilePath)
{

	return true;
}



LRESULT CDemoProjectDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	switch (message)
	{
	case WM_NCLBUTTONDBLCLK:
		if (HTCAPTION == wParam)
		{
			return 0;
		}
	}
	return CDialog::DefWindowProc(message, wParam, lParam);
}


void CDemoProjectDlg::OnNcMButtonDblClk(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnNcMButtonDblClk(nHitTest, point);
}


void CDemoProjectDlg::OnNcLButtonDblClk(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnNcLButtonDblClk(nHitTest, point);
}


void CDemoProjectDlg::OnBnClickedButtonFoldpage()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_bIsWaiting)
	{
		return ;
	}
#ifdef COMMON_DEMO_VERSION
	CRect wndRect;	GetClientRect(wndRect);	ClientToScreen(wndRect);
	CPoint pp;		GetCursorPos(&pp);

	if (m_bSidebarHide)	// 要展开
	{
		//if (pp.x >= wndRect.right - 5 - 3 && pp.x <= wndRect.right - 5 + 3)
		{
			m_bSidebarHide = FALSE;
			VisualSidebar();
			ModifySidebarWidth(m_nSideWidthTemp);
		}
	}
	else				// 要隐藏
	{
		CRect rectBk;
		GetDlgItem(IDC_STATIC_BCK)->GetWindowRect(rectBk);
		//if (pp.x >= rectBk.left - 3 && pp.x <= rectBk.left + 3)
		{
			m_bSidebarHide = TRUE;
			VisualSidebar();
			m_nSideWidthTemp = m_nSideWidth;
			ModifySidebarWidth(10);
		}
	}
#endif
}
