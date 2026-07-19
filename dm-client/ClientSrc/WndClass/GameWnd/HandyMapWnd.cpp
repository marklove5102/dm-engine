#include "HandyMapWnd.h"
#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "GameData/SimpleCharacter.h"
#include "GameMap/GameMap.h"
#include "GameAI/AIAutoPickMgr.h"
#include "GameAI/AIPromptMgr.h"
#include "GameData/OtherData.h"
#include "GameData/MsgBoxMgr.h"
#include "GameAI/AIConfigMgr.h"
#include "Engine/TextureD3D.h"
#include "GameClient/SDOAInterface.h"


INIT_WND_POSX(CHandyMapWnd,POS_VARIABLE,POS_VARIABLE)


int CHandyMapWnd::m_siHandyMapStatus = 1;
CHandyMapWnd::CHandyMapWnd(void)
{

	m_iControlMode = CTRL_MODE_HANDYMAPWND;

	m_bNoMove = true;
	m_bNoChangeLevel = true;
	m_bNeedSavePos  = false;
	m_bDisableEscape = true;

	m_sName = "HandyMapWnd";


	m_pMinMax		= NULL;
	m_pCourseTable	= NULL;
	m_pPublicBord	= NULL;
	m_pWorldMap		= NULL;
	m_pInfoInform	= NULL;
	m_pNpcAutoPath	= NULL;
	m_pUITypeChange = NULL;
	m_pSystemSetting= NULL;
	m_pWenwen		= NULL;

	m_iPages = 1;
	m_iAlignType = XAL_TOPRIGHT;
	m_iOffX = 3;
	m_iOffY = 0;

	int iX = 0,iY = 0;

	//if (g_EutUiType == EUT_CLASSIC)//典型
	//{
	//	if(m_siHandyMapStatus != 0) 
	//		m_iIndex = 13303;
	//	else 
	//		m_iIndex = 13319;
	//} 
	//else//
	//{
		if(m_siHandyMapStatus != 0) 
			m_iIndex = 23090;
		else 
			m_iIndex = 21933;
	//}

	//m_pTemplate = g_pTexMgr->LoadTex(PACKAGE_INTERFACE,21901);

}

CHandyMapWnd::~CHandyMapWnd(void)
{
	//if (m_pTemplate)
	//{
	//	g_pTexMgr->ReleaseTex(m_pTemplate);
	//}
}

