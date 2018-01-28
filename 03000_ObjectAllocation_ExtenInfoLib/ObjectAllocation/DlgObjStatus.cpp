// DlgObjStatus.cpp : implementation file
//

#include "stdafx.h"
#include "ObjectAllocation.h"
#include "DlgObjStatus.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgObjStatus dialog


CDlgObjStatus::CDlgObjStatus(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgObjStatus::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgObjStatus)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nNumWorkstation = 0;
	m_pnObjNum = NULL;
	m_bPainted = FALSE;
	m_bShowing = FALSE;
	
	m_StatusImg[0].LoadBitmap(IDB_BITMAP_GREEN);
	m_StatusImg[1].LoadBitmap(IDB_BITMAP_RED);
	m_StatusImg[2].LoadBitmap(IDB_BITMAP_YELLOW);

	m_BackBrush.CreateSolidBrush(RGB(200,200,200));
}


void CDlgObjStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgObjStatus)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgObjStatus, CDialog)
	//{{AFX_MSG_MAP(CDlgObjStatus)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_MESSAGE(CHANGE_STATUS_MSG_2IDLE, OnMsg2Idle)
	ON_MESSAGE(CHANGE_STATUS_MSG_2DO, OnMsg2Do)
	ON_MESSAGE(CHANGE_STATUS_MSG_2DRAW, OnMsg2Draw)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgObjStatus message handlers

void CDlgObjStatus::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
// 	if (!m_bPainted)
// 	{
// 		m_bPainted = TRUE;
// 
// 		int nWidth = GetSystemMetrics(0);
// 		int nHeight = GetSystemMetrics(1);
// 		
// 		CRect rect;
// 		int nWindowHeight = 102+92*(m_nNumWorkstation-1);
// 		int nWindowWidth = 124+84*(m_nMaxObjNum-1);
// 		rect.top = (nHeight - nWindowHeight)/2;
// 		rect.bottom = rect.top + nWindowHeight;
// 		rect.left = (nWidth - nWindowWidth)/2;
// 		rect.right = rect.left + nWindowWidth;
// 		MoveWindow(rect);
// 
// 		for (int nWS=1; nWS<=m_nNumWorkstation; ++nWS)
// 		{
// 			for (int nObj=1; nObj<=m_pnObjNum[nWS-1]; ++nObj)
// 			{
// 				rect.top = 14 + 84*(nWS-1);
// 				rect.bottom = rect.top + 67;
// 				rect.left = 14 + 92*(nObj-1);
// 
// 				int nIdle = nWS*1000 + 100 + nObj;
// 				int nDoing = nWS*1000 + 200 + nObj;
// 				int nDrawing = nWS*1000 + 300 + nObj;
// 
// 				CStatic* pCtrl1 = new CStatic;
// 				pCtrl1->Create("", WS_CHILD|WS_VISIBLE|SS_BITMAP, rect, this, nIdle);
// 				pCtrl1->SetBitmap(HBITMAP(m_StatusImg[0]));
// 				pCtrl1->ShowWindow(SW_SHOW);
// 
// 				CStatic* pCtrl2 = new CStatic;
// 				pCtrl2->Create("", WS_CHILD|WS_VISIBLE|SS_BITMAP, rect, this, nDoing);
// 				pCtrl2->SetBitmap(HBITMAP(m_StatusImg[1]));
// 				pCtrl2->ShowWindow(SW_HIDE);
// 
// 				CStatic* pCtrl3 = new CStatic;
// 				pCtrl3->Create("", WS_CHILD|WS_VISIBLE|SS_BITMAP, rect, this, nDrawing);
// 				pCtrl3->SetBitmap(HBITMAP(m_StatusImg[2]));
// 				pCtrl3->ShowWindow(SW_HIDE);
// 			}
// 		}
// 		SetTimer(1, 10, NULL);
// 	}
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CDlgObjStatus::InitData(int nNumWS, int* pNumObj)
{
	m_nNumWorkstation = nNumWS;

	m_pnObjNum = new int[m_nNumWorkstation+1];
	m_ppChange = new int*[m_nNumWorkstation+1];
	m_nMaxObjNum = 0;
	for (int n=0; n<m_nNumWorkstation; ++n)
	{
		m_pnObjNum[n] = pNumObj[n];
		if (m_pnObjNum[n] > m_nMaxObjNum)
			m_nMaxObjNum = m_pnObjNum[n];
	}
	m_pnObjNum[m_nNumWorkstation] = -1;

	for (int n2=0; n2<m_nNumWorkstation; ++n2)
	{
		m_ppChange[n2] = new int[m_nMaxObjNum+1];
		ZeroMemory(m_ppChange[n2], sizeof(int)*m_nMaxObjNum+1);
	}
	m_ppChange[m_nNumWorkstation] = NULL;
}

