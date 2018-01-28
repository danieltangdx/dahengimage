// AVTCamDH.h: interface for the CAVTCamDH class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AVTCAMDH_H__DED529A9_1D8C_4586_AB4D_5583AC9C174D__INCLUDED_)
#define AFX_AVTCAMDH_H__DED529A9_1D8C_4586_AB4D_5583AC9C174D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Afxmt.h"
#include <FGCamera.h>

#include "DHGrabForAVT.h"


typedef struct
{
  BOOL          LimitSpeed;                             // Limit speed
  UINT32HL      Guid;                                   // GUID for this camera
  UINT8         NodeId;                                 // ID of this node
  TCHAR          DeviceName[128];                        // Name of device
  CFrameWnd     *pCameraFrame;                           // Pointer to camera frame
}CAMERAINFO;

enum ErrorStruct
{
	ErrorNoFindCam = 0,		// 没有找到相机
	ErrorGrabberSN,			// 相机序号超出能够找到的相机数
	ErrorGetCamInfo,		// 得到相机信息失败
	ErrorValOverFlow,		// 数据溢出
	ErrorOpenCam,			// 打开相机失败
	ErrorInit,				// 初始化失败
	ErrorStartGrab,			// 开始采集失败
	ErrorStopGrab,			// 停止采集失败
	ErrorClose,				// 关闭失败
	ErrorSendBufAdd,		// 传出图像内存地址失败
	ErrorGetParam,			// 非法获取参数
	ErrorSetParam,			// 非法设置参数
	ErrorParamDlg,			// 调用参数设置对话框失败
	ErrorReadIniFile		// 读取配置文件失败
};

typedef struct
{
  UINT32                ImageFormat;  //图像格式
  UINT32				Format7ModeAdress;  //Format7ModeX基址
  UINT32                XSize;        //图像宽度
  UINT32                XSizeMax;     //图像最大宽度
  UINT32                XSizeMin;     //图像最小宽度
  UINT32                XSizeUnit;	  //图像宽度横向分辨率
  
  UINT32                YSize;        //图像高度
  UINT32                YSizeMax;     //图像最大高度
  UINT32                YSizeMin;     //图像最小高度
  UINT32                YSizeUnit;	  //图像高度纵向分辨率
  
  UINT32                XPos;         //图像左上角横坐标
  UINT32                XPosMax;      //图像左上角最大横坐标
  UINT32                XPosMin;      //图像左上角最小横坐标
  UINT32                XPosUnit;     //图像左上角横坐标分辨率
  
  UINT32                YPos;        //图像左上角纵坐标
  UINT32                YPosMax;     //图像左上角最大纵坐标
  UINT32                YPosMin;     //图像左上角最小纵坐标
  UINT32                YPosUnit;    //图像左上角纵坐标分辨率
  
  UINT32                Trigger;     //触发方式
  UINT32				Debounce;
  UINT32				DebounceMin;
  UINT32				DebounceMax;

  UINT32                FrameBufferCount;    //缓冲帧数

  UINT8                 BayerPattern;           // Used bayer pattern
  UINT8                 YIsRawMode;             // Raw mode flag
  UINT8                 Y16Invert;              // Inversmode for Y16
  
  UINT32                DmaFlags;               // Dma flags

  UINT32                WhiteBalanceCB;         //白平衡CB分量  
  UINT32                WhiteBalanceMinCB;      //白平衡最小CB分量
  UINT32                WhiteBalanceMaxCB;      //白平衡最大CB分量
  
  UINT32                WhiteBalanceCR;         //白平衡CR分量
  UINT32                WhiteBalanceMinCR;      //白平衡最小CR分量
  UINT32                WhiteBalanceMaxCR;      //白平衡最大CR分量

  UINT32                Brightness;     //亮度
  UINT32                BrightnessMin;  //亮度最小值
  UINT32                BrightnessMax;  //亮度最大值
  UINT32                Gain;           //增益
  UINT32                GainMin;        //增益最小值
  UINT32                GainMax;        //增益最大值
  UINT32                Shutter;        //曝光
  UINT32                ShutterMin;     //曝光最小值
  UINT32                ShutterMax;     //曝光最大值
}CAMPARMS;


const int MaxFileNameLen = 255;			// 初始化文件名的最大长度

static int   m_iMaxGrabberSN=0;         //最大的一个采集的SN  

const int iMaxCameraNum = 20;			//采集库支持的最大相机数

typedef struct _AVTInitStruct			// 参数初始化结构
{
	int iGrabberTypeSN;						// 采集卡类型编号
	int nGrabberSN;							// 采集卡序号
    TCHAR strDeviceName[GBMaxTextLen];		// 设备名称
	TCHAR strDeviceMark[GBMaxTextLen];		// 设备标识
	TCHAR strGrabberFile[GBMaxFileNameLen];	// 用于初始化的文件
	PGBCALLBACK CallBackFunc;				// 回调函数指针
	PVOID Context;							// 存放拥有回调函数的对象的this指针	
}s_AVT_INITSTRUCT;// 参数初始化结构

