// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 EXTENINFO_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// EXTENINFO_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

#pragma once
#include "typeinfo"
#include "iostream"
#include "string"
using namespace std;


#ifdef EXTENINFO_EXPORTS
#define EXTENINFO_API __declspec(dllexport)
#else
#define EXTENINFO_API __declspec(dllimport)
#endif

// 此类是从 ExtenInfo.dll 导出的
// class EXTENINFO_API CExtenInfo {
// public:
// 	CExtenInfo(void);
// 	// TODO:  在此添加您的方法。
// };

extern EXTENINFO_API int nExtenInfo;

EXTENINFO_API int fnExtenInfo(void);


//**[7/18/2017 ConanSteve]**:  扩展功能定义
enum e_OptionalFuncCode {
	e_OptionalFuncCode_NONE = 0,//**[1/11/2018 ConanSteve]**:  默认未设置
	Sys2Obj_SaveImgPara = 1,	//**[7/26/2017 ConanSteve]**:  系统使用多对象和算法存图相关参数
	Obj2Alg_SaveImgPara = 2,	//**[7/26/2017 ConanSteve]**:  多对象给算法存图相关参数
	Sys2Obj_SetObjStatus = 3,	//**[7/26/2017 ConanSteve]**:  处理是否显示多对象实时状态窗口
	Obj2Alg_SetAlgObject = 4, //**[7/26/2017 ConanSteve]**:  初始化多个对象时，多对象给检测对象第一个检测对象的地址
	Obj2Alg_AlgDetectPara = 5,//**[10/9/2017 ConanSteve]**:  多对象调用算法Detect时使用
	Obj2Alg_AlgCopyWindow = 6,//**[10/9/2017 ConanSteve]**:  多对象调用算法的拷贝窗口时使用


	//**[1/11/2018 ConanSteve]**:  s_CheckInitParam

	/*************************************<s_CheckInParam>************************************[1/11/2018 ConanSteve]*/
	e_Fn_CheckInParam_SaveImageNum = 0x2000,//**[1/11/2018 ConanSteve]**:  最大存图张数int
	e_Fn_CheckInParam_ImageWidth = 0x2001,//**[1/11/2018 ConanSteve]**:  图像宽度int
	e_Fn_CheckInParam_ImageHeight = 0x2002,//**[1/11/2018 ConanSteve]**:  int
	e_Fn_CheckInParam_ImageChannel= 0x2003,//**[1/11/2018 ConanSteve]**:  int
	e_Fn_CheckInParam_WhetherCicleSaveImage = 0x2004,//**[1/11/2018 ConanSteve]**:  是否循环存图bool
	e_Fn_CheckInParam_SaveImagePath = 0x2005,//**[1/11/2018 ConanSteve]**:  存图路径,使用


	/*************************************<s_CheckOutPara>************************************[1/11/2018 ConanSteve]*/
	e_Fn_CheckOutPara_AlgCheckErrorDesc = 0x4000   //**[1/10/2018 ConanSteve]**:  算法返回的错误描述

	//**[1/11/2018 ConanSteve]**:  多对象存图相关
	
};

/*************************************<加载模板控制参数定义>*************************************/
enum e_LoadModelCtrlPara
{
	e_LoadModelCtrlPara_ReadPara = 1,
	e_LoadModelCtrlPara_ReadModel = 2,
	e_LoadModelCtrlPara_ReadParaAndModel = 3
};
/************************************************************************/

class EXTENINFO_API COptionalFunc//**[7/18/2017 ConanSteve]**:  扩展功能父类
{
public:
	COptionalFunc() {}
	virtual e_OptionalFuncCode GetFuncCode();//**[9/25/2017 ConanSteve]**:  获得当前子类的代号
	virtual bool SetFuncCode(e_OptionalFuncCode);
protected:
	void* pInterior;
};

//**[1/10/2018 ConanSteve]**:  使用256*256个TCHAR字符传递信息
class EXTENINFO_API CGeneralParam :public COptionalFunc
{
public:
	CGeneralParam();
	CGeneralParam(e_OptionalFuncCode iFuncCode);
	~CGeneralParam();

	bool SetInt(int iVal);
	int GetInt();
	bool SetArrayInt(int* pArrInt, int iLen);
	bool GetArrayInt(int* pArrInt, int& iLen);

	bool SetBool(bool bVal);
	bool GetBool();
	bool SetArrayBool(bool* pArrBool, int iLen);
	bool GetArrayBool(bool* pArrBool, int& iLen);

	bool SetDouble(double lfVal);
	double GetDouble();
	bool SetArrayDouble(double* plfVal, int iLen);
	bool GetArrayDouble(double* plfVal, int& iLen);

	bool SetPTCHAR(TCHAR* lpData);//**[1/10/2018 ConanSteve]**:  默认长度256
	TCHAR* GetPTCHAR();
	bool SetArrayPTCHAR(TCHAR** lppData, int iLen);//**[1/10/2018 ConanSteve]**:  最大字符串长度256
	bool GetArrayPTCHAR(TCHAR** lppData, int &iLen);//**[1/10/2018 ConanSteve]**:  最大数组长度256
													//CGeneralParam& operator=(const CGeneralParam&);
};
// {
// 	CExtenInfo extenInfo;
// 	CGeneralParam generalParam(e_GeneralParamType_AlgCheckErrorDesc);
// 	TCHAR buf[MAX_PATH];
// 	generalParam.SetPTCHAR(buf);
// 	extenInfo.push_back(&generalParam);
// 	iReserve4 = &extenInfo;
// }

