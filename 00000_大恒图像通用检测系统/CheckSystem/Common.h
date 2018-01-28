#ifndef Common_h__
#define Common_h__
/********************************************************************
	created:	2015/07/15
	created:	15:7:2015   17:37
	filename: 	Common.h
	file path:	WinMain
	file base:	Common
	file ext:	h
	author:		 大色秘密

	purpose:	炫彩兼容头文件(兼容vc6 vs2008 2010 2013 2015 等)
*********************************************************************/





#include <windows.h>
#include <tchar.h>


#if _MSC_VER == 1200  // MS VC++6.0 _MSC_VER=1200 vc6
#define UNICODE
#define _UNICODE
	#define strcpy_s strcpy
#endif

#if _MSC_VER == 1500  // MS VC++9.0 _MSC_VER=1500 vs2008
#endif



//包含炫彩界面库文件
#pragma comment(lib, "XCGUI.lib")
#include "xcgui.h"


#ifdef USE_GDIPLUS
#include <GdiPlus.h>
using namespace Gdiplus;
#define ULONG_PTR ULONG
#pragma comment(lib,"gdiplus.lib")
#endif



// #ifndef _T
// #define _T(x) L ## x
// #endif

#ifndef wstring
#include <string>
using namespace std;
#endif



class CCmdLine
{
public:
	CCmdLine()
	{
		m_lpCmd = CommandLineToArgvW(GetCommandLineW(),&m_nArc);
	}
	wchar_t* operator[](int nIndex)
	{
		return m_lpCmd[nIndex];
	}
	int GetArgCount()
	{
		return m_nArc;
	}
	~CCmdLine()
	{
		GlobalFree(m_lpCmd);
	}
private:
	int     m_nArc;
	LPWSTR* m_lpCmd;
};


class XC
{
public:
	BOOL GetFileSize(const char* lpFile,DWORD* pdwSizeLow,DWORD* pdwSizeHight = NULL)
	{
		HANDLE hFile = CreateFileA(lpFile,
			GENERIC_READ,FILE_SHARE_READ,
			NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		if (hFile == INVALID_HANDLE_VALUE)
			return FALSE;
		
		DWORD dwFileSizeHight = 0;
		int nSize = ::GetFileSize(hFile,&dwFileSizeHight);
		CloseHandle(hFile);
		
		if ( pdwSizeLow != NULL)
			*pdwSizeLow = nSize;
		
		if (pdwSizeHight != NULL)
			*pdwSizeHight = dwFileSizeHight;
		
		return TRUE;
	}
	
	BOOL ReadTextFile(const char* lpFile,std::string& outFileData)
	{
		DWORD dwSize = 0;
		GetFileSize(lpFile,&dwSize,NULL);
		
		FILE* pFile1{NULL};
		fopen_s(&pFile1, lpFile,"r");
		char* pbuffer = new char[dwSize+1];
		pbuffer[dwSize] = '\0';
		fread(pbuffer,1,dwSize,pFile1);
		outFileData = pbuffer;
		delete [] pbuffer;
		fclose(pFile1);
		
		return TRUE;
	}
private:
};



	// type redefinition
typedef unsigned char  u1byte; // an 8 bit unsigned character type
typedef unsigned short u2byte; // a 16 bit unsigned integer type
typedef unsigned long  u4byte; // a 32 bit unsigned integer type
typedef void*          pvoid;  // a 32 bit undefined type pointer

class Thunk
{
public:

