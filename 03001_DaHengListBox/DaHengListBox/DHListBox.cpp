// CCCCC.cpp: implementation of the CCCCCC class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DaHengListBox.h"
#include "DHListBox.h"
#include <io.h>
#include <dbt.h>
#include "UIDlgModelRename.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CDaHengListBoxApp theApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDHListBox::CDHListBox()
{
	m_nRightItem = -1;
	m_nLanguage = 1;
}

CDHListBox::~CDHListBox()
{

}

BEGIN_MESSAGE_MAP(CDHListBox, CListBox)
	//{{AFX_MSG_MAP(CDHListBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

int CDHListBox::AddString(LPCTSTR lpszItem)
{
	int iRet = CListBox::AddString(lpszItem);
	RefushHorizontalScrollBar();
	if (iRet >= 0)
		SetItemData(iRet, -1);
	return iRet;
}

int CDHListBox::AddString(LPCTSTR lpszItem, int iImg)
{
	int iRet = CListBox::AddString(lpszItem);
	RefushHorizontalScrollBar();
	if (iRet >= 0)
		SetItemData(iRet, iImg);
	return iRet;
}

int CDHListBox::InsertString(int iIndex, LPCTSTR lpszItem)
{
	int iRet = CListBox::InsertString(iIndex,lpszItem);
	RefushHorizontalScrollBar();
	if (iRet >= 0)
		SetItemData(iRet, -1);
	return iRet;
}

void CDHListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

	if ((int)lpDrawItemStruct->itemID < 0)
	{
		// If there are no elements in the List Box 
		// based on whether the list box has Focus or not 
		// draw the Focus Rect or Erase it,
		if ((lpDrawItemStruct->itemAction & ODA_FOCUS) && 
			(lpDrawItemStruct->itemState & ODS_FOCUS))
		{
			pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
		}
		else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&	
			!(lpDrawItemStruct->itemState & ODS_FOCUS)) 
		{
			pDC->DrawFocusRect(&lpDrawItemStruct->rcItem); 
		}
		return;
	}

    CRect  rcItem(lpDrawItemStruct->rcItem); // To draw the focus rect.
    CRect  rClient(rcItem); // Rect to highlight the Item
    CRect  rText(rcItem); // Rect To display the Text
    CPoint Pt( rcItem.left , rcItem.top ); // Point To draw the Image

	// if the Image list exists for the list box
	// adjust the Rect sizes to accomodate the Image for each item.
	if(m_pImageList)
	{
		rClient.left += 18;
		rText.left += 18;
//		rText.top += 2;
	}
	else
	{
		rText.top += 2;
	}


	COLORREF crText;
	CString strText;

	// Image information in the item data.
	int iImg = (int)lpDrawItemStruct->itemData;

	// If item selected, draw the highlight rectangle.
	// Or if item deselected, draw the rectangle using the window color.
	if ((lpDrawItemStruct->itemState & ODS_SELECTED) &&
		 (lpDrawItemStruct->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		CBrush br(::GetSysColor(COLOR_HIGHLIGHT));
		pDC->FillRect(&rClient, &br);
	}
	else if (!(lpDrawItemStruct->itemState & ODS_SELECTED) && 
		(lpDrawItemStruct->itemAction & ODA_SELECT)) 
	{
		CBrush br(::GetSysColor(COLOR_WINDOW));
		pDC->FillRect(&rClient, &br);
	}

	// If the item has focus, draw the focus rect.
	// If the item does not have focus, erase the focus rect.
	if ((lpDrawItemStruct->itemAction & ODA_FOCUS) && 
		(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		pDC->DrawFocusRect(&rcItem); 
	}
	else if ((lpDrawItemStruct->itemAction & ODA_FOCUS) &&	
		!(lpDrawItemStruct->itemState & ODS_FOCUS))
	{
		pDC->DrawFocusRect(&rcItem); 
	}

	// To draw the Text set the background mode to Transparent.
	int iBkMode = pDC->SetBkMode(TRANSPARENT);

	if (lpDrawItemStruct->itemState & ODS_SELECTED)
		crText = pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	else if (lpDrawItemStruct->itemState & ODS_DISABLED)
		crText = pDC->SetTextColor(::GetSysColor(COLOR_GRAYTEXT));
	else
		crText = pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));

	// Get the item text.
	GetText(lpDrawItemStruct->itemID, strText);

	// Setup the text format.
	UINT nFormat = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	if (GetStyle() & LBS_USETABSTOPS)
		nFormat |= DT_EXPANDTABS;
	

	// if the ImageList is Existing and there is an associated Image
	// for the Item, draw the Image.
	if(m_pImageList && (iImg != -1 ) )
		m_pImageList->Draw(pDC,iImg,Pt,ILD_NORMAL);
	
	//Draw the Text
	pDC->DrawText(strText, -1, &rText, nFormat | DT_CALCRECT);
	pDC->DrawText(strText, -1, &rText, nFormat);

	pDC->SetTextColor(crText); 
	pDC->SetBkMode(iBkMode);
}

void CDHListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your code to determine the size of specified item
	lpMeasureItemStruct->itemHeight = 16;
}

// void CDHListBox::RefushHorizontalScrollBar()
// {
// 	CDC *pDC = this->GetDC();
// 	ASSERT(pDC);
// 	
// 	int nCount = this->GetCount();
// 	if ( nCount < 1 )
// 	{
// 		this->SetHorizontalExtent( 0 );
// 		return;
// 	}
// 	
// 	int nMaxExtent = 0;
// 	CString szText;
// 	for ( int i = 0; i < nCount; ++i )
// 	{
// 		this->GetText( i, szText );
// 		CSize &cs = pDC->GetTextExtent( szText );
// 		if ( cs.cx > nMaxExtent )
// 		{
// 			nMaxExtent = cs.cx;
// 		}
// 	}
// 	this->SetHorizontalExtent( nMaxExtent );
// }


