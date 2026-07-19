#include "MinMapWnd.h"
#include "Global/Interface/TextureInterface.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "Global/GlobalMsg.h"
#include "Baseclass/Control/ParserTip.h"
#include "WndClass/GameWnd/GameManager.h"
#include "GameData/ConfigData.h"
#include "GameData/TalkMgr.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/TipsCfg.h"


#define MINMAP_TEXTCOLOR  0xFFE4C800
#define MINMAP_SELCOLOR   0xFF2F8406

INIT_WND_POSX(CMinMapWnd,POS_VARIABLE,POS_VARIABLE)

CMinMapWnd::CMinMapWnd(void)
{
	m_iIndex = 13997;
	m_iPages = 1;
	
	//m_iAlignType = XAL_TOP;
	//m_iOffX = 0;
	//m_iOffY = 40;

	//if(g_pGfx->IsBig())
	//	m_iOffY = 120;
	//else
	//	m_iOffY = 60;

	m_iAlignType = XAL_CENTER;
	m_iOffX -= 135;


	m_rcArea.left = 19;
	m_rcArea.top = 82;
	m_rcArea.right = 659;
	m_rcArea.bottom = 432;
	m_pAreaMap = NULL;
	m_pJumpPoint = NULL;
	m_pNpc = NULL;

	m_iMinMapX = 0;
	m_iMinMapY = 0;
	m_iMinMapWidth = m_rcArea.right - m_rcArea.left;
	m_iMinMapHeight = m_rcArea.bottom - m_rcArea.top;
	m_fRateX = 2.0f;
	m_fRateY = 1.0f;
	m_bFriend = true;
	m_bEntireWorldMap = false;
	m_bBlock = false;
	m_bMenuDown=false;

	//m_pMapTex = NULL;
	m_wMinMapID = 0;

	m_pPersonGuideWnd = NULL;
	m_bNeedReflashedMapInfo = true;

}

CMinMapWnd::~CMinMapWnd(void)
{
	//g_pTexMgr->ReleaseTex(m_pMapTex);	
	g_pControl->PopupWindow(MSG_CTRL_PERSONS_GUIDE_WND,OPER_CLOSE);
}

void CMinMapWnd::OnCreate()
{
	g_pControl->MsgToWnd(MSG_CTRL_HANDYMAP_WND,MSG_CTRL_FLASH_BTN_WND,1002);
	g_pControl->MsgToWnd(MSG_CTRL_HANDYMAP_WND,MSG_REMOVE_ARROWTIP_CONTROL,EP_FirstWalkToZhongZhou_PaoPao);
	//第一次在中州打开地图界面弹泡泡
	if (!g_pGameData->HasPaoPaoStatus(EP_FirstOpenMapWnd_PaoPao) && strcmp(g_pGameMap->GetMapName(),"0") == 0)
	{
		AddArrowTip(EP_FirstOpenMapWnd_PaoPao,61 + 118,49 + 10,XAL_TOPLEFT,false,XAL_TOPLEFT,0,0,0,false);	
	}


	SetCloseButton(648,3, 80);

	//创建下面三个按钮
	//世界地图按钮
	m_pWorldButton = new CCtrlMenuButton();
	AddControl(m_pWorldButton);
	m_pWorldButton->Create(this,61,49,118,140);
	m_pWorldButton->SetFont(FONT_YAHEI);
	m_pWorldButton->SetMenuTextColor(MINMAP_TEXTCOLOR);
	m_pWorldButton->SetSelColor(MINMAP_TEXTCOLOR,0xFF2F8406);

	//显示队友的Radio Control
	m_pShowFriends=new CCtrlRadio();
	AddControl(m_pShowFriends);
	m_pShowFriends->Create(this,571,48,125,126,127,128);
	m_pShowFriends->SetChecked(true);


	//跳转点
	m_pJumpButton = new CCtrlMenuButton();
	AddControl(m_pJumpButton);
	m_pJumpButton->Create(this,272,49,118,140);
	m_pJumpButton->SetMenuTextColor(MINMAP_TEXTCOLOR);
	m_pJumpButton->SetSelColor(MINMAP_TEXTCOLOR,0xFF2F8406);
	m_pJumpButton->SetFont(FONT_YAHEI);

	//回到前一地图
	m_pBackButton = new CCtrlButton();
	AddControl(m_pBackButton);
	m_pBackButton->CreateEx(this,398,45,90, 91, 92, 93);
	m_pBackButton->SetText("返回前页", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);
	////人物指南
	//m_pPersonGuide = new CCtrlButton();
	//AddControl(m_pPersonGuide);
	//m_pPersonGuide->CreateEx(this,476,45,90, 91, 92, 93);
	//m_pPersonGuide->SetText("人物指南", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

	//设置当前地图的TexID
	CMinMap::VWorldmapInfo_s& WorldMap = g_pMinMap->GetWorldMap();
	m_pWorldButton->AddString("全地图");

	string strCurMapName;
	LocalmapInfo_s* pLocalMap = g_pMinMap->GetLocalMap(g_pGameMap->GetMapName());
	if(pLocalMap)
		strCurMapName = pLocalMap->strMapName;

	for (int i=0;i<WorldMap.size();i++)
	{
		m_pWorldButton->AddString(WorldMap[i]->strMapName.c_str());
		if (strCurMapName == WorldMap[i]->strMapName)
		{
			m_pWorldButton->SetCurSel(i + 1);
		}
	}


	OnUpdateInterface(g_pGameMap->GetMapName());

	//初始化堆栈
	m_strMapFileStack.clear();

	g_pMinMap->ClearFindPoint();

	g_pControl->PopupWindow(MSG_CTRL_PERSONS_GUIDE_WND,OPER_RECREATE);
	m_pPersonGuideWnd = g_pControl->FindWindowByName("PersonsGuideWnd");
	m_pPersonGuideWnd->Move(m_iScreenX + m_iWidth - 3,m_iScreenY);
	m_pPersonGuideWnd->SetOriginalOffX(m_iScreenX + m_iWidth - 3);
	m_pPersonGuideWnd->SetOriginalOffY(m_iScreenY);

}

