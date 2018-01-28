#include "stdafx.h"
#include "DHIOCardForPIO24CInterior.h"


CDHIOCardForPIO24CInterior::CDHIOCardForPIO24CInterior()
{
}


CDHIOCardForPIO24CInterior::~CDHIOCardForPIO24CInterior()
{
	if (!m_bEnableClose)
	{
		CloseCard();
	}
}

bool CDHIOCardForPIO24CInterior::InitCard(CIOCardInitInfo & rIOCardInitInfo)
{
	m_InitInfo = rIOCardInitInfo;
	if (FALSE == m_InitInfo.lpszPathFileOfInit)
	{
		m_sLastError.Format(_T("接口卡初始化文件'%s'不存在!"), m_InitInfo.lpszPathFileOfInit);
		return false;
	}
	if (FALSE == m_InitInfo.lpszPathFileOfInit)
	{
		m_sLastError.Format(_T("接口卡连接文件'%s'不存在!"), m_InitInfo.lpszPathFileOfConnection);
		return false;
	}
	m_iniCfgOfConnect.Init(m_InitInfo.lpszPathFileOfConnection);
	m_iIOCardSN = m_iniCfgOfConnect.GetIntEx(_T("基础参数"), _T("接口卡编号"), 0);
	/*************************************<初始化连接文件>************************************[12/19/2017 ConanSteve]*/
	//**[12/19/2017 ConanSteve]**:  相机相关
	m_iGrabNum = m_iniCfgOfConnect.GetIntEx(_T("基础参数"), _T("相机个数"), 1);
	m_iKickQueueNum = m_iniCfgOfConnect.GetIntEx(_T("基础参数"), _T("剔废队列数"), 1);
	CString sModuleName{ _T("") }, sKeyName{ _T("") };
	for (int i = 0; i < m_iGrabNum; ++i)
	{
		sModuleName.Format(_T("相机%d"), i + 1);
		int iRegisterId = m_iniCfgOfConnect.GetIntEx(sModuleName, _T("受控触发物理端口"), -1, _T("C卡此值应该为物理端口连接的队列ID，只可能是28,29,30,31, 分别表示延时队列X，0,1,2"));
		if (-1 != iRegisterId)
		{
			m_map_GrabSN2QueueRegisterID.insert(make_pair(i, iRegisterId));
		}
	}
	//**[12/19/2017 ConanSteve]**:  剔废相关
	for (int i = 0; i < m_iKickQueueNum; ++i)
	{
		sModuleName.Format(_T("剔废队列%d"), i);
		int iKickportNumOfCurKickQueue = m_iniCfgOfConnect.GetIntEx(sModuleName, _T("剔废口数"), 1);
		int iSysKickSN = m_iniCfgOfConnect.GetIntEx(sModuleName, _T("当前接口卡剔废队列对应系统剔废队列SN"), -1);
		if (-1 != iSysKickSN)
		{
			s_KickCountInfo sKickCntInfo;
			sKickCntInfo.iKickSN = i;
			sKickCntInfo.iKickPortNum = iKickportNumOfCurKickQueue;
			for (int j = 0; j < iKickportNumOfCurKickQueue; ++j)
			{
				sKeyName.Format(_T("剔废队列索引%d对应OutportId"), j);
				int iPhysicalOutportId = m_iniCfgOfConnect.GetIntEx(sModuleName, sKeyName, -1);
				if (iPhysicalOutportId < 0 || iPhysicalOutportId>23)
				{
					AfxMessageBox(sKeyName + _T("不正确，请检查Connection文件配置！"));
				}
				sKickCntInfo.arr_iKickPortId[j] = iPhysicalOutportId;
				sKickCntInfo.arr_iKickPortCount[j] = 0;
				sKickCntInfo.arr_iAutoKickCnt[j] = 0;
				sKeyName.Format(_T("剔废队列索引%d对应ChannelId"), j);
				int iKickChannelId = m_iniCfgOfConnect.GetIntEx(sModuleName, sKeyName, -1, _T("C卡剔废out口到对应的通道ID，可能取值是0,1,2,3"));
				m_map_CardKickOutport2Channel.insert(make_pair(iPhysicalOutportId, iKickChannelId));
			}
			m_map_SysKickSN2KickCntInfo.insert(make_pair(iSysKickSN, sKickCntInfo));
		}


	}
	//**[12/19/2017 ConanSteve]**:  系统端口到接口卡端口的映射
	sModuleName = _T("Inport映射字典");
	m_iMaxInport = m_iniCfgOfConnect.GetIntEx(sModuleName, _T("接口卡最大InportID"), 0);
	for (int i = 0; i <= m_iMaxInport; ++i)
	{
		sKeyName.Format(("接口卡Inport%d对应的系统Inport"), i);
		int iSysInport = m_iniCfgOfConnect.GetIntEx(sModuleName, sKeyName, -1);
		if (-1 != iSysInport)
		{
			m_map_SysInport2CardInport.insert(make_pair(iSysInport, i));
		}
	}
	sModuleName = _T("Outport映射字典");
	m_iMaxOutport = m_iniCfgOfConnect.GetIntEx(sModuleName, _T("接口卡最大OutportID"), 0);
	for (int i = 0; i <= m_iMaxOutport; ++i)
	{
		sKeyName.Format(("接口卡Outport%d对应的系统Outport"), i);
		int iSysOutport = m_iniCfgOfConnect.GetIntEx(sModuleName, sKeyName, -1, _T("不是必须配置，只有系统需要手动设置和读取Out口状态和值时才有意义"));
		if (-1 != iSysOutport)
		{
			m_map_SysOutport2CardOutport.insert(make_pair(iSysOutport, i));
		}
	}
	m_bInitSuccess = true;
	return true;
}

