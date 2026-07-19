#include "BaoJianWnd.h"
#include "GameControl/GameControl.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameData/NpcData.h"
#include "BaseClass/Control/ParserTip.h"
#include "BaseClass/Audio/Audio.h"

INIT_WND_POSX(CBaoJianWnd,POS_AUTO,POS_AUTO)

#define CardDistance 80

CBaoJianWnd::CBaoJianWnd()
{
	m_bModel = false;	

	m_iAlignType = XAL_TOPLEFT;
	
	m_iIndex = 20901;

	m_iFirstPos = -1;
	m_iSecondPos = -1;
	m_iThirdPos = -1;		
	m_iAdditionPos = -1;

	m_dwStartTime = GetTickCount();

	for(int i = 0;i < 12;i++)
	{
		m_ptGrid[i].x = 75 + i % 4 * CardDistance;
		m_ptGrid[i].y = 125 + i / 4 * CardDistance;
	}

	m_dwShowAllPrizeStart = -1;;
	
	for(int i = 0;i < 12;i++)
	{
		m_PrizeIndex[i] = i;
		m_bShowPrize[i] = false;
		m_bRealPrize[i] = false;
	}

	for(int i = 0;i < 12;i++)
	{
		int iPos = rand() % 12;
		int iTemp = m_PrizeIndex[i];
		m_PrizeIndex[i] = m_PrizeIndex[iPos];
		m_PrizeIndex[iPos] = iTemp;
	}

	m_dwShuffleRand = 0;

	m_iOpenedGrid = 0;

	m_dwLastOpenGridTime = 0;
}

CBaoJianWnd::~CBaoJianWnd(void)
{
	if(m_iOpenedGrid < 3 && m_dwShowAllPrizeStart == -1)	//没开满三轮且铁英之怒没满那么关闭界面要发消息给服务器
	{
		g_pGameControl->SEND_SELECT_BAOJIAN(4);
	}
}

