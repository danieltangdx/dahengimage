#pragma once


// CDlgParamSet 对话框

#include "DHPropGridCtrl.h"
/*************************************<>************************************[9/13/2017 ConanSteve]*/


enum class e_Grid1DllID {//**[11/23/2017 ConanSteve]**:  ParamList一级菜单枚举
	SystemConfig=0,
	RunControl=1
};

enum class e_Grid2SysCfg {
	SaveImage=0,//**[12/5/2017 ConanSteve]**:  存图
};

enum class e_Grid2RunCtrl {
	RunCtrl = 0
};

class CDlgParamSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgParamSet)

public:
	CDlgParamSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgParamSet();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ParamSet };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL UpdateData(BOOL bSaveAndValidate = TRUE);
	DECLARE_MESSAGE_MAP()



public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg LRESULT OnDHPropGrid_DataChanged(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDHPropGrid_ButtonClicked(WPARAM wParam, LPARAM lParam);

	virtual BOOL OnInitDialog();

private:
	//**[9/11/2017 ConanSteve]**:  初始化DHPropertyGridCtrl控件
	bool InstallPropertyGrid();
private:
	CDHPropGridCtrl  m_gridParamSet;
	//**[11/24/2017 ConanSteve]**:  是否打开相机调试模式
	unique_ptr<bool[]> m_p_bEnableDebugCam;
public:
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedOk();
};
