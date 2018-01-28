#include "stdafx.h"
#include "ExFn.h"

/*************************************<>************************************[9/6/2017 ConanSteve]*/


/*************************************<>************************************[9/6/2017 ConanSteve]*/
CExFn::CExFn()
{
}


CExFn::~CExFn()
{
}

BOOL CExFn::ShowInTaskbar(HWND hWnd, BOOL bShow)
{
	HRESULT hr=0;
	ITaskbarList* pTaskbarList=NULL;
	hr = CoCreateInstance(CLSID_TaskbarList, NULL, CLSCTX_INPROC_SERVER,
		IID_ITaskbarList, (void**)&pTaskbarList);

	if (SUCCEEDED(hr))
	{
		pTaskbarList->HrInit();
		if (bShow)
			pTaskbarList->AddTab(hWnd);
		else
			pTaskbarList->DeleteTab(hWnd);

		pTaskbarList->Release();
		return TRUE;
	}

	return FALSE;
}

BOOL CExFn::SetWindowPosTop(HWND hwnd)
{
	return ::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

CString CExFn::GetCurrentAppPath()
{
	CString sRet = _T("");
	GetModuleFileName(NULL, sRet.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	sRet.ReleaseBuffer();
	int iPos = sRet.ReverseFind('\\');
	sRet = sRet.Left(iPos);
	return sRet;
}
bool CExFn::FindSpecificFiles(LPCTSTR lpszPathFolder,
	LPCTSTR lpszFilter,
	vector<CString>& vec_sPathFile,
	vector<CString>& vec_sFileName,
	bool bSearchSub,
	int iType,
	CString* pError)
{
	if (!PathFileExists(lpszPathFolder))
	{
		*pError = _T("Path does not exist!");
		return false;
	}
	vec_sPathFile.clear();
	vec_sFileName.clear();
	CFileFind tempFind;
	CString sTempFileFind = (CString)lpszPathFolder + _T("\\")+lpszFilter;
	BOOL IsFinded = tempFind.FindFile(sTempFileFind);
	bool bAdd = false;
	while (IsFinded)
	{

		IsFinded = tempFind.FindNextFile();
		bAdd = false;
		if (!tempFind.IsDots())
		{
			CString sFoundFileName = _T("") ;
			sFoundFileName = tempFind.GetFilePath();		
			if (tempFind.IsDirectory())
			{
				if ((iType & 2) == 2)
					bAdd = true;
				else
					bAdd = false;
			}
			else if ((iType & 1) == 1)//**[1/5/2018 ConanSteve]**:	文件 
				bAdd = true;
			if (bAdd)
			{
				vec_sPathFile.push_back(sFoundFileName);
				vec_sFileName.push_back(tempFind.GetFileName());
			}
			if (tempFind.IsDirectory())
			{
				if(bSearchSub)
					FindSpecificFiles(sFoundFileName, lpszFilter, vec_sPathFile, vec_sFileName, bSearchSub, iType, pError);
			}
		}
	}
	tempFind.Close();
	return true;
}
BOOL CExFn::DeleteDirectory(LPCTSTR lpszPath, LPCTSTR lpszFileType, bool bIncludeFolders)
{
	CFileFind tempFind;
	CString sTempFileFind = (CString)lpszPath+ _T("\\")+lpszFileType;
	BOOL IsFinded = tempFind.FindFile(sTempFileFind);
	while (IsFinded)
	{

		IsFinded = tempFind.FindNextFile();

		if (!tempFind.IsDots())
		{
			CString sFoundFileName = _T("") ;
			sFoundFileName = tempFind.GetFilePath();

			if (tempFind.IsDirectory())
			{
				DeleteDirectory(sFoundFileName, lpszFileType, bIncludeFolders);
			}
			else
			{
				DeleteFile(sFoundFileName);
			}
		}
	}
	tempFind.Close();
	if (bIncludeFolders)
	{
		if (!RemoveDirectory(lpszPath))
		{
			return FALSE;
		}
	}
	return TRUE;
}

bool CExFn::CreateMultiDirectory(LPCTSTR lpszPath)
{
	CString sPathCur = _T("") , sPathOfEntire= (CString)lpszPath +_T("\\");
	int iPos= -1 ;
	while (true)
	{
		++iPos;
		iPos = sPathOfEntire.Find(_T('\\'), iPos);
		if (iPos < 0)
			break;
		sPathCur = sPathOfEntire.Left(iPos);
		if (!PathFileExists(sPathCur))
		{
			BOOL bRet = CreateDirectory(sPathCur, NULL);
			if (FALSE == bRet)
				//AfxMessageBox(tr("Create Directory Failed!"));
				return FALSE;
		}
	}
	return TRUE;
}

BOOL CExFn::CopyChildFiles(LPCTSTR lpszExistingFileName, LPCTSTR lpszNewFileName, BOOL bFailIfExists, CString* psErrorDesc)
{
	if (FALSE == PathFileExists(lpszExistingFileName))
		return FALSE;
	if (FALSE == PathFileExists(lpszNewFileName))
		CreateMultiDirectory(lpszNewFileName);
	vector<CString> vec_sPathFileOld, vec_sFileNameOld;
	CString sFilePathNew = _T("") ;
	FindSpecificFiles(lpszExistingFileName, _T("*.*"), vec_sPathFileOld, vec_sFileNameOld, false, 3);
	//FindSpecificFiles(_T("s:\\My_Docs1"), _T("*.*"), vec_sPathFileOld, vec_sFileNameOld, false, 3);
	BOOL bRet = TRUE;
	int iRet = 1;
	for (int i = 0; i < vec_sFileNameOld.size(); ++i)
	{
		sFilePathNew = (CString)lpszNewFileName + _T("\\") + vec_sFileNameOld[i];
		bRet = CopyFile(vec_sPathFileOld[i], sFilePathNew, bFailIfExists);
		//bRet = CopyFile(vec_sPathFileOld[i], _T("s:\\My_Docs2"), bFailIfExists);
// 		{
// 			SHFILEOPSTRUCT   sfo;
// 
// 			sfo.hwnd = NULL;
// 			sfo.wFunc = FO_COPY;
// // 			sfo.pFrom = "s:\\My_Docs1\\1.txt";
// // 			sfo.pTo = "s:\\My_Docs2";
//  			sfo.pFrom = vec_sPathFileOld[i].GetBuffer();
//  			sfo.pTo = lpszNewFileName;
// 			sfo.fFlags = FOF_SILENT | FOF_NOCONFIRMATION | FOF_NOCONFIRMMKDIR;
// 			iRet = SHFileOperation(&sfo);
//		}
		if (FALSE == bRet||FALSE == iRet)
		{
			if(NULL!=psErrorDesc)
				*psErrorDesc = CExFn::GetLastErrorMsg();
			//return FALSE;
		}
	}
	return TRUE;
}


/*判断一个路径是否是已存在的目录*/
BOOL CExFn::IsDirectory(LPCTSTR pstrPath)
{
	if (NULL == pstrPath)
	{
		return FALSE;
	}

	/*去除路径末尾的反斜杠*/
	CString strPath = pstrPath;
	if (strPath.Right(1) == _T('\\'))
	{
		strPath.Delete(strPath.GetLength() - 1);
	}

	CFileFind finder;
	BOOL bRet = finder.FindFile(strPath);
	if (!bRet)
	{
		return FALSE;
	}

	/*判断该路径是否是目录*/
	finder.FindNextFile();
	bRet = finder.IsDirectory();
	finder.Close();
	return bRet;
}

/*复制目录中的所有内容*/
BOOL CExFn::CopyFolder(LPCTSTR pstrSrcFolder, LPCTSTR pstrDstFolder)
{
	if ((NULL == pstrSrcFolder) || (NULL == pstrSrcFolder))
	{
		return FALSE;
	}

	/*检查源目录是否是合法目录*/
	if (!IsDirectory(pstrSrcFolder))
	{
		return FALSE;
	}

	/*把文件名称转换为CString类型，并确认目录的路径末尾为反斜杠*/
	CString strSrcFolder(pstrSrcFolder);
	if (strSrcFolder.Right(1) != _T('\\'))
	{
		strSrcFolder += _T("\\");
	}
	CString strDstFolder(pstrDstFolder);
	if (strDstFolder.Right(1) != _T("\\"))
	{
		strDstFolder += _T("\\");
	}

	/*如果是目录自身复制，直接返回复制成功*/
	if (0 == _tcscmp(strSrcFolder, strDstFolder))
	{
		return TRUE;
	}

	/*如果目的目录不存在,则创建目录*/
	if (!IsDirectory(strDstFolder))
	{
		if (!CreateDirectory(strDstFolder, NULL))
		{
			/*创建目的目录失败*/
			return FALSE;
		}
	}

	/*创建源目录中查找文件的通配符*/
	CString strWildcard(strSrcFolder);
	strWildcard += _T("*.*");

	/*打开文件查找，查看源目录中是否存在匹配的文件*/
	/*调用FindFile后，必须调用FindNextFile才能获得查找文件的信息*/
	CFileFind finder;
	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		/*查找下一个文件*/
		bWorking = finder.FindNextFile();

		/*跳过当前目录“.”和上一级目录“..”*/
		if (finder.IsDots())
		{
			continue;
		}

		/*得到当前要复制的源文件的路径*/
		CString strSrcFile = finder.GetFilePath();

		/*得到要复制的目标文件的路径*/
		CString strDstFile(strDstFolder);
		strDstFile += finder.GetFileName();

		/*判断当前文件是否是目录,*/
		/*如果是目录，递归调用复制目录,*/
		/*否则，直接复制文件*/
		if (finder.IsDirectory())
		{
			if (!CopyFolder(strSrcFile, strDstFile))
			{
				finder.Close();
				return FALSE;
			}
		}
		else
		{
			if (!CopyFile(strSrcFile, strDstFile, FALSE))
			{
				finder.Close();
				return FALSE;
			}
		}

	} /*while (bWorking)*/

	  /*关闭文件查找*/
	finder.Close();

	return TRUE;

}

bool CExFn::GetPrivateProfileBool(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bDefaultValue, LPCTSTR lpszFileName, LPCTSTR lpszAnnotation)
{
	return TRUE == GetPrivateProfileInt(lpszAppName, lpszKeyName, bDefaultValue ? 1 : 0, lpszFileName, lpszAnnotation);
}

int CExFn::GetPrivateProfileInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iDefaultValue, LPCTSTR lpszFileName, LPCTSTR lpszAnnotation)
{
	return ::GetPrivateProfileInt(lpszAppName, lpszKeyName, iDefaultValue, lpszFileName);
}

