// SetCipherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DlgSetCipher.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSetCipher dialog


CDlgSetCipher::CDlgSetCipher(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSetCipher::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSetCipher)
	m_new1 = _T("");
	m_new2 = _T("");
	m_old = _T("");
	//}}AFX_DATA_INIT
}


void CDlgSetCipher::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSetCipher)
	DDX_Text(pDX, IDC_CIPHER_NEW1, m_new1);
	DDV_MaxChars(pDX, m_new1, 6);
	DDX_Text(pDX, IDC_CIPHER_NEW2, m_new2);
	DDV_MaxChars(pDX, m_new2, 6);
	DDX_Text(pDX, IDC_CIPHER_OLD, m_old);
	DDV_MaxChars(pDX, m_old, 6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSetCipher, CDialog)
	//{{AFX_MSG_MAP(CDlgSetCipher)
	ON_BN_CLICKED(IDC_BTNSETCIPHER, OnBtnsetcipher)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSetCipher message handlers
/********************************************************************
	created:	2001/09/06
	created:	6:9:2001   21:07
	author:		lfl
	Param:          
	Return:         
	
	purpose:	修改用户密码,该密码针对系统的一些高级
	           设置而设定的,他保证只要密码权限的用户可以修改这些
			   参数设置
	modify:        
*********************************************************************/

void CDlgSetCipher::OnBtnsetcipher() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	char chTemp[6];
	CString strOldCipher;
	CString str;
	CString strCipher;

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
 	strCipher.Format("%s",pFrame->m_sCipherPath1);

	GetPrivateProfileString("cipher","1","daheng",chTemp,7,strCipher);
	strOldCipher=chTemp;
	if(m_old.GetLength()!=6 || m_old!=strOldCipher)
	{
		AfxMessageBox("密码输入错误,请重新输入!!");
		return;
// 		goto END_ERR;
	}
	if(m_new2.GetLength()!=6 || m_new1.GetLength()!=6)
	{
		AfxMessageBox("新密码必须是6位!!");
// 		goto END_ERR;
		return;
	}
	if(m_new2==m_old || m_new1==m_old)
	{
		AfxMessageBox("新密码与旧密码相同!!");
		goto END_ERR;
	}
	if (m_new2!=m_new1) 
	{
		AfxMessageBox("新密码必须是相同!!");
		goto END_ERR;
	}
//	WritePrivateProfileString("cipher","1",m_new2,"cipher.cip");
	WritePrivateProfileString("cipher","1",m_new2,strCipher);
	
	str.Format("密码修改成功!\n新密码为%s",m_new2);
	AfxMessageBox(str);
END_ERR:
	m_new2 = "";
	m_new1 = "";
	m_old = "";
	UpdateData(false);

	CDialog::OnOK();
}
