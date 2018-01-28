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
	
    // 初始化
	s_Status Init(s_CheckInitParam sInitParam);//含窗口初始化+读算法参数模板+检测参数

	// 释放资源
	s_Status Free();

	// 进入检测线程 
	s_Status Detect(s_CheckInParam sCheckInParam);
	
	// 参数设置
	s_Status ShowDetail(int nCheckNS);
	
	s_Status Set(s_AlgModelInput sAlgModelInput);//读参数
protected:
	void* m_pInspObj;
};

#endif // !defined(AFX_OBJALLOC_H__8642FBEB_F57C_4058_8912_DD030E209D81__INCLUDED_)