CString& CExFn::GetPrivateProfileString(LPCTSTR lpAppName, LPCTSTR lpKeyName, CString& sReturnString, LPCTSTR lpFileName, LPCTSTR lpDefault,LPCTSTR lpszAnnotation)
{
	CString sDefaultValue = _T("") ;
	if (NULL != lpDefault)
		sDefaultValue = lpDefault;
	::GetPrivateProfileString(lpAppName, lpKeyName, sDefaultValue.GetBuffer(), sReturnString.GetBuffer(MAX_BUF_LEN), MAX_BUF_LEN, lpFileName);
	sReturnString.ReleaseBuffer();
	if (_T("") == sReturnString && NULL != lpDefault)
	{
		sReturnString = lpDefault;
	}
	return sReturnString;
}

bool CExFn::GetPrivateProfileBoolEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bDefaultValue, LPCTSTR lpszFileName,LPCTSTR lpszAnnotation)
{
	return TRUE == GetPrivateProfileIntEx(lpszAppName, lpszKeyName, bDefaultValue?1:0, lpszFileName, lpszAnnotation);
}

int CExFn::GetPrivateProfileIntEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iDefaultValue , LPCTSTR lpszFileName,  LPCTSTR lpszAnnotation)
{
	int iRet = 0;
	iRet = ::GetPrivateProfileInt(lpszAppName, lpszKeyName, iDefaultValue, lpszFileName);
	WritePrivateProfileInt(lpszAppName, lpszKeyName, iRet, lpszFileName, lpszAnnotation);
	return iRet;
}

CString& CExFn::GetPrivateProfileStringEx(LPCTSTR lpAppName,LPCTSTR lpKeyName,CString& sReturnString,LPCTSTR lpFileName, LPCTSTR lpDefault, LPCTSTR lpszAnnotation)
{
	GetPrivateProfileString(lpAppName, lpKeyName, sReturnString, lpFileName, lpDefault, lpszAnnotation);
	WritePrivateProfileString(lpAppName, lpKeyName, sReturnString, lpFileName, lpszAnnotation);
	return sReturnString;
}

bool CExFn::WritePrivateProfileBool(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bKeyValue, LPCTSTR lpszFileName, LPCTSTR lpszAnnotation)
{
	return CExFn::WritePrivateProfileInt(lpszAppName,lpszKeyName,bKeyValue?1:0,lpszFileName, lpszAnnotation);
}

bool CExFn::WritePrivateProfileInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iKeyValue, LPCTSTR lpszFileName,LPCTSTR lpszAnnotation)
{
	CString sValue= _T("") ;
	sValue.Format(_T("%d"), iKeyValue); 
	return CExFn::WritePrivateProfileString(lpszAppName, lpszKeyName, sValue, lpszFileName, lpszAnnotation);
}
bool CExFn::WritePrivateProfileString(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszFileName, LPCTSTR lpszAnnotation)
{
	CString sValueName = lpszValue;
	int iOffset = 0;
	TCHAR buffVal[1024];
	TCHAR buffAll[1024];
	TCHAR buffAnnotation[1024];
	memset(buffVal, 0, sizeof(buffVal));
	memset(buffAll, 0, sizeof(buffAll));
	memset(buffAnnotation, 0, sizeof(buffAnnotation));
	::GetPrivateProfileString(lpszAppName, lpszKeyName, NULL, buffAll, 1024, lpszFileName);
	TCHAR* lpTemp = StrStr(buffAll, _T(" //"));
	if (NULL == lpTemp)
	{
		StrCat(buffAll, _T(" //"));
		lpTemp = StrStr(buffAll, _T(" //"));
	}
	iOffset = (lpTemp - buffAll + 3);
	memcpy_s(buffAnnotation, sizeof(buffAnnotation), buffAll + iOffset, sizeof(buffAnnotation));
	if (NULL != lpszAnnotation)
	{
		sValueName += (CString)_T("        //") + lpszAnnotation;
	}
	else if(0<_tcslen(buffAnnotation))
	{
		sValueName += (CString)_T("        //") + buffAnnotation;
	}
	return TRUE == ::WritePrivateProfileString(lpszAppName, lpszKeyName, sValueName, lpszFileName);
}
CString CExFn::Int2CS(int iVal)
{
	CString sRet= _T("") ;
	sRet.Format(_T("%d"), iVal);
	return sRet;
}
CString CExFn::GetLastErrorMsg()
{
	CString sErrorMsg= _T("") ;
	LPVOID lpMsgBuf =NULL;
	DWORD dwErrorCode = GetLastError();
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErrorCode,
		0, // Default language
		(LPTSTR)&lpMsgBuf,
		0,
		NULL
	);
	sErrorMsg.Format(_T("ErrorCode:%d  %s"),dwErrorCode, lpMsgBuf);
	LocalFree(lpMsgBuf);
	return sErrorMsg;
}

