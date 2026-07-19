#include "Defend12GongWnd.h"
#include "Baseclass/Control/ParserTip.h"
#include "Global/Interface/TextureInterface.h"
#include "GameData/ConfigData.h"
#include "GameData/TalkMgr.h"
#include "GameControl/GameControl.h"


INIT_WND_POSX(CDefend12GongWnd,POS_VARIABLE,POS_VARIABLE)

CDefend12GongWnd::CDefend12GongWnd(void)
:m_pLeftScroll(NULL)
{
	m_iIndex = 13997;
	m_iAlignType = XAL_CENTER;
	m_iOffX -= 135;

	g_pGameControl->SEND_GetBingInfo();
}

CDefend12GongWnd::~CDefend12GongWnd(void)
{
	
}

void CDefend12GongWnd::OnCreate()
{
	CCtrlWindowX::OnCreate();

	SetCloseButton(648,3, 80);
	
	m_pClose2 = new CCtrlButton();
	AddControl(m_pClose2);
	m_pClose2->Create(this,917,3,80);
	m_pClose2->SetMask(80);


	int iTStartX = 0;
	int iTStartY = 0;
	for (int i = 0; i < 4; ++i)
	{
		iTStartX = 20 + (i%2) * 310;
		iTStartY = 80 + (i/2) * 175;

		m_pBtnAdd[i] = new CCtrlButton();
		AddControl(m_pBtnAdd[i]);
		m_pBtnAdd[i]->CreateEx(this, iTStartX + 150,iTStartY + 105, 132, 133, 134, 135);
		m_pBtnAdd[i]->SetText("招 募", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_SMALL,0,FONT_YAHEI);
		m_pBtnAdd[i]->SetEnable(false);
		m_pBtnAdd[i]->SetShow(false);
	}

	m_pLeftScroll = new CCtrlScroll();
	AddControl(m_pLeftScroll);
	m_pLeftScroll->CreateEx(this,642,80,14,353);
	m_pLeftScroll->SetStep(4);

	m_pBingGrid = new CCtrlGrid();
	AddControl(m_pBingGrid);
	m_pBingGrid->Create(this,25 + 681,57,245 + 681,166,D12BING_LINE_COUNT,21.0f);
	m_pBingGrid->AddScrollEx(202,0,16,110);
	m_pBingGrid->SetFont(FONT_YAHEI,FONTSIZE_SMALL);

	m_pBingGrid->SetLinesPerPage(D12BING_LINE_COUNT);
	m_pBingGrid->PushColumn(140);
	m_pBingGrid->PushColumn(110);
	m_pBingGrid->SetDrawOffXY(0,7);

	m_pBingGrid->SetTotalCount((int)g_OtherData.GetBings().size());	
}

