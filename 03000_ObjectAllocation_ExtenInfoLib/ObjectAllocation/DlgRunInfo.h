#pragma once
#include "afxwin.h"


// CDlgRunInfo dialog

class CDlgRunInfo : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRunInfo)

public:
	CDlgRunInfo(CWnd* pParent = NULL,  bool bEnableXCGUI = false);   // standard constructor
	virtual ~CDlgRunInfo();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_RunInfo };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListBox m_listRunInfo;
	afx_msg void OnPaint();
	CEdit m_editRunInfo;
	void UpdateRunInfo(CString & sInfo);//**[8/23/2017 ConanSteve]**:  更新多对象运行窗口信息
	//CString GetCurrentAppPath();
	int OnEventBtnClick(BOOL *pBool);
	void ShowWindowEx(int iShowMode);
	HWINDOW m_hWindow;
	HXCGUI m_hShapeTextRunInfo;
	HELE m_hBtnOK;
	CString m_sAppPath;
	std::wstring m_wsResPath;
	bool m_bEnableXCGUI{ false };
	bool m_bOpenXCWnd{ false };
};
