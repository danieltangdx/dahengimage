// ExtenInfo.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ExtenInfo.h"
#include "ExtenInfoInterior.h"


// 这是导出变量的一个示例
EXTENINFO_API int nExtenInfo=0;

// 这是导出函数的一个示例。
EXTENINFO_API int fnExtenInfo(void)
{
    return 42;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 ExtenInfo.h
CExtenInfo::CExtenInfo()
{
	pInterior = (CExtenInfoInterior*)new CExtenInfoInterior();
}


CExtenInfo::~CExtenInfo()
{
	CExtenInfoInterior* pTempInterior = (CExtenInfoInterior*)pInterior;
	if (nullptr != pTempInterior)
		delete pTempInterior;
	pInterior = NULL;
}

void CExtenInfo::push_back(COptionalFunc* pOptionalFunc)
{
	CExtenInfoInterior* pTempInterior = (CExtenInfoInterior*)pInterior;
	if (nullptr != pTempInterior)
		pTempInterior->push_back(pOptionalFunc);
}

bool CExtenInfo::remove_last()
{
	CExtenInfoInterior* pTempInterior = (CExtenInfoInterior*)pInterior;
	if (nullptr != pTempInterior)
		pTempInterior->remove_last();
	return true;
}

void CExtenInfo::clear()
{
	CExtenInfoInterior* pTempInterior = (CExtenInfoInterior*)pInterior;
	if (nullptr != pTempInterior)
		pTempInterior->clear();
}

int CExtenInfo::GetFuncNum()
{
	CExtenInfoInterior* pTempInterior = (CExtenInfoInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->GetFuncNum();
	return 0;
}

COptionalFunc* CExtenInfo::GetFuncAddr(e_OptionalFuncCode iFuncCode)
{
	CExtenInfoInterior* pTempInterior = (CExtenInfoInterior*)pInterior;
	if (nullptr != pTempInterior)
		 return (COptionalFunc*)pTempInterior->GetFuncAddr((e_OptionalFuncCode)iFuncCode);
	return nullptr;
}

CGeneralParam & CExtenInfo::GetGeneralParam(e_OptionalFuncCode iFuncCode)
{
	// TODO: insert return statement here
	CExtenInfoInterior* pTempInterior = (CExtenInfoInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->GetGeneralParam(iFuncCode);
}

CExtenInfo& CExtenInfo::operator=(const CExtenInfo& extenInfo)
{
	CExtenInfoInterior* pTempInterior = (CExtenInfoInterior*)pInterior;
	CExtenInfoInterior* pTempInteriorCopy = (CExtenInfoInterior*)extenInfo.pInterior;
	if (nullptr != pTempInterior&&nullptr != pTempInteriorCopy)
	{
		*pTempInterior = *pTempInteriorCopy;
	}
	return *this;
}

/*************************************<>************************************[10/13/2017 ConanSteve]*/
e_OptionalFuncCode COptionalFunc::GetFuncCode()
{	
	COptionalFuncInterior* pTempInterior = (COptionalFuncInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->GetFuncCode();
	return (e_OptionalFuncCode)0;
}

bool COptionalFunc::SetFuncCode(e_OptionalFuncCode iFuncCode)
{
	COptionalFuncInterior* pTempInterior = (COptionalFuncInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->SetFuncCode(iFuncCode);
	return false;
}

/************************************************************************/

CMultiObjSaveImgPara::CMultiObjSaveImgPara()
{
	pInterior = (CMultiObjSaveImgParaInterior*)new CMultiObjSaveImgParaInterior(this);
}

CMultiObjSaveImgPara::~CMultiObjSaveImgPara()
{
	CMultiObjSaveImgParaInterior* pTempInterior = (CMultiObjSaveImgParaInterior*)pInterior;
	if (nullptr != pTempInterior)
		delete pTempInterior;
}

void CMultiObjSaveImgPara::SetImgNum(int iImgNum)
{
	CMultiObjSaveImgParaInterior* pTempInterior = (CMultiObjSaveImgParaInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->SetImgNum(iImgNum);
}

int CMultiObjSaveImgPara::GetImgNum()
{
	CMultiObjSaveImgParaInterior* pTempInterior = (CMultiObjSaveImgParaInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->GetImgNum();
}

void CMultiObjSaveImgPara::SetImgWidth(int iImgWidth)
{
	CMultiObjSaveImgParaInterior* pTempInterior = (CMultiObjSaveImgParaInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->SetImgWidth(iImgWidth);
}

int CMultiObjSaveImgPara::GetImgWidth()
{
	CMultiObjSaveImgParaInterior* pTempInterior = (CMultiObjSaveImgParaInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->GetImgWidth();
}

void CMultiObjSaveImgPara::SetImgHeight(int iImgHeight)
{
	CMultiObjSaveImgParaInterior* pTempInterior = (CMultiObjSaveImgParaInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->SetImgHeight(iImgHeight);
}

int CMultiObjSaveImgPara::GetImgHeight()
{
	CMultiObjSaveImgParaInterior* pTempInterior = (CMultiObjSaveImgParaInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->GetImgHeight();
}
void CMultiObjSaveImgPara::SetImgChannel(int iImgChannel)
{
	CMultiObjSaveImgParaInterior* pTempInterior = (CMultiObjSaveImgParaInterior*)pInterior;
	if (nullptr != pTempInterior)
		pTempInterior->SetImgChannel(iImgChannel);
}

int CMultiObjSaveImgPara::GetImgChannel()
{
	CMultiObjSaveImgParaInterior* pTempInterior = (CMultiObjSaveImgParaInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->GetImgChannel();
}

void CMultiObjSaveImgPara::SetIsCirCleSaveImg(bool bIsCirCleSaveImg)
{
	CMultiObjSaveImgParaInterior* pTempInterior = (CMultiObjSaveImgParaInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->SetIsCirCleSaveImg(bIsCirCleSaveImg);
}
bool CMultiObjSaveImgPara::GetIsCirCleSaveImg()
{
	CMultiObjSaveImgParaInterior* pTempInterior = (CMultiObjSaveImgParaInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->GetIsCirCleSaveImg();
}

void CMultiObjSaveImgPara::SetSaveImgFolderPath(LPCTSTR lpszSaveImgFolderPath, int iLen)
{
	CMultiObjSaveImgParaInterior* pTempInterior = (CMultiObjSaveImgParaInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->SetSaveImgFolderPath(lpszSaveImgFolderPath, iLen);
}

TCHAR* CMultiObjSaveImgPara::GetAddrOfSaveImgFolderPath()
{
	CMultiObjSaveImgParaInterior* pTempInterior = (CMultiObjSaveImgParaInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->GetAddrOfSaveImgFolderPath();
}

/*************************************<>************************************[9/22/2017 ConanSteve]*/
 CAlgSaveImgPara::CAlgSaveImgPara()
 {
	 pInterior = (CAlgSaveImgParaInterior*)new CAlgSaveImgParaInterior(this);
 }
 
 CAlgSaveImgPara::~CAlgSaveImgPara()
 {
	 CAlgSaveImgParaInterior* pTempInterior = (CAlgSaveImgParaInterior*)pInterior;
	 if (nullptr != pTempInterior)
		 delete pTempInterior;
 }
 
 void CAlgSaveImgPara::SetImgSN(int iImgSN)
 {
	 CAlgSaveImgParaInterior* pTempInterior = (CAlgSaveImgParaInterior*)pInterior;
	 if (nullptr != pTempInterior)
		 pTempInterior->SetImgSN(iImgSN);
 }
 
 int CAlgSaveImgPara::GetImgSN()
 {
	 CAlgSaveImgParaInterior* pTempInterior = (CAlgSaveImgParaInterior*)pInterior;
	 if (nullptr != pTempInterior)
		 return pTempInterior->GetImgSN();
 }
 
 void CAlgSaveImgPara::SetImgSavedCnt(int iImgSaveCnt)
 {
	 CAlgSaveImgParaInterior* pTempInterior = (CAlgSaveImgParaInterior*)pInterior;
	 if (nullptr != pTempInterior)
		 pTempInterior->SetImgSavedCnt(iImgSaveCnt);
 }
 
 int CAlgSaveImgPara::GetImgSavedCnt()
 {
	 CAlgSaveImgParaInterior* pTempInterior = (CAlgSaveImgParaInterior*)pInterior;
	 if (nullptr != pTempInterior)
		 return pTempInterior->GetImgSavedCnt();
 }
 
 void CAlgSaveImgPara::SetSaveImgFolderPath(LPCTSTR lpszSaveImgFolderPath, int iLen)
 {
	 CAlgSaveImgParaInterior* pTempInterior = (CAlgSaveImgParaInterior*)pInterior;
	 if (nullptr != pTempInterior)
		 pTempInterior->SetSaveImgFolderPath(lpszSaveImgFolderPath, iLen);
 }
 
 
 TCHAR* CAlgSaveImgPara::GetAddrOfSaveImgFolderPath()
 {
	 CAlgSaveImgParaInterior* pTempInterior = (CAlgSaveImgParaInterior*)pInterior;
	 if (nullptr != pTempInterior)
		 return pTempInterior->GetAddrOfSaveImgFolderPath();
 }
 
 CSetObjStatusWnd::CSetObjStatusWnd()
 {
	 pInterior = (CSetObjStatusWndInterior*)new CSetObjStatusWndInterior();
 }

 CSetObjStatusWnd::~CSetObjStatusWnd()
 {
	 CSetObjStatusWndInterior* pTempInterior = (CSetObjStatusWndInterior*)pInterior;
	 if (nullptr != pTempInterior)
		 delete pTempInterior;
 }
 
 void CSetObjStatusWnd::SetIsShow(bool bIsShow)
 {
	 CSetObjStatusWndInterior* pTempInterior = (CSetObjStatusWndInterior*)pInterior;
	 if (nullptr != pTempInterior)
		 pTempInterior->SetIsShow(bIsShow);
 }
 
 bool CSetObjStatusWnd::GetIsShow()
 {
	 CSetObjStatusWndInterior* pTempInterior = (CSetObjStatusWndInterior*)pInterior;
	 if (nullptr != pTempInterior)
		 return pTempInterior->GetIsShow();
 }
 /************************************************************************/
 
 CSetAlgObject::CSetAlgObject()
 {
	 pInterior = (CSetAlgObjectInterior*)new CSetAlgObjectInterior(this);
 }

 CSetAlgObject::~CSetAlgObject()
 {
	 CSetAlgObjectInterior* pTempInterior = (CSetAlgObjectInterior*)pInterior;
	 if (nullptr != pTempInterior)
		 delete pTempInterior;
 }
 
 void CSetAlgObject::SetAddrOfModelObj(void* pAddrOfModelObject)
 {
	 CSetAlgObjectInterior* pTempInterior = (CSetAlgObjectInterior*)pInterior;
	 if (nullptr != pTempInterior)
		 pTempInterior->SetAddrOfModelObj(pAddrOfModelObject);
 }
 
 void* CSetAlgObject::GetAddrOfModelObj()
 {
	 CSetAlgObjectInterior* pTempInterior = (CSetAlgObjectInterior*)pInterior;
	 if (nullptr != pTempInterior)
		return pTempInterior->GetAddrOfModelObj();
 }
 
 void CSetAlgObject::SetLoadModelCtrlPara(int iLoadModelCtrlPara)
 {
	 CSetAlgObjectInterior* pTempInterior = (CSetAlgObjectInterior*)pInterior;
	 if (nullptr != pTempInterior)
		 pTempInterior->SetLoadModelCtrlPara(iLoadModelCtrlPara);
 }
 
 int CSetAlgObject::GetLoadModelCtrlPara()
 {
	 CSetAlgObjectInterior* pTempInterior = (CSetAlgObjectInterior*)pInterior;
	 if (nullptr != pTempInterior)
		 return pTempInterior->GetLoadModelCtrlPara();
	 return 0;
 }

 void CSetAlgObject::SetAlgObjIndex(int iAlgObjIndex)
 {
	 CSetAlgObjectInterior* pTempInterior = (CSetAlgObjectInterior*)pInterior;
	 if (nullptr != pTempInterior)
		 pTempInterior->SetAlgObjIndex(iAlgObjIndex);
 }
 int CSetAlgObject::GetAlgObjIndex()
 {
	 CSetAlgObjectInterior* pTempInterior = (CSetAlgObjectInterior*)pInterior;
	 if (nullptr != pTempInterior)
		 return pTempInterior->GetAlgObjIndex();
	 return 0;
 }
/*************************************<2次Detect相关参数>*************************************/
CAlgDetectPara::CAlgDetectPara()
{
	pInterior = (CAlgDetectParaInterior*)new CAlgDetectParaInterior();
}

CAlgDetectPara::~CAlgDetectPara()
{
	CAlgDetectParaInterior* pTempInterior = (CAlgDetectParaInterior*)pInterior;
	if (nullptr != pTempInterior)
		delete pTempInterior;
}

void CAlgDetectPara::SetDetectTimes(int iDetectTimes)
{
	CAlgDetectParaInterior* pTempInterior = (CAlgDetectParaInterior*)pInterior;
	if (nullptr != pTempInterior)
		pTempInterior->SetDetectTimes(iDetectTimes);
}
int CAlgDetectPara::GetDetectTimes()
{
	CAlgDetectParaInterior* pTempInterior = (CAlgDetectParaInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->GetDetectTimes();
}

/*************************************<>************************************[10/12/2017 ConanSteve]*/

CAlgCopyWindow::CAlgCopyWindow()
{
	pInterior = (CAlgCopyWindowInterior*)new CAlgCopyWindowInterior();
}

CAlgCopyWindow::~CAlgCopyWindow()
{
	CAlgCopyWindowInterior* pTempInterior = (CAlgCopyWindowInterior*)pInterior;
	if (nullptr != pTempInterior)
		delete pTempInterior;
}

void CAlgCopyWindow::SetTimeOfWait(int iTimeOfWait)
{
	CAlgCopyWindowInterior* pTempInterior = (CAlgCopyWindowInterior*)pInterior;
	if (nullptr != pTempInterior)
		pTempInterior->SetTimeOfWait(iTimeOfWait);
}

int CAlgCopyWindow::GetTimeOfWait()
{
	CAlgCopyWindowInterior* pTempInterior = (CAlgCopyWindowInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->GetTimeOfWait();
}

void CAlgCopyWindow::SetTimeOfFirstDetect(int iTimeOfFirstDetect)
{
	CAlgCopyWindowInterior* pTempInterior = (CAlgCopyWindowInterior*)pInterior;
	if (nullptr != pTempInterior)
		pTempInterior->SetTimeOfFirstDetect(iTimeOfFirstDetect);
}

int CAlgCopyWindow::GetTimeOfFirstDetect()
{
	CAlgCopyWindowInterior* pTempInterior = (CAlgCopyWindowInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->GetTimeOfFirstDetect();
}

void CAlgCopyWindow::SetTimeOfSecondDetect(int iTimeOfSecondDetect)
{
	CAlgCopyWindowInterior* pTempInterior = (CAlgCopyWindowInterior*)pInterior;
	if (nullptr != pTempInterior)
		pTempInterior->SetTimeOfSecondDetect(iTimeOfSecondDetect);
}

int CAlgCopyWindow::GetTimeOfSecondDetect()
{
	CAlgCopyWindowInterior* pTempInterior = (CAlgCopyWindowInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->GetTimeOfSecondDetect();
}

void CAlgCopyWindow::SetTickOfPushQueue(LONGLONG llTickOfPushQueue)
{
	CAlgCopyWindowInterior* pTempInterior = (CAlgCopyWindowInterior*)pInterior;
	if (nullptr != pTempInterior)
		pTempInterior->SetTickOfPushQueue(llTickOfPushQueue);
}

LONGLONG CAlgCopyWindow::GetTickOfPushQueue()
{
	CAlgCopyWindowInterior* pTempInterior = (CAlgCopyWindowInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->GetTickOfPushQueue();
}

CAlgCopyWindow & CAlgCopyWindow::operator=(const CAlgCopyWindow & algCopyWindow)
{
	CAlgCopyWindowInterior* pTempInterior = (CAlgCopyWindowInterior*)pInterior;
	CAlgCopyWindowInterior* pCopyTempInterior = (CAlgCopyWindowInterior*)algCopyWindow.pInterior;
	if (nullptr != pTempInterior)
	{
		*pTempInterior = *pCopyTempInterior;
	}
	return *this;
}

CGeneralParam::CGeneralParam()
{
	pInterior = (CGeneralParamInterior*)new CGeneralParamInterior();
}

CGeneralParam::CGeneralParam(e_OptionalFuncCode iFuncCode)
{
	pInterior = (CGeneralParamInterior*)new CGeneralParamInterior(iFuncCode);
	
}

CGeneralParam::~CGeneralParam()
{
	CGeneralParamInterior* pTempInterior = (CGeneralParamInterior*)pInterior;
	if (nullptr != pTempInterior)
		delete pTempInterior;
}


bool CGeneralParam::SetInt(int iVal)
{
	CGeneralParamInterior* pTempInterior = (CGeneralParamInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->SetInt(iVal);
	return false;
}

int CGeneralParam::GetInt()
{
	CGeneralParamInterior* pTempInterior = (CGeneralParamInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->GetInt();
	return 0;
}

bool CGeneralParam::SetArrayInt(int * pArrInt, int iLen)
{
	CGeneralParamInterior* pTempInterior = (CGeneralParamInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->SetArrayInt(pArrInt, iLen);
	return false;
}

bool CGeneralParam::GetArrayInt(int * pArrInt, int & iLen)
{
	CGeneralParamInterior* pTempInterior = (CGeneralParamInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->GetArrayInt(pArrInt, iLen);
	return false;
}

bool CGeneralParam::SetBool(bool bVal)
{
	CGeneralParamInterior* pTempInterior = (CGeneralParamInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->SetBool(bVal);
	return false;
}

bool CGeneralParam::GetBool()
{
	CGeneralParamInterior* pTempInterior = (CGeneralParamInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->GetBool();
	return false;
}

bool CGeneralParam::SetArrayBool(bool * pArrBool, int iLen)
{
	CGeneralParamInterior* pTempInterior = (CGeneralParamInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->SetArrayBool(pArrBool, iLen);
	return false;
}

bool CGeneralParam::GetArrayBool(bool * pArrBool, int & iLen)
{
	CGeneralParamInterior* pTempInterior = (CGeneralParamInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->GetArrayBool(pArrBool, iLen);
	return false;
}

bool CGeneralParam::SetDouble(double lfVal)
{
	return false;
}

double CGeneralParam::GetDouble()
{
	return 0.0;
}

bool CGeneralParam::SetArrayDouble(double * plfVal, int iLen)
{
	return false;
}

bool CGeneralParam::GetArrayDouble(double * plfVal, int & iLen)
{
	return false;
}

bool CGeneralParam::SetPTCHAR(TCHAR * lpData)
{
	CGeneralParamInterior* pTempInterior = (CGeneralParamInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->SetPTCHAR(lpData);
	return false;
}

TCHAR * CGeneralParam::GetPTCHAR()
{
	CGeneralParamInterior* pTempInterior = (CGeneralParamInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->GetPTCHAR();
	return nullptr;
}

bool CGeneralParam::SetArrayPTCHAR(TCHAR ** lppData, int iLen)
{
	CGeneralParamInterior* pTempInterior = (CGeneralParamInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->SetArrayPTCHAR(lppData, iLen);
	return false;
}

bool CGeneralParam::GetArrayPTCHAR(TCHAR ** lppData, int & iLen)
{
	CGeneralParamInterior* pTempInterior = (CGeneralParamInterior*)pInterior;
	if (nullptr != pTempInterior)
		return pTempInterior->GetArrayPTCHAR(lppData, iLen);
	return false;
}
