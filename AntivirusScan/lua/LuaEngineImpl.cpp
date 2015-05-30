#include "stdafx.h"
#include "LuaEngineImpl.h"
#include "ILuaCallBack.h"
#include "AutoPtrT.h"
#include "LuaVariantTable.h"
#include <map>
#include <ShellAPI.h>
#include <fstream>
#include "utilities\strings\StringsHelper.h"
#include <TlHelp32.h>

//#define DEBUG_LUAAPI
#define REPORT365_SCRIPT_KEY		_T("lua.report.%s")

//枚举子窗口用到的信息
typedef struct _WINDOWSINFO
{
	LPCTSTR lpChildClassName;
	LPCTSTR lpChileTitleName;
	LPCTSTR lpSetValue;   //设置的值
	LPTSTR  lpGetValue;   //获得的值
	int		iType;		  //1表示设置置，2表示获得值
}WINDOWSINFO, *LPWINDOWSINFIO;

CLuaEngineImpl::CLuaEngineImpl()
{
}

CLuaEngineImpl::~CLuaEngineImpl()
{
}

long CLuaEngineImpl::OnEnterScript(void)
{
	return 0;
}
long CLuaEngineImpl::OnLeaveScript(void)
{
	return 0;
}
long CLuaEngineImpl::OnScriptError(const EXCEPINFO* pexcepinfo)
{
	std::wstring des  = pexcepinfo->bstrDescription;
	std::wstring luaid = pexcepinfo->bstrSource;
	ISAFE_WriteProgramLog(ISAFE_LOG_MASK_ALL,_T("Script OnScriptError des=%s,luaid=%s"),des.c_str(),luaid.c_str());
	luaid += L".excute.end.error";
	std::wstring realkey = elex::strings::Printf(REPORT365_SCRIPT_KEY,luaid.c_str());
#if !defined(DEBUG_LUAAPI)
	//elex::commumod::SCommuMod::instance()->SendKeyString(realkey.c_str(), des.c_str());
#endif
	return 0;
}
long CLuaEngineImpl::OnScriptTerminate(const CLuaVariant& rvarRes, const EXCEPINFO *pexcepinfo)
{
	std::wstring des  = pexcepinfo->bstrDescription;
	std::wstring luaid = pexcepinfo->bstrSource;
	ISAFE_WriteProgramLog(ISAFE_LOG_MASK_ALL,_T("Script OnScriptTerminate des=%s,luaid=%s"),des.c_str(),luaid.c_str());
	luaid += L".excute.end.terminate";
	std::wstring realkey = elex::strings::Printf(REPORT365_SCRIPT_KEY,luaid.c_str());
#if !defined(DEBUG_LUAAPI)
	//elex::commumod::SCommuMod::instance()->SendKeyString(realkey.c_str(), des.c_str());
#endif
	return 0;
}

long CLuaEngineImpl::GetAllStartups(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::vector<std::wstring> v;
	CSmartPtr<CLuaVariantTable> pColl = NULL;
	CLuaVariantTable::Create(&pColl);
	if(pColl == NULL)
		return -1;

	elex::thread::Lock lock(m_Mutex_LuaCallBack);
	if(m_pLuaCallBack)
	{
		m_pLuaCallBack->GetAllStartups(v,rDispParams[0].getValue<bool>());
		for(const std::wstring& st : v)
		{
			CLuaVariant var;				
			var.setValue<std::wstring>(st);			 
			pColl->Add(CLuaVariant(),var);	
		}
	}
	rVarResult.setValue<ILuaElementCollection*>(pColl);
	return 0;
}

long CLuaEngineImpl::AddStartupResult(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring wstrId = rDispParams[0].getValue<std::wstring>();
	elex::thread::Lock lock(m_Mutex_LuaCallBack);
	if(m_pLuaCallBack)
	{
		m_pLuaCallBack->AddStartupResult(rDispParams[1].getValue<std::wstring>(), 
			rDispParams[2].getValue<int>(), 
			rDispParams[3].getValue<int>());
	}
	return 0;
}

