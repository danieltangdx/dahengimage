
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the IOCARD_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// IOCONTROL_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef IOCARD_EXPORTS
#define IOCONTROL_API __declspec(dllexport)
#else
#define IOCONTROL_API __declspec(dllimport)
#endif

// This class is exported from the IoCard.dll

#ifndef INCLODE_IOCARD_6356
#define  INCLODE_IOCARD_6356

#define		BOARDTYPE_3S500		0
#define		BOARDTYPE_LX45		1

typedef	struct _CTRLOUT_RESULT {
	int		nline;
	int		nIndex;
	BYTE	mOP;
	bool	mNeedDelay;
	int		mDelay;//0-127
}CTRLOUT_RESULT,*PCTRLOUT_RESULT;


typedef	struct _CTRLOUT_RESULT_INDEP {
	int		nline;
	int		nIndex;
	int		nResult;
}CTRLOUT_RESULT_INDEP,*PCTRLOUT_RESULT_INDEP;

typedef struct  _CTRL_TIMEREG_LX45
{
	DWORD	m_timeCnt10us;
	
	WORD	m_HCntRem0;
	WORD	m_Fcnt0;
	DWORD	m_HCntAll0;

	WORD	m_HCntRem1;
	WORD	m_Fcnt1;
	DWORD	m_HCntAll1;

	WORD	m_HCntRem2;
	WORD	m_Fcnt2;
	DWORD	m_HCntAll2;

	WORD	m_HCntRem3;
	WORD	m_Fcnt3;
	DWORD	m_HCntAll3;
}CTRL_TIMEREG_LX45,*PCTRL_TIMEREG_LX45;

typedef struct  _CTRL_TIMEREG
{
	DWORD	m_timeCnt10us;
	DWORD	m_HCntAll1;
	DWORD	m_HCntAll2;
	int		m_HCntRem1;
	int		m_HCntRem2;
	int		m_Fcnt1;
	int		m_Fcnt2;
}CTRL_TIMEREG,*PCTRL_TIMEREG;


#define		EN_FPGATIMEINT		0x02
#define		EN_INPUTINT			0x04
#define		EN_C51INT			0x01


// 回调函数参数结构体
typedef struct IOParam
{
	PVOID pContext;			// 存放this指针
	int iParam1;			// 包含队列号及计数值
	int iParam2;			// 包含时间信息
	int iMsgType;			// 消息类型
	
	int iReserve1;			// 保留字1
	int iReserve2;			// 保留字2
	int iReserve3;			// 保留字3
	int iReserve4;			// 保留字4
	
}s_IOParam;
typedef void (WINAPI* PIOCALLBACK)(const s_IOParam* pParam);
// 初始化参数结构体
typedef struct InitParam
{
	PVOID pContext;			// this指针
	char* strPath;			//初始化路径
	PIOCALLBACK pFunc;		//回调函数
	
	int iReserve1;			// 保留字1
	int iReserve2;			// 保留字2
	int iReserve3;			// 保留字3
	int iReserve4;			// 保留字4	
}s_InitParam;

class __declspec(dllexport) CioControl
{
	BOOL  ReadWorkMode2Param(const char* strPath);
	BOOL  ReadParameterFile_NewLX45(const char* strPath);
	BOOL  ReadParameterFile_OLDS500(const char* strPath);
	void  SetCodeFramelg_LX45();
public:
	CioControl();
	virtual ~CioControl();

	void* m_pData;
	int		mWork_Mode;
	int		mCntVOutDelay;
	int		mKeepExBase_t;
	int		mKeepExBase_h;
	int		mRunType1;
	int		mRunType2;
	int		mCode1dsType;
	int		mCode2dsType;
	int		mCode1hMax;
	int		mCode2hMax;
	int		mCodehNum[4];//LX45四个编码器的每帧行数
	DWORD   mInCoeOut;//输入与输出级联设置
	DWORD   mEnInIntFlag;//输入与输入延迟中断使能
	DWORD   mInIntDelayms;//输入延迟中断延迟时间
	BOOL    m_bOpenCardOK;//开卡正常
	s_InitParam* m_pInitParam;//初始化结构体
	BOOL	m_bRejectTest;//踢费测试
	int		m_nRejectQueue;
	int		m_nRejectOutPort;
	int		m_nRejectMode;
	CString m_strInitFile;
	BOOL    m_bSetParamInternal;
	CRITICAL_SECTION cs;// 多线程读输出加锁

	//设备控制
	BOOL Open(int mDev,HWND hWnd=NULL);
	BOOL SetMsgWnd(HWND hWnd=NULL);
	BOOL Close();
	BOOL IsOpen();
	int  ReadBoardNo();//读板卡号
	//设备与软件版本信息
	int  ReadBoardInfo(char *sysVer);//读板卡服务的硬件系统版本信息
	int  GetSoftVer(char *SoftVer);//读板卡服务的软件版本信息:DLL/LIB
	int  GetHardVer(char *HardVer);//读板卡服务的固件版本信息:FPGA/C51/驱动
	
