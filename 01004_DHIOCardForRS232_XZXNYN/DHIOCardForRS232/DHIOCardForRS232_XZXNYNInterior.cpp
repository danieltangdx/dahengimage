#include "stdafx.h"
#include "DHIOCardForRS232_XZXNYNInterior.h"
#include "ExFn.h"
#include "resource.h"

CDHIOCardForRS232_XZXNYNInterior::~CDHIOCardForRS232_XZXNYNInterior()
{
	if (0 != m_hDll)
	{
		FreeLibrary(m_hDll);
	}
}
bool CDHIOCardForRS232_XZXNYNInterior::InitCard(CIOCardInitInfo& rIOCardInitInfo)
{
	/*************************************<必要初始化>************************************[12/15/2017 ConanSteve]*/
	m_fileLogWrite.Open(CExFn::GetCurrentAppPath() + _T("\\RS232_XZXNYN_Write.txt"));
	m_fileLogReset.Open(CExFn::GetCurrentAppPath() + _T("\\RS232_XZXNYN_Reset.txt"));
	LARGE_INTEGER ldFreq{ 0 };
	QueryPerformanceFrequency(&ldFreq);
	m_iFrequency = ldFreq.QuadPart;

	//**[12/15/2017 ConanSteve]**:  初始化窗口
	m_dlg.Create(IDD_DIALOG_RS232_XZXNYN,NULL);
	m_dlg.SetIOCardPoint(this);
	m_dlg.ShowWindow(SW_HIDE);
	m_dlg.InitWindow();
	m_bmpOn.LoadBitmap(IDB_BITMAP_ON);
	m_bmpOff.LoadBitmap(IDB_BITMAP_OFF);


	m_InitInfo = rIOCardInitInfo;
	m_iniCfgOfInit.Init(rIOCardInitInfo.lpszPathFileOfInit);

	
	
	
	return true;
}

bool CDHIOCardForRS232_XZXNYNInterior::OpenTools(CWnd * pParent)
{
	m_dlg.InitWindow();
	m_dlg.ShowWindow(SW_SHOW);
	return false;
}

bool CDHIOCardForRS232_XZXNYNInterior::Enable(bool bEnable)
{
	return true;
}

bool CDHIOCardForRS232_XZXNYNInterior::OpenCard()
{
	m_bOpenCardSuccess = false;
	fill(m_arr_bOpen.begin(), m_arr_bOpen.end(), false);
	fill(m_arr_iTriggerRegister.begin(), m_arr_iTriggerRegister.end(), 0);
	m_sPathFileOfInit = m_InitInfo.lpszPathFileOfInit;
	m_iPort = GetPrivateProfileInt(_T("IOCard Parameters"), _T("COM"), -1, m_sPathFileOfInit);
	m_iBoardId = m_iniCfgOfInit.GetIntEx(_T("IOCard Parameters"), _T("BoardId"), 1);
	m_iTimeOfRefreshCoil = GetPrivateProfileInt(_T("IOCard Parameters"), _T("RefreshPeroid"), 10, m_sPathFileOfInit);
	m_iFrameDelayOfKick = GetPrivateProfileInt(_T("IOCard Parameters"), _T("FrameDelayOfKick"), 10, m_sPathFileOfInit);
	memset(m_arrKickTable.data(), 0, sizeof(m_arrKickTable));
	CString sKeyName{ _T("") };

	m_hDll = LoadLibrary(_T("XZXNYN.dll"));
	if (NULL == m_hDll)
	{
		m_sLastErrorInfo = CExFn::GetLastErrorMsg();
		return false;
	}
	//**[9/11/2017 ConanSteve]**:  获取函数地址
	pOpenPort = (POpenPort)::GetProcAddress(m_hDll, "OpenPort");
	if (NULL == pOpenPort)
	{
		m_sLastErrorInfo = tr("Get ProcAddress Failed!");
		return false;
	}
	//**[12/15/2017 ConanSteve]**:  获取串口继电器地址
	pClosePort = (PClosePort)::GetProcAddress(m_hDll, "ClosePort");
	pSetCoil = (PSetCoil)::GetProcAddress(m_hDll, "SetCoil");
	pSetMultCoil = (PSetMultCoil)::GetProcAddress(m_hDll, "SetMultCoil");


	(*pSetMultCoil)(m_iBoardId, 0, c_CoilNum, 0xFFFFFFFF);

	for (int i = 0; i < c_CoilNum; ++i)
	{
		m_sHisTimeInfoOfSetCoil[i].bNeedBeenReset = false;
	}

	bool bRet = 0 == pOpenPort(m_iPort);//**[9/11/2017 ConanSteve]**:  0表示打开成功
	if (!bRet)
	{
		m_sLastErrorInfo = _T("打开接口卡失败!");
		return false;
	}
// 	for (int i = 0; i < c_CoilNum; ++i)
// 	{
// 		((CStatic*)(m_dlg.GetDlgItem(IDC_STATIC_Bitmap1 + i)))->SetBitmap((HBITMAP)m_bmpOn);
// 	}
	UpdateToolsWindowInfo();
	m_bOpenCardSuccess = true;
	return true;
}

