#pragma once

#ifndef MAX_BUF_LEN
#define MAX_BUF_LEN  1024
#endif // MAX_BUF_LEN


enum e_IOCardType {
	e_IOCardType_NONE,//**[12/8/2017 ConanSteve]**:  无卡
	e_IOCardType_SG,//**[12/12/2017 ConanSteve]**:  模拟接口卡
	e_IOCardType_PIOB,
	e_IOCardType_PIOC,//**[9/11/2017 ConanSteve]**:  C卡
	e_IOCardType_PCI_6356,//**[12/8/2017 ConanSteve]**:  PCI6356卡
	e_IOCardType_PCIE_6356,
	e_IOCardType_RS232_XZXNYN
};

/*************************************<写卡相关>************************************[9/13/2017 ConanSteve]*/
typedef struct s_sRsltInfoWritten
{
	int iKickSN;
	int iImgSN;
	int iRsltNum;
	bool arr_bKick[30];
	s_sRsltInfoWritten()
	{
		memset(arr_bKick, 0, sizeof(arr_bKick));
		iKickSN = 0;
		iImgSN = 0;
		iRsltNum = 0;
	}
	s_sRsltInfoWritten(bool* arr_bKick, int iKickSN, int iImgSN, int iRsltNum)
	{
		memcpy_s(this->arr_bKick, sizeof(this->arr_bKick), arr_bKick, sizeof(arr_bKick));
		this->iKickSN = iKickSN;
		this->iImgSN = iImgSN;
		this->iRsltNum = iRsltNum;
	}
}s_sRsltInfoWritten;





/*************************************<相机回调通知接口卡信息>************************************[9/13/2017 ConanSteve]*/
typedef struct s_NotifyInfoOfGrabCallback
{
	int iGrabSN{0};
	int iCallbackCnt{0};
	s_NotifyInfoOfGrabCallback() = default;
	s_NotifyInfoOfGrabCallback(int iGrabSN, int iCallbackCnt)
	{
		this->iGrabSN = iGrabSN;
		this->iCallbackCnt = iCallbackCnt;
	}
}s_NotifyInfoOfGrabCallback;
/*************************************<接口卡初始化信息>************************************[9/13/2017 ConanSteve]*/
class CIOCardInitInfo {
public:
	//**[12/13/2017 ConanSteve]**:  接口卡配置文件，与相机接线无关
	void* pContext;//**[12/15/2017 ConanSteve]**:  初始化接口卡的上下文，一般是窗口指针CWnd*
	void* pIOCallback;//**[12/15/2017 ConanSteve]**:  接口卡回调函数指针
	TCHAR lpszPathFileOfInit[MAX_PATH];//**[12/19/2017 ConanSteve]**:  初始化文件
	//**[12/13/2017 ConanSteve]**:  用于描述接口卡输出口和相机之间关系的描述文件，使接口卡能够正确处理系统读指定相机对应的图像号的命令请求
	TCHAR lpszPathFileOfConnection[MAX_PATH];


	CIOCardInitInfo()
	{
		memset(lpszPathFileOfInit, 0, sizeof(lpszPathFileOfInit));
		memset(lpszPathFileOfConnection, 0, sizeof(lpszPathFileOfConnection));
		pContext = NULL;
		pIOCallback = NULL;
	}
};