void CDefend12GongWnd::Draw(void)
{
	CCtrlWindowX::Draw();

	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(340 + m_iScreenX, 10 + m_iScreenY, g_pGameMap->GetMapTitle(), COLOR_BTN_MOUSEON, FONT_YAHEI, FONTSIZE_BIG,DTF_Center);	


	LPTexture	pMonster = NULL;
	MonsterTex	MTEX;
	int iTStartX = 0;
	int iTStartY = 0;
	char tmp[64] = {0};

	vector<S_Bing>& lBings = g_OtherData.GetBings();
	int bingsize = lBings.size();
	for (int i = 0; i < 4; ++i)
	{
		int iCurIndex = i + m_pLeftScroll->GetPos() * 4;
		if (iCurIndex >= bingsize)
		{
			m_pBtnAdd[i]->SetEnable(false);
			m_pBtnAdd[i]->SetShow(false);
			continue;
		}

		m_pBtnAdd[i]->SetEnable(true);
		m_pBtnAdd[i]->SetShow(true);

		iTStartX = m_iScreenX + 20 + (i%2) * 310;
		iTStartY = m_iScreenY + 80 + (i/2) * 175;

		S_Bing& bing = lBings[iCurIndex];

		if (bing.iAction == ACTION_STAND)
		{
			bing.iStandContiFrame--;
			if (bing.iStandContiFrame <= 0)
			{
				if (rand() % 10 >= 4)
				{
					bing.iAction = ACTION_ATTACK1;
					bing.iAttackFrameStart = g_pGfx->GetFrameCount();
					bing.iFrameNow = 0;
				}
				else
				{
					bing.iAction = ACTION_RUN;
					bing.iRunContiFrame = rand() % 200 + 300;
				}
			}
		}
		else if (bing.iAction == ACTION_RUN)
		{
			bing.iRunContiFrame--;
			if (bing.iRunContiFrame <= 0)
			{
				if (rand() % 10 >= 3)
				{
					bing.iAction = ACTION_ATTACK1;
					bing.iAttackFrameStart = g_pGfx->GetFrameCount();
					bing.iFrameNow = 0;
				}
				else
				{
					bing.iAction = ACTION_STAND;
					bing.iStandContiFrame = rand() % 150 + 100;
				}
			}
		}
		else if (bing.iAction == ACTION_ATTACK1)
		{
			bing.iFrameNow = (g_pGfx->GetFrameCount() - bing.iAttackFrameStart) / 6;
			if (bing.iFrameNow >= bing.iAttackFrameCount)
			{
				if (rand() % 10 >= 5)
				{
					bing.iAction = ACTION_STAND;
					bing.iStandContiFrame = rand() % 150 + 100;
				}
				else
				{
					bing.iAction = ACTION_RUN;
					bing.iRunContiFrame = rand() % 200 + 300;
				}
			}
		}

		if (bing.iAction == ACTION_ATTACK1)
		{
			MTEX = g_ActionInf.GetMonsterTex(bing.info.wLooksShape,bing.iAction,bing.iFrameNow,4,0,0,false,NULL);
			if(MTEX.dwShapeTex != 0)
			{
				pMonster = g_pTexMgr->GetTexFromID(MTEX.dwShapeTex,EP_MONSTER);
			}
		}
		else if (bing.iAction == ACTION_STAND)
		{
			MTEX = g_ActionInf.GetMonsterTex(bing.info.wLooksShape,bing.iAction,g_pGfx->GetFrameCount() / 16 % bing.iStandFrameCount,4,0,0,false,NULL);
			if(MTEX.dwShapeTex != 0)
			{
				pMonster = g_pTexMgr->GetTexFromID(MTEX.dwShapeTex,EP_MONSTER);
			}
		}
		else if (bing.iAction == ACTION_RUN)
		{
			MTEX = g_ActionInf.GetMonsterTex(bing.info.wLooksShape,bing.iAction,g_pGfx->GetFrameCount() / 4 % bing.iRunFrameCount,4,0,0,false,NULL);
			if(MTEX.dwShapeTex != 0)
			{
				pMonster = g_pTexMgr->GetTexFromID(MTEX.dwShapeTex,EP_MONSTER);
			}
		}

		if (pMonster)
		{
			g_pGfx->DrawTextureNL(iTStartX-40,iTStartY-20,pMonster);
		}
		
		g_pFont->DrawText(iTStartX + 160,iTStartY + 60,bing.info.szName,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_MIDDLE);
		
		sprintf(tmp, "当前数量 %d", bing.iCurNum);
		g_pFont->DrawText(iTStartX + 160,iTStartY + 80,tmp,COLOR_TEXT_NORMAL,FONT_YAHEI,FONTSIZE_MIDDLE);
	}	

	DrawBingLineData();
	DrawBingTips();
}

void CDefend12GongWnd::OnDraw()
{
	g_pGfx->DrawTextureNL(m_iScreenX + 681,m_iScreenY,g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,13230,EP_UI));	
}

void CDefend12GongWnd::DrawBingLineData()
{
	g_pFont->DrawText(814 + m_iScreenX, 10 + m_iScreenY,"圣殿守卫军", COLOR_BTN_MOUSEON, FONT_YAHEI, FONTSIZE_BIG,DTF_Center);

	vector<S_Bing>& vBings = g_OtherData.GetBings();

	size_t bingNum = vBings.size();
	int pos = m_pBingGrid->GetScrollPos();

	char sztemp[32] = {0};

	//先画选中效果底框
	for(size_t i = 0; i < D12BING_LINE_COUNT && i + pos < bingNum; i ++)
	{
		sprintf(sztemp,"%s(%d个)",vBings[i+pos].info.szName,vBings[i+pos].info.Num);
		m_pBingGrid->DrawGrid(i,0,sztemp);
	}
}

void CDefend12GongWnd::DrawBingTips()
{
	int iSel = m_pBingGrid->GetSelLine();
	if (iSel >= 0)
	{
		vector<S_Bing>& lBings = g_OtherData.GetBings();
		if (iSel < lBings.size())
		{
			char tmp[64] = {0};
			sprintf(tmp, "攻击          %d-%d", lBings[iSel].info.ac1, lBings[iSel].info.ac2);
			g_pFont->DrawText(m_iScreenX + 715,m_iScreenY + 198,tmp,0xFFFFFF00,FONT_DEFAULT,FONTSIZE_DEFAULT);
			sprintf(tmp, "防御          %d", lBings[iSel].info.df);
			g_pFont->DrawText(m_iScreenX + 715,m_iScreenY + 198 + 21,tmp,0xFFFFFF00,FONT_DEFAULT,FONTSIZE_DEFAULT);
			sprintf(tmp, "生命          %d", lBings[iSel].info.hp);
			g_pFont->DrawText(m_iScreenX + 715,m_iScreenY + 198 + 42,tmp,0xFFFFFF00,FONT_DEFAULT,FONTSIZE_DEFAULT);

			sprintf(tmp, "需要花费荣誉点%u", lBings[iSel].info.money);
			g_pFont->DrawText(m_iScreenX + 715,m_iScreenY + 198 + 63 + 69,tmp,0xFFFFFF00,FONT_DEFAULT,FONTSIZE_DEFAULT);

			sTipsCfg * pTipCfg = g_TipsCfg.GetOtherTips(lBings[iSel].info.szName);
			if (pTipCfg)
			{
				int startx = m_iScreenX + 715;
				int starty = m_iScreenY + 198 + 63 + 69 + 21;
				for (int iLine = 0; iLine < pTipCfg->VLines.size(); iLine ++)
				{
					sTipsCfgLine &line = pTipCfg->VLines.at(iLine);
					vector<sTipsCfgCol> & VCol = line.VCols;

					for (int iCol = 0; iCol < VCol.size(); iCol ++)
					{
						sTipsCfgCol & col = VCol.at(iCol);

						if (col.iPicPackage != 0 && col.iPicIdx != 0)
						{
						}
						else
						{
							g_pFont->DrawText(startx+col.iOffX,starty+col.iOffY,col.strContent.c_str(),col.dwColor,col.iFont,col.iFontSize);
						}
					}

					starty += 21;
				}
			}
		}
	}
}

