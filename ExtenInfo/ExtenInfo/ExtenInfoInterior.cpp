#include "stdafx.h"
#include "ExtenInfoInterior.h"



CExtenInfoInterior::CExtenInfoInterior()
{
// 	pp_arrOptionalFunc = NULL;
// 	iCapacityOfFunc = 2;
// 	iSizeOfFunc = 0;
// 	pp_arrOptionalFunc = new COptionalFunc*[iCapacityOfFunc];
// 	memset(pp_arrOptionalFunc, 0, sizeof(pp_arrOptionalFunc)*iCapacityOfFunc);
}

CExtenInfoInterior::~CExtenInfoInterior()
{
// 	if (NULL != pp_arrOptionalFunc)
// 	{
// 		delete[] pp_arrOptionalFunc;
// 		pp_arrOptionalFunc = NULL;
// 	}
}
void CExtenInfoInterior::push_back(COptionalFunc* pOptionalFunc)
{
// 	int iFuncCode = pOptionalFunc->GetFuncCode();
 	vecOptionalFunc.push_back(pOptionalFunc);


// 	if (iSizeOfFunc == iCapacityOfFunc)
// 	{
// 		COptionalFunc** pNewOptionalFunc = new COptionalFunc*[iCapacityOfFunc * 2];
// 		memcpy(pNewOptionalFunc, pp_arrOptionalFunc, iCapacityOfFunc * sizeof(COptionalFunc*));
// 		delete[]pp_arrOptionalFunc;
// 		pp_arrOptionalFunc = pNewOptionalFunc;
// 		iCapacityOfFunc *= 2;
// 	}
// 	pp_arrOptionalFunc[iSizeOfFunc++] = pOptionalFunc;

}

bool CExtenInfoInterior::remove_last()
{
	if (vecOptionalFunc.size() > 0)
	{
		vecOptionalFunc.erase(vecOptionalFunc.end()-1);
	}
// 	if (iSizeOfFunc > 0)
// 	{
// 		--iSizeOfFunc;
// 	}
	return true;
}

void CExtenInfoInterior::clear()
{	
	vecOptionalFunc.clear();
	/*iSizeOfFunc = 0;*/
}

CExtenInfoInterior & CExtenInfoInterior::operator=(const CExtenInfoInterior &extenInfoInterior)
{
	// TODO: 在此处插入 return 语句
	vecOptionalFunc = extenInfoInterior.vecOptionalFunc;
		
// 	delete[] pp_arrOptionalFunc;
// 	iSizeOfFunc = extenInfoInterior.iSizeOfFunc;
// 	iCapacityOfFunc = extenInfoInterior.iCapacityOfFunc;
// 	pp_arrOptionalFunc = new COptionalFunc*[iCapacityOfFunc];
// 	//copy(extenInfoInterior.pp_arrOptionalFunc, extenInfoInterior.pp_arrOptionalFunc + extenInfoInterior.iCapacityOfFunc, pp_arrOptionalFunc);
// 	memcpy_s(pp_arrOptionalFunc, iCapacityOfFunc * sizeof(COptionalFunc*), extenInfoInterior.pp_arrOptionalFunc, iCapacityOfFunc * sizeof(COptionalFunc*));
	return *this;
}