/*************************************<参数设置及获取相关>************************************[9/13/2017 ConanSteve]*/
enum class IOCardParaType {
	NONE,
	RefreshPeroid,//**[9/12/2017 ConanSteve]**:  继电器刷新周期:RS232
	FrameDelayOfKick,//**[9/12/2017 ConanSteve]**:  延迟写卡帧数:RS232
};
//**[9/13/2017 ConanSteve]**:  接口卡信息变量，主要用于绑定DHPropertyCridCtrl控件用
class CIOCardInfo {
public:
	int m_iRefreshPeroid{ 0 };
	int m_iFrameDelayOfKick{ 0 };
};
//**[9/13/2017 ConanSteve]**:  参数通信结构体,用于设置和读取接口参数，支持无限扩展
typedef struct s_IOCardParaInfo {
	IOCardParaType type;
	int* piVal{ nullptr };
	float* pfVal{ nullptr };
	bool* pbVal{ nullptr };
	s_IOCardParaInfo()
	{
		type = IOCardParaType::NONE;
	}
}s_IOCardParaInfo;
/*************************************<剔废计数信息结构体>************************************[12/13/2017 ConanSteve]*/
typedef struct s_KickCountInfo
{
	int iKickSN;//**[12/13/2017 ConanSteve]**:  接口卡自己的剔废队列号，注意与系统剔废队列号区分开来，一般单张卡的系统两者值相同
	int iKickPortNum{ 1 };//**[12/13/2017 ConanSteve]**:  //**[12/13/2017 ConanSteve]**:  此队列的剔废口个数，泡罩在线和口服液分色可能不为1
	int arr_iKickPortId[30];//**[12/13/2017 ConanSteve]**: 剔废队列的每个逻辑剔废口对应的实际接口卡物理端口值
	int arr_iKickPortCount[30];//**[12/13/2017 ConanSteve]**:  每个剔废口的计数
	int arr_iAutoKickCnt[30];//**[12/17/2017 ConanSteve]**: 补踢计数 
	bool arr_bIsBadKick[30];//**[12/28/2017 ConanSteve]**:  标记每个踢废口踢的是不是废品
	s_KickCountInfo() {
		iKickSN = 0;
		memset(arr_iKickPortCount, 0, sizeof(arr_iKickPortCount));
		memset(arr_iKickPortId, 0, sizeof(arr_iKickPortId));
		memset(arr_iAutoKickCnt, 0, sizeof(arr_iAutoKickCnt));
		memset(arr_bIsBadKick, 1, sizeof(arr_bIsBadKick));
	}
}s_KickCountInfo;
// **[12/13/2017 ConanSteve]**:  保存踢废队列到out口之间的信息，便于剔废查询使用
// typedef struct s_KickQueryDict {
// 	int iKickSN;//**[12/13/2017 ConanSteve]**:  剔废队列号
// 	int iKickPortNum;//**[12/13/2017 ConanSteve]**:  当前剔废队列的剔废口数目
// }s_KickQueryDict;
/*************************************<>************************************[12/13/2017 ConanSteve]*/
class CDHIOCard
{
public:
	CDHIOCard(){}
	virtual ~CDHIOCard() {}
	virtual bool InitCard(CIOCardInitInfo& rIOCardInitInfo) = 0;//**[9/4/2017 ConanSteve]**:  初始化接口卡
	virtual e_IOCardType GetIOCardType() =0;
	//**[9/4/2017 ConanSteve]**:  打开接口卡
	virtual bool OpenCard() { return true; };
	//**[9/4/2017 ConanSteve]**:  关闭接口卡
	virtual bool CloseCard() = 0;
	//**[12/13/2017 ConanSteve]**:  重置接口卡所有数据
	virtual bool ResetIOCard(){return true;}
	//**[9/4/2017 ConanSteve]**:  打开接口卡调试工具, pParent为空创建模态对话框，否则为非模态对话框
	virtual bool OpenTools(CWnd* pParent = NULL) { return true; }
	//**[11/28/2017 ConanSteve]**:  使能接口卡正常工作。包括队列和单个端口的使用
	virtual bool Enable(bool bEnable) = 0;
	//**[12/12/2017 ConanSteve]**:  使能所有队列，但是不影响单个端口的状态设置，（考虑到部分接口卡停止检测时，不希望队列触发，但是需要使报警灯常亮）
	virtual bool EnableQueue(bool bEnable) {return true;}
	//**[9/12/2017 ConanSteve]**:  获取接口卡参数
	virtual bool GetParam(s_IOCardParaInfo& para) {return false;};
	//**[9/12/2017 ConanSteve]**:  设置接口卡参数
	virtual bool SetParam(s_IOCardParaInfo& para) {return false;};
	//**[9/11/2017 ConanSteve]**:  写检测结果
	virtual bool WriteKickResult(s_sRsltInfoWritten & rslt) {return true;};
	//**[9/11/2017 ConanSteve]**:  相机回调通知,主要方便一些自己没有图像号寄存器的接口卡来计算图像号
	virtual bool NotifyCardOfGrabCallBack(s_NotifyInfoOfGrabCallback &notifyInfo) {return true;};
	//**[9/4/2017 ConanSteve]**:  获得错误信息
	virtual CString GetLastError() {return _T("");};

	/*************************************<参数读取和设置>************************************[12/13/2017 ConanSteve]*/
	//**[12/13/2017 ConanSteve]**:  获取光电计数
	virtual bool GetInputIndex(int iInputport, int &iValue) { return false; }
	//**[12/13/2017 ConanSteve]**:  读取指定相机号对应的图像号
	virtual bool GetGrabImgSN(int iGrabSN, int& iImageSN) { return false; }
	//**[12/13/2017 ConanSteve]**:  获取指定剔废队列对应的输出口计数
	virtual bool GetKickCountInfo(int iKickSN, s_KickCountInfo& sKickCntInfo) { return false; }
	//**[12/13/2017 ConanSteve]**:  设置接口卡输出口的状态值，更主要的原因是不同项目的插件需要设置接口卡输出口状态
	virtual bool SetOutIOStatus(int iOutport, bool bStatus) { return true; }
	//**[12/13/2017 ConanSteve]**:  获取指定out口的计数
	virtual bool GetOutputCount(int iOutport, int& iValue) { return 0; };
};
