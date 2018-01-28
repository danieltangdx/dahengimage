/************************************************************************/
/* Copyright (c) 2005, 北京大恒图像视觉有限公司视觉系统部     
/* All rights reserved.													
/*																		
/* 文件名称： LogManager.h												
/* 文件标识： 
/* 摘要： 对涉及日志文件操作的各种函数进行汇总, 提供文件及路径常用操作,
/*    保存及打开图像文件, 保存文本文件等功能
/*
/* 当前版本： 2.0
/* 作者： 葛争
/* 完成日期： 2009年8月17日
/*修改日期：2009年8月18日 超出条数则命名为原名字_nextN.txt N=1,2,3...
/************************************************************************/

#if !defined(LogManager_h_)
#define LogManager_h_

//定义程序所需要的变量和结构体



enum e_SaveLogType	    //保存日志类型
{
	OperationLog = 0,			//操作日志
	CheckLog,		            //检测日志
	MendParamLog,			    //修改参数日志
	AbnormityLog                //异常日志
};

#define MAXLOGCOUNT 5000	// 单文件日志最大写入条数 [8/14/2009 GZ]


class CLogManager 
{
public:
	// 获取本执行程序所在的路径
	CString GetCurrentAppliationPath();
	
	//判断路径是否存在
	BOOL IsPathExist(const CString &strPath);
	
	//判断文件是否存在
	BOOL IsFileExist(const CString &strFileName);
	
	//创建一个多层目录，如果存在就不创建
	//(strPath  合法: "D:\\abc\\me" 或 "me\\you" 或 ""; 
	//			不合法: "D:\\abc\\me\\" 或 "me\\you\\" 或 "\\")
	BOOL CreateMultiLevelPath(const CString &strPath);

	CString GetLastManagerError();//如题
	CString m_strLastManagerError;
	CString m_strNext;// 条数超出MAXLOGCOUNT时文件名后加的后缀 [8/14/2009 GZ]
	CLogManager();
	virtual ~CLogManager(); 
	//创建一个目录，如果存在不创建
	BOOL MyCreatePath(const CString &strPath);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//函数MyWriteLogFile()
	//功能：	写日志，
	//			1.自动创建 自定义日志路径如下：
	//			Run\ (当前文件目录)
	//				LogFiles\
	//					 OperationLog\                （操作日志 通讯操作等）
	//					 AbnormityLog\               （异常日志）
	//					 CheckLog\                   （检测日志）
	//					 MendParamLog\              （修改参数日志  特殊的操作）

	//			2.支持超出单文件日志最大写入条数后新建文件写入日志
	//参数：
	//				//参数1：strLogFilePath为一个多层目录，如果存在就不创建，不存在则新创建,函数会在此路径下创建 功能1中的路径 然后写日志
	//				//		(strLogFilePath  合法: "D:\\abc\\me" 或 "me\\you" 或 ""; 
	//				//					不合法: "D:\\abc\\me\\" 或 "me\\you\\" 或 "\\")
	//			参数1：strSession为分类，即日志具体内容类别
	//			参数2：日志具体内容
	//			参数3：日志类型，见以上enum e_SaveLogType类型；0为操作日志，1为检测日志，2为修改参数日志，3为异常日志
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	BOOL MyWriteLogFile(/*const CString& strLogFilePath, */ // 去掉路径，默认为当前文件夹 [8/17/2009 GZ]
		const CString& strSession, 
		const CString& strContent,
		int   iLogType);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

protected:
private:
};

#endif // !define(LogManager_h_)