bool CDefend12GongWnd::OnWheel(int iWheel)
{
	int iMouseX, iMouseY;
	g_pControl->GetMouseXY(iMouseX, iMouseY);

	if (iMouseX < m_iScreenX + 656)
	{
		return m_pLeftScroll->OnWheel(iWheel);
	}

	return CCtrlWindowX::OnWheel(iWheel);
}

bool CDefend12GongWnd::IsInControl( int iX,int iY )
{
	LPTexture pTex = g_pTexMgr->GetTexImm(PACKAGE_INTERFACE,13230,EP_UI);
	if(pTex && pTex->IsPointInTex(iX - 681,iY) == 2)
	{
		return true;
	}

	return CCtrlWindowX::IsInControl(iX,iY);
}

bool CDefend12GongWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_DEFEND12GONG_WND:
		{
			if (dwData == 10)
			{				
				UpdateBings();
				m_pBingGrid->SetTotalCount((int)g_OtherData.GetBings().size());
				m_pLeftScroll->SetRange(((int)g_OtherData.GetBings().size()-1) / 4);
				return true;
			}			
		}
		break;
	case MSG_CTRL_GRID_SELECT_LINE_COL:
		{
			OnClickRow();
		}
		break;	
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pClose2)
			{
				OnClickCloseButton();
				return true;
			}

			vector<S_Bing>& lBings = g_OtherData.GetBings();
			int bingsize = lBings.size();
			for (int i = 0; i < 4; ++i)
			{
				if (pControl == m_pBtnAdd[i])
				{
					int iCurIndex = i + m_pLeftScroll->GetPos() * 4;
					if (iCurIndex < bingsize)
					{
						S_Bing& bing = lBings[iCurIndex];
						g_pGameControl->SEND_EditBingAdd(bing.info.byType,1,0,0);
					}
					return true;
				}
			}				
		}
		break;
	case MSG_INPUT_RIGHTBT_UP:
		break;
	default:
		break;
	}

	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CDefend12GongWnd::OnClickRow()
{
	int row = m_pBingGrid->GetSelLine();
	if(row >= 0 && g_pControl->GetFocusCtrl() == m_pBingGrid)
	{
		m_bClick = false;
		m_bRBClick = false;
	}
}

void CDefend12GongWnd::UpdateBings()
{
	vector<S_Bing>& lBings = g_OtherData.GetBings();

	//lBings.clear();
	//S_Bing bing;
	//memset(&bing.info,0,sizeof(bing.info));
	//bing.info.wLooksShape = 420;
	//lBings.push_back(bing);
	//bing.info.wLooksShape = 419;
	//lBings.push_back(bing);
	//bing.info.wLooksShape = 420;
	//lBings.push_back(bing);
	//bing.info.wLooksShape = 419;
	//lBings.push_back(bing);
	//bing.info.wLooksShape = 420;
	//lBings.push_back(bing);
	//bing.info.wLooksShape = 419;
	//lBings.push_back(bing);

	for (int i = 0; i < lBings.size(); ++i)
	{
		S_Bing& bing = lBings[i];
		bing.iStandContiFrame = rand() % 150 + 100;
		bing.iRunContiFrame = rand() % 200 + 300;
		if (bing.iStandFrameCount == 0)
		{
			stActionPic* ActionPic = g_ActionInf.GetActionPic(ACTION_STAND,CHARACTER_MONSTER,bing.info.wLooksShape);
			if(ActionPic)
			{
				bing.iStandFrameCount = ActionPic->iFrameReal;
			}
		}

		if (bing.iRunFrameCount == 0)
		{
			stActionPic* ActionPic = g_ActionInf.GetActionPic(ACTION_RUN,CHARACTER_MONSTER,bing.info.wLooksShape);
			if(ActionPic)
			{
				bing.iRunFrameCount = ActionPic->iFrameReal;
			}
		}

		if (bing.iAttackFrameCount == 0)
		{
			stActionPic* ActionPic = g_ActionInf.GetActionPic(ACTION_ATTACK1,CHARACTER_MONSTER,bing.info.wLooksShape);
			if(ActionPic)
			{
				bing.iAttackFrameCount = ActionPic->iFrameReal;
			}
		}
	}
}