
// MainFrm.h : interface of the CMainFrame class
//

#pragma once
#include "CheckSystem.h"
#include "DlgParamSet.h"
#include "ImageHandle.h"
#include "ExtenInfo.h"
#include "..\\..\\IncPublic\\ThreeTierRights.h"
#include <map>
#include <queue>
#include "Raw2Rgb.h"
#pragma comment(lib, "..\\Lib\\raw2rgb.lib")
//**[9/11/2017 ConanSteve]**:  相机头文件
#include "DHGrabForMER.h"
#include "DHGrabForSG.h"
//#include "DHGrabForAVT.h"
//#include "DHGrabForJAI.h"
//#include "DHGrabForBaumer.h"

//**[10/30/2017 ConanSteve]**:  MultiObejct头文件
#include "ObjAlloc.h"
#include "MultiObjectDef.h"

//**[9/11/2017 ConanSteve]**:  接口卡头文件
#include "DHIOCardForSG.h"
#include "DHIOCardForPCI6356.h"
#include "DHIOCardForRS232_XZXNYN.h"
#include "DHIOCardForPIO24C.h"


//**[11/24/2017 ConanSteve]**:  算法接口

#include "alg_struct_general.h"
#include "alg_struct_stopper.h"
#include "alg_struct_lampInsp.h"
#include "alg_struct_Label.h"//**[12/12/2017 ConanSteve]**:  采血管结构体，过渡使用


/*************************************<系统结构体>************************************[11/16/2017 ConanSteve]*/
//**[11/16/2017 ConanSteve]**:  错误信息传递
class CRetInfo{
public:
	bool bRet{ false };
	CString sErrorInfo{_T("")};
};

typedef struct s_DefectInfo {
public:
	int iCnt;//**[12/1/2017 ConanSteve]**:  缺陷计数统计
	CString sDesc{ _T("") };//**[12/1/2017 ConanSteve]**:  缺陷描述
	s_DefectInfo(int iCnt, LPCTSTR lpszDesc)
	{
		this->iCnt = iCnt;
		this->sDesc = lpszDesc;
	}
};


/*************************************<>************************************[11/16/2017 ConanSteve]*/

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
public:
//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar        m_wndStatusBar;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CWnd* pParentWnd = NULL, LPCTSTR lpszMenuName = NULL, DWORD dwExStyle = 0, CCreateContext* pContext = NULL);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);





/************************************************************************/
private://**[8/24/2017 ConanSteve]**:  私有参数
	int m_cxScreenSize{ 0 }, m_cyScreenSize{ 0 };
	//**[11/15/2017 ConanSteve]**:  产品线名称
	e_ProductLine m_eProductLine{ e_ProductLine_NO };
	/*************************************<>************************************[9/18/2017 ConanSteve]*/
	bool bExit{ false };//**[12/13/2017 ConanSteve]**:  是否退出系统
public://**[8/24/2017 ConanSteve]**:  公有参数

	//**[1/27/2018 ConanSteve]**:	是否需要加密狗验证， 此值只有系统开发人员具有权限修改
	bool m_bEnableVerification{ true };
	//**[11/23/2017 ConanSteve]**:  存图方式，0，是算法存图，1是系统存图
	int m_iSaveImgObj{e_SaveImgObj_Alg};
	//**[9/13/2017 ConanSteve]**:	保存图像类型，0是不存图，正值按配置文件定义
	int m_iSaveImgType{ e_SaveImgType_NO };
	//**[8/24/2017 ConanSteve]**:  camera number
	int m_iGrabberNum{ 0 };
	//**[10/30/2017 ConanSteve]**:  检测组数即工位数
	int m_iCheckGroupNum{ 0 };
	//**[9/4/2017 ConanSteve]**:  系统是否初始化成功
	bool m_bInitSucceed{ false };
	//**[9/4/2017 ConanSteve]**:  系统是否初始化完成
	bool m_bInitCompleted{ false };
	//**[9/5/2017 ConanSteve]**:  是否正在检测
	bool m_bIsDetecting{ false };
	//**[9/5/2017 ConanSteve]**:  是否开启系统画图功能(画原图)
	bool m_bEnableDrawImg{ false };
	//**[9/6/2017 ConanSteve]**:  是否初始化相机
	bool m_bEnableInitGrabber{ false };
	//**[9/6/2017 ConanSteve]**:  是否初始化接口卡
	bool m_bEnableInitIOCard{ false };
	//**[12/14/2017 ConanSteve]**:  是否初始化接口卡成功
	bool m_bInitIOCardSuccess{ false };
	//**[10/30/2017 ConanSteve]**:  是否初始化多对象
	bool m_bEnableInitMultiObjectAndCheck{ false };
	//**[9/13/2017 ConanSteve]**:  是否循环存图
	bool m_bEnableCircleSaveImg{ true };
	//**[1/23/2018 ConanSteve]**:	是否空品踢废
	bool m_bEnableEmptyKick{ true };
	//**[11/23/2017 ConanSteve]**:  系统最大存图张数
	int m_iMaxNumberOfSaveImg{ 100 };
	//**[1/17/2018 ConanSteve]**:  算法绘图类型
	General::e_DrawImgType m_iDrawImgType;
	//**[1/15/2018 ConanSteve]**:  单路测试
