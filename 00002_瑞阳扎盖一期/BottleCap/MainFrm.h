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
#include "ObjAlloc.h"
#include "ParamDefine.h"
#include "PIO24BLib.h"
#include "Raw2Rgb.h"
#include "DHAlgInterface.h"
#include "MultiObjectDef.h"
#include "alg_struct.h"
#include "BottleCap.h"
#include "IoCard.h"

enum e_IOCardType {
	e_IOCardType_NONE,
	e_IOCardType_PIOB,
	e_IOCardType_PIOC,//**[9/11/2017 ConanSteve]**:  C卡
	e_IOCardType_NewCard,
	e_IOCardType_RS232_XZXNYN
};

#define  SPOT1

#ifdef SPOT
#define __RealGrabber_							//相机实时采集
#define   USE_IO_CARD							//使用接口卡时打开
#else 
#define __SGGrabber_                                //模拟采集
#endif // SPOT





#define   PIC_COUNT						30			//每个瓶盖拍照个数
#define	  PE_COUNT						3			//输入信号个数
#define	  IOCARD_COUNT					1			//IO卡个数
#define	  ALLELEMENT_COUNT				3			//相机组组的个数
#define   CAMERA_COUNT                 3			//系统使用的相机个数    
#define   CHECK_THREAD_COUNT            8			//检测线程的个数
#define   DATA_ELEMENT_COUNT			5			//回调中队列个数
#define   MAX_DEFECT_TYPE_COUNT		30			//缺陷类型总个数,每个工位40个
#define   ID_START_CHECK_TIMER          102         //不断更新数据库信息
#define   ID_UPDATE_DEFECT_INFO_TIMER	103				//不断更新缺陷统计信息
#define	  ID_UPDATE_DEFECT_SHOW_INFO	104				//不断更新缺陷统计显示信息

