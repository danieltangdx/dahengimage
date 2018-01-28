// DlgUserAdm.cpp : implementation file
//

#include "stdafx.h"
#include "BottleCap.h"
#include "DlgUserAdm.h"
#include "DlgAddUser.h"
#include "MainFrm.h"
#include "DlgModifyCipher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgUserAdm dialog


CDlgUserAdm::CDlgUserAdm(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUserAdm::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgUserAdm)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_iUserType=0;
}


void CDlgUserAdm::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgUserAdm)
	DDX_Control(pDX, IDC_LIST_USER, m_listUser);
	DDX_Control(pDX, IDC_LIST_DEBUG, m_listDebug);
	DDX_Control(pDX, IDC_LIST_ADM, m_listAdm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgUserAdm, CDialog)
	//{{AFX_MSG_MAP(CDlgUserAdm)
	ON_BN_CLICKED(IDC_BTN_ADM_ADD, OnBtnAdmAdd)
	ON_BN_CLICKED(IDC_BTN_ADM_DELETE, OnBtnAdmDelete)
	ON_BN_CLICKED(IDC_BTN_ADM_MODIFY, OnBtnAdmModify)
	ON_BN_CLICKED(IDC_BTN_DEBUG_ADD, OnBtnDebugAdd)
	ON_BN_CLICKED(IDC_BTN_DEBUG_DELETE, OnBtnDebugDelete)
	ON_BN_CLICKED(IDC_BTN_DEBUG_MODIFY, OnBtnDebugModify)
	ON_BN_CLICKED(IDC_BTN_USER_ADD, OnBtnUserAdd)
	ON_BN_CLICKED(IDC_BTN_USER_DELETE, OnBtnUserDelete)
	ON_BN_CLICKED(IDC_BTN_USER_MODIFY, OnBtnUserModify)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgUserAdm message handlers

BOOL CDlgUserAdm::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	LoadUser();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgUserAdm::LoadUser()
{
	int i=0,j=0;
	
	CString strName,strCipher;
	CString strApp,strKey;
	int iAdv=0;
	
	// 获取管理员
	strApp.Format("%s","Adv");
	strKey.Format("%s","Num");
	iAdv = GetPrivateProfileInt(strApp,strKey,1,"Cipher.cip");
	m_listAdm.ResetContent();
	for(i=0;i<iAdv;i++)
	{
		strKey.Format("Name_%d",i+1);
		GetPrivateProfileString(strApp,strKey,"",strName.GetBuffer(255),255,"Cipher.cip");
				
		if(strName!="")
		{
			m_listAdm.AddString(strName);
		}
	}
	
	// 获取调试员
	strApp.Format("%s","Debugger");
	strKey.Format("%s","Num");
	iAdv = GetPrivateProfileInt(strApp,strKey,0,"Cipher.cip");
	m_listDebug.ResetContent();
	for(i=0;i<iAdv;i++)
	{
		strKey.Format("Name_%d",i+1);
		GetPrivateProfileString(strApp,strKey,"",strName.GetBuffer(255),255,"Cipher.cip");
		
		if(strName!="")
		{
			m_listDebug.AddString(strName);
		}
	}

	//获取普通用户
	strApp.Format("%s","User");
	strKey.Format("%s","Num");
	iAdv = GetPrivateProfileInt(strApp,strKey,0,"Cipher.cip");
	m_listUser.ResetContent();
	for(i=0;i<iAdv;i++)
	{
		strKey.Format("Name_%d",i+1);
		GetPrivateProfileString(strApp,strKey,"",strName.GetBuffer(255),255,"Cipher.cip");
		
		if(strName!="")
		{
			m_listUser.AddString(strName);
		}
	}

}

