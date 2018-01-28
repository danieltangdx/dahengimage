#ifndef GENERALFUNC_YANG_H__795A6DCF_2F01_4A64_B39E_6274456D4211__INCLUDED_
#define GENERALFUNC_YANG_H__795A6DCF_2F01_4A64_B39E_6274456D4211__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// #include <io.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <io.h>

#define STOPPER_DISPLAY_ROWS_APART	30

using namespace std;
#pragma once

class CGeneralFunc_Yang  
{
public:
	CGeneralFunc_Yang(){}
	virtual ~CGeneralFunc_Yang(){}

	static int DeletePrivateProfileString(char* lpAppName, char* lpKeyName, char* lpFileName)
	{
		ifstream   file;   
		file.open(lpFileName);   
		if(!file.good())
			return -1;
		
		file.seekg(0, ios::end);
		int len = file.tellg();
		file.seekg(0, ios::beg);

		char* content = new char[len+1];
		memset(content, 0, len+1);
		
		char buffer[256];
		char* chapp = new char[strlen(lpAppName)+3];
		char* chkey = new char[strlen(lpKeyName)+2];
		sprintf(chapp, "[%s]", lpAppName);
		sprintf(chkey, "%s=", lpKeyName);
		bool bfoundline = false;
		while (1)
		{
			file.getline(buffer, 255);
			if (buffer[0] == 0)
				break;
			if (bfoundline)
			{
				strcat(content, buffer);
				strcat(content, "\n");
				break;
			}

			if (strcmp(chapp, buffer) == 0)
			{
				strcat(content, buffer);
				strcat(content, "\n");

				while (1)
				{
					file.getline(buffer, 255);
					if (buffer[0] == 0)
						break;

					if (buffer[0]==91 && buffer[strlen(buffer)-1]==93)
					{
						strcat(content, buffer);
						strcat(content, "\n");
						break;
					}
					
					if (bfoundline)
					{
						strcat(content, buffer);
						strcat(content, "\n");
					}
					else
					{
						char* chkey2 = new char[strlen(lpKeyName)+2];
						memcpy(chkey2, buffer, strlen(lpKeyName)+1);
						chkey2[strlen(lpKeyName)+1] = 0;
						if (strcmp(chkey2, chkey) != 0)
						{
							strcat(content, buffer);
							strcat(content, "\n");
						}
						else
						{
							bfoundline = true;
						}
						delete [] chkey2;
					}
				}
			}
			else
			{
				strcat(content, buffer);
				strcat(content, "\n");
			}
		}
		
		delete [] chapp;
		delete [] chkey;

		file.close();
		DeleteFile(lpFileName);

		ofstream ofile;
		ofile.open(lpFileName);
		ofile.write(content, strlen(content)+1);
		ofile.close();
		
		return 0;
	}

	template <class number>
	static number min_array(number* nArray, int nLen)
	{
		number nMin = nArray[0];
		for (int n=1; n<nLen; ++n)
		{
			if (nArray[n] < nMin)
				nMin = nArray[n];
		}
		return nMin;
	}

	template <class number>
	static number max_array(number* nArray, int nLen)
	{
		number nMax = nArray[0];
		for (int n=1; n<nLen; ++n)
		{
			if (nArray[n] > nMax)
				nMax = nArray[n];
		}
		return nMax;
	}
	template <class number>
	static void max2_array(number* Array1, number* Array2, number* Array3, int nLen)
	{
		for (int n=0; n<nLen; ++n)
		{
			if (Array1[n] >= Array2[n])
				Array3[n] = Array1[n];
			else
				Array3[n] = Array2[n];
		}
	}
	
	template <class number>
	static int find_array(number* Array, number Data, int nLen, int &nIndex)
	{
		int N=0;
		bool bUpdata;
		bUpdata=FALSE;
		for (int n=0; n<nLen; ++n)
		{
			if (Array[n] == Data)
			{
				if (!bUpdata)
				{
					nIndex = n;
					bUpdata=TRUE;
				}			
				N++;
			}
		}
		return N;
	}

