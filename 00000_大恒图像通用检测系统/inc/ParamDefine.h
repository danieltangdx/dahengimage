
/************************************************************************/
/* Copyright (c) 2009, 北京大恒图像视觉有限公司视觉系统部     
/* All rights reserved.													
/*																		
/* 文件名称: ParamDefine.h		
/* 摘    要: 
/*
/* 当前版本: 1.2
/* 作    者: 杨富森
/* 完成日期: 2009年08月13日
/* 2009-8-16  将编码器信号合并到机器信号中
/* 2009-8-19  在所有信息类中加入采集周期计数，即大张号
/* 添加bayer变换的类
/* 2009-8-21 将相机编号放入大元素中;定义保存图像元素
/************************************************************************/

#pragma  once

//#include "Image.h"
#include "alg_struct.h"
using namespace Alg; // 使用命名空间 [2009-9-15-yangfs]

#define  MAX_COUNT_BY_CIRCLE    20     //一个采集周期内采集的最多图像数

////////////////////////////////////////////////
// PI定义
#ifndef PI
#ifdef  M_PI
#define PI               M_PI
#else
#define PI               3.14159265358979323846
#endif
#endif
#ifndef PI_2
#define PI_2             (PI / 2.0)
#endif
#ifndef PI_4
#define PI_4             (PI / 4.0)
#endif


#if defined(_WIN32)||defined(TRIMEDIA)||defined(_TMS320C6X)||defined (__vxworks)
#ifndef M_PI
#define M_PI             PI
#endif
#ifndef M_PI_2
#define M_PI_2           (PI / 2.0)
#endif
#ifndef M_PI_4
#define M_PI_4           (PI / 4.0)
#endif
#endif
////////////////////////////////////////////

//定义程序所需要的变量和结构体

//保存图像类型
enum e_SaveImageType	
{
	GrabImage = 0,				//采集的图像
	ErrorImage,		            //错误的图像
	AbnormityImage			    //异常的图像
};

// 采集图像类型
enum e_GrabImageType	
{
	GImageBody = 0,	    // 瓶身
	GImageBottom		// 瓶底
};

// 采集状态
enum e_GrabStatus	
{
	GSNormal = 0,	// 正常
	GSLostFrame		// 丢帧
};

// 相机序号
enum e_CameraType	
{
	Camera1 = 0,	 
	Camera2,		 
	Camera3,
	Camera4,
	Camera5,
	Camera6,
	Camera7,
	Camera8,
	Camera9,
};

enum e_ThreadType
{
	CheckThread1 = 0,
	CheckThread2,
	CheckThread3,
	CheckThread4,
	CheckThread5,
	CheckThread6,
	CheckThread7,
	CheckThread8,
	CheckThread9,  //[2012-4-16 by zhy]
};

enum e_WorkState  //工作状态(用于状态栏显示)
{
	SystemStarting = 0,			//正在启动系统
	SystemStarted,				//系统已启动
	IOCardInfo,                 //接口卡信息
	Checking,					//正在检测...
	CheckResult,                //检测结果
	StopChecked,					//检测已停止
	AlgCheckTime,               //算法检测时间
	SetCheckParam,              //设置算法参数
	Grabbing,					//采集中...
	StopGrabbing,				//停止采集
	ContinueImageSaving,        //连续保存图像
	ConstantImageSaving,        //固定保存图像
	ErrorImageSaving,           //错误图像保存
	AbnormalImageSaving,        //异常图像保存
	ModelImageSaving,			//模板图像保存
	ImageSaved,					//图像保存完毕
	StopSave,                   //停止保存
	SystemClosing,				//正在退出系统
	FirstStop,					//第一工位无盖停机
	ThirdStop,					//第三工位无盖停机
	NoEnoughPressure,			//气压不足停机
	BatchFinished,				//批次完成报警
	FirstNoKick,				//第一工位不剔废
	SecondNoKick,				//第二工位不剔废
	ThirdNoKick,				//第三工位不剔废
	FirstAlwaysKick,			//第一工位持续剔废
	SecondAlwaysKick,			//第二工位持续剔废
	ThirdAlwaysKick,			//第三工位持续剔废
	TestBeltSpeed				//测试皮带速度

};  //工作状态

//////////////////////////////////////////////////////////////////////////
// 数据结构

