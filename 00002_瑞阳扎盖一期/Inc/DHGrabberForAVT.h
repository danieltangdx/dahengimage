/************************************************************************/
/* Copyright (c) 2010, 中国大恒集团北京图像视觉技术分公司视觉系统部  
/* All rights reserved.													
/*																		
/* 文件名称： DHGrabberForAVT.h	
/* 摘要： 针对AVT相机FGCamera版本(2.10)及其以前版本  如有更高版本则需要升级
/*
/* 当前版本： 4.0.0.1Beta5
/* 修改者： 杨富森
/* 修改内容：加入程序退出是否保存XML文件标志
/* 完成日期： 2010-7-5
/*
/* 当前版本： 4.0.0.1Beta4
/* 修改者： 杨富森
/* 修改内容：解决多个相机连接问题
/* 完成日期： 2010-6-23
/*
/* 以前版本： 4.0.0.1Beta3
/* 修改内容： 相机退出时再保存一次XML
/* 修改时间： 2010-6-21
/*
/* 以前版本： 4.0.0.1Beta2
/* 修改内容： 在停止采集时，可以在相机参数对话框上，将修改的相机参数保存为XML
/* 修改时间： 2010-5-26
/*
/* 以前版本： 4.0.0.1Beta1
/* 修改内容： 将以前的相机配置文件ini替换成xml格式
/* 修改时间： 2010-5-23
/*
/* 以前版本： 3.0.0.1
/* 以前版本编写：李庆林
/* 以前记录：适应多个相机；更新FGCamera;读取白平衡最大值;参数设置框修改触发初始化;
/* 添加亮度、增益、曝光最大值；修改触发值不对;修改黑白相机不要获得白平衡值
/* 修改者： 杨富森
/************************************************************************/

/*
采集卡类型编号定义如下
0:模拟采集卡
1:DALSA X64采集卡
2:AVT相机
3:千兆网相机
4:公司CG300模拟采集卡
5:公司1394相机
6:EURESYS 采集卡*/


#if !defined(DHGrabber_h_)
#define DHGrabber_h_

const int GBMaxFileNameLen = 255;
const int GBMaxTextLen = 255;

typedef struct _GBSignalInfoStruct//回调信息结构
{
	PVOID Context;								// 存放拥有回调函数的对象的this指针
	int iGrabberTypeSN;							// 采集卡类型编号
	int nGrabberSN;								// 采集卡序号
	int nErrorCode;								// 错误代码（0：正常）
	int nFrameCount;							// 已采集的帧记数
	char strDescription[GBMaxTextLen];			// 描述信息
	int iReserve1;								// 备用字段1
	int iReserve2;								// 备用字段2	
}s_GBSIGNALINFO;//回调信息结构

//回调函数指针声明
typedef void (WINAPI *PGBCALLBACK)(const s_GBSIGNALINFO* SignalInfo);

typedef struct _GBInitStruct//采集卡初始化结构
{
	int iGrabberTypeSN;						// 采集卡类型编号
	int nGrabberSN;							// 采集卡序号
    char strDeviceName[GBMaxTextLen];		// 设备名称
	char strDeviceMark[GBMaxTextLen];		// 设备标识  用于标识连接的相机与此标识一致
	char strGrabberFile[GBMaxFileNameLen];	// 用于初始化的文件
	PGBCALLBACK CallBackFunc;				// 回调函数指针
	PVOID Context;							// 存放拥有回调函数的对象的this指针	
	int iReserve1;							// 备用字段1
	int iReserve2;							// 备用字段2
}s_GBINITSTRUCT;//采集卡初始化结构

typedef struct _GBErrorInfoStruct//错误信息结构
{
   	int nErrorCode;							//错误代码（0：正常）
	char strErrorDescription[GBMaxTextLen];	//错误描述
	char strErrorRemark[GBMaxTextLen];		//附加信息
}s_GBERRORINFO;//错误信息结构

//公共参数
enum GBParamID
{
	GBImageWidth = 0,		//图像宽度 (单位:字节)
	GBImageHeight,			//图像高度 (单位:字节)
	GBImagePixelSize,		//图像象素大小 (单位:字节)
	GBImageBufferSize,		//图像缓冲区大小 (单位:字节)
	GBImageBufferAddr,		//图像缓冲区地址
	GBGrabberTypeSN			//采集卡类型编号
};

