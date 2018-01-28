// DlgAddUser.cpp : implementation file
//

#include "stdafx.h"
#include "BottleCap.h"
#include "DlgAddUser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAddUser dialog


CDlgAddUser::CDlgAddUser(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddUser::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAddUser)
	m_strPassword = _T("");
	m_strUser = _T("");
	//}}AFX_DATA_INIT

	m_iUserType=0;
}


void CDlgAddUser::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAddUser)
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDV_MaxChars(pDX, m_strPassword, 6);
	DDX_Text(pDX, IDC_EDIT_USER, m_strUser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAddUser, CDialog)
	//{{AFX_MSG_MAP(CDlgAddUser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAddUser message handlers

void CDlgAddUser::OnOK() 
{
	// TODO: Add extra validation here
	int i=0;
	UpdateData(TRUE);
	if(m_strUser.IsEmpty())
	{
		AfxMessageBox("用户名称不能为空!");
		return;
	}

	if(m_strPassword.IsEmpty())
	{
		AfxMessageBox("密码不能为空");
		return;
	}

	CString strApp,strKey,strValue;

	// 获取管理员人数
	if(1==m_iUserType)
	{
		strApp.Format("%s","Adv");
	}
	else if(2==m_iUserType)
	{
		strApp.Format("%s","Debugger");
	}
	else if(3==m_iUserType)
	{
		strApp.Format("%s","User");
	}
	else
	{
		AfxMessageBox("用户类型不合法，请确认后再添加!");
		return;
	}

	strKey.Format("%s","Num");
	
	int nNum = GetPrivateProfileInt(strApp,strKey,0,"Cipher.cip");
	
	if(nNum<1)
	{
		nNum=1;
	}
	
	for(i=0;i<nNum;i++)
	{
		strKey.Format("Name_%d",i+1);
		GetPrivateProfileString(strApp,strKey,"",strValue.GetBuffer(255),255,"Cipher.cip");
		
		if(strValue==m_strUser)
		{
			AfxMessageBox("改用户名已经存在，请重新输入!");
			return;
		}
	}
	
	CDialog::OnOK();
}

BOOL CDlgAddUser::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(1==m_iUserType)
	{
		SetWindowText("添加管理员");
	}
	else if(2==m_iUserType)
	{
		SetWindowText("添加调试员");
	}
	else if(3==m_iUserType)
	{
		SetWindowText("添加普通用户");
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