#ifdef __RealGrabber_
// #include "..\\Inc\\DHGrabberJAI.h"
// #include "..\\Inc\\DHGrabberForAVT.h"
#include "..\\Inc\\DHGrabberForMER.h"
/*enum GBParamID
{
 		GBImageWidth = 0,		//图像宽度 (单位:字节)
		GBImageHeight,			//图像高度 (单位:字节)
 		GBImagePixelSize,		//图像象素大小 (单位:字节)
 		GBImageBufferSize,		//图像缓冲区大小 (单位:字节)
		GBGrabberTypeSN			//采集卡类型编号
};*/
#else
#include "..\\Inc\\GrabberExportForSG.h"
#endif

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

	int m_iStopCountDiff[ALLELEMENT_COUNT];						// 第i个工位停止计数判断条件 
	int m_iInspectionObj[ALLELEMENT_COUNT];						// 第i个工位的检测对象个数
	int m_iHardImageCount[ALLELEMENT_COUNT];					// 第i个工位的图像大张号
	int m_iLastHardImageCount[ALLELEMENT_COUNT];				// 第i个工位上次图像大张号
	int m_iLastImageCount[ALLELEMENT_COUNT];					// 第i个工位的上次图像寄存器计数
	int	m_iImageCountOverFlowNumber[ALLELEMENT_COUNT];			// 第i路相机计数溢出次数 
	int m_iSaveCameraImageCount[ALLELEMENT_COUNT];				// 连续保存图像的大张数 或周期数
	int m_iSaveCameraImageCount_Setting[ALLELEMENT_COUNT];		// 设定的需要保存的图像张数
	int m_iSaveCircleCameraImageCount[ALLELEMENT_COUNT];		// 循环保存图像的张数
	int m_iSaveImageType[ALLELEMENT_COUNT];						// 保存图像类型
	int	m_iIsCircle[ALLELEMENT_COUNT];							// 是否循环保存图像
	int m_iKickInterval[ALLELEMENT_COUNT];						// 配置文件中参数：算法超过多长时间导致剔废
	int m_iGrabCountSingleCircle[ALLELEMENT_COUNT];				// 一个周期采集的图像数---按最大的分配
	int m_iImageKickThread[ALLELEMENT_COUNT];					// 在剔废线程中使用的图像号
	int m_iImageCheckThread[ALLELEMENT_COUNT];
	int m_iImageCountInPE[ALLELEMENT_COUNT];					// 获取图像号时使用的图像数值
	int m_iLastImageCountInPE[ALLELEMENT_COUNT];				// 获取图像号时使用的上次图像数值
	int m_iFlowOverImageCountInPE[ALLELEMENT_COUNT];			// 获取补剔图像号时使用图像号溢出次数
	int m_iHardKickCount[ALLELEMENT_COUNT];						// 软帧踢废计数(工位i)
	int m_iLastKickCount[ALLELEMENT_COUNT];						// 剔废i的上一次计数	
	int m_iKickFlowNumber[ALLELEMENT_COUNT];					// 剔废i的溢出次数
	int m_iLastImageID[ALLELEMENT_COUNT];						// 第i个剔废线程中上一次的图像号码
	int m_iCheckResultTable[ALLELEMENT_COUNT][256];				// 工位i的检测结果记录表,用于剔废线程
	int m_iGrabCircleCount[ALLELEMENT_COUNT];					// 采集的大张计数
	int m_iCheckErrorCount[ALLELEMENT_COUNT];					// 检测错误数
	int m_iCheckImageCount[ALLELEMENT_COUNT];					// 检测总个数
	int m_iDefectTypePerWS[ALLELEMENT_COUNT];					// 每个工位的缺陷类型个数
	int m_iDefectCount[ALLELEMENT_COUNT][MAX_DEFECT_TYPE_COUNT];// 每种缺陷类型统计数值
	int m_iLabelErrorCount[ALLELEMENT_COUNT];					// 1为标签位置错误个数，2为标签内容连续错误报警个数
	int m_iAbnormalImage[ALLELEMENT_COUNT];						// 保存异常图像张数
	int m_iListImagePathCount[CAMERA_COUNT];					// 队列m_ListImagePath数组中元素个数
	int m_iOriginSaveImageNum[CAMERA_COUNT];                    // 记录保存图像前M盘下各工位各自图像数量，用于控制新图像替换旧图像使用
	int m_iPicCount[CAMERA_COUNT];								// 每个相机所有采集图像
	int m_iCamCardNum[CAMERA_COUNT];							// 每个相机使用的接口卡编号
	int m_iCamOutPort[CAMERA_COUNT];							// 每个相机使用的OUT口编号

	int m_iFrameCount[PE_COUNT];								// 相机回调中读取的光电计数  [2012-2-10 by zhy] 
	int m_iLastFrameCount[PE_COUNT];							// 相机回调中读取的光电上一次的帧计	
	int m_iFrameCountOverFlowNumber[PE_COUNT];					// 相机回调中读取的光电帧计数溢出的次数

	int m_iRC_PE[PE_COUNT];										// 记录光电
	int m_iRC_ThreadBad[ALLELEMENT_COUNT];						// 检测线程中给出坏信号时记录
	int m_iRC_ThreadGood[ALLELEMENT_COUNT];						// 检测线程中给出好信号时记录
	int m_iRC_Good[ALLELEMENT_COUNT];							// 写好信号记录
	int m_iRC_AddKickImage[ALLELEMENT_COUNT];					// 记录补剔图像号
	int m_iRC_KickImage[ALLELEMENT_COUNT];						// 记录剔废图像号
	int m_iRC_ImageNum[ALLELEMENT_COUNT];						// 记录重复记录
	int m_iRC_Image[ALLELEMENT_COUNT];							// 记录进入检测线程的图像号
	int m_iRC_BeforeD[ALLELEMENT_COUNT];						// 记录算法调用之前的图像号
	int m_iRC_Detect[ALLELEMENT_COUNT];							// 记录算法报错的图像号及检测时间
	int m_iRC_Kick[ALLELEMENT_COUNT];							// 记录剔废图像号
	int m_iRC_Call[ALLELEMENT_COUNT];							// 记录回调中图像号及回调间隔
	int m_iRC_DetectTime[ALLELEMENT_COUNT];						// 记录算法检测时间
	int m_iRC_RecordImage[ALLELEMENT_COUNT];					// 记录线程中图像号变化时的时间
	int m_iRC_SetEvent[ALLELEMENT_COUNT];						// 进入检测线程
	int m_iRC_WriteImage[ALLELEMENT_COUNT];						// 获取图像数据[2014-12-5 zhy]
	int m_iRC_StopMachine[ALLELEMENT_COUNT];					// 停机次数
	int m_iRC_RecordKick[ALLELEMENT_COUNT];						// 记录获取图像号中的剔废信息
	int m_iCamSN[CAMERA_COUNT];									// 相机图像序号
	int m_iPicCountPerCam[CAMERA_COUNT];						// 每个相机拍的序列图个数

	int m_iUserType;												//0为不合法用户 1位管理员 2为调试员 3为普通用户
	int m_iEmptyCount;											// 空压头个数
	int m_iLicenseExpDate;										// License到期提醒时间
	int m_iCamTestMode;											// 相机调试模式 0为正常采集 1为调试模式
	int m_iKickMode;											// 剔废模式 0为全踢，1为全不踢，2为隔张踢，3为正常踢
	int m_iKickOffset;											// 剔废偏移量
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

	BOOL m_bFirstCheck;											// 刚启动或者复位
	BOOL m_bAdvMode;											// 是否处于高级操作员模式
	BOOL m_bReadModel;											// 判断读写模板是否成功
	BOOL m_bStartRecord;										// 控制开始记录
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
	BOOL m_bUpdateCheckData;											//是否正在更新生产数据

	BOOL m_bIsKick[ALLELEMENT_COUNT];							// 各路是否踢废
	BOOL m_bSaveCameraImage[ALLELEMENT_COUNT];					// 是否保存图像
	BOOL m_bDisplayImage[ALLELEMENT_COUNT];						// 三个工位是否显示图像
	BOOL m_bDisplayAlgImage[ALLELEMENT_COUNT];					// 三个工位是否显示算法图像
	BOOL m_bIsCheck[ALLELEMENT_COUNT];							// 是否检测
	BOOL m_bIsSimulateIniExist[CAMERA_COUNT];					// 模拟采图，用于判断模拟采集时配置文件是否存在
	BOOL m_bGrabFlip[CAMERA_COUNT];								// 是否进行翻转
	BOOL m_bBottleCapResult[ALLELEMENT_COUNT][256];			// 存放用于判断连续保存的检测结果 报错为真，无措为假
	BOOL m_bCheckResultOffset[ALLELEMENT_COUNT][256];			// 工位1是否剔废偏移 
	BOOL m_bLabelAlarm[ALLELEMENT_COUNT];						// 标签检测报警
	BOOL m_bAlarm_StopCount[ALLELEMENT_COUNT];					// 停止计数报警
	BOOL m_bWriteImage[CAMERA_COUNT][256][PIC_COUNT];			// 是否写入数据

	CString m_sPassword;												//用户密码
	CString m_sUserName;												//用户名称
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
	CString m_sDemoIniPath;										// 算法配置程序DEMO的配置文件路径
	CString m_sDemoName;										// Demo名称
	CString	m_sSimulateGrabPath[CAMERA_COUNT];					// 模拟采图路径
	CString m_sDefectInfo[ALLELEMENT_COUNT][MAX_DEFECT_TYPE_COUNT];//每种缺陷的名称
	CString m_strErrorDesc[ALLELEMENT_COUNT][256];				//算法返回的错误信息链表,用于列表框
	CString m_sSaveImagePath[ALLELEMENT_COUNT];					//保存图像路径
	CString m_sCamConfigPath[CAMERA_COUNT];					//相机配置文件路径
	CString m_sCamConfigName[CAMERA_COUNT];					//相机配置文件名称


	CFile m_filePE[PE_COUNT];									// 记录光电
	CFile m_fileThreadBad[ALLELEMENT_COUNT];					// 检测线程中给出坏信号时记录
	CFile m_fileThreadGood[ALLELEMENT_COUNT];					// 检测线程中给出好信号时记录
	CFile m_fileGood[ALLELEMENT_COUNT];							// 写好信号记录
	CFile m_fileAddKickImage[ALLELEMENT_COUNT];					// 记录补剔图像号
	CFile m_fileKickImage[ALLELEMENT_COUNT];					// 记录剔废图像号
	CFile m_fileImageNum[ALLELEMENT_COUNT];						// 记录重复记录
	CFile m_fileImage[ALLELEMENT_COUNT];						// 记录进入检测线程的图像号
	CFile m_fileBeforeD[ALLELEMENT_COUNT];						// 记录算法调用之前的图像号
	CFile m_fileDetect[ALLELEMENT_COUNT];						// 记录算法报错的图像号及检测时间
	CFile m_fileKick[ALLELEMENT_COUNT];							// 记录剔废图像号
	CFile m_fileCall[ALLELEMENT_COUNT];							// 记录回调中图像号及回调间隔
	CFile m_fileDetectTime[ALLELEMENT_COUNT];					// 记录算法检测时间
	CFile m_fileRecordImage[ALLELEMENT_COUNT];					// 记录线程中图像号变化时的时间
	CFile m_fileSetEvent[ALLELEMENT_COUNT];						// 进入检测线程
	CFile m_fileWriteImage[ALLELEMENT_COUNT];					// 获取图像数据[2014-12-5 zhy]
	CFile m_fileStopMachine[ALLELEMENT_COUNT];					// 停机报警日志
	CFile m_fileRecordKick[ALLELEMENT_COUNT];
	

	CEvent m_evtTrigDemoMould;									// 检测等待demo加载模板线程是否死亡[2014-11-4]
	CEvent m_evtGetIOCardImageNum;								// 获取计数线程是否死亡
	CEvent m_evtKillWait;

	double m_dFreq;												// 记录计算机时钟频率
	double m_dwCheckBadRatio;									// 坏品率
	double m_dAlgCheckTime[ALLELEMENT_COUNT];					// 算法检测时间

	//Bayer变化相关变量
	BYTE m_pLutR[256];
	BYTE m_pLutG[256];
	BYTE m_pLutB[256];
	BYTE* m_pAlgElement[CAMERA_COUNT];							// 专门为算法的参数设置和修改模板而开设的内存
	BYTE* m_pAlgErrorElement[CAMERA_COUNT];						// 专门为算法最近一张错误图像开设的内存
	BYTE* m_pbRGBImage[CAMERA_COUNT];							// 采图贝尔变换图像缓冲区
	BYTE* m_pImageAddr[ALLELEMENT_COUNT];						// 存放图像数据

	char m_chSavePath[ALLELEMENT_COUNT][512];

	CSpendTime m_SpendTime[ALLELEMENT_COUNT][256];				// 统计是否超时使用

	LONGLONG m_prevqt1;											// 记录侧面工位上次回调中所读取的计算机时间信息
	LONGLONG m_lCameraTimeInfo[ALLELEMENT_COUNT];				// 记录三个工位上次回调中所读取的计算机时间信息
	LONGLONG m_lLastQt_PE[PE_COUNT];							// 记录光电上次计数
	LONGLONG m_lLastQt_TX[PE_COUNT];							// 记录图像号上次计数

	CList<CString,CString&> m_ListImagePath[CAMERA_COUNT];		// 图像保存路径

	CCriticalSection m_csImagePath[CAMERA_COUNT];				// 保护图像保存路径，防止操作互斥

	_ConnectionPtr m_Connect;									// 连接数据库
	_RecordsetPtr m_RecordSet;									// 记录集

	CLogManager  m_LogManager;									// 日志操作类
	CPIO24BLib m_IOCard[IOCARD_COUNT];			     			// PIO24B接口卡对象
	CioControl m_NewIOCard;
	bool m_bReadIOCardParamFile{ true };
	e_IOCardType m_eIOCardType{ e_IOCardType::e_IOCardType_PIOC };
	CINICfg m_cfgAlgCfg;
	CBayerTransform m_BayerTransform[CAMERA_COUNT];			

	BITMAPINFO* m_pBitmapInfo[CAMERA_COUNT];				
	HANDLE m_hImageInspectStart[ALLELEMENT_COUNT];				// 启动检测事件句柄数组
	e_ShowModeFlip m_eShowModeFlip[CAMERA_COUNT];				// 进行翻转类型
	s_ElementInfo m_sElement[ALLELEMENT_COUNT][DATA_ELEMENT_COUNT];			// 记录回调信息
	s_ImageInformationStruct m_sCameraImageInfo[CAMERA_COUNT]; //相机采集的图像信息

	e_SaveImgObj m_eSaveImageType[ALLELEMENT_COUNT];			//是否循环存图，0为不循环 1为循环；

	CObjAlloc m_InspectionObj;									// 检测对象

	s_CheckCameraInfo m_sCamInfo[ALLELEMENT_COUNT];
