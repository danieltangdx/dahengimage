/************************************************************************/
/* Copyright (c) 2005, 中国大恒集团北京图像视觉技术分公司视觉技术部     
/* All rights reserved.													
/*																		
/* 文件名称： FileOperate.cpp											
/* 文件标识： 见配置管理计划书
/* 摘要： 对涉及文件操作的各种函数进行汇总, 提供文件及路径常用操作,
/*    保存及打开图像文件, 保存文本文件等功能
/*
/* 当前版本： 1.0
/* 作者： 江卓佳
/* 完成日期： 2005年10月10日
/************************************************************************/

#include "stdafx.h"
#include "FileOperate.h"

//保存BMP图像的函数
BOOL SaveBMPFile(LPCTSTR lpFileName, const BITMAPINFO *pBmpInfo, const BYTE *pImageBuffer)
{
	BOOL bRVal				= TRUE;
	DWORD dwBytesRead		= 0;
	DWORD dwSize			= 0;
	BITMAPFILEHEADER bfh	= {0};
	int nTable				= 0;
	DWORD dwImageSize		= 0;
	
	if (pBmpInfo->bmiHeader.biBitCount > 8) 
	{
		nTable = 0;
	}
	else
	{
		nTable = 256;
	}
	
	dwImageSize =  (pBmpInfo->bmiHeader.biWidth * pBmpInfo->bmiHeader.biHeight) * ((pBmpInfo->bmiHeader.biBitCount + 7) / 
		
		8);
	
	if (dwImageSize <= 0) 
	{
		bRVal = FALSE;
	}
	else{
		bfh.bfType		= (WORD)_T('M') << 8 | _T('B');
		bfh.bfOffBits	= sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nTable * sizeof(RGBQUAD);
		bfh.bfSize		= bfh.bfOffBits + dwImageSize;			
		
		HANDLE hFile = ::CreateFile(lpFileName,
			GENERIC_WRITE ,
			0,
			NULL,
			CREATE_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL
			);
		if (hFile == INVALID_HANDLE_VALUE) {
			bRVal = FALSE;
		}
		else{
			dwSize = sizeof(BITMAPFILEHEADER);
			::WriteFile(hFile, &bfh, dwSize, &dwBytesRead, NULL );
			
			dwSize = sizeof(BITMAPINFOHEADER) + nTable * sizeof(RGBQUAD);
			::WriteFile(hFile, pBmpInfo, dwSize, &dwBytesRead, NULL );
			
			dwSize = dwImageSize;
			WriteFile(hFile, pImageBuffer, dwSize, &dwBytesRead, NULL );
			
			CloseHandle(hFile);
		}
	}
	
	return bRVal;
}


