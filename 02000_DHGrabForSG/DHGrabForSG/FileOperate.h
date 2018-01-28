/************************************************************************/
/* Copyright (c) 2005, 中国大恒集团北京图像视觉技术分公司视觉技术部     
/* All rights reserved.													
/*																		
/* 文件名称： FileOperate.h												
/* 文件标识： 见配置管理计划书
/* 摘要： 对涉及文件操作的各种函数进行汇总, 提供文件及路径常用操作,
/*    保存及打开图像文件, 保存文本文件等功能
/*
/* 当前版本： 1.0
/* 作者： 江卓佳
/* 完成日期： 2005年10月10日
/************************************************************************/

#if !defined(FileOperate_h_)
#define FileOperate_h_

#include <afxtempl.h>//为了使用CList

//保存BMP图像的函数
BOOL SaveBMPFile(LPCSTR lpFileName, const BITMAPINFO *pBmpInfo, const BYTE *pImageBuffer);

// 获取本执行程序所在的路径
CString GetCurrentAppPath();

//判断路径是否存在
BOOL IsPathExist(const CString &strPath);

//判断文件是否存在
BOOL IsFileExist(const CString &strFileName);

//创建一个多层目录，如果存在就不创建
//(strPath  合法: "D:\\abc\\me" 或 "me\\you" 或 ""; 
//			不合法: "D:\\abc\\me\\" 或 "me\\you\\" 或 "\\")
BOOL CreateMultiLevelPath(const CString &strPath);

//写INI文件
BOOL MyWriteIniFile(const CString& strFilePath, const CString& strSession, const CString& str);

//写BMP文件
BOOL MyWriteBMPFile(const CString& strFileFullName, const PBITMAPINFO pBmpInfo, const BYTE *pImageBuffer);

//写将链表中的字符串顺序写入文件
BOOL MyWriteStringFile(const CString& strFilePath, const CString& strFileName, const CList<CString,CString&>& lCheckRltStrList);

//打开图像文件函数
BOOL OpenBMPFile(LPCTSTR lpFileName, BITMAPINFO *pBmpInfo, BYTE *pImageBuffer);

#endif // !define(FileOperate_h_)