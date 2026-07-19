#pragma once

#include "Global/Global.h"
#include "Global/MathUtil.h"
#include "ContentInterface.h"
#include "WidgetProxy.h"
#include "Global/Interface/TexManagerInterface.h"

enum E_HOSTAPPID
{
	APPID_VOICE_CHAT =     0x00000001,//igw调用客户端语聊
	APPID_C2C        =     0x00000002,//igw调用客户端c2c
};

enum E_WEB_TYPE
{
	EWT_FIRST = 0,          //商城,拍卖行,充值,注册等
	EWT_RADIO,              //内嵌IE电台
	EWT_KFZ_IE,             //跨服战内嵌IE,排名,对阵图
	EWT_HOTACTIVITY,        //热点活动
	EWT_GREAT_EVENT,		//行会大事件
	EWT_INNER_REGISTER,     //内嵌注册
	EWT_TODAYACTIVITY,      //今日活动
	EWT_AUTHEN,				//防沉迷
	EWT_NOTICE,				//游戏公告
	EWT_WATCH_PRIVILEGE,    //查看vip特权
	EWT_LOGIN_CHECK,		//登录校验码
	EWT_CAIHONGHELP,		//彩虹帮助

	EWT_NUM,
};


//通过此类调用Widget接口函数提供的方法来进行操作
//控制大小等

class CWidgetManager
{
public:
	CWidgetManager(void);
	~CWidgetManager(void);

	void SetAuthenURL(const std::string& str){ m_strAuthenURL = str; }
	const std::string& GetAuthenURL(){ return m_strAuthenURL; }

	void LoginPaiMai();
	void LoginPayServer();
	void LoginRealStore();
	void LoginJiFengStore();
	void LoginFengHongStore();
	void LoginRegister();
	void LoginAuthen();
	void LoginTodayActivity();
	void LoginVipPrivilege();
	//void OpenNpcIEUrl(const char* pUrl);
	void LoginRadio(const char* strRadioUrl);
	void LogoutRadio();
	void LoginKfzRange();
	void LoginKfzVSMap();
	void LoginTopRange();
	void LoginXLRange();
	void LoginHotActivity();
	void LoginCheckMask(const char* pUrl);
	void LoginGuildGreatEvent(int iArea,int iGroup,string strGuildName);
	void LoginNoticeIE(const string& url);
	void LoginCaiHong(int iArea, int iGroup, const std::string& strChannelName);

	bool Init();
	void NewWidgetContent(E_WEB_TYPE t);
	void Release();
	bool IsShow(E_WEB_TYPE t){ return m_bShow[t]; }
	void SetShow(E_WEB_TYPE t,bool b){ m_bShow[t] = b; }
	void SetFocus(E_WEB_TYPE t,bool b){ m_bFocus[t] = b; }
	void Navigate(E_WEB_TYPE t,const char* szUrl);
	void MoveWindow(E_WEB_TYPE t,int x,int y,int w = 0,int h = 0); //设置窗口的位置
	BOOL OnWindowMessage(DWORD dwMsg,WPARAM wParam,LPARAM lParam,LRESULT& lRet);
	void RenderWidget(E_WEB_TYPE t); //绘制当前的Widget的内容

	void OnHostApp(DWORD AppID,LPARAM lParam);
	void OnDocComplete(const char * strFileName,const char * strFullUrl,CWidgetProxy *pControl); //显示窗口
	void OnTitleChange(const char* str,CWidgetProxy *pControl);
	void OnGUID(const char *pGuidStr); //返回GUID
	void OnSNDAMark(DWORD iNum); //您的可用积分
	void OnCloseQuery(BOOL& CanClose, CWidgetProxy *pControl);

	bool ExecScript(E_WEB_TYPE t, const char* script);

private:
	CWidgetProxy*   m_pWindowProxy[EWT_NUM];
	//CHtmlContent*   m_pWidgetContent[EWT_NUM];
	SRect           m_rcBound[EWT_NUM];
	LPTexture       m_pWidgetTex[EWT_NUM];
	bool            m_bShow[EWT_NUM]; //显示或者隐藏
	bool            m_bFocus[EWT_NUM]; //获得焦点

	std::string		m_strAuthenURL;
};

extern CWidgetManager g_WidgetMgr;
