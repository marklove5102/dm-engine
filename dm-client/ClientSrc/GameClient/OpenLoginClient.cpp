

#include "OpenLoginClient.h"

#include "BaseClass/Control/Control.h"
#include <string>



#ifdef _DEBUG
#pragma comment(lib, "OpenLoginClient.lib")
#else
#pragma comment(lib, "OpenLoginClient.lib")
#endif


COpenLoginClient::COpenLoginClient()
{
	m_pOpenLogin = NULL;
}

COpenLoginClient::~COpenLoginClient()
{
	if (m_pOpenLogin)
	{
		m_pOpenLogin->SetEventHandler(0);
		//m_pOpenLogin->RecoverYYUI();
		m_pOpenLogin->DestroyAllWnd();
		ReleaseHandle();
		m_pOpenLogin = NULL;
	}
}

bool COpenLoginClient::Open(HWND hWnd, void* device)
{
	m_pOpenLogin = OpenLogin::GetOpenLoginModule();
	if (m_pOpenLogin == NULL)
		return false;

	m_pOpenLogin->SetEventHandler(this);

	if (!m_pOpenLogin->CreateLoginWnd(hWnd, (LPDIRECTDRAW_STRUCT*)device))
		return false;



	// 第三方OAuth登录窗口（输入第三方账号密码处）
	//m_pOpenLogin->MoveWindow(OpenLogin::IOpenLogin::WindowType::OAUTHWINDOW, 60, 170);
	// 第三方平台选择条
	//m_pOpenLogin->MoveWindow(OpenLogin::IOpenLogin::WindowType::MORELOGINWINDOW, 800, 200);
	//m_pOpenLogin->MoveWindow(OpenLogin::IOpenLogin::WindowType::LOGINPASSWINDOW, 650, 700);

	return true;
}

void COpenLoginClient::ResetWndPos(int iClientWidth, int iClientHeight)
{
	if (m_pOpenLogin == NULL)
		return;

	if (iClientHeight > 600)
		m_pOpenLogin->MoveWindow(OpenLogin::IOpenLogin::WindowType::OAUTHWINDOW, 60, 170);
	else
		m_pOpenLogin->MoveWindow(OpenLogin::IOpenLogin::WindowType::OAUTHWINDOW, 30, 100);
}

void SDAPI COpenLoginClient::OnSelectLoginType(TCHAR** loginWay)
{
	// 此处添加选择登录类型之后的操作

	if (strcmp((*loginWay),"oa") == 0)
	{
		g_pControl->Msg(MSG_CTRL_ENABLEOPENLOGIN,10);
	}
	else
	{
		g_pControl->Msg(MSG_CTRL_ENABLEOPENLOGIN,11);
	}
}

//void SDAPI COpenLoginClient::OnReceiveYYToken(char** szToken)
//{
//	char buf[512] = {0};
//	memset(buf, 0, 512);
//	sprintf(buf, "%s;%s", *szToken, "yy");
//
//	g_pGameControl->SEND_APLogin_Req("yytoken", buf);
//}

void SDAPI COpenLoginClient::OnReceiveAuthCode(TCHAR** szAuthCode, TCHAR** loginWay)
{
	char buf[512] = {0};
	memset(buf, 0, 512);
	sprintf(buf, "%s;%s", *szAuthCode, *loginWay);	

	std::string code = *szAuthCode;
	if (code == "")
	{
		g_pGameControl->SEND_APLogin_Req((g_strChannelName + "nocode").c_str(), buf);
	}
	else
	{
		g_pGameControl->SEND_APLogin_Req((g_strChannelName + "code").c_str(), buf);

		m_pOpenLogin->ShowSpecifyWindow(OpenLogin::IOpenLogin::WindowType::OAUTHWINDOW, SW_HIDE);
	}
}

bool COpenLoginClient::OnRecLoginAck(char* szAuthAct, char* szAuthResStr)
{
	if (m_pOpenLogin == NULL)
		return false;

	string strNoCode = g_strChannelName + "nocode";
	string strCode = g_strChannelName + "code";

	if (strcmp(szAuthAct, strCode.c_str()) == 0)
	{
		return true;
	}
	//else if (strcmp(szAuthAct, strNoCode.c_str()) == 0)
	//{
	//	if (g_strChannelName == "yy")
	//	{
	//		char * p = szAuthResStr;
	//		m_pOpenLogin->SetYYGenRequest(&p);
	//	}
	//}
	//else if (strcmp(szAuthAct, "yytoken") == 0)
	//{
	//	std::string apid = szAuthResStr;
	//	if (apid != "")
	//	{
	//		m_pOpenLogin->SetLoginState(0);
	//		return true;
	//	}
	//	else
	//	{
	//		m_pOpenLogin->SetLoginState(1);
	//	}
	//}
	return false;
}

//void COpenLoginClient::RenderYYUI()
//{
//	if (m_pOpenLogin)
//		m_pOpenLogin->RenderYYUI();
//}
//
//void COpenLoginClient::RecoverYYUI()
//{
//	if (m_pOpenLogin)
//		m_pOpenLogin->RecoverYYUI();
//}
//
//void COpenLoginClient::YYCallBack(UINT msg, WPARAM wParam, LPARAM lParam)
//{
//	if (m_pOpenLogin)
//		m_pOpenLogin->YYCallBack(msg,wParam,lParam);
//}
