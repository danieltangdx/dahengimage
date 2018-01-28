
#if !defined(AFX_CINSPCLASS_H__B99533A4_69F3_46D1_8C50_DE5054773748__INCLUDED_)
#define AFX_CINSPCLASS_H__B99533A4_69F3_46D1_8C50_DE5054773748__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "alg_struct.h"
using namespace Alg;

class CInspBase
{
public:
	CInspBase(){}														//构造
	virtual ~CInspBase(){}												//析构
	// 初始化
	virtual s_Status Init(s_CheckInitSingleParam sInitParam) = 0;//含窗口初始化+读参数

	// 释放资源
	virtual s_Status Free() = 0;

	// 检测函数
	virtual s_Status Detect(s_CheckInParam sCheckInParam, s_CheckOutParam* sCheckOutParam) = 0;

	// 参数设置
	virtual s_Status ShowDetail() = 0;

	virtual s_Status Set(s_AlgModelInput sAlgModelInput) = 0;//显示图像，读参数及特殊功能				
protected:
	void* m_pDetect;
};

#endif // !defined(AFX_CINSPCLASS_H__B99533A4_69F3_46D1_8C50_DE5054773748__INCLUDED_)
//函数指针
//typedef CInspBase* (WINAPI* create_instance)(int nNumObj);
//typedef void (WINAPI* destroy_instance)(CInspBase* pCheck);