void CHandyMapWnd::OnCreate()
{
	m_pCourseTable	= NULL;
	m_pPublicBord	= NULL;
	m_pWorldMap		= NULL;
	m_pInfoInform	= NULL;
	m_pNpcAutoPath	= NULL;
	m_pUITypeChange = NULL;
	m_pSystemSetting= NULL;
	m_pWenwen		= NULL;

	if(m_pMinMax == NULL)
		m_pMinMax		= new CCtrlButton();

	AddControl(m_pMinMax);

	m_pWorldMap	= new CCtrlButton();	
	m_pPublicBord	= new CCtrlButton();
	m_pCourseTable	= new CCtrlButton();	
	m_pNpcAutoPath	= new CCtrlButton();
	m_pNewHandPrompt= new CCtrlButton();	
	m_pUITypeChange = new CCtrlButton();
	m_pInfoInform	= new CCtrlButton();

	if(m_siHandyMapStatus != 0)
	{
		AddControl(m_pWorldMap);
		AddControl(m_pPublicBord);
		AddControl(m_pUITypeChange);
		AddControl(m_pInfoInform);
	}
	AddControl(m_pCourseTable);	
	AddControl(m_pNewHandPrompt);
	AddControl(m_pNpcAutoPath);

	//if (g_EutUiType == EUT_CLASSIC)//典型
	//{
	//	if(m_siHandyMapStatus == 0)
	//	{
	//		m_pMinMax->SetAlignType(XAL_TOPRIGHT);
	//		m_pMinMax->CreateEx(this,-4,2,13325,13326,13327);
	//		m_pNewHandPrompt->SetAlignType(XAL_TOPRIGHT);
	//		m_pNewHandPrompt->CreateEx(this,-124,7,13341,13342,13343);
	//		m_pNpcAutoPath->SetAlignType(XAL_BOTTOMRIGHT);
	//		m_pNpcAutoPath->CreateEx(this,-61,-7,13300,13301,13302);
	//		m_pCourseTable->SetAlignType(XAL_BOTTOMRIGHT);
	//		m_pCourseTable->CreateEx(this,-4,-7,13307,13308,13309);
	//		SetMask(13303);		
	//	}
	//	else
	//	{
	//		m_pMinMax->SetAlignType(XAL_TOPRIGHT);
	//		m_pMinMax->CreateEx(this,-4,2,13316,13317,13318);	

	//		int iLeft = 2;
	//		int iBottom = 25;	
	//		m_pUITypeChange->CreateEx(this,iLeft,iBottom,13349,13350,13351);
	//		iBottom += 24;
	//		m_pWorldMap->CreateEx(this,iLeft,iBottom,13310,13311,13312);
	//		iBottom += 24;
	//		m_pInfoInform->CreateEx(this,iLeft,iBottom,13313,13314,13315);
	//		iBottom += 24;
	//		m_pPublicBord->CreateEx(this,iLeft,iBottom,13304,13305,13306);
	//		iBottom += 24;

	// 		m_pNpcAutoPath->SetAlignType(XAL_BOTTOMLEFT);
	//		m_pNpcAutoPath->CreateEx(this,36,- 7,13300,13301,13302);
	//		m_pCourseTable->SetAlignType(XAL_BOTTOMLEFT);
	//		m_pCourseTable->CreateEx(this,93,- 7,13307,13308,13309);
	//		m_pNewHandPrompt->CreateEx(this,0,137,13341,13342,13343);
	//		SetMask(13319);	
	//	}

	//	m_bNoMove = true;
	//} 
	//else//时尚
	//{
		m_pSystemSetting = new CCtrlButton();
		AddControl(m_pSystemSetting);

		//AP不要彩虹帮助,里面有广告,彩虹无法去掉,平台不接受
		if (g_strChannelName.empty())
		{
			m_pWenwen = new CCtrlButton();
			AddControl(m_pWenwen);
		}


		if(m_siHandyMapStatus == 0)
		{
			m_pMinMax->CreateEx(this,117,1,21926,21927,21928);
		}
		else
		{
			m_pMinMax->CreateEx(this,191,1,21926,21927,21928);
			m_pNewHandPrompt->CreateEx(this,168,20,21923,21924,21925);
			m_pNpcAutoPath->CreateEx(this,37,78,21902,21903,21904);
			m_pCourseTable->CreateEx(this,29,33,23092,23093,23094);
			m_pInfoInform->CreateEx(this,74,140,21908,21909,21910);
			m_pPublicBord->CreateEx(this,101,151,21911,21912,21913);
			m_pWorldMap->CreateEx(this,130,152,21914,21915,21916);
			m_pUITypeChange->CreateEx(this,186,56,21917,21918,21919);
			m_pSystemSetting->CreateEx(this,186,119,21920,21921,21922);

			SetMask(23090);
			m_pSystemSetting->SetTips("系统设置");

			if(m_pWenwen)
			{
				m_pWenwen->CreateEx(this,166,139,21935,21936,21937);			
				m_pWenwen->SetTips("彩虹帮助");
			}
		}
	//}

	if (m_siHandyMapStatus != 0)
	{
		m_pUITypeChange->SetTips("界面切换");
		m_pPublicBord->SetTips("传世残卷(CTRL+J)");
		m_pWorldMap->SetTips("查看本区域地图(CTRL+Tab)");
		m_pInfoInform->SetTips("查看最新消息(CTRL+O)");
		m_pCourseTable->SetTips("活动日志(CTRL+N)");	
		m_pNpcAutoPath->SetTips("查看附近的NPC和玩家(CTRL+K)");//，并可\n自动移动到指定NPC处，或与\n附近玩家进行快捷的交互");
		m_pNewHandPrompt->SetTips("备忘录");
	}

	if(m_siHandyMapStatus == 0)
		m_pMinMax->SetTips("最大化");
	else if(m_siHandyMapStatus == 1)
		m_pMinMax->SetTips("半透明");
	else if(m_siHandyMapStatus == 2) 
		m_pMinMax->SetTips("最小化");

}