CDHDiskBox::CDHDiskBox()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_pImageList = NULL;
	m_pImageList = new CImageList;

	m_pImageList->Create(16,16, ILC_COLOR24, 3, 3);
	ASSERT(m_pImageList->m_hImageList);
	
	CBitmap bitResBmp;
	bitResBmp.LoadBitmap(IDB_BITMAP_CD);		//0
	m_pImageList->Add(&bitResBmp, RGB(0,0,0));
	bitResBmp.Detach();
	
	bitResBmp.LoadBitmap(IDB_BITMAP_FLOPPY);	//1
	m_pImageList->Add(&bitResBmp, RGB(0,0,0));
	bitResBmp.Detach();

	bitResBmp.LoadBitmap(IDB_BITMAP_HARD);		//2
	m_pImageList->Add(&bitResBmp, RGB(0,0,0));
	bitResBmp.Detach();
}

CDHDiskBox::~CDHDiskBox()
{
	delete m_pImageList;
}

BEGIN_MESSAGE_MAP(CDHDiskBox, CListBox)
	//{{AFX_MSG_MAP(CDHDiskBox)
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_MENU_DENTER, OnMenuEnterDisk)
	ON_COMMAND(ID_MENU_DREFRESH, OnMenuRefresh)
	ON_COMMAND(ID_MENU_UREFRESH, OnMenuRefresh)
END_MESSAGE_MAP()

BOOL CDHDiskBox::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam<VK_LEFT || pMsg->wParam>VK_DOWN)
		{
			if (pMsg->wParam == VK_F5)
			{
				int nIndex = GetCurSel();
				Refresh();
				if (nIndex>=0 && nIndex<GetCount())
					SetCurSel(nIndex);
			}
			else if (pMsg->wParam == VK_RETURN)
			{
				if (GetCount()>0)
				{
					CString strDisk;
					int nIndex = GetCurSel();
					if (nIndex >= 0)
					{
						GetText(GetCurSel(), strDisk);
						m_pFileBox->EnterDirFromDisk(strDisk);
					}
				}
			}
			return FALSE;
		}
	}
	return CListBox::PreTranslateMessage(pMsg);
}

void CDHDiskBox::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (GetCount() <= 0)
		return;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	BOOL bOutSide;
	int nIndex = ItemFromPoint(point, bOutSide);

	if (bOutSide)
	{
		SetCurSel(-1);
		CMenu cmenu;
		if (m_nLanguage == 1)
			VERIFY(cmenu.LoadMenu(IDR_REFRESHMENU));//加载菜单资源
		else
			VERIFY(cmenu.LoadMenu(IDR_REFRESHMENU_EN));//加载菜单资源
		CMenu* pMenu = new CMenu;
		pMenu = cmenu.GetSubMenu(0);
		CPoint oPoint; GetCursorPos(&oPoint); 
		ASSERT(pMenu!=NULL);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,oPoint.x,oPoint.y,this); //在鼠标当前位置弹出菜单
//		delete pMenu;		
	}
	else
	{
		SetCurSel(nIndex);
		m_nRightItem = nIndex;
		CMenu cmenu;
		if (m_nLanguage == 1)
			VERIFY(cmenu.LoadMenu(IDR_ENTERMENU));//加载菜单资源
		else
			VERIFY(cmenu.LoadMenu(IDR_ENTERMENU_EN));//加载菜单资源
		CMenu* pMenu = new CMenu;
		pMenu = cmenu.GetSubMenu(0);
		CPoint oPoint; GetCursorPos(&oPoint); 
		ASSERT(pMenu!=NULL);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,oPoint.x,oPoint.y,this); //在鼠标当前位置弹出菜单
//		delete pMenu;
	}
	return;
}

void CDHDiskBox::OnMenuEnterDisk()
{
	CString strDisk;
	GetText(m_nRightItem, strDisk);
	m_pFileBox->EnterDirFromDisk(strDisk);
}

void CDHDiskBox::OnMenuRefresh()
{
	Refresh();
	return;
}

void CDHDiskBox::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if (GetCount()>0)
	{
		CString strDisk;
		GetText(GetCurSel(), strDisk);
		m_pFileBox->EnterDirFromDisk(strDisk);
		CListBox::OnLButtonDblClk(nFlags, point);
	}
}

void CDHDiskBox::DeviceChanged(int nChangeSignal)
{
	Refresh();
	if (nChangeSignal == 0)
	{
		char chCurPath[MAX_PATH];
		m_pFileBox->GetCurrentPath(&chCurPath[0]);
		CString strCurPath = chCurPath;
		strCurPath = strCurPath.Left(3);
		if (_access(strCurPath.GetBuffer(0),0)==-1)
		{
			m_pFileBox->Empty();
		}
	}
}

void CDHDiskBox::Refresh(CString sFilter)
{
	ResetContent();
	CHAR SysDir[128]=_T("");
	int strLen=0;
	CString strSysDir = "";
	strLen=GetLogicalDriveStrings(sizeof(SysDir),SysDir);
	if(strLen!=0)
	{
		for(int i=0;i<strLen;i++)
		{
			if(SysDir[i]=='\0')
			{				
				char chDriveName[255], FileSystemName[255];	memset(chDriveName, 0, sizeof(char)*255);
				DWORD VolumeSerialNumber, MaximumComponentLength, FileSystemFlags;
				GetVolumeInformation(&SysDir[i-3], chDriveName, 255, &VolumeSerialNumber, &MaximumComponentLength, &FileSystemFlags, FileSystemName, 255);
				CString strType;
				UINT DriveType=GetDriveType(&SysDir[i-3]);
				if (DriveType==2)
				{
					if (m_nLanguage == 1)
						strSysDir += "(闪存)";
					else
						strSysDir += "(Flash)";
					strSysDir += strType;
					strSysDir += " ";
					strSysDir += chDriveName;
					int nRst = AddString(strSysDir, 1);
					if (nRst >= 0)
						SetItemData(nRst, 1);
				}
				if (DriveType==3)
				{
					if (m_nLanguage == 1)
						strSysDir +="(硬盘)";
					else
						strSysDir +="(Hard) ";
					strSysDir += strType;
					strSysDir += " ";
					strSysDir += chDriveName;
					int nRst = AddString(strSysDir, 2);
					if (nRst >= 0)
						SetItemData(nRst, 2);
				}
				if (DriveType==5)
				{
					if (m_nLanguage == 1)
						strSysDir +="(光盘)";
					else
						strSysDir +="(CD-R) ";
					strSysDir += strType;
					strSysDir += " ";
					strSysDir += chDriveName;
					int nRst = AddString(strSysDir, 0);
					if (nRst >= 0)
						SetItemData(nRst, 0);
				}
				
				strSysDir.Empty();
			}
			else
			{
				strSysDir+=SysDir[i];
			}
		}
	}
}

