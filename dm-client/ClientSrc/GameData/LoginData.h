#pragma once

#include "GameDefine.h"
#include "Global/Global.h"

struct _GroupInfo
{
	_GroupInfo(){}

	_GroupInfo(string name,string show)
	{
		strName = name;
		strName_Show = show;
	}
	string strName;//用来登录的
	string strName_Show;//用来显示的
};


class CLoginData
{
public:
	typedef vector<_GroupInfo> VGroupInfo;

	CLoginData();

	VGroupInfo& GetGroupList(){ return m_VGroupList; }
	const char* GetGroupName();
	void SetGroupName_Bak(const char *strName){m_strGroupName_Bak = strName;}
	const char* GetGroupName_Bak(){return m_strGroupName_Bak.c_str();}
	const char* GetGroupName_Show();
	void    SetGroupNo(int i) { m_iGroupNo = i; }
	int		GetGroupNo() { return m_iGroupNo;}
	void    SetGroupNo_Bak(int i) { m_iGroupNo_Bak = i; }
	int		GetGroupNo_Bak() { return m_iGroupNo_Bak;}
	void    SetInnerGroupNo(int i) { m_iInnerGroupNo = i; }
	int		GetInnerGroupNo() { return m_iInnerGroupNo;}

	void    SetAreaNo(int i){ m_iAreaNo = i; }
	int     GetAreaNo()     { return m_iAreaNo; }
	void    SetAreaNo_Bak(int i){ m_iAreaNo_Bak = i; }
	int     GetAreaNo_Bak()     { return m_iAreaNo_Bak; }
	void    SetInnerAreaNo(int i){ m_iInnerAreaNo = i; }
	int     GetInnerAreaNo()     { return m_iInnerAreaNo; }
	void    SetAreaName(const char *strName){ m_strAreaName = strName; }
	const char *  GetAreaName() { return m_strAreaName.c_str(); }
	void    SetRoleName_Bak(const char *strName){ m_strRoleName_Bak = strName; }
	const char *  GetRoleName_Bak() { return m_strRoleName_Bak.c_str(); }

	void    SetLoginID(const char* str){ m_strLoginID.assign(str); }
	const char*     GetLoginID()    { return m_strLoginID.c_str();  }
	void	  SetSessionID(int value ){ m_nCurGroupNO = value; }
	int		GetSessionID()    { return m_nCurGroupNO;  }

	const char* GetServerIP(){ return m_strServerIP.c_str(); }
	int     GetServerPort(){  return m_iServerPort;   }
	void    SetMyIP(const char* ip){ m_strMyIp = ip; }
	void    SetMyPort(int port){ m_iMyPort = port; }
	const char*    GetMyIP(){return m_strMyIp.c_str(); }
	int    GetMyPort(){return  m_iMyPort; }

	void    SetServer(const char* ip,int port);	//设置游戏RUNGATE地址
	void    SetLoginServer(const char* ip,int port);//设置登陆服务器地址
	void    SetKfzLoginString(const string &str);//设置跨服战登陆服务器信息   ptAccount/AreaNo/AreaName/LoginGateIp/LoginGatePort/GroupName/GroupNickName

	const char* GetLoginServer()  { return m_strLoginServerIP.c_str(); }
	int     GetLoginServerPort()  { return m_iLoginServerPort; }
	const char* GetKfzLoginString()  { return m_strKfzLoginString.c_str(); }

	const char* GetCurChar();
	void    SetSelectChar(int i);  //设置选中的角色

	void    SetPoptang_str(const char* str){ m_strPoptang_str.assign(str);}
	const char* GetPoptang_str() { return m_strPoptang_str.c_str(); }
	bool	IsValidPoptang_str() { return (!m_strPoptang_str.empty() && m_strPoptang_str != "0"); }

	const std::string& GetThirdChannel() const	{ return m_strThirdChannel; }
	void SetThirdChannel(const std::string& val){ m_strThirdChannel = val; }
	const std::string& GetThirdUserID() const	{ return m_strThirdUserID; }
	void SetThirdUserID(const std::string& val) { m_strThirdUserID = val; }


	//void    SetPoptang(const char* str){ m_strPoptang.assign(str); }
	//const char* GetPoptang(){    return m_strPoptang.c_str(); }