//错误代码定义
const int GBOK = 0;

//采集卡
class CGrabber
{
//操作
public:
	CGrabber()
	{
	}
	virtual ~CGrabber()
	{
	}
	
	/////////////////////////////////////////七个通用接口函数
	//初始化
	virtual BOOL Init(const s_GBINITSTRUCT* pInitParam) = 0;

	//关闭
	virtual BOOL Close() = 0;

	//开始采集
	virtual BOOL StartGrab() = 0;

	//停止采集
	virtual BOOL StopGrab() = 0;

	//单帧采集
	virtual BOOL Snapshot() = 0;
	
	//设置参数
	virtual BOOL SetParamInt(GBParamID Param, int nInputVal) = 0;
	
	//得到参数
	virtual BOOL GetParamInt(GBParamID Param, int &nOutputVal) = 0;
	
	//调用参数对话框
	virtual void CallParamDialog() = 0;

	//得到出错信息
	virtual void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo) = 0 ;
	
};

#endif // !define(DHGrabber_h_)

#if !defined(DHGrabberAVT_h_)
#define DHGrabberAVT_h_

//////////////////////////////////////////////////////////////////////////
//采集类
#define	_EXPORTING
#ifdef _EXPORTING
#define CLASS_DECLSPEC    __declspec(dllexport)  //程序编译时使用
#else
#define CLASS_DECLSPEC    __declspec(dllimport)	 //动态库应用时使用
#endif


enum AVTParamID	//AVT相机专有参数
{
	AVTColor,               //相机类型  黑白和彩色 //  [2010-5-27 yangfs]
	AVTBrightness,			//相机亮度大小
	AVTShutter,				//相机快门值
	AVTGain,				//相机增益大小
	AVTU,					//相机U值
 	AVTV,					//相机V值
	AVTTriggerOnOff			//相机外触发是否开启
};


// AVT相机专有接口
enum AVTParamVal
{
	AVTTriggerOff = 0,		//外触发关闭//内部触发,连续采集
	AVTTriggerOn,	    	//上升沿触发//外触发开启
	AVTTriggerMode2,		//下降沿触发
	AVTTriggerMode3,		//高电平触发
	AVTTriggerMode4,		//低电平触发
	AVTTriggerMode5,		//编程模式上升沿触发
	AVTTriggerMode6,		//编程模式下降沿触发	
};

class CLASS_DECLSPEC CDHGrabberAVT : public CGrabber
{
//操作
public:
	CDHGrabberAVT();
	virtual ~CDHGrabberAVT();
	
	//初始化
	BOOL Init(const s_GBINITSTRUCT* pInitParam);

	//关闭
	BOOL Close();

	//开始采集
	BOOL StartGrab();

	//停止采集
	BOOL StopGrab();

	//单贞采集
	BOOL Snapshot();
	
	//设置参数
	BOOL SetParamInt(GBParamID Param, int nInputVal);
	
	//得到参数
	BOOL GetParamInt(GBParamID Param, int &nOutputVal);

	//调用参数对话框
	void CallParamDialog();

