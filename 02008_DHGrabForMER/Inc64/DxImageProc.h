//--------------------------------------------------------------- 
/** 
\file      DxImageProc.h
\brief     Image Processing Library
\version   v1.0.1405.9221 
\date      2014-5-22
\author    Software Department 
<p>Copyright (c) 2012-2014 China Daheng Group, Inc. Beijing Image 
Vision Technology Branch and all right reserved.</p> 
*/ 
//--------------------------------------------------------------- 

#if !defined (_DXIMAGEPROC_H)         
#define _DXIMAGEPROC_H 			///< pre-compiled macro define

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32                   
#include "stdafx.h"
#define DHDECL __stdcall

#else                          
#include <stdlib.h>
#include <string.h>
#define  DHDECL
#endif

/// 类型定义
typedef char            VxInt8;      ///< 8位有符号整型
typedef short           VxInt16;     ///< 16位有符号整型
typedef long            VxInt32;     ///< 32位有符号整型
#ifdef _WIN32 
typedef __int64         VxInt64;     ///< 64位有符号整形
#else
typedef long long       VxInt64;     ///< 64位有符号整型
#endif
typedef unsigned char   VxUint8;     ///< 8位无符号整型
typedef unsigned short  VxUint16;    ///< 16位无符号整型
typedef unsigned int    VxUint32;    ///< 32位无符号整型

/// 状态码定义
typedef enum  tagDX_STATUS
{
	DX_OK                         = 0,    ///< 操作成功
	DX_PARAMETER_INVALID          = -101, ///< 输入参数无效
	DX_PARAMETER_OUT_OF_BOUND     = -102, ///< 参数越界
	DX_NOT_ENOUGH_SYSTEM_MEMORY   = -103, ///< 系统内存不足
	DX_NOT_FIND_DEVICE            = -104, ///< 没有找到设备
	DX_STATUS_NOT_SUPPORTED       = -105, ///< 不支持的格式
	DX_CPU_NOT_SUPPORT_ACCELERATE = -106  ///< CPU不支持加速
} DX_STATUS;

/// 像素bayer格式 pixel color filter
typedef enum  tagDX_PIXEL_COLOR_FILTER
{
	NONE    = 0,   ///< 非Bayer格式
	BAYERRG = 1,   ///< 第一行以RG开始
	BAYERGB = 2,   ///< 第一行以GB开始
	BAYERGR = 3,   ///< 第一行以GR开始
	BAYERBG = 4    ///< 第一行以BG开始
} DX_PIXEL_COLOR_FILTER;



/// 定义Bayer转换类型
typedef enum tagDX_BAYER_CONVERT_TYPE
{
	RAW2RGB_NEIGHBOUR  = 0,   ///< 邻域平均插值算法
	RAW2RGB_ADAPTIVE   = 1,   ///< 边缘自适应插值算法
	RAW2RGB_NEIGHBOUR3 = 2    ///< 区域更大的邻域平均算法
} DX_BAYER_CONVERT_TYPE;

/// 有效数据位
typedef enum tagDX_VALID_BIT
{ 
	DX_BIT_0_7	    = 0,    ///< bit 0~7
	DX_BIT_1_8	    = 1,    ///< bit 1~8
	DX_BIT_2_9	    = 2,    ///< bit 2~9
	DX_BIT_3_10	    = 3,    ///< bit 3~10
	DX_BIT_4_11	    = 4     ///< bit 4~11
} DX_VALID_BIT;

/// 图像实际位数
typedef enum tagDX_ACTUAL_BITS
{
	DX_ACTUAL_BITS_10 = 10,    ///< 10位
	DX_ACTUAL_BITS_12 = 12,    ///< 12位 
	DX_ACTUAL_BITS_14 = 14,    ///< 14位
	DX_ACTUAL_BITS_16 = 16     ///< 16位
} DX_ACTUAL_BITS;

/// 图像镜像翻转类型
typedef enum DX_IMAGE_MIRROR_MODE
{
	HORIZONTAL_MIRROR = 0,     ///< 水平镜像
	VERTICAL_MIRROR   = 1      ///< 垂直镜像
}DX_IMAGE_MIRROR_MODE;

