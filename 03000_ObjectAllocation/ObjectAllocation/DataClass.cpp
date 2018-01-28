// DataClass.cpp: implementation of the CDataClass class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ObjectAllocation.h"
#include "DataClass.h"
#include <afxmt.h>
#include "typeinfo"
// #include "excel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


#define SELFMSG_CREATESTDLG	WM_USER+15
#define SELFMSG_CHANGEST	WM_USER+16

CDataClass::CDataClass()
{

	m_strAppDir = CExFn::GetCurrentAppPath();
	m_strAlgconfigDir.Format(_T("%s\\Config\\AlgConfig.ini"), m_strAppDir);

	LARGE_INTEGER iLarge{ 0 };
	QueryPerformanceFrequency(&iLarge);
	m_dFreq = (double)iLarge.QuadPart;
	m_fileDebugInfo.Open(CExFn::GetCurrentAppPath() + _T("\\DebugInfo.txt"));
	fill(m_pCheckStatusParam.begin(), m_pCheckStatusParam.end(), nullptr);
	fill(m_iGroupGoodCnt.begin(), m_iGroupGoodCnt.end(), 0);
	fill(m_iGroupBadCnt.begin(), m_iGroupBadCnt.end(), 0);
	fill(m_iKickErrorCount.begin(), m_iKickErrorCount.end(), 0);
	fill(m_iKickGoodCnt.begin(), m_iKickGoodCnt.end(), 0);

}


CDataClass::~CDataClass()
{

}

// BEGIN_MESSAGE_MAP(CTestExeDlg, CDialog)
// //{{AFX_MSG_MAP(CTestExeDlg)
// ON_WM_SYSCOMMAND()
// ON_WM_PAINT()
// ON_WM_QUERYDRAGICON()
// ON_BN_CLICKED(IDC_BTN_SHOW, OnBtnShow)
// ON_BN_CLICKED(IDC_BTN_HIDE, OnBtnHide)
// ON_MESSAGE(99, OnMsgInit)
// //}}AFX_MSG_MAP
// END_MESSAGE_MAP()

s_Status CDataClass::Init(s_CheckInitParam sInitParam)
{
	int i = 0, j = 0;
	s_Status sError;
	sError.pThis = m_pCallThis;

	// 各工位对象个数
	CString strInitFile{ _T("") };
	strInitFile.Format(_T("%s"), sInitParam.pFilePath);
	m_CheckCallback = sInitParam.KickCallbackFunc;
	m_StatusCallback = sInitParam.StatusCallbackFunc;
	m_pCallThis = sInitParam.pThis;
	m_iCallType = sInitParam.iCallType;

	if (sInitParam.iReserve1 != e_MachineType_Capsule && sInitParam.iReserve1 != e_MachineType_Liquor)
	{
		m_iMachineType = e_MachineType_Capsule;
	}
	else
	{
		m_iMachineType = sInitParam.iReserve1;
	}
	// 测试参数是否合法 不合法1，合法0;
	if (!InitObjects(strInitFile, sInitParam.iCallType))
	{
		sError.iStatusType = STATUS_ILG;
		sError.pErrorInfo = GetErrorInfo(_T("Insufficient Construction Information"));
		return sError;
	}
	if (!m_bEnableInitObjectAndCheck)
		return sError;
	// 若demo调用，无需分配内存
	if (e_CallType_Demo != m_iCallType)
	{
		// 各对象、工位 是否在检测或者画图
		m_plObjBusyTimes = new long[m_iCheckGroupNum + 1]{ 0 };
		m_pbIsDrawing = make_unique<bool[]>(m_iCheckGroupNum + 1);
		fill(m_pbIsDrawing.get(), m_pbIsDrawing.get() + m_iCheckGroupNum + 1, false);

		for (i = 0; i < m_iCheckGroupNum; i++)
		{
			//m_nObjIndex[i] = new int[m_pnObjectNumber[i] + 1]{e_DetectStatus_NoChecking};//队列是否空闲，0空闲，1忙
			fill(m_nObjIndex[i].begin(), m_nObjIndex[i].end(), e_DetectStatus_NoChecking);
		}

		m_ppCacheUseCnt = new int*[m_iCheckGroupNum];

		// 初始化图像数据和检测线程参数
		/************************************************************************/
		//**[8/2/2017 ConanSteve]**:  2次Detect创建额外
		if (e_ObjDetectMode_2 == m_iObjDetectMode)
		{
			m_ppCheckInParamEx = new BYTE*[m_iCheckGroupNum];
			m_ppCheckOutParamEx = new BYTE*[m_iCheckGroupNum];
			m_ppCheckStatusParamEx = new BYTE*[m_iCheckGroupNum];
			m_sppCheckInParamEx = new s_CheckInParam *[m_iCheckGroupNum];
			m_sppCheckOutParamEx = new s_CheckOutParam*[m_iCheckGroupNum];
			m_sppCheckStatusParamEx = new s_Status*[m_iCheckGroupNum];
		}
		/************************************************************************/
		for (i = 0; i < m_iCheckGroupNum; i++)
		{
			// 创建触发检测线程内核事件
			m_hImageInspectStart[i] = CreateEvent(NULL, FALSE, FALSE, NULL);

			m_pWriteDataStatus[i] = new int[m_iGroupCheckQueueNum[i]];
			memset(m_pWriteDataStatus[i], 0, sizeof(int)*m_iGroupCheckQueueNum[i]);
			m_pDetectStatus[i] = new int[m_iGroupCheckQueueNum[i]];
			memset(m_pDetectStatus[i], 0, sizeof(int)*m_iGroupCheckQueueNum[i]);
			m_pCircleSN[i] = new int[m_iGroupCheckQueueNum[i]];
			memset(m_pCircleSN[i], 0, sizeof(int)*m_iGroupCheckQueueNum[i]);
			// 
			m_pImageData[i] = new BYTE[m_iGroupImageSize[i] * m_iGroupCheckQueueNum[i]];
			memset(m_pImageData[i], 0, sizeof(BYTE)*m_iGroupImageSize[i] * m_iGroupCheckQueueNum[i]);
			m_pCheckInParam[i] = new BYTE[m_iGroupCheckQueueNum[i] * m_iGroupCheckInSize[i]];
			memset(m_pCheckInParam[i], 0, sizeof(BYTE)*m_iGroupCheckQueueNum[i] * m_iGroupCheckInSize[i]);
			m_pCheckOutParam[i] = new BYTE[m_iGroupCheckQueueNum[i] * m_iGroupCheckOutSize[i]];
			memset(m_pCheckOutParam[i], 0, sizeof(BYTE)*m_iGroupCheckQueueNum[i] * m_iGroupCheckOutSize[i]);
			m_pCheckStatusParam[i] = new BYTE[m_iGroupCheckQueueNum[i] * m_iGroupCheckStatusSize[i]];

			memset(m_pCheckStatusParam[i], 0, sizeof(BYTE)*m_iGroupCheckQueueNum[i] * m_iGroupCheckStatusSize[i]);
			m_spCheckInParam[i] = new s_CheckInParam[m_iGroupCheckQueueNum[i]];
			memset(m_spCheckInParam[i], 0, sizeof(s_CheckInParam)*m_iGroupCheckQueueNum[i]);
			m_spCheckOutParam[i] = new s_CheckOutParam[m_iGroupCheckQueueNum[i]];
			memset(m_spCheckOutParam[i], 0, sizeof(s_CheckOutParam)*m_iGroupCheckQueueNum[i]);
			m_spCheckStatusParam[i] = new s_Status[m_iGroupCheckQueueNum[i]];
			memset(m_spCheckStatusParam[i], 0, sizeof(s_Status)*m_iGroupCheckQueueNum[i]);
			m_ppCacheUseCnt[i] = new int[m_iGroupCheckQueueNum[i]]{ 0 };
			memset(m_ppCacheUseCnt[i], 0, sizeof(int)*m_iGroupCheckQueueNum[i]);

			m_pResetWriteStatus[i] = shared_ptr<s_ResetWriteStatus>(new s_ResetWriteStatus[m_iGroupCheckQueueNum[i]], [](s_ResetWriteStatus *p)
			{
				delete[] p;
			});

			for (j = 0; j < m_iGroupCheckQueueNum[i]; j++)
			{
				m_spCheckInParam[i][j].pImageData = (char*)m_pImageData[i] + j * m_iGroupImageSize[i];
				m_spCheckInParam[i][j].pParam = m_pCheckInParam[i] + j * m_iGroupCheckInSize[i];
				m_spCheckOutParam[i][j].pParam = m_pCheckOutParam[i] + j * m_iGroupCheckOutSize[i];
				m_spCheckStatusParam[i][j].pParam = m_pCheckStatusParam[i] + j * m_iGroupCheckStatusSize[i];
			}
			/************************************************************************/
			//**[8/2/2017 ConanSteve]**:  2次Detect创建额外参数
			if (e_ObjDetectMode_2 == m_iObjDetectMode)
			{
				m_ppCheckInParamEx[i] = new BYTE[m_pnObjectNumber[i] * m_iGroupCheckInSize[i]];
				m_ppCheckOutParamEx[i] = new BYTE[m_pnObjectNumber[i] * m_iGroupCheckOutSize[i]];
				m_ppCheckStatusParamEx[i] = new BYTE[m_pnObjectNumber[i] * m_iGroupCheckStatusSize[i]];
				m_sppCheckInParamEx[i] = new s_CheckInParam[m_pnObjectNumber[i]];
				m_sppCheckOutParamEx[i] = new s_CheckOutParam[m_pnObjectNumber[i]];
				m_sppCheckStatusParamEx[i] = new s_Status[m_pnObjectNumber[i]];

				for (int j = 0; j < m_pnObjectNumber[i]; ++j)
				{
					m_sppCheckInParamEx[i][j].pParam = m_ppCheckInParamEx[i] + j * m_iGroupCheckInSize[i];
					m_sppCheckOutParamEx[i][j].pParam = m_ppCheckOutParamEx[i] + j * m_iGroupCheckOutSize[i];
					m_sppCheckStatusParamEx[i][j].pParam = m_ppCheckStatusParamEx[i] + j * m_iGroupCheckStatusSize[i];

				}

			}
			/************************************************************************/

		}
		// 存放检测结果及状态
		for (i = 0; i < m_iKickQueueNum; i++)
		{
			for (j = 0; j < 256; j++)
			{
				m_pKickResult[i][j] = new BYTE[m_iKickQueueSize[i]];
				m_pbKickWrite[i][j] = new BYTE[m_iKickResultNum[i]];
			}
		}

		// 开启检测线程
		CWinThread* pThread = NULL;
		for (i = 0; i < m_iCheckThreadNum; i++)
		{
			pThread = NULL;
			m_sThreadParam[i].pThis = this;
			m_sThreadParam[i].iThreadID = i;
			pThread = AfxBeginThread((AFX_THREADPROC)ImageInspectionThread, &m_sThreadParam[i], THREAD_PRIORITY_HIGHEST);
			if (NULL == pThread)
			{
				DWORD dError = GetLastError();
				sError.iStatusType = STATUS_ILG;
				char chInfo[c_iMaxStringLen]{ 0 };
				sprintf_s(chInfo, "CObjAlloc::Initialize Init Image Inspection Thread Failed!\r\nError Code: %d", dError);
				sError.pErrorInfo = GetErrorInfo(chInfo);
				return sError;
			}
		}

		// 开启剔废线程
		pThread = NULL;
		pThread = AfxBeginThread((AFX_THREADPROC)KickThread, this, THREAD_PRIORITY_NORMAL);
		if (NULL == pThread)
		{
			DWORD dwError = GetLastError();
			sError.iStatusType = STATUS_ILG;
			sError.pErrorInfo = GetErrorInfo("CObjAlloc::Initialize Init Kick Thread Failed!");
			return sError;
		}

		/*************************************<初始化状态窗口>*************************************/
		m_pObjStatusDlg = NULL;
		// 		if (!m_pObjStatusDlg)
		// 		{
		// 			CRect rcDlg{ 0 }, rcNewDlg{0};
		// 			int iLeft = 0, iTop = 0;
		// 			int iWScreen = GetSystemMetrics(SM_CXSCREEN);
		// 			int iHScreen = GetSystemMetrics(SM_CYSCREEN);
		// 
		// 			m_pObjStatusDlg = make_shared<CDlgObjStatus>(new CDlgObjStatus);
		// 			m_pObjStatusDlg->InitData(m_iCheckGroupNum, m_pnObjectNumber);
		// 			m_pObjStatusDlg->Create(IDD_DLG_OBJSTATUS);
		// 			m_pObjStatusDlg->GetWindowRect(&rcDlg);
		// 			iLeft = (iWScreen - rcDlg.Width()) / 2;
		// 			iTop = (iHScreen = rcDlg.Height()) / 2;
		// 			m_pObjStatusDlg->MoveWindow(iLeft, iTop, rcDlg.Width(), rcDlg.Height());
		// 			m_pObjStatusDlg->ShowWindow(SW_HIDE);
		// 		}
		m_pDlgRunInfo = new CDlgRunInfo(NULL, m_bEnableXCGUI);
		m_pDlgRunInfo->Create(IDD_DIALOG_RunInfo);
		if (m_bShowObjRunInfoWnd)
			m_pDlgRunInfo->ShowWindowEx(SW_SHOW);
		/************************************************************************/
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 算法相关

	// 各工位窗口数
	// 载入动态库
	CString strDllPath{ _T("") };
	for (i = 0; i < m_iCheckGroupNum; ++i)
	{
		if (-1 != m_iSingleTest)
		{
			if (i != m_iSingleTest)
				continue;
		}

		strDllPath = m_strArrDllPath.GetAt(i);
		if (strDllPath == _T("NULL"))
		{
			continue;
		}

		if (m_pnObjectNumber[i] < 0 || m_pnObjectNumber[i]>100)
		{
			sError.iStatusType = STATUS_ILG;
			TCHAR chInfo[c_iMaxStringLen]{ 0 };
			_tprintf_s(chInfo, tr("CObjAlloc::Initialize Wrong Number of Objects of Camera%d"), i + 1);
			sError.pErrorInfo = GetErrorInfo(chInfo);
			return sError;
		}
		//strDllPath = m_strArrDllPath.GetAt(n);
		// 		BOOL bExist = PathFileExists(strDllPath);
		// 		SetCurrentDirectory(_T("D:\\Workspace\\Projects\\DahengProjects\\CheckSystem\\CheckSystem2\\bin"));
		HMODULE hDll = LoadLibrary(strDllPath);
		if (hDll)
		{
			// 创建检测类
			pFunc_Create createobj;
			createobj = (pFunc_Create)(GetProcAddress(hDll, _T("create_instance")));
			if (createobj)
			{
				if (m_bEnableInitObjectAndCheck)
				{
					CInspBase* pInsp = createobj(m_pnObjectNumber[i]);
					if (NULL == pInsp)
					{
						sError.iStatusType = STATUS_ILG;
						sError.pErrorInfo = GetErrorInfo(tr("CObjAlloc::Initialize Create Inspection Objects Failed!"));
						FreeLibrary(hDll);
						return sError;
					}
					m_iIdleJob[i] = (int)pInsp;
				}
			}
			else
			{
				AfxMessageBox(CExFn::GetLastErrorMsg());
				sError.iStatusType = STATUS_ILG;
				char chInfo[c_iMaxStringLen]{ 0 };
				sprintf_s(chInfo, tr("CObjAlloc::Initialize Execute Create Function Fail Camera%d"), i + 1);
				sError.pErrorInfo = GetErrorInfo(chInfo);
				FreeLibrary(hDll);
				return sError;
			}
		}
		else
		{
			sError.pErrorInfo = GetErrorInfo(CExFn::GetLastErrorMsg().GetBuffer());
			sError.iStatusType = STATUS_ILG;
			return sError;
		}
	}





	// 挨个初始化
	int nObjIdx = 0;
	try
	{
		// 只有检测端调用初始化函数Init()
		if (e_CallType_Demo != sInitParam.iCallType)
		{
			for (i = 0; i < m_iCheckGroupNum; ++i)
			{
				if (-1 != m_iSingleTest)
				{
					if (i != m_iSingleTest)
						continue;
				}

				strDllPath = m_strArrDllPath.GetAt(i);
				if (strDllPath == _T("NULL"))
				{
					continue;
				}
				s_CheckInitSingleParam sSingleInit;
				sSingleInit.iCheckSN = i;//+1;
										 // 				sSingleInit.pParam = ((DWORD*)sInitParam.pParam+i);  //每次调用传的参数都是一样的
										 // 				sSingleInit.pParam = sInitParam.pParam;				 //[2016-5-23] 邵建超要求每次传入参数地址不同

				if (m_iMachineType == e_MachineType_Capsule)
				{
					sSingleInit.pParam = sInitParam.pParam;				//胶囊使用
				}
				else if (m_iMachineType == e_MachineType_Liquor)
				{
					sSingleInit.pParam = (int*)sInitParam.pParam + i;		//[2016-5-23] 邵建超要求每次传入参数地址不同
				}

				for (int nObjIdx = 0; nObjIdx < m_pnObjectNumber[i]; ++nObjIdx)
				{
					CInspBase* pInsp = (CInspBase*)m_iIdleJob[i] + nObjIdx;
					sSingleInit.iObjIndex = nObjIdx;
					if (m_bEnableInitObjectAndCheck)
						sError = pInsp->Init(sSingleInit);
					if (sError.iStatusType != STATUS_NOR)
					{
						return sError;
					}
				}
			}
		}
	}
	catch (...)
	{
		DWORD dError = GetLastError();
		sError.iStatusType = STATUS_ILG;
		char chInfo[MAX_CHAR_LENGTH]{ 0 };
		sprintf(chInfo, "CObjAlloc::Initialize Init Fail Check%d Object%d\r\nError Code: %d", i + 1, nObjIdx + 1, dError);
		sError.pErrorInfo = GetErrorInfo(chInfo);
		return sError;
	}

	//详情显示界面
	//	DisplayObjectsStatus(SW_SHOW);

	/************************************************************************/
	/*       开启多对象存图线程                [6/6/2017 ConanSteve]                         */
	if (e_CallType_Demo != m_iCallType)
	{
		m_hSaveImgs = (HANDLE)_beginthreadex(NULL, 0, (unsigned(_stdcall*)(void*))FnThreadSaveImg, this, 0, NULL);
		/** 触发检测事件              [6/12/2017 ConanSteve]**/
		m_hEventSaveImgs = CreateEvent(NULL, FALSE, FALSE, NULL);
		CloseHandle(m_hSaveImgs);
	}
	/************************************************************************/
	ReseMultiObject();
	return sError;
}


s_Status CDataClass::ShowDetail(int iCheckSN)
{
	s_Status sError;
	ZeroMemory(&sError, sizeof(s_Status));
	sError.iStatusType = STATUS_NOR;
	sError.pThis = m_pCallThis;

	if (0 == iCheckSN)
	{
		m_bShowStatus = TRUE;
		m_pObjStatusDlg->ShowWindow(SW_SHOW);

	}
	else
	{
		m_bShowStatus = FALSE;
		m_pObjStatusDlg->ShowWindow(SW_HIDE);

	}

	return sError;
}

s_Status CDataClass::Set(s_AlgModelInput sAlgModelInput)
{
	s_Status sError;
	if (!m_bEnableInitObjectAndCheck)
		return sError;
	ZeroMemory(&sError, sizeof(s_Status));
	sError.iStatusType = STATUS_NOR;
	sError.pThis = m_pCallThis;

	int i = 0, j = 0;

	int iCheckSN = sAlgModelInput.iCheckSN;
	int iOpera = sAlgModelInput.iOperSig;
	int iImageType = sAlgModelInput.iReserve1;	//保存图像时用于指定存图类型
	int iImageCount = sAlgModelInput.iReserve2;	//保存图像时用于指定存图数量

	ASSERT(iCheckSN >= 0 && iCheckSN <= m_iCameraNum);

	if (-1 != m_iSingleTest)
	{
		if (iCheckSN != m_iSingleTest)
			return sError;
	}
	switch (iOpera)
	{
	case e_AlgModelInputOper_LoadModel:
	{
		fill(m_arr_bIsLoadModel.begin(), m_arr_bIsLoadModel.end(), true);
		for (i = 0; i < m_pnObjectNumber[iCheckSN]; ++i)
		{
			if (!m_bEnableInitObjectAndCheck)
				continue;
			CInspBase* pInsp = (CInspBase*)(m_iIdleJob[iCheckSN]) + i;
			CExtenInfo extenInfo;
			extenInfo.push_back(COptionalFunc(e_Fn_AlgModelInput_CheckObjIndex).SetInt(i));
			extenInfo.push_back(COptionalFunc(e_Fn_AlgModelInput_FirstCheckObjAddr).SetPointer((void*)m_iIdleJob));
			//sAlgModelInput.iReserve4 = (int)(&extenInfo);
			{

				int iTemp = 0;
				while (m_nObjIndex[iCheckSN][i] == e_DetectStatus_Checking)
				{
					Sleep(1);
					iTemp++;
					if (iTemp > 2000)
					{
						fill(m_arr_bIsLoadModel.begin(), m_arr_bIsLoadModel.end(), false);
						sError.iStatusType = STATUS_ALG_ABNORMAL;
						return sError;
					}
				}
			}
			sError = pInsp->Set(sAlgModelInput);
			if (sError.iStatusType != STATUS_NOR)
				return sError;
		}
		fill(m_arr_bIsLoadModel.begin(), m_arr_bIsLoadModel.end(), false);
	}
	break;
	case e_AlgModelInputOper_SaveImage:
	{
		//查看是否存图
		if (iImageCount > 0)
		{
			m_csSaveImage[iCheckSN].Lock();
			m_bGroupSaveImage[iCheckSN] = true;
			m_iGroupSaveImageType[iCheckSN] = iImageType;
			m_iGroupSaveImageCount[iCheckSN] = 0;
			m_iGroupSaveImageCount_SET[iCheckSN] = iImageCount;
			m_csSaveImage[iCheckSN].Unlock();

			m_iSaveImageStatus = 1;

			for (i = 0; i < MAX_CAMERA_COUNT; i++)
			{
				m_iSaveImageNum[i] = 1;
			}
		}
		else
		{
			m_csSaveImage[iCheckSN].Lock();
			m_bGroupSaveImage[iCheckSN] = false;
			m_iGroupSaveImageCount[iCheckSN] = 0;
			m_iGroupSaveImageCount_SET[iCheckSN] = 0;
			m_csSaveImage[iCheckSN].Unlock();
		}
		m_iSaveImgObj = sAlgModelInput.iReserve3;
		m_iSaveImgNamingRegulation = sAlgModelInput.iReserve4;

		m_srwlSaveImgHistory.Lock();
		while (!m_queSaveImgHistory.empty())
			m_queSaveImgHistory.pop();
		m_srwlSaveImgHistory.Unlock();
	}
	break;
	//**[7/31/2017 ConanSteve]**:  查看多对象运行信息
	case e_AlgModelInputOper_UseExtenInfoFunc:
	{
		CExtenInfo* pExtenInfo{ NULL };
		pExtenInfo = (CExtenInfo*)sAlgModelInput.iReserve4;
		if (NULL == pExtenInfo)
			return sError;
		{
			m_bShowObjRunInfoWnd = pExtenInfo->GetFuncStruct(e_Fn_AlgModelInput_WhetherShowObjectRunInfo).GetBool();
			if (m_bShowObjRunInfoWnd)
			{
				m_pDlgRunInfo->ShowWindowEx(SW_SHOW);
			}
			else
			{
				m_pDlgRunInfo->ShowWindowEx(SW_HIDE);
			}
		}
	}
	break;
	case e_AlgModelInputOper_Reset:
	{
		ReseMultiObject();
	}
	break;
	default:
	{
		for (i = 0; i < m_pnObjectNumber[iCheckSN]; ++i)
		{
			if (!m_bEnableInitObjectAndCheck)
				continue;
			CInspBase* pInsp = (CInspBase*)(m_iIdleJob[iCheckSN]) + i;
			if (NULL != pInsp)
				sError = pInsp->Set(sAlgModelInput);
			if (sError.iStatusType != STATUS_NOR)
				return sError;
		}
	}
	break;
	}



	return sError;
}

s_Status CDataClass::Free()
{
	s_Status sError;
	if (!m_bEnableInitObjectAndCheck)
		return sError;
	ZeroMemory(&sError, sizeof(s_Status));
	sError.iStatusType = STATUS_NOR;
	sError.pThis = m_pCallThis;

	ReleaseThreadData();

	return sError;
}

void CDataClass::DisplayObjectsStatus(int nCmdShow) //对象状态写文件或显示
{
	// 	if (nCmdShow == SW_HIDE)
	// 	{
	// 		m_bShowStatus = FALSE;
	// 	}
	// 	else if (nCmdShow == SW_SHOW)
	// 	{
	// 		m_bShowStatus = TRUE;
	// 	}
}
CString ConvertErrorCodeToString(DWORD ErrorCode)
{
	CString sErrMsg{ _T("") };
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, ErrorCode, 0, (PTSTR)sErrMsg.GetBuffer(1000), 0, NULL);
	sErrMsg.ReleaseBuffer();
	return sErrMsg;
}
BOOL CDataClass::InitObjects(CString strInitFile, int iCallType)//测试动态库是否可以调用
{
	/************************************************************************/
	/*                       [6/6/2017 ConanSteve]                         */
	/************************************************************************/
	//从配置文件读取，检测路数，动态库名
	//array<char, MAX_PATH> chBuf{0};
	CString strSegment{ _T("") };
	CString sVal{ _T("") };
	strSegment.Format(_T("Inspection Parameters"));
	GetPrivateProfileString(strSegment, "CheckNum", "5", sVal.GetBuffer(80), 80, strInitFile);
	sVal.ReleaseBuffer();
	/************************************************************************/
	m_iniAlgCfg.Init(CExFn::GetCurrentAppPath() + _T("\\Config\\AlgConfig.ini"));
	m_bEnableExtenInfo = m_iniAlgCfg.GetBoolEx(_T("SystemConfiguration"), _T("EnableExtenInfo"), false, _T("是否使用多对象扩展信息结构体"));
	m_bEnableOvertimeCallback = m_iniAlgCfg.GetBoolEx(_T("Systemconfiguration"), _T("EnableOvertimeCallback"), true, _T("是否使用超时回调"));
	m_iSysDetectMode = m_iniAlgCfg.GetIntEx(_T("SystemConfiguration"), _T("SystemDetectMode"), e_SysDetectMode_Multi, _T("系统检测模式,1为多对象收图检测。0为系统收图"));
	m_iObjDetectMode = m_iniAlgCfg.GetIntEx(_T("SystemConfiguration"), _T("ObjectDetectMode"), e_ObjDetectMode_1, _T("0为1次Detect，1为2次Detect"));
	m_bShowObjRunInfoWnd = m_iniAlgCfg.GetBoolEx(_T("SystemConfiguration"), _T("EnableObjectRunInfoWindow"), false, _T("是否打开窗口运行信息"));
	m_bEnableXCGUI = m_iniAlgCfg.GetBoolEx(_T("SystemConfiguration"), _T("EnableXCGUI"), false, _T("是否使用XCGUI组件"));
	m_iSingleTest = m_iniAlgCfg.GetIntEx(_T("SystemConfiguration"), _T("SingleTest"), -1, _T("进行单路测试，-1时，正常运行,从0开始"));
	m_bEnableInitObjectAndCheck = m_iniAlgCfg.GetBoolEx(_T("SystemConfiguration"), _T("EnableInitMultiObjectAndCheck"), true, _T("是否执行多对象的初始化和算法检测工作"));
	/************************************************************************/
	m_iCheckGroupNum = _ttoi(sVal.GetBuffer());
	if (m_iCheckGroupNum <= 0)
		return FALSE;

	int iTemp = 0;

	//从配置文件读对象个数
	CString strAlg{ _T("") };
	int nCheckObjNum = 0;
	int i = 0, j = 0;
	m_pnObjectNumber = new int[m_iCheckGroupNum + 1]{ 0 };
	for (i = 0; i < m_iCheckGroupNum; ++i)
	{
		if (iCallType == e_CallType_Demo)//demo调用
		{
			m_pnObjectNumber[i] = 1;
		}
		else
		{
			strAlg.Format(_T("CheckObjNum_%d"), i + 1);
			GetPrivateProfileString(strSegment, strAlg, _T("1"), sVal.GetBuffer(80), 80, strInitFile);
			sVal.ReleaseBuffer();
			nCheckObjNum = _ttoi(sVal.GetString());
			m_pnObjectNumber[i] = nCheckObjNum;
		}
	}

	// 尝试找到每个动态库的接口
	CString strDllKeyName{ _T("") }, strDllPath{ _T("") };
	CString strBuf{ _T("") };
	CString sDebugInfo{ _T("") };
	for (i = 0; i < m_iCheckGroupNum; ++i)
	{
		strDllKeyName.Format(_T("CheckDllName_%d"), i + 1);
		sDebugInfo.Format(tr("CheckDllName_%d does not exist!"), i + 1);
		sVal = m_iniAlgCfg.GetStringEx(strSegment, strDllKeyName, sVal, _T("NULL"));
		if (sVal == _T("NULL") || _T("") == sVal)//当前路动态库为空，读下一个
		{
			m_strArrDllPath.InsertAt(i, _T("NULL"));
			AfxMessageBox(sDebugInfo);
			return false;
		}
		strDllPath.Format(_T("%s\\%s"), m_strAppDir.GetBuffer(0), sVal.GetString());
		if (0 == strDllPath.Right(4).CompareNoCase(_T(".dll")))
			strDllPath = strDllPath.Left(strDllPath.GetLength() - 4);
#ifdef DEBUG
		strDllPath += _T("D");
#endif

		if (strDllPath.Right(4) != _T(".dll") && strDllPath.Right(4) != _T(".DLL"))
			strDllPath += _T(".dll");

		bool bRet = PathFileExists(strDllPath.GetBuffer());
		if (false == bRet)
		{
			CString sInfo{ _T("") };
			sInfo.Format(tr("算法检测动态库_%d\"%s\" 不存在!"), i + 1, strDllPath);
			AfxMessageBox(sInfo);
			return false;
		}
		else
			m_strArrDllPath.InsertAt(i, strDllPath);
	}

	// 读取检测线程个数
	strSegment.Format(_T("Basic Parameters"));
	m_iCheckThreadNum = m_iniAlgCfg.GetIntEx(_T("Basic Parameters"), _T("CheckThreadNum"), 10, _T("检测线程个数"));

	// 读取相机个数

	m_iCameraNum = m_iniAlgCfg.GetIntEx(_T("Basic Parameters"), _T("CameraNum"), 1, _T("相机个数"));

	// 读取剔废队列个数
	strSegment.Format(_T("Basic Parameters"));
	m_iKickQueueNum = m_iniAlgCfg.GetIntEx(_T("Basic Parameters"), _T("KickQueueNum"), 3);

	/** Last checking group[5/3/2017 ConanSteve]**/
	int iLastGroup = MAX_IMAGE_COUNT;
	/** the current index in checking group              [5/3/2017 ConanSteve]**/
	int	iGroupIndex = 0;
	int iLastKick = MAX_IMAGE_COUNT, iKickIndex = 0, iLastKickSize = 0;
	int iKickResult = 0, iLastKickResult = 0;
	for (i = 0; i < m_iCameraNum; i++)
	{
		// 获取图像宽高位数
		strSegment.Format(_T("Camera%d Parameters"), i + 1);
		strAlg.Format(_T("Width"));
		GetPrivateProfileString(strSegment, strAlg, _T("1292"), sVal.GetBuffer(80), 80, strInitFile);
		sVal.ReleaseBuffer();
		m_iImageWidth[i] = _ttoi(sVal.GetString());

		m_iImageHeight[i] = m_iniAlgCfg.GetIntEx(strSegment, _T("Height"), 954, _T("图像高度"));

		//*[6/30/2017 ConanSteve]*:  获取当前相机大图包含的小图张数
		m_arr_iCamSmallImgNum[i] = GetPrivateProfileInt(strSegment, _T("PictureNumberPerCamera"), 1, strInitFile);

		strAlg.Format(_T("Byte"));
		m_iImageByte[i] = m_iniAlgCfg.GetIntEx(strSegment, strAlg, 1, _T("每像素字节数"));

		m_iImageSize[i] = m_iImageWidth[i] * m_iImageHeight[i] * m_iImageByte[i];

		// 相机的组号
		strAlg.Format(_T("CheckGroup"));
		GetPrivateProfileString(strSegment, strAlg, _T("1"), sVal.GetBuffer(80), 80, strInitFile);
		sVal.ReleaseBuffer();
		m_iCamGroup[i] = (_ttoi(sVal.GetString()) - 1);

		// 相机属于的剔废队列编号
		strAlg.Format(_T("KickGroup"));
		GetPrivateProfileString(strSegment, strAlg, _T("1"), sVal.GetBuffer(80), 80, strInitFile);
		sVal.ReleaseBuffer();
		m_iCamKick[i] = (_ttoi(sVal.GetString()) - 1);

		// 本相机在剔废队列中的偏移量
		strAlg.Format(_T("KickOffset"));
		GetPrivateProfileString(strSegment, strAlg, _T("10"), sVal.GetBuffer(80), 80, strInitFile);
		sVal.ReleaseBuffer();
		m_iCamKickOffsetY[i] = _ttoi(sVal.GetString());


		// 本相机的算法结果个数
		strAlg.Format(_T("CheckOutNum"));
		GetPrivateProfileString(strSegment, strAlg, _T("1"), sVal.GetBuffer(80), 80, strInitFile);
		sVal.ReleaseBuffer();
		m_iCheckOutNum[i] = _ttoi(sVal.GetString());

		// 每个相机检测输出结构体void* pParam的字节大小
		strAlg.Format(_T("CheckOutSize"));
		GetPrivateProfileString(strSegment, strAlg, _T("300"), sVal.GetBuffer(80), 80, strInitFile);
		sVal.ReleaseBuffer();
		iTemp = _ttoi(sVal.GetString());
		m_iCheckOutSize[i] = m_iCheckOutNum[i] * iTemp;


		// 每个检测输入结构体void* pParam大小
		strAlg.Format(_T("CheckInSize"));
		GetPrivateProfileString(strSegment, strAlg, _T("400"), sVal.GetBuffer(80), 80, strInitFile);
		sVal.ReleaseBuffer();
		m_iCheckInSize[i] = _ttoi(sVal.GetString());

		// 每个状态结构体void* pParam大小
		strAlg.Format(_T("CheckStatusSize"));
		m_iCheckStatusSize[i] = m_iniAlgCfg.GetIntEx(strSegment, strAlg, 200, _T("算法检测状态结构体中的void* pParam大小（字节为单位）"));

		// 统计每个组中相机个数
		m_iGroupElementNum[m_iCamGroup[i]]++;

		// 统计每个剔废队列相机个数
		m_iKickElementNum[m_iCamKick[i]]++;

		// 统计每个检测组的图像数据大小
		m_iGroupImageSize[m_iCamGroup[i]] += m_iImageSize[i];

		//*[6/30/2017 ConanSteve]*:  单个产品所有图像的大小之和
		m_iSizeOfOneArrImgs += m_iImageSize[i];

		//*[7/14/2017 ConanSteve]*:  计算每个相机单张图的大小
		m_iSmallImgSize[i] = m_iImageSize[i] / m_arr_iCamSmallImgNum[i];

		// 统计每个检测组中的算法检测输入参数 void* pParam大小
		m_iGroupCheckInSize[m_iCamGroup[i]] += m_iCheckInSize[i];

		// 统计每个检测组中的算法检测输出参数 void* pParam大小
		m_iGroupCheckOutSize[m_iCamGroup[i]] += m_iCheckOutSize[i];

		// 统计每个检测组中的算法检测返回状态参数 void* pParam大小
		m_iGroupCheckStatusSize[m_iCamGroup[i]] += m_iCheckStatusSize[i];

		// 统计每个检测组中的算法检测输出结果的个数
		m_iGroupCheckOutNum[m_iCamGroup[i]] += m_iCheckOutNum[i];

		// 统计每个剔废队列中每个元素的字节大小
		m_iKickQueueSize[m_iCamKick[i]] += m_iCheckOutSize[i];

		// 统计每个剔废队列结果个数
		m_iKickResultNum[m_iCamKick[i]] += m_iCheckOutNum[i];

		// 获取当前组的第一个相机号
		if (1 == m_iGroupElementNum[m_iCamGroup[i]])
		{
			m_iGroupFirstCam[m_iCamGroup[i]] = i;
		}

		// 获取当前剔废队列中第一个相机编号
		if (1 == m_iKickElementNum[m_iCamKick[i]])
		{
			m_iKickFirstCam[m_iCamKick[i]] = i;
		}

		// 每个相机在对应组中的序号
		if (m_iCamGroup[i] != iLastGroup)
		{
			iGroupIndex = 0;
			m_iIndexInGroup[i] = iGroupIndex;
			iLastGroup = m_iCamGroup[i];
		}
		else
		{
			iGroupIndex++;
			m_iIndexInGroup[i] = iGroupIndex;
		}


		// 每个相机第一个结果在对应的剔废队列中的序号
		if (m_iCamKick[i] != iLastKick)
		{
			iKickIndex = 0;
			m_iIndexInKick[i] = iKickIndex;

			iLastKickSize = 0;
			m_iCamKickOffsetX[i] = iLastKickSize;
			iLastKickSize = m_iCheckOutSize[i];

			iLastKickResult = 0;
			m_iCamKickOffsetX_Written[i] = iLastKickResult;
			iLastKickResult = m_iCheckOutNum[i];

			iLastKick = m_iCamKick[i];
		}
		else
		{
			iKickIndex++;
			m_iIndexInKick[i] = iKickIndex;

			m_iCamKickOffsetX[i] = iLastKickSize;
			iLastKickSize += m_iCheckOutSize[i];

			m_iCamKickOffsetX_Written[i] = iLastKickResult;
			iLastKickResult += m_iCheckOutNum[i];
		}
	}

	// 求出每个剔废队列中剔废偏移量最小的相机号
	//**[7/31/2017 ConanSteve]**:  和剔废偏移最大的相机号
	array<int, MAX_CAMERA_COUNT> iMinKickSN{ 0 };
	for (i = 0; i < MAX_CAMERA_COUNT; i++)
	{
		iMinKickSN[i] = MAX_IMAGE_COUNT;
		m_iMaxOffsetOfKick[i] = 0;
	}

	for (i = 0; i < m_iCameraNum; i++)
	{
		for (j = 0; j < m_iKickQueueNum; j++)
		{
			if (m_iCamKick[i] == j)
			{
				if (m_iCamKickOffsetY[i] < iMinKickSN[j])
				{
					iMinKickSN[j] = m_iCamKickOffsetY[i];
					m_iTimeOutIndex[j] = i;
				}
				if (m_iCamKickOffsetY[i] > m_iMaxOffsetOfKick[j])
				{
					m_iMaxOffsetOfKick[j] = m_iCamKickOffsetY[i];
				}
			}
		}
	}
	for (int i = 0; i < m_iKickQueueNum; ++i)
	{
		m_iKickRemediation[i] = (-1)*m_iMaxOffsetOfKick[i];
	}
	// 验证每个组中输出结构体中void* pParam大小 并分配每个结果大小
	int iResult = 0;
	for (i = 0; i < m_iCheckGroupNum; i++)
	{
		if (m_iGroupCheckOutNum[i] > 0)
		{
			iResult = m_iGroupCheckOutSize[i] % m_iGroupCheckOutNum[i];

			if (0 != iResult)
			{
				// 每个结果大小不为0 说明设置有问题
				AfxMessageBox(_T("Error Code: 2!"));
				return FALSE;
			}
			else
			{
				m_iGroupCheckOutPerResultSize[i] = m_iGroupCheckOutSize[i] / m_iGroupCheckOutNum[i];
			}
		}
		else
		{
			m_iGroupCheckOutPerResultSize[i] = 0;
		}
	}

	// 读取检测队列参数
	for (i = 0; i < m_iCheckGroupNum; i++)
	{
		strSegment.Format(_T("CheckGroup%d"), i + 1);
		m_iGroupCheckQueueNum[i] = m_iniAlgCfg.GetIntEx(strSegment, _T("ElementNum"), 2, _T("每个检测组缓冲队列的长度"));
	}

	// 读取剔废队列参数
	for (i = 0; i < m_iKickQueueNum; i++)
	{
		strSegment.Format(_T("KickGroup%d"), i + 1);
		strAlg.Format(_T("TimeType"));
		m_iTimeType[i] = m_iniAlgCfg.GetIntEx(strSegment, strAlg, 0, _T("剔废队列1定时基准 0为时间，单位为毫秒；1为帧/行，计时单位为帧个数/编码器行频"));

		strAlg.Format(_T("TimeOut"));
		CString sAnnotation = 0 == m_iTimeType[i] ? _T("毫秒") : _T("帧/行");
		m_iTimeOut[i] = m_iniAlgCfg.GetIntEx(strSegment, strAlg, 120);
	}


	// 相同相机之间的最小间隔
	strSegment.Format(_T("Other Parameters"));
	strAlg.Format(_T("MinIntervalOfSameCamera"));
	m_iCallInterval = m_iniAlgCfg.GetIntEx(strSegment, strAlg, 20, _T("同一相机同一图像号最小触发间隔"));

	strSegment.Format(_T("Other Parameters"));
	strAlg.Format(_T("MinIntervalOfDiffPeriod"));
	m_iPeriodInterval = m_iniAlgCfg.GetIntEx(strSegment, strAlg, 30);

	strSegment.Format(_T("Other Parameters"));
	strAlg.Format(_T("bWriteLog"));
	m_bWriteLog = m_iniAlgCfg.GetBoolEx(strSegment, strAlg, false);

	strSegment.Format(_T("Other Parameters"));
	strAlg.Format(_T("MaxLogNum"));
	m_iMaxLogLineNum = m_iniAlgCfg.GetIntEx(strSegment, strAlg, 10000);

	strSegment.Format(_T("DEMO Controls"));
	strAlg.Format(_T("VirtualDisk"));
	GetPrivateProfileString(strSegment, strAlg, _T("M"), m_strVitrualDisk.GetBuffer(5), 5, m_strAlgconfigDir);
	m_strVitrualDisk.ReleaseBuffer();
	WritePrivateProfileString(strSegment, strAlg, m_strVitrualDisk, m_strAlgconfigDir);


	// 创建日志文件

	if (e_CallType_Demo != m_iCallType && TRUE == m_bWriteLog)
	{
		if (!CreateLogFile())
		{
			return FALSE;
		}
	}
	//**[7/26/2017 ConanSteve]**:  读取剔废表的默认值
	m_iDefaultCheckRslt = m_iniAlgCfg.GetIntEx(_T("SystemConfiguration"), _T("DefaultCheckResult"), -3, _T("剔废表默认检测结果"));
	m_iLostImageRslt = m_iniAlgCfg.GetIntEx(_T("SystemConfiguration"), _T("DefaultLostImageResult"), -4, _T("剔废表在多对象主动丢图时写的检测结果"));
	//**[8/1/2017 ConanSteve]**:  读取算法异常的检测结果
	m_iAlgExceptionVal = GetPrivateProfileInt(_T("SystemConfiguration"), _T("AlgorithmExceptionValue"), 0, m_strAlgconfigDir);

	return TRUE;
}

