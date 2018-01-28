// DlgModelLog.cpp : implementation file
//

#include "stdafx.h"
#include "DemoProject.h"
#include "DlgModelLog.h"
#include "afxdialogex.h"


// CDlgModelLog dialog

IMPLEMENT_DYNAMIC(CDlgModelLog, CDialogEx)

CDlgModelLog::CDlgModelLog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ModelLog, pParent)
{

}

CDlgModelLog::~CDlgModelLog()
{
}

void CDlgModelLog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ModelLog, m_listModelLog);
}


BEGIN_MESSAGE_MAP(CDlgModelLog, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CDlgModelLog message handlers


void CDlgModelLog::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	GetParent()->GetDlgItem(IDC_MODELLIST)->GetFocus();
	CDialogEx::OnClose();
}


BOOL CDlgModelLog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	switch (pMsg->message)
	{
	case WM_PAINT:
	{
		CRect rect{ 0 };
		GetClientRect(&rect);
		m_listModelLog.MoveWindow(CRect(rect.left, rect.top, rect.right, rect.bottom), TRUE);
		//Invalidate();
	}
	break;
	case WM_SIZE:
	{
		
	}
	break;
	default:
		break;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgModelLog::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
// 	switch (message)
// 	{
// 	case WM_PAINT:
// 	{
// 		
// 	}
// 	break;
// 	case WM_SIZE:
// 	{
// 		CRect rect{ 0 };
// 		GetClientRect(&rect);
// 		m_listModelLog.MoveWindow(CRect(rect.left, rect.top, rect.right, rect.bottom), TRUE);
// 		//Invalidate();
// 	}
// 		break;
// 	default:
//		break;
// 	}
	return CDialogEx::OnWndMsg(message, wParam, lParam, pResult);
}


void CDlgModelLog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages
	CRect rect{ 0 };
	GetClientRect(&rect);
	m_listModelLog.MoveWindow(CRect(rect.left, rect.top, rect.right, rect.bottom), TRUE);
}