e_IOCardType CDHIOCardForPIO24CInterior::GetIOCardType()
{
	return e_IOCardType();
}

bool CDHIOCardForPIO24CInterior::OpenCard()
{
	ClearData();
	bool bRet = false;
	if (!m_bInitSuccess)
	{
		m_sLastError = tr("初始化未能成功！");
		return false;
	}
	try
	{
		bRet = TRUE == m_IOCard.openCard(m_iIOCardSN);
		if (!bRet)
		{
			m_IOCard.initRegisters((const char*)m_InitInfo.lpszPathFileOfInit, "PIO24B");
			m_sLastError.Format(tr("函数'openCard'失败, 接口卡编号:%d"), m_iIOCardSN);
			return false;
		}
		if(bRet)
			m_IOCard.hardReset();
		m_IOCard.initRegisters((const char*)m_InitInfo.lpszPathFileOfInit, "PIO24B");
		if(bRet)
			m_IOCard.softReset();
	}
	catch (...)
	{
		m_sLastError = _T("InitIOCard:接口卡初始化异常!");
		return false;
	}
	m_bInitSuccess = true;
	return m_bInitSuccess;
}

bool CDHIOCardForPIO24CInterior::CloseCard()
{
	if (m_bInitSuccess)
	{
		try
		{
			m_IOCard.enable(false);
			m_IOCard.closeCard();
			m_bEnableClose = true;
		}
		catch (...)
		{
			m_sLastError = _T("关闭接口卡异常");
			return false;
		}
	}
	return true;
}

bool CDHIOCardForPIO24CInterior::ResetIOCard()
{
	ClearData();
	m_IOCard.softReset();
	return true;
}

bool CDHIOCardForPIO24CInterior::OpenTools(CWnd * pParent)
{
	m_IOCard.Show_PIO24B_DebugDialog();
	return false;
}

bool CDHIOCardForPIO24CInterior::Enable(bool bEnable)
{
	if (m_bInitSuccess)
	{
		m_IOCard.enable(bEnable);
		m_bEnableWorking = bEnable;
		return true;
	}
	return false;
}

bool CDHIOCardForPIO24CInterior::EnableQueue(bool bEnable)
{
	return false;
}

bool CDHIOCardForPIO24CInterior::GetParam(s_IOCardParaInfo & para)
{
	return false;
}

bool CDHIOCardForPIO24CInterior::SetParam(s_IOCardParaInfo & para)
{
	return false;
}

bool CDHIOCardForPIO24CInterior::WriteKickResult(s_sRsltInfoWritten & rslt)
{
	int iSysKickSN = rslt.iKickSN;
	int iImgSN = rslt.iImgSN;
	auto itera_find = m_map_SysKickSN2KickCntInfo.find(iSysKickSN);
	if (m_map_SysKickSN2KickCntInfo.end() != itera_find && m_bInitSuccess)
	{
		s_KickCountInfo& rKickCntInfoTemp = itera_find->second;
		for (int i = 0; i < rslt.iRsltNum; ++i)
		{
			int iPhysicalOutportId = rKickCntInfoTemp.arr_iKickPortId[i];
			int iChannelId = m_map_CardKickOutport2Channel[iPhysicalOutportId];
			m_IOCard.sendChckRslt(rslt.arr_bKick[i]?TRUE:FALSE, iImgSN % 256, iChannelId);
		}
		return true;
	}
	return false;
}



CString CDHIOCardForPIO24CInterior::GetLastError()
{
	return m_sLastError;
}

bool CDHIOCardForPIO24CInterior::GetInputIndex(int iInputport,int& iValue)
{
	auto itera_find = m_map_SysInport2CardInport.find(iInputport);
	if (m_map_SysInport2CardInport.end() != itera_find)
	{
		if (m_bInitSuccess)
		{
			int iCardInportId = itera_find->second;
			int iPECntInRegister = m_IOCard.readCounter(iCardInportId);
			int iLargeCircle = m_arr_iInportCnt[iCardInportId] / 256;
			if (iPECntInRegister < 50 && m_arr_iInportCnt[iCardInportId] % 256 >200)
			{
				++iLargeCircle;
			}
			m_arr_iInportCnt[iCardInportId] = iLargeCircle * 256 + iPECntInRegister;
			iValue = m_arr_iInportCnt[iCardInportId];
		}
		else
		{
			iValue = 0;
		}
		return true;
	}
	return false;
}