void CMinMapWnd::ResetWidthAndHeight(int iW,int iH)
{
	CCtrlWindowX::ResetWidthAndHeight(iW,iH);

	m_pPersonGuideWnd->Move(m_iScreenX + m_iWidth - 3,m_iScreenY);
	m_pPersonGuideWnd->SetOriginalOffX(m_iScreenX + m_iWidth - 3);
	m_pPersonGuideWnd->SetOriginalOffY(m_iScreenY);
}


//单击关闭按钮
void CMinMapWnd::OnClickCloseButton()
{
	CloseWindow();
}

void CMinMapWnd::Draw(void)
{
	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	g_pFont->DrawText(28+m_iScreenX, 50+m_iScreenY, "地图                         区域出入口", COLOR_TEXT_NORMAL, FONT_YAHEI);
	if (m_bEntireWorldMap)
	{
		g_pFont->DrawText(310+m_iScreenX, 10 + m_iScreenY, "世界地图", COLOR_BTN_MOUSEON, FONT_YAHEI, FONTSIZE_BIG);
	}
	else
	{
		g_pFont->DrawText(340 + m_iScreenX, 10 + m_iScreenY, m_strMapName.c_str(), COLOR_BTN_MOUSEON, FONT_YAHEI, FONTSIZE_BIG,DTF_Center);
	}
 
	g_pFont->DrawText(597+m_iScreenX, 52+m_iScreenY, "显示队友", COLOR_BTN_PRESS, FONT_YAHEI, FONTSIZE_SMALL);
}

