// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             //  �� Windows ͷ�ļ����ų�����ʹ�õ���Ϣ
// Windows ͷ�ļ�:
#include <windows.h>

// C ����ʱͷ�ļ�
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include "shlwapi.h"

#include "iSafeProgramLog.h"
#include "utilities/strings/stringprintf.h"

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�

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