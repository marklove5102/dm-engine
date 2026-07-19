#include "PlayerPanelWnd.h"
#include "Global/Interface/StreamInterface.h"
#include "GameData/OtherData.h"
#include "Global/Global.h"
#include "GameData/GameData.h"
#include "Global/Interface/AudioInterface.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameData/GameGlobal.h"

INIT_WND_POSX(CPlayerPanelWnd,10,30)

CPlayerPanelWnd::CPlayerPanelWnd(void)
{
	m_iIndex = 11250;
	//m_bNoMove = true;
	m_bNoChangeLevel = true;
	m_bNeedSavePage = false;
	m_bNeedSavePos  = false;
	m_bDisableEscape = true;


	m_dwHPAlarmRand = 0;

}

CPlayerPanelWnd::~CPlayerPanelWnd(void)
{
}

void CPlayerPanelWnd::OnCreate()
{
	CCtrlWindowX::OnCreate();

	SetMask(11250);

	g_OtherData.InitAlarmState();

}

void CPlayerPanelWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	int iSex = SELF.GetSex();
	int iCareer = SELF.GetCareer();
	LPTexture pFaceTex	= g_pTexMgr->GetTex(PACKAGE_INTERFACE,11291 + 3*iSex + iCareer,EP_UI);
	if(pFaceTex)
	{
		g_pGfx->DrawTextureNL(m_iScreenX + 22,m_iScreenY + 26,pFaceTex);
	}	

	RECT rc;
	rc.left = rc.top = 0;
	//LPTexture pWeight	= g_pTexMgr->GetTex(PACKAGE_INTERFACE,11291,EP_UI);
	//if(pWeight && SELF.GetPackageWeightMax() !=0)
	//{
	//	float fPercent = (float) SELF.GetPackageWeight()/SELF.GetPackageWeightMax();
	//	if(fPercent > 1.0)
	//		fPercent = 1.0;

	//	char temp[256] = {0};
	//	sprintf(temp,"%2d%%",(int)((float) SELF.GetPackageWeight()/SELF.GetPackageWeightMax()*100));

	//	rc.right = (int)(fPercent*pWeight->GetWidth0());
	//	rc.bottom = pWeight->GetHeight0();
	//	DrawTexture(603,100,pWeight,-1,&rc);
	//}

	LPTexture pExp	= g_pTexMgr->GetTex(PACKAGE_INTERFACE,11253,EP_UI);
	if(pExp && SELF.GetLevelUpExp() !=0)
	{
		double fPercent = 0.0f;  //显示图形
		if(SELF.GetLevelUpExp() != 0)
		{
			fPercent = (double) SELF.GetExp() / SELF.GetLevelUpExp();
		}

		if(fPercent > 1.0)
			fPercent = 1.0;

		DWORD dwColor = 0xFFFFFFFF;
		if((GetTickCount() - g_OtherData.GetExpUpTime()) < 4000 && (GetTickCount() % 500) > 250)
			dwColor = 0x60FFFFFF;

		rc.right = (int)((pExp->GetWidth0()*fPercent));
		rc.bottom = pExp->GetHeight0();
		DrawTexture(79,87,pExp,-1,&rc);
	}

	//画红和蓝
	int iHP = SELF.GetHP();
	int iHPMax = SELF.GetHPMax();
	int iMP = SELF.GetMP();
	int iMPMax = SELF.GetMPMax();

	//显示级别数
	LPTexture pLife = g_pTexMgr->GetTex(PACKAGE_INTERFACE,11251,EP_UI);
	LPTexture pMana = g_pTexMgr->GetTex(PACKAGE_INTERFACE,11252,EP_UI);

	float fHPRate = CalRate(iHP,iHPMax);
	float fMPRate = CalRate(iMP,iMPMax);

	if(pLife)
	{
		rc.left = 0;
		rc.top = 0;
		rc.bottom = pLife->GetHeight();
		rc.right = (int)(pLife->GetWidth()*fHPRate);

		DrawTexture(95,59,pLife,-1,&rc);		
	}

	if(pMana)
	{
		rc.left = 0;
		rc.top = 0;
		rc.bottom = pMana->GetHeight();
		rc.right = (int)(pMana->GetWidth()*fMPRate);

		DrawTexture(90,73,pMana,-1,&rc);		
	}

	//血量较少时声音报警
	if(fHPRate <= 0.20f && iHP > 0)
	{ 
		if(!SELF.IsDead() && m_dwHPAlarmRand == 0)
		{
			m_dwHPAlarmRand = g_pAudio->GetRand();
			g_pAudio->Play(EAT_OTHER,27,g_pAudio->GetRand()++,true,true);
		}
	}
	else
	{
		if(m_dwHPAlarmRand > 0)
		{
			m_dwHPAlarmRand = 0;
			g_pAudio->Stop(EAT_OTHER,27,m_dwHPAlarmRand);
		}
	}

	//画网络状况及本机性能
	g_OtherData.UpdateLocalState();

	for(int i = 0; i < 3; i++)
	{
		stAlarm &alarm = g_OtherData.GetAlarmState(i);
		DWORD dwColor = g_OtherData.GetAlarmColor(alarm.iState);
		DrawTexture(alarm.x,alarm.y,g_pTexMgr->GetTex(PACKAGE_INTERFACE,711,EP_UI),dwColor);
	}


	//等级
	char strTemp[12] = {0};
	sprintf(strTemp,"%d",SELF.GetLevel());
	m_iFontSize = FONTSIZE_MIDDLE;
	TextOut(24,73,strTemp,0xFFFFFF00,DTF_Center);
	m_iFontSize = FONTSIZE_DEFAULT;


	if(SELF.GetReserveData(plyDrinkSuperRed))
	{
		LPTexture pTex = NULL;
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,11254,EP_UI);
		g_pGfx->SetRenderMode(RM_ADD2);
		DrawTexture(63,28,pTex);
		g_pGfx->SetRenderMode();
	}

	if(SELF.GetReserveData(plyDrinkSuperBlue))
	{
		LPTexture pTex = NULL;
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,11255,EP_UI);
		g_pGfx->SetRenderMode(RM_ADD2);
		DrawTexture(63,41,pTex);
		g_pGfx->SetRenderMode();
	}
}