	//得到出错信息
	void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo);

	//////////////////////////////////////////////////////////////////////////
	// AVT相机专有接口
	/************************************************************************/
	/*  功能：设置相机参数
	/*  输入：AVTParamID  参数类型 见上面的定义
	/*	输入：int nInputVal  参数对应的值  
	/*	返回：正确:TRUE   错误:FALSE
	/************************************************************************/
	BOOL AVTSetParamPro(AVTParamID Param, int nInputVal);	

	/************************************************************************/
	/*  功能：获取相机参数 
	/*  输入：AVTParamID  参数类型 见上面的定义
	/*	输出：int nOutputVal  参数对应的值  
	/*	返回：正确:TRUE   错误:FALSE
	/************************************************************************/
	BOOL AVTGetParamPro(AVTParamID Param, int &nOutputVal);

	/************************************************************************/
	/*  功能：设置相机参数
	/*  输入：AVTParamID  参数类型 见上面的定义
	/*	输入：AVTParamVal  参数对应的值  见上面的定义
	/*	返回：正确:TRUE   错误:FALSE
	/************************************************************************/
	BOOL AVTSetParamPro(AVTParamID Param, AVTParamVal ParamValInput);

	/************************************************************************/
	/*  功能：获取相机参数 
	/*  输入：AVTParamID  参数类型 见上面的定义
	/*	输出：AVTParamVall  参数对应的值   见上面的定义
	/*	返回：正确:TRUE   错误:FALSE
	/************************************************************************/
	BOOL AVTGetParamPro(AVTParamID Param, AVTParamVal &ParamValOutput);

	/************************************************************************/
	/*  功能：设置相机采集方式 
	/*  输入：AVTParamVal  参数类型 见上面的定义
	/*	输出：无
	/*	返回：正确:TRUE   错误:FALSE
	/************************************************************************/
	BOOL AVTSetTriggerParam(AVTParamVal ParamValInput);

	/************************************************************************/
	/*  功能：获得相机的序列号
	/*  输出：nGaidHigh  高六位
	/*	输出：nGaidLow   低九位
	/*	返回：正确:TRUE   错误:FALSE
	/************************************************************************/
	BOOL AVTGetCamGuid(int &nGaidHigh,int &nGaidLow);

	/************************************************************************/
	/*  功能：获得采集窗口区域
	/*  输出：RECT    采集窗口大小
	/*	返回：正确:TRUE   错误:FALSE
	/************************************************************************/
	BOOL AVTGetAOI(RECT &rtAOI);

	/************************************************************************/
	/*  功能：设置采集窗口区域
	/*  输入：RECT    采集窗口大小
	/*	返回：正确:TRUE   错误:FALSE
	/************************************************************************/
	BOOL AVTSetAOI(RECT rtAOI);

	//////////////////////////////////////////////////////////////////////////
//属性
protected:
	CGrabber *m_pGrabber;
};

#endif// !defined(DHGrabberAVT_h_)

//////////////////////////////////////////////////////////////////////////
//使用说明:   目前采集库基于AVT相机的驱动版本为2.10  
//适用相机类型：Guppy、Stingray、Marlin系列
//1. 将原来的ini相机参数配置文件直接更换为相机默认的参数配置文件XML
//2. 将相机配置修改如下：
//   设备A序号=0
//   设备A名称=6735372523180860609 (可以自己定义一个名称)
//   设备A标识=6735372523180860609
//   设备A初始化文件名=GuppyConfig.xml   //将用AVT 的SmartView保存的xml文件名称更换到这里
//3. 另外将保存的XML文件拷贝到Config文件夹下(即原来的GuppyConfig.ini同级目录下)
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//修改记录
/*
2010.12.7  V4.0.0.3  杨富森**目的：修改多个相机读写XML文件文件***
修改内容：
1. 解决多个相机时，修改参数保存到XML文件的问题；
2. 相机配置序列号，可以只是写入前面的15位(6+9),将相机配置修改如下：
设备A序号=0
设备A名称=AVT
设备A标识=673537252318086
设备A初始化文件名=GuppyConfig.xml   //将用AVT 的SmartView保存的xml文件名称更换到这里
另外还兼容后面带有0609版本
3. 修改接口AVTGetCamGuid名称，由原来的AVTGetCamGaid修改成AVTGetCamGuid
4. 程序退出默认保存XML文件

2010.5.24  V4.0.0.1Beta1  **目的：从原有的ini配置文件升级直接使用相机保存的xml文件***
修改内容：
1. 将原来的ini相机参数配置文件直接更换为相机默认的参数配置文件XML；
2. 将相机配置修改如下：
设备A序号=0
设备A名称=6735372523180860609
设备A标识=6735372523180860609
设备A初始化文件名=GuppyConfig.xml   //将用AVT 的SmartView保存的xml文件名称更换到这里
3. 另外将保存的XML文件拷贝到Config文件夹下(即原来的GuppyConfig.ini同级目录下)

2010.5.26  V4.0.0.1Beta2  **目的：在停止采集时，可以在相机参数对话框上，将修改的相机参数保存为XML**
修改内容：
1. 可以在相机参数中保存为XML；Beta2
2. 添加获取相机类型(黑白和彩色)功能；通过AVTColor参数获得；

2010.6.21  V4.0.0.1Beta3  **目的：方便在检测过程中(相机正在采集时)调整的相机参数，用于下一次使用**
修改内容：
1. 相机退出时再保存一次XML。

2010.6.23  V4.0.0.1Beta4  
修改内容：
1. 解决多个AVT相机采集的问题；
*/
//////////////////////////////////////////////////////////////////////////