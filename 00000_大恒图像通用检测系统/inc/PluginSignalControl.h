#pragma once
//**[12/13/2017 ConanSteve]**:  系统运行数据,方便插件的查询
#include "vector"
#include "array"
#include "DHIOCard.h"

#define MAX_IOCARD_NUM 5//**[12/28/2017 ConanSteve]**:  最大接口卡个数
#define MAX_ASSEMBLYLINE_NUM 5 //**[12/28/2017 ConanSteve]**:  最大流水线个数
#define MAX_KICK_QUEUE_NUM 10//**[12/28/2017 ConanSteve]**:  最大剔废组个数
#define MAX_CHECK_GROUP_NUM 20//**[12/28/2017 ConanSteve]**:  最大检测组个数
#define MAX_CAMER_COUNT	40//**[12/28/2017 ConanSteve]**:  最大相机个数

using namespace std;
typedef struct s_SysDataInfoInit {//**[1/10/2018 ConanSteve]**:  初始化插件结构体
	int iSize;
	int iIOCardNum;//**[1/9/2018 ConanSteve]**:  接口卡数目
	int iAssemblyLineNum;//**[12/13/2017 ConanSteve]**:  系统的流水线数， 最大5
	int iKickQueueNum;//**[12/13/2017 ConanSteve]**:  剔废队列个数
	int iCheckGroupNum;//**[12/13/2017 ConanSteve]**:  检测组个数
	int iGrabNum;//**[12/13/2017 ConanSteve]**:  相机个数，最大20个相机
	int arr_iKickQueueNumOfEachaAssembly[MAX_ASSEMBLYLINE_NUM];//**[1/10/2018 ConanSteve]**:  每个流水线的剔废队列个数
	int arr_veciKickSNInAssemblyLine[MAX_ASSEMBLYLINE_NUM][MAX_KICK_QUEUE_NUM];//**[12/13/2017 ConanSteve]**:  每个流水线包含的剔废组序列号
	int arr_iCheckGroupNumOfEachKickQueue[MAX_KICK_QUEUE_NUM];
	int arr_veciCheckSNInKickQueue[MAX_KICK_QUEUE_NUM][MAX_CHECK_GROUP_NUM];//**[12/13/2017 ConanSteve]**:  每个剔废队列包含的检测组索引,最大支持20个剔废队列
	int arr_iGrabNumOfEachGroup[MAX_CHECK_GROUP_NUM];
	int arr_veciGrabSNInCheckGroup[MAX_CHECK_GROUP_NUM][MAX_CAMER_COUNT];//**[12/13/2017 ConanSteve]**:  每个检测组包含的相机索引，最大支持20个检测组
	CDHIOCard * arr_pIOCard[MAX_IOCARD_NUM];//**[12/13/2017 ConanSteve]**:  接口卡指针数组	
}s_SysDataInfoInit;

typedef struct  s_SysDataInfoRealTime
{
	int iSize;
	int  arr_HardImgCnt[MAX_CAMER_COUNT];//**[12/13/2017 ConanSteve]**:  相机图像号
	s_KickCountInfo arr_KickCntInfo[MAX_KICK_QUEUE_NUM];//**[1/10/2018 ConanSteve]**:  剔废信息

}s_SysDataInfoRealTime;

class CSignaleCnt
{
public:
protected:
private:
};