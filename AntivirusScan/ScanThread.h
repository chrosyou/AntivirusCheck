
#pragma once
#include <string>
#include <vector>

extern std::wstring g_strTaskID;

class GeneralScan
{
public:
	GeneralScan(LPCTSTR lpConfigPath, LPTHREAD_START_ROUTINE lpStartAddress);
	virtual ~GeneralScan();
	virtual bool StartScan();
private:
	std::wstring m_strConfigPath;
	LPTHREAD_START_ROUTINE m_lpStartAddress;
};


class ScanThread
{
public:
	ScanThread(){};
	virtual~ScanThread(){};
	virtual HANDLE StartThread() = 0;
};


class ScanThreadNoui : public ScanThread
{
public:
	ScanThreadNoui(){};
	virtual~ScanThreadNoui(){};
	virtual HANDLE StartThread();
};

class ScanThreadHasui : public ScanThread
{
public:
	ScanThreadHasui(){};
	virtual~ScanThreadHasui(){};
	virtual HANDLE StartThread();
};