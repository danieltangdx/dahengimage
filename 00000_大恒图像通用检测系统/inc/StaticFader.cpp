// StaticFader.cpp : implementation file
//

#include "stdafx.h"
#include "StaticFader.h"
	//	#include "include/MemDC.h"
	//	#include "include/AutoFont.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define TIMERINTERVAL 10

/////////////////////////////////////////////////////////////////////////////
// CStaticFader

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
CStaticFader::CStaticFader()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	// Set defaults:

	m_uAlignFlag = DT_CENTER;
	m_bDrawBorder = true;
	m_bDrawShadow=false;
	m_crBackground = RGB(210,210,210);
	m_crSubText = RGB(0,0,60);
	m_crMainText = RGB(0,0,60);

	m_strSubText = m_strMainText = "";

	m_pSubFont = new CAutoFont("Arial");
	m_pMainFont = new CAutoFont("Arial");
	m_bDep=false;
	m_bChangFont=false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
CStaticFader::~CStaticFader()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	if (m_pSubFont != NULL)
	{
		delete m_pSubFont;
	}
	if (m_pMainFont != NULL)
	{
		delete m_pMainFont;
	}
	
}


BEGIN_MESSAGE_MAP(CStaticFader, CStatic)
	//{{AFX_MSG_MAP(CStaticFader)
	ON_WM_TIMER()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticFader message handlers

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
void CStaticFader::OnTimer(UINT nIDEvent) 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	if(m_bDep) 
	{
		m_bDep=false;
		OnPaint();
	}
	if ( (GetTickCount()-m_dwStopwatch) > 1000)	KillTimer(500);

	Invalidate(FALSE);
	
	CStatic::OnTimer(nIDEvent);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
void CStaticFader::Display(CString strText, CString strSubText, int nFadePercent, bool bResetColours, CString strSubFont, CString strMainFont)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{

	if (!strMainFont.IsEmpty())		
		m_pMainFont->ExtractFont(strMainFont);
	if (!strSubFont.IsEmpty())		
		m_pSubFont->ExtractFont(strSubFont);

	if (bResetColours)
	{
		m_crBackground = RGB(210,210,210);
		m_crSubText = RGB(0,0,60);
		m_crMainText = RGB(0,0,60);
	}

	m_strMainText.Format("%s",strText);
	m_strSubText.Format("%s",strSubText);
	m_nFadePercent = nFadePercent;
	m_dwStopwatch = GetTickCount();

	Invalidate(true);
}

//显示高亮度的文字，定时消隐
void CStaticFader::DisplayDepText(CString strText, CString strSubText, int interger)
{
	m_strMainText = strText;
	m_strSubText = strSubText;
	m_nFadePercent = 0;
	m_dwStopwatch = GetTickCount();
	m_bDep=TRUE;
	Invalidate(true);
	SetTimer(500,interger,NULL);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
void CStaticFader::OnPaint() 
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	CPaintDC dc(this); // device context for painting
	CMemDCX memDC(&dc, m_rectClient);
	CMemDCX* pDC = &memDC;
	GetClientRect(&m_rectClient);

	pDC->SetBkMode(TRANSPARENT);

	if ( m_strSubText.IsEmpty() )
	{
		pDC->FillSolidRect( m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom, m_crBackground );
		
		if (m_bDrawBorder)	
			pDC->Draw3dRect( m_rectClient.left, m_rectClient.top, m_rectClient.right, m_rectClient.bottom, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT) );
	}
	else
	{
		//2007.8.29 修改参数，使得可以完全显示  原始值为： 59  60  现在改为79  80
		pDC->FillSolidRect( m_rectClient.left, m_rectClient.top, m_rectClient.left+79, m_rectClient.bottom, m_crBackground );

		if (m_bDrawBorder)	
			pDC->Draw3dRect( m_rectClient.left, m_rectClient.top, m_rectClient.left+79, m_rectClient.bottom, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT) );

		pDC->FillSolidRect( m_rectClient.left+80, m_rectClient.top, m_rectClient.right-80, m_rectClient.bottom, m_crBackground);
		
		if (m_bDrawBorder)	
			pDC->Draw3dRect( m_rectClient.left+80, m_rectClient.top, m_rectClient.right-80, m_rectClient.bottom, ::GetSysColor(COLOR_BTNSHADOW), ::GetSysColor(COLOR_BTNHILIGHT) );
	}

    DrawText(pDC);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