void CMinMapWnd::OnDraw()
{
	DWORD dwCount = GetTickCount();

	DWORD dwMapColor = (g_Config.GetTransColor()) | 0x00FFFFFF;

	//世界全地图时只显示世界全地图
	if (m_bEntireWorldMap)
	{
		LPTexture pEntireMap = g_pTexMgr->GetTex(PACKAGE_stateitem,8000,EP_UI);
		if (pEntireMap)
		{
			int x = m_iScreenX + m_rcArea.left+((m_rcArea.Width() - pEntireMap->GetWidth()) / 2);
			int y = m_iScreenY + m_rcArea.top+((m_rcArea.Height() - pEntireMap->GetHeight()) / 2);
			g_pGfx->DrawTextureNL(x,y,pEntireMap,dwMapColor);
		}

	}
	else
	{
		int iMapScrX = m_iScreenX+m_rcArea.left+m_iMinMapX;
		int iMapScrY = m_iScreenY+m_rcArea.top+m_iMinMapY;


		LPTexture pTex = NULL;
		if (m_wMinMapID != 0)
		{
			pTex = g_pTexMgr->GetTexImm(PACKAGE_timap,m_wMinMapID,EP_UI);
			if(pTex == NULL && !g_pTexMgr->HasTexInServer(PACKAGE_timap,m_wMinMapID))//没有图有可能是还没有下载下来,也有可能是本来就没有
				pTex = g_pTexMgr->GetTexImm(PACKAGE_mmap,m_wMinMapID,EP_UI);
		}
		//地图
		if (pTex)
		{
			g_pGfx->DrawTextureNL(iMapScrX,iMapScrY,pTex,dwMapColor);
			if (m_bNeedReflashedMapInfo)
			{
				OnUpdateInterface(m_strMapFile.c_str());
				m_bNeedReflashedMapInfo = false;
			}
		}
		else
		{
			g_pGfx->DrawRect(iMapScrX,iMapScrY,m_iMinMapWidth,m_iMinMapHeight,0xFF000000);
		}

		if (m_pAreaMap && m_pAreaMap->iWidth>0 && m_pAreaMap->iHeight>0)
		{
			//当前地图绘制自动寻路的路径
			if (m_strMapFile.compare(g_pGameMap->GetMapName()) == 0 && SELF.GetMapPathFinder().IsOnRoute())
			{
				vectorPoint& VecPoint = SELF.GetMapPathFinder().GetCurrentRoute();
                DWORD dwLineColor = 0xFF00FFFF;

				int selfX, selfY;
                SELF.GetXY(selfX,selfY);
				int iLastX = (int)(iMapScrX + selfX*m_fRateX);
				int iLastY = (int)(iMapScrY + selfY*m_fRateY);

				int iMidX = iLastX;
				int iMidY = iLastY;

				for(int i = 0;i < (int)VecPoint.size();i++)
				{
					if(i == 0)
					{
						iMidX = (int)(iMapScrX + VecPoint[0].x*m_fRateX);
						iMidY = (int)(iMapScrY + VecPoint[0].y*m_fRateY);		
					}

					int iRealX = 0,iRealY = 0;

					//计算方向，不是最后一个点的时候，如果同方向合并跳过此点
					if(i + 1 < (int)VecPoint.size())
					{
						iRealX = (int)(iMapScrX + VecPoint[i+1].x*m_fRateX);
						iRealY = (int)(iMapScrY + VecPoint[i+1].y*m_fRateY);

						//如果同方向继续
						if((iRealX - iMidX)*(iMidY - iLastY) == (iRealY - iMidY)*(iMidX - iLastX))
						{
							iMidX  = iRealX;
							iMidY  = iRealY;
							continue;
						}
					}

					g_pGfx->DrawGouraudLine(iLastX,iLastY,iMidX,iMidY,dwLineColor,dwLineColor);
                    
					iLastX = iMidX;
                    iLastY = iMidY;
					iMidX  = iRealX;
					iMidY  = iRealY;
				}
			}

			if (m_strMapFile.compare(g_pGameMap->GetMapName()) == 0)
			{//当前地图
				MDynMiniMapObj& MiniMapObj = g_pGameData->GetDynMinMapObj();
				MDynMiniMapObj::iterator it = MiniMapObj.find(m_strMapFile);
				if (it != MiniMapObj.end())
				{
					std::vector<DynMiniMapObj>& VDynMinMap = it->second;
					for (int i = 0;i < VDynMinMap.size();i++)
					{
						if(VDynMinMap[i].bShow)
						{
							LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,VDynMinMap[i].wTex,EP_UI);
							if (pTex)
							{
								int x = iMapScrX + (int)(VDynMinMap[i].wX*m_fRateX);
								int y = iMapScrY + (int)(VDynMinMap[i].wY*m_fRateY);
								g_pGfx->DrawTextureNL(x,y,pTex);
								if (VDynMinMap[i].strDes.size() > 0)
								{
									g_pFont->DrawText(x,y - 5,VDynMinMap[i].strDes.c_str(),0xFF00FF00);
								}
							}
						}
					}
				}
			}

			//绘制跳转点NPC和队友信息
			DWORD dwColor = 0;
			int ii = 0;

			for(ii = 0;ii < m_pAreaMap->VJump.size();ii++)
			{
				JumplumpInfo_s& inf = m_pAreaMap->VJump.at(ii);
				if (inf.bShowJump)
				{
					int x = iMapScrX + (int)(inf.iSrcX*m_fRateX);
					int y = iMapScrY + (int)(inf.iSrcY*m_fRateY);
					g_pGfx->DrawFillRect(x-1,y-1,3,3,0xFF0000FF);
				}
			}

			for(ii = 0;ii < m_pAreaMap->VNpc.size();ii++)
			{
				NpcInfo_s& inf = m_pAreaMap->VNpc.at(ii);
				int x = iMapScrX + (int)(inf.iX*m_fRateX);
				int y = iMapScrY + (int)(inf.iY*m_fRateY);
				g_pGfx->DrawFillRect(x-1,y-1,3,3,0xFFFF0000); //NPC颜色
			}

			CMinMap::MExLocalNpcInfo& exeNpc = g_pMinMap->GetExLocalNpc();
			CMinMap::MExLocalNpcInfo::iterator it = exeNpc.find(m_strMapFile);
			if (it != exeNpc.end())
			{
				//找到
				vector<NpcInfo_s>& vexeNpc = it->second;
				for (int i = 0;i < vexeNpc.size();i++)
				{
					NpcInfo_s& inf = vexeNpc.at(i);
					int x = iMapScrX + (int)(inf.iX*m_fRateX);
					int y = iMapScrY + (int)(inf.iY*m_fRateY);
					if(inf.iExNpcType == 1)//行会塔
					{
						LPTexture pTowerTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16285,EP_UI);
						g_pGfx->DrawTextureNL(x-8,y-7,pTowerTex);
					}
					else if(inf.iExNpcType == 2 || inf.iExNpcType == 3)//飞升台
					{
						LPTexture pTowerTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,16263 + inf.iExNpcType,EP_UI);
						g_pGfx->DrawTextureNL(x-8,y-7,pTowerTex);
					}
				}
			}

			//显示选中的跳转点
			if (m_pJumpPoint)
			{
				int x = iMapScrX + (int)(m_pJumpPoint->iSrcX*m_fRateX);
				int y = iMapScrY + (int)(m_pJumpPoint->iSrcY*m_fRateY);
				g_pGfx->DrawRect(x-2,y-2,5,5,0xFFFFFF00);
			}

			//显示选中的NPC
			if (m_pNpc)
			{
				int x = iMapScrX + (int)(m_pNpc->iX*m_fRateX);
				int y = iMapScrY + (int)(m_pNpc->iY*m_fRateY);
				g_pGfx->DrawRect(x-2,y-2,5,5,0xFFFFFF00);
			}

			//当前地图绘制队友信息
			if (m_strMapFile.compare(g_pGameMap->GetMapName()) == 0)
			{
				if (m_bFriend && g_pGameData->GetTroopMembers() > 0)
				{
					static DWORD dwGroupPosTime = 0;
					if(dwCount-dwGroupPosTime >3000)
					{
						dwGroupPosTime = dwCount;
						g_pGameControl->SEND_Group_Query();
					}

					int nFriendSize = g_pGameData->GetGroupPos().size();
					for(int i=0; i<nFriendSize; ++i)
					{
						int iFriendX = g_pGameData->GetGroupPos()[i]>>16; //高位x
						int iFriendY = g_pGameData->GetGroupPos()[i] & 0x0000FFFF;

						int x = iMapScrX + (int)(iFriendX*m_fRateX);
						int y = iMapScrY + (int)(iFriendY*m_fRateY);
						g_pGfx->DrawFillRect(x-1,y-1,3,3,0xFFFFFF00);
					}
				}

				//绘制自己
				int iSelfX = 0,iSelfY = 0;
				SELF.GetXY(iSelfX,iSelfY);
				int x = iMapScrX + (int)(iSelfX*m_fRateX);
				int y = iMapScrY + (int)(iSelfY*m_fRateY);

				if ((dwCount % 400) < 200)
					g_pGfx->DrawRect(x-2,y-2,5,5,0xFF00FF00);

				//绘制选择的点
				int iFindPointX = 0,iFindPointY = 0;
				if(g_pMinMap->GetFindPoint(iFindPointX,iFindPointY))
				{
					x = iMapScrX + (int)(iFindPointX*m_fRateX);
					y = iMapScrY + (int)(iFindPointY*m_fRateY);
					g_pGfx->DrawFillRect(x-1,y-1,3,3,0xFFFF00FF);
					g_pGfx->DrawRect(x-2,y-2,5,5,0xFF0000FF);
				}

			}
		}
	}


	//g_pGfx->DrawTextureNL(m_iScreenX + 19,m_iScreenY + m_iHeight - 71,g_pTexMgr->GetTex(PACKAGE_INTERFACE,14300));
	//if (m_bEntireWorldMap)
	//{
	//	g_pFont->DrawText(m_iScreenX + 19 + 63,m_iScreenY + m_iHeight - 71 + 11,"世界地图",0xFF172020,FONT_LISHU,20,DTF_Center);
	//}
	//else
	//{
	//	g_pFont->DrawText(m_iScreenX + 19 + 63,m_iScreenY + m_iHeight - 71 + 11,m_strMapName.c_str(),0xFF172020,FONT_LISHU,20,DTF_Center);
	//}

}