long CLuaEngineImpl::AddScanFileResult(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring wstrId = rDispParams[0].getValue<std::wstring>();
	elex::thread::Lock lock(m_Mutex_LuaCallBack);
	if(m_pLuaCallBack)
	{
		m_pLuaCallBack->AddScanFileResult(rDispParams[1].getValue<std::wstring>(), 
			rDispParams[2].getValue<bool>(), 
			rDispParams[3].getValue<bool>());
	}
	return 0;
}

long CLuaEngineImpl::AddScanRegResult(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring wstrId = rDispParams[0].getValue<std::wstring>();
	elex::thread::Lock lock(m_Mutex_LuaCallBack);
	if(m_pLuaCallBack)
	{
		m_pLuaCallBack->AddScanRegResult(
			rDispParams[1].getValue<std::wstring>(), 
			rDispParams[2].getValue<bool>());
	}
	return 0;
}

long CLuaEngineImpl::AddScanRegValueResult(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring wstrId = rDispParams[0].getValue<std::wstring>();
	elex::thread::Lock lock(m_Mutex_LuaCallBack);
	if (m_pLuaCallBack)
	{
		m_pLuaCallBack->AddScanRegValueResult(
			rDispParams[1].getValue<std::wstring>(),
			rDispParams[2].getValue<std::wstring>(), 
			rDispParams[3].getValue<bool>());
	}
	return 0;
}



bool CLuaEngineImpl::AddLuaCallBack(const std::wstring &wstrId, ILuaCallBack* pCallBack)
{
	elex::thread::Lock lock(m_Mutex_LuaCallBack);
	m_pLuaCallBack = pCallBack;
	m_wstrId = wstrId;
	return true;
}

bool CLuaEngineImpl::RemoveLuaCallBack(const std::wstring &wstrId )
{
	elex::thread::Lock lock(m_Mutex_LuaCallBack);
	if ( wstrId == m_wstrId)
	{
		m_pLuaCallBack = NULL;
		return true;
	}
	return false;
}

long CLuaEngineImpl::Print(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring str = rDispParams[0];
	ISAFE_WriteProgramLog(ISAFE_LOG_MASK_ALL,_T("Script call oaApi.Print Print=%s"),str.c_str());
	return 0;
}

long CLuaEngineImpl::PrintBit(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::vector<char> vBit = rDispParams[0].getValue<std::vector<char>>();
	ISAFE_WriteProgramLog(ISAFE_LOG_MASK_ALL,_T("Script call oaApi.Print Print=%d"), vBit.size());
	return 0;
}

long CLuaEngineImpl::PrintTable(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	CSmartPtr<ILuaElementCollection> spLuaColl = rDispParams[0];
	size_t count = 0;
	spLuaColl->get_count(count);

	std::map<std::wstring,std::wstring> mValue;
	std::vector<std::wstring> vTavle;
	CLuaVariant rvarRes;
	for (size_t i = 0;i< count;i++)
	{
		CLuaVariant varKey;
		spLuaColl->item(varKey, i, rvarRes);
		spLuaColl->get_key(i,varKey);
		// 		int itest = 0;
		// 		vTavle.push_back(rvarRes);
		//std::wstring iKey = varKey.getValue<std::wstring>();

		//mValue.insert(make_pair(iKey,rvarRes.getValue<std::wstring>()));
	}
	return 0;
}
long CLuaEngineImpl::Test(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	TCHAR lpText[MAX_PATH] = {0};
	//GetPrivateProfileStringW(_T("result"), _T("virusversion"), _T(""), lpText, MAX_PATH, _T("C:\\share\\result\\avast.ini"));

	return 0;
}
LPCTSTR IsStringEmpty(std::wstring& strValue)
{
	if (!strValue.empty())
	{
		return strValue.c_str();
	}
	return NULL;
}

