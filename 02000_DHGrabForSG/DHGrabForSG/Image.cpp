/************************************************************************/
/* Copyright (c) 2005, 中国大恒集团北京图像视觉技术分公司视觉技术部     
/* All rights reserved.													
/*																		
/* 文件名称： Image.cpp												
/* 文件标识： 见配置管理计划书
/* 摘要： CImage类, 对表现一个图像的各数据结构进行封装, 实现了图像拷贝,
/*    图像显示, 图像存盘, 图像剪切等功能
/*
/* 当前版本： 1.0
/* 作者： 江卓佳
/* 完成日期： 2005年10月10日
/************************************************************************/

#include "stdafx.h"
#include "Image.h"
#include "FileOperate.h"
//#include "SplitMerge.h"
#include "dh_ImgLibEx.h"

CImage::CImage()
{
	m_pImageBuff = NULL;	//图像缓冲区
	m_lBuffSize = 0;		//图像数据大小

	m_lImageWidth = 0;		//图像宽度
	m_lImageHeight = 0;		//图像长度
	m_lImageBitCount = 0;	//象素位数
	m_lImagePixelSize = 0;	//象素大小（单位：字节）
	m_lImageSize = 0;		//图像数据大小
	
	//自己内部的
	m_pBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 255*sizeof(RGBQUAD)];
    m_pBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_pBitmapInfo->bmiHeader.biPlanes = 1;
    m_pBitmapInfo->bmiHeader.biBitCount = 0;
    m_pBitmapInfo->bmiHeader.biCompression = BI_RGB;
    m_pBitmapInfo->bmiHeader.biSizeImage = 0;
    m_pBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
    m_pBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
    m_pBitmapInfo->bmiHeader.biClrUsed = 0;
    m_pBitmapInfo->bmiHeader.biClrImportant = 0;
    m_pBitmapInfo->bmiHeader.biWidth = 0 ;
    m_pBitmapInfo->bmiHeader.biHeight = 0 ;
	for (int i = 0 ; i < 256 ; i++)
	{
		m_pBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
		m_pBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
		m_pBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
		m_pBitmapInfo->bmiColors[i].rgbReserved = 0;
	}
	
	//公共显示用的
	m_pShowBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 255*sizeof(RGBQUAD)];
    m_pShowBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_pShowBitmapInfo->bmiHeader.biPlanes = 1;
    m_pShowBitmapInfo->bmiHeader.biBitCount = 0;
    m_pShowBitmapInfo->bmiHeader.biCompression = BI_RGB;
    m_pShowBitmapInfo->bmiHeader.biSizeImage = 0;
    m_pShowBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
    m_pShowBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
    m_pShowBitmapInfo->bmiHeader.biClrUsed = 0;
    m_pShowBitmapInfo->bmiHeader.biClrImportant = 0;
    m_pShowBitmapInfo->bmiHeader.biWidth = 0 ;
    m_pShowBitmapInfo->bmiHeader.biHeight = 0 ;
	for (int i = 0 ; i < 256 ; i++)
	{
		m_pShowBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
		m_pShowBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
		m_pShowBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
		m_pShowBitmapInfo->bmiColors[i].rgbReserved = 0;
	}
	
	//公共保存用的
	m_pSaveBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 255*sizeof(RGBQUAD)];
    m_pSaveBitmapInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    m_pSaveBitmapInfo->bmiHeader.biPlanes = 1;
    m_pSaveBitmapInfo->bmiHeader.biBitCount = 0;
    m_pSaveBitmapInfo->bmiHeader.biCompression = BI_RGB;
    m_pSaveBitmapInfo->bmiHeader.biSizeImage = 0;
    m_pSaveBitmapInfo->bmiHeader.biXPelsPerMeter = 0;
    m_pSaveBitmapInfo->bmiHeader.biYPelsPerMeter = 0;
    m_pSaveBitmapInfo->bmiHeader.biClrUsed = 0;
    m_pSaveBitmapInfo->bmiHeader.biClrImportant = 0;
    m_pSaveBitmapInfo->bmiHeader.biWidth = 0 ;
    m_pSaveBitmapInfo->bmiHeader.biHeight = 0 ;
	for (INT i = 0 ; i < 256 ; i++)
	{
		m_pSaveBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
		m_pSaveBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
		m_pSaveBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
		m_pSaveBitmapInfo->bmiColors[i].rgbReserved = 0;
	}
}

