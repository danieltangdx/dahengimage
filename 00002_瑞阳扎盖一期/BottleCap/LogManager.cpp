/************************************************************************/
/* Copyright (c) 2005, 北京大恒图像视觉有限公司视觉系统部     
/* All rights reserved.													
/*																		
/* 文件名称： lLogManager.cpp												
/* 文件标识： 
/* 摘要： 对涉及日志文件操作的各种函数进行汇总, 
/*    
/*
/* 当前版本： 1.1
/* 作者： 葛争
/* 完成日期： 2009年8月17日
/************************************************************************/

#include "stdafx.h"
#include "LogManager.h"
#include <afxtempl.h>//为了使用CList


CLogManager::CLogManager()
{	
	m_strNext="_next";
	m_strLastManagerError="";
}
CLogManager::~CLogManager()
{
	
}
//创建一个目录，如果存在不创建
BOOL CLogManager::MyCreatePath(const CString &strPath)
{
	WIN32_FIND_DATA fd; 
	HANDLE hFind = FindFirstFile(strPath, &fd); 
	if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{ 
		FindClose(hFind);
		return true; 
	} 
	FindClose(hFind); 
	
	return CreateDirectory(strPath, NULL);
}



//写日志文件
BOOL CLogManager::MyWriteLogFile(
						 const CString& strSession, 
						 const CString& strContent,
						 int   iLogType)
{
	//构造Logs文件目录和ini文件名
	CString strIniPath, strIniFileName, strTime;
	CTime curTime = CTime::GetCurrentTime();
	DWORD dwTime = GetTickCount()%1000;
	CString strLogFilePath=GetCurrentAppliationPath();

	static int iCount_Operation=0;// 用于写入条数计数 [8/18/2009 GZ]
	static int iCount_Check=0;
	static int iCount_MendParam=0;
	static int iCount_Abnormity=0;// 用于写入条数计数 [8/18/2009 GZ]
	int tmpLogCount=0;//本文件已写入记录条数计数	
	switch(iLogType)
	{
	case OperationLog:   //操作日志
		{
			strIniPath.Format("%s%s%4d%s%02d%s", strLogFilePath, "LogFiles\\OperationLog\\",
							curTime.GetYear(), "年",curTime.GetMonth(), "月");

			strIniFileName.Format("%s%s%4d%02d%02d%s", 
							strIniPath, "\\", 
							curTime.GetYear(), 
							curTime.GetMonth(), 
							curTime.GetDay(), ".txt");
			iCount_Operation++;
			tmpLogCount=iCount_Operation;
		}
		break;
	case CheckLog:       //检测日志
		{
			strIniPath.Format("%s%s%4d%s%02d%s", strLogFilePath, "LogFiles\\CheckLog\\",
							curTime.GetYear(), "年",curTime.GetMonth(), "月");

			strIniFileName.Format("%s%s%4d%02d%02d%s", 
							strIniPath, "\\", 
							curTime.GetYear(), 
							curTime.GetMonth(), 
							curTime.GetDay(), ".txt");
			iCount_Check++;
			tmpLogCount=iCount_Check;
		}
		break;
	case MendParamLog:   //修改参数日志
		{
			strIniPath.Format("%s%s%4d%s%02d%s", strLogFilePath, "LogFiles\\MendParamLog\\",
							curTime.GetYear(), "年",curTime.GetMonth(), "月");

			strIniFileName.Format("%s%s%4d%02d%02d%s", 
							strIniPath, "\\", 
							curTime.GetYear(), 
							curTime.GetMonth(), 
							curTime.GetDay(), ".txt");
			iCount_MendParam++;
			tmpLogCount=iCount_MendParam;
		}
		break;
	case AbnormityLog:   //异常日志
		{
			strIniPath.Format("%s%s%4d%s%02d%s", strLogFilePath, "LogFiles\\AbnormityLog\\",
							curTime.GetYear(), "年",curTime.GetMonth(), "月");

			strIniFileName.Format("%s%s%4d%02d%02d%s", 
							strIniPath, "\\", 
							curTime.GetYear(), 
							curTime.GetMonth(), 
							curTime.GetDay(), ".txt");
			iCount_Abnormity++;
			tmpLogCount=iCount_Abnormity;
		}
		break;
	default:
		{
			strIniPath.Format("%s%s%4d%s%02d%s", strLogFilePath, "LogFiles\\OperationLog\\",
							curTime.GetYear(), "年",curTime.GetMonth(), "月");

			strIniFileName.Format("%s%s%4d%02d%02d%s", 
							strIniPath, "\\", 
							curTime.GetYear(), 
							curTime.GetMonth(), 
							curTime.GetDay(), ".txt");
			iCount_Operation++;
			tmpLogCount=iCount_Operation;
		}
	}
		
	strTime.Format("%02d%s%02d%s%02d:%03d%03d", 
		curTime.GetHour(), ":", 
		curTime.GetMinute(), ":", 
		curTime.GetSecond(),dwTime, rand()%1000);
	
	if (!IsPathExist(strIniPath))// 路径不存在，创建路径 [8/17/2009 GZ]
	{
		switch(iLogType)
		{
		case OperationLog:
			iCount_Operation=0;
			break;
		case CheckLog:
			iCount_Check=0;
			break;
		case MendParamLog:
			iCount_MendParam=0;
			break;
		case AbnormityLog:
			iCount_Abnormity=0;
			break;
		default:
			iCount_Operation=0;
			break;
		}
		if (CreateMultiLevelPath(strIniPath))
		{
			
			if (!WritePrivateProfileString(strSession, strTime, strContent, strIniFileName))
			{
				m_strLastManagerError.Format("%s","WritePrivateProfileString函数该条日志写入未成功");
				return FALSE;
			}
			
		}
		else
		{
			//AfxMessageBox("文件夹路径没有创建成功!");
			m_strLastManagerError="文件夹路径没有创建成功";
			return FALSE;
		}
	}
	else  //路径存在，先判断文件是否存在
	{
		//如果文件存在，先判断是否条数超出限制，再写入条数
		if (IsFileExist(strIniFileName))
		{

			if (tmpLogCount>MAXLOGCOUNT)// 如果超出最大限制，重新开文件存储 [8/14/2009 GZ]
			{
				
				CString tmpstrNext("");
				tmpstrNext=strIniFileName;
				tmpstrNext.Delete(tmpstrNext.GetLength()-4,4);
				CString strNextName("");
				strNextName.Format("%s_next%d.txt",tmpstrNext,tmpLogCount/MAXLOGCOUNT);
				
		
				if (!WritePrivateProfileString(strSession, strTime, strContent, strNextName))
				{
					m_strLastManagerError.Format("%s","WritePrivateProfileString函数该条日志写入未成功");
					return FALSE;
				}

			}
			else// 正常则正常写 [8/18/2009 GZ]
			{
						
				if (!WritePrivateProfileString(strSession, strTime, strContent, strIniFileName))
				{
					m_strLastManagerError.Format("%s","WritePrivateProfileString函数该条日志写入未成功");
					return FALSE;
				}
						
			}
			
			
		}
		else// 文件不存在清零 [8/18/2009 GZ]
		{
			switch(iLogType)
			{
			case OperationLog:
				iCount_Operation=0;
				break;
			case CheckLog:
				iCount_Check=0;
				break;
			case MendParamLog:
				iCount_MendParam=0;
				break;
			case AbnormityLog:
				iCount_Abnormity=0;
				break;
			default:
				iCount_Operation=0;
				break;
			}
			if (!WritePrivateProfileString(strSession, strTime, strContent, strIniFileName))
			{
				m_strLastManagerError.Format("%s","WritePrivateProfileString函数该条日志写入未成功");
				return FALSE;
			}
		}
	}

	return TRUE;
}

