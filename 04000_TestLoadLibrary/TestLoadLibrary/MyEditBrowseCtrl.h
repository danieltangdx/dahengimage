#pragma once


// CMyEditBrowseCtrl

class CMyEditBrowseCtrl : public CMFCEditBrowseCtrl
{
	DECLARE_DYNAMIC(CMyEditBrowseCtrl)

public:
	CMyEditBrowseCtrl();
	virtual ~CMyEditBrowseCtrl();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
};


