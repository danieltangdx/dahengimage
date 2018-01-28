#include "stdafx.h"
#include "ThreeTierRightsInterior.h"
#include "DlgLogin.h"
#include "DlgUserManage.h"
#include "ExFn.h"


CThreeTierRightsInterior::CThreeTierRightsInterior()
{
	m_bEnable = false;
	ReadConfigFile();
}

CThreeTierRightsInterior::CThreeTierRightsInterior(CWnd * pParent)
{
	m_pParent = pParent;
	m_bEnable = false;
	ReadConfigFile();
}


CThreeTierRightsInterior::~CThreeTierRightsInterior()
{
	SaveConfigFile();
}

bool CThreeTierRightsInterior::Login(CWnd * pParent, bool bEnbaleFromDisk, bool bHint)
{
	m_pParent = pParent;
	bool bRet = false;
	ReadConfigFile();
	m_xml.ResetPos();
	bRet = m_xml.FindElem(_T("ThreeTierRights"));
	if (!bRet)
	{
		m_xml.AddElem(_T("ThreeTierRights"));
	}
	m_xml.IntoElem();
	bRet = m_xml.FindElem(_T("UserInfo"));
	if(bEnbaleFromDisk)//**[1/26/2018 ConanSteve]**:	如果调用方式是从配置文件读取是否验证三级权限 
	{
		CString sEnable = m_xml.GetAttrib(_T("enable"));
		if (_T("") == sEnable)
		{
			bRet = m_xml.AddAttrib(_T("enable"), _T("true"));
			bRet = m_xml.Save(m_sPathFileOfCfg);
		}
		if (_T("false") == sEnable)
		{
			m_bEnable = true;
			m_sCurUsername = _T("daheng");
			m_iCurUserType = e_UserType_SuperAdmin;
			return true;
		}
	}
	m_sCurUsername = m_xml.GetAttrib(_T("lastLoginUserName"));//**[1/2/2018 ConanSteve]**:	读取上一次用户名 
	m_bEnable = true;
	bool bHasLastLoginUserNameTag = _T("") != m_sCurUsername;
	CDlgLogin dlgLogin(m_pParent);
	dlgLogin.SetCallPointer((void*)this);
	//((CWnd*)m_pParent)->ShowWindow(SW_HIDE);
	CExFn::SetWindowPosTop(dlgLogin.m_hWnd);
	if (IDOK == dlgLogin.DoModal())
	{
		m_sCurUsername = dlgLogin.m_sUsername;
		if (bHint)
		{
			CString sInfo{ _T("") };
			sInfo.Format(_T("%s用户%s登录成功，"), ParseUserType(m_iCurUserType), m_sCurUsername);
			//MessageBox(m_pParent->m_hWnd, tr("警告"), sInfo, MB_OK | MB_ICONWARNING);
			AfxMessageBox(sInfo);
		}
		if (bHasLastLoginUserNameTag)
		{
			if (_T("cs") != m_sCurUsername)
				m_xml.SetAttrib(_T("lastLoginUserName"), m_sCurUsername);
		}
		else
		{
			if (_T("cs")!=m_sCurUsername)
				m_xml.AddAttrib(_T("lastLoginUserName"), m_sCurUsername);
		}
		//((CWnd*)m_pParent)->ShowWindow(SW_NORMAL);
		SaveConfigFile();
		return true;
	}
	return false;
}



bool CThreeTierRightsInterior::VerifyRights(e_UserType iUserType, bool bHint)
{
	if(!m_bEnable)
		return true;
	if (iUserType > m_iCurUserType)
	{
		if(bHint&&NULL != m_pParent)
			::MessageBox(m_pParent->m_hWnd, tr("当前用户不具有权限，请切换用户！"), tr("警告"), MB_OK | MB_ICONWARNING);
		return false;
	}
	return true;
}

bool CThreeTierRightsInterior::VerifyRights(CWnd* pWnd, e_RightType iRightType, bool bHint)
{
	switch (iRightType)
	{
	case e_RightType_ThreeTierVerification:
	{
		auto itera_find = m_map_PWnd2UserType.find(pWnd);
		if (m_map_PWnd2UserType.end() == itera_find)
		{
			if (bHint)
				::MessageBox(m_pParent->m_hWnd, tr("当前用户不具有权限，请切换用户！"), tr("警告"), MB_OK | MB_ICONWARNING);
			return false;
		}
		return VerifyRights(itera_find->second.iUserType, bHint);
	}
		break;
	case e_RightType_EnableShow:
	{
		auto itera_find = map_PWnd2EnableShow.find(pWnd);
		if (map_PWnd2EnableShow.end() == itera_find)
		{
			return false;
		}
		return itera_find->second;
	}
		break;
	default:
		break;
	}
}

