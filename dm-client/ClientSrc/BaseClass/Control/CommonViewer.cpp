#include "CommonViewer.h"
#include "GameData/GameData.h"
#include "Global/Interface/AudioInterface.h"

#ifdef _DEBUG
#include "global/new.h"
#define new DEBUG_NEW
#endif

DTI_IMPLEMENT(CCommonViewer, CControl)

CCommonViewer::CCommonViewer(void)
{
	m_ViewerType = CV_NONE;
	m_dwHPAlarmRand = 0;
}

CCommonViewer::~CCommonViewer(void)
{
}

bool CCommonViewer::Create(CControl * pParent,int iX,int iY,eCOMMON_VIEWER t)
{
	m_ViewerType = t;
	return CControl::Create(pParent,iX,iY,0,0);
}

bool CCommonViewer::CreateXML(CControl* pParent,CXmlControl* ctrl,eCOMMON_VIEWER t)
{
	m_ViewerType = t;
	return CControl::CreateXML(pParent,ctrl);
}

void CCommonViewer::OnCreate()
{

}

void CCommonViewer::SetType(eCOMMON_VIEWER t)
{
	m_ViewerType = t;
}

void CCommonViewer::OnDraw()
{
	switch(m_ViewerType)
	{
	case CV_PANEL_HPMP:
	//case CV_PANEL_HPMP_NEW:
		DrawPanelHPMP();
		break;
	default:
		break;
	}
}

void CCommonViewer::DrawPanelHPMP()
{
	int iHP = SELF.GetHP();
	int iHPMax = SELF.GetHPMax();
	int iMP = SELF.GetMP();
	int iMPMax = SELF.GetMPMax();
	bool bHpOnly = false;
	if (m_ViewerType == CV_PANEL_HPMP && SELF.GetCareer() == 0 && SELF.GetLevel() < 27)
		bHpOnly = true;
	//显示级别数
	LPTexture pLife = NULL;
	LPTexture pMana = NULL;

	if (m_ViewerType == CV_PANEL_HPMP)
	{
		if (!bHpOnly)
		{	
			pLife = g_pTexMgr->GetTex(PACKAGE_INTERFACE,525,EP_UI);
			pMana= g_pTexMgr->GetTex(PACKAGE_INTERFACE,526,EP_UI);
		}
		else
			pLife = g_pTexMgr->GetTex(PACKAGE_INTERFACE,547,EP_UI);
	}
	//else
	//{
	//	pLife = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22050,EP_UI);
	//	pMana= g_pTexMgr->GetTex(PACKAGE_INTERFACE,22051,EP_UI);
	//}


	float fHPRate = CalRate(iHP,iHPMax);
	float fMPRate = CalRate(iMP,iMPMax);

	if (m_ViewerType == CV_PANEL_HPMP)
	{
		if(pMana)
		{
			RECT rc;
			rc.left = 0;
			rc.bottom = pMana->GetHeight();
			rc.right = pMana->GetWidth();
			rc.top = (int)(pMana->GetHeight()*(1.0f - fMPRate));

			DrawTexture(22 + 41,9,pMana,-1,&rc);		
		}

		if(pLife)
		{
			RECT rc;
			rc.left = 0;
			rc.bottom = pLife->GetHeight();
			rc.right = pLife->GetWidth();
			rc.top = (int)(pLife->GetHeight()*(1.0f - fHPRate));

			DrawTexture(22,9,pLife,-1,&rc);		
		}

	}
	//else
	//{
	//	if(pMana)
	//	{
	//		RECT rc;
	//		rc.left = 0;
	//		rc.bottom = pMana->GetHeight();
	//		rc.right = pMana->GetWidth();
	//		rc.top = (int)(pMana->GetHeight()*(1.0f - fMPRate));

	//		DrawTexture(508,21,pMana,-1,&rc);		
	//	}

	//	if(pLife)
	//	{
	//		RECT rc;
	//		rc.left = 0;
	//		rc.bottom = pLife->GetHeight();
	//		rc.right = pLife->GetWidth();
	//		rc.top = (int)(pLife->GetHeight()*(1.0f - fHPRate));

	//		DrawTexture(480,22,pLife,-1,&rc);		
	//	}

	//}


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
		if(m_dwHPAlarmRand >0)
		{
			m_dwHPAlarmRand = 0;
			g_pAudio->Stop(EAT_OTHER,27,m_dwHPAlarmRand);
		}
	}

}

void CCommonViewer::DrawAlarmLight()
{
	//    UpdateLocalState();
	//    LPTexture pTex1 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,710);
	//    LPTexture pTex2 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,711);
	//
	//    if(pTex1 && pTex2)
	//    {
	//        for(int i = 0; i < 3; i++)
	//        {
	//            g_pGfx->DrawTextureNL(m_iX + m_Alarm[i].x,m_iY + m_Alarm[i].y,pTex1);
	//            g_pGfx->DrawTextureNL(m_iX + m_Alarm[i].x,m_iY + m_Alarm[i].y,pTex2,m_dwAlarmColor[m_Alarm[i].dwState]);
	//        }
	//    }
}
void    CCommonViewer::DrawHPMP()
{
	int iHP = SELF.GetHP();
	int iHPMax = SELF.GetHPMax();
	int iMP = SELF.GetMP();
	int iMPMax = SELF.GetMPMax();
	bool bHpOnly = false;
	if (m_ViewerType == CV_PANEL_HPMP && SELF.GetCareer() == 0 && SELF.GetLevel() < 27)
		bHpOnly = true;

	if (m_ViewerType == CV_PANEL_HPMP)
	{
		if (bHpOnly)
		{
			string str = StringUtil::format("%d / %d",iHP,iHPMax);
			TextOut(22 + 15,9 + 40,str.c_str(),COLOR_GREEN,DTF_BlackFrame);
		}
		else
		{
			//绘制分割条
			DrawTexture(22 + 34,9 + 4,546);

			string str = StringUtil::format("%d/%d",iHP,iHPMax);
			if(m_bScale)
			{
				TextOut(22 - 10,9 + 24,str.c_str(),COLOR_GREEN,DTF_BlackFrame);
			}
			else
			{
				TextOut(22 - 10,9 + 44,str.c_str(),COLOR_GREEN,DTF_BlackFrame);
			}

			str = StringUtil::format("%d/%d",iMP,iMPMax);
			TextOut(22 + m_iWidth/2,9 + 44,str.c_str(),COLOR_GREEN,DTF_BlackFrame);
		}
	}
	//else
	//{
	//	if (bHpOnly)
	//	{
	//		string str = StringUtil::format("%d/%d",iHP,iHPMax);
	//		TextOut(506 - g_pFont->GetLen(str.c_str(),NULL,FONTSIZE_DEFAULT),60,str.c_str(),COLOR_GREEN,DTF_BlackFrame);
	//	}
	//	else
	//	{
	//		string str = StringUtil::format("%d/%d",iHP,iHPMax);
	//		if(m_bScale)
	//		{
	//			TextOut(506 - g_pFont->GetLen(str.c_str(),NULL,FONTSIZE_DEFAULT),40,str.c_str(),COLOR_GREEN,DTF_BlackFrame);
	//		}
	//		else
	//		{
	//			TextOut(506 - g_pFont->GetLen(str.c_str(),NULL,FONTSIZE_DEFAULT),60,str.c_str(),COLOR_GREEN,DTF_BlackFrame);
	//		}

	//		str = StringUtil::format("%d/%d",iMP,iMPMax);
	//		TextOut(512,60,str.c_str(),COLOR_GREEN,DTF_BlackFrame);
	//	}
	//	
	//}
}