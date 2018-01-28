// DemoProjectDlg.h : header file
//

#if !defined(AFX_DEMOPROJECTDLG_H__B0780076_FE0D_481E_ABD0_F74BD7787628__INCLUDED_)
#define AFX_DEMOPROJECTDLG_H__B0780076_FE0D_481E_ABD0_F74BD7787628__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "resource.h"
#include "StaticEx.h"
#include "DHListBox.h"
#include <vector>
#include "InspectClass.h"
#include "ObjAlloc.h"
#include "afxwin.h"

/************************************************************************/
/*                       [5/31/2017 ConanSteve]                         */
#include "DlgModelLog.h"
#include "memory"
#include "map"
#include "vector"
#include "DemoInterface.h"
/************************************************************************/
using namespace std;

//#define CAPSULE_DEMO_VERSION
#define COMMON_DEMO_VERSION
#define   MSG_APP_LOGIN			WM_APP+6000			//DEMO启动后，系统通知DEMO是否使能保存模板功能

typedef struct _CTRL_POS_RATIO
{
	float fTop;
	float fBottom;
	float fLeft;
	float fRight;
}s_CtrlPosRatio;
/////////////////////////////////////////////////////////////////////////////
// CDemoProjectDlg dialog

typedef CInspBase*(*pFunc_Create)(int);//int 表示new 算法对象的个数 ，0表示1个，1表示两个 new CDetect[xxx];

typedef void(*pFunc_Destroy)(CInspBase*);

typedef void (*pFunc_GetTextFromLPK)(char*, int, ...);

class CDemoProjectDlg : public CDialog
{
// Construction
public:
	CDemoProjectDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CDemoProjectDlg)
	enum { IDD = IDD_DEMOPROJECT_DIALOG };
	CButton	m_SAVESINGLE;
	CDHResultBox	m_lResult;
	CStaticEx		m_NewModel;
	CStaticEx		m_NewName;
	CStaticEx		m_MList;
	CDHModelBox		m_lModelNames;
	CTabCtrl		m_Tab;
	CString			m_strNewModelName;
	CString			m_strCModel;
	CString			m_strVersion;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDemoProjectDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