CString CLogManager::GetLastManagerError()
{
	return m_strLastManagerError;
}

// 获取本执行程序所在的路径
CString CLogManager::GetCurrentAppliationPath()
{
	char str[MAX_PATH], drive[MAX_PATH], dir[_MAX_DIR], fname[_MAX_FNAME], ext[_MAX_EXT];
	CString AppPath;
	GetModuleFileName(NULL, str, sizeof(str));
	_splitpath(str, drive, dir, fname, ext);
	strcat(drive, dir);
	AppPath = drive;

	return AppPath;
}
#include <io.h>//头文件

//判断路径是否存在
BOOL CLogManager::IsPathExist(const CString &strPath)
{
	
#ifdef _UNICODE
#define IsDirectoryExist(path) ::_waccess(path,0) == 0
	//_waccess的第二个参数表示仅判断存在性，返回0表示存在，返回-1表示不存在
#else
#define IsDirectoryExist(path) ::_access(path,0) == 0//_access是MBCS版本
#endif

	if(IsDirectoryExist(strPath))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//判断文件是否存在
BOOL CLogManager::IsFileExist(const CString &strFileName)
{
	CFileFind searcher;

	if(searcher.FindFile(strFileName))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//创建一个多层目录，如果存在就不创建
//(strPath  合法: "D:\\abc\\me" 或 "me\\you" 或 ""; 
//			不合法: "D:\\abc\\me\\" 或 "me\\you\\" 或 "\\")
BOOL CLogManager::CreateMultiLevelPath(const CString &strPath)
{
	CString tmpPath;// = strPath;
	tmpPath.Format("%s",strPath);
	CList<CString,CString&> FolderStrList;
	int nStrLen = 0;
	
	for(;;)
	{
		nStrLen = tmpPath.GetLength();
		if(nStrLen <= 0)
		{
			break;//从头建文件夹
		}		
	
		BOOL bPathExist = FALSE;//该路径是否已经存在

		if(tmpPath.Right(1) == ":")
		{
			bPathExist = IsPathExist(tmpPath + "\\");
		}
		else
		{
			bPathExist = IsPathExist(tmpPath);
		}

		if(bPathExist)//如果路径存在
		{
			break;//从已存在的路径下建文件夹
		}
		
		//如果路径不存在		
		//每次将最后的一个文件夹名入队
		
		int nPos = 0;
		nPos = tmpPath.ReverseFind('\\');//从后往前确定文件夹名
		if(nPos == -1)//已到达第一个文件夹名
		{
			nPos = 0;//将第一个文件夹名入队
		}
		
		//将文件夹名入队
		CString tmpStr = tmpPath.Right(nStrLen - nPos);
		FolderStrList.AddHead(tmpStr);
		tmpPath.Delete(nPos, nStrLen - nPos);		
	}	
	
	//根据文件夹名队列顺序建立
	while(FolderStrList.IsEmpty() == FALSE)
	{
		tmpPath += FolderStrList.RemoveHead();
		BOOL bRet = FALSE;
		bRet = CreateDirectory(tmpPath, NULL);
		if(bRet == FALSE)
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