void CDlgUserAdm::OnBtnAdmAdd() 
{
	// TODO: Add your control notification handler code here

	CString strApp,strKey,strValue;
	CString strUser,strPW;
	int i=0;

	CDlgAddUser dlg;
	dlg.m_iUserType=1;

	if(IDOK==dlg.DoModal())
	{
		strUser = dlg.m_strUser;
		strPW = dlg.m_strPassword;

		// 获取管理员人数
		strApp.Format("%s","Adv");
		strKey.Format("%s","Num");

		int nNum = GetPrivateProfileInt(strApp,strKey,1,"Cipher.cip");

		if(nNum<1)
		{
			nNum=1;
		}

		// 写入信息
		strValue.Format("%d",(nNum+1));
		WritePrivateProfileString(strApp,strKey,strValue,"Cipher.cip");

		strKey.Format("Name_%d",nNum+1);
		strValue=strUser;
		WritePrivateProfileString(strApp,strKey,strValue,"Cipher.cip");

		strKey.Format("Cipher_%d",nNum+1);
		strValue=strPW;
		WritePrivateProfileString(strApp,strKey,strValue,"Cipher.cip");
	}

	LoadUser();

}

void CDlgUserAdm::OnBtnAdmDelete() 
{
	// TODO: Add your control notification handler code here
	int i=0,j=0;

	CString strCurr;
	CString strApp,strKey,strName,strPW,strVaule;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();

	if(m_listAdm.GetCurSel()<0)
		return;

	m_listAdm.GetText(m_listAdm.GetCurSel(),strCurr);

	if(strCurr==pFrame->m_sUserName)
	{
		AfxMessageBox("不能删除当前用户!");
		return;
	}

	if(strCurr=="administrator")
	{
		AfxMessageBox("不能删除管理员 'administrator' !");
		return;
	}

	// 获取管理员个数
	strApp.Format("%s","Adv");
	strKey.Format("%s","Num");
	
	int nNum = GetPrivateProfileInt(strApp,strKey,1,"Cipher.cip");

	CStringList lsUser,lsPW;
	for(i=0;i<nNum;i++)
	{
		strKey.Format("Name_%d",i+1);
		GetPrivateProfileString(strApp,strKey,"",strName.GetBuffer(255),255,"Cipher.cip");

		strKey.Format("Cipher_%d",i+1);
		GetPrivateProfileString(strApp,strKey,"",strPW.GetBuffer(255),255,"Cipher.cip");
		
		if(strName!=strCurr)
		{
			lsUser.AddTail(strName);
			lsPW.AddTail(strPW);
		}
	}


	for(i=0;i<nNum-1;i++)
	{
		strKey.Format("Name_%d",i+1);
		WritePrivateProfileString(strApp,strKey,lsUser.GetHead(),"Cipher.cip");
		lsUser.RemoveHead();

		strKey.Format("Cipher_%d",i+1);
		WritePrivateProfileString(strApp,strKey,lsPW.GetHead(),"Cipher.cip");
		lsPW.RemoveHead();
	}

	// 修改个数及最后一个
	strKey.Format("%s","Num");
	strVaule.Format("%d",nNum-1);
	WritePrivateProfileString(strApp,strKey,strVaule,"Cipher.cip");

	strKey.Format("Name_%d",nNum);
	WritePrivateProfileString(strApp,strKey,"","Cipher.cip");

	strKey.Format("Cipher_%d",nNum);
	WritePrivateProfileString(strApp,strKey,"","Cipher.cip");

	LoadUser();
}

void CDlgUserAdm::OnBtnAdmModify() 
{
	// TODO: Add your control notification handler code here
	
	CString strCurr;

	if(m_listAdm.GetCurSel()<0)
		return;

	m_listAdm.GetText(m_listAdm.GetCurSel(),strCurr);

	CDlgModifyCipher dlg;
	dlg.m_iUserType=1;
	dlg.m_iCallType=1;
	dlg.m_strUser=strCurr;
	dlg.DoModal();

}

