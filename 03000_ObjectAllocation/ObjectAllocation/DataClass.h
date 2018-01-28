// DataClass.h: interface for the CDataClass class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATACLASS_H__5ECB82A9_A4F3_49B6_BE83_9C3AE25DDC0C__INCLUDED_)
#define AFX_DATACLASS_H__5ECB82A9_A4F3_49B6_BE83_9C3AE25DDC0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ObjAlloc.h"
#include "DlgObjStatus.h"
#include "ObjStatusList.h"
#include "InspectClass.h"
#include "SpendTime.h"
#include <queue>
#include "DlgRunInfo.h"
#include "StdAfx.h"
#include "alg_struct.h"
#include "algorithm"
#include "numeric"
#include "set"
#include "ExFn.h"

#ifdef DEBUG
#define CAMERA_COUNT 6
#endif



using namespace std;

#define MAX_CHAR_LENGTH		1000
#define MAX_CAMERA_COUNT	50
#define  MAX_KICK_COUNT     50
#define MAX_IMAGE_COUNT		2000000000

const int c_iMaxFileNum{ 20 };

typedef CInspBase*(*pFunc_Create)(int);
typedef void(*pFunc_Destroy)(CInspBase*);
typedef int(*pFunc_GetWndNum)();

typedef struct _ThreadParam
{
	void* pThis;
	int iThreadID;
}s_ThreadParam;

const int c_iErrorInfoTableLen{ 1024 };
const int c_iMaxStringLen{ 1024 };


class CDataClass
{
public:
	CDataClass();
	virtual ~CDataClass();

	// 初始化 通用
	s_Status Init(s_CheckInitParam sInitParam);//含窗口初始化+读参数

											   // 释放资源
	s_Status Free();

	// 检测函数 DEMO使用
	//s_Status Check(int iIndex, int iThreadID, s_CheckInParam sCheckInParam, s_CheckOutParam* sCheckOutParam);

	// 参数设置
	s_Status ShowDetail(int iCheckSN);

	s_Status Set(s_AlgModelInput sAlgModelInput);//读参数

	void DisplayObjectsStatus(int nCmdShow);

	// 检测函数 检测端使用
	s_Status Detect(s_CheckInParam sCheckInParam);



protected:
	CString m_strAppDir{ TEXT("") }, m_strCfgFilePath{ TEXT("") };
	CString m_strAlgconfigDir{ TEXT("") };			//AlgConfig.ini 路径
	CString m_strVitrualDisk{ TEXT("") };			//虚拟硬盘名称
	CStringArray m_strArrDllPath;		//存放动态库名称的矩阵

										//检测用	 
	long*	m_plObjBusyTimes{ NULL };						//记录每路检测对象全忙的次数
	int*	m_pnObjectNumber{ NULL };						//每一路的检测对象个数
	unique_ptr<bool[]>    m_pbIsDrawing{ nullptr };							//每路是否在画图

	int		m_iCallType{ 0 };								//调用类型 0为检测端实时调用	1为DEMO调用		2为检测端模拟调用
	int     m_iCheckGroupNum{ 0 };					//算法检测路数 即检测组数

	int		m_iMachineType{ 0 };						//胶囊为0 药液为1

	BOOL	m_bShowStatus{ 0 };		//**[7/26/2017 ConanSteve]**:  是否更新多对象检测窗口信息
	bool	m_bKillThread{ false };							//杀死检测线程

	int		m_iCameraNum{ 0 };							//相机个数
	int		m_iKickQueueNum{ 0 };						//剔废口个数
	int		m_iCheckThreadNum{ 0 };						//检测线程个数

	int		m_iSaveImageNum[MAX_CAMERA_COUNT]{ 0 };		//保存图像序号 每次开始存图开始为1;
	int     m_iProductNum[MAX_CAMERA_COUNT]{ 0 };			//胶囊产品个数

	int		m_iTimeType[MAX_CAMERA_COUNT]{ 0 };			//每个剔废队列的定时基准 0为时间毫秒 1为帧
	int		m_iTimeOut[MAX_CAMERA_COUNT]{ 0 };			//超时时间
	int		m_iTimeOutIndex[MAX_CAMERA_COUNT]{ 0 };	//每个剔废队列判断超时用的相机号

