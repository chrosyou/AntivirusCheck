#pragma once
#include "ILuaApiT.h"
#include "ILuaScriptHost.h"
#include "thread/Mutex.h"

#define		GETVALUE	1		//获得控件title的值
#define		SETVALUE	2		//设置控件title的值

using namespace elex::lua;

class ILuaCallBack;

class CLuaEngineImpl :  public CApiDispImplRootT<CLuaEngineImpl> ,public elex::lua::ILuaScriptEngineSite
{
public:
	BEGIN_APIDISP_MAP(CLuaEngineImpl)

		APIDISP_ENTRY_F(Print, "s")
		APIDISP_ENTRY_F(PrintBit, "b")
		APIDISP_ENTRY_F(PrintTable, "U")
		APIDISP_ENTRY_F(Test, "")
		APIDISP_ENTRY_F(RunPrccess, "ssi")
		APIDISP_ENTRY_F(Convert2UTF8, "s")
		APIDISP_ENTRY_F(Convert2ANSI, "s")


		APIDISP_ENTRY_F(MouseMove, "ii")
		APIDISP_ENTRY_F(GetMousePos, "")
		APIDISP_ENTRY_F(MouseClick, "si")
		APIDISP_ENTRY_F(KeyboardDown, "ii")
		APIDISP_ENTRY_F(KeyboardUp, "ii")
		APIDISP_ENTRY_F(GetWindowTitle, "s")		//获得窗体title
		APIDISP_ENTRY_F(SetWindowFocus, "ss")		//将某个窗口设置焦点
		APIDISP_ENTRY_F(ShowWindows, "ssi")			//显示窗体(正常方式显示)
		APIDISP_ENTRY_F(GetWindowPos, "ss")			//获得窗口的位置
		APIDISP_ENTRY_F(IsWindowExit, "ss")			//窗口是否存在
		APIDISP_ENTRY_F(CloseWindow, "ss")			//关闭窗口
		APIDISP_ENTRY_F(GetPiexlColor, "ii")		//取屏幕某点的颜色
		APIDISP_ENTRY_F(OpeWindowTextValue, "sssssi")	//设置控件中的值

		APIDISP_ENTRY_F(KillProcessByName, "s")	    //通过名称杀掉一个进程
		APIDISP_ENTRY_F(GetFileCreateTime, "s")	    //获得文件的创建时间，修改时间

		APIDISP_ENTRY_F(RemoveDir, "s")			//删除文件夹，保留最外层的

		APIDISP_ENTRY_F(MessageBoxTest, "ss")	        //弹对话框(调试使用)

	END_APIDISP_MAP()
public:
	CLuaEngineImpl();
	~CLuaEngineImpl();
	bool AddLuaCallBack(const std::wstring &wstrId,  ILuaCallBack* pCallBack);
	bool RemoveLuaCallBack(const std::wstring &wstrId );