bool CDHIOCardForPIO24CInterior::GetGrabImgSN(int iGrabSN, int& iImageSN)
{
	auto itera_find = m_map_GrabSN2QueueRegisterID.find(iGrabSN);
	if (m_map_GrabSN2QueueRegisterID.end() != itera_find)
	{
		if(m_bInitSuccess)
		{
			int iRegisterId = itera_find->second;
			int iCntInRegister = m_IOCard.readCounter(iRegisterId);
			int iLargeCicle = m_arr_iQueueRegisterCnt[iRegisterId] / 256;
			if (iCntInRegister < 50 && m_arr_iQueueRegisterCnt[iRegisterId] % 256 > 200)
			{
				++iLargeCicle;
			}
			m_arr_iQueueRegisterCnt[iRegisterId] = iLargeCicle * 256 + iCntInRegister;
			iImageSN = m_arr_iQueueRegisterCnt[iRegisterId];
		}
		else
		{
			iImageSN = 0;
		}
		return true;
	}
	return false;
}

bool CDHIOCardForPIO24CInterior::GetKickCountInfo(int iKickSN, s_KickCountInfo & sKickCntInfo)
{
	auto itera_find = m_map_SysKickSN2KickCntInfo.find(iKickSN);
	if (m_map_SysKickSN2KickCntInfo.end() != itera_find &&m_bInitSuccess)
	{
		s_KickCountInfo& rKickCntInfoTemp = itera_find->second;
		for (int i = 0; i < rKickCntInfoTemp.iKickPortNum; ++i)
		{
			int iPhysicalKickOutport = rKickCntInfoTemp.arr_iKickPortId[i];
			int iCntInRegister = m_IOCard.readCounter(iPhysicalKickOutport + 4);//**[12/19/2017 ConanSteve]**:  C卡的out0从4开始计数
			int iLargeCircle = m_arr_iOutportCnt[iPhysicalKickOutport] / 256;
			if (iCntInRegister < 50 && m_arr_iOutportCnt[iPhysicalKickOutport] % 256>200)
			{
				++iLargeCircle;
			}
			m_arr_iOutportCnt[iPhysicalKickOutport] = iLargeCircle * 256 + iCntInRegister;
			rKickCntInfoTemp.arr_iKickPortCount[i] = m_arr_iOutportCnt[iPhysicalKickOutport];
		}
		sKickCntInfo = rKickCntInfoTemp;
		return true;

	}
	return false;
}

bool CDHIOCardForPIO24CInterior::SetOutIOStatus(int iOutport, bool bStatus)
{
	auto itera_find = m_map_SysOutport2CardOutport.find(iOutport);
	if (m_map_SysOutport2CardOutport.end() != itera_find&&m_bInitSuccess)
	{
		int iPhysicalOutprotId = itera_find->second;
		m_IOCard.setCardOutput(iPhysicalOutprotId + 4, bStatus);
		return true;
	}
	return false;
}

bool CDHIOCardForPIO24CInterior::GetOutputCount(int iOutport, int&iValue)
{
	auto itera_find = m_map_SysOutport2CardOutport.find(iOutport);
	if (m_map_SysOutport2CardOutport.end()!= itera_find&&m_bInitSuccess)
	{
		int iPhysicalOutprotId = itera_find->second;
		iValue = m_IOCard.readCounter(iPhysicalOutprotId + 4);
		return true;
	}
	return false;
}


bool CDHIOCardForPIO24CInterior::ClearData()
{
	fill(m_arr_iInportCnt.begin(), m_arr_iInportCnt.end(), 0);
	fill(m_arr_iOutportCnt.begin(), m_arr_iOutportCnt.end(), 0);
	fill(m_arr_iQueueRegisterCnt.begin(), m_arr_iQueueRegisterCnt.end(), 0);

	/*************************************<打开日志>************************************[12/28/2017 ConanSteve]*/
	CINICfg iniCfgAlg(CExFn::GetCurrentAppPath() + _T("\\Config\\AlgConfig.ini"));
	CString sPathOfFloder{ _T("") }, sPathFileOfKick{_T("")};
	m_bWriteLog = iniCfgAlg.GetBoolEx(_T("Other Parameters"), _T("bWriteLog"), false);
	sPathOfFloder = iniCfgAlg.GetStringEx(_T("DEMO Controls"), _T("VirtualDisk"), sPathOfFloder);

	sPathOfFloder = sPathOfFloder + _T(":\\LogFiles\\IOCardLogs");
	//CExFn::DeleteDirectory(sPathOfFloder);
	//Sleep(100);
	CExFn::CreateMultiDirectory(sPathOfFloder);
	for (int i = 0; i < 4; ++i)
	{
		sPathFileOfKick.Format(_T("%s\\卡%d_Kick%d.txt"), sPathOfFloder, m_iIOCardSN, i);
		m_arr_fileKick[i].Open(sPathFileOfKick, &m_bWriteLog);
	}
	/*************************************<>************************************[12/28/2017 ConanSteve]*/
	return true;
}