	Thunk()
	{
		m_pThis = (Thunk*)VirtualAlloc(NULL, sizeof (Thunk), 
		MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	}
	/*virtual*/ ~Thunk()
	{
		if (NULL == m_pThis)
			return;
	VirtualFree(m_pThis, 0, MEM_RELEASE);
	}

	pvoid Thiscall(pvoid pThis, u4byte MemberFxnAddr)
	{
	/**
	encoded machine instruction   equivalent assembly languate notation
	---------------------------   -------------------------------------
	B9 ?? ?? ?? ??                mov ecx, pThis  ; load ecx with this pointer
	E9 ?? ?? ?? ??                jmp target addr ; jump to target function
	*/

	/**
	#define GETBYTE(b, n) ((u1byte)(b >> ((n - 1) * 8) & 0x000000FF))

	u4byte dwJmpAdr = MemberFxnAddr - 
		(u4byte)(&(m_pThis->m_BytecodeThiscall)) - 
		sizeof (m_pThis->m_BytecodeThiscall);

	m_pThis->m_BytecodeThiscall[0] = 0xB9;
	m_pThis->m_BytecodeThiscall[1] = GETBYTE((u4byte)pThis, 1);
	m_pThis->m_BytecodeThiscall[2] = GETBYTE((u4byte)pThis, 2);
	m_pThis->m_BytecodeThiscall[3] = GETBYTE((u4byte)pThis, 3);
	m_pThis->m_BytecodeThiscall[4] = GETBYTE((u4byte)pThis, 4);
	m_pThis->m_BytecodeThiscall[5] = 0xE9;
	m_pThis->m_BytecodeThiscall[6] = GETBYTE((u4byte)dwJmpAdr, 1);
	m_pThis->m_BytecodeThiscall[7] = GETBYTE((u4byte)dwJmpAdr, 2);
	m_pThis->m_BytecodeThiscall[8] = GETBYTE((u4byte)dwJmpAdr, 3);
	m_pThis->m_BytecodeThiscall[9] = GETBYTE((u4byte)dwJmpAdr, 4);

	FlushInstructionCache(GetCurrentProcess(), 
		&(m_pThis->m_BytecodeThiscall), 
		sizeof (m_BytecodeThiscall));

	return &(m_pThis->m_BytecodeThiscall);
	*/

	m_pThis->m_BytecodeThiscall.Mov  = 0xB9;
	m_pThis->m_BytecodeThiscall.This = (u4byte)pThis;
	m_pThis->m_BytecodeThiscall.Jmp  = 0xE9;
	m_pThis->m_BytecodeThiscall.Adrr = MemberFxnAddr - 
		(u4byte)(&(m_pThis->m_BytecodeThiscall)) - 
		sizeof (BYTECODE_THISCALL);

	FlushInstructionCache(GetCurrentProcess(), 
		&(m_pThis->m_BytecodeThiscall), 
		sizeof (BYTECODE_THISCALL));

	return &(m_pThis->m_BytecodeThiscall);
	}
	pvoid Stdcall (pvoid pThis, u4byte MemberFxnAddr)
	{

			/**
	encoded machine instruction   equivalent assembly languate notation
	---------------------------   -------------------------------------
	FF 34 24                      push dword ptr [esp]          ; save (or duplicate)
																; the return Address into stack
	C7 44 24 04 ?? ?? ?? ??       mov  dword ptr [esp+4], pThis ; overwite the old
																; return address with 'this pointer'
	E9 ?? ?? ?? ??                jmp  target addr              ; jump to target function
	*/

	

	m_pThis->m_BytecodeStdcall.Push[0] = 0xFF;
	m_pThis->m_BytecodeStdcall.Push[1] = 0x34;
	m_pThis->m_BytecodeStdcall.Push[2] = 0x24;
	m_pThis->m_BytecodeStdcall.Move    = 0x042444C7;
	m_pThis->m_BytecodeStdcall.This    = (u4byte)pThis;
	m_pThis->m_BytecodeStdcall.Jmp     = 0xE9;
	m_pThis->m_BytecodeStdcall.Adrr    = MemberFxnAddr - 
		(u4byte)(&(m_pThis->m_BytecodeStdcall)) - 
		sizeof (BYTECODE_STDCALL);
	
	FlushInstructionCache(GetCurrentProcess(), 
		&(m_pThis->m_BytecodeStdcall), 
		sizeof (BYTECODE_STDCALL));

	return &(m_pThis->m_BytecodeStdcall);
	}

	template <typename T> 
	static u4byte GetMemberFxnAddr(T MemberFxnName)
	{
		union 
		{
			T From; 
			u4byte To;
		}	union_cast;
		union_cast.From = MemberFxnName;
		return union_cast.To;
	}

private:

	#pragma pack (push, 1)
	
	typedef struct _BYTECODE_THISCALL
	{
		u1byte Mov;     // 0xB9
		u4byte This;    // this pointer
		u1byte Jmp;     // 0xE9
		u4byte Adrr;    // relative jmp
	}	BYTECODE_THISCALL, *PBYTECODE_THISCALL;

	typedef struct _BYTECODE_STDCALL
	{
		u1byte Push[3]; // push dword ptr[esp] ;
		u4byte Move;    // mov dword ptr [esp + 4], ?? ?? ?? ?? ;
		u4byte This;    // this pointer
		u1byte Jmp;     // 0xE9
		u4byte Adrr;    // relative jmp
	}	BYTECODE_STDCALL, *PBYTECODE_STDCALL;

	#pragma pack (pop)

	// u1byte m_BytecodeThiscall[10];
	// u1byte m_BytecodeStdcall [16];

	BYTECODE_THISCALL m_BytecodeThiscall;
	BYTECODE_STDCALL  m_BytecodeStdcall;

	Thunk* m_pThis;
};



#endif // Common_h__