// BottleCapDoc.cpp : implementation of the CBottleCapDoc class
//

#include "stdafx.h"
#include "BottleCap.h"
#include "BottleCapDoc.h"
#include "MainFrm.h"
#include "BottleCapView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainFrame*	g_pFrame; //È«¾ÖÖ¸Õë
extern CBottleCapView*	g_pView;


/////////////////////////////////////////////////////////////////////////////
// CBottleCapDoc

IMPLEMENT_DYNCREATE(CBottleCapDoc, CDocument)

BEGIN_MESSAGE_MAP(CBottleCapDoc, CDocument)
	//{{AFX_MSG_MAP(CBottleCapDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBottleCapDoc construction/destruction

CBottleCapDoc::CBottleCapDoc()
{
	// TODO: add one-time construction code here

}

CBottleCapDoc::~CBottleCapDoc()
{
}

BOOL CBottleCapDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CBottleCapDoc serialization

void CBottleCapDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBottleCapDoc diagnostics

#ifdef _DEBUG
void CBottleCapDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBottleCapDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBottleCapDoc commands