CRect CExFn::AutoResizeRect(int iWidth, int iHeight, CRect rect)
{
	int iActualHeight = 0, iActualWidth = 0;
	if (iWidth* rect.Height() > iHeight*rect.Width())//**[12/4/2017 ConanSteve]**:  图像实际大小宽度偏长，以高度作为压缩标准
	{
		iActualWidth = rect.Width();
		iActualHeight = iActualWidth * iHeight / iWidth;
	}
	else
	{
		iActualHeight = rect.Height();
		iActualWidth = iActualHeight * iWidth / iHeight;
	}
	CRect rectRet;
	rectRet.left = rect.left + (rect.Width() - iActualWidth) / 2;
	rectRet.right = rectRet.left + iActualWidth;
	rectRet.top = rect.top + (rect.Height() - iActualHeight) / 2;
	rectRet.bottom = rectRet.top + iActualHeight;
	return rectRet;
}

vector<CRect> CExFn::AutoLayoutRect(CRect rectParent, int iCntHorizon, int iCntVertical, int iGapHorizon, int iGapVertical, int iChildNum, int iType, CRect* pRect)
{
	//**[1/25/2018 ConanSteve]**:	rectPaent:父窗口大小 
	//**[1/25/2018 ConanSteve]**:	iXCnt：基准轴（自变量）的最大个数
	//**[1/25/2018 ConanSteve]**:	iType:0,以x轴为基准，1以y轴为基准 
	vector<CRect> vec_rect;
	int iXCnt = 0, iYCnt = 0;
	int iXGap = 0, iYGap = 0;
	int iXParentSize = 0, iYParentSize = 0;//**[1/25/2018 ConanSteve]**:	自变量与应变量的父尺寸
	if (0 == iType)
	{
		iXParentSize = rectParent.Width();
		iYParentSize = rectParent.Height();
		iXGap = iGapHorizon;
		iYGap = iGapVertical;
		iXCnt = iCntHorizon<iChildNum?iCntHorizon:iChildNum;
	}
	else
	{
		iYParentSize = rectParent.Width();
		iXParentSize = rectParent.Height();
		iXGap = iGapVertical;
		iYGap = iGapHorizon;
		iXCnt = iCntVertical<iChildNum?iCntVertical:iChildNum;
	}
	if (0 == iXCnt)
		return vec_rect;
	iYCnt = (iChildNum + iXCnt - 1) / iXCnt;//**[1/25/2018 ConanSteve]**:	应变量的大小
	int iXChildSize = 0, iYChildSize=0; 
	iXChildSize = (iXParentSize - iXGap * (iXCnt + 1)) / iXCnt;//**[12/27/2017 ConanSteve]**:  水平方向每个小窗口的宽度
	iYChildSize = (iYParentSize - iYGap * (iYCnt + 1)) / iYCnt;//**[12/27/2017 ConanSteve]**:  垂直方向每个小窗口的高度
	for (int i = 0; i < iChildNum; ++i)
	{
		CRect rectCurChild;
		int iCurRectPos_X = i % iXCnt;//**[12/27/2017 ConanSteve]**:  当前窗口在水平方向的个数坐标
		int iCurRectPos_Y = i / iXCnt;//**[12/27/2017 ConanSteve]**:  当前窗口在垂直方向的个数坐标
		if(0 == iType)
		{
			rectCurChild.left = rectParent.left + iCurRectPos_X * iXChildSize + (iCurRectPos_X + 1)*iXGap;
			rectCurChild.top = rectParent.top + iCurRectPos_Y * iYChildSize + (iCurRectPos_Y + 1)*iYGap;
			rectCurChild.right = rectParent.left + (iCurRectPos_X + 1)*(iXChildSize + iXGap);
			rectCurChild.bottom = rectParent.top + (iCurRectPos_Y + 1)*(iYChildSize + iYGap);
		}
		else if (1 == iType)
		{
			rectCurChild.top = rectParent.top + iCurRectPos_X * iXChildSize + (iCurRectPos_X + 1)*iXGap;
			rectCurChild.left = rectParent.left + iCurRectPos_Y * iYChildSize + (iCurRectPos_Y + 1)*iYGap;
			rectCurChild.bottom = rectParent.top + (iCurRectPos_X + 1)*(iXChildSize + iXGap);
			rectCurChild.right = rectParent.left + (iCurRectPos_Y + 1)*(iYChildSize + iYGap);
		}
		vec_rect.push_back(rectCurChild);
		if (NULL != pRect)
			pRect[i] = rectCurChild;
	}
	return vec_rect;
}

/*************************************<CLogFile>*************************************/
CLogFile::CLogFile(LPCTSTR lpFilePath)
{
	m_bOpenFile = false;
	m_pbWrite = nullptr;
	m_iCurLine = 0;
	m_iMaxLine = 10000;
	m_iLastTick = 0;
	m_iFrequency = 1;
	if(NULL!=lpFilePath)
		Open(lpFilePath);
	//InitializeCriticalSection(&m_csWrite);
}

CLogFile::~CLogFile()
{
	if(m_bOpenFile)
	{
		m_file.Close();
	}
	//DeleteCriticalSection(&m_csWrite);
}

void CLogFile::Flush()
{
	if(m_bOpenFile)
		m_file.Flush();
}

bool CLogFile::Open(LPCTSTR lpFilePath, bool* pbWrite, int iFlag)
{
	if(m_bOpenFile)
		return true;
// 	if (nullptr != pbWrite&&false == *pbWrite)
// 		return false;
	if (nullptr != pbWrite)
		m_pbWrite = pbWrite;
#ifdef UNICODE
	iFlag |= CFile::typeUnicode;
#endif
	CFileException e;
	try
	{
		m_bOpenFile = m_file.Open(lpFilePath, iFlag, &e);
		LARGE_INTEGER frequency , tickCnt;
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&tickCnt);
		m_iFrequency = frequency.QuadPart;
		m_iLastTick = tickCnt.QuadPart;
	}
	catch (CFileException* e)
	{
		TCHAR  szCause[255];
		CString strFormatted=_T("");
		e->GetErrorMessage(szCause, 255);
		// In real situations, it is
		// appropriate to read this from
		// a string resource so it would be easy to
		// localize.
		strFormatted = tr("The data file could not be opened because of this error: ");
		strFormatted += szCause;
		//AfxMessageBox(strFormatted);
	}
#ifdef UNICODE
	if (m_bOpenFile)
	{
		m_file.Write("\xff\xfe", 2);
	}
#endif
	return m_bOpenFile;
}

bool CLogFile::Open(LPCTSTR lpFilePath, int iFlag)
{
	return Open(lpFilePath, nullptr, iFlag);
}

bool CLogFile::Close()
{
	if (m_bOpenFile)
	{
		m_file.Close();
		m_bOpenFile = false;
	}
	return false;
}

