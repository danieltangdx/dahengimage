// ObjAlloc.h: interface for the CObjAlloc class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJALLOC_H__8642FBEB_F57C_4058_8912_DD030E209D81__INCLUDED_)
#define AFX_OBJALLOC_H__8642FBEB_F57C_4058_8912_DD030E209D81__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "alg_struct.h"
using namespace Alg;

#define	_EXPORTING

#ifdef _EXPORTING
#define CLASS_DECLSPEC    __declspec(dllexport)  //程序编译时使用
#else
#define CLASS_DECLSPEC    __declspec(dllimport)	 //动态库应用时使用
#endif

class CLASS_DECLSPEC CObjAlloc  
{
public:
	CObjAlloc();
	virtual ~CObjAlloc();
	
	s_Status Initialize(s_AlgInitPara sInitPara);								//初始化
	s_Status ReadModel(s_AlgModelInput sModelIn);								//模板设置
	s_Status Inspect(s_AlgInspInPara& sInspectIn, s_AlgInspResult &sInspectOut);//检测函数
	s_Status Free();															//释放资源（可不调用，类析构函数自动调用）
	void     ShowInfoDlg(int nCmdShow);											//SW_SHOW or SW_HIDE
	/*
	nCameraIndex 一律从0开始
	*/
protected:
	void* m_pInspObj;
};

#endif // !defined(AFX_OBJALLOC_H__8642FBEB_F57C_4058_8912_DD030E209D81__INCLUDED_)
