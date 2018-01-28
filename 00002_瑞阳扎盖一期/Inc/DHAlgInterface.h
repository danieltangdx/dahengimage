

/************************************************************************/
/* Copyright (c) 2016, 北京大恒图像视觉有限公司  
/* All rights reserved.													

// 2015-08-21
//灯检机特殊类型结构体定义

2016.09.21
李靓：增加异常文件存图

2016-10-08
错误类型完善：鲁威威、王鑫、李靓、邵建超、申磊
错误类型细化：错误类型	(0:OK,1:NA,20:扎盖,30:装量,40:异物,50:瓶脖裂纹,51:瓶身裂纹（瓶身下部-明场）,52:瓶底裂纹,60:结石,70:塑盖,80:其他)

2016-10-11 
李靓：完善异常存图传入方法，不再通过s_DetectInParam传入
s_DetectModelInput中增加异常图像文件夹路径，算法catch到异常后自动保存异常图像
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


//初始化检测信息
typedef struct _SCHECKCAMERAINFO
{
	int iHeight;  //高度
	int iWidth;   //宽度
	int iBitCount;//位数
	
	int iType;    //检测类型
	int iCount;   //张数
	
}s_CheckCameraInfo;

// 检测算法初始化
typedef struct _DetectInitParam 
{
	int* pHWnd;						// 窗口句柄(窗口传入0表示重新加载算法参数)
	int  iAlgSN;				    // 算法号(1:算法一	2:算法二  n:算法n)
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
	void* pImageInfo;	// pImageInfo传入s_CheckCameraInfo结构体指针首地址

	int* pImageData;	// 图像数据
	int	iDisplayMode;	// 显示模式(0:不显示,1:显示所有,2:显示有瓶,3:显示好品,4:显示坏品,5:显示无瓶)

	int iCircleIndex;	// 图像Circle号
	BOOL bSnapScreen;	// 是否抓屏 true表示抓屏，false表示不抓屏
	int  iScreenIndex;  // 抓屏序号 图片命名规则checksn-snapindex.jpeg，比如0-1.jpeg
	int	iRejectTest;	// 0表示正常检测 1表示“踢废测试 全NG” 2表示“踢费测试 全OK”

/////2016.05.25新增存图/////////////////////////////////////////////////////////////////////
	int iSaveType;		// 存图类型 0-所有 1-坏品 2-好品，-1-异常
	char* cSavePath;	// 存图路径指定到文件夹，比如R:\SaveImages\AllImages\2016-10-14 09-38. 算法根据iCricleIndex和iSaveType以及iCheckSN保存图像
						// 保存图片命名规则：format("%s\\Camera%d\\ProductID(%06d)\\Type(%d)_%02d.bmp",
											//cSavePath,iCheckSN+1,iCircleIndex,iSaveType,序号);
						
//////////////////////////////////////////////////////////////////////////

	int	iReserve1;		// 保留
	int iReserve2;
	int iReserve3;

}s_DetectInParam;

// 检测输出参数
typedef struct _DetectOutParam 
{  
    int		iErrorType;					// 错误类型	(0:OK,1:NA,20:扎盖,30:装量,40:异物,50:瓶脖裂纹,51:瓶身裂纹（瓶身下部-明场）,52:瓶底裂纹,60:结石,70:塑盖,80:其他)
	int		iBubbleValue;				// 气泡标志0--没有气泡，1--有气泡(iReserve1改为iBubble)
	int		iImageBrightness;			// 图像亮度（iReserve2改为iLightValue）
	int     iTimeConsuming;				// 检测耗时
	
	int		iDefectCount;				// 缺陷个数

	char	chCheckInfo[MAX_LENGTH];	// 调试信息
	
	int		iReserve1;
	int		iReserve2;
	int		iReserve3;

}s_DetectOutParam;

typedef struct _DetectModelInput
{
	char    chPath[MAX_LENGTH];		// 参数路径(传入..\Run\Prescription\Alg)
	char    chLog[MAX_LENGTH];     	// 日志文件路径(记录参数修改记录)
	char    chImageBufferPath[MAX_LENGTH];//检测界面截图路径 ..\ImageBuffer

	
	// 前端传入路径格式：R:\SaveImages\AbnormityImages,算法自己创建子目录
	// 出现异常时的命名例子：R:\SaveImages\AbnormityImages\2016-10-14 09-38-00\Camera1\\ProductID(000001)\Type(-1)_01.bmp
	char    chAbnormalPath[MAX_LENGTH];	//异常图片存放路径，算法catch到异常时存图。

}s_DetectModelInput;

#endif // !defined(AFX_ALGRITHM_INTERFACE_H_)