	//ILuaCallBack function only for lua script
	long AddStartupResult(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	long AddScanFileResult(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	long AddScanRegResult(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	long AddScanRegValueResult(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	long GetAllStartups(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);

	DECLARE_LUAFUNC(Print)
	DECLARE_LUAFUNC(PrintBit)
	DECLARE_LUAFUNC(PrintTable)
	/**
    @brief: 测试函数
	@param[out]：成功为true，失败为false
    **/
	long Test(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: 起进程
	@param[in]: 进程路径
	@param[in]：命令行
	@param[in]：等待时间
	@param[out]：成功为true，失败为false
    **/
	long RunPrccess(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: 日志格式转换(杀软日志输出是ansi，需要转换为utf-8，增加了中文的支持)
	@param[in]: 日志路径
	@param[out]：成功为true，失败为false
    **/
	long Convert2UTF8(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: 将日志格式转化为ANSI
	@param[in]: 日志路径
	@param[out]：成功为true，失败为false
    **/
	long Convert2ANSI(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: 移动鼠标到制定位置
	@param[in]: x
	@param[in]: y
	@param[out]：成功为true，失败为false
    **/
	long MouseMove(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: 获得鼠标位置
	@param[out]：0为成功，其他为失败
	@param[out]：x
	@param[out]：y
    **/
	long GetMousePos(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: 鼠标点击
	@param[in]: 点击的位置, "L" 表示左击， "M" 表示中间， "R" 表示右击
	@param[in]: 点击的次数
	@param[out]：成功为true，失败为false
    **/
	long MouseClick(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: 按键按下
	@param[in]: msdn中指定的按键值(参见Virtual-Key Codes)
	@param[in]: true表示记录日志，false表示不记录日志
    **/
	long KeyboardDown(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: 按键弹起
	@param[in]: msdn中指定的按键值(参见Virtual-Key Codes)
	@param[in]: true表示记录日志，false表示不记录日志
    **/
	long KeyboardUp(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: 寻找窗口
	@param[in]: 窗口类名
	@param[in]: 窗口标题名
	@param[out]：窗口的标题名
    **/
	long GetWindowTitle(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: 通过classname将某个窗口激活
	@param[in]: classname
	@param[in]: titlename
	@param[out]：true表示设置焦点成功，false表示设置焦点失败
    **/
	long SetWindowFocus(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: 通过classname将某个窗口显示出来
	@param[in]: classname
	@param[in]: titlename
	@param[in]: 显示类型(SW_SHOWNORMAL(1)正常显示,(2最小化，3最大化))
	@param[out]：true表示设置焦点成功，false表示设置焦点失败
    **/
	long ShowWindows(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: 通过窗口类获得程序坐标
	@param[in]: classname
	@param[in]: titlename
	@param[out]：x1
	@param[out]：y1
	@param[out]：x2
	@param[out]：x2
    **/
	long GetWindowPos(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: 判断窗口是否存在
	@param[in]: classname
	@param[out]：true表示窗体存在，false表示窗体不存在
    **/
	long IsWindowExit(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: 关闭窗口
	@param[in]: classname
	@param[in]: titlename
	@param[out]：返回值判断对应函数是否执行成功，如返回0成功，其他失败。
    **/
	long CloseWindow(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: 取屏幕某点的颜色
	@param[in]: x值
	@param[in]: y值
	@param[out]：返回值判断对应函数是否执行成功，如返回0成功，其他失败。
	@param[out]：r
	@param[out]：g
	@param[out]：b
    **/
	long GetPiexlColor(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: 设置edit控件的值
	@param[in]: classname
	@param[in]: titilename
	@param[in]: childclasssname
	@param[in]: childtitilename
	@param[in]: 设置的字符串
	@param[in]: 类型，1表示获取控件的title值，2表示设置控件的title值(此时参数5就是设置的值)，
	@param[out]：返回值判断对应函数是否执行成功，如返回0成功，其他失败。
	@param[out]：如果参数6是1，则这里返回控件的title。
    **/
	long OpeWindowTextValue(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: 通过进程名杀掉进程
	@param[in]: 进程名
	@param[out]：返回值判断对应函数是否执行成功，如返回0成功，其他失败。
    **/
	long KillProcessByName(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: 获得文件的创建时间，修改时间
	@param[in]: 文件全路径
	@param[out]：返回值判断对应函数是否执行成功，如返回0成功，其他失败。
	@param[out]：创建时间
	@param[out]：修改时间
    **/
	long GetFileCreateTime(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: 删除文件夹(最外层保留)
	@param[in]: 删除文件夹的路径
    **/
	long RemoveDir(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: 弹对话框(调试使用)
	@param[in]: title
	@param[in]：value
    **/
	long MessageBoxTest(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);



	//ILuaScriptEngineSite
	long OnEnterScript(void);
	long OnLeaveScript(void);
	long OnScriptError(const EXCEPINFO* pexcepinfo);
	long OnScriptTerminate(const CLuaVariant& rvarRes, const EXCEPINFO *pexcepinfo);
private:
	ILuaCallBack* m_pLuaCallBack;
	std::wstring m_wstrId;
	elex::thread::Mutex m_Mutex_LuaCallBack;
};