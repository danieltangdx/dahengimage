// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 EXTENINFO_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// EXTENINFO_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

#pragma once
#include "ExtenInfoDef.h"
#include "iostream"
#include "string"
using namespace std;

/************************************************************************/
const GUID guID = { 0x15ac02db, 0x48c8, 0x499a,{ 0xa2, 0x96, 0x39, 0x62, 0xc7, 0xfa, 0x8e, 0x93 } };
class COptionalFunc
{
public:
	COptionalFunc();
	COptionalFunc(e_OptionalFuncCode iFuncCode);
	e_OptionalFuncCode GetFuncCode();//**[9/25/2017 ConanSteve]**:  获得当前子类的代号
	COptionalFunc& SetFuncCode(e_OptionalFuncCode iFuncCode);

	COptionalFunc& SetBool(bool bVal);
	bool GetBool();

	COptionalFunc& SetInt(int iVal);
	int GetInt();

	COptionalFunc& SetPointer(void* ptr);
	void* GetPointer();

	COptionalFunc& SetDouble(double lfVal);
	double GetDouble();

	COptionalFunc& SetLonglong(LONGLONG ldVal);
	LONGLONG GetLonglong();

	COptionalFunc& SetArrayTCHAR(TCHAR* lpData);//**[1/10/2018 ConanSteve]**:  默认长度256
	TCHAR* GetArrayTCHAR();
protected:
	e_OptionalFuncCode iFuncCode;//**[7/19/2017 ConanSteve]**:  功能码，指明这个结构体传递的是哪些功能的参数
	union {
		bool bData;
		int iData;
		void* pPtr;
		double lfData;
		LONGLONG ldData;
		TCHAR lpszData[MAX_PATH];
	}m_unionData;
};

class CExtenInfo
{
public:
	CExtenInfo();
	~CExtenInfo();
	COptionalFunc& GetFuncStruct(e_OptionalFuncCode iFuncCode);
	COptionalFunc& push_back(COptionalFunc Func);//**[9/25/2017 ConanSteve]**:  将一个COptionalFunc的子类添加进CExtenInfo中
	COptionalFunc& push_back(e_OptionalFuncCode iFuncCode);
// 	bool remove_last();//**[9/25/2017 ConanSteve]**:  移除最后一个，当被调用对象使用完之后，调用对象必须移除最后一个，否则可能发生异常
// 	void clear();
// 	int GetFuncNum();//**[9/22/2017 ConanSteve]**:  获得扩展类的数目
	CExtenInfo& operator=(const CExtenInfo&);
protected:
	COptionalFunc * p_arrOptionalFunc;
	// {15AC02DB-48C8-499A-A296-3962C7FA8E93}
	GUID  m_guFlag;//**[1/23/2018 ConanSteve]**:  如果不为此值，那么极有可能是数据传递错误导致，应当避免访问‘p_arrOptionalFunc’
	int m_iSizeOfFunc;
	int iCapacityOfFunc;
};
/*************************************<>*************************************/
// 							CExtenInfo extenInfo;
// 							extenInfo.push_back(COptionalFunc(e_Fn_CheckOutPara_AlgCheckErrorDesc).SetArrayTCHAR(_T("裂纹")));
// 							//iReserve4 = (int)&extenInfo;//**[1/12/2018 ConanSteve]**:	extenInfo还是要用全局的
// 
// 							//**[1/12/2018 ConanSteve]**:	 "CExtenInfo指针"->GetFuncStruct("参数的宏定义").Get*****();
// 							int iDetectTimes = pExtenInfo->GetFuncStruct(e_Fn_CheckInParam_DetectTimes).GetIntEx();
// 							TCHAR* pBuf = pExtenInfo->GetFuncStruct(e_Fn_CheckOutPara_AlgCheckErrorDesc).GetArrayTCHAR();
// 
//							TCHAR(*lpszSaveImagePath)[MAX_PATH] = (TCHAR(*)[MAX_PATH])pExtenInfo->GetFuncStruct(e_Fn_AlgModelInput_SaveImagePath).GetPointer();
// 							CString sPath0 = lpszSaveImagePath[0];//**[1/12/2018 ConanSteve]**:	 获得存图路径1
// 							CString sPath1 = lpszSaveImagePath[1];


