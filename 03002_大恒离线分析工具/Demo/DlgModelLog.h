#pragma once


// CDlgModelLog dialog

class CDlgModelLog : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgModelLog)

public:
	CDlgModelLog(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgModelLog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	/************************************************************************/
	/*                       [5/31/2017 ConanSteve]                         */
	CListBox m_listModelLog;
	/************************************************************************/
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnPaint();
};
