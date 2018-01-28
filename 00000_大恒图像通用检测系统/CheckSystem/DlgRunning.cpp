// DlgRunning.cpp : implementation file
//

#include "stdafx.h"
#include "CheckSystem.h"
#include "DlgRunning.h"
#include "afxdialogex.h"
#include "CheckSystemDoc.h"
#include "CheckSystemView.h"

extern CMainFrame*		g_pFrame; //全局指针
extern CCheckSystemDoc*		g_pDoc;
extern CCheckSystemView*	g_pView;
// CDlgRunning dialog

IMPLEMENT_DYNAMIC(CDlgRunning, CDialogEx)

CDlgRunning::CDlgRunning(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_Running, pParent)
{

}

CDlgRunning::~CDlgRunning()
{
}

void CDlgRunning::DoDataExchange(CDataExchange* pDX)
{

	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_STATIC_Test, m_staticTest);
	DDX_Control(pDX, IDC_STATIC_Logo, m_staticCompanyLogo);
	DDX_Control(pDX, IDC_STATIC_Time, m_staticTime);
}


BEGIN_MESSAGE_MAP(CDlgRunning, CDialogEx)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CREATE()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BUTTON_RESET_SYSTEM, IDC_BUTTON_SWITCH_MODEL + 100, &CDlgRunning::OnButtonClick)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDlgRunning message handlers


void CDlgRunning::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages
	CRect rect{ 0 };
	GetClientRect(&rect);
	CBrush brush;
	//brush.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
	//brush.CreateSolidBrush(COLORREF(0x1E1E1E));
	brush.CreateSolidBrush(COLORREF(0xFFFFFF));
	dc.FillRect(&rect, &brush);
	brush.DeleteObject();

// #define SHS_NOISE 0
// #define SHS_DIAGSHADE 1
// #define SHS_HSHADE 2
// #define SHS_VSHADE 3
// #define SHS_HBUMP 4
// #define SHS_VBUMP 5
// #define SHS_SOFTBUMP 6
// #define SHS_HARDBUMP 7
// #define SHS_METAL 8

// 	m_btnSwitchModel.SetShade(SHS_HARDBUMP, 8, 20, 5, RGB(0, 0, 255));
// 	m_btnOpenIOCardTools.SetShade(SHS_HARDBUMP, 8, 20, 5, RGB(0, 0, 255));
// 	m_btnRecord.SetShade(SHS_HARDBUMP, 8, 20, 5, RGB(0, 0, 255));


	for (int i = 0; i < m_iCustomBtnNum; ++i)
	{
		m_arr_btnCustomBtn[i].SetShade(m_iCustomBtnStyle, 8, 20, 5, RGB(0, 0, 255));
	}
	for (int i = 0; i < m_vec_pCustomLabel.size(); ++i)
	{
		m_vec_pCustomLabel[i]->SetShade(m_iCustomLabelStyle, 8, 20, 5, RGB(255, 0, 0));
	}



// 	m_staticErrorCntLabel.m_pMainFont->SetHeight(20);
// 	m_staticErrorCntLabel.SetMainTextColour(RGB(0, 0, 0));
// 	m_staticErrorCntLabel.Display("错误总数:", "", 0, FALSE);
}


void CDlgRunning::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CRect rectThis{ 0 };
	CRect rectHisErrorInfo{ 0 };
	CRect rectLabel;

	bool bShow = false;//**[12/1/2017 ConanSteve]**:  是否显示当前控件
	GetClientRect(rectThis);
// 	CString sPathFileOfLayout{ _T("") }, sModelName{_T("")};
// 	CINICfg iniAlgCfg(CExFn::GetCurrentAppPath() + _T("Config\\AlgConfig.ini"));
// 	sModelName = iniAlgCfg.GetStringEx(_T("Demo Controls"), _T("DefaultModel"), sModelName);

	
	m_layout.Init(CExFn::GetCurrentAppPath() + _T("\\Config\\LayoutRunDlg.xml"), rectThis);

	bShow = m_layout.GetPosition(_T("HistoryErrorInfo"), rectHisErrorInfo);
	m_pdlgHisErrorInfo->MoveWindow(rectHisErrorInfo, true);
	m_pdlgHisErrorInfo->ShowWindow(bShow ? SW_SHOW : SW_HIDE);

	/*************************************<对图像大小进行自适应>************************************[12/4/2017 ConanSteve]*/
	CMainFrame *pFrame = (CMainFrame *)AfxGetMainWnd();

	/*************************************<Static静态文本显示>************************************[12/4/2017 ConanSteve]*/
