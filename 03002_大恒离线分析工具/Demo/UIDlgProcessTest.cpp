// UIDlgProcessTest.cpp : implementation file
//

#include "stdafx.h"
#include "DemoProject.h"
#include "UIDlgProcessTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUIDlgProcessTest dialog


CUIDlgProcessTest::CUIDlgProcessTest(CWnd* pParent /*=NULL*/)
	: CDialog(CUIDlgProcessTest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUIDlgProcessTest)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CUIDlgProcessTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUIDlgProcessTest)
	DDX_Control(pDX, IDC_LIST_PROCESS, m_lProcessList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUIDlgProcessTest, CDialog)
	//{{AFX_MSG_MAP(CUIDlgProcessTest)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUIDlgProcessTest message handlers