	//const char*  GetAccount()    { return m_strAccount.c_str(); }
	//void    SetAccount(const char* str,bool pt);
	void    SetAntiCM(bool b)    { m_bIsAntiCM = b; }
	bool    IsAntiCM()           { return m_bIsAntiCM; }
	//bool    IsPoptang()          { return m_bPoptang; }

	bool    IsTuiguang()         { return m_bTuiguang;  }
	const char* GetTuiguang_str(){ return m_strTuiguang.c_str(); }
	void    SetTuiguang(bool b){  m_bTuiguang = b; }
	void    SetTuiguang_str(const char* str){ m_strTuiguang.assign(str); }

	bool    IsLoginSend()         { return m_bLoginSend; }
	bool    IsCharSend()          { return m_bCharSend;  }
	bool    IsGroupSend()         { return m_bGroupSend; }
	void    SetLoginSend(bool b)  { m_bLoginSend = b;    }
	void    SetCharSend(bool b)   { m_bCharSend  = b;    }
	void    SetGroupSend(bool b)  { m_bGroupSend = b;    }

	string& GetAutoUser(){     return m_strAutoUser; }
	string& GetAutoPassword(){ return m_strAutoPassword; }

	string& GetPubKey()         { return m_strPubKey; }
	int     GetLoginGateType(){ return m_iLoginGateType;}
	void    SetLoginGateType(int iType){m_iLoginGateType = iType;}

    bool  IsShowRainBow(){ return m_bShowRainBow;}
    void  SetShowRainBow(bool b){ m_bShowRainBow = b;}
    //void SetEnterCheckType(int iType){ m_iEnterCheckType = iType;}
    //int  GetEnterCheckType(){ return m_iEnterCheckType;}
//-----------------------------IGW--------------------------
	const char* GetPassWord() { return m_strPassWord.c_str();}//获得这个内容之后先逐字节和g_byPwdEncode异或之后再Decode
	void SetPassWord(const char* str) {m_strPassWord.assign(str);}//这里存储的内容是经过encode之后再逐字节和g_byPwdEncode异或之后的内容

	const char* GetEKey() { return m_strEKey.c_str();}
	void SetEKey(const char* str) {m_strEKey.assign(str);}

	int GetLoginType() { return m_nLoginType;}
	void SetLoginType(const int nType) {m_nLoginType = nType;}
//----------------------------------------------------------
	void SetHaveAutoLoginIn(bool b){m_bHaveAutoLoginIn = b;}
	bool IsHaveAutoLoginIn(){return m_bHaveAutoLoginIn;}
	void SetAutoLoginInType(int iType){m_iAutoLoginInType = iType;}
	int GetAutoLoginInType(){return m_iAutoLoginInType;}

	void SetGameIniContent(const string &strContent){m_strGameIni_Bak = strContent;}
	const string &GetGameIniContent(){return m_strGameIni_Bak;}

	const char* GetUsbEKeyChallengeCode() { return m_strUsbEKeyChallengeCode.c_str();}
	void SetUsbEKeyChallengeCode(const char* str) {m_strUsbEKeyChallengeCode.assign(str);}

	void SetIsTianJueMoYu(bool b){m_bTianJueMoYu = b;}
	bool IsTianJueMoYu(){return m_bTianJueMoYu;}

	void SetInHaoFangJingJi(bool b){m_bInHaoFangJingJi = b;}
	bool IsInHaoFangJingJi(){return m_bInHaoFangJingJi;}

	int GetLoginInExpType() const		{ return m_iLoginInExpType; }
	void SetLoginInExpType(int val)	{ m_iLoginInExpType = val; }

	void SetInLoginInExp(bool b)	{ m_bInLoginInExp = b; }
	bool IsInLoginInExp()			{ return m_bInLoginInExp; }

	void SetLoginInExp2AuthCode(const string& s)	{ m_strLoginInExp2AuthCode = s; }
	const string& GetLoginInExp2AuthCode()			{ return m_strLoginInExp2AuthCode; }


	void SetSDOALogin(bool b){m_bSDOALogin = b;}
	bool IsSDOALogin(){ return m_bSDOALogin; }


