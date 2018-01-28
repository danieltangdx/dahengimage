// StaticEx.h: interface for the CStaticEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATICEX_H__B1F7168A_D62F_484D_8524_AA90A39E35E7__INCLUDED_)
#define AFX_STATICEX_H__B1F7168A_D62F_484D_8524_AA90A39E35E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CStaticEx  : public CStatic
{
//	DECLARE_DYNAMIC(CStaticEx)  
public:
	CStaticEx();
	virtual ~CStaticEx();

	COLORREF m_TextColor;
protected:  
    virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);  
	
protected:  
    DECLARE_MESSAGE_MAP()
    afx_msg HBRUSH CtlColor(CDC* pDC, UINT /*nCtlColor*/);  
};

#endif // !defined(AFX_STATICEX_H__B1F7168A_D62F_484D_8524_AA90A39E35E7__INCLUDED_)
