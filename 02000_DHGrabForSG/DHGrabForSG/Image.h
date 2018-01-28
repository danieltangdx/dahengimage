/************************************************************************/
/* Copyright (c) 2005, 中国大恒集团北京图像视觉技术分公司视觉技术部     
/* All rights reserved.													
/*																		
/* 文件名称： Image.h												
/* 文件标识： 见配置管理计划书
/* 摘要： CImage类, 对表现一个图像的各数据结构进行封装, 实现了图像拷贝,
/*    图像显示, 图像存盘, 图像剪切等功能
/*
/* 当前版本： 1.0
/* 作者： 江卓佳
/* 完成日期： 2005年10月10日
/************************************************************************/

#if !defined(Image_h_)
#define Image_h_

#include <afxmt.h>          //为了使用CEvent
//#define ByteBitCount 8


enum e_ShowModeFlip	//翻转显示方式
{
	NoFlip = 0,					//不翻转显示
	FlipVertical,				//垂直翻转显示
	FlipHorizintal,				//水平翻转显示
	Flip						//全翻转显示
};	//翻转显示方式

enum e_ShowModeStretch	//拉伸显示方式
{
	NoStretch = 0,				//不拉伸显示
	Stretch						//拉伸显示
};	//拉伸显示方式

class CImage
{	
//属性
protected:	
	BYTE* m_pImageBuff;	//图像缓冲区
	long m_lBuffSize;	//图像缓冲区大小
	
	long m_lImageWidth;		//图像宽度
	long m_lImageHeight;	//图像长度
	long m_lImageBitCount;	//象素位数
	long m_lImagePixelSize;	//象素大小（单位：字节）
	long m_lImageSize;		//图像数据大小

	BITMAPINFO* m_pBitmapInfo;
	
	BITMAPINFO* m_pShowBitmapInfo;
	CCriticalSection m_csShowBitmapInfoSection;
	BITMAPINFO* m_pSaveBitmapInfo;
	CCriticalSection m_csSaveBitmapInfoSection;

//操作
public:
	CImage();
	virtual ~CImage();
	
	void Init(long lImageBuffSize);
	void SetImageSize(long lImageWidth, long lImageHeight, long lImageBitCount);
	void AcceptImage(const BYTE* pSrcImageBuff, long lSrcImageWidth, long lSrcImageHeight, long lSrcImageBitCount);

	BYTE* GetImageBuffAddr();
	long GetImageBuffSize();
	long GetImageSize();
	long GetImageWidth();
	long GetImageHeight();
	long GetImageBitCount();
	long GetImagePixelSize();
	const BITMAPINFO* GetBitmapInfo();
	
	void ShowDIBImage(CDC* pDC, CRect Rect, e_ShowModeFlip ShowModeFlip, e_ShowModeStretch ShowModeStretch,
		const BYTE* pImageBuff, long lImageWidth, long lImageHeight, long lImageBitCount);
	void SaveDIBImageToBMPFile(const CString& strFileName, const BYTE* pImageBuff, long lImageWidth, long lImageHeight, long lImageBitCount);
	void CutDIBImage(BYTE *pDest, CRect DestRect, const BYTE* pSrcImageBuff, long lSrcImageWidth, long lSrcImageHeight, long lSrcImageBitCount);
	void CutPlaneRGBImage(BYTE *pDest, CRect DestRect, const BYTE* pSrcImageBuff, long lSrcImageWidth, long lSrcImageHeight);
	void ConvertRGBToPlaneR_G_B(BYTE *pDestImageBuffR, BYTE *pDestImageBuffG, BYTE *pDestImageBuffB, 
		const BYTE* pSrcImageBuff, long lSrcImageWidth, long lSrcImageHeight);
	void ConvertPlaneR_G_BToRGB(BYTE* pDestImageBuff, const BYTE *pSrcImageBuffR, const BYTE *pSrcImageBuffG, const BYTE *pSrcImageBuffB, 
		long lSrcImageWidth, long lSrcImageHeight);
	void ConvertRGBToPlaneRGB(BYTE *pDestPlaneRGBBuff,	const BYTE* pSrcImageRGBBuff, long lSrcImageWidth, long lSrcImageHeight);
	void ConvertPlaneRGBToRGB(BYTE* pDestImageRGBBuff, const BYTE *pSrcPlaneRGBBuff, long lSrcImageWidth, long lSrcImageHeight);

};

class CImageException
{
//属性
protected:	
	int m_nExceptionType;				//异常的类型
	CString m_strExceptionDescription;	//异常的描述
	CString m_strExceptionContext;		//异常的附加信息
	
//操作
public:
	CImageException(int nExceptionType, const CString &strExceptionContext)
		: m_nExceptionType(nExceptionType), 
		m_strExceptionContext(strExceptionContext)
	{
		m_strExceptionDescription.Format(_T("Error Type(%d)"));
	};
	
	CImageException(int nExceptionType, const CString &strExceptionDescription, const CString &strExceptionContext)
		: m_nExceptionType(nExceptionType), 
		m_strExceptionDescription(strExceptionDescription),
		m_strExceptionContext(strExceptionContext)
	{
	};
	
	~CImageException()
	{
	};

	//得到类型
	int GetType() const
	{
		return m_nExceptionType;
	}

	//得到该类型的描述
	CString GetDescription() const
	{
		return m_strExceptionDescription;
	}
	
	//得到附加信息
	CString GetContext() const
	{
		return m_strExceptionContext;
	}
	
	void ShowMe() const
	{
		CString str;
		str.Format(_T("错误类型：%d\n描述：%s\n附加信息：%s"), 
			m_nExceptionType,
			m_strExceptionDescription,
			m_strExceptionContext);

		AfxMessageBox(str);
	}
};


#endif // !define(Image_h_)