CImage::~CImage()
{
	if(m_pImageBuff != NULL)
	{
		delete []m_pImageBuff;
		m_pImageBuff = NULL;
		m_lBuffSize = 0;
	}

	if (m_pBitmapInfo != NULL) 
	{
		delete []m_pBitmapInfo;
		m_pBitmapInfo = NULL;
	}
	
	if (m_pShowBitmapInfo != NULL) 
	{
		delete []m_pShowBitmapInfo;
		m_pShowBitmapInfo = NULL;
	}
	
	if (m_pSaveBitmapInfo != NULL) 
	{
		delete []m_pSaveBitmapInfo;
		m_pSaveBitmapInfo = NULL;
	}
}

void CImage::Init(long lImageBuffSize)
{
	ASSERT(lImageBuffSize >= 0);

	m_lImageWidth = 0;	//图像宽度
	m_lImageHeight = 0;	//图像长度
	m_lImageBitCount = 0;	//象素位数
	m_lImagePixelSize = 0;	//象素大小（单位：字节）
	m_lImageSize = 0;	//图像数据大小
	
	m_pBitmapInfo->bmiHeader.biWidth = m_lImageWidth;
	m_pBitmapInfo->bmiHeader.biHeight = m_lImageHeight;
	m_pBitmapInfo->bmiHeader.biBitCount = (WORD)m_lImageBitCount;

	if(m_pImageBuff != NULL)
	{
		delete []m_pImageBuff;
		m_pImageBuff = NULL;
		m_lBuffSize = 0;
	}
	
	m_pImageBuff = new BYTE[lImageBuffSize];
	if(m_pImageBuff == NULL)
	{
		m_lBuffSize = 0;
		throw(CImageException(1, "内存分配不成功", "Init()"));
		return;
	}
	else
	{
		m_lBuffSize = lImageBuffSize;
	}
	
}

void CImage::SetImageSize(long lImageWidth, long lImageHeight, long lImageBitCount)
{
	ASSERT(lImageWidth >= 0 && lImageHeight >= 0 && lImageBitCount > 0);
	
	long lImageSize = lImageWidth * lImageHeight * ((lImageBitCount+7)/8);
	
	if(lImageSize > m_lBuffSize)
	{
		throw(CImageException(2, "参数不合法", "SetImageSize()"));
		return;
	}
	
	m_lImageWidth = lImageWidth;				//图像宽度
	m_lImageHeight = lImageHeight;				//图像长度
	m_lImageBitCount = lImageBitCount;			//象素位数
	m_lImagePixelSize = (lImageBitCount+7)/8;	//象素大小（单位：字节）
	m_lImageSize = lImageSize;					//图像数据大小
	
	m_pBitmapInfo->bmiHeader.biWidth = m_lImageWidth;
	m_pBitmapInfo->bmiHeader.biHeight = m_lImageHeight;
	m_pBitmapInfo->bmiHeader.biBitCount = (WORD)m_lImageBitCount;
}

void CImage::AcceptImage(const BYTE* pSrcImageBuff, long lSrcImageWidth, long lSrcImageHeight, long lSrcImageBitCount)
{
	ASSERT(pSrcImageBuff != NULL && lSrcImageWidth >= 0 && lSrcImageHeight >= 0 && lSrcImageBitCount > 0);

	long lImageSize = lSrcImageWidth * lSrcImageHeight * (lSrcImageBitCount/8);
	if(lImageSize > m_lBuffSize)
	{
		throw(CImageException(2, "参数不合法", "AcceptImage()"));
	}
		
	memcpy(m_pImageBuff, pSrcImageBuff, lImageSize);
	m_lImageWidth = lSrcImageWidth;
	m_lImageHeight = lSrcImageHeight;
	m_lImageBitCount = lSrcImageBitCount;
	m_lImagePixelSize = ((lSrcImageBitCount+7)/8);
	m_lImageSize = lImageSize;	
		
	m_pBitmapInfo->bmiHeader.biWidth = m_lImageWidth;
	m_pBitmapInfo->bmiHeader.biHeight = m_lImageHeight;
	m_pBitmapInfo->bmiHeader.biBitCount = (WORD)m_lImageBitCount;
}

