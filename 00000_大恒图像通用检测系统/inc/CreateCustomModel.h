#pragma once


#ifdef EXDLL
#define DLLDECL _declspec(dllexport)
#else
#define DLLDECL _declspec(dllimport)
#endif

typedef bool(*pCreateCustomModel)(HWND hwnd, LPTSTR lpszModelName);

extern "C" bool DLLDECL CreateCustomModel(HWND hwnd, LPTSTR lpszModelName);