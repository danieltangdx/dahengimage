// TestCipherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DlgTestCipher.h"
#include "resource.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTestCipher dialog


CDlgTestCipher::CDlgTestCipher(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTestCipher::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTestCipher)
	m_cipher = _T("");
	//}}AFX_DATA_INIT
	m_itimes	= 1;
	m_bOK		= false;
}


void CDlgTestCipher::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTestCipher)
	DDX_Text(pDX, IDC_CIPHER_TEXT, m_cipher);
	DDV_MaxChars(pDX, m_cipher, 6);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTestCipher, CDialog)
	//{{AFX_MSG_MAP(CDlgTestCipher)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTestCipher message handlers
/********************************************************************
	created:	2001/09/06
	created:	6:9:2001   22:07
	author:		lfl
	Param:          
	Return:         
	
	purpose:	确认密码是否正确,超出三次错误,程序结束
	modify:        
*********************************************************************/

void CDlgTestCipher::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(true);
	char s[6];
	CString cipher;
	CString str;
//	GetPrivateProfileString("cipher","1","111111",s,7,"cipher.cip");


	CString strCipher;
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	strCipher.Format("%s",pFrame->m_sCipherPath1);
// 	GetPrivateProfileString("cipher","1","111111",s,7,strCipher);
	GetPrivateProfileString("cipher","1","daheng",s,7,strCipher);

	cipher=s;

// 	if(m_cipher=="lfl"|| m_cipher=="dh")
// 	{
// 		m_bOK=true;
// 		CDialog::OnOK();
// 		return;
// 	}
	
	if(m_cipher!=cipher)
	{
		m_itimes++;
		if (m_itimes>3) 
		{
			AfxMessageBox("您已经三次输入错误的密码!!\n 请重新确认密码是否正确.");
			CDialog::OnOK();
			return;
		}
		AfxMessageBox("输入的密码错误,请重新输入.");
		m_cipher="";
		UpdateData(false);
		return;
	}
	else
	{
		m_bOK=true;
	}
	CDialog::OnOK();
}

BOOL CDlgTestCipher::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_cipher="";
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
