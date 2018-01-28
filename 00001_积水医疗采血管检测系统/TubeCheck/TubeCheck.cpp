// TubeCheck.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TubeCheck.h"

#include "MainFrm.h"
#include "TubeCheckDoc.h"
#include "TubeCheckView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMainFrame*			g_pFrame = NULL; //全局指针
CTubeCheckDoc*	    g_pDoc  = NULL;
CTubeCheckView*	    g_pView = NULL;



/////////////////////////////////////////////////////////////////////////////
// CTubeCheckApp

BEGIN_MESSAGE_MAP(CTubeCheckApp, CWinApp)
	//{{AFX_MSG_MAP(CTubeCheckApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTubeCheckApp construction

CTubeCheckApp::CTubeCheckApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTubeCheckApp object

CTubeCheckApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTubeCheckApp initialization

BOOL CTubeCheckApp::InitInstance()
{
	AfxEnableControlContainer();
	
	XInitXCGUI();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	//////////////////////////////////////////////////////////////////////////
	// 如果没有发现加密狗
	if(FALSE==RockeyIsExist())
	{
		return FALSE;
	}

	//保证程序运行唯一
	HANDLE hMapping = ::CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 16, "TubeCheck");
	HANDLE hCheckSystem = CreateEvent(NULL, FALSE, FALSE, _T("{8D264CC6-8633-45DC-A616-4597E2BA97F6}"));
	if (hMapping == NULL)
	{
		TRACE("Failed in call to CreateFileMapping, GetLastError returned %d\n", GetLastError());
		return FALSE;
	}
	else
	{
		DWORD ret=GetLastError();
		if(ret==ERROR_ALREADY_EXISTS)
		{
			AfxMessageBox("TubeCheck程序已经打开！");
			return FALSE;
		}
	}

	/////////////////////////////////////////////////////////////////////////

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTubeCheckDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CTubeCheckView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.


	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED); //窗口最大化
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	//////////////////////////////////////
	g_pFrame = (CMainFrame*)GetMainWnd(); //获取主框架指针
	//获取全局类指针
	g_pDoc  = (CTubeCheckDoc*)g_pFrame->GetActiveDocument();
	g_pView = (CTubeCheckView*)g_pFrame->GetActiveView();
	
	if(TRUE==g_pFrame->m_bOpenAlgCheck)
		g_pFrame->InitInspectionDll();
	
	g_pFrame->m_iLicenseExpDate = m_iLicenseExpDate;

	return TRUE;
}

