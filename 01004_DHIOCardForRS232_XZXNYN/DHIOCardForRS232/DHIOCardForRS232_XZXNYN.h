#pragma once
#include "DHIOCard.h"

#define  EXDLL


#ifdef EXDLL
#define CLASS_DECLSPEC    __declspec(dllexport)  //程序编译时使用
#else
#define CLASS_DECLSPEC    __declspec(dllimport)	 //动态库应用时使用
#endif


class CLASS_DECLSPEC CDHIOCardForRS232_XZXNYN: public CDHIOCard
{
public:
	CDHIOCardForRS232_XZXNYN();
	virtual ~CDHIOCardForRS232_XZXNYN();
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
protected:
	CDHIOCard* m_pIOCard{nullptr};
};