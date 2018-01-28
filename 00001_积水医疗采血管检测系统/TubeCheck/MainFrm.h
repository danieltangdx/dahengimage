// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__4B8DECDB_A7FA_4037_B1C9_9B8E1F287E47__INCLUDED_)
#define AFX_MAINFRM_H__4B8DECDB_A7FA_4037_B1C9_9B8E1F287E47__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LogManager.h"
#include "SpendTime.h"
#include "..\\..\\IncPublic\\ObjAlloc.h"
#include "ParamDefine.h"
#include "PIO24BLib.h"
#include "..\\..\\IncPublic\\alg_struct_Label.h"
#include "..\\..\\IncPublic\\ExtenInfo.h"
#include "..\\..\\IncPublic\\ExFn.h"
#include "..\\..\\IncPublic\\ThreeTierRights.h"

#include <array>
#include <queue>

#define SPOT


#ifdef SPOT
 #define __RealGrabber_							//相机实时采集
 #define   USE_IO_CARD							//使用接口卡时打开
#else

#define __SGGrabber_                                //模拟采集
#endif

//#define	  c_KickCount				2				//踢废口个数
#define   PIC_COUNT						30			//每个瓶盖拍照个数
#define	  PE_COUNT						3			//输入信号个数
#define	  IOCARD_COUNT					2			//IO卡个数
#define   CAMERA_COUNT						 6			//系统使用的相机个数    
#define   CHECK_THREAD_COUNT            8			//检测线程的个数
//#define   m_iSysDataEleLen			1			//回调中队列个数
#define   MAX_DEFECT_TYPE_COUNT		30			//缺陷类型总个数,每个工位40个
#define   ID_START_CHECK_TIMER          102         //不断更新数据库信息
#define   ID_UPDATE_DEFECT_INFO_TIMER	103				//不断更新缺陷统计信息
#define	  ID_UPDATE_DEFECT_SHOW_INFO	104				//不断更新缺陷统计显示信息
#define   MAX_INIT_IMAGE_NUM			100000000		//初始化图像号


/************************************************************************/
/*                       [5/16/2017 ConanSteve]                         */
const int c_AllElementCount{ 6 };			//相机组组的个数
const int c_KickCount{ 3 };
const bool g_bCSRecord{ true };
enum e_KickPort {
	e_KickPort_1=0,
	e_KickPort_2,
	e_KickPort_3
};

enum e_IOCard {
	e_IOCard_1,
	e_IOCard_2
};

enum e_KickMode {// 剔废模式 0为全踢，1为全不踢，2为隔张踢，3为正常踢
	e_KickMode_KickAll,
	e_KickMode_AllNotKick,
	e_KickMode_Alternative,
	e_KickMode_Noraml
};

#define TIMER_SG_READ_IMG 0x00FFFFFF
/************************************************************************/

#ifdef __RealGrabber_
// #include "..\\Inc\\DHGrabberJAI.h"
#include "..\\Inc\\DHGrabberForMER.h"
// enum GBParamID
// {
// 		GBImageWidth = 0,		//图像宽度 (单位:字节)
// 		GBImageHeight,			//图像高度 (单位:字节)
// 		GBImagePixelSize,		//图像象素大小 (单位:字节)
// 		GBImageBufferSize,		//图像缓冲区大小 (单位:字节)
// 		GBImageBufferAddr,		//图像缓冲区地址
// 		GBGrabberTypeSN			//采集卡类型编号
// };
#else
#include "..\\Inc\\GrabberExportForSG.h"
#endif

static int iTest[6]{ 0 };
class CMainFrame : public CFrameWnd
{

protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	// Attributes
public:
	bool m_bCheckPushed{ false };

