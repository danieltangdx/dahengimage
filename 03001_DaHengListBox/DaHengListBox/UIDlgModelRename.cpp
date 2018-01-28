// UIDlgModelRename.cpp : implementation file
//

#include "stdafx.h"
#include "DaHengListBox.h"
#include "UIDlgModelRename.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUIDlgModelRename dialog


CUIDlgModelRename::CUIDlgModelRename(CWnd* pParent /*=NULL*/)
	: CDialog(CUIDlgModelRename::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUIDlgModelRename)
	m_strNewName = _T("");
	m_strMsg = _T("");
	//}}AFX_DATA_INIT
}


void CUIDlgModelRename::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUIDlgModelRename)
	DDX_Text(pDX, IDC_EDIT_NEWNAME, m_strNewName);
	DDV_MaxChars(pDX, m_strNewName, 255);
	DDX_Text(pDX, IDC_STATIC_MSG, m_strMsg);
	DDV_MaxChars(pDX, m_strMsg, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUIDlgModelRename, CDialog)
	//{{AFX_MSG_MAP(CUIDlgModelRename)
	ON_BN_CLICKED(IDC_BTN_OK, OnBtnOk)
	ON_BN_CLICKED(IDC_BTN_NOK, OnBtnNok)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUIDlgModelRename message handlers

void CUIDlgModelRename::OnBtnOk() 
{
	UpdateData(TRUE);
	EndDialog(IDOK);
}

void CUIDlgModelRename::OnBtnNok() 
{
	EndDialog(IDCANCEL);
}

BOOL CUIDlgModelRename::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