UINT CDataClass::ThreadFunc_ChangeStatus(LPVOID lParam)
{
	int nWS = ((int*)lParam)[0];
	int nObj = ((int*)lParam)[1];
	int nFunc = ((int*)lParam)[2];

	if (nFunc >= 0 && nFunc <= 2)
		m_pObjStatusDlg->SendMessage(1700 + nFunc, nWS * 1000 + nObj);

	return 1;
}

void CDataClass::GetLogDirPath(CString& strDir)
{
	SYSTEMTIME systime;
	GetLocalTime(&systime);

	strDir = m_strAppDir + _T("\\LogFiles");
	if (TRUE != PathFileExists(strDir))
		CreateDirectory(strDir.GetBuffer(0), 0);
	strDir += _T("\\InspectObjsLog");
	if (TRUE != PathFileExists(strDir))
		CreateDirectory(strDir.GetBuffer(0), 0);
	CString strTimeDir;
	strTimeDir.Format(_T("\\%04d-%02d-%02d"), systime.wYear, systime.wMonth, systime.wDay);
	strDir += strTimeDir;
	if (TRUE != PathFileExists(strDir))
		CreateDirectory(strDir.GetBuffer(0), 0);
	strTimeDir.Format(_T("\\%02d-%02d-%02d"), systime.wHour, systime.wMinute, systime.wSecond);
	strDir += strTimeDir;
	if (TRUE != PathFileExists(strDir))
		CreateDirectory(strDir.GetBuffer(0), 0);
}