// 机器信号
class CMachineSignal  //根据具体现场的需求来定义机器信号内容
{
public:	
	int m_iReturnCount1;	// 第一个光电计数
	int m_iReturnCount2;	// 第二个光电计数
	int m_iReturnCount3;	// 第二个光电计数
	int m_iImageCount1;		// 接口卡图像信号计数  图像号计数0 对应相机1
	int m_iImageCount2;		// 图像号计数1  // 对应相机2
	int m_iImageCount3;		// 图像号计数2  // 对应相机3
	int m_iImageCount4;     // 图像号计数3  // 对应相机4
	int m_iKickCount1;      // 相机1和2踢废计数 
    int m_iKickCount2;		// 相机3的剔废计数 [2012-4-11 by zhy]
	int m_iKickCount3;		// 相机3的剔废计数 [2012-4-11 by zhy]
	int m_iTriggerCount1;   // 相机1的触发计数
	int m_iTriggerCount2;  // 相机2的触发计数
	int m_iTriggerCount3;  // 相机3的触发计数
public:
	
	CMachineSignal()
	{
		m_iReturnCount1		= 0; 
		m_iReturnCount2		= 0;
		m_iReturnCount3		= 0;
		m_iImageCount1		= 0;
		m_iImageCount2		= 0;
		m_iImageCount3		= 0;
		m_iImageCount4		= 0;
		m_iKickCount1		= 0;       // 相机1踢废计数
		m_iKickCount2		= 0;       // 相机2的剔废计数
		m_iKickCount3		= 0;       // 相机3的剔废计数
		m_iTriggerCount1	= 0;
		m_iTriggerCount2	= 0;
		m_iTriggerCount3	= 0;
	}

	virtual ~CMachineSignal()
	{
	}

	CMachineSignal(const CMachineSignal &MachineSignal)
	{
		m_iReturnCount1		= MachineSignal.m_iReturnCount1;
		m_iReturnCount2		= MachineSignal.m_iReturnCount2;
		m_iReturnCount3		= MachineSignal.m_iReturnCount3;
		m_iImageCount1		= MachineSignal.m_iImageCount1;
		m_iImageCount2		= MachineSignal.m_iImageCount2;
		m_iImageCount3		= MachineSignal.m_iImageCount3;
		m_iImageCount4		= MachineSignal.m_iImageCount4;
		m_iKickCount1		= MachineSignal.m_iKickCount1;
		m_iKickCount2		= MachineSignal.m_iKickCount2;
		m_iKickCount3		= MachineSignal.m_iKickCount3;
		m_iTriggerCount1	= MachineSignal.m_iTriggerCount1;
		m_iTriggerCount2	= MachineSignal.m_iTriggerCount2;
		m_iTriggerCount3	= MachineSignal.m_iTriggerCount3;
	}

	const CMachineSignal& operator=(const CMachineSignal &MachineSignal)
	{
		m_iReturnCount1		= MachineSignal.m_iReturnCount1;
		m_iReturnCount2		= MachineSignal.m_iReturnCount2;
		m_iReturnCount3		= MachineSignal.m_iReturnCount3;
		m_iImageCount1		= MachineSignal.m_iImageCount1;
		m_iImageCount2		= MachineSignal.m_iImageCount2;
		m_iImageCount3		= MachineSignal.m_iImageCount3;
		m_iImageCount4		= MachineSignal.m_iImageCount4;
		m_iKickCount1		= MachineSignal.m_iKickCount1;
		m_iKickCount2		= MachineSignal.m_iKickCount2;
		m_iKickCount3		= MachineSignal.m_iKickCount3;
		m_iTriggerCount1	= MachineSignal.m_iTriggerCount1;
		m_iTriggerCount2	= MachineSignal.m_iTriggerCount2;
		m_iTriggerCount3	= MachineSignal.m_iTriggerCount3;
		return *this;
	}
};

//采集图像信息
class CGrabImageInfo
{
public:
	e_GrabImageType     m_eGrabImageType;		// 图像类型
	BOOL			    m_bHaveImage;           // 是否有图像
	CImage			    m_myImage;	            // 具体的图像信息  宽度、高度、数据区等
	DWORD				m_iCircleCount;			// 大张号
	int                 m_iCameraSN;			// 相机编号
public:
	CGrabImageInfo()
	{
		m_eGrabImageType = GImageBody;  // 图像类型
		m_bHaveImage     = FALSE;       // 是否有图像
		m_iCameraSN   = 0;           // 图像序号
		m_iCircleCount   = 0;           // 采集的周期数 即大张数
	}