///黑白图像处理功能设置结构体
typedef  struct  MONO_IMG_PROCESS
{
	bool            bDefectivePixelCorrect;    ///< 坏点校正开关
	bool            bSharpness;                ///< 锐化开关
	bool            bAccelerate;               ///< 加速使能
	float           fSharpFactor;              ///< 锐化强度因子
	VxUint8         *pProLut;                  ///< 查找表Buffer
	VxUint16        nLutLength;                ///< Lut Buffer长度
	VxUint8         arrReserved[32];           ///< 保留32字节
} MONO_IMG_PROCESS;

///彩色图像处理功能设置结构体
typedef  struct  COLOR_IMG_PROCESS
{
	bool                   bDefectivePixelCorrect; ///< 坏点校正开关
	bool                   bDenoise;               ///< 降噪开关
	bool                   bSharpness;             ///< 锐化开关
	bool                   bAccelerate;            ///< 加速使能
	VxInt16                *parrCC;                ///< 色彩处理参数数组地址
	VxUint8                nCCBufLength;           ///< parrCC长度（sizeof(VxInt16)*9）
	float                  fSharpFactor;           ///< 锐化强度因子
	VxUint8                *pProLut;               ///< 查找表Buffer
	VxUint16               nLutLength;             ///< Lut Buffer长度
    DX_BAYER_CONVERT_TYPE  cvType;                 ///< 插值方法选择
	DX_PIXEL_COLOR_FILTER  emLayOut;               ///< BAYER格式
	bool                   bFlip;                  ///< 翻转标志
	VxUint8                arrReserved[32];        ///< 保留32字节
} COLOR_IMG_PROCESS;

//--------------------------------------------------
/**
\brief  Raw8图像转换成RGB24图像
\param  pInputBuffer   	[in] 输入图像缓冲区
\param  pOutputBuffer   [out]输出图像缓冲区（new buffer）
\param  nWidth  	    [in] 图像宽度
\param  nHeight   	    [in] 图像高度
\param  cvtype          [in] 插值算法类型
\param  nBayerType      [in] bayer类型
\param  bFlip           [in] 图像翻转标记 true:翻转 false:不翻转

\return emStatus  
*/
//--------------------------------------------------
VxInt32 DHDECL DxRaw8toRGB24(void *pInputBuffer, void *pOutputBuffer, VxUint32 nWidth, VxUint32 nHeight, DX_BAYER_CONVERT_TYPE cvtype, DX_PIXEL_COLOR_FILTER nBayerType, bool bFlip);

//--------------------------------------------------
/**
\brief  Raw12Packed格式转化为Raw16
\param  pInputBuffer   	[in] 输入图像缓冲区
\param  pOutputBuffer   [out]输出图像缓冲区（new buffer）
\param  nWidth          [in] 图像宽度
\param  nHeight         [in] 图像高度                      

\return emStatus  
*/
//--------------------------------------------------
VxInt32 DHDECL DxRaw12PackedToRaw16(void* pInputBuffer, void* pOutputBuffer, VxUint32 nWidth, VxUint32 nHeight);

//-------------------------------------------------
/**
\brief  Raw10Packed格式转化为Raw16
\param  pInputBuffer   	[in] 输入图像缓冲区
\param  pOutputBuffer   [out]输出图像缓冲区（new buffer）
\param  nWidth          [in] 图像宽度
\param  nHeight         [in] 图像高度

\return emStatus
*/
//-------------------------------------------------
VxInt32 DHDECL DxRaw10PackedToRaw16(void* pInputBuffer, void* pOutputBuffer, VxUint32 nWidth, VxUint32 nHeight);

//------------------------------------------------
/**
\brief  8位图像顺时针旋转90度
\param  pInputBuffer  	[in] 输入图像缓冲区
\param  pOutputBuffer	[out]输出图像缓冲区（new buffer）  
\param  nWidth        	[in] 图像宽度
\param  nHeight       	[in] 图像高度

\return emStatus
*/
//------------------------------------------------
VxInt32 DHDECL DxRotate90CW8B(void* pInputBuffer, void* pOutputBuffer, VxUint32 nWidth, VxUint32 nHeight);

