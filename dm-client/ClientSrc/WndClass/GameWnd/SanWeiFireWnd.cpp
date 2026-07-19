#include "SanWeiFireWnd.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameData/OtherData.h"
#include "GameAI/AIConfigMgr.h"
#include "Global/Interface/AudioInterface.h"

INIT_WND_POSX(CSanWeiFireWnd,POS_AUTO,POS_AUTO)

char*  CSanWeiFireWnd::s_vParaName[17] = 
{
	"生命值加成",
	"魔法值加成",
	"物攻加成",
	"魔攻加成",
	"道术加成",
	"物防加成",
	"魔防加成",
	"物理命中加成",
	"魔法命中加成",
	"暴击加成",
	"伤害反弹加成",
	"吸血加成",
	"破防加成",
	"抵抗加成",
	"物理躲避加成",
	"魔法躲避加成",
	"幸运加成",
};

char*  CSanWeiFireWnd::s_vJingJieName[9] = 
{
	"下阶青火境",
	"中阶青火境",
	"上阶青火境",
	"下阶石火境",
	"中阶石火境",
	"上阶石火境",
	"下阶天火境",
	"中阶天火境",
	"上阶天火境",
};

CSanWeiFireWnd::CSanWeiFireWnd(void):
m_pLianHua(NULL)
{	
	m_iIndex = 22529;

	m_pShowHelp = NULL;
}

CSanWeiFireWnd::~CSanWeiFireWnd(void)
{
}

void CSanWeiFireWnd::OnCreate()
{	
	SetCloseButton(566,4,80);

	g_pControl->MsgToWnd(MSG_CTRL_PANEL_WND,MSG_CTRL_FLASH_BTN,1007);//停止按钮闪烁

	m_pLianHua = new CCtrlButton;
	AddControl(m_pLianHua);
	m_pLianHua->CreateEx(this,95,400,90,91,92);
	m_pLianHua->SetText("炼 化",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,12);
	m_pLianHua->SetTips("开启三昧真火炼化炉界面");

	m_pShowHelp = new CCtrlButton();
	AddControl(m_pShowHelp);

	if(g_pGameData->GetShowFireLianHuaHelp())
	{
		m_pShowHelp->CreateEx(this,583,210,10709,10710,10711);
		m_pShowHelp->SetTips("单击收回页面。");
	}
	else
	{
		m_pShowHelp->CreateEx(this,583,210,10706,10707,10708);
		m_pShowHelp->SetTips("单击展开页面，查看帮助。");
	}

	m_pMarkViewer = new CMarkViewer(16,35);
	AddControl(m_pMarkViewer);
	m_pMarkViewer->Create(this,616,45,180,400);
	m_pMarkViewer->SetTagText(&m_kHelpText);
	m_pMarkViewer->SetEnable(true);
	m_pMarkViewer->SetShow(g_pGameData->GetShowFireLianHuaHelp());

	string str = g_AICfgMgr.GetFireLianHuaHelpByName("@@main");

	m_kHelpText.Clear();
	m_kHelpText.Parse(str,0,"\\",str.length());
}

void CSanWeiFireWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(m_iScreenX + 275,m_iScreenY + 10,"三昧真火",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG,DTF_BlackFrame);

	if (g_OtherData.GetSanWeiZhenHuoInfo().byFireLevel > 0)
	{
		double percent = 0.0;
		if (g_OtherData.GetSanWeiZhenHuoInfo().dwFireExpMax > 0)
			percent = ((double)g_OtherData.GetSanWeiZhenHuoInfo().dwFireExp) / ((double)g_OtherData.GetSanWeiZhenHuoInfo().dwFireExpMax);		

		if (percent > 0)
		{
			LPTexture zhenhuozhi = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22526,EP_UI);
			g_pGfx->DrawPartTexture(m_iScreenX + 281,m_iScreenY + 96,zhenhuozhi,0,0,(int)(percent * 283));

		}

		g_pGfx->SetRenderMode(RM_ADD2);
		LPTexture zhenhuoEffect = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22572,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 257,m_iScreenY + 62,zhenhuoEffect);
		g_pGfx->SetRenderMode();

		if (g_OtherData.GetSanWeiZhenHuoInfo().wFireMulty > 0 && g_OtherData.GetSanWeiZhenHuoInfo().wFireMulty <= 3)
		{
			LPTexture SanWeiFire = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22584 + g_OtherData.GetSanWeiZhenHuoInfo().wFireMulty,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX + 76,m_iScreenY + 131,SanWeiFire);
		}
		else if (g_OtherData.GetSanWeiZhenHuoInfo().wFireMulty > 3 && g_OtherData.GetSanWeiZhenHuoInfo().wFireMulty <= 6)
		{

			LPTexture SanWeiFire = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22584 + 3,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX + 36,m_iScreenY + 131,SanWeiFire);

			LPTexture SanWeiFireSecond = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22584 + g_OtherData.GetSanWeiZhenHuoInfo().wFireMulty,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX + 116,m_iScreenY + 131,SanWeiFireSecond);
		}
		else if (g_OtherData.GetSanWeiZhenHuoInfo().wFireMulty > 6 && g_OtherData.GetSanWeiZhenHuoInfo().wFireMulty <= 9)
		{

			LPTexture SanWeiFire = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22584 + 3,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX + 36,m_iScreenY + 171,SanWeiFire);

			LPTexture SanWeiFireSecond = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22584 + 6,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX + 116,m_iScreenY + 171,SanWeiFireSecond);

			LPTexture SanWeiFireThird = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22584 + g_OtherData.GetSanWeiZhenHuoInfo().wFireMulty,EP_UI);
			g_pGfx->DrawTextureNL(m_iScreenX + 76,m_iScreenY + 111,SanWeiFireThird);
		}
		
		g_pGfx->SetRenderMode(RM_ADD2);
		LPTexture backGround = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22576,EP_UI);
		g_pGfx->DrawTextureNL(m_iScreenX + 8,m_iScreenY + 24,backGround);
		g_pGfx->SetRenderMode();
	}	

	char ctemp[512] = {0};

	sprintf(ctemp,"三昧真火等级: %u",g_OtherData.GetSanWeiZhenHuoInfo().byFireLevel);
	g_pFont->DrawText(m_iScreenX + 290,m_iScreenY + 210,ctemp,0xffb48c5a,FONT_YAHEI,FONTSIZE_MIDDLE);
	if (g_OtherData.GetSanWeiZhenHuoInfo().wFireMulty > 0 && g_OtherData.GetSanWeiZhenHuoInfo().wFireMulty < 10)
	{
		sprintf(ctemp,"三昧真火境界: %s",s_vJingJieName[g_OtherData.GetSanWeiZhenHuoInfo().wFireMulty - 1]);
		g_pFont->DrawText(m_iScreenX + 290,m_iScreenY + 210 + 30 * 1,ctemp,0xffb48c5a,FONT_YAHEI,FONTSIZE_MIDDLE);
	}
	
	sprintf(ctemp,"三昧真火值: %u / %u",g_OtherData.GetSanWeiZhenHuoInfo().dwFireExp,g_OtherData.GetSanWeiZhenHuoInfo().dwFireExpMax);
	g_pFont->DrawText(m_iScreenX + 290,m_iScreenY + 210 + 30 * 2,ctemp,0xffb48c5a,FONT_YAHEI,FONTSIZE_MIDDLE);

	int count = 0;
	for (int row = 0; row != 17; ++row)
	{
		if (*((DWORD*)(&g_OtherData.GetSanWeiZhenHuoInfo().dwHPAdd + row)) != 0)
		{
			sprintf(ctemp,"%s: %u",s_vParaName[row],*((DWORD*)(&g_OtherData.GetSanWeiZhenHuoInfo().dwHPAdd + row)));
			g_pFont->DrawText(m_iScreenX + 286 + count/7 * 150, m_iScreenY + 308 + 15 * count - count/7 * 105, ctemp,0xffb48c5a, FONT_YAHEI,FONTSIZE_SMALL);
			++count;
		}
	}

	ShowHelp();
}


