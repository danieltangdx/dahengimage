#pragma once

/*************************************<动态（用户）创建控件资源ID>************************************[8/28/2017 ConanSteve]*/

#define IDR_USER 0x00010000

#define ID_STATIC_CAMERA_IMAGE IDR_USER+0
#define ID_STATIC_ALGORITHM_IMAGE IDR_USER+50


//**[12/8/2017 ConanSteve]**:  静态文本资源
#define IDC_STATIC_CHECK_COUNT IDR_USER+100//**[12/1/2017 ConanSteve]**:  检测总数
#define IDC_STATIC_GOOD_COUNT IDR_USER+101//**[12/4/2017 ConanSteve]**:  好品计数
#define IDC_STATIC_ERROR_COUNT IDR_USER+102//**[12/4/2017 ConanSteve]**:  错误总数
#define IDC_STATIC_GOOD_RATE IDR_USER+103//**[12/4/2017 ConanSteve]**:  好品率


//**[12/27/2017 ConanSteve]**:  按钮资源，但是不绑定事件，只是作为显示使用


//**[1/24/2018 ConanSteve]**:  最多支持5道流水线
/*************************************<200>************************************[12/28/2017 ConanSteve]*/
#define		IDC_BUTTON_LABEL_TOTLE_COUNT_ASSEMBLY	IDR_USER +200 //**[1/24/2018 ConanSteve]**:  流水线检测总数
#define		IDC_BUTTON_LABEL_GOOD_COUNT_ASSEMBLY		IDR_USER+205//**[12/27/2017 ConanSteve]**:  流水线好品计数
#define		IDC_BUTTON_LABEL_BAD_COUNT_ASSEMBLY		IDR_USER+210//**[12/27/2017 ConanSteve]**:  流水线坏品计数
#define		IDC_BUTTON_LABEL_GOOD_RATIO_ASSEMBLY		IDR_USER+215//**[12/27/2017 ConanSteve]**:  流水线合格率
#define		IDC_BUTTON_LABEL_CHECK_SPEED_ASSEMBLY				IDR_USER +220//**[1/24/2018 ConanSteve]**:	单道流水线的检测速度 

/*************************************<250>************************************[12/28/2017 ConanSteve]*/

#define		IDC_BUTTON_LABEL_BEGIN				IDR_USER+250//**[12/27/2017 ConanSteve]**:  开头
#define		IDC_BUTTON_LABEL_END				IDC_BUTTON_LABEL_BEGIN+1+7  //**[1/26/2018 ConanSteve]**:	最后一个数和最后一个控件的附加值一样 

#define		IDC_BUTTON_LABEL_MODEL_NAME						IDC_BUTTON_LABEL_BEGIN+0//**[1/24/2018 ConanSteve]**:  模板名称
#define		IDC_BUTTON_LABEL_USER_INFO						IDC_BUTTON_LABEL_BEGIN+1
#define		IDC_BUTTON_LABEL_TIME							IDC_BUTTON_LABEL_BEGIN+2//**[1/26/2018 ConanSteve]**:	当前时间 
#define		IDC_BUTTON_LABEL_CHECK_COUNT_TOTLE					IDC_BUTTON_LABEL_BEGIN+3//**[1/26/2018 ConanSteve]**:	检测总数 
#define		IDC_BUTTON_LABEL_GOOD_COUNT_TOTLE						IDC_BUTTON_LABEL_BEGIN+4//**[1/26/2018 ConanSteve]**:	好品总数 
#define		IDC_BUTTON_LABEL_ERROR_COUNT_TOTLE					IDC_BUTTON_LABEL_BEGIN+5//**[1/26/2018 ConanSteve]**:	坏品总数 
#define		IDC_BUTTON_LABEL_GOOD_RATE_TOTLE						IDC_BUTTON_LABEL_BEGIN+6//**[1/26/2018 ConanSteve]**:	合格率 
#define		IDC_BUTTON_LABEL_CHECK_SPEED					IDC_BUTTON_LABEL_BEGIN+7//**[1/26/2018 ConanSteve]**:	检测速度 



