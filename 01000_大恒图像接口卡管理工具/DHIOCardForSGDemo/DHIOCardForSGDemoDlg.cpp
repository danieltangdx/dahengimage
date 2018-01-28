
// DHIOCardForSGDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DHIOCardForSGDemo.h"
#include "DHIOCardForSGDemoDlg.h"
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


// CDHIOCardForSGDemoDlg 对话框



CDHIOCardForSGDemoDlg::CDHIOCardForSGDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DHIOCARDFORSGDEMO_DIALOG, pParent)
	, m_iCurIOCardType(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDHIOCardForSGDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_IOCardSN, m_comboIOcardSN);
	DDX_Radio(pDX, IDC_RADIO1, m_iCurIOCardType);
}

BEGIN_MESSAGE_MAP(CDHIOCardForSGDemoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OpenCard1, &CDHIOCardForSGDemoDlg::OnBnClickedButtonOpencard1)
	ON_CBN_SELCHANGE(IDC_COMBO_IOCardSN, &CDHIOCardForSGDemoDlg::OnCbnSelchangeComboIocardsn)
	ON_BN_CLICKED(IDC_BUTTON_OpenIOCardCfgFile, &CDHIOCardForSGDemoDlg::OnBnClickedButtonOpeniocardcfgfile)
	ON_CONTROL_RANGE(BN_CLICKED ,IDC_RADIO1, IDC_RADIO6, &CDHIOCardForSGDemoDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_BUTTON_OpenConnectionFile, &CDHIOCardForSGDemoDlg::OnBnClickedButtonOpenconnectionfile)
	ON_BN_CLICKED(IDC_BUTTON_OpenManageToolCfgFile, &CDHIOCardForSGDemoDlg::OnBnClickedButtonOpenmanagetoolcfgfile)
	ON_BN_CLICKED(IDC_BUTTON_ReInit, &CDHIOCardForSGDemoDlg::OnBnClickedButtonReinit)
END_MESSAGE_MAP()


// CDHIOCardForSGDemoDlg 消息处理程序

BOOL CDHIOCardForSGDemoDlg::OnInitDialog()
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

	InitWindow();
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDHIOCardForSGDemoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CDHIOCardForSGDemoDlg::OnPaint()
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
HCURSOR CDHIOCardForSGDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDHIOCardForSGDemoDlg::OnBnClickedButtonOpencard1()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	int iIOCardSN = m_comboIOcardSN.GetCurSel();
	m_pIOCard[iIOCardSN].reset();
	switch (m_p_eIOCardType[iIOCardSN])
	{
	case e_IOCardType::e_IOCardType_NONE:
		return;
	case e_IOCardType::e_IOCardType_SG:
		m_pIOCard[iIOCardSN] = make_shared<CDHIOCardForSG>();
		break;
	case e_IOCardType::e_IOCardType_PIOB:
		return;
	case e_IOCardType::e_IOCardType_PIOC:
		m_pIOCard[iIOCardSN] = make_shared<CDHIOCardForPIO24C>();
		break;
	case e_IOCardType::e_IOCardType_PCI_6356:
		m_pIOCard[iIOCardSN] = make_shared<CDHIOCardForPCI6356>();
		break;
	case e_IOCardType::e_IOCardType_PCIE_6356:
		break;
	case e_IOCardType::e_IOCardType_RS232_XZXNYN:
		m_pIOCard[iIOCardSN] = make_shared<CDHIOCardForRS232_XZXNYN>();
		break;
	default:
		return;
	}
	if (nullptr == m_pIOCard[iIOCardSN])
		return;
	//AfxMessageBox(m_p_sPathFileOfInit[iIOCardSN]);
	BOOL bTemp = PathFileExists(m_p_sPathFileOfInit[iIOCardSN]);
	if (FALSE == PathFileExists(m_p_sPathFileOfInit[iIOCardSN]))
	{
		AfxMessageBox(_T("打开失败, 接口卡配置文件不存在！"));
		return;
	}
	CINICfg iniTemp(m_p_sPathFileOfInit[iIOCardSN]);
