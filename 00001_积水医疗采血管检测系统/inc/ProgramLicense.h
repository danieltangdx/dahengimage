// ProgramLicense.h: interface for the CProgramLicense class.
//
//////////////////////////////////////////////////////////////////////
#include "..\\..\\IncPublic\\alg_struct.h"
//using namespace Alg;

#if !defined(AFX_PROGRAMLICENSE_H__A36FE32F_4A7E_435E_B3E1_D226AF592D80__INCLUDED_)
#define AFX_PROGRAMLICENSE_H__A36FE32F_4A7E_435E_B3E1_D226AF592D80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef GUID ver_code;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	_EXPORTING

#ifdef _EXPORTING
#define CLASS_DECLSPEC    __declspec(dllexport)  //程序编译时使用
#else
#define CLASS_DECLSPEC    __declspec(dllimport)	 //动态库应用时使用
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define KV_NOLIMITATION			0		//序列号无限制
#define KV_NOTREACHLIMITATION	-1		//未达到限制，且离截止日期还很远
#define KV_CLOSETODEADLINE		-2		//未达到限制，离截止日期很近

#define KV_NOTFINDROCKEYDLL		1		//没找到加密狗动态库
#define KV_NOTFINDDONGLE		2		//没找到加密狗
#define KV_OPENDONGLEFAIL		3		//打开加密狗失败
#define KV_FAILLOADKEY			4		//加载密钥失败
#define KV_UNVALIDUSERID		5		//用户名错误
#define KV_INVALIDKEY			6		//非法密钥
#define KV_EXPIREDKEY			7		//密钥过期
#define KV_LICENSEFILENOTFOUND  8		//没找到License文件
#define KV_INFOCHANGED			9		//注册信息被更改
#define KV_VCODEERROR			10		//UUID验证失败
#define KV_UNKNOWPROBLEM		99		//其他错误

typedef struct _KeyVerfResult
{
	int nError;				//对应上边的define的东西	
	int nExpireDate;		//-1: 无此项限制  0: 未到截止日期  1: 距离截止日期少于15天
	int nDays;				//-1: 无此项限制  0: 未达到使用天数限制  1: 距离使用天数限制还剩少于15天
	int nGlobalRuntime;		//-1: 无此项限制  0: 未达到使用时间限制  1: 距离使用天数限制少于30分钟
	int nExecutions;		//-1: 无此项限制  0: 未达到使用次数限制  1: 距离使用次数限制还剩少于10次
	char chErrorDetail[1024];
}s_KeyVerfResult;

class CLASS_DECLSPEC CProgramLicense  
{
public:
	CProgramLicense();
	virtual ~CProgramLicense();
	
	s_KeyVerfResult CheckLicenseValid(BOOL bFirst);
	void GetVerCode(ver_code* vCode);
	int CheckDogTime();	//0: Good  1: 回拨日期  2: 非法日期或加/解密错误  >3: 加密狗类型错误
	void ShowDongleErrorCode();

	int	ReadHardwareID(char* chHardwareID);
	void OpenDog(); //调试用！！
private:
	void* m_pCheck;
//	LONGLONG m_llFreq;
};

#endif // !defined(AFX_PROGRAMLICENSE_H__A36FE32F_4A7E_435E_B3E1_D226AF592D80__INCLUDED_)
