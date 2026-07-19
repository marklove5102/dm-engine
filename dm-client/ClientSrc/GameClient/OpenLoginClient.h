#pragma once
#include "GameControl/GameControl.h"
#include "IOpenLoginClient.h"

using namespace OpenLogin;

#ifdef WIN32
#define SDAPI  __stdcall
#endif

class COpenLoginClient : public IEventHandler
{
public: 
	COpenLoginClient();
	~COpenLoginClient();

	virtual void SDAPI OnSelectLoginType(TCHAR** loginWay);
	virtual void SDAPI OnReceiveAuthCode(TCHAR** szAuthCode, TCHAR** loginWay);
	//virtual void SDAPI OnReceiveYYToken(TCHAR** szToken);

	bool Open(HWND hWnd, void* device);

	//void RenderYYUI();
	//void RecoverYYUI();
	//void YYCallBack(UINT msg, WPARAM wParam, LPARAM lParam);

	// 返回是否能进入游戏
	bool OnRecLoginAck(char* szAuthAct, char* szAuthResStr);

	void ResetWndPos(int iClientWidth, int iClientHeight);

protected:
	IOpenLogin * m_pOpenLogin;
};