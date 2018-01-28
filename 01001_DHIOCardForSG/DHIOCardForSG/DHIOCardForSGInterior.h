#pragma once
#include "DHIOCard.h"
#include "memory"
#include "map"
#include "resource.h"
#include "DlgIOCard.h"
#include "ExFn.h"

#include <mmsystem.h>   
#pragma comment(lib, "WINMM.LIB") //**[12/8/2017 ConanSteve]**:  多媒体计时器



#define ID_EVENT_TIMER WM_USER+100
#define MAX_QUEUE 30//**[12/8/2017 ConanSteve]**:  最大支持30个队列，实际上由于多媒体计时器个数限制，不可能达到这个值

#define MAX_OUTPORT_NUM 24
#define MAX_INPORT_NUM 12
using namespace std;
class CDHIOCardForSGInterior :
	public CDHIOCard
{
public:
	CDHIOCardForSGInterior();
	~CDHIOCardForSGInterior();
	virtual bool InitCard(CIOCardInitInfo& rIOCardInitInfo)override;
	virtual bool OpenCard()override;
	virtual bool CloseCard()override; 
	virtual bool ResetIOCard()override;
	virtual bool OpenTools(CWnd* pParent/* = NULL */)override;
	virtual bool Enable(bool bEnable)override;
	virtual bool EnableQueue(bool bEnable) override;
	virtual bool GetParam(s_IOCardParaInfo& para) override;//**[9/12/2017 ConanSteve]**:  获取接口卡参数
	virtual bool SetParam(s_IOCardParaInfo& para) override;//**[9/12/2017 ConanSteve]**:  设置接口卡参数
	virtual bool WriteKickResult(s_sRsltInfoWritten & rslt)override;//**[9/11/2017 ConanSteve]**:  写检测结果
	virtual bool NotifyCardOfGrabCallBack(s_NotifyInfoOfGrabCallback &notifyInfo)override;//**[9/11/2017 ConanSteve]**:  相机回调通知
	virtual e_IOCardType GetIOCardType() override;
	virtual CString GetLastError()override;//**[9/4/2017 ConanSteve]**:  获得错误信息


	/*************************************<参数读取和设置>************************************[12/13/2017 ConanSteve]*/
	//**[12/13/2017 ConanSteve]**:  获取光电计数
	virtual bool GetInputIndex(int iInputport, int &iValue) override;
	//**[12/13/2017 ConanSteve]**:  读取指定相机号对应的图像号
	virtual bool GetGrabImgSN(int iGrabSN, int& iImageSN)override;
	//**[12/13/2017 ConanSteve]**:  获取指定剔废队列对应的输出口计数
	virtual bool GetKickCountInfo(int iKickSN, s_KickCountInfo& sKickCntInfo) override;
	//**[12/13/2017 ConanSteve]**:  设置接口卡输出口的状态值，更主要的原因是不同项目的插件需要设置接口卡输出口状态
	virtual bool SetOutIOStatus(int iOutport, bool bStatus) override;
	//**[12/13/2017 ConanSteve]**:  获取指定out口的计数
	virtual bool GetOutputCount(int iOutport, int& iValue) override;
private:
	CString m_sLastErrorInfo{ _T("") };
	bool m_bCloseCard{ false };
	bool  m_bEnableQueue{false};//**[12/8/2017 ConanSteve]**:  接口卡是否在工作
	unique_ptr<CDlgIOCard> m_pDlgTools;//**[12/8/2017 ConanSteve]**:  接口卡调试工具窗口
	CString m_sPathFileOfInit;//**[12/8/2017 ConanSteve]**:  虚拟接口卡触发文件
	CINICfg m_iniCfg;//**[12/8/2017 ConanSteve]**:  配置文件
	array<int, MAX_QUEUE> m_arr_iTimerID;//**[12/8/2017 ConanSteve]**:  每个队列对应的多媒体计时器ID
	array<s_KickCountInfo, MAX_OUTPORT_NUM> m_arr_sKickCntInfo;//**[12/13/2017 ConanSteve]**:  剔废计数信息
	array<vector<int>, MAX_INPORT_NUM> m_arr_veciQueueOutInfo;//**[12/17/2017 ConanSteve]**:  每个队列对应的out口号的集合
	map<UINT, int> m_mapTimerID2QueueID;//**[12/9/2017 ConanSteve]**:  方便从计时器ID找到对应的队列号
	//**[12/13/2017 ConanSteve]**:  相机号到out口之间关联，模拟相机
	map<int, int> m_mapGrabSN2PhysicalPortId;
	map<int, int> m_mapInportId2QueueId;//**[12/17/2017 ConanSteve]**:  输入口ID到队列号ID的字典


public:
	CIOCardInitInfo m_InitInfo;
	int m_iKickQueueNum{ 1 };//**[12/13/2017 ConanSteve]**:  剔废队列个数
	int m_iQueueNum;//**[12/8/2017 ConanSteve]**:  触发队列个数
	array<int, MAX_QUEUE> m_arr_iQueueInport;//**[12/14/2017 ConanSteve]**:  队列输入口
	array<int, MAX_QUEUE> m_arr_iActiveInterval{0};//**[12/8/2017 ConanSteve]**:  每个队列触发的采图周期
	array<int, MAX_QUEUE> m_arr_iTriggerCnt{0};//**[12/8/2017 ConanSteve]**:  每个队列触发的次数
	array<int, MAX_INPORT_NUM> m_arr_iInportCnt{ 0 };//**[12/14/2017 ConanSteve]**:  每个In口的计数，总共12个寄存器
	array<int, MAX_OUTPORT_NUM> m_arr_iOutportCnt{ 0 };//**[12/13/2017 ConanSteve]**:  每个out口的计数,总共24个寄存器
	array<bool, MAX_OUTPORT_NUM> m_arr_bOutportStatus;//**[12/16/2017 ConanSteve]**:  Out口开关状态
	array<HANDLE, MAX_OUTPORT_NUM> m_arr_OutputEventHandle;//**[12/9/2017 ConanSteve]**:  每个队列对应的内核事件句柄
	array<bool, MAX_INPORT_NUM> m_arr_bEnableInternalActive;//**[12/17/2017 ConanSteve]**:  输入口是否内触发

	map<int, int> m_mapSysKickSN2CardKickId;//**[12/18/2017 ConanSteve]**:  系统剔废队列号到接口卡剔废队列编号的字典，主要满足一个系统多个接口卡的需求，例如C卡

public:
	//**[12/13/2017 ConanSteve]**:  启动所有队列
	bool AddMultimediaTimer();
	//**[12/13/2017 ConanSteve]**:  计时器周期触发回调函数
	static void CALLBACK CallBackFuncTimer(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2);
	void CallBackFuncTimerFunc(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2);
	//**[12/13/2017 ConanSteve]**:  关闭所有队列
	bool DeleteMultimediaTimer();
	bool TriggerInport(int iInportId);
};

