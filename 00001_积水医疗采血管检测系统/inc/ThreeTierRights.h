#pragma once

#ifdef THREETIERRIGHTS_EXPORTS
#define THREETIERRIGHTS_EXPORTS __declspec(dllexport)
#else
#define THREETIERRIGHTS_EXPORTS __declspec(dllimport)
#endif

enum e_UserType {
	e_UserType_Illegal,
	e_UserType_Operator,
	e_UserType_Debugger,
	e_UserType_Admin,
	e_UserType_SuperAdmin//**[1/15/2018 ConanSteve]**:  开发人员权限，
};
enum e_DataStoreType {
	e_DataStoreType_Xml,//**[1/15/2018 ConanSteve]**:  用xml文件方式，需要添加文件加密
	e_DataStoreType_Mysql//**[1/15/2018 ConanSteve]**:  用数据库存储用户信息
};

typedef struct s_DatabaseInfo {
	TCHAR host[MAX_PATH];
	TCHAR database[MAX_PATH];
	TCHAR password[MAX_PATH];
	TCHAR tableName[MAX_PATH];
}s_DatabaseInfo;

class THREETIERRIGHTS_EXPORTS CThreeTierRights
{
public:
	CThreeTierRights(CWnd* pParent=NULL);
	~CThreeTierRights();
public:
	bool Init(e_DataStoreType  dataStoreType= e_DataStoreType_Xml, s_DatabaseInfo* pDatabaseInfo = NULL);//**[1/15/2018 ConanSteve]**:  初始化方式，默认
	bool Login(CWnd* pParent = NULL, bool bHint = false);//**[1/15/2018 ConanSteve]**:  用户登录和切换用户使用
	bool VerifyRights(e_UserType iUserType, bool bHint = false);//**[1/15/2018 ConanSteve]**:  验证权限
	bool VerifyRights(CWnd* pWnd, bool bHint = false);//**[1/15/2018 ConanSteve]**:  获取指定控件的权限
	void ManageUsers();//**[1/15/2018 ConanSteve]**:  管理用户
	bool AddCtrl(CWnd* pWnd);//**[1/15/2018 ConanSteve]**:  添加控件ID到3级权限管理库中

private:
	void* pInteroir;
};