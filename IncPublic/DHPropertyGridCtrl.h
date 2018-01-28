#if !defined(AFX_CUSTOMPROPERTIES_H__C3F44405_4572_4B2B_895B_E56C1D8CFEA0__INCLUDED_)
#define AFX_CUSTOMPROPERTIES_H__C3F44405_4572_4B2B_895B_E56C1D8CFEA0__INCLUDED_

#pragma once

#define DHPG_MSG_BTN_CLICKED		WM_USER+3001
#define DHPG_MSG_PROPERTY_CHANGED	WM_USER+3002

typedef struct _DHPROPMSGSTRUCT
{
	int nDllIndex;
	char chGroupName[256];
	int nID;
	char chValue[256];
}s_DHPROGMSGSTRUCT, *P_DHPROGMSGSTRUCT;

/*
s_DHPROGMSGSTRUCT2说明
该版本在发送属性变更的消息时，已将s_DHPROGMSGSTRUCT结构改为s_DHPROGMSGSTRUCT2
区别是加入了Group的序号
同时s_DHPROGMSGSTRUCT结构依旧可以使用
*/
typedef struct _DHPROPMSGSTRUCT2
{
	int nDllIndex;
	char chGroupName[256];
	int nID;
	char chValue[256];

	int nGroupIndex;
	CWnd* pCtrlWnd;
}s_DHPROGMSGSTRUCT2, *P_DHPROGMSGSTRUCT2;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	_EXPORTING
#ifdef _EXPORTING
#define CLASS_DECLSPEC    __declspec(dllexport)  //程序编译时使用
#else
#define CLASS_DECLSPEC    __declspec(dllimport)	 //动态库应用时使用
#endif
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CLASS_DECLSPEC DHPropGridData
{
public:
	DHPropGridData();
	~DHPropGridData();	

	DHPropGridData(CString*);	//1
	DHPropGridData(int*);		//2
	DHPropGridData(UINT*);		//3
	DHPropGridData(long*);		//4
	DHPropGridData(DWORD*);		//5
	DHPropGridData(float*);		//6
	DHPropGridData(double*);	//7
	DHPropGridData(BYTE*);		//8
	DHPropGridData(short*);		//9
	DHPropGridData(bool*);		//10
	DHPropGridData(LONGLONG*);	//11
	DHPropGridData(CTime*);		//12
	DHPropGridData(char*);		//13
	DHPropGridData(void*);		//99

	void ModifyData(CString);	
	void ModifyData(int);		
	void ModifyData(UINT);		
	void ModifyData(long);		
	void ModifyData(DWORD);		
	void ModifyData(float);		
	void ModifyData(double);	
	void ModifyData(BYTE);		
	void ModifyData(short);		
	void ModifyData(bool);		
	void ModifyData(LONGLONG);	
	void ModifyData(CTime);
	void ModifyData(char*);	
	void ModifyData(void*);		

	BOOL GetData(CString&);
	BOOL GetData(int&);
	BOOL GetData(UINT&);
	BOOL GetData(long&);
	BOOL GetData(DWORD&);
	BOOL GetData(float&);
	BOOL GetData(double&);
	BOOL GetData(BYTE&);
	BOOL GetData(short&);
	BOOL GetData(bool&);
	BOOL GetData(LONGLONG&);
	BOOL GetData(CTime&);
	BOOL GetData(char*);
	BOOL GetData(void*);

	int GetDataType();

protected:
	void* m_pData;
	int m_nDataType;
	void FreeData();
};

class CLASS_DECLSPEC CDHPropertyGridCtrl
{
public:
	CDHPropertyGridCtrl(void);
	~CDHPropertyGridCtrl(void);

