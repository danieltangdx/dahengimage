// DlgSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "CheckSystem.h"
#include "DlgSetting.h"
#include "afxdialogex.h"


// CDlgSetting 对话框

IMPLEMENT_DYNAMIC(CDlgSetting, CDialogEx)

CDlgSetting::CDlgSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_Setting, pParent)
{

}

CDlgSetting::~CDlgSetting()
{
}

void CDlgSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgSetting, CDialogEx)
END_MESSAGE_MAP()


// CDlgSetting 消息处理程序