bool CPlayerPanelWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl )
{
	switch( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			//if(pControl == m_pRainbowButton)
			//{
			//	return true;
			//}
		}
		break;

	default:
		break;
	}

	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CPlayerPanelWnd::OnMouseMove(int iX, int iY)
{
	CCtrlWindowX::OnMouseMove(iX,iY);
	CParserTip *pTip = g_pControl->GetTipWnd();
	int iOldX = iX,iOldY = iY;
	if(m_bScale)
	{
		iOldX = (int)(iOldX / g_ScaleRate.fx + 0.5);
		iOldY = (int)(iOldY / g_ScaleRate.fy + 0.5);
	}

	int x = m_iScreenX + iX - 10;
	int y = m_iScreenY + iY - 20;

	bool bShowTips = false;
	if(IsInExp(iOldX,iOldY))
	{
		pTip->Clear();
		pTip->AddText("经验值:");
		char temp[256] = {0};
		sprintf(temp,"%I64d/%I64d",SELF.GetExp(),SELF.GetLevelUpExp());
		pTip->AddText(temp,-1,-1);

		bShowTips = true;
		x = m_iScreenX + iX - 10;
		y = m_iScreenY + iY - 35;

	}
	//else if(IsInWeight(iOldX,iOldY))
	//{
	//	pTip->Clear();
	//	char temp[256] = {0};
	//	sprintf(temp,"负重:%d/%d",SELF.GetPackageWeight(),SELF.GetPackageWeightMax());
	//	pTip->AddText(temp);

	//	bShowTips = true;
	//}
	else if(int iTemp = InAlarmTip(iOldX,iOldY))
	{
		char sTemp[128];
		stAlarm &alarm = g_OtherData.GetAlarmState(iTemp - 1);
		strcpy(sTemp,alarm.sAlarmName);
		strcat(sTemp,":");
		strcat(sTemp,alarm.sAlarmTip + alarm.iState * 64);
		pTip->Clear();
		pTip->AddText(sTemp); 

		bShowTips = true;
	}
	else if(IsInHP(iX,iY))
	{
		pTip->Clear();
		char temp[256];

		sprintf(temp,"生命值:  %d/%d",SELF.GetHP(),SELF.GetHPMax());
		double per = double(SELF.GetHP()) / double(max(1,SELF.GetHPMax()));
		if( per < 0.1)
			pTip->AddText(temp,0xFFFF0000,false);
		else if( per< 0.2 )
			pTip->AddText(temp,0xFFFF0080,false);
		else if( per < 0.3)
			pTip->AddText(temp,0xFFFF00FF,false);
		else
			pTip->AddText(temp,0xFFFFFFFF,false);

		bShowTips = true;
	}
	else if(IsInMP(iX,iY))
	{
		pTip->Clear();
		char temp[256];

		sprintf(temp,"魔法值:  %d/%d",SELF.GetMP(), SELF.GetMPMax());
		double  per = double(SELF.GetMP()) / double(max(1,SELF.GetMPMax()));
		if( per < 0.1f)
			pTip->AddText(temp,0xFFFF0000,false);
		else if( per< 0.2f)
			pTip->AddText(temp,0xFFFF0080,false);
		else if( per < 0.3f)
			pTip->AddText(temp,0xFFFF00FF,false);
		else
			pTip->AddText(temp,0xFFFFFFFF,false);

		bShowTips = true;
	}
	else if(IsInFace(iX,iY))
	{
		pTip->Clear();
		char temp[256];


		int iCareer = SELF.GetCareer();
		if(iCareer == 0)
			pTip->AddText("职业:战士",0xFF00FF00,false);
		else if(iCareer == 1)
			pTip->AddText("职业:魔法师",0xFF00FF00,false);
		else 
			pTip->AddText("职业:道士",0xFF00FF00,false);

		if(SELF.IsMale())
			sprintf(temp,"性别:男");
		else
			sprintf(temp,"性别:女");

		pTip->AddText(temp);  

		bShowTips = true;
		x = m_iScreenX + iX - 10;
		y = m_iScreenY + iY - 35;
	}

	if (bShowTips)
	{
		pTip->SetShow(true);		
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		return true;
	}

	return true;
}

