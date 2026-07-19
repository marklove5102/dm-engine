#include "WuXingPrizeWnd.h"
#include "GameData/NpcData.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "GameData/TipsCfg.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/OtherData.h"
#include "Baseclass/Control/ParserTip.h"

INIT_WND_POSX(CWuXingPrizeWnd,POS_AUTO,POS_AUTO)

CWuXingPrizeWnd::CWuXingPrizeWnd()
{
	m_bModel = false;
    m_sName = "WuXingPrizeWnd";

	m_iIndex = 15183;
	m_iAlignType = XAL_TOPLEFT;
    m_iType = sm_dwWindowType;  

    m_pOKButton = m_pChangeLiHuoLing = m_pCancelButton = NULL;

	m_bIsJinKa = SELF.GetVipCardType() == 2 ? true : false;
}

CWuXingPrizeWnd::~CWuXingPrizeWnd(void)
{
    if(m_iType == 2)        //询问窗口不记录位置
    {
        g_pControl->DelSPos(m_sName.c_str());
    }
}

void CWuXingPrizeWnd::OnCreate()
{
	//确定按钮
    if(m_iType == 1 || m_iType == 3)
    {
        m_pOKButton = new CCtrlButton();
	    AddControl(m_pOKButton);	    
		m_pOKButton->CreateEx(this,165,225,132,133,134,135);
		if (m_iType == 1)
		{
			m_pOKButton->SetText("领取酬劳");
		}
		else
		{
			m_pOKButton->SetText("离开");
		}
    }
    else if(m_iType == 2)
    {
        m_pOKButton = new CCtrlButton();
        AddControl(m_pOKButton);
		m_pOKButton->CreateEx(this,75,225,132,133,134,135);
		m_pOKButton->SetText("再来一次");

		m_pChangeLiHuoLing = new CCtrlButton();
		AddControl(m_pChangeLiHuoLing);
		m_pChangeLiHuoLing->CreateEx(this,164,225,132,133,134,135);
		m_pChangeLiHuoLing->SetText("兑换离火令");

        m_pCancelButton = new CCtrlButton();
        AddControl(m_pCancelButton);		
		m_pCancelButton->CreateEx(this,253,225,132,133,134,135);
		m_pCancelButton->SetText("离开");
    }
}

void CWuXingPrizeWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	char temp[128] = "";
	if (m_iType == 1 || m_iType == 2)
	{
		_WuXingResult& result = g_NPC.GetWuXingResult();

		LPTexture pTex1 = NULL;
		if(result.bySuccess == 0)
		{
			pTex1 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15594,EP_UI);
		}
		else
		{
			pTex1 = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15595,EP_UI);
		}
		if(pTex1)
		{
			g_pGfx->SetRenderMode(RM_ADD1);
			g_pGfx->DrawTextureNL(m_iScreenX + 6,m_iScreenY - 9,pTex1);
			g_pGfx->SetRenderMode();
		}

		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,15187 + result.bySuccess,EP_UI);
		if(pTex)
		{
			g_pGfx->DrawTextureNL(m_iScreenX + 170 - 17,m_iScreenY + 50,pTex);
		}

		g_pFont->DrawText(m_iScreenX + 150 - 17,m_iScreenY + 200,"所用时间： ",0xffffff00,FONT_DEFAULT,FONTSIZE_MIDDLE);
		sprintf(temp,"%d 分 %d 秒",result.dwTime / 60,result.dwTime % 60);
		g_pFont->DrawText(m_iScreenX + 230 - 17,m_iScreenY + 200,temp,0xffe28819,FONT_DEFAULT,FONTSIZE_MIDDLE);

		g_pFont->DrawText(m_iScreenX + 150 - 17,m_iScreenY + 90,"逃跑怪物：",0xffffff00,FONT_DEFAULT,FONTSIZE_MIDDLE);
		sprintf(temp,"%d 个",result.dwMonster);
		g_pFont->DrawText(m_iScreenX + 230 - 17,m_iScreenY + 90,temp,0xffff1a13,FONT_DEFAULT,FONTSIZE_MIDDLE);

		int iStartY = 110;
		int iStartX = 230;
		if(result.bySuccess == 0 && result.strPrize.size() > 0)
		{
			g_pFont->DrawText(m_iScreenX + 130 - 17,m_iScreenY + 110,"很遗憾，你只能获得酬劳：",0xffffff00,FONT_DEFAULT,FONTSIZE_MIDDLE);
			iStartY = 130;
			iStartX = 180;
		}
		else if(result.bySuccess == 0 && result.strPrize.size() == 0)
		{
			g_pFont->DrawText(m_iScreenX + 130 - 17,m_iScreenY + 110,"很遗憾，你未能获得酬劳。",0xffffff00,FONT_DEFAULT,FONTSIZE_MIDDLE);			
		}
		else
		{
			g_pFont->DrawText(m_iScreenX + 150 - 17,m_iScreenY + 110,"酬    劳：",0xffffff00,FONT_DEFAULT,FONTSIZE_MIDDLE);
		}

		for(int i = 0;i < result.strPrize.size();i++)
		{
			g_pFont->DrawText(m_iScreenX + iStartX - 17,m_iScreenY + iStartY + i * 20,result.strPrize[i].c_str(),0xffff1a13,FONT_DEFAULT,FONTSIZE_MIDDLE);
		}

		if(result.strPrize.size() > 0 && m_bIsJinKa && m_iType == 1)
		{
			g_pFont->DrawText(m_iScreenX + 65,m_iScreenY + 165,"由于你是龙骧金卡成员，本次经验奖励额外增加20%",0xffff0000,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_Bold);
		}
	}
	else if (m_iType == 3)
	{
		sTXZG &txzg = g_OtherData.GetTXZG();
    	sprintf(temp,"当前等级：%d",SELF.GetLevel());
		g_pFont->DrawText(m_iScreenX + 170,m_iScreenY + 53,temp,0xffffff00,FONT_YAHEI,FONTSIZE_BIG);
		if (txzg.byVictorySide == txzg.bySide)
		{
			g_pFont->DrawText(m_iScreenX + 88,m_iScreenY + 95,"本局为获胜一方：",0xffeae677,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);
		}
		else if (txzg.byVictorySide == 255)
		{
			g_pFont->DrawText(m_iScreenX + 88,m_iScreenY + 95,"本局结果为平局：",0xffeae677,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);
		}
		else
		{
			g_pFont->DrawText(m_iScreenX + 88,m_iScreenY + 95,"本局为失败一方：",0xffeae677,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);
		}

		LPTexture pTex = g_pTexMgr->GetTexImm(PACKAGE_items,5100,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 88 + 130,m_iScreenY + 85,pTex);

		sprintf(temp,"* %d",txzg.wGetExp);
		g_pFont->DrawText(m_iScreenX + 88 + 167,m_iScreenY + 95,temp,0xff0000ff,FONT_YAHEI,FONTSIZE_BIG);

		g_pFont->DrawText(m_iScreenX + 88 + 48,m_iScreenY + 105 + 23,"个人贡献：",0xffeae677,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);
		sprintf(temp," 经验 * %d",txzg.wContributeExp*1000);
		g_pFont->DrawText(m_iScreenX + 88 + 120,m_iScreenY + 105 + 23,temp,0xff0000ff,FONT_YAHEI,FONTSIZE_BIG);

	}

}