s_Status CDataClass::Detect(s_CheckInParam sCheckInParam)
{

	s_Status sError;
	if (m_arr_bIsLoadModel[sCheckInParam.iCheckSN])//**[1/7/2018 ConanSteve]**:	如果正在重新加载模板，直接返回 
		return sError;

	if (!m_bEnableInitObjectAndCheck)
		return sError;
	if (-1 != m_iSingleTest)
	{
		if (sCheckInParam.iCheckSN != m_iSingleTest)
			return sError;
	}
	/************************************************************************/
	/*                       [6/29/2017 ConanSteve]                         */
	m_iSysDetectMode = sCheckInParam.iReserve1;
	//**[9/15/2017 ConanSteve]**:  判断是否使用了扩展信息结构体，如果没用，将iReserve4置为0，防止未初始化的变量
	if (!m_bEnableExtenInfo)
		sCheckInParam.iReserve4 = 0;
	if (e_SysDetectMode_Single == m_iSysDetectMode)
	{
		return DetectEx(sCheckInParam);
	}
	/************************************************************************/
	char chInfo[MAX_CHAR_LENGTH]{ 0 };
	array<char, MAX_CHAR_LENGTH> chLogInfo{ 0 };

	int i = 0, j = 0;
	int iComplete = 0;
	int iCurrIndex = 0, iMinIndex = 0;
	int iMinCircleSN = MAX_IMAGE_COUNT;

	bool bBusy = true;

	float fDiff = 0;
	LONGLONG lInfo = 0;
	LARGE_INTEGER liValue = { 0 };

	sError.iStatusType = STATUS_NOR;
	sError.pThis = m_pCallThis;
	sError.pErrorInfo = NULL;

	// 若停止检测则不再调用
	if (m_bKillThread)
	{
		sError.iStatusType = STATUS_ILG;
		sError.pErrorInfo = GetErrorInfo("Kill Thread!");
		return sError;
	}


	// 	// 根据客户设置决定是否显示对象使用情况
	// 	ShowCurrStatus();

	if (e_CallType_Demo != m_iCallType)
	{
		// 相机号
		int iCheckSN = sCheckInParam.iCheckSN;

		// 图像号
		int iImageSN = sCheckInParam.iImageSN;

		// 组号
		int iGroupSN = m_iCamGroup[iCheckSN];

		// 该相机在组的顺序号
		int iOrderSN = m_iIndexInGroup[iCheckSN];

		// 队列号
		int iKickSN = m_iCamKick[iCheckSN];

		ASSERT(iCheckSN >= 0 && iCheckSN < m_iCameraNum);



		// 每一路相机进入检测的个数
		m_iCamCircle[iCheckSN]++;
		int iCircleSN = m_iCamCircle[iCheckSN];

		QueryPerformanceCounter(&liValue);
		lInfo = liValue.QuadPart;

		// 当前相机本张图像与上一张图像的时间差
		m_csCameraDiff[iCheckSN].Lock();
		fDiff = 1000 * (lInfo - m_lCamTime[iCheckSN][(iCircleSN - 1) % 256]) / m_dFreq;
		m_lCamTime[iCheckSN][iCircleSN % 256] = lInfo;
		m_csCameraDiff[iCheckSN].Unlock();

		CString sInfo{ _T("") };
		sInfo.Format(_T("[相机号:%2d] [图像号:%8d] [检测组号:%2d] [剔废号:%2d] [组中相机索引:%2d] [剔废索引:%2d] [入检个数:%8d]"), iCheckSN,
			iImageSN, iGroupSN, iKickSN + 1, iOrderSN, m_iIndexInKick[iCheckSN], iCircleSN);
		m_fileCam[iCheckSN][0].Write(sInfo);


		/** 如果本相机序号是该剔废队列的离踢废口最近相机的序号，则开始对剔废超时计时              [5/3/2017 ConanSteve]**/
		if (m_iTimeOutIndex[iKickSN] == iCheckSN)
		{
			m_iKickImageSN[iKickSN][iCircleSN % 256] = iImageSN;

			// 定时基准是时间
			if (m_bEnableOvertimeCallback)
			{
				if (e_TimeType_MilliSecond == m_iTimeType[iKickSN])
				{
					m_spTime[iKickSN][iCircleSN % 256].Start();
				}
			}
			m_iKickCircleSN[iKickSN] = iCircleSN;
		}

		//**[7/19/2017 ConanSteve]**:   判断是否是一个新周期
		if (TRUE == JudgeNewCircle(iGroupSN, iCheckSN, iImageSN, iCircleSN))
		{
			//**[7/28/2017 ConanSteve]**:  新图写剔废表默认结果
			ThreadWriteResult(iImageSN, iGroupSN, iKickSN, false);
			//**[7/19/2017 ConanSteve]**:   判断上一个周期是否完成,若未完成，则补图检测
			int iLastImageSN = m_iPeriodImageSN[iGroupSN][m_iGroupCircle[iGroupSN] % 256];
			int iLastIndex = m_iPeriodIndex[iGroupSN][m_iGroupCircle[iGroupSN] % 256];

			iComplete = 0;
			/** 判断指定检测组图像是否到齐              [5/5/2017 ConanSteve]**/
			for (i = 0; i < m_iGroupElementNum[iGroupSN]; i++)
			{
				if (m_bCamComplete[m_iGroupFirstCam[iGroupSN] + i][iLastImageSN % 256])
				{
					iComplete++;
				}
			}

			//*[7/5/2017 ConanSteve]*:  写日志
			CString sInfo{ _T("") };
			sInfo.Format(_T("[相机号:%2d] [图像号:%8d] [检测组号:%2d] [剔废队列号:%2d] [上一图像号:%8d] [上一索引:%2d] [相机处理图像个数:%8d] [组相机收图计数:%2d] [组相机个数:%d]"),
				iCheckSN, iImageSN, iGroupSN, iKickSN, iLastImageSN, iLastIndex, m_iCamCircle[iCheckSN], iComplete,
				m_iGroupElementNum[iGroupSN]);
			m_fileDetect[iGroupSN][1].Write(sInfo);


			//**[7/19/2017 ConanSteve]**:   对于实际采集，未收齐图像仍然送去检测
			if (m_iCallType == e_CallType_RealTime)
			{
				if (iComplete != m_iGroupElementNum[iGroupSN] && m_iCamCircle[iCheckSN] != 1)
				{
					// 触发检测
					SetEvent(m_hImageInspectStart[iGroupSN]);

					m_csWriteDataStatus[iGroupSN].Lock();
					m_pWriteDataStatus[iGroupSN][iLastIndex] = e_WriteDataStatus_Complete;
					m_csWriteDataStatus[iGroupSN].Unlock();


					sInfo.Format(_T("[检测组号:%2d] [相机号:%2d] [当前图像号:%08d] [上一工位图像号:%8d] [上一工位收齐数:%2d]"),
						iGroupSN, iCheckSN, iImageSN, iLastImageSN, iComplete);
					m_fileDetect[iGroupSN][6].Write(sInfo);
				}
			}

			/*************************************<查询队列中空闲对象>************************************[10/9/2017 ConanSteve]*/
			m_csWriteDataStatus[iGroupSN].Lock();
			int iStatusBefore = 0;
			for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; i++)
			{
				iStatusBefore = m_pWriteDataStatus[iGroupSN][i];
				if (m_pWriteDataStatus[iGroupSN][i] != e_WriteDataStatus_Leisure)
				{
					continue;
				}
				else
				{
					bBusy = false;
					iCurrIndex = i;
					m_pWriteDataStatus[iGroupSN][i] = e_WriteDataStatus_Busy;
					++m_ppCacheUseCnt[iGroupSN][i];
					break;
				}
			}
			m_csWriteDataStatus[iGroupSN].Unlock();


			/** 写日志              [5/5/2017 ConanSteve]**/
			sInfo.Format(_T("[相机号:%2d] [检测组号:%2d] [图像号:%8d] [队列是否忙:%s] [缓存队列Before索引:%d] [缓存写状态:%3s]"),
				iCheckSN, iGroupSN, iImageSN, bBusy ? _T("是") : _T("否"), iCurrIndex,
				e_WriteDataStatus_Leisure == iStatusBefore ? _T("空闲") : (e_WriteDataStatus_Busy == iStatusBefore) ? _T("正在写") : _T("已写完"));
			m_fileDetect[iGroupSN][2].Write(sInfo);

			//处理方法2 将正等待检测的最小编号元素数据剔除出队列，放入最新数据	
			// 替换最小图像号
			if (true == bBusy)
			{
				m_iGroupAbnormalNum[iCheckSN]++;

				m_csWriteDataStatus[iGroupSN].Lock();
				for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; ++i)
				{
					if (iMinCircleSN > m_pCircleSN[iGroupSN][i] && m_pWriteDataStatus[iGroupSN][i] == e_WriteDataStatus_Complete && m_pDetectStatus[iGroupSN][i] == e_DetectStatus_NoChecking)
					{
						iMinCircleSN = m_pCircleSN[iGroupSN][i];
						iMinIndex = i;
					}
				}
				m_csWriteDataStatus[iGroupSN].Unlock();

				// 有未在检测的元素
				if (MAX_IMAGE_COUNT != iMinCircleSN)
				{
					//旧的图像号进入剔废队列
					iCurrIndex = iMinIndex;
					int iOldImageSN = m_spCheckInParam[iGroupSN][iMinIndex].iImageSN;

					m_iGroupCircle[iGroupSN]++;

					// 旧图像号写坏结果
					ThreadWriteResult(iOldImageSN, iGroupSN, iKickSN, true);

					// 调用状态回调 通知系统
					TCHAR chStatusInfo[MAX_CHAR_LENGTH]{ 0 };
					s_Status sStatus;
					sStatus.iCheckSN = iCheckSN;
					sStatus.iStatusType = STATUS_THROW_IMAGE;
					sStatus.pThis = m_pCallThis;
					sStatus.pParam = NULL;
					_tprintf_s(chStatusInfo, _T("Checking Image No.%d, Throwing Image No.%d!"), iImageSN, iOldImageSN);
					sStatus.pErrorInfo = GetErrorInfo(chStatusInfo);
					m_StatusCallback(&sStatus);

					m_csWriteDataStatus[iGroupSN].Lock();
					m_pWriteDataStatus[iGroupSN][iCurrIndex] = e_WriteDataStatus_Busy;
					m_csWriteDataStatus[iGroupSN].Unlock();


					//新来图像代替旧图像,则处于写的状态
					m_spCheckInParam[iGroupSN][iCurrIndex].iCheckSN = iGroupSN;
					m_spCheckInParam[iGroupSN][iCurrIndex].iImageSN = iImageSN;
					m_spCheckInParam[iGroupSN][iCurrIndex].iReserve1 = sCheckInParam.iReserve1;
					m_spCheckInParam[iGroupSN][iCurrIndex].iReserve2 = sCheckInParam.iReserve2;
					m_spCheckInParam[iGroupSN][iCurrIndex].iReserve3 = sCheckInParam.iReserve3;
					m_spCheckInParam[iGroupSN][iCurrIndex].iReserve4 = sCheckInParam.iReserve4;
					LARGE_INTEGER liTime;
					QueryPerformanceCounter(&liTime);
					m_arr_ldTickOfPushInCacheQueue[iGroupSN][iCurrIndex]= liTime.QuadPart;

					m_pCircleSN[iGroupSN][iCurrIndex] = m_iGroupCircle[iGroupSN];

					m_iPeriodImageSN[iGroupSN][m_iGroupCircle[iGroupSN] % 256] = sCheckInParam.iImageSN;
					m_iPeriodIndex[iGroupSN][m_iGroupCircle[iGroupSN] % 256] = iCurrIndex;

					m_srwlImgQue[iGroupSN].Lock();
					BYTE* pAddr = (BYTE*)(m_spCheckInParam[iGroupSN][iCurrIndex].pImageData + iOrderSN * m_iImageSize[iCheckSN]);
					memcpy_s(pAddr, m_iImageSize[iCheckSN], sCheckInParam.pImageData, m_iImageSize[iCheckSN]);

					BYTE* pParam = (BYTE*)(m_spCheckInParam[iGroupSN][iCurrIndex].pParam);
					memcpy_s(pParam, m_iGroupCheckInSize[iGroupSN], sCheckInParam.pParam, m_iGroupCheckInSize[iGroupSN]);
					m_srwlImgQue[iGroupSN].Unlock();

					for (i = 0; i < m_iGroupElementNum[iGroupSN]; i++)
					{
						m_csCamComplete[m_iGroupFirstCam[iGroupSN] + i].Lock();
						m_bCamComplete[m_iGroupFirstCam[iGroupSN] + i][iImageSN % 256] = FALSE;
						m_csCamComplete[m_iGroupFirstCam[iGroupSN] + i].Unlock();
					}
					//**[7/18/2017 ConanSteve]**:  write logs
					sInfo.Format(_T("相机号:%2d] [检测组号:%2d] [缓存索引:%2d] [组处理图像个数:%d] [踢图索引:%d] [周期图像号:%d] [周期索引:%d]"),
						iCheckSN, iGroupSN, iCurrIndex, m_iGroupCircle[iGroupSN], iMinCircleSN,
						m_iPeriodImageSN[iGroupSN][m_iGroupCircle[iGroupSN] % 256],
						m_iPeriodIndex[iGroupSN][m_iGroupCircle[iGroupSN] % 256]);
					m_fileDetect[iGroupSN][7].Write(sInfo);
				}
				else		//无可替换元素
				{
					//**[9/22/2017 ConanSteve]**:  当前图像写坏结果
					ThreadWriteResult(iImageSN, iGroupSN, iKickSN, true);
					sError.iStatusType = STATUS_ILG;
					ZeroMemory(&chInfo[0], MAX_CHAR_LENGTH);

					CString str1{ _T("") }, str2{ _T("") };

					// 组编号
					str1.Format(_T("Cam:%d"), iGroupSN);

					// 图像号
					str2.Format(_T("C:%d "), iImageSN);
					str1 += str2;

					// 检测队列中的序号
					for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; i++)
					{
						str2.Format(_T("%d "), m_pCircleSN[iGroupSN][i]);
						str1 += str2;
					}

					// 检测队列中元素图像数据写入状态
					str2.Format(_T("W"));
					for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; i++)
					{
						str2.Format(_T("%d "), m_pWriteDataStatus[iGroupSN][i]);
						str1 += str2;
					}

					// 检测队列中元素检测状态
					str2.Format(_T("D"));
					for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; i++)
					{
						str2.Format(_T("%d "), m_pDetectStatus[iGroupSN][i]);
						str1 += str2;
					}

					//memcpy(&chInfo[0], str1.GetBuffer(0), str1.GetLength());


					sInfo.Format(_T("[相机号:%2d] [检测组号:%2d] [索引号:%2d] [组处理图像计数:%8d] [踢图索引:%2d] [周期图像号:%8d] [周期元素索引:%d]"),
						iCheckSN, iGroupSN, iCurrIndex, m_iGroupCircle[iGroupSN], iMinCircleSN,
						m_iPeriodImageSN[iGroupSN][m_iGroupCircle[iGroupSN] % 256],
						m_iPeriodIndex[iGroupSN][m_iGroupCircle[iGroupSN] % 256]);
					m_fileDetect[iGroupSN][7].Write(sInfo);

					// 调用状态回调 通知系统所有队列元素已经用完，且全部参加检测
					char chStatusInfo[MAX_CHAR_LENGTH];
					ZeroMemory(chStatusInfo, MAX_CHAR_LENGTH);
					s_Status sStatus;
					ZeroMemory(&sStatus, sizeof(sStatus));
					sStatus.iCheckSN = iCheckSN;
					sStatus.iStatusType = STATUS_THROW_IMAGE;
					sStatus.pThis = m_pCallThis;
					sStatus.pParam = NULL;
					sprintf(chStatusInfo, "Current Image No.%d, All Elements are busy!", iImageSN);
					sStatus.pErrorInfo = GetErrorInfo(chStatusInfo);
					m_StatusCallback(&sStatus);

					return sStatus;
				}
			}
			/** 将当前图像号添加进检测队列              [5/5/2017 ConanSteve]**/
			else
			{
				m_iGroupCircle[iGroupSN]++;

				m_spCheckInParam[iGroupSN][iCurrIndex].iCheckSN = iGroupSN;
				m_spCheckInParam[iGroupSN][iCurrIndex].iImageSN = iImageSN;
				m_spCheckInParam[iGroupSN][iCurrIndex].iReserve1 = sCheckInParam.iReserve1;
				m_spCheckInParam[iGroupSN][iCurrIndex].iReserve2 = sCheckInParam.iReserve2;
				m_spCheckInParam[iGroupSN][iCurrIndex].iReserve3 = sCheckInParam.iReserve3;
				m_spCheckInParam[iGroupSN][iCurrIndex].iReserve4 = sCheckInParam.iReserve4;

				m_pCircleSN[iGroupSN][iCurrIndex] = m_iGroupCircle[iGroupSN];

				m_iPeriodImageSN[iGroupSN][m_iGroupCircle[iGroupSN] % 256] = iImageSN;
				m_iPeriodIndex[iGroupSN][m_iGroupCircle[iGroupSN] % 256] = iCurrIndex;


				m_srwlImgQue[iGroupSN].Lock();
				BYTE* pAddr = (BYTE*)m_spCheckInParam[iGroupSN][iCurrIndex].pImageData + iOrderSN * m_iImageSize[iCheckSN];
				memcpy_s(pAddr, m_iImageSize[iCheckSN], sCheckInParam.pImageData, m_iImageSize[iCheckSN]);

				BYTE* pParam = (BYTE*)(m_spCheckInParam[iGroupSN][iCurrIndex].pParam);
				memcpy_s(pParam, m_iGroupCheckInSize[iGroupSN], sCheckInParam.pParam, m_iGroupCheckInSize[iGroupSN]);
				m_srwlImgQue[iGroupSN].Unlock();
				int iSize = 0;
				int iWidth = 0;
				int iHeight = 0;
				int iBit = 0;
				int iType = 0;
				int iCount = 0;

				/** 写日志              [5/5/2017 ConanSteve]**/
				sInfo.Format("[相机号:%2d] [检测组号:%2d] [索引号:%2d] [组处理图像计数:%8d] [周期图像号:%8d] [周期元素索引:%d]",
					iCheckSN, iGroupSN, iCurrIndex, m_iGroupCircle[iGroupSN],
					m_iPeriodImageSN[iGroupSN][m_iGroupCircle[iGroupSN] % 256],
					m_iPeriodIndex[iGroupSN][m_iGroupCircle[iGroupSN] % 256]);
				m_fileDetect[iGroupSN][3].Write(sInfo);

				LARGE_INTEGER liTime;
				QueryPerformanceCounter(&liTime);
				m_arr_ldTickOfPushInCacheQueue[iGroupSN][iCurrIndex]=liTime.QuadPart;

				for (i = 0; i < m_iGroupElementNum[iGroupSN]; i++)
				{
					m_csCamComplete[m_iGroupFirstCam[iGroupSN] + i].Lock();
					m_bCamComplete[m_iGroupFirstCam[iGroupSN] + i][iImageSN % 256] = FALSE;
					m_csCamComplete[m_iGroupFirstCam[iGroupSN] + i].Unlock();
				}
			}
		}
		/** 不是新周期              [5/5/2017 ConanSteve]**/
		// 不是新周期，判断是否已经写入；若没写入则向内存写入数据；
		// 若写入了，则查询上一张图像号是否已经被写入，若未写入且时间间隔太小，则判断为连盖导致图像号相同
		else
		{

			sInfo.Format(_T("[相机号:%2d] [检测组号:%2d] [图像号:%8d] [时间差:%4.0f]"),
				iCheckSN, iGroupSN, iImageSN, fDiff);
			m_fileDetect[iGroupSN][1].Write(sInfo);

			sError.iStatusType = STATUS_ILG;
			sError.pThis = m_pCallThis;

			memset(&chInfo[0], 0, MAX_CHAR_LENGTH);
			sprintf_s(chInfo, "=Cam:%d=Image:%d=Same Image\r\n", iCheckSN, iImageSN);
			sError.pErrorInfo = GetErrorInfo(chInfo);

			return sError;
		}

		m_csCamComplete[iCheckSN].Lock();
		m_bCamComplete[iCheckSN][iImageSN % 256] = TRUE;
		m_csCamComplete[iCheckSN].Unlock();


		iComplete = 0;
		//**[7/19/2017 ConanSteve]**:   查看本工位一个周期图像是否收齐
		for (i = 0; i < m_iGroupElementNum[iGroupSN]; i++)
		{
			if (m_bCamComplete[m_iGroupFirstCam[iGroupSN] + i][iImageSN % 256])
			{
				iComplete++;
			}
		}
		/** 如果收齐图像，触发检测              [5/5/2017 ConanSteve]**/
		if (iComplete == m_iGroupElementNum[iGroupSN])
		{
			m_csWriteDataStatus[iGroupSN].Lock();
			m_pWriteDataStatus[iGroupSN][iCurrIndex] = e_WriteDataStatus_Complete;
			m_csWriteDataStatus[iGroupSN].Unlock();

			// 触发检测
			SetEvent(m_hImageInspectStart[iGroupSN]);

			sInfo.Format(_T("[相机号:%2d] [检测组号:%2d] [图像号:%8d] [组入检计数:%8d] [队列索引:%2d] [剔废表是否被写入:%d]"),
				iCheckSN, iGroupSN, iImageSN, iCircleSN, iCurrIndex, *m_pbKickWrite[0][iImageSN % 256]);
			m_fileDetect[iGroupSN][4].Write(sInfo);
		}
	}
	return sError;
}

BOOL CDataClass::JudgeNewCircle(int iGroupSN, int iCameraSN, int iImageSN, int iCircleSN)
{

	BOOL bRet = FALSE;
	LONGLONG lInfo;
	LARGE_INTEGER liValue{ 0 };
	char chInfo[MAX_CHAR_LENGTH]{ 0 };

	m_iGroupCallCount[iGroupSN]++;
	QueryPerformanceCounter(&liValue);
	lInfo = liValue.QuadPart;

	m_csGroupDiff[iGroupSN].Lock();
	float dDiff = (lInfo - m_lPeriodTime[iGroupSN][(m_iGroupCallCount[iGroupSN] - 1) % 256]) * 1000 / m_dFreq;
	m_lPeriodTime[iGroupSN][m_iGroupCallCount[iGroupSN] % 256] = lInfo;
	m_csGroupDiff[iGroupSN].Unlock();

	if (e_CallType_RealTime == m_iCallType)
	{
		if (iImageSN != m_iGroupLastImageSN[iGroupSN])
		{
			bRet = TRUE;
		}
		else
		{
			if (dDiff > m_iPeriodInterval)
			{
				bRet = TRUE;
			}
			else
			{
				bRet = FALSE;
			}
		}
	}
	else if (e_CallType_Simulation == m_iCallType)
	{
		// 模拟采集 
		if (iImageSN != m_iGroupLastImageSN[iGroupSN] && iCircleSN > m_iGroupCircle[iGroupSN])
		{
			bRet = TRUE;
		}
		else
		{
			bRet = FALSE;
		}
	}


	CString sInfo{ _T("") };
	sInfo.Format(_T("[相机号:%2d] [检测组号:%2d] [图像号:%8d] [上一图像号:%8d] [是否新周期:%2s] [TimeDiff:%4.0f]"),
		iCameraSN, iGroupSN, iImageSN, m_iGroupLastImageSN[iGroupSN], bRet ? _T("是") : _T("否"), dDiff);
	m_fileDetect[iGroupSN][0].Write(sInfo);
	// 	if (m_iGroupLastImageSN[iGroupSN] == iImageSN && bRet == TRUE)
	// 	{
	// 		int kk = 0;
	// 	}
	m_iGroupLastImageSN[iGroupSN] = iImageSN;

	return bRet;
}

DWORD CDataClass::ImageInspectionThread(LPVOID lpParam)
{
	CDataClass* pThis = (CDataClass*)(((s_ThreadParam*)lpParam)->pThis);
	int iThreadID = ((s_ThreadParam*)lpParam)->iThreadID;
	DWORD dwRet = pThis->ImageInspectionThreadFunc(iThreadID);
	return dwRet;
}

DWORD CDataClass::ImageInspectionThreadFunc(int iThreadID)
{
	int iGroupSN = 0;

	while (false == m_bKillThread)
	{
		iGroupSN = WaitForMultipleObjects(m_iCheckGroupNum, m_hImageInspectStart, FALSE, 10) - WAIT_OBJECT_0;
		if (iGroupSN >= 0 && iGroupSN < m_iCheckGroupNum && false == m_bKillThread)
		{
			CString sInfo{ _T("") };
			sInfo.Format(_T("[检测组:%2d]  [线程ID:%2d]"), iGroupSN, iThreadID);
			m_fileCheck[iGroupSN][5].Write(sInfo);
			if (m_arr_bIsLoadModel[iGroupSN])
			{
				// 				UpdateRunInfo();
				// 				Sleep(1);
				continue;
			}
			InspectionContextEx2(iGroupSN, iThreadID);

		}
	}

	m_evtKillInspThread[iThreadID].SetEvent();

	TRACE(_T("Kill Insp %d Thread!\r\n"), iThreadID + 1);


	return 0;
}
DWORD CDataClass::InspectionContextEx2(int iGroupSN, int iThreadID)
{
	// 找到编号最小的进行检测
	if (0 == iGroupSN)
		UpdateRunInfo();
	int i{ 0 }, j{ 0 };
	int iMaxNum{ 0 }, iMaxGroup{ 0 }, iTempMaxNum{ 0 };
	int iMinIndex{ 0 };
	int iMinCircleSN{ MAX_IMAGE_COUNT };
	BOOL bRet{ TRUE };

	while (!m_bKillThread)
	{
		iMaxNum = 0;
		iMaxGroup = 0;

		/*************************************<查找所有组中待检数据长度最大的组>*************************************/
		{
			m_csGlobalCheck.Lock();
			for (i = 0; i < m_iCheckGroupNum; i++)
			{
				iTempMaxNum = 0;

				m_csWriteDataStatus[i].Lock();
				for (j = 0; j < m_iGroupCheckQueueNum[i]; j++)
				{
					if (m_pWriteDataStatus[i][j] == e_WriteDataStatus_Complete && m_pDetectStatus[i][j] == e_DetectStatus_NoChecking)
					{
						iTempMaxNum++;
					}
				}
				m_csWriteDataStatus[i].Unlock();

				if (iTempMaxNum > iMaxNum)
				{
					iMaxNum = iTempMaxNum;
					iMaxGroup = i;
				}
			}
			m_csGlobalCheck.Unlock();
		}
		{
			if (iMaxNum == 0)
			{
				break;
			}
			else
			{
				iGroupSN = iMaxGroup;
			}
		}

		/*************************************<查找对应组待检数据索引最小的索引>*************************************/
		{
			iMinCircleSN = MAX_IMAGE_COUNT;

			m_csWriteDataStatus[iGroupSN].Lock();
			for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; i++)
			{
				if (m_pWriteDataStatus[iGroupSN][i] == e_WriteDataStatus_Complete && m_pDetectStatus[iGroupSN][i] == e_DetectStatus_NoChecking)
				{
					if (m_pCircleSN[iGroupSN][i] < iMinCircleSN)
					{
						iMinIndex = i;
						iMinCircleSN = m_pCircleSN[iGroupSN][i];
					}
				}
			}

			if (MAX_IMAGE_COUNT == iMinCircleSN)
			{
				m_csWriteDataStatus[iGroupSN].Unlock();
				break;
			}
			else
			{
				m_pDetectStatus[iGroupSN][iMinIndex] = e_DetectStatus_Checking;
			}

			m_csWriteDataStatus[iGroupSN].Unlock();
		}
		//处理最小图像号
		CheckEx2(iMinIndex, iThreadID, m_spCheckInParam[iGroupSN][iMinIndex]);
	}


	return 0;
}

