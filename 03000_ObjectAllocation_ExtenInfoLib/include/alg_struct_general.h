/************************************************************************/
/* Copyright (c) 2017, 北京大恒图像视觉有限公司  
/* All rights reserved.													

// 2017-04-05
//采血管标签特殊结构体定义

2017.04.07
郭嘉琦：重新编写采血管所用特殊结构体
/************************************************************************/


#pragma once
#include "alg_struct.h"
#include "vector"
using namespace std;

namespace General 
{
	class CAlgSingleInitParam
	{
	public:
		int iSize;//**[10/10/2017 ConanSteve]**:  类的内存大小
		int iStation;//**[8/30/2017 ConanSteve]**:  相机工位号
		int	iCallType;//**[8/2/2017 ConanSteve]**:  0：检测端多对象现场调用，1：DEMO单对象调用	2:检测端多对象模拟调用
		int iWindowNum;//**[10/18/2017 ConanSteve]**:  单个工位窗口的个数。
		int* p_iCount;//**[10/10/2017 ConanSteve]**:  每个窗口多少张图
		int* p_iWidth;//**[8/30/2017 ConanSteve]**:  图像宽度
		int* p_iHeight;//**[8/30/2017 ConanSteve]**:  图像高度
		int* p_iBayer;//**[8/30/2017 ConanSteve]**:  贝尔变换类型
		int* p_iRevert;//**[8/30/2017 ConanSteve]**:  翻转类型
		HWND* hWndCamImg;//**[8/30/2017 ConanSteve]**:  系统图像句柄数组首地址
		HWND* hWndAlgImg;//**[8/30/2017 ConanSteve]**:  算法图像句柄数组首地址
		HWND* hWndAlgScaleImg;// 算法缩放小窗口 [10/23/2017 NeverMore]

		CAlgSingleInitParam()
		{ 
			iSize = sizeof(CAlgSingleInitParam);
			iStation = 0;
			iCallType = 0;
			iWindowNum = 0;
			p_iCount = NULL;
			p_iWidth = 0;
			p_iHeight = 0;
			p_iBayer = 0;
			p_iRevert = 0;
			hWndCamImg = NULL;
			hWndAlgImg = NULL;
			hWndAlgScaleImg = NULL;
		}
	};

	/*************************************<算法检测参数类>************************************[8/30/2017 ConanSteve]*/
	enum e_DisplayImgTypeOfSystem//**[11/21/2017 ConanSteve]**:   0都不显示，1只显示系统图像，2只显示算法检测图像,3两者都显示
	{
		e_DisplayImgTypeOfSystem_NONE,//**[11/21/2017 ConanSteve]**:  都不显示
		e_DisplayImgTypeOfSystem_Sys,//**[11/21/2017 ConanSteve]**:  只显示系统图像
		e_DisplayImgTypeOfSystem_Alg,//**[11/21/2017 ConanSteve]**:  只显示算法检测图像
		e_DisplayImgTypeOfSystem_Both//**[11/21/2017 ConanSteve]**:  两者都显示
	};
	class CAlgInspInputParam
	{
	public:
		int iSize;
		bool	bExecuteAlgCheck;//**[10/10/2017 ConanSteve]**:  算法是否执行真正的算法检测，如果为0，那么算法在Detect时，直接默认返回结果0，表示好品
		int		iDisplayImgMode;			//**[10/10/2017 ConanSteve]**:  从0 开始，只能是正值，需要显示图像对应的检测结果值：具体值是多少以及显示关系，读取ParamGrid配置文件, 比如显示好品/不显示/坏品等
		int		iDisplayImgTypeOfSystem;	//*[7/3/2017 ConanSteve]*:  0都不显示，1只显示系统图像，2只显示算法检测图像,3两者都显示
		int		iSaveImageMode;	//**[10/10/2017 ConanSteve]**:  存图模式： 0不存图 其他值，根据ParamGrid配置文件来配置
		TCHAR(*lpszSaveImagePath)[MAX_PATH] ;	//**[1/5/2018 ConanSteve]**:  存图路径数组，可支持单工位多相机:
														//**[1/5/2018 ConanSteve]**:  这是个指针，指针指向一个数组的首地址，这个数组中的每个元素又是另外一个字符数组(真正的存图路径)的首地址，
														//**[1/5/2018 ConanSteve]**:  这个1维数组的长度为当前工位的窗口个数，即实际相机的个数
	public:
		CAlgInspInputParam()
		{ 
			iSize = sizeof(CAlgInspInputParam);
			bExecuteAlgCheck = true;
			iDisplayImgTypeOfSystem = e_DisplayImgTypeOfSystem_Both;
			iSaveImageMode = 0;
			lpszSaveImagePath = NULL;
		}

	};

	enum e_AlgStatus {
		e_AlgStatus_Good,//**[12/5/2017 ConanSteve]**:  好品
		e_AlgStatus_Bad,//**[12/5/2017 ConanSteve]**:  坏品
		e_AlgStatus_AlgException,//**[12/5/2017 ConanSteve]**:  算法异常
		e_AlgStatus_EmptyNoKick,//**[12/5/2017 ConanSteve]**:  空产品，不剔废，目前项目都是空不踢
		e_AlgStatus_EmptyKick,//**[12/5/2017 ConanSteve]**:  空产品且需要剔废
		e_AlgStatus_LostImg,//**[12/5/2017 ConanSteve]**:  多对象丢图
		e_AlgStatus_Overtime//**[12/5/2017 ConanSteve]**:  检测超时
	};

	class CAlgInspResult
	{
	public:
		//**[8/30/2017 ConanSteve]**:  具体对应什么意思看配置文件如何配置
		//**[12/4/2017 ConanSteve]**:  0是好品，1是坏品，2是空品，3是算法异常
		e_AlgStatus	iAlgStatus;
		int iSize;
		int iTimeTotle; //**[8/30/2017 ConanSteve]**:  检测总时间
		int iRsltNum;//**[11/24/2017 ConanSteve]**:  检测结果个数
		e_AlgStatus arrAlgStatus[100];//**[11/24/2017 ConanSteve]**: 
		int	iLenOfDefect;//**[10/10/2017 ConanSteve]**:  缺陷描述的长度
		int arrHeadOfDefectDesc[256];//**[8/30/2017 ConanSteve]**:  缺陷描述
		bool bStopMachine[40];//**[1/9/2018 ConanSteve]**:  检测停机
		bool bAlarm[40];//**[1/9/2018 ConanSteve]**:  报警
	public:
		CAlgInspResult() 
		{
			iAlgStatus = e_AlgStatus_Good;
			iSize = sizeof(CAlgInspResult);
			iTimeTotle = 0;
			iRsltNum = 1;
			iLenOfDefect = 0;
			memset(arrAlgStatus, 0, sizeof(arrAlgStatus));
			memset(arrHeadOfDefectDesc, 0, sizeof(arrHeadOfDefectDesc)); 
			memset(bStopMachine, 0, sizeof(bStopMachine));
			memset(bAlarm, 0, sizeof(bAlarm));
		}

	};
}




/*************************************<算法初始化参数类>************************************[8/30/2017 ConanSteve]*/


