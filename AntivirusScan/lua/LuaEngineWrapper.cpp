#include "stdafx.h"
#include "LuaEngineWrapper.h"
#include "LuaEngineImpl.h"
#include "lua/ScriptFileStorage.h"

CLuaEngineWrapper::CLuaEngineWrapper()
{
}

CLuaEngineWrapper::~CLuaEngineWrapper()
{

}

bool CLuaEngineWrapper::Init(CLuaEngineImpl* pLuaEngineImpl)
{
	bool bRet = InitScriptEngine(pLuaEngineImpl);
	if (bRet)
	{
		bRet = InitScriptApi();
		if(bRet)
		{
			m_spScriptEngine->AddScriptSite(pLuaEngineImpl);
		}
	}

	return bRet;
}

bool CLuaEngineWrapper::OpenLuaBuff(const std::vector<char>& vecBuff)
{
	CSmartPtr<elex::lua::IScriptLibStorage> spStorage;
	if (S_OK != MyCreateObject(L"Base.LuaStorage.ScriptLib", (void**)&spStorage) || !spStorage)
		return false;

	if(S_OK != spStorage->OpenBuff(vecBuff))
		return false;

	if(S_OK != m_spScriptEngine->Close())
		return false;

	if(S_OK != m_spScriptEngine->Open(spStorage))
		return false;

	return true;
}

bool CLuaEngineWrapper::OpenLuaFile(const std::wstring& strFilePath)
{
	CSmartPtr<IScriptLibStorage> spStorage;
	CScriptFileStorage* p = new CScriptFileStorage;
	p->AddRef();
	spStorage.Attach(p);
	//IMODULE_DEFINE(Base.LuaStorage.ScriptLib, isafebase);
	//if (S_OK != CreateDllObject(L"Base.LuaStorage.ScriptLib", (void**)&spStorage) || !spStorage)
		//return false;

	if (S_OK != spStorage->Open(strFilePath))
		return false;

	if(S_OK != m_spScriptEngine->Open(spStorage))
		return false;

	spStorage.Release();

	return true;
}

bool CLuaEngineWrapper::InitScriptEngine(CLuaEngineImpl *pLuaEngineImpl)
{
	if(NULL == m_spScriptEngine)
	{
		if (S_OK != MyCreateObject(L"Base.iLuaScriptHost.Engine", (void**)&m_spScriptEngine) || !m_spScriptEngine)
			return false;
	}

	m_spScriptEngine->AddNamedItem(L"oaApi", pLuaEngineImpl);
	return true;
}

bool CLuaEngineWrapper::InitScriptApi()
{
	if(NULL == m_spOsApi)
	{
		if (S_OK != MyCreateObject(L"Base.LuaApi.osApi", (void**)&m_spOsApi) || !m_spOsApi)
			return false;
	}

	m_spScriptEngine->AddNamedItem(L"osApi", m_spOsApi);

	return true;
}

void CLuaEngineWrapper::Uninit()
{
	m_spScriptEngine.Release();
	m_spOsApi.Release();
	m_spLogicApi.Release();
}

bool CLuaEngineWrapper::Execute(const std::wstring &strGuidFile, const std::wstring &strCmd,CLuaVariant& rResult)
{
	if (!m_spScriptEngine || m_spScriptEngine->Execute2(strGuidFile, strCmd,rResult) != S_OK)
	{
		return false;
	}
	return true;
}