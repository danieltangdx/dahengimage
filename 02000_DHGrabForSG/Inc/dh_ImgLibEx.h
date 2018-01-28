/************************************************************************/
/* Copyright (c) 2007, 中国大恒集团北京图像视觉技术分公司机器视觉算法部     
/* All rights reserved.													
/*																		
/* 文件名称： CPQIAssistant.h												
/* 文件标识： 
/* 摘要： 准确地计算时间
/*
/* 当前版本： 1.0
/* 作者： 李文峰
/* 完成日期： 2007年08月28日
/************************************************************************/

// CPQIAssistant.h : main header file for the CPQIASSISTANT DLL

#if !defined(AFX_CPQIASSISTANT_H_)
#define AFX_CPQIASSISTANT_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif
#include "vector"
using namespace std;
#include "resource.h"		// main symbols

#define DLL_EXT _declspec(dllexport)
#define RGB24  1
#define RGB32  2
#define RRGGBB 3
/////////////////////////////////////////////////////////////////////////////
// CCPQIAssistantApp
// See CPQIAssistant.cpp for the implementation of this class
//

typedef struct tagRectPara
{
   RECT rect;		// 匹配成功的MARK区域
   double fRatio;	// 匹配度
}MultiRect;

typedef struct tagImgPara
{
   int nWidth;		// 图像宽
   int nHeight;		// 图像高
   int nChannel;	// 图像通道数，目前只支持1和3
   PBYTE pImgbuf;	// 图像数据指针
}s_ImgData;

class CCPQIAssistantApp : public CWinApp
{
public:
	CCPQIAssistantApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCPQIAssistantApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCPQIAssistantApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	vector<MultiRect> m_vMultiRect;

};
/**********************************************************************
created:	28:8:2007 
function:   判断是否有进纸信号，原理为：输入的RGB三通道图像的平均灰度值大于设定的灰度值为
            有进纸信号，否则无（自己编写）
Input_para: const char * dataR, const char * dataG, const char * dataB 输入RGB图像的指针
			int nWidth, int nHeight 图像宽高
			RECT rect,              指定的ROI区域
			int nThresh             输入判断值
Output_para:
return:     有进纸返回true，否则返回false
***********************************************************************/
DLL_EXT bool  EnterPaper(const char * dataR, const char * dataG, const char * dataB, 
						int nWidth, int nHeight, RECT rect, int nThresh);
/**********************************************************************
created:	28:8:2007 
function:   判断是否有进纸信号，原理为：输入的RGB三通道图像的平均灰度值大于设定的灰度值为
            有进纸信号，否则无（用OpenCV）
Input_para: const char * dataR, const char * dataG, const char * dataB 输入RGB图像的指针
			int nWidth, int nHeight 图像宽高
			RECT rect,              指定的ROI区域
			int nThresh             输入判断值
Output_para:
return:     有进纸返回true，否则返回false
***********************************************************************/
DLL_EXT bool EnterPaper_OpenCv(char *dataR, char *dataG, char *dataB, 
							   int nWidth, int nHeight, RECT rect, int nThresh);

_declspec(dllexport) int PixToPlane(const char *srcRGB, char *dstR, char *dstG, 
									 char *dstB, int nWidth, int nHeight);

_declspec(dllexport) int  PlaneToPix(const char *srcR, const char *srcG, 
									 const char *srcB, char *dstRGB,  int nWidth,
									 int nHeight);
/************************************************************************/
/* 
功能：将RGB 图像分解为R、G、B三通道图像，将32位图像分解为为R、G、B三通道图像
      或者将RRGGBB图像分解为R、G、B三通图像
参数：const char *srcRGB 输入源图像的数据
	  char *dstR, char *dstG, char *dstB 输出图像R、G、B三通道图像
	  int nWidth, int nHeight输入图像的宽高
      const int nType分解图像的类型： RGB24  1：24位图像数据，RGB32  2：32位图像数据，
      RRGGBB 3：八位的灰度图像数据
    

                                                                     */
/************************************************************************/
bool _declspec(dllexport)RGBToGray(const char *srcRGB, char *dstR, char *dstG, 
					char *dstB, int nWidth, int nHeight, const int nType);


/*******************************************************************************
输入参数：    
	sImgData：  图像数据结构
	rcMark：    mark区域
	dMatchRedio:外部输入的最小匹配率
	nMarkExtend:mark区域搜索扩展范围
	nWNum：	    宽度方向个数
	nHNum：     高度方向个数
    nptLast：   最后一个MARK区域的左上角顶点大概坐标
输出参数：
	sRectVector：存储MultiRect结构的vector指针队列
返回值：bool 类型，true：搜索成功；false：搜索失败
***********************************************************************************/
DLL_EXT bool FindMultiRegion(const s_ImgData& sImgData,
				const RECT& rcMark,
				const double dMatchRedio,
				int nMarkExtend,
				int nWNum,
				int nHNum,
				const POINT& nptLast,
				vector<MultiRect> **sRectVector);

