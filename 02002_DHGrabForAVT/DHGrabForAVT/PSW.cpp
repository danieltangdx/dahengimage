// PSW.cpp : implementation file
//

#include "stdafx.h"
#include "avtgrabber.h"
#include "PSW.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PSW dialog


PSW::PSW(CString str,CWnd* pParent /*=NULL*/)
	: CDialog(PSW::IDD, pParent)
{
	//{{AFX_DATA_INIT(PSW)
	m_sOldPsw = _T("");
	m_sNewPsw = _T("");
	//}}AFX_DATA_INIT
	m_sIniFile = str;
}


void PSW::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(PSW)
	DDX_Text(pDX, IDC_EDIT_OLDPSW, m_sOldPsw);
	DDX_Text(pDX, IDC_EDIT_NEWPSW, m_sNewPsw);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(PSW, CDialog)
	//{{AFX_MSG_MAP(PSW)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PSW message handlers

void PSW::OnOK() 
{
	UpdateData(TRUE);
	char cc[100];

	if (m_sOldPsw == _T(""))
	{
		AfxMessageBox(_T("旧密码不能为空！"));
	}
	else
	{
		if (m_sOldPsw.GetLength() > 6)
		{
			AfxMessageBox(_T("旧密码过长!"));
		}
		else
		{
			CString sTemp,sTemp1,sTemp2;
			GetPrivateProfileString(_T("相机参数"),_T("Psw"),_T(""),sTemp.GetBuffer(100),100,m_sIniFile);
			sTemp1.Format(_T("%s"),sTemp);
			
			int n1 = sTemp1.GetLength();
			
			for (int j = 0; j <n1; j++)
			{
				cc[j] = sTemp1.GetAt(j);
				cc[j] +=8;//解密
			}
			
			cc[n1] = '\0';
			sTemp1.Format(_T("%s"),cc);
			sTemp2.Format(_T("%s\0"),m_sOldPsw);

			if (sTemp1 == sTemp2)
			{
				if (m_sNewPsw == _T(""))
				{
					AfxMessageBox(_T("新密码不能为空！"));
				}
				else
				{
					if (m_sNewPsw.GetLength() > 6)
					{
						AfxMessageBox(_T("新密码过长!"));
					}
					else
					{
						int n2 = m_sNewPsw.GetLength();
						for (int j = 0; j <n2; j++)
						{
							cc[j] = m_sNewPsw.GetAt(j);
							if (isalnum(cc[j])  == 0)
							{
								AfxMessageBox(_T("请输入字母或数字!"));
								return;
							}
							else
							{
								cc[j] -=8;//加密
							}
						}
						
						cc[n2] = '\0';
						
						m_sNewPsw.Format(_T("%s"),cc);
						WritePrivateProfileString(_T("相机参数"), _T("Psw"), m_sNewPsw, m_sIniFile);

					}//
				}
			}
			else
			{
				AfxMessageBox(_T("旧密码错误!"));
			}
		}
	}

	
	CDialog::OnOK();
}
