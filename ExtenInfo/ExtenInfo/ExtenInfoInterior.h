
#pragma once
#include "typeinfo"
#include "iostream"
#include "string"
#include "vector"
#include "ExtenInfo.h"
using namespace std;


class CExtenInfoInterior
{
public:
	CExtenInfoInterior();
	~CExtenInfoInterior();
	void* GetFuncAddr(e_OptionalFuncCode iFuncCode);//**[9/26/2017 ConanSteve]**:  返回的是COptionalFunczhizhen
	CGeneralParam& GetGeneralParam(e_OptionalFuncCode iFuncCode);
//	COptionalFunc* GetFuncAddr(const type_info & typeInfo);
	void push_back(COptionalFunc* );
	bool remove_last();
	void clear();
	int GetFuncNum() { return vecOptionalFunc.size(); };
	CExtenInfoInterior& operator=(const CExtenInfoInterior&);

protected: 
	vector<COptionalFunc*> vecOptionalFunc;//**[7/19/2017 ConanSteve]**:  创建一个指针数组，来存储各功能结构体指针
// 	COptionalFunc** pp_arrOptionalFunc;
// 	int iSizeOfFunc;
// 	int iCapacityOfFunc;
};

class COptionalFuncInterior:public COptionalFunc
{
public:
	virtual e_OptionalFuncCode GetFuncCode() override { return iFuncCode; }
	virtual bool SetFuncCode(e_OptionalFuncCode iFuncCode)override { this->iFuncCode = iFuncCode;return true; };
protected:
	e_OptionalFuncCode iFuncCode;//**[7/19/2017 ConanSteve]**:  功能码，指明这个结构体传递的是哪些功能的参数
	int iSize;//**[7/19/2017 ConanSteve]**:  结构体大小，便于扩展
};

/*****************************************************************************************/

 class CMultiObjSaveImgParaInterior :public COptionalFuncInterior//**[7/18/2017 ConanSteve]**:  多对象存图相关参数
 {
 public:
 	CMultiObjSaveImgParaInterior(void* = nullptr);
 	~CMultiObjSaveImgParaInterior();
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
 
 protected:
 	int iImgNum;//*[7/7/2017 ConanSteve]*:  每个相机单个产品所拍的连续图像个数
 	int iImgWidth;//*[7/7/2017 ConanSteve]*:  单张图的宽度
 	int iImgHeight;//*[7/7/2017 ConanSteve]*:  单张图的高度
 	int iImgChannel;//*[7/7/2017 ConanSteve]*:  通道数
 	int iLenOfImgFolderPathBuf;	//**[7/26/2017 ConanSteve]**:  保存图像路径缓冲区长度
 	TCHAR lpszSaveImgFolderPath[MAX_PATH];//*[7/7/2017 ConanSteve]*:  当前相机图像文件的路径
 								//	wchar_t* pwszSaveImgFolderPath;//**[7/26/2017 ConanSteve]**:  为支持Unicode 而保留
 	bool bIsCirCleSaveImg;//*[7/10/2017 ConanSteve]*:  是否循环存图
 };
 
 class CAlgSaveImgParaInterior :public COptionalFuncInterior//**[7/18/2017 ConanSteve]**:  算法存图相关参数
 {
 public:
 	CAlgSaveImgParaInterior(void* = nullptr);
 	~CAlgSaveImgParaInterior();
 	void SetImgSN(int iImgSN);
 	int GetImgSN();//**[9/19/2017 ConanSteve]**:  获得当前图像号
 	void SetImgSavedCnt(int iImgSavedCnt);
 	int GetImgSavedCnt();//**[9/19/2017 ConanSteve]**:  获得已存图像号张数
 	void SetSaveImgFolderPath(LPCTSTR lpszAddr, int iLen);
 	TCHAR* GetAddrOfSaveImgFolderPath();

 protected:
 	int iImgSN{ 0 };//**[7/18/2017 ConanSteve]**:  当前图像号
 	int iImgSaveCnt{ 0 };//**[7/18/2017 ConanSteve]**:  已存图张数
 	int iLenOfImgFolderPathBuf;	//**[7/26/2017 ConanSteve]**:  保存图像路径缓冲区长度
 	TCHAR lpszSaveImgFolderPath[MAX_PATH];//**[7/18/2017 ConanSteve]**:  存图文件夹路径，只有算法存序列图时才需要这个参数
 								//wchar_t* pwszSaveImgFolderPath;
 };
 
 class CSetObjStatusWndInterior :public COptionalFuncInterior
 {
 public:
 	CSetObjStatusWndInterior(void* = nullptr);
 	void SetIsShow(bool bIsShow);
 	bool GetIsShow();
 protected:
 	bool bIsShow;
 };
 
 class CSetAlgObjectInterior :public COptionalFuncInterior//**[7/26/2017 ConanSteve]**:  读模板传递模板对象
 {
 public:
 	CSetAlgObjectInterior(void* = nullptr);
 	~CSetAlgObjectInterior() {}
 	void SetAddrOfModelObj(void* pAddrOfModelObject);
 	void * GetAddrOfModelObj();
 	void SetLoadModelCtrlPara(int iLoadModelCtrlPara);
 	int GetLoadModelCtrlPara();
	void SetAlgObjIndex(int iAlgObjIndex);//**[10/9/2017 ConanSteve]**:  设置算法检测对象在多对象管理队列中的索引
	int GetAlgObjIndex();
 protected:
 	void* pAddrOfModelObject;
 	int   iLoadModelCtrlPara;
	int iAlgObjIndex;
 };