BYTE* CImage::GetImageBuffAddr()
{
	return m_pImageBuff;
}
long CImage::GetImageBuffSize()
{
	return m_lBuffSize;
}
long CImage::GetImageWidth()
{
	return m_lImageWidth;
}
long CImage::GetImageHeight()
{
	return m_lImageHeight;
}
long CImage::GetImageBitCount()
{
	return m_lImageBitCount;
}
long CImage::GetImagePixelSize()
{
	return m_lImagePixelSize;
}
long CImage::GetImageSize()
{
	return m_lImageSize;
}
const BITMAPINFO* CImage::GetBitmapInfo()
{
	return m_pBitmapInfo;
}

//void CImage::ShowMe(CDC* pDC, CRect Rect, enum e_ShowModeFlip ShowModeFlip, enum e_ShowModeStretch ShowModeStretch)
void CImage::ShowDIBImage(CDC* pDC, CRect Rect, e_ShowModeFlip ShowModeFlip, e_ShowModeStretch ShowModeStretch,
		const BYTE* pImageBuff, long lImageWidth, long lImageHeight, long lImageBitCount)
{
	int XDest = 0;
	int YDest = 0;
	int nDestWidth = 0;
	int nDestHeight = 0;

	ASSERT(pImageBuff != NULL && lImageWidth >= 0 && lImageHeight >= 0 && lImageBitCount > 0);

	m_csShowBitmapInfoSection.Lock();
	
	m_pShowBitmapInfo->bmiHeader.biWidth = lImageWidth;
	m_pShowBitmapInfo->bmiHeader.biHeight = lImageHeight;
	m_pShowBitmapInfo->bmiHeader.biBitCount = (WORD)lImageBitCount;

	if(ShowModeStretch == NoStretch)
	{
		nDestWidth = lImageWidth;
		nDestHeight = lImageHeight;
	}
	else
	{
		nDestWidth = Rect.Width();
		nDestHeight = Rect.Height();
	}
	
	switch(ShowModeFlip) 
	{
	case NoFlip:			//不翻转
		XDest = Rect.left;
		YDest = Rect.top;
		break;
	case FlipHorizintal:	//水平翻转
		XDest = Rect.right;
		YDest = Rect.top;
		nDestWidth = -nDestWidth;
		break;
	case FlipVertical:		//垂直翻转
		m_pBitmapInfo->bmiHeader.biHeight = -lImageHeight;
		XDest = Rect.left;
		YDest = Rect.top;
		break;
	case Flip:				//全翻转
		m_pBitmapInfo->bmiHeader.biHeight = -lImageHeight;
		XDest = Rect.right;
		YDest = Rect.top;
		nDestWidth = -nDestWidth;
		break;
	default:				//不翻转
		XDest = Rect.left;
		YDest = Rect.top;
	}

	SetStretchBltMode(pDC->m_hDC, COLORONCOLOR);
	
	StretchDIBits(pDC->GetSafeHdc(),
		XDest,
		YDest,	
		nDestWidth,				//显示窗口宽度
		nDestHeight,			//显示窗口高度
		0,
		0,
		lImageWidth,			//图像宽度
		lImageHeight,			//图像高度
		pImageBuff,				//图像缓冲区
		m_pShowBitmapInfo,			//BMP图像描述信息
		DIB_RGB_COLORS,
		SRCCOPY
		);

	m_csShowBitmapInfoSection.Unlock();
}

