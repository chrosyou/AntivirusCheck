// AntivirusScan.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "AntivirusScan.h"
#include "lua/LuaEngineMgr.h"
#include "exception\ExceptionLog.h"
#include "RunTask.h"
#include <fstream>

#ifdef _DEBUG
#define ISAFEBASE L"iSafeBased.dll"
#else
#define ISAFEBASE L"iSafeBase.dll"
#endif

HMODULE g_hModule = NULL;

typedef HRESULT (__stdcall *DllCreateObjectFunc)(LPCWSTR name, void **ppv);

DllCreateObjectFunc g_func = NULL;

HRESULT MyCreateObject(const wchar_t* full_name,void** ppv)
{
	if (g_func)
	{
		return g_func(full_name,ppv);
	}
	return E_FAIL;
}

//应该还要处理排队的问题
bool Run(elex::strings::CCmdLine& cmd)
{
	CAutoAntivirusScan autorun;
	if (!autorun.Init(cmd))
	{
		return false;
	}

	autorun.DoTask();
	return true;
}


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	//MyMessageBox(_T("test"), _T("test"));
	elex::exception::InitialExceptionLog(0,_T("AntiVirusScan"),NULL,NULL);
	ISAFE_InitProgramLogEx();
	ISAFE_WriteProgramLog( ISAFE_LOG_MASK_ALL, _T("AntivirusScan run with %s cmdline") ,lpCmdLine);

	elex::strings::CCmdLine cmd(lpCmdLine);

	g_hModule = LoadLibrary(ISAFEBASE);
	if(NULL == g_hModule)
		return 0;

	g_func = (DllCreateObjectFunc)::GetProcAddress(g_hModule, "DllCreateObject");
	if(NULL == g_func)
		return 0;
	
	Run(cmd);

	ISAFE_CloseProgramLog();

	FreeLibrary(g_hModule);
	g_hModule= NULL;

	return 0;
}

void MyMessageBox(LPCTSTR lpTital, LPCTSTR lpValue)
{
	MessageBox(NULL, lpTital, lpValue, MB_OK);
}