BOOL CTubeCheckApp::RockeyIsExist()
{
	
	// 从配置文件读取日期
	m_strWorkPath = GetCurrentAppPath();				
	m_strWorkPath.Delete(m_strWorkPath.GetLength()-1, 1);	
#ifdef USE_IO_CARD
	m_strCheckSystemConfigPath.Format("%s\\Config\\CheckSystemConfig.ini",m_strWorkPath);
#else
	m_strCheckSystemConfigPath.Format("%s\\Config\\CheckSystemConfig-sim.ini",m_strWorkPath);
#endif
	m_iLicenseExpDate = GetPrivateProfileInt("系统配置","License提醒",3,m_strCheckSystemConfigPath);

#ifdef USE_IO_CARD
#ifdef __cplusplus
	typedef WORD (CALLBACK *ROCKEY) (WORD, WORD*, DWORD*, DWORD*, WORD*, WORD*, WORD*, WORD*, BYTE*);
#else
	WORD Rockey(WORD function, WORD* handle, DWORD* lp1,  DWORD* lp2, WORD* p1, WORD* p2, WORD* p3, WORD* p4, BYTE* buffer);
#endif
	
	HINSTANCE hDll;
	ROCKEY Rockey;
	WORD handle[16], p1, p2, p3, p4, retcode;
	DWORD lp1, lp2;
	BYTE buffer[1024];
	char errmsg[1000];
	
	// Load Library
	hDll = GetModuleHandle("Rockey4ND.dll");
	if (hDll == NULL)
	{
		hDll = LoadLibrary("Rockey4ND.dll");
		if (hDll == NULL)
		{
			sprintf(errmsg,"Can't find Rockey4ND.dll");
			AfxMessageBox(errmsg);
			return FALSE;
		}
	}
	Rockey = (ROCKEY)GetProcAddress(hDll, "Rockey");


	p1 = 0x1EEF;
	p2 = 0x7B78;
	p3 = 0x4938;
	p4 = 0x4FDA;

	//Find
	lp1 = 0;
	retcode = Rockey(RY_FIND, &handle[0], &lp1, &lp2, &p1, &p2, &p3, &p4, buffer);
	if (retcode)
	{
		AfxMessageBox("未发现加密狗，程序退出！");
		return FALSE;
	}

	p1 = 0x1EEF;
	p2 = 0x7B78;
	p3 = 0x4938;
	p4 = 0x4FDA;

	retcode = Rockey(RY_OPEN, &handle[0], &lp1, &lp2, &p1, &p2, &p3, &p4, buffer);
	if (retcode)
	{
		AfxMessageBox("打开加密狗基本密码错误，程序退出！");
		return FALSE;
	}
	//Close
	retcode = Rockey(RY_CLOSE, &handle[0], &lp1, &lp2, &p1, &p2, &p3, &p4, buffer);
	if (retcode)
	{
		AfxMessageBox("关闭加密狗基本密码错误，程序退出！");
		return FALSE;
	}
	
	FreeLibrary(hDll);

	// 验证试用期[2014-5-7 zhy]
	const char g_UidChar[]= "06A6914A-D863-43E1-800E-7E2EECE22FD7"; //瓶盖专用
	ver_code uucode;
	m_ProgramLicense.GetVerCode(&uucode);
	CString strCode;
	strCode.Format("%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		uucode.Data1, uucode.Data2, uucode.Data3,
		uucode.Data4[0], uucode.Data4[1],
		uucode.Data4[2], uucode.Data4[3],
		uucode.Data4[4], uucode.Data4[5],
		uucode.Data4[6], uucode.Data4[7]);
	if (strCode.Compare(g_UidChar))
	{
		AfxMessageBox("试用已经过期，不通过！！！");
		return FALSE;
	}
	
	BOOL bCheckDate=FALSE;
	int nDays = 0;
	CString strTemp;
	s_KeyVerfResult Result=m_ProgramLicense.CheckLicenseValid(TRUE);
	
	if(0!=Result.nError && -1!=Result.nError && -2!=Result.nError)
	{
		strTemp.Format("密钥验证未通过,错误代码为:%d, 错误描述:%s ",Result.nError,Result.chErrorDetail);
		AfxMessageBox(strTemp);
		return FALSE;
	}
	else
	{
		if(0==Result.nError || -1==Result.nError)
		{
			bCheckDate=FALSE;
		}
		else
		{
			bCheckDate=TRUE;
		}
	}
	
	if(TRUE==bCheckDate)
	{
		nDays = m_ProgramLicense.ReadHardwareID("getexpdate");
		
		if(-1==nDays)
		{
			strTemp.Format("密钥验证未通过,请与大恒图像技术人员联系!");
			AfxMessageBox(strTemp);
			return FALSE;
		}
		
		if(nDays<=m_iLicenseExpDate)
		{
			strTemp.Format("本软件还有%d天使用期,请提前与大恒图像技术或销售人员联系更新升级!",nDays);
			AfxMessageBox(strTemp);
		}
	}
#endif

	return TRUE;
}

BOOL CTubeCheckApp::RockeyIsExist_HD()
{
	
	// 验证试用期[2014-5-7 zhy]
	const char g_UidChar[]= "06A6914A-D863-43E1-800E-7E2EECE22FD7"; //瓶盖专用
	ver_code uucode;
	m_ProgramLicense.GetVerCode(&uucode);
	CString strCode;
	strCode.Format("%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
		uucode.Data1, uucode.Data2, uucode.Data3,
		uucode.Data4[0], uucode.Data4[1],
		uucode.Data4[2], uucode.Data4[3],
		uucode.Data4[4], uucode.Data4[5],
		uucode.Data4[6], uucode.Data4[7]);
	if (strCode.Compare(g_UidChar))
	{
		AfxMessageBox("试用已经过期，不通过！！！");
		return FALSE;
	}
	
	s_KeyVerfResult Result=m_ProgramLicense.CheckLicenseValid(TRUE);
	
	if(Result.nError==0 || Result.nError == -1 || Result.nError==-2)
	{
		
	}
	else
	{
		CString strC;
		strC.Format("密钥验证未通过，错误代码为：%d, 错误描述：%s ",Result.nError,Result.chErrorDetail);
		AfxMessageBox(strC);
		return FALSE;
	}

	return TRUE;
}

//获取本执行程序所在的路径
CString CTubeCheckApp::GetCurrentAppPath()
{
	char str[MAX_PATH], drive[MAX_PATH], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
	CString AppPath;
	GetModuleFileName(NULL, str, sizeof(str));
	_splitpath(str, drive, dir, fname, ext);
	strcat(drive, dir);
	AppPath = drive;
	return AppPath;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CTubeCheckApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CTubeCheckApp message handlers


int CTubeCheckApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	CoUninitialize();
	//XExitXCGUI();
	return CWinApp::ExitInstance();
}