void CDHDiskBox::Init(CDHFileBox* pFileBox, CWnd* pParent, int nMsgID, CFont* pFont, int nLang)
{
	if (pParent)
	{
		if (pFont)
		{
			LOGFONT lf;
			pFont->GetLogFont(&lf);
			m_cFont.CreateFontIndirect(&lf);
		}
		else
		{
			CFont* pf = GetFont();
			LOGFONT lf;
			pf->GetLogFont(&lf);
			m_cFont.CreateFontIndirect(&lf);
		}
		SetFont(&m_cFont);

		m_pFileBox = pFileBox;
	}
	m_nLanguage = nLang;
	m_pFileBox->Init(pParent, nMsgID, &m_cFont, nLang);
	Refresh();
}


CDHFileBox::CDHFileBox()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

//	CDHListBox::CDHListBox();

	m_pImageList = NULL;
	m_pImageList = new CImageList;

	m_pImageList->Create(16,16, ILC_COLOR24, 2, 2);
	ASSERT(m_pImageList->m_hImageList);

	CBitmap bitResBmp;
	bitResBmp.LoadBitmap(IDB_BITMAP_BMP);		//0
	m_pImageList->Add(&bitResBmp, RGB(0,0,0));
	bitResBmp.Detach();

	bitResBmp.LoadBitmap(IDB_BITMAP_FOLDER);	//1
	m_pImageList->Add(&bitResBmp, RGB(0,0,0));
	bitResBmp.Detach();

	m_strCurrentDir = "";
	m_nRightItem = 0;
	m_pParent = NULL;
	m_strModuleDir = "";
	m_bCtrlDown = false;

	m_hBrush = CreateSolidBrush(RGB(245, 222, 179));
}

CDHFileBox::~CDHFileBox()
{
	delete m_pImageList;
}

BEGIN_MESSAGE_MAP(CDHFileBox, CListBox)
	//{{AFX_MSG_MAP(CDHFileBox)
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_COMMAND(ID_MENU_OPENIMAGE, OnMenuOpenImage)
	ON_COMMAND(ID_MENU_DELETEIMAGE, OnMenuDeleteImage)
	ON_COMMAND(ID_MENU_DENTER, OnMenuEnterDir)
	ON_COMMAND(ID_MENU_REFRESH, OnMenuRefresh)
	ON_COMMAND(ID_MENU_DREFRESH, OnMenuRefresh)
	ON_COMMAND(ID_MENU_UREFRESH, OnMenuRefresh)
END_MESSAGE_MAP()

BOOL CDHFileBox::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam<VK_LEFT || pMsg->wParam>VK_DOWN)
		{
			if (pMsg->wParam == VK_F5)
			{
				int nIndex = GetCurSel();
				Refresh();
				if (nIndex>=0 && nIndex<GetCount())
					SetCurSel(nIndex);
			}
			else if (pMsg->wParam == VK_RETURN)
			{
				if (GetCount() > 0)
				{
					int nIndex = GetCurSel();
					if (nIndex >= 0)
					{
						CString strCDir, strDDir;
						GetText(nIndex, strCDir);
						if (strCDir == ".")
						{
							strDDir = m_strCurrentDir.Left(2);
							EnterDir(strDDir);
						}
						else if (strCDir == "..")
						{
							if (m_strCurrentDir.Right(1) == "\\")
								strDDir = m_strCurrentDir.Left(m_strCurrentDir.GetLength()-1);
							else
								strDDir = m_strCurrentDir;
							int r1 = strDDir.ReverseFind('\\');
							strDDir = strDDir.Left(r1);
							EnterDir(strDDir);
						}
						else
						{
							strDDir = m_strCurrentDir;
							if (m_strCurrentDir.Right(1) != '\\')
								strDDir += "\\";
							strDDir += strCDir;
							if (DirOrBmp(strCDir))
								ShellExecute(NULL, "open", strDDir.GetBuffer(0), NULL, NULL, SW_SHOW);
							else
								EnterDir(strDDir);
						}
					}
					
				}
			}
			else if (pMsg->wParam == VK_BACK)
			{
				CString strDDir;
				if (m_strCurrentDir.Right(1) == "\\")
					strDDir = m_strCurrentDir.Left(m_strCurrentDir.GetLength()-1);
				else
					strDDir = m_strCurrentDir;
				if (strDDir.GetLength() > 3)
				{
					int r1 = strDDir.ReverseFind('\\');
					strDDir = strDDir.Left(r1);
					EnterDir(strDDir);
				}
			}
			return FALSE;
		}
	}

	return CListBox::PreTranslateMessage(pMsg);
}

void CDHFileBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_CONTROL)
	{
		m_bCtrlDown = true;
	}

	if (GetCount() <= 0)
		return;

	int nIndex = GetCurSel();
	if (nIndex < 0)
		return;

	if (nChar == VK_CONTROL)
	{
		SelectImage(nIndex);
	}
	if (nChar==VK_DOWN || nChar==VK_RIGHT)
	{
		if (nIndex<=GetCount()-2)
		{
			SetCurSel(++nIndex);
			SelectImage(nIndex);
		}
	}
	if (nChar==VK_LEFT || nChar==VK_UP)
	{
		if (nIndex >= 1)
		{
			SetCurSel(--nIndex);
			SelectImage(nIndex);
		}
	}
	if (nChar == VK_DELETE)
	{
		CString strFileName, strFilePath;
		GetText(nIndex, strFileName);
		strFilePath = m_strCurrentDir + "\\" + strFileName;

		if (DirOrBmp(strFilePath))
		{
			if(_access(strFilePath.GetBuffer(0), 0)==0)
			{
				CString strShowMsg;
				if (m_nLanguage == 1)
					strShowMsg.Format("是否确定要删除图像?\r\n%s", strFileName.GetBuffer(0));
				else
					strShowMsg.Format("Do you want to delete the image file?\r\n%s", strFileName.GetBuffer(0));
				
				if (IDNO == MessageBox(strShowMsg.GetBuffer(0), NULL, MB_YESNO|MB_ICONQUESTION))
					return;
				
				CFileStatus FStatus;
				CFile::GetStatus(strFilePath.GetBuffer(0), FStatus);
				FStatus.m_attribute = FStatus.m_attribute&BYTE(0x0E);
				CFile::SetStatus(strFilePath.GetBuffer(0), FStatus);
				if(DeleteFile(strFilePath.GetBuffer(0)))
					DeleteString(nIndex);
			}
		}
	}
	if (nChar == VK_RETURN)
	{
		CString strFileName, strFilePath;
		GetText(nIndex, strFileName);
		strFilePath = m_strCurrentDir + "\\" + strFileName;

		if (DirOrBmp(strFilePath))
		{
			ShellExecute(NULL, "open", strFilePath, NULL, NULL, SW_SHOW);
		}
		else
		{
			EnterDir(strFilePath);
		}
	}
	return;
}

void CDHFileBox::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_CONTROL)
		m_bCtrlDown = false;

	if (GetCount() <= 0)
		return;
	
	int nIndex = GetCurSel();
	if (nIndex < 0)
		return;

	if (nChar == VK_CONTROL)
	{
		SelectImage(nIndex);
	}
}

void CDHFileBox::SelectImage(int nIndex)
{
	CString strCDir, strDDir;
	GetText(nIndex, strCDir);
	if (strCDir!="." && strCDir!="..")
	{
		// 判断是不是灯检机目录Circlexxx,如果是灯检机目录，则直接发送目录 [8/17/2015 Administrator]

		strDDir = m_strCurrentDir;
		if (m_strCurrentDir.Right(1) != '\\')
			strDDir += "\\";
		strDDir += strCDir;
		if (DirOrBmp(strDDir.GetBuffer(0)))
		{	
			COPYDATASTRUCT cpd;
			if (m_bCtrlDown)
				cpd.dwData = DHLIST_SELIMGFILECTRL;
			else
				cpd.dwData = DHLIST_SELIMGFILE;
			cpd.cbData = m_nMsgID;
			cpd.lpData = (PVOID)(strDDir.GetBuffer(0));
			LRESULT lResult = m_pParent->SendMessage(DHLISTMSG, (WPARAM)AfxGetApp()->m_pMainWnd->GetSafeHwnd(), (LPARAM)&cpd);
		}
	}
}

void CDHFileBox::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (GetCount() <= 0)
		return;
	
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	BOOL bOutSide;
	int nIndex = ItemFromPoint(point, bOutSide);
	
	if (bOutSide)
	{
		SetCurSel(-1);
		m_nRightItem = -1;
		CMenu cmenu;
		if (m_nLanguage == 1)
			VERIFY(cmenu.LoadMenu(IDR_REFRESHMENU));//加载菜单资源
		else
			VERIFY(cmenu.LoadMenu(IDR_REFRESHMENU_EN));//加载菜单资源
		CMenu* pMenu = new CMenu;
		pMenu = cmenu.GetSubMenu(0);
		CPoint oPoint; GetCursorPos(&oPoint); 
		ASSERT(pMenu!=NULL);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,oPoint.x,oPoint.y,this); //在鼠标当前位置弹出菜单
		//		delete pMenu;		
	}
	else
	{
		SetCurSel(nIndex);
		m_nRightItem = nIndex;
		
		CString strFileName, strFilePath;
		GetText(nIndex, strFileName);
		strFilePath = m_strCurrentDir + "\\" + strFileName;
		if (DirOrBmp(strFilePath))
		{
			CMenu cmenu;
			if (m_nLanguage == 1)
				VERIFY(cmenu.LoadMenu(IDR_RCMENU));//加载菜单资源
			else
				VERIFY(cmenu.LoadMenu(IDR_RCMENU_EN));//加载菜单资源 
			CMenu* pMenu = new CMenu;
			pMenu = cmenu.GetSubMenu(0);
			CPoint oPoint; GetCursorPos(&oPoint); 
			ASSERT(pMenu!=NULL);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,oPoint.x,oPoint.y,this); //在鼠标当前位置弹出菜单
			//		delete pMenu;
		}
		else
		{
			CMenu cmenu;
			if (m_nLanguage == 1)
				VERIFY(cmenu.LoadMenu(IDR_REFRESHMENU));//加载菜单资源
			else
				VERIFY(cmenu.LoadMenu(IDR_REFRESHMENU_EN));//加载菜单资源
			CMenu* pMenu = new CMenu;
			pMenu = cmenu.GetSubMenu(0);
			CPoint oPoint; GetCursorPos(&oPoint); 
			ASSERT(pMenu!=NULL);
			pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON,oPoint.x,oPoint.y,this); //在鼠标当前位置弹出菜单
			//		delete pMenu;
		}

	}
	return;
}

void CDHFileBox::Init(CWnd* pParent, int nMsgID, CFont* pFont, int nLang)
{
	if(pParent)
	{
		m_pParent = pParent;
		m_nMsgID = nMsgID;

		LOGFONT lf;
		pFont->GetLogFont(&lf);
		m_cFont.CreateFontIndirect(&lf);
		SetFont(&m_cFont);
	}

	m_nLanguage = nLang;
}

void CDHFileBox::OnMenuOpenImage()
{
	CString strFilePath;
	GetText(m_nRightItem, strFilePath);
	strFilePath = m_strCurrentDir + "\\" + strFilePath;
	ShellExecute(NULL, "open", strFilePath.GetBuffer(0), NULL, NULL, SW_SHOW);
}

