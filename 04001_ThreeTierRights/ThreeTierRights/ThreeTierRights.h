#pragma once

#ifdef THREETIERRIGHTS_EXPORTS
#define THREETIERRIGHTS_EXPORTS __declspec(dllexport)
#else
#define THREETIERRIGHTS_EXPORTS __declspec(dllimport)
#endif

enum e_UserType {
	e_UserType_Illegal,
	/*************************************<用户权限>************************************[12/28/2017 ConanSteve]*/
	e_UserType_Operator,//**[1/26/2018 ConanSteve]**:	普通操作员权限 
	e_UserType_Debugger,//**[1/26/2018 ConanSteve]**:	调试人员权限 
	e_UserType_Admin, //**[1/26/2018 ConanSteve]**:	 管理员权限
	/*************************************<大恒技术人员权限>************************************[12/28/2017 ConanSteve]*/
	e_UserType_SuperAdmin,//**[1/15/2018 ConanSteve]**:  超级管理员权限，供技服，算法开发人员使用
	e_UserType_SystemDeveloper//**[1/26/2018 ConanSteve]**:	系统开发人员权限，最高级，可以使用一切功能 
};
enum e_DataStoreType {
	e_DataStoreType_Xml,//**[1/15/2018 ConanSteve]**:  用xml文件方式，需要添加文件加密
	e_DataStoreType_Mysql,//**[1/15/2018 ConanSteve]**:  用数据库存储用户信息
	e_DataStoreType_SQLite3
};

typedef struct s_DatabaseInfo {//**[1/27/2018 ConanSteve]**:	数据库连接信息 

	TCHAR host[MAX_PATH];
	TCHAR database[MAX_PATH];
	TCHAR password[MAX_PATH];
}s_DatabaseInfo;

typedef struct s_UserInfoOfThreeTierRights
{
	TCHAR lpszUserName[MAX_PATH];
	TCHAR lpszUserType[MAX_PATH];
	s_UserInfoOfThreeTierRights()
	{
		memset(lpszUserName, 0, sizeof(lpszUserName));
		memset(lpszUserType, 0, sizeof(lpszUserType));
	}
};

enum e_RightType
{
	e_RightType_ThreeTierVerification,//**[1/26/2018 ConanSteve]**:	需要三级权限验证
	e_RightType_EnableShow//**[1/26/2018 ConanSteve]**:	单纯的控制控件是否显示，需要超级管理员级以上权限才能修改 
};

class THREETIERRIGHTS_EXPORTS CThreeTierRights
{
public:
	CThreeTierRights(CWnd* pParent = NULL);
	~CThreeTierRights();
public:
	bool Init(e_DataStoreType  dataStoreType= e_DataStoreType_Xml, s_DatabaseInfo* pDatabaseInfo = NULL);//**[1/15/2018 ConanSteve]**:  初始化方式，默认
	bool Login(CWnd* pParent = NULL, bool bEnbaleFromDisk = false, bool bHint = false);//**[1/15/2018 ConanSteve]**:  用户登录和切换用户使用
	bool VerifyRights(e_UserType iUserType, bool bHint = false);//**[1/15/2018 ConanSteve]**:  验证权限
	bool VerifyRights(CWnd* pWnd, e_RightType iRightType = e_RightType_ThreeTierVerification, bool bHint = false);//**[1/15/2018 ConanSteve]**:  获取指定控件的权限
	void ManageUsers();//**[1/15/2018 ConanSteve]**:  管理用户
	bool GetCurUserInfo(s_UserInfoOfThreeTierRights& rUserInfo);
	bool AddCtrl(CWnd* pWnd, e_RightType iRightType= e_RightType_ThreeTierVerification);//**[1/15/2018 ConanSteve]**:  添加控件ID到3级权限管理库中

private:
	void* pInteroir;
};