	static void GetCurrentAppDir(char* chDir, bool bSetCurDir = false)	//无最右侧斜杠
	{
		char path[MAX_PATH], drive[MAX_PATH], dir[MAX_PATH], filename[MAX_PATH], ext[MAX_PATH];
		GetModuleFileName(NULL, path, MAX_PATH);
		
		_splitpath(path, drive, dir, filename, ext);
		ZeroMemory(chDir, MAX_PATH);

		strcat(chDir, drive);
		strcat(chDir, dir);
		chDir[strlen(chDir)-1] = 0;

		if (bSetCurDir)
			SetCurrentDirectory(chDir);
	}

	static void GetCurrentAppDir(CString& strDir, bool bSetCurDir = false)	//无最右侧斜杠
	{
		char path[MAX_PATH], drive[MAX_PATH], dir[MAX_PATH], filename[MAX_PATH], ext[MAX_PATH];
		GetModuleFileName(NULL, path, MAX_PATH);
		
		_splitpath(path, drive, dir, filename, ext);
		strDir.Format("%s%s", drive, dir);
		strDir = strDir.Left(strDir.GetLength()-1);
		
		if (bSetCurDir)
			SetCurrentDirectory(strDir);
	}
	
	static void GetOriginalAppDir(char* chDir, bool bSetCurDir = false)	//无最右侧斜杠
	{
		char path[MAX_PATH], drive[MAX_PATH], dir[MAX_PATH], filename[MAX_PATH], ext[MAX_PATH];
		GetModuleFileName(NULL, path, MAX_PATH);
		
		_splitpath(path, drive, dir, filename, ext);
		ZeroMemory(chDir, MAX_PATH);
		
		strcat(chDir, drive);
		strcat(chDir, dir);
		chDir[strlen(chDir)-1] = 0;

		if (bSetCurDir)
			SetCurrentDirectory(chDir);

		strcpy(path, chDir);
		strcat(path, "\\Set.ini");

		char buf[MAX_PATH];
		GetPrivateProfileString("设置", "初始化路径", "NULL", buf, MAX_PATH, path);
		if (strcmp(buf, "NULL") != 0)
		{
			strcpy(chDir, buf);
			if (chDir[strlen(chDir)-1] == 92)
				chDir[strlen(chDir)-1] = 0;
		}
	}

	static void GetOriginalAppDir(CString& strDir, bool bSetCurDir = false)	//无最右侧斜杠
	{
		char path[MAX_PATH], drive[MAX_PATH], dir[MAX_PATH], filename[MAX_PATH], ext[MAX_PATH];
		GetModuleFileName(NULL, path, MAX_PATH);
		
		_splitpath(path, drive, dir, filename, ext);
		strDir.Format("%s%s", drive, dir);
		strDir = strDir.Left(strDir.GetLength()-1);
		
		if (bSetCurDir)
			SetCurrentDirectory(strDir);
		
		strcpy(path, strDir.GetBuffer(0));
		strcat(path, "\\Set.ini");
		
		char buf[MAX_PATH];
		GetPrivateProfileString("设置", "初始化路径", "NULL", buf, MAX_PATH, path);
		if (strcmp(buf, "NULL") != 0)	//有路径
		{
			strDir = buf;
			if (strDir.Right(1) == "\\")
				strDir = strDir.Left(strDir.GetLength()-1);
		}
	}

	static int IsNum(char* chText, int nLen)
	{
		bool bFindDot = false;
		for (int n=0; n<nLen; ++n)
		{
			if (chText[0]<48 || chText[0]>57)
			{
				if (n == 0)
				{
					if (chText[n]!=43 || chText[n]!=45)
					{
						return -1;
					}
				}
				else
				{
					if (chText[n]==46)
					{
						if (bFindDot)
							return -1;
						else
							bFindDot = true;
					}
					else
						return -1;	//不是数
				}
			}
		}
		if (bFindDot)
			return 1;	//小数
		else
			return 0;	//整数
	}

