//--------------------------------------------------------------- 
/** 
\file      CameraBase.h
\brief     Declarations for camerabase method 
\version   v1.0.1305.9301 
\date      2013-5-30
*/ 
//--------------------------------------------------------------- 

#if !defined(_CAMERABASE_H)
#define _CAMERABASE_H

#include "GxIAPI.h"
#include "DxImageProc.h"
#include "DHGrabForMER.h"
#include "SetParamDialog.h"
#include <string>
#include <iostream>
using namespace std;

#define _WIN32_DCOM
#include <ObjBase.h>
#pragma comment(lib, "ole32.lib")

#include "Vfw.h"
#pragma comment(lib, "Vfw32.lib")

// 初始化文件名的最大长度
const int MaxFileNameLen = 255;

///< ´íÎóÌáÊ¾º¯Êýºê¶¨Òå
#define  GX_VERIFY(emStatus) \
	if (emStatus != GX_STATUS_SUCCESS)\
                                     {\
	                                  ShowErrorString(emStatus); \
	                                  return ;\
                                     } 
///< ÅÐ¶Ï·µ»ØÖµºê¶¨Òå
#define VERIFY_STATUS_RET(emStatus) \
 if (emStatus != GX_STATUS_SUCCESS) \
                                   {\
	                                return emStatus;\
                                   }\

///////////////////////////////////////////////////////////+++
// 一些结构体的定义

// typedef struct _HVCamDHSignalInfoStruct		//回调信息结构
// {
// 	PVOID Context;								// 存放拥有回调函数的对象的this指针
// 	int iGrabberSN;								// 采集卡序号
// 	int iErrorCode;								// 错误代码（0：正常）
// 	int iFrameCount;							// 已采集的帧记数
// 	TCHAR lpszDescription[MaxFileNameLen];		// 描述信息
// }s_DC_SIGNALINFO;	//回调信息结构
// 
// 
// typedef struct _HVCamDHInitStruct			// 参数初始化结构
// {
// 	int iGrabberTypeSN ;					//采集卡类型序号
// 	int iGrabberSN;							// 采集卡序号
// 	TCHAR strDeviceName[MaxFileNameLen];		// 设备名称
// 	TCHAR strDeviceMark[MaxFileNameLen];		// 设备名称
// 	
// 	TCHAR strGrabberFile[MaxFileNameLen];	// 用于初始化的文件
// 	PGBCALLBACK CallBackFunc;				// 回调函数指针
// 	PGBEVENTCALLBACK EventCallbackFunc;		// 触发不响应事件回调函数指针
// 	PVOID Context;							// 存放拥有回调函数的对象的this指针
// }s_DC_INITSTRUCT;// 参数初始化结构
// 
// typedef struct _DCErrorInfoStruct//错误信息结构
// {
//    	int iErrorCode;							// 错误代码（0:正常）
// 	TCHAR strErrorDescription[MaxFileNameLen];	// 描述信息
// 	TCHAR strErrorRemark[MaxFileNameLen];		// 附加信息
// }s_DCERRORINFO;//错误信息结构


enum DCErrorStruct
{
	DCErrorNoFindCam = 0,	// 没有找到相机
		DCErrorGrabberSN,		// 相机序号超出能够找到的相机数
		DCErrorGetCamInfo,		// 得到相机信息失败
		DCErrorValOverFlow,		// 数据溢出
		DCErrorOpenCam,			// 打开相机失败
		DCErrorInit,			// 初始化失败
		DCErrorStartGrab,		// 开始采集失败
		DCErrorStopGrab,		// 停止采集失败
		DCErrorClose,			// 关闭失败
		DCErrorSendBufAdd,		// 传出图像内存地址失败
		DCErrorGetParam,		// 非法获取参数
		DCErrorSetParam,		// 非法设置参数
		DCErrorParamDlg,		// 调用参数设置对话框失败
		DCErrorReadIniFile		// 读取配置文件失败
};


//////////////////////////////////////////////////////////////////////////