bool CMinMapWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_MINIMAP_WND_REFRESH:
		{
			CMinMap::VWorldmapInfo_s& WorldMap = g_pMinMap->GetWorldMap();

			string strCurMapName;
			LocalmapInfo_s* pLocalMap = g_pMinMap->GetLocalMap(g_pGameMap->GetMapName());
			if(pLocalMap)
				strCurMapName = pLocalMap->strMapName;

			for (int i=0;i<WorldMap.size();i++)
			{
				m_pWorldButton->AddString(WorldMap[i]->strMapName.c_str());
				if (strCurMapName == WorldMap[i]->strMapName)
				{
					m_pWorldButton->SetCurSel(i + 1);
				}
			}

			OnUpdateInterface(g_pGameMap->GetMapName());
		}
		break;
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl == m_pWorldButton)
		{
			if (m_pWorldButton && m_pWorldButton->IsMenuDown())
			{
				this->Msg(MSG_REMOVE_ARROWTIP_CONTROL,EP_FirstOpenMapWnd_PaoPao,NULL);
			}
			return true;
		}
		else if (pControl == m_pJumpButton)
		{
			if (m_pJumpButton && m_pJumpButton->IsMenuDown())
			{
				this->Msg(MSG_REMOVE_ARROWTIP_CONTROL,EP_FirstSelectMap_PaoPao,NULL);
			}
			return true;
		}
		if (pControl == m_pShowFriends)
		{
			m_bFriend = m_pShowFriends->IsChecked();
			return true;
		}
		else if (pControl == m_pBackButton)
		{
			if (!m_strMapFileStack.empty())
			{
				string str = m_strMapFileStack.back();
				m_strMapFileStack.pop_back();
				OnUpdateInterface(str.c_str());
			}
			return true;
		}
		//else if (pControl == m_pPersonGuide)
		//{
		//	g_pControl->PopupWindow(MSG_CTRL_PERSONS_GUIDE_WND,OPER_CREATE);
		//	return true;
		//}
		else
		{
			if(m_pAreaMap)
			{
				for (int i = 0; i < m_VJumpBtn.size(); i++)
				{
					CCtrlButton *p = m_VJumpBtn[i];
					if (pControl == p)
					{
						JumplumpInfo_s& inf = m_pAreaMap->VJump.at(p->GetData());
						bool bSucess = g_MapFinder.AutoLookforPath(g_pGameMap->GetMapName(),0,0,inf.strDestMapFile.c_str(),inf.iDestX,inf.iDestY,false);
						if (!bSucess)
						{
							g_MsgBoxMgr.PopSimpleAlert("无法自动走到这个位置",0,0);
						}
						break;
						return true;
					}
				}
			}
		}

		break;
	case MSG_CTRL_MENU_SELCHANGED:
		{
			if(pControl == m_pWorldButton)
			{
				int iSel = m_pWorldButton->GetCurSel();
				if (iSel<0)
					return true;
				if (iSel == 0)
				{
					m_bEntireWorldMap = !m_bEntireWorldMap;

					if(m_bEntireWorldMap)
					{
						strcpy(m_currentMinMapName,m_pWorldButton->GetText());
						m_pWorldButton->SetCurSel(0);
					}
					else
					{
						m_pWorldButton->SetCurSel(m_currentMinMapName);
					}

					//清除跳转按钮
					for (int i = 0; i < m_VJumpBtn.size(); i++)
					{
						CControl *p = m_VJumpBtn[i];
						RemoveControl(&p);
					}
					m_VJumpBtn.clear();
					//

					return true;
				}
				else
				{
					m_bEntireWorldMap = false;
				}

				WorldmapInfo_s* pWorldMap = g_pMinMap->GetWorldMap().at(iSel-1);
				if(pWorldMap)
				{
					if (m_strMapFile != pWorldMap->strMapFile)
					{
						if (!m_strMapFile.empty())
						{
							if (m_strMapFileStack.size() > MAX_MAPFILE_BACK_COUNT)
								m_strMapFileStack.pop_front();
							m_strMapFileStack.push_back(m_strMapFile);
						}
						OnUpdateInterface(pWorldMap->strMapFile.c_str());
						//m_pWorldButton->SetCurSel(pWorldMap->strMapName.c_str());
						m_pWorldButton->SetText(pWorldMap->strMapName.c_str());
					}
				}

				//第一次在中州对地图位置进行了选择时弹泡泡
				if (!g_pGameData->HasPaoPaoStatus(EP_FirstSelectMap_PaoPao) && strcmp(g_pGameMap->GetMapName(),"0") == 0)
				{
					AddArrowTip(EP_FirstSelectMap_PaoPao,272 + 118,49 + 10,XAL_TOPLEFT,false,XAL_TOPLEFT,0,0,0,false);	
				}

				return true;
			}
			else if(pControl == m_pJumpButton)
			{
				if (!m_pAreaMap)
					return true;

				int iSel = m_pJumpButton->GetCurSel();
				if (iSel<0)
					return true;

				JumplumpInfo_s& inf = m_pAreaMap->VJump.at(iSel);

				if (m_strMapFile.compare(inf.strDestMapFile) != 0)
				{
					if (!m_strMapFile.empty())
					{
						if (m_strMapFileStack.size() > MAX_MAPFILE_BACK_COUNT)
							m_strMapFileStack.pop_front();
						m_strMapFileStack.push_back(m_strMapFile);
					}
					OnUpdateInterface(inf.strDestMapFile.c_str());
				}
				return true;
			}
			return true;
		}
	case MSG_CTRL_MENU_SELCHANGING:
		{
			if (!pControl)
				return true;
			if (!m_pAreaMap)
				return true;
			return true;
		}
	case MSG_CTRL_MENU_RSELCHANGED:
		{
			if (pControl == m_pJumpButton)
			{
				if (!m_pAreaMap)
					return true;
				int iSel = m_pJumpButton->GetCurSel();
				if (iSel<0)
					return true;

				m_pJumpPoint = &m_pAreaMap->VJump.at(iSel);
				return true;
			}
			else
			{
				return Msg(MSG_CTRL_MENU_SELCHANGED,dwData,pControl);
			}
			return true;
		}
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