// 	//**[12/1/2017 ConanSteve]**:  检测总数
// 	bShow = m_layout.GetPosition(_T("CheckCountLabel"), rectLabel);
// 	if (NULL != m_staticCheckCntLabel.m_hWnd)
// 	{
// 		m_staticCheckCntLabel.MoveWindow(rectLabel);
// 		m_staticCheckCntLabel.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
// 	}
// 
// 	//**[12/1/2017 ConanSteve]**:  错误总数
// 	bShow = m_layout.GetPosition(_T("ErrorCountLabel"), rectLabel);
// 	if (NULL != m_staticErrorCntLabel.m_hWnd)
// 	{
// 		m_staticErrorCntLabel.MoveWindow(rectLabel);
// 		m_staticErrorCntLabel.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
// 	}
// 
// 	//**[12/1/2017 ConanSteve]**:  合格总数
// 	bShow = m_layout.GetPosition(_T("GoodCountLabel"), rectLabel);
// 	if (NULL != m_staticErrorCntLabel.m_hWnd)
// 	{
// 		m_staticGoodCntLabel.MoveWindow(rectLabel);
// 		m_staticGoodCntLabel.ShowWindow(bShow ? SW_SHOWNORMAL : SW_HIDE);
// 	}
// 	
// 	//**[12/1/2017 ConanSteve]**:  合格率
// 	bShow = m_layout.GetPosition(_T("GoodRateLabel"), rectLabel);
// 	if (NULL != m_staticGoodRateLabel.m_hWnd)
// 	{
// 		m_staticGoodRateLabel.MoveWindow(rectLabel);
// 		m_staticGoodRateLabel.ShowWindow(bShow ? SW_SHOWNORMAL : SW_HIDE);
// 	}
// 
// 	//**[12/4/2017 ConanSteve]**:  公司Logo
// 	bShow = m_layout.GetPosition(_T("StaticLabelCompanyLogo"), rectLabel);
// 	if (NULL != m_staticCompanyLogo.m_hWnd)
// 	{
// 		m_staticCompanyLogo.MoveWindow(rectLabel);
// 		m_staticCompanyLogo.ShowWindow(bShow ? SW_SHOWNORMAL : SW_HIDE);
// 	}
// 
// 	//**[12/4/2017 ConanSteve]**:  时间
// 	bShow = m_layout.GetPosition(_T("StaticLabelTime"), rectLabel);
// 	if (NULL != m_staticTime.m_hWnd)
// 	{
// 		m_staticTime.MoveWindow(rectLabel);
// 		m_staticTime.ShowWindow(bShow ? SW_SHOWNORMAL : SW_HIDE);
// 	}
// 	/*************************************<按钮显示>************************************[12/4/2017 ConanSteve]*/
// 	//**[12/1/2017 ConanSteve]**:  模板管理
// 	bShow = m_layout.GetPosition(_T("ButtonSwitchModel"), rectLabel);
// 	if (NULL != m_staticGoodRateLabel.m_hWnd)
// 	{
// 		m_btnSwitchModel.MoveWindow(rectLabel);
// 		m_btnSwitchModel.ShowWindow(bShow ? SW_SHOWNORMAL : SW_HIDE);
// 	}
// 	//**[12/8/2017 ConanSteve]**:  接口卡工具
// 	{
// 		bShow = m_layout.GetPosition(_T("ButtonOpenIOCardTools"), rectLabel);
// 		if (NULL != m_btnOpenIOCardTools.m_hWnd)
// 		{
// 			m_btnOpenIOCardTools.MoveWindow(rectLabel);
// 			m_btnOpenIOCardTools.ShowWindow(bShow ? SW_SHOWNORMAL : SW_HIDE);
// 		}
// 	}
// 	//**[12/13/2017 ConanSteve]**:  开始记录日志
// 	{
// 		bShow = m_layout.GetPosition(_T("ButtonRecord"), rectLabel);
// 		if (NULL != m_btnRecord.m_hWnd)
// 		{
// 			m_btnRecord.MoveWindow(rectLabel);
// 			m_btnRecord.ShowWindow(bShow ? SW_SHOWNORMAL : SW_HIDE);
// 		}
// 	}
// 
// 	//**[12/8/2017 ConanSteve]**:  CListCtrl 
	bShow = m_layout.GetPosition(_T("ListctrlRealtimeRunInfo"), rectLabel);
	if (NULL != m_listctrlRealtimeRunInfo.m_hWnd)
	{
		m_listctrlRealtimeRunInfo.MoveWindow(rectLabel);
		m_listctrlRealtimeRunInfo.ShowWindow(bShow ? SW_SHOWNORMAL : SW_HIDE);
	}


	//::MoveWindow(m_pdlgHisErrorInfo->m_hWnd, 0, rectThis.Height()-150, 400, 150, true);
	//::MoveWindow(m_pdlgHisErrorInfo->m_hWnd, rectHisErrorInfo.left, rectHisErrorInfo.top, rectHisErrorInfo.Width(), rectHisErrorInfo.Height(), true);
	// TODO: Add your message handler code here


// 	CPaintDC dc(this);
// 	CRect rect{ 0 };
// 	GetClientRect(&rect);
// 	CBrush brush;
// 	//brush.CreateSolidBrush(GetSysColor(COLOR_WINDOW));
// 	brush.CreateSolidBrush(COLORREF(RGB(255, 255, 255)));
// 	dc.FillRect(&rect, &brush);
// 	brush.DeleteObject();
}


BOOL CDlgRunning::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