private:
	// Generated message map functions
	//{{AFX_MSG(CDemoProjectDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBtnSave();
	afx_msg void OnButtonMlist();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBtnCreate();
	afx_msg void OnBtnDelete();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnBtnSetd();
	afx_msg void OnBtnClrst();
	afx_msg void OnBtnErrimg();
	afx_msg void OnBtnGrabimg();
	afx_msg void OnBtnCapimg();
	afx_msg void OnBtnImgTest();
	afx_msg void OnBtnUpdmdl();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnBtnSaveSingle();
	afx_msg void OnBtnParkour();
	afx_msg void OnBtnLanguage();
	afx_msg void OnBtnInsp();
	afx_msg void OnBtnExit2();
	afx_msg void OnSelchangeComboStoppertype();
	//}}AFX_MSG
	afx_msg LRESULT OnMsgFromListBox(WPARAM wParam, LPARAM lParam); 
	afx_msg LRESULT OnMsgFromSysReadModel(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL	OnDeviceChange(UINT nEventType, DWORD dwData);
	afx_msg LRESULT OnMsgFromBackupM(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgFromInternalThread(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnEnterSizeMove(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnExitSizeMove(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnAppLogin(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnOK();
	virtual void OnCancel();
	void CreateTabWins(int nWinTop);
	void ShowTabWins(int nWinNum);
	void StopParkour();

	CString m_strSystemName, m_strSysModelName, m_strDemoTitle, m_strImgPath[3];
	CString	m_strModelNameUsing;
	CString m_sAlgConfigPath{ TEXT("") };
	bool m_bNewInProcess;
	bool m_bOpenViaSystem{ false };
	BOOL m_bShowSide;
	BOOL m_bDisableCreate, m_bDisableDelete, m_bDisableSetDef, m_bDisableSave, m_bDisableLoad, m_bDisableImgBtn[3];
	BOOL m_bCtrlIsDown;
	CStringArray m_strArrVersion, m_strListPath, m_strDllName, m_strImgFolder;

	BOOL m_bStopperMode;
	int  m_nStopperType;
	CString m_sStopperType{ TEXT("") };
	bool m_bFirstInitModel{ true };

protected:
	bool CopyDir(CString strSource, CString strDest);
	BOOL DeleteDir(CString strDir);
	bool GetIfNeedSave();
#ifdef COMMON_DEMO_VERSION
	BOOL LoadConfigs();
#endif
#ifdef CAPSULE_DEMO_VERSION
	BOOL Capsule_LoadConfigs();
#endif
	void DisEnMdlList(int nFlag);
	bool GetIfDrawing();
	void GetFileVersion(CString strPath, CString& strVersion);
	void SaveModelBeforeQuit(BOOL bPopMsgBox);
	void LoadControlStrings();
	void ConfigInterCtrls();
	void FreeMemories();
	void WriteOpenParas();
	void GetAllRatios();
	void ModifySidebarWidth(int nNewWidth, BOOL bMoveAll=TRUE);
	void ScaleMoveWindow();
	void IntegrateCtrls();
	void VisualSidebar();
	
	int m_nNumWS;
	HICON m_hIcon;
	CRect m_StaticRect;
	CRect m_TabRect;
	UINT m_uTimerSetWindow, m_uTimerWaiting, m_uSearchBM;
	int m_nOpenMode; //0:双击打开  1:系统打开
	BOOL m_bIsWaiting;
	BOOL m_bFirstCreate;

	BOOL m_bMoveSize;
	BOOL m_bF11Pressed;
	CRect m_CurWndSize;
	int m_StandWndSize;
	CRect m_WndInitSize;
	BOOL m_bLBtnDown;
	int m_nSideWidth;
	int m_nSideWidthTemp;
	float m_fSideWidth;
	BOOL m_bSidebarHide;
	vector<UINT> m_MainCtrl;
	vector<UINT> m_SideCtrl;
	s_CtrlPosRatio* m_pMainCtrlPR;
	s_CtrlPosRatio* m_pSideCtrlPR;
	BOOL m_bNeedRemap;

	bool    m_bFileIsShow;
	//Static透明背景
	CBrush m_brush;
	int m_nbrush;

	// Tab
	CImageList m_TabImgList;

	int  m_nLastPage;
	int m_DefaultTab;
	bool m_bMouseIsDoing;
	CBitmap m_cBitLogo;
	int* m_nInitTopIndex;
	CDHFileBox* m_plFile;
	CDHDiskBox* m_plDisk;

	BOOL m_bInitSuccess;

	bool m_bModelChanged;
	bool m_bAskingSystem;
	bool m_bChangeType;
	bool m_bNewCreateModel;
	CFont m_cModelNameFont, m_cVersionFont;

	CObjAlloc m_multiObjManager;

	

/************************************************************************/
/*                       [5/31/2017 ConanSteve]                         */
public:
		int m_iShowModelLog;
		CEdit m_editModelLog;
		shared_ptr<CDlgModelLog> m_pDlgModelLog{ nullptr };
		map<CString, vector<CString>> m_mapModelLog;
		bool m_bEnableModelSearch{ false };//**[8/8/2017 ConanSteve]**:  打开模板搜索功能
		bool m_bEnableShowModelLog{ false };//**[8/8/2017 ConanSteve]**:  打开日志搜索功能
		bool m_bEnableVerificationOfSaveModel{ false };
		bool m_bShowLoadModalDlg{ false };//**[12/22/2017 ConanSteve]**:  是否显示加载并退出窗口
		bool ReadSystemConfig();
		bool SaveSystemConfig();
		bool InitModel(CString sFilter = TEXT("*.*"));//**[8/8/2017 ConanSteve]**:  Initialize comboBox ModelList
		void InitModelLog();//**[8/14/2017 ConanSteve]**:  读取模板日志记录
		afx_msg void OnEnChangeEditModellog();/**   搜索框信息发生修改事件            [6/5/2017 ConanSteve]**/
		afx_msg void OnLbnSelchangeModellist();
/************************************************************************/
		afx_msg void OnBnClickedCheckShowmodellog();
		void CreateModelLog(CString& sModelName);
		void FnWriteLogOfSaveModel(int iIndex);
		CStdioFile m_fileErrorLog;
		bool m_bOpenFileOfErrorLog{ false };
		HANDLE m_hEvent_DemoMessage_SendCommandOfLoadModel{NULL};
		HANDLE m_hEvent_DemoMessage_ReceiveSuccessOfLoadModel{ NULL };
		HANDLE m_hEvent_DemoMessage_ReceiveFailureOfLoadModel{ NULL };
		CCriticalSection m_csProgressCtrl;
		static DWORD WINAPI ThreadLoadModel(LPVOID pPara);
		DWORD ThreadFnLoadModel();
		bool m_bHandledLoadModelMsg{ false };
		bool UpdateUILanguage(CString sLanguageFilePath);//**[8/18/2017 ConanSteve]**:  更新界面语言
		afx_msg void OnBnClickedBtnSavesinglemodel();
		virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		afx_msg void OnNcMButtonDblClk(UINT nHitTest, CPoint point);
		afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
		afx_msg void OnBnClickedButtonFoldpage();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEMOPROJECTDLG_H__B0780076_FE0D_481E_ABD0_F74BD7787628__INCLUDED_)