BOOL CDlgObjStatus::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam==VK_ESCAPE || pMsg->wParam==VK_RETURN)
			return TRUE;
	}

	return CDialog::PreTranslateMessage(pMsg);
}

HBRUSH CDlgObjStatus::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	return m_BackBrush;

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CDlgObjStatus::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_bPainted)
	{
		if (m_nNumWorkstation > 0)
		{
			for (int nWS=1; nWS<=m_nNumWorkstation; ++nWS)
			{
				for (int nObj=1; nObj<=m_pnObjNum[nWS-1]; ++nObj)
				{
					int nIdle = nWS*1000 + 100 + nObj;
					int nDoing = nWS*1000 + 200 + nObj;
					int nDrawing = nWS*1000 + 300 + nObj;
					
					CStatic* pCtrl = (CStatic*)GetDlgItem(nIdle);
					delete pCtrl;
					pCtrl = (CStatic*)GetDlgItem(nDoing);
					delete pCtrl;
					pCtrl = (CStatic*)GetDlgItem(nDrawing);
					delete pCtrl;
				}
			}
			delete [] m_pnObjNum;

			for (int n=0; n<m_nNumWorkstation; ++n)
				delete [] m_ppChange[n];
			delete [] m_ppChange;
		}
	}
	else
	{
		if (m_pnObjNum)
			delete [] m_pnObjNum;

		if (m_ppChange)
		{
			for (int n=0; n<m_nNumWorkstation; ++n)
				delete [] m_ppChange[n];
			delete [] m_ppChange;
		}
	}
	
	return CDialog::DestroyWindow();
}

LRESULT CDlgObjStatus::OnMsg2Idle(WPARAM wParam, LPARAM lParam)
{
	int nWS = int(wParam)/1000;
	int nObj = int(wParam) - nWS;
	if(CheckDataValid(nWS, nObj))
	{
		int nIdle = nWS*1000 + 100 + nObj;
		int nDoing = nWS*1000 + 200 + nObj;
		int nDrawing = nWS*1000 + 300 + nObj;

		GetDlgItem(nDoing)->ShowWindow(SW_HIDE);
		GetDlgItem(nDrawing)->ShowWindow(SW_SHOW);
		GetDlgItem(nIdle)->ShowWindow(SW_SHOW);
	}
	return 1;
}

LRESULT CDlgObjStatus::OnMsg2Do(WPARAM wParam, LPARAM lParam)
{
	int nWS = int(wParam)/1000;
	int nObj = int(wParam) - nWS;
	if(CheckDataValid(nWS, nObj))
	{
		int nIdle = nWS*1000 + 100 + nObj;
		int nDoing = nWS*1000 + 200 + nObj;
		int nDrawing = nWS*1000 + 300 + nObj;

		GetDlgItem(nIdle)->ShowWindow(SW_HIDE);
		GetDlgItem(nDrawing)->ShowWindow(SW_HIDE);
		GetDlgItem(nDoing)->ShowWindow(SW_SHOW);
	}
	return 1;
}

LRESULT CDlgObjStatus::OnMsg2Draw(WPARAM wParam, LPARAM lParam)
{
	int nWS = int(wParam)/1000;
	int nObj = int(wParam) - nWS;
	if(CheckDataValid(nWS, nObj))
	{
		int nIdle = nWS*1000 + 100 + nObj;
		int nDoing = nWS*1000 + 200 + nObj;
		int nDrawing = nWS*1000 + 300 + nObj;

		GetDlgItem(nIdle)->ShowWindow(SW_HIDE);
		GetDlgItem(nDrawing)->ShowWindow(SW_HIDE);
		GetDlgItem(nDoing)->ShowWindow(SW_SHOW);
	}
	return 1;
}

bool CDlgObjStatus::CheckDataValid(int nWS, int nObj)
{
	if (nWS>0 && nWS<=m_nNumWorkstation)
	{
		if (nObj>0 && nObj<=m_pnObjNum[nWS-1])
			return true;
	}
	return FALSE;
}

