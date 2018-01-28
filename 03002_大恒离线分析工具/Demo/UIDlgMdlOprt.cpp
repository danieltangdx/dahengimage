// UIDlgMdlOprt.cpp : implementation file
//

#include "stdafx.h"
#include "DemoProject.h"
#include "UIDlgMdlOprt.h"
#include "DemoProjectDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUIDlgMdlOprt dialog


CUIDlgMdlOprt::CUIDlgMdlOprt(CWnd* pParent /*=NULL*/)
	: CDialog(CUIDlgMdlOprt::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUIDlgMdlOprt)
	m_strModelNameUsing = _T("");
	//}}AFX_DATA_INIT
}


void CUIDlgMdlOprt::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUIDlgMdlOprt)
	DDX_Control(pDX, IDC_MODELLIST, m_lbModels);
	DDX_Text(pDX, IDC_EDIT1, m_strModelNameUsing);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUIDlgMdlOprt, CDialog)
	//{{AFX_MSG_MAP(CUIDlgMdlOprt)
	ON_BN_CLICKED(IDC_NEWMODEL, OnNewModel)
	ON_LBN_SELCHANGE(IDC_MODELLIST, OnSelchangeModellist)
	ON_BN_CLICKED(IDC_BTN_SAVEMODEL, OnBtnSaveModel)
	ON_BN_CLICKED(IDC_BTNREADMODEL, OnBtnReadModel)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUIDlgMdlOprt message handlers

