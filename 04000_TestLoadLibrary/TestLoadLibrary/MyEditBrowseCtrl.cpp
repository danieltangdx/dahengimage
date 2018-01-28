// MyEditBrowseCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "TestLoadLibrary.h"
#include "MyEditBrowseCtrl.h"


// CMyEditBrowseCtrl

IMPLEMENT_DYNAMIC(CMyEditBrowseCtrl, CMFCEditBrowseCtrl)

CMyEditBrowseCtrl::CMyEditBrowseCtrl()
{

}

CMyEditBrowseCtrl::~CMyEditBrowseCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyEditBrowseCtrl, CMFCEditBrowseCtrl)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()



// CMyEditBrowseCtrl 消息处理程序




void CMyEditBrowseCtrl::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TCHAR szFilePathName[_MAX_PATH + 1] = { 0 };

	//得到文件个数      
	UINT nNumOfFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	for (UINT nIndex = 0; nIndex < nNumOfFiles; ++nIndex)
	{
		//　得到文件名   
		DragQueryFile(hDropInfo, nIndex, (LPTSTR)szFilePathName, _MAX_PATH);
		this->SetWindowText(szFilePathName);		
	}

	//完成拖放操作
	DragFinish(hDropInfo);
	CMFCEditBrowseCtrl::OnDropFiles(hDropInfo);
}