//------------------------------------------------
/**
\brief  8位图像逆时针旋转90度
\param  pInputBuffer    [in] 输入图像缓冲区
\param  pOutputBuffer	[out]输出图像缓冲区（new buffer）  
\param  nWidth          [in] 图像宽度
\param  nHeight         [in] 图像高度

\return emStatus
*/
//------------------------------------------------
VxInt32 DHDECL DxRotate90CCW8B(void* pInputBuffer, void* pOutputBuffer, VxUint32 nWidth, VxUint32 nHeight);

//-----------------------------------------------
/**
\brief  图像亮度调节(24位RGB图像或者8位灰度图像)
\param  pInputBuffer  	[in] 输入图像缓冲区      
\param  pOutputBuffer	[out]输出图像缓冲区  
\param  nImagesize    	[in] 原始图像的大小，以字节为单位(RGB:图像宽x图像高x3)
\param  nFactor        	[in] 调节因子，范围：-150~150

\return emStatus
*/
//-----------------------------------------------
VxInt32 DHDECL DxBrightness(void* pInputBuffer, void* pOutputBuffer, VxUint32 nImagesize, VxInt32 nFactor);

//--------------------------------------------------
/**
\brief  图像对比度调节(24位RGB图像或者8位灰度图像)
\param  pInputBuffer	[in] 输入图像缓冲区      
\param  pOutputBuffer	[out]输出图像缓冲区
\param  nImagesize      [in] 原始图像的大小，以字节为单位(RGB:图像宽x图像高x3)
\param  nFactor	        [in] 调节因子，范围：-50~100

\return emStatus
*/
//--------------------------------------------------
VxInt32 DHDECL DxContrast(void* pInputBuffer, void* pOutputBuffer, VxUint32 nImagesize, VxInt32 nFactor);

//--------------------------------------------------
/**
\brief  24位RGB图像锐化
\param  pInputBuffer  	[in] 输入图像缓冲区      
\param  pOutputBuffer 	[out]输出图像缓冲区
\param  nWidth        	[in] 图像宽度
\param  nHeight       	[in] 图像高度
\param  fFactor        	[in] 调节因子，范围：0.1~5.0

\return emStatus
*/
//--------------------------------------------------
VxInt32 DHDECL DxSharpen24B(void* pInputBuffer, void* pOutputBuffer, VxUint32 nWidth, VxUint32 nHeight, float fFactor);

//--------------------------------------------------
/**
\brief  24位RGB图像饱和度调节函数
\param  pInputBuffer   	[in] 数据输入buffer指针
\param  pOutputBuffer  	[out]数据输出buffer指针
\param  nImageSize     	[in] RGB图像尺寸 (图像宽x图像高)
\param  nFactor        	[in] 调节参数,范围：0 ~ 128

\return emStatus   
*/
//--------------------------------------------------
VxInt32 DHDECL DxSaturation(void* pInputBuffer, void* pOutputBuffer, VxUint32 nImagesize, VxInt32 nFactor);

//--------------------------------------------------
/**
\brief  获取白平衡系数函数，为了计算准确，相机照客观“白”区域，或者传入图像为“白色区域”
\param  pInputBuffer   	[in] 数据输入buffer指针（24bitRGB图像）
\param  nWidth        	[in] 图像宽度
\param  nHeight       	[in] 图像高度
\param  dRatioR         [out]R分量白平衡系数
\param  dRatioG         [out]G分量白平衡系数
\param  dRatioB         [out]B分量白平衡系数

\return emStatus  
*/
//--------------------------------------------------
VxInt32 DHDECL DxGetWhiteBalanceRatio(void *pInputBuffer, VxUint32 nWidth, VxUint32 nHeight, double* dRatioR, double* dRatioG, double* dRatioB);

