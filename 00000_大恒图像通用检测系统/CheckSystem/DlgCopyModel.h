#pragma once
#include "afxwin.h"


// CDlgCopyModel dialog

class CDlgCopyModel : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCopyModel)

public:
	CDlgCopyModel(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgCopyModel();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_AddModel };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CComboBox m_comboModelList;
	afx_msg void OnBnClickedOk();
};