//	int m_iSingleTest = -1;
	//**[8/24/2017 ConanSteve]**:  模块路径
	CString m_sPathOfApp{ _T("") };
	//**[8/28/2017 ConanSteve]**:  AlgConfig.ini文件绝对路径
	CString m_sPathFileOfAlgCfg{ _T("") };
	//**[8/24/2017 ConanSteve]**:  虚拟硬盘路径
	CString m_sPathOfVirtualDisk{ _T("") };
	//**[9/6/2017 ConanSteve]**:  系统配置文件路径（在ModelInfo文件夹中）
	CString m_sPathFileOfCheckSysCfgInModelInfo{ _T("") };
	//**[8/25/2017 ConanSteve]**:  算法模板文件夹路径,包含当前模板名称
	CString m_sPathOfAlgModelInfo{ _T("") };
	//**[8/25/2017 ConanSteve]**:  窗口名称
	CString m_sWindowTitle{ _T("大恒图像通用检测系统") };
	//**[8/25/2017 ConanSteve]**:  当前模板名称
	CString m_sModelName{_T("")};
	//**[8/25/2017 ConanSteve]**:  配置文件夹路径
	CString m_sPathOfCfg{ _T("") };
	//**[9/1/2017 ConanSteve]**:  系统日志文件夹路径
	CString m_sPathOfSysLogs{_T("")};
	//**[9/6/2017 ConanSteve]**:  调试信息文件路径
	CString m_sPathFileOfDebugInfo{ _T("") };

/*************************************<相机相关>************************************[9/5/2017 ConanSteve]*/
//**[8/28/2017 ConanSteve]**:  当相机类型不全部一样时，设置此值为666
	e_GrabType m_iUnitedGrabType{ e_GrabType::e_GrabType_SG };
	//**[9/8/2017 ConanSteve]**:  参数设置窗口
	shared_ptr<CDlgParamSet> m_pdlgParamSet{nullptr};

	
	//**[11/23/2017 ConanSteve]**:  每工位已经存图的张数
	unique_ptr<int[]> m_p_iCurNumOfSaveImgOfEachGrab;
	//**[11/24/2017 ConanSteve]**:  当前图像号的小张图计数
	unique_ptr<int[]> m_p_iCurNumOfSmallImgOfEachImgSN;
	unique_ptr<LampInsp::s_CheckCameraInfo []> m_p_CamInfo_LampInsp;//**[11/24/2017 ConanSteve]**:  灯检机算法图像结构体
	
	array<BYTE, 256> m_pLutR, m_pLutG, m_pLutB;
	