void CDlgObjStatus::w2Idle(int nWS, int nObj)
{
	if(CheckDataValid(nWS, nObj))
	{
		int nIdle = nWS*1000 + 100 + nObj;
		int nDoing = nWS*1000 + 200 + nObj;
		int nDrawing = nWS*1000 + 300 + nObj;
		
		GetDlgItem(nDoing)->ShowWindow(SW_HIDE);
		GetDlgItem(nDrawing)->ShowWindow(SW_SHOW);
		GetDlgItem(nIdle)->ShowWindow(SW_SHOW);
	}
}

void CDlgObjStatus::w2Do(int nWS, int nObj)
{
	if(CheckDataValid(nWS, nObj))
	{
		int nIdle = nWS*1000 + 100 + nObj;
		int nDoing = nWS*1000 + 200 + nObj;
		int nDrawing = nWS*1000 + 300 + nObj;
		
		GetDlgItem(nIdle)->ShowWindow(SW_HIDE);
		GetDlgItem(nDrawing)->ShowWindow(SW_HIDE);
		GetDlgItem(nDoing)->ShowWindow(SW_SHOW);
	}
}

void CDlgObjStatus::w2Draw(int nWS, int nObj)
{
	if(CheckDataValid(nWS, nObj))
	{
		int nIdle = nWS*1000 + 100 + nObj;
		int nDoing = nWS*1000 + 200 + nObj;
		int nDrawing = nWS*1000 + 300 + nObj;
		
		GetDlgItem(nIdle)->ShowWindow(SW_HIDE);
		GetDlgItem(nDrawing)->ShowWindow(SW_HIDE);
		GetDlgItem(nDoing)->ShowWindow(SW_SHOW);
	}
}

void CDlgObjStatus::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if (m_bShowing && nIDEvent==1)
	{
		for (int nWS=0; nWS<=m_nNumWorkstation-1; ++nWS)
		{
			for (int nObj=0; nObj<=m_pnObjNum[nWS]-1; ++nObj)
			{
				if(m_ppChange[nWS][nObj])
				{
					int nIdle = (nWS+1)*1000 + 100 + (nObj+1);
					int nDoing = (nWS+1)*1000 + 200 + (nObj+1);
					int nDrawing = (nWS+1)*1000 + 300 + (nObj+1);
					switch (m_ppChange[nWS][nObj])
					{
					case 1:
						GetDlgItem(nDoing)->ShowWindow(SW_HIDE);
						GetDlgItem(nDrawing)->ShowWindow(SW_HIDE);
						GetDlgItem(nIdle)->ShowWindow(SW_SHOW);
						break;
					case 2:
						GetDlgItem(nIdle)->ShowWindow(SW_HIDE);
						GetDlgItem(nDrawing)->ShowWindow(SW_HIDE);
						GetDlgItem(nDoing)->ShowWindow(SW_SHOW);
						break;
					case 3:
						GetDlgItem(nIdle)->ShowWindow(SW_HIDE);
						GetDlgItem(nDoing)->ShowWindow(SW_HIDE);
						GetDlgItem(nDrawing)->ShowWindow(SW_SHOW);
						break;
					}
					m_ppChange[nWS][nObj] = 0;
				}
			}
		}
	}
	else if(nIDEvent==100)
	{

	}
	
	CDialog::OnTimer(nIDEvent);
}

void CDlgObjStatus::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
// 	delete this;
	CDialog::PostNcDestroy();
}


void CDlgObjStatus::ShowInfo(char* pObj, char* pList, char* pImage, char* pCam)
{
	CString strObj,strList,strImage,strCam;

	strObj.Format("%s",pObj);
	strList.Format("%s",pList);
	strImage.Format("%s",pImage);
	strCam.Format("%s",pCam);

// 	SetDlgItemText(IDC_STATIC_OBJ,pObj);
// 	SetDlgItemText(IDC_STATIC_LIST,pList);
// 	SetDlgItemText(IDC_STATIC_IMAGE,pImage);
// 	SetDlgItemText(IDC_STATIC_CAM,pCam);

	SetDlgItemText(IDC_STATIC_OBJ,strObj);
	SetDlgItemText(IDC_STATIC_LIST,strList);
	SetDlgItemText(IDC_STATIC_IMAGE,strImage);
	SetDlgItemText(IDC_STATIC_CAM,strCam);


}