	array<int, c_AllElementCount>  m_iStopCountDiff;						// 第i个工位停止计数判断条件 
	array<int, c_AllElementCount>  m_iInspectionObj;						// 第i个工位的检测对象个数
	array<int , c_AllElementCount> m_iHardImageCount;					// 第i个工位的图像大张号
	array<int ,c_AllElementCount> m_iLastHardImageCount;				// 第i个工位上次图像大张号
	array<int, c_AllElementCount> m_iLastImageCount;					// 第i个工位的上次图像寄存器计数
	array<int, c_AllElementCount>	m_iImageCountOverFlowNumber;			// 第i路相机计数溢出次数 
	array<int, c_AllElementCount> m_iSaveCameraImageCount;				// 连续保存图像的大张数 或周期数
	array<int,c_AllElementCount> m_iSaveCameraImageCount_Setting;		// 设定的需要保存的图像张数
	array<int, c_AllElementCount> m_iSaveCircleCameraImageCount;		// 循环保存图像的张数
	int m_iSaveImageType{ 0 };					// 保存图像类型
	int	m_iIsCircle{0};							// 是否循环保存图像
	array<int ,c_AllElementCount> m_iKickInterval;						// 配置文件中参数：算法超过多长时间导致剔废
	array<int, c_AllElementCount> m_iGrabCountSingleCircle;				// 一个周期采集的图像数---按最大的分配
	array<int, c_AllElementCount> m_iImageKickThread;					// 在剔废线程中使用的图像号
	array<int, c_AllElementCount> m_iImageCheckThread;
	array<int, c_AllElementCount> m_iImageCountInPE;					// 获取图像号时使用的图像数值
	array<int, c_AllElementCount> m_iLastImageCountInPE;				// 获取图像号时使用的上次图像数值
	array<int, c_AllElementCount> m_iFlowOverImageCountInPE;			// 获取补剔图像号时使用图像号溢出次数
	array<int, c_AllElementCount> m_iHardKickCount;						// 软帧踢废计数(工位i)
	array<int, c_AllElementCount> m_iLastKickCount;						// 剔废i的上一次计数	
	array<int, c_AllElementCount> m_iKickFlowNumber;					// 剔废i的溢出次数
	array<int, c_AllElementCount> m_iLastImageID;						// 第i个剔废线程中上一次的图像号码
	array<array<int, 256>,c_AllElementCount> m_iCheckResultTable;				// 工位i的检测结果记录表,用于剔废线程
	array<int, c_AllElementCount> m_iGrabCircleCount;					// 采集的大张计数
	array<int, c_AllElementCount> m_iTubeCnt{ 0 };						/** 采集有管图的计数              [5/26/2017 ConanSteve]**/

	array<int, c_AllElementCount> m_iCheckErrorCount{0};					// 检测错误数
	array<int, c_AllElementCount> m_iCheckImageCount;					// 检测总个数
	array<int, c_AllElementCount> m_iDefectTypePerWS;					// 每个工位的缺陷类型个数
	array<array<int, MAX_DEFECT_TYPE_COUNT>, c_AllElementCount> m_iDefectCount{0};// 每种缺陷类型统计数值
	array<int, c_AllElementCount> m_iLabelErrorCount;					// 1为标签位置错误个数，2为标签内容连续错误报警个数
	array<int, c_AllElementCount> m_iAbnormalImage;						// 保存异常图像张数
	array<int, CAMERA_COUNT> m_iOriginSaveImageNum;                    // 记录保存图像前M盘下各工位各自图像数量，用于控制新图像替换旧图像使用
	array<int, CAMERA_COUNT> m_iPicCount;								// 每个相机所有采集图像
	array<int, CAMERA_COUNT> m_iCamCardNum;							// 每个相机使用的接口卡编号
	array<int, CAMERA_COUNT> m_iCamOutPort;							// 每个相机使用的OUT口编号

	array<int, c_AllElementCount> m_iFrameCount{0};								// 相机回调中读取的光电计数  [2012-2-10 by zhy] 
	array<int, c_AllElementCount> m_iLastFrameCount{0};							// 相机回调中读取的光电上一次的帧计	
	array<int, c_AllElementCount> m_iFrameCountOverFlowNumber{0};				// 相机回调中读取的光电帧计数溢出的次数

