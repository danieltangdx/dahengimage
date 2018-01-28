/************************************************************************/
/* Copyright (c) 2014, 北京大恒图像视觉有限公司  
/* All rights reserved.													

// 2015-08-21
//灯检机特殊类型结构体定义
/************************************************************************/
#include <vector>
using namespace std;

#if !defined(AFX_DHALGINTERFACE_H_)
#define AFX_DHALGINTERFACE_H_


#define MAX_LENGTH		 2048		

//错误类型定义
enum e_ErroeType
{
	Status_OK,   	    	         //正常
	Status_Err_LicenseInvalid,       //Halcon许可文件丢失
	Status_Err_LicenseMismatch,      //Halcon版本不匹配
	Status_Err_WinFailed,            //打开显示窗口失败
	Status_Err_ParaIni,	             //配置文件读取错误
	Status_Err_Model,                //模板文件错误
	Status_Err_ImageLess,  	         //图像数据不足错误
	Status_Err_ImageBlack,           //序列图像中存在黑图
	Status_Err_TimeOut,              //超时
	Status_Err_HalconAbnor,          //halcon算法异常
	Status_Err_CheckFailed,          //demo检测异常
	Status_Err_LightFailed,          //光源亮度错误
	Status_Err_Other,                //其他错误类型
};

//缺陷类型定义
enum e_DefectType
{ 
	OK,                              //好品 
	NA,                              //无瓶 
	glass,                           //玻璃 
	fiber,                           //纤维
	foregionparticle,                //异物
	cap,                             //瓶盖外观
	body,                            //瓶身外观
	capacity,                        //装量
	other                            //其他
};

//初始化检测信息
typedef struct _SCHECKCAMERAINFO
{
	int iHeight;  //高度
	int iWidth;   //宽度
	int iBitCount;//位数
	
	int iType;    //检测类型
	int iCount;   //张数
	
}s_CheckCameraInfo;

// 初始化
typedef struct _DetectInitParam 
{
	int* pHWnd;						// 窗口句柄(窗口传入0表示重新加载算法参数)
	int  iAlgSN;				        // 算法号(1:算法一	2:算法二  n:算法n)
	int  iRunMode;					// 0-ONLINE 1-OFFLINE,区分在线和离线程序
		
	char chLanPath[MAX_LENGTH];		// 语言文件路径 
	char chLanguage[MAX_LENGTH];	// 2014-09-15语言类型 Chinese\English\Russian...

	int  iReserve1;
	int  iReserve2;
	int  iReserve3;
	int  iReserve4;
	
}s_DetectInitParam;


// 检测输入参数
typedef struct _DetectInParam 
{
	vector<s_CheckCameraInfo> vCameraInfo;   //图像信息
	int* pImageData;	// 图像数据
	int	iDisplayMode;	// 显示模式(0:不显示,1:显示有瓶无瓶图像,2:显示周期所有图像3:只显示有瓶图像4:只显示坏品图像5:只显示无瓶图像)

	int iCircleIndex;	// 2014-09-15图像Circle号
	BOOL bSnapScreen;	// 是否抓屏 true表示抓屏，false表示不抓屏
	int  iScreenIndex;  // 抓屏序号 图片命名规则checksn-snapindex.jpeg，比如0-1.jpeg
	int	iReserve1;		// 保留
	int	iReserve2;		// 保留
	int iReserve3;
	int iReserve4;

}s_DetectInParam;

// 检测输出参数
typedef struct _DetectOutParam 
{  
    int		iErrorType;					// 错误类型	水针(0:OK,1:NA,2:玻璃,3:纤维,4:异物,5:外观,6:液位)
										//			冻干(0:OK,1:NA,2:异物,3:外观(胶塞/喷瓶),4:装量,5:轧盖,6:裂纹)
	int		iBubbleValue;				// 气泡标志0--没有气泡，1--有气泡(iReserve1改为iBubble)
	int		iImageBrightness;			// 图像亮度（iReserve2改为iLightValue）
	int     iTimeConsuming;				// 2014-07-22检测耗时
	
	int		iDefectCount;				// 2014-07-22缺陷个数

	char	chCheckInfo[MAX_LENGTH];	// 调试信息
	
	int		iReserve1;
	int		iReserve2;
	int		iReserve3;

}s_DetectOutParam;

//
typedef struct _DetectModelInput
	{
		char    chPath[MAX_LENGTH];		// 参数路径(传入..\Run\Prescription\Alg)
		char    chLog[MAX_LENGTH];     	// 日志文件路径(记录参数修改记录)
	    char    chImageBufferPath[MAX_LENGTH];//检测界面截图路径 ..\ImageBuffer
	}s_DetectModelInput;

#endif // !defined(AFX_ALGRITHM_INTERFACE_H_)

