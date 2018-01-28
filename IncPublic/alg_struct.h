#if !defined(AFX_CHECK_H__3D35A3B4_D261_4116_8EBD_714891F3AEF3__INCLUDED_)
#define AFX_CHECK_H__3D35A3B4_D261_4116_8EBD_714891F3AEF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STATUS_STRING_MAXLENGTH	256			//描述错误信息的最大字符数

//函数返回状态类型
#define STATUS_THREAD_BUSY			-1		//所有线程忙
#define STATUS_NOR					0		//正常
#define STATUS_ILG					1		//参数非法
#define STATUS_THROW_IMAGE			2		//检测队列已满，开始丢图[20160531 zhy]
#define STATUS_KICK_TIMEOUT			3		//检测超时 [20160531 zhy]
#define STATUS_ALG_ABNORMAL			4		//算法检测异常 [20160531 zhy]
#define STATUS_STOP_SAVEIMAGE		301		//停止保存图像 [20160531 zhy]

//消息
#define DHOA_MSG_CHECK_FINISH   WM_USER+4001
//#include "typeinfo"

enum e_BayerHalcon { //**[12/13/2017 ConanSteve]**:  1:GB,2:GR,3:BG,4:RG
	e_BayerHalcon_NONE =0,
	e_BayerHalcon_GB =1,
	e_BayerHalcon_GR = 2,
	e_BayerHalcon_BG = 3,
	e_BayerHalcon_RG = 4
};
//**[1/17/2018 ConanSteve]**:  检测端类型
enum e_CallType
{
	e_CallType_RealTime,
	e_CallType_Demo,
	e_CallType_Simulation
};
/*************************************<Demo使用结构体>************************************[1/17/2018 ConanSteve]*/
typedef struct s_DemoAlgSetParam
{
	TCHAR	chModelName[STATUS_STRING_MAXLENGTH];   //DEMO选图的路径。单击文件夹和单个图都发消息检测，如果只处理单张图算法需要屏蔽文件夹目录
	CWnd*	pParent;	                            //读、删、保存模板时赋NULL
	CRect	rect;
	bool	bTopWin;	                            //是否为最前窗口（模板窗口初始化时最前显示的一路选项卡）
	int     iCheckSN;                               //当前检测第几路。检测
	int iReserve1;//**[8/10/2017 ConanSteve]**:  新增的4个保留字
	int iReserve2;
	int iReserve3;
	int iReserve4;//**[8/10/2017 ConanSteve]**:  此保留字为专为扩展功能定义使用

	s_DemoAlgSetParam()
	{
		memset(chModelName, 0, sizeof(chModelName));
		pParent = NULL;
		rect = 0;
		bTopWin = false;
		iCheckSN = 0;
		iReserve1 = iReserve2 = iReserve3 = iReserve4 = 0;
	}

}s_DemoAlgSetParam;

/*************************************<四代算法接口>************************************[1/17/2018 ConanSteve]*/
namespace Alg
{
	// 函数执行状态信息
	typedef struct s_Status
	{	
		int  iCheckSN;                                      //多对象返回给检测端。检测路数，从0开始，无工位概念
		int	 iStatusType;									//返回状态类型
		TCHAR* pErrorInfo;									//错误信息描述
		void* pThis;										//存放拥有回调函数的对象的this指针
		void* pParam;                                       //特殊结构体
		s_Status()
		{
			iCheckSN = 0;
			iStatusType = STATUS_NOR;
			pErrorInfo = NULL;
			pThis = NULL;
			pParam = NULL;
		}
	}s_Status;

	// 剔废结构体
	typedef struct s_KickParam
	{
		void*	pThis;									//存放拥有回调函数的对象的this指针
		void*	pParam;									//存放剔废结果
		TCHAR*   pResult;								//存放返回结果描述

		int		iKickSN;								//剔废口编号
		int		iImageSN;								//图像号
		
		int		iReserve1;                              //保留字
		int		iReserve2;                              //保留字
		int		iReserve3;                              //保留字
		int     iReserve4;                              //保留字

		s_KickParam()
		{
			pThis = NULL;
			pParam = NULL;
			pResult = NULL;
			iKickSN = 0;
			iImageSN = 0;
			iReserve1 = iReserve2 = iReserve3 = iReserve4 = 0;
		}

		
	}s_KickParam;
	