bool CLogFile::Write(LPCTSTR lpLogInfo)
{
	CString sLogInfoTemp = lpLogInfo;
	if (nullptr != m_pbWrite && false == *m_pbWrite)
		return false;
	sLogInfoTemp += _T("\r\n");
	if (m_bOpenFile)
	{
		//EnterCriticalSection(&m_csWrite);
		if (m_iCurLine + 1 > m_iMaxLine)
		{
			m_iCurLine = 0;
			m_file.SeekToBegin();
		}
		SYSTEMTIME sysTime;
		GetLocalTime(&sysTime);
		CString sTime= _T("");
		LARGE_INTEGER tickCnt;
		LONGLONG diffCnt =  0 ;
		QueryPerformanceCounter(&tickCnt);
		diffCnt = (tickCnt.QuadPart - m_iLastTick)*1000/m_iFrequency;
		m_iLastTick = tickCnt.QuadPart;
		sTime.Format(_T("[%8d][Time:%04d%02d%02d_%02d:%02d:%02d:%03d][Elapse:%8d]   "), m_iCurLine+1,sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds, diffCnt);
		sLogInfoTemp = sTime + sLogInfoTemp;
		m_file.Write(sLogInfoTemp.GetBuffer(), sLogInfoTemp.GetLength()*sizeof(TCHAR));
		++m_iCurLine;
		//LeaveCriticalSection(&m_csWrite);
		return true;
	}
	TRACE(_T("-----")+sLogInfoTemp);
	return false;
}

// bool CLogFile::Write(const TCHAR * fmt, ...)
// {
// 	TCHAR lpszInfo[1024*1024];
// 	fill(begin(lpszInfo), end(lpszInfo), 0);
// 	va_list v_args;
// 	va_start(v_args, fmt);
// 	_vstprintf(lpszInfo, fmt, v_args);
// 	va_end(v_args);
// 	CString sLogInfoTemp = lpszInfo;
// 	if (nullptr != m_pbWrite && false == *m_pbWrite)
// 		return false;
// 	sLogInfoTemp += _T("\r\n");
// 	if (m_bOpenFile)
// 	{
// 		m_csWrite.Lock();
// 		if (m_iCurLine + 1 > m_iMaxLine)
// 		{
// 			m_iCurLine = 0;
// 			m_file.SeekToBegin();
// 		}
// 		SYSTEMTIME sysTime;
// 		GetLocalTime(&sysTime);
// 		CString sTime{ _T("") };
// 		LARGE_INTEGER tickCnt{ 0 };
// 		LONGLONG diffCnt{ 0 };
// 		QueryPerformanceCounter(&tickCnt);
// 		diffCnt = (tickCnt.QuadPart - m_iLastTick) * 1000 / m_iFrequency;
// 		m_iLastTick = tickCnt.QuadPart;
// 		sTime.Format(_T("[%8d][Time:%04d%02d%02d_%02d:%02d:%02d:%03d][Elapse:%8d]   "), m_iCurLine + 1, sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds, diffCnt);
// 		sLogInfoTemp = sTime + sLogInfoTemp;
// 		m_file.Write(sLogInfoTemp.GetBuffer(), sLogInfoTemp.GetLength() * sizeof(TCHAR));
// 		++m_iCurLine;
// 		m_csWrite.Unlock();
// 		return true;
// 	}
// 	TRACE(_T("-----") + sLogInfoTemp);
// 	return false;
// }

bool CLogFile::SetMaxLine(int iMaxLine)
{
	m_iMaxLine = iMaxLine;
	return true;
}
bool CLogFile::SeekToBegin()
{
	//EnterCriticalSection(&m_csWrite);
	m_file.SeekToBegin();
	//LeaveCriticalSection(&m_csWrite);
	return false;
}
/*************************************<字符转换类CStringConvert>************************************[9/14/2017 ConanSteve]*/
//将wstring转换成string

