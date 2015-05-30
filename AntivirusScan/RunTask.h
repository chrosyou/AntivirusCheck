
#pragma once
#include <string>
#include <vector>
#include "utilities\strings\CmdLineHelper.h"

class CAutoAntivirusScan
{
public:
	CAutoAntivirusScan();
	~CAutoAntivirusScan();
	bool Init(elex::strings::CCmdLine& cmd);
	int  DoTask();
	void Uninit();


private:
	std::wstring m_FileName;
	std::wstring m_Path;
	std::wstring m_TaskId;
	//ICOM_PRODUCTINFO m_info;
	std::vector<char> m_vecBuff;

	bool m_bLocal;
	std::wstring m_strLocalPath;
	std::wstring m_strLocalIniFile;
};