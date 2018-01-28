// DlgNewModel.cpp : implementation file
//

#include "stdafx.h"
#include "BottleCap.h"
#include "DlgNewModel.h"
#include "MainFrm.h"
#include "BottleCapVIEW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainFrame*			g_pFrame; //全局指针
extern CBottleCapView*     g_pView;

/////////////////////////////////////////////////////////////////////////////
// CDlgNewModel dialog


CDlgNewModel::CDlgNewModel(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgNewModel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgNewModel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgNewModel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgNewModel)
	DDX_Control(pDX, IDC_COMBO_SPEC, m_cmbSpec);
	DDX_Control(pDX, IDC_COMBO_PERCENT, m_cmbPercent);
	DDX_Control(pDX, IDC_COMBO_NAME, m_cmbName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgNewModel, CDialog)
	//{{AFX_MSG_MAP(CDlgNewModel)
	ON_BN_CLICKED(IDC_RADIO_BODY_SHAPE_1, OnRadioBodyShape1)
	ON_BN_CLICKED(IDC_RADIO_BODY_SHAPE_2, OnRadioBodyShape2)
	ON_BN_CLICKED(IDC_RADIO_HEAD_SHAPE_1, OnRadioHeadShape1)
	ON_BN_CLICKED(IDC_RADIO_HEAD_SHAPE_2, OnRadioHeadShape2)
	ON_CBN_SELCHANGE(IDC_COMBO_NAME, OnSelchangeComboName)
	ON_CBN_SELCHANGE(IDC_COMBO_PERCENT, OnSelchangeComboPercent)
	ON_CBN_SELCHANGE(IDC_COMBO_SPEC, OnSelchangeComboSpec)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgNewModel message handlers


void CDlgNewModel::OnOK() 
{
	// TODO: Add extra validation here
	
	int i1=0,i2=0;
	int j1=0,j2=0;

	// 获取瓶体形状
	i1 = ((CButton*)GetDlgItem(IDC_RADIO_BODY_SHAPE_1))->GetCheck();
	i2 = ((CButton*)GetDlgItem(IDC_RADIO_BODY_SHAPE_2))->GetCheck();


	// 获取瓶头形状
	j1 = ((CButton*)GetDlgItem(IDC_RADIO_HEAD_SHAPE_1))->GetCheck();
	j2 = ((CButton*)GetDlgItem(IDC_RADIO_HEAD_SHAPE_2))->GetCheck();


	if(0==i1 && 0==i2)
	{
		AfxMessageBox("请选择瓶子类型");
		return;
	}

	if(0==j1 && 0==j2)
	{
		AfxMessageBox("请选择瓶头类型");
		return;
	}


	// 查看是否增加新名称
	int i=0,j=0;
	BOOL bChange=TRUE;
	CString strTemp1,strTemp2;

	// 产品类型
	m_iLabelType = (m_iNameIndex+1)*100+(m_iSpecIndex+1)*10+(m_iPercentIndex+1);

	// 获取标签名称
	if(0==m_iNameIndex)
	{
		m_sModelName.Format("%s","葡萄糖注射液");
	}
	else if(1==m_iNameIndex)
	{
		m_sModelName.Format("%s","氯化钠注射液");
	}
	else if(2==m_iNameIndex)
	{
		m_sModelName.Format("%s","乳酸纳林格注射液");
	}
	else if(3==m_iNameIndex)
	{
		m_sModelName.Format("%s","葡萄糖氯化钠注射液");
	}
	else if(4==m_iNameIndex)
	{
		m_sModelName.Format("%s","甘露醇注射液");
	}


	//获取毫升数
	if(0==m_iSpecIndex)
	{
		m_sModelName+="50";
	}
	else if(1==m_iSpecIndex)
	{
		m_sModelName+="250";
	}
	else if(2==m_iSpecIndex)
	{
		m_sModelName+="500";
	}
	else if(3==m_iSpecIndex)
	{
		m_sModelName+="1000";
	}
	else if(4==m_iSpecIndex)
	{
		m_sModelName+="150";
	}

	strTemp2 = "无百分比";
	m_cmbPercent.AddString(strTemp2);
	strTemp2 = "0.9%";
	m_cmbPercent.AddString(strTemp2);
	strTemp2 = "5%";
	m_cmbPercent.AddString(strTemp2);
	strTemp2 = "10%";
	m_cmbPercent.AddString(strTemp2);

	// 添加百分比
	if(0==m_iPercentIndex)
	{
		m_sModelName+="";
	}
	else if(1==m_iPercentIndex)
	{
		m_sModelName+="09";
	}
	else if(2==m_iPercentIndex)
	{
		m_sModelName+="5";
	}
	else if(3==m_iPercentIndex)
	{
		m_sModelName+="10";
	}

	// 瓶口类型
	if(100==m_iHeadShape)
	{
		m_sModelName+="拉环";
	}
	else if(101==m_iHeadShape)
	{
		m_sModelName+="双阀";
	}

	//瓶子类型 塑瓶和可立袋
	if(100==m_iBodyShape)
	{
		m_sModelName+="塑瓶";
	}
	else if(101==m_iBodyShape)
	{
		m_sModelName+="可立袋";
	}


	CDialog::OnOK();
}