	array<int,c_AllElementCount> m_iRC_PE;										// 记录光电
	array<int, c_AllElementCount> m_iRC_ThreadBad;						// 检测线程中给出坏信号时记录
	array<int, c_AllElementCount> m_iRC_ThreadGood;						// 检测线程中给出好信号时记录
	array<int, c_AllElementCount> m_iRC_Good;							// 写好信号记录
	array<int, c_AllElementCount> m_iRC_AddKickImage;					// 记录补剔图像号
	array<int, c_AllElementCount> m_iRC_KickImage;						// 记录剔废图像号
	array<int, c_AllElementCount> m_iRC_ImageNum;						// 记录重复记录
	array<int, c_AllElementCount> m_iRC_Image;							// 记录进入检测线程的图像号
	array<int, c_AllElementCount> m_iRC_BeforeD;						// 记录算法调用之前的图像号
	array<int, c_AllElementCount> m_iRC_Detect;							// 记录算法报错的图像号及检测时间
	array<int, c_AllElementCount> m_iRC_Kick;							// 记录剔废图像号
	array<int, c_AllElementCount> m_iRC_Call;							// 记录回调中图像号及回调间隔
	array<int, c_AllElementCount> m_iRC_DetectTime;						// 记录算法检测时间
	array<int, c_AllElementCount> m_iRC_RecordImage;					// 记录线程中图像号变化时的时间
	array<int, c_AllElementCount> m_iRC_SetEvent;						// 进入检测线程
	array<int, c_AllElementCount> m_iRC_WriteImage;						// 获取图像数据[2014-12-5 zhy]
	array<int, c_AllElementCount> m_iRC_StopMachine;					// 停机次数
	array<int, c_AllElementCount> m_iRC_RecordKick;						// 记录获取图像号中的剔废信息
	array<int, CAMERA_COUNT> m_iCamSN;									// 相机图像序号
	array<int, CAMERA_COUNT> m_iPicCountPerCam;						// 每个相机拍的序列图个数

	int m_iSaveImageSys;										//是否使用系统程序保存图像 0为否，1为是
	int m_iShowDevice;											//显示图像的设备 0为系统图像 1为算法图像
	int m_iKickOffset5;												//相机5相对相机6的棍子个数
	int m_iEmptyCount;											// 空压头个数
	int m_iLicenseExpDate;										// License到期提醒时间
	int m_iCamTestMode;											// 相机调试模式 0为正常采集 1为调试模式
	int m_iKickMode;											// 剔废模式 0为全踢，1为全不踢，2为隔张踢，3为正常踢
	int m_iSaveImage_Abnormal;									// 保存异常图像张数
	int m_iRecord_Set;											// 设定的保存记录的条数
	int m_iErrorSignal;											// 误触发间隔

	int m_iAlarmCardNum;										// 报警所使用的接口卡编号
	int m_iAlarmOutPort;										// 报警所使用的接口卡输出端口号
	int m_iCheckCountTotal;										// 检测总数
	int m_iCheckErrorCountTotal;								// 检测错误瓶盖个数统计(最后一次使能接口卡后的计数) 
	int m_iCheckGoodCountTotal;
	int m_iMaxCameraImageWidth;									// 采集图像宽度
	int m_iMaxCameraImageHeight;								// 采集图像高度
	int m_iMaxCameraImageSize;									// 采集图像大小
	int m_iMaxCameraImagePixelSize;								// 采集图像像素大小
	int m_iImageShowMode;										// 显示图像模式 0:不显示; 1:显示所有; 2:仅显示错误
	int m_iIntervalDisplayTime;									// 图像显示间隔时间
	int m_iMaxSaveImageCount;									// 每个工位最大存图张数
	int	m_iPreAllocateSaveInfoElementCount;						// 预分配保存图像信息元素个数：所有信息元素×一个周期采集的个数
	int	m_iMaxGrabCountSingleCircle;							// 全部工位的一周期内的采集图像的最大值
	int	m_iLastCheckCount;										// 记录模拟采集时上一次的图像计数，用于计算实时检测速度
	int m_iCheckSpeedInterval;									// 计算检测速度的时间间隔
	int m_iLastCheckSpeedCount;									// 检测速度计数个数

