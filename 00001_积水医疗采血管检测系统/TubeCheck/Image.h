/************************************************************************/
/* Copyright (c) 2005, 中国大恒集团北京图像视觉技术分公司视觉技术部     
/* All rights reserved.													
/*																		
/* 文件名称： Image.h												
/* 文件标识： 见配置管理计划书
/* 摘要： CImage类, 对表现一个图像的各数据结构进行封装, 实现了图像拷贝,
/*    图像显示, 图像存盘, 图像剪切等功能
/*
/* 当前版本： 1.1
/* 作者： 江卓佳
/* 完成日期： 2005年10月10日
/*修改: 加入AutoSaveImageToPath()函数和  SaveMeToPath()自动保存图到既定路径 葛争//  [8/19/2009 GZ]
/* 加入保存小开图像函数SaveSingleImage //  [2009-8-23-yangfs]
/* // SaveSingleImage()函数中加入相机序号参数，保存路径及文件名中都加入相机序号 [9/1/2009 GZ]
/************************************************************************/

#if !defined(Image_h_)
#define Image_h_


//////////////////////////////////////////////////////////////////////////
//

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

enum eSaveImageType
{
	SaveGrabImage=0,                // 保存连续采集图像 [8/19/2009 GZ]
	SaveErrorImage,					// 保存错误图像
	SaveAbnormityImage,				// 保存异常图像
	SaveCorrectImage,				// 保存正确图像
	SaveModelImage					// 保存模板图像
}; 

enum eAutoSaveErrorInfo
{
	GrabAllFaultError=-9,
	ParamTypeError,				// 参数2保存类型错误错误 [8/19/2009 GZ]
	ParamLPElementError,			// 参数1错误元素指针为空 [8/19/2009 GZ]
	ListCountError,					// 获得的元素中的链表个数为零或负数 [8/19/2009 GZ]
	SaveBMPError,					// 函数SaveBMPFile()未成功 [8/19/2009 GZ]
	CreatePathError,				// 创建路径错误
	FileNameError,					// 文件名错误
	ImageWHBITError,				// 图片长宽位数错误
	ImageBufferError				// 图像数据区错误
};

#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)


class CImage
{	
//属性
protected:	
	// 获取本执行程序所在的路径
	CString inGetCurrentAppliationPath();
	
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
public:
 	BITMAPINFO* m_pSaveBitmapInfo;
	CCriticalSection m_csSaveBitmapInfoSection;

//操作
public:
	BOOL inCreateMultiLevelPath(const CString &strPath);
	BOOL inIsPathExist(const CString &strPath);
	//AutoSaveImageToPath参数：iSaveImageType为1~3，1为采集存储 [8/20/2009 GZ]
	int AutoSaveImageToPath(LPVOID lpElementParam,int iSaveImageType);
	int SaveMeToPath(LPVOID lpElementParam,int iSaveImageType,int iCircleSN);
	BOOL inSaveBMPFile(LPCSTR lpFileName, const BITMAPINFO *pBmpInfo, const BYTE *pImageBuffer);

	//保存小开图像 
	//iSaveImageType 保存图像类型
	//iCircleCount为对应的大张号
	//iCamSN为相机序号
	//iGroupSN为工位号
	//iPicCount为每次触发采集的图像张数
	int SaveSingleImage(const int iSaveImageType,const int iCircleCount, const int iCamSN, const int iGroupSN, const int iPicCount, const CString strSaveTime);

	//对图像数据进行镜像
	int MirrorSingleImage(e_ShowModeFlip  eShowMode=FlipHorizintal); //只有水平和垂直镜像 //  [2009-8-24-yangfs]

	CImage();
	virtual ~CImage();
	
	void Init(long lImageBuffSize, long lPicCount);
	void SetImageSize(long lImageWidth, long lImageHeight, long lImageBitCount, long lPicCount);
	void AcceptImage(const BYTE* pSrcImageBuff, long lSrcImageWidth, long lSrcImageHeight, long lSrcImageBitCount, long lPicCount);

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
//	void SaveDIBImageToBMPFile(const CString& strFileName, const BYTE* pImageBuff, long lImageWidth, long lImageHeight, long lImageBitCount);
	void CutDIBImage(BYTE *pDest, CRect DestRect, const BYTE* pSrcImageBuff, long lSrcImageWidth, long lSrcImageHeight, long lSrcImageBitCount);
	BOOL OpenBMPFileX(LPCSTR lpFileName,long lPicCount);
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
		m_strExceptionDescription.Format("Error Type(%d)");
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
		str.Format("错误类型：%d\n描述：%s\n附加信息：%s", 
			m_nExceptionType,
			m_strExceptionDescription,
			m_strExceptionContext);

		AfxMessageBox(str);
	}
};


#endif // !define(Image_h_)