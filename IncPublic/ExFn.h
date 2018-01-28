#pragma once
#include "windows.h"
#include "afx.h"
#include "vector"
#include "array"
#include "memory"
#include "Markup.h"
#include "algorithm"
#include "numeric"
using namespace std;

#define MAX_BUF_LEN 1024

#ifndef tr(quote)
	#define tr(quote) _T(quote)
#endif

/*************************************<扩展功能类>*************************************/
class CExFn
{
public:
	CExFn();
	~CExFn();
public:
	//**[8/4/2017 ConanSteve]**:  接口
	static BOOL ShowInTaskbar(HWND hWnd, BOOL bShow);//**[8/4/2017 ConanSteve]**:  设置窗口的任务栏图标是否显示
	static BOOL SetWindowPosTop(HWND hwnd);//**[8/4/2017 ConanSteve]**:  设置窗口置顶
	static CString GetCurrentAppPath(); //**[8/7/2017 ConanSteve]**:  获取当前进程运行路径	
	//**[9/5/2017 ConanSteve]**:  查找当前路径下所有子目录下指定格式文件
	//**[1/5/2018 ConanSteve]**:	iType=1,只搜索文件，iType =2只搜索文件夹，iType=3,都搜索 
	static bool FindSpecificFiles(LPCTSTR lpszPathFolder, LPCTSTR lpszFilter, vector<CString>& vec_sPathFile, vector<CString>& vec_sFileName, bool bSearchSub = true, int iType = 3, CString* pError = nullptr);
	static BOOL DeleteDirectory(LPCTSTR lpszPath, LPCTSTR lpszType = _T("*.*"), bool bIncludeFolders = true);//**[8/7/2017 ConanSteve]**:  快速删除当前文件夹及所有子目录文件
	static bool CreateMultiDirectory(LPCTSTR lpszPath);//**[8/25/2017 ConanSteve]**:  创建多级目录文件夹
	static BOOL CopyChildFiles(LPCTSTR lpszExistingFileName, LPCTSTR lpszNewFileName, BOOL bFailIfExists, CString* psErrorDesc = NULL);//**[1/9/2018 ConanSteve]**:  拷贝整个文件夹中的所有文件
	//**[1/11/2018 ConanSteve]**:  https://www.cnblogs.com/huhu0013/p/4617767.html
	static BOOL IsDirectory(LPCTSTR pstrPath);
	static BOOL CopyFolder(LPCTSTR pstrSrcFolder, LPCTSTR pstrDstFolder);

	//**[1/25/2018 ConanSteve]**:	读取值 
	static bool GetPrivateProfileBool(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bDefaultValue, LPCTSTR lpszFileName, LPCTSTR lpszAnnotation = NULL);
	static int GetPrivateProfileInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iDefaultValue, LPCTSTR lpszFileName, LPCTSTR lpszAnnotation = NULL);
	static CString& GetPrivateProfileString(_In_opt_ LPCTSTR lpAppName, _In_opt_ LPCTSTR lpKeyName, CString &sReturnString, _In_opt_ LPCTSTR lpFileName, _In_opt_ LPCTSTR lpDefault = NULL, LPCTSTR lpszAnnotation = NULL);//**[9/1/2017 ConanSteve]**: 
	//**[1/25/2018 ConanSteve]**:	读取值，并写入值 
	static bool GetPrivateProfileBoolEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bDefaultValue , LPCTSTR lpszFileName, LPCTSTR lpszAnnotation = NULL);
	static int GetPrivateProfileIntEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iDefaultValue, LPCTSTR lpszFileName, LPCTSTR lpszAnnotation = NULL);
	static CString& GetPrivateProfileStringEx(_In_opt_ LPCTSTR lpAppName,_In_opt_ LPCTSTR lpKeyName,CString &sReturnString,_In_opt_ LPCTSTR lpFileName,_In_opt_ LPCTSTR lpDefault=NULL, LPCTSTR lpszAnnotation = NULL);//**[9/1/2017 ConanSteve]**: 
	
	static bool WritePrivateProfileBool(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bKeyValue, LPCTSTR lpszFileName, LPCTSTR lpszAnnotation = NULL);
	static bool WritePrivateProfileInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iKeyValue, LPCTSTR lpszFileName,LPCTSTR lpszAnnotation=NULL);
	static bool WritePrivateProfileString(LPCTSTR lpszAppName, LPCTSTR lpKeyName, LPCTSTR lpszValue, LPCTSTR lpszFileName, LPCTSTR lpszAnnotation = NULL);
	static CString Int2CS(int iVal);
	static CString GetLastErrorMsg();//**[9/13/2017 ConanSteve]**:  获得GetLastError返回的错误码对应的字符串信息

	//**[1/25/2018 ConanSteve]**:	根据宽高比例，调整矩形大小和位置 
	static CRect AutoResizeRect(int iWidth, int iHeight, CRect rect);
	//**[1/25/2018 ConanSteve]**:	 计算子窗口的大小位置
	static vector<CRect> AutoLayoutRect(CRect rectParent, int iCntHorizon, int iCntVertical, int iGapHorizon,int iGapVertical, int iChildNum, int iType=1, CRect* pRect = NULL);
};
/*************************************<CStringConvert字符转换类>************************************[9/14/2017 ConanSteve]*/
enum e_EncodeFormat {
	e_EncodeFormat_Error = 0,
	e_EncodeFormat_ANSI_GB2312 = 1,
	e_EncodeFormat_UTF8 = 2,
	e_EncodeFormat_UTF8_BOM,
	e_EncodeFormat_UCS2_BE_BOM,
	e_EncodeFormat_UCS2_LE_BOM
};

