#if !defined(AFX_CUSTOMPROPERTIES_H__C3F44405_4572_4B2B_895B_E56C1D8CFEA0__INCLUDED_)
#define AFX_CUSTOMPROPERTIES_H__C3F44405_4572_4B2B_895B_E56C1D8CFEA0__INCLUDED_

#pragma once

#define DHPG_MSG_BTN_CLICKED		WM_USER+3001
#define DHPG_MSG_PROPERTY_CHANGED	WM_USER+3002
//#define DHPG_MSG_PARA_CLICKED      WM_USER+3003

#define MAX_SUB_ALG_NUM 15
#define MAX_PARA_NUM 200   //共享内存最大的参数个数

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

//与列表进行数据更新的变量类型，常见的变量类型需先转换为此类型再进行数据更新
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
//***事件和内存名称
//共享内存名称：Format(ShareMemory_%d",m_nCamIndex)
//写内存完毕事件名称：Format("ShareEvent_%d",m_nCamIndex)
//共享内存大小:sizeof(SharedMemory)*MAX_PARA_NUM
//****//
struct SharedMemory//共享内存结构体
{   
    int nChangedNum;  //共享内存中此结构体有效个数在第一个结构体中
	int nCamIndex;//相机号
	int nParaIndex;//列表号
    char strDllName[32];//动态库名
	char strGroupName[32];//组名
	char strCtrlName[32];//控件名
	int nType;//变量类型 0 数字型  1字符型
	char strValueOld[256];//字符型 老变量值
	char strValueNew[256];//字符型 新变量值
	double dValue[2]; //dValue[0]数字型新变量值   dValue[1]数字型老变量值
};