bool CDHIOCardForRS232_XZXNYNInterior::CloseCard()
{
	BOOL bRet = pClosePort();
	m_bOpenCardSuccess = false;
	CExFn::WritePrivateProfileInt(_T("IOCard Parameters"), _T("RefreshPeroid"), m_iTimeOfRefreshCoil, m_sPathFileOfInit, _T("继电器输出脉宽"));
	CExFn::WritePrivateProfileInt(_T("IOCard Parameters"), _T("FrameDelayOfKick"), m_iFrameDelayOfKick, m_sPathFileOfInit, _T("延迟输出帧"));
	m_iniCfgOfInit.WriteInt(_T("IOCard Parameters"), _T("COM"), m_iPort);
	m_iniCfgOfInit.WriteBool(_T("IOCard Parameters"), _T("BoardId"), m_iBoardId);

	WaitForSingleObject(m_hThred, INFINITE);
	//CloseHandle(m_hThred);
	return 0 == bRet;//**[9/11/2017 ConanSteve]**:  
}

bool CDHIOCardForRS232_XZXNYNInterior::GetParam(s_IOCardParaInfo& para)
{
	switch (para.type)
	{
	case IOCardParaType::RefreshPeroid:
		*para.piVal = m_iTimeOfRefreshCoil;
		break;
	case IOCardParaType::FrameDelayOfKick:
		*para.piVal = m_iFrameDelayOfKick;
		break;
	default:
		break;
	}
	return true;
}

bool CDHIOCardForRS232_XZXNYNInterior::SetParam(s_IOCardParaInfo& para)
{
	switch (para.type)
	{
	case IOCardParaType::RefreshPeroid:
		m_iTimeOfRefreshCoil = *para.piVal;
		break;
	case IOCardParaType::FrameDelayOfKick:
	{
		m_iFrameDelayOfKick = *para.piVal;

	}
		break;
	default:
		break;
	}
	return true;
}

bool CDHIOCardForRS232_XZXNYNInterior::WriteKickResult(s_sRsltInfoWritten & rslt)
{
	try
	{
		int iCoilSN = 0;	//m_iMapCoil[rslt.iKickSN];
		int iImgSN = rslt.iImgSN;
		m_arr_sKickCntInfo[0].iKickPortNum = rslt.iRsltNum;
		for (iCoilSN = 0; iCoilSN < rslt.iRsltNum; ++iCoilSN)
		{
			bool bRslt = rslt.arr_bKick[iCoilSN];
			if (iCoilSN < 0 || iCoilSN >= c_CoilNum)
				throw(_T(""));
			m_arrKickTable[iCoilSN][iImgSN % 256] = bRslt;
			m_setCoilBeWritten.insert(iCoilSN);
		}
		
	}
	catch (...)
	{
		m_sLastErrorInfo = _T("地址越界，接口卡端口配置错误");
		return false;
	}
	return true;
}

bool CDHIOCardForRS232_XZXNYNInterior::NotifyCardOfGrabCallBack(s_NotifyInfoOfGrabCallback &notifyInfo)
{
	//int StateValue = 0x0000FFFF;
	CString sInfo{ _T("") };
	m_arr_iTriggerRegister[notifyInfo.iGrabSN] = notifyInfo.iCallbackCnt;
	m_csWriteDevice.Lock();
	for (int i = 0; i < c_CoilNum; ++i)
	{
		if (m_arrKickTable[i][(notifyInfo.iCallbackCnt + 256 - m_iFrameDelayOfKick) % 256])//**[9/13/2017 ConanSteve]**:  写之前第m_iFrameDelayOfKick个的结果
		{
			sInfo += _T("T");

			m_iStateValue = BuildState(m_iStateValue, i, false);
			//pSetCoil(m_iBoardId, i, true);
// 			LARGE_INTEGER ldTime;
// 			QueryPerformanceCounter(&ldTime);
// 			m_sHisTimeInfoOfSetCoil[i].ldHisTimeOfSetCoil = ldTime.QuadPart;
// 			m_sHisTimeInfoOfSetCoil[i].bNeedBeenReset = true;
			m_arrKickTable[i][(notifyInfo.iCallbackCnt + 256 - m_iFrameDelayOfKick) % 256] = false;//**[12/15/2017 ConanSteve]**:  重置踢废表的值

			++m_arr_sKickCntInfo[0].arr_iKickPortCount[i];
		}
		else
		{
			sInfo += _T("F");
			m_iStateValue = BuildState(m_iStateValue, i, true);
		}
		
	}
	m_fileLogWrite.Write(sInfo);
	WriteDevice();
	m_csWriteDevice.Unlock();
	UpdateToolsWindowInfo();
	return true;
}

e_IOCardType CDHIOCardForRS232_XZXNYNInterior::GetIOCardType()
{
	return e_IOCardType::e_IOCardType_RS232_XZXNYN;
}

CString CDHIOCardForRS232_XZXNYNInterior::GetLastError()
{
	return m_sLastErrorInfo;
}

bool CDHIOCardForRS232_XZXNYNInterior::GetInputIndex(int iInputport, int& iValue)
{
	return 0;
}