bool CMinMapWnd::OnMouseMove(int iX, int iY)
{
	CParserTip* pTip = g_pControl->GetTipWnd();
	pTip->SetShow(false);
	pTip->Clear();

	if (IsInArea(iX,iY) && !m_pWorldButton->IsMenuDown() && !m_pJumpButton->IsMenuDown())
	{
		if (!m_bEntireWorldMap && IsInMapArea(iX,iY))
		{
			int iPos = 0;
			for (; iPos < m_VJumpBtn.size(); iPos++)
			{
				CControl *p = m_VJumpBtn[iPos];
				if (p && iX >= p->GetX() && iX <p->GetX() + p->GetWidth() && iY >= p->GetY() && iY < p->GetY() + p->GetHeight())
				{
					break;
				}
			}

			bool bShowTips = false;
			//鼠标在某个按钮上
			if (m_pAreaMap && iPos >= 0 && iPos < m_VJumpBtn.size())
			{
				int iJumpPos = m_VJumpBtn[iPos]->GetData();
				if (iJumpPos >= 0 && iJumpPos < m_pAreaMap->VJump.size())
				{
					JumplumpInfo_s& inf = m_pAreaMap->VJump.at(iJumpPos);
					if (!inf.szTipsName.empty())
					{
						//配置物品说明
						sTipsCfg * pTipCfg = g_TipsCfg.GetOtherTips(inf.szTipsName.c_str());
						if(pTipCfg)
						{
							pTip->AddCfgTips(pTipCfg);
							bShowTips = true;
						}
					}
				}
			}
			else
			{
				bShowTips = true;
				int iRealX = (int)(float(iX-m_rcArea.left-m_iMinMapX) / m_fRateX);
				int iRealY = (int)(float(iY-m_rcArea.top-m_iMinMapY) / m_fRateY);

				char temp[64] = {0};
				sprintf(temp,"%d:%d",iRealX,iRealY);
				if (m_strMapFile.compare(g_pGameMap->GetMapName()) == 0)
					m_bBlock = g_pGameMap->IsBlock(iRealX,iRealY);
				else
					m_bBlock = false;

				SetInfoTip(iRealX,iRealY,(int)(3/min(m_fRateX,m_fRateY)));
				pTip->AddText(temp,0xFF00FF00);
			}

			pTip->SetShow(bShowTips);
			pTip->Move(m_iScreenX+iX-pTip->GetWidth(),m_iScreenY + iY-pTip->GetHeight());
		}
		else
		{
			pTip->SetShow(false);
		}
		return true;
	}
	else
	{
		pTip->SetShow(false);
	}

	return CCtrlWindowX::OnMouseMove(iX,iY);
}