class CVxCriticalSection  
{
public:
	/// 构造
	CVxCriticalSection () {InitializeCriticalSection(&m_cs);}
	/// 析构
	virtual ~CVxCriticalSection() {DeleteCriticalSection(&m_cs);}
public:
	/// 进入临界区
	void Enter(){EnterCriticalSection(&m_cs);}
	/// 离开临界区
	void Leave(){LeaveCriticalSection(&m_cs);}
private:
	CRITICAL_SECTION m_cs; ///< 临界段变量	
};


class CSetParamDialog;
class CameraBase:public CGrabber
{
public:
	 CameraBase();
	 virtual ~CameraBase();
public:
	// 初始化
	BOOL Init(const s_GBINITSTRUCT* pInitParam);
	virtual bool Init(const CGrabInitInfo& rGrabInitInfo);

	//开始采集
	BOOL StartGrab(); 
	//停止采集
	BOOL StopGrab(); 
	// 单帧采集 
	BOOL Snapshot();			
	//关闭
	BOOL Close();
	//设置通用参数
	BOOL SetParamInt(GBParamID Param, int nReturnVal);	
	//得到通用参数
	BOOL GetParamInt(GBParamID Param, int &nReturnVal);	
	bool GetParamBool(GBParamID Param, bool bRetVal)override;

	bool SetParamBool(GBParamID Param, bool bInputVal)override;
	//调用参数对话框
	void CallParamDialog();
	// 获得错误信息
	void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo);

	//设置专有参数
	BOOL MERSetParamInt(MERParamID Param,int nInputVal1,int nInputVal2 = 0,int nInputVal3 = 0,int nInputVal4 = 0,int nInputVal5 = 0,void *sInputVal6 = NULL);	
	//得到相机专有参数
	BOOL MERGetParamInt(MERParamID Param, int &nReturnVal1,int &nReturnVal2,int &nReturnVal3);
	//保存相机设置信息
	void SaveParamToINI();
	//输出端口发送信号
	BOOL SetOutputValue(int nOutputPort,int nOutputValue);
	//初始化相机参数
	void InitParamFromINI();	
	// 设置参数
	void SetInitParam();//设置相机使用参数

	int GetCameraMaxWid();//获得相机的最大宽度
	int GetCameraMaxHei();//获得相机的最大高度

	int m_maxwid;
	int m_maxhei;//

	//GX_STATUS Open(const s_DC_INITSTRUCT* pInitParam);
	GX_STATUS Open(const s_GBINITSTRUCT* pInitParam);
	bool      IsOpen();         
	GX_STATUS RegisterShowWnd(void* pWnd);
	GX_STATUS RegisterLogWnd(void* pWnd);
	bool      IsSnaping();
	GX_STATUS StartCallbackAcq();
	GX_STATUS StopCallbackAcq();
	GX_STATUS SnapCallbackAcq();

	void      SetBMPFolder(TCHAR* pBMPFolder);
	void      SetAVIFolder(TCHAR* pAVIFolder);
	void      EnableShowImg(bool bShowImg);
	void      EnableSaveBMP(bool bSaveBMP);
	void      CompressedAVI(bool bCompressed);
	GX_STATUS EnableSaveAVI(bool bSaveAVI);	
	GX_STATUS IsImplemented(GX_FEATURE_ID emFeatureID, bool* pbIsImplemented);
	GX_STATUS GetFeatureName(GX_FEATURE_ID emFeatureID, char* pszName, size_t* pnSize);
	GX_STATUS GetIntRange(GX_FEATURE_ID emFeatureID, GX_INT_RANGE* pIntRange);
	GX_STATUS GetInt(GX_FEATURE_ID emFeatureID, int64_t* pnValue);
	GX_STATUS SetInt(GX_FEATURE_ID emFeatureID, int64_t nValue);
	GX_STATUS GetFloatRange(GX_FEATURE_ID emFeatureID, GX_FLOAT_RANGE* pFloatRange);
	GX_STATUS GetFloat(GX_FEATURE_ID emFeatureID, double* pdValue);
	GX_STATUS SetFloat(GX_FEATURE_ID emFeatureID, double dValue);
	GX_STATUS GetEnumEntryNums(GX_FEATURE_ID emFeatureID, uint32_t* pnEntryNums);
	GX_STATUS GetEnumDescription(GX_FEATURE_ID emFeatureID, GX_ENUM_DESCRIPTION* pEnumDescription, size_t* pBufferSize);
	GX_STATUS GetEnum(GX_FEATURE_ID emFeatureID, int64_t* pnValue);
	GX_STATUS SetEnum(GX_FEATURE_ID emFeatureID, int64_t nValue);
	GX_STATUS GetBool(GX_FEATURE_ID emFeatureID, bool* pbValue);
	GX_STATUS SetBool(GX_FEATURE_ID emFeatureID, bool bValue);
	GX_STATUS GetStringLength(GX_FEATURE_ID emFeatureID, size_t* pnSize);
	GX_STATUS GetString(GX_FEATURE_ID emFeatureID, char* pszContent, size_t* pnSize);
	GX_STATUS SetString(GX_FEATURE_ID emFeatureID, char* pszContent);
	GX_STATUS GetBufferLength(GX_FEATURE_ID emFeatureID, size_t* pnSize);
	GX_STATUS GetBuffer(GX_FEATURE_ID emFeatureID, uint8_t* pBuffer, size_t* pnSize);
	GX_STATUS SetBuffer(GX_FEATURE_ID emFeatureID, uint8_t* pBuffer, size_t nSize);
	GX_STATUS SendCommand(GX_FEATURE_ID emFeatureID);
	GX_STATUS GetImage(GX_FRAME_DATA* pFrameData, int32_t nTimeout);
	void      EnableGetImageProcess(bool bEnableGetImageProcess);

