// MyListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "DHIOCardForRS232_XZXNYN.h"
#include "MyListCtrl.h"


// CMyListCtrl

IMPLEMENT_DYNAMIC(CMyListCtrl, CListCtrl)

CMyListCtrl::CMyListCtrl()
{

}

CMyListCtrl::~CMyListCtrl()
{
}


BEGIN_MESSAGE_MAP(CMyListCtrl, CListCtrl)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()



// CMyListCtrl 消息处理程序




void CMyListCtrl::OnDropFiles(HDROP hDropInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	char szFilePathName[_MAX_PATH + 1] = { 0 };

	//得到文件个数      
	UINT nNumOfFiles = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);

	for (UINT nIndex = 0; nIndex < nNumOfFiles; ++nIndex)
	{
		//　得到文件名   
		DragQueryFile(hDropInfo, nIndex, (LPTSTR)szFilePathName, _MAX_PATH);

		//获取ListCtrl已经存在的项数
		int nCnt = this->GetItemCount();

		//插入文本
		LVITEM lvi = { 0 };
		lvi.mask = LVIF_TEXT;
		lvi.iItem = nCnt;
		lvi.iSubItem = 0;
		lvi.pszText = szFilePathName;
		this->InsertItem(&lvi);
	}

	//完成拖放操作
	DragFinish(hDropInfo);
	CListCtrl::OnDropFiles(hDropInfo);
}