//	CExtenInfo m_arrExten[ALLELEMENT_COUNT];
	array<CExtenInfo, ALLELEMENT_COUNT> m_arrExten;
	CMultiObjSaveImgPara m_arrObjSaveImgPara[ALLELEMENT_COUNT];
	CCriticalSection m_csIOCard;


	
#ifdef __SGGrabber_
	CDHGrabberSG  m_CameraGrabber[CAMERA_COUNT];				// 相机采集对象
#else
//	CDHGrabberAVT  m_CameraGrabber[CAMERA_COUNT];				// 相机采集对象
// 	CDHGrabberJAI m_CameraGrabber[CAMERA_COUNT];
	CDHGrabberMER m_CameraGrabber[CAMERA_COUNT];

#endif
	

// Operations
public:

	// 删除文件夹
	BOOL DeleteDirectory(char* sDirName);

	// 执行报警动作,bStart为TRUE开始报警，bStart为FALSE停止报警
	BOOL AlarmAction(BOOL bStart);

	// 获取当前时间
	CString GetCurTime();

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

	// 更新记录
	void UpdateCheckData();

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

	/*************************************<>*************************************/
public:
	int m_iSaveImgObj{ 0 };
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__4B8DECDB_A7FA_4037_B1C9_9B8E1F287E47__INCLUDED_)