// BOOL CDataClass::ThreadWriteCheckResult(int iGroupSN, int iIndex)
// {
// 	int i = 0;
// 
// 	// 获取改组第一个相机号
// 	int iFirstCameraSN = m_iGroupFirstCam[iGroupSN];
// 
// 	// 获取所在剔废队列号
// 	int iKickQueueSN = m_iCamKick[iFirstCameraSN];
// 
// 	// 获取周期号
// 	int iImageSN = m_spCheckInParam[iGroupSN][iIndex].iImageSN;
// 
// 	// 获取在剔废队列的偏移量(以图像张数为单位)，确定剔废表的纵向位置
// 	int	iKickY = m_spCheckInParam[iGroupSN][iIndex].iImageSN + m_iCamKickOffsetY[iFirstCameraSN];
// 
// 	// 在剔废表中的横向位置
// 	int iKickX = m_iCamKickOffsetX[iFirstCameraSN];
// 	int iKickX_R = m_iCamKickOffsetX_Written[iFirstCameraSN];
// 
// 	BYTE* pDestAddr = NULL, *pDestAddr2 = NULL;
// 	BYTE* pOriAddr = NULL;
// 
// 	int iOffsetX1 = 0, iOffsetX1_R = 0, iOffsetY1 = 0;
// 	int iOutResult = 0;
// 	iOutResult = m_iCheckOutNum[iFirstCameraSN];
// 
// 	CString str1, str2;
// 
// 	//将检测结果分不同行列写入剔废表中
// 	for (i = 0; i < m_iCheckOutNum[iFirstCameraSN]; i++)
// 	{
// 		iOffsetY1 = (iKickY + iOutResult - i - 1) % 256;
// 		iOffsetX1 = iKickX + (iOutResult - i - 1)*m_iGroupCheckOutPerResultSize[iGroupSN];
// 		iOffsetX1_R = iKickX_R + iOutResult - i - 1;
// 
// 		m_csKickResult[iKickQueueSN][iOffsetY1].Lock();
// 		// 		pDestAddr = m_pKickResult[iKickQueueSN]+iOffsetY1*m_iKickQueueSize[iKickQueueSN]+iOffsetX1;
// 		// 		pDestAddr2 = m_pbKickWrite[iKickQueueSN]+iOffsetY1*m_iKickResultNum[iKickQueueSN]+iOffsetX1_R;
// 		pDestAddr = m_pKickResult[iKickQueueSN][iOffsetY1] + iOffsetX1;
// 		pDestAddr2 = m_pbKickWrite[iKickQueueSN][iOffsetY1] + iOffsetX1_R;
// 
// 		//**[7/28/2017 ConanSteve]**:  检测结果的结构体地址
// 		pOriAddr = (BYTE*)m_spCheckOutParam[iGroupSN][iIndex].pParam + i*m_iGroupCheckOutPerResultSize[iGroupSN];
// 
// 		memset(pDestAddr, 0, m_iGroupCheckOutPerResultSize[iGroupSN]);
// 		memcpy(pDestAddr, pOriAddr, m_iGroupCheckOutPerResultSize[iGroupSN]);
// 
// 		*pDestAddr2 = 1;
// 
// 		str1.Format("%d", *((int*)pOriAddr));
// 		str2 += str1;
// 
// 		m_csKickResult[iKickQueueSN][iOffsetY1].Unlock();
// 
// 	}
// 
// 
// 	if (TRUE == m_bWriteLog)
// 	{
// 		m_iFileCheck[iGroupSN][3]++;
// 		if (m_iFileCheck[iGroupSN][3] > m_iLogNum)
// 		{
// 			m_iFileCheck[iGroupSN][3] = 0;
// 			m_fileCheck[iGroupSN][3].SeekToBegin();
// 		}
// 
// 		CString strInfo;
// 		strInfo.Format(_T("[检测组号:%2d] [缓存队列索引:%2d] [图像号:%8d] [缓存写状态:%s] [缓存检测状态:%s] [剔废队列号:%4d] [剔废偏移量Y:%3d] [剔废偏移X:%3d] [目的地址:0x%08X] [Info:%10s] [CurrentTime:%s]\n"),
// 			iGroupSN, iIndex, iImageSN,
// 			e_WriteDataStatus_Leisure == m_pWriteDataStatus[iGroupSN][iIndex] ? _T("空闲") : (e_WriteDataStatus_Busy == m_pWriteDataStatus[iGroupSN][iIndex] ? _T("写忙") : _T("写完")),
// 			e_DetectStatus_NoChecking ==m_pDetectStatus[iGroupSN][iIndex]?_T("未检测"):_T("检测中"),
// 			iKickQueueSN, iKickY, iKickX,
// 			pDestAddr, str2, GetCurrTime());
// 		m_fileCheck[iGroupSN][3].WriteString(strInfo);
// 	}
// 
// 
// 	return TRUE;
// }

BOOL CDataClass::ThreadWriteCheckResultEx2(int iGroupSN, s_CheckInParam* spCheckInParam, s_CheckOutParam* spCheckOutParam)
{
	int i = 0;
	if (NULL == spCheckInParam || NULL == spCheckOutParam)
		return false;

	// 获取改组第一个相机号
	int iFirstCameraSN = m_iGroupFirstCam[iGroupSN];

	// 获取所在剔废队列号
	int iKickQueueSN = m_iCamKick[iFirstCameraSN];

	// 获取周期号
	int iImageSN = spCheckInParam->iImageSN;

	// 获取在剔废队列的偏移量(以图像张数为单位)，确定剔废表的纵向位置
	int	iKickY = spCheckInParam->iImageSN + m_iCamKickOffsetY[iFirstCameraSN];

	// 在剔废表中的横向位置
	int iKickX = m_iCamKickOffsetX[iFirstCameraSN];
	int iKickX_R = m_iCamKickOffsetX_Written[iFirstCameraSN];

	BYTE* pDestAddr = NULL, *pDestAddr2 = NULL;
	BYTE* pOriAddr = NULL;

	int iOffsetX1 = 0, iOffsetX1_R = 0, iOffsetY1 = 0;
	int iOutResult = 0;//**[12/27/2017 ConanSteve]**:  本剔废组第一个相机的检测结果个数
	iOutResult = m_iCheckOutNum[iFirstCameraSN];

	CString sAllAlgStatus{ _T("") }, sSingleAlgStatus{ _T("") };

	//将检测结果分不同行列写入剔废表中
	for (i = 0; i < m_iCheckOutNum[iFirstCameraSN]; i++)
	{
		iOffsetY1 = (iKickY + iOutResult - i - 1) % 256;
		iOffsetX1 = iKickX + (iOutResult - i - 1)*m_iGroupCheckOutPerResultSize[iGroupSN];
		iOffsetX1_R = iKickX_R + iOutResult - i - 1;

		m_csKickResult[iKickQueueSN][iOffsetY1].Lock();
		pDestAddr = m_pKickResult[iKickQueueSN][iOffsetY1] + iOffsetX1;
		pDestAddr2 = m_pbKickWrite[iKickQueueSN][iOffsetY1] + iOffsetX1_R;

		pOriAddr = (BYTE*)spCheckOutParam->pParam + i * m_iGroupCheckOutPerResultSize[iGroupSN];


		memset(pDestAddr, 0, m_iGroupCheckOutPerResultSize[iGroupSN]);
		memcpy(pDestAddr, pOriAddr, m_iGroupCheckOutPerResultSize[iGroupSN]);

		*pDestAddr2 = 1;

		sSingleAlgStatus.Format(_T("%d"), *((int*)pOriAddr));
		sAllAlgStatus += sSingleAlgStatus;

		m_csKickResult[iKickQueueSN][iOffsetY1].Unlock();

	}


	CString strInfo{ _T("") };
	strInfo.Format(_T("[检测组号:%2d] [图像号:%8d] [剔废队列号:%4d] [剔废偏移量Y:%3d] [剔废偏移X:%3d] [目的地址:0x%08X] [检测结果:%10s]"),
		iGroupSN, iImageSN,
		iKickQueueSN, iKickY, iKickX,
		pDestAddr, sAllAlgStatus);
	m_fileCheck[iGroupSN][3].Write(strInfo);

	if (!m_bEnableOvertimeCallback)
	{
		m_csBoolKickWrite[iKickQueueSN][iImageSN % 256].Lock();
		IsCheckCompleted(iGroupSN, iKickQueueSN, iImageSN);
		m_csBoolKickWrite[iKickQueueSN][iImageSN % 256].Unlock();
	}

	return TRUE;
}

DWORD CDataClass::KickThread(LPVOID lpParam)
{
	CDataClass* pThis = (CDataClass*)lpParam;

	pThis->KickThreadFuncEx2();


	return 0;
}


BOOL CDataClass::CreateMultiLevelPath(const CString &strPath)
{
	//创建一个多层目录，如果存在就不创建
	//(strPath  合法: "D:\\abc\\me" 或 "me\\you" 或 ""; 
	//			不合法: "D:\\abc\\me\\" 或 "me\\you\\" 或 "\\")
	CString strTempPath{ _T("") };// = strPath;
	strTempPath.Format(_T("%s"), strPath);
	CList<CString, CString&> FolderStrList;
	int nStrLen = 0;

	for (;;)
	{
		nStrLen = strTempPath.GetLength();
		if (nStrLen <= 0)
		{
			break;//从头建文件夹
		}

		BOOL bPathExist = FALSE;//该路径是否已经存在

		if (strTempPath.Right(1) == _T(":"))
		{
			bPathExist = PathFileExists(strTempPath + _T("\\"));
		}
		else
		{
			bPathExist = PathFileExists(strTempPath);
		}

		if (bPathExist)//如果路径存在
		{
			break;//从已存在的路径下建文件夹
		}

		//如果路径不存在		
		//每次将最后的一个文件夹名入队

		int nPos = 0;
		nPos = strTempPath.ReverseFind(_T('\\'));//从后往前确定文件夹名
		if (nPos == -1)//已到达第一个文件夹名
		{
			nPos = 0;//将第一个文件夹名入队
		}

		//将文件夹名入队
		CString strTemp = strTempPath.Right(nStrLen - nPos);
		FolderStrList.AddHead(strTemp);
		strTempPath.Delete(nPos, nStrLen - nPos);
	}

	//根据文件夹名队列顺序建立
	while (FolderStrList.IsEmpty() == FALSE)
	{
		strTempPath += FolderStrList.RemoveHead();
		BOOL bRet = FALSE;
		bRet = CreateDirectory(strTempPath, NULL);
		if (bRet == FALSE)
		{
			return FALSE;
		}
	}

	return TRUE;
}

// CString CDataClass::GetCurrTime()
// {
// 	LARGE_INTEGER iLarge;
// 	LONGLONG qt;
// 	int nHour = 0, nMinute = 0, nSecond = 0, nMiliSecond = 0;
// 	QueryPerformanceCounter(&iLarge);
// 	qt = iLarge.QuadPart;
// 
// 	nMiliSecond = qt * 1000 / m_dFreq;
// 	nSecond = nMiliSecond / 1000;
// 	nMinute = nSecond / 60;
// 	nHour = nMinute / 60;
// 
// 	CString strInfo;
// 	strInfo.Format(_T("%02d:%02d:%02d.%03d"), nHour % 24, nMinute % 60, nSecond % 60, nMiliSecond % 1000);
// 	return strInfo;
// }

int CDataClass::SaveBmp(const TCHAR* pPathName, int iWidth, int iHeight, int iByte, TBYTE *pDest)
{
	BITMAPFILEHEADER bmFH;
	int nPaletteEntries = 256;
	DWORD dwSize = iWidth * iHeight*iByte;

	if (iByte < 3)
	{
		nPaletteEntries = 256;
	}
	else
	{
		nPaletteEntries = 0;
	}

	bmFH.bfType = 19778;
	//	bmFH.bfType = "BM";

	bmFH.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
		sizeof(BITMAPINFOHEADER) +
		nPaletteEntries * sizeof(RGBQUAD)) +
		dwSize;
	bmFH.bfReserved1 = bmFH.bfReserved2 = 0;
	bmFH.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) +
		sizeof(BITMAPINFOHEADER) +
		nPaletteEntries * sizeof(RGBQUAD);

	BITMAPINFO *FileBMP;
	char chBmpBuf[2048];
	FileBMP = (BITMAPINFO *)chBmpBuf;
	FileBMP->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	FileBMP->bmiHeader.biWidth = iWidth;
	FileBMP->bmiHeader.biHeight = iHeight;
	FileBMP->bmiHeader.biPlanes = 1;
	FileBMP->bmiHeader.biBitCount = iByte * 8;
	FileBMP->bmiHeader.biCompression = BI_RGB;
	FileBMP->bmiHeader.biSizeImage = 0;
	FileBMP->bmiHeader.biXPelsPerMeter = 0;
	FileBMP->bmiHeader.biYPelsPerMeter = 0;
	FileBMP->bmiHeader.biClrUsed = 0;
	FileBMP->bmiHeader.biClrImportant = 0;

	//(2003-4-2)
	//对于8位位图来说，其需要调色板。对于256色地位图，调色板数组的个数位256。
	for (int k = 0; k < 256; k++)
	{
		FileBMP->bmiColors[k].rgbBlue = (BYTE)k;
		FileBMP->bmiColors[k].rgbGreen = (BYTE)k;
		FileBMP->bmiColors[k].rgbRed = (BYTE)k;
		FileBMP->bmiColors[k].rgbReserved = 0;
	}


	CFile ImageFile;

	if (ImageFile.Open(pPathName, CFile::modeCreate | CFile::modeReadWrite))
	{
		ImageFile.Write(&bmFH, sizeof(BITMAPFILEHEADER));
		ImageFile.Write(&FileBMP->bmiHeader, sizeof(BITMAPINFOHEADER));
		ImageFile.Write(&FileBMP->bmiColors[0], sizeof(RGBQUAD) * nPaletteEntries);
		ImageFile.Write(pDest, dwSize);
		ImageFile.Close();
		return 1;
	}
	else
		return -1;

}

BOOL CDataClass::CreateLogFile()
{
	int i = 0, j = 0;
	BOOL bRet = TRUE;
	CString strAppPath, strLog;
	CString strDetect, strCheck, strKick, strInfo;

	strAppPath = CExFn::GetCurrentAppPath();
	// 	strLog.Format("%sLogfiles\\MultipleObj",strAppPath);
	strLog.Format(_T("%s:\\Logfiles\\MultipleObj"), m_strVitrualDisk);
	DeleteDirectory(strLog.GetBuffer());

	CExFn::CreateMultiDirectory(strLog.GetBuffer());

	strDetect.Format(_T("%s\\Detect"), strLog);
	CExFn::CreateMultiDirectory(strDetect.GetBuffer());
	if (!PathFileExists(strDetect))
	{
		if (!CreateMultiLevelPath(strDetect))
		{
			CString sErrorInfo{ _T("") };
			sErrorInfo.Format(_T("Create logfiles path1 failed!\n%s"), strDetect);
			AfxMessageBox(sErrorInfo);
			return FALSE;
		}
	}

	strCheck.Format(_T("%s\\Check"), strLog);
	CExFn::CreateMultiDirectory(strCheck.GetBuffer());
	if (!PathFileExists(strCheck))
	{
		if (!CreateMultiLevelPath(strCheck))
		{
			CString sErrorInfo{ _T("") };
			sErrorInfo.Format(_T("Create logfiles path2 failed!\n%s"), strCheck);
			AfxMessageBox(sErrorInfo);
			return FALSE;
		}
	}

	strKick.Format(_T("%s\\Kick"), strLog);
	CExFn::CreateMultiDirectory(strKick.GetBuffer());
	if (!PathFileExists(strKick))
	{
		if (!CreateMultiLevelPath(strKick))
		{
			CString sErrorInfo{ _T("") };
			sErrorInfo.Format(_T("Create logfiles path3 failed!\n%s"), strKick);
			AfxMessageBox(sErrorInfo);
			return FALSE;
		}
	}

	m_fileRunInfoStatus.Open(strLog + _T("\\RunInfoStatus.txt"));
	for (i = 0; i < m_iCameraNum; i++)
	{
		for (j = 0; j < c_iMaxFileNum; j++)
		{
			strInfo.Format(_T("%s\\Camera%02d_%02d_"), strDetect, i + 1, j);
			bool bOpenFile = true;
			switch (j)
			{
			case 0:
				strInfo += _T("图片信息");
				break;
			default:
				bOpenFile = false;
				break;
			}
			strInfo += _T(".txt");
			if (bOpenFile)
			{
				m_fileCam[i][j].Open(strInfo, &m_bWriteLog);
				m_fileCam[i][j].SetMaxLine(m_iMaxLogLineNum);
			}
		}
	}

	for (i = 0; i < m_iCheckGroupNum; i++)
	{
		for (j = 0; j <c_iMaxFileNum; j++)
		{
			strInfo.Format(_T("%s\\Detect%02d_%02d_"), strDetect, i + 1, j);
			bool bOpenFile = true;
			switch (j)
			{
			case 0:
				strInfo += _T("是否是新周期");
				break;
			case 1:
				strInfo += _T("Detect函数来图信息");
				break;
			case 2:
				strInfo += _T("缓冲队列信息");
				break;
			case 3:
				strInfo += _T("拷图信息");
				break;
			case 4:
				strInfo += _T("触发线程检测信息");
				break;
			case 5:
				strInfo += _T("检测前剔废表信息");
				break;
			case 6:
				strInfo += _T("未收齐图直接送检信息");
				break;
			case 7:
				strInfo += _T("队列满丢图信息");
				break;
			case 8:
				strInfo += _T("Check前信息");
				break;
			case 9:
				strInfo += _T("算法存图时间");
				break;
			case 10:
				strInfo += _T("小张图张数越界丢图");
				break;
			default:
				bOpenFile = false;
				break;
			}
			strInfo += _T(".txt");
			if (bOpenFile)
			{
				m_fileDetect[i][j].Open(strInfo, &m_bWriteLog);
				m_fileDetect[i][j].SetMaxLine(m_iMaxLogLineNum);
			}
		}

		for (j = 0; j < c_iMaxFileNum; j++)
		{
			bool bOpenFile = true;
			strInfo.Format(_T("%s\\CheckGroup%02d_%02d_"), strCheck, i + 1, j);
			switch (j)
			{
			case 0:
				strInfo += _T("检测前队列信息");
				break;
			case 1:
				strInfo += _T("当前检测结果信息");
				break;
			case 2:
				strInfo += _T("检测结果信息");
				break;
			case 3:
				strInfo += _T("写剔废表信息");
				break;
			case 4:
				strInfo += _T("检测完缓存队列信息");
				break;
			case 5:
				strInfo += _T("检测线程信息");
				break;
			case 6:
				strInfo += _T("检测对象忙信息");
				break;
			case 7:
				strInfo += _T("算法异常信息");
				break;
			default:
				bOpenFile = false;
				break;
			}
			strInfo += _T(".txt");
			if (bOpenFile)
			{
				m_fileCheck[i][j].Open(strInfo, &m_bWriteLog);
				m_fileCheck[i][j].SetMaxLine(m_iMaxLogLineNum);
			}
		}

	}

	for (i = 0; i < m_iKickQueueNum; i++)
	{
		for (j = 0; j < c_iMaxFileNum; j++)
		{
			bool bOpenFile = true;
			strInfo.Format("%s\\Kick%02d_%d_", strKick, i + 1, j);
			switch (j)
			{
			case 0:
				strInfo += _T("剔废表信息");
				break;
			case 1:
				strInfo += _T("多对象补踢日志记录");
				break;
			case 2:
				strInfo += _T("坏品信息");
				break;
			default:
				bOpenFile = false;
				break;
			}
			strInfo += _T(".txt");
			if (bOpenFile)
			{
				m_fileKick[i][j].Open(strInfo, &m_bWriteLog);
				m_fileKick[i][j].SetMaxLine(m_iMaxLogLineNum);
			}
		}
	}


	return bRet;
}

BOOL CDataClass::ThreadWriteResult(int iImageSN, int iGroupSN, int iKickSN, bool bInit)
{

	// 获取改组第一个相机号
	int iFirstCameraSN = m_iGroupFirstCam[iGroupSN];

	// 获取在剔废队列的偏移量(以图像张数为单位)，确定剔废表的纵向位置
	int	iKickY = iImageSN + m_iCamKickOffsetY[iFirstCameraSN];

	// 在剔废表中的横向位置
	int iKickX = m_iCamKickOffsetX[iFirstCameraSN];
	int iKickX_IsWritten = m_iCamKickOffsetX_Written[iFirstCameraSN];

	BYTE* pDestAddr = NULL;
	BYTE *pDestAddr2 = NULL;

	int iOffsetX1 = 0, iOffsetX1_R = 0, iOffsetY1 = 0;
	int iOutResult = 0;//**[9/22/2017 ConanSteve]**:  一般是1
	iOutResult = m_iCheckOutNum[iFirstCameraSN];

	//将检测结果分不同行列写入剔废表中
	for (int i = 0; i < m_iCheckOutNum[iFirstCameraSN]; i++)
	{
		iOffsetY1 = (iKickY + iOutResult - i - 1) % 256;
		iOffsetX1 = iKickX + (iOutResult - i - 1)*m_iGroupCheckOutPerResultSize[iGroupSN];
		iOffsetX1_R = iKickX_IsWritten + iOutResult - i - 1;

		m_csKickResult[iKickSN][iOffsetY1].Lock();
		pDestAddr = m_pKickResult[iKickSN][iOffsetY1] + iOffsetX1;
		pDestAddr2 = m_pbKickWrite[iKickSN][iOffsetY1] + iOffsetX1_R;

		int *pDefaultStatus = (int*)pDestAddr;
		*pDestAddr = m_iDefaultCheckRslt;
		BYTE *pDefaulWrite = pDestAddr2;//**[9/22/2017 ConanSteve]**:  是否已经写入了值
		if (bInit)
		{
			*pDefaultStatus = m_iLostImageRslt;//**[9/22/2017 ConanSteve]**:  丢图写-4
			*pDefaulWrite = 1;
		}
		else
		{
			*pDefaultStatus = m_iDefaultCheckRslt;//**[9/22/2017 ConanSteve]**:  默认写-3
			*pDefaulWrite = 0;
		}

		m_csKickResult[iKickSN][iOffsetY1].Unlock();

		CString sInfo{ _T("") };
		sInfo.Format(_T("[检测组号:%2d] [图像号:%8d] [剔废号:%2d] [是否初始化:%s] [目的地址:0x%08X] [是否写入:%2s] [Y偏移:%8d] [X偏移:%8d] [X_R偏移:%8d]"),
			iGroupSN, iImageSN, iKickSN, TRUE == bInit ? _T("是") : _T("否"), pDestAddr2, TRUE == *pDefaulWrite ? _T("是") : _T("否"),
			iOffsetY1, iOffsetX1, iOffsetX1_R);
		m_fileDetect[iGroupSN][5].Write(sInfo);
	}
	if (!m_bEnableOvertimeCallback)
	{
		m_csBoolKickWrite[iKickSN][iImageSN % 256].Lock();
		IsCheckCompleted(iGroupSN, iKickSN, iImageSN);
		m_csBoolKickWrite[iKickSN][iImageSN % 256].Unlock();
	}
	return TRUE;
}

