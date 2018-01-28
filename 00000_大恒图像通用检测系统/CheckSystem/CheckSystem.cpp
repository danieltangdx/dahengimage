
// CheckSystem.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "CheckSystem.h"
#include "MainFrm.h"



#include "CheckSystemDoc.h"
#include "CheckSystemView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CMainFrame*		g_pFrame{nullptr}; //全局指针
extern CCheckSystemDoc*		g_pDoc{ nullptr };
extern CCheckSystemView*	g_pView{nullptr};
// CCheckSystemApp

BEGIN_MESSAGE_MAP(CCheckSystemApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CCheckSystemApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CCheckSystemApp construction

CCheckSystemApp::CCheckSystemApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("CheckSystem.AppID.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CCheckSystemApp object

CCheckSystemApp theApp;


// CCheckSystemApp initialization

BOOL CCheckSystemApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();
	XInitXCGUI();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)

								// 如果没有发现加密狗
	


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CCheckSystemDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CCheckSystemView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	//**[9/5/2017 ConanSteve]**:  
	g_pFrame = (CMainFrame*)GetMainWnd(); //获取主框架指针
										  //获取全局类指针
	g_pDoc = (CCheckSystemDoc*)g_pFrame->GetActiveDocument();
	g_pView = (CCheckSystemView*)g_pFrame->GetActiveView();

	{
		//**[1/5/2018 ConanSteve]**:  当接口卡不为模拟接口卡且相机不为模拟相机时，才需要加密狗验证
		g_pFrame->m_arr_iGrabType[0] = (e_GrabType)g_pFrame->m_iniCheckSystemConfig.GetIntEx(_T("采集参数"), _T("设备A类型"));
		g_pFrame->m_arr_iIOCardType[0] = (e_IOCardType)g_pFrame->m_iniCheckSystemConfig.GetIntEx(_T("IOCard Parameters"), _T("IOCardType_0"));
		bool bRet = g_pFrame->m_threeTierRights.Login(g_pFrame);
		if (!bRet)
		{
			return false;
		}
		//**[1/27/2018 ConanSteve]**:	如果是客户权限，则再判断相机类型和接口卡类型是否需要加密狗验证 
		if (!g_pFrame->m_threeTierRights.VerifyRights(e_UserType_SuperAdmin))
		{
			if (e_GrabType_SG != g_pFrame->m_arr_iGrabType[0] && e_GrabType_SG != g_pFrame->m_arr_iIOCardType[0] )
			{
				if(g_pFrame->m_bEnableVerification && FALSE == RockeyIsExist())
					return FALSE;
			}
		}
		//保证程序运行唯一
		HANDLE hMapping = ::CreateFileMapping((HANDLE)0xFFFFFFFF, NULL, PAGE_READWRITE, 0, 16, "[Guid(54B9FC3B - 8A71 - 4891 - 8E4A - D616357157DD)]");
		if (hMapping == NULL)
		{
			TRACE("Failed in call to CreateFileMapping, GetLastError returned %d\n", GetLastError());
			return FALSE;
		}
		else
		{
			DWORD ret = GetLastError();
			if (ret == ERROR_ALREADY_EXISTS)
			{
				AfxMessageBox(tr("程序已经打开！"));
				return FALSE;
			}
		}
	}

	DWORD dwRet = g_pFrame->ThreadInitBackstageFunc(2);
	if (-1 == dwRet)
		exit(0);
	//g_pFrame->InitMultiObject();
	return TRUE;
}

int CCheckSystemApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);
	XExitXCGUI();
	return CWinApp::ExitInstance();
}

// CCheckSystemApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CCheckSystemApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CCheckSystemApp message handlers
BOOL CCheckSystemApp::RockeyIsExist()
{

	// 从配置文件读取日期
	m_strWorkPath = CExFn::GetCurrentAppPath ();
	m_strWorkPath.Delete(m_strWorkPath.GetLength() - 1, 1);

	m_strCheckSystemConfigPath.Format("%s\\Config\\CheckSystemConfig.ini", m_strWorkPath);
	m_iLicenseExpDate = GetPrivateProfileInt("系统配置", "License提醒", 3, m_strCheckSystemConfigPath);

#ifdef __cplusplus
	typedef WORD(CALLBACK *ROCKEY) (WORD, WORD*, DWORD*, DWORD*, WORD*, WORD*, WORD*, WORD*, BYTE*);
#else
	WORD Rockey(WORD function, WORD* handle, DWORD* lp1, DWORD* lp2, WORD* p1, WORD* p2, WORD* p3, WORD* p4, BYTE* buffer);
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
			sprintf(errmsg, "Can't find Rockey4ND.dll");
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
	const char g_UidChar[] = "06A6914A-D863-43E1-800E-7E2EECE22FD7"; //瓶盖专用
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

	BOOL bCheckDate = FALSE;
	int nDays = 0;
	CString strTemp;
	s_KeyVerfResult Result = m_ProgramLicense.CheckLicenseValid(TRUE);

	if (0 != Result.nError && -1 != Result.nError && -2 != Result.nError)
	{
		strTemp.Format("密钥验证未通过,错误代码为:%d, 错误描述:%s ", Result.nError, Result.chErrorDetail);
		AfxMessageBox(strTemp);
		return FALSE;
	}
	else
	{
		if (0 == Result.nError || -1 == Result.nError)
		{
			bCheckDate = FALSE;
		}
		else
		{
			bCheckDate = TRUE;
		}
	}

	if (TRUE == bCheckDate)
	{
		nDays = m_ProgramLicense.ReadHardwareID("getexpdate");

		if (-1 == nDays)
		{
			strTemp.Format("密钥验证未通过,请与大恒图像技术人员联系!");
			AfxMessageBox(strTemp);
			return FALSE;
		}

		if (nDays <= m_iLicenseExpDate)
		{
			strTemp.Format("本软件还有%d天使用期,请提前与大恒图像技术或销售人员联系更新升级!", nDays);
			AfxMessageBox(strTemp);
		}
	}

	return TRUE;
}

BOOL CCheckSystemApp::RockeyIsExist_HD()
{

	// 验证试用期[2014-5-7 zhy]
	const char g_UidChar[] = "06A6914A-D863-43E1-800E-7E2EECE22FD7"; //瓶盖专用
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

	s_KeyVerfResult Result = m_ProgramLicense.CheckLicenseValid(TRUE);

	if (Result.nError == 0 || Result.nError == -1 || Result.nError == -2)
	{

	}
	else
	{
		CString strC;
		strC.Format("密钥验证未通过，错误代码为：%d, 错误描述：%s ", Result.nError, Result.chErrorDetail);
		AfxMessageBox(strC);
		return FALSE;
	}

	return TRUE;
}