void CDlgUserAdm::OnBtnDebugAdd() 
{
	// TODO: Add your control notification handler code here
	CString strApp,strKey,strValue;
	CString strUser,strPW;
	
	CDlgAddUser dlg;
	dlg.m_iUserType=2;
	
	if(IDOK==dlg.DoModal())
	{
		strUser = dlg.m_strUser;
		strPW = dlg.m_strPassword;
		
		// 获取调试员人数
		strApp.Format("%s","Debugger");
		strKey.Format("%s","Num");
		
		int nNum = GetPrivateProfileInt(strApp,strKey,0,"Cipher.cip");
		
		if(nNum<0)
		{
			nNum=0;
		}
		
		// 写入信息
		strValue.Format("%d",(nNum+1));
		WritePrivateProfileString(strApp,strKey,strValue,"Cipher.cip");
		
		strKey.Format("Name_%d",nNum+1);
		strValue=strUser;
		WritePrivateProfileString(strApp,strKey,strValue,"Cipher.cip");
		
		strKey.Format("Cipher_%d",nNum+1);
		strValue=strPW;
		WritePrivateProfileString(strApp,strKey,strValue,"Cipher.cip");
	}
	
	LoadUser();
}

void CDlgUserAdm::OnBtnDebugDelete() 
{
	// TODO: Add your control notification handler code here
	int i=0,j=0;
	
	CString strCurr;
	CString strApp,strKey,strName,strPW,strVaule;
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	
	if(m_listDebug.GetCurSel()<0)
		return;
	
	m_listDebug.GetText(m_listDebug.GetCurSel(),strCurr);
	
	if(strCurr==pFrame->m_sUserName)
	{
		AfxMessageBox("不能删除当前用户!");
		return;
	}
	
	
	// 获取管理员个数
	strApp.Format("%s","Debugger");
	strKey.Format("%s","Num");
	
	int nNum = GetPrivateProfileInt(strApp,strKey,0,"Cipher.cip");
	
	CStringList lsUser,lsPW;
	for(i=0;i<nNum;i++)
	{
		strKey.Format("Name_%d",i+1);
		GetPrivateProfileString(strApp,strKey,"",strName.GetBuffer(255),255,"Cipher.cip");
		
		strKey.Format("Cipher_%d",i+1);
		GetPrivateProfileString(strApp,strKey,"",strPW.GetBuffer(255),255,"Cipher.cip");
		
		if(strName!=strCurr)
		{
			lsUser.AddTail(strName);
			lsPW.AddTail(strPW);
		}
	}
	
	
	for(i=0;i<nNum-1;i++)
	{
		strKey.Format("Name_%d",i+1);
		WritePrivateProfileString(strApp,strKey,lsUser.GetHead(),"Cipher.cip");
		lsUser.RemoveHead();
		
		strKey.Format("Cipher_%d",i+1);
		WritePrivateProfileString(strApp,strKey,lsPW.GetHead(),"Cipher.cip");
		lsPW.RemoveHead();
	}
	
	// 修改个数及最后一个
	strKey.Format("%s","Num");
	strVaule.Format("%d",nNum-1);
	WritePrivateProfileString(strApp,strKey,strVaule,"Cipher.cip");
	
	strKey.Format("Name_%d",nNum);
	WritePrivateProfileString(strApp,strKey,"","Cipher.cip");
	
	strKey.Format("Cipher_%d",nNum);
	WritePrivateProfileString(strApp,strKey,"","Cipher.cip");
	
	LoadUser();
}

void CDlgUserAdm::OnBtnDebugModify() 
{
	// TODO: Add your control notification handler code here
	CString strCurr;

	if(m_listDebug.GetCurSel()<0)
		return;

	m_listDebug.GetText(m_listDebug.GetCurSel(),strCurr);

	CDlgModifyCipher dlg;
	dlg.m_iUserType=2;
	dlg.m_iCallType=1;
	dlg.m_strUser=strCurr;
	dlg.DoModal();
}