	BOOL m_bHaveStartSaveThread;						// 是否开启了存图线程
	BOOL m_bFirstCheck;											// 刚启动或者复位
	BOOL m_bAdvMode;											// 是否处于高级操作员模式
	BOOL m_bReadModel;											// 判断读写模板是否成功
	bool m_bStartRecord;										// 控制开始记录
	BOOL m_bIsDetecting;										// 判断是否处于正在检测状态
	BOOL m_bIOCardValid;										// 接口卡是否有效
	BOOL m_bIOCardInitSuccess;									// 接口卡是否初始化成功
	BOOL m_bOpenAlgCheck;										// 是否开启算法检测[2015-3-12 zhy]
	BOOL m_bOpenImageSave;										// 是否保存图像
	BOOL m_bCalAlg;												// 是否调用算法检测
	BOOL m_bSoftResetIOCard;									// 是否软件复位接口卡
	BOOL m_bPauseSaveImageThread;								// 是否暂停保存图像线程
	BOOL m_bKillSaveImageThread;								// 是否停止保存图像线程
	BOOL m_bCloseCount;											// 用于控制是否获取IO卡中的图像计数
	BOOL m_bWaitDemoMould;
	BOOL m_bStopReadKickImageNo;								// 读图像号

	array<BOOL, c_AllElementCount> m_bIsKick;							// 各路是否踢废
	array<BOOL, c_AllElementCount> m_bSaveCameraImage;					// 是否保存图像
	array<BOOL, c_AllElementCount> m_bDisplayImage;						// 三个工位是否显示图像
	array<BOOL, c_AllElementCount> m_bDisplayAlgImage;					// 三个工位是否显示算法图像
	array<BOOL, c_AllElementCount> m_bIsCheck;							// 是否检测
	array<BOOL, CAMERA_COUNT> m_bIsSimulateIniExist;					// 模拟采图，用于判断模拟采集时配置文件是否存在
	array<BOOL, CAMERA_COUNT> m_bGrabFlip;								// 是否进行翻转
	array<array<BOOL, 256>,c_AllElementCount> m_bTubeCheckResult;			// 存放用于判断连续保存的检测结果 报错为真，无措为假
	array<array<BOOL,256>, c_AllElementCount> m_bCheckResultOffset;			// 工位1是否剔废偏移 
	array<BOOL, c_AllElementCount> m_bLabelAlarm;						// 标签检测报警
	array<BOOL, c_AllElementCount> m_bAlarm_StopCount;					// 停止计数报警
	array<array<array<BOOL, PIC_COUNT>,256>,c_AllElementCount> m_bWriteImage;			// 是否写入数据


	CString m_sModelName;										// 当前模板名称
	CString	m_sOldModelName;									// 旧的模板名字
	CString	m_sModelPath;										// 模板路径
	CString m_sWorkPath;										// 应用程序工作路径
	CString m_sSysConfigFileName;								// 系统配置文件
	CString	m_sVirtualDisk;
	CString m_sCipherPath1;										// 系统密码文件夹
	CString m_sStartTime;										// 记录开始检测时间
	CString m_sImageFilePath;									// 图像文件夹路径
	CString m_sLogFilePath;										// 日志文件夹路径
	CString m_sAlgIniFilePath;									// 算法配置参数文件夹路径
	CString	m_sCheckLogFilePath;								// 检测日志所在文件夹
	CString	m_sDataFilePath;									// 保存数据库文件路径
	CString m_sWindowTitle;										// 窗口标题
	CString m_sAlgConfigFilePath;										// 算法配置程序DEMO的配置文件路径
	CString m_sDemoName;										// Demo名称
	array<CString, CAMERA_COUNT>m_sSimulateGrabPath;					// 模拟采图路径
	array<array<CString, MAX_DEFECT_TYPE_COUNT>,c_AllElementCount> m_sDefectInfo;//每种缺陷的名称
	array<array<CString,256>,c_AllElementCount> m_strErrorDesc;				//算法返回的错误信息链表,用于列表框
	array<CString, c_AllElementCount> m_sSaveImagePath;					//保存图像路径
	array<CString, CAMERA_COUNT> m_sCamConfigPath;					//相机配置文件路径
	array<CString, CAMERA_COUNT> m_sCamConfigName;					//相机配置文件名称