//void CImage::SaveMe(const CString& strFileName)
void CImage::SaveDIBImageToBMPFile(const CString& strFileName, const BYTE* pImageBuff, long lImageWidth, long lImageHeight, long lImageBitCount)
{
	BOOL bRet = FALSE;

	ASSERT(pImageBuff != NULL && lImageWidth >= 0 && lImageHeight >= 0 && lImageBitCount > 0);
	ASSERT((strFileName.GetLength() >= 5) && (strFileName.Right(4) == ".bmp"));

	m_csSaveBitmapInfoSection.Lock();
	
	m_pSaveBitmapInfo->bmiHeader.biWidth = lImageWidth;
	m_pSaveBitmapInfo->bmiHeader.biHeight = lImageHeight;
	m_pSaveBitmapInfo->bmiHeader.biBitCount = (WORD)lImageBitCount;

	bRet = MyWriteBMPFile(strFileName, m_pSaveBitmapInfo, pImageBuff);

	m_csSaveBitmapInfoSection.Unlock();

	if(!bRet)
	{
		throw(CImageException(3, "文件保存不成功", "SaveDIBImageToBMPFile()"));
		return;
	}	
}

//裁剪函数(适用于DIB图像数据)(以原图像左上角为原点, x向右, y向下)
//目标缓冲区//剪切矩形
//返回值: TRUE: 成功
//BOOL CImage::CutMe(BYTE *pDest, CRect DestRect)
void CImage::CutDIBImage(BYTE *pDest, CRect DestRect, const BYTE* pSrcImageBuff, long lSrcImageWidth, long lSrcImageHeight, long lSrcImageBitCount)
{
	//注: DIB图像数据的存储方式是: 原图像的第一行存储在DIB图像数据缓冲区的最后一行, 缓冲区从下往上逐行存储.
	
	ASSERT(pDest != NULL && pSrcImageBuff != NULL && lSrcImageWidth >= 0 && lSrcImageHeight >= 0 && lSrcImageBitCount > 0);

	int nOffset_Source = 0;//相对于缓冲区头部偏移的字节数(源)
	int nOffset_Dest = 0;  //相对于缓冲区头部偏移的字节数(目标)

	int CutPositionX = DestRect.left;
	int CutPositionY = DestRect.top;
	int CutWidth = DestRect.Width();
	int CutHeight = DestRect.Height();

	int nPixelByteCount = (lSrcImageBitCount+7)/8;//一个象素用几个字节表示(RGB:3)
	
	//参数检查
	if((CutPositionX + CutWidth) > lSrcImageWidth || (CutPositionY + CutHeight) > lSrcImageHeight)
	{
		throw(CImageException(2, "参数不合法", "CutDIBImage()"));
		return;
	}
	
	//进行剪切起始行的转换
	int nBeginLine = lSrcImageHeight - CutPositionY - CutHeight;
	
	//去到DIB图像数据缓冲区的剪切行的起始处
	nOffset_Source = (lSrcImageWidth * nBeginLine + CutPositionX) * nPixelByteCount;
	
	for (int i = 0; i < CutHeight; i++)//切多少行
	{
		memcpy(pDest + nOffset_Dest, pSrcImageBuff + nOffset_Source, CutWidth*nPixelByteCount);
		
		nOffset_Dest += (CutWidth * nPixelByteCount);  //下一行(目标)
		
		nOffset_Source += (lSrcImageWidth*nPixelByteCount);  //下一行(源)
	}
}

void CImage::CutPlaneRGBImage(BYTE *pDest, CRect DestRect, const BYTE* pSrcImageBuff, long lSrcImageWidth, long lSrcImageHeight)
{	
	ASSERT(pDest != NULL && pSrcImageBuff != NULL && lSrcImageWidth >= 0 && lSrcImageHeight >= 0);

	int nOffset_Source = 0;//相对于缓冲区头部偏移的字节数(源)
	int nOffset_Dest = 0;  //相对于缓冲区头部偏移的字节数(目标)

	int CutPositionX = DestRect.left;
	int CutPositionY = DestRect.top;
	int CutWidth = DestRect.Width();
	int CutHeight = DestRect.Height();
	
	//参数检查
	if((CutPositionX + CutWidth) > lSrcImageWidth || (CutPositionY + CutHeight) > lSrcImageHeight)
	{
		throw(CImageException(2, "参数不合法", "CutDIBImage()"));
		return;
	}
	
	//进行剪切起始行的转换
	int nBeginLine = lSrcImageHeight - CutPositionY - CutHeight;
	
	//去到DIB图像数据缓冲区的剪切行的起始处
	nOffset_Source = lSrcImageWidth * nBeginLine + CutPositionX;
	
	for (int i = 0; i < CutHeight; i++)//切多少行
	{
		memcpy(pDest + nOffset_Dest, pSrcImageBuff + nOffset_Source, CutWidth);					//copy一行B的数据
		memcpy(pDest + nOffset_Dest + CutHeight * CutWidth,
			pSrcImageBuff + nOffset_Source + lSrcImageWidth * lSrcImageHeight, CutWidth);		//copy一行G的数据
		memcpy(pDest + nOffset_Dest + CutHeight * CutWidth * 2,
			pSrcImageBuff + nOffset_Source + lSrcImageWidth * lSrcImageHeight * 2, CutWidth);	//copy一行R的数据
		
		nOffset_Dest += CutWidth;  //下一行(目标)
		
		nOffset_Source += lSrcImageWidth;  //下一行(源)
	}
}