public:
	//相机参数
	int m_nWidth;//感兴趣区域宽度
	int m_nHeight;//感兴趣区域高度
	int m_nOffsetX;//感兴趣区域X偏移量
	int m_nOffsetY;//感兴趣区域Y偏移量
	int m_nExposureMode;//曝光模式
	double m_dExposureTime;//曝光时间
	bool m_bExposureAuto;//自动曝光使能
	int m_nGain;//增益值
	bool m_bGainAuto;//自动增益使能
	int m_nBlacklevel;//黑电平值
	bool m_bBlacklevelAuto;//自动黑电平使能
	double m_dBalanceRatio;//白平衡系数
	bool m_bBalanceRatioAuto;//自动白平衡使能
	bool m_bTriggerMode;//外触发模式开关
	int m_nTriggerActivation;//触发极性 0上升沿 1下降沿
	int m_nTriggerSource;		//触发源 0软件触发 1是Line0, 2是Line1, 3是Line2  [2016-11-17 zhy]
	/*************************************<>************************************[12/18/2017 ConanSteve]*/
	int m_iImageWidth{ 0 };//**[9/7/2017 ConanSteve]**:  相机当前图像宽
	int m_iImageHeight{ 0 };//**[9/7/2017 ConanSteve]**:  相机当前图像高
	int m_iOffsetX{ 0 };
	int m_iOffsetY{ 0 };
	int m_iImagePixelSize{ 1 };//**[9/7/2017 ConanSteve]**:  相机采集原图每个图素的字节数
	int m_iExposureTime{ 1000 };//**[9/11/2017 ConanSteve]**:  曝光时间
	int m_iGain{ 0 };//**[9/11/2017 ConanSteve]**:  增益
	BOOL m_bIsColorFilter{ FALSE };//**[9/7/2017 ConanSteve]**:  相机是否支持Bayer变换
	int  m_iBayerType{ 0 };//**[9/11/2017 ConanSteve]**:  Bayer转换类型
	BOOL m_bEnableFlip{ FALSE };//**[9/7/2017 ConanSteve]**:  是否翻转
	CSetParamDialog* m_pParamSetDlg;

