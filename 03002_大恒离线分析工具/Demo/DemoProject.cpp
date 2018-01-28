// DemoProject.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "DemoProject.h"
#include "DemoProjectDlg.h"
#include <WINDOWS.H>
#include <TLHELP32.H>
#include "UIDlgProcessTest.h"
#include "UIDlgStart.h"
#include <io.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

UINT ThreadFuncStart(LPVOID);
/////////////////////////////////////////////////////////////////////////////
// CDemoProjectApp

BEGIN_MESSAGE_MAP(CDemoProjectApp, CWinApp)
	//{{AFX_MSG_MAP(CDemoProjectApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoProjectApp construction

CDemoProjectApp::CDemoProjectApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
//	g_Event = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CDemoProjectApp object
CDemoProjectApp theApp;
CUIDlgStart* theStart = NULL;
HMODULE g_hLanguage = NULL;
pFunc_GetTextFromLPK g_pGetTextFromLPK = NULL;
CString g_strAppDir, g_strLogDir,g_strConfigDir,g_strLanguageDir,g_strPresDir,g_strCurSet, g_strIniPath, g_strAllModelPath, g_strCheckIni, g_strLanguage="",g_strCheckPath;
int g_nLangType = 1; //1:中文  0:英文


/////////////////////////////////////////////////////////////////////////////
// CDemoProjectApp initialization
BOOL CDemoProjectApp::InitInstance()
{
	AfxEnableControlContainer();


	/*************************************<初始化语言>*************************************/
	CString sConfigFilePath = CExFn::GetCurrentAppPath() + TEXT("/Config/AlgConfig.ini");
	GetPrivateProfileString(TEXT("Demo Controls"), TEXT("Language"), TEXT("Default"), m_sLanguageFilePath.GetBuffer(256), 256, sConfigFilePath);
	m_sLanguageFilePath.ReleaseBuffer();
	m_sLanguageFilePath = CExFn::GetCurrentAppPath() + TEXT("\\Language\\Demo\\") + m_sLanguageFilePath + TEXT(".ini");
	InitLanguage(m_sLanguageFilePath);
	SetModuleName(TEXT("Demo"));
	m_bEnableXCGUI = 1 == GetPrivateProfileInt(TEXT("SystemConfiguration"), TEXT("EnableXCGUI"), 0, sConfigFilePath);
	if (m_bEnableXCGUI)
	{
		XInitXCGUI();
	}
	/*************************************<判断是否打开过Demo  互斥>*************************************/ 
	HANDLE Event = CreateMutex(NULL, FALSE, "DHProgramDemo");
	DWORD dError = GetLastError(); //获取驱动程序列表 的错误？
	if (dError == ERROR_ALREADY_EXISTS)
	{
// 		TCHAR buf[MAX_PATH];
// 		GetWindowText()
		HWND hwnd = ::FindWindow(NULL, _T("模板设置与管理程序"));
		if (NULL != hwnd)
		{
			::SetForegroundWindow(hwnd);
			return false;
		}
		AfxMessageBox(tr("Demo can not be running with two instance!"));
		return FALSE;
	}

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	InitLanguageEx();
	AfxBeginThread(ThreadFuncStart, NULL);

	CDemoProjectDlg dlg;
	m_pMainWnd = &dlg;// 让dlg 成为主窗口
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	CloseHandle(Event);
	return FALSE;
}

BOOL CDemoProjectApp::ShowWindow(int nCmdShow)
{
	if (m_pMainWnd)
	{
		m_pMainWnd->ShowWindow(nCmdShow);
		m_pMainWnd->SetForegroundWindow();
	}

	return TRUE;
}


