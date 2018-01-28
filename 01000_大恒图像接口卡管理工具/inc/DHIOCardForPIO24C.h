
#pragma once
#include "DHIOCard.h"



#ifdef DHIOCARDFORPIO24C

#define CLASS_DECLSPEC    __declspec(dllexport)  //程序编译时使用
#else
#define CLASS_DECLSPEC    __declspec(dllimport)	 //动态库应用时使用
#endif


class CLASS_DECLSPEC CDHIOCardForPIO24C : public CDHIOCard
{
public:
	CDHIOCardForPIO24C();
	virtual ~CDHIOCardForPIO24C();
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
	//**[12/13/2017 ConanSteve]**:  获取指定剔废队列对应的输出口计数
	virtual bool GetKickCountInfo(int iKickSN, s_KickCountInfo& sKickCntInfo) override;
	//**[12/13/2017 ConanSteve]**:  设置接口卡输出口的状态值，更主要的原因是不同项目的插件需要设置接口卡输出口状态
	virtual bool SetOutIOStatus(int iOutport, bool bStatus) override;
	//**[12/13/2017 ConanSteve]**:  获取指定out口的计数
	virtual bool GetOutputCount(int iOutport, int& iValue) override;
protected:
	CDHIOCard* m_pIOCard;
};