bool CHandyMapWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	
	switch ( dwMsg )
	{
	case MSG_CTRL_FLASH_BTN_WND:
		if (dwData == 1)//每日活动按钮闪烁
		{
			m_pCourseTable->SetFlashTexID(1);
		}
		else if (dwData == 1001)//每日活动按钮停止闪烁
		{
			m_pCourseTable->SetFlashTexID(0);
		}
		else if (dwData == 2)//小地图界面地图按钮闪烁
		{
			if(m_pWorldMap)
			{
				m_pWorldMap->SetFlashTexID(1);
			}
		}
		else if(dwData == 1002)
		{
			if(m_pWorldMap)
			{
				m_pWorldMap->SetFlashTexID(0);
			}
		}

		break;
	case MSG_CTRL_BUTTON_CLICK:
		if(pControl == m_pMinMax)
		{
			m_siHandyMapStatus ++;
			m_siHandyMapStatus %= 3;

			g_pControl->PopupWindow(MSG_CTRL_HANDYMAP_WND,OPER_RECREATE);
			return true;
		}
		else if(pControl == m_pWorldMap)
		{
			if(strnicmp(g_pGameMap->GetMapName(),"maze",4) != 0)
				g_pControl->PopupWindow(MSG_CTRL_MINMAP_WND,OPER_UPDATE);
			else g_pControl->PopupWindow(MSG_CTRL_MAZE_WND,OPER_UPDATE);

			return true;
		}
		else if(pControl == m_pNpcAutoPath)
		{
			g_pControl->PopupWindow(MSG_CTRL_PERSONS_GUIDE_WND,OPER_UPDATE);
			return true;
		}
		else if (pControl == m_pNewHandPrompt)
		{
			Msg(MSG_REMOVE_ARROWTIP_CONTROL,EP_FirstLevelUp_PaoPao);
			if (m_pNewHandPrompt->GetBtnTexID(BTEX_NORMAL) > 0)
			{
				if(m_pNewHandPrompt->GetBtnTexID(BTEX_NORMAL) == MAKELONG(13340,PACKAGE_INTERFACE))
				{
					m_pNewHandPrompt->ReloadTex(13341,13342,13343);

					g_pControl->PopupWindow(MSG_CTRL_MEMOIRE_WND,OPER_RECREATE,2);
					CCtrlWindow * pControl = g_pControl->FindWindowByName("MemoireWnd");
					if(pControl) pControl->Move(g_pControl->GetWidth() - pControl->GetWidth(),0);
				}
				else g_pControl->PopupWindow(MSG_CTRL_MEMOIRE_WND,OPER_UPDATE);
			}

			return true;
		}
		else if(pControl == m_pInfoInform)
		{
			g_pControl->PopupWindow(MSG_CTRL_RECEIVE_MSG_WND,OPER_UPDATE);
			return true;
		}
		else if(pControl == m_pPublicBord)//传世残卷
		{
			g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORY_WND,OPER_UPDATE);
			return true;			
		}
		else if(pControl == m_pCourseTable)
		{
			g_pControl->PopupWindow(MSG_CTRL_ACTIVITY_LOG_WND,OPER_UPDATE);
			//DWORD uID = -5;
			//g_pGameControl->SEND_Exchange_Goin(uID);
			return true;
		}
		else if(pControl == m_pUITypeChange)
		{
			g_pControl->ChangeUi(EUT_NONE,true);
			return true;
		}
		else if(pControl == m_pSystemSetting)
		{
			g_pControl->PopupWindow(MSG_CTRL_SYSCONFIG_WND,OPER_UPDATE);
			return true;			
		}
		else if (pControl == m_pWenwen)
		{
			g_pGameControl->Send_Player_Prompt_Status(30,1);

			StartRainBowSpirt();

			return true;
		}
		break;
	case MSG_CTRL_HANDYMAP_WND:
		if(dwData == 1)//IGW开了就用IGW的电台
		{
			if(m_pInfoInform)
				m_pInfoInform->SetTips("查看最新消息(CTRL+O)");
			return true;
		}
		else if (dwData == 2)
		{
			if(m_pNewHandPrompt)
				m_pNewHandPrompt->ReloadTex(13340,13342,13343);

			if(!g_pGameData->HasPaoPaoStatus(EP_FirstLevelUp_PaoPao))
			{				
				AddArrowTip(EP_FirstLevelUp_PaoPao,m_pNewHandPrompt->GetX()+16,m_pNewHandPrompt->GetY()+16,XAL_TOPLEFT,false,XAL_TOPRIGHT,0,0,0,false);
			}

			return true;
		}

		break;
	default:
		break;
	}
	return  CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CHandyMapWnd::Draw()
{
	if(m_siHandyMapStatus > 0) 
		DrawMiniMap();

	CCtrlWindowX::Draw();
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	
	if (m_siHandyMapStatus != 0 && m_pCourseTable && m_pCourseTable->IsShow())
	{
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,23096,EP_UI);		
		if (pTex)
		{
			g_pGfx->SetRenderMode(RM_ADD2);	
			g_pGfx->DrawTextureNL(m_iScreenX + m_pCourseTable->GetX() + 1,m_iScreenY + m_pCourseTable->GetY() + 1,pTex);
			g_pGfx->SetRenderMode();
		}		
	}
	
	if(HasMessage())
	{
		static int iFrame = 0;

		iFrame++;

		if(iFrame % 30 > 15)
		{
			//if (g_EutUiType == EUT_CLASSIC)//典型
			//{
			//	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,13314,EP_UI);
			//	if(pTex)
			//	{
			//		if(m_siHandyMapStatus != 0)
			//		{
			//			g_pGfx->DrawTextureNL(m_iScreenX + 3,m_iScreenY + 73,pTex);
			//		}
			//	}
			//} 
			//else//时尚
			//{
				LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,21909,EP_UI);
				if(pTex)
				{
					if(m_siHandyMapStatus != 0)
					{
						g_pGfx->DrawTextureNL(m_iScreenX + 74,m_iScreenY + 140,pTex);
					}
				}
			//}
		}		
	}

	DrawOther();

}