long CLuaEngineImpl::RunPrccess(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring wsExePath = rDispParams[0].getValue<std::wstring>();
	std::wstring wsCmdLine = rDispParams[1].getValue<std::wstring>();
	int iWaitTime = rDispParams[2].getValue<int>();

	ISAFE_WriteProgramLogNoMask(L"oaApi.RunPrccess wsExePath=%s, wsCmdLine=%s" ,wsExePath.c_str(), wsCmdLine.c_str());

	SHELLEXECUTEINFO sei = {0};
	BOOL bRtn = FALSE;
	int	 iRetPos = 0;
	DWORD dwRet = 0;

	sei.cbSize = sizeof(SHELLEXECUTEINFO);
	sei.fMask = SEE_MASK_NOCLOSEPROCESS;
	sei.lpFile = wsExePath.c_str();
	sei.lpParameters = wsCmdLine.c_str();
	sei.nShow = SW_NORMAL;

	bRtn = ShellExecuteExW(&sei);
	if (false == bRtn)
	{
		iRetPos = 1;
		dwRet = GetLastError();
		goto FAILEPOS;
	}

	DWORD dwWaitRet = WaitForSingleObject(sei.hProcess, iWaitTime);
	if (WAIT_OBJECT_0 != dwWaitRet)
	{
		iRetPos = 2;
		dwRet = GetLastError();
		goto FAILEPOS;
	}

	rVarResult = true;
	return 0;

FAILEPOS:
	ISAFE_WriteProgramLogNoMask(L"oaApi.RunPrccess ShellExecuteEx failcode=%d, pos=%d " , dwRet, iRetPos);
	rVarResult = false;
	return 0;
}

