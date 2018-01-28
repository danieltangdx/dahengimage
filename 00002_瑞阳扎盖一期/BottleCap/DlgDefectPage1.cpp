// DlgDefectPage1.cpp : implementation file
//

#include "stdafx.h"
#include "BottleCap.h"
#include "DlgDefectPage1.h"
#include "MainFrm.h"
#include "BottleCapVIEW.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainFrame*			g_pFrame; //全局指针
extern CBottleCapView*     g_pView;


/////////////////////////////////////////////////////////////////////////////
// CDlgDefectPage1 dialog


CDlgDefectPage1::CDlgDefectPage1(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgDefectPage1::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgDefectPage1)
	//}}AFX_DATA_INIT
}


void CDlgDefectPage1::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgDefectPage1)
	DDX_Control(pDX, IDC_LIST1, m_listctrl1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgDefectPage1, CDialog)
	//{{AFX_MSG_MAP(CDlgDefectPage1)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgDefectPage1 message handlers

BOOL CDlgDefectPage1::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	CMainFrame* g_pFrame = (CMainFrame*)AfxGetMainWnd();
	
	CRect rcList;
	GetDlgItem(IDC_LIST1)->GetWindowRect(&rcList);

	int i=0,j=0,k=0;
	int iWidth1=0,iWidth2=0;
	CString str1;

	iWidth1 = rcList.Width()*3/4;
	iWidth2 = rcList.Width()-iWidth1;

	m_listctrl1.InsertColumn(0,"缺陷类型",LVCFMT_LEFT,iWidth1);
	m_listctrl1.InsertColumn(1,"缺陷个数",LVCFMT_LEFT,iWidth2);

	m_listctrl1.DeleteAllItems();
	m_listctrl1.ModifyStyle(NULL,LVS_REPORT);
	m_listctrl1.SetExtendedStyle(LVS_EX_HEADERDRAGDROP|LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	for(i=0;i<ALLELEMENT_COUNT;i++)
	{
		k=0;
		for(j=0;j<g_pFrame->m_iDefectTypePerWS[i];j++)
		{
			if(""!=g_pFrame->m_sDefectInfo[i][j])
			{
				if(0==i)
				{
					m_listctrl1.InsertItem(k,g_pFrame->m_sDefectInfo[i][j]);
					str1.Format("%d",g_pFrame->m_iDefectCount[i][j]);
					m_listctrl1.SetItemText(k,1,str1);
				}
				k++;
			}
		}

	}

	SetTimer(ID_UPDATE_DEFECT_SHOW_INFO,1000,NULL);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgDefectPage1::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent==ID_UPDATE_DEFECT_SHOW_INFO)
	{
		int i=0,j=0,k=0;
		CString str1;
		for(i=0;i<ALLELEMENT_COUNT;i++)
		{
			k=0;
			for(j=0;j<g_pFrame->m_iDefectTypePerWS[i];j++)
			{
				if(""!=g_pFrame->m_sDefectInfo[i][j])
				{
					if(0==i)
					{
						str1.Format("%d",g_pFrame->m_iDefectCount[i][j]);
						m_listctrl1.SetItemText(k,1,str1);
					}
					k++;
				}
			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}


bool CDlgDefectPage1::UpdateInfo()
{
	return false;
}
