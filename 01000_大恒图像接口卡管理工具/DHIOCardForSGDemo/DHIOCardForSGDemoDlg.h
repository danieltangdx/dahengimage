
// DHIOCardForSGDemoDlg.h : 头文件
//

#pragma once

#include "DHIOCard.h"
#include "DHIOCardForSG.h"
#include "DHIOCardForPIO24C.h"//**[12/19/2017 ConanSteve]**:  C卡
#include "DHIOCardForPCI6356.h"
 #include "DHIOCardForRS232_XZXNYN.h"
#include "ExFn.h"
#include "afxwin.h"
#include "array"
using namespace std;

#pragma comment(lib, "DHIOCardForPCI6356D.lib")
// CDHIOCardForSGDemoDlg 对话框
class CDHIOCardForSGDemoDlg : public CDialogEx
{
// 构造
public:
	CDHIOCardForSGDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DHIOCARDFORSGDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonOpencard1();
	CComboBox m_comboIOcardSN;
	int m_iCurIOCardType; 
	int m_iIOCardNum;
	int m_iLastIOCardSN;//**[12/16/2017 ConanSteve]**:  上一次选中接口卡的序列号
	array<bool, 6> m_arr_bEnable{ false };//**[12/16/2017 ConanSteve]**:  记录每种接口卡是否存在
	unique_ptr<shared_ptr<CDHIOCard>[] > m_pIOCard{ nullptr };
	unique_ptr<e_IOCardType[]> m_p_eIOCardType{ nullptr };//**[12/16/2017 ConanSteve]**:  接口卡类型
	unique_ptr<CString[]> m_p_sPathFileOfInit{ nullptr };//**[12/16/2017 ConanSteve]**:  接口卡配置文件
	unique_ptr<CString[]> m_p_sPathFileOfConnect{ nullptr };//**[12/16/2017 ConanSteve]**:  系统与接口卡连接配置文件配置文件
	CINICfg m_iniIOCardManageCfg;//**[12/16/2017 ConanSteve]**:  接口卡管理工具配置文件
	afx_msg void OnCbnSelchangeComboIocardsn();
	afx_msg void OnBnClickedButtonOpeniocardcfgfile();
	afx_msg void OnBnClickedRadio1(UINT uID);
	afx_msg void OnBnClickedButtonOpenconnectionfile();
	afx_msg void OnBnClickedButtonOpenmanagetoolcfgfile();
	afx_msg void OnBnClickedButtonReinit();
	bool InitWindow();
};
