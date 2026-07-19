#include "TigerRuneWnd.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameData/GameData.h"
#include "GameControl/GameControl.h"

INIT_WND_POSX(CTigerRuneWnd,POS_AUTO,POS_AUTO)

CTigerRuneWnd::CTigerRuneWnd(void)
{
	m_sName = "TigerRuneWnd";
	m_iIndex = 768;
	m_iLastPutIn = -1;
	m_dwStartShowTime = GetTickCount();
	pLightingBorder = 0;
	m_dwCurFrameOfLightingBorder = 0;
	m_dwTigerRuneID = sm_dwWindowType;

	for(int i = 0; i != 16; ++i)
	{
		m_runePos[i].x = 19 + i % 4 * 50;
		m_runePos[i].y = 38 + i / 4 * 48; 
	}

	m_dwExp[0] = "经验值400000点";
	m_dwExp[1] = "经验值600000点";
	m_dwExp[2] = "经验值1000000点";
	m_dwExp[3] = "经验值1000000点";
}

CTigerRuneWnd::~CTigerRuneWnd(void)
{
}
void CTigerRuneWnd::OnCreate()
{
	SetCloseButton(378,1,80);
	CCtrlWindowX::OnCreate();
}
void CTigerRuneWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	//加载多帧纹理,亮框动画
	if (pLightingBorder == NULL)
	{
		pLightingBorder = g_pTexMgr->GetTex(PACKAGE_INTERFACE,770,EP_UI);
	}
	
	for (int i = 0; i != 16; ++i)
	{
		LPTexture pRune = g_pTexMgr->GetTex(PACKAGE_items,GetRuneIndex(i),EP_UI);

		//获得物品属性,判读第i位是否为1
		CGood *pGood = SELF.PackageGood().FindGood(m_dwTigerRuneID);
		if (!pGood || !((pGood->GetDemonDark1() >> i) & 1))
		{
			//无字符,画暗色的字符
			g_pGfx->DrawTextureNL(m_runePos[i].x + m_iScreenX, m_runePos[i].y + m_iScreenY, pRune,0x40FFFFFF);
			continue;
		}

		//画字符
		if (pRune)
		{
			g_pGfx->DrawTextureNL(m_runePos[i].x + m_iScreenX, m_runePos[i].y + m_iScreenY, pRune);
		}
		//画字符存在后的亮框
		pRune = g_pTexMgr->GetTex(PACKAGE_INTERFACE,771,EP_UI);
		if (pRune)
		{
			g_pGfx->SetRenderMode(RM_ADD2);
			g_pGfx->DrawTextureNL(m_runePos[i].x + m_iScreenX - 31, m_runePos[i].y + m_iScreenY -31, pRune);
			g_pGfx->SetRenderMode();
		}
		
	}
	
	//放入字符时的亮框动画
	if (m_iLastPutIn != -1 && m_dwCurFrameOfLightingBorder < 10 && pLightingBorder)
	{
		pLightingBorder->SetCurFrame(m_dwCurFrameOfLightingBorder);
		
		if (GetTickCount() - m_dwStartShowTime > 80)//间隔最低80毫秒一帧
		{
			m_dwStartShowTime = GetTickCount();
			++m_dwCurFrameOfLightingBorder;
		}
		
		g_pGfx->SetRenderMode(RM_ADD2);
		g_pGfx->DrawTextureNL(m_runePos[m_iLastPutIn].x + m_iScreenX - 31, m_runePos[m_iLastPutIn].y + m_iScreenY -31, pLightingBorder);
		g_pGfx->SetRenderMode();
	}//动画结束,还原数据
	else 
	{
		m_iLastPutIn = -1;
		m_dwCurFrameOfLightingBorder = 0;
	}

	g_pFont->DrawText(m_iScreenX + 178,m_iScreenY + 5,"虎",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG);
	g_pFont->DrawText(m_iScreenX + 209,m_iScreenY + 5,"符",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,FONTSIZE_BIG);
	
	//获得该虎符已凑齐几个成语
	int number = 0;
	for (int i = 0; i < 16; i += 4)
	{
		if (((SELF.PackageGood().FindGood(m_dwTigerRuneID)->GetDemonDark1() >> i) & 0xf) == 0xf)
		{
			++number; 
		}
	}
	
	char tempstr[1024] = {0};
	sprintf(tempstr,"可兑换奖励 ( %d/4 )",number);
	g_pFont->DrawText(222 + m_iScreenX, 40 + m_iScreenY,tempstr, COLOR_TEXT_SBU_TITLE,FONT_YAHEI,FONTSIZE_MIDDLE);
	for (int i = 0; i != number; ++i )
	{
		g_pFont->DrawText(253 + m_iScreenX, 63 + m_iScreenY + i * 21,m_dwExp[i].c_str(), -1);
	}
	for (int i = number; i != 4; ++i)
	{
		g_pFont->DrawText(253 + m_iScreenX, 63 + m_iScreenY + i * 21,m_dwExp[i].c_str(), COLOR_TEXT_DISABLE);
	}

	g_pFont->DrawText(220 + m_iScreenX, 160 + m_iScreenY,"注:以上奖励为叠加关系。", COLOR_TEXT_NUMBER);
	g_pFont->DrawText(220 + m_iScreenX, 181 + m_iScreenY,"将所有成语全被激活的虎符交予", COLOR_TEXT_NUMBER);
	g_pFont->DrawText(220 + m_iScreenX, 202 + m_iScreenY,"NPC, 可额外抽取超级大奖!", COLOR_TEXT_NUMBER);
}