	// 检测结束后的回调函
	typedef void (WINAPI *PKICKCALLBACK)(const s_KickParam* sKickParam);
	typedef void (WINAPI *PSTATUSCALLBACK)(const s_Status* sStatusParam);
	
	// 检测端与多对象初始化结构体
	typedef struct s_CheckInitParam
	{
		PKICKCALLBACK KickCallbackFunc;				//剔废回调
		PSTATUSCALLBACK StatusCallbackFunc;			//状态回调	301保存图像回调 302丢图回调	

		TCHAR* pFilePath;							//配置文件路径，用于读取动态库名称，对象个数等
		void* pThis;								//存放拥有回调函数的对象的this指针
		void* pParam;                               //特殊结构体，传入的是所有工位的数组指针(同时包含图像检测时，系统和算法交互参数)
		
		int iCallType;								//调用类型，0：检测端多对象现场调用，1：DEMO单对象调用	2:检测端多对象模拟调用

		int iReserve1;								//保留字		胶囊为0，药液为1
		int iReserve2;								//保留字
		int iReserve3;								//保留字
		int iReserve4;								//保留字

		s_CheckInitParam()
		{
			KickCallbackFunc = NULL;
			StatusCallbackFunc = NULL;
			pFilePath = NULL;
			pThis = NULL;
			pParam = NULL;
			iCallType = 0;
			iReserve1 = iReserve2 = iReserve3 = iReserve4 = 0;
		}
		
	}s_CheckInitParam;

	//多对象与算法初始化结构体
	typedef struct s_CheckInitSingleParam
	{		
		void* pParam;                   //特殊结构体

		int iCheckSN;                   //检测路数，0开始，无工位概念。算法建立参数文件夹用。
		int	iObjIndex;		            //当前调用的对象号，0开始。算法中记录当前第几个对象调用算法。
		
		int iReserve1;                  //保留字
		int iReserve2;                  //保留字
		int iReserve3;                  //保留字
		int iReserve4;                  //保留字

		s_CheckInitSingleParam()
		{
			pParam = NULL;
			iCheckSN = 0;
			iObjIndex = 0;
			iReserve1 = iReserve2 = iReserve3 = iReserve4 = 0;
		}
		
	}s_CheckInitSingleParam;

	// 检测输入参数
	typedef struct s_CheckInParam 
	{
		void* pParam;       //特殊结构体
		char* pImageData;	//图像数据地址

		int iImageSN;		//图像大张号，检测端传给多对象，可用于记录
		int iCheckSN;       /**Camera SN              [5/4/2017 ConanSteve]**/
		
		int	iReserve1;		//保留字
		int	iReserve2;		//保留字
		int iReserve3;      //保留字
		int iReserve4;      //保留字
		s_CheckInParam()
		{
			pParam = NULL;
			pImageData = NULL;
			iImageSN = 0;
			iCheckSN = 0;
			iReserve1 = iReserve2 = iReserve3 = iReserve4 = 0;
		}
	}s_CheckInParam;

	enum e_OptionalFunction
	{

	};

	

	// 检测输出参数
	typedef struct s_CheckOutParam 
	{	
		TCHAR*	pFilePath;								//存图路径 每个图像绝对路径以“\n”为分隔符；
		void*   pParam;                                 //特殊结构体       
		
		int     iCheckSN;                               //当前检测第几路。算法返回给检测端，结果是第几路。与算法无关。
		int		iImageSN;								//图像号
		int	    iDisplay;                               //0:不显示，1：显示; 多对象与算法之间的显示接口。算法通过整合显示模式，返回给多对象是否显示，当返回1时多对象调用算法CopyRectangle	
		int		iSave;									//是否保存图像 1存图， 2不存
		int     iImageNum;								//序列图像个数，单个相机采集序列图数量
		int		iPathLength;							//存图路径长度，与保存图像相关

		int		iReserve1;                              //保留字
		int		iReserve2;                              //保留字
		int		iReserve3;                              //保留字
		int     iReserve4;                              //保留字	

		s_CheckOutParam()
		{
			pFilePath = NULL;
			pParam = NULL;
			iCheckSN = 0;
			iImageSN = 0;
			iDisplay = 0;
			iSave = 0;
			iImageNum = 0;
			iPathLength = 0;
			iReserve1 = iReserve2 = iReserve3 = iReserve4 = 0;
		}
	}s_CheckOutParam;
	