	virtual ~CGrabImageInfo()
	{
	}
};

// 采集机器信息
class CGrabMachineInfo
{
public:
	int				m_iGrabSpendTime;	// 采集花费时间	
	e_GrabStatus	m_eGrabStatus;		// 采集状态
	BOOL			m_bIsCheck;			// 是否检测
	CMachineSignal	m_MachineSignal;	// 机器信号信息
	CString			m_strDescription;	// 有用信息描述

public:

	CGrabMachineInfo()
	{
		m_iGrabSpendTime= 0;
		m_bIsCheck		= FALSE;
		m_eGrabStatus	= GSNormal;
	}

	virtual ~CGrabMachineInfo()
	{
	}
	CGrabMachineInfo(const CGrabMachineInfo &GrabMachineInfo)
	{
		m_iGrabSpendTime	= GrabMachineInfo.m_iGrabSpendTime;	
		m_eGrabStatus		= GrabMachineInfo.m_eGrabStatus;	
		m_strDescription    = GrabMachineInfo.m_strDescription; 
		m_bIsCheck			= GrabMachineInfo.m_bIsCheck;
		m_MachineSignal     = GrabMachineInfo.m_MachineSignal;
	}

	const CGrabMachineInfo& operator=(const CGrabMachineInfo &GrabMachineInfo)
	{
		m_iGrabSpendTime	= GrabMachineInfo.m_iGrabSpendTime;	
		m_eGrabStatus		= GrabMachineInfo.m_eGrabStatus;	
		m_strDescription    = GrabMachineInfo.m_strDescription; 
		m_bIsCheck			= GrabMachineInfo.m_bIsCheck;
		m_MachineSignal		= GrabMachineInfo.m_MachineSignal;
		return *this;
	}
};

//Bayer变换操作
class CBayerTransform
{
public:
	BOOL	m_bAreaCamBayer;				// 面阵相机图像是否进行Bayer转换
	BYTE	m_pLutR[256];                  //Bayer变换红色查找表
	BYTE	m_pLutG[256];                  //Bayer变换绿色查找表
	BYTE	m_pLutB[256];                  //Bayer变换蓝色查找表
	double	m_dBayerRedGain;             //Bayer变换的红色白平衡增益值
	double  m_dBayerGreenGain;           //Bayer变换的绿色白平衡增益值
	double  m_dBayerBlueGain;            //Bayer变换的蓝色白平衡增益值
public:
	CBayerTransform()
	{
		m_bAreaCamBayer = FALSE;
		m_dBayerRedGain = 0.0;             //Bayer变换的红色白平衡增益值
		m_dBayerGreenGain = 0.0;           //Bayer变换的绿色白平衡增益值
		m_dBayerBlueGain = 0.0;            //Bayer变换的蓝色白平衡增益值
		for (int i=0; i< 256;i++)
		{
			m_pLutR[i] = min((int)(i*m_dBayerRedGain),255);
			m_pLutG[i] = min((int)(i*m_dBayerGreenGain),255);
			m_pLutB[i] = min((int)(i*m_dBayerBlueGain),255);
		}
	}
	