void CDHFileBox::OnMenuDeleteImage()
{
	CString strFilePath;
	GetText(m_nRightItem, strFilePath);
	strFilePath = m_strCurrentDir + "\\" + strFilePath;

	if(_access(strFilePath.GetBuffer(0), 0)==0)
	{
		CString strShowMsg;
		if (m_nLanguage == 1)
			strShowMsg.Format("是否确定要删除图像?\r\n%s", strFilePath.GetBuffer(0));
		else
			strShowMsg.Format("Do you want to delete the image file?\r\n%s", strFilePath.GetBuffer(0));

		if (IDNO == MessageBox(strShowMsg.GetBuffer(0), NULL, MB_YESNO|MB_ICONQUESTION))
			return;

		CFileStatus FStatus;
		CFile::GetStatus(strFilePath.GetBuffer(0), FStatus);
		FStatus.m_attribute = FStatus.m_attribute&BYTE(0x0E);
		CFile::SetStatus(strFilePath.GetBuffer(0), FStatus);
		if(DeleteFile(strFilePath.GetBuffer(0)))
			DeleteString(m_nRightItem);
	}
}

void CDHFileBox::OnMenuEnterDir()
{
	CString strDirPath;
	GetText(m_nRightItem, strDirPath);
	strDirPath = m_strCurrentDir + "\\" + strDirPath;

	EnterDir(strDirPath);
}

void CDHFileBox::OnMenuRefresh()
{
	EnterDir(m_strCurrentDir, TRUE);
	return;
}

void CDHFileBox::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (GetCount()==0)
		return;

	CListBox::OnLButtonDblClk(nFlags, point);

	int nIndex = GetCurSel();
	if (nIndex < 0)
		return;
	CString strCDir, strDDir;
	GetText(nIndex, strCDir);
	if (strCDir == ".")
	{
		strDDir = m_strCurrentDir.Left(2);
		EnterDir(strDDir);
	}
	else if (strCDir == "..")
	{
		if (m_strCurrentDir.Right(1) == "\\")
			strDDir = m_strCurrentDir.Left(m_strCurrentDir.GetLength()-1);
		else
			strDDir = m_strCurrentDir;
		int r1 = strDDir.ReverseFind('\\');
		strDDir = strDDir.Left(r1);
		EnterDir(strDDir);
	}
	else
	{
		strDDir = m_strCurrentDir;
		if (m_strCurrentDir.Right(1) != '\\')
			strDDir += "\\";
		strDDir += strCDir;
		if (DirOrBmp(strCDir))
			;//EnterDir(strDDir);/*ShellExecute(NULL, "open", strDDir.GetBuffer(0), NULL, NULL, SW_SHOW)*/
		else
			EnterDir(strDDir);
	}
}

void CDHFileBox::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (GetCount()<=0)
		return;

	CListBox::OnLButtonDown(nFlags, point);

	BOOL bOutSide;
	ItemFromPoint(point, bOutSide);

	if (!bOutSide)
	{
		int nIndex = GetCurSel();
		if (nIndex < 0)
			return;

		SelectImage(nIndex);
	}
}

void CDHFileBox::Refresh(CString sFilter)
{
	EnterDir(m_strCurrentDir);
}

BOOL CDHFileBox::EnterDir(CString strDir, BOOL bResetIfFailed, BOOL bPopup)
{
	if (strDir.Right(1) == "\\")
	{
		strDir = strDir.Left(strDir.GetLength()-1);
	}

	if(_access(strDir.GetBuffer(0), 0) == -1)
	{
		if (bPopup == TRUE)
		{
			CString strMsg;
			if (m_nLanguage == 1)
				strMsg.Format(_T("路径[%s]不存在"), strDir);
			else
				strMsg.Format(_T("Path [%s] doesn't exist"), strDir);
			AfxMessageBox(strMsg);
		}
		
		if (bResetIfFailed)
		{
			Empty();
		}
		return FALSE;
	}

 	CFileFind finder;
	if (strDir.GetLength() > 3)
	{
		finder.FindFile(strDir.GetBuffer(0));
		finder.FindNextFile();
		if (!finder.IsDirectory())
			return FALSE;
	}

	ResetContent();
	
	CString strSearch = strDir;
	strSearch += "\\*.*";
	BOOL Ret = finder.FindFile(strSearch.GetBuffer(0));
	CStringArray strArrSuperDir, strArrSuperImg;

	while(Ret)
	{
		Ret = finder.FindNextFile();
		
		if (finder.IsDirectory() || finder.IsDots())
		{
			if (finder.GetFileName()=="$RECYCLE.BIN" || finder.GetFileName()=="~1" || 
				finder.GetFileName()=="System Volume Information" || finder.GetFileName()=="RECYCLER")
				continue;
			
			CFileStatus FStatus;	
			CFile::GetStatus(finder.GetFilePath(), FStatus);
			if ((FStatus.m_attribute&2) == 2)//2表示隐藏属性
				continue;
			
			strArrSuperDir.Add(finder.GetFileName());
		}
		else if ((finder.GetFileName()).Right(4)==".bmp" || (finder.GetFileName()).Right(4)==".BMP")
		{
//			AfxMessageBox(finder.GetFileName());
			strArrSuperImg.Add(finder.GetFileName());
		}
	}

	m_strCurrentDir = strDir;
	if (m_strCurrentDir.Right(1)=="\\")
		m_strCurrentDir = m_strCurrentDir.Left(m_strCurrentDir.GetLength()-1);
	Rearrange(&strArrSuperDir, &strArrSuperImg);

// 	if (bSendMessage)
// 	{			
		COPYDATASTRUCT cpd;
		cpd.dwData = DHLIST_DIRCHANGE;
		cpd.cbData = m_nMsgID;
		cpd.lpData = m_strCurrentDir.GetBuffer(0);
		LRESULT lResult = m_pParent->SendMessage(DHLISTMSG,
			(WPARAM)AfxGetApp()->m_pMainWnd->GetSafeHwnd(), (LPARAM)&cpd);
// 	}

	return TRUE;
}

void CDHFileBox::EnterDirFromDisk(CString strDir)
{
	CString strEnter = strDir;
	strEnter = strEnter.Left(2);
	EnterDir(strEnter);
}

void CDHFileBox::GetCurrentPath(char* chPath)
{
	strcpy(&chPath[0], m_strCurrentDir.GetBuffer(0));
}

