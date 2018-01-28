// TubeCheckDoc.cpp : implementation of the CTubeCheckDoc class
//

#include "stdafx.h"
#include "TubeCheck.h"
#include "TubeCheckDoc.h"
#include "MainFrm.h"
#include "TubeCheckView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CMainFrame*	g_pFrame; //È«¾ÖÖ¸Õë
extern CTubeCheckView*	g_pView;


/////////////////////////////////////////////////////////////////////////////
// CTubeCheckDoc

IMPLEMENT_DYNCREATE(CTubeCheckDoc, CDocument)

BEGIN_MESSAGE_MAP(CTubeCheckDoc, CDocument)
	//{{AFX_MSG_MAP(CTubeCheckDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTubeCheckDoc construction/destruction

CTubeCheckDoc::CTubeCheckDoc()
{
	// TODO: add one-time construction code here

}

CTubeCheckDoc::~CTubeCheckDoc()
{
}

BOOL CTubeCheckDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CTubeCheckDoc serialization

void CTubeCheckDoc::Serialize(CArchive& ar)
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
// CTubeCheckDoc diagnostics

#ifdef _DEBUG
void CTubeCheckDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CTubeCheckDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTubeCheckDoc commands
