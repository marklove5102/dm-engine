#include "relivewnd.h"
#include "gamedata/gamedata.h"
#include "GameControl\GameControl.h"
#include "GameData/MsgBoxMgr.h"
#include "Global/Interface/AudioInterface.h"
#include "GameData/MacroDefine.h"
#include "GameData/OtherData.h"

INIT_WND_POSX(CReliveWnd,POS_VARIABLE,POS_VARIABLE)

CReliveWnd::CReliveWnd(void)
{
	SetLifeTime(GetTickCount() + 20000);

	m_bClickedReliveBtn = false;
	m_iLastSecond = 20;
	m_iPages = 1;
	m_iAlignType = XAL_CENTER;
	m_iOffY = 83;
	m_iIndex = 12800;
	m_uStyle &= ~CTRL_STYLE_TRANS;
}

CReliveWnd::~CReliveWnd(void)
{

}


void CReliveWnd::OnCreate()
{
	//复活按钮
	m_pRelive = new CCtrlButton();
	this->AddControl(m_pRelive);
	m_pRelive->CreateEx(this,20,20,12801,12802,12803);
	SetCloseButton(220,3);
}

//单击关闭按钮
void CReliveWnd::OnClickCloseButton()
{
	g_pControl->Msg(MSG_CTRL_QUIT_WND,OPER_CREATE);
	CloseWindow();
}

void CReliveWnd::Draw(void)
{
	if(!m_bClickedReliveBtn )
	{
		CCtrlWindowX::Draw();  
		//背景图片还没有下载下来,其它的东西不画
		if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
		{
			return;
		}
	}

	DWORD iDeadTime = GetTickCount() - g_dwDeathTime;
	int iDelaySecond = 20 - iDeadTime/1000;
	DWORD dwLeftTick = 20000 - iDeadTime;

	if(iDelaySecond <= 0)
		return;

	float fDura = (float)(dwLeftTick % 1000) / 1000;
	const float fScale = 1.5f * fDura + 0.4f * (1.0f-fDura);

	DWORD dwColor = ((DWORD)(0xFF*fDura)<<24) | 0x00FFFFFF;

	if(iDelaySecond == 20)
	{
		g_pTexMgr->PreLoad(PACKAGE_INTERFACE,12810,12820,EP_UI);
	}

	//读秒音效
	if(m_iLastSecond - iDelaySecond >= 1)
	{
		int iSelfX,iSelfY;
		SELF.GetXY(iSelfX,iSelfY);
		g_pAudio->PlayEx(EAT_OTHER,807, g_pAudio->GetRand()++,iSelfX,iSelfX,iSelfY,iSelfY,true);

		m_iLastSecond = iDelaySecond;
	}

	int iInterval = (int)(80*fScale);
	int iLeft = 80-iInterval;

	int x = (g_pGfx->GetWidth() - m_iWidth) / 2;
	int y = (g_pGfx->GetHeight() - m_iHeight) / 2+83;

	LPTexture pTex,pTex1;

	if( iDelaySecond < 10)
	{
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,12810+iDelaySecond,EP_UI);
		if(pTex)
			g_pGfx->DrawTextureFX(x + 48 + iLeft/2,y - m_iY - 20,pTex,dwColor,NULL,&POS(fScale,fScale));
	}
	else
	{

		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,12810+iDelaySecond/10,EP_UI);
		pTex1 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,12810+iDelaySecond%10,EP_UI);

		if(pTex)
			g_pGfx->DrawTextureFX(x+iLeft,y - m_iY-20, pTex,dwColor,NULL,&POS(fScale,fScale));

		if(pTex1)
			g_pGfx->DrawTextureFX(x+iLeft+iInterval,y - m_iY -20, pTex1,dwColor,NULL,&POS(fScale,fScale));
	}
}

bool CReliveWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	if( MSG_CTRL_BUTTON_CLICK == dwMsg)
	{
		if(pControl == m_pRelive && !m_bClickedReliveBtn)
		{ 
			OnReliveImm();
			return true;
		}
		else if(pControl == m_pClose)
		{
			OnClickCloseButton();
			return true;
		}
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CReliveWnd::OnReliveImm()
{
	if(m_bClickedReliveBtn)
		return ;

	m_bClickedReliveBtn = true;

	int pos = SELF.PackageGood().FindGoodByStdmode(3,28);
	if(pos < 0)
	{
		//改成如果没有还魂神符这个窗口不应该创建了,没还魂神符会创建使用元宝复活的界面
		OnClickCloseButton();
		return;

	}
	else //原地复活，如果有物品就马上使用
	{
		DWORD id = SELF.GetPackageGood(pos).GetID();
		g_pGameControl->SEND_Use_ReliveItem(SELF.GetID(),id);

		CloseWindow();
		g_pControl->Msg(MSG_CTRL_QUIT_WND,OPER_CLOSE);
	}

}

void CReliveWnd::ReliveUseYanBao(bool bUse)
{
	DWORD lYuanBao = SELF.GetYuanBao();
	if(bUse && lYuanBao < g_OtherData.GetReLivePrice())
	{
		g_MsgBoxMgr.PopTagAlert(STRING_NOT_ENOUGH_YUANBAO);

		m_bClickedReliveBtn = false;
		OnClickCloseButton();
	}
	else if(bUse)
	{
		g_pGameControl->SEND_Use_ReliveItem(SELF.GetID(),0);
	}
	else
	{
		m_bClickedReliveBtn = false;
		OnClickCloseButton();
	}

}

bool CReliveWnd::OnKeyDown(WORD wState,UCHAR cKey)
{
	if(cKey == VK_RETURN)
	{
		OnReliveImm();
		return true;
	}
	return CCtrlWindowX::OnKeyDown(wState,cKey);
}