int CDataClass::ShowCurrStatus()
{
	// 	int i = 0, j = 0;
	// 
	// 	if (TRUE == m_bShowStatus)
	// 	{
	// 		char chObj[MAX_CHAR_LENGTH];
	// 		char chCam[MAX_CHAR_LENGTH];
	// 		char chList[MAX_CHAR_LENGTH];
	// 		char chImage[MAX_CHAR_LENGTH];
	// 		char chTemp[MAX_CHAR_LENGTH];
	// 
	// 		for (i = 0; i < m_iCheckGroupNum; i++)
	// 		{
	// 			int iTemp = 0;
	// 
	// 			m_csWriteDataStatus[i].Lock();
	// 			for (j = 0; j < m_iGroupCheckQueueNum[i]; j++)
	// 			{
	// 				if (e_WriteDataStatus_Leisure == m_pWriteDataStatus[i][j])
	// 				{
	// 					iTemp++;
	// 				}
	// 			}
	// 
	// 			m_csWriteDataStatus[i].Unlock();
	// 
	// 			m_iGroupLeftList[i] = iTemp;
	// 		}
	// 
	// 		ZeroMemory(chList, MAX_CHAR_LENGTH);
	// 		sprintf(chList, "检测队列状态:\r\n");
	// 		for (i = 0; i < m_iCheckGroupNum; i++)
	// 		{
	// 			ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 			sprintf(chTemp, " %d ", i + 1);
	// 			strcat(chList, chTemp);
	// 
	// 			for (j = 0; j < m_iGroupCheckQueueNum[i]; j++)
	// 			{
	// 				ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 				sprintf(chTemp, "%d ", m_pWriteDataStatus[i][j]);
	// 				strcat(chList, chTemp);
	// 			}
	// 
	// 			ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 			sprintf(chTemp, "  %d\r\n", m_iGroupLeftList[i]);
	// 			strcat(chList, chTemp);
	// 		}
	// 
	// 		ZeroMemory(chImage, MAX_CHAR_LENGTH);
	// 		sprintf(chImage, "队列元素检测图像号:\r\n");
	// 		for (i = 0; i < m_iCheckGroupNum; i++)
	// 		{
	// 			ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 			sprintf(chTemp, " %d ", i + 1);
	// 			strcat(chImage, chTemp);
	// 
	// 			for (j = 0; j < m_iGroupCheckQueueNum[i]; j++)
	// 			{
	// 				ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 				sprintf(chTemp, "%d ", m_pCircleSN[i][j]);
	// 				strcat(chImage, chTemp);
	// 			}
	// 
	// 			ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 			sprintf(chTemp, "\r\n");
	// 			strcat(chImage, chTemp);
	// 		}
	// 
	// 
	// 		for (i = 0; i < m_iCheckGroupNum; i++)
	// 		{
	// 			int iTemp = 0;
	// 			for (j = 0; j < m_pnObjectNumber[i]; j++)
	// 			{
	// 				if (0 == m_nObjIndex[i][j])
	// 				{
	// 					iTemp++;
	// 				}
	// 			}
	// 			m_iGroupLeftObj[i] = iTemp;
	// 		}
	// 
	// 		ZeroMemory(chObj, MAX_CHAR_LENGTH);
	// 		sprintf(chObj, "检测对象检测状态：\r\n");
	// 		for (i = 0; i < m_iCheckGroupNum; i++)
	// 		{
	// 			ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 			sprintf(chTemp, " %d ", i + 1);
	// 			strcat(chObj, chTemp);
	// 
	// 			for (j = 0; j < m_pnObjectNumber[i]; j++)
	// 			{
	// 				ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 				sprintf(chTemp, "%d ", m_nObjIndex[i][j]);
	// 				strcat(chObj, chTemp);
	// 			}
	// 
	// 			ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 			sprintf(chTemp, "  %d \r\n", m_iGroupLeftObj[i]);
	// 			strcat(chObj, chTemp);
	// 		}
	// 
	// 		ZeroMemory(chCam, MAX_CHAR_LENGTH);
	// 		sprintf(chCam, "每个检测组丢图个数:\r\n");
	// 		for (i = 0; i < m_iCameraNum; i++)
	// 		{
	// 			ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 			sprintf(chTemp, "%d ", m_iGroupAbnormalNum[i]);
	// 			strcat(chCam, chTemp);
	// 		}
	// 
	// 		ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 		sprintf(chTemp, "\r\n相机采图个数:\r\n");
	// 		strcat(chCam, chTemp);
	// 
	// 		for (i = 0; i < m_iCameraNum; i++)
	// 		{
	// 			ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 			sprintf(chTemp, "%d ", m_iCamCircle[i]);
	// 			strcat(chCam, chTemp);
	// 		}
	// 
	// 		ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 		sprintf(chTemp, "\r\n检测个数:\r\n");
	// 		strcat(chCam, chTemp);
	// 
	// 		for (i = 0; i < m_iCheckGroupNum; i++)
	// 		{
	// 			ZeroMemory(chTemp, MAX_CHAR_LENGTH);
	// 			sprintf(chTemp, " %d", m_iGroupAlgCheck[i]);
	// 			strcat(chCam, chTemp);
	// 		}
	// 		m_pObjStatusDlg->ShowInfo(chObj, chList, chImage, chCam);
	// 	}

	return 0;
}


int CDataClass::KillAllThread()
{
	DWORD dwResult = -1;

	//Sleep(1000);

	for (int i = 0; i < m_iCheckThreadNum; i++)
	{
		dwResult = WaitForSingleObject(m_evtKillInspThread[i], 1000);
		if (dwResult == WAIT_OBJECT_0)
		{
			TRACE(_T("...检测线程%d退出成功!\n"), i + 1);
		}
	}

	dwResult = WaitForSingleObject(m_evtKillKickThread, INFINITE);
	if (dwResult == WAIT_OBJECT_0)
	{
		TRACE("...剔废线程退出成功!\n");
	}
	return 0;
}

int CDataClass::ReleaseThreadData()
{



	/************************************************************************/
	if (e_CallType_Demo != m_iCallType)
	{
		m_bKillThread = true;
		KillAllThread();
	}
	CString strDllPath;
	int i = 0, j = 0;
	for (i = 0; i < m_iCheckGroupNum; ++i)
	{
		strDllPath = m_strArrDllPath.GetAt(i);
		if (strDllPath == _T("NULL"))
		{
			continue;
		}

		HMODULE hDll = LoadLibrary(strDllPath.GetBuffer(0));
		if (hDll)
		{
			pFunc_Destroy destroyobj;
			destroyobj = (pFunc_Destroy)(GetProcAddress(hDll, _T("destroy_instance")));
			if (destroyobj&&m_bEnableInitObjectAndCheck)
			{
				CInspBase* pInsp1 = NULL;
				for (int j = 0; j < m_pnObjectNumber[i]; ++j)
				{
					pInsp1 = (CInspBase*)m_iIdleJob[i] + j;
					break;

				}
				//destroyobj(pInsp1);
				FreeLibrary(hDll);
			}
			else
			{

			}
		}
		else
		{

		}

		// 释放内存
		// 		if (m_nObjIndex[i] != NULL)
		// 		{
		// 			delete[] m_nObjIndex[i];
		// 			m_nObjIndex[i] = NULL;
		// 		}
	}//endfor

	if (m_pnObjectNumber != NULL)
	{
		delete[] m_pnObjectNumber;
		m_pnObjectNumber = NULL;
	}



	if (m_plObjBusyTimes != NULL)
	{
		delete[] m_plObjBusyTimes;
		m_plObjBusyTimes = NULL;
	}

	for (i = 0; i < m_iCheckGroupNum; i++)
	{
		// 释放数据内存
		if (m_pImageData[i] != NULL)
		{
			delete[]m_pImageData[i];
			m_pImageData[i] = NULL;
		}

		// 释放检测队列对象状态使用的内存
		if (m_pWriteDataStatus[i] != NULL)
		{
			delete[]m_pWriteDataStatus[i];
			m_pWriteDataStatus[i] = NULL;
		}

		if (m_pDetectStatus[i] != NULL)
		{
			delete[]m_pDetectStatus[i];
			m_pDetectStatus[i] = NULL;
		}

		// 释放检测队列对象编号使用的内存
		if (m_pCircleSN[i] != NULL)
		{
			delete[]m_pCircleSN[i];
			m_pCircleSN[i] = NULL;
		}

		// 是否检测线程内存
		if (m_spCheckInParam[i] != NULL)
		{
			delete[]m_spCheckInParam[i];
			m_spCheckInParam[i] = NULL;
		}

		if (m_spCheckOutParam[i] != NULL)
		{
			delete[]m_spCheckOutParam[i];
			m_spCheckOutParam[i] = NULL;
		}

		if (m_spCheckStatusParam[i] != NULL)
		{
			delete[]m_spCheckStatusParam[i];
			m_spCheckStatusParam[i] = NULL;
		}

		if (m_pCheckInParam[i] != NULL)
		{
			delete[]m_pCheckInParam[i];
			m_pCheckInParam[i] = NULL;
		}

		if (m_pCheckOutParam[i] != NULL)
		{
			delete[]m_pCheckOutParam[i];
			m_pCheckOutParam[i] = NULL;
		}

		if (m_pCheckStatusParam[i] != NULL)
		{
			delete[]m_pCheckStatusParam[i];
			m_pCheckStatusParam[i] = NULL;
		}
	}

	for (i = 0; i < m_iKickQueueNum; i++)
	{
		for (j = 0; j < 256; j++)
		{
			m_csKickResult[i][j].Lock();
			if (m_pKickResult[i][j] != NULL)
			{
				delete[]m_pKickResult[i][j];
				m_pKickResult[i][j] = NULL;
			}

			if (m_pbKickWrite[i][j] != NULL)
			{
				delete[]m_pbKickWrite[i][j];
				m_pbKickWrite[i][j] = NULL;
			}
			m_csKickResult[i][j].Unlock();
		}
	}
	if (e_CallType_Demo != m_iCallType)
	{
		if (e_ObjDetectMode_2 == m_iObjDetectMode)
		{
			for (int i = 0; i < m_iCheckGroupNum; ++i)
			{
				delete[] m_ppCheckInParamEx[i];
				delete[] m_ppCheckOutParamEx[i];
				delete[] m_ppCheckStatusParamEx[i];
				delete[] m_sppCheckInParamEx[i];
				delete[] m_sppCheckOutParamEx[i];
				delete[] m_sppCheckStatusParamEx[i];
			}


			delete[] m_ppCheckInParamEx;
			delete[] m_ppCheckOutParamEx;
			delete[] m_ppCheckStatusParamEx;
			delete[] m_sppCheckInParamEx;
			delete[] m_sppCheckOutParamEx;
			delete[] m_sppCheckStatusParamEx;
		}
		for (int i = 0; i < m_iCheckGroupNum; ++i)
		{
			if (NULL != m_ppCacheUseCnt[i])
				delete[] m_ppCacheUseCnt[i];

		}
		if (NULL != m_ppCacheUseCnt)
			delete[] m_ppCacheUseCnt;
	}

	if (NULL != m_pDlgRunInfo)
	{
		delete m_pDlgRunInfo;
	}
	return 0;
}

int CDataClass::FnSavebmp(const TCHAR *lpszPathName, int W, int H, int nByte, TBYTE *lpDest)
{
	DWORD dwSize = W * H*nByte;
	BITMAPFILEHEADER bmFH{ 0 };
	int nPaletteEntries = 256;

	if (nByte < 3)
	{
		nPaletteEntries = 256;
	}
	else
	{
		nPaletteEntries = 0;
	}

	bmFH.bfType = 19778;
	//	bmFH.bfType = "BM";

	bmFH.bfSize = (DWORD)(sizeof(BITMAPFILEHEADER) +
		sizeof(BITMAPINFOHEADER) +
		nPaletteEntries * sizeof(RGBQUAD)) +
		dwSize;
	bmFH.bfReserved1 = bmFH.bfReserved2 = 0;
	bmFH.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) +
		sizeof(BITMAPINFOHEADER) +
		nPaletteEntries * sizeof(RGBQUAD);

	BITMAPINFO *FileBMP;
	char bmpBuf[2048];
	FileBMP = (BITMAPINFO *)bmpBuf;
	FileBMP->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	FileBMP->bmiHeader.biWidth = W;
	FileBMP->bmiHeader.biHeight = H;
	FileBMP->bmiHeader.biPlanes = 1;
	FileBMP->bmiHeader.biBitCount = nByte * 8;
	FileBMP->bmiHeader.biCompression = BI_RGB;
	FileBMP->bmiHeader.biSizeImage = 0;
	FileBMP->bmiHeader.biXPelsPerMeter = 0;
	FileBMP->bmiHeader.biYPelsPerMeter = 0;
	FileBMP->bmiHeader.biClrUsed = 0;
	FileBMP->bmiHeader.biClrImportant = 0;

	//(2003-4-2)
	//对于8位位图来说，其需要调色板。对于256色地位图，调色板数组的个数位256。
	for (int k = 0; k < 256; k++)
	{
		FileBMP->bmiColors[k].rgbBlue = (BYTE)k;
		FileBMP->bmiColors[k].rgbGreen = (BYTE)k;
		FileBMP->bmiColors[k].rgbRed = (BYTE)k;
		FileBMP->bmiColors[k].rgbReserved = 0;
	}


	CFile ImageFile;
	DWORD dwOffset = 0;

	if (ImageFile.Open(lpszPathName, CFile::modeCreate | CFile::modeReadWrite))
	{
		ImageFile.Write(&bmFH, sizeof(BITMAPFILEHEADER));
		ImageFile.Write(&FileBMP->bmiHeader, sizeof(BITMAPINFOHEADER));
		ImageFile.Write(&FileBMP->bmiColors[0], sizeof(RGBQUAD) * nPaletteEntries);
		ImageFile.Write(lpDest, dwSize);
		ImageFile.Close();
		return 1;
	}
	else
		return -1;

}

DWORD CDataClass::FnThreadSaveImg(PVOID pParam)
{
	CDataClass* pThis = (CDataClass*)(pParam);
	return pThis->FnThreadSaveImgFunc();
}

DWORD CDataClass::FnThreadSaveImgFunc()
{
	while (1)
	{
		DWORD dwRet = WaitForSingleObjectEx(m_hEventSaveImgs, 10, TRUE);
		switch (dwRet)
		{
		case WAIT_OBJECT_0:
			if (m_bKillThread)
				return 0;
			break;
		case WAIT_IO_COMPLETION:
			return 0;
		case WAIT_TIMEOUT:
			if (m_bKillThread)
			{
				return 0;
			}
		default:
			continue;
		}
		/*************************************<>*************************************/
		m_srwlImgsWaitForSave.Lock();
		while (!m_queImgsWaitForSave.empty())
		{
			s_ImgInfoWaitForSave sImgInfoWaitForSave = m_queImgsWaitForSave.front();
			m_queImgsWaitForSave.pop();
			int iGroupSN = sImgInfoWaitForSave.sCheckInParam.iCheckSN;
			int iMinIndex = sImgInfoWaitForSave.iMinIndex;
			if (0 == sImgInfoWaitForSave.sCheckInParam.iReserve4)
			{
				//**[8/8/2017 ConanSteve]**:  需要释放对象，不然会导致死锁
				(m_pResetWriteStatus[iGroupSN].get() + iMinIndex)->bSaveComplete = true;
				if ((m_pResetWriteStatus[iGroupSN].get() + iMinIndex)->bFnIsComplete())
				{
					m_csWriteDataStatus[iGroupSN].Lock();
					m_pWriteDataStatus[iGroupSN][iMinIndex] = e_WriteDataStatus_Leisure;
					m_pDetectStatus[iGroupSN][iMinIndex] = e_DetectStatus_NoChecking;
					m_csWriteDataStatus[iGroupSN].Unlock();
				}
				m_srwlImgsWaitForSave.Unlock();
				break;
			}
			CExtenInfo* pExtenInfo = (CExtenInfo*)(int(sImgInfoWaitForSave.sCheckInParam.iReserve4));
			TCHAR* lpszSaveImagePath = pExtenInfo->GetFuncStruct(e_Fn_CheckInParam_SaveImagePath).GetArrayTCHAR();

			CString sSaveImgFolderPath{ _T("") };
#ifdef UNICODE
			wstring wsSaveImgFolderPath = CStringConvert::ANSI_UCS2(lpszSaveImagePath);
			sSaveImgFolderPath.Format(_T("%s"), wsSaveImgFolderPath.c_str());
#else
			sSaveImgFolderPath.Format(_T("%s"), lpszSaveImagePath);
#endif
			sSaveImgFolderPath.Format(_T("%s"), lpszSaveImagePath);
			/** Check Disk free space              [6/12/2017 ConanSteve]**/
			{

				ULONGLONG ulFreeSpace = GetFreeSpaceOfHardDisk(sSaveImgFolderPath);
				while (ulFreeSpace <= m_iSizeOfOneArrImgs && ulFreeSpace <50 * 1024 * 1024)
				{
					/** 如果删除历史记录不为空,删除整组图像              [6/12/2017 ConanSteve]**/
					if (!m_queSaveImgHistory.empty())
					{
						auto sSaveImgInfo = m_queSaveImgHistory.front();
						auto vec_sSaveImgPaths = sSaveImgInfo.vec_sSaveImgPaths;
						m_queSaveImgHistory.pop();

						for (auto itera = vec_sSaveImgPaths.begin(); itera != vec_sSaveImgPaths.end(); ++itera)
						{
							DeleteDirectory((*itera).GetBuffer(MAX_PATH));
						}

					}
					ulFreeSpace = GetFreeSpaceOfHardDisk(sSaveImgFolderPath);
				}
				//	}
				/************************************************************************/
				/*       存图                [6/12/2017 ConanSteve]                         */
				{
					CString sAlgConfigPath = m_strAppDir + _T("/Config/AlgConfig.ini");
					bool bEnableSequentialCheck = 1 == GetPrivateProfileInt(_T("Demo Controls"), _T("EnableSequentialCheck"), 0, sAlgConfigPath);
					CString sSequentialCheckFolderFilter{ _T("") };
					if (bEnableSequentialCheck)
					{
						GetPrivateProfileString(_T("Demo Controls"), _T("SequentialCheckFolderFilter"), _T(""), sSequentialCheckFolderFilter.GetBuffer(256), 256, sAlgConfigPath);
					}

					CString sSaveImgFilePath{ _T("") };/** 图像文件路径名              [6/12/2017 ConanSteve]**/
					CString sSaveImgFolderPath{ _T("") };/** 存图路径文件夹名              [6/12/2017 ConanSteve]**/
					CString sImgSNName{ _T("") };//*[7/13/2017 ConanSteve]*:  当前图片所在文件夹名称
					sImgSNName.Format(_T("\\%s%08d"), bEnableSequentialCheck ? sSequentialCheckFolderFilter : _T("Needle_"), sImgInfoWaitForSave.SN);
					sSaveImgFolderPath += sImgSNName;
					CreateDirectory(sSaveImgFolderPath, NULL);

					m_srwlImgQue[iGroupSN].Lock();
					int iImageWidth = pExtenInfo->GetFuncStruct(e_Fn_CheckInParam_ImageWidth).GetInt();
					int iImageHeight = pExtenInfo->GetFuncStruct(e_Fn_CheckInParam_ImageHeight).GetInt();
					int iImageChannel = pExtenInfo->GetFuncStruct(e_Fn_CheckInParam_ImageChannel).GetInt();
					for (int i = 0; i <pExtenInfo->GetFuncStruct(e_Fn_CheckInParam_SmallImageNumOfEachGrab).GetInt(); ++i)
					{
						sSaveImgFilePath.Format(_T("\\%02d.bmp"), i + 1);
						sSaveImgFilePath = sSaveImgFolderPath + sSaveImgFilePath;
						unsigned char* pAddr = (unsigned char*)(sImgInfoWaitForSave.sCheckInParam.pImageData + i * (iImageWidth)*(iImageHeight)*(iImageChannel));
						SaveBmp(sSaveImgFilePath, iImageWidth, iImageHeight, iImageChannel, pAddr);
					}
					m_srwlImgQue[iGroupSN].Unlock();
					/************************************************************************/
					/*                       [6/27/2017 ConanSteve]                         */
					(m_pResetWriteStatus[iGroupSN].get() + iMinIndex)->bSaveComplete = true;
					if ((m_pResetWriteStatus[iGroupSN].get() + iMinIndex)->bFnIsComplete())
					{
						m_csWriteDataStatus[iGroupSN].Lock();
						m_pWriteDataStatus[iGroupSN][iMinIndex] = e_WriteDataStatus_Leisure;
						m_pDetectStatus[iGroupSN][iMinIndex] = e_DetectStatus_NoChecking;
						m_csWriteDataStatus[iGroupSN].Unlock();
					}
					/************************************************************************/
					bool bNewImgSN{ false };
					if (m_queSaveImgHistory.empty())
					{
						bNewImgSN = true;
					}
					else
					{
						if (sImgInfoWaitForSave.SN != m_queSaveImgHistory.front().SN)
						{
							bNewImgSN = true;
						}
					}
					if (bNewImgSN)/** 新的图像号图像              [6/12/2017 ConanSteve]**/
					{
						s_ImgInfoBeenSaved sImgInfo;
						vector<CString> vec_sImgPath;
						vec_sImgPath.push_back(sSaveImgFolderPath);
						sImgInfo.SN = sImgInfoWaitForSave.SN;
						sImgInfo.vec_sSaveImgPaths = vec_sImgPath;
						m_queSaveImgHistory.push(sImgInfo);
					}
					else/** 已经存在此图像号的路径名              [6/12/2017 ConanSteve]**/
					{
						s_ImgInfoBeenSaved* pImgInfo = &m_queSaveImgHistory.front();
						pImgInfo->vec_sSaveImgPaths.push_back(sSaveImgFolderPath);
					}
				}
				/************************************************************************/
			}

		}
		m_srwlImgsWaitForSave.Unlock();
		Sleep(1);
	}
}

// BOOL CDataClass::OP_Directory(LPCSTR lpsrcPath, LPCSTR lpdstPath, int type, int flag)
// {
// 	SHFILEOPSTRUCT FileOp;
// 
// 	FileOp.hwnd = NULL;
// 	FileOp.wFunc = type;
// 	FileOp.pFrom = lpsrcPath;
// 	FileOp.pTo = lpdstPath;
// 	FileOp.fFlags = flag;//FOF_NOCONFIRMATION|FOF_NOCONFIRMMKDIR|FOF_NOERRORUI|FOF_SILENT;   
// 	FileOp.hNameMappings = NULL;
// 	FileOp.lpszProgressTitle = NULL;
// 
// 	int iRet = -1;
// 	iRet = SHFileOperation(&FileOp);
// 
// 	if (0 == iRet)
// 		return TRUE;
// 	else
// 		return FALSE;
// }


ULONGLONG CDataClass::GetFreeSpaceOfHardDisk(LPCTSTR pDisk)
{
	double dRet = 0;

	ULARGE_INTEGER lpuse;
	ULARGE_INTEGER lptotal;
	ULARGE_INTEGER lpfree;
	GetDiskFreeSpaceEx(pDisk, &lpuse, &lptotal, &lpfree);
	return lpfree.QuadPart;
}

bool CDataClass::DeleteDirectory(TCHAR* sDirName)
{

	CFileFind tempFind;

	TCHAR sTempFileFind[MAX_PATH];

	_tprintf_s(sTempFileFind, _T("%s/*.*"), sDirName);
	BOOL IsFinded = tempFind.FindFile(sTempFileFind);
	while (IsFinded)
	{

		IsFinded = tempFind.FindNextFile();

		if (!tempFind.IsDots())
		{
			TCHAR sFoundFileName[MAX_PATH]{ 0 };
			_tcscpy(sFoundFileName, tempFind.GetFileName().GetBuffer(MAX_PATH));

			if (tempFind.IsDirectory())
			{
				TCHAR sTempDir[MAX_PATH]{ 0 };
				_tprintf(sTempDir, _T("%s/%s"), sDirName, sFoundFileName);
				DeleteDirectory(sTempDir);
			}
			else
			{
				TCHAR sTempFileName[MAX_PATH]{ 0 };
				_tprintf(sTempFileName, _T("%s/%s"), sDirName, sFoundFileName);
				DeleteFile(sTempFileName);
			}
		}
	}
	tempFind.Close();
	if (!RemoveDirectory(sDirName))
	{
		return FALSE;
	}
	return TRUE;
}

