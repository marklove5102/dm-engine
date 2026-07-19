#include "BlessCompoundWnd.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameData/GameData.h"
#include "GameControl/GameControl.h"
#include "GameData/NpcData.h"
#include "GameData/TalkMgr.h"
#include "GameData/MsgBoxMgr.h"
#include "Baseclass/Control/ParserTip.h"
#include "GameAI/AIGoodMgr.h"
#include "GameData/MagicDefine.h"


INIT_WND_POSX(CBlessCompoundWnd,POS_AUTO,POS_AUTO)


CBlessCompoundWnd::CBlessCompoundWnd(void)
{
	m_pOK = 0;
	m_pCancle = 0;

	m_iType = sm_dwWindowType;
	m_iMsg = -1;

	if (m_iType == EBCT_SYHF)
	{
		m_iIndex = 22286;
	}
	else if (m_iType == EBCT_JLL || m_iType == EBCT_BJRH)
	{
		m_iIndex = 22305;
	}
	else
	{
		m_iIndex = 22285;
	}


	m_iSuccessFrame = 10000;
	m_iFailFrame = 10000;
	m_iOneLineShowNum = 0;
	
	m_iRotFrame = -1;

	//预读
	if (m_iType == EBCT_JLL)
	{
		g_pTexMgr->PreLoad(PACKAGE_INTERFACE,22315,22331,EP_UI);
		g_pTexMgr->PreLoad(PACKAGE_INTERFACE,22335,22351,EP_UI);
	}
	else
	{
		g_pTexMgr->PreLoad(PACKAGE_INTERFACE,4722,4732,EP_UI);
	}	
}


CBlessCompoundWnd::~CBlessCompoundWnd(void)
{
	if(g_NPC.GetBlessGoodLeft().GetID()!=0)
	{
		SELF.PackageGood().BackToArray(g_NPC.GetBlessGoodLeft());
		g_NPC.GetBlessGoodLeft().SetID(0);
	}

	if(g_NPC.GetBlessGoodRight().GetID()!=0)
	{
		SELF.PackageGood().BackToArray(g_NPC.GetBlessGoodRight());
		g_NPC.GetBlessGoodRight().SetID(0);
	}

	if(g_NPC.GetBlessGoodFu().GetID()!=0)
	{
		SELF.PackageGood().BackToArray(g_NPC.GetBlessGoodFu());
		g_NPC.GetBlessGoodFu().SetID(0);
	}
}