BOOL CUIDlgMdlOprt::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CFileFind finder;	
	BOOL Ret = FALSE;	 
	Ret = finder.FindFile(((CDemoProjectDlg*)m_pDlgMainWindow)->strAllModelPath + "\\*.*", 0);
	while(Ret)
	{
		Ret = finder.FindNextFile();
		if (finder.IsDirectory())
		{
			CString strDir;
			if (finder.GetFileName() == "." || finder.GetFileName() == "..")
			{
				continue;
			}
			m_lbModels.AddString(finder.GetFileName());
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CUIDlgMdlOprt::OnNewModel() 
{
	// TODO: Add your control notification handler code here
	if (((CDemoProjectDlg*)m_pDlgMainWindow)->m_bNewInProcess == true)
	{
		CString strMsg;
		strMsg.Format("正在创建模板\"%s\",请先进行完。",m_strModelNameUsing);
		AfxMessageBox(strMsg);
		GetDlgItem(IDC_EDIT1)->SetWindowText(m_strModelNameUsing);
		return;
	}
	((CDemoProjectDlg*)m_pDlgMainWindow)->m_bNewInProcess = true;
	UpdateData(TRUE);
	//检测模板名称是否符合要求
	if (((CDemoProjectDlg*)m_pDlgMainWindow)->m_strModelNameUsing == "")
	{
		AfxMessageBox("模板名称不能为空");
		((CDemoProjectDlg*)m_pDlgMainWindow)->m_bNewInProcess = false;
		return;
	}
	
	//检测模板是否已存在
	CFileFind finder;	
	BOOL Ret = FALSE;	 
	Ret = finder.FindFile(((CDemoProjectDlg*)m_pDlgMainWindow)->strAllModelPath + "\\*.*", 0);
	while(Ret)
	{
		Ret = finder.FindNextFile();
		if (finder.IsDirectory())
		{
			CString strDir;
			if (finder.GetFileName() == "." || finder.GetFileName() == "..")
			{
				continue;
			}
			if (finder.GetFileName() == m_strModelNameUsing)
			{
				AfxMessageBox("模板已经存在，请输入其它名称。");
				((CDemoProjectDlg*)m_pDlgMainWindow)->m_bNewInProcess = false;
				return;
			}
		}
	}	
	
	((CDemoProjectDlg*)m_pDlgMainWindow)->CreateTabWins(1);
	((CDemoProjectDlg*)m_pDlgMainWindow)->ShowTabWins(1);
	((CDemoProjectDlg*)m_pDlgMainWindow)->m_Tab.SetCurSel(0);

	return;
}

void CUIDlgMdlOprt::OnSelchangeModellist() 
{
	// TODO: Add your control notification handler code here
	CString filename;
	m_lbModels.GetText(m_lbModels.GetCaretIndex(), filename);
	if (filename.Mid(0,6) != " [DIR]")
	{
		s_AlgModelInput sIntop;
		CRect rect;
		sIntop.nOperSig=2;
		strcpy(sIntop.chModelName,filename);
		((CDemoProjectDlg*)m_pDlgMainWindow)->m_strModelNameUsing = filename;
		((CDemoProjectDlg*)m_pDlgMainWindow)->CreateTabWins((((CDemoProjectDlg*)m_pDlgMainWindow)->m_Tab).GetCurSel()+1);
		((CDemoProjectDlg*)m_pDlgMainWindow)->ShowTabWins((((CDemoProjectDlg*)m_pDlgMainWindow)->m_Tab).GetCurSel()+1);
		CString strWinTitle;
		strWinTitle.Format("%s-修改模板\"%s\"",((CDemoProjectDlg*)m_pDlgMainWindow)->m_strProgramName,((CDemoProjectDlg*)m_pDlgMainWindow)->m_strModelNameUsing);
		((CDemoProjectDlg*)m_pDlgMainWindow)->SetWindowText(strWinTitle);
		
	}
}

void CUIDlgMdlOprt::RefreshModelList()
{
	m_lbModels.ResetContent();
	CFileFind finder;	
	BOOL Ret = FALSE;	 
	Ret = finder.FindFile(((CDemoProjectDlg*)m_pDlgMainWindow)->strAllModelPath + "\\*.*", 0);
	while(Ret)
	{
		Ret = finder.FindNextFile();
		if (finder.IsDirectory())
		{
			CString strDir;
			if (finder.GetFileName() == "." || finder.GetFileName() == "..")
			{
				continue;
			}
			m_lbModels.AddString(finder.GetFileName());
		}
	}
}

void CUIDlgMdlOprt::OnBtnSaveModel() 
{
	// TODO: Add your control notification handler code here
	
	s_AlgModelInput sModelIn;
	sModelIn.nOperSig = 4;
	strcpy(sModelIn.chModelName,((CDemoProjectDlg*)m_pDlgMainWindow)->m_strModelNameUsing);
	s_Status sRst = ((CDemoProjectDlg*)m_pDlgMainWindow)->topcheck1->SetModel(sModelIn);
	sRst = ((CDemoProjectDlg*)m_pDlgMainWindow)->topcheck2->SetModel(sModelIn);
	sRst = ((CDemoProjectDlg*)m_pDlgMainWindow)->topcheck3->SetModel(sModelIn);
	sRst = ((CDemoProjectDlg*)m_pDlgMainWindow)->topcheck4->SetModel(sModelIn);
	sRst = ((CDemoProjectDlg*)m_pDlgMainWindow)->topcheck5->SetModel(sModelIn);
	
	sModelIn.nOperSig = 5;
	sRst = ((CDemoProjectDlg*)m_pDlgMainWindow)->topcheck1->SetModel(sModelIn);
	sRst = ((CDemoProjectDlg*)m_pDlgMainWindow)->topcheck2->SetModel(sModelIn);
	sRst = ((CDemoProjectDlg*)m_pDlgMainWindow)->topcheck3->SetModel(sModelIn);
	sRst = ((CDemoProjectDlg*)m_pDlgMainWindow)->topcheck4->SetModel(sModelIn);
	sRst = ((CDemoProjectDlg*)m_pDlgMainWindow)->topcheck5->SetModel(sModelIn);
	AfxMessageBox("保存成功");
	((CDemoProjectDlg*)m_pDlgMainWindow)->m_bNewInProcess = false;
	
	CFileFind finder;	
	BOOL Ret = FALSE;	 
	Ret = finder.FindFile(((CDemoProjectDlg*)m_pDlgMainWindow)->strAllModelPath + "\\*.*", 0);
	m_lbModels.ResetContent();
	while(Ret)
	{
		Ret = finder.FindNextFile();
		if (finder.IsDirectory())
		{
			CString strDir;
			if (finder.GetFileName() == "." || finder.GetFileName() == "..")
			{
				continue;
			}
			m_lbModels.AddString(finder.GetFileName());
		}
	}
	LRESULT copyDataResult;	
	CWnd *pOtherWnd = CWnd::FindWindow(NULL, ((CDemoProjectDlg*)m_pDlgMainWindow)->m_strSystemName.GetBuffer(0));
	if (pOtherWnd)
	{
		COPYDATASTRUCT cpd;
		cpd.dwData = DEMO_APPLY;
		cpd.cbData = 0;
		cpd.lpData = NULL;
		
		copyDataResult = pOtherWnd->SendMessage(WM_COPYDATA,
												(WPARAM)AfxGetApp()->m_pMainWnd->GetSafeHwnd(),
												(LPARAM)&cpd);
	}	
}

void CUIDlgMdlOprt::OnBtnReadModel() 
{
	// TODO: Add your control notification handler code here
	LRESULT copyDataResult;	
	CWnd *pOtherWnd = CWnd::FindWindow(NULL, ((CDemoProjectDlg*)m_pDlgMainWindow)->m_strSystemName.GetBuffer(0));
	if (pOtherWnd) {
		COPYDATASTRUCT cpd;
		cpd.dwData = DEMO_CHANGE;//告诉系统读取模板
		cpd.cbData = ((CDemoProjectDlg*)m_pDlgMainWindow)->m_strModelNameUsing.GetLength();
		cpd.lpData = ((CDemoProjectDlg*)m_pDlgMainWindow)->m_strModelNameUsing.GetBufferSetLength(((CDemoProjectDlg*)m_pDlgMainWindow)->m_strModelNameUsing.GetLength());
		copyDataResult = pOtherWnd->SendMessage(WM_COPYDATA,
												(WPARAM)AfxGetApp()->m_pMainWnd->GetSafeHwnd(),
												(LPARAM)&cpd);
	}	
}

void CUIDlgMdlOprt::OnOK() 
{
	// TODO: Add extra validation here
//	((CDemoProjectDlg*)m_pDlgMainWindow)->OnOK();
	((CDemoProjectDlg*)m_pDlgMainWindow)->m_bShowSide = FALSE;
	this->ShowWindow(SW_HIDE);
	CDialog::OnOK();
}

void CUIDlgMdlOprt::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if   (IsIconic())
	{
		;
	}
	else
	{
		CRect   rect;
		CPaintDC   dc(this);
		GetClientRect(rect);
		dc.FillSolidRect(rect,RGB(0,0,0));   //设置为绿色背景
	}
CDialog::OnPaint();
	// Do not call CDialog::OnPaint() for painting messages
}
