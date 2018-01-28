// BottleCapView.h : interface of the CBottleCapView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BottleCapVIEW_H__2CE4C44C_E38A_4C4D_A1CD_244FE6789ECE__INCLUDED_)
#define AFX_BottleCapVIEW_H__2CE4C44C_E38A_4C4D_A1CD_244FE6789ECE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "resource.h"       // main symbols
#include "MainFrm.h"
#include "StaticFader.h"
#include "BottleCapDoc.h"
#include "xShadeButton.h"
#include "DlgShowImage.h"
#include "DlgPage1.h"
#include "DlgPage2.h"
#include "DlgAlarm.h"
#include "DlgDefect.h"
#include "DlgAlarmPos.h"
#include "DlgAlarmStopCount.h"
#include "DlgUserAdm.h"

class CBottleCapView : public CFormView
{
protected: // create from serialization only
	CBottleCapView();
	DECLARE_DYNCREATE(CBottleCapView)

public:
	//{{AFX_DATA(CBottleCapView)
	enum { IDD = IDD_BottleCap_FORM };
	CStaticFader	m_UserData;
	CStaticFader	m_UserLabel;
	CStaticFader	m_SpeedData;
	CStaticFader	m_SpeedLabel;
	CxShadeButton	m_BtnZero;
	CxShadeButton	m_BtnDefectStatistic;
	CTabCtrl	m_Tab1;
	CxShadeButton	m_BtnModel;
	CxShadeButton	m_BtnParam;
	CxShadeButton	m_BtnQuit;
	CxShadeButton	m_BtnRecord;
	CxShadeButton	m_BtnStatistic;
	CxShadeButton	m_BtnStart;
	CStaticFader	m_TotalGoodRatioData;
	CStaticFader	m_TotalGoodRatioLabel;
	CStaticFader	m_TotalGoodLabel;
	CStaticFader	m_TotalGoodData;
	CStaticFader	m_ModelNameLabel;
	CStaticFader	m_ModelNameData;
	CStaticFader	m_TotalData;
	CStaticFader	m_TotalLabel;
	//}}AFX_DATA

// Attributes
public:
	CBottleCapDoc* GetDocument();

public:
	int m_iIndex;					//列表框中条目个数
	int m_iSaveImageType;			//保存图像类型 0为连续，1为错误
	int m_iIsCircle;				//是否循环保存图像 0为不循环
	int m_iSaveImageCount;			//保存图像张数
	int m_iLabelFontHeight;			//标签字体大小

	BOOL m_bAlarm_StopCount[ALLELEMENT_COUNT]; //是否停止计数报警
	BOOL m_bLabelAlarm[ALLELEMENT_COUNT];	// 标签检测报警
	BOOL m_bQuitAlarmThread;		// 退出报警线程

	CRect  m_rectShowImage[CAMERA_COUNT];								//图像压缩显示框		
	CCriticalSection m_csShowBitmapInfoSection;

	CFont m_fontTitle;				//标题字体
	CFont m_fontTime;				//时间字体
	CFont m_fontSystem;				//系统设置字体
	CFont m_fontAdv;				//高级模式字体

	CDlgShowImage* m_pShowImage;	//显示图像
	CDlgAlarm* m_pdlgAlarm;			//显示报警信息
	CDlgAlarmPos* m_pdlgAlarmPos;	//标签位置报警信息
	CDlgDefect* m_pdlgDefect;		//显示缺陷统计信息

	CDlgAlarmStopCount* m_pdlgAlarmStop;	//停止计数报警

	
	CDlgPage1 m_dlgPage1;			//查看错误图像属性页1

	// 显示报警信息线程
	CWinThread* m_pShowWarmingInfoThread;	
	static DWORD ShowWarmingInfoThread(LPVOID lpParam);
	DWORD ShowWarmingInfoThreadFunc();

	// 监测停止计数
	CWinThread* m_pMonitorStopCountThread;	
	static DWORD MonitorStopCountThread(LPVOID lpParam);
	DWORD MonitorStopCountThreadFunc();



// Operations
public:
	//添加记录到列表控件
	//iCamGroup 表示工位号 iImageSN 图像号  iCircleSN 采集序号(误触发及丢帧情况下与iImageSN不同)
	void AddRecordToListCtrl(int iCamGroup, int iImageSN, int iCircleSN, CString strSaveTime);

	// 加载光源信息和产品信息
	BOOL LoadLightAndProductInfo(CString strModelName,CString sOldModelName);

	//外触发情况下显示图像
	void DisplayCameraImage(CDC* pDC, const int iCameraSN, int iIndex, e_ShowModeFlip eShowMode);
	void DisplayImage(const int iCamGroup, int iIndex);

	void ShowDIBImage(CDC* pDC, CRect Rect, e_ShowModeFlip ShowModeFlip, 
		e_ShowModeStretch ShowModeStretch,const BYTE* pImageBuff,
		long lImageWidth, long lImageHeight, long lImageBitCount,int iCameraSN);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBottleCapView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:

	// 初始化窗口
	BOOL InitWindowRect();
	virtual ~CBottleCapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBottleCapView)
	afx_msg void OnBtnQuit();
	afx_msg void OnBtnParam();
	afx_msg void OnBtnMouldAdm();
	afx_msg void OnBtnStatistic();
	afx_msg void OnBtnStart();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBtnRecord();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSaveImage();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnIocard();
	afx_msg void OnSystemConfig();
	afx_msg void OnAdvMode();
	afx_msg void OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnZero();
	afx_msg void OnBtnDefectStatistics();
	afx_msg void OnStopSaveImage();
	afx_msg void OnUserAdm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedStopSaveImage();
	afx_msg void OnStnClickedMp();
	afx_msg void OnStnDblclickMp();
};

#ifndef _DEBUG  // debug version in BottleCapView.cpp
inline CBottleCapDoc* CBottleCapView::GetDocument()
   { return (CBottleCapDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BottleCapVIEW_H__2CE4C44C_E38A_4C4D_A1CD_244FE6789ECE__INCLUDED_)
