#include "stdafx.h"
#include "DHIOCardForSGInterior.h"



CDHIOCardForSGInterior::CDHIOCardForSGInterior()
{
}


CDHIOCardForSGInterior::~CDHIOCardForSGInterior()
{
	if (m_bEnableQueue)
		CloseCard();
}

bool CDHIOCardForSGInterior::InitCard(CIOCardInitInfo& rIOCardInitInfo)
{
	bool bRet = true;
	fill(m_arr_iTimerID.begin(), m_arr_iTimerID.end(), 0);
	fill(m_arr_sKickCntInfo.begin(), m_arr_sKickCntInfo.end(), s_KickCountInfo());
	fill(m_arr_veciQueueOutInfo.begin(), m_arr_veciQueueOutInfo.end(), vector<int>());
	m_mapTimerID2QueueID.clear();
	m_mapGrabSN2PhysicalPortId.clear();
	m_mapInportId2QueueId.clear();
	fill(m_arr_iQueueInport.begin(), m_arr_iQueueInport.end(), 0);
	fill(m_arr_iActiveInterval.begin(), m_arr_iActiveInterval.end(), 0);
	fill(m_arr_iTriggerCnt.begin(), m_arr_iTriggerCnt.end(), 0);
	fill(m_arr_iInportCnt.begin(), m_arr_iInportCnt.end(), 0);
	fill(m_arr_iOutportCnt.begin(), m_arr_iOutportCnt.end(), 0);
	fill(m_arr_bOutportStatus.begin(), m_arr_bOutportStatus.end(), false);
	fill(m_arr_OutputEventHandle.begin(), m_arr_OutputEventHandle.end(), (HANDLE)NULL);
	fill(m_arr_bEnableInternalActive.begin(), m_arr_bEnableInternalActive.end(), false);
	m_mapSysKickSN2CardKickId.clear();

	m_InitInfo = rIOCardInitInfo;
	m_sPathFileOfInit = rIOCardInitInfo.lpszPathFileOfInit;
	if (FALSE == PathFileExists(m_sPathFileOfInit))
	{
		m_sLastErrorInfo.Format(tr("接口卡配置文件\"%s\"不存在"), m_sPathFileOfInit);
		return false;
	}
	m_iniCfg.Init(m_sPathFileOfInit);
	if (e_IOCardType::e_IOCardType_SG != (e_IOCardType)m_iniCfg.GetIntEx(_T("Critical Config"), _T("接口卡类型"), (int)e_IOCardType::e_IOCardType_NONE, _T("0：无卡，1模拟接口卡，2：B卡，3：C卡，4：PCI6356卡，5：PCIE_6356卡， 6：RS232_XZXNYN串口继电器")))
	{
		m_sLastErrorInfo.Format(tr("接口卡类型不正确，当前配置文件不是虚拟接口卡配置文件，请检查系统配置文件"));
		return false;
	}


	m_iQueueNum = m_iniCfg.GetIntEx(_T("接口卡参数"), _T("采集队列数"), 1);
	int iUnitedInterval = m_iniCfg.GetIntEx(_T("接口卡参数"), _T("统一采集周期"), 500, _T("单位毫秒, 0表示每个队列触发周期不同，否则所有队列使用统一触发队列"));
	
	for (int i = 0; i < m_iQueueNum; ++i)
	{
		CString sKeyName{ _T("") };
		CString sModuleName{ _T("") };
		sModuleName.Format(_T("采集队列%d"), i);
		m_arr_iActiveInterval[i] = m_iniCfg.GetIntEx(sModuleName, _T("周期"), 500);
		int iInport = m_arr_iQueueInport[i] = m_iniCfg.GetIntEx(sModuleName, _T("队列输入口"), -1);
		m_arr_bEnableInternalActive[iInport] = m_iniCfg.GetBoolEx(sModuleName, _T("是否内触发"), true);
		m_mapInportId2QueueId.insert(make_pair(iInport, i));
		int iOutputNum = m_iniCfg.GetIntEx(sModuleName, _T("输出口数"), 1, _T("当前队列使用的输出口数量"));
		for (int j = 0; j < iOutputNum; ++j)
		{
			sKeyName.Format(_T("队列输出口%d"), j);
			int iOutport = m_iniCfg.GetIntEx(sModuleName, sKeyName, -1);
			if (0 <= iOutport&&iOutport < 24)
			{
				m_arr_veciQueueOutInfo[i].push_back(iOutport);
			}
		}
		if (0 != iUnitedInterval)
		{
			m_arr_iActiveInterval[i] = iUnitedInterval;
		}
	}


	/*************************************<系统查询接口卡各种信息文件>************************************[12/13/2017 ConanSteve]*/
	//**[12/13/2017 ConanSteve]**:  通过此文件，接口卡能对系统查询寄存器图像号， 剔废寄存器，光电寄存器的值做出正确反馈
	CString sPathFileBtwGrabAndIOCard = rIOCardInitInfo.lpszPathFileOfConnection;
	if (FALSE == PathFileExists(sPathFileBtwGrabAndIOCard))
	{
		m_sLastErrorInfo.Format(_T("接口卡关联文件\"%s\"不存在"), sPathFileBtwGrabAndIOCard);
		bRet = false;
	}
	{
		CINICfg iniCfgOfConnect(sPathFileBtwGrabAndIOCard);
		CString sModuleName = _T("基础参数");
		CString sKeyName{ _T("") };
		//**[12/13/2017 ConanSteve]**:  根据相机连接的out口准确读出图像号寄存器的值
		int iGrabNumber = iniCfgOfConnect.GetIntEx(sModuleName, _T("相机个数"), -1);
		int iCurOutport = 0;
		for (int i = 0; i < iGrabNumber; ++i)
		{
			sModuleName.Format(_T("相机%d"), i + 1);
			iCurOutport = iniCfgOfConnect.GetIntEx(sModuleName, _T("受控触发OutportId"), -1);
			if (-1 != iCurOutport)
			{
				m_mapGrabSN2PhysicalPortId.insert(make_pair(i, iCurOutport));
			}
		}
		//**[12/13/2017 ConanSteve]**:  剔废信号相关
		m_iKickQueueNum = iniCfgOfConnect.GetIntEx(_T("基础参数"), _T("剔废队列数"), 1);
		for (int i = 0; i < m_iKickQueueNum; ++i)
		{
			sModuleName.Format(_T("剔废队列%d"), i);
			int iKickNumOfCurQueue = iniCfgOfConnect.GetIntEx(sModuleName, _T("剔废口数"), 1);//**[12/13/2017 ConanSteve]**:  当前剔废队列的剔废口个数
			int iSysKickSN = i;
			//**[12/19/2017 ConanSteve]**:  暂时不用，1张卡足以
			//iSysKickSN = iniCfgOfConnect.GetIntEx(sModuleName, _T("当前接口卡剔废队列对应系统剔废队列SN"), i);
			m_mapSysKickSN2CardKickId.insert(make_pair(iSysKickSN, i));
			m_arr_sKickCntInfo[i].iKickSN = iSysKickSN;
			m_arr_sKickCntInfo[i].iKickPortNum = iKickNumOfCurQueue;
			for (int j = 0; j < iKickNumOfCurQueue; ++j)
			{
				sKeyName.Format(_T("剔废队列索引%d对应OutportId"), j);
				m_arr_sKickCntInfo[i].arr_iKickPortId[j] = iniCfgOfConnect.GetIntEx(sModuleName, sKeyName, -1);
			}
		}
	}
	return bRet;
}

