#pragma once
#include "DHIOCard.h"

#define EXDLL

#ifdef EXDLL
#define DLLDECL _declspec(dllexport)
#else
#define DLLDECL _declspec(dllimport)
#endif

class DLLDECL CDHIOCardForSG :public CDHIOCard
{
public:
	CDHIOCardForSG();
	virtual ~CDHIOCardForSG();
	virtual bool InitCard(CIOCardInitInfo& rIOCardInitInfo)override;
	virtual bool OpenCard()override;
	virtual bool CloseCard()override; 
	virtual bool ResetIOCard()override;
	virtual bool OpenTools(CWnd* pParent = NULL);//**[9/4/2017 ConanSteve]**:  打开接口卡调试工具
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
protected:
	CDHIOCard* m_pIOCard;
};

