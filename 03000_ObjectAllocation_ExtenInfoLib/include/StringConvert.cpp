//将wstring转换成string

#include "stdafx.h"
#include "StringConvert.h"

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