BOOL CDHFileBox::GetSelectedPath(char* chPath)
{
	if (GetCount() <= 0)
		return -1;
	CString strSelDir, strPath;
	int nSel = GetCurSel();
	if (nSel < 0)
		return  -2;
	GetText(nSel, strSelDir);
	strPath = m_strCurrentDir;

	if (strSelDir == ".")
	{
		strPath = m_strCurrentDir.Left(2);
		strcpy(&chPath[0], strPath.GetBuffer(0));
		return 1;
	}
	else if (strSelDir == "..")
	{
		if (m_strCurrentDir.Right(1) == "\\")
			strPath = strPath.Left(strPath.GetLength() - 1);
		int rp = strPath.ReverseFind('\\');
		strPath = strPath.Left(rp);
		strcpy(&chPath[0], strPath.GetBuffer(0));
		return 1;	
	}

	if (m_strCurrentDir.Right(1) != "\\")
		strPath += "\\";
	strPath += strSelDir;
	strcpy(&chPath[0], strPath.GetBuffer(0));
	CFileFind finder;
	finder.FindFile(strPath.GetBuffer(0));
	finder.FindNextFile();
	return finder.IsDirectory();
}


BOOL CDHFileBox::DirOrBmp(const CString& strFilePath)
{
	//////////////////////////////////////////////////////////////////////////
	int  n =0;
	int index = strFilePath.ReverseFind('\\');

// 	bool bCheckImg{ true };//*[7/13/2017 ConanSteve]*:  是否检测图片
// 	CFileFind fileFind;
// 	bool bRet = fileFind.FindFile(strFilePath+TEXT("\\*.*"));
// 	while (bRet)
// 	{
// 		bRet = fileFind.FindNextFile();
// 		CString sFilePathName{ TEXT("") };
// 		sFilePathName = fileFind.GetFileName();
// 		if(fileFind.IsDots())
// 			continue;
// 		else if (fileFind.IsDirectory())
// 			return false;
// 		auto iPos = sFilePathName.ReverseFind(TEXT('.'));
// 		if (-1 == iPos)
// 			return false;
// 		CString sExt = sFilePathName.Mid(iPos + 1, sFilePathName.GetLength());
// 		if (0 != sExt.CompareNoCase(TEXT("BMP")))
// 			return false;
// 	}
// 
// 	return true;
	CString sAppPath{ TEXT("") };
	GetModuleFileName(NULL, sAppPath.GetBuffer(256), 256);
	sAppPath.ReleaseBuffer();
	sAppPath = sAppPath.Mid(0, sAppPath.ReverseFind('\\'));
	CString sAlgConfigPath = sAppPath + TEXT("\\Config\\AlgConfig.ini");
	bool bEnableSequentialCheck = 1==GetPrivateProfileInt(TEXT("Demo Controls"), TEXT("EnableSequentialCheck"), 0, sAlgConfigPath);
	CString sSequentialCheckFolderFilter{ TEXT("") };
	GetPrivateProfileString(TEXT("Demo Controls"), TEXT("SequentialCheckFolderFilter"), TEXT(""), sSequentialCheckFolderFilter.GetBuffer(256), 256, sAlgConfigPath);

	CString strAJDZ=strFilePath.Right(strFilePath.GetLength()-index-1);

	if (bEnableSequentialCheck && strAJDZ.Find(sSequentialCheckFolderFilter) >=0)//文件夹的名字中包含Circle
	{
		//判断这个文件夹下面是否含有bmp文件
		CFileFind finder;
		BOOL bRet = finder.FindFile(strFilePath+"\\*.bmp");
		//AfxMessageBox(finder.FindFile(strFilePath+"\\*.bmp",0));
		if (bRet)
		{
			
			//灯检机Circle文件夹
			return TRUE;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (strFilePath.Right(4)==".bmp" || strFilePath.Right(4)==".BMP")
		return TRUE;
	else
		return FALSE;
}

void CDHFileBox::Rearrange(CStringArray* strArrDirNames, CStringArray* strArrImgNames)
{
// 	int *nRst = new int[strArrLongNames->GetSize()];
// 	int *nImg = new int[strArrLongNames->GetSize()];
	int nSize = strArrDirNames->GetSize();
	int n=0;
	for (n=0; n<nSize; ++n)
	{
		CString strThisPath = strArrDirNames->GetAt(n);
		AddString(strThisPath.GetBuffer(0), 1);
	}

	nSize = strArrImgNames->GetSize();
	for (n=0; n<nSize; ++n)
	{
		CString strThisPath = strArrImgNames->GetAt(n);
		AddString(strThisPath.GetBuffer(0), 0);
	}
}

void CDHFileBox::Empty()
{
	m_strCurrentDir.Empty();
	ResetContent();
}

CDHResultBox::CDHResultBox()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_pImageList = NULL;
	m_pImageList = new CImageList;
	
	m_pImageList->Create(16,16, TRUE, 3, 3);
	
	ASSERT(m_pImageList->m_hImageList);
	
	HICON hIcon = NULL;	
	hIcon = theApp.LoadIcon(IDI_ICON_YES);	//0
	m_pImageList->Add(hIcon);
	
	hIcon = theApp.LoadIcon(IDI_ICON_NO);	//1
	m_pImageList->Add(hIcon);
	
	hIcon = theApp.LoadIcon(IDI_ICON_NULL);	//2
	m_pImageList->Add(hIcon);

	hIcon = theApp.LoadIcon(IDI_ICON_EXP);	//3
	m_pImageList->Add(hIcon);
}

CDHResultBox::~CDHResultBox()
{
	delete m_pImageList;
}

BOOL CDHResultBox::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam<VK_LEFT || pMsg->wParam>VK_DOWN)
		{
			return TRUE;
		}
	}
	return CListBox::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CDHResultBox, CListBox)
