#if !defined(AFX_ALGSTOPPER_H__3D35A3B4_D261_9285_8EBD_714891F3AEF3__INCLUDED_)
#define AFX_ALGSTOPPER_H__3D35A3B4_D261_9285_8EBD_714891F3AEF3__INCLUDED_
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/*
* Copyright (c) 2017,北京大恒图像视觉有限公司医药检测事业部
* All rights reserved.
*
* 文件名称：alg_struct_stopper.h
* 文件标识：
* 摘要：算法与系统接口中使用的结构体为通用结构体，该结构体内容是固定且通用的。
		不同的项目中算法与系统人员可能需要按照自己项目需要去自定义结构体，该结构体文件
		即为药包事业部胶塞项目组自定义结构体，文件适用于(但不限于)胶塞，采血，PP项目。
* 当前版本：1.0
* 作者：
* 完成日期：2017年3月16日
*
*/

//=========================算法自定义结构体===================================
namespace Stopper
{
	//初始化	
	typedef struct _AlgInitParam
	{
		CDC	**pDC;		//窗口句柄
		int	nBayer[30];	//贝尔变换模式，支持30个相机  0无贝尔变换 1gb模式 2gr模式 3bg模式 4rg模式
		int	nRevert[30];//图像反转模式，支持30个相机  0不反转 1左右反转 2上下反转 3对角反转
	}s_AlgInitParam;


	//检测输入
	typedef struct _AlgDetectInput
	{
		int		nInSig;			//系统程序设置为0 !!!，非0状态为算法内部使用。
		int		bRunCheck;		//是否检测 0不检 1检测

		int		nImageWidth;	//图像宽
		int		nImageHeight;	//图像高
		int		nChannel;		//通道数
				
		int		nDisplayMode;	//显示模式： 0不显示 1显示所有 2显示坏品 3显示好品
		int		nSaveImageMode;	//存图模式： 0不存图 1保存所有 2保存错图 3保存有检测目标图
		char	chSaveImagePath[MAX_PATH];//存图路径
	}s_AlgDetectInput;
	//检测输出
	typedef struct _AlgDetectOutput
	{
		int	nResult;			//检测结果(单个目标时使用)
		int	szResults[50];		//检测结果(多个目标时使用，支持50个目标)
	}s_AlgDetectOutput;


	//算法设置
	typedef struct _AlgSetParam
	{
		char	chModelName[256];//模板名称

		//以下参数系统程序用不到
		CWnd*	pParent;
		CRect	rect;
		bool	bTopWin;
	}s_AlgSetParam;
}
#endif // !defined(AFX_ALGSTOPPER_H__3D35A3B4_D261_9285_8EBD_714891F3AEF3__INCLUDED_)