// 	int iIOCardTypeTemp = iniTemp.GetIntEx(_T("Critical Config"), _T("接口卡类型"), 0, _T("0：无卡，1模拟接口卡，2：B卡，3：C卡，4：PCI6356卡，5：PCIE_6356卡， 6：RS232_XZXNYN串口继电器"));
// 	if (m_p_eIOCardType[iIOCardSN] != (e_IOCardType)iIOCardTypeTemp)
// 	{
// 		if (IDOK == AfxMessageBox(_T("接口卡配置文件类型不符,是否打开配置文件修改？"), MB_OKCANCEL))
// 		{
// 			OnBnClickedButtonOpeniocardcfgfile();
// 		}
// 		return;
// 	}

	CIOCardInitInfo initInfo;
	int iSrcSize = m_p_sPathFileOfInit[iIOCardSN].GetLength() * sizeof(TCHAR);
	memcpy_s(initInfo.lpszPathFileOfInit, MAX_PATH*sizeof(TCHAR), m_p_sPathFileOfInit[iIOCardSN], m_p_sPathFileOfInit[iIOCardSN].GetLength() * sizeof(TCHAR));
	memcpy_s(initInfo.lpszPathFileOfConnection, MAX_PATH * sizeof(TCHAR), m_p_sPathFileOfConnect[iIOCardSN], m_p_sPathFileOfConnect[iIOCardSN].GetLength() * sizeof(TCHAR));
	if (!m_pIOCard[iIOCardSN]->InitCard(initInfo))
	{
		AfxMessageBox(_T("初始化接口卡失败\n")+m_pIOCard[iIOCardSN]->GetLastError());
	}
	if (!m_pIOCard[iIOCardSN]->OpenCard())
	{
		AfxMessageBox(_T("打开接口卡失败\n")+m_pIOCard[iIOCardSN]->GetLastError());
	}
	m_pIOCard[iIOCardSN]->OpenTools(this);
}


void CDHIOCardForSGDemoDlg::OnCbnSelchangeComboIocardsn()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	if (m_iIOCardNum > 0)
	{
		m_iLastIOCardSN = m_comboIOcardSN.GetCurSel();
		m_iCurIOCardType = (int)m_p_eIOCardType[m_iLastIOCardSN]-1;
	}
	UpdateData(FALSE);
}


void CDHIOCardForSGDemoDlg::OnBnClickedButtonOpeniocardcfgfile()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	int iIOCardSN = m_comboIOcardSN.GetCurSel();
	if (0 < m_iIOCardNum)
	{
		if (FALSE == PathFileExists(m_p_sPathFileOfInit[iIOCardSN]))
		{
			AfxMessageBox(_T("接口卡配置文件不存在！"));
			return;
		}
		CString sCmd = _T("notepad.exe ") + m_p_sPathFileOfInit[iIOCardSN];
		//WinExec(sCmd, SW_SHOW);
		//system(m_p_sPathFileOfInit[iIOCardSN]);
		ShellExecute(NULL, _T("open"), m_p_sPathFileOfInit[iIOCardSN], NULL, NULL, SW_SHOWNORMAL);
	}
}


void CDHIOCardForSGDemoDlg::OnBnClickedRadio1(UINT uID)
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	for (int i = 0; i < m_iIOCardNum; ++i)
	{
		if ((int)m_p_eIOCardType[i] == m_iCurIOCardType + 1)
		{
			m_comboIOcardSN.SetCurSel(i);
			UpdateData(FALSE);
			return;
		}
	}
	AfxMessageBox(_T("配置文件无此类型接口卡！"));
	m_iCurIOCardType = (int)m_p_eIOCardType[m_iLastIOCardSN]-1;
	UpdateData(FALSE);
}


void CDHIOCardForSGDemoDlg::OnBnClickedButtonOpenconnectionfile()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData();
	int iIOCardSN = m_comboIOcardSN.GetCurSel();
	if (0 < m_iIOCardNum)
	{
		ShellExecute(NULL, _T("open"), m_p_sPathFileOfConnect[iIOCardSN], NULL, NULL, SW_SHOWNORMAL);
	}
}


void CDHIOCardForSGDemoDlg::OnBnClickedButtonOpenmanagetoolcfgfile()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sPathFileManageTool = CExFn::GetCurrentAppPath() + _T("\\Config\\DHIOCard\\DHIOCardManageTools.ini");
	if (TRUE == PathFileExists(sPathFileManageTool))
	{
		ShellExecute(NULL, _T("open"), sPathFileManageTool, NULL, NULL, SW_SHOWNORMAL);
	}
}