int CDlgRunning::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_pFrame = (CMainFrame *)AfxGetMainWnd();
	CRect rectThis;
	GetClientRect(rectThis);
	CString sPathFileOfUICfg{ _T("") };
	sPathFileOfUICfg.Format(_T("%s_%d_%d.ini"), CExFn::GetCurrentAppPath() + _T("\\Config\\UIConfig"), GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	CINICfg iniCfgUI(sPathFileOfUICfg);
	CINICfg iniAlgCfg(CExFn::GetCurrentAppPath() + _T("\\Config\\AlgConfig.ini"));
	m_iGrabberNum = iniCfgUI.GetIntEx(_T("基础参数"), _T("相机个数"), iniAlgCfg.GetInt(_T("Basic Parameters"), _T("CameraNum")));

	if (0 != m_iGrabberNum)
	{
		m_p_rectCamImg = unique_ptr<CRect[]>(new CRect[m_iGrabberNum]);
		m_p_rectAlgImg = unique_ptr<CRect[]>(new CRect[m_iGrabberNum]);
		ASSERT(NULL != m_p_rectCamImg);
		ASSERT(NULL != m_p_rectAlgImg);
	}

	if (0 != m_iGrabberNum)
	{
		m_pStaticCamImg = unique_ptr<CStatic[]>(new CStatic[m_iGrabberNum]);
		m_pStaticAlgImg = unique_ptr<CStatic[]>(new CStatic[m_iGrabberNum]);
		ASSERT(NULL != m_pStaticCamImg);
		ASSERT(NULL != m_pStaticAlgImg);
	}
	for (int i = 0; i < m_iGrabberNum; ++i)
	{
		m_pStaticCamImg[i].Create(_T(""), WS_CHILD| SS_BITMAP| WS_BORDER, m_p_rectCamImg[i], this, ID_STATIC_CAMERA_IMAGE + i);
		m_pStaticAlgImg[i].Create(_T(""), WS_CHILD | SS_BITMAP| WS_BORDER, m_p_rectAlgImg[i], this, ID_STATIC_ALGORITHM_IMAGE + i);
		m_pStaticCamImg[i].ShowWindow(SW_SHOWNORMAL);
		m_pStaticAlgImg[i].ShowWindow(SW_SHOWNORMAL);
	}
	m_pdlgHisErrorInfo = make_shared<CDlgHisErrorInfo>();
	m_pdlgHisErrorInfo->Create(IDD_DIALOG_HisErrorInfo, this);
	m_pdlgHisErrorInfo->ShowWindow(SW_SHOW);
	

	/*************************************<信息显示>************************************[12/1/2017 ConanSteve]*/
	CRect rectLable;
	bool bShow{ false };//**[12/1/2017 ConanSteve]**:  控制按钮是否显示
	m_staticCheckCntLabel.Create(tr("检测总数："), WS_CHILD, rectLable, this, IDC_STATIC_CHECK_COUNT);

	//**[12/1/2017 ConanSteve]**:  错误总数
	m_staticErrorCntLabel.Create(tr("不合格总数："), WS_CHILD, rectLable, this, IDC_STATIC_ERROR_COUNT);
	//**[12/1/2017 ConanSteve]**:  合格总数
	m_staticGoodCntLabel.Create(tr("合格总数："), WS_CHILD, rectLable, this, IDC_STATIC_ERROR_COUNT);
	m_staticGoodRateLabel.Create(tr("合格率："), WS_CHILD, rectLable, this, IDC_STATIC_GOOD_RATE);
	
	//**[12/1/2017 ConanSteve]**: 
	{
		m_listctrlRealtimeRunInfo.Create(WS_BORDER | LVS_REPORT | LVS_ALIGNLEFT, rectLable, this, IDC_LISTCTRL_REALTIME_RUN_INFO);
		//获取及设置风格  
		LONG lStyle;
		lStyle = GetWindowLong(m_listctrlRealtimeRunInfo.m_hWnd, GWL_STYLE);         //获取当前窗口style  
		lStyle &= ~LVS_TYPEMASK;                                            //清除显示方式位  
		lStyle |= LVS_REPORT;                                               //设置style  
		SetWindowLong(m_listctrlRealtimeRunInfo.m_hWnd, GWL_STYLE, lStyle);          //设置style  
		m_listctrlRealtimeRunInfo.ShowWindow(SW_SHOW);

		//获取及设置扩展风格  
		DWORD dwStyle = m_listctrlRealtimeRunInfo.GetExtendedStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;                            //选中某行使整行高亮（只适用与report风格的listctrl）  
		dwStyle |= LVS_EX_GRIDLINES;                                //网格线（只适用与report风格的listctrl）  
		dwStyle |= LVS_EX_CHECKBOXES;                               //item前生成checkbox控件  
		m_listctrlRealtimeRunInfo.SetExtendedStyle(dwStyle);             //设置扩展风格  

																	 // 添加控件内容  

		m_listctrlRealtimeRunInfo.InsertColumn(0, "系统运行信息", LVCFMT_LEFT, 100);
	}

	/*************************************<信息显示Button>************************************[12/27/2017 ConanSteve]*/
	{
		m_iCustomLabelNum = (IDC_BUTTON_LABEL_END)-(IDC_BUTTON_LABEL_BEGIN);
		m_iCustomLabelStyle = iniCfgUI.GetIntEx(_T("信息标签窗口"), _T("阴影风格"), 0, _T("0-8为有效值"));
		if (m_iCustomLabelStyle < 0 || m_iCustomLabelStyle>8)
			m_iCustomLabelStyle = 0;
		m_iCustomLabelFontSize = iniCfgUI.GetIntEx(_T("信息标签窗口"), _T("文本字体大小"), 20);
		m_iCustomLabelFontWeight = iniCfgUI.GetIntEx(_T("信息标签窗口"), _T("文本字体Weight"), 400);
		m_iCustomLabelFontTextColor = iniCfgUI.GetIntEx(_T("信息标签窗口"), _T("文本颜色"), RGB(0x0, 0x72, 0xc6));
		m_sCustomLabelFontName = iniCfgUI.GetStringEx(_T("信息标签窗口"), _T("文本字体"), m_sCustomLabelFontName, _T("黑体"));
		m_bEnableCustomLabelBtnStyle = iniCfgUI.GetBoolEx(_T("信息标签窗口"), _T("是否按钮风格"), false);
		int i = 0;
		CString sText{ _T("") };
		UINT uBtnID = 0;
		for (; i < m_iCustomLabelNum; ++i)
		{
			uBtnID = i + IDC_BUTTON_LABEL_BEGIN;
			switch (uBtnID)
			{
			case IDC_BUTTON_LABEL_MODEL_NAME:
				sText = tr("配方");
				break;
			case IDC_BUTTON_LABEL_USER_INFO:
				sText = tr("用户");
				break;
			case IDC_BUTTON_LABEL_CHECK_COUNT_TOTLE:
				sText = tr("检测总数");
				break;
			case IDC_BUTTON_LABEL_GOOD_COUNT_TOTLE:
				sText = tr("合格总数");
				break;
			case IDC_BUTTON_LABEL_ERROR_COUNT_TOTLE:
				sText = tr("不合格总数");
				break;
			case IDC_BUTTON_LABEL_GOOD_RATE_TOTLE:
				sText = tr("合格率");
				break;
			case IDC_BUTTON_LABEL_CHECK_SPEED:
				sText = tr("检测速度");
				break;
			default:
				break;
			}
			m_arr_btnCustomLabel[i].Create(sText, WS_CHILD, rectLable, this, uBtnID);
			m_arr_btnCustomLabel[i].SetFont(m_sCustomLabelFontName, m_iCustomLabelFontSize, m_iCustomLabelFontWeight, FALSE, FALSE);
			m_arr_btnCustomLabel[i].SetTextColor(m_iCustomLabelFontTextColor);
// 			CString sTempText{ _T("") };
// 			sTempText.Format(_T("信息标签%d"), i);
// 			bool bShowLabel = iniCfgUI.GetBoolEx(_T("信息标签是否显示"), sTempText, true);
// 			if (bShowLabel)
				m_vec_pCustomLabel.push_back(&m_arr_btnCustomLabel[i]);
		}
		/*************************************<创建流水线信息显示标签>************************************[12/28/2017 ConanSteve]*/
		CINICfg iniAlgCfg(CExFn::GetCurrentAppPath() + _T("\\Config\\AlgConfig.ini"));
		CString sModelName{ _T("") };
		sModelName = iniAlgCfg.GetStringEx(_T("Demo Controls"), _T("DefaultModel"), sModelName);
		CINICfg iniCheckSystem(CExFn::GetCurrentAppPath() + _T("\\ModelInfo\\") + sModelName + _T("\\CheckSystemConfig.ini"));
		m_iAssemblyLineNum = iniCheckSystem.GetIntEx(_T("Critical Config"), _T("AssemblyLineNumber"));
		i = 0;
		if (1 < m_iAssemblyLineNum&& m_iAssemblyLineNum <= 5)
		{
			for (int j = 0; j < m_iAssemblyLineNum; ++j)
			{
				for (int k = 0; k < 5; ++k)
				{
					switch (k)
					{
					case 0://**[1/24/2018 ConanSteve]**:  流水线检测总数
						sText.Format(_T("%s%d%s:"), tr("流水线"), j + 1, tr("检测总数"));
						uBtnID = IDC_BUTTON_LABEL_TOTLE_COUNT_ASSEMBLY + j;
						break;
					case 1://**[1/24/2018 ConanSteve]**:  流水线好品总数
						sText.Format(_T("%s%d%s:"), tr("流水线"), j + 1, tr("合格总数"));
						uBtnID = IDC_BUTTON_LABEL_GOOD_COUNT_ASSEMBLY + j;
						break;
					case 2://**[1/24/2018 ConanSteve]**:  流水线坏品总数
						sText.Format(_T("%s%d%s:"), tr("流水线"), j + 1, tr("不合格总数"));
						uBtnID = IDC_BUTTON_LABEL_BAD_COUNT_ASSEMBLY + j;
						break;
					case 3://**[1/24/2018 ConanSteve]**:  流水线合格率
						sText.Format(_T("%s%d%s:"), tr("流水线"), j + 1, tr("合格率"));
						uBtnID = IDC_BUTTON_LABEL_GOOD_RATIO_ASSEMBLY + j;
						break;
					case 4://**[1/24/2018 ConanSteve]**:	流水线检测速度 
						sText.Format(_T("%s%d%s:"), tr("流水线"), j + 1, tr("检测速度"));
						uBtnID = IDC_BUTTON_LABEL_CHECK_SPEED_ASSEMBLY + j;
						break;
					default:
						break;
					}
					int iLableIndexOfAssembly = j * 5 + k;
					m_arr_labelPerAssembly[iLableIndexOfAssembly].Create(sText, WS_CHILD, rectLable, this, uBtnID);
					m_arr_labelPerAssembly[iLableIndexOfAssembly].SetFont(m_sCustomLabelFontName, m_iCustomLabelFontSize, m_iCustomLabelFontWeight, FALSE, FALSE);
					m_arr_labelPerAssembly[iLableIndexOfAssembly].SetTextColor(m_iCustomLabelFontTextColor);
// 					CString sTempText{ _T("") };
// 					sTempText.Format(_T("流水线信息标签%d"), i);
// 					bool bShowLabel = iniCfgUI.GetBoolEx(_T("信息标签是否显示"), sTempText, true);
// 					if (bShowLabel)
//						m_vec_pCustomLabel.push_back(&m_arr_labelPerAssembly[iLableIndexOfAssembly]);
					m_vec_pCustomLabel.push_back(&m_arr_labelPerAssembly[iLableIndexOfAssembly]);
					++i;
				}
			}
		}
	}
	/*************************************<Button>************************************[12/8/2017 ConanSteve]*/
	{
		m_iCustomBtnNum = (IDC_BUTTON_END)-(IDC_BUTTON_BEGIN);
		m_iCustomBtnStyle = iniCfgUI.GetIntEx(_T("按钮窗口"), _T("阴影风格"), 7, _T("0-8为有效值"));
		if (m_iCustomBtnStyle < 0 || m_iCustomBtnStyle>8)
			m_iCustomBtnStyle = 0;
		m_iCustomBtnFontSize = iniCfgUI.GetIntEx(_T("按钮窗口"), _T("文本字体大小"), 20);
		m_iCustomBtnFontWeight = iniCfgUI.GetIntEx(_T("按钮窗口"), _T("文本字体Weight"), 400);
		m_iCustomBtnFontTextColor = iniCfgUI.GetIntEx(_T("按钮窗口"), _T("文本颜色"), RGB(0x0, 0x72, 0xc6));
		m_sCustomBtnFontName = iniCfgUI.GetStringEx(_T("按钮窗口"), _T("文本字体"), m_sCustomLabelFontName, _T("黑体"));
		for (int i = 0; i < m_iCustomBtnNum; ++i)
		{
			CString sText{ _T("") };
			UINT uBtnID = i + IDC_BUTTON_BEGIN;
			switch (uBtnID)
			{
			case IDC_BUTTON_RESET_SYSTEM:
				sText = tr("计数清零");
				break;
			case IDC_BUTTON_OPEN_IOCARD_TOOLS:
				sText = tr("接口卡工具");
				break;
			case IDC_BUTTON_SWITCH_MODEL:
				sText = tr("配方管理");
				break;
			case IDC_BUTTON_RECORD:
				sText = tr("开始记录");
				break;
			case IDC_BUTTON_DEFECT_STATISTICS:
				sText = tr("缺陷统计");
				break;
			case IDC_BUTTON_DATA_STATISTICS:
				sText = tr("产品数据统计");
				break;
			case IDC_BUTTON_OPEN_MULTI_OBJECT_RUN_INFO_WINDOW:
				sText = tr("多对象运行信息");
				break;
			case IDC_BUTTON_USER_MANAGEMENT:
				sText = tr("用户管理");
				break;
			case IDC_BUTTON_SWITCH_USER:
				sText = tr("切换用户");
				break;
			case  IDC_BUTTON_SUPER_ADMIN_SETTING:
				sText = tr("超级管理员设置");
				break;
			case IDC_BUTTON_RESIZE_LAYOUT:
				sText = tr("重新布局窗口");
				break;
			case IDC_BUTTON_SYSTEM_DEVELOPER_SETTING:
				sText = tr("系统开发者设置");
				break;
			default:
				break;
			}
			m_arr_btnCustomBtn[i].Create(sText, WS_CHILD, rectLable, this, uBtnID);
			m_arr_btnCustomBtn[i].SetFont(m_sCustomBtnFontName, m_iCustomBtnFontSize, m_iCustomBtnFontWeight, FALSE, FALSE);
			m_arr_btnCustomBtn[i].SetTextColor(m_iCustomBtnFontTextColor);
			m_vec_pCustomBtn.push_back(&m_arr_btnCustomBtn[i]);
		}

	}
	
	
	return 0;
}

