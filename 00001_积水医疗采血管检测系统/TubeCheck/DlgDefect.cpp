// DlgDefect.cpp : implementation file
//

#include "stdafx.h"
#include "TubeCheck.h"
#include "DlgDefect.h"
#include "MainFrm.h"
#include "TubeCheckVIEW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainFrame*			g_pFrame; //全局指针
extern CTubeCheckView*     g_pView;


/////////////////////////////////////////////////////////////////////////////
// CDlgDefect dialog


CDlgDefect::CDlgDefect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDefect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDefect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgDefect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDefect)
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDefect, CDialog)
	//{{AFX_MSG_MAP(CDlgDefect)
	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDefect message handlers

BOOL CDlgDefect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	int cxScreen=0, cyScreen=0;
	cxScreen=GetSystemMetrics(SM_CXSCREEN);
	cyScreen=GetSystemMetrics(SM_CYSCREEN);

	CRect rcDlg,rcTab,rcPage;
	GetWindowRect(&rcDlg);
	rcTab.left = 20;
	rcTab.right = rcDlg.Width()-20;
	rcTab.top = 10;
	rcTab.bottom = rcDlg.Height()-30;
	m_TabCtrl.MoveWindow(rcTab);

	rcPage.left = rcTab.left-20;
	rcPage.right = rcTab.right-20;
	rcPage.top = rcTab.top-20;
	rcPage.bottom=rcTab.bottom-32;

	m_TabCtrl.InsertItem(0,"实时统计信息");
	m_TabCtrl.InsertItem(1,"历史查询");
	m_page1.Create(IDD_DLG_DEFECT_PAGE_1,&m_TabCtrl);
	m_page2.Create(IDD_DLG_DEFECT_PAGE_2,&m_TabCtrl);

	m_page2.m_Connect = pFrame->m_Connect;
	m_page2.m_RecordSet = pFrame->m_RecordSet;

	m_page1.MoveWindow(rcPage);
 	m_page2.MoveWindow(rcPage);

	// Page1
	int iWidth=0,iHeight_Static=0,iHeight_List=0;
	int iLeft_Static=0;
	iLeft_Static = (rcPage.Width()/2-30)/2;
	iWidth = rcPage.Width()/2-10;
	iHeight_Static = 20;
	iHeight_List = rcPage.Height()-iHeight_Static-15;
	CRect rcStatic[c_AllElementCount],rcList[c_AllElementCount];

	rcStatic[Camera1].left = iLeft_Static;
	rcStatic[Camera1].right = rcStatic[Camera1].left+30;
	rcStatic[Camera1].top = rcPage.top+25;
	rcStatic[Camera1].bottom = rcStatic[Camera1].top+iHeight_Static;
	m_page1.GetDlgItem(IDC_WS_1)->MoveWindow(rcStatic[Camera1]);

	rcList[Camera1].left = 5;
	rcList[Camera1].right = rcList[Camera1].left+iWidth;
	rcList[Camera1].top = rcStatic[Camera1].bottom+5;
	rcList[Camera1].bottom = rcStatic[Camera1].top+iHeight_List;
	m_page1.m_listctrl1.MoveWindow(rcList[Camera1]);
	

	// page2
	CRect rcGroup,rcCheckDate,rcCheckName,rcComboName,rcList2;
	CRect rcStaticStart,rcStaticStop,rcCtrlStart,rcCtrlStop;
	CRect rcBtnInquiry,rcBtnExcel;

	int iWidth_Start_Static=0,iWidth_Start_Ctrl=0;

	int iHeight_Group,iHeight_List2=0;
	iHeight_Group = (rcPage.Height()-20-10)*5/11;
	iHeight_List2 = rcPage.Height()-iHeight_Group-20-10;

	rcGroup.left = 5;
	rcGroup.right = rcPage.right-10;
	rcGroup.top = rcPage.top+25;
	rcGroup.bottom = rcGroup.top+iHeight_Group;
	m_page2.GetDlgItem(IDC_STATIC_INQUIRY)->MoveWindow(rcGroup);

	rcCheckDate.left = rcGroup.left+5;
	rcCheckDate.right = rcCheckDate.left+100;
	rcCheckDate.top = rcGroup.top+20;
	rcCheckDate.bottom = rcCheckDate.top+20;
	m_page2.GetDlgItem(IDC_CHECK_DATE)->MoveWindow(rcCheckDate);

	iWidth_Start_Static = 60;
	CRect rcTemp;
	m_page2.GetDlgItem(IDC_START_DATE)->GetClientRect(&rcTemp);
	iWidth_Start_Ctrl = rcTemp.Width();
	rcStaticStart.left = rcCheckDate.right+10;
	rcStaticStart.right=rcStaticStart.left+iWidth_Start_Static;
	rcStaticStart.top=rcCheckDate.top;
	rcStaticStart.bottom=rcCheckDate.bottom;
	m_page2.GetDlgItem(IDC_STATIC_START)->MoveWindow(rcStaticStart);

	rcCtrlStart.left = rcStaticStart.right+5;
	rcCtrlStart.right=rcCtrlStart.left+iWidth_Start_Ctrl;
	rcCtrlStart.top =rcCheckDate.top;
	rcCtrlStart.bottom=rcCtrlStart.top+rcTemp.Height();
	m_page2.GetDlgItem(IDC_START_DATE)->MoveWindow(rcCtrlStart);

	rcStaticStop.left = rcCtrlStart.right+10;
	rcStaticStop.right=rcStaticStop.left+iWidth_Start_Static;
	rcStaticStop.top=rcCheckDate.top;
	rcStaticStop.bottom=rcCheckDate.bottom;
	m_page2.GetDlgItem(IDC_STATIC_STOP)->MoveWindow(rcStaticStop);
	
	rcCtrlStop.left = rcStaticStop.right+5;
	rcCtrlStop.right=rcCtrlStop.left+iWidth_Start_Ctrl;
	rcCtrlStop.top =rcCheckDate.top;
	rcCtrlStop.bottom=rcCtrlStop.top+rcTemp.Height();
	m_page2.GetDlgItem(IDC_STOP_DATE)->MoveWindow(rcCtrlStop);

	rcList2.left = rcGroup.left;
	rcList2.right = rcGroup.right;
	rcList2.top = rcGroup.bottom+5;
	rcList2.bottom=rcPage.bottom-10;
	m_page2.GetDlgItem(IDC_LIST1)->MoveWindow(rcList2);


	rcCheckName.left = rcCheckDate.left;
	rcCheckName.right = rcCheckDate.right;
	rcCheckName.bottom = rcGroup.bottom-20;
	rcCheckName.top = rcCheckName.bottom-30;
	m_page2.GetDlgItem(IDC_CHECK_MODEL_NAME)->MoveWindow(rcCheckName);

	m_page2.m_cmbModelName.GetWindowRect(rcTemp);
	rcComboName.left = rcCheckName.right+10;
	rcComboName.right = rcComboName.left+300;
	rcComboName.top = rcCheckName.top+5;
	rcComboName.bottom = rcCheckName.top+rcTemp.Height();
	m_page2.GetDlgItem(IDC_COMBO_MODEL_NAME)->MoveWindow(rcComboName);

	rcBtnInquiry.left = rcComboName.right+30;
	rcBtnInquiry.right = rcBtnInquiry.left+100;
	rcBtnInquiry.bottom = rcGroup.bottom-10;
	rcBtnInquiry.top = rcBtnInquiry.bottom-40;
	m_page2.GetDlgItem(IDC_BTN_INQUIRY)->MoveWindow(rcBtnInquiry);

	rcBtnExcel.left = rcBtnInquiry.right+30;
	rcBtnExcel.right = rcBtnExcel.left+100;
	rcBtnExcel.top = rcBtnInquiry.top;
	rcBtnExcel.bottom = rcBtnInquiry.bottom;
	m_page2.GetDlgItem(IDC_BTN_EXCEL)->MoveWindow(rcBtnExcel);

	m_page1.ShowWindow(SW_SHOW);
	m_page2.ShowWindow(SW_HIDE);
	
	m_TabCtrl.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDefect::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
}

void CDlgDefect::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	switch(m_TabCtrl.GetCurSel())
	{
	case 0:
		{
			m_page1.ShowWindow(SW_SHOW);
			m_page2.ShowWindow(SW_HIDE);
			break;
		}
	case 1:
		{
			m_page1.ShowWindow(SW_HIDE);
			m_page2.ShowWindow(SW_SHOW);
			break;
		}
	}

	*pResult = 0;
}

void CDlgDefect::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialog::PostNcDestroy();
}