void CImage::ConvertRGBToPlaneR_G_B(BYTE *pDestImageBuffR, BYTE *pDestImageBuffG, BYTE *pDestImageBuffB, 
									const BYTE* pSrcImageBuff, long lSrcImageWidth, long lSrcImageHeight)
{
	ASSERT(pDestImageBuffR != NULL && pDestImageBuffG != NULL && pDestImageBuffB != NULL
		&& pSrcImageBuff != NULL && lSrcImageWidth >= 0 && lSrcImageHeight >= 0);

	//RGB -> R、G、B
	PixToPlane((const char *)pSrcImageBuff,
		(char *)pDestImageBuffR, (char *)pDestImageBuffG, (char *)pDestImageBuffB, 
		lSrcImageWidth, lSrcImageHeight);
}

void CImage::ConvertPlaneR_G_BToRGB(BYTE* pDestImageBuff, const BYTE *pSrcImageBuffR, const BYTE *pSrcImageBuffG, const BYTE *pSrcImageBuffB, 
									long lSrcImageWidth, long lSrcImageHeight)
{
	ASSERT(pDestImageBuff != NULL
		&& pSrcImageBuffR != NULL && pSrcImageBuffG != NULL && pSrcImageBuffB != NULL
		&& lSrcImageWidth >= 0 && lSrcImageHeight >= 0);

	//R、G、B -> RGB
	PlaneToPix((const char *)pSrcImageBuffR, (const char *)pSrcImageBuffG, (const char *)pSrcImageBuffB,
		(char *)pDestImageBuff, 
		lSrcImageWidth, lSrcImageHeight);
}

void CImage::ConvertRGBToPlaneRGB(BYTE *pDestPlaneRGBBuff,	const BYTE* pSrcImageRGBBuff, long lSrcImageWidth, long lSrcImageHeight)
{
	ASSERT(pDestPlaneRGBBuff != NULL && pSrcImageRGBBuff != NULL
		&& lSrcImageWidth >= 0 && lSrcImageHeight >= 0);
	
	//RGB -> R、G、B
	PixToPlane((const char *)pSrcImageRGBBuff,
		(char *)pDestPlaneRGBBuff, 
		(char *)pDestPlaneRGBBuff+lSrcImageWidth*lSrcImageHeight, 
		(char *)pDestPlaneRGBBuff+lSrcImageWidth*lSrcImageHeight*2, 
		lSrcImageWidth, lSrcImageHeight);
}

void CImage::ConvertPlaneRGBToRGB(BYTE* pDestImageRGBBuff, const BYTE *pSrcPlaneRGBBuff, long lSrcImageWidth, long lSrcImageHeight)
{
	ASSERT(pDestImageRGBBuff != NULL && pSrcPlaneRGBBuff != NULL
		&& lSrcImageWidth >= 0 && lSrcImageHeight >= 0);
	
	//R、G、B -> RGB
	PlaneToPix((const char *)pSrcPlaneRGBBuff, 
		(const char *)pSrcPlaneRGBBuff+lSrcImageWidth*lSrcImageHeight, 
		(const char *)pSrcPlaneRGBBuff+lSrcImageWidth*lSrcImageHeight*2,
		(char *)pDestImageRGBBuff, 
		lSrcImageWidth, lSrcImageHeight);
}