	void    SetLoginTime(const char *time){ m_strLoginTime = time; }
	const char *  GetLoginTime()		  { return m_strLoginTime.c_str(); }

protected:
	// 登陆服务器信息
	VGroupInfo	m_VGroupList; //组列表

	int         m_iAreaNo;    //区号
	string      m_strAreaName;
	int         m_iGroupNo;   //组号
	int         m_iInnerAreaNo;    //对内区号
	int         m_iInnerGroupNo;   //对内组号
	int         m_iGroupNo_Bak;   //之前选择的组号,比如登录跨服战后要返回原来的服务器,这里记下之前的组号
	string      m_strGameIni_Bak; //登录跨服战之前的game.ini备份
	string      m_strRoleName_Bak; //登录跨服战之前的选择的角色名,登录跨服战服务器时要从那里复制快捷键等设置
	string      m_strGroupName_Bak;//登录跨服战之前的选择的组名,登录跨服战服务器时要从那里复制快捷键等设置
	int         m_iAreaNo_Bak;    //之前选择区号,登录跨服战服务器时要从那里复制快捷键等设置


	string		m_strLoginID;
	int		    m_nCurGroupNO;

	string		m_strServerIP;
	string		m_strMyIp;
	string		m_strCharID;
	int		    m_iServerPort;
	int         m_iMyPort;

	string		m_strLoginServerIP;
	int         m_iLoginServerPort;
	string		m_strKfzLoginString;//  ptAccount/AreaNo/AreaName/LoginGateIp/LoginGatePort/GroupName/GroupNickName
	bool        m_bHaveAutoLoginIn;//是否已经登录gs,收到角色列表后如果此时开门动画还没有播放完,则先设置这个标记,等播完了再登录

	string		m_strPoptang_str; //数字帐号
	//string      m_strPoptang; //PT帐号
	int        m_iAutoLoginInType;//自动登录的类型,1为登录跨服战服务器,2为返回原服务器,3登录仙灵陆,4返回中州大陆,5:从浩方竞技登录游戏,直接到选角色界面
	bool       m_bInHaoFangJingJi;//登录的是否为浩方竞技平台,如果是大退或连接服务器失败,都要直接退出游戏,而不是退到登录界面
	
	int			m_iLoginInExpType;//登录的类型，1为免注册登录，2为登录提前
	bool		m_bInLoginInExp;//以m_iLoginInExpType方式登陆进了游戏，大退或连接服务器失败,都要直接退出游戏,而不是退到登录界面
	string		m_strLoginInExp2AuthCode;//登录提前AuthCode

	BOOL	    m_bJump;

	//bool        m_bPoptang;     //是否PT帐号
	//string		m_strAccount;   //当前登陆时使用的帐号名
	bool        m_bTuiguang;    //推广员
	string		m_strTuiguang;  //推广员信息
	bool        m_bIsAntiCM;    //防沉迷

	bool        m_bLoginSend;   //已经发送登陆消息
	bool        m_bCharSend;    //已经发送选角色消息
	bool        m_bGroupSend;   //已经发送选组消息

	string		m_strAutoUser;  //自动登陆用户名,登录到选区界面
	string		m_strAutoPassword;//自动登陆密码
	string      m_strPubKey;
	int         m_iLoginGateType;//logingate类型:0表示老的,1表示把动态加解密提前到logingate,这时在小退后不要清除动态加解,一直用,大退要清除

    bool	  m_bShowRainBow;//显示彩虹帮助
    //int      m_iEnterCheckType;//登录验证类型，0为选择角色之后进入游戏时弹出的输入验证码，2为进入角色选择界面之间的验证

//-------------------IGW-------------
	string      m_strEKey; //密宝
	string      m_strPassWord; //密码,这里存储的内容是经过encode之后再逐字节和g_byPwdEncode异或之后的内容
	int         m_nLoginType;  //登陆类型
//-----------------------------------
	string      m_strUsbEKeyChallengeCode;//用来获得usb密宝内容的挑战码
	bool        m_bTianJueMoYu;//是否天绝魔域服务器

	bool		m_bSDOALogin;//是否为SDOA登录

	string		m_strLoginTime;//角色登入时间

	string      m_strThirdChannel; //第三方登陆
	string      m_strThirdUserID;  //第三方登陆userid
};

extern CLoginData g_Login;