	int		m_iImageWidth[MAX_CAMERA_COUNT]{ 0 };		//每相机图像宽度
	int		m_iImageHeight[MAX_CAMERA_COUNT]{ 0 };		//每相机图像高度
	int		m_iImageByte[MAX_CAMERA_COUNT]{ 0 };			//每相机图像字节
	array<int, MAX_CAMERA_COUNT> m_iCamSmallImgCnt{ 0 };//*[6/30/2017 ConanSteve]*:每个相机当前图像号图像计数
	array<int, MAX_CAMERA_COUNT>		m_iImageSize;			//每相机图像大小

	int		m_iCamCircle[MAX_CAMERA_COUNT]{ 0 };			//每相机处理的图像个数
	int		m_iCamGroup[MAX_CAMERA_COUNT]{ 0 };			//每相机的组号

	array<int, MAX_CAMERA_COUNT> m_arr_iCamLastImgSN{ 0 };
	array<int, MAX_CAMERA_COUNT> m_arr_iCamSmallImgNum{ 0 };//*[6/30/2017 ConanSteve]*:每个相机大张图包含的小张图数量
	int		m_iCamKick[MAX_CAMERA_COUNT]{ 0 };			//每相机所在的剔废队列编号
	int		m_iCamKickOffsetX[MAX_CAMERA_COUNT]{ 0 };	//每相机中第一个结果的剔废偏移量X(字节)
	int		m_iCamKickOffsetY[MAX_CAMERA_COUNT]{ 0 };	//每相机中第一个结果的剔废偏移量Y(字节)
	int		m_iCamKickOffsetX_Written[MAX_CAMERA_COUNT]{ 0 };	//每相机中第一个结果的剔废偏移量X(统计是否写入结果),标记结果是否写入


	int		m_iCheckOutNum[MAX_CAMERA_COUNT]{ 0 };		//每相机经算法检测返回的结果个数
	int		m_iCheckOutSize[MAX_CAMERA_COUNT]{ 0 };		//每相机输出结构体void* pParam大小（单位为字节）
	int		m_iCheckInSize[MAX_CAMERA_COUNT]{ 0 };		//每相机输入结构体void* pParam大小
	int     m_iCheckStatusSize[MAX_CAMERA_COUNT]{ 0 };	//每相机结果状态结构体void* pParam大小

	int		m_iGroupCheckInSize[MAX_CAMERA_COUNT]{ 0 };		//每组输入结构体void* pParam大小；
	int		m_iGroupCheckOutSize[MAX_CAMERA_COUNT]{ 0 };		//每组输出结构体void* pParam大小；
	int		m_iGroupCheckStatusSize[MAX_CAMERA_COUNT]{ 0 };	//每组返回状态结构体void* pParam大小；
	int		m_iGroupCheckOutNum[MAX_CAMERA_COUNT]{ 0 };		//每组输出结构体中结果个数
	int		m_iGroupCheckOutPerResultSize[MAX_CAMERA_COUNT]{ 0 }; //每组输出结构体中每个结果大小（单位为字节）

	array<UINT, MAX_KICK_COUNT>	m_iKickErrorCount;	//每个剔废队列坏品个数
	int		m_iKickResultNum[MAX_CAMERA_COUNT]{ 0 };		//每个剔废队列中每个元素包含检测结果个数
	int		m_iKickElementNum[MAX_CAMERA_COUNT]{ 0 };	//每个剔废队列中相机个数
	int		m_iKickQueueSize[MAX_CAMERA_COUNT]{ 0 };		//每个剔废队列中元素大小（单位为字节）
	int		m_iKickFirstCam[MAX_CAMERA_COUNT]{ 0 };		//每个剔废队列中第一个相机编号
	int		m_iKickCircleSN[MAX_CAMERA_COUNT]{ 0 };		//剔废线程中使用的计数编号
	int		m_iKickImageSN[MAX_CAMERA_COUNT][256];  //每个剔废队列的图像号
	BYTE*	m_pKickResult[MAX_CAMERA_COUNT][256]{ NULL };	//存放剔废表信息
	BYTE*	m_pbKickWrite[MAX_CAMERA_COUNT][256]{ NULL };	//记录每个剔废表格是否被写入