wstring CStringConvert::MBCS_UCS2(string& str, int iCodePage)
{
	wstring wsRet = L"";
	int nLen = MultiByteToWideChar(iCodePage, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pWchar_t = new wchar_t[nLen + 3];
	memset(pWchar_t, 0, sizeof(pWchar_t));
	MultiByteToWideChar(iCodePage, 0, str.c_str(), -1, pWchar_t, nLen);
	wsRet = pWchar_t;
	delete[] pWchar_t;
	return wsRet;
}

wstring CStringConvert::ANSI_UCS2(string& str) {
	return MBCS_UCS2(str, CP_ACP);
};

wstring CStringConvert::ANSI_UCS2(const char* ch)
{
	if (NULL == ch)
		return L"";
	else
		return ANSI_UCS2(string(ch));
}

wstring CStringConvert::UTF8_UCS2(string& str)
{
	return MBCS_UCS2(str, CP_UTF8);
}

wstring CStringConvert::UTF8_UCS2(char* ch)
{
	if (NULL == ch)
		return L"";
	else
		return UTF8_UCS2((string)ch);
}

wstring CStringConvert::GB2312_UCS2(string& str)
{
	return MBCS_UCS2(str, CP_ACP);
}

/************************************************************************/

string CStringConvert::UCS2_MBCS(wstring& wstr, int codePage)
{
	string strRet("");
	int nLen = WideCharToMultiByte(codePage, 0, wstr.c_str(), -1, NULL, 0, NULL, NULL);
	char* pChar = new char[nLen + 4];
	memset(pChar, 0, sizeof(pChar));
	WideCharToMultiByte(codePage, 0, wstr.c_str(), -1, pChar, nLen, NULL, NULL);
	strRet = pChar;
	delete[]pChar;
	return strRet;
}

string CStringConvert::UCS2_ANSI(wstring& wstr)
{
	return UCS2_MBCS(wstr, CP_ACP);
}

string CStringConvert::UCS2_ANSI(wchar_t* wch)
{
	if (NULL == wch)
		return "";
	else
	{
		return UCS2_ANSI(wstring(wch));
	}
}

string CStringConvert::UCS2_UTF8(wstring& wstr)
{
	return UCS2_MBCS(wstr, CP_UTF8);
}

string CStringConvert::UCS2_UTF8(wchar_t* wch)
{
	if (NULL == wch)
		return "";
	else
	{
		return UCS2_UTF8(wstring(wch));
	}
}

/************************************************************************/

string CStringConvert::ANSI_UTF8(string& str)
{
	return UCS2_UTF8(ANSI_UCS2(str));
}

string CStringConvert::ANSI_UTF8(char* ch)
{
	return UCS2_UTF8(ANSI_UCS2(string(ch)));
}

string CStringConvert::UTF8_ANSI(string& str)
{
	return UCS2_ANSI(UTF8_UCS2(str));
}

string CStringConvert::UTF8_ANSI(char* ch)
{
	return UCS2_ANSI(UTF8_UCS2(string(ch)));
}


/************************************************************************/
bool CStringConvert::IsTextUTF8(const char* str, long length)
{
	unsigned char chr;

	int nBytes = 0; // UFT8可用1-6个字节编码,ASCII用一个字节  
	bool bAllAscii = true; // 如果全部都是ASCII, 说明不是UTF-8  

	for (int i = 0; i < length; i++)
	{
		chr = *(str + i);

		if ((chr & 0x80) != 0) // 判断是否ASCII编码,如果不是,说明有可能是UTF-8, ASCII用7位编码,但用一个字节存,最高位标记为0,o0xxxxxxx  
		{
			bAllAscii = false;
		}

		if (nBytes == 0) // 如果不是ASCII码,应该是多字节符,计算字节数  
		{
			if (chr >= 0x80)
			{
				if (chr >= 0xFC && chr <= 0xFD)
					nBytes = 6;
				else if (chr >= 0xF8)
					nBytes = 5;
				else if (chr >= 0xF0)
					nBytes = 4;
				else if (chr >= 0xE0)
					nBytes = 3;
				else if (chr >= 0xC0)
					nBytes = 2;
				else
					return false;

				nBytes--;
			}
		}
		else // every char of ascii buffer looks like 10xxxxxx, except the first char  
		{
			if ((chr & 0xC0) != 0x80)
			{
				return false;
			}
			nBytes--;
		}
	}

	if (nBytes > 0) // format error  
	{
		return false;
	}

	if (bAllAscii) // if all chars are ascii, the buffer is not utf-8  
	{
		return false;
	}

	return true;
}

e_EncodeFormat CStringConvert::GetEncodeFormat(wstring& wsFilePathName)
{
	e_EncodeFormat ret = e_EncodeFormat_Error;
	//	std::ifstream in(wsFilePathName.c_str());
	// 	if (file.Open(sFilePathName, CFile::modeRead | CFile::shareDenyNone | CFile::typeBinary))
	// 	{
	// 		unsigned char ch[3]{ 0 };
	// 		file.Read(ch, 3);
	// 		if (0xFF == ch[0] && 0xFE == ch[1])
	// 		{
	// 			ret = e_EncodeFormat_UCS2_LE_BOM;
	// 		}
	// 		else if (0xFE == ch[0] && 0xFF == ch[1])
	// 		{
	// 			ret = e_EncodeFormat_UCS2_BE_BOM;
	// 		}
	// 		else if (0xEF == ch[0] && 0xBB == ch[1] && 0xBF == ch[2])
	// 		{
	// 			ret = e_EncodeFormat_UTF8_BOM;
	// 		}
	// 		//**[7/20/2017 ConanSteve]**:  查找所有字符，判断是ANSI还是UTF8
	// 		else
	// 		{
	// 			int iLen = file.GetLength();
	// 			file.SeekToBegin();
	// 			char* buf = new char[iLen + 1]{ 0 };
	// 			file.Read(buf, iLen);
	// 			if (CStringConvert::IsTextUTF8(buf, iLen))
	// 				ret = e_EncodeFormat_UTF8;
	// 		}
	// 		file.Close();
	// 	}
	return ret;
}
e_EncodeFormat CStringConvert::GetEncodeFormat(string& sFilePathName)
{
	e_EncodeFormat ret = e_EncodeFormat_Error;
	//std::ifstream in(sFilePathName.c_str());
	return ret;
}

e_EncodeFormat CStringConvert::GetEncodeFormat(char* pszFilePathName)
{
	if (NULL == pszFilePathName)
		return e_EncodeFormat_Error;
	return GetEncodeFormat(string(pszFilePathName));
}

CXMLCfg::CXMLCfg(LPCTSTR lpszFilePath)
{
	m_bOpenFile = false;
	m_sPathFile = _T("");
	if(NULL!=lpszFilePath)
		Init(lpszFilePath);
}

/*************************************<CXMLCfg>************************************[9/14/2017 ConanSteve]*/
CXMLCfg::~CXMLCfg()
{
}

bool CXMLCfg::Load(LPCTSTR lpszFilePath)
{
	return false;
}

bool CXMLCfg::Init(LPCTSTR lpszFilePath)
{
	if(NULL == lpszFilePath)
		return FALSE;
	bool bRet=false;
	m_bOpenFile = false;
	m_sPathFile = lpszFilePath;
	m_bOpenFile = true;
	m_xml.Load(lpszFilePath);
	m_xml.ResetMainPos();
	bRet = m_xml.FindElem(_T("root"));
	if (!bRet)
	{
		m_xml.AddElem(_T("root"));
	}
	m_xml.IntoElem();
	return true;
}


bool CXMLCfg::UpdateData()
{
	if (m_bOpenFile)
	{
		for (auto itera = m_vecNodeVar.begin(); m_vecNodeVar.end()!=itera; ++itera)
		{
			if (VarType::__no == itera->varType)
				continue;
			m_xml.ResetMainPos();
			m_xml.FindElem(itera->sAppName);
			m_xml.FindChildElem(itera->sKeyName);
			switch (itera->varType)
			{
			case VarType::__int:
				m_xml.SetChildData(*(itera->ptr.piInt));
				break;
			case VarType::__float:
				m_xml.SetChildData(*(itera->ptr.pfFloat));
				break;
			case VarType::__CString:
				m_xml.SetChildData(*(itera->ptr.psCString));
				break;
			default:
				break;
			}
		}
		m_xml.Save(m_sPathFile);
	}
	return true;
}
CString CXMLCfg::GetStringEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName,  LPCTSTR lpszDefaultValue, LPCTSTR lpszAnnotation)
{
	bool bRet= false ;
	CString sRetKeyValue= _T("") ;
	if (NULL != lpszDefaultValue)
	{
		sRetKeyValue = lpszDefaultValue;
	}
	if (m_bOpenFile)
	{
		m_xml.ResetMainPos();
		bRet = m_xml.FindElem(lpszAppName);
		if(!bRet)//**[9/14/2017 ConanSteve]**: 如果不存在，创建
		{
			m_xml.AddElem(lpszAppName);
		}
		bRet = m_xml.FindChildElem(lpszKeyName);
		if (!bRet)//**[9/14/2017 ConanSteve]**:  如果不存在，就添加当前节点，并添加注释信息
		{
			m_xml.AddChildElem(lpszKeyName, sRetKeyValue);
		}
		else
		{
			sRetKeyValue = m_xml.GetChildData();
		}
		if (NULL != lpszAnnotation)
		{
			m_xml.AddChildAttrib(_T("注释说明"), lpszAnnotation);
		}
	}
	return sRetKeyValue;
}
CString& CXMLCfg::GetStringEx(_In_opt_ LPCTSTR lpszAppName, _In_opt_ LPCTSTR lpszKeyName, CString &sRetKeyValue, LPCTSTR lpszDefaultValue, LPCTSTR lpszAnnotation)
{
	bool bRet= false ;
	NodeVar node;
	node.varType = VarType::__CString;
	node.ptr.psCString = &sRetKeyValue;
	node.sAppName = lpszAppName;
	node.sKeyName = lpszKeyName;
	m_vecNodeVar.push_back(node);
	sRetKeyValue = GetStringEx(lpszAppName, lpszKeyName,  lpszDefaultValue,lpszAnnotation);
	return sRetKeyValue;
}
bool CXMLCfg::WriteString(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszValue,LPCTSTR lpszAnnotation)
{
	bool bRet= false ;
	if (m_bOpenFile)
	{
		m_xml.ResetMainPos();
		bRet = m_xml.FindElem(lpszAppName);
		if (!bRet)
		{
			if(!m_xml.AddElem(lpszAppName))
				return false;
		}
		if (!m_xml.FindChildElem(lpszKeyName))//**[9/14/2017 ConanSteve]**:  不存在这个键值
		{
			if (!m_xml.AddChildElem(lpszKeyName, lpszValue))
				return false;
		}
		if(NULL!=lpszAnnotation)
		m_xml.SetChildAttrib(_T("说明"), lpszAnnotation);
		return true;
	}
	return true;
}
int CXMLCfg::GetInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iDefautlValue,LPCTSTR lpszAnnotation)
{
	bool bRet= false ;
	int iRetKeyValue = iDefautlValue;
	CString sDefaultVal= _T("") ;
	CString sRetKeyValue= _T("") ;
	sDefaultVal.Format(_T("%d"), iDefautlValue);
// 	m_xml.ResetMainPos();
// 	bRet = m_xml.FindElem(lpszAppName);
// 	if (!bRet)//**[9/14/2017 ConanSteve]**:  找不到
// 	{
// 		m_xml.AddElem(lpszAppName);
// 	}
// 	m_xml.IntoElem();
// 	bRet = m_xml.FindElem(lpszKeyName);
// 	if (!bRet)//**[9/14/2017 ConanSteve]**:  如果不存在，就添加当前节点，并添加注释信息
// 	{
// 		m_xml.AddElem(lpszKeyName, sDefaultVal);
// 	}
// 	else
// 	{
// 		iRetKeyValue = _ttoi(m_xml.GetData());
// 	}
// 	if (NULL != lpszAnnotation)
// 	{
// 		m_xml.AddAttrib(_T("注释说明"), lpszAnnotation);
// 	}
// 	m_xml.OutOfElem();
	iRetKeyValue = _ttoi(GetStringEx(lpszAppName, lpszKeyName, sDefaultVal, lpszAnnotation));
	return iRetKeyValue;
}
int& CXMLCfg::GetIntEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int& iKeyValue, int iDefaultValue, LPCTSTR lpszAnnotation)
{
	NodeVar node;
	node.varType = VarType::__int;
	node.ptr.piInt = &iKeyValue;
	node.sAppName = lpszAppName;
	node.sKeyName = lpszKeyName;
	m_vecNodeVar.push_back(node);
	iKeyValue = GetInt(lpszAppName, lpszKeyName, iDefaultValue,lpszAnnotation);
	return iKeyValue;
}
bool CXMLCfg::WriteInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iKeyValue, LPCTSTR lpszAnnotation)
{
	CString sKeyVal= _T("") ;
	sKeyVal.Format(_T("%d"), iKeyValue);
	WriteString(lpszAppName, lpszKeyName, sKeyVal, lpszAnnotation);
	return false;
}