	int InstallControl(CWnd* pParentDlg, const CRect rcPos, const char* chConfigMidName, const int nCameraIndex);
	/*
		pParentDlg:			父窗口指针
		rcPos:				位置
		strConfigMidName:	标示名称 - 配置文件名称则必须为"DHPropGridLayoutConfig %s%d.ini";
		nCameraIndex:		第几路 从1开始

		AppName后边加上VL 就变为ValueList 例如[定位VL]
		AppName不可以重复否则后边的将无法载入到控件中

		参数：		       CtrlDesc
		EditCtrl0/1/2	:  CtrlRangeMin CtrlRangeMax CtrlPrecision/CtrlTextLen  int/double/char
			int默认-2147483647到2147483647
			double默认-99999.0到99999.0 小数点位数精度为2 最大为5
			char默认最长255
		Button			:  CtrlValue
		CheckButton		:  
		SliderCtrl		:  CtrlRangeMin CtrlRangeMax
		ComboBox		:  CtrlValue(中间用 ?*%? 隔开)
	*/
	int SetColumnWidth(const int nVal);
	int SetRowHeight(const int nVal);
	int DisplayCtrl(const int nCmdShow);
	int SetFont(LOGFONT& lFont);

	//EnableCtrl的几种方法
	int EnableCtrl(int nDllIndex, char* chGroupName, int nCtrlIndex, BOOL bEnable = TRUE);
	int EnableCtrl(int nDllIndex, char* chGroupName, char* chCtrlName, BOOL bEnable = TRUE);
	int EnableCtrl(int nDllIndex, const char* chGroupName, const char* chCtrlName, BOOL bEnable = TRUE);
	int EnableCtrl(int nDllIndex, int nGroupIndex, int nCtrlIndex, BOOL bEnable = TRUE);
	/*
	nDllIndex设为-1，即都生效
	chGroupName设为NULL，即该DllIndex下都生效
	nCtrlIndex设为-1，即该Group下面的都生效 是配置文件中CtrlName后边的序号
	*/
	int MoveWindow(const CRect rect, BOOL bRepaint = 1);
	int ReadModel(const char* chModelName);	//暂时没用
	int WriteModel(const char* chModelName);//暂时没用


	//Update的几种方法
	int UpdatePara(const int nDllIndex, const char* chGroupName, const int nCtrlIndex, DHPropGridData& Data, BOOL bGetValue = TRUE);
	int UpdatePara(const int nDllIndex, const int nGroupIndex, const int nCtrlIndex, DHPropGridData& Data, BOOL bGetValue = TRUE);
	/*
	nCtrlIndex 是配置文件中CtrlName后边的序号
	nGroupIndex 可在配置文件中设置 如果没设置 则会从0开始顺序计算
	*/
	int UpdatePara(const int nDllIndex, const char* chGroupName, const int nCtrlIndex, int& nValue, BOOL bGetValue = TRUE);
	int UpdatePara(const int nDllIndex, const char* chGroupName, const int nCtrlIndex, double& dValue, BOOL bGetValue = TRUE);
	int UpdatePara(const int nDllIndex, const char* chGroupName, const int nCtrlIndex, char* chValue, BOOL bGetValue = TRUE);
	int UpdatePara(const int nDllIndex, const char* chGroupName, const int nCtrlIndex, float& fValue, BOOL bGetValue = TRUE);

	int Do(int, char*, void*, void*, void*);
	/*
	int = 1: 警报控件 三个void*依次为 char* Color, int* nDllIndex, int* nGroupIndex, int* nCtrlIndex
		  2: 解除警报 三个void*依次为 int* nDllIndex, int* nGroupIndex, int* nCtrlIndex
		  3: 展开收起 三个void*依次为 int* nDllIndex, int* nGroupIndex
		  4: 结束编辑！
		  5: 返回CurrentSelection
		  6: 获取ColumnWidth
	*/
protected:
	void* m_pPropGridCtrl;
};

#endif // !defined(AFX_CUSTOMPROPERTIES_H__C3F44405_4572_4B2B_895B_E56C1D8CFEA0__INCLUDED_)