long CLuaEngineImpl::Convert2UTF8(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring strFileName = rDispParams[0].getValue<std::wstring>();

	if(strFileName.empty())
	{
		rVarResult = false;
		return 0;
	}

	std::fstream file;
	file.open(strFileName,std::ios::in|std::ios::binary|std::ios::_Nocreate);
	if(file.fail())
	{
		rVarResult = false;
		return 0;
	}

	file.seekg(0, std::ios::end);      //设置文件指针到文件流的尾部
	std::ios::pos_type ps = file.tellg();  //读取文件指针的位置
	std::streamsize uSize = ps;
	file.seekg(0, std::ios::beg);      //设置文件指针到文件流的头部
	if (uSize < 3)
	{
		file.close();
		rVarResult = false;
		return 0;
	}

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
		std::string buff;
		file.seekg(0, std::ios::beg); 
		buff.resize((int)uSize - offset,0);
		file.read(&buff[0], uSize - offset);
		std::wstring strTmp =  elex::strings::s2ws(buff, CP_ACP);
		vecBuff = elex::strings::ws2s(strTmp, CP_UTF8);
	}
	file.close();


	file.open(strFileName,std::ios::out|std::ios::binary);
	file.write(utf8Txt, 3);
	file.write(vecBuff.c_str(), vecBuff.size());
	file.close();

	rVarResult = true;
	return S_OK;
}
long CLuaEngineImpl::Convert2ANSI(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	return 0;
}
long CLuaEngineImpl::MouseMove(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	int iX = rDispParams[0].getValue<int>();
	int iY = rDispParams[1].getValue<int>();

	SetCursorPos(iX,iY);
//	mouse_event(MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE, iX, iY, 0, 0);
//	mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);

	ISAFE_WriteProgramLogNoMask(L"oaApi.MouseMove x=%d, y=%d" , iX, iY);

	rVarResult = true;
	return 0;
}
long CLuaEngineImpl::GetMousePos(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	POINT pos = {0};
	BOOL bRet = ::GetCursorPos(&pos);

	if (!bRet)
	{
		return 1;
	}

	LuaVarArray arrayRet;
	CLuaVariant var;
	var.setValue<int>(pos.x);
	arrayRet.push_back(var);
	var.setValue<int>(pos.y);
	arrayRet.push_back(var);

	rVarResult = arrayRet;
	return 0;
}
long CLuaEngineImpl::MouseClick(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring strType = rDispParams[0].getValue<std::wstring>();
	int iTimes = rDispParams[1].getValue<int>();
	rVarResult = true;

	ISAFE_WriteProgramLogNoMask(L"oaApi.MouseClick type=%s, times=%d" , strType.c_str(), iTimes);

	if (_T("L") == strType && 1 == iTimes)  //左键单击
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		Sleep(8);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
	}
	else if (_T("L") == strType && 2 == iTimes)
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
		Sleep(9);
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0);
	}
	else if (_T("R") == strType && 1 == iTimes)
	{
		mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
	}
	else if (_T("R") == strType && 2 == iTimes)
	{
		mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
		Sleep(9);
		mouse_event(MOUSEEVENTF_RIGHTDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_RIGHTUP,0,0,0,0);
	}
	else if (_T("M") == strType && 1 == iTimes)
	{
		mouse_event(MOUSEEVENTF_MIDDLEDOWN,0,0,0,0);
		mouse_event(MOUSEEVENTF_MIDDLEUP,0,0,0,0);
	}
	else if (_T("M") == strType && 2 == iTimes)
	{
	}
	else
	{
		rVarResult = false;  //参数不规范
	}

	return 0;
}
long CLuaEngineImpl::KeyboardDown(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	int iVirtualKey = rDispParams[0].getValue<int>();
	int bIsWriteLog = rDispParams[1].getValue<bool>();
	keybd_event(iVirtualKey, 0, 0, 0);

	if (bIsWriteLog)
		ISAFE_WriteProgramLogNoMask(L"oaApi.KeyboardDown type=%d", iVirtualKey);

	rVarResult = true;
	return 0;
}
long CLuaEngineImpl::KeyboardUp(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	int iVirtualKey = rDispParams[0].getValue<int>();
	int bIsWriteLog = rDispParams[1].getValue<bool>();
	keybd_event(iVirtualKey, 0, KEYEVENTF_KEYUP, 0);

	if (bIsWriteLog)
		ISAFE_WriteProgramLogNoMask(L"oaApi.KeyboardUp type=%d", iVirtualKey);

	rVarResult = true;
	return 0;
}
long CLuaEngineImpl::GetWindowTitle(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring strClassName = rDispParams[0].getValue<std::wstring>();

	LPCTSTR lpClassName = IsStringEmpty(strClassName);

	HWND hwnd = ::FindWindow(lpClassName, NULL);
	if (NULL == hwnd)
	{
		ISAFE_WriteProgramLogNoMask(L"oaApi.GetWindowTitle dont find this window classname=%s" , strClassName.c_str());
		rVarResult = false;
		return 0;
	}

	TCHAR lpText[MAX_PATH] = {0};
	GetWindowText(hwnd, lpText, MAX_PATH);
	rVarResult = lpText;

	return 0;
}
long CLuaEngineImpl::SetWindowFocus(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring strClassName = rDispParams[0].getValue<std::wstring>();
	std::wstring strTitleName = rDispParams[1].getValue<std::wstring>();

	LPCTSTR lpClassName = IsStringEmpty(strClassName);
	LPCTSTR lpTitleName = IsStringEmpty(strTitleName);

	ISAFE_WriteProgramLogNoMask(L"oaApi.SetWindowFocus,classname=%s, titlename=%s ", strClassName.c_str(), strTitleName.c_str());

	HWND hwnd = ::FindWindow(lpClassName, lpTitleName);
	if (NULL == hwnd)
	{
		rVarResult = false;
		return 0;
	}

	::SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE);
	//BringWindowToTop(hwnd);
	//BOOL bSmall = IsIconic(hwnd);    //是否最小化
	//BOOL bSmall = IsZoomed(hwnd);	   //是否最大化
	//ShowWindow(hwnd, SW_SHOWNORMAL);   //正常显示
	//int iRet = SetForegroundWindow(hwnd) ;
	//HWND hRet = ::SetFocus(hwnd);
	//if (NULL == hRet)
	//{
	//	ISAFE_WriteProgramLogNoMask(L"oaApi.SetWindowFocus setfocus fail classname=%s" , strClassName.c_str());
	//	rVarResult = false;
	//	return 0;
	//}

	rVarResult = true;
	return 0;
}
long CLuaEngineImpl::ShowWindows(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring strClassName = rDispParams[0].getValue<std::wstring>();
	std::wstring strTitleName = rDispParams[1].getValue<std::wstring>();
	int iType = rDispParams[2].getValue<int>();

	LPCTSTR lpClassName = IsStringEmpty(strClassName);
	LPCTSTR lpTitleName = IsStringEmpty(strTitleName);

	ISAFE_WriteProgramLogNoMask(L"oaApi.ShowWindows,classname=%s, titlename=%s ", strClassName.c_str(), strTitleName.c_str());

	HWND hwnd = ::FindWindow(lpClassName, lpTitleName);
	if (NULL == hwnd)
	{
		rVarResult = false;
		return 0;
	}

	ShowWindow(hwnd, iType);   //正常显示
	//int iRet = SetForegroundWindow(hwnd) ;

	rVarResult = true;
	return 0;
}
long CLuaEngineImpl::GetWindowPos(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring strClassName = rDispParams[0].getValue<std::wstring>();
	std::wstring strTitleName = rDispParams[1].getValue<std::wstring>();

	LPCTSTR lpClassName = IsStringEmpty(strClassName);
	LPCTSTR lpTitleName = IsStringEmpty(strTitleName);

	HWND hwnd = ::FindWindow(lpClassName, lpTitleName);
	if (NULL == hwnd)
	{
		ISAFE_WriteProgramLogNoMask(L"oaApi.GetWindowPos dont find this window classname=%s,titlename=%s " , strClassName.c_str(), strTitleName.c_str());
		return 1;
	}

	RECT appRect = {0};
	LuaVarArray arrayRet;
	CLuaVariant var;
	BOOL bRet = GetWindowRect(hwnd, &appRect);
	if (bRet)  //get success
	{
		var.setValue<int>(appRect.left);
		arrayRet.push_back(var);
		var.setValue<int>(appRect.top);
		arrayRet.push_back(var);
		var.setValue<int>(appRect.right);
		arrayRet.push_back(var);
		var.setValue<int>(appRect.bottom);
		arrayRet.push_back(var);

		rVarResult.setValue<LuaVarArray>(arrayRet);
		return 0;
	}

	return 1;
}
long CLuaEngineImpl::IsWindowExit(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring strClassName = rDispParams[0].getValue<std::wstring>();
	std::wstring strTitleName = rDispParams[1].getValue<std::wstring>();

	LPCTSTR lpClassName = IsStringEmpty(strClassName);
	LPCTSTR lpTitleName = IsStringEmpty(strTitleName);

	HWND hwnd = ::FindWindow(lpClassName, lpTitleName);
	if (NULL == hwnd)
	{
		ISAFE_WriteProgramLogNoMask(L"oaApi.IsWindowExit dont find this window classname=%s,titlename=%s " , strClassName.c_str(), strTitleName.c_str());
		return 1;
	}

	return 0;
}
long CLuaEngineImpl::CloseWindow(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring strClassName = rDispParams[0].getValue<std::wstring>();
	std::wstring strTitleName = rDispParams[1].getValue<std::wstring>();

	LPCTSTR lpClassName = IsStringEmpty(strClassName);
	LPCTSTR lpTitleName = IsStringEmpty(strTitleName);

	HWND hwnd = ::FindWindow(lpClassName, lpTitleName);
	if (NULL == hwnd)
	{
		ISAFE_WriteProgramLogNoMask(L"oaApi.CloseWindow dont find this window classname=%s, titlename=%s " , strClassName.c_str(), strTitleName.c_str());
		return 1;
	}

	BOOL bRet = ::PostMessage( hwnd, WM_CLOSE, 0, 0 );
	//BOOL bRet = ::CloseWindow(hwnd); 
	return bRet ? 0 : 1;
}
long CLuaEngineImpl::GetPiexlColor(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	int iPosX = rDispParams[0].getValue<int>();
	int iPosY = rDispParams[1].getValue<int>();

	HDC hDC = ::GetDC(NULL);
	COLORREF colorref = ::GetPixel(hDC, iPosX, iPosY);  //Get the cursor color
	::ReleaseDC(NULL,hDC);

	LuaVarArray arrayRet;
	CLuaVariant var;
	var.setValue<int>(colorref&0xFF);
	arrayRet.push_back(var);
	var.setValue<int>((colorref>>8)&0xFF);
	arrayRet.push_back(var);
	var.setValue<int>(colorref>>16);
	arrayRet.push_back(var);

	rVarResult = arrayRet;
	return 0;
}
BOOL CALLBACK EnumChildProc(HWND hWnd,LPARAM lParam)  
{  
	WINDOWSINFO* lpWinInfo = (WINDOWSINFO*)lParam;
	TCHAR lpClassName[MAX_PATH] = {0};
	::GetClassName(hWnd, lpClassName, MAX_PATH);

	if (0 == _tcsicmp(lpClassName, lpWinInfo->lpChildClassName))
	{
		if (GETVALUE == lpWinInfo->iType)
		{
			SendMessage(hWnd, WM_GETTEXT, MAX_PATH, (LPARAM)lpWinInfo->lpGetValue);
		}
		else if (SETVALUE == lpWinInfo->iType)
		{
			SendMessage(hWnd, WM_SETTEXT, MAX_PATH, (LPARAM)lpWinInfo->lpSetValue);
		}
	}
 
	return true;  
}
long CLuaEngineImpl::OpeWindowTextValue(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring strParentClassName = rDispParams[0].getValue<std::wstring>();
	std::wstring strParentTitleName = rDispParams[1].getValue<std::wstring>();
	std::wstring strChildClassName = rDispParams[2].getValue<std::wstring>();
	std::wstring strChildTitleName = rDispParams[3].getValue<std::wstring>();
	std::wstring strValue = rDispParams[4].getValue<std::wstring>();
	int iType = rDispParams[5].getValue<int>();

	LPCTSTR lpParentClassName = IsStringEmpty(strParentClassName);
	LPCTSTR lpParentTitleName = IsStringEmpty(strParentTitleName);
	LPCTSTR lpChildClassName = IsStringEmpty(strChildClassName);
	LPCTSTR lpChildTitleName = IsStringEmpty(strChildTitleName);

	HWND hwnd = ::FindWindow(lpParentClassName, lpParentTitleName);
	if (NULL == hwnd)
	{
		ISAFE_WriteProgramLogNoMask(L"oaApi.SetWindowTextValue dont find this window classname=%s" , strParentClassName.c_str());
		rVarResult = false;
		return 0;
	}

	TCHAR lpGetValue[MAX_PATH] = {0};
	WINDOWSINFO winInfo = {0};
	winInfo.lpChildClassName = lpChildClassName;
	winInfo.lpChileTitleName = lpChildTitleName;
	winInfo.lpSetValue = strValue.c_str();
	winInfo.lpGetValue = lpGetValue;
	winInfo.iType = iType;
	::EnumChildWindows(hwnd, EnumChildProc, LPARAM(&winInfo));

	if (GETVALUE == iType)
	{
		rVarResult = lpGetValue;
	}

	DWORD dwRet = GetLastError();

	return 0;
}
long CLuaEngineImpl::KillProcessByName(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring strProcessName = rDispParams[0].getValue<std::wstring>();

	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(PROCESSENTRY32);
	if(!Process32First(hSnapShot,&pe))
	{
		ISAFE_WriteProgramLogNoMask(L"oaApi.KillProcessByName Process32First fail processname=%s" , strProcessName.c_str());
		return 1;
	}
	DWORD dwCurrentID = GetCurrentProcessId();
	while (Process32Next(hSnapShot,&pe))
	{
		std::wstring scTmp = pe.szExeFile;
		if(scTmp==strProcessName && dwCurrentID != pe.th32ProcessID)
		{
			DWORD dwProcessID = pe.th32ProcessID;
			HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE,FALSE,dwProcessID);
			::TerminateProcess(hProcess,0);
			CloseHandle(hProcess);
			ISAFE_WriteProgramLogNoMask(L"oaApi.KillProcessByName kill success processname=%s" , strProcessName.c_str());
			return 0;
		}
	}

	ISAFE_WriteProgramLogNoMask(L"oaApi.KillProcessByName kill fail processname=%s" , strProcessName.c_str());
	return 1;
}

