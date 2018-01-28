#if !defined(AFX_CHECK_H__3D35A3B4_D261_4116_8EBD_714891F3AEF3__INCLUDED_)
#define AFX_CHECK_H__3D35A3B4_D261_4116_8EBD_714891F3AEF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STATUS_STRING_MAXLENGTH	256			//描述错误信息的最大字符数
//函数返回状态类型
#define STATUS_NOR	             0     //正常
#define STATUS_ILG			    1	   //参数非法
#define STATUS_PFW				 2	   //图像格式错误
#define STATUS_FOF               3     //文件操作失败
#define	STATUS_LLF	             4	   //加载license失败

#define STATUS_MOS				 5     //模板操作成功
#define STATUS_MOF				 6     //模板操作失败
#define STATUS_MOC				 7     //模板操作取消 

#define STATUS_TRAIN_FAIL        8     //训练失败
#define STATUS_FILE_OPERATE_FAIL 9	   //文件操作失败
#define STATUS_OTHER			 10	   //其它原因  
#define STATUS_THREAD_BUSY       11	   //所有线程忙
#define STATUS_FATAL_ERROR		 12    //算法崩溃
#define STATUS_BAD_INSPCT		 13	   //有错误的胶囊



//模板操作标志
#define READ_MODEL				0	//读取模板
#define NEW_MODEL				1	//新建模板
#define MODIFY_MODEL			2	//修改模板
#define DELETE_MODEL			3	//删除模板

/*剩下的以后慢慢补充*/
#define NUM_THREAD				12	//线程数
#define NUM_BL_CAM				4	//背光相机检测胶囊个数
#define THREAD_NUM_ILLEGAL		-1  //线程个数不合法


//所有工位 nCapSuleStatus
#define ONLY_INIT				-2	//初始化
#define EMPTY_SLOT				-1	//空位置
#define GOOD_CAPSULE			0	//好胶囊
#define DEFECT_FIND				1	//Sobel发现的缺陷
#define LOCATE_FAIL				2	//定位失败
#define BAD_INCISION			3	//切口缺陷
#define WRONG_SIZE				4	//尺寸错误

//第三工位
#define NO_CHECK_RECTS			0	//FrontLocate读模板时未找到检测框文件
#define FRONTLOCATE_PARA_ILL	-1	//FrontLocate训练出来的参数非法
#define MONO_CAPSULE			5	//单片
#define CROP_EXCEPTION			6	//crop_domain时出错
#define	WRONG_COLOR				7	//颜色错误
#define BAD_REFLECTION			8	//反光区错误

namespace Alg{
	
	//图像信息结构体类型
	typedef struct _ImageInfo
	{

	}s_ImageInfo;


	// 函数执行状态信息
	typedef struct _Status
	{
		int		nStatusType;									//返回状态类型
		char	chErrorInfo[STATUS_STRING_MAXLENGTH];			//错误信息描述
		char    chErrorContext[STATUS_STRING_MAXLENGTH];		//错误详细信息描述	
		void*   Empty;
	}s_Status;

	// 算法初始化参数
	typedef struct _AlgInitParam
	{
		int		nWidth[10];         //图像宽度
		int		nHeight[10];	    //图像高度
		CDC		*pDC[15];			//最大化窗口句柄  1,2,3,4,4.1,4.2,4.3,4.4,5,5.1,5.2,5.3,5.4
		int		nCameraNum[10];		//对象个数
	}s_AlgInitPara;

	typedef struct _AlgSingleInit
	{
		int		nWidth;         //图像宽度
		int		nHeight;	    //图像高度
		CDC		**pDC;			//窗口句柄
		int		nCameraIndex;	//相机工位号
		int		nObjIndex;		//第几个对象
	}s_AlgSingleInit;


	// 检测输入参数 algorithm check input parameter
	typedef struct _AlgInspectionInputParam
	{
		int		nWidth;			//图像宽
		int		nHeight;		//图像高
		int		nChannel;		//通道数			<0时 为Halcon读图的数据
		int		nCamIndex;		//相机号			
		char	*pcImageData;	//图像数据	
		int		bRunCheck;		//是否检测 0不检 1检
		int		nInSig;			//设置为0 !!!	2只显示图
		int		bDsp;			//0不显示 1全显示边缘 2显示错误图像边缘	101全显示填充 102错误填充
	}s_AlgInspInPara;

	//模板操作输入
	typedef struct _AlgModelInput
	{
		char	chModelName[255];
		int		nOperSig;
		/*
		-2显示模板窗口，-1隐藏模板窗口(外部点击tab页切换时使用)。0读模板，1系统打开DEMO开窗口，2双击应用打开DEMO开窗口，
		3删除模板, 4保存模板, 41用本地图像保存成模板图像, 5保存参数, 6移动界面
		51重新刷界面，52鼠标操作 sRst.nStatusType=0，53读完模板之后的显示操作
		101正常检测图，102只显示图，103当做模板图像，104相机取图测试，105询问是否在画
		202询问是否要保存 sRst.nStatusType=0即可
		203获取版本号
		300copyWindow
		1001-1500为各工位保留
		*/
		CWnd*	pParent;	//读、删、保存模板时赋NULL
		CRect	rect;
		bool	bTopWin;	//是否为最前窗口（模板窗口初始化时最前显示的一路选项卡）
	}s_AlgModelInput;


	// 检测输出参数 algorithm check output parameter
	typedef struct _CapsuleStatus
	{
		int		nStatus;
		char	chInfo[255];	//角度信息
	}s_CapsuleStatus;

	typedef struct _AlgInspectionReslut
	{
		int		nAlgStatus;		//0:好 1:有坏的 2:算法异常 -1:全空
		int     nErrorLink;		//0为不连瓶 1连瓶
		char	chExpInfo[255];
		s_CapsuleStatus		sCapules[20];
		void*   Empty;
	}s_AlgInspResult;

	// 检测输出参数 algorithm model parameter
	// SetAlgParam函数使用
// 	typedef struct _AlgModelPara
// 	{
// 		char	chModelName[MAX_PATH];
// 		int		nCamNumber;
// 		int		nOperateSig;//0读模板，1新建模板，2修改模板，3删除模板
// 	}s_AlgModelPara;

	//Rectangle
	typedef struct _CRect
	{
		int nLefttopR;
		int nLefttopC;
		int nMiddleR;
		int nMiddleC;
		int nRightBottomR;
		int nRightBottomC;
	}s_CRect;

	//Circle
	typedef struct _CCircle
	{
		double	dRadius;
		int		nRow;
		int		nColumn;
	}s_CCircle;

	// 检测输出参数 algorithm model parameter

	// 模板设置初始化信息
	typedef struct _AlgModelInit
	{		
	}s_AlgModelInit;

	//检测参数设置初始化信息
	typedef struct _AlgChecklInit
	{
   	}s_AlgCheckInit;

	// 算法信息结构体
	typedef struct _AlgInfo
	{
		CString DllVersion;	
	}s_AlgInfo;
}

#endif // !defined(AFX_CHECK_H__3D35A3B4_D261_4116_8EBD_714891F3AEF3__INCLUDED_)