void CBlessCompoundWnd::OnCreate()
{
	SetCloseButton(332,35, 80);
	
	m_pOK = new CCtrlButton;
	m_pOK->CreateEx(this,85,412,90,91,92,93);
	if (m_iType == EBCT_SYHF)
	{
		m_pOK->SetText("确认恢复",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
	}
	else if (m_iType == EBCT_JLL)
	{
		m_pOK->SetText("纹佩精炼",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
	}
	else if (m_iType == EBCT_BJRH)
	{
		m_pOK->SetText("纹佩融合",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
	}	
	else
	{
		m_pOK->SetText("开始融合",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
	}
	AddControl(m_pOK);

	m_pCancle = new CCtrlButton;
	m_pCancle->CreateEx(this,240,412,90,91,92,93);
	m_pCancle->SetText("取 消",COLOR_BTN_NORMAL,COLOR_BTN_MOUSEON,COLOR_BTN_PRESS,COLOR_BTN_DISABLE,FONTSIZE_DEFAULT,DTF_BlackFrame);
	AddControl(m_pCancle);
}


bool CBlessCompoundWnd::Msg(DWORD dwMsg, DWORD dwData, CControl *pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BLESSCOMPOUND_WND:
		{
			WORD wResult = HIWORD(dwData);
			WORD wType = LOWORD(dwData);

			/*
			类型 26 合成
			结果 0：成功
				 1：失败
				 2：非法物品
				 3：不是神佑或者神佑不具备加属性的特性
				 4：绑定的神佑
				 5：附加物品不合法
				 6：神佑未激活

			 类型27  延长时间
			 结果 0：成功
				 2：非法物品
				 3：不是神佑或者神佑不具备加属性的特性
				 4：绑定的神佑
				 5：附加物品不合法
				 6：神佑未激活

			 类型28 纹佩强化
			 结果 0：成功
				  1：失败
			*/
			
			if ((m_iType == EBCT_SXRH && wType == 26)
				|| (m_iType == EBCT_GNRH && wType == 26)
				|| (m_iType == EBCT_SYHF && wType == 27)
				|| (m_iType == EBCT_JLL && wType == 28)
				|| (m_iType == EBCT_BJRH && wType == 40)
				)
			{
				m_iMsg = wResult;
			}		
			
			if ((m_iType == EBCT_JLL && wType == 28)
				|| (m_iType == EBCT_BJRH && wType == 40)
				)
			{
				if (wResult == 0)
				{
					m_iSuccessFrame = 0;
				}
				else
				{
					m_iFailFrame = 0;
				}
			}
			else
			{
				m_iSuccessFrame = 0;
			}

			m_iRotFrame = -1;

			return true;
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		{
			if (pControl == m_pOK)
			{
				if (m_iType == EBCT_SXRH)
				{
					if (g_NPC.GetBlessGoodLeft().GetID() != 0 && g_NPC.GetBlessGoodRight().GetID() != 0)
					{
						g_pGameControl->Send_BlessCompound(g_NPC.GetBlessGoodLeft().GetID(), 
							g_NPC.GetBlessGoodRight().GetID(), g_NPC.GetBlessGoodFu().GetID());
					}
				}
				else if (m_iType == EBCT_GNRH)
				{
					if (g_NPC.GetBlessGoodLeft().GetID() != 0 && g_NPC.GetBlessGoodRight().GetID() != 0 && g_NPC.GetBlessGoodFu().GetID() != 0)
					{
						g_pGameControl->Send_BlessCompound(g_NPC.GetBlessGoodLeft().GetID(), 
							g_NPC.GetBlessGoodRight().GetID(), g_NPC.GetBlessGoodFu().GetID());
					}
				}
				else if (m_iType == EBCT_SYHF)
				{
					if (g_NPC.GetBlessGoodLeft().GetID() != 0 && g_NPC.GetBlessGoodRight().GetID() != 0)
					{
						g_pGameControl->Send_BlessExtend(g_NPC.GetBlessGoodLeft().GetID(), g_NPC.GetBlessGoodRight().GetID());
					}
				}
				else if (m_iType == EBCT_JLL)
				{
					if (m_iRotFrame == -1)
					{
						if (g_NPC.GetBlessGoodLeft().GetID() != 0 && g_NPC.GetBlessGoodFu().GetID() != 0)
							m_iRotFrame = 0;
					}
				}
				else if (m_iType == EBCT_BJRH)
				{
					if (m_iRotFrame == -1)
					{
						if (g_NPC.GetBlessGoodLeft().GetID() != 0 && g_NPC.GetBlessGoodRight().GetID() != 0)
							m_iRotFrame = 0;
					}
				}
				
				return true;
			}
			else if (pControl == m_pCancle)
			{
				OnClickCloseButton();
				return true;
			}
		}
		break;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}


void CBlessCompoundWnd::ShowBlessTips(const char* str, int val)
{
	char str2[64] = {0};
	if (val != 0)
	{
		sprintf(str2,str,val);
		strcat(m_str,str2);
		m_iOneLineShowNum++;
	}

	if (m_iOneLineShowNum >= 3)
	{
		m_iShowStartY += 15;
		g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + m_iShowStartY,m_str,0xFF000000,FONT_DEFAULT,FONTSIZE_DEFAULT);
		memset(m_str, 0, 256);
		m_iOneLineShowNum = 0;
	}
}


void CBlessCompoundWnd::Draw()
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	if (m_iType == EBCT_SXRH)	
		g_pFont->DrawText(m_iScreenX + 165,m_iScreenY + 37,"属性融合",0xFF550F0F,FONT_YAHEI,16,DTF_BlackFrame,0,0xFFE0920C);
	else if (m_iType == EBCT_GNRH)
		g_pFont->DrawText(m_iScreenX + 165,m_iScreenY + 37,"功能融合",0xFF550F0F,FONT_YAHEI,16,DTF_BlackFrame,0,0xFFE0920C);
	else if (m_iType == EBCT_SYHF)
		g_pFont->DrawText(m_iScreenX + 165,m_iScreenY + 37,"神佑恢复",0xFF550F0F,FONT_YAHEI,16,DTF_BlackFrame,0,0xFFE0920C);
	else if (m_iType == EBCT_JLL)
		g_pFont->DrawText(m_iScreenX + 165,m_iScreenY + 37,"纹佩精炼炉",0xFF550F0F,FONT_YAHEI,16,DTF_BlackFrame,0,0xFFE0920C);
	else if (m_iType == EBCT_BJRH)
		g_pFont->DrawText(m_iScreenX + 165,m_iScreenY + 37,"纹佩融合炉",0xFF550F0F,FONT_YAHEI,16,DTF_BlackFrame,0,0xFFE0920C);


	if (m_iType == EBCT_JLL || m_iType == EBCT_BJRH)
	{
		if (m_iRotFrame >= 0)
		{			
			LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22307,EP_UI);
			if(pTex)
			{
				g_pGfx->DrawTextureFX(m_iScreenX + 199,m_iScreenY + 205,pTex,0xFFFFFFFF,0,0,0.0f,0.0f,m_iRotFrame * 8.0f);
			}

			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22306,EP_UI);
			if(pTex)
			{
				g_pGfx->DrawTextureNL(m_iScreenX + 113 ,m_iScreenY + 183,pTex);
			}

			++m_iRotFrame;

			if (m_iRotFrame == 360/4)
			{
				m_iRotFrame = -1;
				if (m_iType == EBCT_JLL)
				{
					if (g_NPC.GetBlessGoodLeft().GetID() != 0 && g_NPC.GetBlessGoodFu().GetID() != 0)
					{
						g_pGameControl->SEND_WenPei_Streng(g_NPC.GetBlessGoodLeft().GetID(),g_NPC.GetBlessGoodFu().GetID(),g_NPC.GetBlessGoodRight().GetID());

						SELF.PackageGood().BackToArray(g_NPC.GetBlessGoodLeft());
						g_NPC.GetBlessGoodLeft().SetID(0);
					}
				}
				else if (m_iType == EBCT_BJRH)
				{
					if (g_NPC.GetBlessGoodLeft().GetID() != 0 && g_NPC.GetBlessGoodRight().GetID() != 0)
					{
						g_pGameControl->SEND_WenPei_RongHe(g_NPC.GetBlessGoodLeft().GetID(),g_NPC.GetBlessGoodRight().GetID(),g_NPC.GetBlessGoodFu().GetID());
					}
				}
			}
		}
	}

	if(g_NPC.GetBlessGoodLeft().GetID()!=0)
		CGoodGrid::DrawOneGood(m_iScreenX+113+20, m_iScreenY+186+20, g_NPC.GetBlessGoodLeft());

	if(g_NPC.GetBlessGoodRight().GetID()!=0)
		CGoodGrid::DrawOneGood(m_iScreenX+245+20, m_iScreenY+186+20, g_NPC.GetBlessGoodRight());

	if(g_NPC.GetBlessGoodFu().GetID()!=0)
	{
		//如果有32*32的图下面删除
		POS size(1.0f, 1.0f);
		LPPOS pScale = NULL;
		int iType = 1;
		if (g_AIGoodMgr.IsWenPei(g_NPC.GetBlessGoodFu()))
		{
			size.fx = (32/72.0f);
			size.fy = (32/72.0f);

			pScale = &size;

			iType = 2;
		}
		CGoodGrid::DrawOneGood(m_iScreenX+179+20, m_iScreenY+186+20, g_NPC.GetBlessGoodFu(),pScale,iType);
		//------------------------

		//CGoodGrid::DrawOneGood(m_iScreenX+179+20, m_iScreenY+186+20, g_NPC.GetBlessGoodFu());
	}

	if (m_iType == EBCT_SXRH || m_iType == EBCT_GNRH) 
	{
		if (m_iMsg == -1)
		{
			if (m_iType == EBCT_GNRH)
			{
				int istart1 = 330;
				if(g_NPC.GetBlessGoodLeft().GetID()==0)
				{
					istart1 += 15;
					g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + istart1,"请在功能融合界面左侧位置放入玉兔神佑；",0xFF000000,FONT_DEFAULT,FONTSIZE_DEFAULT);
				}
				if(g_NPC.GetBlessGoodRight().GetID()==0)
				{
					istart1 += 15;
					g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + istart1,"请在功能融合界面右侧位置放入打宝神佑；",0xFF000000,FONT_DEFAULT,FONTSIZE_DEFAULT);
				}
				if(g_NPC.GetBlessGoodFu().GetID()==0)
				{
					istart1 += 15;
					g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + istart1,"请在功能融合界面中间位置放入玉兔令；",0xFF000000,FONT_DEFAULT,FONTSIZE_DEFAULT);
				}

				if ((g_NPC.GetBlessGoodLeft().GetID()!=0)
					&& (g_NPC.GetBlessGoodRight().GetID()!=0)
					&& (g_NPC.GetBlessGoodFu().GetID()!=0))
				{
					g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"融合有概率成功",0xFF000000,FONT_DEFAULT,FONTSIZE_DEFAULT);
					g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 360,"你确认要将玉兔神佑和打宝神佑进行融合吗？",0xFF000000,FONT_DEFAULT,FONTSIZE_DEFAULT);
				}
			}
			else if (m_iType == EBCT_SXRH)
			{
				if ((g_NPC.GetBlessGoodLeft().GetID()!=0) && (g_NPC.GetBlessGoodRight().GetID()!=0))
				{
					g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 338,"合成成功后将获的神佑属性有可能是",0xFF000000,FONT_DEFAULT,FONTSIZE_DEFAULT);

					
					m_iOneLineShowNum = 0;									
					m_iShowStartY = 338;

					memset(m_str, 0, 256);
					
					int iTemp = g_NPC.GetBlessGoodLeft().GetBlessAC() + g_NPC.GetBlessGoodRight().GetBlessAC();
					ShowBlessTips("攻击+%d ", iTemp);

					iTemp = g_NPC.GetBlessGoodLeft().GetBlessMC() + g_NPC.GetBlessGoodRight().GetBlessMC();
					ShowBlessTips("魔法攻击+%d ", iTemp);

					iTemp = g_NPC.GetBlessGoodLeft().GetBlessDC() + g_NPC.GetBlessGoodRight().GetBlessDC();
					ShowBlessTips("道术攻击+%d ", iTemp);
					
					iTemp = (g_NPC.GetBlessGoodLeft().GetBlessAddHP() + g_NPC.GetBlessGoodRight().GetBlessAddHP()) * 10;
					ShowBlessTips("生命值+%d ", iTemp);

					iTemp = g_NPC.GetBlessGoodLeft().GetBlessDefend() + g_NPC.GetBlessGoodRight().GetBlessDefend();
					ShowBlessTips("暴击+%d ", iTemp);

					iTemp = g_NPC.GetBlessGoodLeft().GetBlessMDefend() + g_NPC.GetBlessGoodRight().GetBlessMDefend();
					ShowBlessTips("破防+%d ", iTemp);

					iTemp = g_NPC.GetBlessGoodLeft().GetBlessAttackAvoid() + g_NPC.GetBlessGoodRight().GetBlessAttackAvoid();
					ShowBlessTips("抵抗+%d ", iTemp);

					iTemp = g_NPC.GetBlessGoodLeft().GetBlessAttackHit() + g_NPC.GetBlessGoodRight().GetBlessAttackHit();
					ShowBlessTips("吸血+%d ", iTemp);

					iTemp = g_NPC.GetBlessGoodLeft().GetBlessMagicHit() + g_NPC.GetBlessGoodRight().GetBlessMagicHit();
					ShowBlessTips("反弹+%d ", iTemp);

					iTemp = g_NPC.GetBlessGoodLeft().GetBlessMagicAvoid() + g_NPC.GetBlessGoodRight().GetBlessMagicAvoid();
					ShowBlessTips("魔法躲避+%d%% ", iTemp);

					if (m_iOneLineShowNum > 0)
					{
						m_iShowStartY += 15;
						g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + m_iShowStartY,m_str,0xFF000000,FONT_DEFAULT,FONTSIZE_DEFAULT);
					}
				}
				else
				{
					g_pFont->DrawText(m_iScreenX + 60,m_iScreenY + 345,"请在属性融合界面左侧和右侧位置放入玉兔神佑；",0xFF000000,FONT_DEFAULT,FONTSIZE_DEFAULT);
					g_pFont->DrawText(m_iScreenX + 60,m_iScreenY + 360,"如果融合失败，右侧的玉兔神佑将会消失；",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);
					g_pFont->DrawText(m_iScreenX + 60,m_iScreenY + 375,"你确定要进行融合吗？",0xFF000000,FONT_DEFAULT,FONTSIZE_DEFAULT);
				}

			}
		}
		else if (m_iMsg == 0)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"恭喜您融合神佑成功",0xFF000000,FONT_DEFAULT,FONTSIZE_DEFAULT);
		}
		else if (m_iMsg == 1)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"融合神佑失败",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);
		}
		else if (m_iMsg == 2)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"放入的物品无法进行融合",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);			
		}
		else if (m_iMsg == 3)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"放入的物品无法进行融合",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);			
		}
		else if (m_iMsg == 4)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"绑定物品无法进行融合",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);			
		}
		else if (m_iMsg == 5)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"附加物品不合法",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);			
		}
		else if (m_iMsg == 6)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"神佑未激活，请双击开启后再进行融合",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);			
		}
	}
	else if (m_iType == EBCT_SYHF)
	{
		if (m_iMsg == -1)
		{
			if ((g_NPC.GetBlessGoodLeft().GetID()==0) || (g_NPC.GetBlessGoodRight().GetID() == 0))
			{
				g_pFont->DrawText(m_iScreenX + 60,m_iScreenY + 345,"请在神佑恢复界面左侧位置放入已过期的玉兔神佑；",0xFF000000,FONT_DEFAULT,FONTSIZE_DEFAULT);
				g_pFont->DrawText(m_iScreenX + 60,m_iScreenY + 360,"请在神佑恢复界面右侧位置放入玉兔珍宝；",0xFF000000,FONT_DEFAULT,FONTSIZE_DEFAULT);
			}
			else
			{
				g_pFont->DrawText(m_iScreenX + 60,m_iScreenY + 345,"恢复后玉兔神佑会恢复到7天有效期",0xFF000000,FONT_DEFAULT,FONTSIZE_DEFAULT);				
			}
		}
		else if (m_iMsg == 0)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"恭喜您恢复成功",0xFF000000,FONT_DEFAULT,FONTSIZE_DEFAULT);
		}		
		else if (m_iMsg == 2)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"放入的物品无法进行恢复",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);			
		}
		else if (m_iMsg == 3)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"放入的物品无法进行恢复",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);			
		}
		else if (m_iMsg == 4)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"绑定物品无法进行恢复",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);			
		}
		else if (m_iMsg == 5)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"附加物品不合法",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);			
		}
		else if (m_iMsg == 6)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"神佑未激活，请双击开启后再进行融合",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);			
		}
	}
	else if (m_iType == EBCT_JLL)
	{
		if (m_iMsg == -1)
		{
			int istartx = 50;
			int istarty = 330;
			if(g_NPC.GetBlessGoodFu().GetID()==0)
			{
				istarty += 15;
				g_pFont->DrawText(m_iScreenX + istartx,m_iScreenY + istarty,"请在界面中间位置放入需要精炼的纹佩；",0xFF000000,FONT_DEFAULT,FONTSIZE_DEFAULT);
			}
			if(g_NPC.GetBlessGoodLeft().GetID()==0)
			{
				istarty += 15;
				g_pFont->DrawText(m_iScreenX + istartx,m_iScreenY + istarty,"请在界面左侧位置放入纹佩精华；",0xFF000000,FONT_DEFAULT,FONTSIZE_DEFAULT);
			}
			if(g_NPC.GetBlessGoodRight().GetID()==0)
			{
				istarty += 15;
				g_pFont->DrawText(m_iScreenX + istartx,m_iScreenY + istarty,"要提升精炼的成功率，可以在右侧位置放入幸运符；",0xFF000000,FONT_DEFAULT,FONTSIZE_DEFAULT);
			}

			if (g_NPC.GetBlessGoodFu().GetID()!=0)
			{
				int iLevelUpTimes = g_NPC.GetBlessGoodFu().GetLevelUpTimes();
				if ( iLevelUpTimes >= 3 && iLevelUpTimes <= 5)
				{
					istarty += 15;
					g_pFont->DrawText(m_iScreenX + istartx,m_iScreenY + istarty,"为+3以上纹佩精炼失败时可能降低其圣者属性等级；",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);

					if(g_NPC.GetBlessGoodLeft().GetID()!=0)
					{
						if (g_NPC.GetBlessGoodLeft().GetShape() == 1)
						{
							istarty += 15;
							g_pFont->DrawText(m_iScreenX + istartx,m_iScreenY + istarty,"初级纹佩精华在注入+3以上纹佩时必定会失败；",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);
						}
					}
				}
				else if ( iLevelUpTimes >= 6 && iLevelUpTimes < 9)
				{
					istarty += 15;
					g_pFont->DrawText(m_iScreenX + istartx,m_iScreenY + istarty,"为+6以上纹佩精炼失败时可能使圣者属性等级归零；",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);
				
					if(g_NPC.GetBlessGoodLeft().GetID()!=0)
					{
						if (g_NPC.GetBlessGoodLeft().GetShape() == 1 || g_NPC.GetBlessGoodLeft().GetShape() == 2)
						{
							istarty += 15;
							g_pFont->DrawText(m_iScreenX + istartx,m_iScreenY + istarty,"初级、中级纹佩精华在注入+6以上纹佩时必定会失败；",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);
						}
					}
				}
				else if (iLevelUpTimes >= 9)
				{
					istarty += 15;
					g_pFont->DrawText(m_iScreenX + istartx,m_iScreenY + istarty,"您的纹佩已经精炼到最高等级，继续精炼也不会提升属性；",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);
				}
			}
			
		}
		else if (m_iMsg == 0)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"恭喜您纹佩精炼成功",0xFF000000,FONT_DEFAULT,FONTSIZE_DEFAULT);
		}		
		else if (m_iMsg == 1)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"放入的物品不是纹佩无法进行精炼",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);			
		}
		else if (m_iMsg == 2)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"放入的物品不是幸运符无法进行精炼",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);			
		}
		else if (m_iMsg == 3)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"放入的物品不是纹佩精华无法进行精炼",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);			
		}
		else if (m_iMsg == 4)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"放入的纹佩精华不符合要求",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);			
		}
		else if (m_iMsg == 5)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"此纹佩无法精炼",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);			
		}
		else if (m_iMsg == 6)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"精炼失败，您的纹佩圣者属性等级降低了！",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);			
		}
		else if (m_iMsg == 7)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"精炼失败，您的纹佩圣者属性等级被重置！",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);			
		}
		else if (m_iMsg == 8)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"纹佩精炼失败！",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);			
		}
	}
	else if (m_iType == EBCT_BJRH)
	{
		if (m_iMsg == -1)
		{
			int istartx = 50;
			int istarty = 330;			
			if(g_NPC.GetBlessGoodLeft().GetID()==0 || g_NPC.GetBlessGoodRight().GetID()==0)
			{
				istarty += 15;
				g_pFont->DrawText(m_iScreenX + istartx,m_iScreenY + istarty,"请在左右两侧放入同种类纹佩；",0xFF000000,FONT_DEFAULT,FONTSIZE_DEFAULT);
			}
			if(g_NPC.GetBlessGoodFu().GetID()==0)
			{
				istarty += 15;
				g_pFont->DrawText(m_iScreenX + istartx,m_iScreenY + istarty,"中间放入纹佩融合凭证；",0xFF000000,FONT_DEFAULT,FONTSIZE_DEFAULT);
			}
			istarty += 15;
			g_pFont->DrawText(m_iScreenX + istartx,m_iScreenY + istarty,"放入的纹佩越好则融合后出现高属性纹佩的概率就越高",0xFF000000,FONT_DEFAULT,FONTSIZE_DEFAULT);

		}
		else if (m_iMsg == 0)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"恭喜您纹佩融合成功",0xFF000000,FONT_DEFAULT,FONTSIZE_DEFAULT);
		}		
		else if (m_iMsg == 1)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"放入的物品不是纹佩部件无法进行融合",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);			
		}				
		else if (m_iMsg == 5)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"此纹佩部件无法融合",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);			
		}		
		else if (m_iMsg == 8)
		{
			g_pFont->DrawText(m_iScreenX + 80,m_iScreenY + 345,"纹佩融合失败！",0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT);			
		}
	}
	

	LPTexture pTex = NULL;	

	if (m_iType == EBCT_JLL)
	{
		if (m_iSuccessFrame < 64)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22315+m_iSuccessFrame / 4,EP_UI);
			if(pTex)
			{
				g_pGfx->SetRenderMode(RM_ADD2);
				g_pGfx->DrawTextureNL(m_iScreenX + 53,m_iScreenY + 42,pTex);
				g_pGfx->SetRenderMode();
			}

			++m_iSuccessFrame;
		}		
	}
	else
	{
		if (m_iSuccessFrame < 40)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,4722+m_iSuccessFrame / 4,EP_UI);
			if(pTex)
			{				
				g_pGfx->DrawTextureNL(m_iScreenX,m_iScreenY - 53,pTex);
			}

			++m_iSuccessFrame;
		}
	}	

	if (m_iFailFrame < 64)
	{
		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,22335+m_iFailFrame / 4,EP_UI);
		if(pTex)
		{
			g_pGfx->SetRenderMode(RM_ADD2);
			g_pGfx->DrawTextureNL(m_iScreenX + 53,m_iScreenY + 42,pTex);
			g_pGfx->SetRenderMode();
		}

		++m_iFailFrame;
	}
	
}