long CLuaEngineImpl::GetFileCreateTime(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring strFilePath = rDispParams[0].getValue<std::wstring>();

	WIN32_FIND_DATA ffd ;  
	HANDLE hFind = FindFirstFile(strFilePath.c_str(), &ffd);
	if (NULL == hFind)
	{
		ISAFE_WriteProgramLogNoMask(L"oaApi.GetFileCreateTime do find the file");
		return 1;
	}

	SYSTEMTIME st = {0};
	TCHAR szLastWriteTime[64] = {0};  
	FILETIME LocalFileTimel; 
	FileTimeToLocalFileTime(&ffd.ftLastWriteTime, &LocalFileTimel);   
	FileTimeToSystemTime(&LocalFileTimel, &st);  
	_stprintf_s(szLastWriteTime, _T("%4d%02d%02d"), st.wYear, st.wMonth, st.wDay);

	TCHAR szCreateTime[64] = {0};
	FileTimeToLocalFileTime(&ffd.ftCreationTime, &LocalFileTimel);
	FileTimeToSystemTime(&LocalFileTimel, &st);  
	_stprintf_s(szCreateTime, _T("%4d%02d%02d"), st.wYear, st.wMonth, st.wDay);

	LuaVarArray arrayRet;
	CLuaVariant var;
	var.setValue<std::wstring>(szCreateTime);
	arrayRet.push_back(var);
	var.setValue<std::wstring>(szLastWriteTime);
	arrayRet.push_back(var);

	ISAFE_WriteProgramLogNoMask(L"oaApi.GetFileCreateTime filename=%s, createtime=%s, writetime=%s" , strFilePath.c_str(), szCreateTime, szLastWriteTime);
	rVarResult = arrayRet;
	return 0;
}
bool IsFileExisting(LPCTSTR pszPath)
{
	if (!pszPath || !*pszPath)
		return false;

	DWORD dwAttr = ::GetFileAttributes(pszPath);
	if (INVALID_FILE_ATTRIBUTES == dwAttr)
		return false;

	return true;
}
bool deleteDirectory(std::wstring srcDir,bool bForce)
{
	HANDLE	handle; 
	WIN32_FIND_DATA    FindFileData;
	std::wstring srcTempDir=L"";
	bool bRet = true;

	if(srcDir[srcDir.length() - 1] == L'\\')
	{
		srcDir = srcDir.substr(0,srcDir.length()-1);
	}

	srcTempDir = srcDir;
	srcTempDir.append(L"\\*");

	handle = FindFirstFile(srcTempDir.c_str(),&FindFileData); 

	if(handle == INVALID_HANDLE_VALUE) 
	{ 
		return  bRet; 
	}

	srcTempDir  = (srcDir  + L"\\");

	while(true) 
	{ 
		if(!FindNextFile(handle,&FindFileData))
		{
			break; 
		}

		if(wcscmp(FindFileData.cFileName,L".") != 0 && wcscmp(FindFileData.cFileName,L"..") !=0) 
		{ 
			if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY) 
			{ 
				bRet &= deleteDirectory(srcTempDir + FindFileData.cFileName,bForce);
				SetFileAttributesW((srcTempDir + FindFileData.cFileName).c_str() ,FILE_ATTRIBUTE_NORMAL);
				if(bForce)
				{
					if(!RemoveDirectory((srcTempDir + FindFileData.cFileName).c_str()))
					{
						if( !MoveFileEx((srcTempDir + FindFileData.cFileName).c_str(),NULL,MOVEFILE_REPLACE_EXISTING))
							bRet = false;
					}
				}
				else
					if( !RemoveDirectory((srcTempDir + FindFileData.cFileName).c_str()))
						bRet = false;
			} 
			else 
			{ 
				bRet &= (FALSE != DeleteFile((srcTempDir + FindFileData.cFileName).c_str()));
			} 
		} 
	} 

	FindClose(handle);

	return bRet;
}
long CLuaEngineImpl::RemoveDir(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring strFilePath = rDispParams[0];
	int iOp = 1;
	ISAFE_WriteProgramLogNoMask(L"oaApi.RemoveDir path=%s",strFilePath.c_str());
	if (!IsFileExisting(strFilePath.c_str()))
	{
		rVarResult = true;
		return S_OK;
	}
	bool bret = deleteDirectory(strFilePath.c_str(), iOp == 1);
	rVarResult = bret;
	return 0;
}
long CLuaEngineImpl::MessageBoxTest(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult)
{
	std::wstring strTitle = rDispParams[0].getValue<std::wstring>();
	std::wstring strValue = rDispParams[1].getValue<std::wstring>();

	::MessageBoxW(NULL, strValue.c_str(), strTitle.c_str(), MB_OK);
	return 0;
}