void* CExtenInfoInterior::GetFuncAddr(e_OptionalFuncCode iFuncCode)
{

	int i = 0;
// 	if (vecOptionalFunc.size() > 10);//**[1/11/2018 ConanSteve]**:  目前使用长度不会大于10，大于10，肯定是某个地方使用错误
// 		return NULL;
	for (int i = 0; i < vecOptionalFunc.size(); ++i)
	{
		if (iFuncCode == vecOptionalFunc[i]->GetFuncCode())
		{
			return vecOptionalFunc[i];
		}
	}
// 	for (i = 0; i < iSizeOfFunc; ++i)
// 	{
// 		if (iFuncCode == pp_arrOptionalFunc[i]->GetFuncCode())
// 		{
// 			return pp_arrOptionalFunc[i];
// 		}
// 	}
	return NULL;
}
CGeneralParam & CExtenInfoInterior::GetGeneralParam(e_OptionalFuncCode iFuncCode)
{
	CGeneralParam* pGeneralParam =((CGeneralParam*)GetFuncAddr(iFuncCode));
	if (NULL != pGeneralParam)
		return *pGeneralParam;
	return CGeneralParam();
	// TODO: insert return statement here
}
/************************************************************************/



 CMultiObjSaveImgParaInterior::CMultiObjSaveImgParaInterior(void* pParent)
 {
 	iFuncCode = Sys2Obj_SaveImgPara;
 	iSize = sizeof(CMultiObjSaveImgParaInterior);
 	iImgNum = 0;
 	iImgWidth = 0;
 	iImgHeight = 0;
 	iImgChannel = 1;
 	iLenOfImgFolderPathBuf = 256;
 	bIsCirCleSaveImg = false;
	memset(lpszSaveImgFolderPath, 0, iLenOfImgFolderPathBuf);
 }
 
 CMultiObjSaveImgParaInterior::~CMultiObjSaveImgParaInterior()
 {
 }
 
 void CMultiObjSaveImgParaInterior::SetImgNum(int iImgNum)
 {
 	this->iImgNum = iImgNum;
 }
 
 int CMultiObjSaveImgParaInterior::GetImgNum()
 {
 	return iImgNum;
 }
 
 void CMultiObjSaveImgParaInterior::SetImgWidth(int iImgWidth)
 {
 	this->iImgWidth = iImgWidth;
 }
 
 int CMultiObjSaveImgParaInterior::GetImgWidth()
 {
 	return iImgWidth;
 }
 
 void CMultiObjSaveImgParaInterior::SetImgHeight(int iImgHeight)
 {
 	this->iImgHeight = iImgHeight;
 }
 
 int CMultiObjSaveImgParaInterior::GetImgHeight()
 {
 	return iImgHeight;
 }
 
 void CMultiObjSaveImgParaInterior::SetImgChannel(int iImgChannel)
 {
 	this->iImgChannel = iImgChannel;
 }
 
 int CMultiObjSaveImgParaInterior::GetImgChannel()
 {
 	return iImgChannel;
 }
 
 void CMultiObjSaveImgParaInterior::SetIsCirCleSaveImg(bool bIsCirCleSaveImg)
 {
 	this->bIsCirCleSaveImg = bIsCirCleSaveImg;
 }
 
 bool CMultiObjSaveImgParaInterior::GetIsCirCleSaveImg()
 {
 	return bIsCirCleSaveImg;
 }
 
 void CMultiObjSaveImgParaInterior::SetSaveImgFolderPath(LPCTSTR lpszSaveImgFolderPath, int iLen)
 {
//  	if (iLen > iLenOfImgFolderPathBuf - 1)
//  	{
//  		if (NULL != this->lpszSaveImgFolderPath)
//  			delete[] this->lpszSaveImgFolderPath;
//  		iLenOfImgFolderPathBuf *= 2;
//  		lpszSaveImgFolderPath = new TCHAR[iLenOfImgFolderPathBuf] {0};
//  	}
// 	memset(this->lpszSaveImgFolderPath, 0, iLenOfImgFolderPathBuf);
 	memcpy_s(this->lpszSaveImgFolderPath, MAX_PATH, lpszSaveImgFolderPath, iLen);
 }
 
 TCHAR* CMultiObjSaveImgParaInterior::GetAddrOfSaveImgFolderPath()
 {
 	return lpszSaveImgFolderPath;
 }
 

 /************************************************************************/
 CAlgSaveImgParaInterior::CAlgSaveImgParaInterior(void* pParent)
 {
 	iFuncCode = Obj2Alg_SaveImgPara;
 	iSize = sizeof(CAlgSaveImgParaInterior);
 	iImgSN = 0;
 	iImgSaveCnt = 0;
 	iLenOfImgFolderPathBuf = 256;
	memset(lpszSaveImgFolderPath, 0, iLenOfImgFolderPathBuf);
 }
 
 CAlgSaveImgParaInterior::~CAlgSaveImgParaInterior()
 {
 }
 
 void CAlgSaveImgParaInterior::SetImgSN(int iImgSN)
 {
 	this->iImgSN = iImgSN;
 }
 
 int CAlgSaveImgParaInterior::GetImgSN()
 {
 	return iImgSN;
 }
 
 void CAlgSaveImgParaInterior::SetImgSavedCnt(int iImgSaveCnt)
 {
 	this->iImgSaveCnt = iImgSaveCnt;
 }
 
 int CAlgSaveImgParaInterior::GetImgSavedCnt()
 {
 	return iImgSaveCnt;
 }
 
 void CAlgSaveImgParaInterior::SetSaveImgFolderPath(LPCTSTR lpszSaveImgFolderPath, int iLen)
 {
 	if (iLen > iLenOfImgFolderPathBuf - 1)
 	{
 		if (NULL != this->lpszSaveImgFolderPath)
 			delete[] this->lpszSaveImgFolderPath;
 		iLenOfImgFolderPathBuf *= 2;
		lpszSaveImgFolderPath = new TCHAR[iLenOfImgFolderPathBuf] {0};
 	}
 	memset(this->lpszSaveImgFolderPath, 0, iLenOfImgFolderPathBuf);
 	memcpy_s(this->lpszSaveImgFolderPath, iLenOfImgFolderPathBuf, lpszSaveImgFolderPath, iLen);
 }
  
 TCHAR* CAlgSaveImgParaInterior::GetAddrOfSaveImgFolderPath()
 {
 	return lpszSaveImgFolderPath;
 }
 
 
 /*************************************<>************************************[9/22/2017 ConanSteve]*/
 CSetObjStatusWndInterior::CSetObjStatusWndInterior(void* pParent)
 {
 	iFuncCode = Sys2Obj_SetObjStatus;
 	iSize = sizeof(CSetObjStatusWndInterior);
	bIsShow = false;
 }
 
 void CSetObjStatusWndInterior::SetIsShow(bool bIsShow)
 {
 	this->bIsShow = bIsShow;
 }
 
 bool CSetObjStatusWndInterior::GetIsShow()
 {
 	return bIsShow;
 }
 /************************************************************************/
 
 CSetAlgObjectInterior::CSetAlgObjectInterior(void* pParent)
 {
 	iFuncCode = Obj2Alg_SetAlgObject;
 	iSize = sizeof(CSetAlgObjectInterior);
	pAddrOfModelObject = NULL;
	iLoadModelCtrlPara = e_LoadModelCtrlPara::e_LoadModelCtrlPara_ReadParaAndModel;
 }
 
 void CSetAlgObjectInterior::SetAddrOfModelObj(void* pAddrOfModelObject)
 {
 	this->pAddrOfModelObject = pAddrOfModelObject;
 }
 
 void* CSetAlgObjectInterior::GetAddrOfModelObj()
 {
 	return pAddrOfModelObject;
 }
 
 void CSetAlgObjectInterior::SetLoadModelCtrlPara(int iLoadModelCtrlPara)
 {
 	this->iLoadModelCtrlPara = iLoadModelCtrlPara;
 }
 
 int CSetAlgObjectInterior::GetLoadModelCtrlPara()
 {
 	return iLoadModelCtrlPara;
 }
 void CSetAlgObjectInterior::SetAlgObjIndex(int iAlgObjIndex)
 {
	 this->iAlgObjIndex = iAlgObjIndex;
 }
 int CSetAlgObjectInterior::GetAlgObjIndex()
 {
	 return iAlgObjIndex;
 }
