// CCCCC.h: interface for the CCCCCC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CCCCC_H__C3F44402_4572_4B2B_895B_E56C1D8CFEA0__INCLUDED_)
#define AFX_CCCCC_H__C3F44402_4572_4B2B_895B_E56C1D8CFEA0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	_EXPORTING
#ifdef _EXPORTING
#define CLASS_DECLSPEC    __declspec(dllexport)  //程序编译时使用
#else
#define CLASS_DECLSPEC    __declspec(dllimport)	 //动态库应用时使用
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define DHLISTMSG				WM_USER+1000
#define DHLIST_SELIMGFILE		WM_USER+1001
#define DHLIST_SELIMGFILECTRL	WM_USER+1002
#define DHLIST_DIRCHANGE		WM_USER+1003
#define DHLIST_CHNMODEL			WM_USER+1004
#define DHLIST_RENAMEMDL		WM_USER+1005
#define DHLIST_REFRESHMDL		WM_USER+1006

//class CDHResultBox;
class CDHListBox : public CListBox
{
//	friend class CDHResultBox;
	// Construction
public:
	CDHListBox();
	virtual ~CDHListBox();

	//{{AFX_VIRTUAL(CDHListBox)
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg){return CListBox::PreTranslateMessage(pMsg);}
	//}}AFX_VIRTUAL
	

	// Implementation
protected:
	HBRUSH m_hBrush;
	CImageList* m_pImageList;
	int m_nRightItem;
	CFont m_cFont;
	int m_nLanguage;	//0:英文 1:中文
	

	int AddString(LPCTSTR lpszItem);
	int AddString(LPCTSTR lpszItem, int iImg);
	int InsertString(int iIndex, LPCTSTR lpszItem);
	int InsertString(int iIndex, LPCTSTR lpszItem, int iImg)
	{
		int iRet = CListBox::InsertString(iIndex,lpszItem);
		RefushHorizontalScrollBar();
		if (iRet >= 0)
			SetItemData(iRet, iImg);
		return iRet;
	}
	void SetItemImage(int iIndex, int iImg)
	{
		CListBox::SetItemData(iIndex, iImg);
		RedrawWindow();
	}
	void RefushHorizontalScrollBar()
	{
		CDC *pDC = this->GetDC();
		ASSERT(pDC);
		
		int nCount = this->GetCount();
		if ( nCount < 1 )
		{
			this->SetHorizontalExtent( 0 );
			return;
		}
		
		int nMaxExtent = 0;
		CString szText;
		for ( int i = 0; i < nCount; ++i )
		{
			this->GetText( i, szText );
			CSize &cs = pDC->GetTextExtent( szText );
			if ( cs.cx > nMaxExtent )
			{
				nMaxExtent = cs.cx;
			}
		}
		this->SetHorizontalExtent( nMaxExtent );
	}
	virtual void Refresh(CString sFilter=TEXT("*.*")) = 0;

	DECLARE_MESSAGE_MAP()
};

class CDHFileBox;
class CLASS_DECLSPEC CDHDiskBox : public CDHListBox
{
// Construction
public:
	CDHDiskBox();

	//{{AFX_VIRTUAL(CDHDiskBox)
	//}}AFX_VIRTUAL
	
	// Implementation
public:
	virtual ~CDHDiskBox();
	void Refresh(CString sFilter=TEXT("*.*"));
	void Init(CDHFileBox* pFileBox, CWnd* pParent, int nMsgID, CFont* pFont=NULL, int nLang=1);	//0:英文  1:中文
	void DeviceChanged(int nChangeSignal); //0卸载 1插入
private:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDHDiskBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
protected:
	CDHFileBox* m_pFileBox;
	// Generated message map functions
	//{{AFX_MSG(CDHDiskBox)
	//}}AFX_MSG
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMenuEnterDisk();
	afx_msg void OnMenuRefresh();
	DECLARE_MESSAGE_MAP()
};

class CLASS_DECLSPEC CDHFileBox : public CDHListBox
{
	friend class CDHDiskBox;
	// Construction
public:
	CDHFileBox();

	//{{AFX_VIRTUAL(CDHFileBox)
	//}}AFX_VIRTUAL
	
	// Implementation

	virtual ~CDHFileBox();
	void Refresh(CString sFilter = TEXT("*.*"));
	void GetCurrentPath(char* chPath);
	BOOL GetSelectedPath(char* chPath);
	BOOL EnterDir(CString strDir, BOOL bResetIfFailed = FALSE, BOOL bPopup = TRUE);
	void Empty();
private:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDHFileBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
protected:
	void Init(CWnd* pParent, int nMsgID, CFont* pFont, int nLang);	//nMsgID传入第几路即可，从1开始记，在发送消息时储存在LPARAM中的.cbData中
	void EnterDirFromDisk(CString strDir);
	inline void Rearrange(CStringArray* strArrDirNames, CStringArray* strArrImgNames);
	inline BOOL DirOrBmp(const CString& strFilePath);	//FALSE是文件及 TRUE是图像
	inline void SelectImage(int nIndex);

	CString m_strCurrentDir, m_strModuleDir;
	CWnd* m_pParent;
	int	  m_nMsgID;
	bool  m_bCtrlDown;
	// Generated message map functions

	//{{AFX_MSG(CDHFileBox)
	//}}AFX_MSG
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMenuOpenImage();
	afx_msg void OnMenuDeleteImage();
	afx_msg void OnMenuEnterDir();
	afx_msg void OnMenuRefresh();
	DECLARE_MESSAGE_MAP()
};

class CLASS_DECLSPEC CDHResultBox : public CDHListBox
{
	// Construction
public:
	CDHResultBox();		//0好图  1坏图  2空图   3异常图
	
	//{{AFX_VIRTUAL(CDHDiskBox)
	//}}AFX_VIRTUAL
	
	// Implementation

	void Refresh(CString sFilter=TEXT("*.*")){;}
	int InsertString(int iIndex, LPCTSTR lpszItem, int iImg)
	{return CDHListBox::InsertString(iIndex, lpszItem, iImg);}
	virtual ~CDHResultBox();
private:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDHResultBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
protected:
	// Generated message map functions
	//{{AFX_MSG(CDHDiskBox)
	//}}AFX_MSG
	
	DECLARE_MESSAGE_MAP()
};

class CLASS_DECLSPEC CDHModelBox : public CDHListBox
{
	// Construction
public:
	CDHModelBox();
	virtual ~CDHModelBox();

	void Init(CWnd* pParent, int nLang=1, CString sFilter=TEXT("*.*"));	//0:英文  1:中文
	void Refresh(CString sFilter = TEXT("*.*"));
	BOOL ApplyModel(char* chModelName);
private:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDHModelBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL
protected:
	CWnd* m_pParent;
	CString m_strAppDir, m_strModelDir, m_strCurModelName;

	void RenameModel(int nIndex);
	void DeleteModel(int nIndex);
	void DeleteWholeDir(CString& strDir);
	// Generated message map functions
	//{{AFX_MSG(CDHModelBox)
	//}}AFX_MSG
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMenuApply();
	afx_msg void OnMenuRename();
	afx_msg void OnMenuDeleteModel();
	afx_msg void OnMenuOpenDir();
	afx_msg void OnMenuRefreshModel();
	afx_msg void OnMenuOpenModelDir();
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_CCCCC_H__C3F44402_4572_4B2B_895B_E56C1D8CFEA0__INCLUDED_)