void CThreeTierRightsInterior::ManageUsers()
{
	if (!m_bEnable)
		return;
	if (e_UserType_Admin > m_iCurUserType)
	{
		if (NULL != m_pParent)
		{
			::MessageBox(m_pParent->m_hWnd, tr("当前用户不具有权限，请切换用户！"), tr("警告"), MB_OK | MB_ICONWARNING);
		}
		return;
	}
	CDlgUserManage dlg(m_pParent);
	dlg.m_pThreeTierRightsInterior = this;
	if (IDOK == dlg.DoModal())
	{
		m_xml.Save(m_sPathFileOfCfg);
	}
}

bool CThreeTierRightsInterior::GetCurUserInfo(s_UserInfoOfThreeTierRights & rUserInfo)
{
	memcpy_s(rUserInfo.lpszUserName, sizeof(rUserInfo.lpszUserName), m_sCurUsername.GetBuffer(), m_sCurUsername.GetLength() * sizeof(TCHAR));
	memcpy_s(rUserInfo.lpszUserType, sizeof(rUserInfo.lpszUserType), ParseUserType(m_iCurUserType).GetBuffer(), ParseUserType(m_iCurUserType).GetLength() * sizeof(TCHAR));
	return true;
}

bool CThreeTierRightsInterior::AddCtrl(CWnd* pWnd, e_RightType iRightType)
{
	//memcpy_s(ctrlInfo.lpszText, MAX_PATH, lpszText, MAX_PATH);
	/*************************************<先判断此控件是否已经在列表中了>************************************[1/16/2018 ConanSteve]*/
	bool bRet = false;
	m_xml.ResetPos();
	bRet = m_xml.FindElem(_T("ThreeTierRights"));
	if (!bRet)
	{
		m_xml.AddElem(_T("ThreeTierRights"));
	}
	m_xml.IntoElem();
	switch (iRightType)
	{
	case e_RightType_ThreeTierVerification:
	{
		auto itera_find = m_map_PWnd2UserType.find(pWnd);
		if (m_map_PWnd2UserType.end() != itera_find)
			return false;
		bRet = m_xml.FindElem(_T("RightsControlInfo"));
		if (!bRet)
		{
			m_xml.AddElem(_T("RightsControlInfo"));
		}
		/*************************************<将文件中所有用户名添加进用户列表中>************************************[11/14/2017 ConanSteve]*/
		static int  iIndex = 0;
		e_UserType iCtrlRightType = e_UserType_Operator;
		CString sCtrlName{ _T("") }, sCtrlText{ _T("") };
		TCHAR lpszText[MAX_PATH];
		s_CtrlInfo ctrlInfo;
		pWnd->GetWindowText(lpszText, MAX_PATH);
		sCtrlName.Format(_T("Ctrl_%d"), iIndex);
		bRet = m_xml.FindChildElem(sCtrlName);
		if (!bRet)//**[1/15/2018 ConanSteve]**:  不存在，新加的控件
		{
			m_xml.AddChildElem(sCtrlName);
			m_xml.AddChildAttrib(_T("CtrlText"), lpszText);
			m_xml.AddChildAttrib(_T("CtrlRightType"), e_UserType_Operator);
		}
		else
		{
			m_xml.SetChildAttrib(_T("CtrlText"), lpszText);
			iCtrlRightType = (e_UserType)_ttoi(m_xml.GetChildAttrib(_T("CtrlRightType")));
		}
		++iIndex;
		memcpy_s(ctrlInfo.lpszText, sizeof(ctrlInfo.lpszText), lpszText, sizeof(lpszText));
		ctrlInfo.iUserType = iCtrlRightType;
		m_map_PWnd2UserType.insert(make_pair(pWnd, ctrlInfo));
	}
		break;
	case e_RightType_EnableShow:
	{
		auto itera_find = map_PWnd2EnableShow.find(pWnd);
		if (map_PWnd2EnableShow.end() != itera_find)
			return false;
		bRet = m_xml.FindElem(_T("EnableShowControlInfo"));
		if (!bRet)
		{
			m_xml.AddElem(_T("EnableShowControlInfo"));
		}
		/*************************************<将文件中所有用户名添加进用户列表中>************************************[11/14/2017 ConanSteve]*/
		static int  iIndex = 0;
		CString sCtrlName{ _T("") }, sCtrlText{ _T("") };
		TCHAR lpszText[MAX_PATH];
		bool bEnableShow{ true };
		s_CtrlInfo ctrlInfo;
		pWnd->GetWindowText(lpszText, MAX_PATH);
		sCtrlName.Format(_T("Ctrl_%d"), iIndex);
		bRet = m_xml.FindChildElem(sCtrlName);
		if (!bRet)//**[1/15/2018 ConanSteve]**:  不存在，新加的控件
		{
			m_xml.AddChildElem(sCtrlName);
			m_xml.AddChildAttrib(_T("CtrlText"), lpszText);
			m_xml.AddChildAttrib(_T("enableShow"), _T("true"));
		}
		else
		{
			m_xml.SetChildAttrib(_T("CtrlText"), lpszText);
			bEnableShow = 0 == m_xml.GetChildAttrib(_T("CtrlRightType")).CompareNoCase(_T("true"));
		}
		++iIndex;
		map_PWnd2EnableShow.insert(make_pair(pWnd, bEnableShow));
	}
		break;
	default:
		break;
	}

	SaveConfigFile();
	return true;
}