//{{AFX_MSG_MAP(CDHDiskBox)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDHModelBox::CDHModelBox()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	m_strCurModelName = "";
	m_pImageList = NULL;
	m_pImageList = new CImageList;

	m_pImageList->Create(16,16, ILC_COLOR24, 1, 1);
	ASSERT(m_pImageList->m_hImageList);
	
	CBitmap bitResBmp;
	bitResBmp.LoadBitmap(IDB_BITMAP_MODEL);		//0
	m_pImageList->Add(&bitResBmp, RGB(0,0,0));
	bitResBmp.Detach();

	char chPath[MAX_PATH], chDisk[MAX_PATH], chDir[MAX_PATH], chFName[MAX_PATH], chExt[MAX_PATH], chInfo[256];
	GetModuleFileName(NULL, chPath, MAX_PATH);
	_splitpath(chPath, chDisk, chDir, chFName, chExt);
	m_strAppDir.Format("%s%s", chDisk, chDir);
	GetPrivateProfileString("设置", "初始化路径", "NULL", chInfo, 256, m_strAppDir+"Set.ini");
	if (strcmp(chInfo, "NULL") != 0)
		m_strAppDir = chInfo;
	if (m_strAppDir.Right(1) == "\\")
		m_strAppDir = m_strAppDir.Left(m_strAppDir.GetLength()-1);
	m_strModelDir.Format("%s\\ModelInfo", m_strAppDir);
}

CDHModelBox::~CDHModelBox()
{
	delete m_pImageList;
}

BOOL CDHModelBox::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam<VK_LEFT || pMsg->wParam>VK_DOWN)
		{
			if (pMsg->wParam == VK_F5)
			{
				int nIndex = GetCurSel();
				Refresh();
				if (nIndex>=0 && nIndex<GetCount())
					SetCurSel(nIndex);
			}
			else if (pMsg->wParam == VK_RETURN)
			{
				if (GetCount() > 0)
				{
					int nIndex = GetCurSel();
					if (nIndex >= 0)
					{
						CString strModelName;
						GetText(nIndex, strModelName);
						
						COPYDATASTRUCT cpd;
						cpd.dwData = DHLIST_CHNMODEL;
						cpd.lpData = strModelName.GetBuffer(0);
						LRESULT lResult = m_pParent->SendMessage(DHLISTMSG,
							(WPARAM)AfxGetApp()->m_pMainWnd->GetSafeHwnd(), (LPARAM)&cpd);
						
						m_strCurModelName = strModelName;
					}
				}
			}
			else if (pMsg->wParam == VK_DELETE)
			{
				if (GetCount() > 0)
				{
					int nIndex = GetCurSel();
					if (nIndex >= 0)
						DeleteModel(nIndex);
				}
			}
			return FALSE;
		}
	}

	return CListBox::PreTranslateMessage(pMsg);
}

void CDHModelBox::Init(CWnd* pParent, int nLang, CString sFilter)
{
	if (pParent)
		m_pParent = pParent;
	m_nLanguage = nLang;
	Refresh(sFilter);
}

void CDHModelBox::Refresh(CString sFilter)
{
	ResetContent();

	CFileFind finder;
	BOOL bRet = finder.FindFile(m_strModelDir+TEXT("\\")+ sFilter, 0);

	while (bRet)
	{
		bRet = finder.FindNextFile();
		if (finder.IsDirectory())
		{
			CString sFileName = finder.GetFileName();
			if (sFileName !="." && sFileName!="..")
			{
				AddString(sFileName, 0);
			}
		}
	}

	COPYDATASTRUCT cpd;
	cpd.dwData = DHLIST_REFRESHMDL;
	cpd.lpData = NULL;
	LRESULT lResult = m_pParent->SendMessage(DHLISTMSG,
		(WPARAM)AfxGetApp()->m_pMainWnd->GetSafeHwnd(), (LPARAM)&cpd);
}

BOOL CDHModelBox::ApplyModel(char* chModelName)
{
	int nIndex = FindStringExact(0, chModelName);
	if (nIndex < 0)
		return FALSE;
	
	SetCurSel(nIndex);
	// MSVCRD.dll 崩溃 [8/31/2015 Administrator]GetText(nIndex, m_strCurModelName);
	CString strModelName;
	GetText(nIndex, strModelName);
	
	//m_strCurModelName = strModelName;
//	m_strCurModelName.Format("%s",strModelName);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CDHModelBox, CListBox)
	//{{AFX_MSG_MAP(CDHDiskBox)
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_MENU_APPLY, OnMenuApply)
	ON_COMMAND(ID_MENU_RENAME, OnMenuRename)
	ON_COMMAND(ID_MENU_DELETEMODEL, OnMenuDeleteModel)
	ON_COMMAND(ID_MENU_OPENMODEL, OnMenuOpenDir)
	ON_COMMAND(ID_MENU_REFRESH, OnMenuRefreshModel)
	ON_COMMAND(ID_MENU_OPENMODELDIR, OnMenuOpenModelDir)
END_MESSAGE_MAP()

void CDHModelBox::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (GetCount()==0)
		return;
	
	CListBox::OnLButtonDblClk(nFlags, point);
	
	int nIndex = GetCurSel();
	if (nIndex < 0)
		return;
	CString strModelName;
	GetText(nIndex, strModelName);

	COPYDATASTRUCT cpd;
	cpd.dwData = DHLIST_CHNMODEL;
	cpd.lpData = strModelName.GetBuffer(0);
	LRESULT lResult = m_pParent->SendMessage(DHLISTMSG,
		(WPARAM)AfxGetApp()->m_pMainWnd->GetSafeHwnd(), (LPARAM)&cpd);

	m_strCurModelName = strModelName;
	
	return;
}

void CDHModelBox::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (GetCount() <= 0)
		return;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	
	BOOL bOutSide;
	int nIndex = ItemFromPoint(point, bOutSide);

	if (bOutSide)
	{
		SetCurSel(-1);
		m_nRightItem = -1;
 		CMenu cmenu;
		if (m_nLanguage == 1)
 			VERIFY(cmenu.LoadMenu(IDR_MENU_MODEL2));//加载菜单资源
		else
			VERIFY(cmenu.LoadMenu(IDR_MENU_MODEL2_EN));//加载菜单资源
 		CMenu* pMenu = new CMenu;
 		pMenu = cmenu.GetSubMenu(0);
 		CPoint oPoint; GetCursorPos(&oPoint); 
 		ASSERT(pMenu!=NULL);
 		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON, oPoint.x, oPoint.y, this); //在鼠标当前位置弹出菜单
