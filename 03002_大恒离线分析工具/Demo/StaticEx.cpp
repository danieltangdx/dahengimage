// StaticEx.cpp: implementation of the CStaticEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DemoProject.h"
#include "StaticEx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStaticEx::CStaticEx()
{
	m_TextColor = RGB(0,0,0);
}

CStaticEx::~CStaticEx()
{

}

BEGIN_MESSAGE_MAP(CStaticEx, CStatic)  
ON_WM_CTLCOLOR_REFLECT()  
END_MESSAGE_MAP()  

// CStaticEx message handlers  

HBRUSH CStaticEx::CtlColor(CDC* pDC, UINT /*nCtlColor*/)  
{  
	// TODO:  Change any attributes of the DC here  
	pDC->SetTextColor(m_TextColor); //设置文字的颜色     
    pDC->SetBkMode(TRANSPARENT);	//透明     
    return (HBRUSH)::GetStockObject(NULL_BRUSH);  
	
	// TODO:  Return a non-NULL brush if the parent's handler should not be called  
	//return NULL;  
} 

LRESULT CStaticEx::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam)  
{  
	// TODO: Add your specialized code here and/or call the base class  
	if(message==WM_SETTEXT) {  
		CRect rect;  
		GetWindowRect(&rect);  
		CWnd* pParent = GetParent();  
		if(pParent) {  
			pParent->ScreenToClient(&rect);  
			pParent->InvalidateRect(&rect);  
		}  
	}  

	return CStatic::DefWindowProc(message, wParam, lParam);  
}  