//判断路径是否存在
BOOL IsPathExist(const CString &strPath)
{
	WIN32_FIND_DATA fd; 
	HANDLE hFind = FindFirstFile(strPath, &fd); 
	if ((hFind != INVALID_HANDLE_VALUE) && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
	{ 
		FindClose(hFind);
		return TRUE; 
	} 
	else
	{
		FindClose(hFind); 
		return FALSE;
	}
}

//判断文件是否存在
BOOL IsFileExist(const CString &strFileName)
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
BOOL CreateMultiLevelPath(const CString &strPath)
{
	CString tmpPath = strPath;
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
		bPathExist = IsPathExist(tmpPath);
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

//写INI文件
BOOL MyWriteIniFile(const CString& strFilePath, const CString& strSession, const CString& str)
{
	//构造INI文件目录和ini文件名
	CString strIniPath, strIniFileName, strTime;
	CTime curTime = CTime::GetCurrentTime();
	
	strIniPath.Format(_T("%s%s%4d%s%02d%s"), strFilePath, _T("\\Ini\\"), curTime.GetYear(), _T("年"),curTime.GetMonth(), _T("月"));
	strIniFileName.Format(_T("%s%s%4d%02d%02d%s"), strIniPath, _T("\\"), curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), _T(".ini"));
	strTime.Format(_T("%02d%s%02d%s%02d"), curTime.GetHour(), _T(":"), curTime.GetMinute(), ":", curTime.GetSecond());
	
	//如果目录不存在就创建目录
	BOOL bRet = FALSE;
	bRet = CreateMultiLevelPath(strIniPath);
	if(bRet == FALSE)
	{
		return FALSE;
	}

	//写ini文件
	bRet = WritePrivateProfileString(strSession, strTime, str, strIniFileName);
	return bRet;
}

//写BMP文件
BOOL MyWriteBMPFile(const CString& strFileFullName, const PBITMAPINFO pBmpInfo, const BYTE *pImageBuffer)
{
	if(pBmpInfo == NULL || pImageBuffer == NULL)
	{
		return FALSE;
	}

	CString strFilePath;
	int nPos = strFileFullName.ReverseFind('\\');
	if(nPos == -1)
	{
		strFilePath = "";
	}
	else
	{
		strFilePath = strFileFullName.Left(nPos);
	}

	//如果目录不存在就创建目录
	BOOL bRet = FALSE;
	bRet = CreateMultiLevelPath(strFilePath);
	if(bRet == FALSE)
	{
		return FALSE;
	}
	
	//写BMP文件
	bRet = SaveBMPFile(strFileFullName, pBmpInfo, pImageBuffer);
	return bRet;	
	
/*	//构造INI文件目录和ini文件名
	CString strBMPPath, strBMPFileName;
	CTime curTime = CTime::GetCurrentTime();
	
	strBMPPath.Format("%s\\ErrorImage", strAppPath);
	strBMPFileName.Format("%s\\%04d_%02d_%02d_%02d_%02d_%02d.bmp",
		strBMPPath, 
		curTime.GetYear(), curTime.GetMonth(), curTime.GetDay(), 
		curTime.GetHour(), curTime.GetMinute(), curTime.GetSecond()
		);
*/
}

//写将链表中的字符串顺序写入文件
BOOL MyWriteStringFile(const CString& strFilePath, const CString& strFileName, const CList<CString,CString&>& lCheckRltStrList)
{ 
	//////////////////////////////////////////////////////////////////////////
	//
	BOOL bRet = FALSE;
	bRet = CreateMultiLevelPath(strFilePath);
	if(bRet == FALSE)
	{
		return FALSE;
	}

	CString strFileFullName;
	strFileFullName.Format(_T("%s\\%s"), strFilePath, strFileName);
	
	CString cstr ;
	POSITION pos = lCheckRltStrList.GetHeadPosition();
	int nSumL = lCheckRltStrList.GetCount() ;
	for (int i=0; i<nSumL ; i++)
	{
		cstr.Format( _T("NO.%d"), i) ;
		bRet = WritePrivateProfileString(_T("Start"),cstr,(LPCTSTR) lCheckRltStrList.GetNext(pos), strFileFullName);
		if(bRet == FALSE)
		{
			return FALSE;
		}
	}

	return TRUE;
	//
	//////////////////////////////////////////////////////////////////////////

	/*
	CTime ctm = CTime::GetCurrentTime();
	CString strName = ctm.Format( "D:\\TXT" );
	CreateDirectory(strName, NULL) ;
	strName = ctm.Format( "D:\\TXT\\%b-%d" );
	CreateDirectory(strName, NULL) ;
	
	strName = ctm.Format( "D:\\TXT\\%b-%d\\T-%H-%M-%S.TXT" );*/
}

//////////////////////////////////////////////////////////////////////////
//打开图像文件函数
#define WIDTH_BYTES(bits) (((bits) + 31) / 32 * 4)
BOOL OpenBMPFile(LPCTSTR lpFileName, BITMAPINFO *pBmpInfo, BYTE *pImageBuffer)
{
	BOOL bRVal = TRUE;
	DWORD dwBytesRead = 0;
	DWORD dwSize = 0;
	BITMAPFILEHEADER bfh;
	
	HANDLE hFile = ::CreateFile(lpFileName,
								GENERIC_READ,
								FILE_SHARE_READ,
								NULL,
								OPEN_EXISTING,
								FILE_ATTRIBUTE_NORMAL,
								NULL
								);
	if (hFile == INVALID_HANDLE_VALUE) {
		bRVal = FALSE;
	}
	else
	{
		dwSize = sizeof(BITMAPFILEHEADER);
		::ReadFile(hFile, &bfh, dwSize, &dwBytesRead, NULL );
		if (bfh.bfType != ((WORD)'M' << 8 | 'B')) 
		{
			bRVal = FALSE;
		}
		else
		{
			dwSize = sizeof(BITMAPINFOHEADER);
			::ReadFile(hFile, pBmpInfo, dwSize, &dwBytesRead, NULL);
			if(bfh.bfOffBits - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER) > 0)//if (pBmpInfo->bmiHeader.biBitCount <= 8)//[2006-11-02]		
			{
				dwSize = bfh.bfOffBits - sizeof(BITMAPFILEHEADER) - sizeof(BITMAPINFOHEADER);
				bRVal = ReadFile(hFile,
					(char *)pBmpInfo + sizeof(BITMAPINFOHEADER),
					dwSize,
					&dwBytesRead,
					NULL
					);
			}
			
			dwSize = WIDTH_BYTES(pBmpInfo->bmiHeader.biWidth * pBmpInfo->bmiHeader.biBitCount) *
				pBmpInfo->bmiHeader.biHeight;
			
			bRVal = ReadFile(hFile, pImageBuffer, dwSize, &dwBytesRead, NULL);
		}
		
		CloseHandle(hFile);
	}
	
	return bRVal;
}