	int		m_iGroupErrorCount[MAX_CAMERA_COUNT]{ 0 };			//每个组中错误个数
	int		m_iGroupImageSize[MAX_CAMERA_COUNT]{ 0 };			//每个组中所有相机的图像大小总和
	int		m_iGroupElementNum[MAX_CAMERA_COUNT]{ 0 };			//每个组中相机个数
	int		m_iGroupFirstCam[MAX_CAMERA_COUNT]{ 0 };				//每个组中第一个相机的编号，用于获得当前组使用的剔废队列号及剔废偏移量,以0为开始
	int		m_iGroupLastImageSN[MAX_CAMERA_COUNT]{ 0 };			//每组中上一张图像号
	int		m_iGroupCircle[MAX_CAMERA_COUNT]{ 0 };				//每组处理的图像个数
	int		m_iGroupCallCount[MAX_CAMERA_COUNT]{ 0 };			//每组进入采集回调的总个数
	int	    m_iGroupCheckQueueNum[MAX_CAMERA_COUNT]{ 0 };		//每组检测队列中元素个数
	int		m_iGroupAbnormalNum[MAX_CAMERA_COUNT]{ 0 };			//每组因为检测忙丢弃的个数
	int		m_iGroupLeftObj[MAX_CAMERA_COUNT]{ 0 };				//空闲对象个数
	int		m_iGroupLeftList[MAX_CAMERA_COUNT]{ 0 };				//空闲队列个数
	int		m_iGroupAlgCheck[MAX_CAMERA_COUNT]{ 0 };				//算法检测个数

	int     m_iGroupIsSaving[MAX_CAMERA_COUNT]{ 0 };				//每组是否正在保存图像 0为未进行中 1为进行中
	bool	m_bGroupSaveImage[MAX_CAMERA_COUNT]{ 0 };			//每组是否保存图像	0为不保存 1为保存
	int		m_iGroupSaveImageType[MAX_CAMERA_COUNT]{ 0 };		//每组保存图像的类型
	int		m_iGroupSaveImageCount[MAX_CAMERA_COUNT]{ 0 };		//每组正在保存图像的数量
	int		m_iGroupSaveImageCount_SET[MAX_CAMERA_COUNT]{ 0 };	//每组保存图像的设定值


	int		m_iIndexInGroup[MAX_CAMERA_COUNT]{ 0 };		//每个相机在组中的序号
	int		m_iIndexInKick[MAX_CAMERA_COUNT]{ 0 };		//每个相机的第一个结果在剔废队列中的序号

	int		m_iPeriodImageSN[MAX_CAMERA_COUNT][256]{ 0 };//每个周期的图像号
	int		m_iPeriodIndex[MAX_CAMERA_COUNT][256]{ 0 };	//周期元素索引

	int		m_iMaxLogLineNum{ 0 };								//日志记录条数
	int		m_iCallInterval{ 0 };						//相机回调之间允许的回调间隔
	int		m_iPeriodInterval{ 0 };						//产品周期间允许的最小时间间隔
	int		m_iSaveImageStatus{ 0 };						//停止保存图像 0:初始状态 1:保存图像 2:多线程已经停止保存图像且已告诉系统

	int		m_iIdleJob[MAX_CAMERA_COUNT]{ 0 };			//各路检测对象（地址）
	array<array<int, MAX_CAMERA_COUNT>, 20>	m_nObjIndex;			//每路检测对象检测状态 1为检测中 0为闲置

	int*	m_pWriteDataStatus[MAX_CAMERA_COUNT]{ NULL };	//检测线程队列每个元素的状态  0为闲置 1为正在写入图像数据 2为写入数据完成
	int*	m_pDetectStatus[MAX_CAMERA_COUNT]{ NULL };		//检测线程队列每个元素的状态 1为正在检测中 0为未检测
	int*	m_pCircleSN[MAX_CAMERA_COUNT]{ NULL };			//检测线程队列每个元素的编号 编号小的先被处理
	BYTE*   m_pImageData[MAX_CAMERA_COUNT]{ NULL };			//存放图像数据[2015-10-30 zhy]
	BYTE*	m_pCheckInParam[MAX_CAMERA_COUNT]{ NULL };		//存放检测输入中的*pParam信息
	BYTE*	m_pCheckOutParam[MAX_CAMERA_COUNT]{ NULL };		//存放检测输出中的*pParam信息
															//BYTE*	m_pCheckStatusParam[MAX_CAMERA_COUNT]{ NULL };	//存放检测结果中的*pParam信息
	array<BYTE*, MAX_CAMERA_COUNT> m_pCheckStatusParam;