//-----------------------------------------------------
/**
\brief  Raw图像自动坏点检测校正函数,支持8 ~ 16 位数据，这里，位数指实际位数，当超过8位时，数据占两个字节，
        但实际有效位数可能是 9 ~ 16的任意数，该函数进行适时检测校正，每帧均需调用,对于Packed格式，需先转换成raw16
\param  pRawImgBuf      [in,out]Raw图输入buffer指针
\param  nWidth        	[in]图像宽度
\param  nHeight       	[in]图像高度
\param  nBitNum         [in]图像数据的实际位数（注：若是10位数据则输入10，12位数据输入12，以此类推，值范围8 ~ 16），参考说明书

\return emStatus  
*/
//-----------------------------------------------------
VxInt32 DHDECL DxAutoRawDefectivePixelCorrect(void* pRawImgBuf, VxUint32 nWidth, VxUint32 nHeight, VxInt32 nBitNum);

//--------------------------------------------------
/**
\brief  Raw16图像(实际位数16位，其中有效位为10位或12位)转换成Raw8图像（实际位数8位，其中有效位同样为8位）
\param  pInputBuffer   	[in] 输入图像缓冲区（缓冲区大小为W*H*2字节）
\param  pOutputBuffer   [out]输出图像缓冲区（缓冲区大小为W*H字节）
\param  nWidth          [in] 图像宽度
\param  nHeight         [in] 图像高度
\param  nValidBits      [in] 有效位数

\return emStatus  
*/
//--------------------------------------------------
VxInt32 DHDECL DxRaw16toRaw8(void *pInputBuffer, void *pOutputBuffer, VxUint32 nWidth, VxUint32 nHeight, DX_VALID_BIT nValidBits);

//--------------------------------------------------
/**
\brief  RGB48图像转换成RGB24图像（每个RGB分量各占16位）
\param  pInputBuffer   	[in] 输入图像缓冲区（缓冲区大小为W*H*3*2字节）
\param  pOutputBuffer   [out]输出图像缓冲区（缓冲区大小为W*H*3字节）
\param  nWidth          [in] 图像宽度
\param  nHeight         [in] 图像高度
\param  nValidBits      [in] 有效位数

\return emStatus  
*/
//--------------------------------------------------
VxInt32 DHDECL DxRGB48toRGB24(void *pInputBuffer, void *pOutputBuffer, VxUint32 nWidth, VxUint32 nHeight, DX_VALID_BIT nValidBits);

//--------------------------------------------------
/**
\brief  Raw16图像(每个像素占16位)转换成RGB48图像（每个RGB分量各占16位）
\param  pInputBuffer   	[in] 输入图像缓冲区（缓冲区大小为W*H*2字节）
\param  pOutputBuffer   [out]输出图像缓冲区（缓冲区大小为W*H*3*2字节）
\param  nWidth          [in] 图像宽度
\param  nHeight         [in] 图像高度
\param  nActualBits     [in] 图像实际位数
\param  cvtype          [in] 插值算法类型
\param  nBayerType      [in] bayer类型
\param  bFlip           [in] 图像翻转标记 true:翻转 false:不翻转

\return emStatus  
*/
//--------------------------------------------------
VxInt32 DHDECL DxRaw16toRGB48(void *pInputBuffer, void *pOutputBuffer, VxUint32 nWidth, VxUint32 nHeight, DX_ACTUAL_BITS nActualBits, DX_BAYER_CONVERT_TYPE cvtype, DX_PIXEL_COLOR_FILTER nBayerType, bool bFlip);

//--------------------------------------------------
/**
\brief  计算对比度查表（仅支持24位RGB图像）
\param  nContrastParam  [in] 对比度调节参数，范围：-50~100
\param  pContrastLut    [out]对比度查表
\param  pLutLength      [out]获取的查找表长度（单位：字节）

\return emStatus  
*/
//--------------------------------------------------
VxInt32 DHDECL DxGetContrastLut(int nContrastParam, void *pContrastLut, int *pLutLength);

//--------------------------------------------------
/**
\brief  计算Gamma查表（仅支持24位RGB图像）
\param  dGammaParam   [in] Gamma调节参数，范围：(0.1 ~ 10) 
\param  pGammaLut     [out]Gamma查找表
\param  pLutLength    [out]获取的查找表长度（单位：字节）

\return emStatus  
*/
//--------------------------------------------------
VxInt32 DHDECL DxGetGammatLut(double dGammaParam, void *pGammaLut, int *pLutLength);