	//模板操作输入
	typedef struct s_AlgModelInput
	{
		void*   pParam;                                 //检测端与算法,特殊结构体

		int     iCheckSN;                               //当前检测第几路。检测端传给多对象用于调用该路set。与算法无关。
		int		iOperSig;
		//系统与多对象  0读模板，1为设置检测参数(例如算法是否显示图像) 
		//多对象与算法。0读参数，300将虚窗口copyWindow到实窗口
		/*DEMO与算法：
		-2显示模板窗口，
		-1隐藏模板窗口(外部点击tab页切换时使用)。
		0读模板，
		1系统打开DEMO开窗口，
		2双击应用打开DEMO开窗口，
		3删除模板, 
		4保存模板, 
		5保存参数,
		6移动界面
		41用本地图像保存成模板图像,  
		51重新刷界面，
		52鼠标操作 sRst.nStatusType=0，
		53读完模板之后的显示操作
		101正常检测图，
		102只显示图，
		103当做模板图像，
		104相机取图测试，
		105询问是否在画
		202询问是否要保存 sRst.nStatusType=0即可
		203获取版本号
		300copyWindow
		301SaveImage
		1001-1500为各工位保留
		//**[12/26/2017 ConanSteve]**:  601:重置多对象计数
		*/
																
		int		iReserve1;                              //保留字		
		int		iReserve2;                              //保留字		
		int		iReserve3;                              //保留字     
		int     iReserve4;                              //保留字
		s_AlgModelInput()
		{
			pParam = NULL;
			iCheckSN = 0;
			iOperSig = 0;
			iReserve1 = iReserve2 = iReserve3 = iReserve4 = 0;
		}
	}s_AlgModelInput;
}

enum e_AlgModelInputOper {
	e_AlgModelInputOper_LoadModel = 0,//**[10/9/2017 ConanSteve]**:  系统调用读模板
	e_AlgModelInputOper_OpenWindows = 1,//**[10/9/2017 ConanSteve]**:  Demo调用打开窗口
	e_AlgModelInputOper_MoveWindow = 6,//**[10/18/2017 ConanSteve]**:  Demo调用，移动窗口
	e_ALgModelInputOper_ShowAlgModelWnd = 101,//**[10/9/2017 ConanSteve]**:  Demo调用，显示算法 窗口
	e_AlgModelInputOper_HideAlgModelWnd = 102,//**[10/18/2017 ConanSteve]**:  Demo调用，隐藏算法 窗口
	e_AlgModelInputOper_GetVersion = 203, //**[10/18/2017 ConanSteve]**:  获取算法版本
	e_AlgModelInputOper_CopyWindow = 300,//**[10/9/2017 ConanSteve]**:  算法拷贝虚窗口信息到实窗口
	e_AlgModelInputOper_SaveImage = 301,//**[7/26/2017 ConanSteve]**:  保存图像
	e_AlgModelInputOper_Reset = 601,//**[12/26/2017 ConanSteve]**:  重置多对象计数
	e_AlgModelInputOper_ShowRunInfo = 602,//**[1/17/2018 ConanSteve]**:  显示多对象运行信息窗口
	e_AlgModelInputOper_UseExtenInfoFunc = 666,//**[7/26/2017 ConanSteve]**:  使用扩展功能结构体
};

/*****************************************************************************************/

#endif // !defined(AFX_CHECK_H__3D35A3B4_D261_4116_8EBD_714891F3AEF3__INCLUDED_)