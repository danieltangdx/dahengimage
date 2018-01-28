
// XCMFCView.h : interface of the CXCMFCView class
//

#pragma once

//#include "MyWnd.h"
#include "XCTitleBar.h"
#include "XCCtrlBtnBar.h"
#include "XCStatusInfoBar.h"
#include "DlgRunning.h"
#include "DlgSetting.h"
#include "SettingView.h"

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
class CCheckSystemView : public CView
{
protected: // create from serialization only
	CCheckSystemView();
	DECLARE_DYNCREATE(CCheckSystemView)

// Attributes
public:
	CCheckSystemDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CCheckSystemView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnStart();


public:
	CXCTitleBar m_xcTitleBar;//**[9/5/2017 ConanSteve]**:  标题栏
	CXCCtrlBtnBar m_xcCtrlBtnBar;
	CXCStatusInfoBar m_xcStatusInfoBar;
	//**[9/5/2017 ConanSteve]**:  TabPage
	enum e_TabPage
	{
		Running,
		Setting,
	};
	using PTabPage = shared_ptr<CDialogEx>;
	unique_ptr<PTabPage[]> m_pdlgTabPage{ nullptr };
	CDlgRunning* m_pdlgRunning{ nullptr };
	CDlgSetting* m_pdlgSetting{ nullptr };
	int m_iTabPageIndex{ 0 };

	/*************************************<字体>************************************[12/4/2017 ConanSteve]*/
	CFont m_fontTitle;
	CFont m_fontTime;
	CFont m_fontSystem;
	CFont m_fontAdv;


	bool OnBnClickTabPageChange(int iPage);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//**[9/5/2017 ConanSteve]**:  显示相机的原始图像
	void DisplayGrabImg(int iGrabSN);




	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual void PostNcDestroy();
};

#ifndef _DEBUG  // debug version in CheckSystemView.cpp
inline CCheckSystemDoc* CCheckSystemView::GetDocument() const
   { return reinterpret_cast<CCheckSystemDoc*>(m_pDocument); }
#endif

