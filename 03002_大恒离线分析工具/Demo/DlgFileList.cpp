// DlgFileList.cpp : implementation file
//

#include "stdafx.h"
#include "DemoProject.h"
#include "DlgFileList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgFileList dialog


CDlgFileList::CDlgFileList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgFileList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgFileList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgFileList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgFileList)
	DDX_Control(pDX, IDC_LIST_RESULT, m_lbFile);
	DDX_Control(pDX, IDC_LIST_DISK, m_lbDrive);
	DDX_Control(pDX, IDC_LIST_BROWSER, m_lbReport);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgFileList, CDialog)
	//{{AFX_MSG_MAP(CDlgFileList)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgFileList message handlers
