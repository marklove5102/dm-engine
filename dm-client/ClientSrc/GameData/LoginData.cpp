#include "LoginData.h"
#include "GameClient/SDOA4ClientEx.h"

CLoginData g_Login;

CLoginData::CLoginData()
{
	m_VGroupList.clear();
	//m_bPoptang = true;
	m_iServerPort=0;
	m_iRoleGatePort=0;
	m_bTuiguang = false;
	m_iAreaNo = 0;
	m_iInnerAreaNo = 0;
	m_iInnerGroupNo = 0;
	m_strMyIp.clear();
	m_iMyPort = 0;
	m_iGroupNo = 0;
	m_iGroupNo_Bak = 0;

	m_bLoginSend = false;
	m_bCharSend = false;
	m_bGroupSend = false;
	m_bHaveAutoLoginIn = false;
	m_iAutoLoginInType = 0;
	m_iLoginGateType = 0;

    m_bShowRainBow = true;
    //m_iEnterCheckType = 0;
	m_bTianJueMoYu = false;
	m_bInHaoFangJingJi = false;
	m_iLoginInExpType = 0;
	m_bInLoginInExp = false;

	m_strAutoUser.clear();
	m_strAutoPassword.clear();
	m_strPubKey.clear();
	m_nLoginType = LOGINTYPE_PTACCOUNT;

	m_bSDOALogin= false;
}

const char* CLoginData::GetGroupName()
{
	if(m_iGroupNo < 0 || m_iGroupNo >= (int)m_VGroupList.size())
		return "";

	return m_VGroupList[m_iGroupNo].strName.c_str();
}

const char* CLoginData::GetGroupName_Show()
{
	if(m_iGroupNo < 0 || m_iGroupNo >= (int)m_VGroupList.size())
		return "";

	return m_VGroupList[m_iGroupNo].strName_Show.c_str();
}

void CLoginData::SetServer(const char* ip,int port)
{
	m_strServerIP.assign(ip);
	m_iServerPort = port;
}

//设置登陆服务器地址
void CLoginData::SetLoginServer(const char* ip,int port)
{
	m_strLoginServerIP.assign(ip);
	m_iLoginServerPort = port;
}

//保存角色网关地址，小退时用于重连角色服
void CLoginData::SetRoleGate(const char* ip,int port)
{
	m_strRoleGateIP.assign(ip);
	m_iRoleGatePort = port;
}

//void CLoginData::SetAccount(const char* str,bool pt)
//{
//	m_strAccount.assign(str);
//	m_bPoptang = pt;
//}

void CLoginData::SetKfzLoginString(const string &str)
{
	m_strKfzLoginString = str;
}
