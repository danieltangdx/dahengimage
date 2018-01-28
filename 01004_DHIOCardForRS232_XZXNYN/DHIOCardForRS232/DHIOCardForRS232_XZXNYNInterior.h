#pragma once
#include "DHIOCard.h"
#include "ExFn.h"
#include "array"
#include <set>
#include "DlgIOCardForRS232_XZXNYN.h"
using namespace std;


typedef int (WINAPI * POpenPort)(int);
typedef int (WINAPI * PClosePort)();
typedef int (WINAPI * PSetCoil)(int, int, bool);
typedef int (WINAPI * PGetCoil)(int, int, int);
typedef int (WINAPI * PSetMultCoil)(int, int, int, int);
typedef bool (WINAPI * PBitValueOfInt)(int, int);
typedef int (WINAPI * PBitValueToInt)(int, int, bool);

const int c_CoilNum{ 8 };
class CDHIOCardForRS232_XZXNYNInterior : public CDHIOCard
{
public:
	CDHIOCardForRS232_XZXNYNInterior() {}
	virtual ~CDHIOCardForRS232_XZXNYNInterior();
	virtual bool InitCard(CIOCardInitInfo& rIOCardInitInfo)override;
	virtual bool OpenCard()override;
	virtual bool CloseCard()override;
	//**[9/4/2017 ConanSteve]**:  打开接口卡调试工具
	virtual bool OpenTools(CWnd* pParent = NULL) override;
	virtual bool Enable(bool bEnable)override;
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
	virtual bool GetGrabImgSN(int iGrabSN, int& iImageSN) override;
	//**[12/13/2017 ConanSteve]**:  获取指定踢废队列对应的输出口计数
	virtual bool GetKickCountInfo(int iKickSN, s_KickCountInfo& sKickCntInfo) override;
	//**[12/13/2017 ConanSteve]**:  设置接口卡输出口的状态值，更主要的原因是不同项目的插件需要设置接口卡输出口状态
	virtual bool SetOutIOStatus(int iOutport, bool bStatus) override;
	//**[12/13/2017 ConanSteve]**:  获取指定out口的计数
	virtual bool GetOutputCount(int iOutport, int& iValue) override;

private:
	int BuildState(int& Value, int Bit, bool state);//组合状态,最低位时Bit为0

protected:
	//**[9/12/2017 ConanSteve]**: 继电器保持闭合的最大时长：单位ms
	int m_iTimeOfRefreshCoil{ 0 };

	int m_iStateValue{0};//**[12/17/2017 ConanSteve]**:  设备状态
	CCriticalSection m_csWriteDevice;

	CIOCardInitInfo m_InitInfo;//**[12/17/2017 ConanSteve]**:  接口卡初始化信息
	//**[9/12/2017 ConanSteve]**:  刷新继电器线程句柄
	HANDLE m_hThred{ 0 };
	//**[9/12/2017 ConanSteve]**:  设置各继电器闭合的时间信息
	typedef struct s_HisInfoWritten {
		LONGLONG ldHisTimeOfSetCoil{ 0 };
		bool bNeedBeenReset{ false };
		s_HisInfoWritten() { bNeedBeenReset = false; }
	}s_HisInfoWritten;
	array<s_HisInfoWritten, c_CoilNum> m_sHisTimeInfoOfSetCoil;
	LONGLONG m_iFrequency{ 0 };
	//**[9/12/2017 ConanSteve]**:  踢废表
	array<array<bool, 256>, c_CoilNum> m_arrKickTable = { false };
	//**[9/13/2017 ConanSteve]**:  需要写结果的线圈
	set<int> m_setCoilBeWritten;
	//**[9/12/2017 ConanSteve]**:  定时刷新继电器线程
	DWORD static WINAPI  ThreadRefreshCoil(LPVOID pParam);
	CLogFile m_fileLogWrite;
	CLogFile m_fileLogReset;


	CString m_sPathFileOfInit{ _T("") };//**[9/11/2017 ConanSteve]**:  初始化文件路径
	HINSTANCE m_hDll{ 0 };
	//**[9/12/2017 ConanSteve]**:  错误信息
	CString m_sLastErrorInfo{ _T("") };
	//**[9/12/2017 ConanSteve]**: 

	CDlgIOCardForRS232_XZXNYN m_dlg;
	CBitmap   m_bmpOn, m_bmpOff;

	array<int, c_CoilNum> m_arr_iTriggerRegister;//**[12/15/2017 ConanSteve]**:  触发寄存器，保存图像号，供接口查询使用
	array<s_KickCountInfo, c_CoilNum> m_arr_sKickCntInfo;//**[12/15/2017 ConanSteve]**:  存储踢废表信息,此项目只用了第一个踢废队列
	CINICfg m_iniCfgOfInit;
public:
	array<bool, c_CoilNum> m_arr_bOpen;//**[12/17/2017 ConanSteve]**:  所有线圈是否闭合

	POpenPort pOpenPort;
	PClosePort pClosePort;
	PSetCoil pSetCoil;
	PGetCoil pGetCoil;
	PSetMultCoil pSetMultCoil;
	PBitValueOfInt pBitValueOfInt;
	PBitValueToInt pBitValueToInt;
	//OnSetCoil(int Addr, int KAddr, bool State);
private:

	DWORD ThreadRefreshCoilFunc();
private:
public:

	int m_iPort{ 0 };//**[9/11/2017 ConanSteve]**:  串口号
	int m_iBoardId{ 1 };
	//**[9/12/2017 ConanSteve]**:  是否打开接口卡
	bool m_bOpenCardSuccess{ false };
	//**[9/12/2017 ConanSteve]**:  写卡的延迟帧数
	int m_iFrameDelayOfKick{ 0 };
	bool UpdateToolsWindowInfo();


	bool SwitchState(int iBit);

	bool WriteDevice();
};

