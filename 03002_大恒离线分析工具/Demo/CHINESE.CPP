// CHINESE.cpp: implementation of the Chinese Language Pack.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CHINESE.H"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

void GetText(char* chText, int nDoWhat, ...)
{
	if (nDoWhat == 1)
	{
		sprintf(chText, _T("确定"));
	}
	else if (nDoWhat == 2)
	{
		int nCameraIndex;
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		nCameraIndex = va_arg(arg_ptr, int);
		va_end(arg_ptr);

		sprintf(chText, _T("    第%d路    "), nCameraIndex);
	}
	else if (nDoWhat == 3)
	{
		char chModelName[256];
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		strcpy(chModelName, va_arg(arg_ptr, char*));
		va_end(arg_ptr);

		sprintf(chText, _T("创建初始模板[%s]"), chModelName);
	}
	else if (nDoWhat == 4)
	{
		char chFirstModelName[256], chSecondModelName[256];
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		strcpy(chFirstModelName, va_arg(arg_ptr, char*));
		strcpy(chSecondModelName, va_arg(arg_ptr, char*));
		va_end(arg_ptr);

		sprintf(chText, _T("找不到初始模板[%s]，切换为[%s]"), chFirstModelName, chSecondModelName);
	}
	else if (nDoWhat == 5)
	{
		char chModelName[256];
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		strcpy(chModelName, va_arg(arg_ptr, char*));
		va_end(arg_ptr);

		sprintf(chText, _T("读取初始模板[%s]"), chModelName);
	}
	else if (nDoWhat == 6)
	{
		char chModelName[256];
		int nCameraIndex;
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		strcpy(chModelName, va_arg(arg_ptr, char*));
		nCameraIndex = va_arg(arg_ptr, int);
		va_end(arg_ptr);

		sprintf(chText, _T("保存模板[%s]第%d路成功"), chModelName, nCameraIndex);
	}
	else if (nDoWhat == 7)
	{
		char chModelName[256];
		int nCameraIndex;
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		strcpy(chModelName, va_arg(arg_ptr, char*));
		nCameraIndex = va_arg(arg_ptr, int);
		va_end(arg_ptr);

		sprintf(chText, _T("保存模板[%s]第%d路失败"), chModelName, nCameraIndex);
	}
	else if (nDoWhat == 8)
	{
		char chModelName[256];
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		strcpy(chModelName, va_arg(arg_ptr, char*));
		va_end(arg_ptr);

		sprintf(chText, _T("保存模板[%s]成功"), chModelName);
	}
	else if (nDoWhat == 9)
	{
		char chModelName[256];
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		strcpy(chModelName, va_arg(arg_ptr, char*));
		va_end(arg_ptr);

		sprintf(chText, _T("保存模板[%s]成功"), chModelName);
	}
	else if (nDoWhat == 10)
	{
		char chFailIndex[256];
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		strcpy(chFailIndex, va_arg(arg_ptr, char*));
		va_end(arg_ptr);

		sprintf(chText, _T("保存模板..第%s路失败"), chFailIndex);
	}
	else if (nDoWhat == 11)
	{
		char chModelName[256];
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		strcpy(chModelName, va_arg(arg_ptr, char*));
		va_end(arg_ptr);

		sprintf(chText, _T("系统加载模板[%s]成功"), chModelName);
	}
	else if (nDoWhat == 12)
	{
		char chModelName[256];
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		strcpy(chModelName, va_arg(arg_ptr, char*));
		va_end(arg_ptr);

		sprintf(chText, _T("Inspection Loaded Model %s Failure"), chModelName);
	}
	else if (nDoWhat == 13)
	{
		char chModelName[256];
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		strcpy(chModelName, va_arg(arg_ptr, char*));
		va_end(arg_ptr);

		sprintf(chText, _T("是否保存当前模板[%s]？"), chModelName);
	}
	else if (nDoWhat == 14)
	{
		char chModelName[256];
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		strcpy(chModelName, va_arg(arg_ptr, char*));
		va_end(arg_ptr);

		sprintf(chText, _T("创建新模板[%s]成功"), chModelName);
	}
	else if (nDoWhat == 15)
	{
		char chModelName[256];
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		strcpy(chModelName, va_arg(arg_ptr, char*));
		va_end(arg_ptr);

		sprintf(chText, _T("创建新模板[%s]失败"), chModelName);
	}
	else if (nDoWhat == 16)
	{
		char chModelName[256];
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		strcpy(chModelName, va_arg(arg_ptr, char*));
		va_end(arg_ptr);

		sprintf(chText, _T("是否设置模板[%s]为系统默认模板？"), chModelName);
	}
	else if (nDoWhat == 17)
	{
		char chModelName[256];
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		strcpy(chModelName, va_arg(arg_ptr, char*));
		va_end(arg_ptr);

		sprintf(chText, _T("该模板[%s]已设为默认"), chModelName);
	}
	else if (nDoWhat == 18)
	{
		int nCameraIndex;
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		nCameraIndex = va_arg(arg_ptr, int);
		va_end(arg_ptr);

		sprintf(chText, _T("第%d路切换图像路径"), nCameraIndex);
	}
	else if (nDoWhat == 19)
	{
		int nCameraIndex;
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		nCameraIndex = va_arg(arg_ptr, int);
		va_end(arg_ptr);

		sprintf(chText, _T("第%d路测试检测效果"), nCameraIndex);
	}
	else if (nDoWhat == 20)
	{
		char chVersion[256];
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		strcpy(chVersion, va_arg(arg_ptr, char*));
		va_end(arg_ptr);

		sprintf(chText, _T("DEMO版本：%s"),  chVersion);
	}
	else if (nDoWhat == 21)
	{
		sprintf(chText, _T("关于DEMO.exe"));
	}
	else if (nDoWhat == 22)
	{
		sprintf(chText, _T("北京大恒图像视觉有限公司 版权所有(c) 2016."));
	}
	else if (nDoWhat == 23)
	{
		char chFirstModelName[256], chSecondModelName[256];
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		strcpy(chFirstModelName, va_arg(arg_ptr, char*));
		strcpy(chSecondModelName, va_arg(arg_ptr, char*));
		va_end(arg_ptr);

		sprintf(chText, _T("从[%s]切换为[%s]"), chFirstModelName, chSecondModelName);
	}
	else if (nDoWhat == 24)
	{
		char chFirstModelName[256], chSecondModelName[256];
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		strcpy(chFirstModelName, va_arg(arg_ptr, char*));
		strcpy(chSecondModelName, va_arg(arg_ptr, char*));
		va_end(arg_ptr);

		sprintf(chText, _T("模板[%s]重命名为[%s]"), chFirstModelName, chSecondModelName);
	}
	else if (nDoWhat == 25)
	{
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		CTime cTime = va_arg(arg_ptr, CTime);
		va_end(arg_ptr);

		sprintf(chText, _T("20%s"), cTime.Format("%y-%m-%d"));
	}
	else if (nDoWhat == 26)
	{
		char chModelName[256];
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		strcpy(chModelName, va_arg(arg_ptr, char*));
		va_end(arg_ptr);

		sprintf(chText, _T("是否加载模板[%s]？"), chModelName);
	}
	else if (nDoWhat == 27)
	{
		char chDllName[256];
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		strcpy(chDllName, va_arg(arg_ptr, char*));
		va_end(arg_ptr);

		sprintf(chText, _T("找不到检测对象：%s"), chDllName);
	}
	else if (nDoWhat == 28)
	{
		int nError;
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		nError = va_arg(arg_ptr, int);
		va_end(arg_ptr);

		sprintf(chText, _T("加载DLL文件失败\r\n错误代码：%d"), nError);
	}
	else if (nDoWhat == 29)
	{
		char chModelName[256];
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		sprintf(chModelName, va_arg(arg_ptr, char*));
		va_end(arg_ptr);

		sprintf(chText, _T("删除模板[%s]成功"), chModelName);
	}
	else if (nDoWhat == 30)
	{
		char chModelName[256];
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		sprintf(chModelName, va_arg(arg_ptr, char*));
		va_end(arg_ptr);
		
		sprintf(chText, _T("是否删除模板[%s]？"), chModelName);
	}
	else if (nDoWhat == 31)
	{
		int nProbIndex;
		va_list arg_ptr;
		va_start(arg_ptr, nDoWhat);
		nProbIndex = va_arg(arg_ptr, int);
		va_end(arg_ptr);
		
		sprintf(chText, _T("加载第%d个动态库失败，请重新设置！"), nProbIndex);
	}
	else
	{
		ZeroMemory(chText, 256);
	}
	return;
}