bool CBlessCompoundWnd::OnMouseMove(int iX, int iY)
{
	CParserTip* pTip = g_pControl->GetTipWnd();
	if(!pTip)
		return true;

	pTip->Clear();
	pTip->SetShow(false);
	pTip->GetSelfEquipTips()->SetShow(false);

	if(iX > 113 && iX < 150 && iY > 186 && iY < 224)
	{		
		if(g_NPC.GetBlessGoodLeft().GetID() != 0)
		{
			pTip->Parse(g_NPC.GetBlessGoodLeft());			
		}
		else
		{
			if (m_iType == EBCT_SXRH || m_iType == EBCT_GNRH || m_iType == EBCT_SYHF)
			{
				pTip->AddText("请放入玉兔神佑！");
			}
			else if (EBCT_JLL == m_iType)
			{
				pTip->AddText("请放入纹佩精华！");
			}
			else if (m_iType == EBCT_BJRH)
			{
				pTip->AddText("请放入用于融合的纹佩部件！");
			}
			else
			{
				return CCtrlWindowX::OnMouseMove(iX,iY);
			}
		}

		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}
	else if(iX > 245 && iX < 283 && iY > 186 && iY < 224)
	{
		if(g_NPC.GetBlessGoodRight().GetID() != 0)
		{
			pTip->Parse(g_NPC.GetBlessGoodRight());			
		}
		else
		{
			if (m_iType == EBCT_SXRH)
			{
				pTip->AddText("请放入玉兔神佑，如果融合失败，玉兔神佑将消失。");
			}
			else if (m_iType == EBCT_GNRH)
			{
				pTip->AddText("请放入打宝神佑！");
			}
			else if (m_iType == EBCT_SYHF)
			{
				pTip->AddText("请放入玉兔珍宝！");
			}
			else if (EBCT_JLL == m_iType)
			{
				pTip->AddText("请放入幸运符！");
			}
			else if (m_iType == EBCT_BJRH)
			{
				pTip->AddText("请放入用于融合的纹佩部件！");
			}
			else
			{
				return CCtrlWindowX::OnMouseMove(iX,iY);
			}
		}
		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;	
	}
	else if(iX > 179 && iX < 271 && iY > 186 && iY < 224)
	{		
		if(g_NPC.GetBlessGoodFu().GetID() != 0)
		{
			pTip->Parse(g_NPC.GetBlessGoodFu());			
		}
		else
		{
			if (m_iType == EBCT_SXRH)
			{
				pTip->AddText("请放入幸运符！");
			}
			else if (m_iType == EBCT_GNRH)
			{
				pTip->AddText("请放入玉兔令！");
			}			
			else if (EBCT_JLL == m_iType)
			{
				pTip->AddText("请放入圣殿纹佩！");
			}
			else if (EBCT_BJRH == m_iType)
			{
				pTip->AddText("请放入纹佩融合凭证！");
			}
			else
			{
				return CCtrlWindowX::OnMouseMove(iX,iY);
			}

		}
		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}


	return CCtrlWindowX::OnMouseMove(iX,iY);
}

