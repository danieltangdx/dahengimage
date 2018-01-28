#if !defined(AFX_MYPROGRESSCTRL_H__AE61FF63_27A9_4AD5_AC60_B3550F546DA4__INCLUDED_)
#define AFX_MYPROGRESSCTRL_H__AE61FF63_27A9_4AD5_AC60_B3550F546DA4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyProgressCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyProgressCtrl window

class CMyProgressCtrl : public CProgressCtrl
{
	DECLARE_DYNAMIC(CMyProgressCtrl)
// Construction
public:
	CMyProgressCtrl(BOOL bShowPercent, BOOL bChangeColor);
    virtual ~CMyProgressCtrl();  
	
    typedef enum _ALIGN_TEXT_  
    {  
        ALIGN_LEFT = 0x00,  
			ALIGN_CENTER,  
			ALIGN_RIGHT  
    }ALIGN_TEXT;  

public:  
    inline void SetBarColor(COLORREF clr)  
    {  
        m_clrBarColor = clr;  
    }  
	
    inline COLORREF GetBarColor()  
    {  
        return m_clrBarColor;  
    }  
	
    inline void SetBarBkColor(COLORREF clr)  
    {  
        m_clrBarBkColor = clr;  
    }  
	
    inline COLORREF GetBarBkColor()  
    {  
        return m_clrBarBkColor;  
    }  
	
    inline void SetTextColor(COLORREF clr)  
    {  
        m_clrTextColor = clr;  
    }  
	
    inline COLORREF GetTextColor()  
    {  
        return m_clrTextColor;  
    }  
	
    inline void SetTextBkColor(COLORREF clr)  
    {  
        m_clrTextBkColor = clr;  
    }  
	
    inline COLORREF GetTextBkColor()  
    {  
        return m_clrTextBkColor;  
    }  
	
    inline void SetTextAlign(ALIGN_TEXT AlignText)  
    {  
        m_AlignText = AlignText;  
    }  
	
    inline ALIGN_TEXT GetTextAlign()  
    {  
        return m_AlignText;  
    }  
	
    inline void ShowPercent(BOOL bShowPercent = TRUE)  
    {  
        m_bShowPercent = bShowPercent;  
    }  

	inline void ChangeColor(BOOL bChangeColor = TRUE)  
    {  
        m_bChangeColor = bChangeColor;  
    } 
	
    inline BOOL IsShowPercent()  
    {  
        return m_bShowPercent;  
    } 
	
protected:  
    COLORREF m_clrBarColor;  
    COLORREF m_clrBarBkColor;  
    COLORREF m_clrTextColor;  
    COLORREF m_clrTextBkColor;  
    ALIGN_TEXT m_AlignText;  
    BOOL m_bShowPercent;  
	BOOL m_bChangeColor;
    CFont m_font; // Only for vertical style  
	
protected:  
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);  
    afx_msg void OnPaint();  
	
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPROGRESSCTRL_H__AE61FF63_27A9_4AD5_AC60_B3550F546DA4__INCLUDED_)