	array<CFile, c_AllElementCount> m_filePE;									// 记录光电
	array<CFile, c_AllElementCount> m_fileThreadBad;					// 检测线程中给出坏信号时记录
	array<CFile, c_AllElementCount> m_fileThreadGood;					// 检测线程中给出好信号时记录
	array<CFile, c_AllElementCount> m_fileGood;							// 写好信号记录
	array<CFile, c_AllElementCount> m_fileAddKickImage;					// 记录补剔图像号
	array<CFile, c_AllElementCount> m_fileKickImage;					// 记录剔废图像号
	array<CFile, c_AllElementCount> m_fileImageNum;						// 记录重复记录
	array<CFile, c_AllElementCount> m_fileImage;						// 记录进入检测线程的图像号
	array<CFile, c_AllElementCount> m_fileBeforeD;						// 记录算法调用之前的图像号
	array<CFile, c_AllElementCount> m_fileDetect;						// 记录算法报错的图像号及检测时间
	array<CLogFile, c_AllElementCount> m_fileKick;							
	array<CFile, c_AllElementCount> m_fileCall;							// 记录回调中图像号及回调间隔
	array<CFile, c_AllElementCount> m_fileDetectTime;					// 记录算法检测时间
	array<CFile, c_AllElementCount> m_fileRecordImage;					// 记录线程中图像号变化时的时间
	array<CFile, c_AllElementCount> m_fileSetEvent;						// 进入检测线程
	array<CFile, c_AllElementCount> m_fileWriteImage;					// 获取图像数据[2014-12-5 zhy]
	array<CFile, c_AllElementCount> m_fileStopMachine;					// 停机报警日志
	array<CFile, c_AllElementCount> m_fileRecordKick;
	

	CEvent m_evtTrigDemoMould;									// 检测等待demo加载模板线程是否死亡[2014-11-4]
	CEvent m_evtGetIOCardImageNum;								// 获取计数线程是否死亡
	CEvent m_evtKillWait;

	double m_dFreq;												// 记录计算机时钟频率
	double m_dwCheckBadRatio;									// 坏品率
	array<double, c_AllElementCount> m_dAlgCheckTime;					// 算法检测时间

	//Bayer变化相关变量
	array<BYTE,256>  m_pLutR;
	array<BYTE, 256>   m_pLutG;
	array<BYTE, 256>   m_pLutB;
//	BYTE* m_pAlgElement[CAMERA_COUNT];							// 专门为算法的参数设置和修改模板而开设的内存
	array<BYTE*, CAMERA_COUNT> m_pbRGBImage;							// 采图贝尔变换图像缓冲区
	//array<BYTE*, c_AllElementCount> m_pImageAddr;						// 存放图像数据
	array< shared_ptr<BYTE>, c_AllElementCount> m_pImageAddr{nullptr};
	array<array<CSpendTime,256>, c_AllElementCount> m_SpendTime;				// 统计是否超时使用 
	LONGLONG m_prevqt1;											// 记录侧面工位上次回调中所读取的计算机时间信息
	array<LONGLONG, c_AllElementCount> m_lCameraTimeInfo;				// 记录三个工位上次回调中所读取的计算机时间信息
	array<LONGLONG, CAMERA_COUNT> m_lLastQt_PE;							// 记录光电上次计数
	array<LONGLONG, CAMERA_COUNT> m_lLastQt_TX;						// 记录图像号上次计数


	array<CCriticalSection, CAMERA_COUNT> m_csImagePath;				// 保护图像保存路径，防止操作互斥
	array<CCriticalSection, c_AllElementCount> m_csElement;				//保护信息


	_ConnectionPtr m_Connect;									// 连接数据库
	_RecordsetPtr m_RecordSet;									// 记录集

	CLogManager  m_LogManager;									// 日志操作类
	array<CPIO24BLib,IOCARD_COUNT> m_IOCard;			     			// PIO24B接口卡对象
	array<CBayerTransform, CAMERA_COUNT> m_BayerTransform;

