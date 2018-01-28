#if !defined(AFX_STATICFADER_H__7F93423B_AD47_11D3_803B_00805FC1DE10__INCLUDED_)
#define AFX_STATICFADER_H__7F93423B_AD47_11D3_803B_00805FC1DE10__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticFader.h : header file
//

#include <afxtempl.h>
#include "MemDC.h"
#include "AutoFont.h"

/////////////////////////////////////////////////////////////////////////////
// CStaticFader window

class CStaticFader : public CStatic
{
// Construction
public:
	CStaticFader();

// Attributes
public:
   	CAutoFont* m_pSubFont;
	CAutoFont* m_pMainFont;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticFader)
	//}}AFX_VIRTUAL

/////////////////////////////////////////////////////////////////////////////
// Public Member Functions /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
public:
	void Display(CString strMainText, int iSubText, int nFadePercent, bool bResetColours, CString strSubFont, CString strMainFont);
	void Display(int iMainText, CString strSubText, int nFadePercent, bool bResetColours, CString strSubFont, CString strMainFont);

	void Displayint(int iMainText, int iSubText, int nFadePercent, bool bResetColours, CString strSubFont, CString strMainFont);
	void DisplayDepText(CString strText,CString strSubText,int interger);//显示高亮度的文字，定时消隐
	CString m_strSubText, m_strMainText;
	void Initialise( COLORREF crBG, COLORREF crMainText=RGB(0,0,0), COLORREF crSubText=RGB(0,0,0));
	
	void SetAlignLeft(){m_uAlignFlag = DT_LEFT;};
	
	void SetAlignRight(){m_uAlignFlag = DT_RIGHT;};
	
	void SetAlignCentre(){m_uAlignFlag = DT_CENTER;};
	
	void SetDrawShadow(bool bShadow = true)	{	m_bDrawShadow = bShadow; };

	void SetDrawBorder(bool bBorder = true)	{ m_bDrawBorder = bBorder; };

	COLORREF GetBackgroundColour()	{	return m_crBackground;	};
	
	void SetBackgroundColour(COLORREF crColour)	{	m_crBackground = crColour;	};

	void SetSubTextColour(COLORREF crColour)	{	m_crSubText = crColour;	};
	
	void SetMainTextColour(COLORREF crColour)	{	m_crMainText = crColour;};

	void Display(CString strText, CString strSubText, int nFadePercent=50, bool bResetColours=false, CString strSubFont="", CString strMainFont=""); // the fade percent is percent of one second
	
	virtual ~CStaticFader();

/////////////////////////////////////////////////////////////////////////////
// Protected Member Functions /////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
protected:
	void DrawText(CMemDC* pDC);
	COLORREF GetCurrentColour( COLORREF crText=RGB(0,0,0) );
	
/////////////////////////////////////////////////////////////////////////////
// Protected Member Variables /////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
protected:
	UINT m_uAlignFlag;
	int m_nFadePercent;
	bool m_bDrawBorder;
	bool m_bDrawShadow;
	COLORREF m_crSubText, m_crMainText;

	
	COLORREF m_crBackground;
	CRect m_rectClient;
	DWORD m_dwStopwatch;

	
	//{{AFX_MSG(CStaticFader)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bChangFont;//是否改变字体高度
	BOOL m_bDep;//高亮显示标志
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICFADER_H__7F93423B_AD47_11D3_803B_00805FC1DE10__INCLUDED_)