	void*	m_pCallThis{ NULL };							//回调函数所属的对象指针

	double  m_dFreq{ 0 };								//计算时间使用

	bool	m_bWriteLog{ false };							//是否写日志
	BOOL	m_bCamComplete[MAX_CAMERA_COUNT][256];	//每一路相机采图是否到来

	HANDLE m_hImageInspectStart[MAX_CAMERA_COUNT]{ 0 };				//启动检测事件句柄数组

	CSpendTime m_spTime[MAX_CAMERA_COUNT][256];					//统计超时

	LONGLONG m_lCamTime[MAX_CAMERA_COUNT][256];					//记录采集到来时间，用来求相邻图像号时间差
	LONGLONG m_lPeriodTime[MAX_CAMERA_COUNT][256];				//记录每个相机的采图到来时间，用来判断新周期

	s_ThreadParam m_sThreadParam[MAX_CAMERA_COUNT]{ NULL };				//存放线程参数，和检测组绑定
	s_CheckInParam* m_spCheckInParam[MAX_CAMERA_COUNT]{ NULL };			//存放检测线程参数
	s_CheckOutParam* m_spCheckOutParam[MAX_CAMERA_COUNT]{ NULL };		//存放检测结果
																		/*****************************************************************************************************************************************************************************************/
	s_CheckInParam** m_sppCheckInParamEx{ NULL };//**[7/24/2017 ConanSteve]**:  存放检测线程参数，和检测对象绑定
	s_CheckOutParam** m_sppCheckOutParamEx{ NULL };
	s_Status** m_sppCheckStatusParamEx{ NULL };			//**[7/24/2017 ConanSteve]**:  存放检测结果,和检测对象绑定
	BYTE**	m_ppCheckInParamEx{ NULL };		//**[7/24/2017 ConanSteve]**:  存放检测输入中的*pParam信息,和检测对象绑定
	BYTE**	m_ppCheckOutParamEx{ NULL };		//**[7/24/2017 ConanSteve]**:  存放检测输出中的*pParam信息,和检测对象绑定
	BYTE**	m_ppCheckStatusParamEx{ NULL };	//**[7/24/2017 ConanSteve]**:  存放检测结果中的*pParam信息,和检测对象绑定
	int** m_ppCacheUseCnt{ NULL };	//**[8/1/2017 ConanSteve]**:  检测队列使用计数
	array<array<int, 50>, MAX_CAMERA_COUNT> m_ppObjUseCnt{ NULL };	//**[8/1/2017 ConanSteve]**:  检测对象使用计数
																	/*****************************************************************************************************************************************************************************************/

//	array<array<COptionalFunc,20>, 20> m_ppCopyWindowParaCacheQueue;//**[10/9/2017 ConanSteve]**:  图像缓存队列对应的信息
//	array<array<int, 20>, 20> m_arr_iTimeOfWaitInCacheQueue;
	array<array<LONGLONG, 20>, 20> m_arr_ldTickOfPushInCacheQueue;
//	array<array<COptionalFunc, 20>, 20> m_ppCopyWindowParaAlgObjQueue;//**[10/9/2017 ConanSteve]**:  算法检测对象队列对应的信息
	array<array<int, 20>, 20> m_arr_iTimeOfWaitInAlgObjQueue;
	array<array<LONGLONG, 20>, 20> m_arr_ldTickOfPushInAlgObjQueue;
	
	s_Status* m_spCheckStatusParam[MAX_CAMERA_COUNT]{ NULL };			//存放检测结果