void CHandyMapWnd::DrawMiniMap()
{
	int iSelfX,iSelfY;	
	SELF.GetXY(iSelfX,iSelfY);

	if( g_pGfx->IsNoDraw() ) return;
	if(	!g_pGameMap->GetMap() ) return;

	DWORD dwCount = GetTickCount();

	int r = 64;
	int iMinMapX = 0,iMinMapY = 0;
	//if (g_EutUiType == EUT_FASHION)
	{	
		r = 68;//202-134;	
	}

	// 128*128
	float fRateX = 2.0f;
	float fRateY = 1.0f;

	LPTexture pMiniTex = g_pTexMgr->GetTexImm(g_pGameMap->GetMiniTex()/*,EP_UI*/,EP_OBJECT);//这个比较大,而且优先级不高,放到物体同级
	if(pMiniTex)
	{
		fRateX = (float)(pMiniTex->GetWidth()) / g_pGameMap->GetWidth();
		fRateY = (float)(pMiniTex->GetHeight()) / g_pGameMap->GetHeight();
	}

	int selfx = (int)(iSelfX * fRateX);
	int selfy = (int)(iSelfY * fRateY);
	int x = selfx - r;
	int y = selfy - r;

	//if (g_EutUiType == EUT_FASHION)
	{	
	    iMinMapX = 70 - x; //开始坐标
	    iMinMapY = 22 - y;
	}
	//else
	//{
	//    iMinMapX = m_iScreenX + 32 - x; //开始坐标
	//    iMinMapY = m_iScreenY + 22 - y;
	//}



	DWORD dwColorK = 0xFFFFFFFF;
	if(m_siHandyMapStatus == 2)
	{
		dwColorK = 0x55FFFFFF;
	}
		
   

	LPTexture pCutTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,21901,EP_UI);
	LPDIRECT3DSURFACE9 pOldTargetSurface = NULL;
	D3DXMATRIX matOldProjection;

	if (pCutTex /*&& g_EutUiType == EUT_FASHION */&& g_pGfx->GetTempRenderTargetSurf())
	{
		g_pGfx->GetSprite()->End();
		g_pGfx->GetD3DDev()->EndScene();

		g_pGfx->GetD3DDev()->GetTransform(D3DTS_PROJECTION,&matOldProjection);		
		g_pGfx->GetD3DDev()->GetRenderTarget(0,&pOldTargetSurface);	
		g_pGfx->GetD3DDev()->SetRenderTarget(0,g_pGfx->GetTempRenderTargetSurf());

		D3DXMATRIX matProjection;
		D3DXMatrixIdentity(&matProjection);
		g_pGfx->GetD3DDev()->SetTransform(D3DTS_PROJECTION,&matProjection); //set projection matrix 

		g_pGfx->GetD3DDev()->Clear(0,NULL,D3DCLEAR_TARGET,0,0,0);
		g_pGfx->GetD3DDev()->BeginScene();
		g_pGfx->GetSprite()->Begin();

		//D3DVIEWPORT9 vp;
		//D3DSURFACE_DESC desc;

		//g_pGfx->GetTempRenderTargetSurf()->GetDesc(&desc);
		//vp.X = vp.Y = 0;
		//vp.MaxZ = 1.0f;
		//vp.MinZ = 0.0f;
		//vp.Width	= desc.Width;
		//vp.Height	= desc.Height;
	}


	//LPTexture pMiniTex = g_pTexMgr->GetTexImm(g_pGameMap->GetMiniTex());
	if(pMiniTex)
	{
	   //if(m_siHandyMapStatus != 2 && g_EutUiType == EUT_CLASSIC)
 	  //    g_pGfx->DrawFillRect(m_iScreenX + 31,m_iScreenY + 21,m_iWidth - 35,m_iHeight - 48,0xFF000000);

		RECT rc;
		rc.left = x;
		rc.top = y;
		rc.right = x + r*2;
		rc.bottom = y + r*2;

		g_pGfx->DrawTextureFX(iMinMapX,iMinMapY,pMiniTex,dwColorK,&rc);
	}

	g_pGfx->DrawFillRect(iMinMapX + selfx-1,iMinMapY +selfy-1,3,3,-1);

	int BeginX	= iSelfX - 14; //在实际地图上的坐标
	int EndX	= iSelfX + 14;
	int BeginY	= iSelfY - 14;
	int EndY	= iSelfY + 14;

	if(BeginX < 0) BeginX = 0;
	if(BeginY < 0) BeginY = 0;

	if(EndX > g_pGameMap->GetWidth() - 1)
		EndX = g_pGameMap->GetWidth() - 1;

	if(EndY > g_pGameMap->GetHeight() - 1)
		EndY = g_pGameMap->GetHeight() - 1;

	struct npcNode
	{
		npcNode(int x,int y,const char * pName)
		{
			npcx = x;
			npcy = y;
			pNpcName = pName;
		}

		int npcx;
		int npcy;
		const char * pNpcName;
	};

	vector<npcNode> npcVec;

	for(int yy=BeginY; yy<=EndY; yy++ )
	{
		for(int xx=BeginX; xx<=EndX; xx++ )
		{
			// 根据坐标取得一个在此坐标上的人物的链表
			CSimpleCharacterNode* pChar = MapArray.GetSimpleCharacterHead(xx,yy);
			while( pChar )
			{
				bool bFlag = false;
				DWORD dwTempColor = 0x0;
				if(pChar->IsHuman())
				{
					if(pChar->IsGrouped())//组队了的不绘制
					{
						bFlag = false;
					}
					else 
					{
						bFlag = true;
						dwTempColor = 0xFF0000FF;
					}
				}
				else if(pChar->IsNpc())
				{
					bFlag = true;
					dwTempColor = 0xFFFF0000;

					char * pNpcNames[] = {
						"老兵",
						"江湖公告",
						"建材商人",
						"新手向导",
						"首饰店店员",
						"书店老板",
						"铁匠",
						"服装店老板",
						"仓库管理员",
						"炼药师",
						"药店掌柜",
						"屠夫",
						"杂货小贩",
						"新手关怀大使",
						"玄坛使者",
					};

					for(int j = 0;j <sizeof(pNpcNames) / sizeof(char*);j++)
					{
						if(strcmp(pChar->GetName(),pNpcNames[j]) == 0)
						{
							bFlag = false;
							npcVec.push_back(npcNode((int)(xx * fRateX),(int)(yy * fRateY),pChar->GetName()));	
							break;
						}
					}
				}
				else if(pChar->IsMonster())
				{
					int iMonster = pChar->GetLooks().Char.wShape;
					if(!pChar->IsDead() && iMonster != 254 && iMonster != 56 && iMonster != 57)//56 57是门
					{
						bFlag = true;
						if(pChar->IsPet())
							dwTempColor = 0xFF800080;
						else
							dwTempColor = 0xFF00FF00;
					}
				}

				if(bFlag)
				{
					int npcx = (int)(xx * fRateX);
					int npcy = (int)(yy * fRateY);

					g_pGfx->DrawFillRect(iMinMapX + npcx-1,iMinMapY +npcy-1,3,3,dwTempColor);
				}
				pChar =	pChar->m_MapNext;
			}
		}
	}

	static DWORD dwGroupPosTime=0;
	if(g_pGameData->GetTroopMembers() > 0)
	{
		if(dwCount-dwGroupPosTime >3000)
		{
			dwGroupPosTime = dwCount;
			g_pGameControl->SEND_Group_Query();
		}
		int n = g_pGameData->GetGroupPos().size();

		for(int i=0; i<n; ++i)
		{
			int xx= g_pGameData->GetGroupPos()[i]>>16; //高位x
			int yy= g_pGameData->GetGroupPos()[i] & 0x0000FFFF;


			int teamx = (int)(xx * fRateX);
			int teamy = (int)(yy * fRateY);
			if(abs(teamx - iSelfX*fRateX) < r && abs(teamy - iSelfY*fRateY) < r)
			{
				g_pGfx->DrawFillRect(iMinMapX +teamx-1,iMinMapY +teamy-1,3,3,0xFFFFFF00);
			}
		}
	}

	if(dwCount % 400 < 200)
	{
		g_pGfx->DrawRect(iMinMapX+selfx-2,iMinMapY+selfy-2,5,5,0xFF00FF00);

		ListItem& items = g_AutoPickMgr.GetListItem();

		for(ListItem::iterator itr = items.begin(); itr != items.end();itr++)
		{
			CSimpleGood* pItem = g_pGameData->FindSimpleGood(*itr);
			if(pItem == NULL)
				continue;

			int iItemX,iItemY;
			pItem->GetXY(iItemX,iItemY);

			int itemx = (int)(iItemX * fRateX);
			int itemy = (int)(iItemY * fRateY);

			if(abs(itemx - selfx) < r && abs(itemx - selfy) < r) 
			{
				g_pGfx->DrawFillRect(iMinMapX+itemx-2,iMinMapY+itemy-2,4,4,0xFFFFFFFF);
			}
		}

		IDList& bigs = g_AIPromptMgr.GetPromptList();

		for(IDList::iterator itr = bigs.begin();itr != bigs.end();itr++)
		{
			CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(*itr);
			if(pChar == NULL)
				continue;

			int iCharX,iCharY;
			pChar->GetXY(iCharX,iCharY);

			int bossx = (int)(iCharX * fRateX);
			int bossy = (int)(iCharY * fRateY);

			if(abs(bossx - selfx) < r && abs(bossy - selfy) < r) 
			{
				g_pGfx->DrawFillRect(iMinMapX+bossx-2,iMinMapY+bossy-2,4,4,0xFFFF00FF);
			}
		}
	}

	for(int i = 0;i <npcVec.size();i++)
	{
		g_pGfx->DrawFillRect(iMinMapX + npcVec[i].npcx - 1,iMinMapY + npcVec[i].npcy -1,3,3,0xFFFF0000);		
		g_pFont->DrawText(iMinMapX + npcVec[i].npcx - 3,iMinMapY + npcVec[i].npcy - 3,npcVec[i].pNpcName,0xFFFFFF00,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_Center);		
	}

	if (pCutTex/* && g_EutUiType == EUT_FASHION*/ && g_pGfx->GetTempRenderTargetSurf())
	{
		g_pGfx->SetRenderMode(RM_REALPHA);	
        g_pGfx->DrawTextureNL(0,0,pCutTex);


		g_pGfx->GetSprite()->End();
		g_pGfx->GetD3DDev()->EndScene();

		g_pGfx->GetD3DDev()->SetRenderTarget(0,pOldTargetSurface);
		g_pGfx->GetD3DDev()->BeginScene();
		g_pGfx->GetSprite()->Begin();
        g_pGfx->GetD3DDev()->SetTransform(D3DTS_PROJECTION,&matOldProjection); //restore projection matrix

		if(m_siHandyMapStatus == 2)//半透明
		{
			g_pGfx->SetRenderMode(RM_ADD1);
		}
		else
		{
			g_pGfx->SetRenderMode();
		}
		g_pGfx->GetSprite()->Draw(g_pGfx->GetTempRenderTarget(),NULL,NULL,NULL,0,&D3DXVECTOR2((float)m_iScreenX,(float)m_iScreenY),0xffffffff);
		g_pGfx->SetRenderMode();
	}

}