bool CMinMapWnd::OnLeftButtonDown(int iX, int iY)
{
	if (IsInArea(iX,iY))
	{
		if (m_strMapFile.compare(g_pGameMap->GetMapName()) == 0 && IsInMapArea(iX,iY)&&!m_bEntireWorldMap)
		{
			if(SELF.GetBoothState())
				return true;


			int iRealX = (int)(float(iX-m_rcArea.left-m_iMinMapX) / (m_fRateX));
			int iRealY = (int)(float(iY-m_rcArea.top-m_iMinMapY) / m_fRateY);

			if(!g_pGameMap->IsBlock(iRealX,iRealY))
			{
// 				if (g_TrusteeshipData.IsSelGarrsionXY())
// 				{
// 					g_pGameData->SetMouseType(MOUSE_STANDARD);
// 					g_TrusteeshipData.SetSelGarrsionXY(false);
// 					for (int i = 0; i < MAX_TRUSTEESHIP_NUM; i++)
// 					{
// 						if (g_TrusteeshipData.GetTneupMember(i).bSelected)
// 						{
// 							g_pGameControl->Send_TrusteeshipMode(g_TrusteeshipData.GetTneupMember(i).szName, eTrusteeshipModeGarrsion, MAKELONG(iRealX, iRealY));
// 						}
// 					}
// 					return true;
// 				}

				////微操模式下禁止自动寻路
				//if (g_TrusteeshipData.GetMicroControlPos() >= 0)
				//{
				//	g_TalkMgr.PushSysTalk("操控其他队员时不能使用此功能！");
				//}
				//else
				{
					g_pGameMgr->AI_AutoWalk(iRealX,iRealY);
				}
			}
			else
			{
				g_TalkMgr.PushSysTalk("无法行走到这里！");
			}
		}
		return true;
	}
	return CCtrlWindowX::OnLeftButtonDown(iX,iY);
}

bool CMinMapWnd::OnLeftButtonUp(int iX, int iY)
{
	if (IsInArea(iX,iY))
	{
		return true;
	}
	return CCtrlWindowX::OnLeftButtonUp(iX,iY);
}

bool CMinMapWnd::IsInArea(int iX,int iY)
{
	return m_rcArea.PtInRect(iX,iY);
}

