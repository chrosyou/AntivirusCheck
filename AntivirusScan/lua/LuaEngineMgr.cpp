#include "stdafx.h"
#include "LuaEngineMgr.h"
#include "LuaEngineImpl.h"
#include "LuaEngineWrapper.h"
#include "lua/ScriptFileStorage.h"

CLuaEngineMgr::CLuaEngineMgr()
{
	m_pLuaEngineImpl = NULL;
	m_pLuaEngineWrapper = NULL;
	m_CLuaEngineMgr = NULL;
}

CLuaEngineMgr::~CLuaEngineMgr()
{
	if (m_pLuaEngineWrapper)
	{
		delete m_pLuaEngineWrapper;
		m_pLuaEngineWrapper = NULL;
	}
	if (m_pLuaEngineImpl)
	{
		m_pLuaEngineImpl->Release();
		m_pLuaEngineImpl = NULL;
	}
}

static CLuaEngineMgr* g_CLuaEngineMgr = NULL;

CLuaEngineMgr* CLuaEngineMgr::GetInstance()
{
	if(!m_CLuaEngineMgr)
	{
		m_CLuaEngineMgr = new CLuaEngineMgr;
		m_CLuaEngineMgr->_Init();
	}

	return m_CLuaEngineMgr;
}

bool CLuaEngineMgr::Excute(const std::wstring& strFilePath,ILuaCallBack* pCallBack,const std::wstring& strLuaFile,const std::wstring& strCmd,CLuaVariant& rResult)
{
	bool bRet = false;

	if (m_pLuaEngineWrapper && m_pLuaEngineImpl)
	{
		if(m_pLuaEngineWrapper->OpenLuaFile(strFilePath))
		{
			m_pLuaEngineImpl->AddLuaCallBack(strLuaFile, pCallBack);
			bRet = m_pLuaEngineWrapper->Execute(strLuaFile, strCmd,rResult);
			m_pLuaEngineImpl->RemoveLuaCallBack(strLuaFile);
		}
	}

	return bRet;
}

bool CLuaEngineMgr::Excute2(const std::vector<char>& vecBuff,ILuaCallBack* pCallBack,const std::wstring& strLuaGuid,const std::wstring& strCmd,CLuaVariant& rResult)
{
	bool bRet = false;

	if (m_pLuaEngineWrapper && m_pLuaEngineImpl)
	{
		if(m_pLuaEngineWrapper->OpenLuaBuff(vecBuff))
		{
			m_pLuaEngineImpl->AddLuaCallBack(strLuaGuid, pCallBack);
			bRet = m_pLuaEngineWrapper->Execute(strLuaGuid, strCmd,rResult);
			m_pLuaEngineImpl->RemoveLuaCallBack(strLuaGuid);
		}
	}

	return bRet;
}

void CLuaEngineMgr::_Init()
{
	if (!m_pLuaEngineWrapper)
	{
		m_pLuaEngineWrapper = new CLuaEngineWrapper;
		if(!m_pLuaEngineImpl)
		{
			m_pLuaEngineImpl = new CLuaEngineImpl();
			m_pLuaEngineImpl->AddRef();
		}

		m_pLuaEngineWrapper->Init(m_pLuaEngineImpl);
	}
}

void CLuaEngineMgr::Uninit()
{
	if (m_pLuaEngineWrapper)
	{
		m_pLuaEngineWrapper->Uninit();
	}

	g_CLuaEngineMgr = NULL;
	delete this;
}