	virtual ~CBayerTransform()
	{

	}
	CBayerTransform(const CBayerTransform &BayerTransform)
	{
		m_bAreaCamBayer = BayerTransform.m_bAreaCamBayer;
		m_dBayerRedGain = BayerTransform.m_dBayerRedGain;      //Bayer变换的红色白平衡增益值
		m_dBayerGreenGain = BayerTransform.m_dBayerGreenGain;  //Bayer变换的绿色白平衡增益值
		m_dBayerBlueGain = BayerTransform.m_dBayerBlueGain;    //Bayer变换的蓝色白平衡增益值
		for (int i=0; i< 256;i++)
		{
			m_pLutR[i] = min((int)(i*m_dBayerRedGain),255);
			m_pLutG[i] = min((int)(i*m_dBayerGreenGain),255);
			m_pLutB[i] = min((int)(i*m_dBayerBlueGain),255);
		}
	}
	const CBayerTransform& operator=(const CBayerTransform &BayerTransform)
	{
		m_bAreaCamBayer = BayerTransform.m_bAreaCamBayer;
		m_dBayerRedGain = BayerTransform.m_dBayerRedGain;      //Bayer变换的红色白平衡增益值
		m_dBayerGreenGain = BayerTransform.m_dBayerGreenGain;  //Bayer变换的绿色白平衡增益值
		m_dBayerBlueGain = BayerTransform.m_dBayerBlueGain;    //Bayer变换的蓝色白平衡增益值
		for (int i=0; i< 256;i++)
		{
			m_pLutR[i] = min((int)(i*m_dBayerRedGain),255);
			m_pLutG[i] = min((int)(i*m_dBayerGreenGain),255);
			m_pLutB[i] = min((int)(i*m_dBayerBlueGain),255);
		}
		return *this;
	}

};

// class   CSaveImageInfo  //保存图像元素
// {
// public:
// 	int         	 m_iWorkStationSN;	   //相机编号
// 	int              m_iCircleSN;   //采集的周期数 即大张数
// 	int				 m_iImageSN;
// 	eSaveImageType   m_eSaveImageType; //保存类型
// 	CGrabImageInfo   m_GrabImageInfo;  //小开图像信息
// 	CString			 m_strSaveTime;  //保存时间
// public:
// 	CSaveImageInfo()
// 	{
// 		m_iWorkStationSN = 0;	 // 相机编号
// 		m_iCircleSN = 0;  //采集的周期数
// 		m_iImageSN = 0;
// 		m_eSaveImageType = SaveGrabImage; //保存类型
// 	}
// 	virtual ~CSaveImageInfo()
// 	{
// 
// 	}
// };


//所有信息元素类   //根据具体项目的需求来添加新的成员	
class CAllInformationElement
{
public:
	int         		m_iCamGroup;				//相机组号[5/20/2010 ZXF] [2012-1-19 by zhy]
	CGrabMachineInfo	m_GrabMachineInfo;			//采集机器信息
	CPtrList			m_listSingleImageData;		//一个采集周期内采集的图像链表
	DWORD               m_iCircleCount;				//采集周期个数 即大张号
	DWORD				m_iHardCircleCount;         //从接口卡读到的图像号，由于触发过快相机没有全部响应导致与m_iCircleCount不一致
	BOOL                m_bHaveError;				//检测是否有错
	CString				m_strErrorInfo;
public:

	CAllInformationElement()
	{
		m_iCamGroup = 0;
		m_iCircleCount = 0;
		m_iHardCircleCount = 0;
		m_bHaveError = FALSE;
		m_strErrorInfo="";
	}

	virtual ~CAllInformationElement()
	{
		// 释放所有小开图像资源
		CGrabImageInfo* pGrabImageInfo = NULL;
		while (m_listSingleImageData.IsEmpty() == FALSE)
		{
			pGrabImageInfo = (CGrabImageInfo*)m_listSingleImageData.RemoveHead();
			if (pGrabImageInfo != NULL)
			{
				delete pGrabImageInfo;
				pGrabImageInfo = NULL;
			}
		}
	}
};


//菜单类型
enum e_MenuType
{
	UserMenu = 0,		//普通用户菜单
	AdvUserMenu			//高级用户菜单
};

//定时器类型
enum e_TimerType
{
	ViewUpdateTimer = 0,			//View类启动刷新定时器 用一次
	TimeTimer,						//时间定时器
	DetectTimer,					//检测时间定时器
	StartGrabTimer,					//开始采集定时器
	InspectSystemTimer,				//监视系统死机状态定时器
	IntervalDisplayTimerOne,		//相机1间隔显示定时器
	IntervalDisplayTimerTwo,		//相机2间隔显示定时器
	IntervalDisplayTimerThree,		//相机3间隔显示定时器
	CheckSpeedRealTime,				//检测速度实时显示 [12/3/2009 GZ]
	CameraStatus,					//相机状态 [1/21/2010 GZ]
	IO24BSTATUS,
	AlarmOff,  
	AlarmOn,						
	DisplayCount,					// 本设备已经生产的瓶盖数量
	SaveCount,						// 向配置文件写入数据，更新数据
	UpdateCheckData_1,				// 更新按工位检测数据
	UpdateCheckData_2,				// 更新按位置检测数据
	ShowDataInfo,					// 显示数据信息
	CameraTest,						// 相机调试
};

