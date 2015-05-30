#pragma once
#include <string>
#include <LuaVarint.h>

#define		LUA_RETURN_SUCCESS		0x0001

class ILuaCallBack;
class CLuaEngineImpl;
class CLuaEngineWrapper;

class CLuaEngineMgr
{
public:
	CLuaEngineMgr();
	~CLuaEngineMgr();
public:
	CLuaEngineMgr* GetInstance();
	bool Excute(const std::wstring& strFilePath,ILuaCallBack* pCallBack,const std::wstring& strLuaFile,const std::wstring& strCmd,elex::lua::CLuaVariant& rResultt);
	bool Excute2(const std::vector<char>& vecBuff,ILuaCallBack* pCallBack,const std::wstring& strLuaGuid,const std::wstring& strCmd,elex::lua::CLuaVariant& rResult);
	void Uninit();
	int  GetReturn();
private:
	void _Init();
	CLuaEngineImpl* m_pLuaEngineImpl;
	CLuaEngineWrapper* m_pLuaEngineWrapper;
	CLuaEngineMgr* m_CLuaEngineMgr;
};