void CDemoProjectApp::InitLanguageEx()
{
	//获取各种路径
	{
		CGeneralFunc_Yang::GetOriginalAppDir(g_strAppDir);
		g_strCurSet = g_strAppDir + "\\set.ini";
		g_strIniPath = g_strAppDir + "\\Config";
		g_strCheckPath = g_strAppDir + "\\Config\\CheckSystemConfig-sim.ini";

		if (_access(g_strIniPath.GetBuffer(0), 0) == -1)
			CreateDirectory(g_strIniPath.GetBuffer(0), NULL);
		g_strIniPath +=	"\\AlgConfig.ini";
		g_strAllModelPath = g_strAppDir + "\\ModelInfo";
		if (_access(g_strAllModelPath.GetBuffer(0), 0) == -1)
			CreateDirectory(g_strAllModelPath.GetBuffer(0), NULL);
		g_strCheckIni = g_strAppDir + "\\Config\\CheckSystemConfig.ini";
	}

	//获取语言版本
	LCID lcid = GetSystemDefaultLCID(); // 0x804 中国 
	if (lcid == 0x0c04 ||\
		lcid == 0x1404 ||\
		lcid == 0x0804 ||\
		lcid == 0x1004 ||\
		lcid == 0x0404)
		g_nLangType = 1;

	char chLangDll[MAX_PATH];
	CString strLangDll, strLangDllPath;
	GetPrivateProfileString("DEMO Controls", "Language", "NULL", chLangDll, MAX_PATH, g_strIniPath.GetBuffer(0));
	if (strcmp(chLangDll, "NULL")==0 || strlen(chLangDll)<=0)
	{
// 		if (g_nLangType == 1)
// 			MessageBox(NULL, _T("未设置语言，将使用默认语言(中文)"), "DEMO", MB_ICONWARNING);
// 		else
// 			MessageBox(NULL, "Language unset, Chinese will be used.", "DEMO", MB_ICONWARNING);
		
		strcpy(chLangDll, "DEFAULT");
		WritePrivateProfileString("DEMO Controls", "Language", strLangDll.GetBuffer(0), g_strIniPath.GetBuffer(0));
	}
	strLangDll = chLangDll;
	
	BOOL bUseDefault = FALSE;
	if (strLangDll.Compare("DEFAULT") != 0)
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
		strLangDllPath.Format("%s\\%s", g_strAppDir, strLangDll);
		if (_access(strLangDllPath.GetBuffer(0), 0) == -1)
		{
// 			if (g_nLangType == 1)
// 				MessageBox(NULL, "找不到语言文件！\r\n将使用默认语言(中文)", "DEMO", MB_ICONWARNING);
// 			else
// 				MessageBox(NULL, "Can't Find Language File!\r\nThe Default Language (Chinese) Will Be Used", "DEMO", MB_ICONWARNING);
			bUseDefault = TRUE;
		}
		else
		{
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
							MessageBox(NULL, "非法的语言文件！\r\n将使用默认语言(中文)", "DEMO", MB_ICONWARNING);
						else
							MessageBox(NULL, "Illegal Language File!\r\nThe Default Language (Chinese) Will Be Used", "DEMO", MB_ICONWARNING);
						bUseDefault = TRUE;
						FreeLibrary(g_hLanguage);
						g_hLanguage = NULL;
					}
				}
				else
				{
					if (g_nLangType == 1)
						MessageBox(NULL, "非法的语言文件！\r\n将使用默认语言(中文)", "DEMO", MB_ICONWARNING);
					else
						MessageBox(NULL, "Illegal Language File!\r\nThe Default Language (Chinese) Will Be Used", "DEMO", MB_ICONWARNING);
					bUseDefault = TRUE;
					FreeLibrary(g_hLanguage);
					g_hLanguage = NULL;
				}
			}
			else
			{
				DWORD dwError = GetLastError();
				CString strMsg;
				if (g_nLangType == 1)
					strMsg.Format("加载语言文件失败！\r\n错误代码：%d\r\n将使用默认语言(中文)", (int)dwError);
				else
					strMsg.Format("Can't Load Language File!\r\nError Code: %d\r\nThe Default Language (Chinese) Will Be Used", (int)dwError);
				MessageBox(NULL, strMsg.GetBuffer(0), "DEMO", MB_ICONWARNING);
				bUseDefault = TRUE;
			}
		}
	}
	else
	{
		bUseDefault = TRUE;
	}
	
	if (bUseDefault)
	{
		g_hLanguage = NULL;
		g_pGetTextFromLPK = (pFunc_GetTextFromLPK)(GetProcAddress(g_hLanguage, "GetText"));
		g_nLangType = 1;
		g_strLanguage = "中文";
	}
}

UINT ThreadFuncStart(LPVOID)
{
	CUIDlgStart start;
	theStart = &start;
	start.DoModal();
	theApp.ShowWindow(SW_SHOW);


	return 1;
}

int CDemoProjectApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
//	CloseHandle(g_Event);
	return CWinApp::ExitInstance();
}
