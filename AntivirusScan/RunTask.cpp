
#include "stdafx.h"
#include "RunTask.h"
#include "iScriptLib.h"
#include "lua/LuaEngineMgr.h"
#include "ScanThread.h"
#include <fstream>

CAutoAntivirusScan::CAutoAntivirusScan()
{

}

CAutoAntivirusScan::~CAutoAntivirusScan()
{

}

bool CAutoAntivirusScan::Init(elex::strings::CCmdLine& cmd)
{
	/*
	TCHAR lpPath[MAX_PATH] = {};
	GetModuleFileName(NULL,lpPath,MAX_PATH);
	PathRemoveFileSpec(lpPath);
	PathAppend(lpPath, _T("scanscript"));
	m_Path = lpPath;
	PathAppend(lpPath, _T("config.ini"));

	TCHAR lpFileName[MAX_PATH] = {0};
	DWORD dwSize = GetPrivateProfileString(_T("filefolder"), _T("filename"), _T(""), lpFileName, MAX_PATH, lpPath);
	//DWORD dwSize = GetPrivateProfileInt(_T("filefolder"), _T("count"), 0, lpPath);
	if (0 == dwSize)
	{
		ISAFE_WriteProgramLogNoMask(L"GetPrivateProfileString error=%d", GetLastError());
		return false;
	}
	*/

	g_strTaskID = cmd[_T("taskid")];
	//m_FileName = lpFileName;
	return true;
}

int  CAutoAntivirusScan::DoTask()
{
#define		SCANCOUNT		2
	HANDLE hScan[SCANCOUNT] = {0};

	ScanThreadNoui nouiScan;
	hScan[0] = nouiScan.StartThread();


	ScanThreadHasui hasuiScan;
	hScan[1] = hasuiScan.StartThread();


	WaitForMultipleObjects(SCANCOUNT, hScan, TRUE, INFINITE);

	return 0;
}

void CAutoAntivirusScan::Uninit()
{

}