void CSanWeiFireWnd::ShowHelp()
{
	if(g_pGameData->GetShowFireLianHuaHelp())
	{		
		g_pGfx->DrawTextureNL(m_iScreenX + 596,m_iScreenY + 24,g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,22527,EP_UI));
		m_pMarkViewer->Draw();
	}
}

bool CSanWeiFireWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl/* = NULL*/)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pLianHua)
			{
				if (g_OtherData.GetSanWeiZhenHuoInfo().byFireLevel > 0)
				{
					g_pControl->PopupWindow(MSG_CTRL_FIREARTIFICE_WND, OPER_RECREATE);
				}

				return true;
			}
			else if(pControl == m_pShowHelp)
			{
				g_pGameData->SetShowFireLianHuaHelp(!g_pGameData->GetShowFireLianHuaHelp());			

				if(g_pGameData->GetShowFireLianHuaHelp())
				{
					m_pShowHelp->ReloadTex(10709,10710,10711);
					m_pShowHelp->SetTips("单击收回页面。");

					m_pMarkViewer->SetShow(true);
				}
				else
				{
					m_pShowHelp->ReloadTex(10706,10707,10708);
					m_pShowHelp->SetTips("单击展开页面，查看帮助。");

					m_pMarkViewer->SetShow(false);
				}

				g_pControl->Msg(MSG_INPUT_MOVE,MAKELONG(g_pControl->GetMouseX(),g_pControl->GetMouseY()));

				return true;
			}
		}
		break;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CSanWeiFireWnd::OnMouseMove(int iX,int iY)
{
	CParserTip* pTip = g_pControl->GetTipWnd();
	if(!pTip)
		return true;
	
	pTip->Clear();
	pTip->SetShow(false);
	
	if(iX> 279 && iX < 564 && iY > 93 && iY < 133)
	{
		DWORD dwColor = 0xFFFFCF63;
		char temp[128];
		
		//pTip->AddText("当前三味真火等级:",0xff00ff00);
		//sprintf(temp,"%u重",g_OtherData.GetSanWeiZhenHuoInfo().wFireMulty);
		//pTip->AddText(temp,dwColor);	
		pTip->AddText("当前真火值:",0xff00ff00);
		sprintf(temp,"%u / %u",g_OtherData.GetSanWeiZhenHuoInfo().dwFireExp,g_OtherData.GetSanWeiZhenHuoInfo().dwFireExpMax);
		pTip->AddText(temp,dwColor);
		pTip->AddText("在所有地图中杀怪，都能获得一定的真火值。。",0xff5AA0A0);
		pTip->AddText("在三昧真火修炼场中可以获得更多真火值。",0xff5AA0A0);
		pTip->AddText("释放相应等级的火凝丹也可提升真火值。",0xff5AA0A0);

		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}
	return CCtrlWindowX::OnMouseMove(iX,iY);
}

bool CSanWeiFireWnd::OnLeftButtonUp(int iX, int iY)
{
	if(m_pMarkViewer->IsShow())
	{
		string strCommand = m_pMarkViewer->GetCommand();

		if(!strCommand.empty())
		{
			//up 音乐
			g_pAudio->Play(EAT_OTHER,5,g_pAudio->GetRand()++);
			if(strCommand.substr(0,2) == "@@")
			{
				string str = g_AICfgMgr.GetFireLianHuaHelpByName(strCommand);
				m_kHelpText.Clear();
				m_kHelpText.Parse(str,0,"\\",str.length());
				return true;
			}
		}
	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);	
}

bool CSanWeiFireWnd::IsInControl( int iX,int iY )
{
	if(g_pGameData->GetShowFireLianHuaHelp())
	{
		LPTexture pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,22527,EP_UI);
		if(pTex && pTex->IsPointInTex(iX - 596,iY - 24) == 2)
		{
			return true;
		}

		return CCtrlWindowX::IsInControl(iX,iY);
	}
	else
	{
		return CCtrlWindowX::IsInControl(iX,iY);
	}
}