	BOOL  WriteBoardNo(int nbd);//写板卡号
	BOOL  WriteBoardVer(char *sysVer);//写板卡服务的系统版本信息,推荐解析方法


	//队列输出控制方式 : 统一控制，每个队列的只有一个输出控制参数，不独立,主要用于一个队列同时控制多个控制输出（执行效率较高）
	BOOL SetResult(int nChannel,int nIndex,int nResult,int delay=0);//每次控制一个输入产品索引
	BOOL SetResultEx(int num,PCTRLOUT_RESULT pRes);//每次控制N个输入产品索引
	//队列输出控制方式 : 独立控制，每个队列的各个输出控制参数独立控制，主要用于一个队列独立控制多个控制输出（执行效率较低，C51更多的控制参数接收与解析）
	BOOL SetOutCtrlByResult(int nChannel,int nIndex,int lineoutpout,int nResult);//每次只控制一个输入产品索引的一个输出
	BOOL SetOutCtrlByResultEx(int nChannel,int nIndex,int nResult);//每次控制一个输入产品索引的M个输出
	BOOL SetOutCtrlByResultEx2(int num,PCTRLOUT_RESULT_INDEP pCtrl);//每次控制N个输入产品索引的N*M个输出
	
	//读队列索引\输入\输出值
	int ReadInputIndex(int nChannel);//读队列索引值8位
	int ReadInputCount(int inputn);//读输入计数值32位

	int ReadOutputIndex(int nChannel,int nPos);//按读队列配置顺序读输出索引值8位
	int ReadOutputCount(int nChannel,int nPos);//按读队列配置顺序读输出计数值32位
	int ReadOutputCtrlCnt(int nChannel,int nPos);//根据队列位置读读输出索引值32位
	int ReadOutputBTCnt(int outputn,int nPos);//按读队列配置顺序读输出补踢计数器
	int ReadOutputCount(int outputn);//按物理端口号直接读输出计数值32位
	int ReadOutputCtrlCnt(int outputn);//按物理端口号直接读输出索引值32位
	int ReadOutputBTCnt(int outputn);//按物理端口号直接读输出补踢计数器
	//读延迟队列索引\输入\输出值
	int ReadInputdelayIndex(int nChannel);//读队列延迟输入索引值8位
	int ReadOutputdelayIndex(int nChannel,int nPos);//按读队列配置顺序读延迟输出索引值8位

	int ReadInPortdelayCount(int ninPort,int nDelay);//按物理端口号直接读延迟输入计数值8位,同时设置延迟
	int ReadOutputdelayCount(int noutPort,int nDelay);//按物理端口号直接读延迟输出计数值8位,同时设置延迟

	BOOL ReadParameterFile(s_InitParam* pParam);
	BOOL ReadParameterFile(const char* strPath);//输入参数配置初始化状态,并启动工作
//	BOOL WriteParameterFile(const char* strPath);
	
	BOOL IOWrite(int addr,BYTE mval);
	BOOL IORead(int addr,BYTE *pval);

	//扩展函数
	//得到输入电平状态
	BOOL ReadInPortStatus(BYTE &inPortReg);
	//收到中断事件,读取输入及其延迟变化状态和发生时间
	BOOL ReadInChanged(UCHAR &instatus,UCHAR &indelaystatus,DWORD &time2uscnt);

	//启动运行,needReset=TRUE需要复位，内部执行ResetALL()
	BOOL StartWork(BOOL needReset=TRUE);
	//停止运行,阻断所有输入操作和队列操作
	BOOL StopWork(BOOL cutALL=TRUE);
	//重新复位初始化,复位C51控制参数、FPGA队列状态、所有计数器
	BOOL ResetALL();
	//重新复位所有计数器
	BOOL ResetCount();
	//暂停输入控制计数	
	BOOL PauseInCntWork(int nPortIn);
	//继续运行输入计数
	BOOL ContinueInCntWork(int nPortIn);
	//读入当前时间参数,旧函数
	BOOL GetAllTmParam(PCTRL_TIMEREG nowTmParam);
	//读入当前时间参数,兼容旧函数,不建议使用
	BOOL GetAllTmParam(PCTRL_TIMEREG_LX45 nowTmParam);
	//读入时间/编码器计数
	BOOL GetTm2us(DWORD &nowTm2us);
	BOOL GetTmCoder(int coder_n,DWORD &nowhAll,DWORD &nowf,DWORD &nowh);
	BOOL GetTmCoder(int coder_n,DWORD &nowhAll);
	BOOL GetTmCoder(int coder_n,DWORD &nowf,DWORD &nowh);

	//读入输入/编码器速率
	BOOL GetRate(int port,float &rate);

	// 设置输出状态 nOutport 实际out口，0-23；
	BOOL SetOutIOStatus(int nOutport, BOOL bStatus);

	//显示对话框
	void ShowDialog();
};

#endif

IOCONTROL_API int fnIoControl(void);