class CAlgDetectParaInterior :public COptionalFuncInterior
{
public:
	CAlgDetectParaInterior(void* = nullptr);
	~CAlgDetectParaInterior() {}
	void SetDetectTimes(int iDetectTimes);
	int GetDetectTimes();
protected:
	int iDetectTimes{ 0 };

};

class CAlgCopyWindowInterior :public COptionalFuncInterior
{
public:
	CAlgCopyWindowInterior();
	~CAlgCopyWindowInterior() {}
	void SetTimeOfWait(int iTimeOfWait);
	int GetTimeOfWait();//**[10/9/2017 ConanSteve]**:  获得数据在缓冲队列中的等待时间
	void SetTimeOfFirstDetect(int iTimeOfFirstDetect);
	int GetTimeOfFirstDetect();//**[10/9/2017 ConanSteve]**:  获得1次Detect时间
	void SetTimeOfSecondDetect(int iTimeOfSecondDetect);
	int GetTimeOfSecondDetect();//**[10/9/2017 ConanSteve]**:  获得2次Detect时间
	void SetTickOfPushQueue(LONGLONG llTickOfPushQueue);
	LONGLONG GetTickOfPushQueue();
	CAlgCopyWindowInterior& operator=(const CAlgCopyWindowInterior&);
private:
	int iTimeOfWait;
	int iTimeOfFirstDetect;
	int iTimeOfSecondDetect;
	LONGLONG llTickOfPushQueue;//**[10/9/2017 ConanSteve]**:  记录图像数据第一次进入图像数据队列的时钟
};
/*************************************<>*************************************/


class  CGeneralParamInterior :public COptionalFuncInterior
{
public:
	CGeneralParamInterior();
	CGeneralParamInterior(e_OptionalFuncCode e_FuncCode);
	~CGeneralParamInterior();



	bool SetInt(int iVal);
	int GetInt();
	bool SetArrayInt(int* pArrInt, int iLen);
	bool GetArrayInt(int* pArrInt, int& iLen);

	bool SetBool(bool bVal);
	bool GetBool();
	bool SetArrayBool(bool* pArrBool, int iLen);
	bool GetArrayBool(bool* pArrBool, int& iLen);

	//double SetDouble(double lfVal);
	bool SetPTCHAR(TCHAR* lpData);//**[1/10/2018 ConanSteve]**:  默认长度256
	TCHAR* GetPTCHAR();
	bool SetArrayPTCHAR(TCHAR** lppData, int iLen);//**[1/10/2018 ConanSteve]**:  最大字符串长度256
	bool GetArrayPTCHAR(TCHAR** lppData, int &iLen);//**[1/10/2018 ConanSteve]**:  最大数组长度256

private:
	union {
		int iData;
		int arr_iData[MAX_PATH];
		bool bData;
		bool arr_bData[MAX_PATH];
		TCHAR lpszData[MAX_PATH*MAX_PATH];
		TCHAR arr_lpszData[MAX_PATH][MAX_PATH];
	}m_unionData;
	int m_iLen{ 1 };
};