protected:
	//初始化变量	
	int m_nGrabberSN;						// 序号
	
	// 增加采集卡类型返回值
	int m_nGrabberType;

	bool m_bInitSuccess;//是否初始化成功
	int m_nTotalNum;		// 摄象机总数
	
	CString m_sInitFile;					// 用于初始化的文件
	CString m_sDeviceName;					// 设备名
	CString m_strDeviceMark;					// 设备标识
	
	PGBCALLBACK m_CallBackFunc;				// 回调函数指针
	//PGBEVENTCALLBACK m_EventCallbackFunc;	// 触发不响应事件的回调函数
	PGBCALLBACK m_EventCallbackFunc;
	PVOID m_Context;						// 存放调用初始化函数的对象的this指针
	
	//s_DCERRORINFO m_LastErrorInfo;			// 错误信息
	s_GBERRORINFO m_LastErrorInfo;

	// 像素的字节
	int m_nImageByteCount;
	int m_nFrameCount;	// 采集帧计数

	GX_FEATURE_CALLBACK_HANDLE m_hCB;		//定义属性更新回调函数句柄

private:
	GX_DEV_HANDLE m_hDevice;           ///< 设备句柄
	bool          m_bIsOpen;           ///< 设备打开状态
	bool          m_bIsSnaping;        ///< 设备采集状态
	CString        m_strBMPFolder;      ///< bmp图片保存目录
	CString        m_strAVIFolder;      ///< avi视频保存目录
	bool          m_bShowImg;          ///< 是否显示图像，默认值为true
	bool          m_bSaveBMP;          ///< 是否保存bmp图，默认值为false
	bool          m_bSaveAVI;          ///< 是否保存AVI视频，默认值为false
	BITMAPINFO    *m_pBmpInfo;         ///< 用来显示图像的结构指针
	BYTE          *m_pImgRGBBuffer;    ///< 存储RGB图像缓冲区的指针
	BYTE          *m_pImgRaw8Buffer;   ///< 存储Raw8图像缓冲区的指针
	char          *m_chBmpBuf;         ///< BIMTAPINFO 存储缓冲区，m_pBmpInfo即指向此缓冲区
	int           m_nWndWidth;         ///< 图像显示窗口的宽
	int           m_nWndHeight;        ///< 图像显示窗口的高
	int64_t       m_nImageWidth;       ///< 原始图像宽
	int64_t       m_nImageHeight;      ///< 原始图像高
	int64_t       m_nPayLoadSize;      ///< 图像大小
	void          *m_pShowWnd;         ///< 指向图像显示窗口的指针
	void          *m_pDC;              ///< 图像显示窗口指针
	void          *m_pLogWnd;          ///< 指向日志显示窗口的指针
	int64_t       m_nPixelColorFilter; ///< 像素格式
	int64_t       m_nPixelFormat;      ///< 数据格式
	bool          m_bEnableGetImageProcess; ///< 是否允许GetImage接口自行处理获取的数据，进行显示或者保存BMP


	//AVI相关数据
	bool        m_bCompressed;  ///< 是否压缩存储AVI
	PAVIFILE    m_pAVIFile;     ///< AVI文件的句柄
	PAVISTREAM	m_ps;           ///< 通过AVI文件创建的视频流
	PAVISTREAM	m_psCompressed; ///< 通过视频流和编码器创建的压缩视频流
	int         m_nTimeFrame;   ///< 写入avi文件需要的帧ID号
	HIC         m_hic;          ///< 标记使用的编码器
	CVxCriticalSection m_SaverCriSec;     ///< 保存互斥锁

private:
	GX_STATUS __PrepareForShowImg();
	void __UnPrepareForShowImg();
	void __ProcessData(BYTE * pImageBuf, BYTE * pImageRGBBuf, int64_t nImageWidth, int64_t nImageHeight);
	void __SaveBMP(BYTE * pImageBuf, int64_t nImageWidth, int64_t nImageHeight);
	static void __stdcall __OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM* pFrame);
	static void __stdcall OnFeatureCallbackFun(GX_FEATURE_ID_CMD emFeatureID, void* pUserParam);
};

inline CString  MsgComposer( const TCHAR* pFormat, ... )
{
	TCHAR pBuffer[ 256 ];
	va_list vap;
	va_start( vap, pFormat );
	

#if _MSC_VER >= 1400 //for vc8 vc9
	_vsntprintf( pBuffer, 256, pFormat, vap );
#elif _MSC_VER >= 1200 //for vc6
	_vsntprintf( pBuffer, 256, pFormat, vap );
#endif

	va_end(vap);
	return CString( pBuffer);

	
};

#endif  //_CAMERABASE_H
