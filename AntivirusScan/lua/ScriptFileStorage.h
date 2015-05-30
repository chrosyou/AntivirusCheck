#pragma once
#include "IScriptLibStorage.h"
#include <Shlwapi.h>
#include <atlbase.h>
#include <atlcom.h>
#include "rtlmem.h"
#include <string>
#include <map>
#include "memory/ref_counted.h"

class CScriptFileStorage;

//////////////////////////////////////////////////////////////////////////
// CScriptFileStream
class CScriptFileStream : public elex::lua::IScriptLibStream,public elex::RefCountedImpl<CScriptFileStream>
{
	friend CScriptFileStorage;
	CRtlMemFileT<> m_file;
public:
	// IScriptLibStream
	virtual void AddRef()
	{
		InternalAddRef();
	}

	virtual void Release()
	{
		InternalRelease();
	}
	virtual long Read(void *pv,ULONG cb,ULONG *pcbRead);
};


//////////////////////////////////////////////////////////////////////////
// CScriptFileStorage
class CScriptFileStorage : public elex::lua::IScriptLibStorage,public elex::RefCountedImpl<CScriptFileStorage>
{
public:
	std::wstring m_strPath;
	struct NodeInfo 
	{
		DWORD dwOffset;
		DWORD dwLen;
		NodeInfo()
		{
			dwOffset = 0;
			dwLen = 0;
		}
	};
	std::map<std::wstring,NodeInfo> m_mapGuid;
	CRtlMemFileT<> m_mem;
public:
	CScriptFileStorage* GetInstance();
	// IScriptLibStorage
	virtual void AddRef()
	{
		InternalAddRef();
	}

	virtual void Release()
	{
		InternalRelease();
	}
	virtual long Open(const std::wstring& strFileName);
	virtual long OpenBuff(const std::vector<char>& vecBuff)
	{
		return E_NOTIMPL;
	}
	virtual long OpenStream(const std::wstring& strStreamName,elex::lua::IScriptLibStream** ppstm);
};