bool CDHIOCardForSGInterior::OpenCard()
{
	for (int i = 0; i<MAX_OUTPORT_NUM; ++i)
	{
		CString sEventName{ _T("") };
		sEventName.Format(_T("{E3B325CF-34F9-488F-B4E3-8A32D1C43D68}%02d"), i);//**[12/13/2017 ConanSteve]**:  根据物理端口号，来触发内核事件
		m_arr_OutputEventHandle[i] = CreateEvent(NULL, TRUE, FALSE, sEventName);
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			m_sLastErrorInfo = CExFn::GetLastErrorMsg();
		}
	}
	AddMultimediaTimer();
	return true;
}

bool CDHIOCardForSGInterior::CloseCard()
{
	Enable(false);
	DeleteMultimediaTimer();
	for (int i = 0; i < MAX_OUTPORT_NUM; ++i)
	{
		CloseHandle(m_arr_OutputEventHandle[i]);
	}
	m_bCloseCard = true;
	return false;
}

bool CDHIOCardForSGInterior::ResetIOCard()
{
	CloseCard();
	InitCard(m_InitInfo);
	OpenCard();
	return false;
}

bool CDHIOCardForSGInterior::OpenTools(CWnd * pParent)
{
	if (nullptr != pParent)
	{
		m_pDlgTools = make_unique<CDlgIOCard>(pParent, this);
		m_pDlgTools->Create(IDD_DIALOG_SG);
		m_pDlgTools->m_pIOCard = this;
		m_pDlgTools->ShowWindow(SW_SHOW);
	}
	else
	{
		CDlgIOCard dlg(pParent, this);
		dlg.DoModal();
	}
	return false;
}

