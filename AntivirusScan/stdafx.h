// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  从 Windows 头文件中排除极少使用的信息
// Windows 头文件:
#include <windows.h>

// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "shlwapi.h"

#include "iSafeProgramLog.h"
#include "utilities/strings/stringprintf.h"

// TODO: 在此处引用程序需要的其他头文件

#ifdef _DEBUG
#pragma comment(lib,"isafepxyd.lib")
//#pragma comment(lib,"isaferptd.lib")
#pragma comment(lib,"iImportLibD.lib")
//#pragma comment(lib,"ebase_libmdd.lib")
#else
#pragma comment(lib,"isafepxy.lib")
//#pragma comment(lib,"isaferpt.lib")
#pragma comment(lib,"iImportLib.lib")
//#pragma comment(lib,"ebase_libmd.lib")
#endif

#pragma comment(lib, "shlwapi.lib")

void MyMessageBox(LPCTSTR lpTital, LPCTSTR lpValue);

#define		LUANOUICONFIG		(_T("scanscript\\nouiscan\\config.ini"))
#define		LUAUICONFIG			(_T("scanscript\\uiscan\\config.ini"))

HRESULT MyCreateObject(const wchar_t* full_name,void** ppv);