// CreateCustomModel.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "CreateCustomModel.h"
bool CreateCustomModel(HWND hwnd, LPTSTR lpszModelName)
{
	MessageBox(hwnd, _T("Create New Model Plugins"), _T("VS2017"), MB_OK);
	return true;
}