void CDlgUserAdm::OnBtnUserAdd() 
{
	// TODO: Add your control notification handler code here
	CString strApp,strKey,strValue;
	CString strUser,strPW;
	
	CDlgAddUser dlg;
	dlg.m_iUserType=3;
	
	if(IDOK==dlg.DoModal())
	{
		strUser = dlg.m_strUser;
		strPW = dlg.m_strPassword;
		
		// 获取普通用户人数
		strApp.Format("%s","User");
		strKey.Format("%s","Num");
		
		int nNum = GetPrivateProfileInt(strApp,strKey,0,"Cipher.cip");
		
		if(nNum<0)
		{
			nNum=0;
		}
		
		// 写入信息
		strValue.Format("%d",(nNum+1));
		WritePrivateProfileString(strApp,strKey,strValue,"Cipher.cip");
		
		strKey.Format("Name_%d",nNum+1);
		strValue=strUser;
		WritePrivateProfileString(strApp,strKey,strValue,"Cipher.cip");
		
		strKey.Format("Cipher_%d",nNum+1);
		strValue=strPW;
		WritePrivateProfileString(strApp,strKey,strValue,"Cipher.cip");
	}
	
	LoadUser();
}

void CDlgUserAdm::OnBtnUserDelete() 
{
	// TODO: Add your control notification handler code here
	int i=0,j=0;
	
	CString strCurr;
	CString strApp,strKey,strName,strPW,strVaule;
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	
	if(m_listUser.GetCurSel()<0)
		return;
	
	m_listUser.GetText(m_listUser.GetCurSel(),strCurr);
	
	if(strCurr==pFrame->m_sUserName)
	{
		AfxMessageBox("不能删除当前用户!");
		return;
	}
	
	
	// 获取管理员个数
	strApp.Format("%s","User");
	strKey.Format("%s","Num");
	
	int nNum = GetPrivateProfileInt(strApp,strKey,0,"Cipher.cip");
	
	CStringList lsUser,lsPW;
	for(i=0;i<nNum;i++)
	{
		strKey.Format("Name_%d",i+1);
		GetPrivateProfileString(strApp,strKey,"",strName.GetBuffer(255),255,"Cipher.cip");
		
		strKey.Format("Cipher_%d",i+1);
		GetPrivateProfileString(strApp,strKey,"",strPW.GetBuffer(255),255,"Cipher.cip");
		
		if(strName!=strCurr)
		{
			lsUser.AddTail(strName);
			lsPW.AddTail(strPW);
		}
	}
	
	
	for(i=0;i<nNum-1;i++)
	{
		strKey.Format("Name_%d",i+1);
		WritePrivateProfileString(strApp,strKey,lsUser.GetHead(),"Cipher.cip");
		lsUser.RemoveHead();
		
		strKey.Format("Cipher_%d",i+1);
		WritePrivateProfileString(strApp,strKey,lsPW.GetHead(),"Cipher.cip");
		lsPW.RemoveHead();
	}
	
	// 修改个数及最后一个
	strKey.Format("%s","Num");
	strVaule.Format("%d",nNum-1);
	WritePrivateProfileString(strApp,strKey,strVaule,"Cipher.cip");
	
	strKey.Format("Name_%d",nNum);
	WritePrivateProfileString(strApp,strKey,"","Cipher.cip");
	
	strKey.Format("Cipher_%d",nNum);
	WritePrivateProfileString(strApp,strKey,"","Cipher.cip");
	
	LoadUser();
}

void CDlgUserAdm::OnBtnUserModify() 
{
	// TODO: Add your control notification handler code here
	CString strCurr;

	if(m_listUser.GetCurSel()<0)
		return;

	m_listUser.GetText(m_listUser.GetCurSel(),strCurr);

	CDlgModifyCipher dlg;
	dlg.m_iUserType=3;
	dlg.m_iCallType=1;
	dlg.m_strUser=strCurr;
	dlg.DoModal();
}
