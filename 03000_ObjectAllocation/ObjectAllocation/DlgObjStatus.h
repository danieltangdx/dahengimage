#if !defined(AFX_DLGOBJSTATUS_H__89389211_5C5F_4E55_B97C_CEE6CD20CE84__INCLUDED_)
#define AFX_DLGOBJSTATUS_H__89389211_5C5F_4E55_B97C_CEE6CD20CE84__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgObjStatus.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgObjStatus dialog

#define CHANGE_STATUS_MSG_2IDLE	1700
#define CHANGE_STATUS_MSG_2DO	1701
#define CHANGE_STATUS_MSG_2DRAW	1702

class CDlgObjStatus : public CDialog
{
// Construction
public:
	CDlgObjStatus(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgObjStatus)
	enum { IDD = IDD_DLG_OBJSTATUS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgObjStatus)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	int **m_ppChange;
	BOOL m_bShowing;
	void InitData(int nNumWS, int* pNumObj);
	void w2Idle(int nWS, int nObj);	//0
	void w2Do(int nWS, int nObj);	//1
	void w2Draw(int nWS, int nObj);	//2

	// Show check information
	void ShowInfo(char* pObj, char* pList, char* pImage, char* pCam);

protected:
	inline bool CheckDataValid(int nWS, int nObj);
	int m_nNumWorkstation;
	int *m_pnObjNum;
	int m_nMaxObjNum;
	BOOL m_bPainted;
	CBrush m_BackBrush;
	CBitmap m_StatusImg[3];

	// Generated message map functions
	//{{AFX_MSG(CDlgObjStatus)
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	afx_msg LRESULT OnMsg2Idle(WPARAM wParam, LPARAM lParam);	//0
	afx_msg LRESULT OnMsg2Do(WPARAM wParam, LPARAM lParam);		//1
	afx_msg LRESULT OnMsg2Draw(WPARAM wParam, LPARAM lParam);	//2
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOBJSTATUS_H__89389211_5C5F_4E55_B97C_CEE6CD20CE84__INCLUDED_)
