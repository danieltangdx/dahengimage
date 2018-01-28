// ExtenInfo.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "ExtenInfo.h"
CExtenInfo::CExtenInfo()
{
	p_arrOptionalFunc = NULL;
	iCapacityOfFunc = 2;
	iSizeOfFunc = 0;
	p_arrOptionalFunc = new COptionalFunc[iCapacityOfFunc];
	memset(p_arrOptionalFunc, 0, sizeof(p_arrOptionalFunc)*iCapacityOfFunc);
}


CExtenInfo::~CExtenInfo()
{
	if (p_arrOptionalFunc)
		delete p_arrOptionalFunc;
	p_arrOptionalFunc = NULL;
}

void CExtenInfo::push_back(COptionalFunc Func)
{
	//vecOptionalFunc.push_back(Func);


	if (iSizeOfFunc == iCapacityOfFunc)
	{
		COptionalFunc* pNewOptionalFunc = new COptionalFunc[iCapacityOfFunc * 2];
		memcpy(pNewOptionalFunc, p_arrOptionalFunc, iCapacityOfFunc * sizeof(COptionalFunc*));
		delete[]p_arrOptionalFunc;
		p_arrOptionalFunc = pNewOptionalFunc;
		iCapacityOfFunc *= 2;
	}
	for (int i = 0; i < iSizeOfFunc; ++i)
	{
		if (p_arrOptionalFunc[i].GetFuncCode() == Func.GetFuncCode())
		{
			p_arrOptionalFunc[i] = Func;
			return;
		}
	}
	p_arrOptionalFunc[iSizeOfFunc++] = Func;
}

bool CExtenInfo::remove_last()
{
// 	if (vecOptionalFunc.size() > 0)
// 	{
// 		vecOptionalFunc.erase(vecOptionalFunc.end() - 1);
// 	}
	if (iSizeOfFunc > 0)
	{
		--iSizeOfFunc;
	}
	return true;
}

void CExtenInfo::clear()
{
	//vecOptionalFunc.clear();
	iSizeOfFunc = 0;
}

int CExtenInfo::GetFuncNum()
{
	return iSizeOfFunc;
}

COptionalFunc& CExtenInfo::GetFuncStruct(e_OptionalFuncCode iFuncCode)
{
	for (int i = 0; i < iSizeOfFunc; ++i)
	{
		if (iFuncCode == p_arrOptionalFunc[i].GetFuncCode())
		{
			return p_arrOptionalFunc[i];
		}
	}
	return COptionalFunc();
}

CExtenInfo& CExtenInfo::operator=(const CExtenInfo& extenInfo)
{
	this->iCapacityOfFunc = extenInfo.iCapacityOfFunc;
	this->iSizeOfFunc = extenInfo.iSizeOfFunc;
	if(p_arrOptionalFunc!=NULL)
	{
		delete p_arrOptionalFunc;
		p_arrOptionalFunc = NULL;
	}
	p_arrOptionalFunc = new COptionalFunc[iCapacityOfFunc];
	memcpy_s(p_arrOptionalFunc, sizeof(p_arrOptionalFunc), extenInfo.p_arrOptionalFunc, sizeof(p_arrOptionalFunc));
	return *this;
}



/*************************************<OptionalFunc>************************************[10/13/2017 ConanSteve]*/
COptionalFunc::COptionalFunc()
{
	memset(&m_unionData, 0, sizeof(m_unionData));
	this->iFuncCode = e_Fn_NONE;
}

COptionalFunc::COptionalFunc(e_OptionalFuncCode iFuncCode)
{
	memset(&m_unionData, 0, sizeof(m_unionData));
	this->iFuncCode = iFuncCode;
}
e_OptionalFuncCode COptionalFunc::GetFuncCode()
{	
	return this->iFuncCode;
}

COptionalFunc& COptionalFunc::SetFuncCode(e_OptionalFuncCode iFuncCode)
{
	this->iFuncCode = iFuncCode;
	return *this;
}

COptionalFunc & COptionalFunc::SetBool(bool bVal)
{
	// TODO: insert return statement here
	this->m_unionData.bData = bVal;
	return *this;
}

bool COptionalFunc::GetBool()
{
	return m_unionData.bData;
}

COptionalFunc& COptionalFunc::SetInt(int iVal)
{
	m_unionData.iData = iVal;
	return *this;
}

int COptionalFunc::GetInt()
{
	return m_unionData.iData;
}

COptionalFunc & COptionalFunc::SetPointer(void * ptr)
{
	m_unionData.pPtr = ptr;
	return *this;
}

void * COptionalFunc::GetPointer()
{
	return m_unionData.pPtr;
}

COptionalFunc & COptionalFunc::SetDouble(double lfVal)
{
	m_unionData.lfData = lfVal;
	return *this;
}

double COptionalFunc::GetDouble()
{
	return m_unionData.lfData;
}

COptionalFunc & COptionalFunc::SetLonglong(LONGLONG ldVal)
{
	m_unionData.ldData = ldVal;
	return *this;
}

LONGLONG COptionalFunc::GetLonglong()
{
	return m_unionData.ldData;
}

COptionalFunc & COptionalFunc::SetArrayTCHAR(TCHAR * lpData)
{
	memcpy_s(m_unionData.lpszData, MAX_PATH, lpData, MAX_PATH);
	return *this;
}

TCHAR * COptionalFunc::GetArrayTCHAR()
{
	return m_unionData.lpszData;
}

/************************************************************************/



