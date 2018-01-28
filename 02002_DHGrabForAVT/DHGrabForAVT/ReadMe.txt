
/////////////////////////////////////////////////////////////////////////////
4.0.0.3beta3 2011.3.7

4.0.0.4beta1 2011.5.26 by SGQ
采用AVT FirePackage
Version: 3.0


4.0.0.4beta2 2011.6.3 by SGQ
将支持相机最大数改为20 


4.0.0.5beta1 2011.7.14 by SGQ
更新UniApi库 解决XML文件不能保存相机触发模式问题。
这个版本需要安装AVTFirePackage_3.0和AVTUniversalPackage_2.0才能用

4.0.0.5beta3 for Pike 2011.12.20 by SGQ
因为Pike相机不支持UniApi库，所以单独生成一个版本，采用INI配置文件方式保存。

Pike相机使用方法：
1. 将不经常改动的参数保存到相机内存中，上电启动后自动加载。
2. 将经常改动的参数保存到INI配置文件中。

5.0.0.1beta2 2012.8.2 by SGQ
 1. 去掉XML文件初始化配置相机模式，改用自生成的INI配置文件初始化配置相机； 
 2. 驱动程序只需安装FirePackage，不再需要其他驱动程序； 
 3. 完善开采停采控制方式； 
 4.完善相机设置对话框，添加AOI设置功能,采集帧率显示功能,修改参数需要申请权限。

 5.0.0.1beta3 2012.8.3 by SGQ
 1. 完善Format7寄存器读写方式

  5.0.0.1beta5 2012.8.13 by SGQ
 1. 添加相机Reset接口
 	/************************************************************************/
	/*  功能：设置相机软复位，本功能在相机出现异常无回调情况下使用，
	/*  基本动作包括：
	/*  第一步:相机内部加载默认设置;
	/*  第二步: bus resets will occur;
	/*	第三步:	The FPGA will be rebooted; 
	/*	第四步:加载INI配置文件设置;
	/*  输入：
	/*	返回：正确:TRUE   错误:FALSE
	/************************************************************************/
	//BOOL Reset();

 2. 添加创建默认INI配置文件功能
 	/************************************************************************/
	/*  功能：生成默认INI配置文件
	/*  输入：sInitFile 正确格式：D:\\Run\\CameraParm.ini
	/*	返回：正确:TRUE   错误:FALSE
	/************************************************************************/
	//BOOL CreateDefaultINI(CString &sInitFile);

  5.0.0.1beta7 2012.8.28 by SGQ
1. 支持AVT 1394系列彩色相机
2. 完善Reset接口

  5.0.0.1beta8 2012.8.29 by SGQ
1. 完善相机控制框登录方式


  5.0.0.1beta9 2012.8.29 by SGQ
1. 完善设置AOI控制方式


  5.0.0.1beta10 2012.8.30 by SGQ
1. 在配置文件设置包长参数 Packet=0。

  5.0.0.1beta11 2012.9.18 by SGQ
1. 修改在设置框中设置AOI的Bug
2.修改由非Format7换到Format7时，第一次启动失败问题

  6.0.0.1beta1 2012.10.8 by SGQ
重整框架

  6.0.0.1beta2 2013.5.7 by SGQ
集成MultiShot功能

MultiShot模式设置过程：
1. 相机配置文件中
Trigger=9

2. 设置触发采集n帧数(例如n=14帧)
方式一：
相机配置文件中
MultishotNum=14
方式二：
初始化时，调用 
int Param = 13;
int nInputVal = 14;
m_pCamera->SetParamInt(Param,nInputVal);

3. 读取当前采集帧数
int Param = 13;
int nOutputVal = 0;
m_pCamera->GetParamInt(Param, nOutputVal);


4. 检测端开线程，实时扫描外部触发信号(灯检机为回摆信号)，当有触发信号时，
调用 m_pCamera->Snapshot()发送软触发信号，相机开始按照设置的帧数连续采集n帧图像，然后
停止采集等待下一个触发信号。

5. Multishot模式转化为内触发模式(连续采集模式)
int ParamValInput = 0;
m_pCamera->SetTriggerParam(0);

6. 内触发模式(连续采集模式)转化为Multishot模式
int ParamValInput = 9;
m_pCamera->SetTriggerParam(0);


7.读取当前触发模式
int ParamValOutput = 0;
m_pCamera->GetTriggerParam(ParamValOutput);

6.0.0.1beta3 2013.11.1 by SGQ
AOI设置


6.0.0.5 2013.12.3 by SGQ
解决初始化AOI参数保存到FLASH冲突问题

6.0.0.6 2013.12.27 by SGQ
去掉相机flash操作
