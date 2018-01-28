// DlgModifyCipher.cpp : implementation file
//

#include "stdafx.h"
#include "BottleCap.h"
#include "DlgModifyCipher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgModifyCipher dialog


CDlgModifyCipher::CDlgModifyCipher(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgModifyCipher::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgModifyCipher)
	m_strUser = _T("");
	m_strOldPW = _T("");
	m_strNewPW = _T("");
	//}}AFX_DATA_INIT

	m_iUserType=0;
	m_iCallType=0;
}


void CDlgModifyCipher::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgModifyCipher)
	DDX_Text(pDX, IDC_EDIT_USER, m_strUser);
	DDX_Text(pDX, IDC_EDIT_OLD_PASSWORD, m_strOldPW);
	DDV_MaxChars(pDX, m_strOldPW, 6);
	DDX_Text(pDX, IDC_EDIT_NEW_PASSWORD, m_strNewPW);
	DDV_MaxChars(pDX, m_strNewPW, 6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgModifyCipher, CDialog)
	//{{AFX_MSG_MAP(CDlgModifyCipher)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgModifyCipher message handlers

void CDlgModifyCipher::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	int i=0,iNum=0;
	CString strApp,strKey,strUser,strPW,strValue;

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
		AfxMessageBox("用户类型不正确，请确认后再修改!");
		return;
	}

	strKey.Format("%s","Num");
	iNum = GetPrivateProfileInt(strApp,strKey,0,"Cipher.cip");
	for(i=0;i<iNum;i++)
	{
		strKey.Format("Name_%d",i+1);
		GetPrivateProfileString(strApp,strKey,"",strUser.GetBuffer(255),255,"Cipher.cip");

		strKey.Format("Cipher_%d",i+1);
		GetPrivateProfileString(strApp,strKey,"",strPW.GetBuffer(255),255,"Cipher.cip");

		if(strUser==m_strUser)
		{
			break;
		}
	}

	if(strPW!=m_strOldPW)
	{
		AfxMessageBox("旧密码不正确，请重新输入!");
		return;
	}

	strKey.Format("Cipher_%d",i+1);
	WritePrivateProfileString(strApp,strKey,m_strNewPW,"Cipher.cip");

	AfxMessageBox("修改密码成功!");
	
	CDialog::OnOK();
}

BOOL CDlgModifyCipher::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int i=0,iNum=0;
	CString strApp,strKey,strUser,strPW,strValue;
	
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
		AfxMessageBox("用户类型不正确，请确认后再修改!");
		return FALSE;
	}
	
	strKey.Format("%s","Num");
	iNum = GetPrivateProfileInt(strApp,strKey,0,"Cipher.cip");
	for(i=0;i<iNum;i++)
	{
		strKey.Format("Name_%d",i+1);
		GetPrivateProfileString(strApp,strKey,"",strUser.GetBuffer(255),255,"Cipher.cip");
		
		strKey.Format("Cipher_%d",i+1);
		GetPrivateProfileString(strApp,strKey,"",strPW.GetBuffer(255),255,"Cipher.cip");
		
		if(strUser==m_strUser)
		{
			break;
		}
	}

	if(m_iCallType==1)
	{
		m_strOldPW=strPW;
	}

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