//显示图像
void CDlgRunning::DisplayImage(const int iCamGroup, int iIndex)
{

	CDC* pDC = m_pStaticCamImg[iCamGroup].GetDC();
	DisplayCameraImage(pDC, iCamGroup, iIndex, NoFlip);
	ReleaseDC(pDC);

}

void CDlgRunning::DisplayCameraImage(CDC *pDC, const int iCamGroup, int iIndex, e_ShowModeFlip eShowMode)
{
	int i = 0, j = 0;
	int iImageWidth = 0, iImageHeight = 0, iImageByte = 0, iImageSize = 0;
	int  iDisplayLeft = 0, iDisplayTop = 0;
	CString  strShow;

	CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	pDC->SelectObject(pBrush);
	pDC->SetTextColor(RGB(0, 255, 255));  //设置文本颜色
	pDC->SetBkMode(TRANSPARENT);        //将文本背景设为透明的

	CMainFrame * pFrame = g_pFrame;// (CMainFrame*)AfxGetMainWnd();
	if (pFrame == NULL)
	{
		return;
	}

	try
	{
		iImageWidth = pFrame->m_arr_sCameraImageInfo[iCamGroup].m_iCameraImageWidth;
		iImageHeight = pFrame->m_arr_sCameraImageInfo[iCamGroup].m_iCameraImageHeight;
		iImageByte = pFrame->m_arr_GrabInfo[iCamGroup].m_bIsColorFilter == 1?3:1;
		iImageSize = pFrame->m_arr_sCameraImageInfo[iCamGroup].m_iCameraImageSize;
		int iOffset = iIndex*iImageSize*pFrame->m_arr_iSmallImgCntPerGrab[iCamGroup];

		ShowDIBImage(pDC, m_p_rectCamImg[iCamGroup], eShowMode, Stretch,
			&(pFrame->m_arr_pbtBufferOfBayerImg[iCamGroup][0]),
			iImageWidth, iImageHeight, iImageByte, iCamGroup);


		iDisplayLeft = m_p_rectCamImg[iCamGroup].left;
		iDisplayTop = m_p_rectCamImg[iCamGroup].top;

		// 		strShow.Format("第%d工位相机", iCamGroup + 1);
		// 		pDC->TextOut(iDisplayLeft, iDisplayTop, strShow);
		// 
		// 		strShow.Format("第%d路", iCamGroup + 1);
		// 		pDC->TextOut(iDisplayLeft, iDisplayTop + 20, strShow);
		// 
		// 		strShow.Format("采集大张数-%d", pFrame->m_sElement[iCamGroup][iIndex].iImageSN);
		// 		pDC->TextOut(iDisplayLeft, iDisplayTop + 40, strShow);
		// 
		// 		pDC->SetTextColor(RGB(255, 0, 0));  //设置文本颜色
		// 		strShow.Format("%s", pFrame->m_sElement[iCamGroup][iIndex].strErrorInfo);
		// 		pDC->TextOut(iDisplayLeft, iDisplayTop + 60, strShow);

	}
	catch (...)
	{
		// 		CString  strExceptionDes;
		// 		strExceptionDes.Format("DisplayCameraImageOne-相机%d出错!", iCamGroup + 1);
		// 		pFrame->m_LogManager.MyWriteLogFile("图像显示", strExceptionDes, AbnormityLog);
	}
}
void CDlgRunning::ShowDIBImage(CDC* pDC, CRect Rect, e_ShowModeFlip ShowModeFlip, e_ShowModeStretch ShowModeStretch,
	const BYTE* pImageBuff, long lImageWidth, long lImageHeight, long lImageBitCount, int iCameraSN)
{
	int XDest = 0;
	int YDest = 0;
	int nDestWidth = 0;
	int nDestHeight = 0;

	ASSERT(pImageBuff != NULL && lImageWidth >= 0 && lImageHeight >= 0 && lImageBitCount > 0);

	m_csShowBitmapInfoSection.Lock();

// 	ScreenToClient(&Rect);
// 	XDest = Rect.left;
// 	YDest = Rect.top;
	XDest = 0;
	YDest = 0;
	nDestWidth = Rect.Width()-2;
	nDestHeight = Rect.Height()-2;

	if (ShowModeStretch == NoStretch)
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
			g_pFrame->m_arr_pBitmapInfo[iCameraSN].get(), // lpbitsinfo 
			DIB_RGB_COLORS); // wusage 
	}
	else
	{
		switch (ShowModeFlip)
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
			g_pFrame->m_arr_pBitmapInfo[iCameraSN].get(),			//BMP图像描述信息
			DIB_RGB_COLORS,
			SRCCOPY
		);
	}

	m_csShowBitmapInfoSection.Unlock();
}