/*************************************<2次Detect相关参数>*************************************/
CAlgDetectParaInterior::CAlgDetectParaInterior(void* pParent)
{
	iFuncCode = Obj2Alg_AlgDetectPara;
	iSize = sizeof(CAlgDetectParaInterior);
	iDetectTimes = 0;
}

void CAlgDetectParaInterior::SetDetectTimes(int iDetectTimes)
{
	this->iDetectTimes = iDetectTimes;
}
int CAlgDetectParaInterior::GetDetectTimes()
{
	return iDetectTimes;
}
/*************************************<>************************************[10/12/2017 ConanSteve]*/

CAlgCopyWindowInterior::CAlgCopyWindowInterior()
{
	iFuncCode = Obj2Alg_AlgCopyWindow;
	iSize = sizeof(CAlgCopyWindowInterior);
	iTimeOfWait = 0;
	iTimeOfFirstDetect = 0;
	iTimeOfSecondDetect = 0;
}

void CAlgCopyWindowInterior::SetTimeOfWait(int iTimeOfWait)
{
	this->iTimeOfWait = iTimeOfWait;
}

int CAlgCopyWindowInterior::GetTimeOfWait()
{
	return iTimeOfWait;
}

void CAlgCopyWindowInterior::SetTimeOfFirstDetect(int iTimeOfFirstDetect)
{
	this->iTimeOfFirstDetect = iTimeOfFirstDetect;
}

