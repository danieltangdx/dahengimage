// DlgLangSel.cpp : implementation file
//

#include "stdafx.h"
#include "DemoProject.h"
#include "DlgLangSel.h"
#include "DemoProjectDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLangSel dialog
extern HMODULE g_hLanguage;
extern pFunc_GetTextFromLPK g_pGetTextFromLPK;
extern CString g_strAppDir;

CDlgLangSel::CDlgLangSel(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLangSel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLangSel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nCount = 0;
	m_bPainted = FALSE;
}


void CDlgLangSel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLangSel)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLangSel, CDialog)
	//{{AFX_MSG_MAP(CDlgLangSel)
	ON_WM_PAINT()
	ON_COMMAND_RANGE(1001,1999,OnButton)
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLangSel message handlers

BOOL CDlgLangSel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	char chText[256];
	LoadString(g_hLanguage, 5052, chText, 256);
	SetWindowText(chText);
	GetClientRect(&m_rectORI); 
	
	CRect rect,rect1;
	GetWindowRect(rect);

//	m_pDlgMainWindow->GetWindowRect(rect);//如果需要获取父窗口整个的dlg  rect 用这个。
	
	CWnd *p = (CWnd*)(m_pDlgMainWindowLan->GetDlgItem(IDC_BTN_SAVESINGLEMODEL)); //如果需要获取父窗口某个空间的rect 用这种方式
	p->GetWindowRect(rect1);

//	下面这段注释掉的和dialog 的 center  勾选上是一样的
// 	int x, y;
// 	x = GetSystemMetrics(SM_CXSCREEN); //屏幕宽度
// 	y = GetSystemMetrics(SM_CYSCREEN); //屏幕高度
// 	MoveWindow(x/2-rect.Width()/2,y/2-rect.Height()/2,rect.Width(),rect.Height());

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgLangSel::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	if (m_bPainted == FALSE)
	{
		m_bPainted = TRUE;
	
		CRect rect1, rect2, rectW;
		GetDlgItem(IDC_STATIC_1)->GetWindowRect(rect1);//得到静态框1的rect
		GetWindowRect(rectW);//得到整个dialog 的rect
		int nRowDiff = 1.38235294117647 * rect1.Height() + 0.5;
		int nColDiff = 1.2947368421 * rect1.Width() + 0.5;
		int nWndBottom = rectW.bottom - rect1.bottom;
		ScreenToClient(rect1);//dialog 左上角相当于（0,0）啦
		{
			m_nCount++;
			
			int nRow = (m_nCount-1)/4;
			int nCol = (m_nCount-1)%4;
			
			CRect rect;
			rect.top = rect1.top + nRow*nRowDiff;
			rect.bottom = rect.top + rect1.Height();
			rect.left = rect1.left + nCol*nColDiff;
			rect.right = rect.left + rect1.Width();
			
			CString strTitle = _T("DEFAULT\r\n中文(默认)");
			m_strArrTexts.Add(strTitle);
			CButton* pButton = new CButton;
			pButton->Create(_T(strTitle.GetBuffer(0)), WS_CHILD|BS_MULTILINE, rect, this, 1000+m_nCount);
			pButton->ShowWindow(SW_SHOW);
		}

		CFileFind finder;
		BOOL bRet = finder.FindFile(g_strAppDir+"\\*.dlk");
		while(bRet)
		{
			bRet = finder.FindNextFile();

			CString strPath = finder.GetFilePath();
			HMODULE hDll = LoadLibrary(strPath.GetBuffer(0));
			if (hDll)
			{
				pFunc_GetTextFromLPK pFunc_GT;
				pFunc_GT = (pFunc_GetTextFromLPK)(GetProcAddress(hDll, "GetText"));
				if (pFunc_GT)
				{
					char chInfo[256];
					int nRet = LoadString(hDll, 5000, chInfo, 256);
					
					if (nRet > 0)
					{
						m_nCount++;
						
						int nRow = (m_nCount-1)/4;
						int nCol = (m_nCount-1)%4;

						CRect rect;
						rect.top = rect1.top + nRow*nRowDiff;
						rect.bottom = rect.top + rect1.Height();
						rect.left = rect1.left + nCol*nColDiff;
						rect.right = rect.left + rect1.Width();

						CString strTitle;
						strTitle.Format("%s\r\n%s", finder.GetFileName().Left(finder.GetFileName().GetLength()-4), chInfo);
						m_strArrTexts.Add(strTitle);
						CButton* pButton = new CButton;
						pButton->Create(_T(strTitle.GetBuffer(0)), WS_CHILD|BS_MULTILINE, rect, this, 1000+m_nCount);
						pButton->ShowWindow(SW_SHOW);
					}
				}
			}
		}

		{
			CRect rect;
			int nRow = (m_nCount)/4;
			int nCol = (m_nCount)%4;
			rect.top = rect1.top + nRow*nRowDiff;
			rect.bottom = rect.top + rect1.Height();
			rect.left = rect1.left + nCol*nColDiff;
			rect.right = rect.left + rect1.Width();
			GetDlgItem(IDCANCEL)->MoveWindow(rect);

			char chText[256];
			LoadString(g_hLanguage, 5045, chText, 256);
			GetDlgItem(IDCANCEL)->SetWindowText(chText);
		}

		{
			CRect rect3;
			GetDlgItem(IDCANCEL)->GetWindowRect(rect3);
			rectW.bottom = rect3.bottom + nWndBottom;
			MoveWindow(rectW);
		}
	}
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgLangSel::OnButton(UINT uID)
{
	EndDialog(uID);
}

void CDlgLangSel::GetDllName(char* chDllName, UINT uID)
{
	int nIndex = uID-1001;
	if (nIndex<0 || nIndex>m_strArrTexts.GetSize()-1)
		strcpy(chDllName, "DEFAULT");
	else
	{
		CString strDllName = m_strArrTexts.GetAt(uID-1001);
		
		int nFind = strDllName.Find("\r\n", 0);
		if (nFind >= 0)
			strDllName = strDllName.Left(nFind);
		
		strcpy(chDllName, strDllName.GetBuffer(0));
	}
}

//  可变大小 随之而变

void CDlgLangSel::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);

// 	CWnd *pWnd; pWnd = GetDlgItem(IDCANCEL); //获取控件句柄
// 	if(pWnd && nType!=1)//判断是否为空，因为对话框创建时会调用此函数，而当时控件还未创建
// 	{
// 		CRect rect; //获取控件变化前大小
// 		pWnd->GetWindowRect(&rect); ScreenToClient(&rect);//将控件大小转换为在对话框中的区域坐标
// 		// cx/m_rect.Width()为对话框在横向的变化比例
// 		rect.left=rect.left*cx/m_rectORI.Width();/////调整控件大小
// 		rect.right=rect.right*cx/m_rectORI.Width();
// 		rect.top=rect.top*cy/m_rectORI.Height();
// 		rect.bottom=rect.bottom*cy/m_rectORI.Height();
// 		pWnd->MoveWindow(rect);//设置控件大小
// 	} GetClientRect(&m_rectORI);//将变化后的对话框大小设为旧大小
// 	// TODO: Add your message handler code here
	
}