class EXTENINFO_API CExtenInfo
{
public:
	CExtenInfo();
	~CExtenInfo();
	COptionalFunc* GetFuncAddr(e_OptionalFuncCode iFuncCode);//**[9/25/2017 ConanSteve]**:  获得指定代号的类地址
	CGeneralParam& GetGeneralParam(e_OptionalFuncCode iFuncCode);
	void push_back(COptionalFunc*);//**[9/25/2017 ConanSteve]**:  将一个COptionalFunc的子类添加进CExtenInfo中
	bool remove_last();//**[9/25/2017 ConanSteve]**:  移除最后一个，当被调用对象使用完之后，调用对象必须移除最后一个，否则可能发生异常
	void clear();
	int GetFuncNum();//**[9/22/2017 ConanSteve]**:  获得扩展类的数目
	CExtenInfo& operator=(const CExtenInfo&);
protected:
	void* pInterior;
};


/*****************************************************************************************/

class EXTENINFO_API CMultiObjSaveImgPara :public COptionalFunc//**[7/18/2017 ConanSteve]**:  多对象存图相关参数
{
public:
	CMultiObjSaveImgPara();
	~CMultiObjSaveImgPara();
	void SetImgNum(int iImgNum);//**[7/26/2017 ConanSteve]**:  设置图像张数
	int GetImgNum();//**[7/26/2017 ConanSteve]**:  获得图像张数
	void SetImgWidth(int iImgWidth);//**[7/26/2017 ConanSteve]**:  设置图像高度
	int GetImgWidth();//**[7/26/2017 ConanSteve]**:  获得图像宽度
	void SetImgHeight(int iImgHeight);//**[7/26/2017 ConanSteve]**:  设置图像高度
	int GetImgHeight();
	void SetImgChannel(int iImgChannel);
	int GetImgChannel();
	void SetIsCirCleSaveImg(bool bIsCirCleSaveImg);
	bool GetIsCirCleSaveImg();
	void SetSaveImgFolderPath(LPCTSTR lpszSaveImgFolderPath, int iLen);
	TCHAR* GetAddrOfSaveImgFolderPath();
};
 
 class EXTENINFO_API CAlgSaveImgPara :public COptionalFunc//**[7/18/2017 ConanSteve]**:  算法存图相关参数
 {
 public:
	 CAlgSaveImgPara();
	 ~CAlgSaveImgPara();
 	void SetImgSN(int iImgSN);
 	int GetImgSN();//**[9/19/2017 ConanSteve]**:  获得当前图像号
 	void SetImgSavedCnt(int iImgSavedCnt);
 	int GetImgSavedCnt();//**[9/19/2017 ConanSteve]**:  获得已存图像号张数
 	void SetSaveImgFolderPath(LPCTSTR lpszAddr, int iLen =MAX_PATH);
 	TCHAR* GetAddrOfSaveImgFolderPath();
 };
 
 class EXTENINFO_API CSetObjStatusWnd :public COptionalFunc
 {
 public:
	 CSetObjStatusWnd();
	 ~CSetObjStatusWnd();
 	void SetIsShow(bool bIsShow);
 	bool GetIsShow();
 };
 
 class EXTENINFO_API CSetAlgObject :public COptionalFunc//**[7/26/2017 ConanSteve]**:  读模板传递模板对象
 {
 public:
	 CSetAlgObject();
	 ~CSetAlgObject();
 	void SetAddrOfModelObj(void* pAddrOfModelObject);
 	void * GetAddrOfModelObj();
 	void SetLoadModelCtrlPara(int iLoadModelCtrlPara);
 	int GetLoadModelCtrlPara();
	void SetAlgObjIndex(int iAlgObjIndex);//**[10/9/2017 ConanSteve]**:  设置算法检测对象在多对象管理队列中的索引
	int GetAlgObjIndex();
 };

class EXTENINFO_API CAlgDetectPara :public COptionalFunc//**[9/22/2017 ConanSteve]**:  多对象调用算法Detect传递信息类
{
public:
	CAlgDetectPara();
	~CAlgDetectPara();
	void SetDetectTimes(int iDetectTimes);//**[9/25/2017 ConanSteve]**:  设置Detect次数，0表示一次Detect，1、2分别表示二次Detect的第1次和第2次调用
	int GetDetectTimes();//**[9/25/2017 ConanSteve]**:  获得多对象的DetectTimes值
};

class EXTENINFO_API CAlgCopyWindow :public COptionalFunc
{
public:
	CAlgCopyWindow();
	~CAlgCopyWindow();
	void SetTimeOfWait(int iTimeOfWait);
	int GetTimeOfWait();//**[10/9/2017 ConanSteve]**:  获得数据在缓冲队列中的等待时间
	void SetTimeOfFirstDetect(int iTimeOfFirstDetect);
	int GetTimeOfFirstDetect();//**[10/9/2017 ConanSteve]**:  获得1次Detect时间
	void SetTimeOfSecondDetect(int iTimeOfSecondDetect);
	int GetTimeOfSecondDetect();//**[10/9/2017 ConanSteve]**:  获得2次Detect时间
	void SetTickOfPushQueue(LONGLONG llTickOfPushQueue);
	LONGLONG GetTickOfPushQueue();
	CAlgCopyWindow& operator=(const CAlgCopyWindow&);
};



/*************************************<>*************************************/