	array<BITMAPINFO*, CAMERA_COUNT> m_pBitmapInfo;
	array<e_ShowModeFlip, CAMERA_COUNT> m_eShowModeFlip;				// 进行翻转类型
	//array<array<s_ElementInfo, m_iSysDataEleLen>, c_AllElementCount> m_sElement;			// 记录回调信息
	array<s_ElementInfo*, c_AllElementCount> m_sElement;
	array<s_ImageInformationStruct, CAMERA_COUNT> m_sCameraImageInfo; //相机采集的图像信息

	CObjAlloc m_InspectionObj;									// 检测对象
	array<LabelCheck::s_AlgInspResult, 256>   m_sOut5;					//相机5的输出参数

	array<s_ThreadParam, CHECK_THREAD_COUNT> m_sThreadParam;				//检测线程参数
	array<HANDLE, c_AllElementCount> m_hImageSaveStart;							// 启动保存图片事件句柄数组

	array<BOOL, 5> m_arr_iAlgStopMachineCtrl;//**[12/20/2017 ConanSteve]**:  算法停机控制


#ifdef __SGGrabber_
	array<CDHGrabberSG, CAMERA_COUNT>  m_CameraGrabber;				// 相机采集对象
#else
//	CDHGrabberAVT  m_CameraGrabber[CAMERA_COUNT];				// 相机采集对象
// 	CDHGrabberJAI m_CameraGrabber[CAMERA_COUNT];
	array<CDHGrabberMER, CAMERA_COUNT> m_CameraGrabber;

#endif
	

// Operations
public:

	// 执行报警动作,bStart为TRUE开始报警，bStart为FALSE停止报警
	BOOL AlarmAction(BOOL bStart);

	// 获取当前时间
	CString GetCurTime(DWORD* dwTime=NULL);

	// 开始检测
	BOOL StartCheck();

	// 停止检测
	BOOL StopCheck();

	//退出检测程序
	inline void QuitCheckProgram();								

	// 初始化数据库（包含删除空记录，插入新纪录，开始数据库更新）
	void InitDataBase();

	// 判断数据库中是否有检测总数为0的记录，若有，从数据库中删除检测总数为0的记录
	void InitTableData(CString strTableName);		
	
	// 更新最后一条记录
	void UpdateTableData(s_CheckResultInfo info, CString strTableName);	

	// 插入一条新记录
	void InsertTableData(s_CheckResultInfo info, CString strTableName);	

	// 更新缺陷信息记录
	void UpdateDefectTableData(s_CheckDefectInfo info, CString strTableName);	

	// 插入一条缺陷信息新记录
	void InsertDefectTableData(s_CheckDefectInfo info, CString strTableName);	

	// 连接access数据库
	BOOL ADOConnectDB();

	// 生成表
	BOOL CreateTable(CString strTable);		
	
	// 判断数据表是否存在数据库中
	BOOL ExistTable(CString strTable);									

	// 重置接口卡后进行的相关动作
	void SoftResetIOCard(CString strModelName);

	// 加载虚拟硬盘M盘下的所有图片名称
	inline BOOL LoadAllImage();                                 

	// 保存图像
	int savebmp(const char *lpszPathName,int W,int H, int nByte, unsigned char *lpDest);

	// 保存异常图像
	int SaveAbnormityImageFunc(const char* pPath,int iWidth, int iHeight, int iByte, unsigned char* pAddr);

	// 初始化检测动态库
	void InitInspectionDll();

	// 获取本执行程序所在的路径
	CString GetCurrentAppPath();

	//建立文件夹路径
	BOOL FindFilePathFunc();
	
	// 读取系统配置文件
	BOOL ReadSystemConfig(const CString &strLoadFile);	
	
	// 保存系统配置文件
	BOOL SaveSystemConfig(const CString &strLoadFile);		

	// 文件操作
	BOOL OP_Directory(LPCSTR lpsrcPath, LPCSTR lpdstPath, int type,int flag);
	
	// 创建多层目录
	BOOL inCreateMultiLevelPath(const CString &strPath);
	
	// 判断目录是否存在
	BOOL inIsPathExist(const CString& strPath);

	//设置LUT表
	BOOL SetLutTable(double dRatioR,double dRatioG,double dRatioB); 

