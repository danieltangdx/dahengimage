// DALSACamDH.h: interface for the CDALSACamDH class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BAUMERCAMDH_H__3407C25C_9F6F_44BF_8DCF_3E41CFAC6409__INCLUDED_)
#define AFX_BAUMERCAMDH_H__3407C25C_9F6F_44BF_8DCF_3E41CFAC6409__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DHGrabForBaumer.h"
#include <vector>
//#include "bgapi.h"
#include "bgapi.hpp"
#include <stdlib.h>
#include <stdio.h>
#include <conio.h> //20120815
using namespace std;
////////////////////////////////////////////////////////////////////////////
//
//定义网络设备最大数
const int SystemMaxNum  = 10;  

//定义相机最大数
const int CameraMaxNum  = 10;

//定义图像队列最大数
const int ImageMaxNum  = 10;

// 初始化文件名的最大长度
const int MaxFileNameLen = 255;

typedef struct _BAUMERInitStruct			// 参数初始化结构
{
	int nGrabberSN;							// 采集卡序号
    char strDeviceName[MaxFileNameLen];		// 设备名称
	char strDeviceMark[GBMaxTextLen];		// 设备标识
	char strGrabberFile[MaxFileNameLen];	// 用于初始化的文件
	PGBCALLBACK CallBackFunc;				// 回调函数指针
	PVOID Context;							// 存放拥有回调函数的对象的this指针
}s_BAUMER_INITSTRUCT;// 参数初始化结构

typedef struct _SystemCameraStruct			// 系统相机关联参数初始化结构
{
	BGAPI::Camera *m_pCamera;    //相机对象指针
	BGAPI::System * m_pSystem;   //相关系统指针
}s_SYSTEMCAMERASTRUCT;// 系统相机关联参数初始化结构


class CBaumerCamDH : public CGrabber  
{
protected: //相机操作

	//采集回调
	static BGAPI_RESULT BGAPI_CALLBACK imageCallback( void * callBackOwner, BGAPI::Image* pImage  );

	//初始化网络设备
	int init_systems(int * system_count, BGAPI::System ** externppSystem ,BGAPI::Camera ** externppCamera);
	//初始化相机
	int init_camera();

    //初始化
	BOOL InitParm(const s_BAUMER_INITSTRUCT* pInitParam); 
	void Fun1();
	int Fun2(); 



protected: //相机属性

	DWORD          m_FrameCnt;                                 //采集图像的数量

public:
	CBaumerCamDH();
	virtual ~CBaumerCamDH();

	BOOL Init(const s_GBINITSTRUCT* pInitParam);                //初始化
	virtual bool Init(const CGrabInitInfo& rGrabInitInfo) override;
	
	BOOL StartGrab();                                           // 开始采集

	BOOL StopGrab();                                            // 停止采集

	BOOL Snapshot();											//单帧采集

	BOOL Close();	                                            // 关闭
	
	BOOL GetParamInt(GBParamID Param, int &nOutputVal);         //设置参数

	BOOL SetParamInt(GBParamID Param, int nInputVal);          //设置参数
	
	void CallParamDialog();                                     //调用参数对话框
	
	void GetLastErrorInfo(s_GBERRORINFO *pErrorInfo);           //得到出错信息

	BOOL SetParamfloat(int Param, float nInputVal);
	BOOL GetParamfloat(int Param, float& nOutputVal);
	BOOL SetTriggerParam(int ParamValInput);						//设置触发类型
	BOOL SetAOI(RECT rtAOI);													//设置采集区域
	BOOL GetAOI(RECT &rtAOI);													//得到采集区域



	//得到触发模式
	BOOL GetTriggerParam(int &ParamValOutput);
	
	//重启设备
	BOOL Reset();
	
	//得到系统信息
	BOOL GetSystemInfo(CString &sOutputInfo);
	
	//创建默认配置文件
	BOOL CreateDefaultConfigurationFile(CString &sInitFile);
	
	//保存配置文件
	BOOL SaveToConfigurationFile(CString &sInitFile);


	BOOL SetImageFlipMode(int iType);
	/*使用友员类可以解决成员变量私有访问的问题，以下变量原为protected类型*/
	s_GBERRORINFO		m_LastErrorInfo;	    //错误信息2007.12.18使控制对话框可以访问，提供错误代码
    BOOL			    m_bGrabbing;	        //是否正在采集
	BOOL			    m_bInitSucceed;         //是否初始化成功
	CString				m_sInitFile;			//用于初始化的文件


	
protected:
	int  m_nGrabberSN;							//序号
	CString m_sDeviceName;			            //设备名
	CString m_strDeviceMark;					//设备标识
	PGBCALLBACK m_CallBackFunc;					//回调函数指针
	PVOID m_Context;							//存放调用初始化函数的对象的this指针

	int m_nImageWidth;                          //图像宽度
	int m_nImageHeight;                         //图像高度

	int m_nImageBufferSize;                     //图像缓存大小
	int m_nImageByteCount;                      //图像每个像素的字节数
	void*  m_pbImageBufferAdd;                  //用于传出的图像内存地址

	BOOL m_bSet1,m_bSet2,m_bSet3;						//相机参数设置控制标志
	BOOL m_bSquencerShot;								//单帧采集控制标志
	int m_nTrigger,m_nSequencerNum;

	int m_nExMax,m_nExMin;						//相机最大（最小）曝光值
	float m_nGainMax,m_nGainMin;				//相机最大（最小）增益值

	BGAPI::Camera *m_pCamera;					//当前相机对象指针
	BGAPI::System * m_pSystem;					//当前系统对象指针

	BGAPI::System * m_ppSystem[SystemMaxNum];			//记录已经创建的网络设备对象数组
	s_SYSTEMCAMERASTRUCT m_SystemCamera[CameraMaxNum];	//记录已经创建的相机对象数组
	BGAPI::Image *m_pImage[ImageMaxNum];				//记录已经创建的图像队列对象数组
	int m_CameraNum;									//实际创建的相机对象个数

	int m_ndoSequencerReset;

};

#endif // !defined(AFX_BAUMERCAMDH_H__3407C25C_9F6F_44BF_8DCF_3E41CFAC6409__INCLUDED_)