class CLASS_DECLSPEC CDHPropGridCtrl
{
public:
	CDHPropGridCtrl(void);
	~CDHPropGridCtrl(void);

	
	//增加是否绑定参数文件标志bIsOperateFile，若需要绑定，把标志位置为1。
	//增加指定参数文件路径的接口chFileSectionName：格式应为如下，"file1,section;file2,section;file3,section" 
	//分号隔断的是不同子动态库，逗号隔断的是一个动态库中的参数文件名和APP字段。一个子动态库只能对应一组file,section
	//INI文件的路径必须完整,文件名前面的各级目录必须存在,否则写入不成功,该函数返回 FALSE 值.
	//文件名的路径中必须为 \\ ,因为在VC++中, \\ 才表示一个 \ .
	//也可将INI文件放在程序所在目录,此时 file 参数为: ".\\name.ini".
	//读取与保存参数 调用ReadModel() WriteModel(),初始化时会自动调用ReadModel()
	int InstallControl(CWnd* pParentDlg, const CRect rcPos, const char* chConfigMidName, const int nCameraIndex=0,const int nParaIndex=0,bool bIsOperateFile=false,const char* chFileSectionName="");
	/*
		pParentDlg:			父窗口指针
		rcPos:				位置
		strConfigMidName:	 列表配置文件名称 末尾不需要加".ini" 
		nCameraIndex:		第几路 从1开始
		nParaIndex:       第几个参数列表  从1开始
		///失效  AppName后边加上VL 就变为ValueList 例如[定位VL]
		bIsOperateFile:      是否关联参数文件标志位
		chFileSectionName:    参数文件路径及参数所在AppName接口。传递方式为"file1,section;file2,section;file3,section" 分号隔断的是不同子动态库，逗号隔断的是一个动态库中的参数文件名和APP字段。一个子动态库对应
		一个file，sectiion

	//*******************************INI文件设置规则******************************	
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

		绑定变量相关的新增加参数:
		[基本信息]中增加
		DefaultCtrlFileName=Default/realfilepath/NULL//      realfilepath格式如： *\\*\\*.ini  
        DefaultCtrlSectionName=Default/realsectionname/NULL//                realsectionname格式如：检测参数   
		
        ini各参数项中增加：    
		*_CtrlFileName=Default/Interface/realfilepath/NULL//
        *_CtrlSectionName=Default/Interface/realsectionname/NULL//
        *_CtrlKeyName=Default/realkeyname/NULL//                            realkeyname格式如：面积
		
		//*****************************FileName*****************************
		*_CtrlFileName为Default时，其取值会使用DefaultCtrlFileName的键值，此时当DefaultCtrlFileName为Default时，*_CtrlFileName会使用run\ModelInfo\($GridName)para.ini作为关联的参数文件名称             
		                                                                  当DefaultCtrlFileName为realfilepath时，*_CtrlFileName会使用realfilepath作为参数文件名
																		  当DefaultCtrlFileName为NULL时，所有*_CtrlFileName值为Default的参数不进行变量关联。
		*_CtrlFileName为Interface时*_CtrlFileName会使用InstallControl接口中chFileSectionName参数解析出的文件名。
		*_CtrlFileName为realfilepath时，*_CtrlFileName会使用realfilepath作为参数文件名
		*_CtrlFileName为NULL时，此控件不进行变量关联。
		
		//****************************SectionName*********************************
		*_CtrlSectionName为Default时，其取值会使用DefaultCtrlSectionName的键值，此时当DefaultCtrlSectionName为Default时，*_CtrlSectionName会使用当前参数所在参数列表中的组名作为其参数组名。           
		                                                                  当DefaultCtrlSectionName为realsectionname时，*_CtrlSectionName会使用realsectionname作为组名
																		  当DefaultCtrlSectionName为NULL时，所有*_CtrlSectionName值为Default的参数不进行变量关联。
		*_CtrlSectionName为Interface时*_CtrlSectionName会使用InstallControl接口中chFileSectionName参数解析出的文件名。
		*_CtrlSectionName为realsectionname时，*_CtrlSectionName会使用realsectionname作为参数文件名
		*_CtrlSectionName为NULL时，此控件不进行变量关联。
		
		//*****************************KeyName************************************
		*_CtrlKeyName为Default时，*_CtrlKeyName会使用当前参数在参数列表中的名称作为参数名。
		*_CtrlKeyName为realkeyname时，*_CtrlKeyName会使用realkeyname作为参数名
		*_CtrlKeyName为NULL时，此控件不进行变量关联。
		
	
		控件显示相关新增加参数：
		任意变量增加   *_CtrlIsVisible=0/1 表示此参数是否可以显示
		组或者动态库中增加 IsVisible=0/1   表示此组或动态库是否可以显示
		
		

	*/
	int SetColumnWidth(const int nVal);  //设定列表左侧参数名列的宽度
	int SetRowHeight(const int nVal);    //设定列表每行的高度
	int DisplayCtrl(const int nCmdShow); //是否显示列表
	int SetFont(LOGFONT& lFont);        //设定列表中文字字体

	//EnableCtrl的几种方法
	int EnableCtrl(int nDllIndex, char* chGroupName, int nCtrlIndex, BOOL bEnable = TRUE);
	int EnableCtrl(int nDllIndex, char* chGroupName, char* chCtrlName, BOOL bEnable = TRUE);
	int EnableCtrl(int nDllIndex, const char* chGroupName, const char* chCtrlName, BOOL bEnable = TRUE);
	int EnableCtrl(int nDllIndex, int nGroupIndex, int nCtrlIndex, BOOL bEnable = TRUE);
	/*
	nDllIndex设为-1，即都生效
	chGroupName设为-1，即该DllIndex下都生效
	nCtrlIndex设为-1，即该Group下面的都生效 是配置文件中CtrlName后边的序号
	*/
	int MoveWindow(const CRect rect, BOOL bRepaint = 1);


	int ReadModel();//从参数文件中读取参数刷新到界面上
	int WriteModel();//将参数界面上的参数值写入到参数文件中


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