s_Status CDataClass::DetectEx(s_CheckInParam sCheckInParam)
{
	s_Status sError;
	char chInfo[MAX_CHAR_LENGTH]{ 0 };
	char chLogInfo[MAX_CHAR_LENGTH]{ 0 };

	int i{ 0 }, j{ 0 };
	int iComplete{ 0 };
	int iCurrIndex{ 0 }, iMinIndex{ 0 };
	int iMinCircleSN{ MAX_IMAGE_COUNT };

	//*[7/4/2017 ConanSteve]*:  内存缓冲区是否忙
	BOOL bBusy{ TRUE };

	float fDiff{ 0.0 };
	LONGLONG lInfo{ 0 };
	LARGE_INTEGER liValue{ 0 };

	sError.iStatusType = STATUS_NOR;
	sError.pThis = m_pCallThis;
	sError.pErrorInfo = chInfo;


	// 若停止检测则不再调用
	if (m_bKillThread)
	{
		sError.iStatusType = STATUS_ILG;
		m_csErrorInfoTable.Lock();
		memcpy(&m_arrErrorInfo[m_iErrorInfoIndex][0], _T("Kill Thread!"), sizeof(_T("Kill Thread!")));
		m_iErrorInfoIndex = (m_iErrorInfoIndex + 1) % c_iErrorInfoTableLen;
		m_csErrorInfoTable.Unlock();
		sError.pErrorInfo = &m_arrErrorInfo[m_iErrorInfoIndex][0];
		return sError;
	}

	if (e_CallType_Demo != m_iCallType)
	{
		//*[6/30/2017 ConanSteve]*:  相机号              
		int iCameraSN = sCheckInParam.iCheckSN;
		//*[6/30/2017 ConanSteve]*:  ImageSN
		int iImageSN = sCheckInParam.iImageSN;
		//*[6/30/2017 ConanSteve]*:  GroupSN
		int iGroupSN = m_iCamGroup[iCameraSN];
		//**[7/28/2017 ConanSteve]**:  相机的组中索引
		int iIndexInGroup = m_iIndexInGroup[iCameraSN];
		int iKickSN = m_iCamKick[iCameraSN];
		//**[12/14/2017 ConanSteve]**:  是否是新的图像号，有可能系统使用错误，导致新周期的图像号和上一图像号相同，此时主动丢弃此图像
		bool bCamNewBigImg = m_arr_iCamLastImgSN[iCameraSN] != iImageSN;


		ASSERT(iCameraSN >= 0 && iCameraSN < m_iCameraNum);

		//*[6/30/2017 ConanSteve]*:  每一路相机进入检测的个数
		//*[6/30/2017 ConanSteve]*:  比较是否是新的大张图
		if (bCamNewBigImg)
		{
			m_iCamCircle[iCameraSN]++;
			m_iCamSmallImgCnt[iCameraSN] = 1;
		}
		else
		{
			++m_iCamSmallImgCnt[iCameraSN];
			if (m_iCamSmallImgCnt[iCameraSN] > m_arr_iCamSmallImgNum[iCameraSN])//**[12/14/2017 ConanSteve]**:  如果某一图像号的小张计数超过最大张数，丢弃此图像
			{
				CString sErrorInfo{ _T("") }, sLogInfo{ _T("") };
				sErrorInfo.Format(_T("相机%d图像号%d小张图张数%d超过最大张数%d！"), i + 1, iImageSN, m_iCamSmallImgCnt[iCameraSN], m_arr_iCamSmallImgNum[iCameraSN]);
				sError.pErrorInfo = GetErrorInfo(sErrorInfo);
				sLogInfo.Format(_T("[相机号:%2d] [图像号:%8d] [检测组号:%2d] [小张计数:%2d] [最大小张计数:%2d]"), iCameraSN, iImageSN, iGroupSN, m_arr_iCamSmallImgNum[iCameraSN], m_iCamSmallImgCnt[iCameraSN]);
				m_fileDetect[iGroupSN][10].Write(sLogInfo);
				return sError;
			}
		}
		m_arr_iCamLastImgSN[iCameraSN] = iImageSN;
		//*[6/30/2017 ConanSteve]*:  大张图像个数(正常情况下应该等于图像号)
		int iCircleSN = m_iCamCircle[iCameraSN];

		//*[6/30/2017 ConanSteve]*:  当前相机本张图像与上一张图像的时间差
		m_csCameraDiff[iCameraSN].Lock();
		fDiff = 1000 * (lInfo - m_lCamTime[iCameraSN][(iCircleSN - 1) % 256]) / m_dFreq;
		m_lCamTime[iCameraSN][iCircleSN % 256] = lInfo;
		m_csCameraDiff[iCameraSN].Unlock();


		CString sInfo{ _T("") };
		sInfo.Format(_T("[相机号:%2d] [图像号:%8d] [检测组号:%2d] [剔废号:%2d] [相机组中索引:%2d] [剔废索引:%2d] [大张个数:%8d]"),
			iCameraSN, iImageSN, iGroupSN, iKickSN + 1, iIndexInGroup, m_iIndexInKick[iCameraSN], iCircleSN);
		m_fileCam[iCameraSN][0].Write(sInfo);


		//*[6/30/2017 ConanSteve]*:  如果本相机序号是该剔废队列的离踢废口最近相机的序号，则开始对剔废超时计时
		if (m_iTimeOutIndex[iKickSN] == iCameraSN && bCamNewBigImg)
		{
			m_iKickImageSN[iKickSN][iCircleSN % 256] = iImageSN;

			if (m_bEnableOvertimeCallback)
			{
				//*[6/30/2017 ConanSteve]*:  定时基准是时间
				if (e_TimeType_MilliSecond == m_iTimeType[iKickSN])
				{
					m_spTime[iKickSN][iCircleSN % 256].Start();
				}
			}
			m_iKickCircleSN[iKickSN] = iCircleSN;
		}

		//*[6/30/2017 ConanSteve]*:  判断是否是新周期
		if (TRUE == JudgeNewCircle(iGroupSN, iCameraSN, iImageSN, iCircleSN))
		{
			iCurrIndex = 0;
			ThreadWriteResult(iImageSN, iGroupSN, iKickSN, false);
			//*[6/30/2017 ConanSteve]*:置当前相机图像为未收齐
			m_csCamComplete[iCameraSN].Lock();
			m_bCamComplete[iCameraSN][iImageSN % 256] = FALSE;
			m_csCamComplete[iCameraSN].Unlock();
			// 判断上一个周期是否完成,若未完成，则补图检测
			int iLastImageSN = m_iPeriodImageSN[iGroupSN][m_iGroupCircle[iGroupSN] % 256];
			int iLastIndex = m_iPeriodIndex[iGroupSN][m_iGroupCircle[iGroupSN] % 256];

			iComplete = 0;
			//*[7/4/2017 ConanSteve]*:  判断指定检测工位的上一组图像是否到齐
			for (i = 0; i < m_iGroupElementNum[iGroupSN]; i++)
			{
				if (m_bCamComplete[m_iGroupFirstCam[iGroupSN] + i][iLastImageSN % 256])
				{
					iComplete++;
				}
			}
			//*[7/5/2017 ConanSteve]*:  写日志
			sInfo.Format(_T("[相机号:%2d] [图像号:%8d] [上一图像号:%8d] [检测组号:%2d] [剔废队列号:%2d] [上一索引:%2d] [相机处理图像个数:%8d] [组相机收图计数:%2d] [组相机个数:%d]"),
				iCameraSN, iImageSN, iGroupSN, iKickSN, iLastImageSN, iLastIndex, m_iCamCircle[iCameraSN], iComplete,
				m_iGroupElementNum[iGroupSN]);
			m_fileDetect[iGroupSN][1].Write(sInfo);

			//*[6/30/2017 ConanSteve]*:对于实际采集，未收齐图像仍然送去检测
			if (m_iCallType == e_CallType_RealTime)
			{
				if (iComplete != m_iGroupElementNum[iGroupSN] && m_iCamCircle[iGroupSN] != 1)
				{
					// 触发检测
					SetEvent(m_hImageInspectStart[iGroupSN]);

					m_csWriteDataStatus[iGroupSN].Lock();
					m_pWriteDataStatus[iGroupSN][iLastIndex] = e_WriteDataStatus_Complete;
					m_csWriteDataStatus[iGroupSN].Unlock();

					sInfo.Format(_T("[检测组号:%2d] [相机号:%2d] [当前图像号:%08d] [上一工位图像号:%8d] [上一工位收齐数:%2d]"),
						iGroupSN, iCameraSN, iImageSN, iLastImageSN, iComplete);
					m_fileDetect[iGroupSN][6].Write(sInfo);
				}
			}

			// 查询队列中空闲对象
			m_csWriteDataStatus[iGroupSN].Lock();
			int iStatusBefore = 0;
			for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; i++)
			{
				iStatusBefore = m_pWriteDataStatus[iGroupSN][i];
				if (m_pWriteDataStatus[iGroupSN][i] != e_WriteDataStatus_Leisure)
				{
					continue;
				}
				else
				{
					bBusy = FALSE;
					iCurrIndex = i;
					m_pWriteDataStatus[iGroupSN][i] = e_WriteDataStatus_Busy;
					++m_ppCacheUseCnt[iGroupSN][i];
					break;
				}
			}
			m_csWriteDataStatus[iGroupSN].Unlock();

			/** 写日志              [5/5/2017 ConanSteve]**/
			sInfo.Format(_T("[相机号:%2d] [检测组号:%2d] [图像号:%8d] [队列是否忙:%s] [缓存队列索引:%d] [缓存写之前状态:%6s]"),
				iCameraSN, iGroupSN, iImageSN, bBusy ? _T("是") : _T("否"), iCurrIndex,
				e_WriteDataStatus_Leisure == iStatusBefore ? _T("空闲") : (e_WriteDataStatus_Busy == iStatusBefore) ? _T("正在写") : _T("已写完"));
			m_fileDetect[iGroupSN][2].Write(sInfo);

			//*[6/30/2017 ConanSteve]*:如果没有空闲对象， 将正等待检测的最小编号元素数据剔除出队列，放入最新数据	
			//*[6/30/2017 ConanSteve]*:替换最小图像号
			++m_iGroupCircle[iGroupSN];
			if (TRUE == bBusy)
			{
				m_iGroupAbnormalNum[iGroupSN]++;//*[6/30/2017 ConanSteve]*:异常图像数增加

				m_csWriteDataStatus[iGroupSN].Lock();
				for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; ++i)
				{
					if (iMinCircleSN > m_pCircleSN[iGroupSN][i] && m_pWriteDataStatus[iGroupSN][i] == e_WriteDataStatus_Complete && m_pDetectStatus[iGroupSN][i] == e_DetectStatus_NoChecking)
					{
						iMinCircleSN = m_pCircleSN[iGroupSN][i];
						iMinIndex = i;
					}
				}
				m_csWriteDataStatus[iGroupSN].Unlock();

				//*[6/30/2017 ConanSteve]*: 有未在检测的元素
				//*[6/30/2017 ConanSteve]*:有可替换的队列元素
				bool bHasCompleteNoCheck = MAX_IMAGE_COUNT != iMinCircleSN;
				sInfo.Format(_T("[新图图像号:%08d] [被替换图像号:%08d] [队列是否有未送检测数据:%4s] [丢图计数:%8d]"),
					iImageSN,
					bHasCompleteNoCheck ? m_spCheckInParam[iGroupSN][iMinCircleSN].iImageSN : 0,
					bHasCompleteNoCheck ? _T("是") : _T("否"),
					m_iGroupAbnormalNum[iGroupSN]);
				m_fileDetect[iGroupSN][7].Write(sInfo);

				if (bHasCompleteNoCheck)
				{
					//*[6/30/2017 ConanSteve]*:旧的图像号进入剔废队列
					iCurrIndex = iMinIndex;
					int iOldImageSN = m_spCheckInParam[iGroupSN][iMinIndex].iImageSN;

					//*[6/30/2017 ConanSteve]*:调用状态回调 通知系统
					char chStatusInfo[MAX_CHAR_LENGTH]{ 0 };
					char chInfo[c_iMaxStringLen]{ 0 };
					s_Status sStatus;
					sStatus.iCheckSN = iCameraSN;
					sStatus.iStatusType = STATUS_THROW_IMAGE;
					sStatus.pThis = m_pCallThis;
					sStatus.pParam = NULL;
					m_csErrorInfoTable.Lock();
					sprintf(chInfo, "Checking Image No.%d, Throwing Image No.%d!", iImageSN, iOldImageSN);
					sStatus.pErrorInfo = GetErrorInfo(chInfo);
					m_StatusCallback(&sStatus);

					m_csWriteDataStatus[iGroupSN].Lock();
					m_pWriteDataStatus[iGroupSN][iCurrIndex] = e_WriteDataStatus_Busy;
					m_csWriteDataStatus[iGroupSN].Unlock();

					//*[6/30/2017 ConanSteve]*:新来图像代替旧图像,则处于写的状态
					m_spCheckInParam[iGroupSN][iCurrIndex].iCheckSN = iGroupSN;
					m_spCheckInParam[iGroupSN][iCurrIndex].iImageSN = iImageSN;
					m_spCheckInParam[iGroupSN][iCurrIndex].iReserve1 = sCheckInParam.iReserve1;
					m_spCheckInParam[iGroupSN][iCurrIndex].iReserve2 = sCheckInParam.iReserve2;
					m_spCheckInParam[iGroupSN][iCurrIndex].iReserve3 = sCheckInParam.iReserve3;
					m_spCheckInParam[iGroupSN][iCurrIndex].iReserve4 = sCheckInParam.iReserve4;
					m_pCircleSN[iGroupSN][iCurrIndex] = m_iGroupCircle[iGroupSN];
					LARGE_INTEGER liTime;
					QueryPerformanceCounter(&liTime);
					m_arr_ldTickOfPushInCacheQueue[iGroupSN][iCurrIndex]=liTime.QuadPart;

					m_iPeriodImageSN[iGroupSN][m_iGroupCircle[iGroupSN] % 256] = sCheckInParam.iImageSN;
					m_iPeriodIndex[iGroupSN][m_iGroupCircle[iGroupSN] % 256] = iCurrIndex;


					m_srwlImgQue[iGroupSN].Lock();
					//*[6/30/2017 ConanSteve]*:求出图像缓存地址
					BYTE* pAddr = (BYTE*)(m_spCheckInParam[iGroupSN][iCurrIndex].pImageData +
						(m_iCamSmallImgCnt[iCameraSN] - 1)*m_iSmallImgSize[iCameraSN]);
					for (int i = 0; i < iIndexInGroup; ++i)
					{
						pAddr += m_iImageSize[m_iGroupFirstCam[iGroupSN] + i];//*[7/14/2017 ConanSteve]*:  加上同一工位的其他相机图像的地址偏移
					}
					memcpy_s(pAddr, m_iSmallImgSize[iCameraSN], sCheckInParam.pImageData, m_iSmallImgSize[iCameraSN]);

					BYTE* pParam = (BYTE*)(m_spCheckInParam[iGroupSN][iCurrIndex].pParam);
					memcpy_s(pParam, m_iGroupCheckInSize[iGroupSN], sCheckInParam.pParam, m_iGroupCheckInSize[iGroupSN]);
					m_srwlImgQue[iGroupSN].Unlock();

					sInfo.Format(_T("[检测组号:%2d] [相机号:%2d] [图像大张号:%8d] [图像小张号:%2d] [队列索引:%2d]"),
						iGroupSN, iCameraSN, iImageSN, m_iCamSmallImgCnt[iCameraSN], iIndexInGroup);
					m_fileDetect[iGroupSN][3].Write(sInfo);

				}
				//*[6/30/2017 ConanSteve]*:无可替换的队列元素
				else
				{
					sError.iStatusType = STATUS_ILG;
					ZeroMemory(chInfo, MAX_CHAR_LENGTH);

					CString str1{ _T("") }, str2{ _T("") };

					// 组编号
					str1.Format(_T("Cam:%d"), iGroupSN);

					// 图像号
					str2.Format(_T("C:%d "), iImageSN);
					str1 += str2;

					// 检测队列中的序号
					for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; i++)
					{
						str2.Format(_T("%d "), m_pCircleSN[iGroupSN][i]);
						str1 += str2;
					}

					// 检测队列中元素图像数据写入状态
					str2.Format(_T("W"));
					for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; i++)
					{
						str2.Format(_T("%d "), m_pWriteDataStatus[iGroupSN][i]);
						str1 += str2;
					}

					// 检测队列中元素检测状态
					str2.Format(_T("D"));
					for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; i++)
					{
						str2.Format(_T("%d "), m_pDetectStatus[iGroupSN][i]);
						str1 += str2;
					}

					memcpy(chInfo, str1.GetBuffer(0), str1.GetLength());
					// 调用状态回调 通知系统所有队列元素已经用完，且全部参加检测
					s_Status sStatus;
					char chInfo[c_iMaxStringLen]{ 0 };
					ZeroMemory(&sStatus, sizeof(sStatus));
					sStatus.iCheckSN = iCameraSN;
					sStatus.iStatusType = STATUS_THROW_IMAGE;
					sStatus.pThis = m_pCallThis;
					sStatus.pParam = NULL;
					m_csErrorInfoTable.Lock();
					sprintf(chInfo, "Current Image No.%d, All Elements are busy!", iImageSN);
					sStatus.pErrorInfo = GetErrorInfo(chInfo);
					m_StatusCallback(&sStatus);

					return sStatus;
				}

			}
			//*[6/30/2017 ConanSteve]*:将当前图像号添加进检测队列
			else
			{
				m_csWriteDataStatus[iGroupSN].Lock();
				m_pWriteDataStatus[iGroupSN][iCurrIndex] = e_WriteDataStatus_Busy;
				m_csWriteDataStatus[iGroupSN].Unlock();

				//*[6/30/2017 ConanSteve]*:新来图像代替旧图像,则处于写的状态
				m_spCheckInParam[iGroupSN][iCurrIndex].iCheckSN = iGroupSN;
				m_spCheckInParam[iGroupSN][iCurrIndex].iImageSN = iImageSN;
				m_spCheckInParam[iGroupSN][iCurrIndex].iReserve1 = sCheckInParam.iReserve1;
				m_spCheckInParam[iGroupSN][iCurrIndex].iReserve2 = sCheckInParam.iReserve2;
				m_spCheckInParam[iGroupSN][iCurrIndex].iReserve3 = sCheckInParam.iReserve3;
				m_spCheckInParam[iGroupSN][iCurrIndex].iReserve4 = sCheckInParam.iReserve4;
				LARGE_INTEGER liTime;
				QueryPerformanceCounter(&liTime);
				m_arr_ldTickOfPushInCacheQueue[iGroupSN][iCurrIndex]=liTime.QuadPart;

				m_pCircleSN[iGroupSN][iCurrIndex] = m_iGroupCircle[iGroupSN];

				m_iPeriodImageSN[iGroupSN][m_iGroupCircle[iGroupSN] % 256] = sCheckInParam.iImageSN;
				m_iPeriodIndex[iGroupSN][m_iGroupCircle[iGroupSN] % 256] = iCurrIndex;


				m_srwlImgQue[iGroupSN].Lock();
				//*[6/30/2017 ConanSteve]*:求出图像缓存地址
				BYTE* pAddr = (BYTE*)(m_spCheckInParam[iGroupSN][iCurrIndex].pImageData +
					(m_iCamSmallImgCnt[iCameraSN] - 1)*m_iSmallImgSize[iCameraSN]);
				for (int i = 0; i < iIndexInGroup; ++i)
				{
					pAddr += m_iImageSize[m_iGroupFirstCam[iGroupSN] + i];//*[7/14/2017 ConanSteve]*:  加上同一工位的其他相机图像的地址偏移
				}
				memcpy_s(pAddr, m_iSmallImgSize[iCameraSN], sCheckInParam.pImageData, m_iSmallImgSize[iCameraSN]);

				BYTE* pParam = (BYTE*)(m_spCheckInParam[iGroupSN][iCurrIndex].pParam);
				memcpy_s(pParam, m_iGroupCheckInSize[iGroupSN], sCheckInParam.pParam, m_iGroupCheckInSize[iGroupSN]);
				m_srwlImgQue[iGroupSN].Unlock();

				sInfo.Format(_T("[检测组号:%2d] [相机号:%2d] [图像大张号:%8d] [图像小张号:%2d] [队列索引:%2d]"),
					iGroupSN, iCameraSN, iImageSN, m_iCamSmallImgCnt[iCameraSN], iIndexInGroup);
				m_fileDetect[iGroupSN][3].Write(sInfo);

			}

		}
		//*[6/30/2017 ConanSteve]*:  不是新周期,直接拷贝图像		
		else
		{
			m_csWriteDataStatus[iGroupSN].Lock();
			int iStatusBefore = 0;
			for (i = 0; i < m_iGroupCheckQueueNum[iGroupSN]; i++)
			{
				iStatusBefore = m_pWriteDataStatus[iGroupSN][i];
				if (m_pWriteDataStatus[iGroupSN][i] == e_WriteDataStatus_Busy)
				{
					iCurrIndex = i;
					break;
				}
			}
			m_csWriteDataStatus[iGroupSN].Unlock();

			/** 写日志              [5/5/2017 ConanSteve]**/
			sInfo.Format(_T("[相机号:%2d] [检测组号:%2d] [图像号:%8d] [队列是否忙:%s] [缓存队列索引:%d] [缓存写之前状态:%6s]"),
				iCameraSN, iGroupSN, iImageSN, bBusy ? _T("是") : _T("否"), iCurrIndex,
				e_WriteDataStatus_Leisure == iStatusBefore ? _T("空闲") : (e_WriteDataStatus_Busy == iStatusBefore) ? _T("正在写") : _T("已写完"));
			m_fileDetect[iGroupSN][2].Write(sInfo);




			m_srwlImgQue[iGroupSN].Lock();
			//*[6/30/2017 ConanSteve]*:求出图像缓存地址
			BYTE* pAddr = (BYTE*)(m_spCheckInParam[iGroupSN][iCurrIndex].pImageData +
				(m_iCamSmallImgCnt[iCameraSN] - 1)*m_iSmallImgSize[iCameraSN]);
			for (int i = 0; i < iIndexInGroup; ++i)
			{
				pAddr += m_iImageSize[m_iGroupFirstCam[iGroupSN] + i];
			}
			memcpy_s(pAddr, m_iSmallImgSize[iCameraSN], sCheckInParam.pImageData, m_iSmallImgSize[iCameraSN]);
			//			BYTE* pParam = (BYTE*)(m_spCheckInParam[iGroupSN][iCurrIndex].pParam);
			// 			memcpy_s(pParam, m_iGroupCheckInSize[iGroupSN], sCheckInParam.pParam, m_iGroupCheckInSize[iGroupSN]);
			// 			m_srwlImgQue[iGroupSN].Unlock();
			sInfo.Format(_T("[检测组号:%2d] [相机号:%2d] [图像大张号:%8d] [图像小张号:%2d] [队列索引:%2d]"),
				iGroupSN, iCameraSN, iImageSN, m_iCamSmallImgCnt[iCameraSN], iIndexInGroup);
			m_fileDetect[iGroupSN][3].Write(sInfo);
		}
		//*[6/30/2017 ConanSteve]*:  如果当前相机图像全部收齐
		if (m_iCamSmallImgCnt[iCameraSN] == m_arr_iCamSmallImgNum[iCameraSN])
		{
			m_csCamComplete[iCameraSN].Lock();
			m_bCamComplete[iCameraSN][iImageSN % 256] = TRUE;
			m_csCamComplete[iCameraSN].Unlock();
		}
		//*[6/30/2017 ConanSteve]*:  查看本工位一个周期图像是否收齐
		iComplete = 0;
		for (i = 0; i < m_iGroupElementNum[iGroupSN]; i++)
		{
			if (m_bCamComplete[m_iGroupFirstCam[iGroupSN] + i][iImageSN % 256])
			{
				iComplete++;
			}
		}
		//*[6/30/2017 ConanSteve]*:  如果当前工位所有相机均收齐图像，触发检测 
		if (iComplete == m_iGroupElementNum[iGroupSN])
		{
			m_csWriteDataStatus[iGroupSN].Lock();
			m_pWriteDataStatus[iGroupSN][iCurrIndex] = e_WriteDataStatus_Complete;
			m_csWriteDataStatus[iGroupSN].Unlock();

			// 触发检测
			SetEvent(m_hImageInspectStart[iGroupSN]);

			//*[6/30/2017 ConanSteve]*:  写日志
			sInfo.Format(_T("[相机号:%2d] [检测组号:%2d] [图像号:%8d] [组入检计数:%8d] [队列索引:%2d] [剔废表是否被写入:%d]"),
				iCameraSN, iGroupSN, iImageSN, iCircleSN, iCurrIndex, *m_pbKickWrite[0][iImageSN % 256]);
			m_fileDetect[iGroupSN][4].Write(sInfo);
		}

	}

	return sError;
}