/*************************************<检测相关>************************************[9/13/2017 ConanSteve]*/
	using VecInt = vector<int>;
	/*************************************<>************************************[9/15/2017 ConanSteve]*/
	CXMLCfg m_xmlAlgConfig;
	CINICfg m_iniAlgConfig;
	CINICfg m_iniCheckSystemConfig;
	
	CObjAlloc m_multiObject;//**[10/30/2017 ConanSteve]**:  多对象
	CThreeTierRights m_threeTierRights;
	/*************************************<算法控制>************************************[12/14/2017 ConanSteve]*/
	int m_iAlgDisplayMode{0};//**[12/14/2017 ConanSteve]**:  算法显示图像类型，默认0显示所有，1不显示，2显示好品，3显示坏品，4显示有产品，5显示无产品，具体算法可可根据ParaGradList自行定义
	/*************************************<扩展信息接口>************************************[11/22/2017 ConanSteve]*/


	/*************************************<接口卡组>************************************[12/28/2017 ConanSteve]*/
	int m_iIOCardNum{ 0 };//**[9/11/2017 ConanSteve]**:  接口卡个数
	array<CIOCardInfo, MAX_IOCARD_NUM> m_arr_IOCardInfo;//**[12/8/2017 ConanSteve]**:  接口卡信息
	array<shared_ptr<CDHIOCard>, MAX_IOCARD_NUM> m_arr_pIOCard;
	array<CString, MAX_IOCARD_NUM> m_arr_sPathFileOfIOCardInitFile;//**[12/8/2017 ConanSteve]**:  接口卡初始化文件相对路径，相对于Run
	array<e_IOCardType, MAX_IOCARD_NUM> m_arr_iIOCardType;


	/*************************************<数据显示相关>************************************[12/1/2017 ConanSteve]*/
	int m_iPENum{0};//**[12/28/2017 ConanSteve]**:  光电个数或者系统输入信号个数
	int m_iCheckTotleCntType{ e_CheckTotleCntType_Rlst};//**[12/4/2017 ConanSteve]**:  检测总数计数类型
	int m_iCheckCntTotle{ 0 };//**[12/1/2017 ConanSteve]**:  检测总数
	int m_iCheckErrorCntTotle{ 0 };//**[12/1/2017 ConanSteve]**:  总的缺陷总数
	array<int, MAX_CAMER_COUNT> m_arr_iPECount;//**[12/28/2017 ConanSteve]**:  光电计数


	/*************************************<状态栏显示信息控制>************************************[12/28/2017 ConanSteve]*/
	bool m_bEnableShowPE{ true };//**[1/26/2018 ConanSteve]**:	是否显示光电输入
	bool m_bEnableShowAutoKick{ true };//**[1/26/2018 ConanSteve]**:	是否显示补踢
	bool m_bEnableShowGrabLostFrame{ true };//**[1/26/2018 ConanSteve]**:	是否显示相机丢帧

	/*************************************<相机组>************************************[12/28/2017 ConanSteve]*/

	array<bool, MAX_CAMER_COUNT> m_arr_bWhetherImageConvertedToMultiObj;//**[1/26/2018 ConanSteve]**:	表示算法接受的图像是否是系统bayer变换和翻转之后的 
	array<int, MAX_CAMER_COUNT> m_arr_iGrabCallbackCnt;//**[12/1/2017 ConanSteve]**:  相机采集回调计数
	array<int, MAX_CAMER_COUNT> m_arr_iLastHardImageCountOfGrabInRegister;//**[12/13/2017 ConanSteve]**:  上次图像寄存器计数
	array<int, MAX_CAMER_COUNT> m_arr_iHardImageCountOfGrabInRegister;//**[12/13/2017 ConanSteve]**:  相机对应图像寄存器大张计数	
	array<int, MAX_CAMER_COUNT> m_arr_iLostFrameEachGrab;//**[1/26/2018 ConanSteve]**:	每个相机丢帧计数
	array<int, MAX_CAMER_COUNT> m_arr_iCurSmallCntEachGrab;//**[1/26/2018 ConanSteve]**:	每个相机当前图像号的小张计数 

	array<int, MAX_CAMER_COUNT> m_arr_iWidth;//**[1/26/2018 ConanSteve]**:	相机图像宽度 
	array<int, MAX_CAMER_COUNT> m_arr_iHeight;//**[1/26/2018 ConanSteve]**:	相机图像高度 
	array<int, MAX_CAMER_COUNT> m_arr_iChannel;//**[1/26/2018 ConanSteve]**:	相机图像通道数，黑白图为1，彩色图为3 
	array<int, MAX_CAMER_COUNT> m_arr_iSmallImgCntPerGrab;//**[8/30/2017 ConanSteve]**:  每个相机大张图包含的小张图数量
	array<int, MAX_CAMER_COUNT> m_arr_iSystemBayerType;//**[12/28/2017 ConanSteve]**:   系统bayer变换类型
	array<int, MAX_CAMER_COUNT> m_arr_iSystemFilpType;//**[12/28/2017 ConanSteve]**:  系统翻转类型
	array<int, MAX_CAMER_COUNT> m_arr_iHalconBayerType;//**[1/26/2018 ConanSteve]**:	相机Halcon算法需要的bayer变换值 
	array<int, MAX_CAMER_COUNT> m_arr_iHalconRevertType;//**[1/26/2018 ConanSteve]**:	相机Halcon算法需要实现的反转类型 												  
	array<int, MAX_CAMER_COUNT> m_arr_iKickGroupSNOfCurGrab;//**[11/22/2017 ConanSteve]**:  每个相机所在的剔废组号																	
	array<int, MAX_CAMER_COUNT> m_arr_iCheckGroupSNOfCurGrab;//**[11/22/2017 ConanSteve]**:  每个相机所在的检测组号	
	array<int, MAX_CAMER_COUNT> m_arr_iLastImgSNOfEachGrab;//**[11/24/2017 ConanSteve]**:  每个相机的上一张图的图像号	
	array<int, MAX_CAMER_COUNT> m_arr_iCheckGroupFirstGrabIndexInCamGroup;//**[11/16/2017 ConanSteve]**:  每个检测组的第一个相机在相机组中的索引	
	array<CString, MAX_CAMER_COUNT> m_arr_sPathFileOfGrabInitInModelInfo;//**[9/6/2017 ConanSteve]**:  每个相机在ModelInfo文件夹中的配置文件路径																  
	array<CString, MAX_CAMER_COUNT> m_arr_sPathFileOfGrabInitInConfig;//**[9/6/2017 ConanSteve]**:  每个相机在Config文件夹中的配置文件路径
	array<shared_ptr<CGrabber>, MAX_CAMER_COUNT> m_arr_pGrabber; //**[8/30/2017 ConanSteve]**:  相机地址的数组	
	array<e_GrabType, MAX_CAMER_COUNT> m_arr_iGrabType;//**[12/12/2017 ConanSteve]**:  单个相机的相机类型
	array<CGrabInfo, MAX_CAMER_COUNT> m_arr_GrabInfo;//**[9/11/2017 ConanSteve]**:  系统关于相机参数结构体：变量用于绑定参数设置模块
	array<CImageInformation, MAX_CAMER_COUNT> m_arr_sCameraImageInfo;//**[8/30/2017 ConanSteve]**:  图像信息,只提供给算法使用，注意与CGrabInfo的区别， CGrabInfo只用于系统和相机的交互
	array<TCHAR[MAX_PATH], MAX_CAMER_COUNT> m_arr_lpszPathOfSaveImg;//**[11/22/2017 ConanSteve]**:  相机存图路径， 用TCHAR数组不用CString主要是为了方便多对象拷贝地址，而达到信息传递，接口不建议使用CString*类型，因为VS不同版本不兼容
	array< unique_ptr <BYTE[] >, MAX_CAMER_COUNT> m_arr_pbtBufferOfBayerImg;//**[9/6/2017 ConanSteve]**:  Bayer变换后的图像数据地址/翻转后的图像数据地址：主要用于系统自己显示用
	array< unique_ptr <BYTE[]>, MAX_CAMER_COUNT> m_arr_pbtBufferOfGrabImageCopy;//**[1/26/2018 ConanSteve]**:	对相机图像进行拷贝，防止配置文件错误，导致多对象拷贝图像地址越界 
	array<shared_ptr<BITMAPINFO>, MAX_CAMER_COUNT> m_arr_pBitmapInfo;
	array<CBayerTransform, MAX_CAMER_COUNT> m_arr_BayerTransform;
	
	/*************************************<检测组>************************************[12/28/2017 ConanSteve]*/
	
	array<VecInt,MAX_CAMER_COUNT> m_arr_veciGrabSNOfEachCheckGroup;//**[11/27/2017 ConanSteve]**:  每个检测组包含的相机序列号。
	map<int, int> m_map_CheckSN2KickSN;//**[12/28/2017 ConanSteve]**:  检测组到所在剔废组的映射

	array<bool, MAX_CHECK_GROUP_NUM> m_arr_bEnableSaveImgOfEachCheckGroup;//**[11/23/2017 ConanSteve]**:  每工位是否存图
	array<int, MAX_CHECK_GROUP_NUM> m_arr_iCheckCntOfCheckGroup;//**[12/5/2017 ConanSteve]**:  每检测组产品计数
	array<int, MAX_CHECK_GROUP_NUM> m_arr_iGoodCntOfCheckGroup;//**[12/5/2017 ConanSteve]**:  每个检测组好品计数
	array<int, MAX_CHECK_GROUP_NUM> m_arr_iBadCntOfCheckGroup;//**[12/1/2017 ConanSteve]**:  每检测组检测缺陷总数
	array<int, MAX_CHECK_GROUP_NUM> m_arr_iAlgExceptionCntOfCheckGroup;//**[12/5/2017 ConanSteve]**:  每工位算法异常计数
	array< map<int, s_DefectInfo>, MAX_CHECK_GROUP_NUM> m_arr_mapDefectStatistics;//**[12/1/2017 ConanSteve]**:  缺陷统计
	array<bool, MAX_CHECK_GROUP_NUM> m_arr_bEnableExecCheckOfEachGroup;//**[12/14/2017 ConanSteve]**:  每个检测工位是否执行真正的检测，如果不检测，则算法直接返回好品
	array<CExtenInfo, MAX_CHECK_GROUP_NUM> m_arr_ExtenInfo;//**[11/22/2017 ConanSteve]**:  扩展信息结构体
	