bool CXMLCfg::GetBool(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bDefaultValue, LPCTSTR lpszAnnotation)
{
	bool bRet= bDefaultValue ;
	CString sRetValue= _T("") ;
	sRetValue = bDefaultValue ? _T("true") : _T("false");
	if (m_bOpenFile)
	{
		m_xml.ResetMainPos();
		bRet = m_xml.FindElem(lpszAppName);
		if (!bRet)//**[9/14/2017 ConanSteve]**: 如果不存在，创建
		{
			m_xml.AddElem(lpszAppName);
		}
		bRet = m_xml.FindChildElem(lpszKeyName);
		if (!bRet)//**[9/14/2017 ConanSteve]**:  如果不存在，就添加当前节点，并添加注释信息
		{
			m_xml.AddChildElem(lpszKeyName, sRetValue);
		}
		else
		{
			bRet = 0 == m_xml.GetChildData().CompareNoCase(_T("true"));
		}
		//**[9/15/2017 ConanSteve]**:  添加注释
		if (NULL != lpszAnnotation)
		{
			m_xml.AddChildAttrib(_T("注释说明"), lpszAnnotation);
		}
	}
	return bRet;
}

bool CXMLCfg::GetBoolEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool & bRetKeyValue, bool bDefaultValue, LPCTSTR lpszAnnotation)
{
	NodeVar node;
	node.varType = VarType::__int;
	node.ptr.pbBool = &bRetKeyValue;
	node.sAppName = lpszAppName;
	node.sKeyName = lpszKeyName;
	m_vecNodeVar.push_back(node);
	bRetKeyValue = GetBool(lpszAppName, lpszKeyName, bDefaultValue, lpszAnnotation);
	return bRetKeyValue;
}

bool CXMLCfg::WriteBool(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bKeyValue, LPCTSTR lpszAnnotation)
{
	bool bRet= false ;
	CString sKeyValue = bKeyValue ? _T("true") : _T("false");
	if (m_bOpenFile)
	{
		m_xml.ResetMainPos();
		bRet = m_xml.FindElem(lpszAppName);
		if (!bRet)
		{
			if (!m_xml.AddElem(lpszAppName))
				return false;
		}
		if (!m_xml.FindChildElem(lpszKeyName))//**[9/14/2017 ConanSteve]**:  不存在这个键值
		{
			if (!m_xml.AddChildElem(lpszKeyName, sKeyValue))
				return false;
		}
		if (NULL != lpszAnnotation)
			m_xml.SetChildAttrib(_T("说明"), lpszAnnotation);
		return true;
	}
	return true;
}

CString  CXMLCfg::GetStringEx(vector<CString>& vecNodePath, LPCTSTR lpszKeyName, LPCTSTR lpszDefaultValue)
{
	// TODO: 在此处插入 return 语句
	bool bRet= false ;
	CString sRetKeyValue= _T("") ;
	if (NULL != lpszDefaultValue)
	{
		sRetKeyValue = lpszDefaultValue;
	}
	if (m_bOpenFile)
	{
		m_xml.ResetPos();
		bRet = m_xml.FindElem();
		m_xml.IntoElem();
		for (auto i = 0; i < vecNodePath.size(); ++i)
		{
			bRet = m_xml.FindElem(vecNodePath[i]);//**[9/18/2017 ConanSteve]**:  查找当前节点
			if (!bRet)
			{
				m_xml.AddElem(vecNodePath[i]);
			}
			m_xml.IntoElem();
		}
		m_xml.OutOfElem();
		if (NULL != lpszKeyName)//**[9/19/2017 ConanSteve]**:  表示设置当前节点的属性值
		{
			sRetKeyValue = m_xml.GetAttrib(lpszKeyName);
			if (NULL != lpszDefaultValue&&sRetKeyValue == _T(""))
			{
				m_xml.SetAttrib(lpszKeyName, lpszDefaultValue);
				sRetKeyValue = lpszDefaultValue;
			}
		}
		else//**[9/19/2017 ConanSteve]**:  表示设置当前节点的节点值
		{
			sRetKeyValue = m_xml.GetData();
			if (NULL != lpszDefaultValue&&sRetKeyValue == _T(""))
			{
				m_xml.SetData(lpszDefaultValue);
				sRetKeyValue = lpszDefaultValue;
			}
		}
		
	}
	return sRetKeyValue;
}

CString & CXMLCfg::GetStringEx(vector<CString>& vecNodePath, LPCTSTR lpszKeyName, CString & sRetKeyValue, LPCTSTR lpszDefaultValue)
{
	// TODO: 在此处插入 return 语句
	return sRetKeyValue;
}

CString& CXMLCfg::GetAttrString(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszAttrName, CString &sAttrValue)
{
	return sAttrValue;
}
CString& CXMLCfg::GetAttrStringEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszAttrName, CString &sAttrValue)
{
	return sAttrValue;
}
bool CXMLCfg::WriteAttrString(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszAttrName, LPCTSTR lpszAttrValue)
{
	return true;
}
int& CXMLCfg::GetAttrInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszAttrName, int& iAttrValue)
{
	return iAttrValue;
}
int& CXMLCfg::GetAttrStringEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszAttrName, int& iAttrValue)
{
	return iAttrValue;
}
bool CXMLCfg::WriteAttrInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszAttrName, int iAttrValue)
{
	return true;
}
CINICfg::CINICfg(LPCTSTR lpszFilePath)
{
	m_bOpenFile = false;
	m_sPathFile = _T("");
	if(NULL!=lpszFilePath)
		Init(lpszFilePath);
}
/*************************************<CINICfg>************************************[9/14/2017 ConanSteve]*/
CINICfg::~CINICfg()
{
	
}
bool CINICfg::Load(LPCTSTR lpszFilePath)
{
	return false;
}
bool CINICfg::Init(LPCTSTR lpszFilePath)
{
	m_sPathFile = lpszFilePath;
	return TRUE== PathFileExists(m_sPathFile);
}

