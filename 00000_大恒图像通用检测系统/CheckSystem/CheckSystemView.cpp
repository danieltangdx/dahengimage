
// XCMFCView.cpp : implementation of the CXCMFCView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "CheckSystem.h"
#endif

#include "CheckSystemDoc.h"
#include "CheckSystemView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CMainFrame*		g_pFrame; //全局指针
extern CCheckSystemDoc*		g_pDoc;
extern CCheckSystemView*	g_pView;

// CXCMFCView

IMPLEMENT_DYNCREATE(CCheckSystemView, CView)

BEGIN_MESSAGE_MAP(CCheckSystemView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_BN_CLICKED(1000, &CCheckSystemView::OnBnStart)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CXCMFCView construction/destruction

CCheckSystemView::CCheckSystemView()
{
	// TODO: add construction code here

}

CCheckSystemView::~CCheckSystemView()
{
}

BOOL CCheckSystemView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CXCMFCView drawing

void CCheckSystemView::OnDraw(CDC* /*pDC*/)
{
	CCheckSystemDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CXCMFCView printing

BOOL CCheckSystemView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCheckSystemView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCheckSystemView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CXCMFCView diagnostics

#ifdef _DEBUG
void CCheckSystemView::AssertValid() const
{
	CView::AssertValid();
}

void CCheckSystemView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCheckSystemDoc* CCheckSystemView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCheckSystemDoc)));
	return (CCheckSystemDoc*)m_pDocument;
}
#endif //_DEBUG


// CXCMFCView message handlers


BOOL CCheckSystemView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}


int CCheckSystemView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_pdlgTabPage = make_unique<PTabPage[]>(2);
	m_xcTitleBar.Create(this->m_hWnd);
	m_xcTitleBar.AdjustLayout();
// 	m_xcCtrlBtnBar.Create(m_hWnd);
// 	m_xcCtrlBtnBar.AdjustLayout();
// 	m_xcStatusInfoBar.Create(m_hWnd);
// 	m_xcStatusInfoBar.AdjustLayout();
	m_pdlgTabPage[e_TabPage::Running] = make_shared<CDlgRunning>();
	m_pdlgTabPage[e_TabPage::Running]->Create(IDD_DIALOG_Running, this);
	m_pdlgRunning = (CDlgRunning*)m_pdlgTabPage[e_TabPage::Running].get();
	m_pdlgTabPage[e_TabPage::Running]->ShowWindow(SW_SHOW);

	m_pdlgTabPage[e_TabPage::Setting] = make_shared<CDlgSetting>();
	m_pdlgTabPage[e_TabPage::Setting]->Create(IDD_DIALOG_Setting, this);
	m_pdlgSetting = (CDlgSetting*)m_pdlgTabPage[e_TabPage::Setting].get();
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
//	pFrame->m_threeTierRights.Login(pFrame);
	

	return 0;
}


void CCheckSystemView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CView::OnPaint() for painting messages

	
}


BOOL CCheckSystemView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return CView::OnEraseBkgnd(pDC);
}

void CCheckSystemView::OnBnStart()
{
	MessageBox(_T("Hello World"));
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return 0;
}




bool CCheckSystemView::OnBnClickTabPageChange(int iPage)
{
	for (auto i = 0; i < 2; ++i)
	{
		if (i == iPage)
			m_pdlgTabPage[i]->ShowWindow(SW_SHOW);
		else
			m_pdlgTabPage[i]->ShowWindow(SW_HIDE);
	}
	return false;
}


void CCheckSystemView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	CRect rectView{ 0 }, rectTabPage{ 0 }, rectCtrlBtnBar{ 0 }, rectStatusInfoBar{0};
	::GetWindowRect(m_xcTitleBar.m_hWnd, rectTabPage);
	//::GetWindowRect(m_xcCtrlBtnBar.m_hWnd, rectCtrlBtnBar);
	//::GetWindowRect(m_xcStatusInfoBar.m_hWnd, rectStatusInfoBar);
	GetClientRect(&rectView);
	if (NULL != m_xcTitleBar.m_hWnd)
		::MoveWindow(m_xcTitleBar.m_hWnd, 0, 0, rectView.Width(), rectTabPage.Height(), true);
// 	if (NULL != m_xcStatusInfoBar.m_hWnd)
// 		::MoveWindow(m_xcStatusInfoBar.m_hWnd, 0, rectView.Height()-30, rectView.Width(), 30, true);
// 	if (NULL != m_xcCtrlBtnBar.m_hWnd)
// 	 	::MoveWindow(m_xcCtrlBtnBar.m_hWnd, rectView.Width()-rectCtrlBtnBar.Width(), rectTabPage.Height(), rectCtrlBtnBar.Width(), rectView.Height()-rectTabPage.Height(), true);
	
	
	for (int i = 0; i < 2; ++i)
	{
		::MoveWindow(m_pdlgTabPage[i]->m_hWnd, 0, rectTabPage.Height(), rectView.Width(), rectView.Height() - rectTabPage.Height(), true);
	}
// 	CRect rectRunning;
// 	rectRunning.left = 0;
// 	rectRunning.top = rectTabPage.Height();
// 	rectRunning.right = rectView.Width();
// 	rectRunning.bottom = rectView.Height() - rectTabPage.Height();
// 	::MoveWindow(m_pdlgRunning->m_hWnd, 0, rectTabPage.Height(), rectView.Width(), rectView.Height() - rectTabPage.Height(), true);
	//::MoveWindow(m_pdlgRunning->m_hWnd, 0, rectTabPage.Height(), rectView.Width() - rectCtrlBtnBar.Width(), rectView.Height() - rectTabPage.Height(), true);
}


void CCheckSystemView::DisplayGrabImg(int iGrabSN)
{
	m_pdlgRunning->DisplayImage(iGrabSN, 0);
}




void CCheckSystemView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	m_fontTitle.CreateFont(
		50,                        // nHeight
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
/*	GetDlgItem(IDC_WINDOW_TITLE)->SetFont(&m_fontTitle);*/

	m_fontTime.CreateFont(
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

	m_pdlgRunning->GetDlgItem(IDC_STATIC_Time)->SetFont(&m_fontTime);

	m_fontSystem.CreateFont(
		20,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Arial");


	
// 	GetDlgItem(IDC_SAVE_IMAGE)->SetFont(&m_fontSystem);
// 	GetDlgItem(IDC_SYSTEM_CONFIG)->SetFont(&m_fontSystem);
// 	GetDlgItem(IDC_IOCARD)->SetFont(&m_fontSystem);
// 	GetDlgItem(IDC_STOP_SAVE_IMAGE)->SetFont(&m_fontSystem);
// 	GetDlgItem(IDC_CIPHER)->SetFont(&m_fontSystem);

	m_fontAdv.CreateFont(
		15,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Arial");
/*	GetDlgItem(IDC_ADV_MODE)->SetFont(&m_fontAdv);*/

	SetTimer(ID_EVENT_UPDATE_TIME, 1000, NULL); 
	// TODO: 在此添加专用代码和/或调用基类
}


void CCheckSystemView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case ID_EVENT_UPDATE_TIME:
	{
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);
		CString sTime{ _T("") };
		sTime.Format(_T("%04d-%02d-%02d\n  %02d:%02d:%02d"), sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond);
		m_pdlgRunning->GetDlgItem(IDC_BUTTON_LABEL_TIME)->SetWindowText(sTime);
		m_pdlgRunning->m_staticTime.SetWindowText(sTime);
	}
	break;
	default:
		break;
	}
	CView::OnTimer(nIDEvent);
}


void CCheckSystemView::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	CView::PostNcDestroy();
}