void CDHIOCardForSGDemoDlg::OnBnClickedButtonReinit()
{
	// TODO: 在此添加控件通知处理程序代码
	InitWindow();
	AfxMessageBox(_T("载入成功！"));
	
}


bool CDHIOCardForSGDemoDlg::InitWindow()
{
	m_comboIOcardSN.ResetContent();
	m_iIOCardNum = 0;
	fill(m_arr_bEnable.begin(), m_arr_bEnable.end(), false);


	m_iniIOCardManageCfg.Init(CExFn::GetCurrentAppPath() + _T("\\Config\\DHIOCard\\DHIOCardManageTools.ini"));
	m_iIOCardNum = m_iniIOCardManageCfg.GetIntEx(_T("Critical Config"), _T("接口卡数目"), 0);
	m_pIOCard = make_unique<shared_ptr<CDHIOCard>[]>(m_iIOCardNum);
	m_p_eIOCardType = make_unique<e_IOCardType[]>(m_iIOCardNum);
	m_p_sPathFileOfInit = make_unique<CString[]>(m_iIOCardNum);
	m_p_sPathFileOfConnect = make_unique<CString[]>(m_iIOCardNum);
	m_iCurIOCardType = (int)e_IOCardType::e_IOCardType_NONE;
	m_iLastIOCardSN = 0;
	for (int i = 0; i < m_iIOCardNum; ++i)
	{
		CString sTemp{ _T("") };
		sTemp.Format(_T("接口卡%d参数"), i);
		m_p_eIOCardType[i] = (e_IOCardType)m_iniIOCardManageCfg.GetIntEx(sTemp, _T("接口卡类型"), 0, _T("0：无卡，1模拟接口卡，2：B卡，3：C卡，4：PCI6356卡，5：PCIE_6356卡， 6：RS232_XZXNYN串口继电器"));
		m_p_sPathFileOfInit[i] = m_iniIOCardManageCfg.GetStringEx(sTemp, _T("初始化文件路径名"), m_p_sPathFileOfInit[i], _T("NULL"));
		m_p_sPathFileOfConnect[i] = m_iniIOCardManageCfg.GetStringEx(sTemp, _T("接口卡与系统接口连接文件"), m_p_sPathFileOfConnect[i], _T("NULL"));
		m_p_sPathFileOfInit[i] = CExFn::GetCurrentAppPath() + _T("\\") + m_p_sPathFileOfInit[i];
		m_p_sPathFileOfConnect[i] = CExFn::GetCurrentAppPath() + _T("\\") + m_p_sPathFileOfConnect[i];
		if (0 <= (int)m_p_eIOCardType[i] - 1)
			m_arr_bEnable[(int)m_p_eIOCardType[i] - 1] = true;
		if (0 == i)
		{
			m_iCurIOCardType = (int)m_p_eIOCardType[0] - 1;
		}
		CString sIOCardName{ _T("") };
		switch (m_p_eIOCardType[i])
		{
		case e_IOCardType::e_IOCardType_NONE:
			sIOCardName = _T("无卡");
			break;
		case e_IOCardType::e_IOCardType_SG:
			sIOCardName = _T("模拟接口卡");
			break;
		case e_IOCardType::e_IOCardType_PIOB:
			sIOCardName = _T("B卡");
			break;
		case e_IOCardType::e_IOCardType_PIOC:
			sIOCardName = _T("C卡");
			break;
		case e_IOCardType::e_IOCardType_PCI_6356:
			sIOCardName = _T("PCI6356卡");
			break;
		case e_IOCardType::e_IOCardType_PCIE_6356:
			sIOCardName = _T("PCIE6356卡");
			break;
		case e_IOCardType::e_IOCardType_RS232_XZXNYN:
			sIOCardName = _T("RS232串口继电器");
			break;
		default:
			break;
		}

		sTemp.Format(_T("接口卡%d  [%s]"), i, sIOCardName);
		m_comboIOcardSN.AddString(sTemp);
	}
	m_comboIOcardSN.SetCurSel(0);

	for (int i = 0; i < 6; ++i)
	{
		if (!m_arr_bEnable[i])
		{
			GetDlgItem(IDC_RADIO1 + i)->EnableWindow(FALSE);
		}
	}
	UpdateData(FALSE);
	return true;
}