bool CINICfg::UpdateData()
{
	for (auto itera = m_vecNodeVar.begin(); m_vecNodeVar.end()!=itera; ++itera)
	{
		if (VarType::__no == itera->varType)
			continue;
		switch (itera->varType)
		{
		case VarType::__bool:
			CExFn::WritePrivateProfileBool(itera->sAppName, itera->sKeyName, *itera->ptr.pbBool, itera->sAnnotation);
			break;
		case VarType::__int:
			CExFn::WritePrivateProfileInt(itera->sAppName, itera->sKeyName, *itera->ptr.piInt, m_sPathFile, itera->sAnnotation);
			break;
		case VarType::__float:
			break;
		case VarType::__CString:
			CExFn::WritePrivateProfileString(itera->sAppName, itera->sKeyName, *itera->ptr.psCString, m_sPathFile);
			break;
		default:
			break;
		}
	}
	return true;
}

//**[1/25/2018 ConanSteve]**:	读取值 
bool CINICfg::GetBool(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bDefaultValue, LPCTSTR lpszAnnotation)
{
	return CExFn::GetPrivateProfileBool(lpszAppName, lpszKeyName, bDefaultValue ? 1 : 0, m_sPathFile, lpszAnnotation);
}

int CINICfg::GetInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iDefaultValue, LPCTSTR lpszAnnotation)
{
	return CExFn::GetPrivateProfileInt(lpszAppName, lpszKeyName, iDefaultValue, m_sPathFile, lpszAnnotation);
}

double CINICfg::GetDouble(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, double fDefaultValue, LPCTSTR lpszAnnotation)
{
	CString sTemp= _T("") ;
	double fRet = 0.0f;
	sTemp.Format(_T("%f"), fDefaultValue);
	GetString(lpszAppName, lpszKeyName, sTemp, sTemp, lpszAnnotation);
	fRet = _ttof(sTemp.GetBuffer());
	return fRet;
}

CString & CINICfg::GetString(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, CString & sReturnString, LPCTSTR lpszDefault, LPCTSTR lpszAnnotation)
{
	return CExFn::GetPrivateProfileString(lpszAppName, lpszKeyName, sReturnString, m_sPathFile, lpszDefault, lpszAnnotation);
}


//**[1/25/2018 ConanSteve]**:	 写值
bool CINICfg::WriteBool(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bKeyValue, LPCTSTR lpszAnnotation)
{
	return TRUE == WritePrivateProfileString(lpszAppName, lpszKeyName, bKeyValue ? _T("1") : _T("0"), m_sPathFile);;
}

bool CINICfg::WriteInt(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iKeyValue, LPCTSTR lpszAnnotation)
{
	return CExFn::WritePrivateProfileInt(lpszAppName, lpszKeyName, iKeyValue, m_sPathFile, lpszAnnotation);
}

bool CINICfg::WriteDouble(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, double lfValue, LPCTSTR lpszAnnotation)
{
	CString sTemp= _T("") ;
	double fRet = 0.0f;
	sTemp.Format(_T("%f"), lfValue);
	return WriteString(lpszAppName, lpszKeyName, sTemp, lpszAnnotation);
}

bool CINICfg::WriteString(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, LPCTSTR lpszValue, LPCTSTR lpszAnnotation)
{
	return CExFn::WritePrivateProfileString(lpszAppName, lpszKeyName, lpszValue, m_sPathFile);
}


//**[1/25/2018 ConanSteve]**:	读取，并写入默认值 
bool CINICfg::GetBoolEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool bDefaultValue, LPCTSTR lpszAnnotation)
{
	return CExFn::GetPrivateProfileBoolEx(lpszAppName, lpszKeyName, bDefaultValue ? 1 : 0, m_sPathFile, lpszAnnotation);
}

int CINICfg::GetIntEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int iDefaultValue, LPCTSTR lpszAnnotation)
{
	return CExFn::GetPrivateProfileIntEx(lpszAppName, lpszKeyName, iDefaultValue, m_sPathFile, lpszAnnotation);
}

double CINICfg::GetDoubleEx(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, double fDefaultValue, LPCTSTR lpszAnnotation)
{
	double lfRet = GetDouble(lpszAppName, lpszKeyName, fDefaultValue, lpszAnnotation);
	WriteDouble(lpszAppName, lpszKeyName, lfRet, lpszAnnotation);
	return lfRet;
}

CString& CINICfg::GetStringEx(_In_opt_ LPCTSTR lpszAppName, _In_opt_ LPCTSTR lpszKeyName, CString &sReturnString, _In_opt_ LPCTSTR lpszDefault, LPCTSTR lpszAnnotation)
{
	return CExFn::GetPrivateProfileStringEx(lpszAppName, lpszKeyName, sReturnString, m_sPathFile, lpszDefault, lpszAnnotation);
}

//**[1/25/2018 ConanSteve]**:	 
bool CINICfg::GetBoolEx2(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, bool & bRetKeyValue, bool bDefaultValue, LPCTSTR lpszAnnotation)
{
	NodeVar node;
	node.varType = VarType::__bool;
	node.ptr.pbBool = &bRetKeyValue;
	node.sAppName = lpszAppName;
	node.sKeyName = lpszKeyName;
	node.sAnnotation = lpszAnnotation;
	m_vecNodeVar.push_back(node);
	return GetBoolEx(lpszAppName, lpszKeyName, bDefaultValue, lpszAnnotation);
}

int& CINICfg::GetIntEx2(LPCTSTR lpszAppName, LPCTSTR lpszKeyName, int& iKeyValue, int iDefaultValue, LPCTSTR lpszAnnotation)
{
	NodeVar node;
	node.varType = VarType::__int;
	node.ptr.piInt = &iKeyValue;
	node.sAppName = lpszAppName;
	node.sKeyName = lpszKeyName;
	node.sAnnotation = lpszAnnotation;
	m_vecNodeVar.push_back(node);
	iKeyValue = GetIntEx(lpszAppName, lpszKeyName, iDefaultValue, lpszAnnotation);
	return iKeyValue;
}

CString& CINICfg::GetStringEx2(_In_opt_ LPCTSTR lpszAppName, _In_opt_ LPCTSTR lpszKeyName, CString &sReturnString, _In_opt_ LPCTSTR lpszDefault, LPCTSTR lpszAnnotation)
{
	bool bRet= false ;
	NodeVar node;
	node.varType = VarType::__CString;
	node.ptr.psCString = &sReturnString;
	node.sAppName = lpszAppName;
	node.sKeyName = lpszKeyName;
	m_vecNodeVar.push_back(node);
	sReturnString = GetStringEx(lpszAppName, lpszKeyName, sReturnString, lpszDefault, lpszAnnotation);
	return sReturnString;
}

/*************************************<>************************************[9/14/2017 ConanSteve]*/
CCPUUsage::CCPUUsage()
{
	init(); _processor = get_processor_number();
}