void CDlgRunning::OnButtonClick(UINT nID)
{
	m_pFrame = (CMainFrame*)AfxGetMainWnd();
	switch (nID)
	{
	case IDC_BUTTON_RESET_SYSTEM:
	{
		if (IDYES == MessageBox(tr("确定重置所有计数吗"), _T("提醒"), MB_YESNO | MB_ICONQUESTION))
		{
			m_pFrame->ResetSystem();
		}
		break;
	}
	case IDC_BUTTON_OPEN_IOCARD_TOOLS:
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		if (!pFrame->m_threeTierRights.VerifyRights(e_UserType_Debugger))
			return;
		m_pFrame->OnBtnClickOpenIOCardTools();
		break;
	}
	case IDC_BUTTON_SWITCH_MODEL:
	{
		CDlgSwitchModel dlg;
		m_pDlgSwitchModel = make_shared<CDlgSwitchModel>();
		//m_pDlgSwitchModel->Create(IDD_DIALOG_SwitchModel, this);
		//m_pDlgSwitchModel->ShowWindow(SW_NORMAL); 
		dlg.DoModal();
		CString sText{ _T("") };
		sText.Format(_T("%s: %s"), tr("配方"), m_pFrame->m_sModelName);
		SetDlgItemText(IDC_BUTTON_LABEL_MODEL_NAME, sText);
		m_pFrame->m_iniAlgConfig.WriteString(_T("Demo Controls"), _T("DefaultModel"), m_pFrame->m_sModelName);
		break;
	}
	case IDC_BUTTON_RECORD:
	{
		CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
		pFrame->m_bEnableLog = !pFrame->m_bEnableLog;
		if (pFrame->m_bEnableLog)
		{
			SetDlgItemText(IDC_BUTTON_RECORD, tr("停止记录"));
		}
		else
		{
			SetDlgItemText(IDC_BUTTON_RECORD, tr("开始记录"));
		}
		break;
	}
	case IDC_BUTTON_DEFECT_STATISTICS:
	{
		g_pFrame->OnBtnClickOpenDefectStatistics();
		break;
	}
	case IDC_BUTTON_DATA_STATISTICS:
	{
		g_pFrame->OnBtnClickOpenDataStatistics();
		break;
	}
	case IDC_BUTTON_OPEN_MULTI_OBJECT_RUN_INFO_WINDOW:
	{
		if (g_pFrame->m_bEnableInitMultiObjectAndCheck)
		{
			CExtenInfo extenInfo;
			s_AlgModelInput sIn;
			extenInfo.push_back(COptionalFunc(e_Fn_AlgModelInput_WhetherShowObjectRunInfo).SetBool(true));
			sIn.iOperSig = e_AlgModelInputOper_UseExtenInfoFunc;
			sIn.iReserve4 = (int)&extenInfo;
			g_pFrame->m_multiObject.Set(sIn);
		}
// 		if (g_pFrame->m_bEnableInitMultiObjectAndCheck)
// 		{
// 			CExtenInfo extenInfo;
// 			CSetObjStatusWnd setObjStatus;
// 			s_AlgModelInput sIn;
// 			setObjStatus.SetIsShow(TRUE);
// 			extenInfo.push_back(&setObjStatus);
// 			sIn.iOperSig = e_AlgModelInputOper_UseExtenInfoFunc;
// 			sIn.iReserve4 = (int)&extenInfo;
// 			g_pFrame->m_multiObject.Set(sIn);
// 		}
		break;
	}
	case IDC_BUTTON_USER_MANAGEMENT:
		g_pFrame->m_threeTierRights.ManageUsers();
		break;
	case IDC_BUTTON_SWITCH_USER:
	{
		if(g_pFrame->m_threeTierRights.Login(g_pFrame))
			ResizeLayout();
		break;
	}
	case IDC_BUTTON_RESIZE_LAYOUT:
		g_pFrame->ResizeLayout();
		break;
	default:
		break;
	}
	UpdateData(FALSE);
}