bool CMinMapWnd::IsInMapArea(int iX,int iY)
{
	int x = iX-m_rcArea.left-m_iMinMapX;
	int y = iY-m_rcArea.top-m_iMinMapY;
	if (x>=0 && x<=m_iMinMapWidth &&y>=0 && y<=m_iMinMapHeight)
		return true;

	return false;
}

void CMinMapWnd::OnUpdateInterface(const char* szMapFile)
{
	if (NULL == szMapFile || strlen(szMapFile) == 0)
		return;

	////还是原来那个地图则不更新
	//if (m_strMapFile.compare(szMapFile) == 0)
	//	return ;

	m_strMapFile.assign(szMapFile);
	m_bNeedReflashedMapInfo = true;

	m_pAreaMap = g_pMinMap->GetLocalMap(szMapFile);
	if (m_pAreaMap)
	{
		m_pJumpButton->ResetContent();
		m_strMapName = m_pAreaMap->strMapName;

		int ii = 0;
		for(ii = 0;ii < m_pAreaMap->VJump.size();ii++)
		{
			JumplumpInfo_s &info = m_pAreaMap->VJump.at(ii);
			if (info.bShowJump)
			{
				string strMapFile = info.strDestMapFile;
				LocalmapInfo_s* loc = g_pMinMap->GetLocalMap(strMapFile.c_str());
				if(loc)
				{
					char temp[256] = {0};
					sprintf(temp,"%s",loc->strMapName.c_str());
					m_pJumpButton->AddString(temp);
				}
			}
		}

		m_pJumpButton->SetCurSel(m_pAreaMap->strMapName.c_str());
	}
	else
	{
		m_strMapName.clear();
		m_pJumpButton->ResetContent();
	}

	//删除并创建新的Texture
	//if (m_pMapTex)
	//{
	//	g_pTexMgr->ReleaseTex(m_pMapTex);
	//	m_pMapTex = NULL;
	//}

	m_wMinMapID = g_pMinMap->GetMinMapID(szMapFile);

	LPTexture pTex = NULL;
	if (m_wMinMapID != 0)
	{
		pTex = g_pTexMgr->GetTexImm(PACKAGE_timap,m_wMinMapID,EP_UI);
		if(pTex == NULL && !g_pTexMgr->HasTexInServer(PACKAGE_timap,m_wMinMapID))//没有图有可能是还没有下载下来,也有可能是本来就没有
			pTex = g_pTexMgr->GetTexImm(PACKAGE_mmap,m_wMinMapID,EP_UI);
	}

	//计算偏移和缩放比例
	if(!m_pAreaMap || m_pAreaMap->iWidth<=0 || m_pAreaMap->iHeight<=0)
	{
		m_fRateX = 2.0f;
		m_fRateY = 1.0f;

		m_iMinMapWidth = m_rcArea.Width();
		m_iMinMapHeight = m_rcArea.Height();
		m_iMinMapX = 0;
		m_iMinMapY = 0;
	}
	else if (pTex)
	{
		m_fRateX = (float)(pTex->GetWidth()) / m_pAreaMap->iWidth;
		m_fRateY = (float)(pTex->GetHeight()) / m_pAreaMap->iHeight;
		m_iMinMapWidth = pTex->GetWidth();
		m_iMinMapHeight = pTex->GetHeight();
		m_iMinMapX = (m_rcArea.Width() - m_iMinMapWidth) / 2;
		m_iMinMapY = (m_rcArea.Height() - m_iMinMapHeight) / 2;
	}
	else
	{
		//计算偏移和缩放比例
		m_fRateX = (float)(m_rcArea.Width()) / m_pAreaMap->iWidth;
		m_fRateY = (float)(m_rcArea.Height()) / m_pAreaMap->iHeight;

		if(m_fRateX < m_fRateY*2)
			m_fRateY = m_fRateX / 2;
		else
			m_fRateX = m_fRateY * 2;

		m_iMinMapWidth = (int)(m_pAreaMap->iWidth * m_fRateX);
		m_iMinMapHeight = (int)(m_pAreaMap->iHeight * m_fRateY);
		m_iMinMapX = (m_rcArea.Width() - m_iMinMapWidth) / 2;
		m_iMinMapY = (m_rcArea.Height() - m_iMinMapHeight) / 2;
	}

	g_pControl->GetTipWnd()->Clear();
	g_pControl->GetTipWnd()->SetShow(false);

	m_pJumpPoint = NULL;
	m_pNpc = NULL;



	ReflashJumBtn();

}

