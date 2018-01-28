// DlgInspSet.cpp : implementation file
//

#include "stdafx.h"
#include "DemoProject.h"
#include "DlgInspSet.h"
#include "DlgInspNumWS.h"
#include "DemoProjectDlg.h"
#include "GeneralFun_Yang.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgInspSet dialog
extern HMODULE g_hLanguage;
extern pFunc_GetTextFromLPK g_pGetTextFromLPK;
extern CString g_strAppDir;

CDlgInspSet::CDlgInspSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgInspSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgInspSet)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bPainted = FALSE;
	m_nPreNumWS = -1;
}


void CDlgInspSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgInspSet)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgInspSet, CDialog)
	//{{AFX_MSG_MAP(CDlgInspSet)
	ON_WM_PAINT()
	ON_COMMAND_RANGE(1001,1999,OnButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgInspSet message handlers

void CDlgInspSet::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	if (!m_bPainted)
	{
		m_bPainted = TRUE;

		CRect rect1, rect2, rectW;
		GetDlgItem(IDC_STATIC_1)->GetWindowRect(rect1);
		this->GetWindowRect(rectW);
		ClientToScreen(rectW);
		int nRowDiff = 1.38235294117647 * rect1.Height() + 0.5;
		int nColDiff = 1.2947368421 * rect1.Width() + 0.5;
		int nWndBottom = rectW.bottom - rect1.bottom;
		ScreenToClient(rect1);

		char chText[256];
		for (int n=0; n<m_nPreNumWS; ++n)
		{
			int nRow = n/4;
			int nCol = n%4;

			CRect rect;
			rect.top = rect1.top + nRow*nRowDiff;
			rect.bottom = rect.top + rect1.Height();
			rect.left = rect1.left + nCol*nColDiff;
			rect.right = rect.left + rect1.Width();
			
			CString strDllName;
			if(!TryToGetDllNameByIndex(n, strDllName))
			{
				LoadString(g_hLanguage, 5053, chText, 256);
				strDllName = chText;
			}

			CButton* pButton = new CButton;
			pButton->Create(_T(strDllName.GetBuffer(0)), WS_CHILD|BS_MULTILINE, rect, this, 1000+n+1);
			pButton->ShowWindow(SW_SHOW);
		}

		CRect rect;
		int nRow = (m_nPreNumWS)/4;
		int nCol = (m_nPreNumWS)%4;
		rect.top = rect1.top + nRow*nRowDiff;
		rect.bottom = rect.top + rect1.Height();
		rect.left = rect1.left + nCol*nColDiff;
		rect.right = rect.left + rect1.Width();
		GetDlgItem(IDOK)->MoveWindow(rect);

		nRow = (m_nPreNumWS+1)/4;
		nCol = (m_nPreNumWS+1)%4;
		rect.top = rect1.top + nRow*nRowDiff;
		rect.bottom = rect.top + rect1.Height();
		rect.left = rect1.left + nCol*nColDiff;
		rect.right = rect.left + rect1.Width();
		GetDlgItem(IDCANCEL)->MoveWindow(rect);

		CRect rect3;
		GetDlgItem(IDCANCEL)->GetWindowRect(rect3);
		rectW.bottom = rect3.bottom + nWndBottom;
		MoveWindow(rectW);
	}

	
	// Do not call CDialog::OnPaint() for painting messages
}

BOOL CDlgInspSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CDlgInspNumWS dlg;
	if (m_nPreNumWS > 0)
		dlg.m_nNumWS = m_nPreNumWS;
	if (IDOK == dlg.DoModal())
	{
		m_nPreNumWS = dlg.m_nNumWS;
	}
	else
	{
		CDialog::OnCancel();
		return TRUE;
	}
	
	char chText[256];
	LoadString(g_hLanguage, 5045, chText, 256);
	GetDlgItem(IDCANCEL)->SetWindowText(chText);
	LoadString(g_hLanguage, 5054, chText, 256);
	GetDlgItem(IDOK)->SetWindowText(chText);


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDlgInspSet::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CDlgInspSet::TryToGetDllNameByIndex(int nIndex, CString& strDllName)
{
	CString strIniPath = g_strAppDir + "\\Config\\AlgConfig.ini";
	CString strKeyName;
	strKeyName.Format("CheckDllName_%d", nIndex+1);
	
	char chBuf[256];
	GetPrivateProfileString("Inspection Parameters", strKeyName, "NULL", chBuf, 256, strIniPath.GetBuffer(0));

	if (strcmp(chBuf, "NULL") == 0)
		return FALSE;

	strDllName = chBuf;
	if (strDllName.Right(4)==".dll" || strDllName.Right(4)==".DLL")
		strDllName = strDllName.Left(strDllName.GetLength()-4);

	return TRUE;
}

void CDlgInspSet::OnOK() 
{
	// TODO: Add extra validation here
	int nProbIndex;
	if (CheckAllValid(nProbIndex))
	{
		CString strIniPath = g_strAppDir + "\\Config\\AlgConfig.ini";
		CGeneralFunc_Yang::WritePrivateProfileInt(m_nPreNumWS, "Inspection Parameters", "CheckNum", strIniPath.GetBuffer(0));
		for (int n=0; n<m_nPreNumWS; ++n)
		{
			CString strDllName;
			GetDlgItem(1000+n+1)->GetWindowText(strDllName);
			CString strKeyName;
			strKeyName.Format("CheckDllName_%d", n+1);
			WritePrivateProfileString("Inspection Parameters", strKeyName, strDllName, strIniPath.GetBuffer(0));
		}
	}
	else
	{
		char chText[256];
		g_pGetTextFromLPK(chText, 31, nProbIndex+1);
		MessageBox(chText, "DEMO", MB_ICONERROR);
		return;
	}
	
	CDialog::OnOK();
}

BOOL CDlgInspSet::CheckAllValid(int& nProbIndex)
{
	nProbIndex = -1;
	for (int n=0; n<m_nPreNumWS; ++n)
	{
		CString strDllName;
		GetDlgItem(1000+n+1)->GetWindowText(strDllName);
		strDllName = g_strAppDir + "\\" + strDllName + ".dll";
		HMODULE hDll = LoadLibrary(strDllName.GetBuffer(0));
		if (hDll)
		{
			pFunc_Create createobj;
			createobj = (pFunc_Create)(GetProcAddress(hDll, "create_instance"));
			if (createobj)
			{
				pFunc_Destroy destroyobj;
				destroyobj = (pFunc_Destroy)(GetProcAddress(hDll, "destroy_instance"));
				if (!destroyobj)
				{
					nProbIndex = n;
					return FALSE;
				}
			}
			else
			{
				nProbIndex = n;
				return FALSE;
			}
		}
		else
		{
			nProbIndex = n;
			return FALSE;
		}
	}
	return TRUE;
}

void CDlgInspSet::OnButton(UINT uID)
{
	CFileDialog file(TRUE, "dll", NULL, OFN_HIDEREADONLY, "Dynamic Link Libraries Files (*.dll)|*.dll;*.DLL|ALL Files (*.*)|*.*||", NULL);
	file.m_ofn.lpstrInitialDir = g_strAppDir.GetBuffer(0);

	if (file.DoModal() == IDOK)
	{
		CString strName = file.GetPathName();
		int nF1 = strName.ReverseFind('\\');
		strName = strName.Mid(nF1+1, strName.GetLength()-nF1-1-4);
		GetDlgItem(uID)->SetWindowText(strName);
	}
}