class CImageInformation//**[1/2/2018 ConanSteve]**:  算法和多对象使用的结构体
{
public:
	int m_iCameraImageWidth;		// 采集图像宽度
	int m_iCameraImageHeight;		// 采集图像高度
	int m_iCameraImageSize;			// 采集图像大小
	int m_iCameraImagePixelSize;	// 采集图像像素大小	
	int m_iBayerType{ 0 };//**[9/7/2017 ConanSteve]**:  Bayer变换类型。从配置文件读取的，符合Halcon标准
	int m_iFlipType{ 0 };//**[9/7/2017 ConanSteve]**: 翻转类型，0不翻转，1水平翻转，2垂直翻转，3全翻转，算法使用，系统不用
};

//数据库表中的项目名称
typedef struct _CheckResultInfo
{
	CString strStartTime;			//开检时间
	CString strStopTime;			//停检时间
	CString	strModelName;			// 模板名称
	CString strTotalCount;			// 检测总数
	CString strTotalGood;			// 好品总数
	CString strTotalBad;			// 坏品数
	CString strTotalBadRatio;		// 坏品率
	CString strTotal1;				// 第1工位检测总数
	CString strGood1;				// 第1工位好品
	CString strBad1;				// 第1工位坏品
	CString strBadRatio1;			// 第1工位坏品率
	CString strTotal2;				// 第2工位检测总数
	CString strGood2;				// 第2工位好品
	CString strBad2;				// 第2工位坏品
	CString strBadRatio2;			// 第2工位坏品率

}s_CheckResultInfo;

//数据库表中的项目名称
typedef struct _CheckDefectInfo
{
	CString sStartTime;			//开检时间
	CString sStopTime;			//停检时间
	CString	sModelName;			// 模板名称
	int iTotalCount;			// 检测总数
	int iTotalGood;				// 好品总数
	int iTotalBad;				// 坏品数
	int iTotal1;				// 工位1检测总数
	int iBad1;					// 工位1坏品数
	int iTotal2;				// 工位2检测总数
	int iBad2;					// 工位2坏品数
	int	iDefectCount[200];		// 每种缺陷个数
}s_CheckDefectInfo;


typedef struct _UserInfo
{
	CString strUser;											// 用户名称
	CString strMonitor;											// 班长
	CString strInspector;										// 质检员
	CString strProductCode;										// 产品代码
	CString strBatch;											// 生产批次
	CString strSize;											// 型号
	CString strFormat;											// 规格
	CString strBrand;											// 商标
	CString strColor;											// 颜色
	CString strAluminumMaterial;								// 铝制材料
	CString strMouldNum;										// 模具号
	CString strMachineCode;										// 机器号
	CString strClass;											// 班别
}s_UserInfo;

typedef struct _LoginInfo
{
	CString strUserPath;										// 存储操作员信息的路径
	CString strMonitorPath;										// 存储班长信息的路径
	CString strInspectorPath;									// 存储质检员信息的路径
	CString strProductPath;										// 存储产品信息的路径
	CString strSysConfigFileName;								// 系统配置文件路径
	CString strAlgIniFilePath;									// 算法配置文件路径
	CString strBatchCode;										// 产品批号
	CString strModelName;										// 模板名称
	CString strUser;											// 操作员
	CString strMonitor;											// 班长
	CString strMachineCode;										// 机器号
	CString strClass;											// 班别
}s_LoginInfo;

typedef struct _ElementInfo
{
	int iCameraSN;			//工位号
	int iCircleSN;			//采集号
	int iImageSN;			//图像号
	int iIsUsing;			//是否向内存写入图像数据,0为未写，1为正在写，2为写入完毕
	int iIsDetecting;		//是否正在检测
	BOOL bHaveError;		//是否有错
	BOOL bIsCheck;			//是否正在检测
	BYTE* pAddr;			//内存地址
	CString strErrorInfo;	//报错信息
}s_ElementInfo;

typedef struct _ThreadParam
{
	void* pThis;
	int iThreadID;
}s_ThreadParam;

//////////////////////////////END////////////////////////////////////////////