CCPUUsage::~CCPUUsage()
{
	clear();
}

CCPUUsage::CCPUUsage(DWORD ProcessID)
{
	init();
	_processor = get_processor_number();
	setpid(ProcessID);
}

void CCPUUsage::init()
{
	_last_system_time = 0;
	_last_time = 0;
	_hProcess = 0;
}

void CCPUUsage::clear()
{
	if (_hProcess)
	{
		CloseHandle(_hProcess);
		_hProcess = 0;
	}
}

HANDLE CCPUUsage::setpid(DWORD ProcessID) {
	clear();    //如果之前监视过另一个进程，就先关闭它的句柄  
	init();
	return _hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, false, ProcessID);
}

float CCPUUsage::get_cpu_usage()
{

	FILETIME now;
	FILETIME creation_time;
	FILETIME exit_time;
	FILETIME kernel_time;
	FILETIME user_time;
	int64_t system_time;
	int64_t time;
	int64_t system_time_delta;
	int64_t time_delta;
	DWORD exitcode;

	float cpu = -1;

	if (!_hProcess) return -1;

	GetSystemTimeAsFileTime(&now);

	//判断进程是否已经退出
	GetExitCodeProcess(_hProcess, &exitcode);
	if (exitcode != STILL_ACTIVE) {
		clear();
		return -1;
	}

	//计算占用CPU的百分比
	if (!GetProcessTimes(_hProcess, &creation_time, &exit_time, &kernel_time, &user_time))
	{
		clear();
		return -1;
	}
	system_time = (file_time_2_utc(&kernel_time) + file_time_2_utc(&user_time))
		/ _processor;
	time = file_time_2_utc(&now);

	//判断是否为首次计算
	if ((_last_system_time == 0) || (_last_time == 0))
	{
		_last_system_time = system_time;
		_last_time = time;
		return -2;
	}

	system_time_delta = system_time - _last_system_time;
	time_delta = time - _last_time;

	if (time_delta == 0) {
		return -1;
	}

	cpu = (float)system_time_delta * 100 / (float)time_delta;
	_last_system_time = system_time;
	_last_time = time;
	return cpu;
}

CCPUUsage::uint64_t CCPUUsage::file_time_2_utc(const FILETIME* ftime)
{
	LARGE_INTEGER li;

	li.LowPart = ftime->dwLowDateTime;
	li.HighPart = ftime->dwHighDateTime;
	return li.QuadPart;
}

int CCPUUsage::get_processor_number()
{
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return info.dwNumberOfProcessors;
}
/************************************************************************/

CLayout::~CLayout()
{
	if (PathFileExists(m_sInitFilePath))
		m_xml.Save(m_sInitFilePath);
}

bool CLayout::Init(LPCTSTR lpszInitFilePath, CRect & rectParent)
{
	m_rectParent = rectParent;
	if(!PathFileExists(lpszInitFilePath))
		return false;
	m_sInitFilePath = lpszInitFilePath;
	m_bInitSuccess = m_xml.Load(lpszInitFilePath);
	return m_bInitSuccess;
}

bool CLayout::GetPosition(LPCTSTR lpszWndName, CRect & rectWnd)
{
	bool bRet = false;
	if(FindFixedWnd(lpszWndName, rectWnd))
		return true;
	if(FindDynamicWnd(lpszWndName, rectWnd))
		return true;
	return bRet;
}

CString CLayout::GetAttrib(LPCTSTR lpszWndName, LPCTSTR lpszAttribName, CString & sRetVal)
{
	m_xml.ResetPos();
	m_xml.FindElem();
	m_xml.IntoElem();
	if (m_xml.FindElem(_T("FixedWindow")))
	{
		m_xml.IntoElem();
		while (m_xml.FindElem())
		{
			if (0 == m_xml.GetTagName().Compare(lpszWndName))//**[10/10/2017 ConanSteve]**:  找到当前窗口名称所在节点
			{
				CString sTempVal = m_xml.GetAttrib(lpszAttribName);
				if (_T("") != sTempVal)
					sRetVal = sTempVal;
				m_xml.AddAttrib(lpszAttribName, sRetVal);
				return sRetVal;
			}
		}
	}
	return _T("");
}

int CLayout::GetAttrib(LPCTSTR lpszWndName, LPCTSTR lpszAttrName, int iDefaultVal)
{
	CString sRet= _T("") ;
	sRet.Format(_T("%d"), iDefaultVal);
	sRet = GetAttrib(lpszWndName, lpszAttrName, sRet);
	if (_T("") != sRet)
	{
		return _ttoi(sRet.GetBuffer());
	}
	return 0;
}

bool CLayout::FindFixedWnd(LPCTSTR lpszWndName, CRect & rectWnd)
{
	m_xml.ResetPos();
	m_xml.FindElem();
	m_xml.IntoElem();
	if (m_xml.FindElem(_T("FixedWindow")))
	{
		m_xml.IntoElem();
		while (m_xml.FindElem())
		{
			if (0 == m_xml.GetTagName().Compare(lpszWndName))//**[10/10/2017 ConanSteve]**:  找到当前窗口名称所在节点
			{
				rectWnd.top = _ttoi(m_xml.GetAttrib(_T("top")));
				rectWnd.left = _ttoi(m_xml.GetAttrib(_T("left")));
				rectWnd.right = rectWnd.left + _ttoi(m_xml.GetAttrib(_T("width")));
				rectWnd.bottom = rectWnd.top + _ttoi(m_xml.GetAttrib(_T("height")));
				if (0 == m_xml.GetAttrib(_T("enableShow")).Compare(_T("false")))
					return false;
				return true;
			}
		}
	}
	return false;
}

bool CLayout::FindDynamicWnd(LPCTSTR lpszWndName, CRect & rectWnd)
{
	m_xml.ResetPos();
	m_xml.FindElem();
	m_xml.IntoElem();
	if (m_xml.FindElem(_T("DynamicWindow")))
	{
		m_xml.IntoElem();
		while (m_xml.FindElem())
		{
			if (ParseCurNode(lpszWndName, m_rectParent, rectWnd))
				return true;
		}
	}
	return false;
}

bool CLayout::ParseCurNode(LPCTSTR lpszWndName, CRect rectParent, CRect & rectWnd)
{

	if (0 == m_xml.GetTagName().Compare(_T("LayoutObject")))
	{

	}
	else if (0 == m_xml.GetTagName().Compare(lpszWndName))//**[10/10/2017 ConanSteve]**:  找到当前窗口名称所在节点
	{
		if (0 == m_xml.GetAttrib(_T("enableShow")).Compare(_T("false")))
			return false;
		rectWnd.top = _ttoi(m_xml.GetAttrib(_T("top")))+ rectParent.top;
		rectWnd.left = _ttoi(m_xml.GetAttrib(_T("left")))+rectParent.left;
		rectWnd.right = rectWnd.left + _ttoi(m_xml.GetAttrib(_T("width")));
		rectWnd.bottom = rectWnd.top + _ttoi(m_xml.GetAttrib(_T("height")));
		return true;
	}
	return false;
}

array<int, 4>& CLayout::ParsePadding(LPCTSTR lpszPadding, array<int, 4>& arrPadding)
{
	// TODO: 在此处插入 return 语句
	CString sPadding = lpszPadding;
	if (sPadding == _T(""))
		fill(arrPadding.begin(), arrPadding.end(), 0);
	return arrPadding;
}