bool CWuXingPrizeWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(m_iType == 1)
			{
				if(pControl == m_pOKButton)
				{
					g_pGameControl->SEND_WuXing_Request_Prize();					
				}
			}
			else if(m_iType == 2)
			{
				if(pControl == m_pOKButton)
				{
					g_pGameControl->SEND_PLAY_WXXG(1);	
					g_pControl->PopupWindow(MSG_CTRL_WUXING_PRIZE_WND,OPER_CLOSE);	//关闭窗口
					return true;
				}
				else if(pControl == m_pChangeLiHuoLing)
				{
					if (SELF.PackageGood().FindGoodByStdmode(38,3) < 0)
					{
						g_MsgBoxMgr.PopSimpleAlert("你包裹中没有离火令，无法兑换经验值，离火令可通过完成各种任务获取。");
					}
					else
					{
						g_pGameControl->SEND_PLAY_WXXG(2); //请求兑换离火令   
						m_pChangeLiHuoLing->SetEnable(false);
					}

					return true;
				}
				else if(pControl == m_pCancelButton)
				{
					g_pGameControl->SEND_PLAY_WXXG(0);	
					g_pControl->PopupWindow(MSG_CTRL_WUXING_PRIZE_WND,OPER_CLOSE);	//关闭窗口
					return true;
				}
			}
			else if(m_iType == 3)
			{
				if(pControl == m_pOKButton)
				{
					CloseWindow();
					return true;
				}
			}
		}
		break;
	case MSG_CTRL_WUXING_PRIZE_WND_CLOSE:
		if (m_iType != 3)
		{
			CloseWindow();
		}
		return true;
		break;

	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CWuXingPrizeWnd::OnMouseMove(int iX,int iY)
{
	CParserTip *pTip = g_pControl->GetTipWnd();

	pTip->Clear();
	if(m_iType == 3 && iX > 88 + 130 && iX < 88 + 130 + 32 && iY > 85 && iY < 85 + 32)
	{
		pTip->AddText("铁血战场勋章",TIPS_GOODNAME,-1,0,-1,FONT_DEFAULT,FONTSIZE_MIDDLE);
		pTip->AddText("重量1",TIPS_WHITE);

		//配置物品说明
		sTipsCfg * pTipCfg = g_TipsCfg.GetCfgTips("铁血战场勋章");
		if(pTipCfg)
		{
			pTip->AddCfgTips(pTipCfg);
		}


		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}
	else
	{
		pTip->SetShow(false);
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}