int CAlgCopyWindowInterior::GetTimeOfFirstDetect()
{
	return iTimeOfFirstDetect;
}

void CAlgCopyWindowInterior::SetTimeOfSecondDetect(int iTimeOfSecondDetect)
{
	this->iTimeOfSecondDetect = iTimeOfSecondDetect;
}

int CAlgCopyWindowInterior::GetTimeOfSecondDetect()
{
	return iTimeOfSecondDetect;
}

void CAlgCopyWindowInterior::SetTickOfPushQueue(LONGLONG llTickOfPushQueue)
{
	this->llTickOfPushQueue = llTickOfPushQueue;
}

LONGLONG CAlgCopyWindowInterior::GetTickOfPushQueue()
{
	return llTickOfPushQueue;
}

CAlgCopyWindowInterior & CAlgCopyWindowInterior::operator=(const CAlgCopyWindowInterior & algCopyWindow)
{
	// TODO: 在此处插入 return 语句
	iTimeOfWait = algCopyWindow.iTimeOfWait;
	iTimeOfFirstDetect = algCopyWindow.iTimeOfFirstDetect;
	iTimeOfSecondDetect = algCopyWindow.iTimeOfSecondDetect;
	llTickOfPushQueue = algCopyWindow.llTickOfPushQueue;
	return *this;
}

CGeneralParamInterior::CGeneralParamInterior()
{
	memset(m_unionData.lpszData, 0, sizeof(m_unionData.lpszData));
	this->iFuncCode = e_OptionalFuncCode_NONE;
	this->iSize = sizeof(CGeneralParamInterior);
}

CGeneralParamInterior::CGeneralParamInterior(e_OptionalFuncCode iFuncCode)
{
	memset(m_unionData.lpszData, 0, sizeof(m_unionData.lpszData));
	this->iFuncCode = iFuncCode;
	this->iSize = sizeof(CGeneralParamInterior);
}

CGeneralParamInterior::~CGeneralParamInterior()
{
}

bool CGeneralParamInterior::SetInt(int iVal)
{
	m_unionData.iData = iVal;
	return false;
}

int CGeneralParamInterior::GetInt()
{
	return m_unionData.iData;
}

bool CGeneralParamInterior::SetArrayInt(int * pArrInt, int iLen)
{
	memcpy_s(m_unionData.arr_iData, MAX_PATH*MAX_PATH * sizeof(int), pArrInt, iLen * sizeof(int));
	m_iLen = iLen;
	return true;
}

bool CGeneralParamInterior::GetArrayInt(int * pArrInt, int & iLen)
{
	pArrInt = m_unionData.arr_iData;
	iLen = m_iLen;
	return true;
}

bool CGeneralParamInterior::SetBool(bool bVal)
{
	return false;
}

bool CGeneralParamInterior::GetBool()
{
	return false;
}

bool CGeneralParamInterior::SetArrayBool(bool * pArrBool, int iLen)
{
	return false;
}

bool CGeneralParamInterior::GetArrayBool(bool * pArrBool, int & iLen)
{
	return false;
}

bool CGeneralParamInterior::SetPTCHAR(TCHAR * lpData)
{
	memcpy_s(m_unionData.lpszData, MAX_PATH*MAX_PATH * sizeof(TCHAR), lpData, MAX_PATH * sizeof(TCHAR));
	return true;
}

TCHAR * CGeneralParamInterior::GetPTCHAR()
{
	return m_unionData.lpszData;
}

bool CGeneralParamInterior::SetArrayPTCHAR(TCHAR ** lppData, int iLen)
{
	memcpy_s(m_unionData.arr_lpszData, MAX_PATH*MAX_PATH * sizeof(TCHAR), lppData, MAX_PATH * sizeof(TCHAR)*iLen);
	m_iLen = iLen;
	return true;
}

bool CGeneralParamInterior::GetArrayPTCHAR(TCHAR ** lppData, int & iLen)
{
	//memcpy_s(lppData, MAX_PATH * sizeof(TCHAR)*m_iLen, m_unionData.arr_lpszData, MAX_PATH * sizeof(TCHAR)*m_iLen);
	lppData = (TCHAR **)m_unionData.arr_lpszData;
	iLen = m_iLen;
	return false;
}