bool CThreeTierRightsInterior::ReadConfigFile()
{
	bool bEnableUser{ true };
	CString sUserName{ _T("") };
	CString sPassword{ _T("") };
	e_UserType eUserType{ e_UserType_Illegal };
	CUserInfo UserInfo;
	bool bRet{ false };
	m_mapUserInfo.clear();
	m_mapUserInfo.insert(make_pair(_T("daheng"), CUserInfo(_T("daheng"), _T("daheng"), e_UserType_Admin)));//**[11/14/2017 ConanSteve]**:  添加超级管理员
	m_mapUserInfo.insert(make_pair(_T("Admin"), CUserInfo(_T("Admin"), _T("daheng"), e_UserType_SuperAdmin)));
	m_mapUserInfo.insert(make_pair(_T("cs"), CUserInfo(_T("cs"), _T("cs"), e_UserType_SystemDeveloper)));
	if (_T("") == m_sPathFileOfCfg||FALSE == PathFileExists(m_sPathFileOfCfg))
	{
		m_sPathFileOfCfg = CExFn::GetCurrentAppPath() + _T("\\Config\\ThreeTierRights.xml");
	}
	bRet = m_xml.Load(m_sPathFileOfCfg);

	//**[11/14/2017 ConanSteve]**:  找主目录
	m_xml.ResetPos();
	bRet = m_xml.FindElem(_T("ThreeTierRights"));
	if (!bRet)
	{
		m_xml.AddElem(_T("ThreeTierRights"));
	}
	m_xml.IntoElem();
	bRet = m_xml.FindElem(_T("UserInfo"));
	if (!bRet)
	{
		m_xml.AddElem(_T("UserInfo"));
	}
	/*************************************<将文件中所有用户名添加进用户列表中>************************************[11/14/2017 ConanSteve]*/
	while (m_xml.FindChildElem())
	{
		//sUserName = m_xml.GetChildTagName();
		sUserName = m_xml.GetChildAttrib(_T("username"));
		sPassword = m_xml.GetChildAttrib(_T("password"));
		eUserType = (e_UserType)_ttoi(m_xml.GetChildAttrib(_T("userType")).GetBuffer());
		bEnableUser = 0 == m_xml.GetChildAttrib(_T("enable")).CompareNoCase(_T("true"));
		if (eUserType > e_UserType_Admin)
			eUserType = e_UserType_Admin;
		if(m_mapUserInfo.end() == m_mapUserInfo.find(sUserName))
			m_mapUserInfo.insert(make_pair(sUserName, CUserInfo(sUserName, sPassword, eUserType, bEnableUser)));
	}
	return true;

}

void CThreeTierRightsInterior::SaveConfigFile()
{
// 	bool bRet{ false };
// 	m_xml.ResetPos();
// 	bRet = m_xml.FindElem(_T("UserInfo"));
// 	if (!bRet)
// 	{
// 		m_xml.AddElem(_T("UserInfo"));
// 	}
// 	m_xml.IntoElem();
// 	for (auto itera = m_mapUserInfo.begin(); itera != m_mapUserInfo.end(); ++itera)
// 	{
// 		
// 		m_xml.AddElem(itera->first);
// 		m_xml.AddAttrib(_T("password"), itera->second.sPassword);
// 		m_xml.AddAttrib(_T("userType"), itera->second.eUserType);
// 	}
	m_xml.Save(m_sPathFileOfCfg);
}

CString CThreeTierRightsInterior::ParseUserType(int iUserType)
{
	switch (iUserType)
	{
	case e_UserType_Illegal:
		return tr("非法用户");
	case e_UserType_Operator:
		return tr("操作员");
	case e_UserType_Debugger:
		return tr("调试员");
	case e_UserType_Admin:
		return tr("管理员");
	case e_UserType_SuperAdmin:
		return tr("超级管理员");
	case e_UserType_SystemDeveloper:
		return tr("系统研发工程师");
	default:
		return CString();
	}
}

e_UserType CThreeTierRightsInterior::ParseUserType(CString sUserType)
{
	if (sUserType == tr("操作员"))
		return e_UserType_Operator;
	if (sUserType == tr("调试员"))
		return e_UserType_Debugger;
	if (sUserType == tr("管理员"))
		return e_UserType_Admin;
	if (sUserType == tr("超级管理员"))
		return e_UserType_SuperAdmin;
	if (sUserType == tr("系统研发工程师"))
		return e_UserType_SystemDeveloper;
	return e_UserType_Illegal;
}
