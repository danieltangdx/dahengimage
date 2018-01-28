#pragma once
//**[7/18/2017 ConanSteve]**:  扩展功能定义
enum e_OptionalFuncCode {
	e_Fn_NONE = 0,//**[1/11/2018 ConanSteve]**:  默认未设置




	/*************************************<s_CheckInParam>************************************[1/11/2018 ConanSteve]*/
	e_Fn_CheckInParam_SmallImageNumOfEachGrab = 0x20000,//**[1/11/2018 ConanSteve]**:  当前工位每个相机的小张图张数	int
	e_Fn_CheckInParam_ImageWidth = 0x20001,//**[1/11/2018 ConanSteve]**:  图像宽度	int
	e_Fn_CheckInParam_ImageHeight = 0x20002,//**[1/11/2018 ConanSteve]**:	int
	e_Fn_CheckInParam_ImageChannel = 0x20003,//**[1/11/2018 ConanSteve]**:	int
	e_Fn_CheckInParam_WhetherCicleSaveImage = 0x20004,//**[1/11/2018 ConanSteve]**:  是否循环存图	bool
	e_Fn_CheckInParam_SaveImagePath = 0x20005,//**[1/11/2018 ConanSteve]**:  存图路径
	e_Fn_CheckInParam_GrabNumPerCheckGroup =0x20006,//**[1/25/2018 ConanSteve]**:  每个检测工位的相机个数

	e_Fn_CheckInParam_DetectTimes = 0x20100,//**[1/11/2018 ConanSteve]**:  算法检测次数	int

	e_Fn_CheckInParam_TimeWait = 0x20200,//**[1/11/2018 ConanSteve]**:  图像数据等待时间
	e_Fn_CheckInParam_TimeFirstDetect = 0x20201,//**[1/11/2018 ConanSteve]**:  一次Detect时间 int
	e_Fn_CheckInParam_TimeSecondDetect = 0x20202,//**[1/11/2018 ConanSteve]**:  2次Detect时间 int
	e_Fn_CheckInParam_MomentOfPushQueue = 0x20203,//**[1/12/2018 ConanSteve]**:  图像数据入队列的时刻

	/*************************************<s_AlgModelInput>************************************[1/11/2018 ConanSteve]*/
	//**[1/11/2018 ConanSteve]**:  读模板
	e_Fn_AlgModelInput_LoadModelType = 0x30000,//**[1/11/2018 ConanSteve]**:  加载模板的类型 int
	e_Fn_AlgModelInput_CheckObjIndex = 0x30001,//**[1/11/2018 ConanSteve]**:  检测对象在检测队列中的索引	void*
	e_Fn_AlgModelInput_FirstCheckObjAddr = 0x30002,//**[1/11/2018 ConanSteve]**:  首个检测对象的指针	int

	//**[1/11/2018 ConanSteve]**:  存图300使用
	e_Fn_AlgModelInput_ImageSN = 0x30100,//**[1/11/2018 ConanSteve]**:  算法存图的图像号 int
	e_Fn_AlgModelInput_ImageSavedCnt = 0x30101,//**[1/11/2018 ConanSteve]**:  系统已经存图张数 int
	e_Fn_AlgModelInput_SaveImagePath = 0x30102,//**[1/11/2018 ConanSteve]**:  算法存图的路径，单相机单张为bmp路径，单相机序列图为文件夹，void*转TCHAR[][MAX_PATH]

	e_Fn_AlgModelInput_WhetherShowObjectRunInfo = 0x30200,//**[1/12/2018 ConanSteve]**:  是否显示多对象运行信息窗口
	/*************************************<s_CheckOutPara>************************************[1/11/2018 ConanSteve]*/
	e_Fn_CheckOutPara_AlgCheckErrorDesc = 0x40000,   //**[1/10/2018 ConanSteve]**:  算法返回的错误描述 TCHAR[]


	e_Fn_MAX = 0xFFFFFFFF
																																									 //**[1/11/2018 ConanSteve]**:  多对象存图相关
};

/*************************************<加载模板控制参数定义>*************************************/
enum e_LoadModelCtrlPara
{
	e_LoadModelCtrlPara_ReadPara = 1,
	e_LoadModelCtrlPara_ReadModel = 2,
	e_LoadModelCtrlPara_ReadParaAndModel = 3
};