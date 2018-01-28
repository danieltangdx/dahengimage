// DlgBtnCtrlGroup.cpp : 实现文件
//

#include "stdafx.h"
#include "CheckSystem.h"
#include "DlgBtnCtrlGroup.h"
#include "afxdialogex.h"


// CDlgBtnCtrlGroup 对话框

IMPLEMENT_DYNAMIC(CDlgBtnCtrlGroup, CDialogEx)

CDlgBtnCtrlGroup::CDlgBtnCtrlGroup(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_BtnCtrlGroup, pParent)
{

}

CDlgBtnCtrlGroup::~CDlgBtnCtrlGroup()
{
}

void CDlgBtnCtrlGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgBtnCtrlGroup, CDialogEx)
END_MESSAGE_MAP()


// CDlgBtnCtrlGroup 消息处理程序
