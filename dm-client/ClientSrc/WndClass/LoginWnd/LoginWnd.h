///////////////////////////////////////////////////////////////////////
//文件名：   .cpp
//版权：上海盛大网络有限公司版权所有
//作者：
//创建日期：
//版本：
//文件说明：
//
//
//
//
//
//
///////////////////////////////////////////////////////////////////////
#pragma once

#include "Global/Global.h"
#include "BaseClass/Control/Control.h"

class CUserLoginWnd;
class CSelectServerWnd;
//class CTodayActivityWnd;

class CLoginWnd : public CCtrlWindow
{
	DTI_DECLARE(CLoginWnd,CCtrlWindow)
public:
	CLoginWnd();
	~CLoginWnd();

	virtual void OnCreate();
	virtual bool Msg(DWORD dwMsg,DWORD dwData,CControl * pControl = NULL);
	virtual void Draw();
	virtual void ResetControlPos();

protected:
	bool OnInput();
	void SetControlState();

	CUserLoginWnd * m_pUserLoginWnd;
	CSelectServerWnd* m_pSelectServerWnd;
	//CTodayActivityWnd* m_pTodayActWnd;
	//LPTexture   m_pCoverTex;
	//LPTexture   m_pSNDATex;
	DWORD       m_dwSndaTexID;
	DWORD       m_dwCoverTime; //开始绘制版权信息的时间
	DWORD       m_dwSndaTime;  //盛大LOGO的开始时间

	//bool        m_bDrawIga;
	//LPDIRECT3DTEXTURE9 m_pIgaTex;//320*240iga纹理
	DWORD       m_dwAutoLoginGsFrame;//自动登录gs的过场动画帧控制
	bool        m_bAutoLgoinStartCartoon;//是否已经登录了,如果正在登录则不显示"正在登录，请稍候..."

	std::string m_strHomeUrl;

	//static void _stdcall IGALiveVideoNotify( void * pParameters);

};
