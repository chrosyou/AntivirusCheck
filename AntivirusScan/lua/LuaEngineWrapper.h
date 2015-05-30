#pragma once
#include "ILuaScriptHost.h"
#include <string>
#include "AutoPtrT.h"
#include <LuaVarint.h>

class CLuaEngineImpl;

class CLuaEngineWrapper
{
public:
	CLuaEngineWrapper();
	~CLuaEngineWrapper();
	bool Init(CLuaEngineImpl* pLuaEngineImpl);
	void Uninit();
	bool OpenLuaBuff(const std::vector<char>& vecBuff);
	bool OpenLuaFile(const std::wstring& strFilePath);
	bool Execute(const std::wstring &strGuidFile, const std::wstring &strCmd,elex::lua::CLuaVariant& rResult);
private:
	bool InitScriptEngine(CLuaEngineImpl *pLuaEngineImpl);
	bool InitScriptApi();

	CSmartPtr<elex::lua::ILuaScriptEngine> m_spScriptEngine;
	CSmartPtr<elex::lua::ILuaApi> m_spOsApi;
	CSmartPtr<elex::lua::ILuaApi> m_spLogicApi;
};