bool CPlayerPanelWnd::IsInExp(int ix,int iy)
{
	if(ix >= 79 && iy >= 87 && ix <= 79 + 115 && iy <= 87 + 9)
		return true;

	return false;
}

bool CPlayerPanelWnd::IsInWeight(int ix,int iy)
{
	if(ix >= 4 && iy >= 172 && ix <= 4 + 122 && iy <= 172 + 8)
		return true;

	return false;
}

int CPlayerPanelWnd::InAlarmTip(int iX, int iY)
{
	int i = 0;
	for(; i < 3; i++)
	{
		stAlarm &alarm = g_OtherData.GetAlarmState(i);
		if(iX >= alarm.x && iY >= alarm.y && iX < alarm.x + 6 && iY < alarm.y + 6)
			break;
	}

	if(i == 3)
		return 0;
	else
		return i + 1;
}

bool CPlayerPanelWnd::IsInHP(int ix,int iy)
{
	if(ix >= 95 && iy >= 59 && ix <= 95 + 85 && iy <= 59 + 9)
		return true;

	return false;
}

bool CPlayerPanelWnd::IsInMP(int ix,int iy)
{
	if(ix >= 90 && iy >= 73 && ix <= 90 + 98 && iy <= 73 + 9)
		return true;

	return false;
}

bool CPlayerPanelWnd::IsInFace(int ix,int iy)
{
	if(ix >= 21 && iy >= 24 && ix <= 21 + 66 && iy <= 24 + 66)
		return true;

	return false;
}