//static CEvent m_NodelistChangedEvent;	//相机列表变化事件
void WINAPI FGCallBackFunc(void* Context,UINT32 wParam,void* lParam); //采集回调函数

class CAVTCamDH : public CGrabber 
{
private:
	//////////////////////////////////////////////////////////////////////////
	//相机属性
	int             m_nGaidHigh;             //序列号高位值六位
	int             m_nGaidLow;              //序列号低位值九位  
	BOOL            m_InitSucceed;           //初始化是否成功
	BOOL            m_bSnapShot;			 //是否软触发采集

public:
	CAMPARMS        m_CamParms;             // Some parameters...
	CAMERAINFO     *m_pCameraInfo;          // Node lists camera info    
	BOOL            m_CameraActive;         // Camera active or not
	BOOL            m_bIsColorCamera;       //是否为彩色相机 
	CFGCamera       m_Camera;               // Our camera we work with
	DWORD           m_FrameCnt;             // Number of received frames
	int				m_nMultishotNum;        //设定Multishot采集桢数

	int      m_nImageWidth;          //图像宽度
	int      m_nImageHeight;         //图像高度
	int      m_nImageBufferSize;     //图像缓冲区大小
	int      m_nImageByteCount;      //图像字节数	
	BYTE*    m_pbImageBufferAdd;  // 用于传出的图像内存地址
	//////////////////////////////////////////////////////////////////////////
	BOOL m_bSet1,m_bSet2,m_bSet3,m_bSet4,m_bSet5;		//控制标志位
	
public:	
	CAVTCamDH();
	virtual ~CAVTCamDH();

	//公共接口
	BOOL Init(const s_GBINITSTRUCT* pInitParam);
	virtual bool Init(const CGrabInitInfo& rGrabInitInfo) override;
	BOOL Close();	  // 关闭
	BOOL StartGrab(); // 开始采集
	BOOL StopGrab();  // 停止采集
	BOOL Snapshot(); // 单帧采集
	BOOL SetParamInt(GBParamID Param, int nReturnVal);   
	BOOL GetParamInt(GBParamID Param, int &nReturnVal);
	
	//设置参数
	BOOL SetParamfloat(int Param, float nInputVal);
	
	//得到参数
	BOOL GetParamfloat(int Param, float &nOutputVal);

	void CallParamDialog();//调用参数对话框
	void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo);//得到出错信息
	
	BOOL GetTriggerParam(int &ParamValOutput);
	BOOL SetTriggerParam(int ParamValInput);
	BOOL GetSystemInfo(CString &sOutputInfo);
	BOOL SetAOI(RECT rtAOI);
	BOOL GetAOI(RECT &rtAOI);
	BOOL Reset();
	BOOL CreateDefaultConfigurationFile(CString &sInitFile);

	//回调函数
	void OnNewFrame();
	void OnNodelistChanged();
	void OnError(int nError);
protected:

	void CheckResult(CString pFunction,DWORD Result,int n);
	BOOL SetCameraFromINI();
	
	BOOL Init_Grabber_Module(); // 初始化采集模块 
	BOOL Exit_Grabber_Module(); // 释放采集模块
	BOOL InitParm(const s_AVT_INITSTRUCT* pInitParam); // 初始化
	BOOL InitCamera();
//	int TestFramerate();
	//BOOL m_bTestFramerate;

	//判断路径是否存在
	BOOL IsPathExist(const CString &strPath);
	//判断文件是否存在
	BOOL IsFileExist(const CString &strFileName);
	//创建一个多层目录，如果存在就不创建
	//(strPath  合法: "D:\\abc\\me" 或 "me\\you" 或 ""; 
	//			不合法: "D:\\abc\\me\\" 或 "me\\you\\" 或 "\\")
	BOOL CreateMultiLevelPath(const CString &strPath);

	//保存配置文件
	BOOL SaveToConfigurationFile(CString &sInitFile);

	int m_nGrabberSN;				// 序号

	// 语言类型 [4/8/2014 SJC]
	CString m_strLanType;

protected:	

	int m_nGrabberTypeSN;           // 采集卡类型号 
	CString m_sInitFile;			// 用于初始化的文件
	CString m_sDeviceName;			// 设备名
	CString m_sDeviceMark;          // 设备标识  
	PGBCALLBACK m_CallBackFunc;		//回调函数指针
	PVOID m_Context;				// 存放调用初始化函数的对象的this指针
	s_GBERRORINFO m_LastErrorInfo;	// 错误信息

//	int m_nInitNum;
	UINT32 m_lFrameLengh;
	BOOL m_bFirstFrame;
};

#endif // !defined(AFX_AVTCAMDH_H__DED529A9_1D8C_4586_AB4D_5583AC9C174D__INCLUDED_)