void CStaticFader::DrawText(CMemDCX *pDC)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	CFont *pOldFont = NULL;
	
	if ( ! m_strSubText.IsEmpty())
	{
		if(m_bDep)
		{
			m_pSubFont->SetBold(true);
			m_pSubFont->SetItalic(true);
				m_pSubFont->SetUnderline(true);
			m_pSubFont->SetHeight(m_pSubFont->GetHeight()+2);
			m_bChangFont=true;
		}
		else
		{
			m_pSubFont->SetBold(false);
			m_pSubFont->SetItalic(false);
			m_pSubFont->SetUnderline(false);
			if(m_bChangFont)
				m_pSubFont->SetHeight(m_pSubFont->GetHeight()-2);
		}
	    pOldFont=pDC->SelectObject(m_pSubFont);
		CRect rect = m_rectClient;
		rect.right=80;  //控制文字开始显示位置， 以前是60  现在改为80   2007.8.29

		pDC->SetTextColor( GetCurrentColour( m_crSubText ) );
		pDC->DrawText((LPCTSTR)m_strSubText,m_strSubText.GetLength(),
			&rect, DT_CENTER| DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER );
		pDC->SelectObject(pOldFont);
	}

	if ( ! m_strMainText.IsEmpty())
	{
		if(m_bDep)
		{
			m_pMainFont->SetBold(true);
			m_pMainFont->SetItalic(true);
			m_pMainFont->SetUnderline(true);
			m_pMainFont->SetHeight(m_pMainFont->GetHeight()+2);
		
		}
		else
		{
			m_pMainFont->SetBold(false);
			m_pMainFont->SetItalic(false);
			m_pMainFont->SetUnderline(false);
			if(m_bChangFont)
			{
				m_pMainFont->SetHeight(m_pMainFont->GetHeight()-2);
				m_bChangFont=false;
			}
				
		}
	    pOldFont=pDC->SelectObject(m_pMainFont);
		CRect rect = m_rectClient;
		rect.right-=1;
		
		if ( ! m_strSubText.IsEmpty())	rect.left+=60;

		// If required draw text with darker colour to appear as a shadow

		if (m_bDrawShadow)
		{
			CRect shadowrect = rect;
			shadowrect.top+=2;
			shadowrect.left+=2;

			int r = GetRValue(m_crBackground);	
			int g = GetGValue(m_crBackground);	
			int b = GetBValue(m_crBackground);

			if ( (r-=50)<0 ) r = 0;	
			if ( (g-=50)<0 ) g = 0;		
			if ( (b-=50)<0 ) b = 0;
			
			pDC->SetTextColor( GetCurrentColour( RGB(r,g,b) ) );
			pDC->DrawText((LPCTSTR)m_strMainText, m_strMainText.GetLength(),&shadowrect, m_uAlignFlag | DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER );
		}

		pDC->SetTextColor( GetCurrentColour( m_crMainText ) );
		pDC->DrawText((LPCTSTR)m_strMainText, m_strMainText.GetLength(),&rect, m_uAlignFlag | DT_NOPREFIX | DT_SINGLELINE | DT_VCENTER );
		pDC->SelectObject(pOldFont);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
COLORREF CStaticFader::GetCurrentColour(COLORREF crText)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	return crText;//2001 lfl 直接显示目标字体;
	UINT uCurrentPercent = (UINT) (GetTickCount() - m_dwStopwatch)/10;
	if(m_bChangFont) return RGB(0,0,50);
	if ( uCurrentPercent >= (UINT)m_nFadePercent) return crText;

	short Back_r = GetRValue(m_crBackground);
	short Back_g = GetGValue(m_crBackground);
	short Back_b = GetBValue(m_crBackground);

	short Target_r = GetRValue(crText);
	short Target_g = GetGValue(crText);
	short Target_b = GetBValue(crText);

	short Diff_r = Back_r - Target_r;
	short Diff_g = Back_g - Target_g;
	short Diff_b = Back_b - Target_b;

	short Current_r = Back_r - (short) (((float)Diff_r/(float)m_nFadePercent)*uCurrentPercent);
	short Current_g = Back_g - (short) (((float)Diff_g/(float)m_nFadePercent)*uCurrentPercent);
	short Current_b = Back_b - (short) (((float)Diff_b/(float)m_nFadePercent)*uCurrentPercent);

	return RGB( Current_r, Current_g, Current_b );
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Function Header
void CStaticFader::Initialise(COLORREF crBG, COLORREF crMainText, COLORREF crSubText)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
{
	m_crBackground = crBG;
	m_crMainText = crMainText;
	m_crSubText = crSubText;
}



void CStaticFader::Displayint(int iMainText, int iSubText, int nFadePercent, bool bResetColours, CString strSubFont, CString strMainFont)
{
	if (!strMainFont.IsEmpty())		m_pMainFont->ExtractFont(strMainFont);
	if (!strSubFont.IsEmpty())		m_pSubFont->ExtractFont(strSubFont);
	
	if (bResetColours)
	{
		m_crBackground = RGB(210,210,210);
		m_crSubText = RGB(0,0,60);
		m_crMainText = RGB(0,0,60);
	}
	
	m_strMainText.Format("%d",iMainText);//Text;
	m_strSubText.Format("%d",iSubText);// strSubText;
	m_nFadePercent = nFadePercent;
	m_dwStopwatch = GetTickCount();

		Invalidate(FALSE);
}
void CStaticFader::Display(int iMainText, CString strSubText, int nFadePercent, bool bResetColours, CString strSubFont, CString strMainFont)
{
	if (!strMainFont.IsEmpty())		m_pMainFont->ExtractFont(strMainFont);
	if (!strSubFont.IsEmpty())		m_pSubFont->ExtractFont(strSubFont);
	
	if (bResetColours)
	{
		m_crBackground = RGB(210,210,210);
		m_crSubText = RGB(0,0,60);
		m_crMainText = RGB(0,0,60);
	}
	
	m_strMainText.Format("%d",iMainText);//Text;
	m_strSubText=strSubText;//.Format("%d",iSubText);// strSubText;
	m_nFadePercent = nFadePercent;
	m_dwStopwatch = GetTickCount();
	Invalidate(FALSE);

}
void CStaticFader::Display(CString strMainText, int iSubText, int nFadePercent, bool bResetColours, CString strSubFont, CString strMainFont)
{
	if (!strMainFont.IsEmpty())		m_pMainFont->ExtractFont(strMainFont);
	if (!strSubFont.IsEmpty())		m_pSubFont->ExtractFont(strSubFont);
	
	if (bResetColours)
	{
		m_crBackground = RGB(210,210,210);
		m_crSubText = RGB(0,0,60);
		m_crMainText = RGB(0,0,60);
	}
	
	m_strMainText=strMainText;//.Format("%d",iMainText);//Text;
	m_strSubText.Format("%d",iSubText);// strSubText;
	m_nFadePercent = nFadePercent;
	m_dwStopwatch = GetTickCount();
		Invalidate(FALSE);

}
