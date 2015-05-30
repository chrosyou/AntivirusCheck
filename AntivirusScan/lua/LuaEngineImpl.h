#pragma once
#include "ILuaApiT.h"
#include "ILuaScriptHost.h"
#include "thread/Mutex.h"

#define		GETVALUE	1		//��ÿؼ�title��ֵ
#define		SETVALUE	2		//���ÿؼ�title��ֵ

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
		APIDISP_ENTRY_F(GetWindowTitle, "s")		//��ô���title
		APIDISP_ENTRY_F(SetWindowFocus, "ss")		//��ĳ���������ý���
		APIDISP_ENTRY_F(ShowWindows, "ssi")			//��ʾ����(������ʽ��ʾ)
		APIDISP_ENTRY_F(GetWindowPos, "ss")			//��ô��ڵ�λ��
		APIDISP_ENTRY_F(IsWindowExit, "ss")			//�����Ƿ����
		APIDISP_ENTRY_F(CloseWindow, "ss")			//�رմ���
		APIDISP_ENTRY_F(GetPiexlColor, "ii")		//ȡ��Ļĳ�����ɫ
		APIDISP_ENTRY_F(OpeWindowTextValue, "sssssi")	//���ÿؼ��е�ֵ

		APIDISP_ENTRY_F(KillProcessByName, "s")	    //ͨ������ɱ��һ������
		APIDISP_ENTRY_F(GetFileCreateTime, "s")	    //����ļ��Ĵ���ʱ�䣬�޸�ʱ��

		APIDISP_ENTRY_F(RemoveDir, "s")			//ɾ���ļ��У�����������

		APIDISP_ENTRY_F(MessageBoxTest, "ss")	        //���Ի���(����ʹ��)

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
    @brief: ���Ժ���
	@param[out]���ɹ�Ϊtrue��ʧ��Ϊfalse
    **/
	long Test(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: �����
	@param[in]: ����·��
	@param[in]��������
	@param[in]���ȴ�ʱ��
	@param[out]���ɹ�Ϊtrue��ʧ��Ϊfalse
    **/
	long RunPrccess(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: ��־��ʽת��(ɱ����־�����ansi����Ҫת��Ϊutf-8�����������ĵ�֧��)
	@param[in]: ��־·��
	@param[out]���ɹ�Ϊtrue��ʧ��Ϊfalse
    **/
	long Convert2UTF8(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: ����־��ʽת��ΪANSI
	@param[in]: ��־·��
	@param[out]���ɹ�Ϊtrue��ʧ��Ϊfalse
    **/
	long Convert2ANSI(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: �ƶ���굽�ƶ�λ��
	@param[in]: x
	@param[in]: y
	@param[out]���ɹ�Ϊtrue��ʧ��Ϊfalse
    **/
	long MouseMove(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: ������λ��
	@param[out]��0Ϊ�ɹ�������Ϊʧ��
	@param[out]��x
	@param[out]��y
    **/
	long GetMousePos(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: �����
	@param[in]: �����λ��, "L" ��ʾ����� "M" ��ʾ�м䣬 "R" ��ʾ�һ�
	@param[in]: ����Ĵ���
	@param[out]���ɹ�Ϊtrue��ʧ��Ϊfalse
    **/
	long MouseClick(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: ��������
	@param[in]: msdn��ָ���İ���ֵ(�μ�Virtual-Key Codes)
	@param[in]: true��ʾ��¼��־��false��ʾ����¼��־
    **/
	long KeyboardDown(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: ��������
	@param[in]: msdn��ָ���İ���ֵ(�μ�Virtual-Key Codes)
	@param[in]: true��ʾ��¼��־��false��ʾ����¼��־
    **/
	long KeyboardUp(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: Ѱ�Ҵ���
	@param[in]: ��������
	@param[in]: ���ڱ�����
	@param[out]�����ڵı�����
    **/
	long GetWindowTitle(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: ͨ��classname��ĳ�����ڼ���
	@param[in]: classname
	@param[in]: titlename
	@param[out]��true��ʾ���ý���ɹ���false��ʾ���ý���ʧ��
    **/
	long SetWindowFocus(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: ͨ��classname��ĳ��������ʾ����
	@param[in]: classname
	@param[in]: titlename
	@param[in]: ��ʾ����(SW_SHOWNORMAL(1)������ʾ,(2��С����3���))
	@param[out]��true��ʾ���ý���ɹ���false��ʾ���ý���ʧ��
    **/
	long ShowWindows(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: ͨ���������ó�������
	@param[in]: classname
	@param[in]: titlename
	@param[out]��x1
	@param[out]��y1
	@param[out]��x2
	@param[out]��x2
    **/
	long GetWindowPos(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: �жϴ����Ƿ����
	@param[in]: classname
	@param[out]��true��ʾ������ڣ�false��ʾ���岻����
    **/
	long IsWindowExit(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: �رմ���
	@param[in]: classname
	@param[in]: titlename
	@param[out]������ֵ�ж϶�Ӧ�����Ƿ�ִ�гɹ����緵��0�ɹ�������ʧ�ܡ�
    **/
	long CloseWindow(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: ȡ��Ļĳ�����ɫ
	@param[in]: xֵ
	@param[in]: yֵ
	@param[out]������ֵ�ж϶�Ӧ�����Ƿ�ִ�гɹ����緵��0�ɹ�������ʧ�ܡ�
	@param[out]��r
	@param[out]��g
	@param[out]��b
    **/
	long GetPiexlColor(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: ����edit�ؼ���ֵ
	@param[in]: classname
	@param[in]: titilename
	@param[in]: childclasssname
	@param[in]: childtitilename
	@param[in]: ���õ��ַ���
	@param[in]: ���ͣ�1��ʾ��ȡ�ؼ���titleֵ��2��ʾ���ÿؼ���titleֵ(��ʱ����5�������õ�ֵ)��
	@param[out]������ֵ�ж϶�Ӧ�����Ƿ�ִ�гɹ����緵��0�ɹ�������ʧ�ܡ�
	@param[out]���������6��1�������ﷵ�ؿؼ���title��
    **/
	long OpeWindowTextValue(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: ͨ��������ɱ������
	@param[in]: ������
	@param[out]������ֵ�ж϶�Ӧ�����Ƿ�ִ�гɹ����緵��0�ɹ�������ʧ�ܡ�
    **/
	long KillProcessByName(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: ����ļ��Ĵ���ʱ�䣬�޸�ʱ��
	@param[in]: �ļ�ȫ·��
	@param[out]������ֵ�ж϶�Ӧ�����Ƿ�ִ�гɹ����緵��0�ɹ�������ʧ�ܡ�
	@param[out]������ʱ��
	@param[out]���޸�ʱ��
    **/
	long GetFileCreateTime(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: ɾ���ļ���(����㱣��)
	@param[in]: ɾ���ļ��е�·��
    **/
	long RemoveDir(const DISP_PARAMS& rDispParams,CLuaVariant& rVarResult);
	/**
    @brief: ���Ի���(����ʹ��)
	@param[in]: title
	@param[in]��value
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