void CMinMapWnd::SetInfoTip(int iRealX,int iRealY,int iDelta)
{
	int iTipCount = 0;

	CParserTip* pTip = g_pControl->GetTipWnd();
	pTip->Clear();

	if(m_pAreaMap)
	{
		int ii = 0;
		for(ii = 0;ii < m_pAreaMap->VJump.size();ii++)
		{
			JumplumpInfo_s& inf = m_pAreaMap->VJump.at(ii);

			if(inf.bShowJump && abs(inf.iSrcX-iRealX)<iDelta && abs(inf.iSrcY-iRealY)<iDelta)
			{
				LocalmapInfo_s* loc = g_pMinMap->GetLocalMap(inf.strDestMapFile.c_str());
				if(loc)
				{
					char temp[1024] = {0};
					sprintf(temp,"去%s(%d,%d)",loc->strMapName.c_str(),inf.iDestX,inf.iDestY);
					pTip->AddText(temp,0xFF00FF00);
					iTipCount++;
				}
			}
		}

		for(ii = 0;ii < m_pAreaMap->VNpc.size();ii++)
		{
			NpcInfo_s& inf = m_pAreaMap->VNpc.at(ii);
			if(abs(inf.iX-iRealX)<iDelta && abs(inf.iY-iRealY)<iDelta)
			{
				char temp[1024] = {0};
				strcpy(temp,inf.strNpcName.c_str());
				if(!inf.strDesc.empty())
				{
					strcat(temp,":");
					strcat(temp,inf.strDesc.c_str());
				}
				pTip->AddText(temp,0xFF00FF00);
				iTipCount++;
			}
		}

		CMinMap::MExLocalNpcInfo& exeNpc = g_pMinMap->GetExLocalNpc();
		CMinMap::MExLocalNpcInfo::iterator it = exeNpc.find(m_strMapFile);
		if (it != exeNpc.end())
		{
			//找到
			vector<NpcInfo_s>& vexeNpc = it->second;
			for (int i = 0;i< vexeNpc.size();i++)
			{
				NpcInfo_s& inf = vexeNpc.at(i);
				if(abs(inf.iX-iRealX)<iDelta && abs(inf.iY-iRealY)<iDelta)
				{
					char temp[1024] = {0};
					strcpy(temp,inf.strNpcName.c_str());
					if(!inf.strDesc.empty())
					{
						if (inf.iExNpcType != 4)
							strcat(temp,":");
						strcat(temp,inf.strDesc.c_str());
					}
					pTip->AddText(temp,0xFF00FF00);
					iTipCount++;
				}
			}
		}
	}
	pTip->SetShow(iTipCount>0);
}

void CMinMapWnd::OnMove()
{
	CCtrlWindowX::OnMove();
	
	if(g_pControl->GetFocusCtrl() == this && m_pPersonGuideWnd)
	{
		m_pPersonGuideWnd->Move(m_iScreenX + m_iWidth - 3,m_iScreenY);
	}	
}

void CMinMapWnd::OnSwitchToTop()
{
	if(m_pPersonGuideWnd)
	{
		g_pControl->SwitchToTop(m_pPersonGuideWnd,false);
	}
}

void CMinMapWnd::ReflashJumBtn()
{
	for (int i = 0; i < m_VJumpBtn.size(); i++)
	{
		CControl *p = m_VJumpBtn[i];
		RemoveControl(&p);
	}
	m_VJumpBtn.clear();

	if (!m_pAreaMap)
	{
		return;
	}

	for(int i = 0;i < m_pAreaMap->VJump.size();i++)
	{
		JumplumpInfo_s& inf = m_pAreaMap->VJump.at(i);
		if (inf.bShowBtn)
		{
			string strShow = inf.szShowDestName;
			if (strShow.empty())
			{
				LocalmapInfo_s* loc = g_pMinMap->GetLocalMap(inf.strDestMapFile.c_str());
				if(loc)
				{
					strShow = loc->strMapName;
				}
			}

			if(!strShow.empty())
			{
				int iX = (int)(inf.iSrcX * m_fRateX) + m_rcArea.left + m_iMinMapX;
				int iY = (int)(inf.iSrcY * m_fRateY) + m_rcArea.top + m_iMinMapY;
				if (iX >= m_iWidth - 105)
				{
					iX = iX - 105;
				}
				if (iY >= m_iHeight - 40)
				{
					iY = iY - 40;
				}

				CCtrlButton *p = new CCtrlButton();
				AddControl(p);
				if (inf.iType == 0)//地表地图
				{
					p->CreateEx(this,iX,iY,14305,14306, 14307);
					p->SetText(strShow.c_str(), 0xFF80C269 ,0xFFB3D465, 0xFF3C6D62, COLOR_BTN_DISABLE, FONTSIZE_DEFAULT, 0, FONT_YAHEI);
				}
				else if (inf.iType == 1)//地宫地图
				{
					p->CreateEx(this,iX,iY,14301,14302, 14303);
					p->SetText(strShow.c_str(), 0xFFF1AF65 ,0xFFFAC897, 0xFF7A3924, COLOR_BTN_DISABLE, FONTSIZE_DEFAULT, 0, FONT_YAHEI);
				}

				p->SetData(i);//记下来以便点击按钮时处理

				m_VJumpBtn.push_back(p);			
			}
		}
	}
}
