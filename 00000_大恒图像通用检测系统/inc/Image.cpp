/************************************************************************/
/* Copyright (c) 2005, 中国大恒集团北京图像视觉技术分公司视觉技术部     
/* All rights reserved.													
/*																		
/* 文件名称： Image.cpp												
/* 文件标识： 见配置管理计划书
/* 摘要： CImage类, 对表现一个图像的各数据结构进行封装, 实现了图像拷贝,
/*    图像显示, 图像存盘, 图像剪切等功能
/*
/* 当前版本： 1.1
/* 作者： 江卓佳
/* 完成日期： 2005年10月10日
/*修改: 加入AutoSaveImageToPath()函数和  SaveMeToPath()自动保存图到既定路径 葛争//  [8/19/2009 GZ]
/************************************************************************/

#include "stdafx.h"
#include "Image.h"
#include "FileOperate.h"
#include "LogManager.h"
#include "MainFrm.h"
#include "SpendTime.h"
#include "BottleCapView.h"

//////////////////////////////////////////////////////////////////////////
//
extern CMainFrame *g_pFrame;
CImage::CImage()
{
	int i(0);
	m_pImageBuff = NULL;	//图像缓冲区
	m_lBuffSize = 0;		//图像数据大小

	m_lImageWidth = 0;		//图像宽度
	m_lImageHeight = 0;		//图像长度
	m_lImageBitCount = 0;	//象素位数
	m_lImagePixelSize = 0;	//象素大小（单位：字节）
	m_lImageSize = 0;		//图像数据大小
	
	//自己内部的
	m_pBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 256*sizeof(RGBQUAD)];
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
	for (i = 0 ; i < 256 ; i++)
	{
		m_pBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
		m_pBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
		m_pBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
		m_pBitmapInfo->bmiColors[i].rgbReserved = 0;
	}
	
	//公共显示用的
	m_pShowBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFO) + 256*sizeof(RGBQUAD)];
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
	for (i = 0 ; i < 256 ; i++)
	{
		m_pShowBitmapInfo->bmiColors[i].rgbBlue = (BYTE)i;
		m_pShowBitmapInfo->bmiColors[i].rgbGreen = (BYTE)i;
		m_pShowBitmapInfo->bmiColors[i].rgbRed = (BYTE)i;
		m_pShowBitmapInfo->bmiColors[i].rgbReserved = 0;
	}
	
	//公共保存用的
	m_pSaveBitmapInfo = (BITMAPINFO *) new BYTE[sizeof(BITMAPINFOHEADER) + 256*sizeof(RGBQUAD)];
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

	for (i = 0 ; i < 256 ; i++)
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

void CImage::Init(long lImageBuffSize,long lPicCount)
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
	
	m_pImageBuff = new BYTE[lImageBuffSize*lPicCount];
	memset(m_pImageBuff,0,sizeof(m_pImageBuff));

	if(m_pImageBuff == NULL)
	{
		m_lBuffSize = 0;
		throw(CImageException(1, "内存分配不成功", "Init()"));
		return;
	}
	else
	{
		m_lBuffSize = lImageBuffSize*lPicCount;
	}

}

