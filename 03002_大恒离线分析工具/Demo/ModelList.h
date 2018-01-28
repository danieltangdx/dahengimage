#if !defined(AFX_MODELLIST_H__424F53A8_B691_466C_86A7_98C54B6D7E67__INCLUDED_)
#define AFX_MODELLIST_H__424F53A8_B691_466C_86A7_98C54B6D7E67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ModelList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CModelList window

class CModelList : public CListBox
{
// Construction
public:
	CModelList();

// Attributes
protected:
	CImageList* m_pImageList;

// Operations
public:
	int AddString(LPCTSTR lpszItem);
	int AddString(LPCTSTR lpszItem, int iImg);
	int InsertString(int iIndex, LPCTSTR lpszItem);
	int InsertString(int iIndex, LPCTSTR lpszItem, int iImg);
	void SetItemImage(int iIndex, int iImg);
	inline void SetImageList(CImageList* pImgList = NULL)
	{ m_pImageList = pImgList; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CModelList)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CModelList();

	// Generated message map functions
protected:
	//{{AFX_MSG(CModelList)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MODELLIST_H__424F53A8_B691_466C_86A7_98C54B6D7E67__INCLUDED_)
