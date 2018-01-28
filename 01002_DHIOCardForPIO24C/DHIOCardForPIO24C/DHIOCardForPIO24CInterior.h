#pragma once
#include "DHIOCard.h"
#include "PIO24BLib.h"
#include "ExFn.h"
#include "array"
#include "map"
using namespace std;

#pragma comment(lib, "..\\lib\\PIO24B_DLL.lib")

#define MAX_INPORT_NUM 4
#define MAX_OUTPORT_NUM 8
class CDHIOCardForPIO24CInterior :
	public CDHIOCard
{
public:
	CDHIOCardForPIO24CInterior();
	virtual ~CDHIOCardForPIO24CInterior();
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
	//**[9/4/2017 ConanSteve]**:  获得错误信息
	virtual CString GetLastError() override;

	/*************************************<参数读取和设置>************************************[12/13/2017 ConanSteve]*/
	//**[12/13/2017 ConanSteve]**:  获取光电计数
	virtual bool GetInputIndex(int iInputport, int &iValue) override;
	//**[12/13/2017 ConanSteve]**:  读取指定相机号对应的图像号
	virtual bool GetGrabImgSN(int iGrabSN, int& iImageSN) override;
	//**[12/13/2017 ConanSteve]**:  获取指定剔废队列对应的输出口计数
	virtual bool GetKickCountInfo(int iKickSN, s_KickCountInfo& sKickCntInfo) override;
	//**[12/13/2017 ConanSteve]**:  设置接口卡输出口的状态值，更主要的原因是不同项目的插件需要设置接口卡输出口状态
	virtual bool SetOutIOStatus(int iOutport, bool bStatus) override;
	//**[12/13/2017 ConanSteve]**:  获取指定out口的计数
	virtual bool GetOutputCount(int iOutport, int& iValue) override;


private:
	CPIO24BLib m_IOCard;
	CIOCardInitInfo m_InitInfo;
	CINICfg m_iniCfgOfConnect;
	CString m_sLastError;//**[12/19/2017 ConanSteve]**:  错误信息
	int m_iIOCardSN{0};//**[12/19/2017 ConanSteve]**:  接口卡编号
	bool m_bWriteLog{ false };//**[1/7/2018 ConanSteve]**:	是否打开日志 
	bool m_bInitSuccess{ false };
	bool m_bEnableWorking{ false };//**[12/19/2017 ConanSteve]**:  接口卡是否正在工作
	bool m_bEnableClose{ false };//**[12/19/2017 ConanSteve]**:  是否关闭了接口卡
	int m_iGrabNum{0};//**[12/19/2017 ConanSteve]**:  接口卡连接的相机个数
	int m_iKickQueueNum{0};//**[12/19/2017 ConanSteve]**:  接口卡队列个数
	int m_iMaxInport{ 0 };//**[12/19/2017 ConanSteve]**:  接口卡使用的最大InportID
	int m_iMaxOutport{ 0 };//**[12/19/2017 ConanSteve]**:  接口卡使用的最大OurportID;
	array<int, MAX_INPORT_NUM> m_arr_iInportCnt;//**[12/19/2017 ConanSteve]**:  每个Inport的计数
	array<int, MAX_OUTPORT_NUM> m_arr_iOutportCnt;//**[12/19/2017 ConanSteve]**:  每个Outport的计数
	array<int, 32> m_arr_iQueueRegisterCnt;//**[12/19/2017 ConanSteve]**:  队列寄存器的个数
	map<int, int> m_map_GrabSN2QueueRegisterID;//**[12/19/2017 ConanSteve]**:  记录相机号对应的队列号寄存器地址，只可能是28,29,30,31
	map<int, int> m_map_SysInport2CardInport;//**[12/19/2017 ConanSteve]**: 系统In口到接口卡In口的映射，一般卡1两者相同，卡2不相同
	map<int, int> m_map_SysOutport2CardOutport;//**[12/19/2017 ConanSteve]**:  系统Out口到接口卡Out口的映射
	map<int, s_KickCountInfo> m_map_SysKickSN2KickCntInfo;//**[12/19/2017 ConanSteve]**:  系统剔废队列号到剔废信息的映射
	map<int, int> m_map_CardKickOutport2Channel;//**[12/19/2017 ConanSteve]**:  C卡剔废对应的out口到通道之间的关系

	array<CLogFile, 4> m_arr_fileKick;
private:
	bool ClearData();//**[12/30/2017 ConanSteve]**:清零接口卡数据 
};