void CBaoJianWnd::Draw( void )
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	_BaoJianData& stBaoJianData = g_NPC.GetBaoJianData();
	
	g_pFont->DrawText(m_iScreenX + 200,m_iScreenY + 37,"群英宝鉴",COLOR_TEXT_MAIN_TITLE,FONT_YAHEI,16,DTF_BlackFrame);
	
	char str[256] = "";
	char *p[4] = {"金","银","铜","铁"};

	g_pFont->DrawText(m_iScreenX + 75,m_iScreenY + 375,"同一个宝鉴中连续三次开到铜英令可额外获得一个金英令奖励",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);

	int iCount = 0;	//开到铜英令的次数

	if(stBaoJianData.byFirstPrize / 3 == 2)
		iCount++;
	if(stBaoJianData.bySecondPrize / 3 == 2)
		iCount++;
	if(stBaoJianData.byThirdPrize / 3 == 2)
		iCount++;
	
	sprintf(str,"目前开到铜英令的次数: %d",iCount);
	g_pFont->DrawText(m_iScreenX + 160,m_iScreenY + 395,str,0xffffff00,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);

	LPTexture pAnger = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20900,EP_UI);
	g_pGfx->DrawPartTexture(m_iScreenX + 51,m_iScreenY + 418,pAnger,0,0,(372 * stBaoJianData.anger / 100));
	

	g_pFont->DrawText(m_iScreenX + 105,m_iScreenY + 428,"铁英之怒累满，立即获取该群英宝鉴中所有奖励",COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);


	if(!DrawShuffling())
	{
		if(m_dwShowAllPrizeStart != -1)		//铁英之怒满了
		{
			g_pFont->DrawText(m_iScreenX + 100,m_iScreenY + 80,"恭喜你成功发动铁英之怒，你将获取该该群英宝鉴",0xff00ff00,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX + 100,m_iScreenY + 95,"中所有奖励，如包裹不够剩余奖励请到老兵处领取",0xff00ff00,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		}
		else if(stBaoJianData.byCurrentRound == 0)
		{
			g_pFont->DrawText(m_iScreenX + 100,m_iScreenY + 80,"请双击以下令牌立刻获取对应奖励，如果三次均",0xff00ff00,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
			g_pFont->DrawText(m_iScreenX + 95,m_iScreenY + 95,"点选到铜英令奖励，可额外再获得金英令奖励一份",0xff00ff00,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
		}
		else if(stBaoJianData.byCurrentRound < 3)
		{
			int iType = 0;			

			if(stBaoJianData.byCurrentRound == 1)
			{
				iType = stBaoJianData.byFirstPrize / 3;
			}
			else if(stBaoJianData.byCurrentRound == 2)
			{
				iType = stBaoJianData.bySecondPrize / 3;
			}

			if(iType >= 0 && iType <= 3)
			{
				sprintf(str,"恭喜你获得%s英令奖励，你还可以用一个群英令选取一次奖励",p[iType]);
				g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 90,str,0xff00ff00,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
			}
		}
		else if(stBaoJianData.byCurrentRound == 3 && stBaoJianData.byThirdPrize >= 0 && stBaoJianData.byThirdPrize < 12)
		{
			if(stBaoJianData.byFirstPrize / 3 == 2 && stBaoJianData.bySecondPrize / 3 == 2 && stBaoJianData.byThirdPrize / 3 == 2)	//三连环
			{
				g_pFont->DrawText(m_iScreenX + 70,m_iScreenY + 80,"恭喜你连续三次获得铜英令奖励，为此你将额外再获得一个金",0xff00ff00,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
				g_pFont->DrawText(m_iScreenX + 65,m_iScreenY + 95, "英令奖励，如果还想继续获取令牌奖励，请在寻找一个群英宝鉴",0xff00ff00,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
			}
			else
			{
				sprintf(str,"恭喜你获得%s英令奖励，如果还想继续",p[stBaoJianData.byThirdPrize / 3]);
				g_pFont->DrawText(m_iScreenX + 135,m_iScreenY + 80,str,0xff00ff00,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
				g_pFont->DrawText(m_iScreenX + 135,m_iScreenY + 95,"获取令牌奖励，请再寻找一个群英宝鉴",0xff00ff00,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);
			}
			
		}

		LPTexture pTex = NULL;

		CGood prize;
		prize.SetID(1);

		for(int i = 0;i < 12;i++)
		{	
			if(m_bShowPrize[i])
			{
				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20920 + m_PrizeIndex[i] / 3,EP_UI);
				prize.SetLooks(stBaoJianData.prize[m_PrizeIndex[i]].looks);	

				g_pGfx->DrawTextureNL(m_iScreenX + m_ptGrid[i].x,m_iScreenY + m_ptGrid[i].y,pTex);				
				CGoodGrid::DrawOneGood(m_iScreenX + m_ptGrid[i].x + 39,m_iScreenY + m_ptGrid[i].y + 39,prize);

				if(m_bRealPrize[i])
				{
					pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20953 + m_PrizeIndex[i] / 3,EP_UI);
					g_pGfx->SetRenderMode(RM_ADD2);
					g_pGfx->DrawTextureNL(m_iScreenX + m_ptGrid[i].x,m_iScreenY + m_ptGrid[i].y,pTex);
					g_pGfx->SetRenderMode();
				}
			}
			else
			{
				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20924,EP_UI);
				g_pGfx->DrawTextureNL(m_iScreenX + m_ptGrid[i].x,m_iScreenY + m_ptGrid[i].y,pTex);
			}
		}


		if(m_dwShowAllPrizeStart != -1)
		{
			DWORD dwTime = GetTickCount();

			if((dwTime - m_dwShowAllPrizeStart) < 6000 - m_iOpenedGrid * 500)
			{
				stBaoJianData.anger = 100 - 100 * (dwTime - m_dwShowAllPrizeStart) / (6000 - m_iOpenedGrid * 500);

				LPTexture pPowerMask = g_pTexMgr->GetTex(PACKAGE_INTERFACE,14710,EP_UI);

				POS sizeNum(1.0f, 1.0f);
				sizeNum.fx *= 372.0f / 474.0f;

				g_pGfx->SetRenderMode(RM_ADD2);
				g_pGfx->DrawTextureFX(m_iScreenX + 32,m_iScreenY + 368,pPowerMask,-1,NULL,&sizeNum);
				g_pGfx->SetRenderMode();

				if(m_dwLastOpenGridTime == 0 || dwTime - m_dwLastOpenGridTime > 500)
				{
					if(m_dwLastOpenGridTime == 0)
					{
						for(int i = 0;i < 12;i++)
						{
							if(m_bShowPrize[i] == false)
							{
								m_bShowPrize[i] = true;
								m_bRealPrize[i] = true;
								break;
							}
						}
					}
					else
					{
						int iGridToOpen = (dwTime - m_dwLastOpenGridTime) / 500;

						for(int i = 0;i < 12;i++)
						{
							if(iGridToOpen > 0 && m_bShowPrize[i] == false)
							{
								m_bShowPrize[i] = true;
								m_bRealPrize[i] = true;
								iGridToOpen--;
							}
						}
					}

					m_dwLastOpenGridTime = dwTime;
				}
			}
			else
			{
				stBaoJianData.anger = 0;
			}
		}		
	}	

	if(stBaoJianData.byCurrentRound == 3 && GetTickCount() > stBaoJianData.dwCloseTime - 5000)	//翻开所有牌
	{
		for(int i = 0;i < 12;i++)
		{
			m_bShowPrize[i] = true;
		}
	}
	
	//开满三轮或者铁英之怒满了到时自动关闭界面
	if((stBaoJianData.byCurrentRound == 3 || m_dwShowAllPrizeStart != -1) && GetTickCount() > stBaoJianData.dwCloseTime)
	{
		CloseWindow();
	}
}

#define EffectBegin 3000
#define Phase2Begin (EffectBegin + 400)
#define Phase3Begin (Phase2Begin + 400)
#define Phase4Begin (Phase3Begin + 1000)
#define Phase5Begin (Phase4Begin + 400)
#define EffectEnd (Phase5Begin + 400)

bool CBaoJianWnd::OnMouseMove( int iX,int iY )
{
	CParserTip *pTip = g_pControl->GetTipWnd();
	pTip->Clear();	

	char str[256] = "";
	
	_BaoJianData& stBaoJianData = g_NPC.GetBaoJianData();
	
	if(iX > 47 && iY > 414 && iX < 47 + 378 && iY < 414 + 10)
	{
		sprintf(str,"铁英之怒： %d/100",stBaoJianData.anger);
		pTip->AddText(str);

		int x = m_iScreenX + iX + 10;
		int y = m_iScreenY + iY + 10;
		pTip->AdjustXY(x,y);
		pTip->Move(x,y);
		pTip->SetShow(true);
		return true;
	}		

	for(int i = 0;i < 12;i++)
	{
		if(iX > m_ptGrid[i].x + 20 && iY > m_ptGrid[i].y + 20 && iX < m_ptGrid[i].x + 60 && iY < m_ptGrid[i].y + 60)
		{
			int iPrizePos = -1;

			if(m_bShowPrize[i])
			{
				iPrizePos = m_PrizeIndex[i];
			}			
			else if(GetTickCount() - m_dwStartTime < EffectBegin - 800)
			{
				iPrizePos = i % 4 * 3 + i / 4;
			}

			if(strcmp(stBaoJianData.prize[iPrizePos].name.c_str(),"") != 0)
			{
				if(stBaoJianData.prize[iPrizePos].dwnum > 1)
				{
					sprintf(str,"%s%u",stBaoJianData.prize[iPrizePos].name.c_str(),stBaoJianData.prize[iPrizePos].dwnum);
				}
				else
				{
					if(stBaoJianData.prize[iPrizePos].wgrade > 1000)
					{
						sprintf(str,"%d个%s",stBaoJianData.prize[iPrizePos].wgrade - 1000,stBaoJianData.prize[iPrizePos].name.c_str());
					}
					else if(stBaoJianData.prize[iPrizePos].wgrade != 0)
					{
						sprintf(str,"%d阶%s",stBaoJianData.prize[iPrizePos].wgrade,stBaoJianData.prize[iPrizePos].name.c_str());
					}
					else
					{
						sprintf(str,"%s",stBaoJianData.prize[iPrizePos].name.c_str());
					}
				}

				

				pTip->AddText(str);

				int x = m_iScreenX + iX + 10;
				int y = m_iScreenY + iY + 10;
				pTip->AdjustXY(x,y);
				pTip->Move(x,y);
				pTip->SetShow(true);
			}

			return true;
		}
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}



bool CBaoJianWnd::OnLeftButtonDClick( int iX,int iY )
{
	_BaoJianData& stBaoJianData = g_NPC.GetBaoJianData();	

	if(g_NPC.GetBaoJianData().byCurrentRound < 3 && GetTickCount() - m_dwStartTime > EffectEnd)
	{
		for(int i = 0;i < 12;i++)
		{
			if(iX > m_ptGrid[i].x + 15 && iY > m_ptGrid[i].y + 6 && iX < m_ptGrid[i].x + 50 + 15 && iY < m_ptGrid[i].y + 65 + 6)
			{
				if(m_bShowPrize[i])		//显示的奖品不能双击了
				{
					
				}
				else
				{
					if(g_NPC.GetBaoJianData().byCurrentRound == 0 && m_iFirstPos == -1)
					{
						g_pAudio->Play(EAT_OTHER,908,g_pAudio->GetRand()++);
						g_pGameControl->SEND_SELECT_BAOJIAN(1);
						m_iFirstPos = i;
					}
					else if(g_NPC.GetBaoJianData().byCurrentRound == 1 && m_iSecondPos == -1)
					{
						g_pAudio->Play(EAT_OTHER,908,g_pAudio->GetRand()++);
						g_pGameControl->SEND_SELECT_BAOJIAN(2);
						m_iSecondPos = i;
					}
					else if(g_NPC.GetBaoJianData().byCurrentRound == 2 && m_iThirdPos == -1)
					{
						g_pAudio->Play(EAT_OTHER,908,g_pAudio->GetRand()++);
						g_pGameControl->SEND_SELECT_BAOJIAN(3);
						m_iThirdPos = i;
					}
				}

				return true;
			}
		}
	}

	return CCtrlWindowX::OnLeftButtonDClick(iX,iY);
}

bool CBaoJianWnd::DrawShuffling()
{
	DWORD dwElapse = GetTickCount() - m_dwStartTime;

	LPTexture pTex = NULL;

	_BaoJianData& stBaoJianData = g_NPC.GetBaoJianData();

	if(dwElapse <= EffectBegin)
	{
		g_pFont->DrawText(m_iScreenX + 55,m_iScreenY + 90,"请看清每个群英令牌对应的奖励，稍后可点击获取自己需要的群英令牌",0xff00ff00,FONT_YAHEI,FONTSIZE_DEFAULT,DTF_BlackFrame);

		if(dwElapse > EffectBegin - 800)
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20924,EP_UI);

			for(int i = 0;i < 12;i++)
			{
				g_pGfx->DrawTextureNL(m_iScreenX + m_ptGrid[i].x,m_iScreenY + m_ptGrid[i].y,pTex);							
			}
		}
		else
		{
			CGood prize;
			prize.SetID(1);

			for(int i = 0;i < 12;i++)
			{
				pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20920 + i % 4,EP_UI);

				g_pGfx->DrawTextureNL(m_iScreenX + m_ptGrid[i].x,m_iScreenY + m_ptGrid[i].y,pTex);

				prize.SetLooks(stBaoJianData.prize[i % 4 * 3 + i / 4].looks);			

				CGoodGrid::DrawOneGood(m_iScreenX + m_ptGrid[i].x + 39,m_iScreenY + m_ptGrid[i].y + 39,prize);			
			}
		}
		

		return true;
	}

	if(dwElapse >= EffectEnd)
	{
		m_dwShuffleRand = 0;
		return false;
	}	

	if(dwElapse > Phase3Begin && dwElapse <= Phase4Begin)
	{
		if(m_dwShuffleRand != 0)
		{
			g_pAudio->Stop(EAT_OTHER,907,m_dwShuffleRand);
			m_dwShuffleRand = 0;

			g_pAudio->Play(EAT_OTHER,908,g_pAudio->GetRand()++);
		}

		dwElapse = Phase3Begin;		//持续
	}
	else if(dwElapse > Phase4Begin && dwElapse < EffectEnd)
	{
		if(m_dwShuffleRand == 0)
		{
			m_dwShuffleRand = g_pAudio->GetRand();
			g_pAudio->Play(EAT_OTHER,907,g_pAudio->GetRand()++);
		}

		dwElapse = EffectEnd - dwElapse + EffectBegin;		//倒过来放
	}	

	pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,20924,EP_UI);

	if(dwElapse > EffectBegin && dwElapse < Phase2Begin)
	{
		if(m_dwShuffleRand == 0)
		{
			m_dwShuffleRand = g_pAudio->GetRand();
			g_pAudio->Play(EAT_OTHER,907,g_pAudio->GetRand()++);
		}

		int iOffset = (int)(float(dwElapse - EffectBegin) / (Phase2Begin - EffectBegin) * CardDistance);

		for(int i = 0;i < 12;i++)
		{
			if(i >= 0 && i <= 3)
			{
				g_pGfx->DrawTextureNL(m_iScreenX + m_ptGrid[i].x,m_iScreenY + m_ptGrid[i].y + iOffset,pTex);
			}
			else if(i >= 8 && i <= 11)
			{
				g_pGfx->DrawTextureNL(m_iScreenX + m_ptGrid[i].x,m_iScreenY + m_ptGrid[i].y - iOffset,pTex);
			}
			else
			{
				g_pGfx->DrawTextureNL(m_iScreenX + m_ptGrid[i].x,m_iScreenY + m_ptGrid[i].y,pTex);
			}
		}

		return true;
	}

	if(dwElapse >= Phase2Begin && dwElapse <= Phase3Begin)
	{
		int iOffset1 = (int)(float(dwElapse - Phase2Begin) / (Phase3Begin - Phase2Begin) * (CardDistance / 2 + CardDistance));
		int iOffset2 = (int)(float(dwElapse - Phase2Begin) / (Phase3Begin - Phase2Begin) * (CardDistance / 2));

		g_pGfx->DrawTextureNL(m_iScreenX + m_ptGrid[4].x + iOffset1,m_iScreenY + m_ptGrid[4].y,pTex);
		g_pGfx->DrawTextureNL(m_iScreenX + m_ptGrid[5].x + iOffset2,m_iScreenY + m_ptGrid[5].y,pTex);
		g_pGfx->DrawTextureNL(m_iScreenX + m_ptGrid[6].x - iOffset2,m_iScreenY + m_ptGrid[6].y,pTex);
		g_pGfx->DrawTextureNL(m_iScreenX + m_ptGrid[7].x - iOffset1,m_iScreenY + m_ptGrid[7].y,pTex);

		return true;
	}

	return true;
}