	static double GetPrivateProfileDouble(char* lpAppName, char* lpKeyName, double dDefault, char* lpFileName, int nPrecision = 0)
	{
		char buf[256], chdef[128];
		switch (nPrecision)
		{
		case 0:
			sprintf(chdef, "%f", dDefault);
			break;
		case 1:
			sprintf(chdef, "%.1f", dDefault);
			break;
		case 2:
			sprintf(chdef, "%.2f", dDefault);
			break;
		case 3:
			sprintf(chdef, "%.3f", dDefault);
			break;
		case 4:
			sprintf(chdef, "%.4f", dDefault);
			break;
		case 5:
			sprintf(chdef, "%.5f", dDefault);
			break;
		case 6:
			sprintf(chdef, "%.6f", dDefault);
			break;
		case 7:
			sprintf(chdef, "%.7f", dDefault);
			break;
		case 8:
			sprintf(chdef, "%.8f", dDefault);
			break;
		case 9:
			sprintf(chdef, "%.9f", dDefault);
			break;
		case 10:
			sprintf(chdef, "%.10f", dDefault);
			break;
		default:
			sprintf(chdef, "%f", dDefault);
			break;
		}
		GetPrivateProfileString(lpAppName, lpKeyName, chdef, buf, 256, lpFileName);
		return atof(buf);
	}

	static int WritePrivateProfileInt(int nNumber, char* lpAppName, char* lpKeyName, char* lpFileName)
	{
		char chNumber[16];
		sprintf(chNumber, "%d", nNumber);
		WritePrivateProfileString(lpAppName, lpKeyName, chNumber, lpFileName);
		return 1;
	}

	static int WritePrivateProfileDouble(double dNumber, char* lpAppName, char* lpKeyName, char* lpFileName, int nPrecision = 0)
	{
		char chNumber[32];
		switch (nPrecision)
		{
		case 0:
			sprintf(chNumber, "%f", dNumber);
			break;
		case 1:
			sprintf(chNumber, "%.1f", dNumber);
			break;
		case 2:
			sprintf(chNumber, "%.2f", dNumber);
			break;
		case 3:
			sprintf(chNumber, "%.3f", dNumber);
			break;
		case 4:
			sprintf(chNumber, "%.4f", dNumber);
			break;
		case 5:
			sprintf(chNumber, "%.5f", dNumber);
			break;
		case 6:
			sprintf(chNumber, "%.6f", dNumber);
			break;
		case 7:
			sprintf(chNumber, "%.7f", dNumber);
			break;
		case 8:
			sprintf(chNumber, "%.8f", dNumber);
			break;
		case 9:
			sprintf(chNumber, "%.9f", dNumber);
			break;
		case 10:
			sprintf(chNumber, "%.10f", dNumber);
			break;
		default:
			sprintf(chNumber, "%f", dNumber);
			break;
		}
		WritePrivateProfileString(lpAppName, lpKeyName, chNumber, lpFileName);
		return 1;
	}

	static int GetFileVersion(char* chFilePath, char* chVersion)
	{
		if (_access(chFilePath, 0) == -1)
			return -1;

		int cbInfoSize = GetFileVersionInfoSize(chFilePath, NULL);   
		LPVOID Version = new char[cbInfoSize];  
		UINT cbVerSize;
		GetFileVersionInfo(chFilePath, NULL, cbInfoSize, Version);
		int ma, mj, r, b;
		VS_FIXEDFILEINFO *VInfo;
		if(VerQueryValue(Version, "\\", ((void**)&VInfo), (PUINT)&cbVerSize))
		{
			ma= HIWORD(VInfo->dwFileVersionMS);
			mj= LOWORD(VInfo->dwFileVersionMS);
			r=	HIWORD(VInfo->dwFileVersionLS);
			b=  LOWORD(VInfo->dwFileVersionLS);
		}
		sprintf(chVersion, "%d.%d.%d.%d", ma, mj, r, b);
		delete []Version;

		return 1;
	}

	static int GetFileVersion(char* chFilePath, CString& strVersion)
	{
		if (_access(chFilePath, 0) == -1)
			return -1;
		
		int cbInfoSize = GetFileVersionInfoSize(chFilePath, NULL);   
		LPVOID Version = new char[cbInfoSize];  
		UINT cbVerSize;
		GetFileVersionInfo(chFilePath, NULL, cbInfoSize, Version);
		int ma, mj, r, b;
		VS_FIXEDFILEINFO *VInfo;
		if(VerQueryValue(Version, "\\", ((void**)&VInfo), (PUINT)&cbVerSize))
		{
			ma= HIWORD(VInfo->dwFileVersionMS);
			mj= LOWORD(VInfo->dwFileVersionMS);
			r=	HIWORD(VInfo->dwFileVersionLS);
			b=  LOWORD(VInfo->dwFileVersionLS);
		}
		strVersion.Format("%d.%d.%d.%d", ma, mj, r, b);
		delete []Version;
		
		return 1;
	}

