#pragma once
#define MAX_IOCARD_NUM 5//**[12/28/2017 ConanSteve]**:  最大接口卡个数
#define MAX_ASSEMBLYLINE_NUM 5 //**[12/28/2017 ConanSteve]**:  最大流水线个数
#define MAX_KICK_QUEUE_NUM 10//**[12/28/2017 ConanSteve]**:  最大剔废组个数
#define MAX_CHECK_GROUP_NUM 20//**[12/28/2017 ConanSteve]**:  最大检测组个数
#define MAX_CAMER_COUNT	40//**[12/28/2017 ConanSteve]**:  最大相机个数

enum eSaveImageType
{
	SaveGrabImage = 0,                // 保存连续采集图像 [8/19/2009 GZ]
	SaveErrorImage,					// 保存错误图像
	SaveAbnormityImage,				// 保存异常图像
	SaveCorrectImage,				// 保存正确图像
	SaveModelImage					// 保存模板图像
};

enum e_ProductLine
{
	e_ProductLine_NO,//**[11/15/2017 ConanSteve]**:  无，系统退出
	e_ProductLine_General,//**[11/21/2017 ConanSteve]**:  通用算法
	e_ProductLine_Stopper,//**[11/21/2017 ConanSteve]**:  胶塞算法
	e_ProductLine_LampInsp,//**[11/24/2017 ConanSteve]**:  灯检机算法
	e_ProductLine_LableCheck,//**[12/12/2017 ConanSteve]**:  采血管算法
	e_ProductLine_BlisterOnline//**[12/14/2017 ConanSteve]**:  泡罩在线算法，结构体与General一样，但是在计算剔废结果时，一个踢废回调给多个剔废结果
};

enum e_KickType {
	e_KickType_Single,//**[12/14/2017 ConanSteve]**:  单路剔废，综合多路检测结果，只写一个剔废结果给接口卡
	e_KickType_Multi_Vertical,//**[12/14/2017 ConanSteve]**:  多路剔废，单次剔废回调的每个检测结果都分别写一个踢废口，例如泡罩在线，瓶盖分色,如果检测结果只有一个，实际效果等同单路剔废
	e_KickType_Multi_Horizontal//**[12/14/2017 ConanSteve]**:  多路剔废水平模式，以胶囊为例，每个产品水平8个检测工位，垂直4个检测结果，则每次剔废也写4个结果
};

enum e_SaveImgType {
	e_SaveImgType_NO,//**[11/23/2017 ConanSteve]**:  不存图
	e_SaveImgType_Consecutive,//**[11/23/2017 ConanSteve]**:  存连续图像
	e_SaveImgType_Error,//**[11/23/2017 ConanSteve]**:  错误图像
	e_SaveImgType_ProductExist//**[11/23/2017 ConanSteve]**:  有产品存图
};


enum e_CheckTotleCntType {//**[12/4/2017 ConanSteve]**:  检测总数的统计类型
	e_CheckTotleCntType_PE,//**[12/4/2017 ConanSteve]**:  光电输入
	e_CheckTotleCntType_Rlst//**[12/4/2017 ConanSteve]**:  1工位检测结果有产品，则检测总数增加
};

enum e_AlgStatus {
	e_AlgStatus_Good,//**[12/5/2017 ConanSteve]**:  好品
	e_AlgStatus_Empty,
	e_AlgStatus_Bad,//**[12/5/2017 ConanSteve]**:  坏品
	e_AlgStatus_AlgException,//**[12/5/2017 ConanSteve]**:  算法异常
	e_AlgStatus_LostImg,//**[12/5/2017 ConanSteve]**:  多对象丢图
	e_AlgStatus_Overtime//**[12/5/2017 ConanSteve]**:  检测超时
};;
