
#include "stdafx.h"
#include "ScanThread.h"
#include "lua/LuaEngineMgr.h"


#define		NUMSIZE				20
#define		WAITTIME			6000000			//所有扫描等待最长10分钟

std::wstring g_strTaskID;

typedef struct _PATHPARAM
{
	LPTSTR lpBasePath;    //所在的路径
	LPTSTR lpFileName;	  //脚本文件名
}PATHPARAM;

bool RunLuaScript(PATHPARAM* lpPathParam)
{
	ISAFE_WriteProgramLogNoMask(L"Run LuaScript id=%s Begin", (lpPathParam)->lpFileName);
	elex::lua::CLuaVariant varResult;
	CLuaEngineMgr luaEngine;
	std::wstring wsParam = _T("\"") + g_strTaskID;
	wsParam = wsParam + _T("\"");
	std::wstring wsCmd = _T("Scan ") + wsParam;
	ISAFE_WriteProgramLogNoMask(L"Run LuaScript cmd=%s ", wsCmd.c_str());
	bool bRet = luaEngine.GetInstance()->Excute(lpPathParam->lpBasePath, NULL, lpPathParam->lpFileName, wsCmd, varResult);
	ISAFE_WriteProgramLogNoMask(L"Run LuaScript id=%s End Result=%d", lpPathParam->lpFileName, bRet);
	return bRet;
}

DWORD WINAPI NoUiScanFun(LPVOID lpParam)
{
	//ISAFE_WriteProgramLogNoMask(L"NoUiScanFun");
	PATHPARAM* lpPathParam = (PATHPARAM*)lpParam;

	return RunLuaScript(lpPathParam);
}

DWORD WINAPI UiScanFun(LPVOID lpParam)
{
	//ISAFE_WriteProgramLogNoMask(L"UiScanFun");
	PATHPARAM* lpPathParam = (PATHPARAM*)lpParam;

	return RunLuaScript(lpPathParam);
}

DWORD WINAPI TestScanFun(LPVOID lpParam)
{
	ISAFE_WriteProgramLogNoMask(L"TestScanFun, no this task");
	return 1;
}

void GetScriptPath(LPTSTR lpScriptPath, LPCTSTR lpcFolderPath)
{
	GetModuleFileName(NULL, lpScriptPath, MAX_PATH);
	PathRemoveFileSpec(lpScriptPath);
	PathAppend(lpScriptPath, lpcFolderPath);
}

bool GetScriptBasePath(LPTSTR lpBasePath, int iSize, LPCTSTR lpConfigPath)
{
	memset(lpBasePath, 0, sizeof(TCHAR)*iSize);
	GetScriptPath(lpBasePath, lpConfigPath);
	PathRemoveFileSpec(lpBasePath);
	return true;
}

LPTHREAD_START_ROUTINE GetThradAddress(LPCTSTR lpTaskType, std::wstring& strConfigPath)
{
	if (0 == _tcsicmp(lpTaskType, _T("nouiscan")))
	{
		strConfigPath = LUANOUICONFIG;
		return NoUiScanFun;
	}
	else if (0 == _tcsicmp(lpTaskType, _T("uiscan")))
	{
		strConfigPath = LUAUICONFIG;
		return UiScanFun;
	}
	else
	{
		return NULL;
	}
}


GeneralScan::GeneralScan(LPCTSTR lpConfigPath, LPTHREAD_START_ROUTINE lpStartAddress)
{
	m_strConfigPath = lpConfigPath;
	m_lpStartAddress = lpStartAddress;
}

GeneralScan::~GeneralScan(){}

bool GeneralScan::StartScan()
{
	if (NULL == m_lpStartAddress || m_strConfigPath.empty())
	{
		return false;
	}

	TCHAR lpModulePath[MAX_PATH] = {0};
	GetScriptPath(lpModulePath, m_strConfigPath.c_str());

	int iNumber = GetPrivateProfileInt(_T("script"), _T("count"), 0, lpModulePath);
	if (0 == iNumber)
	{
		return false;
	}

	HANDLE* lpHandle = new HANDLE[iNumber];
	memset(lpHandle, 0, sizeof(HANDLE)*iNumber);

	PATHPARAM* pathParam = new PATHPARAM[iNumber];	

	int i = 0;
	DWORD dwSize = 0;
	TCHAR lpKeyName[NUMSIZE] = {0};
	while (i < iNumber)
	{
		dwSize = 0;
		pathParam[i].lpBasePath = new TCHAR[MAX_PATH];
		pathParam[i].lpFileName = new TCHAR[MAX_PATH];

		_stprintf_s(lpKeyName, NUMSIZE, _T("%s%d"), _T("filename"), i);
		dwSize = GetPrivateProfileString(_T("script"), lpKeyName, _T("0"), pathParam[i].lpFileName, MAX_PATH, lpModulePath);
		if (0 == dwSize)
			continue;

		GetScriptBasePath(pathParam[i].lpBasePath, MAX_PATH, m_strConfigPath.c_str());
		lpHandle[i] = CreateThread(NULL, 0, m_lpStartAddress, LPVOID(&pathParam[i]), 0, NULL);

		i++;
	}

	DWORD dwRet = WaitForMultipleObjects(iNumber, lpHandle, TRUE, WAITTIME);
	ISAFE_WriteProgramLogNoMask(L"StartScan WaitForMultipleObjects return Result=%d", dwRet);

	i = 0;
	while (i < iNumber)
	{
		delete [] pathParam[i].lpBasePath;
		delete [] pathParam[i].lpFileName;
		i++;
	}
	delete [] lpHandle;
	delete [] pathParam;

	return true;
}


DWORD WINAPI OuterScanNoui(LPVOID lpParam)
{
	GeneralScan generalscan(LUANOUICONFIG, NoUiScanFun);
	generalscan.StartScan();
	return 0;
}

HANDLE ScanThreadNoui::StartThread()
{
	return  CreateThread(NULL, 0, OuterScanNoui, NULL, 0, NULL);
}


DWORD WINAPI OuterScanHasui(LPVOID lpParam)
{
	GeneralScan generalscan(LUAUICONFIG, UiScanFun);
	generalscan.StartScan();
	return 0;
}

HANDLE ScanThreadHasui::StartThread()
{
	return  CreateThread(NULL, 0, OuterScanHasui, NULL, 0, NULL);
}
