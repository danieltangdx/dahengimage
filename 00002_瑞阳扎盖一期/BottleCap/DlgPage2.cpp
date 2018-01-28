// DlgPage2.cpp : implementation file
//

#include "stdafx.h"
#include "MainFrm.h"
#include "BottleCap.h"
#include "BottleCapVIEW.h"
#include "DlgPage2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainFrame*			   g_pFrame; //全局指针
extern CBottleCapView*     g_pView;

/////////////////////////////////////////////////////////////////////////////
// CDlgPage2 dialog


CDlgPage2::CDlgPage2(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPage2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPage2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPage2)
	DDX_Control(pDX, IDC_LIST1, m_lscInfo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPage2, CDialog)
	//{{AFX_MSG_MAP(CDlgPage2)
	ON_NOTIFY(NM_CLICK, IDC_LIST1, OnClickList1)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST1, OnKeydownList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPage2 message handlers

void CDlgPage2::OnClickList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int nIndex = m_lscInfo.GetNextItem(-1,LVNI_SELECTED);
	
	if(nIndex<0)
	{
		*pResult = 0;
		return;
	}
	
	// 获取图像大张号
	CString strCircle = m_lscInfo.GetItemText(nIndex,1);
	int nCircle=0;
	nCircle = atoi(strCircle.GetBuffer(0));
	
	// 错误描述
	CString strErrorDes = m_lscInfo.GetItemText(nIndex,2);
	
	// 获取时间
	CString strSaveTime = m_lscInfo.GetItemText(nIndex,3);
	
	// 获取选择记录中图像路径
	CTime curTime = CTime::GetCurrentTime();
	CString strPath1(""),strTime("");
	CString strImageName1("");
	
	strTime.Format("%04d年%02d月%02d日",curTime.GetYear(),curTime.GetMonth(),curTime.GetDay());
	strPath1.Format("%s:\\SaveImages\\ErrorImages\\%s\\Camera%d",g_pFrame->m_sVirtualDisk,strTime,Camera2+1);
	strImageName1.Format("Error_%d_%s.bmp",nCircle,strSaveTime);
	strPath1=strPath1+"\\"+strImageName1;
	
	// 无图则返回
	BOOL bRet = FALSE;
	bRet = g_pFrame->inIsPathExist(strPath1);
	if(FALSE==bRet)
	{
		return;
	}
	
	
	// 如果相机号为0，则调用CDlgAlgWnd1对话框查看
	if (strCircle != "")
	{
		g_pView->m_pShowImage->m_iCameraSN = Camera2;
		g_pView->m_pShowImage->m_sSaveImagePath[Camera2]=strPath1;
		g_pView->m_pShowImage->m_sErrorDes[Camera2]=strErrorDes;
		
		g_pView->m_pShowImage->ShowWindow(SW_SHOW);
		
		// 显示采集原图像
		g_pView->m_pShowImage->ShowImage();
		
	}

	*pResult = 0;
}

void CDlgPage2::OnKeydownList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	int nCircle=0;
	CTime curTime;
	BOOL bRet = FALSE;
	
	CString strCircle;
	CString strErrorDes;
	CString strSaveTime;
	CString strPath1,strTime;
	CString strImageName1;
	
	POSITION pos = m_lscInfo.GetFirstSelectedItemPosition();
	if(pos!=NULL)
	{
		int nIndex = m_lscInfo.GetNextItem(-1,LVNI_SELECTED);
		
		if(pLVKeyDow->wVKey==VK_UP)
		{
			if(nIndex<=0)
			{
				*pResult = 0;
				return;
			}
			
			nIndex=nIndex-1;
		}
		else if(pLVKeyDow->wVKey==VK_DOWN)
		{			
			if(nIndex<0)
			{
				*pResult = 0;
				return;
			}
			
			if(nIndex==((g_pFrame->m_iMaxSaveImageCount)-1))
			{
				*pResult = 0;
				return;
			}
			
			nIndex=nIndex+1;
		}
		
		if(pLVKeyDow->wVKey==VK_UP || pLVKeyDow->wVKey==VK_DOWN)
		{
			// 获取图像大张号
			strCircle = m_lscInfo.GetItemText(nIndex,1);
			nCircle = atoi(strCircle.GetBuffer(0));
			
			// 获取错误描述
			strErrorDes = m_lscInfo.GetItemText(nIndex,2);
			
			// 获取时间
			strSaveTime= m_lscInfo.GetItemText(nIndex,3);
			
			// 获取选择记录中图像路径
			curTime = CTime::GetCurrentTime();
			
			strTime.Format("%04d年%02d月%02d日",curTime.GetYear(),curTime.GetMonth(),curTime.GetDay());
			strPath1.Format("%s:\\SaveImages\\ErrorImages\\%s\\Camera%d",g_pFrame->m_sVirtualDisk,strTime,Camera2+1);
			strImageName1.Format("Error_%d_%s.bmp",nCircle,strSaveTime);
			strPath1=strPath1+"\\"+strImageName1;
			
			// 无图则返回
			bRet = g_pFrame->inIsPathExist(strPath1);
			if(FALSE==bRet)
			{
				return;
			}
			
			
			// 如果相机号为0，则调用CDlgAlgWnd1对话框查看
			if (strCircle != "")
			{
				g_pView->m_pShowImage->m_iCameraSN = Camera2;
				g_pView->m_pShowImage->m_sSaveImagePath[Camera2]=strPath1;
				g_pView->m_pShowImage->m_sErrorDes[Camera2]=strErrorDes;
				
				g_pView->m_pShowImage->ShowWindow(SW_SHOW);
				
				// 显示图像
				g_pView->m_pShowImage->ShowImage();
			}
		}
	}

	*pResult = 0;
}

BOOL CDlgPage2::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