//		delete pMenu;
	}
	else
	{
		SetCurSel(nIndex);
		m_nRightItem = nIndex;
		CMenu cmenu;
		if (m_nLanguage == 1)
			VERIFY(cmenu.LoadMenu(IDR_MODELMENU));//加载菜单资源
		else
			VERIFY(cmenu.LoadMenu(IDR_MODELMENU_EN));//加载菜单资源
		CMenu* pMenu = new CMenu;
		pMenu = cmenu.GetSubMenu(0);
		CPoint oPoint; GetCursorPos(&oPoint); 
		ASSERT(pMenu!=NULL);
 		pMenu->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON, oPoint.x, oPoint.y, this);
	}
	return;
}

void CDHModelBox::OnMenuApply()
{
	if (m_nRightItem < 0)
		return;
	
	CString strModelName;
	GetText(m_nRightItem, strModelName);

	COPYDATASTRUCT cpd;
	cpd.dwData = DHLIST_CHNMODEL;
	cpd.lpData = strModelName.GetBuffer(0);
	LRESULT lResult = m_pParent->SendMessage(DHLISTMSG,
		(WPARAM)AfxGetApp()->m_pMainWnd->GetSafeHwnd(), (LPARAM)&cpd);

	m_strCurModelName = strModelName;
	return;
}

void CDHModelBox::OnMenuRename()
{
	if (m_nRightItem < 0)
		return;
	
	RenameModel(m_nRightItem);

	return;
}

void CDHModelBox::RenameModel(int nIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	HWND hwnd = AfxGetApp()->m_pMainWnd->GetSafeHwnd();
	

	CString strModelName;
	GetText(nIndex, strModelName);

	CUIDlgModelRename cDlg;
	if (m_nLanguage == 1)
		cDlg.m_strMsg.Format("将[%s]更名为：", strModelName);
	else
		cDlg.m_strMsg.Format("Rename Model [%s] to: ", strModelName);
	bool bContiDo = true;
	CString strName;
	while (bContiDo)
	{
		int nRst = cDlg.DoModal();
		if (nRst == IDOK)
		{
			CString strNewName = cDlg.m_strNewName;
			bool bFindSame = false;
			for (int n = 0; n < GetCount(); ++n)
			{
				CString strNames;
				GetText(n, strNames);
				if (strNames.CompareNoCase(strNewName) == 0)
				{
					bFindSame = true;
					break;
				}
			}
			if (!bFindSame)
			{
				strName = strNewName;
				break;
			}
		}
		else
		{
			bContiDo = false;
			break;
		}
	}

	if (bContiDo)
	{
		CString str1, str2;
		str1 = m_strModelDir + "\\" + strModelName;
		str2 = m_strModelDir + "\\" + strName;
		rename(str1.GetBuffer(0), str2.GetBuffer(0));

		Refresh();
		int nIndex = FindStringExact(0, strName);
		SetCurSel(nIndex);

		if (strModelName.CompareNoCase(m_strCurModelName) == 0)
		{
			COPYDATASTRUCT cpd;
			cpd.dwData = DHLIST_RENAMEMDL;
			cpd.lpData = strName.GetBuffer(0);
			LRESULT lResult = m_pParent->SendMessage(DHLISTMSG,
				(WPARAM)hwnd, (LPARAM)&cpd);
		}
	}


	return;
}

void CDHModelBox::OnMenuDeleteModel()
{
	if (m_nRightItem < 0)
		return;

	DeleteModel(m_nRightItem);

	return;
}

void CDHModelBox::DeleteModel(int nIndex)
{
	CString strModelName;
	GetText(m_nRightItem, strModelName);

	if (strModelName.Compare(m_strCurModelName) == 0)
	{
		if (m_nLanguage == 1)
			MessageBox(_T("模板正在使用中，不能删除！"), NULL, MB_ICONERROR);
		else
			MessageBox(_T("Can't Delete Using Model"), NULL, MB_ICONERROR);
	}
	else
	{
		CString strModelPath = m_strModelDir + "\\" + strModelName;
		DeleteWholeDir(strModelPath);
	}
	Refresh();
	return;
}

void CDHModelBox::DeleteWholeDir(CString& strDir)
{
	CString strSearch = strDir + "\\*.*";
	CFileFind finder;
	BOOL bRet = finder.FindFile(strSearch.GetBuffer(0));
	
	while (bRet)
	{
		bRet = finder.FindNextFile();
		if (finder.IsDirectory())
		{
			if (! (finder.IsDots() || finder.GetFileName()=="$RECYCLE.BIN" || 
				finder.GetFileName()=="System Volume Information" || finder.GetFileName()=="RECYCLER") )
			{
				DeleteWholeDir(finder.GetFilePath());
			}
		}
		else
		{
			CString strPath = finder.GetFilePath();
			CFileStatus FStatus;
			CFile::GetStatus(strPath.GetBuffer(0), FStatus);
			FStatus.m_attribute = FStatus.m_attribute&BYTE(0x0E);
			CFile::SetStatus(strPath.GetBuffer(0), FStatus);
			DeleteFile(strPath);
		}
	}
	finder.Close();
	RemoveDirectory(strDir.GetBuffer(0));
}

void CDHModelBox::OnMenuOpenDir()
{
	if (m_nRightItem < 0)
		return;
	
	CString strModelName;
	GetText(m_nRightItem, strModelName);

	ShellExecute(NULL, "open", (CString)m_strModelDir+"\\"+strModelName, NULL, NULL, SW_SHOW);
	return;
}

void CDHModelBox::OnMenuRefreshModel()
{
	Refresh();
	return;
}

void CDHModelBox::OnMenuOpenModelDir()
{
	ShellExecute(NULL, "open", m_strModelDir, NULL, NULL, SW_SHOW);
	return;
}

void CDHModelBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (GetCount() <= 0)
		return;
	
	int nIndex = GetCurSel();
	if (nIndex < 0)
		return;
	
	if (nChar == VK_DELETE)
	{
		DeleteModel(nIndex);
	}
	if (nChar == VK_F2)
	{
		RenameModel(nIndex);
	}
}