//--------------------------------------------------
/**
\brief  图像质量提升函数（仅支持24位RGB图像）
\param  pInputBuffer   	      [in] 输入图像缓冲区
\param  pOutputBuffer         [out]输出图像缓冲区
\param  nWidth                [in] 图像宽度
\param  nHeight               [in] 图像高度
\param  nColorCorrectionParam [in] 颜色校正参数
\param  pContrastLut          [in] 对比度查找表
\param  pGammaLut             [in] Gamma查找表

\return emStatus  
*/
//--------------------------------------------------
VxInt32 DHDECL DxImageImprovment(void *pInputBuffer, void *pOutputBuffer, VxUint32 nWidth, VxUint32 nHeight, VxInt64 nColorCorrectionParam, void *pContrastLut, void *pGammaLut);

//-------------------------------------------------------------
/**
\brief   图像镜像翻转函数（仅支持8位Raw图像和8位黑白图像） 
\param  pInputBuff   	[in] 输入图像缓冲区（缓冲区大小为W*H字节）
\param  pOutputBuf      [out]输出图像缓冲区（缓冲区大小为W*H字节）
\param  nWidth          [in] 图像宽度
\param  nHeight         [in] 图像高度
\param  emMirrorMode    [in] 图像镜像模式

\return emStatus
*/
//-------------------------------------------------------------
VxInt32 DHDECL DxImageMirror(void *pInputBuffer, void *pOutputBuffer, VxUint32 nWidth, VxUint32 nHeight, DX_IMAGE_MIRROR_MODE emMirrorMode);

//--------------------------------------------------
/**
\brief  计算图像处理8位查找表
\param  nContrastParam  [in] 对比度调节参数，范围：-50~100
\param  dGamma          [in] Gamma系数（0.1~10）
\param  nLightness      [in] 亮度调节参数（-150~150）
\param  pLut            [out]查找表
\param  pLutLength      [in] 查找表长度（单位：字节）

\return emStatus
*/
//--------------------------------------------------
VxInt32 DHDECL DxGetLut(VxInt32 nContrastParam, double dGamma, VxInt32 nLightness, VxUint8 *pLut, VxUint16 *pLutLength);

//--------------------------------------------------
/**
\brief  计算图像处理色彩调节数组
\param  nColorCorrectionParam   [in] 颜色校正参数buffer地址（从设备获取）
\param  nSaturation             [in] 饱和度调节系数（0~128）
\param  parrCC                  [out]数组地址
\param  nLength                 [in] 数组长度（sizeof(VxInt16)*9）

\return emStatus
*/
//--------------------------------------------------
VxInt32 DHDECL DxCalcCCParam(VxInt64 nColorCorrectionParam, VxInt16 nSaturation, VxInt16 *parrCC, VxUint8 nLength);

//--------------------------------------------------
/**
\brief  对Raw8图像进行处理
\param  pRaw8Buf    	      [in] Raw8输入图像缓冲区
\param  pRgb24Buf             [out]RGB图像输出缓冲区
\param  nWidth                [in] 图像宽度
\param  nHeight               [in] 图像高度
\param  pstClrImgProc         [in] 彩色图像处理方法结构体指针

\return emStatus  
*/
//--------------------------------------------------
VxInt32 DHDECL DxRaw8ImgProcess(void *pRaw8Buf, void *pRgb24Buf, VxUint32 nWidth, VxUint32 nHeight, COLOR_IMG_PROCESS *pstClrImgProc);

//--------------------------------------------------
/**
\brief  对Mono8图像进行处理
\param  pInputBuf    	      [in] 图像输入缓冲区
\param  pOutputBuf            [out]图像输出缓冲区
\param  nWidth                [in] 图像宽度
\param  nHeight               [in] 图像高度
\param  pstGrayImgProc        [in] 灰度图像处理方法结构体指针

\return emStatus  
*/
//--------------------------------------------------
VxInt32 DHDECL DxMono8ImgProcess(void *pInputBuf, void *pOutputBuf, VxUint32 nWidth, VxUint32  nHeight, MONO_IMG_PROCESS *pstGrayImgProc);

#ifdef __cplusplus
}
#endif

#endif




