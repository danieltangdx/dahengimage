
// TestLoadLibraryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "TestLoadLibrary.h"
#include "TestLoadLibraryDlg.h"
#include "afxdialogex.h"
#include "..\..\IncPublic\\\ExFn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CTestLoadLibraryDlg 对话框



CTestLoadLibraryDlg::CTestLoadLibraryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TESTLOADLIBRARY_DIALOG, pParent)
	, m_sDllPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestLoadLibraryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MFCEDITBROWSE_DllPath, m_sDllPath);
	DDX_Control(pDX, IDC_MFCEDITBROWSE_DllPath, m_editBrowse);
}

BEGIN_MESSAGE_MAP(CTestLoadLibraryDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Load, &CTestLoadLibraryDlg::OnBnClickedButtonLoad)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CTestLoadLibraryDlg 消息处理程序

BOOL CTestLoadLibraryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_sDllPath = CExFn::GetCurrentAppPath() + _T("\\");
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestLoadLibraryDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestLoadLibraryDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTestLoadLibraryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTestLoadLibraryDlg::OnBnClickedButtonLoad()
{
	// TODO: 在此添加控件通知处理程序代码
// 	CINICfg iniCfg(_T("M:\\1.ini"));
// 	iniCfg.WriteInt(_T("Test"), _T("Int"), 1);
// 	iniCfg.WriteString(_T("Test"), _T("String"), _T("String"));
// 	return;

	UpdateData();
	CString sPathOfDllDirectory{ _T("") };
	sPathOfDllDirectory = m_sDllPath.Left(m_sDllPath.ReverseFind(_T('\\')));
	SetCurrentDirectory(sPathOfDllDirectory);
	if (FALSE == PathFileExists(m_sDllPath.GetBuffer()))
	{
		AfxMessageBox(_T("路径不存在"));
		return;
	}

	HINSTANCE hDll = LoadLibrary(m_sDllPath);
	if (NULL == hDll)
	{
		AfxMessageBox(CExFn::GetLastErrorMsg());
	}
	else
	{
		AfxMessageBox(_T("加载成功"));
	}
	
}


void CTestLoadLibraryDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TCHAR szFilePathName[_MAX_PATH + 1] = { 0 };

	//得到文件个数      
	UINT nNumOfFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	for (UINT nIndex = 0; nIndex < nNumOfFiles; ++nIndex)
	{
		//　得到文件名   
		DragQueryFile(hDropInfo, nIndex, (LPTSTR)szFilePathName, _MAX_PATH);
		m_editBrowse.SetWindowText(szFilePathName);
	}

	//完成拖放操作
	DragFinish(hDropInfo);
	CDialogEx::OnDropFiles(hDropInfo);
}