bool CDHIOCardForSGInterior::Enable(bool bEnable)
{
	return EnableQueue(bEnable);
}

bool CDHIOCardForSGInterior::EnableQueue(bool bEnable)
{
	//**[12/8/2017 ConanSteve]**:  如果接口卡当前状态和设置装填相同，直接返回
	if (bEnable == m_bEnableQueue)
		return true;
	m_bEnableQueue = bEnable;
	return true;
}

bool CDHIOCardForSGInterior::GetParam(s_IOCardParaInfo & para)
{
	return false;
}

bool CDHIOCardForSGInterior::SetParam(s_IOCardParaInfo & para)
{
	return false;
}

bool CDHIOCardForSGInterior::WriteKickResult(s_sRsltInfoWritten & rslt)
{
	int iKickSN = rslt.iKickSN;
	int iRstNum = rslt.iRsltNum;
	s_KickCountInfo& sKickCntInfo = m_arr_sKickCntInfo[iKickSN];
	for (int i = 0; i < iRstNum; ++i)
	{
		int iPhysicalPortId =  sKickCntInfo.arr_iKickPortId[i];
		if (-1 == iPhysicalPortId)
		{
			m_sLastErrorInfo.Format(_T("写剔废端口不能为-1"));
			return false;
		}
		if(rslt.arr_bKick[i])
			++m_arr_iOutportCnt[iPhysicalPortId];
	}
	return true;
}

bool CDHIOCardForSGInterior::NotifyCardOfGrabCallBack(s_NotifyInfoOfGrabCallback & notifyInfo)
{
	return true;
}

e_IOCardType CDHIOCardForSGInterior::GetIOCardType()
{
	return e_IOCardType_SG;
}

CString CDHIOCardForSGInterior::GetLastError()
{
	return m_sLastErrorInfo;
}

bool CDHIOCardForSGInterior::GetInputIndex(int iInputport, int& iValue)
{
	iValue = m_arr_iInportCnt[iInputport];
	return true;
}

bool CDHIOCardForSGInterior::GetGrabImgSN(int iGrabSN, int& iImageSN)
{
	auto itera_find = m_mapGrabSN2PhysicalPortId.find(iGrabSN);
	if (m_mapGrabSN2PhysicalPortId.end() != itera_find)
	{
		int iQueueID = itera_find->second;
		if (iQueueID < MAX_OUTPORT_NUM)
		{
			iImageSN = m_arr_iOutportCnt[iQueueID];
			return true;

		}
	}
	return false;
}