	template <class cptype>
	static BOOL delete_yang(cptype pObject, int nNum)
	{
		if (nNum > 0)
		{
			if (nNum == 1)
				delete (cptype)pObject;
			else if(nNum > 1)
				delete [](cptype)pObject;
			pObject = NULL;
			return TRUE;
		}
		return FALSE;
	}

	static void AfxMessageBox_Yang(const char* chfmt, ...)
	{
		char chContent[256];
		va_list v_args;
		va_start(v_args, chfmt);
		vsprintf(chContent, chfmt, v_args);
		va_end(v_args);

		AfxMessageBox(chContent);
	}

	template <class Type>
	static void delete_yang(Type* pPointer, int nNum)
	{
		if (nNum == 1)
			delete (Type*)pPointer;
		else if (nNum > 1)
			delete[] (Type*)pPointer;
	}

	static void CreateMultiLevelPath(char* chPath)
	{
		int nLen = strlen(chPath);
		char* chTemp = new char[nLen+1];
		ZeroMemory(chTemp, nLen+1);
		for (int n=0; n<nLen; ++n)
		{
			chTemp[n] = chPath[n];
			if (chTemp[n] == '\\')
			{
				if(_access(chTemp, 0) == -1)
					CreateDirectory(chTemp, 0);
			}
		}
		if(_access(chTemp, 0) == -1)
			CreateDirectory(chTemp, 0);

		delete [] chTemp;
	}

	static int LoadString_256(HMODULE hDll, UINT uID, char* lpText)
	{
		return LoadString(hDll, uID, lpText, 256);
	}

	static void EnableProtectionFile(char* chFilePath)
	{
		if (_access(chFilePath, 0) == 0)
		{
			CFileStatus FStatus;
			CFile::GetStatus(chFilePath, FStatus);
			FStatus.m_attribute = FStatus.m_attribute|BYTE(0x01);
			CFile::SetStatus(chFilePath, FStatus);
		}
	}

	static void DisenableProtectionFile(char* chFilePath)
	{
		if (_access(chFilePath, 0) == 0)
		{
			CFileStatus FStatus;
			CFile::GetStatus(chFilePath, FStatus);
			FStatus.m_attribute = FStatus.m_attribute&BYTE(0x0E);
			CFile::SetStatus(chFilePath, FStatus);
		}
	}

	static void DeleteProtectionFile(char* chFilePath)
	{
		if (_access(chFilePath, 0) == 0)
		{
			DisenableProtectionFile(chFilePath);
			DeleteFile(chFilePath);
		}
	}

	static void RenameProtectionFile(char* chSourceFilePath, char* chDestFilePath)
	{
		if (_access(chSourceFilePath, 0) == 0)
		{
			DisenableProtectionFile(chSourceFilePath);
			DeleteProtectionFile(chDestFilePath);
			rename(chSourceFilePath, chDestFilePath);
		}
	}

	static void CutFile(char* chSourceFilePath, char* chDestFilePath, BOOL bFailIfExists=FALSE)
	{
		if (_access(chSourceFilePath, 0) == 0)
		{
			if(_access(chDestFilePath, 0)==0)
				DeleteFile(chDestFilePath);
			CopyFile(chSourceFilePath, chDestFilePath, bFailIfExists);
			DeleteFile(chSourceFilePath);
		}
	}

	static void CutProtectionFile(char* chSourceFilePath, char* chDestFilePath, BOOL bFailIfExists=FALSE)
	{
		if (_access(chSourceFilePath, 0) == 0)
		{
			if(_access(chDestFilePath, 0)==0)
				DeleteProtectionFile(chDestFilePath);
			DisenableProtectionFile(chSourceFilePath);
			CopyFile(chSourceFilePath, chDestFilePath, bFailIfExists);
			EnableProtectionFile(chDestFilePath);
			DeleteFile(chSourceFilePath);
		}
	}
};





#endif //GENERALFUNC_YANG_H__795A6DCF_2F01_4A64_B39E_6274456D4211__INCLUDED_