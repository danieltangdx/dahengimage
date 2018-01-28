#pragma once



// CSettingView form view

class CSettingView : public CFormView
{
	DECLARE_DYNCREATE(CSettingView)

protected:
	CSettingView();           // protected constructor used by dynamic creation
	virtual ~CSettingView();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORMVIEW_Setting };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};