	// 初始化接口卡
	inline BOOL InitIOCard();	
	
	// 释放接口卡
	inline BOOL ReleaseIOCard();								
	
	// 初始化内存数据区
	inline BOOL InitDataFunc();		
	
	// 释放内存数据区
	inline BOOL ReleaseDataFunc();								
	
	// 初始化采集对象
	inline BOOL InitGrabber();	
	
	// 释放采集对象
	inline BOOL ReleaseGrabber();	

	// 启动所有线程
	inline BOOL StartAllThread();	
	
	// 杀死所有线程
	inline BOOL KillAllThread();	

	// 开始相机采集
	inline BOOL StartCameraGrab(const int iCameraSN);
	
	// 停止相机采集
	inline BOOL StopCameraGrab(const int iCameraSN);			
	
	// 读取机器信号信息
	inline BOOL ReadMachineSignal(int iGrabSn);	

	//显示图像
	BOOL  ThreadDisplayImage(int iCamGroup, int iIndex);		
	
	//新添 单个相机单入队的函数 [4/30/2010 GZ]
	BOOL  CameraGrabFuncSingle(int iCameraSN,int iAllElementSN ); //单个相机的实际采集函数
	BOOL  CameraGrabFuncSingleEx(int iCameraSN, int iAllElementSN);
	//判断一个采集周期
	inline BOOL  JudgeGrabCircleNew(int  iCameraSN,							//相机序号  [11/16/2009 GZ]
									int  iAllElementSN,						//工位号
									int& iGrabCircle,						//记录采集图像号
									int& iPicSN);							//相机图像序列号

	//对图像数据进行镜像,只有水平和垂直镜像
	int MirrorSingleImage(BYTE* pImageBuff, int lImageWidth, int lImageHeight, int lImageBitCount, e_ShowModeFlip  eShowMode);

	//是否对图像数据进行旋转
	int RotateImage(BYTE* pSrcImageBuff, int iSrcWidth, int iSrcHeight, int iImageBitCount, float fRotateAngle, 
		BYTE* pDestImageBuff, int& iDestWidth, int& iDestHeight);

	// 1工位采集回调	
	static void WINAPI GlobalAreaGrabOverCallback1 (const s_GBSIGNALINFO* SigInfo);
	void AreaGrabOverCallback1(const s_GBSIGNALINFO* SigInfo);	// 1工位第一个相机的回调函数

	// 剔废回调	
	static void WINAPI GlobalKickCallbackFunc(const s_KickParam* sKickParam);
	void KickCallbackFunc1(const s_KickParam* sKickParam);
	
	// 状态回调
	static void WINAPI GlobalStatusCallbackFunc(const s_Status* sStatusParam);
	void StatusCallbackFunc1(const s_Status* sStatusParam);

	// 该线程用于等待离线demo加载模板
	static DWORD WaitDemoMould(LPVOID lpParam);
	DWORD WaitDemoMouldFunc();

	// 获取计数线程
	static DWORD GetIOCardImageNumThread(LPVOID lpParam);
	DWORD GetIOCardImageNumThreadFunc();


	// 获取某个盘符剩余空间 单位为M
	double GetFreeSpaceOfHardDisk(CString strDisk);

	// 获取光电计数
	BOOL GetCount_Guangdian();
	
	// 获取剔废计数
	BOOL GetCount_Tifei();
	
	// 获取图像号寄存器计数
	BOOL GetCount_TuXiangHao();
	