	CCriticalSection m_csTime;									//获取实际
	CCriticalSection m_csGlobalCheck;							//检测时查询
	CCriticalSection m_csWriteDataStatus[MAX_CAMERA_COUNT];		//保护写数据互斥
	CCriticalSection m_csCamComplete[MAX_CAMERA_COUNT];			//保护采集互斥
	CCriticalSection m_csCameraDiff[MAX_CAMERA_COUNT];			//保护相机间隔时间互斥
	CCriticalSection m_csGroupDiff[MAX_CAMERA_COUNT];			//保护组时间互斥
	CCriticalSection m_csAlgCheck[MAX_CAMERA_COUNT];			//保护算法检测
	CCriticalSection m_csAlgDisp[MAX_CAMERA_COUNT];				//保护算法显示
	CCriticalSection m_csAlgSave[MAX_CAMERA_COUNT];				//保护算法保存
	CCriticalSection m_csSaveImage[MAX_CAMERA_COUNT];			//保存图像
	CCriticalSection m_csKickResult[MAX_CAMERA_COUNT][256];		//保护剔废结果

	array<array<CCriticalSection, 256>, MAX_KICK_COUNT> m_csBoolKickWrite;

	CEvent m_evtKillKickThread;									//关闭剔废线程
	CEvent m_evtKillInspThread[50];								//关闭检测线程



	array<array<CLogFile, c_iMaxFileNum>, MAX_CAMERA_COUNT> m_fileCam;
	array<array<CLogFile, c_iMaxFileNum>, MAX_CAMERA_COUNT> m_fileDetect;
	array<array<CLogFile, c_iMaxFileNum>, MAX_CAMERA_COUNT> m_fileCheck;
	array<array<CLogFile, c_iMaxFileNum>, MAX_CAMERA_COUNT> m_fileKick;
	CLogFile m_fileRunInfoStatus;
	CLogFile m_fileDebugInfo;
	CCriticalSection m_csRunInfoStatus;
	array<UINT, MAX_CAMERA_COUNT> m_iGroupBadCnt;//**[9/25/2017 ConanSteve]**:  每工位坏品数
	array<UINT, MAX_CAMERA_COUNT> m_iGroupGoodCnt;//**[9/25/2017 ConanSteve]**:  每工位好品数
	array<UINT, MAX_KICK_COUNT> m_iKickGoodCnt;//**[9/25/2017 ConanSteve]**:  每个剔废回调全0的计数


	shared_ptr<CDlgObjStatus> m_pObjStatusDlg{ NULL };								//显示对象状态

	PKICKCALLBACK m_CheckCallback;								//剔废回调
	PSTATUSCALLBACK m_StatusCallback;							//中间状态回调

																// 初始化检测对象
	BOOL InitObjects(CString strInitFile, int iCallType);
	void GetLogDirPath(CString&);

	// 检测线程函数
	static DWORD ImageInspectionThread(LPVOID lpParam);
	DWORD ImageInspectionThreadFunc(int iThreadID);
	/*	DWORD InspectionContext(int iWorkStationSN, int iThreadID);*/

	// 剔废线程
	static DWORD KickThread(LPVOID lpParam);
	/*	DWORD KickThreadFunc();*/

	UINT ThreadFunc_ChangeStatus(LPVOID);

	// 判断是否是新周期
	BOOL JudgeNewCircle(int iGroupSN, int iCameraSN, int iImageSN, int iCircleSN);

	// 写剔废表
	/*	BOOL ThreadWriteCheckResult(int iGroupSN, int iIndex);*/

	// 写结果（初始化写初始化结果，超时写坏结果）
	//**[7/26/2017 ConanSteve]**:  bInit==true .表示丢图写丢图的结果, false表示正常检测的结果
	BOOL ThreadWriteResult(int iImageSN, int iGroupSN, int iKickSN, bool bInit);


	// 保存图像
	/*	BOOL ThreadSaveImage(int iGroup, int iIndex);*/

	// 创建日志文件
	BOOL CreateLogFile();
	BOOL FlushLogFile();//**[8/17/2017 ConanSteve]**:  清空日志文件缓冲区
						//void WriteLogInfo(e_LogType eLogType, int iGroup, int iIndex, CString& sInfo);//**[8/23/2017 ConanSteve]**:  写入日志信息

						// 创建多层目录
	BOOL CreateMultiLevelPath(const CString &strPath);

	// 判断目录是否存在
	/*	BOOL JudgePathExist(const CString& strPath);*/

	// 保存图像
	int SaveBmp(const TCHAR* pPathName, int iWidth, int iHeight, int iByte, TBYTE *pDest);