void CImage::SetImageSize(long lImageWidth, long lImageHeight, long lImageBitCount, long lPicCount)
{
	ASSERT(lImageWidth >= 0 && lImageHeight >= 0 && lImageBitCount > 0);
	
	long lImageSize = lImageWidth * lImageHeight * ((lImageBitCount+7)/8) * lPicCount;
	
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

void CImage::AcceptImage(const BYTE* pSrcImageBuff, long lSrcImageWidth, long lSrcImageHeight, long lSrcImageBitCount, long lPicCount)
{
	ASSERT(pSrcImageBuff != NULL && lSrcImageWidth >= 0 && lSrcImageHeight >= 0 && lSrcImageBitCount > 0);
	
	long lImageSize = lSrcImageWidth * lSrcImageHeight * (lSrcImageBitCount/8) * lPicCount;

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

	
	XDest = Rect.left;
	YDest = Rect.top;
	nDestWidth = Rect.Width();
	nDestHeight = Rect.Height();
		
	if(ShowModeStretch == NoStretch)
	{
		switch(ShowModeFlip) 
		{
		case NoFlip:			//不翻转
			{
			}
			break;
		case FlipHorizintal:	//水平翻转
			{
			}
			break;
		case FlipVertical:		//垂直翻转
			{
			}
			break;
		case Flip:				//全翻转
			{
			}
			break;
		default:				//不翻转
			{
			}
		}
		
		SetDIBitsToDevice(pDC->GetSafeHdc(), 
			XDest, // destx 
			YDest, // desty 
			nDestWidth,				//显示窗口宽度
			nDestHeight,			//显示窗口高度
			0, // srcx 
			0, // srcy 
			0, // nstartscan 
			lImageHeight, // nnumscans 
			pImageBuff, // lpbits 
			m_pShowBitmapInfo, // lpbitsinfo 
			DIB_RGB_COLORS); // wusage 
	}
	else
	{
		switch(ShowModeFlip) 
		{
		case NoFlip:			//不翻转
			break;
		case FlipHorizintal:	//水平翻转
			XDest = Rect.right;
			nDestWidth = -nDestWidth;
			break;
		case FlipVertical:		//垂直翻转
			YDest = Rect.bottom;
			nDestHeight = -nDestHeight;
			break;
		case Flip:				//全翻转
			XDest = Rect.right;
			YDest = Rect.bottom;
			nDestWidth = -nDestWidth;
			nDestHeight = -nDestHeight;
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
	}
	
	m_csShowBitmapInfoSection.Unlock();
}
/*
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
}//*/

//裁剪函数(适用于DIB图像数据)(以原图像左上角为原点, x向右, y向下)
//目标缓冲区//剪切矩形
//返回值: TRUE: 成功
void CImage::CutDIBImage(BYTE *pDest, CRect DestRect, 
						 const BYTE* pSrcImageBuff, 
						 long lSrcImageWidth, 
						 long lSrcImageHeight, 
						 long lSrcImageBitCount)
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

//////////////////////////////////////////////////////////////////////////
// end of all


int CImage::SaveMeToPath(LPVOID lpElementParam,int iSaveImageType,int iCircleSN)
{

	CString strIniPath, strIniFileName, strTime;
	CTime curTime = CTime::GetCurrentTime();
	DWORD dwTime = GetTickCount();
	CString strRunFilePath=inGetCurrentAppliationPath();

	CAllInformationElement *pElement=(CAllInformationElement *)lpElementParam;
	int iCircleCont=0;	//大张号 [8/19/2009 GZ]
	iCircleCont=pElement->m_iCircleCount;
// 	int iGrabSN=0;		// 周期内序号 [8/19/2009 GZ]
// 	iGrabSN=pElement->m_listSingleImageData.
	int iCameraSN=0;	// 相机序号 [9/1/2009 GZ]
	iCameraSN=pElement->m_iCamGroup;//[2012-1-19 by zhy]

	switch(iSaveImageType)
	{
	case SaveGrabImage:
		{
			strIniPath.Format("%s%s%4d%s%02d%s%02d%sCamera%d", strRunFilePath, "SaveImages\\GrabImages\\",
							curTime.GetYear(), "年",curTime.GetMonth(), "月",curTime.GetDay(),"日\\",iCameraSN);
			strIniFileName.Format("%s\\Grab%d_%d_%d_%02d%02d%02d_%d_%03d%s", 
							strIniPath, iCameraSN, 
							iCircleCont,
							iCircleSN,
							curTime.GetHour(),
							curTime.GetMinute(), 
							curTime.GetSecond(), 
							dwTime,	rand()%1000,
							".bmp");
		}	
		break;
	case SaveErrorImage:
		{
			strIniPath.Format("%s%s%4d%s%02d%s%02d%sCamera%d", strRunFilePath, "SaveImages\\ErrorImages\\",
							curTime.GetYear(), "年",curTime.GetMonth(), "月",curTime.GetDay(),"日\\",iCameraSN);
			strIniFileName.Format("%s\\Error%d_%d_%d_%02d%02d%02d_%d_%03d%s", 
							strIniPath, iCameraSN, 
							iCircleCont,
							iCircleSN,
							curTime.GetHour(),
							curTime.GetMinute(), 
							curTime.GetSecond(), 
							dwTime,	rand()%1000,
							".bmp");
		}	
	    break;
	case SaveAbnormityImage:
		{
			strIniPath.Format("%s%s%4d%s%02d%s%02d%sCamera%d", strRunFilePath, "SaveImages\\AbnormityImages\\",
							curTime.GetYear(), "年",curTime.GetMonth(), "月",curTime.GetDay(),"日\\",iCameraSN);
			strIniFileName.Format("%s\\Abnormity%d_%d_%d_%02d%02d%02d_%d_%03d%s", 
							strIniPath, iCameraSN, 
							iCircleCont,
							iCircleSN,
							curTime.GetHour(),
							curTime.GetMinute(),
							curTime.GetSecond(),
							dwTime,	rand()%1000,
							".bmp");
		}	
	    break;
	default:
		{
			strIniPath.Format("%s%s%4d%s%02d%s%02d%sCamera%d", strRunFilePath, "SaveImages\\GrabImages\\",
				curTime.GetYear(), "年",curTime.GetMonth(), "月",curTime.GetDay(),"日\\",iCameraSN);
			strIniFileName.Format("%s\\Grab%d_%d_%d_%02d%02d%02d_%d_%03d%s", 
				strIniPath, iCameraSN, 
				iCircleCont,
				iCircleSN,
				curTime.GetHour(),
				curTime.GetMinute(), 
				curTime.GetSecond(), 
				dwTime,	rand()%1000,
				".bmp");
		}	
		break;
	}
	//strIniFileName.Format("%s_%d.bmp",strIniPath,iCircleCont);
	
	int lImageWidth,lImageHeight,lImageBitCount;
	BYTE* pImageBuff=NULL;
	pImageBuff=GetImageBuffAddr();
	lImageWidth=GetImageWidth();
	lImageHeight=GetImageHeight();
	lImageBitCount=GetImageBitCount();
	
	if (pImageBuff == NULL)
	{
		return ImageBufferError;
	}
	if (lImageWidth<0||lImageHeight<0||lImageBitCount<0)
	{
		return ImageWHBITError;
	}
	if ((strIniFileName.GetLength() <5)|| (strIniFileName.Right(4) != ".bmp")||strIniFileName.GetLength()>255)
	{
		return FileNameError;
	}
	

	if (!inIsPathExist(strIniPath))
	{
		if (inCreateMultiLevelPath(strIniPath))
		{
			m_csSaveBitmapInfoSection.Lock();
			
			m_pSaveBitmapInfo->bmiHeader.biWidth = lImageWidth;
			m_pSaveBitmapInfo->bmiHeader.biHeight = lImageHeight;
			m_pSaveBitmapInfo->bmiHeader.biBitCount = (WORD)lImageBitCount;
			
			if (inSaveBMPFile(strIniFileName, m_pSaveBitmapInfo, pImageBuff))
			{
			}
			else
			{
				m_csSaveBitmapInfoSection.Unlock();
				return SaveBMPError;
			}
			
			m_csSaveBitmapInfoSection.Unlock();

		}
		else
			return CreatePathError;
	}
	else
	{
		m_csSaveBitmapInfoSection.Lock();
		
		m_pSaveBitmapInfo->bmiHeader.biWidth = lImageWidth;
		m_pSaveBitmapInfo->bmiHeader.biHeight = lImageHeight;
		m_pSaveBitmapInfo->bmiHeader.biBitCount = (WORD)lImageBitCount;
		
		if (!inSaveBMPFile(strIniFileName, m_pSaveBitmapInfo, pImageBuff))
		{
			m_csSaveBitmapInfoSection.Unlock();
			return SaveBMPError;
		}	
		m_csSaveBitmapInfoSection.Unlock();
	}
	return 1;
}

//保存小开图像  
// iSaveImageType  保存类型
// iCircleCont   采集的周期数  即大张数
// iCircleSN     周期内序号 
// iCameraSN     相机序号 
int CImage::SaveSingleImage(const int iSaveImageType,const int iCircleCount, const int iCamSN, const int iCamGroup, const int iPicCount, const CString strSaveTime)
{
	CBottleCapView* pView=(CBottleCapView*)g_pFrame->GetActiveView();
	BYTE* pImageBuff=NULL;


	int iOriginNum=0,iCount=0;
	int iWidth=0,iHeight=0,iBitCount=0;
	CString strIniPath, strIniFileName,strVirtualDisk;
	CTime curTime= CTime::GetCurrentTime();
	strVirtualDisk.Format("%s:\\",g_pFrame->m_sVirtualDisk);

	/////////////////////////////////////////////////////////////////////////////////////////////
	//控制保存图像的张数

	iOriginNum = g_pFrame->m_iOriginSaveImageNum[iCamSN];

	g_pFrame->m_csImagePath[iCamSN].Lock();
	iCount = g_pFrame->m_ListImagePath[iCamSN].GetCount();
	if(iCount>=(g_pFrame->m_iMaxSaveImageCount) || iOriginNum>0)
	{
		CString strDeleteFilePath;	
		strDeleteFilePath=g_pFrame->m_ListImagePath[iCamSN].RemoveHead();
// 		DeleteFile(strDeleteFilePath);

		strDeleteFilePath+='\0';
		g_pFrame->OP_Directory(strDeleteFilePath,NULL,FO_DELETE,FOF_NOCONFIRMATION | FOF_SIMPLEPROGRESS |FOF_NOERRORUI);

		if(iOriginNum>0)
		{
			g_pFrame->m_iOriginSaveImageNum[iCamSN]--;
		}
	}
	g_pFrame->m_csImagePath[iCamSN].Unlock();

	/////////////////////////////////////////////////////////////////////////////////////////////

	switch(iSaveImageType)
	{
	case SaveGrabImage:
		{
			strIniPath.Format("%s%s%4d%s%02d%s%02d%sCamera%d\\%d_%s", strVirtualDisk, "SaveImages\\GrabImages\\",
							curTime.GetYear(), "年",curTime.GetMonth(), "月",curTime.GetDay(),"日\\",iCamSN+1,
							iCircleCount,strSaveTime);
			strIniFileName.Format("%s\\Grab_%d_%s%s", 
				strIniPath,
				iCircleCount,
				strSaveTime,
				".bmp");
		}	
		break;
	case SaveErrorImage:
		{
			strIniPath.Format("%s%s%4d%s%02d%s%02d%sCamera%d\\%d_%s", strVirtualDisk, "SaveImages\\ErrorImages\\",
				curTime.GetYear(), "年",curTime.GetMonth(), "月",curTime.GetDay(),"日\\",iCamSN+1,
							iCircleCount,strSaveTime);
			strIniFileName.Format("%s\\Error_%d_%s%s", 
				strIniPath, 
				iCircleCount,
				strSaveTime,
				".bmp");

		}

	    break;
	case SaveCorrectImage:
		{
			strIniPath.Format("%s%s%4d%s%02d%s%02d%sCamera%d\\%d_%s", strVirtualDisk, "SaveImages\\CorrectImages\\",
				curTime.GetYear(), "年",curTime.GetMonth(), "月",curTime.GetDay(),"日\\",iCamSN+1,
							iCircleCount,strSaveTime);
				strIniFileName.Format("%s\\Correct_%d_%s%s", 
				strIniPath, 
				iCircleCount,
				strSaveTime,
				".bmp");

		}
	    break;
	case SaveAbnormityImage:
		{
			strIniPath.Format("%s%s%4d%s%02d%s%02d%sCamera%d\\%d_%s", strVirtualDisk, "SaveImages\\AbnormityImages\\",
				curTime.GetYear(), "年",curTime.GetMonth(), "月",curTime.GetDay(),"日\\",iCamSN+1,
							iCircleCount,strSaveTime);
			strIniFileName.Format("%s\\Abnormity_%d_%s%s", 
				strIniPath, 
				iCircleCount,
				strSaveTime,
				".bmp");
		}	
	    break;
	case SaveModelImage:
		{
			strIniPath.Format("%s%s%4d%s%02d%s%02d%sCamera%d\\%d_%s", strVirtualDisk, "SaveImages\\ModelImages\\",
				curTime.GetYear(), "年",curTime.GetMonth(), "月",curTime.GetDay(),"日\\",iCamSN+1,
							iCircleCount,strSaveTime);
			strIniFileName.Format("%s\\Model_%d_%s%s", 
				strIniPath, 
				iCircleCount,
				strSaveTime,
				".bmp");
		}	
	    break;		

	default:
		{
			strIniPath.Format("%s%s%4d%s%02d%s%02d%sCamera%d\\%d_%s", strVirtualDisk, "SaveImages\\GrabImages\\",
				curTime.GetYear(), "年",curTime.GetMonth(), "月",curTime.GetDay(),"日\\",iCamSN+1,
							iCircleCount,strSaveTime);
			strIniFileName.Format("%s\\Grab_%d_%s%s", 
				strIniPath, 
				iCircleCount,
				strSaveTime,
				".bmp");
		}	
		break;
	}
	
	pImageBuff=GetImageBuffAddr();
	iWidth=GetImageWidth();
	iHeight=GetImageHeight();
	iBitCount=GetImageBitCount();
	
	if (pImageBuff == NULL)
	{
		return ImageBufferError;
	}
	if (iWidth<0||iHeight<0||iBitCount<0)
	{
		return ImageWHBITError;
	}
 	if ((strIniFileName.GetLength() <5)|| (strIniFileName.Right(4) != ".bmp")||strIniFileName.GetLength()>255)
	{
		return FileNameError;
	}
	
	if (!inIsPathExist(strIniPath))
	{
		if (inCreateMultiLevelPath(strIniPath))
		{
			m_csSaveBitmapInfoSection.Lock();
			
			m_pSaveBitmapInfo->bmiHeader.biWidth = iWidth;
			m_pSaveBitmapInfo->bmiHeader.biHeight = iHeight;
			m_pSaveBitmapInfo->bmiHeader.biBitCount = (WORD)iBitCount;

			int iImageSize = iWidth*iHeight*iBitCount/8;

			for(int i=0;i<iPicCount;i++)
			{
				strIniFileName.Format("%s\\%02d.bmp",strIniPath,i+1);
				if (!inSaveBMPFile(strIniFileName, m_pSaveBitmapInfo, (pImageBuff+i*iImageSize)))
				{
					m_csSaveBitmapInfoSection.Unlock();
					return SaveBMPError;
				}
			}
			m_csSaveBitmapInfoSection.Unlock();
		}
		else
		{
			return CreatePathError;
		}
	}
	else
	{
		m_csSaveBitmapInfoSection.Lock();
		
		m_pSaveBitmapInfo->bmiHeader.biWidth = iWidth;
		m_pSaveBitmapInfo->bmiHeader.biHeight = iHeight;
		m_pSaveBitmapInfo->bmiHeader.biBitCount = (WORD)iBitCount;

		int iImageSize = iWidth*iHeight*iBitCount/8;

		for(int i=0;i<iPicCount;i++)
		{
			strIniFileName.Format("%s\\%d.bmp",strIniPath,i+1);
			if (!inSaveBMPFile(strIniFileName, m_pSaveBitmapInfo, (pImageBuff+i*iImageSize)))
			{
				m_csSaveBitmapInfoSection.Unlock();
				return SaveBMPError;
			}

		}
		m_csSaveBitmapInfoSection.Unlock();
	}

	g_pFrame->m_csImagePath[iCamSN].Lock();
	g_pFrame->m_ListImagePath[iCamSN].AddTail(strIniPath);
	g_pFrame->m_iListImagePathCount[iCamSN] = g_pFrame->m_ListImagePath[iCamSN].GetCount();
	g_pFrame->m_csImagePath[iCamSN].Unlock();

	return 1;
}

BOOL CImage::inSaveBMPFile(LPCSTR lpFileName, const BITMAPINFO *pBmpInfo, const BYTE *pImageBuffer)
{

	BOOL bRVal				= TRUE;
	DWORD dwBytesRead		= 0;
	DWORD dwSize			= 0;
	BITMAPFILEHEADER bfh	= {0};
	int nTable				= 0;
	DWORD dwImageSize		= 0;
	
	if (pBmpInfo->bmiHeader.biBitCount > 8) 
	{
		nTable = 0;
	}
	else
	{
		nTable = 256;
	}

	dwImageSize =  (pBmpInfo->bmiHeader.biWidth * pBmpInfo->bmiHeader.biHeight) * ((pBmpInfo->bmiHeader.biBitCount + 7) / 
		
		8);
	
	if (dwImageSize <= 0) 
	{
		bRVal = FALSE;
	}
	else{
		bfh.bfType		= (WORD)'M' << 8 | 'B';
		bfh.bfOffBits	= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nTable * sizeof(RGBQUAD);
		bfh.bfSize		= bfh.bfOffBits + dwImageSize;			
		
		HANDLE hFile = ::CreateFile(lpFileName,
			GENERIC_WRITE ,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);
		if (hFile == INVALID_HANDLE_VALUE) {
			bRVal = FALSE;
		}
		else{
			dwSize = sizeof(BITMAPFILEHEADER);
			::WriteFile(hFile, &bfh, dwSize, &dwBytesRead, NULL );
			
			dwSize = sizeof(BITMAPINFOHEADER) + nTable * sizeof(RGBQUAD);
			::WriteFile(hFile, pBmpInfo, dwSize, &dwBytesRead, NULL );
			
			dwSize = dwImageSize;
			WriteFile(hFile, pImageBuffer, dwSize, &dwBytesRead, NULL );
			
			CloseHandle(hFile);
		}
	}
	
	return bRVal;
}

#include <io.h>//头文件
BOOL CImage::inIsPathExist(const CString &strPath)
{
#ifdef _UNICODE
#define IsDirectoryExist(path) ::_waccess(path,0) == 0
	//_waccess的第二个参数表示仅判断存在性，返回0表示存在，返回-1表示不存在
#else
#define IsDirectoryExist(path) ::_access(path,0) == 0//_access是MBCS版本
#endif
	
	if(IsDirectoryExist(strPath))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

BOOL CImage::inCreateMultiLevelPath(const CString &strPath)
{

	//创建一个多层目录，如果存在就不创建
	//(strPath  合法: "D:\\abc\\me" 或 "me\\you" 或 ""; 
	//			不合法: "D:\\abc\\me\\" 或 "me\\you\\" 或 "\\")
	CString tmpPath;// = strPath;
	tmpPath.Format("%s",strPath);
	CList<CString,CString&> FolderStrList;
	int nStrLen = 0;
	
	for(;;)
	{
		nStrLen = tmpPath.GetLength();
		if(nStrLen <= 0)
		{
			break;//从头建文件夹
		}		
		
		BOOL bPathExist = FALSE;//该路径是否已经存在
		
		if(tmpPath.Right(1) == ":")
		{
			bPathExist = inIsPathExist(tmpPath + "\\");
		}
		else
		{
			bPathExist = inIsPathExist(tmpPath);
		}
		
		if(bPathExist)//如果路径存在
		{
			break;//从已存在的路径下建文件夹
		}
		
		//如果路径不存在		
		//每次将最后的一个文件夹名入队
		
		int nPos = 0;
		nPos = tmpPath.ReverseFind('\\');//从后往前确定文件夹名
		if(nPos == -1)//已到达第一个文件夹名
		{
			nPos = 0;//将第一个文件夹名入队
		}
		
		//将文件夹名入队
		CString tmpStr = tmpPath.Right(nStrLen - nPos);
		FolderStrList.AddHead(tmpStr);
		tmpPath.Delete(nPos, nStrLen - nPos);		
	}	
	
	//根据文件夹名队列顺序建立
	while(FolderStrList.IsEmpty() == FALSE)
	{
		tmpPath += FolderStrList.RemoveHead();
		BOOL bRet = FALSE;
		bRet = CreateDirectory(tmpPath, NULL);
		if(bRet == FALSE)
		{
			return FALSE;
		}
	}
	
	return TRUE;
}
// 获取本执行程序所在的路径
CString CImage::inGetCurrentAppliationPath()
{
	char str[MAX_PATH], drive[MAX_PATH], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
	CString AppPath;
	GetModuleFileName(NULL, str, sizeof(str));
	_splitpath(str, drive, dir, fname, ext);
	strcat(drive, dir);
	AppPath = drive;

	return AppPath;
}

int CImage::AutoSaveImageToPath(LPVOID lpElementParam,int iSaveImageType)
{
	int tmpCountSuccess=0;
	int tmpCountError=0;
	int tmpTotal=0;	
	if (iSaveImageType<0)
	{
		return ParamTypeError;
	}
	if (lpElementParam==NULL)
	{
		return ParamLPElementError;
	}
	CAllInformationElement *pElement=(CAllInformationElement *)lpElementParam;

	int iCircleTotal=pElement->m_listSingleImageData.GetCount();
	if (iCircleTotal<=0)
	{
		return ListCountError;
	}

	switch(iSaveImageType)
	{
	case SaveGrabImage:
		{
			for (int tmpInt=0;tmpInt<iCircleTotal;tmpInt++)
			{
				CGrabImageInfo *pGrabImageInfo=NULL;
				pGrabImageInfo=	(CGrabImageInfo *)pElement->m_listSingleImageData.GetAt(pElement->m_listSingleImageData.FindIndex(tmpInt));
				if (pGrabImageInfo==NULL)
				{
					tmpCountError++;
					continue; 
				}
				else
				{
					if (pGrabImageInfo->m_bHaveImage)
					{
						if (pGrabImageInfo->m_myImage.SaveMeToPath(lpElementParam,SaveGrabImage,tmpInt+1))
						{
							tmpCountSuccess++;
							tmpTotal=tmpTotal+tmpInt;
						}
						else
						{
							tmpCountError++;
						}						
					}					
				}
			}
			if (tmpCountSuccess==iCircleTotal)// 全部保存成功 [8/20/2009 GZ]
			{
				return tmpCountSuccess;//返回成功个数
			}
			else if(tmpCountSuccess>0&&tmpCountSuccess<iCircleTotal)//部分保存成功
			{
				return -(tmpCountSuccess*1000+tmpTotal);
			}
			else if(tmpCountSuccess==0)//一张也没保存成功
			{
				return GrabAllFaultError;
			}			
		}	
		break;
	case SaveErrorImage:
		{
			return 0;
		}	
	    break;
	case SaveAbnormityImage:
		{
			return 0;
		}	
	    break;
	}
	return tmpCountSuccess;
}

//对图像数据进行镜像
int CImage::MirrorSingleImage(e_ShowModeFlip  eShowMode) //只有水平和垂直镜像
{
	int lImageWidth,lImageHeight,lImageBitCount;
	BYTE* pImageBuff=NULL;
	pImageBuff=GetImageBuffAddr();
	lImageWidth=GetImageWidth();
	lImageHeight=GetImageHeight();
	lImageBitCount=GetImageBitCount();
	
	if (pImageBuff == NULL)
	{
		return ImageBufferError;
	}
	if (lImageWidth<0||lImageHeight<0||lImageBitCount<0)
	{
		return ImageWHBITError;
	}

	if (lImageBitCount==8)
	{	
		lImageWidth=WIDTHBYTES(lImageWidth*8);
		if (eShowMode==FlipHorizintal) //水平翻转
		{
			BYTE  tem;
			for (int i=0;i<lImageHeight;i++)
			{
				for (int j=0;j<lImageWidth/2;j++)
				{
					tem=*(pImageBuff+i*lImageWidth+j);
					*(pImageBuff+i*lImageWidth+j)=*(pImageBuff+i*lImageWidth+lImageWidth-1-j);
					*(pImageBuff+i*lImageWidth+lImageWidth-1-j)=tem;
				}
			}
			return TRUE;
		}
		if (eShowMode==FlipVertical) //垂直翻转
		{
			BYTE  tem;
			for (int i=0;i<lImageHeight/2;i++)
			{
				for (int j=0;j<lImageWidth;j++)
				{
					tem=*(pImageBuff+i*lImageWidth+j);
					*(pImageBuff+i*lImageWidth+j)=*(pImageBuff+(lImageHeight-1-i)*lImageWidth+j);
					*(pImageBuff+(lImageHeight-1-i)*lImageWidth+j)=tem;
				}
			}
			return TRUE;
		}
	}
	if (lImageBitCount==24)//24位镜像
	{
		lImageWidth=WIDTHBYTES(lImageWidth*8);

		if (eShowMode==FlipHorizintal) //水平翻转
		{
			BYTE  tem1,tem2,tem3;
			for (int i=0;i<lImageHeight;i++)
			{
				for (int j=0;j<lImageWidth/2;j++)
				{
					tem1=*(pImageBuff+i*lImageWidth*3+3*j);
					tem2=*(pImageBuff+i*lImageWidth*3+3*j+1);
					tem3=*(pImageBuff+i*lImageWidth*3+3*j+2);
					*(pImageBuff+i*lImageWidth*3+3*j)=*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j));
					*(pImageBuff+i*lImageWidth*3+3*j+1)=*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+1);
					*(pImageBuff+i*lImageWidth*3+3*j+2)=*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+2);
					*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j))=tem1;
					*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+1)=tem2;
					*(pImageBuff+i*lImageWidth*3+3*(lImageWidth-1-j)+2)=tem3;
				}
			}
			return TRUE;
		}
		if (eShowMode==FlipVertical) //垂直翻转
		{
			BYTE  tem1,tem2,tem3;
			for (int i=0;i<lImageHeight/2;i++)
			{
				for (int j=0;j<lImageWidth;j++)
				{
					tem1=*(pImageBuff+i*lImageWidth*3+3*j);
					tem2=*(pImageBuff+i*lImageWidth*3+3*j+1);
					tem3=*(pImageBuff+i*lImageWidth*3+3*j+2);
					*(pImageBuff+i*lImageWidth*3+3*j)=*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j);
					*(pImageBuff+i*lImageWidth*3+3*j+1)=*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j+1);
					*(pImageBuff+i*lImageWidth*3+3*j+2)=*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j+2);
					*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j)=tem1;
					*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j+1)=tem2;
					*(pImageBuff+(lImageHeight-1-i)*lImageWidth*3+3*j+2)=tem3;
				}
			}
			return TRUE;
		}
	}	
	return TRUE;
}

BOOL CImage::OpenBMPFileX(LPCSTR lpFileName,long lPicCount)//[2007-11-21] by SGQ
{
	BOOL Ret = OpenBMPFile(lpFileName, m_pBitmapInfo, m_pImageBuff);
	if (!Ret)
	{
		
		CString str;
		str.Format("CImage::OpenBMPFileX Open %s 失败!",lpFileName);
		AfxMessageBox(str);
		
		return FALSE;
	}
	
	m_lImageWidth = m_pBitmapInfo->bmiHeader.biWidth; 
	m_lImageHeight = m_pBitmapInfo->bmiHeader.biHeight;
	m_lImageBitCount= (long)(m_pBitmapInfo->bmiHeader.biBitCount);
	long lImageSize = m_lImageWidth * m_lImageHeight * (m_lImageBitCount/8)*lPicCount;
	if(lImageSize > m_lBuffSize)
	{
		throw(CImageException(2, "参数不合法", "AcceptImage()"));
	}
	m_lImageSize = lImageSize;
	
	return TRUE;
}