//	array<CMultiObjSaveImgPara, MAX_CHECK_GROUP_NUM> m_arr_MultipleObjSaveImgPara;

	/*************************************<剔废组>************************************[12/28/2017 ConanSteve]*/
	int m_iKickQueueNum{ 0 };//**[11/27/2017 ConanSteve]**:  剔废组数
	map<int, int> m_map_KickSN2AssemblySN;//**[12/28/2017 ConanSteve]**:  踢废队列号到流水线号的映射

	array<e_KickType, MAX_KICK_QUEUE_NUM> m_arr_eKickType;	//**[12/28/2017 ConanSteve]**:  剔废类型
	array<VecInt, MAX_KICK_QUEUE_NUM> m_arr_veciCheckSNInKickQueue;//**[11/27/2017 ConanSteve]**:  每个剔废组包含的检测组序列号。
	array<s_KickCountInfo, MAX_KICK_QUEUE_NUM> m_arr_sKickCntInfoOfKickQueue;//**[12/1/2017 ConanSteve]**:  每个剔废队列的剔废计数(数据来源于接口卡实际执行了踢废动作的踢废技术)
	array<int, MAX_KICK_QUEUE_NUM> m_arr_iGoodCntOfKickQueue;//**[1/24/2018 ConanSteve]**:	每个踢废队列回调时计算的好品个数
	array<bool, MAX_KICK_QUEUE_NUM> m_arr_bEnableKickOfKickQueue;

	/*************************************<流水线组>************************************[12/28/2017 ConanSteve]*/ 
	int m_iAssemblyLineNum{ 1 };//**[12/5/2017 ConanSteve]**:  流水线数，即产品通道数
	int m_iCheckSpeedUpdateInterval{ 1000 };//**[12/28/2017 ConanSteve]**:  更新流水线速度显示的周期
	array<VecInt, MAX_ASSEMBLYLINE_NUM> m_arr_veciKickSNInAssemblyLine;//**[12/28/2017 ConanSteve]**:  每道流水线中的剔废组号
	array<VecInt, MAX_ASSEMBLYLINE_NUM> m_arr_veciCheckGroupIndexOfAsseblyLine;//**[12/5/2017 ConanSteve]**:  每道流水线的检测组索引

	array<int, MAX_ASSEMBLYLINE_NUM>    m_arr_iCheckCntOfAssembly;//**[12/28/2017 ConanSteve]**:  每道流水线的产品计数
	array<int, MAX_ASSEMBLYLINE_NUM>    m_arr_iCheckErrorCntOfAssembly;//**[12/28/2017 ConanSteve]**:  每道流水线的坏品计数
	array<int, MAX_ASSEMBLYLINE_NUM>	m_arr_iCheckGoodCntOfAssembly;//**[12/28/2017 ConanSteve]**:  每道流水线的好品计数
	array<int, MAX_ASSEMBLYLINE_NUM>	m_arr_iCheckSpeedOfAssembly;//**[12/28/2017 ConanSteve]**:  每道流水线的检测速度
	array<double, MAX_ASSEMBLYLINE_NUM>	m_arr_lfGoodRatioOfAssembly;//**[1/24/2018 ConanSteve]**:	每道流水线的检测速度 
	array<queue<int>, MAX_ASSEMBLYLINE_NUM> m_arr_queCheckCntPerInterval;//**[12/28/2017 ConanSteve]**:  每道流水线历史检测产品计数，最长保持60个
	/*************************************<日志>************************************[12/13/2017 ConanSteve]*/

	//**[8/25/2017 ConanSteve]**:  调试信息日志
	CLogFile m_fileDebugInfo;
	CLogFile m_fileErrorInfo;

	//**[12/13/2017 ConanSteve]**:  运行日志
	bool m_bEnableLog{ false };//**[12/13/2017 ConanSteve]**:  是否记录日志
	unique_ptr<CLogFile[]> m_p_fileImgCntOfGrabInRegister;//**[12/13/2017 ConanSteve]**:  图像寄存器变化日志
	unique_ptr<CLogFile[]> m_p_fileGrabCallback{ nullptr };//**[12/13/2017 ConanSteve]**:  相机回调日志
	array<CLogFile, MAX_KICK_QUEUE_NUM> m_arr_fileKickCallback;//**[12/15/2017 ConanSteve]**:  剔废回调日志
	unique_ptr<CLogFile[]> m_p_fileKickCntInfoChange{ nullptr };//**[12/15/2017 ConanSteve]**:  剔废信息发生变化日志

	/*************************************<临界区资源访问锁>************************************[12/14/2017 ConanSteve]*/
	CCriticalSection m_csGetCount_TuXiangHao;

	/*************************************<离线分析工具通讯事件>************************************[12/28/2017 ConanSteve]*/
	HANDLE m_hEvent_DemoMessage_SendCommandOfLoadModel{ NULL };//**[1/7/2018 ConanSteve]**:	接受读模板事件 
	HANDLE m_hEvent_DemoMessage_ReceiveSuccessOfLoadModel{ NULL };//**[1/7/2018 ConanSteve]**:	发送加载成功消息 
	HANDLE m_hEvent_DemoMessage_ReveiveFailureOfLoadModel{ NULL };//**[1/7/2018 ConanSteve]**:	发送加载失败消息 