/*************[7/2/2007]*********************************************************
  功能：在一个区域内找多个mark
  const s_ImgData& sImgData   图像数据结构--参考具体结构体
  const RECT& rcMark, mark图像区域，在源图像中
  const RECT& rcSearchMark  mark图像搜索区域
  dMatchRedio:外部输入的最小匹配率
输出参数：
	sRectVector：存储MultiRect结构的vector指针队列表示找到mark的个数，具体参数见结构体
  返回值：bool 类型，true：搜索成功；false：搜索失败  
/*************[7/2/2007]*********************************************************/
DLL_EXT bool FindMultiMark(const s_ImgData& sImgData,
				const RECT& rcMark,
				const RECT& rcSearchMark,
				const double dMatchRedio,
				vector<MultiRect> **sRectVector);

/***************************************************************************************
 动态库使用说明：
 1、首先初始化动态库
 2、调用相关函数，其大小必须是初始化好的大小
 3、释放动态库资源
********************************************************************************************/

/*************[5/3/2007]******************************
二值化初始化
int nWidth, int nHeight  原始图像大小
/*************[5/3/2007]*******************************/
DLL_EXT bool Threhold_init(int nWidth, int nHeight); 
/*************[5/1/2007]******************************
功能：仿照halcon中dyn_threshold函数功能
      fDilation 仿照halcon中区域膨胀函数
	  bFillOrNot 仿照halcon中fill_up函数
 
bFillOrNot 0:不填充, 1:填充
fDilation  膨胀系数默认
char *ImageSrc  源图像，三个通道
char *ImageDst  二值化后图像，一个通道
RECT &ROI 感兴趣区域
int nChannel 选择的通道  1==R, 2==G, 3==B
int nFlag   const int DARK = 0；const int LIGHT = 1；

/*************[5/1/2007]*******************************/

DLL_EXT bool Dyn_Threshold_Image3(char *imageRGB,  char *imageDst, RECT &ROI, int nChannel, 
							int nDynOffset,  bool bFillOrNot, 
							int nSizeMean, float fDilation, int nFlag);
/*************[5/1/2007]******************************
与Dyn_Threshold_Image3函数类似

char *ImageSrc  源图像，单色图像
char *ImageDst  二值化后图像，单色图像
RECT &ROI 感兴趣区域
int nFlag   const int DARK = 0；const int LIGHT = 1；

/*************[5/1/2007]*******************************/


DLL_EXT bool Dyn_Threshold_Image(char *imageSrc,  char *imageDst,  RECT &ROI, 
							int nDynOffset,  bool bFillOrNot, 
							int nSizeMean, float fDilation, int nFlag);


/*************[5/1/2007]******************************
 
char *ImageSrc  源图像，单色图像
char *ImageDst  二值化后图像，单色图像
RECT &ROI 感兴趣区域
int nFlag   const int DARK = 0；const int LIGHT = 1；
int nLowThres, int nHighThres阈值范围【nLowThres,nHighThres】,
       如果nLowThres>nHighThres 则阈值范围为【0,nHighThres】and【nLowThres,255】
/*************[5/1/2007]*******************************/
DLL_EXT bool Bin_Threshold_Image(char *imageSrc,  char *imageDst,  RECT &ROI,
							    int nLowThres, int nHighThres, int nFlag);
/*************[5/1/2007]******************************
 
char *imageRGB  源图像，三个通道
char *imageDst  二值化后图像，单色图像
RECT &ROI 感兴趣区域
int nFlag   const int DARK = 0；const int LIGHT = 1；
int nLowThres, int nHighThres阈值范围【nLowThres,nHighThres】,
       如果nLowThres>nHighThres 则阈值范围为【0,nHighThres】and【nLowThres,255】
/*************[5/1/2007]*******************************/
DLL_EXT bool Bin_Threshold_Image3(char *imageRGB,  char *imageDst,  RECT &ROI, int nChannel,  
							      int nLowThres, int nHighThres, int nFlag);
/*************[6/7/2007]*******************************************************
 灰度化图像数据
 输入图像都是原始大小
/*************[6/7/2007]******************************************************/
DLL_EXT bool RGB_to_Gray(char* imageRGB, char* imageDst, RECT &ROI,
					     double dRatioR, double dRatioG, double dRatioB);
/*************[5/3/2007]********************************************************
释放分配的资源
/*************[5/3/2007]********************************************************/
DLL_EXT bool Threshold_Free();
/********************************************************************
	created:	2007/11/30
	file base:	CPQIAssistant
	file ext:	h
	author:		by lwf
    inputPara:  sImageMean 均值模板，sImageVary:偏差模板，sImageIn:小图像，
				cPoint：小图像在模板中的左顶点
				
    outPara:    sImageMean 均值模板，sImageVary:偏差模板
	
	purpose: 为服务器生成均值模板、偏差模板,同时兼容彩色、黑白	
*********************************************************************/
DLL_EXT bool UpDateModel(s_ImgData &sImageMean, s_ImgData &sImageVary,
		s_ImgData sImageIn, CPoint cPoint, bool bFlagR, bool bFlagG, bool bFlagB);

#endif // !defined(AFX_CPQIASSISTANT_H_)
