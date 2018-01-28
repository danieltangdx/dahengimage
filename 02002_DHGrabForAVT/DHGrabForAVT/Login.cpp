// Login.cpp : implementation file
//

#include "stdafx.h"
#include "avtgrabber.h"
#include "Login.h"
#include "PSW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Login dialog


Login::Login(CString sIniFile,CWnd* pParent /*=NULL*/)
	: CDialog(Login::IDD, pParent)
{
	//{{AFX_DATA_INIT(Login)
	m_sPsw = _T("");
	//}}AFX_DATA_INIT
	m_bLogin = FALSE;
	m_sIniFile = sIniFile;
}


void Login::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Login)
	DDX_Text(pDX, IDC_EDIT_PSW, m_sPsw);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Login, CDialog)
	//{{AFX_MSG_MAP(Login)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_NEWPSW, OnButtonNewpsw)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Login message handlers

void Login::OnButtonApply() 
{
	// TODO: Add your control notification handler code here

}

void Login::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);

	if (m_sPsw == _T(""))
	{
		AfxMessageBox(_T("密码不能为空！"));
	}
	else
	{
		if (m_sPsw.GetLength() > 6)
		{
			AfxMessageBox(_T("密码过长!"));
		}
		else
		{
			CString sTemp,sTemp1,sTemp2;
			GetPrivateProfileString(_T("相机参数"),_T("Psw"),_T(""),sTemp.GetBuffer(100),100,m_sIniFile);
			if (sTemp == _T(""))
			{
				sTemp.Format(_T(")*+"));
				WritePrivateProfileString(_T("相机参数"), _T("Psw"), sTemp, m_sIniFile);
			}
			
			sTemp1.Format(_T("%s"),sTemp);

			int n1 = sTemp1.GetLength();
			char cc[100];
			for (int j = 0; j <n1; j++)
			{
				cc[j] = sTemp1.GetAt(j);
				cc[j] +=8;//解密
			}
			
			cc[n1] = '\0';
			sTemp1.Format(_T("%s"),cc);

			sTemp2.Format(_T("%s\0"),m_sPsw);
			if (sTemp1 == sTemp2)
			{
				m_bLogin = TRUE;
			}
			else
			{
				AfxMessageBox(_T("密码错误!"));
			}
		}
	}

	CDialog::OnOK();
}

void Login::OnButtonNewpsw() 
{
	// TODO: Add your control notification handler code here
	PSW pswdlg(m_sIniFile);
	pswdlg.DoModal();
	
}
