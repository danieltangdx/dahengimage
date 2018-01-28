
// ThreeTierRightsDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ThreeTierRightsDemo.h"
#include "ThreeTierRightsDemoDlg.h"
#include "afxdialogex.h"

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


// CThreeTierRightsDemoDlg 对话框



CThreeTierRightsDemoDlg::CThreeTierRightsDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_THREETIERRIGHTSDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CThreeTierRightsDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CThreeTierRightsDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_Login, &CThreeTierRightsDemoDlg::OnBnClickedButtonLogin)
	ON_BN_CLICKED(IDC_BUTTON_UserManage, &CThreeTierRightsDemoDlg::OnBnClickedButtonUsermanage)
	ON_BN_CLICKED(IDC_BUTTON_Operator, &CThreeTierRightsDemoDlg::OnBnClickedButtonOperator)
	ON_BN_CLICKED(IDC_BUTTON_Debugger, &CThreeTierRightsDemoDlg::OnBnClickedButtonDebugger)
	ON_BN_CLICKED(IDC_BUTTON_Admin, &CThreeTierRightsDemoDlg::OnBnClickedButtonAdmin)
	ON_BN_CLICKED(IDC_BUTTON_Admin2, &CThreeTierRightsDemoDlg::OnBnClickedButtonAdmin2)
	ON_BN_CLICKED(IDC_BUTTON_Debugger2, &CThreeTierRightsDemoDlg::OnBnClickedButtonDebugger2)
	ON_BN_CLICKED(IDC_BUTTON_Operator2, &CThreeTierRightsDemoDlg::OnBnClickedButtonOperator2)
	ON_BN_CLICKED(IDC_BUTTON_UserManage2, &CThreeTierRightsDemoDlg::OnBnClickedButtonUsermanage2)
	ON_BN_CLICKED(IDC_BUTTON_SuperAdmin, &CThreeTierRightsDemoDlg::OnBnClickedButtonSuperadmin)
END_MESSAGE_MAP()


// CThreeTierRightsDemoDlg 消息处理程序

BOOL CThreeTierRightsDemoDlg::OnInitDialog()
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
	m_threeTierRights.AddCtrl(GetDlgItem(IDC_BUTTON_Operator2));
	m_threeTierRights.AddCtrl(GetDlgItem(IDC_BUTTON_Debugger2));
	m_threeTierRights.AddCtrl(GetDlgItem(IDC_BUTTON_Admin2));
	m_threeTierRights.AddCtrl(GetDlgItem(IDC_BUTTON_UserManage2));
	m_threeTierRights.AddCtrl(GetDlgItem(IDC_BUTTON_SuperAdmin));
	OnBnClickedButtonLogin();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CThreeTierRightsDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CThreeTierRightsDemoDlg::OnPaint()
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
HCURSOR CThreeTierRightsDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CThreeTierRightsDemoDlg::OnBnClickedButtonLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	m_threeTierRights.Login();
	GetDlgItem(IDC_BUTTON_Operator2)->ShowWindow(m_threeTierRights.VerifyRights(GetDlgItem(IDC_BUTTON_Operator2)));
	GetDlgItem(IDC_BUTTON_Debugger2)->ShowWindow(m_threeTierRights.VerifyRights(GetDlgItem(IDC_BUTTON_Debugger2)));
	GetDlgItem(IDC_BUTTON_Admin2)->ShowWindow(m_threeTierRights.VerifyRights(GetDlgItem(IDC_BUTTON_Admin2)));
	GetDlgItem(IDC_BUTTON_UserManage2)->ShowWindow(m_threeTierRights.VerifyRights(GetDlgItem(IDC_BUTTON_UserManage2)));
	GetDlgItem(IDC_BUTTON_SuperAdmin)->ShowWindow(m_threeTierRights.VerifyRights(GetDlgItem(IDC_BUTTON_SuperAdmin)));
	UpdateData(FALSE);
}


void CThreeTierRightsDemoDlg::OnBnClickedButtonUsermanage()
{
	// TODO: 在此添加控件通知处理程序代码
	m_threeTierRights.ManageUsers();
}


void CThreeTierRightsDemoDlg::OnBnClickedButtonOperator()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_threeTierRights.VerifyRights(e_UserType_Operator, true))
		AfxMessageBox(_T("Execute Succeed!"));
}


void CThreeTierRightsDemoDlg::OnBnClickedButtonDebugger()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_threeTierRights.VerifyRights(e_UserType_Debugger, true))
		AfxMessageBox(_T("Execute Succeed!"));
}


void CThreeTierRightsDemoDlg::OnBnClickedButtonAdmin()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_threeTierRights.VerifyRights(e_UserType_Admin, true))
		AfxMessageBox(_T("Execute Succeed!"));
}


void CThreeTierRightsDemoDlg::OnBnClickedButtonAdmin2()
{
	// TODO: Add your control notification handler code here
}


void CThreeTierRightsDemoDlg::OnBnClickedButtonDebugger2()
{
	// TODO: Add your control notification handler code here
}


void CThreeTierRightsDemoDlg::OnBnClickedButtonOperator2()
{
	// TODO: Add your control notification handler code here
}


void CThreeTierRightsDemoDlg::OnBnClickedButtonUsermanage2()
{
	// TODO: Add your control notification handler code here
}


void CThreeTierRightsDemoDlg::OnBnClickedButtonSuperadmin()
{
	// TODO: Add your control notification handler code here
}
