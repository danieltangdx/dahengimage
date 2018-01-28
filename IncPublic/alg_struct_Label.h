/************************************************************************/
/* Copyright (c) 2017, 北京大恒图像视觉有限公司  
/* All rights reserved.													

// 2017-04-05
//采血管标签特殊结构体定义

2017.04.07
郭嘉琦：重新编写采血管所用特殊结构体
/************************************************************************/


#if !defined(AFX_CHECK_H__3D35A3B4_D261_4116_8EBD_714892F3AEF3__INCLUDED_)
#define AFX_CHECK_H__3D35A3B4_D261_4116_8EBD_714892F3AEF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STATUS_STRING_MAXLENGTH	256			//描述错误信息的最大字符数
//函数返回状态类型
//2017.4.07 全删掉
#include <vector>
using namespace std;



namespace LabelCheck
{
	// 函数执行状态信息
	typedef struct _Status
	{
		int		nStatusType;									//返回状态类型
		char	chErrorInfo[STATUS_STRING_MAXLENGTH]{0};			//错误信息描述
		char    chErrorContext[STATUS_STRING_MAXLENGTH]{0};		//错误详细信息描述	
		void*   Empty;
	}s_Status;

	typedef struct _AlgSingleInit
	{
		int		nWidth;         //图像宽度
		int		nHeight;	    //图像高度
		CDC		**pDC;			//窗口句柄
		int		nCameraIndex;	//相机工位号
		int	nBayer;
		int	nRevert;
	}s_AlgSingleInit;

	//算法设置输入自定义结构体 // 接口 ！
	typedef struct _DemoAlgSetParam
	{
		char chModelName[STATUS_STRING_MAXLENGTH]{0};
		CWnd*	pParent;
		CRect	rect;
		bool	bTopWin;
		int	iCheckSN;
	}s_DemoAlgSetParam;


	// 检测输入参数 algorithm check input parameter
	typedef struct _AlgInspectionInputParam
	{
		int		nWidth;			//图像宽
		int		nHeight;		//图像高
		int		nChannel;		//通道数			<0时 为Halcon读图的数据
		int		nCamIndex;		//相机号			
		char	*pcImageData;	//图像数据	
		//void    *pImageInfo;	// pImageInfo传入s_CheckCameraInfo结构体指针首地址

		int		bRunCheck;		//是否检测 0不检 1检
		int		nInSig;			//设置为0 !!!是系统调用 1是内部调用	2只显示图3
		int		bDsp;			//0不显示 1全显示边缘 3显示错误图像边缘	101全显示填充 102错误填充 
		int		bDisplaySys;	//*[7/3/2017 ConanSteve]*:  1只显示系统图像，0只显示算法检测图像
		int     bDetectAlg;     //*[7/3/2017 ConanSteve]*:  1 表示用算法进行检测  0表示不用算法检测，同时也不检测是否有采血管，但可以显示系统图像
		int     nDispFirstorLast; //*[7/3/2017 ConanSteve]*:  1 表示算法显示中算法或系统显示第一张图，2表示算法显示中算法或系统显示最后一张图

		int	    nDisplayMode;	// 显示模式(0:不显示,1:显示所有,2:显示有采血管,3:显示好品,4:显示坏品,5:显示无瓶)

		int		nSaveImageMode{-1};	//存图模式： -1不存图 0保存所有 1保存错图 2保存有采血管的图
		int		iRunMode{ 0 };//**[8/2/2017 ConanSteve]**:  0：现场系统调用，1：模拟调用
		int     iCount;       //张数
		int     iCircleIndex;  //几个组  第几个采血管
		char	chSaveImagePath[MAX_PATH]{0}; // 存图路径
		//存图路径指定到文件夹，比如M:\SaveImages\AllImages\2016-10-14 09-38. 算法根据iCricleIndex和nSaveImageMode以及iCheckSN保存图像
			// 保存图片命名规则：format("%s\\Camera%d\\ProductID(%06d)\\Type(%d)_%02d.bmp",
											//cSavePath,iCheckSN+1,iCircleIndex,iSaveType,序号);
	}s_AlgInspInPara;

	// 检测输出参数 algorithm check output parameter
	typedef struct _CapsuleStatus
	{ 
		int		nStatus;     
//      错误类型：1工位
//  	sRightResult0="好品";//"好品"
// 		sErrorResult0="坏品";//"坏品"
// 		sErrorResult1="无采血管";
// 		sErrorResult2="单张图多管";


//      错误类型：2工位
//  	sRightResult0="好品";//"好品"
// 		sErrorResult0="坏品";//"坏品"
// 		sErrorResult1="无采血管";
// 		sErrorResult2="单张图多管";


		char	chInfo[255];	//角度信息
	}s_CapsuleStatus;





	typedef struct _AlgInspectionReslut
	{
		int		nAlgStatus;		//0:好 1:一组图（或单张）坏品 2:算法异常 -1:默认 3:空管
		char	chExpInfo[255];
		s_CapsuleStatus		sCapules[20];
		void*   Empty;
		int		iReserve1; ////将对象号返回   m_nObjIndex
		int		iDefectCount;// 缺陷个数
		int     nTimeTotle; //总时间
		int     nStopMachineType[20]{ 0 };// 0：无标签(棍子不转)  1：重签   值0不停机 1停机 [12/20/2017 NeverMore]
	}s_AlgInspResult;
}

#endif // !defined(AFX_CHECK_H__3D35A3B4_D261_4116_8EBD_714892F3AEF3__INCLUDED_)