class CStringConvert {
private:
	CStringConvert() {

	}
	~CStringConvert() {

	}
public:
	//**[7/21/2017 ConanSteve]**:  ANSI存储转Unicode
private:
	static wstring MBCS_UCS2(string& str, int iCodePage);
public:
	static wstring ANSI_UCS2(string& str);
	static wstring ANSI_UCS2(const char* ch);
	static wstring UTF8_UCS2(string& str);//**[7/21/2017 ConanSteve]**:  UTF8存储转Unicode
	static wstring UTF8_UCS2(char* ch);
	static wstring GB2312_UCS2(string& str);

private:
	static string UCS2_MBCS(wstring& wstr, int codePage);
public:
	static string UCS2_ANSI(wstring& wstr);
	static string UCS2_ANSI(wchar_t* wch);
	static string UCS2_UTF8(wstring& wstr);
	static string UCS2_UTF8(wchar_t* wch);

public:
	static string ANSI_UTF8(string &str);
	static string ANSI_UTF8(char* ch);
	static string UTF8_ANSI(string &str);
	static string UTF8_ANSI(char* ch);
	//	static string Int2UTF8(int nVal);
	//	static string WCHAR_UTF8(wchar_t* pwchData, int nLen = -1);


public:
	static bool IsTextUTF8(const char* str, long length);
	static e_EncodeFormat GetEncodeFormat(wstring& wsFilePathName);
	static e_EncodeFormat GetEncodeFormat(wchar_t* pwszFilePathName);
	static e_EncodeFormat GetEncodeFormat(string& sFilePathName);
	static e_EncodeFormat GetEncodeFormat(char* pszFilePathName);
	//static string WCHAR_ANSI(CString csPara);
	//static string CS2UTF8(CString csData);
	//static string Double2UTF8(double dbVal);
	//static int CS2Int(CString csPara);
	//static double CS2Double(CString csPara);
};
//**[8/3/2017 ConanSteve]**:  GBK的code page是CP936 ，BIG5的code page是CP950，GB2312的code page是CP20936
/*************************************<读写XML配置文件类>************************************[9/14/2017 ConanSteve]*/
enum  VarType {
	__no,
	__bool,
	__int,
	__float,
	__DWORD,
	__CString,
};
typedef struct NodeVar
{
	VarType varType;//**[9/14/2017 ConanSteve]**:  变量类型
	CString sAppName;
	CString sKeyName;
	union {
		bool *pbBool;
		int *piInt;
		float* pfFloat;

		CString* psCString;
	}ptr;
	CString sAnnotation;
	NodeVar()
	{
		varType=VarType::__no;//**[9/14/2017 ConanSteve]**:  变量类型
		sAppName= _T("") ;
		sKeyName= _T("") ;
		sAnnotation =_T("") ;
		memset(&ptr, 0, sizeof(ptr));
	}
}NodeVar;
class CXMLCfg
{
public:
	CXMLCfg(LPCTSTR lpszFilePath= NULL);
	~CXMLCfg();
	bool Load(LPCTSTR lpszFilePath);
	bool Init(LPCTSTR lpszFilePath);//**[9/14/2017 ConanSteve]**:  初始化配置文件路径
	bool UpdateData();//**[9/14/2017 ConanSteve]**:  将所有数据写进xml文件
	CString GetStringEx(_In_opt_ LPCTSTR lpszAppName, _In_opt_ LPCTSTR lpszKeyName, LPCTSTR lpszDefaultValue = NULL, LPCTSTR lpszAnnotation = NULL);//**[9/1/2017 ConanSteve]**:
	//CString& GetStringEx(_In_opt_ LPCTSTR lpszAppName, _In_opt_ LPCTSTR lpszKeyName, CString &sRetKeyValue, LPCTSTR lpszDefaultValue=NULL, LPCTSTR lpszAnnotation = NULL);//**[9/1/2017 ConanSteve]**:
	CString& GetStringEx(_In_opt_ LPCTSTR lpszAppName, _In_opt_ LPCTSTR lpszKeyName, CString &sRetKeyValue, LPCTSTR lpszDefaultValue = NULL, LPCTSTR lpszAnnotation = NULL);
	