bool CBlessCompoundWnd::OnLeftButtonUp(int iX,int iY)
{
	stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();

	int iStdMode = GoodFrom.DropGood.GetStdMode();
	int iShape = GoodFrom.DropGood.GetShape();

	if(iX > 113 && iX < 150 && iY > 186 && iY < 224)
	{
		if (m_iType == EBCT_SXRH || m_iType == EBCT_GNRH || m_iType == EBCT_SYHF)
		{
			if (GoodFrom.DropGood.GetID() && strcmp(GoodFrom.DropGood.GetName(), "玉兔神佑") != 0)
			{
				g_TalkMgr.PushSysTalk("请放入玉兔神佑！");
				return true;
			}
		}
		else if (m_iType == EBCT_JLL)
		{
			if (GoodFrom.DropGood.GetID() && iStdMode != 102)
			{
				g_TalkMgr.PushSysTalk("请放入纹佩精华！");
				return true;
			}
		}
		else if (m_iType == EBCT_BJRH)
		{
			if (GoodFrom.DropGood.GetID() && !g_AIGoodMgr.IsSubWenPei(GoodFrom.DropGood))
			{
				g_TalkMgr.PushSysTalk("请放入用于融合的纹佩部件！");
				return true;
			}
		}
		else
		{
			return true;
		}

		CGood tmp = GoodFrom.DropGood;
		GoodFrom.DropGood = g_NPC.GetBlessGoodLeft();
		g_NPC.GetBlessGoodLeft() = tmp;
		CGoodGrid::SetDropGoodFrom(GoodFrom);

		m_iMsg = -1;
			
		return true;
	}
	else if(iX > 245 && iX < 283 && iY > 186 && iY < 224)
	{
		if (m_iType == EBCT_SXRH)
		{
			if (GoodFrom.DropGood.GetID() && strcmp(GoodFrom.DropGood.GetName(), "玉兔神佑") != 0)
			{
				g_TalkMgr.PushSysTalk("请放入玉兔神佑！");
				return true;
			}			
		}
		else if (m_iType == EBCT_GNRH)
		{
			if (GoodFrom.DropGood.GetID() && strcmp(GoodFrom.DropGood.GetName(), "打宝神佑") != 0)
			{
				g_TalkMgr.PushSysTalk("请放入打宝神佑！");
				return true;
			}			
		}
		else if (m_iType == EBCT_SYHF)
		{
			if (GoodFrom.DropGood.GetID() && strcmp(GoodFrom.DropGood.GetName(), "玉兔珍宝") != 0)
			{
				g_TalkMgr.PushSysTalk("请放入玉兔珍宝！");
				return true;
			}			
		}
		else if (m_iType == EBCT_JLL)
		{
			if (GoodFrom.DropGood.GetID() && (!(iStdMode == 44 && (iShape == 3 || iShape == 4))))
			{
				g_TalkMgr.PushSysTalk("请放入幸运符！");
				return true;
			}
		}
		else if (m_iType == EBCT_BJRH)
		{
			if (GoodFrom.DropGood.GetID() && !g_AIGoodMgr.IsSubWenPei(GoodFrom.DropGood))
			{
				g_TalkMgr.PushSysTalk("请放入用于融合的纹佩部件！");
				return true;
			}
		}
		else
		{
			return true;
		}

		CGood tmp = GoodFrom.DropGood;
		GoodFrom.DropGood = g_NPC.GetBlessGoodRight();
		g_NPC.GetBlessGoodRight() = tmp;
		CGoodGrid::SetDropGoodFrom(GoodFrom);

		m_iMsg = -1;

		return true;
	}
	else if(iX > 179 && iX < 271 && iY > 186 && iY < 224)
	{
		if (m_iType == EBCT_SXRH)
		{
			if (GoodFrom.DropGood.GetID() && (!(iStdMode == 44 && (iShape == 3 || iShape == 4))))
			{
				g_TalkMgr.PushSysTalk("请放入幸运符！");
				return true;
			}
		}
		else if (m_iType == EBCT_GNRH)
		{
			if (GoodFrom.DropGood.GetID() && strcmp(GoodFrom.DropGood.GetName(), "玉兔令") != 0)
			{
				g_TalkMgr.PushSysTalk("请放入玉兔令！");
				return true;
			}
		}
		else if (m_iType == EBCT_JLL)
		{
			if (GoodFrom.DropGood.GetID() && !g_AIGoodMgr.IsWenPei(GoodFrom.DropGood))
			{
				g_TalkMgr.PushSysTalk("请放入圣殿纹佩！");
				return true;
			}
		}
		else if (m_iType == EBCT_BJRH)
		{

		}
		else
		{
			return true;
		}

		CGood tmp = GoodFrom.DropGood;
		GoodFrom.DropGood = g_NPC.GetBlessGoodFu();
		g_NPC.GetBlessGoodFu() = tmp;
		CGoodGrid::SetDropGoodFrom(GoodFrom);

		m_iMsg = -1;

		return true;
	}

	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}