void  CHandyMapWnd::DrawOther()
{
	LPTexture pTexture = NULL;
	int iX = m_iScreenX + 1;
	int iY = m_iScreenY + m_iHeight - 21;

	//if(g_OtherData.IsDay())
	//{
	//	pTexture = g_pTexMgr->GetTex(PACKAGE_INTERFACE,13322);
	//	g_pGfx->DrawTextureNL(iX,iY,pTexture);
	//}
	//else
	//{
	//	pTexture = g_pTexMgr->GetTex(PACKAGE_INTERFACE,13323);
	//	g_pGfx->DrawTextureNL(iX,iY,pTexture);
	//}

	//地图名称
	char levelTemp[100]={0};
	int x,y;
	SELF.GetXY(x,y);
	if( g_pGameMap->GetMapTitle() && strlen(g_pGameMap->GetMapTitle())>0 && x>=0 )
	{
		sprintf(levelTemp,"%s %d:%d",g_pGameMap->GetMapTitle(),x,y);
		//if (g_EutUiType == EUT_CLASSIC)
		//{
		//	g_pFont->DrawText(iX+38,4,levelTemp,0xffffffff, FONT_YAHEI, FONTSIZE_SMALL);		
		//}
		//else
		//{
			if (m_siHandyMapStatus == 0)
			{
				g_pFont->DrawText(iX + 6,4,levelTemp,0xffffffff, FONT_YAHEI, FONTSIZE_SMALL);		
			}
			else
			{
				g_pFont->DrawText(iX + 80,4,levelTemp,0xffffffff, FONT_YAHEI, FONTSIZE_SMALL);
			}
		//}
	}
}

bool CHandyMapWnd::HasMessage()
{
	if(g_OtherData.GetLeaveWordsVector().size() > 0)
	{
		vector<_LeaveWords>::iterator itr = g_OtherData.GetLeaveWordsVector().begin();
		for(;itr != g_OtherData.GetLeaveWordsVector().end();itr++)
		{
			if(!itr->bIsReaded)
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		return false;
	}
}
