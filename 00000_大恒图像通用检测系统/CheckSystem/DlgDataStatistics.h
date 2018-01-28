#pragma once


// CDlgDataStatistics dialog

class CDlgDataStatistics : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDataStatistics)

public:
	CDlgDataStatistics(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgDataStatistics();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DateStatistics };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