bool CDHIOCardForRS232_XZXNYNInterior::GetGrabImgSN(int iGrabSN, int &iImageSN)
{
	iImageSN = m_arr_iTriggerRegister[iGrabSN];
	return true;
}

bool CDHIOCardForRS232_XZXNYNInterior::GetKickCountInfo(int iKickSN, s_KickCountInfo& sKickCntInfo)
{
	sKickCntInfo =  m_arr_sKickCntInfo[iKickSN];
	return true;
}

bool CDHIOCardForRS232_XZXNYNInterior::SetOutIOStatus(int iOutport, bool bStatus)
{
	return false;
}

bool CDHIOCardForRS232_XZXNYNInterior::GetOutputCount(int iOutport, int &iValue)
{
	return 0;
}

DWORD CDHIOCardForRS232_XZXNYNInterior::ThreadRefreshCoil(LPVOID pParam)
{
	CDHIOCardForRS232_XZXNYNInterior* pCard = (CDHIOCardForRS232_XZXNYNInterior*)pParam;
	return pCard->ThreadRefreshCoilFunc();;
}

DWORD CDHIOCardForRS232_XZXNYNInterior::ThreadRefreshCoilFunc()
{
	while (1)
	{
		Sleep(1);
		if (!m_bOpenCardSuccess)
			return 0;
		LARGE_INTEGER tmNow;
		QueryPerformanceCounter(&tmNow);
		int iMillisecond{ 0 };
		CString sInfo{ _T("") };
		bool bWrite{ false };
		for (int i = 0; i < c_CoilNum; ++i)
		{
			iMillisecond = (tmNow.QuadPart - m_sHisTimeInfoOfSetCoil[i].ldHisTimeOfSetCoil) * 1000 / m_iFrequency;
			if (iMillisecond > m_iTimeOfRefreshCoil&& m_sHisTimeInfoOfSetCoil[i].bNeedBeenReset)
			{
				//m_ldHisTimeInfoOfSetCoil[i] = tmNow.QuadPart;
				m_sHisTimeInfoOfSetCoil[i].bNeedBeenReset = false;
				//pSetCoil(m_iBoardId, i, false);
				sInfo += _T("F");
				bWrite = true;

				if (m_bOpenCardSuccess)
				{
					int StateValue = 0;
					pSetMultCoil = (PSetMultCoil)::GetProcAddress(m_hDll, "SetMultCoil");
					if (pSetMultCoil)
					{
						if ((*pSetMultCoil)(m_iBoardId, 0, 16, StateValue) != 0)
						{
							//this->MessageBox("设置继电器操作失败");
						}
						else
						{
							//OnStateRd();
							//break;
						}
					}
					else
					{
						//this->MessageBox("设置继电器操作失败");
						return false;
					}
				}
			}
			else
			{
				sInfo += _T("T");
			}

		}
		if(bWrite)
			m_fileLogReset.Write(sInfo);
	}
	return 0;
}

int CDHIOCardForRS232_XZXNYNInterior::BuildState(int& Value, int Bit, bool state)//组合状态,最低位时Bit为0
{
	int iBitVal = state ? 1 : 0;
	if (state)//**[12/15/2017 ConanSteve]**:  置1
	{
		Value |= (1 << Bit);
	}
	else//**[12/15/2017 ConanSteve]**:  置0
	{
		Value &= ~(1 << Bit);
	}
	return Value;

}

bool CDHIOCardForRS232_XZXNYNInterior::UpdateToolsWindowInfo()
{
	for (int i = 0; i < c_CoilNum; ++i)
	{
		int iVal = m_iStateValue >> i & 1;
		if (1 == iVal)//**[12/15/2017 ConanSteve]**:  闭合
		{
			((CStatic*)(m_dlg.GetDlgItem(IDC_STATIC_Bitmap1 + i)))->SetBitmap((HBITMAP)m_bmpOn);
			m_arr_bOpen[i] = true;
		}
		else if (0 == iVal)
		{
			((CStatic*)(m_dlg.GetDlgItem(IDC_STATIC_Bitmap1 + i)))->SetBitmap((HBITMAP)m_bmpOff);
			m_arr_bOpen[i] = false;
		}
	}
	return true;
}

bool CDHIOCardForRS232_XZXNYNInterior::SwitchState(int iBit)
{
	m_csWriteDevice.Lock();
	m_arr_bOpen[iBit] = !m_arr_bOpen[iBit];
	BuildState(m_iStateValue, iBit, m_arr_bOpen[iBit]);
	WriteDevice();
	UpdateToolsWindowInfo();
	m_csWriteDevice.Unlock();
	return false;
}

bool CDHIOCardForRS232_XZXNYNInterior::WriteDevice()
{
	if (m_bOpenCardSuccess)
	{
		if (pSetMultCoil)
		{
			if ((*pSetMultCoil)(m_iBoardId, 0, c_CoilNum, m_iStateValue) != 0)
			{
				//this->MessageBox("设置继电器操作失败");
			}
		}
		else
		{
			//this->MessageBox("设置继电器操作失败");
			return false;
		}
	}
	return true;
}