void CDlgNewModel::OnRadioBodyShape1() 
{
	// TODO: Add your control notification handler code here
	m_iBodyShape = 100;
}

void CDlgNewModel::OnRadioBodyShape2() 
{
	// TODO: Add your control notification handler code here
	m_iBodyShape = 101;
}

void CDlgNewModel::OnRadioHeadShape1() 
{
	// TODO: Add your control notification handler code here
	m_iHeadShape = 100;
}

void CDlgNewModel::OnRadioHeadShape2() 
{
	// TODO: Add your control notification handler code here
	m_iHeadShape = 101;
}

BOOL CDlgNewModel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int i=0,j=0;

	CString strTitle,strTemp1,strTemp2;
// 	strTitle.Format("新建模板--%s",m_sModelName);
	strTitle.Format("新建模板");
	SetWindowText(strTitle);

	// 加载产品名称
	m_cmbName.ResetContent();
	strTemp2 = "葡萄糖注射液";
	m_cmbName.AddString(strTemp2);
	strTemp2 = "氯化钠注射液";
	m_cmbName.AddString(strTemp2);
	strTemp2 = "乳酸纳林格注射液";
	m_cmbName.AddString(strTemp2);
	strTemp2 = "葡萄糖氯化钠注射液";
	m_cmbName.AddString(strTemp2);
	strTemp2 = "甘露醇注射液";
	m_cmbName.AddString(strTemp2);


	// 加载产品规格容量
	m_cmbSpec.ResetContent();
	strTemp2 = "50ml";
	m_cmbSpec.AddString(strTemp2);
	strTemp2 = "150ml";
	m_cmbSpec.AddString(strTemp2);
	strTemp2 = "250ml";
	m_cmbSpec.AddString(strTemp2);
	strTemp2 = "500ml";
	m_cmbSpec.AddString(strTemp2);
	strTemp2 = "1000ml";
	m_cmbSpec.AddString(strTemp2);

	

	// 加载百分比信息
	m_cmbPercent.ResetContent();

	strTemp2 = "无百分比";
	m_cmbPercent.AddString(strTemp2);
	strTemp2 = "0.9%";
	m_cmbPercent.AddString(strTemp2);
	strTemp2 = "5%";
	m_cmbPercent.AddString(strTemp2);
	strTemp2 = "10%";
	m_cmbPercent.AddString(strTemp2);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// 名字
void CDlgNewModel::OnSelchangeComboName() 
{
	// TODO: Add your control notification handler code here
	m_iNameIndex = m_cmbName.GetCurSel();
}

// 百分比
void CDlgNewModel::OnSelchangeComboPercent() 
{
	// TODO: Add your control notification handler code here
	m_iPercentIndex = m_cmbPercent.GetCurSel();
}

// 规格
void CDlgNewModel::OnSelchangeComboSpec() 
{
	// TODO: Add your control notification handler code here

	int iSpec=0;
	iSpec = m_cmbSpec.GetCurSel();

	if(0==iSpec)
	{
		m_iSpecIndex = 0;	//50ml
	}
	else if(1==iSpec)
	{
		m_iSpecIndex = 4;  //150ml
	}
	else if(2==iSpec)
	{
		m_iSpecIndex = 1;   //250ml
	}
	else if(3==iSpec)
	{
		m_iSpecIndex = 2;  //500ml
	}
	else if(4==iSpec)
	{
		m_iSpecIndex = 3;  //1000ml
	}
	
}