	// 加载相机参数文件
	BOOL LoadCamConfig(CString strModelName);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	void QuitPrecedure();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
//	CToolBar    m_wndToolBar;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_iSaveSeqCnt{ 0 };
	int m_iSaveErrCnt{ 0 };
	int m_iSaveExistCnt{ 0 };
	CCriticalSection m_myCriticalSec;
	CStdioFile m_myFileTickResult;
	CStdioFile m_myFileCheckResult;
	int m_iLastFrameCnt{ 0 };
	int m_iDataElementCnt{ 0 };
	array<int, c_AllElementCount> m_iCheckTubeCnt{0};
	array<int, c_AllElementCount> m_iAlgExceptionCnt{ 0 };
	CString m_csLoadString1;
	afx_msg void OnDestroy();
	bool DeleteDirectory(char* sDirName);
	/************************************************************************/
	/*                [5/8/2017 ConanSteve]                     */
	int m_iLabelQueLen{ 0 };
	int m_iDisBtwn5And6{ 0 };
	int m_iSaveImgObj{ 0 };/** the objection of executing save images function       [6/15/2017 ConanSteve]**/
	int		m_iSaveImgNamingRegulation{ 0 };/** 存图命名规则              [6/15/2017 ConanSteve]**/
	CCriticalSection m_csLabelRsltTable;
	array<int, c_AllElementCount> m_iExposureTime{ 0 };/** Exposure time of cameras              [6/15/2017 ConanSteve]**/
	typedef struct s_5CheckInfo {
		bool bKick{false};
		int iCheckRslt{0};
		int iImgSN5{0};
	}s_5CheckInfo;
	queue<int> m_iWaitRsltFrame;//为了防止1次输入，相机采集了2次图像
	queue<s_5CheckInfo> m_sLabelRst;//**[9/27/2017 ConanSteve]**:  标签的检测结果
	array<s_5CheckInfo, 256> m_arrKickTable2;
	int m_iErrorCntOf5And6{0};//56工位都错得计数
	array<HANDLE, c_AllElementCount> m_hEventSGActive{ 0 };
	array<HANDLE, c_AllElementCount> m_hEventComplete{ 0 };
	int m_iSG_ElapseTimeToRead{ 0 };
	int m_iLastTime{ 0 };
	array<DWORD, c_AllElementCount> m_dwLastKickTime{ 0 };

	//boost::shared_array<boost::shared_array<char>> m_pImgDataTemp{ nullptr };
	array<shared_ptr<char>, CAMERA_COUNT> m_pImgDataTemp{ nullptr };
	CString m_sLanguage{TEXT("")};
	array<CExtenInfo, c_AllElementCount> m_arr_ExtenPara;
	int m_iSysDetectMode{ 0 };//**[7/21/2017 ConanSteve]**:  系统检测模式
	BOOL m_bOpenCameraAlgCheck[6]{TRUE};
	bool m_bShowObjStatusWnd{ false };//**[7/26/2017 ConanSteve]**:  是否显示多对象实时检测信息
	int m_iSysDataEleLen{ 0 };
	int m_iObjDetectMode{ 0 };
	deque<int> m_queFrameCnt;
	deque<int> m_queTubeCnt;//*[7/12/2017 ConanSteve]*:  采血管有管计数队列
	std::wstring m_wsResPath;
	bool m_bOpenXCWnd;//**[9/27/2017 ConanSteve]**:  是否打开XC组件
	int m_iPortOfAlarm{ 0 };//**[9/27/2017 ConanSteve]**:  接口卡接收报警信号的端口
	int  m_iPortOfStopMachine{ 0 };//**[9/27/2017 ConanSteve]**:  接口卡接口停机信号的端口
	int m_iPortOfNormalRunning{ 0 };
	int m_iMaxNumOfConstantKick{ 10 };
	int m_iOffsetBtw1And4{19};//1工位和4工位之间的帧数差
	array<int, 6> m_arr_iCurNumOfConstantKick;//**[9/27/2017 ConanSteve]**:  当前每个剔废口的连踢个数
	CINICfg m_iniAlgConfig;
	int m_iLastCheckCntTotle{ 0 };//**[1/19/2018 ConanSteve]**:  上次检测结束时的检测总数计数。
	int m_iLastGoodCntTotle{ 0 };//**[1/19/2018 ConanSteve]**:  上次正常结束或者断电计数时的好品计数

	array<int, 256> m_arr_ImgSNFormFive2Six;//五工位触发时到6工位图像号的映射

	//**[1/16/2018 ConanSteve]**:  三级权限
	CThreeTierRights m_threeTierRights;
	/************************************************************************/
	bool AlarmStopMachine(LPCTSTR sErrorInfo = NULL);
	int StopMachine();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__4B8DECDB_A7FA_4037_B1C9_9B8E1F287E47__INCLUDED_)
