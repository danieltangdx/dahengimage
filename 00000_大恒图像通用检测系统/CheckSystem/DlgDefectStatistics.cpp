// DlgDefectStatistics.cpp : 实现文件
//

#include "stdafx.h"
#include "CheckSystem.h"
#include "DlgDefectStatistics.h"
#include "afxdialogex.h"


// CDlgDefectStatistics 对话框

IMPLEMENT_DYNAMIC(CDlgDefectStatistics, CDialogEx)

CDlgDefectStatistics::CDlgDefectStatistics(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_DefectStatistics, pParent)
{

}

CDlgDefectStatistics::~CDlgDefectStatistics()
{
}

void CDlgDefectStatistics::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgDefectStatistics, CDialogEx)
	ON_WM_SIZE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgDefectStatistics 消息处理程序


BOOL CDlgDefectStatistics::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CRect rcClient;
	GetClientRect(&rcClient);

	if (!m_chart.GetSafeHwnd())
	{
// 		m_chart.Create(CRect(0, 0,
// 			rcClient.Width() - 300,
// 			rcClient.Height() - 80), this, 0);
		m_chart.Create(CRect(0, 0,
			0,
			0), this, 0);
	}
	else
	{
		m_chart.Reset();
	}

	//m_chart.SetTitle("The same control using some random data and colors...");
	m_chart.SetBKColor(RGB(240, 240, 240));
	m_chart.ShowGrid(FALSE, FALSE);

	for (int i = 0; i < 15; i++)
	{
		m_chart.AddBar(rand() % 500, "Rand", RGB(rand() % 255, rand() % 255, rand() % 255), FALSE);
	}
	m_chart.Refresh();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDlgDefectStatistics::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}


void CDlgDefectStatistics::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages
	CRect rectChart, rectGrid;
	GetDlgItem(IDC_STATIC_CHART)->GetWindowRect(rectChart);
	GetDlgItem(IDC_STATIC_GRID)->GetWindowRect(rectGrid);
	ScreenToClient(rectChart);
	ScreenToClient(rectGrid);
	m_chart.MoveWindow(rectChart);
}