HBRUSH CDlgRunning::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		CFont font;
		font.CreateFont(
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

		//GetDlgItem(pWnd->GetDlgCtrlID())->SetFont(&font);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0, 0, 255));
		//return (HBRUSH)::GetStockObject(NULL_BRUSH);
	}


// 	switch (pWnd->GetDlgCtrlID())
// 	{
// 	case IDC_STATIC_Time:
// 		pDC->SetBkMode(TRANSPARENT);
// 		pDC->SetTextColor(RGB(0, 0, 255));
// 		//return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
// 	default:
// 		break;
// 	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


bool CDlgRunning::ResizeLayout()
{
	CRect rectThis;
	GetClientRect(rectThis);
	m_pFrame = (CMainFrame *)AfxGetMainWnd();
	CString sPathFileOfUICfg{ _T("") };
	sPathFileOfUICfg.Format(_T("%s_%d_%d.ini"), CExFn::GetCurrentAppPath() + _T("\\Config\\UIConfig"), GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	CINICfg iniCfgUI(sPathFileOfUICfg);
	iniCfgUI.WriteInt(_T("实际运行窗口参数"), _T("X轴宽度"), rectThis.Width());
	iniCfgUI.WriteInt(_T("实际运行窗口参数"), _T("Y轴高度"), rectThis.Height());
	/*************************************<标签>************************************[12/27/2017 ConanSteve]*/
	//**[1/27/2018 ConanSteve]**:	将所有标签添加进三级权限模块
	for (int i = 0; i < m_vec_pCustomLabel.size(); ++i)
	{
		m_pFrame->m_threeTierRights.AddCtrl(m_vec_pCustomLabel[i], e_RightType_EnableShow);
		m_vec_pCustomLabel[i]->ShowWindow(SW_HIDE);
	}
	{
		int iCnt_X = iniCfgUI.GetIntEx(_T("信息标签窗口"), _T("X轴最大个数"), 2);
		int iCnt_Y = iniCfgUI.GetIntEx(_T("信息标签窗口"), _T("Y轴最大个数"), (m_vec_pCustomLabel.size()+1)/2);
		int iWindowWidth = iniCfgUI.GetIntEx(_T("信息标签窗口"), _T("X轴宽度"), rectThis.Width()*2/3);
		int iWindowHeight = iniCfgUI.GetIntEx(_T("信息标签窗口"), _T("Y轴高度"), rectThis.Height()/3);
		int iWindowLeft = iniCfgUI.GetIntEx(_T("信息标签窗口"), _T("X轴Left"), 0);
		int iWindowTop = iniCfgUI.GetIntEx(_T("信息标签窗口"), _T("Y轴Top"), rectThis.Height()*2 / 3);
		int	iGap_X = iniCfgUI.GetIntEx(_T("信息标签窗口"), _T("X轴间距"), 5);//**[12/27/2017 ConanSteve]**:  相邻窗口之间的距离
		int	iGap_Y = iniCfgUI.GetIntEx(_T("信息标签窗口"), _T("Y轴间距"), 5);
		int iBaseCoordination = iniCfgUI.GetIntEx(_T("信息标签窗口"), _T("基准轴"), 1, _T(" 0表示基本轴为水平， 1为垂直"));

 		CRect rectGroup = CRect(iWindowLeft, iWindowTop, iWindowLeft+iWindowWidth, iWindowTop+iWindowHeight);
		vector<CWnd*> vec_ShowWnd;//**[1/27/2018 ConanSteve]**:	需要显示的标签
		for (int i = 0; i < m_vec_pCustomLabel.size(); ++i)
		{
			if (g_pFrame->m_threeTierRights.VerifyRights(m_vec_pCustomLabel[i]), e_RightType_EnableShow)
				vec_ShowWnd.push_back(m_vec_pCustomLabel[i]);
		}
		vector<CRect> vecRect =  CExFn::AutoLayoutRect(rectGroup, iCnt_X, iCnt_Y, iGap_X, iGap_Y, vec_ShowWnd.size(), iBaseCoordination);

		for (int i = 0; i < vecRect.size(); ++i)
		{
			vec_ShowWnd[i]->MoveWindow(vecRect[i]);
			vec_ShowWnd[i]->ShowWindow(SW_SHOW);
			vec_ShowWnd[i]->EnableWindow(m_bEnableCustomLabelBtnStyle);
		}
		/*****************************************************************************************/
		//**[1/26/2018 ConanSteve]**:	修改模板名称 
		CString sText{ _T("") };
		sText.Format(_T("%s: %s"),tr("配方"), g_pFrame->m_sModelName);
		SetDlgItemText(IDC_BUTTON_LABEL_MODEL_NAME, sText);

		//**[1/26/2018 ConanSteve]**:	修改用户名和用户等级 
		s_UserInfoOfThreeTierRights sUserInfo;
		g_pFrame->m_threeTierRights.GetCurUserInfo(sUserInfo);
		sText.Format(_T("%s: %s(%s)"), tr("用户"),  0 ==_tcscmp(sUserInfo.lpszUserName,_T("cs"))?_T("******"): sUserInfo.lpszUserName, sUserInfo.lpszUserType);
		SetDlgItemText(IDC_BUTTON_LABEL_USER_INFO, sText);
	}
	/*************************************<按钮>************************************[12/27/2017 ConanSteve]*/
	//**[1/16/2018 ConanSteve]**:  添加按钮到三级权限中
	for (int i = 0; i < m_iCustomBtnNum; ++i)
	{
		switch (i + IDC_BUTTON_BEGIN)
		{
			//**[1/26/2018 ConanSteve]**:	这些控件的权限是固定的，任何人员不可配置
		case IDC_BUTTON_OPEN_IOCARD_TOOLS:
		case IDC_BUTTON_SUPER_ADMIN_SETTING:
		case IDC_BUTTON_USER_MANAGEMENT:
		case IDC_BUTTON_SWITCH_USER:
		case IDC_BUTTON_RESIZE_LAYOUT:
		case IDC_BUTTON_SYSTEM_DEVELOPER_SETTING:
		case IDC_BUTTON_OPEN_MULTI_OBJECT_RUN_INFO_WINDOW:
			break;
		default:
			m_pFrame->m_threeTierRights.AddCtrl(m_vec_pCustomBtn[i]);
			break;
		}
		//**[1/16/2018 ConanSteve]**:  隐藏所有按钮控件
		m_vec_pCustomBtn[i]->ShowWindow(SW_HIDE);
	}
	{
		int iCnt_X = iniCfgUI.GetIntEx(_T("按钮窗口"), _T("X轴最大个数"), 1);
		int iCnt_Y = iniCfgUI.GetIntEx(_T("按钮窗口"), _T("Y轴最大个数"), m_vec_pCustomBtn.size());
		int iWindowWidth = iniCfgUI.GetIntEx(_T("按钮窗口"), _T("X轴宽度"), rectThis.Width() / 3);
		int iWindowHeight = iniCfgUI.GetIntEx(_T("按钮窗口"), _T("Y轴高度"), rectThis.Height() / 3);
		int iWindowLeft = iniCfgUI.GetIntEx(_T("按钮窗口"), _T("X轴Left"), rectThis.Width() * 2 / 3);
		int iWindowTop = iniCfgUI.GetIntEx(_T("按钮窗口"), _T("Y轴Top"), rectThis.Height() * 2 / 3);
		int	iGap_X = iniCfgUI.GetIntEx(_T("按钮窗口"), _T("X轴间距"), 5);//**[12/27/2017 ConanSteve]**:  相邻窗口之间的距离
		int	iGap_Y = iniCfgUI.GetIntEx(_T("按钮窗口"), _T("Y轴间距"), 5);
		int iBaseCoordination = iniCfgUI.GetIntEx(_T("按钮窗口"), _T("基准轴"), 1, _T(" 0表示基本轴为水平， 1为垂直"));
		CRect rectGroup = CRect(iWindowLeft, iWindowTop, iWindowLeft + iWindowWidth, iWindowTop + iWindowHeight);
		//m_sStaticGroupOfBtn.Create(_T(""), WS_CHILD | WS_BORDER, rectGroup, this);
		//m_sStaticGroupOfBtn.ShowWindow(SW_SHOW);
		//**[1/16/2018 ConanSteve]**:  统计需要显示的控件个数
		//**[1/16/2018 ConanSteve]**:  计算最佳布局方式
		bool bShowCtrl = false;
		vector<CButton*> vec_pBtnShow;//**[1/26/2018 ConanSteve]**:	需要被显示的按钮 
		for (int i = 0; i < m_vec_pCustomBtn.size(); ++i)
		{
			switch (i + IDC_BUTTON_BEGIN)
			{

			case IDC_BUTTON_SYSTEM_DEVELOPER_SETTING:
				bShowCtrl = g_pFrame->m_threeTierRights.VerifyRights(e_UserType_SystemDeveloper);
				break;
				//**[1/26/2018 ConanSteve]**:	超级管理员级以上才显示的权限 
			case IDC_BUTTON_SUPER_ADMIN_SETTING:
			case IDC_BUTTON_RESIZE_LAYOUT:
				bShowCtrl = g_pFrame->m_threeTierRights.VerifyRights(e_UserType_SuperAdmin);
				break;
				//**[1/26/2018 ConanSteve]**:	普通管理员级以上才能使用的功能 
			case IDC_BUTTON_USER_MANAGEMENT:
				bShowCtrl = g_pFrame->m_threeTierRights.VerifyRights(e_UserType_Admin);
				break;
				//**[1/26/2018 ConanSteve]**:	只有调试员级以上才能看见的权限 
			case IDC_BUTTON_OPEN_IOCARD_TOOLS:
			case IDC_BUTTON_OPEN_MULTI_OBJECT_RUN_INFO_WINDOW:
				bShowCtrl = g_pFrame->m_threeTierRights.VerifyRights(e_UserType_Debugger);
				break;
			case IDC_BUTTON_SWITCH_USER:
				bShowCtrl = true;
				break;
			default:
				bShowCtrl = g_pFrame->m_threeTierRights.VerifyRights(m_vec_pCustomBtn[i]);
				break;
			}
			if (bShowCtrl)
			{
				vec_pBtnShow.push_back(m_vec_pCustomBtn[i]);
			}
		}
		vector<CRect> vecRect = CExFn::AutoLayoutRect(rectGroup, iCnt_X, iCnt_Y, iGap_X, iGap_Y, vec_pBtnShow.size(), iBaseCoordination);
		for (int i = 0, j = 0; i < vec_pBtnShow.size(); ++i)
		{
			vec_pBtnShow[i]->MoveWindow(vecRect[j++]);
			vec_pBtnShow[i]->ShowWindow(SW_SHOW);
		}
	}
	return true;
}