void CDataClass::CheckEx2(int iIndex, int iThreadID, s_CheckInParam sCheckInParam)
{
	/***********************************<初始化变量>*************************************/
	CString sLogInfo{ _T("") };
	int i = 0, j = 0;
	int iIdleObjIndex{ 0 };//**[7/14/2017 ConanSteve]**:  空闲检测对象索引
	int iImgSN = sCheckInParam.iImageSN;//图像号，暂时没用
	int iGroupSN = sCheckInParam.iCheckSN;//检测第几路
	bool bBusy{ true };//**[8/2/2017 ConanSteve]**:  检测对象是否全忙
	s_CheckInParam* spCheckInParam{ NULL };
	s_CheckOutParam * spCheckOutParam{ NULL };
	s_Status* spCheckStatusParam{ NULL };
	CSpendTime spTime;
	CInspBase* pInsp{ NULL };
	int iElapse1{ 0 }, iElapse2{ 0 };//**[8/2/2017 ConanSteve]**:  算法2次Detect花费时间
	s_AlgModelInput sIn;
	/*************************************<查找当前状态为空闲的检测对象>*************************************/
	{
		m_csAlgCheck[iGroupSN].Lock();//锁检测对象

		for (i = 0; i < m_pnObjectNumber[iGroupSN]; i++)
		{
			if (m_nObjIndex[iGroupSN][i] == e_DetectStatus_Checking)
			{
				continue;
			}
			iIdleObjIndex = i;
			++m_ppObjUseCnt[iGroupSN][i];
			m_nObjIndex[iGroupSN][i] = e_DetectStatus_Checking;//检测标志位设为1
			bBusy = false;
			break;
		}
		m_csAlgCheck[iGroupSN].Unlock();//返回前解锁
	}

	/*************************************<检测对象全忙>**************************************/
	if (bBusy)
	{
		InterlockedIncrement(&m_plObjBusyTimes[iGroupSN]);//m_plObjBusyTimes里面计数加1
		m_csWriteDataStatus[iGroupSN].Lock();
		m_pDetectStatus[iGroupSN][iIndex] = e_DetectStatus_NoChecking;
		m_csWriteDataStatus[iGroupSN].Unlock();

		sLogInfo.Format(_T("[错误信息:检测组%2d的检测对象全忙]"), iGroupSN + 1);
		m_fileCheck[iGroupSN][6].Write(sLogInfo);
		return;
	}
	/*************************************<有空闲检测对象处理图像>************************************/
	else
	{

		pInsp = (CInspBase*)(m_iIdleJob[iGroupSN]) + iIdleObjIndex;
		m_iGroupAlgCheck[iGroupSN]++;

		sLogInfo.Format(_T("[检测组号:%2d] [线程ID:%2d] [图像号:%8d] [缓存队列索引:%2d] [检测对象索引:%2d]"), iGroupSN, iThreadID, iImgSN, iIndex, iIdleObjIndex);
		m_fileCheck[iGroupSN][0].Write(sLogInfo);

		m_arr_ldTickOfPushInAlgObjQueue[iGroupSN][iIdleObjIndex] = m_arr_ldTickOfPushInCacheQueue[iGroupSN][iIndex];
		LARGE_INTEGER liEndTime, liFreq;
		QueryPerformanceFrequency(&liFreq);
		QueryPerformanceCounter(&liEndTime);
		m_arr_iTimeOfWaitInAlgObjQueue[iGroupSN][iIdleObjIndex]=(liEndTime.QuadPart - m_arr_ldTickOfPushInAlgObjQueue[iGroupSN][iIdleObjIndex]) * 1000 / liFreq.QuadPart;
		/*************************************<检测>*************************************/
		{
			CExtenInfo extenInfo;
			CExtenInfo* pExtenInfo{ nullptr };
			if (0 != sCheckInParam.iReserve4)//**[9/15/2017 ConanSteve]**:  判断系统是否传递了此参数，如果传递了，需要使用系统的指针
			{
				pExtenInfo = (CExtenInfo*)sCheckInParam.iReserve4;
				extenInfo = *pExtenInfo;
			}
			int iExtenInfoTemp = sCheckInParam.iReserve4;//**[9/26/2017 ConanSteve]**:  临时保存保留字4的值
			sCheckInParam.iReserve4 = (int)&extenInfo;
			if (e_ObjDetectMode_1 == m_iObjDetectMode)
			{
				spCheckInParam = &m_spCheckInParam[iGroupSN][iIndex];
				spCheckOutParam = &m_spCheckOutParam[iGroupSN][iIndex];
				spCheckStatusParam = &m_spCheckStatusParam[iGroupSN][iIndex];

				extenInfo.push_back(COptionalFunc(e_Fn_CheckInParam_DetectTimes).SetInt(0));
				sCheckInParam.iReserve3 = 0;
				spTime.Start();
				*spCheckStatusParam = pInsp->Detect(sCheckInParam, spCheckOutParam);
				spTime.End();
				iElapse1 = spTime.GetMillisecondInt();

			}
			else if (e_ObjDetectMode_2 == m_iObjDetectMode)
			{
				CopyCheckData(iIndex, iIdleObjIndex, iGroupSN);
				spCheckInParam = &m_sppCheckInParamEx[iGroupSN][iIdleObjIndex];
				spCheckOutParam = &m_sppCheckOutParamEx[iGroupSN][iIdleObjIndex];
				spCheckStatusParam = &m_sppCheckStatusParamEx[iGroupSN][iIdleObjIndex];

				extenInfo.push_back(COptionalFunc(e_Fn_CheckInParam_DetectTimes).SetInt(1));
				sCheckInParam.iReserve3 = 1;
				spTime.Start();
				*spCheckStatusParam = pInsp->Detect(sCheckInParam, spCheckOutParam);
				spTime.End();
				iElapse1 = spTime.GetMillisecondInt();
				//*[7/14/2017 ConanSteve]*:  算法拷贝图像完成，缓存队列内存置闲
				{
					m_csWriteDataStatus[iGroupSN].Lock();
					m_pWriteDataStatus[iGroupSN][iIndex] = e_WriteDataStatus_Leisure;
					m_pDetectStatus[iGroupSN][iIndex] = e_DetectStatus_NoChecking;
					m_csWriteDataStatus[iGroupSN].Unlock();
				}
				extenInfo.push_back(COptionalFunc(e_Fn_CheckInParam_DetectTimes).SetInt(2));
				sCheckInParam.iReserve3 = 2;
				spTime.Start();
				*spCheckStatusParam = pInsp->Detect(sCheckInParam, spCheckOutParam);
				spTime.End();
				iElapse2 = spTime.GetMillisecondInt();
			}
			sCheckInParam.iReserve4 = iExtenInfoTemp;
		}
		/************************************************************************/
		BOOL bRet = ThreadWriteCheckResultEx2(iGroupSN, spCheckInParam, spCheckOutParam);//**[8/2/2017 ConanSteve]**:  写剔废表

																						 //**[10/9/2017 ConanSteve]**:  填写算法时间信息
		//**[8/2/2017 ConanSteve]**:    日志：写当前检测结果信息
		sLogInfo.Format(_T("[检测组号:%2d] [线程ID:%2d] [图像号:%8d] [缓存队列索引:%2d] [是否存图:%2s] [是否画图:%2s] [1次Detect时间:%4d] [2次Detect时间:%4d] [算法检测时间:%4d] [数据等待时间:%8d]"),
			iGroupSN, iThreadID, iImgSN, iIndex, 1 == spCheckOutParam->iSave ? _T("是") : _T("否"),
			1 == spCheckOutParam->iDisplay ? _T("是") : _T("否"),
			iElapse1, iElapse2, iElapse1 + iElapse2, m_arr_iTimeOfWaitInAlgObjQueue[iGroupSN][iIdleObjIndex]);
		m_fileCheck[iGroupSN][1].Write(sLogInfo);
		/*************************************<画图>*************************************/
		{
			int nDispIndex{ e_WhetherDraw_NotDraw };
			m_csAlgDisp[iGroupSN].Lock();
			if (spCheckOutParam->iDisplay == e_CheckOutWhetherDraw_Draw && !m_pbIsDrawing[iGroupSN])	//0不显示，1显示
			{
				m_pbIsDrawing[iGroupSN] = true;
				nDispIndex = e_WhetherDraw_Draw;
			}
			m_csAlgDisp[iGroupSN].Unlock();
			if (nDispIndex == e_WhetherDraw_Draw)
			{
				sIn.iOperSig = e_AlgModelInputOper_CopyWindow;
				sIn.iCheckSN = sCheckInParam.iCheckSN;
				CExtenInfo extenInfo;
				extenInfo.push_back(COptionalFunc(e_Fn_CheckInParam_TimeWait).SetInt(m_arr_iTimeOfWaitInAlgObjQueue[iGroupSN][iIdleObjIndex]));
				extenInfo.push_back(COptionalFunc(e_Fn_CheckInParam_TimeFirstDetect).SetInt(iElapse1));
				extenInfo.push_back(COptionalFunc(e_Fn_CheckInParam_TimeSecondDetect).SetInt(iElapse2));
				int i1 = extenInfo.GetFuncStruct(e_Fn_CheckInParam_TimeFirstDetect).GetInt();// +extenInfo.GetFuncStruct(e_Fn_CheckInParam_TimeWait).GetIntEx() + extenInfo.GetFuncStruct(e_Fn_CheckInParam_TimeSecondDetect).GetIntEx();
				sIn.iReserve4 = (int)&extenInfo;
				s_Status sError2;
				sError2.iStatusType = STATUS_NOR;
				sError2.pThis = m_pCallThis;
				CString sDebugInfo{ _T("") };
				sDebugInfo.Format(_T("Group:%d"), iGroupSN);
				m_fileDebugInfo.Write(_T("-----Before-----") + sDebugInfo);
				sError2 = pInsp->Set(sIn);
				if (m_bKillThread)
					return;
				m_fileDebugInfo.Write(_T("-----After-----") + sDebugInfo);
				if (NULL != m_pbIsDrawing)
					m_pbIsDrawing[iGroupSN] = false;
			}

			(m_pResetWriteStatus[iGroupSN].get() + iIndex)->FnSetVal(false, true);
		}
		/*************************************< 保存图像>************************************[1/11/2018 ConanSteve]*/
		if (true == m_bGroupSaveImage[iGroupSN] &&										//要求保存图像
			1 == spCheckOutParam->iSave &&											//保存图像类型匹配
			m_iGroupSaveImageCount[iGroupSN] < m_iGroupSaveImageCount_SET[iGroupSN])	//保存张数未满
		{
			m_iGroupSaveImageCount[iGroupSN]++;

			m_csAlgSave[iGroupSN].Lock();
			m_iGroupIsSaving[iGroupSN] = 1;
			m_csAlgSave[iGroupSN].Unlock();

			//**[7/18/2017 ConanSteve]**:  填写存图信息
			s_AlgModelInput sIn;
			sIn.iOperSig = e_AlgModelInputOper_SaveImage;							//保存图像标志
			sIn.iCheckSN = sCheckInParam.iCheckSN;		//工位号

			if (m_iMachineType == e_MachineType_Capsule)
			{
				sIn.iReserve1 = (m_iSaveImageNum[iGroupSN] * 1000 + sCheckInParam.iImageSN);		//图像号
			}
			else if (m_iMachineType == e_MachineType_Liquor)
			{
				sIn.iReserve1 = sCheckInParam.iImageSN;		//**[7/19/2017 ConanSteve]**:  图像号
			}
			sIn.iReserve2 = (int)sCheckInParam.pImageData;	//*[7/14/2017 ConanSteve]*:  图像数据地址，完全是为了兼容，以后版本算法存图不应该使用这个地址
			m_iSaveImageNum[iGroupSN]++;


			/*****************************************************************************************/
			m_bIsCircle = false;
			bool bExecuteSaveImgOper{ true };//**[7/14/2017 ConanSteve]**:  是否需要多对象或者算法真正执行“存图”这个动作，用来指示是否放进存图队列或者调用pInsp-Set(sIn)
			CExtenInfo* pExtenPara{ NULL };//**[7/14/2017 ConanSteve]**:  扩展参数结构体
			//**[7/14/2017 ConanSteve]**:  使用了扩展结构体
			if (0 != sCheckInParam.iReserve4)
			{
				pExtenPara = (CExtenInfo*)(sCheckInParam.iReserve4);
				//pMultiObjSaveImgPara = (CMultiObjSaveImgPara *)pExtenPara->GetFuncAddr(typeid(CMultiObjSaveImgPara));
				m_bIsCircle = pExtenPara->GetFuncStruct(e_Fn_CheckInParam_WhetherCicleSaveImage).GetBool();
			}
			/*************************************<循环存图磁盘空间判断>************************************[1/11/2018 ConanSteve]*/
			if(m_bIsCircle)
			{
				if (pExtenPara->GetFuncStruct(e_Fn_CheckInParam_SaveImagePath).GetFuncCode() != e_Fn_NONE)
				{
					//*[7/6/2017 ConanSteve]*:  不循环存图,需要判断磁盘空间是否大约50M，小于时，停止存图
					CString sSaveImgFolderPath{ _T("") };
					COptionalFunc opFn = pExtenPara->GetFuncStruct(e_Fn_CheckInParam_SaveImagePath);
					sSaveImgFolderPath.Format(_T("%s"), pExtenPara->GetFuncStruct(e_Fn_CheckInParam_SaveImagePath).GetArrayTCHAR());
					if (false == m_bIsCircle)
					{
						ULONGLONG ulFreeSpace = GetFreeSpaceOfHardDisk(sSaveImgFolderPath);
						//**[7/14/2017 ConanSteve]**:  如果剩余磁盘空间小于50M，存图停止 
						if (ulFreeSpace < 1024 * 1024 * 50)
						{
							bExecuteSaveImgOper = false;
							s_Status sStatus;
							sStatus.iCheckSN = iGroupSN;
							sStatus.iStatusType = STATUS_THROW_IMAGE;
							sStatus.pThis = m_pCallThis;
							sStatus.pParam = NULL;
							sStatus.pErrorInfo = NULL;
							m_StatusCallback(&sStatus);
						}
					}
					//**[7/14/2017 ConanSteve]**:  循环存图，这里统一判断磁盘剩余空间是否大于单个产品一组图像大小之和
					//**[7/14/2017 ConanSteve]**:  多对象循环存图在真正存图操作之前依然需要判断磁盘空间，因为此时空间够，真正存图时，空间可能就不够了
					else
					{
						//**[7/14/2017 ConanSteve]**:  释放磁盘空间
						ULONGLONG ulFreeSpace = GetFreeSpaceOfHardDisk(sSaveImgFolderPath);
						while (ulFreeSpace <= m_iSizeOfOneArrImgs || ulFreeSpace <= 50 * 1024 * 1024)
						{
							//**[7/14/2017 ConanSteve]**:   如果删除历史记录不为空,删除整组图像
							m_srwlSaveImgHistory.Lock();
							if (!m_queSaveImgHistory.empty())
							{
								auto sSaveImgInfo = m_queSaveImgHistory.front();
								auto vec_sSaveImgPaths = sSaveImgInfo.vec_sSaveImgPaths;
								m_queSaveImgHistory.pop();

								for (auto itera = vec_sSaveImgPaths.begin(); itera != vec_sSaveImgPaths.end(); ++itera)
								{//**[1/25/2018 ConanSteve]**:  不管算法存的是文件夹还是单张图像，都删除
									CString sPathOfImgs = *itera;
									CExFn::DeleteDirectory(sPathOfImgs.GetBuffer());
									sPathOfImgs += _T(".bmp");
									DeleteFile(sPathOfImgs);
								}

							}
							else
							{
								m_srwlSaveImgHistory.Unlock();
								bExecuteSaveImgOper = false;//**[8/9/2017 ConanSteve]**:  算法没有使用所给的路径存图，导致删图删不掉,不再存图
								break;
							}
							m_srwlSaveImgHistory.Unlock();
							Sleep(10);
							ulFreeSpace = GetFreeSpaceOfHardDisk(sSaveImgFolderPath);
						}
					}
				}
				else
				{
					ULONGLONG ulFreeSpace = GetFreeSpaceOfHardDisk(m_strVitrualDisk + _T(":"));
					//**[7/14/2017 ConanSteve]**:  如果剩余磁盘空间小于50M，存图停止 
					if (ulFreeSpace < 1024 * 1024 * 50)
					{
						bExecuteSaveImgOper = false;
						s_Status sStatus;
						sStatus.iCheckSN = iGroupSN;
						sStatus.iStatusType = STATUS_THROW_IMAGE;
						sStatus.pThis = m_pCallThis;
						sStatus.pParam = NULL;
						sStatus.pErrorInfo = NULL;
						m_StatusCallback(&sStatus);
					}
				}
			}
			/*************************************<系统存图>************************************[1/11/2018 ConanSteve]*/
			if (e_SaveImgObj_MultiObj == m_iSaveImgObj)
			{
				/************************************************************************/
				/*                       [6/8/2017 ConanSteve]                         */
				/** 添加存图对象到队列，并唤醒存图线程              [6/12/2017 ConanSteve]**/
				if (bExecuteSaveImgOper)
				{
					m_srwlImgsWaitForSave.Lock();
					s_ImgInfoWaitForSave sImgInfoWaitForSave;
					if (e_NamingRegulation_ImgSN == m_iSaveImgNamingRegulation)
					{
						sImgInfoWaitForSave.SN = sCheckInParam.iImageSN;
					}
					else if (e_NamingRegulation_ProductSN == m_iSaveImgNamingRegulation)
					{
						sImgInfoWaitForSave.SN = sCheckInParam.iImageSN;
					}
					sImgInfoWaitForSave.sCheckInParam = sCheckInParam;
					sImgInfoWaitForSave.iMinIndex = iIndex;
					m_queImgsWaitForSave.push(sImgInfoWaitForSave);

					m_srwlImgsWaitForSave.Unlock();

					SetEvent(m_hEventSaveImgs);
					(m_pResetWriteStatus[iGroupSN].get() + iIndex)->FnSetVal(false, false);
				}

			}
			/*************************************<算法存图>************************************[1/11/2018 ConanSteve]*/
			else //if (e_SaveImgObj_Alg == m_iSaveImgObj)
			{
				if (bExecuteSaveImgOper)
				{
//					CString sSaveImgFolderPath{ _T("") };/** 存图路径文件夹名              [6/12/2017 ConanSteve]**/
					CString sImgSNName{ _T("") };//**[7/14/2017 ConanSteve]**:  图像所在文件夹名称，可以由配置文件配置
												 //**[7/19/2017 ConanSteve]**:  如果使用了循环存图，此指针不应该为空
					TCHAR lpszPathSaveImage[10][MAX_PATH];
					memset(lpszPathSaveImage, 0, sizeof(lpszPathSaveImage));
					if (e_Fn_NONE != pExtenPara->GetFuncStruct(e_Fn_CheckInParam_SaveImagePath).GetFuncCode())
					{
						//**[7/14/2017 ConanSteve]**:  确定存图命名规则
						CString sAlgConfigPath = m_strAppDir + _T("\\Config\\AlgConfig.ini");
						bool bEnableSequentialCheck = m_iniAlgCfg.GetBoolEx(_T("Demo Controls"), _T("EnableSequentialCheck"), 0, sAlgConfigPath);
						CString sSequentialCheckFolderFilter{ _T("") };
						if (bEnableSequentialCheck)
						{
							sSequentialCheckFolderFilter = m_iniAlgCfg.GetStringEx(_T("Demo Controls"), _T("SequentialCheckFolderFilter"), sSequentialCheckFolderFilter);
						}
						int iGrabNumOfCurCheckGroup = pExtenPara->GetFuncStruct(e_Fn_CheckInParam_GrabNumPerCheckGroup).GetInt();
						memcpy_s(lpszPathSaveImage, sizeof(TCHAR)*MAX_PATH*iGrabNumOfCurCheckGroup, pExtenPara->GetFuncStruct(e_Fn_CheckInParam_SaveImagePath).GetPointer(), sizeof(TCHAR)*MAX_PATH*iGrabNumOfCurCheckGroup);
						
						CString sAppend{ _T("") };
						sAppend.Format(_T("\\%s%08d"), bEnableSequentialCheck ? sSequentialCheckFolderFilter : _T("Product_"), iImgSN);
						if (0 != spCheckOutParam->iReserve4)
						{
							CExtenInfo* pExtenInfo = (CExtenInfo*)spCheckOutParam->iReserve4;
							TCHAR* pBuf = pExtenInfo->GetFuncStruct(e_Fn_CheckOutPara_AlgCheckErrorDesc).GetArrayTCHAR();
							sAppend += _T("_") + CString(pBuf);
						}
						//**[1/25/2018 ConanSteve]**:  拼接成真正的存图路径
						for (int i = 0; i < iGrabNumOfCurCheckGroup; ++i)
						{
							StrCat(lpszPathSaveImage[i], sAppend.GetBuffer());
							/*****************************************************************************************/
							//**[7/14/2017 ConanSteve]**:  将同一个图像号的存图文件夹放在一起，这样可以同步删除
							bool bNewImgSN{ false };
							m_srwlSaveImgHistory.Lock();
							if (m_queSaveImgHistory.empty())
							{
								bNewImgSN = true;
							}
							else
							{
								if (iImgSN != m_queSaveImgHistory.front().SN)
								{
									bNewImgSN = true;
								}
							}
							if (bNewImgSN)//**[7/14/2017 ConanSteve]**:  新的图像号图像,添加进存图历史中
							{
								s_ImgInfoBeenSaved sImgInfo;
								vector<CString> vec_sImgPath;
								vec_sImgPath.push_back(lpszPathSaveImage[i]);
								sImgInfo.SN = iImgSN;
								sImgInfo.vec_sSaveImgPaths = vec_sImgPath;
								m_queSaveImgHistory.push(sImgInfo);
							}
							else/** 已经存在此图像号的路径名              [6/12/2017 ConanSteve]**/
							{
								s_ImgInfoBeenSaved* pImgInfo = &m_queSaveImgHistory.front();
								pImgInfo->vec_sSaveImgPaths.push_back(lpszPathSaveImage[i]);
							}
							m_srwlSaveImgHistory.Unlock();
							/*****************************************************************************************/
						}
					}
					//**[7/14/2017 ConanSteve]**:  执行存图操作

					LARGE_INTEGER timeBegin{ 0 }, timeEnd{ 0 }, timeFrequency{ 0 };
					QueryPerformanceCounter(&timeBegin);

					sIn.iReserve3 = (int)lpszPathSaveImage[0];
					CExtenInfo extenInfo;
					extenInfo.push_back(COptionalFunc(e_Fn_AlgModelInput_ImageSN).SetInt(iImgSN));
					extenInfo.push_back(COptionalFunc(e_Fn_AlgModelInput_ImageSavedCnt).SetInt(m_iSaveImageNum[iGroupSN]));
					extenInfo.push_back(COptionalFunc(e_Fn_AlgModelInput_SaveImagePath).SetPointer(lpszPathSaveImage));
					sIn.iReserve4 = (int)&extenInfo;
					pInsp->Set(sIn);//**[7/14/2017 ConanSteve]**:  调用算法存图
					QueryPerformanceCounter(&timeEnd);
					timeEnd.QuadPart -= timeBegin.QuadPart;
					CString sInfo{ _T("") };
					QueryPerformanceFrequency(&timeFrequency);
					double ldElapse = timeEnd.QuadPart*1000.0 / timeFrequency.QuadPart;
					sInfo.Format(_T("ProductID:%d\t CameraID:%d\t  Elapse%lf\n"), iImgSN, iGroupSN, ldElapse);
					m_fileDetect[iGroupSN][9].Write(sInfo);
				}

			}




			m_csAlgSave[iGroupSN].Lock();
			m_iGroupIsSaving[iGroupSN] = 0;
			m_csAlgSave[iGroupSN].Unlock();
		}


		PostMessage(NULL, DHOA_MSG_CHECK_FINISH, iGroupSN, iIdleObjIndex);

		/**********************************<算法异常判断>**************************************/
		{
			int iResult1 = 0;
			bool bHaveD = FALSE;
			int iFirstCameraSN = m_iGroupFirstCam[iGroupSN];
			int iOutCount = 0;
			bool bAlgException{ false };
			CString sCurInfo{ _T("") }, sAllInfo{ _T("") };
			iOutCount = m_iCheckOutNum[iFirstCameraSN];
			for (int i = 0; i < iOutCount; i++)
			{
				iResult1 = *((int*)((BYTE*)spCheckOutParam->pParam + (iOutCount - 1 - i)*m_iGroupCheckOutPerResultSize[iGroupSN]));
				sCurInfo.Format(_T("%2d"), iResult1);
				sAllInfo += sCurInfo;
				if (m_iAlgExceptionVal != 0 && iResult1 == m_iAlgExceptionVal)//**[8/1/2017 ConanSteve]**:  如果算法异常，记录日志
					bAlgException = true;
				if (iResult1 > 0)
				{
					bHaveD = TRUE;
				}
			}
			if (bAlgException)
			{
				++m_arr_iAlgExceptionCnt[iGroupSN];//**[8/1/2017 ConanSteve]**:  算法异常计数加1
			}
			if (TRUE == m_bWriteLog)
			{
				if (TRUE == bHaveD)
				{
					m_iGroupErrorCount[iGroupSN]++;
				}

				//**[7/14/2017 ConanSteve]**:  检测结果信息
				CString strInfo;
				strInfo.Format(_T("[检测组号:%2d] [线程ID:%2d] [图像号:%8d] [返回状态类型:%d] [算法检测时间:%4d] [缓存队列索引:%2d] [错误计数:%8d] [检测结果:%s]"),
					iGroupSN, iThreadID, iImgSN, spCheckStatusParam->iStatusType, iElapse1 + iElapse2,
					iIndex,
					m_iGroupErrorCount[iGroupSN], sAllInfo);
				m_fileCheck[iGroupSN][2].Write(strInfo);
				//**[8/1/2017 ConanSteve]**:  记录算法异常日志
				if (bAlgException)
				{
					strInfo.Format(_T("[检测组号:%2d] [线程ID:%2d] [图像号:%8d] [返回状态类型:%d] [算法异常计数:%4d] [检测结果:%s]"), iGroupSN, iThreadID, iImgSN, spCheckStatusParam->iStatusType, m_arr_iAlgExceptionCnt[iGroupSN], sAllInfo);
					m_fileCheck[iGroupSN][7].Write(strInfo);
				}
			}
		}
		/************************************************************************/
		//**[7/31/2017 ConanSteve]**:  释放检测对象
		{
			m_csAlgCheck[iGroupSN].Lock();
			m_nObjIndex[iGroupSN][iIdleObjIndex] = e_DetectStatus_NoChecking;//检完标志位设为0
			m_csAlgCheck[iGroupSN].Unlock();
		}
	}
	/************************************************************************/
	{
		if (spCheckStatusParam->iStatusType != 0)
		{
			// 通知系统 算法异常
			TCHAR chInfo[c_iMaxStringLen]{ 0 };
			sprintf_s(chInfo, "Group:%d Image:%d StatusType:%d", iGroupSN, m_spCheckInParam[iGroupSN][iIndex].iImageSN,
				m_spCheckStatusParam[iGroupSN][iIndex].iStatusType);
			s_Status sStatus;
			sStatus.iCheckSN = iGroupSN;
			sStatus.iStatusType = STATUS_ALG_ABNORMAL;
			sStatus.pThis = m_pCallThis;
			sStatus.pParam = m_spCheckStatusParam[iGroupSN][iIndex].pParam;
			sStatus.pErrorInfo = GetErrorInfo(chInfo);
			m_StatusCallback(&sStatus);
		}
		/*************************************<>*************************************/
		if (e_ObjDetectMode_1 == m_iObjDetectMode)
		{
			(m_pResetWriteStatus[iGroupSN].get() + iIndex)->bCheckComplete = true;
			if (e_SaveImgObj_Alg == m_iSaveImgObj)
			{
				(m_pResetWriteStatus[iGroupSN].get() + iIndex)->bSaveComplete = true;
			}
			if ((m_pResetWriteStatus[iGroupSN].get() + iIndex)->bFnIsComplete())
			{
				m_csWriteDataStatus[iGroupSN].Lock();
				m_pWriteDataStatus[iGroupSN][iIndex] = e_WriteDataStatus_Leisure;
				m_pDetectStatus[iGroupSN][iIndex] = e_DetectStatus_NoChecking;
				m_csWriteDataStatus[iGroupSN].Unlock();
			}
		}
		/************************************************************************/
		/** Write logs              [5/5/2017 ConanSteve]**/
		sLogInfo.Format(_T("[检测组号:%2d] [缓存队列索引:%2d] [图像号:%8d] [缓存写状态:%s] [缓存检测状态:%s]"),
			iGroupSN, iIndex, m_spCheckInParam[iGroupSN][iIndex].iImageSN,
			e_WriteDataStatus_Leisure == m_pWriteDataStatus[iGroupSN][iIndex] ? _T("空闲") : (e_WriteDataStatus_Busy == m_pWriteDataStatus[iGroupSN][iIndex] ? _T("写忙") : _T("写完")),
			e_DetectStatus_NoChecking == m_pDetectStatus[iGroupSN][iIndex] ? _T("未检测") : _T("检测中"));
		m_fileCheck[iGroupSN][4].Write(sLogInfo);

	}
	ShowCurrStatus();
	//if(0==iGroupSN)
	UpdateRunInfo();
}