bool CTigerRuneWnd::OnLeftButtonUp(int iX, int iY)
{
	//判断鼠标上时候有字符,如果有,判断当前点击位置的字符是否已经有了,如果没有并且位置相符,则向服务器发消息,物品放回包裹栏
	stGoodFrom GoodFrom = CGoodGrid::GetDropGoodFrom();
	if (GoodFrom.DropGood.GetID() == 0  || GoodFrom.DropGood.GetStdMode() != 46 || GoodFrom.DropGood.GetShape() != 9)
	{
		return true;
	}

	for (int i = 0; i != 16; ++i)
	{
		if (!(iX  < m_runePos[i].x || iX  > m_runePos[i].x + 37 
			||iY  < m_runePos[i].y || iY  > m_runePos[i].y + 37))
		{
			//如果字符有了
			if ((SELF.PackageGood().FindGood(m_dwTigerRuneID)->GetDemonDark1() >> i) & 1)
			{
				return true;
			}
			
			//如果字符与位置不相符
			if (!IsRightPos(GoodFrom.DropGood.GetAC(), i))
			{
				return true;
			}
			
			//记录新放入字符的位置
			m_iLastPutIn = i;
			if (pLightingBorder)
			{
				pLightingBorder->EnableSysAnim(false);
				m_dwCurFrameOfLightingBorder = 0;
			}
			
			//向服务器发消息
			g_pGameControl->SEND_RUNE_INTO_TIGER(m_dwTigerRuneID, GoodFrom.DropGood.GetID(), i );
			
			//把字符放回包裹
			if(SELF.PackageGood().BackToArray(GoodFrom.DropGood,GoodFrom.DropGood.GetPos()))
			{
				CGoodGrid::ClearGoodFrom();	
			}

			return true;
		}
	}
	return CCtrlWindowX::OnLeftButtonUp(iX, iY);
}

int	CTigerRuneWnd::GetRuneIndex(int pos)
{
	/*
	四个成语:
		虎啸风生
		虎虎生威
		龙腾虎跃
		藏龙卧虎
	*/
	switch (pos)
	{
	case 0:
	case 4:
	case 5:
	case 10:
	case 15:
		return 1473;		//虎
	case 1:
		return 1479;		//啸
	case 2:
		return 1472;		//风
	case 3:
	case 6:	
		return 1475;		//生
	case 7:
		return 1477;		//威
	case 8:
	case 13:	
		return 1474;		//龙
	case 9:
		return 1476;		//腾
	case 11:
		return 1480;		//跃
	case 12:
		return 1471;		//藏
	case 14:
		return 1478;		//卧
	}
	return -1;
}

bool	CTigerRuneWnd::IsRightPos(int rune, int pos)
{
	//字符索引:虎啸风生威龙腾跃藏卧 AC对应1-10
	switch (rune)
	{
	case 1:
		if (pos == 0 || pos == 4 || pos ==5 || pos == 10 || pos == 15)
		{
			return true;
		}
		return false;
	case 2:
		if (pos == 1)
		{
			return true;
		}
		return false;
	case 3:
		if (pos == 2)
		{
			return true;
		}
		return false;
	case 4:
		if (pos == 3 || pos == 6)
		{
			return true;
		}
		return false;
	case 5:
		if (pos == 7)
		{
			return true;
		}
		return false;
	case 6:
		if (pos == 8 || pos == 13)
		{
			return true;
		}
		return false;
	case 7:
		if (pos == 9)
		{
			return true;
		}
		return false;
	case 8:
		if (pos == 11)
		{
			return true;
		}
		return false;
	case 9:
		if (pos == 12)
		{
			return true;
		}
		return false;
	case 10:
		if (pos == 14)
		{
			return true;
		}
		return false;
	}
	return false;
}