/************************************************************************/

private://**[8/24/2017 ConanSteve]**:  私有函数


public://**[8/24/2017 ConanSteve]**:  公有函数
	//**[8/24/2017 ConanSteve]**:  读取配置文件
	bool ReadCfg();
	//**[8/24/2017 ConanSteve]**:  初始化系统
	bool InitializeSystem();
	//**[8/24/2017 ConanSteve]**:  读取系统UI文件
	bool ReadCfgOfUI();
	//**[8/24/2017 ConanSteve]**:  释放系统内存
	bool ReleaseMemory();
	afx_msg void OnClose();
	//**[8/25/2017 ConanSteve]**:  读取算法配置文件
	bool ReadCfgOfAlg();
	//**[8/25/2017 ConanSteve]**:  读取检测端配置文件, CheckSystemConfig.ini
	bool ReadCfgOfSys();
	//**[8/25/2017 ConanSteve]**:  拷贝相机配置文件
	void CheckCamCfgFile();
	//**[8/25/2017 ConanSteve]**:  初始化接口卡
	CRetInfo InitIOCard();
	//**[8/25/2017 ConanSteve]**:  后来初始化进程函数
	DWORD static ThreadInitBackstage(PVOID pParam);
	//**[8/25/2017 ConanSteve]**:  后来初始化实现函数
	DWORD ThreadInitBackstageFunc(int iFunc =0);
	//**[8/25/2017 ConanSteve]**:  初始化相机
	CRetInfo InitGrabber();
	//**[8/25/2017 ConanSteve]**:  初始化数据库
	CRetInfo InitDataBase();
	//**[8/25/2017 ConanSteve]**:  分配内存
	bool AllocateMemory();
	//**[8/30/2017 ConanSteve]**:  相机回调函数
	static void WINAPI AreaGrabOverCallback(const s_GBSIGNALINFO* SignalInfo);
	//**[8/31/2017 ConanSteve]**:  初始化日志系统
	bool InitLogSys();
	//**[9/4/2017 ConanSteve]**:  开始检测
	bool StartCheck();
	//**[9/4/2017 ConanSteve]**:  关闭/暂停检测
	bool StopCheck();
	//**[9/5/2017 ConanSteve]**:  系统实时运行信息
	//**[9/5/2017 ConanSteve]**:  bHint:是否前台显示
	typedef struct {
		bool bHint{ false };
		CString sMsg{_T("")};
	}s_HintMsg;
	//**[12/8/2017 ConanSteve]**:  将信息显示在状态栏，bHint==true表示需要在窗口前端显示
	//**[12/8/2017 ConanSteve]**:  bErrorInfo==true表示信息需要写进系统错误日志中
	bool AddMsg(CString sMsg, bool bHint=false, bool bErrorInfo = false);
	//**[9/5/2017 ConanSteve]**:  相机回调
	void AreaGrabOverCallbackFunc(const s_GBSIGNALINFO* SigInfo);
	bool CameraGrabFuncSingle(int iCameraSN);
	bool WriteCfg();
	bool OnBtnClickOpenDemo();
	//**[9/11/2017 ConanSteve]**:  打开相机参数设置
	bool OnBnClickOpenParamSet();
	//**[9/11/2017 ConanSteve]**:  释放资源
	bool ReleaseResource();
	//**[9/11/2017 ConanSteve]**:  检查接口卡配置文件
	int CheckIOCardCfgFile();
	inline bool JudgeGrabCircleNew(int iCameraSN,int  iAllElementSN,int& iGrabCircle,int& iPicSN);
	CRetInfo InitMultiObject();

	//**[10/30/2017 ConanSteve]**:   剔废回调	
	static void WINAPI GlobalKickCallbackFunc(const s_KickParam* pKickParam);
	void KickCallbackFunc1(const s_KickParam* pKickParam);

	//**[10/30/2017 ConanSteve]**:   状态回调
	static void WINAPI GlobalStatusCallbackFunc(const s_Status* sStatusParam);
	void StatusCallbackFunc1(const s_Status* sStatusParam);
	int SaveBmp(const TCHAR* pPathName, int iWidth, int iHeight, int iByte, TBYTE *pDest);
	//**[12/6/2017 ConanSteve]**:  将系统使用的Bayer变换算法对应的枚举值转换成符合HalconBayer变换的索引值
	e_BayerHalcon TranslateToHalconBayer(int iBayerType);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	bool ResizeLayout();//**[12/4/2017 ConanSteve]**:  初始化相机之后，根据相机大小重新调整必要的布局（图像位置，必须知道）
	//**[12/5/2017 ConanSteve]**:  系统计数清零
	bool ResetSystem(int iType = 7);//**[12/28/2017 ConanSteve]**:  iType = 1只重置计数， 2只重置接口卡，4只重置多对象
	//**[12/8/2017 ConanSteve]**:  打开接口卡工具
	bool OnBtnClickOpenIOCardTools();

	
	BOOL GetCount_Guangdian();// 获取光电计数
	BOOL GetCount_Tifei();// 获取剔废计数
	BOOL GetCount_TuXiangHao();// 获取图像号寄存器计数

	bool OnBtnClickOpenDefectStatistics();//**[12/27/2017 ConanSteve]**:  打开缺陷统计
	inline bool StartAllThread();
	//**[12/28/2017 ConanSteve]**:   获取计数线程
	static DWORD GetIOCardImageNumThread(LPVOID lpParam);
	DWORD GetIOCardImageNumThreadFunc();
	static DWORD WaitLoadModelThread(LPVOID lpParam);
	DWORD WaitLoadModelThreadFunc();
	BOOL StartCameraGrab(int iGrabSN);
	CRetInfo LoadModel(CString sModelName = _T(""));//**[1/7/2018 ConanSteve]**:	加载模板 
	BOOL SetLutTable(double dRatioR, double dRatioG, double dRatioB);
	int MirrorSingleImage(BYTE* pImageBuff, int lImageWidth, int lImageHeight, int lImageBitCount, e_ShowModeFlip  eShowMode);
	bool OnBtnClickOpenDataStatistics();
};


