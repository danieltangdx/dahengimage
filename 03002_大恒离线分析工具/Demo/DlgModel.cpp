// DlgModel.cpp : implementation file
//

#include "stdafx.h"
#include "DemoProject.h"
#include "DlgModel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgModel dialog


CDlgModel::CDlgModel(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgModel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgModel)
	m_strNewModelName = _T("");
	//}}AFX_DATA_INIT
}


void CDlgModel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgModel)
	DDX_Text(pDX, IDC_EDIT1, m_strNewModelName);
	DDV_MaxChars(pDX, m_strNewModelName, 254);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgModel, CDialog)
	//{{AFX_MSG_MAP(CDlgModel)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgModel message handlers
