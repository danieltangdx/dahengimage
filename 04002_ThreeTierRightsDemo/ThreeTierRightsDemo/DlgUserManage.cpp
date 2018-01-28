// DlgUserManage.cpp : 实现文件
//

#include "stdafx.h"
#include "ThreeTierRightsDemo.h"
#include "DlgUserManage.h"
#include "afxdialogex.h"


// CDlgUserManage 对话框

IMPLEMENT_DYNAMIC(CDlgUserManage, CDialogEx)

CDlgUserManage::CDlgUserManage(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_UserManage, pParent)
{

}

CDlgUserManage::~CDlgUserManage()
{
}

void CDlgUserManage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgUserManage, CDialogEx)
END_MESSAGE_MAP()


// CDlgUserManage 消息处理程序