	bool WriteBool(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bKeyValue, LPCTSTR lpszAnnotation = NULL);
	bool WriteString(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszValue,LPCTSTR lpszAnnotation = NULL);
	int GetInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iDefaultValue = 0, LPCTSTR lpszAnnotation = NULL);
	bool WriteInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iKeyValue, LPCTSTR lpszAnnotation = NULL);
	bool GetBool(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bDefaultValue = false, LPCTSTR lpszAnnotation = NULL);


	bool GetBoolEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool& bRetKeyValue, bool bDefaultValue = false, LPCTSTR lpszAnnotation = NULL);
	int& GetIntEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int& iKeyValue, int iDefaultValue, LPCTSTR lpszAnnotation = NULL);//**[9/14/2017 ConanSteve]**:  绑定变量，当调用UpdateDate函数时，会自动写入本次调用绑定的变量
	CString GetStringEx(vector<CString>& vecNodePath, LPCTSTR lpszKeyName, LPCTSTR lpszDefaultValue=NULL);
	CString& GetStringEx(vector<CString>& vecNodePath, LPCTSTR lpszKeyName, CString& sRetKeyValue, LPCTSTR lpszDefaultValue = NULL);
	
	//**[9/14/2017 ConanSteve]**:  写额外属性用，注释也是一种属性
	CString& GetAttrString(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszAttrName, CString &sAttrValue);
	CString& GetAttrStringEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszAttrName, CString &sAttrValue);
	bool WriteAttrString(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszAttrName, LPCTSTR lpszAttrValue);
	int& GetAttrInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszAttrName, int& iAttrValue);
	int& GetAttrStringEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszAttrName, int& iAttrValue);
	bool WriteAttrInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszAttrName, int iAttrValue);


protected:
	bool m_bOpenFile;
	CMarkup m_xml;
	CString m_sPathFile;
	vector<NodeVar> m_vecNodeVar;//**[9/14/2017 ConanSteve]**:  绑定的变量，用于析构时写入

};
/*************************************<CINICfg>************************************[9/14/2017 ConanSteve]*/
class CINICfg
{
public:
	//**[1/25/2018 ConanSteve]**:	初始化和构造函数 
	CINICfg(LPCTSTR lpszFilePath = NULL);
	~CINICfg();
	bool Load(LPCTSTR lpszFilePath = NULL);
	bool Init(LPCTSTR lpszFilePath = NULL);
	bool UpdateData();

	bool GetBool(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bDefaultValue = false, LPCTSTR lpszAnnotation = NULL);
	int GetInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iDefaultValue = -1, LPCTSTR lpszAnnotation = NULL);
	double GetDouble(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, double fDefaultValue = 0, LPCTSTR lpszAnnotation = NULL);
	CString& GetString(_In_opt_ LPCTSTR lpszAppName, _In_opt_ LPCTSTR lpszKeyName, CString &sReturnString, _In_opt_ LPCTSTR lpszDefault = NULL, LPCTSTR lpszAnnotation = NULL);//**[9/1/2017 ConanSteve]**: 


	//**[1/25/2018 ConanSteve]**:	写值 
	bool WriteBool(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bKeyValue, LPCTSTR lpszAnnotation = NULL);
	bool WriteInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iKeyValue, LPCTSTR lpszAnnotation = NULL);
	bool WriteDouble(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, double lfValue, LPCTSTR lpszAnnotation = NULL);
	bool WriteString(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszAnnotation = NULL);


	//**[1/25/2018 ConanSteve]**:	读取值，并写入默认值

	bool GetBoolEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bDefaultValue = false, LPCTSTR lpszAnnotation = NULL);
	int GetIntEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iDefaultValue = -1, LPCTSTR lpszAnnotation = NULL);
	double GetDoubleEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, double fDefaultValue = 0, LPCTSTR lpszAnnotation = NULL);
	CString& GetStringEx(_In_opt_ LPCTSTR lpszAppName, _In_opt_ LPCTSTR lpszKeyName, CString &sReturnString, _In_opt_ LPCTSTR lpszDefault = NULL, LPCTSTR lpszAnnotation = NULL);//**[9/1/2017 ConanSteve]**: 

	//**[9/14/2017 ConanSteve]**:  绑定变量，当类析构时，会自动写入本次调用绑定的变量
	int& GetIntEx2(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int& iKeyValue, int iDefaultValue = -1, LPCTSTR lpszAnnotation = NULL);
	bool GetBoolEx2(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool& bRetKeyValue, bool bDefaultValue = false, LPCTSTR lpszAnnotation = NULL);
	CString& GetStringEx2(_In_opt_ LPCTSTR lpszAppName, _In_opt_ LPCTSTR lpszKeyName, CString &sReturnString, _In_opt_ LPCTSTR lpszDefault = NULL, LPCTSTR lpszAnnotation = NULL);

