#pragma once
#include "DHIOCard.h"
#include "ExFn.h"
#include "..\\inc\\IoCard.h"
#pragma comment(lib, "..\\lib\\iocard.lib")
#include "map"
#include "ExFn.h"
using namespace std;


#define  MAX_OUTPORT_NUM 24

class CDHIOCardForPCI6356Interior:public CDHIOCard
{
public:
	CDHIOCardForPCI6356Interior();
	virtual ~CDHIOCardForPCI6356Interior();
	//**[9/4/2017 ConanSteve]**:  初始化接口卡
	virtual bool InitCard(CIOCardInitInfo& rIOCardInitInfo)override;
	virtual e_IOCardType GetIOCardType() override;
	//**[9/4/2017 ConanSteve]**:  打开接口卡
	virtual bool OpenCard() override;
	//**[9/4/2017 ConanSteve]**:  关闭接口卡
	virtual bool CloseCard() override;
	//**[12/13/2017 ConanSteve]**:  重置接口卡所有数据
	virtual bool ResetIOCard() override;
	//**[9/4/2017 ConanSteve]**:  打开接口卡调试工具
	virtual bool OpenTools(CWnd* pParent = NULL) override;
	//**[11/28/2017 ConanSteve]**:  使能接口卡正常工作。包括队列和单个端口的使用
	virtual bool Enable(bool bEnable) override;
	//**[12/12/2017 ConanSteve]**:  使能所有队列，但是不影响单个端口的状态设置，（考虑到部分接口卡停止检测时，不希望队列触发，但是需要使报警灯常亮）
	virtual bool EnableQueue(bool bEnable) override;
	//**[9/12/2017 ConanSteve]**:  获取接口卡参数
	virtual bool GetParam(s_IOCardParaInfo& para) override;
	//**[9/12/2017 ConanSteve]**:  设置接口卡参数
	virtual bool SetParam(s_IOCardParaInfo& para) override;
	//**[9/11/2017 ConanSteve]**:  写检测结果
	virtual bool WriteKickResult(s_sRsltInfoWritten & rslt) override;
	//**[9/11/2017 ConanSteve]**:  相机回调通知,主要方便一些自己没有图像号寄存器的接口卡来计算图像号
	virtual bool NotifyCardOfGrabCallBack(s_NotifyInfoOfGrabCallback &notifyInfo) override;
	//**[9/4/2017 ConanSteve]**:  获得错误信息
	virtual CString GetLastError() override;

	/*************************************<参数读取和设置>************************************[12/13/2017 ConanSteve]*/
	//**[12/13/2017 ConanSteve]**:  获取光电计数
	virtual bool GetInputIndex(int iInputport, int &iValue) override;
	//**[12/13/2017 ConanSteve]**:  读取指定相机号对应的图像号
	virtual bool GetGrabImgSN(int iGrabSN, int& iImageSN) override;
	//**[12/13/2017 ConanSteve]**:  获取指定踢费队列对应的输出口计数
	virtual bool GetKickCountInfo(int iKickSN, s_KickCountInfo& sKickCntInfo) override;
	//**[12/13/2017 ConanSteve]**:  设置接口卡输出口的状态值，更主要的原因是不同项目的插件需要设置接口卡输出口状态
	virtual bool SetOutIOStatus(int iOutport, bool bStatus) override;
	//**[12/13/2017 ConanSteve]**:  获取指定out口的计数
	virtual bool GetOutputCount(int iOutport, int& iValue) override;

private:

	//**[12/15/2017 ConanSteve]**:   接口卡回调函数	
	static void WINAPI GlobalIOCallback(const s_IOParam* SigInfo);
	void GlobalIOCallbackFunc(const s_IOParam* SigInfo);	// 1工位第一个相机的回调函数

private:
	CString m_sLastErrorInfo{ _T("") };
	CioControl m_IOCard;
	int m_iDevSN{ 0 };//**[11/30/2017 ConanSteve]**:  接口卡设备号
	bool m_bInitSuccess{ false };//**[12/15/2017 ConanSteve]**:  接口卡是否初始化成功
	bool m_bNeedReadCardParam{true};//**[12/18/2017 ConanSteve]**:  是否需要读取参数
	CIOCardInitInfo m_IOCardInitInfo;//**[12/15/2017 ConanSteve]**:  相机初始化对象
	CINICfg m_iniCfgOfInit;
	CINICfg m_iniCfgOfConnection;
	bool m_bEnableLog{ false };
	array<CLogFile, MAX_OUTPORT_NUM> m_arr_fileKick;

	

	/*************************************<Connection>************************************[12/18/2017 ConanSteve]*/
	int m_iGrabNum{1};//**[12/18/2017 ConanSteve]**:  系统相机个数
	int m_iKickQueueNum{ 1 };//**[12/18/2017 ConanSteve]**:  接口卡踢费队列个数
	bool m_bEnableWorking{ false };//**[12/18/2017 ConanSteve]**:  是否正在工作
	array<s_KickCountInfo, MAX_OUTPORT_NUM> m_arr_sKickCntInfo;//**[12/13/2017 ConanSteve]**:  踢费计数信息
	map<int, int> m_map_SysKickSN2CardKickId;//**[12/18/2017 ConanSteve]**:  系统踢费队列号到接口卡踢费队列编号的字典，主要满足一个系统多个接口卡的需求，例如C卡

	map<int,int> m_map_OutportOfGrabRelated;//**[12/18/2017 ConanSteve]**:  每个相机号对应的触发out口
	typedef struct s_OutputCtrlInfo{
		int iChannel;//**[12/18/2017 ConanSteve]**:  队列号
		int iLogicalOutport;//**[12/18/2017 ConanSteve]**:  队列逻辑端口号
	}s_OutputCtrlInfo;
	map<int, s_OutputCtrlInfo> m_map_GrabSN2LogicalOutport;//**[12/18/2017 ConanSteve]**:  相机号到逻辑端口号的映射

// 	typedef struct s_KickKeyName
// 	{
// 		int iKickSNOfIOCard;//**[12/18/2017 ConanSteve]**:  接口卡的踢费队列号,一般就是队列号
// 		int iKickIndexOfCurQueue;//**[12/18/2017 ConanSteve]**:  当前踢费队列的踢费索引
// 		s_KickKeyName() 
// 		{ 
// 			iKickSNOfIOCard = 0; 
// 			iKickIndexOfCurQueue = 0;
// 		}
// 		s_KickKeyName(int iKickSNOfIOCard, int iKickIndexOfCurQueue)
// 		{
// 			this->iKickSNOfIOCard = iKickSNOfIOCard;
// 			this->iKickIndexOfCurQueue = iKickIndexOfCurQueue;
// 		}
// 	}s_KickKeyName;
	typedef struct s_KickKeyValue
	{
		int iChannel;//**[12/18/2017 ConanSteve]**:  对应Out口所在的通道号
		int iLogicalOutport;//**[12/18/2017 ConanSteve]**:  对应Out口的逻辑端口号
		s_KickKeyValue() 
		{
			iChannel = 0;
			iLogicalOutport = 0;
		}
		s_KickKeyValue(int iChannel, int iLogicalOutport)
		{
			this->iChannel = iChannel;
			this->iLogicalOutport = iLogicalOutport;
		}
	}s_KickKeyValue;
	map<int, s_KickKeyValue> m_map_KickPortDict;//**[12/18/2017 ConanSteve]**:  从踢费队列找到实际接口卡写卡的通道和逻辑端口号

};