bool CDataClass::CopyCheckData(int iIndexInCache, int iIndexOfObj, int iGroupSN)
{
	memcpy_s(&m_ppCheckInParamEx[iGroupSN][iIndexOfObj], sizeof(m_iGroupCheckInSize[iGroupSN]), &m_pCheckInParam[iGroupSN][iIndexInCache], sizeof(m_iGroupCheckInSize[iGroupSN]));
	memcpy_s(&m_ppCheckOutParamEx[iGroupSN][iIndexOfObj], sizeof(m_iGroupCheckOutSize[iGroupSN]), &m_pCheckOutParam[iGroupSN][iIndexInCache], sizeof(m_iGroupCheckOutSize[iGroupSN]));
	memcpy_s(&m_ppCheckStatusParamEx[iGroupSN][iIndexOfObj], sizeof(m_iGroupCheckStatusSize[iGroupSN]), &m_pCheckStatusParam[iGroupSN][iIndexInCache], sizeof(m_iGroupCheckStatusSize[iGroupSN]));
	memcpy_s(&m_sppCheckInParamEx[iGroupSN][iIndexOfObj], sizeof(s_CheckInParam), &m_spCheckInParam[iGroupSN][iIndexInCache], sizeof(s_CheckInParam));
	memcpy_s(&m_sppCheckOutParamEx[iGroupSN][iIndexOfObj], sizeof(s_CheckOutParam), &m_spCheckOutParam[iGroupSN][iIndexInCache], sizeof(s_CheckOutParam));
	memcpy_s(&m_sppCheckStatusParamEx[iGroupSN][iIndexOfObj], sizeof(s_Status), &m_spCheckStatusParam[iGroupSN][iIndexInCache], sizeof(s_Status));
	return 0;
}

DWORD CDataClass::KickThreadFuncEx2()
{
	if (!m_bEnableOvertimeCallback)//**[9/20/2017 ConanSteve]**:  如果不打开超时回调，直接返回
	{
		m_evtKillKickThread.SetEvent();
		return 0;
	}
	CString sInfo{ _T("") };
	int i = 0, j = 0;
	int iTime = 0, iCount = 0;
	int iOffsetY = 0;

	int* pAlgStatus{ nullptr };//**[7/19/2017 ConanSteve]**:  算法检测结果状态
	bool bKick{ false };//**[7/19/2017 ConanSteve]**:  是否要踢
	int iCircle[MAX_KICK_COUNT]{ 0 };//**[9/20/2017 ConanSteve]**:  每个踢废口
	CString sCurInfo{ _T("") }, sAllInfo{ _T("") };

	for (i = 0; i < MAX_KICK_COUNT; i++)
	{
		iCircle[i] = 1;
	}
	while (false == m_bKillThread)
	{
		for (i = 0; i < m_iKickQueueNum; i++)
		{
			for (; iCircle[i] <= m_iKickCircleSN[i]; iCircle[i]++)
			{
				iCount = 0;

				iOffsetY = m_iKickImageSN[i][iCircle[i] % 256];


				//**[7/17/2017 ConanSteve]**:   若定时基准是时间
				if (e_TimeType_MilliSecond == m_iTimeType[i])
				{
					iTime = 0;

					m_spTime[i][iCircle[i] % 256].End();
					iTime = m_spTime[i][iCircle[i] % 256].GetMillisecondInt();
				}
				//**[7/19/2017 ConanSteve]**:  计时基准是帧加行
				else if (e_TimeType_Frame == m_iTimeType[i])
				{
					iTime = 0;
					iTime = (m_iKickImageSN[i][m_iKickCircleSN[i] % 256] - iOffsetY + 256) % 256;

				}
				//**[10/20/2017 ConanSteve]**:  超时，算法未返回检测结果
				bKick = false;
				if (iTime >= m_iTimeOut[i] || iTime < 0)
				{
					++m_iKickRemediation[i];//**[8/1/2017 ConanSteve]**:  补踢计数增加					
					s_KickParam sKickParam;
					ZeroMemory(&sKickParam, sizeof(s_KickParam));
					sKickParam.pThis = m_pCallThis;
					sKickParam.iKickSN = i;
					sKickParam.iImageSN = iOffsetY;
					sKickParam.pParam = m_pKickResult[i][iOffsetY % 256];
					sKickParam.pResult = NULL;
					sAllInfo = _T("");
					for (int k = 0; k < m_iKickResultNum[i]; k++)
					{
						pAlgStatus = (int*)((BYTE*)sKickParam.pParam + k * m_iGroupCheckOutPerResultSize[0]);
						sCurInfo.Format(_T("%3d"), *pAlgStatus);

						if (k%m_iGroupCheckOutNum[0] == 0)
							sAllInfo += _T("  ");

						sAllInfo += sCurInfo;

						if (*pAlgStatus != 0)
						{
							bKick = true;
						}
					}

					if (true == bKick)
					{
						m_iKickErrorCount[i]++;
					}
					sInfo.Format(_T("[图像号:%8d]  [剔废计数:%8d]  [检测结果:%s]"),
						iOffsetY, m_iKickErrorCount[i], sAllInfo);
					m_fileKick[i][0].Write(sInfo);

					//**[8/1/2017 ConanSteve]**:  写补踢日志
					sInfo.Format(_T("[图像号:%8d]  [补踢计数:%8d]  [检测结果:%s]"),
						iOffsetY, m_iKickRemediation[i], sAllInfo);
					m_fileKick[i][1].Write(sInfo);

					m_CheckCallback(&sKickParam);

					// 超时状态回调
					TCHAR chInfo[MAX_CHAR_LENGTH];
					ZeroMemory(chInfo, MAX_CHAR_LENGTH);
					s_Status sStatus;
					ZeroMemory(&sStatus, sizeof(sStatus));
					sStatus.iCheckSN = i;
					sStatus.iStatusType = STATUS_KICK_TIMEOUT;
					sStatus.pThis = m_pCallThis;
					sStatus.pParam = NULL;
					// #ifdef UNICODE
					// 					string strInfo = CStringConvert::UCS2_ANSI(sAllInfo.GetBuffer());
					// 					memcpy_s(chInfo, strInfo.size(), strInfo.c_str(), strInfo.size());
					// #else
					// 					sprintf(chInfo, sAllInfo.GetBuffer(0), sAllInfo.GetLength());
					// #endif
					_tprintf_s(chInfo, sAllInfo.GetBuffer(), sAllInfo.GetLength() * sizeof(TCHAR));
					sStatus.pErrorInfo = chInfo;
					m_StatusCallback(&sStatus);

					for (j = 0; j < m_iKickResultNum[i]; j++)
					{
						BYTE* pAddr = NULL;
						pAddr = m_pbKickWrite[i][iOffsetY % 256] + j;
						*pAddr = 0;
					}
				}
				else
				{
					// 未超时查询结果是否已经全部写入
					for (j = 0; j < m_iKickResultNum[i]; j++)
					{
						BYTE* pAddr = m_pbKickWrite[i][iOffsetY % 256] + j;
						if (pAddr != NULL && 1 == *pAddr)
						{
							iCount++;
						}
					}

					if (iCount == m_iKickResultNum[i])
					{
						s_KickParam sKickParam;
						ZeroMemory(&sKickParam, sizeof(s_KickParam));
						sKickParam.pThis = m_pCallThis;
						sKickParam.iKickSN = i;
						sKickParam.iImageSN = iOffsetY;
						sKickParam.pParam = m_pKickResult[i][iOffsetY % 256];
						sKickParam.pResult = NULL;

						sAllInfo = _T("");
						for (int k = 0; k < m_iKickResultNum[i]; k++)
						{
							pAlgStatus = (int*)((BYTE*)sKickParam.pParam + k * m_iGroupCheckOutPerResultSize[0]);
							sCurInfo.Format(_T("%3d"), *pAlgStatus);

							if (k%m_iGroupCheckOutNum[0] == 0)
								sAllInfo += "  ";

							sAllInfo += sCurInfo;

							if (*pAlgStatus != 0)
							{
								bKick = true;
							}
						}


						if (true == bKick)
						{
							m_iKickErrorCount[i]++;
						}
						sInfo.Format(_T("[图像号:%8d]  [剔废计数:%8d]  [检测结果:%s]"),
							iOffsetY, m_iKickErrorCount[i], sAllInfo);
						m_fileKick[i][0].Write(sInfo);
						if (bKick)
						{
							m_fileKick[i][2].Write(sInfo);
						}
						m_CheckCallback(&sKickParam);
						for (j = 0; j < m_iKickResultNum[i]; j++)
						{
							BYTE* pAddr = NULL;
							pAddr = m_pbKickWrite[i][iOffsetY % 256] + j;
							*pAddr = 0;
						}
					}
					else
					{
						break;
					}
				}
			}
		}

		Sleep(1);
	}

	TRACE(_T("Kill Insp Thread!"));

	m_evtKillKickThread.SetEvent();

	return 0;
}

void CDataClass::UpdateRunInfo()
{
	CString sAllInfo{ _T("") }, sCurLine{ _T("") }, sTemp{ _T("") };
	/************************************************************************/
	//**[7/31/2017 ConanSteve]**:  缓冲队列状态及使用计数
	for (int i = 0; i < m_iCheckGroupNum; i++)
	{
		int iTemp = 0;

		m_csWriteDataStatus[i].Lock();
		for (int j = 0; j < m_iGroupCheckQueueNum[i]; j++)
		{
			if (e_WriteDataStatus_Leisure == m_pWriteDataStatus[i][j])
			{
				iTemp++;
			}
		}
		m_csWriteDataStatus[i].Unlock();
		m_iGroupLeftList[i] = iTemp;
	}

	sCurLine.Format(_T("%100s"), tr("缓冲队列状态及使用计数\r\n"));
	sAllInfo += sCurLine;
	for (int i = 0; i < m_iCheckGroupNum; ++i)
	{
		sCurLine.Format(_T("[GROUP:%02d] "), i + 1);
		sTemp.Format(_T("[%s:%2d] "), tr("剩余"), m_iGroupLeftList[i]);
		sCurLine += sTemp;
		m_csWriteDataStatus[i].Lock();
		for (int j = 0; j < m_iGroupCheckQueueNum[i]; ++j)
		{
			sTemp.Format(_T("[%2d:%s%8d] "), j + 1,
				e_WriteDataStatus_Leisure == m_pWriteDataStatus[i][j] ? _T("闲") : (e_WriteDataStatus_Busy == m_pWriteDataStatus[i][j] ? _T("忙") : _T("完")),
				m_ppCacheUseCnt[i][j]);
			sCurLine += sTemp;
		}
		m_csWriteDataStatus[i].Unlock();
		sAllInfo += sCurLine + _T("\r\n");
	}
	/************************************************************************/
	//**[7/31/2017 ConanSteve]**:  检测对象状态
	for (int i = 0; i < m_iCheckGroupNum; i++)
	{
		int iTemp = 0;
		for (int j = 0; j < m_pnObjectNumber[i]; j++)
		{
			if (0 == m_nObjIndex[i][j])
			{
				iTemp++;
			}
		}
		m_iGroupLeftObj[i] = iTemp;
	}
	sCurLine.Format(_T("%100s\r\n"), tr("检测对象状态和使用计数"));
	sAllInfo += sCurLine;
	for (int i = 0; i < m_iCheckGroupNum; ++i)
	{
		sCurLine.Format(_T("[Group:%02d] "), i + 1);
		sTemp.Format(_T("[%s:%2d] "), tr("剩余"), m_iGroupLeftObj[i]);
		sCurLine += sTemp;
		m_csAlgCheck[i].Lock();
		for (int j = 0; j < m_pnObjectNumber[i]; ++j)
		{
			sTemp.Format(_T("[%2d:%s%8d] "), j + 1, e_DetectStatus_Checking == m_nObjIndex[i][j] ? _T("忙") : _T("闲"), m_ppObjUseCnt[i][j]);
			sCurLine += sTemp;
		}
		m_csAlgCheck[i].Unlock();
		sAllInfo += sCurLine + _T("\r\n");
	}
	/************************************************************************/
	//**[7/31/2017 ConanSteve]**:  检测组丢图个数
	sCurLine.Format(_T("%100s"), tr("每个检测组丢图个数"));
	sAllInfo += sCurLine + _T("\r\n");
	sCurLine = _T("");
	for (int i = 0; i < m_iCheckGroupNum; ++i)
	{
		sTemp.Format(_T("[%2d:%8d] "), i + 1, m_iGroupAbnormalNum[i]);
		sCurLine += sTemp;
	}
	sAllInfo += sCurLine + _T("\r\n");
	/************************************************************************/
	//**[7/31/2017 ConanSteve]**:  相机采图个数
	sCurLine.Format(_T("%100s"), tr("相机采图个数"));
	sAllInfo += sCurLine + _T("\r\n");
	sCurLine = _T("");
	for (int i = 0; i < m_iCameraNum; i++)
	{
		sTemp.Format(_T("[%2d:%8d] "), i + 1, m_iCamCircle[i]);
		sCurLine += sTemp;
	}
	sAllInfo += sCurLine + _T("\r\n");
	/************************************************************************/
	//**[7/31/2017 ConanSteve]**:  每组检测个数
	sCurLine.Format(_T("%100s"), tr("每组检测个数"));
	sAllInfo += sCurLine + _T("\r\n");
	sCurLine = _T("");
	for (int i = 0; i < m_iCameraNum; i++)
	{
		sTemp.Format(_T("[%2d:%8d] "), i + 1, m_iGroupAlgCheck[i]);
		sCurLine += sTemp;
	}
	sAllInfo += sCurLine + _T("\r\n");
	/************************************************************************/
	//**[8/1/2017 ConanSteve]**:  算法异常计数
	sCurLine.Format(_T("%100s"), tr("算法异常计数"));
	sAllInfo += sCurLine + _T("\r\n");
	sCurLine = _T("");
	for (int i = 0; i < m_iCheckGroupNum; i++)
	{
		sTemp.Format(_T("[%2d:%8d] "), i + 1, m_arr_iAlgExceptionCnt[i]);
		sCurLine += sTemp;
	}
	sAllInfo += sCurLine + _T("\r\n");
	/************************************************************************/
	//**[7/31/2017 ConanSteve]**:  补剔计数
	sCurLine.Format(_T("%100s"), tr("超时计数"));
	sAllInfo += sCurLine + _T("\r\n");
	sCurLine = _T("");
	for (int i = 0; i < m_iKickQueueNum; i++)
	{
		sTemp.Format(_T("[%2d:%8d] "), i + 1, m_iKickRemediation[i]<0 ? 0 : m_iKickRemediation[i]);
		sCurLine += sTemp;
	}
	sAllInfo += sCurLine + _T("\r\n");
	/************************************************************************/



	if (m_bWriteLog)
	{
		m_csRunInfoStatus.Lock();
		m_fileRunInfoStatus.SeekToBegin();
		m_fileRunInfoStatus.Write(sAllInfo);
		m_csRunInfoStatus.Unlock();
	}
	if (m_bShowObjRunInfoWnd)
	{
		m_pDlgRunInfo->UpdateRunInfo(sAllInfo);
	}
}

TCHAR* CDataClass::GetErrorInfo(LPCTSTR lpszErrorInfo)
{
	m_csErrorInfoTable.Lock();
	if (NULL == lpszErrorInfo)
	{
		TCHAR* pTemp = &m_arrErrorInfo[(m_iErrorInfoIndex + c_iErrorInfoTableLen - 1) % c_iErrorInfoTableLen][0];
		m_csErrorInfoTable.Unlock();
		return pTemp;
	}
	char *tRet{ NULL };
	memset(&m_arrErrorInfo[m_iErrorInfoIndex][0], 0, c_iMaxStringLen);
	memcpy_s(&m_arrErrorInfo[m_iErrorInfoIndex][0], c_iMaxStringLen, lpszErrorInfo, strlen(lpszErrorInfo));
	tRet = &m_arrErrorInfo[m_iErrorInfoIndex][0];
	m_iErrorInfoIndex = (m_iErrorInfoIndex + 1) % c_iErrorInfoTableLen;
	m_csErrorInfoTable.Unlock();
	return tRet;
}

BOOL CDataClass::FlushLogFile()
{
	for (int i = 0; i < MAX_CAMERA_COUNT; ++i)
	{
		for (int j = 0; j < c_iMaxFileNum; ++j)
		{
			m_fileCam[i][j].Flush();
			m_fileDetect[i][j].Flush();
			m_fileCheck[i][j].Flush();
			m_fileKick[i][j].Flush();

		}
	}
	return TRUE;
}

void CDataClass::InitTestInfo()
{
	CXMLCfg xml(CExFn::GetCurrentAppPath() + _T("\\Config\\algConfig.xml"));
	xml.WriteInt(_T("MultiTest"), _T("MultiTest"), m_iCheckGroupNum);
}

bool CDataClass::IsCheckCompleted(int iGroupSN, int iKickSN, int iImgSN)
{
	CString sInfo{ _T("") };
	int iTime = 0, iCount = 0;

	int* pAlgStatus{ nullptr };//**[7/19/2017 ConanSteve]**:  算法检测结果状态
	bool bKickError{ false };//**[7/19/2017 ConanSteve]**:  是否要踢
	int iCircle[MAX_KICK_COUNT]{ 0 };//**[9/20/2017 ConanSteve]**:  每个踢废口
	CString sCurInfo{ _T("") }, sAllInfo{ _T("") };


	for (int j = 0; j < m_iKickResultNum[iKickSN]; j++)
	{
		BYTE* pAddr = m_pbKickWrite[iKickSN][iImgSN % 256] + j;
		if (pAddr != NULL && 1 == *pAddr)
		{
			iCount++;
		}
	}

	if (iCount == m_iKickResultNum[iKickSN])
	{
		s_KickParam sKickParam;
		ZeroMemory(&sKickParam, sizeof(s_KickParam));
		sKickParam.pThis = m_pCallThis;
		sKickParam.iKickSN = iKickSN;
		sKickParam.iImageSN = iImgSN;
		sKickParam.pParam = m_pKickResult[iKickSN][iImgSN % 256];
		sKickParam.pResult = NULL;

		sAllInfo = _T("");
		for (int k = 0; k < m_iKickResultNum[iKickSN]; k++)
		{
			pAlgStatus = (int*)((BYTE*)sKickParam.pParam + k * m_iGroupCheckOutPerResultSize[0]);
			sCurInfo.Format(_T("%3d"), *pAlgStatus);

			if (k%m_iGroupCheckOutNum[0] == 0)
				sAllInfo += _T("  ");

			sAllInfo += sCurInfo;

			if (*pAlgStatus != 0)
			{
				++m_iGroupBadCnt[iGroupSN];
				bKickError = true;
			}
			else
			{
				++m_iGroupGoodCnt[iGroupSN];
			}
		}

		if (true == bKickError)
		{
			m_iKickErrorCount[iKickSN]++;
		}
		else
		{
			++m_iKickGoodCnt[iKickSN];
		}

		sInfo.Format(_T("[图像号:%8d] [坏品计数:%8d] [检测结果:%s]"),
			iImgSN, m_iKickErrorCount[iKickSN], sAllInfo);
		m_fileKick[iKickSN][0].Write(sInfo);
		if (bKickError)
		{
			m_fileKick[iKickSN][2].Write(sInfo);
		}


		m_CheckCallback(&sKickParam);
		for (int j = 0; j < m_iKickResultNum[iKickSN]; j++)
		{
			BYTE* pAddr = NULL;
			pAddr = m_pbKickWrite[iKickSN][iImgSN % 256] + j;
			*pAddr = 0;
		}
	}
	return true;
}


bool CDataClass::ReseMultiObject()
{
	if (m_iCallType == e_CallType::e_CallType_Demo)
		return true;
	for (int i = 0; i < m_iCameraNum; ++i)
	{
		int iKickSN = m_iCamKick[i];
		int iCamIndexInKickQueue = m_iCamKickOffsetX_Written[i];
		int iKickOffsetY = m_iCamKickOffsetY[i];
		for (int j = 1; j <= iKickOffsetY; ++j)//**[9/24/2017 ConanSteve]**:  图像号是0的不存在，但是一循环之后当图像号为256时，j == 0 出现
		{
			m_pbKickWrite[iKickSN][j][iCamIndexInKickQueue] = 1;
		}
	}

	for (int iGroupIndex = 0; iGroupIndex < m_iCheckGroupNum; ++iGroupIndex)
	{
		for (int i = 0; i < 256; ++i)
		{
			int iFirstCameraSN = m_iGroupFirstCam[iGroupIndex];

			// 在剔废表中的横向位置
			int iKickX = m_iCamKickOffsetX[iFirstCameraSN];
			// 获取所在剔废队列号
			int iKickQueueSN = m_iCamKick[iFirstCameraSN];

			int *pDefaultStatus = (int*)(m_pKickResult[iKickQueueSN][i] + iKickX);
			*pDefaultStatus = m_iDefaultCheckRslt;
			// 获取在剔废队列的偏移量(以图像张数为单位)，确定剔废表的纵向位置
		}

	}
	return true;
}


bool CDataClass::Reset()
{
	return false;
}
