// DlgLogin.cpp : implementation file
//

#include "stdafx.h"
#include "BottleCap.h"
#include "DlgLogin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgLogin dialog


CDlgLogin::CDlgLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgLogin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgLogin)
	m_strPassword = _T("");
	m_strUser = _T("");
	//}}AFX_DATA_INIT
}


void CDlgLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgLogin)
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDV_MaxChars(pDX, m_strPassword, 6);
	DDX_Text(pDX, IDC_EDIT_USER, m_strUser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgLogin, CDialog)
	//{{AFX_MSG_MAP(CDlgLogin)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_ShowUserName, &CDlgLogin::OnBnClickedCheckShowusername)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgLogin message handlers

void CDlgLogin::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	m_iUserType = ReadConfigFile(m_strUser,m_strPassword);

	if(0==m_iUserType)
	{	
		AfxMessageBox("用户名或密码错误，请重新输入!");
		return;
	}
	CDialog::OnOK();
}

BOOL CDlgLogin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int CDlgLogin::ReadConfigFile(CString strUser, CString strPW)
{

	int i=0,j=0;

	CString strName,strCipher;
	CString strApp,strKey;
	int iAdv=0;

	// 获取管理员
	strApp.Format("%s","Adv");
	strKey.Format("%s","Num");
	iAdv = GetPrivateProfileInt(strApp,strKey,1,"Cipher.cip");
	for(i=0;i<iAdv;i++)
	{
		strKey.Format("Name_%d",i+1);
		GetPrivateProfileString(strApp,strKey,"daheng",strName.GetBuffer(255),255,"Cipher.cip");

		strKey.Format("Cipher_%d",i+1);
		GetPrivateProfileString(strApp,strKey,"cipher",strCipher.GetBuffer(255),255,"Cipher.cip");

		if(strName=="daheng" && strCipher=="cipher")
		{
			strKey.Format("%s","Num");
			WritePrivateProfileString(strApp,strKey,"1","Cipher.cip");
			
			strKey.Format("Name_%d",1);
			WritePrivateProfileString(strApp,strKey,"administrator","Cipher.cip");

			strKey.Format("Cipher_%d",1);
			WritePrivateProfileString(strApp,strKey,"daheng","Cipher.cip");

			return 1;
		}

		if(strName==strUser&&strPW==strCipher)
		{
			return 1;
		}
	}

	// 获取调试员
	strApp.Format("%s","Debugger");
	strKey.Format("%s","Num");
	iAdv = GetPrivateProfileInt(strApp,strKey,0,"Cipher.cip");
	for(i=0;i<iAdv;i++)
	{
		strKey.Format("Name_%d",i+1);
		GetPrivateProfileString(strApp,strKey,"daheng",strName.GetBuffer(255),255,"Cipher.cip");
		
		strKey.Format("Cipher_%d",i+1);
		GetPrivateProfileString(strApp,strKey,"cipher",strCipher.GetBuffer(255),255,"Cipher.cip");
		
		if(strName==strUser&&strPW==strCipher)
		{
			return 2;
		}
	}

	//获取普通用户
	strApp.Format("%s","User");
	strKey.Format("%s","Num");
	iAdv = GetPrivateProfileInt(strApp,strKey,0,"Cipher.cip");
	for(i=0;i<iAdv;i++)
	{
		strKey.Format("Name_%d",i+1);
		GetPrivateProfileString(strApp,strKey,"daheng",strName.GetBuffer(255),255,"Cipher.cip");
		
		strKey.Format("Cipher_%d",i+1);
		GetPrivateProfileString(strApp,strKey,"cipher",strCipher.GetBuffer(255),255,"Cipher.cip");
		
		if(strName==strUser&&strPW==strCipher)
		{
			return 3;
		}
	}

	return 0;
}

//获取本执行程序所在的路径
CString CDlgLogin::GetCurrentAppPath()
{
	char str[MAX_PATH], drive[MAX_PATH], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
	CString AppPath;
	GetModuleFileName(NULL, str, sizeof(str));
	_splitpath(str, drive, dir, fname, ext);
	strcat(drive, dir);
	AppPath = drive;
	return AppPath;
}