protected:
	bool m_bOpenFile;
	CString m_sPathFile;
	vector<NodeVar> m_vecNodeVar;//**[9/14/2017 ConanSteve]**:  绑定的变量，用于析构时写入
};

/*************************************<日志记录类>*************************************/
class CLogFile {
public:
	CLogFile(LPCTSTR lpFilePath = NULL);
	~CLogFile();
	//**[9/6/2017 ConanSteve]**:  bWrite:是否打开日志记录
	bool Open(LPCTSTR lpFilePath,bool* pbWrite = nullptr, int iFlag = CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyNone);
	bool Open(LPCTSTR lpFilePath, int iFlag);
	bool Close();
	bool Write(LPCTSTR lpLogInfo);
	//bool Write(const TCHAR* fmt, ...);
	void Flush();
	bool SetMaxLine(int iMaxLine);
	bool SeekToBegin();
private:
	bool m_bOpenFile;
	bool* m_pbWrite;
	int m_iCurLine;//**[8/30/2017 ConanSteve]**:  当前记录行数
	int m_iMaxLine;//**[8/30/2017 ConanSteve]**:  最大可记录行数
	unsigned __int64 m_iLastTick;//**[9/5/2017 ConanSteve]**:  上一次时钟计数
	unsigned __int64 m_iFrequency;//**[9/5/2017 ConanSteve]**:  计算机CPU频率
	//CRITICAL_SECTION m_csWrite;
	CFile m_file;
};

/*************************************<CPU使用率类>*************************************/
class CCPUUsage
{
private:
	typedef long long          int64_t;
	typedef unsigned long long uint64_t;
	HANDLE _hProcess;
	int _processor;    //cpu数量    
	int64_t _last_time;         //上一次的时间    
	int64_t _last_system_time;
	    
	uint64_t file_time_2_utc(const FILETIME* ftime);// 时间转换	 
	int get_processor_number();// 获得CPU的核数   	  
	void init(); //初始化 
	void clear();//关闭进程句柄


public:
	CCPUUsage(DWORD ProcessID);
	CCPUUsage();
	~CCPUUsage();
	//返回值为进程句柄，可判断OpenProcess是否成功  
	HANDLE setpid(DWORD ProcessID);
	//-1 即为失败或进程已退出； 如果成功，首次调用会返回-2（中途用setpid更改了PID后首次调用也会返回-2）  
	float get_cpu_usage();
};

class CLayout 
{
public:
	CLayout() {}
	CLayout(LPCTSTR lpszInitFilePath, CRect& rectParent) { Init(lpszInitFilePath, rectParent); }
	~CLayout();
	bool Init(LPCTSTR lpszInitFilePath, CRect& rectParent);//**[10/10/2017 ConanSteve]**:  lpszInitFilePath：XML配置文件路径  rectParent:父窗口的位置信息
	bool GetPosition(LPCTSTR lpszWndName, CRect& rectWnd);//**[10/10/2017 ConanSteve]**:  获得当前窗口的布局位置
	CString GetAttrib(LPCTSTR lpszWndName, LPCTSTR lpszAttrName, CString& sRetVal);
	int GetAttrib(LPCTSTR lpszWndName, LPCTSTR lpszAttrName, int iDefaultVal = 0);
private:
	bool FindFixedWnd(LPCTSTR lpszWndName, CRect& rectWnd);//**[10/10/2017 ConanSteve]**:  寻找固定窗口的位置信息
	bool FindDynamicWnd(LPCTSTR lpszWndName, CRect&	rectWnd);//**[10/10/2017 ConanSteve]**:  寻找动态窗口的位置信息
	bool ParseCurNode(LPCTSTR lpszWndName,CRect rectParent, CRect& rectWnd);
	array<int, 4>& ParsePadding(LPCTSTR lpszPadding, array<int, 4>& arrPadding);
	bool m_bInitSuccess;
	CRect m_rectParent;//**[10/10/2017 ConanSteve]**:  父窗口的位置信息
	CString m_sInitFilePath;
	CMarkup m_xml;

};

