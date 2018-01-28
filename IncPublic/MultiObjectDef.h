#pragma once
/************************************************************************/
/**               [5/3/2017 ConanSteve]**/
enum e_MachineType
{
	e_MachineType_Capsule,//**[7/14/2017 ConanSteve]**:  胶囊
	e_MachineType_Liquor
};
/**               [5/3/2017 ConanSteve]**/
enum e_WriteDataStatus
{
	e_WriteDataStatus_Leisure,
	e_WriteDataStatus_Busy,
	e_WriteDataStatus_Complete
};
enum e_DetectStatus {
	e_DetectStatus_NoChecking,
	e_DetectStatus_Checking
};

enum e_TimeType//**[10/9/2017 ConanSteve]**:  检测超时判断类型
{
	e_TimeType_MilliSecond,
	e_TimeType_Frame
};
enum e_WhetherDraw {//**[10/9/2017 ConanSteve]**:  多对象判断是否画图
	e_WhetherDraw_Draw,
	e_WhetherDraw_NotDraw
};
enum e_CheckOutWhetherDraw {//**[10/9/2017 ConanSteve]**:  checkOut返回的值是否画图
	e_CheckOutWhetherDraw_NotDraw,
	e_CheckOutWhetherDraw_Draw
};


enum e_SaveImgObj {
	e_SaveImgObj_Alg= 0,//**[10/9/2017 ConanSteve]**:  算法存图
	e_SaveImgObj_MultiObj= 1,//**[10/9/2017 ConanSteve]**:  多对象存图
	e_SaveImgObj_System=2//**[11/23/2017 ConanSteve]**:  系统存图
};

enum e_NamingRegulation {
	e_NamingRegulation_ImgSN,
	e_NamingRegulation_ProductSN
};

enum e_SysDetectMode {
	e_SysDetectMode_Multi=0,//**[7/19/2017 ConanSteve]**:  一次一组图(有可能是一张图也有可能是多张图)检测
	e_SysDetectMode_Single//**[7/19/2017 ConanSteve]**:  一次单张图检测
};
enum e_ObjDetectMode//**[7/26/2017 ConanSteve]**:  算法检测模式
{
	e_ObjDetectMode_1,
	e_ObjDetectMode_2
};



/************************************************************************/