bool CDHIOCardForSGInterior::GetKickCountInfo(int iKickSN, s_KickCountInfo& sKickCntInfo)
{
	for (int i = 0; i < m_arr_sKickCntInfo[iKickSN].iKickPortNum; ++i)
	{
		int iPhysicsOutportId = m_arr_sKickCntInfo[iKickSN].arr_iKickPortId[i];
		if(0<= iPhysicsOutportId&&iPhysicsOutportId<24)
		m_arr_sKickCntInfo[iKickSN].arr_iKickPortCount[i] = m_arr_iOutportCnt[iPhysicsOutportId];
	}
	sKickCntInfo = m_arr_sKickCntInfo[iKickSN];
	return true;
}

bool CDHIOCardForSGInterior::SetOutIOStatus(int nOutport, bool bStatus)
{
	return true;
}

bool CDHIOCardForSGInterior::GetOutputCount(int iOutport, int &iValue)
{
	iValue =  m_arr_iOutportCnt[iOutport];
	return true;
}


bool CDHIOCardForSGInterior::AddMultimediaTimer()
{
	TIMECAPS tc;
	//获得定时器分辨率  
	if (timeGetDevCaps(&tc, sizeof(TIMECAPS)) != TIMERR_NOERROR) {
		return false;
	}

	UINT nResolution = min(max(tc.wPeriodMin, 1), tc.wPeriodMax);

	UINT nInterval = 1;
	if (nInterval < nResolution) {
		nInterval = nResolution;
	}

	//设置定时最小分辨率  
	timeBeginPeriod(nResolution);
	//设置定时器  
	m_mapTimerID2QueueID.clear();
	for (int i = 0; i < m_iQueueNum; ++i)
	{
		m_arr_iTimerID[i] = timeSetEvent(m_arr_iActiveInterval[i], 1, (LPTIMECALLBACK)CallBackFuncTimer, (DWORD)this, TIME_PERIODIC);
		m_mapTimerID2QueueID.insert(make_pair(m_arr_iTimerID[i], i));
	}
	return true;
}

void CALLBACK CDHIOCardForSGInterior::CallBackFuncTimer(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	CDHIOCardForSGInterior* pIOCard = (CDHIOCardForSGInterior*)dwUser;
	if (nullptr != pIOCard)
	{
		pIOCard->CallBackFuncTimerFunc(wTimerID, msg, dwUser, dw1, dw2);
	}
	return;
}

void CDHIOCardForSGInterior::CallBackFuncTimerFunc(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2)
{
	//**[12/17/2017 ConanSteve]**:  根据timerID找到QueueID，再根据queueid找到inport，并触发
	auto itera_find = m_mapTimerID2QueueID.find(wTimerID);
	if (itera_find != m_mapTimerID2QueueID.end())
	{
		int iQueueID = itera_find->second;
		int iInportId = m_arr_iQueueInport[iQueueID];
		if(m_arr_bEnableInternalActive[iInportId]&&m_bEnableQueue)//**[12/17/2017 ConanSteve]**:  如果是内触发
			TriggerInport(iInportId);
	}
}


bool CDHIOCardForSGInterior::DeleteMultimediaTimer()
{
	for (int i = 0; i < m_iQueueNum; ++i)
	{
		timeKillEvent(m_arr_iTimerID[i]);
	}
	return true;
}


bool CDHIOCardForSGInterior::TriggerInport(int iInportId)
{
	++m_arr_iInportCnt[iInportId];
	auto itera_find = m_mapInportId2QueueId.find(iInportId);
	if (m_mapInportId2QueueId.end() != itera_find)
	{
		int iQueueID = itera_find->second;
		++m_arr_iTriggerCnt[iQueueID];
		for (int i = 0; i < m_arr_veciQueueOutInfo[iQueueID].size(); ++i)
		{
			int iOutport = m_arr_veciQueueOutInfo[iQueueID][i];
			++m_arr_iOutportCnt[iOutport];
			SetEvent(m_arr_OutputEventHandle[iOutport]);
			//Sleep(1);
			ResetEvent(m_arr_OutputEventHandle[iOutport]);
		}
		return true;
	}
	return false;
}