//**[12/8/2017 ConanSteve]**:  其他控件资源
#define IDC_LISTCTRL_REALTIME_RUN_INFO   IDR_USER+280//**[12/8/2017 ConanSteve]**: 运行信息CListCtrl控件 





//**[12/8/2017 ConanSteve]**:  按钮资源,最大绑定100个按钮事件
#define IDC_BUTTON_USER WM_USER+300 //**[1/26/2018 ConanSteve]**:	因为要绑定事件，所以必须用 WM_SUER 

#define IDC_BUTTON_BEGIN			IDC_BUTTON_USER
#define IDC_BUTTON_END				IDC_BUTTON_USER+1+11 //**[1/26/2018 ConanSteve]**:	最后一个数和最后一个控件的附加值一样 

#define IDC_BUTTON_RESET_SYSTEM					 IDC_BUTTON_BEGIN+0  //**[12/27/2017 ConanSteve]**:  系统计数清零
#define IDC_BUTTON_OPEN_IOCARD_TOOLS			 IDC_BUTTON_BEGIN+1 //**[12/8/2017 ConanSteve]**:  切换模板按钮
#define IDC_BUTTON_SWITCH_MODEL					 IDC_BUTTON_BEGIN+2 //**[12/8/2017 ConanSteve]**:  打开接口卡工具按钮
#define IDC_BUTTON_RECORD  							IDC_BUTTON_BEGIN+3 //**[12/13/2017 ConanSteve]**:  记录日志按钮
#define IDC_BUTTON_DEFECT_STATISTICS					IDC_BUTTON_BEGIN+4//**[12/27/2017 ConanSteve]**:  缺陷统计
#define IDC_BUTTON_DATA_STATISTICS						IDC_BUTTON_BEGIN+5//**[1/26/2018 ConanSteve]**:	数据统计 
#define IDC_BUTTON_OPEN_MULTI_OBJECT_RUN_INFO_WINDOW	IDC_BUTTON_BEGIN+6//**[1/2/2018 ConanSteve]**:	多对象运行信息
#define IDC_BUTTON_USER_MANAGEMENT						IDC_BUTTON_BEGIN+7//**[1/26/2018 ConanSteve]**:	用户管理 
#define IDC_BUTTON_SUPER_ADMIN_SETTING					IDC_BUTTON_BEGIN+8//**[1/26/2018 ConanSteve]**:	超级管理员开发设置 
#define IDC_BUTTON_SYSTEM_DEVELOPER_SETTING					IDC_BUTTON_BEGIN+9//**[1/26/2018 ConanSteve]**:	系统研发人员开发设置 
#define IDC_BUTTON_RESIZE_LAYOUT						IDC_BUTTON_BEGIN+10//**[1/26/2018 ConanSteve]**:	重新调整界面布局 
#define IDC_BUTTON_SWITCH_USER							IDC_BUTTON_BEGIN+11//**[1/26/2018 ConanSteve]**:	切换用户 






//**[12/4/2017 ConanSteve]**:  计时器
#define ID_EVENT_UPDATE_TIME IDR_USER+1000//**[12/4/2017 ConanSteve]**:  时间
#define ID_EVENT_UPDATE_INFO IDR_USER+1001//**[12/4/2017 ConanSteve]**:  检测信息更新
#define ID_EVENT_UPDATE_CHECK_SPEED IDR_USER+1002//**[12/28/2017 ConanSteve]**:  产品速度更新
#define ID_EVENT_START_GRABBER IDR_USER+1003


#define ID_INDICATOR_GRAB               61446
#define ID_INDICATOR_CHECK              61447
#define ID_INDICATOR_ERROR              61448
#define ID_INDICATOR_OPERATION          61449


#define WM_USER_CLOSE WM_USER+1
