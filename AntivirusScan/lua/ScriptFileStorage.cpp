#include "stdafx.h"
#include "ScriptFileStorage.h"
#include <fstream>
#include <vector>
#include "utilities\filepathHelper.h"
#include "utilities\strings\StringsHelper.h"

//////////////////////////////////////////////////////////////////////////
// CScriptFileStream
// IScriptLibStream
long CScriptFileStream::Read(void *pv,ULONG cb,ULONG *pcbRead)
{
	DWORD dwRead = 0;
	HRESULT hr = m_file.Read(
		pv,
		cb,
		dwRead);
	if(pcbRead)
		*pcbRead = dwRead;
	return hr;
}

//////////////////////////////////////////////////////////////////////////
// CScriptFileStorage
CScriptFileStorage* CScriptFileStorage::GetInstance()
{
	CScriptFileStorage* p = new CScriptFileStorage;
	p->AddRef();
	return p;
}

long CScriptFileStorage::Open(const std::wstring& strFileName)
{
	if(strFileName.empty())
		return E_POINTER;

	if(!::PathIsDirectory(strFileName.c_str()))
		return E_INVALIDARG;

	m_strPath = strFileName;
	
	return S_OK;
}

// IScriptLibStorage
long CScriptFileStorage::OpenStream(const std::wstring& strStreamName,elex::lua::IScriptLibStream** ppstm)
{
	if(ppstm == NULL || strStreamName.empty())
		return E_POINTER;

	std::wstring strFilePath = elex::filepath::pathcombine(m_strPath,strStreamName.c_str());
	if(!PathFileExists(strFilePath.c_str()) || PathIsDirectory(strFilePath.c_str()))
		return E_INVALIDARG;

	
	std::fstream file;
	file.open(strFilePath,std::ios::in|std::ios::binary|std::ios::_Nocreate);
	if(file.fail())
	{
		return E_ACCESSDENIED;
	}

	file.seekg(0, std::ios::end);      //设置文件指针到文件流的尾部
	std::ios::pos_type ps = file.tellg();  //读取文件指针的位置
	std::streamsize uSize = ps;
	file.seekg(0, std::ios::beg);      //设置文件指针到文件流的头部

	char header[3] = {};  
	char uniTxt[] = {(char)0xFF, (char)0xFE};          // Unicode file header  
	char endianTxt[] = {(char)0xFE, (char)0xFF};       // Unicode big endian file header  
	char utf8Txt[] = {(char)0xEF, (char)0xBB, (char)0xBF};   // UTF_8 file header  

	int offset(0);
	file.read(header,3);
	std::string vecBuff;
	if(header[0] == uniTxt[0]&&header[1] == uniTxt[1])
	{
		offset = 2;
		file.seekg(2, std::ios::beg);
		std::wstring buff;
		while (!file.eof())
		{
			wchar_t wch;
			file.read((char*)(&wch), 2);
			buff.push_back(wch);
		}
		vecBuff = elex::strings::ws2s(buff);
	}
	else if(header[0] == endianTxt[0]&&header[1] == endianTxt[1])
	{
		offset = 2;
		file.seekg(2, std::ios::beg);    
		std::wstring buff;
		while (!file.eof())
		{
			wchar_t wch;
			file.read((char*)(&wch), 2);
			buff.push_back(wch);
		}
		vecBuff = elex::strings::ws2s(buff);
	}
	else if(header[0] == utf8Txt[0]&&header[1] == utf8Txt[1]&&header[2] == utf8Txt[2])
	{
		offset = 3;
		file.seekg(3, std::ios::beg); 
		vecBuff.resize((int)uSize - offset,0);
		file.read(&vecBuff[0],uSize - offset);
	}
	else
	{
		file.seekg(0, std::ios::beg); 
		vecBuff.resize((int)uSize - offset,0);
		file.read(&vecBuff[0],uSize - offset);
	}
	file.close();

	CScriptFileStream* p = new CScriptFileStream;
	p->m_file.Write(&vecBuff[0], vecBuff.size());
	p->m_file.Seek(0, FILE_BEGIN);

	p->AddRef();
	*ppstm = p;
	return S_OK;
}