	// 显示状态信息
	int ShowCurrStatus();

	// 关闭所有线程
	int KillAllThread();

	// 释放线程使用内存
	int ReleaseThreadData();
	/************************************************************************/
	/*                       [6/6/2017 ConanSteve]                          */

private:
	//**[7/24/2017 ConanSteve]**:  多对象收图检测函数
	s_Status DetectEx(s_CheckInParam sCheckInParam);
	//**[7/24/2017 ConanSteve]**:  2次Detect() 检测函数
	DWORD InspectionContextEx2(int iGroupSN, int iThreadID);
	//	s_Status InitEx2(s_CheckInitParam sInitParam);
	void CheckEx2(int iIndex, int iThreadID, s_CheckInParam sCheckInParam);
	BOOL ThreadWriteCheckResultEx2(int iGroupSN, s_CheckInParam* spCheckInParam, s_CheckOutParam* spCheckOutParam);
	DWORD KickThreadFuncEx2();
	bool CopyCheckData(int iIndexInCache, int iIndexOfObj, int iGroupSN);
	void UpdateRunInfo();

	typedef struct s_ImgInfoBeenSaved {
		int SN{ 0 };
		vector<CString> vec_sSaveImgPaths;
	}s_ImgInfoBeenSaved;
	typedef struct s_ImgInfoWaitForSave {
		int SN{ 0 };
		int iMinIndex{ 0 };
		s_CheckInParam sCheckInParam;
	}s_ImgInfoWaitForSave;
	queue<s_ImgInfoBeenSaved > m_queSaveImgHistory;/** 存图路径历史              [6/6/2017 ConanSteve]**/
	queue<s_ImgInfoWaitForSave> m_queImgsWaitForSave;
	CCriticalSection m_srwlSaveImgHistory;
	CCriticalSection m_srwlImgsWaitForSave;
	int m_iSizeOfOneArrImgs{ 0 };/**   单组图的大小             [6/6/2017 ConanSteve]**/
	HANDLE m_hSaveImgs{ 0 };
	HANDLE m_hEventSaveImgs{ 0 };
	int m_iSaveImgObj{ 0 };
	int m_iSaveImgNamingRegulation{ 0 };
	int m_iProductID{ 0 };
	bool m_bIsCircle{ false };//**[7/14/2017 ConanSteve]**:  是否循环存图
	int FnSavebmp(const TCHAR *lpszPathName, int W, int H, int nByte, TBYTE *lpDest);
	DWORD static FnThreadSaveImg(PVOID pParam);
	DWORD FnThreadSaveImgFunc();
	/*	BOOL OP_Directory(LPCSTR lpsrcPath, LPCSTR lpdstPath, int type, int flag);*/
	ULONGLONG GetFreeSpaceOfHardDisk(LPCTSTR sDisk);
	//*[7/3/2017 ConanSteve]*:  删除文件
	bool DeleteDirectory(TCHAR* sDirName);
	TCHAR* GetErrorInfo(LPCTSTR lpszErrorInfo = NULL);//**[8/2/2017 ConanSteve]**:  使用错误信息缓存区来保存返回的错误信息
	array<CCriticalSection, MAX_CAMERA_COUNT> m_srwlImgQue;
	//*[7/3/2017 ConanSteve]*:  保存是否需要保存图像，如果需要保存则在保存完图像后再重置存图队列标志
	typedef struct s_ResetWriteStatus {
		bool bCheckComplete{ false };
		volatile bool bSaveComplete{ false };
		s_ResetWriteStatus()
		{

		}
		void FnSetVal(bool bCheckComplete, bool bSaveComplete) {
			this->bCheckComplete = bCheckComplete;
			this->bSaveComplete = bSaveComplete;
		}
		bool bFnIsComplete() {
			return bCheckComplete && bSaveComplete;
		}
	}s_ResetWriteStatus;
	array<int, MAX_CAMERA_COUNT> m_iSmallImgSize;
	int m_iObjDetectMode{ 0 };//*[7/14/2017 ConanSteve]*:  多对象调用算法的Detect模式，1次Detect还是2次Detect
	int m_iSysDetectMode{ 0 };//*[7/14/2017 ConanSteve]*:  系统调用多对象的Detect模式
	array<shared_ptr<s_ResetWriteStatus>, 50> m_pResetWriteStatus{ nullptr };
	//*[7/3/2017 ConanSteve]*:  用于显示系统图像用
	array<BITMAPINFO*, MAX_CAMERA_COUNT> m_pBitmapInfo{ NULL };
	//*[7/3/2017 ConanSteve]*:  显示系统图像结构
	array<s_DrawSysPic, MAX_CAMERA_COUNT> m_arrDrawSysPic;
	bool m_bShowObjRunInfoWnd{ false };//**[7/26/2017 ConanSteve]**:  是否显示多对象实时检测信息
	array<array<char, c_iMaxStringLen>, c_iErrorInfoTableLen> m_arrErrorInfo;
	int m_iErrorInfoIndex{ 0 };
	CCriticalSection m_csErrorInfoTable;//**[7/31/2017 ConanSteve]**: 错误信息循环列表临界区
										//shared_ptr<CDlgRunInfo> m_pDlgRunInfo;
	CDlgRunInfo* m_pDlgRunInfo{ NULL };//**[11/9/2017 ConanSteve]**:  多对象运行信息窗口
	array<int, MAX_CAMERA_COUNT> m_iMaxOffsetOfKick{ 0 };//**[8/1/2017 ConanSteve]**:  每个剔废口所有工位的最大偏移量
	array<int, MAX_CAMERA_COUNT> m_iKickRemediation{ 0 };//**[7/31/2017 ConanSteve]**:  超时补踢计数
	array<int, MAX_CAMERA_COUNT> m_arr_iAlgExceptionCnt{ 0 };//**[8/1/2017 ConanSteve]**:  算法异常计数
	array<int, MAX_CAMERA_COUNT> m_iKickOverTimeCnt{ 0 };//**[8/2/2017 ConanSteve]**:  剔废超时计数
	int m_iAlgExceptionVal{ 0 };//**[8/1/2017 ConanSteve]**:  算法异常值
	int m_iDefaultCheckRslt{ -1 };//**[7/26/2017 ConanSteve]**:  检测结果默认值
	int m_iLostImageRslt{ -4 };//**[12/27/2017 ConanSteve]**:  多对象丢图默认检测结果
	bool m_bEnableXCGUI{ false };//**[9/20/2017 ConanSteve]**:  是否使用炫彩GUI动态库
	bool m_bEnableOvertimeCallback{ false };//**[9/20/2017 ConanSteve]**:  是否打开超时回调功能
	CINICfg m_iniAlgCfg;//**[9/20/2017 ConanSteve]**:  algConfig配置文件
	bool m_bEnableExtenInfo{ false };//**[10/21/2017 ConanSteve]**:  是否使用扩展信息结构体
	bool m_bEnableInitObjectAndCheck{ true };//**[10/21/2017 ConanSteve]**:  是否执行多对象的初始化和检测
	bool m_bEnableAlgInit{ true };//**[10/21/2017 ConanSteve]**:  是否执行算法初始化
	int m_iSingleTest{ -1 };//**[11/9/2017 ConanSteve]**:  是否单路测试：为-1时，所有检测组都检测，为正时，只检测当前路

	array<bool, MAX_CAMERA_COUNT> m_arr_bIsLoadModel{ false };//**[1/6/2018 ConanSteve]**:是否正在或者等待加载模板
	void InitTestInfo();//**[10/16/2017 ConanSteve]**:  从配置文件读取测试信息


						/************************************************************************/


						// 	// Generated message map functions
						// 	//{{AFX_MSG(CTestExeDlg)
						// 	//}}AFX_MSG
						// 	LRESULT OnMsgCreateStDlg(WPARAM, LPARAM);
						// 	DECLARE_MESSAGE_MAP()
public:

	bool IsCheckCompleted(int iGroupSN, int iKickGroupSN, int iImgSN);//**[9/20/2017 ConanSteve]**: 检查所有工位是否检测完成，如果检测完成，使用剔废回调 
	bool ReseMultiObject();
	bool Reset();
};

#endif // !defined(AFX_DATACLASS_H__5ECB82A9_A4F3_49B6_BE83_9C3AE25DDC0C__INCLUDED_)