void CBaoJianWnd::OnCreate()
{
	SetCloseButton(411,33,80);
}

bool CBaoJianWnd::Msg( DWORD dwMsg,DWORD dwData,CControl * pControl /*= NULL*/ )
{
	if(dwMsg == MSG_CTRL_BAOJIAN_WND)		
	{
		_BaoJianData& stBaoJianData = g_NPC.GetBaoJianData();

		if(dwData == OPER_CUSTOM + 11)		//第一轮奖励
		{
			m_iOpenedGrid++;

			m_bShowPrize[m_iFirstPos] = true;
			m_bRealPrize[m_iFirstPos] = true;

			if(m_iFirstPos != -1 && m_PrizeIndex[m_iFirstPos] != stBaoJianData.byFirstPrize)
			{
				for(int i = 0;i < 12;i++)
				{
					if(m_PrizeIndex[i] == stBaoJianData.byFirstPrize)
					{
						m_PrizeIndex[i] = m_PrizeIndex[m_iFirstPos];
						m_PrizeIndex[m_iFirstPos] = stBaoJianData.byFirstPrize;		
						break;
					}
				}
			}
		}
		else if(dwData == OPER_CUSTOM + 12)		//第二轮奖励
		{
			m_iOpenedGrid++;

			m_bShowPrize[m_iSecondPos] = true;
			m_bRealPrize[m_iSecondPos] = true;

			if(m_iSecondPos != -1 && m_PrizeIndex[m_iSecondPos] != stBaoJianData.bySecondPrize)
			{
				for(int i = 0;i < 12;i++)
				{
					if(m_PrizeIndex[i] == stBaoJianData.bySecondPrize)
					{
						m_PrizeIndex[i] = m_PrizeIndex[m_iSecondPos];
						m_PrizeIndex[m_iSecondPos] = stBaoJianData.bySecondPrize;	
						break;
					}
				}
			}
		}
		else if(dwData == OPER_CUSTOM + 13)		//第三轮奖励
		{
			m_iOpenedGrid++;

			m_bShowPrize[m_iThirdPos] = true;
			m_bRealPrize[m_iThirdPos] = true;

			if(m_iThirdPos != -1 && m_PrizeIndex[m_iThirdPos] != stBaoJianData.byThirdPrize)
			{
				for(int i = 0;i < 12;i++)
				{
					if(m_PrizeIndex[i] == stBaoJianData.byThirdPrize)
					{
						m_PrizeIndex[i] = m_PrizeIndex[m_iThirdPos];
						m_PrizeIndex[m_iThirdPos] = stBaoJianData.byThirdPrize;		
						break;
					}
				}
			}		
		}
		else if(dwData == OPER_CUSTOM + 14)		//三连环发额外奖励
		{
			m_iOpenedGrid++;			

			for(int i = 0;i < 12;i++)
			{
				if(m_PrizeIndex[i] == stBaoJianData.byAdditionalPrize)
				{
					m_iAdditionPos = i;
					m_bShowPrize[m_iAdditionPos] = true;
					m_bRealPrize[m_iAdditionPos] = true;
					break;
				}
			}
			
		}
		else if(dwData == OPER_CUSTOM + 1)		//本次点击失败
		{
			if(g_NPC.GetBaoJianData().byCurrentRound == 0)
			{
				m_iFirstPos = -1;
			}
			else if(g_NPC.GetBaoJianData().byCurrentRound == 1)
			{
				m_iSecondPos = -1;
			}
			else if(g_NPC.GetBaoJianData().byCurrentRound == 2)
			{
				m_iThirdPos = -1;
			}
		}
		else if(dwData == OPER_CUSTOM + 2)		//铁英之怒满100
		{
			m_dwShowAllPrizeStart = GetTickCount();
		}

		return true;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}