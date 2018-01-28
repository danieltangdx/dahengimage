// ObjAlloc.cpp: implementation of the CObjAlloc class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ObjectAllocation.h"
#include "ObjAlloc.h"
#include "DataClass.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CObjAlloc::CObjAlloc()
{
	m_pInspObj = new CDataClass;
}

CObjAlloc::~CObjAlloc()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	delete (CDataClass*)m_pInspObj;
}

s_Status CObjAlloc::Init(s_CheckInitParam sInitParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return ((CDataClass*)m_pInspObj)->Init(sInitParam);
}

s_Status CObjAlloc::ShowDetail(int nCheckNS)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return ((CDataClass*)m_pInspObj)->ShowDetail(nCheckNS);
}

s_Status CObjAlloc::Free()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return ((CDataClass*)m_pInspObj)->Free();
}

s_Status CObjAlloc::Set(s_AlgModelInput sAlgModelInput)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return ((CDataClass*)m_pInspObj)->Set(sAlgModelInput);
}

s_Status CObjAlloc::Detect(s_CheckInParam sCheckInParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return ((CDataClass*)m_pInspObj)->Detect(sCheckInParam);
}
