#include "stdafx.h"
#include "DemoProjectDlg.h"

void CDemoProjectDlg::IntegrateCtrls()
{
	m_MainCtrl.push_back(IDC_TAB1);					//属性页
	m_MainCtrl.push_back(IDC_STATIC_CMODEL);		//当前模板名称	Static
	m_MainCtrl.push_back(IDC_STATIC_INFO);			//显示模板相关信息 Static
	m_MainCtrl.push_back(IDC_BUTTON_FoldPage);
	m_MainCtrl.push_back(IDC_BTN_PARKOUR);			//一键检测 PushButton
	m_MainCtrl.push_back(IDC_BTN_SAVESINGLEMODEL);	//保存当前路模板 PushButton
	m_MainCtrl.push_back(IDC_BTN_SAVEMODEL);		//保存全部模板	PushButton
	//m_MainCtrl.push_back(IDC_STATIC_SPLIT);			//
	m_MainCtrl.push_back(IDC_BTN_EXIT2);			//退出  PushButton
	m_MainCtrl.push_back(IDC_BTN_UPDMDL);			//加载模板	PushButton
	m_MainCtrl.push_back(IDC_BTN_MLIST);			//模板列表和文件列表切换	PushButton
	m_MainCtrl.push_back(IDC_STATIC_1);
	m_pMainCtrlPR = new s_CtrlPosRatio[m_MainCtrl.size()];

	m_SideCtrl.push_back(IDC_STATIC_BCK);
	m_SideCtrl.push_back(IDC_STATIC_MLIST);		//模板列表 static
	m_SideCtrl.push_back(IDC_STATIC_NEWNAME);	//输入新模板名称 static
	m_SideCtrl.push_back(IDC_MODELLIST);		//模板列表 listbox
	m_SideCtrl.push_back(IDC_STATIC_KJ);		//新建模板 groupbox
	m_SideCtrl.push_back(IDC_EDIT1);			//模板名称 edittext
	m_SideCtrl.push_back(IDC_NEWMODEL);			//新建模板 pushbutton
	m_SideCtrl.push_back(IDC_BTN_DELETE);		//删除模板 pushbutton
	m_SideCtrl.push_back(IDC_BTN_SETD);			//设为默认模板	pushbutton
	m_SideCtrl.push_back(IDC_COMBO_STOPPERTYPE);//组合框 胶塞使用 未用
	m_SideCtrl.push_back(IDC_STATIC_FILEBACK);	//本地图像浏览器 groupbox
	m_SideCtrl.push_back(IDC_STATIC_FILE);		//File 暂未使用
	m_SideCtrl.push_back(IDC_STATIC_DISK);		//硬盘 暂未使用
	m_SideCtrl.push_back(IDC_BTN_ERRIMG);		//定位到错误图像 PushButton
	m_SideCtrl.push_back(IDC_BTN_GRABIMG);		//定位到连续图像 PushButton
	m_SideCtrl.push_back(IDC_BTN_CAPIMG);		//定位到有胶囊图像 PushButton
	m_SideCtrl.push_back(IDC_BTN_CLRST);		//清空	PuchButton
	m_SideCtrl.push_back(IDC_LIST_RESULT);		//算法检测结果 ListBox
	m_SideCtrl.push_back(IDC_EDIT_CURPATH);		//当前属性页的图像路径 EditText
	m_SideCtrl.push_back(IDC_STATIC_VERSION);	//版本信息	Static
	m_SideCtrl.push_back(IDC_EDIT_ModelLog);
	m_SideCtrl.push_back(IDC_CHECK_ShowModelLog);
	m_pSideCtrlPR = new s_CtrlPosRatio[m_SideCtrl.size()];
}
