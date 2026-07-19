#include "GSceneManager.h"
#include "GMagicManager.h"
#include "GameManager.h"
#include "Global/Interface/LightInterface.h"
#include "Global/Interface/WeatherInterface.h"
#include "GameData/ActionInf.h"
#include "BaseClass/Misc/Input.h"
#include "BaseClass/Control/Control.h"
#include "GameAI/AIPickCfgMgr.h"
#include "GameData/GameGlobal.h"
#include "GameAI/AIConfigMgr.h"
#include "GameAI/AIMgr.h"
#include "Global/ColorModel.h"
#include "GameData/OtherData.h"
#include "GameData/BoothData.h"
#include "GameData/ConfigData.h"
#include "Global/Interface/AudioInterface.h"
#include "GameData/MagicCtrlMgr.h"
#include "GameMap/GameMap.h"
#include "GameMap/MinMap.h"
#include "GameData/DirtyWords.h"

CSceneManager* g_pSceneMgr = NULL;

#define MAX_ENABLE_INDOWNLOAD_FILES   15//如果超出MAX_ENABLE_INDOWNLOAD_FILES,那么绘制以及预读的时候都要缩小范围
#define DOWNLOAD_CACHE_X   3//如果超出MAX_ENABLE_INDOWNLOAD_FILES,那么绘制以及预读的时候都要缩小范围,x方向左右各几格
#define DOWNLOAD_CACHE_Y   3//如果超出MAX_ENABLE_INDOWNLOAD_FILES,那么绘制以及预读的时候都要缩小范围,x方向左右各几格


//场景管理器
CSceneManager::CSceneManager(void)
{
    g_pSceneMgr = this;
    m_iScrWidth = g_pGfx->GetWidth();
    m_iScrHeight = g_pGfx->GetHeight();
    m_iOffX = m_iOffY = 0;

    m_dwFrameCount = 0;
    m_iMouseX = m_iMouseY = 0;
    m_iDrawOffX = m_iDrawOffY = 0;
    m_sLastMap = NULL;
    m_iLastX = m_iLastY = 0;
    m_bNeedRt = true;
    m_dwDemonSmokeTime = 0;

    m_iShakeX = m_iShakeY = 0;

	m_iInDownloadFiles = 0;
	m_dwLastCheckDownLoadListTime = GetTickCount();

    g_pWeather->Create(m_iScrWidth,m_iScrHeight);
    g_pWeather->EnableParticle(PARTICLE_NONE);

    g_pLight->CreateLight();
    g_pLight->SetLightColor(0xFFFFFFFF);
	memset(m_TexPartRenderMode,RM_ALPHA,sizeof(m_TexPartRenderMode));
	m_TexPartRenderMode[DP_WingShadowEff] = RM_ADD2;
	m_TexPartRenderMode[DP_WingLeftEff] = RM_ADD2;
	m_TexPartRenderMode[DP_WingRightEff] = RM_ADD2;
	m_TexPartRenderMode[DP_WeaponEff] = RM_ADD2;
	m_TexPartRenderMode[DP_ShieldEff] = RM_ADD1;			
	m_TexPartRenderMode[DP_ClothEff] = RM_ADD1;	
	m_TexPartRenderMode[DP_FaBaoEFF] = RM_ADD2;	
}

CSceneManager::~CSceneManager(void)
{
}


void CSceneManager::DrawSorterEx(int x0,int y0,int x1,int y1)
{
	SMapTile * sMap = g_pGameMap->GetMap();
	int iWidth = g_pGameMap->GetWidth();
	SMapTile * pTile = NULL;

	for(int iTileY = y0; iTileY <= m_iTileHeight + y1; iTileY++)
	{
		int iAbY = m_iTileStartY+iTileY;

		if(iAbY < 0 || iAbY >= m_iMapTileHeight)
			continue;

		int j = iAbY*m_iMapTileWidth + m_iTileStartX;
		int iDrawY = iTileY*TILE_HEIGHT - m_iDrawOffY;

		//物体的x方向从y0到m_iTileWidth+x1,角色的x方向从-2到m_iTileWidth
		for(int iTileX= x0; iTileX<= m_iTileWidth+x1; iTileX++)
		{
			int iAbX = m_iTileStartX+iTileX;

			if(iAbX < 0 || iAbX >= m_iMapTileWidth)
				continue;

			int i = j + iTileX;

			int iDrawX = iTileX*TILE_WIDTH - m_iDrawOffX;

			pTile = &sMap[i];
			if (!(pTile->wExtraAttr & ATO_GROUP))//没有打组
				ObjectIterator(&sMap[i],iAbX,iAbY,iDrawX,iDrawY);//绘制物体

			if (!pTile->pTile2->vGroup.empty())
			{
				for (DWVECTOR::iterator it = pTile->pTile2->vGroup.begin(); it != pTile->pTile2->vGroup.end(); ++it)
				{
					DWORD dwXY = *it;
					WORD wX = LOWORD(dwXY);
					WORD wY = HIWORD(dwXY);
					int iDX = iDrawX + (wX - iAbX) * TILE_WIDTH;
					int iDY = iDrawY + (wY - iAbY) * TILE_HEIGHT;

					ObjectIterator(&sMap[wY * iWidth + wX],iAbX,iAbY,iDX,iDY);//绘制物体
				}
			}

			if (iTileY >= 0 && iTileY <= m_iTileHeight && iTileX >= 0 && iTileX <= m_iTileWidth)//屏幕外的物品及地表魔法不绘制
			{
				OvergroundIterator(&sMap[i],iAbX,iAbY,iDrawX,iDrawY); //绘制物品,地表魔法
			}
		}


		//iTileY < 0 || iTileY > m_iTileHeight时只绘制物体,不绘制角色,因为有些物体很大,
		if (iTileY < 0 || iTileY > m_iTileHeight)
		{
			continue;
		}

		//绘制角色,角色的x方向从-2到m_iTileWidth
		for(int iTileX= -2; iTileX <= m_iTileWidth; iTileX++)
		{
			int iAbX = m_iTileStartX+iTileX;

			if(iAbX < 0 || iAbX >= m_iMapTileWidth)
			{
				continue;
			}

			int i = j + iTileX;
			int iDrawX = iTileX*TILE_WIDTH - m_iDrawOffX;


			//绘制人物
			if(iAbY == m_iPlayerY && iAbX == m_iPlayerX)
			{
				WORD wAction = SELF.GetAction().wAction;
				if(wAction == ACTION_LFMAGICFGH || wAction == ACTION_LFMAGICRAB)
					m_vDrawCharTop.push_back(stDrawChar(m_iScrX-m_iShakeX,m_iScrY-m_iShakeY,NULL));
				else
					DrawSimpleCharacter(m_iScrX-m_iShakeX,m_iScrY-m_iShakeY,NULL,FALSE);
			}

			// 显示其它角色和怪物,取得此坐标上的人物链表
			CSimpleCharacterNode* pChar = MapArray.GetSimpleCharacterHead(iAbX,iAbY);
			if(pChar)
			{
				WORD wAction = pChar->GetAction().wAction;
				if(wAction == ACTION_LFMAGICFGH || wAction == ACTION_LFMAGICRAB)
					m_vDrawCharTop.push_back(stDrawChar(iDrawX,iDrawY,pChar));
				else
					DrawSimpleCharacterNode(iDrawX,iDrawY,pChar,FALSE);
			}

		}

		//显示刀光
		if(g_pMagicMgr)
		{
			for(int iTileX= -2; iTileX <= m_iTileWidth; iTileX++)
			{
				int iAbX = m_iTileStartX+iTileX;

				if(iAbX < 0 || iAbX >= m_iMapTileWidth)
					continue;

				int i = j + iTileX;

				int iDrawX = iTileX*TILE_WIDTH - m_iDrawOffX;

				//绘制人物
				if(iAbY == m_iPlayerY && iAbX == m_iPlayerX)
				{
					g_pMagicMgr->DrawSwearLight(m_iScrX-m_iShakeX,m_iScrY-m_iShakeY,NULL);
				}

				CSimpleCharacterNode* pChar = MapArray.GetSimpleCharacterHead(iAbX,iAbY);
				if(pChar)
				{
					g_pMagicMgr->DrawSwearLight(iDrawX,iDrawY,pChar);
				}
			}
		}
	}
}


void CSceneManager::DrawSorterTop()
{
	for(int ii = 0;ii < m_vDrawCharTop.size(); ii++)
	{
		stDrawChar& charArray = m_vDrawCharTop[ii];
		DrawSimpleCharacter(charArray.sx,charArray.sy,charArray.pChar,FALSE);
	}

	m_vDrawCharTop.clear();
}


//标绘暂存材质链表
void CSceneManager::DrawTexShow(tex_show_t  *head)
{
    LPTexture	pTex = NULL;
    DWORD		iTmp  = 0;
    tex_show_t* tex  = head;

    while (tex)	
    {
        if (tex->iType < EST_GENERAL_TEXT)
        {
            if( pTex = g_pTexMgr->GetTex((WORD)(PACKAGE_magic1+tex->wPackage),tex->wTexID,EP_MAGIC))
            {
                g_pGfx->SetRenderMode((RenderMode)tex->wAdd);
                switch(tex->iType)
                {
                case EST_CLIP:
                    {
                        RECT rcClip = {0,0,tex->dx,tex->dy};
                        g_pGfx->DrawTextureFX(tex->x,tex->y,pTex,tex->dwColor,&rcClip);
                    }
                    break;
                case EST_SCALE:
                    g_pGfx->DrawTextureFX(tex->x,tex->y,pTex,tex->dwColor,NULL,&POS(tex->fScaleX,tex->fScaleY),tex->fRotX,tex->fRotY);
                    break;
                case EST_SCALE_CENTER:
                    {
                        int dw = (int)(0.5*(1-tex->fScaleX)*pTex->GetWidth());
                        int dh = (int)(0.5*(1-tex->fScaleY)*pTex->GetHeight());
                        g_pGfx->DrawTextureFX(tex->x + dw,tex->y + dh,pTex,tex->dwColor,NULL,&POS(tex->fScaleX,tex->fScaleY),tex->fRotX,tex->fRotY);
                    }
                    break;
                default:
                    {
                        //g_pGfx->DrawTextureFX(tex->x,tex->y,pTex,tex->dwColor,NULL,NULL,tex->fRotX,tex->fRotY);
						g_pGfx->DrawTextureNL(tex->x,tex->y,pTex,tex->dwColor);
                    }
                }
                g_pGfx->SetRenderMode();
            }
        }
        else
        {
            switch(tex->iType)
            {
            case EST_GENERAL_TEXT:
				g_pFont->DrawText(tex->x,tex->y,tex->text,tex->dwColor,tex->byFontType,tex->byFontSize);
                break;
            case EST_DIGITAL_TEXT:
                {
                    static char g_digital_text[128] = {0};
                    sprintf(g_digital_text,"%d",tex->wTexID);
                    g_pFont->DrawText(tex->x,tex->y,g_digital_text,COLOR_DEFAULT);
                }
                break;
            }
        }
        tex = tex->next;
    }
}

void CSceneManager::DrawZhongLeiShadow(int x,int y)
{
	int iDrawNum = 0;
	if(m_DrawInf.iFrame == 1 || m_DrawInf.iFrame == 7)
		iDrawNum = 1;
	else if(m_DrawInf.iFrame > 2 && m_DrawInf.iFrame < 7)
		iDrawNum = 2;
	if(iDrawNum > 0)
	{
		int iDevX = 0,iDevY=0;
		GetZhongLeiDev(m_DrawInf.iDir,iDevX,iDevY);

		DWORD dwColor = m_DrawInf.dwColor;
		//绘制残影
		if(iDrawNum >= 1)
		{
			m_DrawInf.dwColor = ColorMultiply(dwColor,0x88FFFFFF);
			DrawLooks(x + iDevX,y + iDevY,&m_DrawInf);
		}

		if(iDrawNum >= 2)
		{
			m_DrawInf.dwColor = ColorMultiply(dwColor,0x44FFFFFF);
			DrawLooks(x + iDevX*2,y + iDevY*2,&m_DrawInf);
		}

		m_DrawInf.dwColor = dwColor;
	}
}

void CSceneManager::GetZhongLeiDev(int iDir,int& iDevX,int& iDevY)
{
	int iaDevX[8],iaDevY[8];
	int iDevTempX = 50,iDevTempY = 25;
	iaDevX[0] = 0;
	iaDevY[0] = 1;
	iaDevX[1] = -1;
	iaDevY[1] = 1;
	iaDevX[2] = -1;
	iaDevY[2] = 0;
	iaDevX[3] = -1;
	iaDevY[3] = -1;
	iaDevX[4] = 0;
	iaDevY[4] = -1;
	iaDevX[5] = 1;
	iaDevY[5] = -1;
	iaDevX[6] = 1;
	iaDevY[6] = 0;
	iaDevX[7] = 1;
	iaDevY[7] = 1;
	if(iDir >= 0 && iDir <= 7)
	{
		iDevX = iaDevX[iDir] * iDevTempX;
		iDevY = iaDevY[iDir] * iDevTempY;
	}
	else 
	{
		iDevX = iDevY = 0;
	}
}

BOOL CSceneManager::DrawLooks(int iX,int iY,DrawCharInf *pDrawInf)
{
	if(g_pGfx->IsNoDraw() || pDrawInf == NULL || pDrawInf->pLooks == NULL)
		return FALSE;

	int			x,y;
	int			isx = 0,isy = 0;
	int			idx			= 0;
	BOOL		bRet		= FALSE;
	DWORD		dwOption	= 0;
	DWORD		dwTemp		= 0;
	DWORD		dwColor		= pDrawInf->dwColor;
	DWORD		dwFlag		= pDrawInf->dwFlag;
	float		fScale		= pDrawInf->fScale;
	float		fRotY		= 0.0f;
	int			iDir		= pDrawInf->iDir;

	if(fScale < 0.2f)
		fScale = 1.0f;	

	x = iX - pDrawInf->iScaleDrawOffX;
	y = iY - pDrawInf->iScaleDrawOffY;

	bool bScale = fabs(fScale - 1.0f) > 1e-3;
	if (bScale)
	{
		isx = (int)(x - 130.0f * (fScale - 1.0f));
		isy = (int)(y - 150.0f * (fScale - 1.0f));
	}

	switch(pDrawInf->pLooks->Player.byType)
	{
	case CHARACTER_HUMAN:		// 玩家
		{
			DWORD dwWingColor = 0xFFFFFFFF;
			DWORD dwWingEffColor = 0xFFFFFFFF;
			DWORD dwFaBaoEffColor = 0xFFFFFFFF;
			if (pDrawInf->byWing > 100)
			{
				switch (pDrawInf->byWing)
				{
				case 102:
					dwWingColor = 0xFF54ADE7;
					break;
				case 103:
					dwWingColor = 0xFF8157ff;
					break;
				case 104:
					dwWingColor = 0xFF382D33;
					break;
				case 105:
					dwWingColor = 0xFFA01010;
					break;
				case 106:
					dwWingColor = 0xFF000000;
					break;
				default:
					break;

				}

				pDrawInf->byWing = 101;

				if (pDrawInf->byWingEff == 3)
				{
					dwWingEffColor = 0xFF000000;
				}
			}

			bool bSelf = ((dwFlag & DRAWINFO_FLAG_SELF) == 0)?false:true;
			int iDir = pDrawInf->iDir;
			PlayerTex TEX = g_ActionInf.GetPlayerTex(pDrawInf);
			SDrawOrder_Dir_Frame * pOrder = g_ActionInf.Get_Draw_Order_Dir_Frame(pDrawInf->iDrawAction,pDrawInf->iDir,pDrawInf->iFrame);
			if(!pOrder)
				return false;


			DWORD *pTexID = (DWORD*)(&TEX);
			DWORD dwTexID = 0;
			LPTexture pTex = NULL;	
			BYTE byDrawPart = 0;

			for(int i = 0;i < DP_DrawPartEffectNum;i++)
			{
				byDrawPart = pOrder->byOrder[i];
				if(byDrawPart >= DP_DrawPartEffectNum)
				{
					break;
				}

				dwTexID = pTexID[byDrawPart];
				if(dwTexID == 0)
				{
					continue;
				}
				pTex = NULL;

				if(bSelf)
					pTex = g_pTexMgr->GetTexImm(dwTexID,/*EP_MOST_HIGH*/EP_UI);
				else
					pTex = g_pTexMgr->GetTexFromID(dwTexID,EP_CHARACTER);

				//没有读到用上一次
				if(!pTex)
				{
					if (bSelf && g_pStreamMgr->IsNeedDownloadFileFromServer())
					{
						//g_pTexMgr->PreDownLoadTex(HIWORD(dwTexID),LOWORD(dwTexID) - pDrawInf->iFrame,LOWORD(dwTexID) - pDrawInf->iFrame + pDrawInf->iActionFrames);
						g_pTexMgr->PreLoadImm(HIWORD(dwTexID),LOWORD(dwTexID) - pDrawInf->iFrame,LOWORD(dwTexID) - pDrawInf->iFrame + pDrawInf->iActionFrames,/*EP_MOST_HIGH*/EP_UI);
					}
					else //if(!bSelf)
					{
						g_pTexMgr->PreLoad(dwTexID - pDrawInf->iFrame,dwTexID - pDrawInf->iFrame + pDrawInf->iActionFrames,EP_CHARACTER);
					}

					if((byDrawPart >= DP_WingShadow && byDrawPart <= DP_WingRightEff) || byDrawPart == DP_Pattern || byDrawPart == DP_MonsterHead || byDrawPart == DP_MonsterHeadArm || byDrawPart == DP_WeaponEff || byDrawPart == DP_ShieldEff || pDrawInf->iAction == ACTION_DEATH)
					{
						//不用上次的缓存的部分
					}
					else
					{
						dwTexID = ((DWORD*)pDrawInf->pLast)[byDrawPart];
						pTexID[byDrawPart] = dwTexID;

						if(bSelf)
							pTex = g_pTexMgr->GetTexImm(dwTexID,EP_UI);
						else
							pTex = g_pTexMgr->GetTexFromID(dwTexID,EP_CHARACTER);
					}

				}


				if(pTex)
				{
					dwColor	= pDrawInf->dwColor;

					if (byDrawPart == DP_WingShadow || byDrawPart == DP_WingLeft || byDrawPart == DP_WingRight)
					{
						dwColor = (dwWingColor & (dwColor | 0x00FFFFFF));
					}
					else if (byDrawPart == DP_WingShadowEff || byDrawPart == DP_WingLeftEff || byDrawPart == DP_WingRightEff)
					{
						dwColor = (dwWingEffColor & (dwColor | 0x00FFFFFF));
					}
					else if (byDrawPart == DP_FaBaoEFF /*|| byDrawPart == DP_FaBaoLeftEFF || byDrawPart == DP_FaBaoRightEFF*/)
					{
						//根据法宝等级决定:0~5 白 6~8 蓝 9 金

						if (pDrawInf->pFaBao->iFaBaoLevel == 9)
						{
							dwFaBaoEffColor = 0xffffff00;
						}
						else if (pDrawInf->pFaBao->iFaBaoLevel >= 6)
						{
							dwFaBaoEffColor = 0xff00ffff;
						}
						else
						{
							dwFaBaoEffColor = 0xffffffff;
						}
						dwColor = (dwFaBaoEffColor & (dwColor | 0x00FFFFFF));

					}


					// 更新鼠标点中情况
					if(pDrawInf->bNeedRt && m_bNeedRt)
					{
						if(byDrawPart == DP_Cloth)
							pDrawInf->iMouseIn = pTex->IsPointInTex(m_iMouseX - x,m_iMouseY - y,fScale,fScale);

						if(pDrawInf->iMouseIn != 2 && byDrawPart == DP_Pattern)
						{
							int iMouseIn = pTex->IsPointInTex(m_iMouseX - x,m_iMouseY - y,fScale,fScale);
							if(iMouseIn > pDrawInf->iMouseIn)
								pDrawInf->iMouseIn = iMouseIn;
						}
					}

					RenderMode eRenderMode = m_TexPartRenderMode[byDrawPart];
					if(eRenderMode != RM_ALPHA)
					{
						//add 模式下的部件在渐隐或drawalpha时不能通过alpha通道实现半透效果,只能通过颜色来实现
						if (((dwColor & 0xFF000000) != 0xFF000000) && (eRenderMode == RM_ADD1 || eRenderMode == RM_ADD2 || eRenderMode == RM_ADD3 || eRenderMode == RM_RGB) )
						{
							BYTE byAlpha = (BYTE)((dwColor & 0xFF000000) >> 24);
							BYTE byR = (BYTE)(((dwColor & 0x00FF0000) >> 16) * byAlpha / 256);
							BYTE byG = (BYTE)(((dwColor & 0x0000FF00) >> 8) * byAlpha / 256);
							BYTE byB = (BYTE)( (dwColor & 0x000000FF) * byAlpha / 256);
							dwColor = ((DWORD)byAlpha << 24) | ((DWORD)byR << 16) | ((DWORD)byG << 8) | (DWORD)byB;						
						}

						g_pGfx->SetRenderMode(eRenderMode);
					}

					if(byDrawPart == DP_Pattern)
					{
						DWORD dwBodyColor = g_OtherData.GetBodyColor(pDrawInf->pLooks->Player.byColor,pDrawInf->iSex);

						if( (dwColor&0x00FFFFFF) != 0x00FFFFFF )
							dwBodyColor = ColorMultiply(dwBodyColor,dwColor);
						else
							dwBodyColor = dwBodyColor&dwColor;

						if (bScale)
							g_pGfx->DrawTextureFX(isx,isy,pTex,dwBodyColor,NULL,&POS(fScale,fScale),0.0f,fRotY);
						else
							g_pGfx->DrawTextureFX(x,y,pTex,dwBodyColor);
					}
					else if(byDrawPart == DP_Hair)
					{
						int iBody  = pDrawInf->wBody;
						DWORD dwHairColor = dwColor;
						//不是黄金甲
						if(iBody != 19 && iBody != 20)
						{
							dwHairColor = g_OtherData.GetHairColor(pDrawInf->pLooks->Player.byHairColor);
							dwHairColor = ColorMultiply(dwHairColor,dwColor);
						}

						if (bScale)
							g_pGfx->DrawTextureFX(isx,isy,pTex,dwHairColor,NULL,&POS(fScale,fScale),0.0f,fRotY);
						else
							g_pGfx->DrawTextureNL(x,y,pTex,dwHairColor);

					}
					else if(byDrawPart == DP_MonsterArm)
					{
						if(pDrawInf->pLooks->Player.wHorse >= 301 && pDrawInf->pLooks->Player.wHorse <= 302)
						{
							g_pGfx->SetRenderMode(RM_ADD1);

							if (bScale)
							{
								if(pDrawInf->pLooks->Player.wHorse == 302)
									g_pGfx->DrawTextureFX(isx,isy,pTex,0xffb000ff,NULL,&POS(fScale,fScale),0.0f,fRotY);
								else
									g_pGfx->DrawTextureFX(isx,isy,pTex,dwColor,NULL,&POS(fScale,fScale),0.0f,fRotY);
							}
							else
							{
								if(pDrawInf->pLooks->Player.wHorse == 302)
									g_pGfx->DrawTextureNL(x,y,pTex,0xffb000ff);
								else
									g_pGfx->DrawTextureNL(x,y,pTex,dwColor);
							}
							
							g_pGfx->SetRenderMode();
						}
						else
						{
							g_pGfx->DrawTextureNL(x,y,pTex,dwColor);
						}
					}
					else if(byDrawPart == DP_FaBao || byDrawPart == DP_FaBaoEFF )
					{
						if (!bScale)
						{
							if (pDrawInf->pLooks->Player.wHorse)
							{
								if(IsLeopard(pDrawInf->pLooks->Player.wHorse))
								{
									g_pGfx->DrawTextureNL(x,y - 15,pTex,dwColor);
								}
								else
								{
									g_pGfx->DrawTextureNL(x,y - 55,pTex,dwColor);
								}
							}
							else
							{
								g_pGfx->DrawTextureNL(x,y - 15,pTex,dwColor);
							}
						}
					}
					else if(byDrawPart == DP_MonsterLeft || byDrawPart == DP_MonsterRight )
					{
						DWORD dwArmTexID = pTexID[byDrawPart - DP_MonsterLeft + DP_MonsterLeftArm];
						
						LPTexture pArmTex = NULL;	

						if(dwArmTexID != 0)
						{
							if(bSelf)
								pArmTex = g_pTexMgr->GetTexImm(dwArmTexID,/*EP_MOST_HIGH*/EP_UI);
							else
								pArmTex = g_pTexMgr->GetTexFromID(dwArmTexID,EP_CHARACTER);
						}
						
						if (bScale)
						{
							g_pGfx->DrawTextureFX(isx,isy,pTex,dwColor,NULL,&POS(fScale,fScale),0.0f,fRotY);

							if (pArmTex)
							{
								g_pGfx->DrawTextureFX(isx,isy,pArmTex,dwColor,NULL,&POS(fScale,fScale),0.0f,fRotY);
							}
						}
						else
						{
							g_pGfx->DrawTextureNL(x,y,pTex,dwColor);

							if (pArmTex)
							{
								g_pGfx->DrawTextureNL(x,y,pArmTex,dwColor);
							}
						}
					}
					else 
					{
						if (bScale)
							g_pGfx->DrawTextureFX(isx,isy,pTex,dwColor,NULL,&POS(fScale,fScale),0.0f,fRotY);
						else
							g_pGfx->DrawTextureNL(x,y,pTex,dwColor);
					}

					if(eRenderMode != RM_ALPHA)
						g_pGfx->SetRenderMode(RM_ALPHA);
				}

			}

			// 保存本次的信息
			memcpy(pDrawInf->pLast,&TEX,sizeof(PlayerTex));
			bRet = TRUE;
			break;
		}
	case CHARACTER_MONSTER:		// 怪物
		{
			MonsterTex	MTEX;
			LPTexture	pMonster		= NULL;
			LPTexture   pArmTex         = NULL;
			LPTexture   pLeftTex        = NULL;
			LPTexture   pRightTex       = NULL;
			LPTexture   pMonsterEff     = NULL;

			MTEX = g_ActionInf.GetMonsterTex(pDrawInf);

			//这里针对法宝的绘制顺序,由于怪物动作,帧数都与人物不同
			//所以这里用STAND动作,第一帧,获得法宝顺序即可
			SDrawOrder_Dir_Frame * pOrder = g_ActionInf.Get_Draw_Order_Dir_Frame(ACTION_STAND,pDrawInf->iDir,1);
			if(!pOrder)
				return false;

			DWORD dwTexID = 0;
			LPTexture pTex = NULL;	
			BYTE byDrawPart = 0;

			for(int i = 0;i < DP_DrawPartEffectNum;i++)
			{
				byDrawPart = pOrder->byOrder[i];
				if(byDrawPart >= DP_DrawPartEffectNum)
				{
					break;
				}

				if (byDrawPart == DP_Cloth)
				{
					if(MTEX.dwShapeTex != 0)
					{
						pMonster = g_pTexMgr->GetTexFromID(MTEX.dwShapeTex,EP_MONSTER);

						if(MTEX.dwArmTex != 0)
						{
							pArmTex = g_pTexMgr->GetTexFromID(MTEX.dwArmTex,EP_MONSTER);
						}				

						// 使用上次图片
						if(!pMonster || (!pArmTex && MTEX.dwArmTex != 0))
						{
							g_ActionInf.CacheMonsterLooks(MTEX,pDrawInf->iFrame,pDrawInf->iActionFrames,EP_MONSTER);
							MTEX.dwShapeTex	= pDrawInf->pLast->dwMShapeTex;					
							pMonster		= g_pTexMgr->GetTexFromID(MTEX.dwShapeTex,EP_MONSTER);

							if(MTEX.dwArmTex != 0)
							{
								MTEX.dwArmTex = pDrawInf->pLast->dwMArmTex;
								pArmTex = g_pTexMgr->GetTexFromID(MTEX.dwArmTex,EP_MONSTER);
							}					
						}

					}   

					if(MTEX.dwMonsterEff != 0)
					{
						pMonsterEff = g_pTexMgr->GetTexFromID(MTEX.dwMonsterEff,EP_MONSTER);
					}


					// 更新鼠标点中情况
					if(pDrawInf->bNeedRt && g_pInput && pMonster && m_bNeedRt)
					{
						if(pDrawInf->pLooks->Char.wShape != 5)//通灵塔不能选中
						{
							pDrawInf->iMouseIn = pMonster->IsPointInTex(m_iMouseX - x,m_iMouseY - y,fScale,fScale);
						}
					}

					if(pMonster) // 绘制怪物
					{
						if(pDrawInf->pLooks->Char.wShape >= 422 && pDrawInf->pLooks->Char.wShape <= 433 )
						{
							g_pGfx->SetRenderMode(RM_ADD2);

							g_pGfx->DrawTextureFX(x,y,pMonster,pDrawInf->dwColor,NULL,&POS(fScale,fScale),0.0f,fRotY);

							g_pGfx->SetRenderMode();
						}
						else
						{
							g_pGfx->DrawTextureFX(x,y,pMonster,pDrawInf->dwColor,NULL,&POS(fScale,fScale),0.0f,fRotY);
						}
					}

					if(pArmTex)
					{
						if(pDrawInf->pLooks->Char.wShape >= 301 && pDrawInf->pLooks->Char.wShape <= 303 && pDrawInf->iSex > 1)
						{
							g_pGfx->SetRenderMode(RM_ADD1);

							if(pDrawInf->pLooks->Char.wShape == 302)
								g_pGfx->DrawTextureFX(x,y,pArmTex,0xffb000ff,NULL,&POS(fScale,fScale),0.0f,fRotY);
							else
								g_pGfx->DrawTextureFX(x,y,pArmTex,pDrawInf->dwColor,NULL,&POS(fScale,fScale),0.0f,fRotY);

							g_pGfx->SetRenderMode();
						}
						else
						{
							g_pGfx->DrawTextureFX(x,y,pArmTex,pDrawInf->dwColor,NULL,&POS(fScale,fScale),0.0f,fRotY);
						}
					}

					if(pMonsterEff)
					{
						if(pDrawInf->pLooks->Char.wShape == 399)
						{
							//RM_ALPHA
						}
						else if(pDrawInf->pLooks->Char.wShape == 306 || pDrawInf->pLooks->Char.wShape == 314 || pDrawInf->pLooks->Char.wShape == 315)
						{
							g_pGfx->SetRenderMode(RM_ADD1);
						}
						else
						{
							g_pGfx->SetRenderMode(RM_ADD2);
						}

						g_pGfx->DrawTextureFX(x,y,pMonsterEff,pDrawInf->dwColor,NULL,&POS(fScale,fScale),0.0f,fRotY);
						g_pGfx->SetRenderMode();
					}

					// 保存信息
					pDrawInf->pLast->dwMShapeTex	= MTEX.dwShapeTex;
					pDrawInf->pLast->dwMArmTex       = MTEX.dwArmTex;	

					bRet = TRUE;
				}
				else if(byDrawPart == DP_FaBao || byDrawPart == DP_FaBaoEFF)
				{

					if (byDrawPart == DP_FaBao)
					{
						dwTexID = MTEX.dwFaBao;
					}
					else 
					{
						dwTexID = MTEX.dwFaBaoEff;
					}

					if(dwTexID == 0)
					{
						continue;
					}

					pTex = NULL;
					pTex = g_pTexMgr->GetTexFromID(dwTexID,EP_MONSTER);

					RenderMode eRenderMode = m_TexPartRenderMode[byDrawPart];
					if(eRenderMode != RM_ALPHA)
					{
						//add 模式下的部件在渐隐或drawalpha时不能通过alpha通道实现半透效果,只能通过颜色来实现
						if (((dwColor & 0xFF000000) != 0xFF000000) && (eRenderMode == RM_ADD1 || eRenderMode == RM_ADD2 || eRenderMode == RM_ADD3 || eRenderMode == RM_RGB) )
						{
							BYTE byAlpha = (BYTE)((dwColor & 0xFF000000) >> 24);
							BYTE byR = (BYTE)(((dwColor & 0x00FF0000) >> 16) * byAlpha / 256);
							BYTE byG = (BYTE)(((dwColor & 0x0000FF00) >> 8) * byAlpha / 256);
							BYTE byB = (BYTE)( (dwColor & 0x000000FF) * byAlpha / 256);
							dwColor = ((DWORD)byAlpha << 24) | ((DWORD)byR << 16) | ((DWORD)byG << 8) | (DWORD)byB;						
						}

						g_pGfx->SetRenderMode(eRenderMode);

					}

					if (pTex)
					{
						g_pGfx->DrawTextureNL(x,y - 55,pTex,dwColor);
					}


					if(eRenderMode != RM_ALPHA)
						g_pGfx->SetRenderMode(RM_ALPHA);
				}
			}
		}
		break;
	case CHARACTER_NPC:			// NPC
		{
			DWORD		dwNpcTex	= 0;
			LPTexture	pNpc		= NULL;			

			dwNpcTex	= g_ActionInf.GetNPCTex(pDrawInf);

			if(dwNpcTex != 0)
			{
				pNpc		= g_pTexMgr->GetTexFromID(dwNpcTex,EP_NPC);

				if(!pNpc)
				{
					dwNpcTex	= pDrawInf->pLast->dwClothTex;
					pNpc		= g_pTexMgr->GetTexFromID(dwNpcTex,EP_NPC);
				}
			}

			if(!pNpc)
				break;

			// 更新鼠标点中情况
			if(pDrawInf->bNeedRt && g_pInput && pNpc && m_bNeedRt)
			{
				int mx = (fRotY == 180.0f)? (x - m_iMouseX) : (m_iMouseX - x);
				pDrawInf->iMouseIn = pNpc->IsPointInTex(mx,m_iMouseY - y,fScale,fScale);
			}

			g_pGfx->DrawTextureFX(iX,iY,pNpc,pDrawInf->dwColor,NULL,&POS(fScale,fScale),0.0f,fRotY);

			// 保存信息
			pDrawInf->pLast->dwClothTex	= dwNpcTex;
			bRet = TRUE;
		}	
		break;
	}
	return bRet;
}

//在原来的基础上再画一层
void CSceneManager::DrawAlpha()
{
    CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(g_pControl->GetMouseOnID());
    if(pChar)
    {
        int cx,cy;
        int cox,coy;
        pChar->GetXY(cx,cy);
        pChar->GetOffset(cox,coy);
		int iDrawOffX = 0,iDrawOffY = 0;
		pChar->GetDrawOffset(iDrawOffX,iDrawOffY);

        int iX = (cx - m_iTileStartX)*TILE_WIDTH - m_iDrawOffX;
        int iY = (cy - m_iTileStartY)*TILE_HEIGHT - m_iDrawOffY;

        g_pGfx->SetRenderMode(RM_ALPHAADD);

        switch(pChar->GetRaceType())
        {
        case CHARACTER_HUMAN:	// 表示为玩家
            DrawHuman(iX+cox+iDrawOffX,iY+coy+iDrawOffY,pChar,FALSE);
            break;
        case CHARACTER_NPC:		// 表示为NPC
            DrawNpc(iX+cox+iDrawOffX,iY+coy+iDrawOffY,pChar,FALSE);
            break;
        default:				// 表示为怪物
            DrawMonster(iX+cox+iDrawOffX+iDrawOffY,iY+coy,pChar,FALSE);
            break;
        }
        g_pGfx->SetRenderMode();
    }

    if(g_Config.GetSelfAlpha()) //绘制主角透明
    {
		int iDrawOffX = 0,iDrawOffY = 0;
		SELF.GetDrawOffset(iDrawOffX,iDrawOffY);
        DrawHuman(m_iScrX-m_iShakeX+iDrawOffX,m_iScrY-m_iShakeY+iDrawOffY,NULL,FALSE);
    }
}

void CSceneManager::DrawSimpleCharacter(int iX,int iY,CSimpleCharacter* pChar,BOOL bCorpus)
{
	if(pChar == NULL)
	{
		SAction& ActionNow = SELF.GetAction();

		BOOL bDeath = (ActionNow.IsDeath() && ActionNow.IsEnd(m_dwFrameCount));

		if(bCorpus == bDeath)
		{	
			int iDrawOffX = 0,iDrawOffY = 0;
			SELF.GetDrawOffset(iDrawOffX,iDrawOffY);
			DrawHuman(iX+iDrawOffX,iY+iDrawOffY,NULL);
		}
		return;
	}

	SAction& ActionNow = pChar->GetAction();
	BOOL bDeath = (ActionNow.IsDeath() && ActionNow.IsEnd(m_dwFrameCount));
	int cox,coy;
	pChar->GetOffset(cox,coy);
	int iCharX,iCharY;
	pChar->GetXY(iCharX,iCharY);
	int iDrawOffX = 0,iDrawOffY = 0;
	pChar->GetDrawOffset(iDrawOffX,iDrawOffY);


	if(abs(iCharY-m_iPlayerY) > 12 || abs(iCharX-m_iPlayerX) > 12)
	{
		return;
	}

	//不能根据人物在地图中的X,Y来画，而应该根据iCharX,iCharY,因为有的时候动作还没做完可能其它的协议就会把该人物在地图中的X,Y更改了
	//这就会导致人物颤动，比如僵尸喷血收到MSG_Magic_Monster2_Attack协议后就会有此问题
	iX = (iCharX - m_iTileStartX)*TILE_WIDTH - m_iDrawOffX;
	iY = (iCharY - m_iTileStartY)*TILE_HEIGHT - m_iDrawOffY;


	if(bCorpus == bDeath)
	{
		switch(pChar->GetRaceType())
		{
		case CHARACTER_HUMAN:	// 表示为玩家
			DrawHuman(iX+cox+iDrawOffX,iY+coy+iDrawOffY,pChar);
			break;
		case CHARACTER_NPC:		// 表示为NPC
			DrawNpc(iX+cox+iDrawOffX,iY+coy+iDrawOffY,pChar);
			break;
		default:				// 表示为怪物
			DrawMonster(iX+cox+iDrawOffX,iY+coy+iDrawOffY,pChar);
			break;
		}
	}
}

void CSceneManager::DrawSimpleCharacterNode(int iX,int iY,CSimpleCharacterNode* pChar,BOOL bCorpus)
{
	if (!pChar)
	{
		return DrawSimpleCharacter(iX,iY,pChar,bCorpus);
	}

    while(pChar)
    {
        if(g_AICfgMgr.IsBury() && bCorpus && pChar->IsMonster() 
			&& pChar->GetRaceNo() != 56 && pChar->GetRaceNo() != 57 
			&& pChar->GetRaceNo() != 209 && pChar->GetRaceNo() != 208) //掩埋尸体
        {
            //pChar = pChar->m_MapNext;
            //continue;
        }
		else
		{
			DrawSimpleCharacter(iX,iY,pChar,bCorpus);
		}

        pChar = pChar->m_MapNext;
    }
}

void CSceneManager::DrawHuman(int iX,int iY,CSimpleCharacter* pChar,BOOL bNotAlpha)
{
    if(!g_pGameData) 
        return;

    // 临时变量
    int				x,y;

    // 角色信息
    int				iSelfX,iSelfY;			// 主角所在格子坐标
    int				iOtherX,iOtherY;		// 当前角色所在格子坐标
    UINT			uID;					// ID
    WORD			wStatus;				// 状态
    WORD			byExtraStatus;			// 状态扩展
    SAction			*pAction;				// 当前动作
    bool            bBoothState;            // 是否在摆摊
	CSimpleCharacter *pCharAttr = NULL;

    // 显示相关
    tex_show_t *	pTexFront = NULL;
    tex_show_t *	pTexBack = NULL;

    // 声音
    UINT			lSoundID = 0;
    bool			bSelf	 = 0;
	m_DrawInf.dwFlag		 = 0;

    memset(&m_DrawInf,0,sizeof(m_DrawInf));
    iSelfX = iSelfY = iOtherX = iOtherY = 0;
    memset(&m_DrawBooth,0,sizeof(m_DrawBooth));

    // 初始化变量
    if(pChar)
    {
		pCharAttr = pChar;
        if(pChar->GetReserveData(pubDisableDraw) == 1)
            return;
        if(pChar->IsBianShen())
        {
            if(bNotAlpha)
                DrawMonster(iX,iY,pChar);

            return;
        }

        uID					= pChar->GetID();

// 		if (g_TrusteeshipData.GetMicroControlPos() >= 0 && uID == g_TrusteeshipData.GetMicroControlPlayer()->dwID)//微操角色已经当自己绘制过了
// 		{
// 			return;
// 		}

        wStatus				= pChar->GetStatus();
        byExtraStatus       = pChar->GetExtraState();
        pAction				= &pChar->GetAction();		

        // 为了防止移动过程中变动方向
        if(pAction->IsMove())
            m_DrawInf.iDir	= pAction->cDir;
		else if (pChar->IsInShadowKill8())
			m_DrawInf.iDir	= pChar->GetShadowKill8Dir();
        else
            m_DrawInf.iDir	= pChar->GetDir();
        m_DrawInf.iSex		= pChar->GetSex();
        m_DrawInf.iJob		= pChar->GetCareer();
        m_DrawInf.pLast		= pChar->GetLastTex();
        m_DrawInf.wBody     = pChar->GetBody();
		if (pChar->GetCharHolyWingLevel() >= 0)
		{
			m_DrawInf.byWing = 100 + pChar->GetCharHolyWingLevel() / 10 + 1;
			m_DrawInf.byWingEff = 1;
			if (pChar->GetCharHolyWingStrongLevel() >= 5)
			{
				m_DrawInf.byWingEff = 2;
			}
		}
		else
		{
            m_DrawInf.byWing    = (BYTE)pChar->GetReserveData(pubHasWings);
		}


		////夺宝1.28版添加 
		//if (m_DrawInf.byWing > 0)
		//{
		//	if (pChar->GetCharHolyWingLevel() == 1)
		//	{
		//		m_DrawInf.byWing = 100 + 5;				
		//	}
		//	else if (pChar->GetCharHolyWingLevel() == 2)
		//	{
		//		m_DrawInf.byWing = 100 + 2;
		//	}

		//	if (pChar->GetCharHolyWingStrongLevel() == 3)
		//	{
		//		m_DrawInf.byWingEff = 2;
		//	}
		//	else if (m_DrawInf.byWing > 100)
		//	{
		//		m_DrawInf.byWingEff = 1;
		//	}
		//}


		//m_DrawInf.iPutOnShield = pChar->GetPutOnShield();
		//m_DrawInf.iGuildFlag = pChar->GetGuildFlag();
		m_DrawInf.iPutOnShield = pChar->GetShieldType();
		m_DrawInf.iShieldLevel = pChar->GetShieldLevel();

        if(pChar->IsBianShen())
            m_DrawInf.pLooks = &(pChar->GetBianShenLooks());
        else
            m_DrawInf.pLooks	= &(pChar->GetLooks());

        pTexFront	 = pChar->GetTexFront();
        pTexBack	 = pChar->GetTexBack();

        bBoothState = pChar->GetBoothState();
        if(bBoothState)
        {
            m_DrawBooth.iBoothType = pChar->GetBoothInfo().iBoothType;
            m_DrawBooth.dwFlagColor = pChar->GetBoothInfo().dwFlagColor;
            m_DrawBooth.iFlagType = pChar->GetBoothInfo().iFlagType;		
        }

        if(pChar->IsOnLepoard())
            m_DrawInf.byArm = pChar->GetMonsterArmLevel()+1;
        else
            m_DrawInf.byArm = 0;
    }
    else
    {
		pCharAttr = &(SELF);
        if(SELF.GetReserveData(pubDisableDraw) == 1)
            return;
        if(SELF.IsBianShen())
        {
            if(bNotAlpha)
                DrawMonster(iX,iY,NULL);

            return;
        }

        uID					= SELF.GetID();
        wStatus				= SELF.GetStatus();
        byExtraStatus		= SELF.GetExtraState();
        pAction				= &SELF.GetAction();

        if(pAction->IsMove())
            m_DrawInf.iDir	= pAction->cDir;
        else if (SELF.IsInShadowKill8())
			m_DrawInf.iDir	= SELF.GetShadowKill8Dir();
		else
            m_DrawInf.iDir	= SELF.GetDir();

        m_DrawInf.iSex		= SELF.GetSex();
        m_DrawInf.iJob		= SELF.GetCareer();
        m_DrawInf.pLast		= SELF.GetLastTex();
        m_DrawInf.wBody     = SELF.GetBody();


		if (SELF.GetHolyWingLevel() >= 0)
		{
			m_DrawInf.byWing = 100 + SELF.GetHolyWingLevel() / 10 + 1;
			m_DrawInf.byWingEff = 1;
			if (SELF.GetHolyWingStrongLevel() >= 5)
			{
				m_DrawInf.byWingEff = 2;
			}
		}		
		else
		{
			m_DrawInf.byWing    = (BYTE)SELF.GetReserveData(pubHasWings);
		}

		////夺宝1.28版添加 
		//if (m_DrawInf.byWing > 0)
		//{
		//	if (SELF.GetCharHolyWingLevel() == 1)
		//	{
		//		m_DrawInf.byWing = 100 + 5;				
		//	}
		//	else if (SELF.GetCharHolyWingLevel() == 2)
		//	{
		//		m_DrawInf.byWing = 100 + 2;
		//	}

		//	if (SELF.GetCharHolyWingStrongLevel() == 3)
		//	{
		//		m_DrawInf.byWingEff = 2;
		//	}
		//	else if (m_DrawInf.byWing > 100)
		//	{
		//		m_DrawInf.byWingEff = 1;
		//	}
		//}

		m_DrawInf.dwFlag   |= DRAWINFO_FLAG_SELF;
		//m_DrawInf.iPutOnShield = SELF.GetPutOnShield();
		//m_DrawInf.iGuildFlag = SELF.GetGuildFlag();

		//if(m_DrawInf.iGuildFlag != 0 &&						//普通盾牌不需要绘制行会旗帜所对应的特效
		//	SELF.EquipGood().Get(ITEM_POS_SHIELD).GetShape() != 1 &&
		//	SELF.EquipGood().Get(ITEM_POS_SHIELD).GetShape() != 2 &&
		//	SELF.EquipGood().Get(ITEM_POS_SHIELD).GetShape() != 3)
		//{
		//	m_DrawInf.iGuildFlag = 0;
		//}
		m_DrawInf.iPutOnShield = SELF.GetShieldType();
		m_DrawInf.iShieldLevel = SELF.GetShieldLevel();

        if(SELF.IsBianShen())
            m_DrawInf.pLooks    = &(SELF.GetBianShenLooks());
        else
            m_DrawInf.pLooks	= &(SELF.GetLooks());

        pTexFront	 = SELF.GetTexFront();
        pTexBack	 = SELF.GetTexBack();
        bSelf		 = true;

        //设置摊位信息
        bBoothState = SELF.GetBoothState();
        if(bBoothState)
        {
            m_DrawBooth.iBoothType = SELF.GetBoothInfo().iBoothType;
            m_DrawBooth.dwFlagColor = SELF.GetBoothInfo().dwFlagColor;
            m_DrawBooth.iFlagType = SELF.GetBoothInfo().iFlagType;
        }
        if(SELF.IsOnLepoard())
            m_DrawInf.byArm = 2;
        else
            m_DrawInf.byArm = 0;
    }	

    // 调整非法方向
    if(m_DrawInf.iDir >= DIR_NO || m_DrawInf.iDir < DIR_UP)
        m_DrawInf.iDir = DIR_DOWN;

    // 画图位置
	// 画图位置,人物中心位置，人物图片为256*256
	//x = iX - 128 + TILE_WIDTH / 2;
	//y = iY - 128 + TILE_HEIGHT / 2;
	x = iX - 96;
	y = iY - 132;

	//骑上豹子以后都往身后位移一点，免得在骑战时两个豹子叠在一起
	bool bIsRideLepoard = false;
	int iDir = 0;
	if(!pChar)
	{
		iDir = SELF.GetDir();
		bIsRideLepoard = SELF.IsOnLepoard();
	}
	else
	{
		iDir = pChar->GetDir();
		bIsRideLepoard = pChar->IsOnLepoard();
	}
	if(bIsRideLepoard)
	{
		int ix = 0,iy = 0;

		GetDevAccordDir(iDir,ix,iy);
		x += ix;
		y += iy;

		iX += ix;
		iY += iy;
	}

    DWORD dwColor = COLOR_DEFAULT;

    // 人物的中毒状态
    if(wStatus & CS_LULL)			//石化
        dwColor = DEFAULT_COLOR_STONE;
    else if(wStatus & CS_RED)		//红色
        dwColor = COLOR_RED;
    else if(wStatus & CS_GREEN)		//绿色
        dwColor = COLOR_GREEN;
    else if(wStatus & CS_PURPLE)	//紫色
        dwColor = COLOR_PURPLE;
    else if(wStatus & CS_BLUE)		//蓝色
        dwColor = 0xFF0000C0;
    else if(wStatus & CS_HIDE)      //隐身
        dwColor = COLOR_HIDE;
    else if(wStatus & CS_BLACK)
        dwColor = DEFAULT_COLOR_BLACK;
    else if(byExtraStatus & ES_FROST) //冻住了
        dwColor = COLOR_FROZEN;
	else if(byExtraStatus & ES_FIRE)		//红色
		dwColor = COLOR_RED;

    if(g_AICfgMgr.IsTeamColor())
    {
        if(pChar)
        {
            if(pChar->GetRelationType() & RT_GROUP)
				dwColor = 0xffffff00;//0xFFFF00FF;
        }
        else
        {
			dwColor = 0xffffff00;//0xFFFF00FF;
        }
    }

	DWORD dwGraduaAppear = pCharAttr->GetReserveData(pubGradualAppear);
 
	if (dwGraduaAppear > 0)
	{
        if(dwGraduaAppear >= 10  && dwGraduaAppear <= 19)//是淡入的处理模式 显现出来
        {
            dwColor &= 0x00FFFFFF;

            DWORD dw = 0x0B;
            int iSpeed = dwGraduaAppear - 9;
            dw = dw*iSpeed*(GetTickCount() - pCharAttr->GetReserveData(pubGradualTime))/500;
            if(dw >= 0xFF)
            {
                dw = 0xFF;
                pCharAttr->SetReserveData(pubGradualAppear,0);//显现出来
                pCharAttr->SetReserveData(pubGradualTime,0);
            }
            dwColor += (dw<<24) ;
        }
		else if(dwGraduaAppear >= 20  && dwGraduaAppear <= 29)/////是淡出的处理模式 消失
        {
            dwColor |= 0xFF000000;

            DWORD dw = 0x0B;
            int iSpeed = dwGraduaAppear - 19;
            dw = dw*iSpeed*(GetTickCount() - pCharAttr->GetReserveData(pubGradualTime))/50;
            if(dw >= 0xFF)
            {
                dw = 0xFF;
            }
            dwColor -= (dw<<24) ;
        }
		else if (dwGraduaAppear > 1000  && dwGraduaAppear < 2000)//是淡入的处理模式 显现出来
		{
			dwColor &= 0x00FFFFFF;

			DWORD dw = 0x0B;

			dw = dw + (GetTickCount() - pCharAttr->GetReserveData(pubGradualTime))/ (dwGraduaAppear - 1000);
			if(dw >= 0xFF)
			{
				dw = 0xFF;
				pCharAttr->SetReserveData(pubGradualAppear,0);//显现出来
				pCharAttr->SetReserveData(pubGradualTime,0);
			}
			dwColor += (dw<<24) ;
		}
		else if(dwGraduaAppear > 2000  && dwGraduaAppear < 3000)/////是淡出的处理模式 消失
        {
            dwColor |= 0xFF000000;

            DWORD dw = 0x0B;

			dw = dw + (GetTickCount() - pCharAttr->GetReserveData(pubGradualTime)) / (dwGraduaAppear - 2000);
            if(dw >= 0xFF)
            {
                dw = 0xFF;
            }
            dwColor -= (dw<<24) ;
        }

	}


 

    // 绘图颜色
    if(bNotAlpha)
        m_DrawInf.dwColor	= dwColor;
    else
    {
        if(pChar)
            m_DrawInf.dwColor   = ColorMultiply(dwColor,0x50999999);
        else
            m_DrawInf.dwColor   = ColorMultiply(dwColor,0x80FFFFFF);
    }

	m_DrawInf.bAlpha	= !bNotAlpha;

	int iHorse = m_DrawInf.pLooks->Player.wHorse;
	SELF.GetXY(iSelfX,iSelfY);
	if (pChar)
	{
		pChar->GetXY(iOtherX,iOtherY);
	}
	else
	{
		iOtherX = iSelfX;
		iOtherY = iSelfY;
	}	

    m_DrawInf.iAction	= pAction->wAction;
	m_DrawInf.iDrawAction = pAction->wDrawAction;	
	m_DrawInf.iActionFrames	= pAction->iFrameCount;
    m_DrawInf.iFrame	= pAction->iFrameNow;	

    if(pAction->wAction == ACTION_APPEAR && pAction->iData == 5)//像食人花消失时就是把出现的动作倒着放
	{
        m_DrawInf.iFrame = pAction->iFrameCount - 1 - m_DrawInf.iFrame;
	}
    m_DrawInf.bNeedRt	= bNotAlpha;
    m_DrawInf.bNotAlpha = bNotAlpha;


	//法师骑战3格以上的话除了第一格和最后一格外中间的放指定那张图
	if(pAction->wAction == ACTION_LFMAGICRAB && pAction->iData > 4)
	{
		int iOffX = 0,iOffY = 0;
		int iX = iSelfX,iY = iSelfY;
		SELF.GetOffset(iOffX,iOffY);
		int iDrawOffX = 0,iDrawOffY = 0;
		SELF.GetDrawOffset(iDrawOffX,iDrawOffY);

		if (pChar)
		{
			iX = iOtherX;iY = iOtherY;
			pChar->GetOffset(iOffX,iOffY);
			pChar->GetDrawOffset(iDrawOffX,iDrawOffY);
		}
		iX += (iOffX + iDrawOffX) / TILE_WIDTH;
		iY += (iOffY + iDrawOffY) / TILE_HEIGHT;

		if(iX != pAction->iStartX || iY != pAction->iStartY)
		{
			if(abs(iX - pAction->iAimX) <= 1 && abs(iY - pAction->iAimY) <= 1)
			{
				m_DrawInf.iFrame -= pAction->uData;
				if(m_DrawInf.iFrame < 3)
					m_DrawInf.iFrame = 3;
			}
			else
			{
				m_DrawInf.iFrame = 3;
			}
		}
		else 
		{
			if(m_DrawInf.iFrame > 3)
				m_DrawInf.iFrame = 3;
		}
	}

// 	if (bNotAlpha && g_TrusteeshipData.IsCaptain())//在组队中，被绘制角色不是自己，并且自己是队长
// 	{
//  	bool bDrawEffect = false;
// 		if (pChar == NULL && (g_TrusteeshipData.GetCaptionSubstitutePos() >= 0 || g_TrusteeshipData.GetMicroControlPos() >= 0))//附身或微操
// 		{
// 			bDrawEffect = true;
// 		}
// 		else if (pChar)//选中
// 		{
// 			for (int i=0; i < MAX_TRUSTEESHIP_NUM; i++)
// 			{
// 				if (pChar->GetID() == g_TrusteeshipData.GetTneupMember(i).dwID && g_TrusteeshipData.GetTneupMember(i).bSelected)
// 				{
// 					bDrawEffect = true;
// 					break;
// 				}
// 			}
// 		}
// 
// 		if (bDrawEffect)
// 		{
// 			g_pGfx->SetRenderMode(RM_ADD2);
// 			LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE, 20999);
// 			g_pGfx->DrawTextureNL(iX-129, iY-165, pTex);
// 			g_pGfx->SetRenderMode();
// 		}
// 
// 	}

    //保存武器,在绘制完人物后恢复，因为绘制野蛮冲撞和突斩时会暂时将m_DrawInf.pLooks->Player.byWeapon设置为0
    BYTE byWeapon;
    if(m_DrawInf.pLooks->Player.byType == CHARACTER_HUMAN)
    {
        byWeapon = m_DrawInf.pLooks->Player.byWeapon;
    }

    // 野蛮冲撞
 	BOOL bSavegeCollide = FALSE;
	if((pAction->wAction == ACTION_RUN) && (pAction->uFlag & SERVER_ACTION) && (pAction->uFlag & COLLIDE_ACTION))
    {
        m_DrawInf.pLooks->Player.byWeapon = 0;
        DrawWildCollide(x, y);
    }

	//强袭烈焰盾攻击;分身放大
	if(pAction->wAction == ACTION_MAGIC)
	{
		if (pAction->iData == MAGICID_FIRE_SHIELD_ATTACK)
		{
			DrawFireShieldAttack(x,y,pChar);
		}
	}

    //残影刀法
    if((pAction->wAction == ACTION_ATTACK1) && ((pAction->uFlag & MAGIC_ACTION) != 0) && (pAction->iData ==  MAGICID_REMAIN_SHADE))
    {
		if(m_DrawInf.iFrame >= 1 && m_DrawInf.iFrame < 6)
		{
			DWORD dwColor = m_DrawInf.dwColor;
			m_DrawInf.dwColor = 0xA0FFFFFF;
			m_DrawInf.iFrame -= 1;
			DrawLooks(x,y,&m_DrawInf);
			//m_DrawInf.iFrame += 1;
			//DrawLooks(x,y,&m_DrawInf);
			m_DrawInf.iFrame += 1;
			m_DrawInf.dwColor = dwColor;
		}
		//残影刀法的音效
		UINT iSound = MAGICID_REMAIN_SHADE*4 +16;
		if (m_DrawInf.iFrame == 4 && pAction->iDealTimesCurFrame == 1)
			g_pAudio->PlayEx(EAT_MAGIC,iSound,g_pAudio->GetRand()++,iSelfX,iOtherX,iSelfY,iOtherY,bSelf,1);
    }

	//法宝
	m_DrawInf.pFaBao = pCharAttr->GetFaBaoType();

    if(bNotAlpha)
    {
        if(bBoothState)
        {
            g_ActionInf.GetBoothInfo(m_DrawInf.iDir,&m_DrawBooth); //取得摊位数据
            DrawSeniorBooth(iX,iY,&m_DrawBooth,TRUE);
        }

        if(pTexBack)
        {
            DrawTexShow(pTexBack);
            if(pChar)
                pChar->ClrTexBack();
            else
                SELF.ClrTexBack();
        }


		if (uID == SELF.GetReserveData(plyAttackLockID) || uID == SELF.GetReserveData(plyMagicLockID) || uID == SELF.GetReserveData(plyAttackLockID))
		{
			g_pGfx->SetRenderMode(RM_ADD2);
			g_pGfx->DrawTextureNL(x,y,g_pTexMgr->GetTex(PACKAGE_INTERFACE,16650,EP_MAGIC));
			g_pGfx->SetRenderMode();
		}

		BYTE TypeBufferQQ = pCharAttr->GetTypeBufferQQ();
		if (TypeBufferQQ > 0)
		{
			int iX,iY;
			pCharAttr->GetTopXY(iX,iY);

			LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE ,22240+TypeBufferQQ,EP_MAGIC);
			
			if(pTex)
			{
				g_pGfx->SetRenderMode(RM_ALPHAADD);
				g_pGfx->DrawTextureNL(iX - 60,iY,pTex);
				g_pGfx->SetRenderMode();
			}
				
		}


    }

	int iFrameNow = pAction->iFrameNow;
	int iDealTimesCurFrame = pAction->iDealTimesCurFrame;

	if(iFrameNow == 0 && iDealTimesCurFrame == 1 && bNotAlpha)
    {
        switch(pAction->wAction)
        {
        case ACTION_WALK:
        case ACTION_RUN:
            {
				if(iHorse > 0)
				{
					if(IsPhenix(iHorse))	//凤凰走路声音
					{
						lSoundID = 2438;
					}
					else if(pAction->wAction == ACTION_WALK)
					{
						if(!IsLeopard(iHorse))
							lSoundID = 1229;
					}
					else
					{
						if(IsLeopard(iHorse))
							lSoundID = 1231;
						else
							lSoundID = 1230;						
					}
					
					if(lSoundID > 0)
						g_pAudio->PlayEx(EAT_MONSTER,lSoundID,g_pAudio->GetRand()++,iSelfX,iOtherX,iSelfY,iOtherY,bSelf);
				}
				else
				{
					BYTE i = (g_pGameMap->GetTile(iOtherX,iOtherY))?g_pGameMap->GetTile(iOtherX,iOtherY)->bySound:0;
					if( pAction->wAction == ACTION_WALK ) 
						lSoundID = 40+i;
					else
						lSoundID = 60+i;

					if(lSoundID > 0)
						g_pAudio->PlayEx(EAT_OTHER,lSoundID,g_pAudio->GetRand()++,iSelfX,iOtherX,iSelfY,iOtherY,bSelf,1);
				}

				
            }
			break;
        case ACTION_ATTACKED:
            {
				if(iHorse > 0)
				{
					if(m_DrawInf.iSex == 1) //女 
						lSoundID = 3;
					else
						lSoundID = 1;

					g_pAudio->PlayEx( EAT_OTHER,127, g_pAudio->GetRand()++,iSelfX,iOtherX,iSelfY,iOtherY,bSelf,1);
					g_pAudio->PlayEx( EAT_MAGIC,lSoundID,g_pAudio->GetRand()++,iSelfX,iOtherX,iSelfY,iOtherY,bSelf,1);
				}
                break;
            }
        case ACTION_LSALUTE:
            {
				if(iHorse >= 166 && iHorse <= 168 )
					lSoundID = 1353;//豹子打招呼
				else if(iHorse >= 185 && iHorse <= 187)
					lSoundID = 1360;
				else if(iHorse >= 39 && iHorse <= 41)
					lSoundID = 1361;//麒麟打招呼
				else if(iHorse >= 301 && iHorse <= 303)
					lSoundID = 2430;
				if(lSoundID>0)
                    g_pAudio->PlayEx(EAT_MONSTER,lSoundID,g_pAudio->GetRand()++,iSelfX,iOtherX,iSelfY,iOtherY,bSelf);

                break;
            }
        default:
            break;
        }
    }

    if(iFrameNow == 1 && iDealTimesCurFrame == 1 && bNotAlpha)
    {
        switch(pAction->wAction)
        {
        case ACTION_ATTACKED:
            {
                if(m_DrawInf.iSex == 1) //女 
                    lSoundID = 3;
                else
                    lSoundID = 1;

                g_pAudio->PlayEx( EAT_OTHER,127, g_pAudio->GetRand()++,iSelfX,iOtherX,iSelfY,iOtherY,bSelf,1);
                g_pAudio->PlayEx( EAT_MAGIC,lSoundID,g_pAudio->GetRand()++,iSelfX,iOtherX,iSelfY,iOtherY,bSelf,1);
                break;
            }
        case ACTION_CUT:
            {
                g_pAudio->PlayEx(EAT_MAGIC,2,g_pAudio->GetRand()++,iSelfX,iOtherX,iSelfY,iOtherY,bSelf,1);	
                break;
            }
        case ACTION_ATTACK1:
            {
                if (iHorse != 13)
                {
                    if (g_AIMgr.HaveChopEquip())
                        g_pAudio->PlayEx(EAT_MAGIC,8,g_pAudio->GetRand()++,iSelfX,iOtherX,iSelfY,iOtherY,bSelf,1);
                    else if(pAction->iData == 0)
                        g_pAudio->PlayEx(EAT_OTHER,128 + m_DrawInf.pLooks->Player.byWeapon,g_pAudio->GetRand()++,iSelfX,iOtherX,iSelfY,iOtherY,bSelf,1);
                }
                break;
            }

        default:
            break;
        }
    }

	if(iFrameNow == 8 && iDealTimesCurFrame == 1 && bNotAlpha && (pAction->wAction == ACTION_WALK || pAction->wAction == ACTION_RUN))
    {
        BYTE i = (g_pGameMap->GetTile(iOtherX,iOtherY))?g_pGameMap->GetTile(iOtherX,iOtherY)->bySound:0;
        if( pAction->wAction == ACTION_WALK ) 
            lSoundID = 40+i;
        else
            lSoundID = 60+i;

        g_pAudio->PlayEx(EAT_OTHER,lSoundID,0,iSelfX,iOtherX,iSelfY,iOtherY,bSelf,1);
    }


	if(DrawLooks(x,y,&m_DrawInf))
    {
		if((pAction->wAction == ACTION_RUN) && (pAction->uFlag & SERVER_ACTION) && (pAction->uFlag & COLLIDE_ACTION))
		{
			m_DrawInf.pLooks->Player.byWeapon = 0;	
			BYTE byType = 0;
			if(pAction->uFlag & COLLIDE_SHEN)
				byType = 1;
			else if (pAction->uFlag & COLLIDE_MO)
				byType = 2;

			DrawSavageCollide(x,y,byType);			
		}

		if(bNotAlpha)
        {
			DrawShadowKill8(x,y,pChar);

            //绘制人物身上的持续特效,如魔法盾等
            if(g_pMagicMgr)
                g_pMagicMgr->DrawHumanEff(iX,iY,pChar);			

			//幻影套装
			if( (pChar && pChar->GetReserveData(pubHuanCaiCloth) == 2) || (!pChar && SELF.GetReserveData(pubHuanCaiCloth) == 2) )
			{
				if(m_DrawInf.iDrawAction == ACTION_RUN)
				{
					DWORD dwColor = m_DrawInf.dwColor;
					
					for(int ii = 0; ii <= 3; ii++)
					{
						int ax,ay,sx,sy;
						GetDirStep(m_DrawInf.iDir,ax,ay);			
						sx = x - (16 * (ii+1) + (ii+1) * (ii+1)) * ax;
						sy = y - (8 * (ii+1) + (ii+1) * (ii+1) / 2) * ay;

						DWORD dwTemp = 0xA0 - 0x1A * (ii+1);
						m_DrawInf.dwColor = (dwTemp<<24)|(dwColor & 0x00FFFFFF);

						DrawLooks(sx,sy,&m_DrawInf);
					}

					m_DrawInf.dwColor = dwColor;
				}
			}

            //身前特效
            if(pTexFront)
            {
                DrawTexShow(pTexFront);
                if(pChar)
                    pChar->ClrTexFront();
                else
                    SELF.ClrTexFront();
            }

            if(bBoothState)
                DrawSeniorBooth(iX,iY,&m_DrawBooth,FALSE);

			//取得信息
            if(pChar)
            {
                pChar->SetTopXY(iX + 32,iY + 26 - 104);
                pChar->SetTexWH(64,104);
            }
            else
            {
                SELF.SetTopXY(iX + 32,iY + 26 - 104);
                SELF.SetTexWH(64,104);
            }

            if((m_DrawInf.iMouseIn == 2  || (pAction->wAction != ACTION_DEATH && m_DrawInf.iMouseIn > 0)) && !m_bMouseInNpc)
            {
				int iType;
				if(m_DrawInf.iMouseIn == 2)
				{
					if( pAction->wAction == ACTION_DEATH )
						iType = 12;
					else
						iType = 13;
				}
				else
				{
					if( pAction->wAction == ACTION_DEATH )
						iType = 2;
					else
						iType = 3;
				}

				if(iType > g_pControl->GetMouseOnType() || (iType == g_pControl->GetMouseOnType() && 104 < g_pControl->GetMouseOnHeight() + 32))
				{
					g_pControl->SetMouseOnID(uID);
					g_pControl->SetMouseOnType(iType);
					g_pControl->SetMouseOnHeight(104);
				}
            }

            //绘制突斩刀光
            if((pAction->wAction == ACTION_RUN)  && (pAction->uFlag & SERVER_ACTION) 
                &&(pAction->uFlag & COLLIDE_ACTION) && (byExtraStatus & 0x04) )
            {
                DrawSuddenKill(x, y, pAction, pChar);
            }
        }

		//绘制纵雷诀残影
		if(m_DrawInf.iDrawAction == ACTION_LFMAGICRAB)
		{				
			DrawZhongLeiShadow(x,y);
		}
    }

    //恢复武器,因为绘制野蛮冲撞和突斩时会将m_DrawInf.pLooks->Player.byWeapon设置为0
    if(m_DrawInf.pLooks->Player.byType == CHARACTER_HUMAN)
    {
        m_DrawInf.pLooks->Player.byWeapon = byWeapon;
    }	
    //画从头上冒出来的经验及其它
   // DrawBubbleUp(NULL);


	//根据状态绘制特效
	CSimpleCharacter* magicOwner = pChar;
	if (magicOwner == NULL)
	{
		magicOwner = &SELF;
	}

	//狂怒旋风重力效果
	if (magicOwner->GetTuTengState() & ETTS_GRAVITY)
	{
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_magic1,47240,EP_MAGIC);
		
		if(pTex)
		{
			g_pGfx->SetRenderMode(RM_ADD2);
			g_pGfx->DrawTextureNL(x,y,pTex);
			g_pGfx->SetRenderMode();
		}

	}

	if (pChar == NULL)
		return;
// 	if (g_TrusteeshipData.IsCaptain() && g_TrusteeshipData.IsSelecting() && pChar->GetID() != SELF.GetID())
// 	{
// 		TneupMember* pMember = g_TrusteeshipData.FindMemberByID(pChar->GetID());
// 		if (pMember)
// 		{
// 			RECT rc = g_TrusteeshipData.GetSelRect();
// 			if (rc.right < rc.left + 5 && rc.bottom < rc.top + 5)
// 				return;
// 
// 			int tmp;
// 			if (rc.right<rc.left)
// 			{
// 				tmp = rc.left;
// 				rc.left = rc.right;
// 				rc.right = tmp;
// 			}
// 			if (rc.bottom<rc.top)
// 			{
// 				tmp = rc.top;
// 				rc.top = rc.bottom;
// 				rc.bottom = tmp;
// 			}
// 			POINT pt = {iX+30, iY};
// 			if (PtInRect(&rc, pt))
// 			{
// 				pMember->bSelected = true;
// 			}
// 			else
// 			{
// 				pMember->bSelected = false;
// 			}	
// 		}
// 	}
// 


}

void CSceneManager::DrawMonster(int iX,int iY,CSimpleCharacter* pChar,BOOL bNotAlpha)
{
    CCharacterAttr *pCharAttr = pChar;
    int iMonster = 0;

    if(!pCharAttr)//pChar为NULL时表示画自己变身后的效果
        pCharAttr = &(SELF);
    else
        iMonster = pCharAttr->GetRaceNo();

    if( !g_pGameData || pCharAttr->GetReserveData(pubDisableDraw) == 1) 
        return;

    if(pCharAttr->IsDead() && g_AICfgMgr.IsBury() && iMonster != 56 && iMonster != 57 && iMonster != 209 && iMonster != 208 && iMonster != 333 && iMonster != 334 && iMonster != 335 && iMonster != 336)//门1、门2两种怪死了后自然处于开启状态
        return;

    // 临时变量
    LPTexture		pTex = NULL;
    int				x,y;

    // 角色信息
    int				iSelfX,iSelfY;			// 主角所在格子坐标
    int				iOtherX,iOtherY;		// 当前角色所在格子坐标
    UINT			uID;					// ID
    SAction		*	pAction;				// 当前动作

    // 声音
    UINT			lSoundID = 0;
    tex_show_t *	pTexFront = NULL;
    tex_show_t *	pTexBack = NULL;

    memset(&m_DrawInf,0,sizeof(m_DrawInf));

    uID					= pCharAttr->GetID();
    pAction	= &(pCharAttr->GetAction());

	m_DrawInf.dwFlag = 0;

    SELF.GetXY(iSelfX,iSelfY);

    //防止移动过程中变动方向
    if(pAction->IsMove())
        m_DrawInf.iDir	= pAction->cDir;
    else
        m_DrawInf.iDir	= pCharAttr->GetDir();

    if(pCharAttr->GetBianShenLooksValue() != 0)
        m_DrawInf.pLooks	= &(pCharAttr->GetBianShenLooks());
    else
        m_DrawInf.pLooks	= &(pCharAttr->GetLooks());

	m_DrawInf.iSex	= pCharAttr->GetSex();

    m_DrawInf.pLast		= pCharAttr->GetLastTex();

	m_DrawInf.pFaBao = pCharAttr->GetFaBaoType();

    pCharAttr->GetXY(iOtherX,iOtherY);
    pTexFront	 = pCharAttr->GetTexFront();
    pTexBack	 = pCharAttr->GetTexBack();


    // 调整非法方向
    if(m_DrawInf.iDir >= DIR_NO || m_DrawInf.iDir < DIR_UP)
        m_DrawInf.iDir = DIR_DOWN;

    m_DrawInf.fScale = 1.0f;
    //m_DrawInf.fScale = g_ScaleRate.fx;

    // 画图位置,人物中心位置，人物图片为256*256
    //x = iX - 128 + TILE_WIDTH / 2;
    //y = iY - 128 + TILE_HEIGHT / 2;
	x = iX - 96;
	y = iY - 132;

    switch(iMonster)
    {
    case 235:
        if(pAction->wAction == ACTION_DEATH)
        {
            x = iX - 96 - 256;
            y = iY - 96 - 256;
        }
        break;
    case 236:
        x = iX - 480;	
        y = iY - 402;
        break;
	case 239://马馆
	case 240://天魔机关
		m_DrawInf.iDir = m_DrawInf.iDir % 3;
		break;
	case 242://寒荒海兽
	case 251://常胜玄兵
	case 252://常胜玄兵
	case 253://常胜玄兵
	case 256://行会怪物1
	case 257://行会怪物2
	case 258://行会怪物3
	case 259://行会怪物4
	case 260://红图腾
	case 261://绿图腾
	case 262://蓝图腾
	case 263://黄图腾
	case 264://红图腾2
	case 265://绿图腾2
	case 266://蓝图腾2
	case 267://黄图腾2
	case 268://红图腾3
	case 269://绿图腾3
	case 270://蓝图腾3
	case 271://黄图腾3
	case 272://红图腾4
	case 273://绿图腾4
	case 274://蓝图腾4
	case 275://黄图腾4
	case 276://箭塔
	case 316://火柱
	case 317://冰柱
	case 320://护塔机关
	case 338://树1
	case 339://树2
	case 340://树3
	case 341://树4
	case 342://树5
	case 343://树6
	case 344://树7
	case 355://炸弹
	case 356:
	case 447:
	case 422:
	case 423:
	case 424:
	case 425:
	case 426:
	case 427:
	case 428:
	case 429:
	case 430:
	case 431:
	case 432:
	case 433:
		m_DrawInf.iDir = 0;
		break;
	case 353://足球
		if (pAction->wAction == ACTION_APPEAR || pAction->wAction == ACTION_STAND)
		{
			m_DrawInf.iDir = 0;
		}
		break;
	case 205://铜人	的图放到71怪物编号的位置了	
		iMonster = 71;
		break;
	case 5://行会通灵塔
		x -= 770;
		y -= 1100;
		break;
	case 321://护塔机关2	
		iMonster = 317;
		m_DrawInf.iDir = 0;
		m_DrawInf.pLooks->Char.wShape = iMonster;
		break;
	case 322://护塔机关3
		iMonster = 316;
		m_DrawInf.iDir = 0;
		m_DrawInf.pLooks->Char.wShape = iMonster;
		break;
	case 333://门
	case 334://门
	case 335://门
	case 336://门
	case 337:
	case 345:
	case 346:
	case 347:
		m_DrawInf.iDir = 0;
		break;
	case 388://圣殿将军状态1
		if(pAction->wAction == ACTION_ATTACK6 || pAction->wAction == ACTION_ATTACK5 || pAction->wAction == ACTION_APPEAR)
			m_DrawInf.iDir = 0;
		break;
	case 389://圣殿将军状态2
		if(pAction->wAction == ACTION_ATTACK6 || pAction->wAction == ACTION_ATTACK5)
			m_DrawInf.iDir = 0;
		break;
	case 390://石头剑
	case 416://关着的门
	case 417://开着的门
	case 418://将军雕像
		if(pAction->wAction == ACTION_STAND || pAction->wAction == ACTION_DEATH)
			m_DrawInf.iDir = 0;
		break;
	case 400://柱子
	case 401://柱子
	case 402://柱子
	case 403://柱子
	case 404://柱子
	case 405://柱子
	case 406://柱子
	case 407://柱子
	case 408://柱子
	case 409://柱子	
		if(pAction->wAction == ACTION_STAND || pAction->wAction == ACTION_APPEAR)
			m_DrawInf.iDir = 0;
		break;
	case 419://圣武斧卫
		y = iY - 148;
		break;
	default:
		break;
    }



    // 绘图颜色
    m_DrawInf.iAction	= pAction->wAction;
	m_DrawInf.iDrawAction = pAction->wDrawAction;
    m_DrawInf.iFrame	= pAction->iFrameNow;
    if(pAction->wAction == ACTION_APPEAR && pAction->iData == 5 )//像食人花消失时就是把出现的动作倒着放
        m_DrawInf.iFrame = pAction->iFrameCount - 1 - m_DrawInf.iFrame;
	m_DrawInf.iActionFrames	= pAction->iFrameCount;
    m_DrawInf.bNeedRt	= bNotAlpha;
    m_DrawInf.bNotAlpha = bNotAlpha;

    if(pChar && pChar->IsCanRideLeopard()) //豹子
        m_DrawInf.byArm = pChar->GetMonsterArmLevel()+1;

    // 人物的中毒状态
    DWORD  dwColor = COLOR_DEFAULT;

    WORD wStatus = pCharAttr->GetStatus();
    WORD byExtraStatus = pCharAttr->GetExtraState();
    if(wStatus & CS_LULL)			//石化
        dwColor = DEFAULT_COLOR_STONE;
    else if(wStatus & CS_RED)		//红色
        dwColor = COLOR_RED;
    else if(wStatus & CS_GREEN)		//绿色
        dwColor = COLOR_GREEN;
    else if(wStatus & CS_PURPLE)	//紫色
        dwColor = COLOR_PURPLE;
    else if(wStatus & CS_BLUE)		//蓝色
        dwColor = 0xFF0000C0;
    else if(wStatus & CS_BLACK)
        dwColor = DEFAULT_COLOR_BLACK;
    else if(byExtraStatus & ES_FROST) //冻住了
        dwColor = COLOR_FROZEN;
	else if(byExtraStatus & ES_FIRE)		//红色
		dwColor = COLOR_RED;

    if(pChar)
    {
        if(pChar->GetReserveData(pubGradualAppear) >= 10  && pChar->GetReserveData(pubGradualAppear) <= 19)// //是淡入的处理模式 显现出来
        {
            dwColor &= 0x00FFFFFF;

            DWORD dw = 0x0B;
            int iSpeed = pChar->GetReserveData(pubGradualAppear) - 9;
            dw = dw*iSpeed*(GetTickCount() - pChar->GetReserveData(pubGradualTime))/500;
            if(dw >= 0xFF)
            {
                dw = 0xFF;
                pChar->SetReserveData(pubGradualAppear,0);//显现出来
                pChar->SetReserveData(pubGradualTime,0);
            }
            dwColor += (dw<<24) ;
        }
        else if(pChar->GetReserveData(pubGradualAppear) >= 20  && pChar->GetReserveData(pubGradualAppear) <= 29)//是淡出的处理模式 消失
        {
            dwColor |= 0xFF000000;

            DWORD dw = 0x0B;
            int iSpeed = pChar->GetReserveData(pubGradualAppear) - 19;
            dw = dw*iSpeed*(GetTickCount() - pChar->GetReserveData(pubGradualTime))/50;
            if(dw >= 0xFF)
            {
                dw = 0xFF;
            }
            dwColor -= (dw<<24);

            //神之召唤中 大BOSS特效 表现为半透明
            //通天教主(198) 阿修罗神(235) 禁地魔王(192) 蛇妖王(228) 铁血魔王(221)
            if(iMonster == 192 || iMonster == 198 || iMonster == 228|| iMonster == 235 || iMonster == 221)
            {
                if (GetTickCount() - pChar->GetReserveData(pubGradualTime) < 2000)
                    return;
                dwColor |= 0xE0FFFFFF;
                dw = 0x0B;
                dw = dw*iSpeed*( GetTickCount() -  pChar->GetReserveData(pubGradualTime))/5000;
                if(dw >= 0xE0)
                {
                    dw = 0xE0;
                }

                dwColor -= (dw<<24);
            }
        }
    }


    if(bNotAlpha)
        m_DrawInf.dwColor	= dwColor;
    else
        m_DrawInf.dwColor   = ColorMultiply(dwColor,0x50999999);

	m_DrawInf.bAlpha	= !bNotAlpha;

	// 野蛮冲撞
	if((pAction->wAction == ACTION_RUN) && (pAction->uFlag & SERVER_ACTION) && (pAction->uFlag & COLLIDE_ACTION))
	{		
		DrawWildCollide(x, y);
	}

    if(bNotAlpha)
    {
        if(pChar)
        {
            // 灵魂之墙
            if(wStatus & CS_SOULWALL)
            {
                DrawSoulWall(x,y,pChar);
            }
            //禁地魔王的光环,魔法效果
            if(iMonster == 192 && pChar->GetReserveData(pubRingOfJD) == 0)
            {		
                LPTexture pTex1 = NULL,pTex2 = NULL;
                if(m_DrawInf.iAction == ACTION_ATTACK1)
                {
                    if(pAction->iFrameNow == 0 && pAction->iDealTimesCurFrame == 1)
                        g_pAudio->PlayEx(EAT_MONSTER,8,g_pAudio->GetRand()++,iSelfX,iOtherX,iSelfY,iOtherY,false);	
                    pTex1 = g_pTexMgr->GetTex(PACKAGE_magic1, 4831 + m_DrawInf.iFrame,EP_MAGIC);
                    pTex2 = g_pTexMgr->GetTex(PACKAGE_magic1, 4841 + m_DrawInf.iFrame,EP_MAGIC);
                }
                else if(m_DrawInf.iAction == ACTION_DEATH)
                {
                    if(m_DrawInf.iFrame == 7)
                        pChar->SetReserveData(pubRingOfJD,1);
                    pTex2 = g_pTexMgr->GetTex(PACKAGE_magic1, 4851 + m_DrawInf.iFrame,EP_MAGIC);
                }
                else
                {
                    pTex1 = g_pTexMgr->GetTex(PACKAGE_magic1, 4821 + m_DrawInf.iFrame % 3,EP_MAGIC);
                }
                if(pTex1)
                {
                    g_pGfx->SetRenderMode(RM_ADD2);
                    g_pGfx->DrawTextureNL(x-384,y-384,pTex1); 
                    g_pGfx->SetRenderMode();
                }
                if (pTex2)
                {
                    g_pGfx->SetRenderMode(RM_ADD2);
                    g_pGfx->DrawTextureNL(x-384,y-384,pTex2);
                    g_pGfx->SetRenderMode();
                }
            }		
            else if(iMonster >= 56 && iMonster <= 57)//对神龙宝藏门(门1,门2两种怪)的特殊处理
            {
                if(m_DrawInf.iAction != ACTION_DEATH && m_DrawInf.iAction != ACTION_APPEAR)
                {
                    float fLife = (float)pChar->GetHP() / pChar->GetHPMax();
                    if(fLife >= 0.9)
                    {
                        pChar->SetDir(0);
                    }
                    else if(fLife > 0.6)
                        pChar->SetDir(1);
                    else if(fLife > 0.3)
                        pChar->SetDir(2);
                    else
                        pChar->SetDir(3);
                }
            }
            else if(m_DrawInf.iAction==ACTION_DEATH && pChar->GetReserveData(pubBoneState) ==1)
            {
                switch(iMonster)
                {
                case 2:			// 鸡
                    pTex = g_pTexMgr->GetTex(PACKAGE_magic1, 2301 + m_DrawInf.iDir,EP_MONSTER);
                    break;
                case 1:			// 鹿
                    pTex = g_pTexMgr->GetTex(PACKAGE_magic1, 2309 + m_DrawInf.iDir,EP_MONSTER);
                    break;
                case 180:		// 猪
                    pTex = g_pTexMgr->GetTex(PACKAGE_magic1, 2317 + m_DrawInf.iDir,EP_MONSTER);
                    break;
                case 121:		// 树桩
                    pTex = g_pTexMgr->GetTex(PACKAGE_magic1, 2325 + m_DrawInf.iDir,EP_MONSTER);
                    break;
                case 181:		// 兔
                    pTex = g_pTexMgr->GetTex(PACKAGE_magic1, 2333 + m_DrawInf.iDir,EP_MONSTER);
                    break;
                case 131:		// 牛
                    pTex = g_pTexMgr->GetTex(PACKAGE_magic1, 2341 + m_DrawInf.iDir,EP_MONSTER);
                    break;
                case 31:		// 狼
                    pTex = g_pTexMgr->GetTex(PACKAGE_magic1, 2349 + m_DrawInf.iDir,EP_MONSTER);
                    break;
                case 243:
                case 244:
                case 245:
                    pTex = g_pTexMgr->GetTex(PACKAGE_magic1, 2333 + m_DrawInf.iDir,EP_MONSTER);
                    break;
                }
                if (pTex)
                {
                    g_pGfx->DrawTextureNL(x,y,pTex);
                }
                return;
            }
        }

        //身后特效
        DrawTexShow(pTexBack);
		pCharAttr->ClrTexBack();

		if (uID == SELF.GetReserveData(plyAttackLockID) || uID == SELF.GetReserveData(plyMagicLockID) || uID == SELF.GetReserveData(plyAttackLockID))
		{
			g_pGfx->SetRenderMode(RM_ADD2);
			g_pGfx->DrawTextureNL(x,y,g_pTexMgr->GetTex(PACKAGE_INTERFACE,16650,EP_MAGIC));
			g_pGfx->SetRenderMode();
		}

    }


	int iTexOrignH = 12;
	//if (bNotAlpha)
	{
		// 取得返回信息
		int iMonster = m_DrawInf.pLooks->Char.wShape;
		if (iMonster == 205)
			iMonster = 71;
		MonsterTex TmpTex;
		if(m_DrawInf.iAction == ACTION_DEATH)
			TmpTex = g_ActionInf.GetMonsterTex(iMonster,ACTION_DEATH,m_DrawInf.iActionFrames - 1,m_DrawInf.iDir,0);
		else
			TmpTex = g_ActionInf.GetMonsterTex(iMonster,ACTION_STAND,0,m_DrawInf.iDir,0);

		LPTexture pTex = g_pTexMgr->GetTexFromID(TmpTex.dwShapeTex,EP_MONSTER);
		int	x0	= x + 128;
		int y0 = y - 16;
		int iTexW = 64,iTexH = 1040;
		int iTexOffX = 0,iTexOffY = 0;
		if(pTex)
		{
			if(m_DrawInf.iAction == ACTION_DEATH && iMonster != 236)//阿修罗神特殊处理了
			{
				iTexW = pTex->GetWidth();
				//int iTexW0 = pTex->GetWidth0();
				iTexOffX = pTex->GetOffX();
				x0	= x - pTex->GetCenterX() + iTexOffX;
				x0  += iTexW / 2;
			}

			//int iTexH0 = pTex->GetHeight0();
			iTexH = (int)(pTex->GetHeight() * m_DrawInf.fScale);
			iTexOffY = (int)(pTex->GetOffY() * m_DrawInf.fScale);
			y0	= y - pTex->GetCenterY() + iTexOffY;
			y0  -= 10;

			iTexOrignH = pTex->GetHeight0();


			
			//m_DrawInf.iScaleDrawOffX = (int)((m_DrawInf.fScale - 1.0f) * (pTex->GetWidth() / 2 + pTex->GetOffX()));//偏移产生的缩放应该全部除掉,实体部分产生的缩放减一半
			//m_DrawInf.iScaleDrawOffY = (int)((m_DrawInf.fScale - 1.0f) * (pTex->GetHeight() * 0.80f + pTex->GetOffY()));//脚下要靠在原来的地方,但放大了脚部也还是要稍微下来一些
			m_DrawInf.iScaleDrawOffX = 0;
			m_DrawInf.iScaleDrawOffY = 0;

			////x0 是血条的中心点,怪物的除了偏移之外的实体部分的中心点,y0是怪物绘制非透明部分起始位置
			y0 -= m_DrawInf.iScaleDrawOffY;

		}

		pCharAttr->SetTopXY(x0,y0);
		pCharAttr->SetTexWH(iTexW,iTexH);
		pCharAttr->SetTexOffXY(iTexOffX,iTexOffY);
		//怪物绘制非透明部分起始位置y - 132 + iTexOffY + iTexH = 怪物脚底y,(怪物绘制非透明部分起始位置y = 怪物脚底y - (iTexOffY + iTexH - 132) )
	}


	if(DrawLooks(x,y,&m_DrawInf))		// 画成功
    {
		// 野蛮冲撞
		if((pAction->wAction == ACTION_RUN) && (pAction->uFlag & SERVER_ACTION) && (pAction->uFlag & COLLIDE_ACTION))
		{
			BYTE byType = 0;
			if(pAction->uFlag & COLLIDE_SHEN)
				byType = 1;
			else if (pAction->uFlag & COLLIDE_MO)
				byType = 2;

			DrawSavageCollide(x,y,byType);
		}

		if(bNotAlpha)
        {
            DrawTexShow(pTexFront);
			pCharAttr->ClrTexFront();

 
			if((m_DrawInf.iMouseIn == 2 || (pAction->wAction != ACTION_DEATH && m_DrawInf.iMouseIn > 0 && iTexOrignH < 120))&& !m_bMouseInNpc)
            {
				int iType;
				if(m_DrawInf.iMouseIn == 2)
				{
					if( pAction->wAction == ACTION_DEATH )
						iType = 12;
					else
						iType = 13;
				}
				else
				{
					if( pAction->wAction == ACTION_DEATH )
						iType = 2;
					else
						iType = 3;
				}
				if(iType > g_pControl->GetMouseOnType() || (iType == g_pControl->GetMouseOnType() &&  iTexOrignH < g_pControl->GetMouseOnHeight() + 32))
				{
					g_pControl->SetMouseOnID(uID);
					g_pControl->SetMouseOnType(iType);
					g_pControl->SetMouseOnHeight(iTexOrignH);
				}
            }
        }
    }


    //画从头上冒出来的经验及其它
    //DrawBubbleUp(pChar);
}

void CSceneManager::DrawNpc(int iX,int iY,CSimpleCharacter * pChar,BOOL bNotAlpha)
{
    if( !g_pGameData || !pChar ) 
        return;

    // 临时变量
    LPTexture		pTex = NULL;
    int				x,y;
    // 角色信息
    UINT			uID;					// ID
    DWORD			dwStatus;				// 状态
    SAction		*	pAction;				// 当前动作

    // 声音
    UINT			lSoundID = 0;

    memset(&m_DrawInf,0,sizeof(m_DrawInf));

    uID					= pChar->GetID();
    pAction	= &(pChar->GetAction());
    dwStatus			= pChar->GetStatus();

    // 为了防止移动过程中变动方向
    if(pAction->IsMove())
        m_DrawInf.iDir	= pAction->cDir;
    else
        m_DrawInf.iDir	= pChar->GetDir();

    m_DrawInf.pLooks = &(pChar->GetLooks());
	m_DrawInf.pLast = pChar->GetLastTex();
    // 调整非法方向
    m_DrawInf.iDir %= 3;

    // 画图位置
    x = iX - 96;		// 32-128
    y = iY - 132;		// 16-148

    switch(pChar->GetRaceNo())
    {
    case 36:
        x -= 10;
        y -= 96;
        break;
    case 53:
        x -= 130;
        y -= 100;
        break;
    case 166:
    case 167:
    case 168:
    case 169:
    case 170:
    case 171:
        x -= 100;
        y -= 200;
        break;
    case 174:
        x -= 60;
        y -= 186;
        break;
    case 175:
        y -= 165;
        break;
    case 176:
        x += 32;
        y -= 54;
        break;
    case 46:
        x = iX - 290;
        y = iY - 420;
        break;
    }

    // 绘图颜色
    DWORD dwColor = COLOR_DEFAULT;
    if(bNotAlpha)
        m_DrawInf.dwColor	= dwColor;
    else
        m_DrawInf.dwColor   = ColorMultiply(dwColor,0x50999999);

    m_DrawInf.iAction	= pAction->wAction;
    m_DrawInf.iFrame	= pAction->iFrameNow;
	m_DrawInf.iActionFrames	= pAction->iFrameCount;
    m_DrawInf.bNeedRt	= bNotAlpha;
    m_DrawInf.bNotAlpha = bNotAlpha;
    int iRaceNo = pChar->GetRaceNo();

    //改变帧数等
    switch(iRaceNo)
    {
    case  17://圣诞树入口
        if(pAction->iFrameCount > 4 )
            pAction->iFrameCount = 4;
        m_DrawInf.iDir = 0;
        break;
    case  25://///幻魔境入口
        if( pAction->iFrameCount < 10 )
        {
            pAction->iFrameCount		= 10;
            pAction->iFrameSpeed	/= 2;
        }
        m_DrawInf.iDir = 0;
        break;
    case  33:	// 魔法阵
        if( pAction->iFrameCount < 10 )
        {
            pAction->iFrameCount		= 10;
            pAction->iFrameSpeed	/= 2;
        }
        m_DrawInf.iDir = 0;
        break;
    case 43:   // 紫金神炉
        if(pAction->iFrameCount < 8)
        {
            pAction->iFrameCount = 8;
        }
        m_DrawInf.iDir = 0;
        break;
    case 44:   // 江湖公告
    case 160:  // 天碑（草地）
    case 161:  // 天碑（石头）
    case 163:  // 箱子
    case 172:  // 1.908商城道具 黄榜大旗旗墩
    case 173:  // 1.908商城道具 黄榜大旗旗
    case 174:  // 1.908商城道具 黄榜大旗大旗
    case 175:  // 1.908商城道具 黄榜大旗华盖天罗旗
    case 176:  // 1.908商城道具 黄榜大旗玄天功德牌
	case 177:  // 天下第一行会
	case 178://神_男战,
	case 179://神_女战
	case 180://神_男法,
	case 181://神_女法
	case 182://神_男道,
	case 183://神_女道
	case 184://魔_男战,
	case 185://魔_女战
	case 186://魔_男法,
	case 187://魔_女法
	case 188://魔_男道,
	case 189://魔_女道	
		pAction->iFrameCount = 1;
        m_DrawInf.iFrame = 0;
        m_DrawInf.iDir = 0;
        break;
    case 45:   // 中心广场
        if(pAction->iFrameCount < 20)
        {
            pAction->iFrameCount = 20;
            pAction->iFrameSpeed /= 2;
        }
        if(m_DrawInf.iFrame>=10)
            m_DrawInf.iFrame = 19-m_DrawInf.iFrame;

        x = iX + 32 - 155;
        y = iY + 16 - 432 - m_DrawInf.iFrame*6;
        m_DrawInf.iFrame = 0;
        m_DrawInf.iDir = 0;
        break;
    case 151: //神秘矿石(任务相关)
        pAction->iFrameCount = 6;
        m_DrawInf.iDir = 0;
        break;
    case 157: //石箱子(任务相关)
        pAction->iFrameCount = 1;
        m_DrawInf.iDir = 0;
        break;
    case 158:
    case 159:
        if(pAction->iFrameCount < 15)
        {
            pAction->iFrameCount = 15;
            pAction->iFrameSpeed /= 2;
        }
        m_DrawInf.iDir = 0;
        break;
    case 165://意识海 灵台仙人
        if( pAction->iFrameCount < 12 )
            pAction->iFrameCount = 12;

        x += 16;
        m_DrawInf.iDir = 0;
        break;
    case 46:
        if(pAction->iFrameCount > 1)
            pAction->iFrameCount = 1;
        m_DrawInf.iFrame = 0;
        m_DrawInf.iDir = 0;
        break;
    case 53:
        pAction->iFrameCount = 25;
        m_DrawInf.iFrame = 0;
        m_DrawInf.iDir = 0;
		break;
	case 57:
		m_DrawInf.iDir = 2;
		break;
	case 190:
		m_DrawInf.iDir = 0;
		m_DrawInf.iFrame = 0;
		break;
	case 193:
	case 194:
	case 195:
	case 196:
		m_DrawInf.iDir = 0;
		if(pAction->iFrameCount < 10 )
			pAction->iFrameCount = 10;
		break;
	case 197:
	case 198:
	case 199:
	case 200:
		m_DrawInf.iDir = 0;
		break;
	case 201:
	case 202:
		m_DrawInf.iDir = 0;
		pAction->iFrameCount = 34;
		pAction->iFrameSpeed = 4;
		if(m_DrawInf.iFrame >= 17)
		{
			m_DrawInf.iFrame = 34 - m_DrawInf.iFrame;
		}
		break;
	case 247:
		{
			pAction->iFrameCount = 9;
		}
		break;
	case 257://宝儿
		{
			if (m_DrawInf.iAction == ACTION_STAND)
			{
				pAction->iFrameCount = 6;
			}
			else if (m_DrawInf.iAction == ACTION_STAND3)
			{
				pAction->iFrameCount = 12;
			}

			if(m_DrawInf.iFrame > pAction->iFrameCount) 
				m_DrawInf.iFrame = 0;
		}
		break;
	case 258://炼火炉
		pAction->iFrameCount = 12;
		m_DrawInf.iFrame = 0;
		m_DrawInf.iDir = 0;
		break;
    default:
		if (m_DrawInf.iAction == ACTION_STAND2)
		{
			if(m_DrawInf.iFrame > 7) m_DrawInf.iFrame = 0;
		}
		else if(m_DrawInf.iFrame > 3)
            m_DrawInf.iFrame = 6 - m_DrawInf.iFrame;
        break;
    }

	//24种摊位
	if (iRaceNo >= 203 && iRaceNo <= 226)
	{
		pAction->iFrameCount = 1;
		m_DrawInf.iFrame = 0;
		m_DrawInf.iDir = 0;
	}

    if(DrawLooks(x,y,&m_DrawInf))		// 画成功
    {
        if(bNotAlpha)
        {
            //特殊NPC的特效
            LPTexture pTex3 = NULL;
            switch(iRaceNo)
            {
            case 45:
                pTex3 = g_pTexMgr->GetTex(PACKAGE_npc1,9002+m_DrawInf.iFrame,EP_MAGIC);
                g_pGfx->SetRenderMode(RM_ADD2);
                g_pGfx->DrawTextureNL(x-80,y,pTex3);
                g_pGfx->SetRenderMode();
                break;
            case 46:
                pTex3 = g_pTexMgr->GetTex(PACKAGE_npc1,9225,EP_MAGIC);
                g_pGfx->DrawTextureNL(x + 20,y,pTex3);
                break;
            case 165:
                pTex3 = g_pTexMgr->GetTex(PACKAGE_npc1,33012+m_DrawInf.iFrame,EP_MAGIC);
                g_pGfx->SetRenderMode(RM_ADD2);
                g_pGfx->DrawTextureNL(x,y,pTex3);
                g_pGfx->SetRenderMode();
                break;
            case 53:
                pTex3 = g_pTexMgr->GetTex(PACKAGE_npc1,10602 + pAction->iFrameNow,EP_MAGIC);
                g_pGfx->SetRenderMode(RM_ADD1);
                g_pGfx->DrawTextureNL(x + 10,y - 240,pTex3);
                g_pGfx->SetRenderMode();
                break;
			case 190:
				pTex3 = g_pTexMgr->GetTex(PACKAGE_npc1,(pChar->GetDir() == 1)? 38002:38003,EP_MAGIC);
				g_pGfx->SetRenderMode(RM_ADD1);
				g_pGfx->DrawTextureNL(x,y,pTex3);
				g_pGfx->SetRenderMode();
				break;
			case 258://炼火炉
				pTex3 = g_pTexMgr->GetTex(PACKAGE_npc1,51602 + pAction->iFrameNow,EP_NPC);
				g_pGfx->SetRenderMode(RM_ADD1);
				g_pGfx->DrawTextureNL(x - 171,y - 150,pTex3);
				g_pGfx->SetRenderMode();
				break;
            }

            //取得信息
			int iTexOrignH = 120;
            int iNpc = iRaceNo;
            LPTexture pTex = g_pTexMgr->GetTexFromID(g_ActionInf.GetNPCTex(iNpc,ACTION_STAND,0,m_DrawInf.iDir),EP_NPC);
            if(pTex)
            {
                int iTexW = pTex->GetWidth();
                int iTexH = pTex->GetHeight();
                int iTexOffX = pTex->GetOffX();
                int iTexOffY = pTex->GetOffY();
				int x0	= x - pTex->GetCenterX() + iTexOffX;
				x0  += iTexW / 2;
                int	y0	= y - pTex->GetCenterY() + iTexOffY;

                pChar->SetTopXY(iX + 32,y0 - 6);
				//pChar->SetTopXY(x0 + 14,y0 - 6);
                pChar->SetTexWH(iTexW,iTexH);
				iTexOrignH = pTex->GetHeight0();
            }
            else
            {
				pChar->SetTopXY(iX + 32,iY + 26 - 104);
				pChar->SetTexWH(64,104);
            }


			if(m_DrawInf.iMouseIn == 2 || (m_DrawInf.iMouseIn > 0 && iTexOrignH < 120))
            {
				int iType;
				if(m_DrawInf.iMouseIn == 2)
				{
					iType = 13;
				}
				else
				{
					iType = 3;
				}

				if(iType > g_pControl->GetMouseOnType() || (iType == g_pControl->GetMouseOnType() &&  iTexOrignH < g_pControl->GetMouseOnHeight() + 32))
				{
					g_pControl->SetMouseOnID(uID);
					g_pControl->SetMouseOnType(iType);
					g_pControl->SetMouseOnHeight(iTexOrignH);
				}

                if(g_AICfgMgr.IsShowNpcPriority())
                {
                    m_bMouseInNpc = true;
                }
            }
        }

        //画黄榜大旗,黄榜大大旗,华盖天罗旗,玄天功德牌上面的文字
        if(iRaceNo >= 173 && iRaceNo <= 176)
            DrawFlagText(iX,iY,pChar);
    }
}

void CSceneManager::DrawSimpleGood(int iX,int iY,CSimpleGood* pGood)
{
    if(!pGood)
        return;

    if(g_AICfgMgr.IsShowBestItem() && (pGood->GetShowType() & ITEMSHOWTYPE_BEST) == 0) //只显示极品
        return;

    int x,y;

    LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_dnitems,pGood->GetLooks(),EP_UI);
    if(pTex) 
    {
        x = iX + (TILE_WIDTH-pTex->GetWidth0()) / 2;
        y = iY + (TILE_HEIGHT-pTex->GetHeight0()) /2;

        g_pGfx->DrawTextureNL(x,y,pTex);

        if(pTex->IsPointInTex(m_iMouseX - x,m_iMouseY - y))
        {
            g_pControl->SetMouseOnID(pGood->GetID());
        }
    }
}

void CSceneManager::Draw(bool bNeedRt)
{
	if (g_pGameMap)
		g_pGameMap->CheckLoadMap();


    if( !g_pGameMap || !g_pGameMap->IsEnable() || !g_pGameMap->GetMap() || g_pGameMap->IsEmptyMap()) 
        return;	

    m_bNeedRt = bNeedRt;

	PrepareDraw();

    //显示背景
    DrawBackground();

    //画地面的大地表、小地表和单格子物体
    DrawGround();

    //画物体，人物
    DrawOverground();

    //画光照
    //DrawLight();

    // 渲染光照贴图
    //g_pLight->RenderLight();

    if(SELF.IsDead()) //死亡以后的遮罩
    {
        g_pGfx->SetRenderMode(RM_MODULATE);
        g_pGfx->DrawFillRect(0,0,m_iScrWidth,m_iScrHeight,0x60FFFF80);
        g_pGfx->SetRenderMode();
    }

    // 显示血条
    DrawOverlay();

	DrawLianJiNumber();

	if(g_Config.IsShowBlock())
	{
		SMapTile * sMap = g_pGameMap->GetMap();
		for(int iTileY = -1; iTileY <= m_iTileHeight; iTileY++)
		{
			int iAbY = m_iTileStartY+iTileY;

			if(iAbY < 0 || iAbY >= m_iMapTileHeight)
				continue;

			int j = iAbY*m_iMapTileWidth + m_iTileStartX;

			for(int iTileX= -1; iTileX<= m_iTileWidth; iTileX++)
			{
				int iAbX = m_iTileStartX+iTileX;

				if(iAbX < 0 || iAbX >= m_iMapTileWidth)
					continue;

				int i = j + iTileX;

				int iDrawX = iTileX*TILE_WIDTH - m_iDrawOffX;
				int iDrawY = iTileY*TILE_HEIGHT - m_iDrawOffY;

				if(g_pGameMap->IsBlock(iAbX,iAbY) || SELF.IsOppBlock(iAbX,iAbY))
					g_pGfx->DrawFillRect(iDrawX,iDrawY,TILE_WIDTH,TILE_HEIGHT,0x88FF0000);
			}
		}
	}

    // 处理纹理预读
    DealCache();

// 	if (g_TrusteeshipData.IsCaptain() && g_TrusteeshipData.IsSelecting())//托管模式绘制队长选中框
// 	{
// 		RECT& rc = g_TrusteeshipData.GetSelRect();
// 		g_pGfx->DrawRect(rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, 0xFF00FF00,2);
// 	}
}

void CSceneManager::PrepareDraw()
{
    SELF.GetOffset(m_iOffX,m_iOffY);
    SELF.GetPlayerXY(m_iPlayerX,m_iPlayerY);

    if(m_iPlayerX < 0 && m_iPlayerY < 0)
        return;

    g_pControl->GetMouseXY(m_iMouseX,m_iMouseY);

    m_dwFrameCount = g_pGfx->GetFrameCount(); 

    SAction& ActioNow = SELF.GetAction();

    //计算所画Tile起始及结束位置
    m_iDrawOffX = m_iOffX % TILE_WIDTH;
    m_iDrawOffY = m_iOffY % TILE_HEIGHT;

	//if(g_pGfx->IsBig())
 //   {
 //       m_iScrX = 512;
 //       m_iScrY = 352;
 //       m_iTileWidth = 17;
 //       m_iTileHeight = 24;

 //       m_iTileStartX = m_iPlayerX - 8;	
 //       m_iTileStartY = m_iPlayerY - 11;
 //   }
 //   else
 //   {
 //       m_iScrX = 384;
 //       m_iScrY = 288;
 //       m_iTileWidth = 13;
 //       m_iTileHeight = 18;

 //       m_iTileStartX = m_iPlayerX - 6;	
 //       m_iTileStartY = m_iPlayerY - 9;
 //   }

	m_iScrX = (g_pGfx->GetWidth() + TILE_WIDTH - 1) / 2;
	m_iScrX -= m_iScrX%TILE_WIDTH;
	m_iScrY = (g_pGfx->GetHeight() + TILE_HEIGHT - 1) / 2;
	m_iScrY -= m_iScrY%TILE_HEIGHT;

	m_iTileWidth = (g_pGfx->GetWidth() + TILE_WIDTH - 1)/TILE_WIDTH;
	m_iTileHeight = (g_pGfx->GetHeight() + TILE_HEIGHT - 1)/TILE_HEIGHT;;

	m_iHalfTileWidth = m_iTileWidth / 2;
	m_iHalfTileHeight = m_iTileHeight / 2;

	m_iTileStartX = m_iPlayerX - m_iTileWidth / 2;	
	m_iTileStartY = m_iPlayerY - m_iTileHeight / 2;	

	m_iScrWidth = g_pGfx->GetWidth();
	m_iScrHeight = g_pGfx->GetHeight();


	m_iInDownloadFiles = g_pStreamMgr->GetDownloadingFiles();

	if (m_iInDownloadFiles > 0)
	{
		if(GetTickCount() - m_dwLastCheckDownLoadListTime > 2000)
		{
			int iRtn = g_pStreamMgr->CheckDownLoadList();
			m_dwLastCheckDownLoadListTime = GetTickCount();
			if (iRtn == 1)
			{
				g_bHasUiInDownloading = true;
			}
			else
			{
				g_bHasUiInDownloading = false;
			}
		}
	}
	else 
	{
		g_bHasUiInDownloading = false;
	}


    SELF.SetScrXY(m_iScrX,m_iScrY);

    int iMouseTileX = m_iTileStartX + (m_iMouseX + m_iDrawOffX) / TILE_WIDTH;
    int iMouseTileY = m_iTileStartY + (m_iMouseY + m_iDrawOffY) / TILE_HEIGHT;

    if(g_pGameMgr)
        g_pGameMgr->SetMouseTile(iMouseTileX,iMouseTileY);

    //进行震动
	DWORD dwCount = GetTickCount();

    m_iShakeX = m_iShakeY = 0;
    if(dwCount < g_AIMgr.GetReserveTime(plyEndShakeTime)) //需要震动
    {
        DWORD dwStartShakeTime = g_AIMgr.GetReserveTime(plyStartShakeTime);
        int   iShakeTime = (dwCount - dwStartShakeTime) % 200;

        if(iShakeTime < 100)
        {
            m_iShakeY = 8 * (50 - abs(iShakeTime - 50)) / 100;
            m_iShakeX = 2 * m_iShakeY;
        }
        else
        {
            m_iShakeY = 8 * (abs(iShakeTime - 150) - 50) / 100;
            m_iShakeX = 2 * m_iShakeY;
        }
    }
    m_iDrawOffX += m_iShakeX;
    m_iDrawOffY += m_iShakeY;

	//output_debug("%d,%d,%d,%d\r\n",m_iOffX,m_iOffY,m_iDrawOffX,m_iDrawOffY);

    //地图Tile尺寸
    m_iMapTileWidth = g_pGameMap->GetWidth();
    m_iMapTileHeight = g_pGameMap->GetHeight();

    if(g_pMagicMgr)
        g_pMagicMgr->DealMagic();

    m_bMouseInNpc = false;

	//动态场景
	//VSDynRecover& vDynRev = g_pGameData->GetVDynRecover();
	//DWORD dwTime = GetTickCount();
	//for (int i = 0;i<vDynRev.size();i++)
	//{
	//	if(dwTime - vDynRev[i].dwTime >= 3000)
	//	{
	//		g_pGameMap->DelDynObject(vDynRev[i].dwID,vDynRev[i].wStartX,vDynRev[i].wStartY);//恢复
	//		vDynRev.erase(vDynRev.begin() + i);
	//		break;
	//	}
	//}
}

void CSceneManager::DealCache()
{
    SMapTile * sMap = g_pGameMap->GetMap();
    if(!sMap) //没有地图
        return;

    if(m_iPlayerX < 0 && m_iPlayerY < 0)//输入的数据非法
        return;

    int iDeltaX = labs(m_iPlayerX - m_iLastX);
    int iDeltaY = labs(m_iPlayerY - m_iLastY);

	int iMinCacheBlocks = 2;
	if (m_iInDownloadFiles > MAX_ENABLE_INDOWNLOAD_FILES)//如果还有很多图在下,cache的范围小,所以移动一格就要处理预读了
	{
		iMinCacheBlocks = 1;
	}

    if(iDeltaX < iMinCacheBlocks && iDeltaY < iMinCacheBlocks && m_sLastMap == sMap) //同一地图，移动的距离只有一格或者不移动
    {
        //进行赋值
        //m_iLastX = m_iPlayerX;
        //m_iLastY = m_iPlayerY;
        return;
    }

    int iCacheTileX = m_iHalfTileWidth + TEXCACHE_TILEX;
    int iCacheTileY = m_iHalfTileHeight + TEXCACHE_TILEY;

    // 转跳或新登陆的情况
    if(iDeltaX > iCacheTileX || iDeltaY > iCacheTileY || m_sLastMap != sMap)
    {
        int iStartX = m_iPlayerX - iCacheTileX;
        int iEndX   = m_iPlayerX + iCacheTileX;
        int iStartY = m_iPlayerY - iCacheTileY;
        int iEndY   = m_iPlayerY + iCacheTileY;

        if(iStartX <  0 ) iStartX = 0;
        if(iStartY <  0 ) iStartY = 0;
        if(iEndX >= m_iMapTileWidth)
            iEndX = m_iMapTileWidth - 1;
        if(iEndY >= m_iMapTileHeight)
            iEndY = m_iMapTileHeight - 1;

        for(int iY = iStartY; iY <= iEndY; iY++)
        {
            SMapTile* pTile = &sMap[iY*m_iMapTileWidth + iStartX];

            for(int iX = iStartX; iX <= iEndX; iX++)
            {
                if(pTile->dwTile > 0)
				{
					//如果还有东西等着下载,超过很多的预读的时候就不要下载
					if (m_iInDownloadFiles > MAX_ENABLE_INDOWNLOAD_FILES && (abs(iX - m_iPlayerX) > m_iHalfTileWidth + DOWNLOAD_CACHE_X + 4 || abs(iY - m_iPlayerY) > m_iHalfTileHeight + DOWNLOAD_CACHE_Y + 4))
					{
						g_pTexMgr->GetTexFromID(pTile->dwTile,EP_DONT_DOWNLOAD);
					}
					else
					{
						g_pTexMgr->GetTexFromID(pTile->dwTile,EP_TILES);
					}
				}

                if(pTile->dwSmTile > 0)
				{
					//如果还有东西等着下载,超过很多的预读的时候就不要下载
					if (m_iInDownloadFiles > MAX_ENABLE_INDOWNLOAD_FILES && (abs(iX - m_iPlayerX) > m_iHalfTileWidth + DOWNLOAD_CACHE_X || abs(iY - m_iPlayerY) > m_iHalfTileHeight + DOWNLOAD_CACHE_Y))
					{
						g_pTexMgr->GetTexFromID(pTile->dwSmTile,EP_DONT_DOWNLOAD);
					}
					else
					{
						g_pTexMgr->GetTexFromID(pTile->dwSmTile,EP_SMTILES);
					}
				}

                pTile++;
            }
        }

        //物体层
        iEndY += 10;
        if( iEndY >= m_iMapTileHeight)
            iEndY = m_iMapTileHeight - 1;

        for(int iY = iStartY; iY <= iEndY; iY++)
        {
            SMapTile* pTile = &sMap[iY*m_iMapTileWidth + iStartX];
            for(int iX = iStartX; iX <= iEndX; iX++)
            {
                if((pTile->uObject & OT_OBJECT_MASK) > 0)
                {
					//如果还有东西等着下载,超过很多的预读的时候就不要下载
					if ((m_iInDownloadFiles > MAX_ENABLE_INDOWNLOAD_FILES && (abs(iX - m_iPlayerX) > m_iHalfTileWidth + DOWNLOAD_CACHE_X || abs(iY - m_iPlayerY) > m_iHalfTileHeight + DOWNLOAD_CACHE_Y))
						|| (abs(iX - m_iPlayerX) > m_iHalfTileWidth + TEXCACHE_TILEX || abs(iY - m_iPlayerY) > m_iHalfTileHeight + TEXCACHE_TILEY)
						)
					{
						g_pTexMgr->GetTexFromID(pTile->uObject & OT_OBJECT_MASK,EP_DONT_DOWNLOAD);
					}
					else
					{
						g_pTexMgr->GetTexFromID(pTile->uObject & OT_OBJECT_MASK,EP_OBJECT);
					}
                }
                pTile++;
            }
        }
    }
    else
    {
        int iStartX,iEndX,iStartY,iEndY;

        // 竖向的块
        if(m_iPlayerX != m_iLastX )
        {
            // tiles
            if(m_iPlayerX < m_iLastX )
            {
                iStartX = m_iPlayerX - iCacheTileX;
                iEndX = iStartX + m_iLastX - m_iPlayerX;
            }
            else
            {
                iStartX = m_iLastX + iCacheTileX;
                iEndX = iStartX + m_iPlayerX - m_iLastX;
            }

            if(m_iPlayerY < m_iLastY )
            {
                iStartY = m_iLastY - iCacheTileY;
                iEndY = m_iPlayerY + iCacheTileY;
            }
            else
            {
                iStartY = m_iPlayerY - iCacheTileY;
                iEndY = m_iLastY + iCacheTileY;
            }

            if( iStartX <  0 ) iStartX = 0;
            if( iStartY <  0 ) iStartY = 0;
            if(	iEndX >= m_iMapTileWidth)
                iEndX = m_iMapTileWidth - 1;
            if( iEndY >= m_iMapTileHeight)
                iEndY = m_iMapTileHeight - 1;

            for(int iY = iStartY; iY <= iEndY; iY++)
            {
                SMapTile* pTile = &sMap[iY*m_iMapTileWidth + iStartX];
                for(int iX = iStartX; iX <= iEndX; iX++)
                {
					if(pTile->dwTile > 0)
					{
						//如果还有东西等着下载,超过很多的预读的时候就不要下载
						if (m_iInDownloadFiles > MAX_ENABLE_INDOWNLOAD_FILES && (abs(iX - m_iPlayerX) > m_iHalfTileWidth + DOWNLOAD_CACHE_X + 4 || abs(iY - m_iPlayerY) > m_iHalfTileHeight + DOWNLOAD_CACHE_Y + 4))
						{
							g_pTexMgr->GetTexFromID(pTile->dwTile,EP_DONT_DOWNLOAD);
						}
						else
						{
							g_pTexMgr->GetTexFromID(pTile->dwTile,EP_TILES);
						}
					}

					if(pTile->dwSmTile > 0)
					{
						//如果还有东西等着下载,超过很多的预读的时候就不要下载
						if (m_iInDownloadFiles > MAX_ENABLE_INDOWNLOAD_FILES && (abs(iX - m_iPlayerX) > m_iHalfTileWidth + DOWNLOAD_CACHE_X || abs(iY - m_iPlayerY) > m_iHalfTileHeight + DOWNLOAD_CACHE_Y))
						{
							g_pTexMgr->GetTexFromID(pTile->dwSmTile,EP_DONT_DOWNLOAD);
						}
						else
						{
							g_pTexMgr->GetTexFromID(pTile->dwSmTile,EP_SMTILES);
						}
					}


                    pTile++;
                }
            }	

            //物体层
            iEndY += 10;
            if( iEndY >= m_iMapTileHeight)
                iEndY = m_iMapTileHeight - 1;

            for(int iY = iStartY; iY <= iEndY; iY++)
            {
                SMapTile* pTile = &sMap[iY*m_iMapTileWidth + iStartX];
                for(int iX = iStartX; iX <= iEndX; iX++)
                {
                    if((pTile->uObject & OT_OBJECT_MASK) > 0)
                    {
						//如果还有东西等着下载,超过很多的预读的时候就不要下载
						if ((m_iInDownloadFiles > MAX_ENABLE_INDOWNLOAD_FILES && (abs(iX - m_iPlayerX) > m_iHalfTileWidth + DOWNLOAD_CACHE_X || abs(iY - m_iPlayerY) > m_iHalfTileHeight + DOWNLOAD_CACHE_Y))
							|| (abs(iX - m_iPlayerX) > m_iHalfTileWidth + TEXCACHE_TILEX || abs(iY - m_iPlayerY) > m_iHalfTileHeight + TEXCACHE_TILEY)
							)
						{
							g_pTexMgr->GetTexFromID(pTile->uObject & OT_OBJECT_MASK,EP_DONT_DOWNLOAD);
						}
						else
						{
							g_pTexMgr->GetTexFromID(pTile->uObject & OT_OBJECT_MASK,EP_OBJECT);
						}
                    }
                    pTile++;
                }
            }
        }

        // 横向的块
        if( m_iPlayerY != m_iLastY )
        {
            // tiles
            iStartX = m_iPlayerX - iCacheTileX;
            iEndX = m_iPlayerX + iCacheTileX;
            if(m_iPlayerY < m_iLastY)
            {
                iStartY = m_iPlayerY - iCacheTileY;
                iEndY = iStartY + m_iLastY - m_iPlayerY;
            }
            else
            {
                iStartY = m_iLastY + iCacheTileY;
                iEndY = iStartY + m_iPlayerY - m_iLastY;
            }

            if( iStartX <  0 ) iStartX = 0;
            if( iStartY <  0 ) iStartY = 0;
            if( iEndX >= m_iMapTileWidth)
                iEndX = m_iMapTileWidth - 1;
            if( iEndY >= m_iMapTileHeight)
                iEndY = m_iMapTileHeight - 1;

            for(int iY = iStartY; iY <= iEndY; iY++)
            {
                SMapTile* pTile = &sMap[iY*m_iMapTileWidth + iStartX];
                for(int iX = iStartX; iX <= iEndX; iX++)
                {
					if(pTile->dwTile > 0)
					{
						//如果还有东西等着下载,超过很多的预读的时候就不要下载
						if (m_iInDownloadFiles > MAX_ENABLE_INDOWNLOAD_FILES && (abs(iX - m_iPlayerX) > m_iHalfTileWidth + DOWNLOAD_CACHE_X + 4 || abs(iY - m_iPlayerY) > m_iHalfTileHeight + DOWNLOAD_CACHE_Y + 4))
						{
							g_pTexMgr->GetTexFromID(pTile->dwTile,EP_DONT_DOWNLOAD);
						}
						else
						{
							g_pTexMgr->GetTexFromID(pTile->dwTile,EP_TILES);
						}
					}

					if(pTile->dwSmTile > 0)
					{
						//如果还有东西等着下载,超过很多的预读的时候就不要下载
						if (m_iInDownloadFiles > MAX_ENABLE_INDOWNLOAD_FILES && (abs(iX - m_iPlayerX) > m_iHalfTileWidth + DOWNLOAD_CACHE_X || abs(iY - m_iPlayerY) > m_iHalfTileHeight + DOWNLOAD_CACHE_Y))
						{
							g_pTexMgr->GetTexFromID(pTile->dwSmTile,EP_DONT_DOWNLOAD);
						}
						else
						{
							g_pTexMgr->GetTexFromID(pTile->dwSmTile,EP_SMTILES);
						}
					}

                    pTile++;
                }
            }

            // objects
            iStartY += 10;
            iEndY += 10;
            if( iStartY >= m_iMapTileHeight ) iStartY = m_iMapTileHeight - 1;
            if( iEndY >= m_iMapTileHeight ) iEndY = m_iMapTileHeight - 1;

            for(int iY = iStartY; iY <= iEndY; iY++)
            {
                SMapTile* pTile = &sMap[iY*m_iMapTileWidth + iStartX];
                for(int iX = iStartX; iX <= iEndX; iX++)
                {
                    if((pTile->uObject & OT_OBJECT_MASK) > 0)
                    {
						//如果还有东西等着下载,超过很多的预读的时候就不要下载
						if ((m_iInDownloadFiles > MAX_ENABLE_INDOWNLOAD_FILES && (abs(iX - m_iPlayerX) > m_iHalfTileWidth + DOWNLOAD_CACHE_X || abs(iY - m_iPlayerY) > m_iHalfTileHeight + DOWNLOAD_CACHE_Y))
							|| (abs(iX - m_iPlayerX) > m_iHalfTileWidth + TEXCACHE_TILEX || abs(iY - m_iPlayerY) > m_iHalfTileHeight + TEXCACHE_TILEY)
							)
						{
							g_pTexMgr->GetTexFromID(pTile->uObject & OT_OBJECT_MASK,EP_DONT_DOWNLOAD);
						}
						else
						{
							g_pTexMgr->GetTexFromID(pTile->uObject & OT_OBJECT_MASK,EP_OBJECT);
						}
                    }

                    pTile++;
                }
            }
        }
    }

    //进行赋值
    m_iLastX = m_iPlayerX;
    m_iLastY = m_iPlayerY;
    m_sLastMap = sMap;
}


bool CSceneManager::BGLayerTileIterator(SMapTile* pTile,int iX,int iY,int sx,int sy)
{
    if( pTile->dwTile == 0)
        return true;

    LPTexture pTex = g_pTexMgr->GetTexFromID(pTile->dwTile,EP_TILES);

    if( pTex)
        g_pGfx->DrawTextureFX(sx,sy,pTex);
    return true;
}

bool CSceneManager::BGLayerSmTileIterator(SMapTile* pTile,int iX,int iY,int sx,int sy)
{
    if(pTile->dwSmTile == 0)
        return true;

    LPTexture pTex = g_pTexMgr->GetTexFromID(pTile->dwSmTile,EP_SMTILES);

    if( pTex)
    {
        g_pGfx->DrawTextureFX(sx,sy,pTex);
    }
    return true;
}

bool CSceneManager::BGLayerObjectIterator(SMapTile* pTile,int iX,int iY,int sx,int sy)
{
    if(pTile->uObject == 0)
        return true;

    LPTexture pTex = NULL;
	if ((m_iInDownloadFiles > MAX_ENABLE_INDOWNLOAD_FILES && (abs(iX - m_iPlayerX) > m_iHalfTileWidth + DOWNLOAD_CACHE_X || abs(iY - m_iPlayerY) > m_iHalfTileHeight + DOWNLOAD_CACHE_Y))
		|| (abs(iX - m_iPlayerX) > m_iHalfTileWidth + TEXCACHE_TILEX || abs(iY - m_iPlayerY) > m_iHalfTileHeight + TEXCACHE_TILEY)
		)
	{
		pTex = g_pTexMgr->GetTexFromID(pTile->uObject,EP_DONT_DOWNLOAD);
	}
	else
	{
		pTex = g_pTexMgr->GetTexFromID(pTile->uObject,EP_OBJECT);
	}

    if(pTex)
	{
		DWORD dwID = pTex->GetID();
		if((( (dwID & 0xFFFF0000) >> 16) == PACKAGE_objects18) || (dwID >= 0x00230310 && dwID <= 0x0023ffff))
		{
			g_pGfx->SetRenderMode(RM_ADD1);
			g_pGfx->DrawTextureNL(sx,sy+TILE_HEIGHT-pTex->GetHeight0(),pTex);
			g_pGfx->SetRenderMode();
		}
		else if (( ((dwID & 0xFFFF0000) >> 16) == PACKAGE_objects19) || (dwID >= 0x002c0161 && dwID <= 0x002c0169))
		{
			g_pGfx->SetRenderMode(RM_ADD2);
			g_pGfx->DrawTextureNL(sx,sy+TILE_HEIGHT-pTex->GetHeight0(),pTex);
			g_pGfx->SetRenderMode();
		}
		else
		{
			g_pGfx->DrawTextureNL(sx,sy+TILE_HEIGHT-pTex->GetHeight0(),pTex);
		}
	}
    return true;
}

void CSceneManager::DrawBackground()
{
    if(g_pGfx->IsNoDraw())
        return;

    MBGLayer& BGLayers = g_pGameMap->GetBgLayers(); 
    for(MBGLayer::iterator itr = BGLayers.begin();itr != BGLayers.end();itr++)
    {
        BGLayerData& bgLayer = itr->second;
        if(bgLayer.iWidth == 0 || bgLayer.iHeight == 0)
            continue;

        BGStartAndOffset(bgLayer);
 
		IterateBGByXY(bgLayer,-2,-4,0,1,&CSceneManager::BGLayerTileIterator); 	//大地砖 
        IterateBGByXY(bgLayer,-2,-4,0,1,&CSceneManager::BGLayerSmTileIterator);     //小地砖
        IterateBGByXY(bgLayer,-1,2,0,0,&CSceneManager::BGLayerObjectIterator);    //物体

		////灵犀仙岛 和 迷仙阵的 背景层 上需要有云雾效果
		if(!(g_Config.GetWeather() && g_pWeather->GetParticleType() != PARTICLE_NONE))
		{
			if(/*strnicmp(g_pGameMap->GetMapName(),"lxxd",4) == 0 || */strnicmp(g_pGameMap->GetMapName(),"MAZE",4) == 0)
			{
				g_pWeather->SetDensity(-30);
				g_pWeather->DrawCloud(bgLayer.iBGStartX,bgLayer.iBGStartY,-bgLayer.iBGOffX,-bgLayer.iBGOffY,-1);			
			}
		}		
    }
}

void CSceneManager::BGStartAndOffset(BGLayerData& bgLayer)
{
    int iStartScrX =m_iTileStartX * TILE_WIDTH + m_iDrawOffX;
    int iStartScrY =m_iTileStartY * TILE_HEIGHT + m_iDrawOffY;

    iStartScrX  += m_iScrX;  //中心点X
    iStartScrY  += m_iScrY; //中心点Y

    //为了防止在绘制四边时背景地图出现黑边，背景地图都做了扩展，左右各加10个格子，上下各加15个格子，即bgLayer.iTileOffX，bgLayer.iTileOffY
	float fRateW = FLOAT(bgLayer.iWidth - bgLayer.iTileOffX*2) / m_iMapTileWidth;
	float fRateH = FLOAT(bgLayer.iHeight - bgLayer.iTileOffY*2) / m_iMapTileHeight;
	int iBGScrX =  (int)(iStartScrX * fRateW) + bgLayer.iTileOffX*TILE_WIDTH;
	int iBGScrY =  (int)(iStartScrY * fRateH) + bgLayer.iTileOffY*TILE_HEIGHT;

	bgLayer.iBGStartX   = iBGScrX / TILE_WIDTH - (int)(m_iScrX * fRateW) / TILE_WIDTH;
	bgLayer.iBGStartY   = iBGScrY / TILE_HEIGHT - (int)(m_iScrY * fRateW) / TILE_HEIGHT;

    bgLayer.iBGOffX     = iBGScrX % TILE_WIDTH + bgLayer.iOffX;
    bgLayer.iBGOffY     = iBGScrY % TILE_HEIGHT + bgLayer.iOffY;
}

void CSceneManager::IterateByXY(int x0,int y0,int x1,int y1,SceneIterator itr)
{
    if(itr == NULL)
        return;

    SMapTile * sMap = g_pGameMap->GetMap();

    for(int iTileY = y0; iTileY <= m_iTileHeight+y1; iTileY++)
    {
        int iAbY = m_iTileStartY+iTileY;

        if(iAbY < 0 || iAbY >= m_iMapTileHeight)
            continue;

        int j = iAbY*m_iMapTileWidth + m_iTileStartX;

        for(int iTileX= x0; iTileX<= m_iTileWidth+x1; iTileX++)
        {
            int iAbX = m_iTileStartX+iTileX;

            if(iAbX < 0 || iAbX >= m_iMapTileWidth)
                continue;

            int i = j + iTileX;

            int iDrawX = iTileX*TILE_WIDTH - m_iDrawOffX;
            int iDrawY = iTileY*TILE_HEIGHT - m_iDrawOffY;

            if(!(this->*itr)(&sMap[i],iAbX,iAbY,iDrawX,iDrawY))
                return;
        }
    }
}

void CSceneManager::IterateBGByXY(BGLayerData& bgLayer,int x0,int y0,int x1,int y1,SceneIterator itr)
{
    if(itr == NULL)
        return;

    SMapTile* pBgMap = bgLayer.pDataArray;
    for(int iTileY = y0;iTileY <= m_iTileHeight+y1;iTileY++)
    {
        int iAbY = bgLayer.iBGStartY+iTileY;

        if(iAbY < 0 || iAbY >= bgLayer.iHeight)
            continue;

        int j = iAbY*bgLayer.iWidth + bgLayer.iBGStartX;

        for(int iTileX = x0;iTileX <= m_iTileWidth + x1;iTileX++)
        {
            int iAbX = bgLayer.iBGStartX+iTileX;

            if(iAbX < 0 || iAbX >= bgLayer.iWidth)
                continue;

            SMapTile *pBgTile = &pBgMap[j + iTileX];

            if(pBgTile == NULL)
                continue;

            int iDrawX = iTileX*TILE_WIDTH - bgLayer.iBGOffX;
            int iDrawY = iTileY*TILE_HEIGHT - bgLayer.iBGOffY;

            if(!(this->*itr)(pBgTile,iAbX,iAbY,iDrawX,iDrawY))
                return;
        }
    }
}

bool CSceneManager::GroundEffIterator(SMapTile* pTile,int iX,int iY,int sx,int sy)
{
    switch(pTile->uObject & OT_TYPE_MASK)
    {
    case OT_WATERTIDE:				// 动态水效果
        if(pTile->dwTile != 0)
        {
            LPTexture pTex0 = 0,pTex1 = 0,pTex2 = 0;
            pTex1 = g_pTexMgr->GetTexFromID(pTile->dwTile,EP_OBJECT);				// 海水

            int iCoast = pTile->uObject & 0x000000FF;
            if(iCoast > 0)
                pTex0 = g_pTexMgr->GetTex(PACKAGE_effect,iCoast,EP_OBJECT);		// 海岸

            int iMask = (pTile->uObject >> 8) & 0x000000FF;
            if(iMask > 0)
                pTex2 = g_pTexMgr->GetTex(PACKAGE_effect,iMask,EP_OBJECT);		// 掩码

            if(pTex0 && pTex1 && pTex2)
            {
                g_pGfx->DrawTextureNL(sx,sy,pTex0);
                //g_pGfx->DrawTextureNL(sx,sy,pTex2);
                g_pGfx->DrawTextureTL(sx,sy,pTex1,pTex2);
            }
            //else if(pTex1)
            //{
            //    g_pGfx->DrawTextureNL(sx,sy,pTex1);
            //}

            //海浪发生器
            BYTE c = (BYTE)((pTile->uObject >> 16) & 0x000000FF);
            if((c & SEAMASK_HASTIDE) != 0 && (c & SEAMASK_ON) == 0)
            {
                int fre = (((c>>3) & 0x03)+1) * 100;
                if( (rand()%1000) <= fre)
                {
                    int ax = rand() % 128;
                    int ay = rand() % 64;
                    g_pWeather->GetSeaTideList()->NewTide(((c&SEAMASK_TYPE)?1:0), 50+(rand()%150), iX*64-ax/2+ax,iY*32-ay/2+ay, (c&SEAMASK_DIR),iX,iY);
                    pTile->uObject |= 0x00200000;
                }
            }
        }
        break;
    case OT_LIGHTCIRCLE:				// 火把光晕效果
        {
            BYTE  cType	= (BYTE)(pTile->uObject>> 20 & 0x0000000F);		// 光晕类型
            BYTE  cColor	= (BYTE)(pTile->uObject>> 16 & 0x0000000F);		// 光晕颜色
            DWORD dwID  = MAKELONG(801+cType,PACKAGE_effect);

            g_pLight->DrawLight(sx + m_vLightColor[cType].iX - 128,sy + m_vLightColor[cType].iY - 128,
                dwID,1.0f,1.0f,m_vLightColor[cColor].dwColor);
            break;
        }
    }
    return true;
}

bool CSceneManager::SMTileIterater(SMapTile* pTile,int iX,int iY,int sx,int sy)
{
	if(g_pGfx->IsNoDraw())
		return false;

	LPTexture pTex = NULL;
	// 显示小地表
	if(pTile->dwSmTile )
	{
		if (m_iInDownloadFiles > MAX_ENABLE_INDOWNLOAD_FILES && (abs(iX - m_iPlayerX) > m_iHalfTileWidth + DOWNLOAD_CACHE_X || abs(iY - m_iPlayerY) > m_iHalfTileHeight + DOWNLOAD_CACHE_Y))
		{
			pTex = g_pTexMgr->GetTexFromID(pTile->dwSmTile,EP_DONT_DOWNLOAD);
		}
		else
		{
			pTex = g_pTexMgr->GetTexFromID(pTile->dwSmTile,EP_SMTILES);
		}

		if(pTex)
		{
			//g_pGfx->DrawTextureNL(sx,sy,pTex,pTile->dwSTColor);
			g_pGfx->DrawTextureNL(sx,sy,pTex);
		}
	}

	return true;
}

bool CSceneManager::NearGroundObjectIterator(SMapTile* pTile,int iX,int iY,int sx,int sy)
{
	if(g_pGfx->IsNoDraw())
		return false;

	LPTexture pTex = NULL;
	// 显示单格子高度的建筑
	if(pTile->uObject > 0 && (pTile->uObject & OT_TYPE_MASK) == OT_NORMAL)
	{
		if ((m_iInDownloadFiles > MAX_ENABLE_INDOWNLOAD_FILES && (abs(iX - m_iPlayerX) > m_iHalfTileWidth + DOWNLOAD_CACHE_X || abs(iY - m_iPlayerY) > m_iHalfTileHeight + DOWNLOAD_CACHE_Y))
			|| (abs(iX - m_iPlayerX) > m_iHalfTileWidth + TEXCACHE_TILEX || abs(iY - m_iPlayerY) > m_iHalfTileHeight + TEXCACHE_TILEY)
			)
		{
			pTex = g_pTexMgr->GetTexFromID(pTile->uObject & OT_OBJECT_MASK,EP_DONT_DOWNLOAD);
		}
		else
		{
			pTex = g_pTexMgr->GetTexFromID(pTile->uObject & OT_OBJECT_MASK,EP_OBJECT);
		}

		if(pTex && (pTex->GetHeight0() <= TILE_HEIGHT || (pTile->byAttr & AT_NEARGROUD) != 0))
		{
			DWORD dwColor = pTile->pTile2->dwObColor;//-1;
			if (pTile->pDynTile)
			{
				DWORD dwTime = pTile->pDynTile->dwTime;
				DWORD dwTimeNow = GetTickCount();

				if(pTile->pDynTile->byAppHide = 1)//渐现
				{
					float fRate = (float)(dwTimeNow - pTile->pDynTile->dwTime)/(float)3000.0;
					if (fRate > 1.0) fRate = 1.0;
					BYTE byData = (BYTE)(fRate * 0xFF);
					dwColor = ((byData<<24) | 0x00FFFFFF);
				}
				else if (pTile->pDynTile->byAppHide == 2)//渐隐
				{
					float fRate = (float)(dwTimeNow - pTile->pDynTile->dwTime)/(float)3000.0;
					if (fRate > 1.0) fRate = 1.0;
					BYTE byData = (BYTE)((1-fRate) * 0xFF);
					dwColor = ((byData << 24) | 0x00FFFFFF);
				}
			}

			DrawObjectTexture(sx,sy+TILE_HEIGHT-(int)pTex->GetHeight0(),pTex,dwColor);
		}
	}

	//显示贴地物体
	if (pTile->uFloor)
	{
		if (m_iInDownloadFiles > MAX_ENABLE_INDOWNLOAD_FILES && (abs(iX - m_iPlayerX) > m_iHalfTileWidth + DOWNLOAD_CACHE_X || abs(iY - m_iPlayerY) > m_iHalfTileHeight + DOWNLOAD_CACHE_Y))
		{
			pTex = g_pTexMgr->GetTexFromID(pTile->uFloor & OT_OBJECT_MASK,EP_DONT_DOWNLOAD);
		}
		else
		{
			pTex = g_pTexMgr->GetTexFromID(pTile->uFloor & OT_OBJECT_MASK,EP_OBJECT);
		}

		if(pTex)
		{
			sy += TILE_HEIGHT-(int)(pTex->GetHeight0());
			DrawObjectTexture(sx,sy,pTex,pTile->pTile2->dwFloorColor);
		}
	}

	return true;
}

bool CSceneManager::UnderObjectIterator(SMapTile* pTile,int iX,int iY,int sx,int sy)
{
	if(g_pGfx->IsNoDraw())
		return false;

	if (pTile->dwUnderObj)
	{
		LPTexture pTex = NULL;
		if (m_iInDownloadFiles > MAX_ENABLE_INDOWNLOAD_FILES && (abs(iX - m_iPlayerX) > m_iHalfTileWidth + DOWNLOAD_CACHE_X || abs(iY - m_iPlayerY) > m_iHalfTileHeight + DOWNLOAD_CACHE_Y))
		{
			pTex =g_pTexMgr->GetTexFromID(pTile->dwUnderObj,EP_DONT_DOWNLOAD);
		}
		else
		{
			pTex =g_pTexMgr->GetTexFromID(pTile->dwUnderObj,EP_OBJECT);
		}

		if(pTex)
		{
			sy += TILE_HEIGHT-(int)(pTex->GetHeight0());
			//DrawObjectTexture(sx,sy,pTex,pTile->dwUnderObjColor);
			DrawObjectTexture(sx,sy,pTex,-1);
		}
	}

	return true;
}

bool CSceneManager::OverObjectIterator(SMapTile* pTile,int iX,int iY,int sx,int sy)
{
	if(g_pGfx->IsNoDraw())
		return false;

	if (pTile->dwOverObj)
	{
		LPTexture pTex = NULL;
		if (m_iInDownloadFiles > MAX_ENABLE_INDOWNLOAD_FILES && (abs(iX - m_iPlayerX) > m_iHalfTileWidth + DOWNLOAD_CACHE_X || abs(iY - m_iPlayerY) > m_iHalfTileHeight + DOWNLOAD_CACHE_Y))
		{
			pTex = g_pTexMgr->GetTexFromID(pTile->dwOverObj,EP_DONT_DOWNLOAD);
		}
		else
		{
			pTex = g_pTexMgr->GetTexFromID(pTile->dwOverObj,EP_OBJECT);
		}

		if(pTex)
		{
			sy += TILE_HEIGHT-(int)(pTex->GetHeight0());
			//DrawObjectTexture(sx,sy,pTex,pTile->dwOverObjColor);
			DrawObjectTexture(sx,sy,pTex,-1);
		}
	}

	return true;
}

bool CSceneManager::GroundIterator(SMapTile* pTile,int iX,int iY,int sx,int sy)
{
    if(g_pGfx->IsNoDraw())
        return false;

    LPTexture pTex = NULL;

    // 显示大地表
    if(pTile->dwTile && (((pTile->uObject & OT_TYPE_MASK) == OT_NORMAL) || (pTile->uObject & OT_TYPE_MASK) == OT_WATERTIDE))
    {
		int iResult = -1;

		if (m_iInDownloadFiles > MAX_ENABLE_INDOWNLOAD_FILES && (abs(iX - m_iPlayerX) > m_iHalfTileWidth + DOWNLOAD_CACHE_X || abs(iY - m_iPlayerY) > m_iHalfTileHeight + DOWNLOAD_CACHE_Y))
		{
			pTex = g_pTexMgr->GetTexFromID(pTile->dwTile,EP_DONT_DOWNLOAD);
		}
		else
		{
			pTex = g_pTexMgr->GetTexFromID(pTile->dwTile,EP_TILES,&iResult);
		}

		if(!pTex && iResult == RR_DOWNLOADING)//大地表下载中,使用一张默认的地表来显示,免得黑屏
		{
			pTex = g_pTexMgr->GetTexImm(PACKAGE_Tiles,2,EP_TILES);
		}

        if(pTex)
             //g_pGfx->DrawTextureNL(sx,sy,pTex,pTile->dwTColor);
		     g_pGfx->DrawTextureNL(sx,sy,pTex);
    }

    //绘制特效
    if((pTile->uObject & OT_TYPE_MASK) != OT_NORMAL)
    {
        GroundEffIterator(pTile,iX,iY,sx,sy);
    }

    return true;
}

bool CSceneManager::GroundBoothIterator(SMapTile* pTile,int iX,int iY,int sx,int sy)
{
    if(g_pGfx->IsNoDraw())
        return false;

    int iDir = DIR_DOWN;

    if(iX == m_iPlayerX && iY == m_iPlayerY) //主角
    {
        if(!SELF.GetBoothState() || SELF.GetBoothInfo().iBoothType != 0)////没有摆摊或不是0号摊位
            return true;

        iDir = SELF.GetDir();
    }
    else
    {
        CSimpleCharacterNode* pChar = MapArray.GetSimpleCharacterHead(iX,iY);

        //没有摆摊或不是0号摊位
        if(!pChar || pChar->IsDead() || pChar->IsNpc())
            return true;

        if(pChar->IsHuman() && !pChar->GetBoothState())
            return true;
        else if(pChar->IsMonster())
            return true;

        if(pChar->GetBoothInfo().iBoothType != 0)//非0号摊位
            return true;

        iDir = pChar->GetDir();
    }
    m_DrawBooth.iBoothType = 0;
    g_ActionInf.GetBoothInfo(iDir,&m_DrawBooth); //0号摊位，始终是画在地面上的，其余摊位才在人物上面画

    LPTexture pTex =  g_pTexMgr->GetTex(PACKAGE_items,5000+m_DrawBooth.iDir,EP_CHARACTER);
    if (pTex)
        g_pGfx->DrawTextureNL(sx+m_DrawBooth.iOffX,sy+m_DrawBooth.iOffY,pTex);

    return true;
}

//绘制怪物和主角的尸体
bool CSceneManager::GroundCorpusIterator(SMapTile* pTile,int iX,int iY,int sx,int sy)
{
    if(g_pGfx->IsNoDraw())
        return false;

    // 显示其它角色和怪物,取得此坐标上的人物链表
    CSimpleCharacterNode* pChar = MapArray.GetSimpleCharacterHead(iX,iY);
    if(pChar)
    {
        DrawSimpleCharacterNode(sx,sy,pChar,TRUE); //绘制尸体
    }

    if(iX == m_iPlayerX && iY == m_iPlayerY)
    {
        DrawSimpleCharacter(sx,sy,NULL,TRUE); //绘制尸体
    }
    return true;
}

//绘制地表魔法之上的场景
//bool CSceneManager::GroundOverEffIterator(SMapTile* pTile,int iX,int iY,int sx,int sy)
//{
//	if(g_pGfx->IsNoDraw())
//		return false;
//
//	if(pTile->uEffect != 0)
//	{
//		LPTexture pTex = g_pTexMgr->GetTexFromID(pTile->uEffect);
//		if(pTex)
//		{
//			g_pGfx->DrawTextureNL(sx,sy,pTex);
//		}
//	}
//
//	return true;
//}

bool CSceneManager::GroundGoodIterator(SMapTile* pTile,int iX,int iY,int sx,int sy)
{
    if(g_pGfx->IsNoDraw())
        return false;

    if(pTile->uObject == 0 || pTile->uObject >= 0x01000000)
    {
        CSimpleGoodNode* pGood = MapArray.GetSimpleGoodHead(iX,iY);
        while( pGood )
        {
            int tx,ty;
            pGood->GetXY(tx,ty);
            if(tx != iX || ty != iY)	// 去除物品残影
            {
				//MapArray.DeleteGood(pGood->GetID());
                //pGood = MapArray.GetSimpleGoodHead(iX,iY);
				CSimpleGoodNode* pNextGood = pGood->m_MapNext;
				int iSelfRealX,iSelfRealY;
				SELF.GetRealXY(iSelfRealX,iSelfRealY);

				if (abs(tx - iSelfRealX) >= 13 || abs(ty - iSelfRealY) >= 13)//只有iSelfRealX,Y相比不在视野才删除,比如飞蝙蝠的过程中SelfX,Y和SelfRealX,Y相关很多,如果根据SelfX,Y删除,飞到了物品就看不到了
					MapArray.DeleteGood(pGood->GetID());

				pGood = pNextGood;
                continue;
            }
            DrawSimpleGood(sx,sy,pGood);
            pGood = pGood->m_MapNext;
        }
    }
    return true;
}

void CSceneManager::DrawGround()
{
	if(g_pGfx->IsNoDraw())
		return;

	IterateByXY(-2,-4,0,5,&CSceneManager::GroundIterator);
	IterateByXY(-2,-3,0,3,&CSceneManager::SMTileIterater);
	IterateByXY(-2,-7,0,30,&CSceneManager::NearGroundObjectIterator);

	if(g_pMagicMgr)
		DrawTexShow(g_pMagicMgr->m_pMagicFloor); //绘制地面魔法 

	//IterateByXY(-3,-5,3,5, &CSceneManager::GroundOverEffIterator);
	IterateByXY(-2,-7,0,30,&CSceneManager::UnderObjectIterator);

	IterateByXY(-1,-1,0,0, &CSceneManager::GroundCorpusIterator);
	IterateByXY(-1,-1,0,0, &CSceneManager::GroundBoothIterator);
	IterateByXY(-1,-1,0,0,&CSceneManager::GroundGoodIterator);

    //海浪更新
    int x = m_iTileStartX*TILE_WIDTH + m_iDrawOffX;
    int y = m_iTileStartY*TILE_HEIGHT + m_iDrawOffY;

    g_pWeather->GetSeaTideList()->UpdateDraw(m_iPlayerX,m_iPlayerY,x,y,m_iScrWidth,m_iScrHeight);

    //绘制天气效果的地面部分
    if(g_Config.GetWeather() || g_pWeather->GetParticleType() == PARTICLE_FLOWER9)
        g_pWeather->DrawGround(m_iPlayerX,m_iPlayerY,m_iDrawOffX + TILE_WIDTH,m_iDrawOffY + TILE_HEIGHT);
}

void CSceneManager::DrawObjectTexture(int sx,int sy,LPTexture pTex,DWORD dwColor)
{
	if(!pTex)
		return;

	if(pTex)
	{
		DWORD dwID = pTex->GetID();
		if((( (dwID & 0xFFFF0000) >> 16) == PACKAGE_objects18) || (dwID >= 0x00230310 && dwID <= 0x00230343))//object18及object7的784到835
		{
			g_pGfx->SetRenderMode(RM_ADD1);
			g_pGfx->DrawTextureNL(sx,sy,pTex,dwColor);
			g_pGfx->SetRenderMode();
		}
		else if (( ((dwID & 0xFFFF0000) >> 16) == PACKAGE_objects19) || (dwID >= 0x002c0161 && dwID <= 0x002c0169))//object19及object16的353到361
		{
			g_pGfx->SetRenderMode(RM_ADD2);
			g_pGfx->DrawTextureNL(sx,sy,pTex,dwColor);
			g_pGfx->SetRenderMode();
		}
		else
		{
			g_pGfx->DrawTextureNL(sx,sy,pTex,dwColor);
		}
	}
}

bool CSceneManager::ObjectIterator(SMapTile* pTile,int iX,int iY,int sx,int sy)
{
    if(g_pGfx->IsNoDraw())
        return false;

    if(pTile->uObject > 0 && (pTile->uObject & OT_TYPE_MASK) == OT_NORMAL)
    {
		LPTexture pTex = NULL;
		if ((m_iInDownloadFiles > MAX_ENABLE_INDOWNLOAD_FILES && (abs(iX - m_iPlayerX) > m_iHalfTileWidth + DOWNLOAD_CACHE_X || abs(iY - m_iPlayerY) > m_iHalfTileHeight + DOWNLOAD_CACHE_Y))
			|| (abs(iX - m_iPlayerX) > m_iHalfTileWidth + TEXCACHE_TILEX || abs(iY - m_iPlayerY) > m_iHalfTileHeight + TEXCACHE_TILEY)
			)
		{
			pTex = g_pTexMgr->GetTexFromID(pTile->uObject & OT_OBJECT_MASK,EP_DONT_DOWNLOAD);
		}
		else
		{
			pTex = g_pTexMgr->GetTexFromID(pTile->uObject & OT_OBJECT_MASK,EP_OBJECT);
		}

		if(!pTex)
			return true;

		if(pTex->GetHeight0() <= TILE_HEIGHT || (pTile->byAttr & AT_NEARGROUD) != 0)//贴地物体
			return true;

		int y = sy + TILE_HEIGHT - pTex->GetHeight0();
		if(y > m_iScrHeight)
			return true;

		//DrawObjectTexture(sx,y,pTex,pTile->dwObColor);
		DrawObjectTexture(sx,y,pTex,-1);
    }
    return true;
}

bool CSceneManager::OvergroundIterator(SMapTile* pTile,int iX,int iY,int sx,int sy)
{
	if(g_pGfx->IsNoDraw())
		return false;

	if (pTile->uObject && pTile->uObject < OT_WATERTIDE)
	{
		CSimpleGoodNode* pGood = MapArray.GetSimpleGoodHead(iX,iY);
		while( pGood )
		{
			int tx,ty;
			pGood->GetXY(tx,ty);
			if( tx!=iX || ty!=iY )	// 去除物品残影
			{
				//MapArray.DeleteGood(pGood->GetID());
				//pGood = MapArray.GetSimpleGoodHead(iX,iY);

				CSimpleGoodNode* pNextGood = pGood->m_MapNext;
				int iSelfRealX,iSelfRealY;
				SELF.GetRealXY(iSelfRealX,iSelfRealY);

				if (abs(tx - iSelfRealX) >= 13 || abs(ty - iSelfRealY) >= 13)//只有iSelfRealX,Y相比不在视野才删除,比如飞蝙蝠的过程中SelfX,Y和SelfRealX,Y相关很多,如果根据SelfX,Y删除,飞到了物品就看不到了
					MapArray.DeleteGood(pGood->GetID());

				pGood = pNextGood;
				continue;
			}
			DrawSimpleGood(sx,sy,pGood);
			//DrawObjectTexture(sx,sy,pGood->GetLooks());
			pGood = pGood->m_MapNext;
		}
	}

	//绘制地表特效
	Magic_Show_s* show = NULL;
	Magic_Show_s** pshow = g_OtherData.GetFireShowList(iX,iY);
	if (pshow)
		show = *pshow;

	while(show)
	{
		Magic_Show_s* p = show->pNext;
		g_pMagicMgr->ShowMagic(show);
		if(g_OtherData.GetFireTexShow())
		{
			DrawTexShow(g_OtherData.GetFireTexShow());					
		}
		g_OtherData.ClearFireTexShow();
		show = p;
	}

	return true;
}
 
void CSceneManager::DrawOverground()
{
    if(g_pGfx->IsNoDraw())
        return;

	DrawSorterEx(-7,-7,7,46+7);//物体及角色

	DrawSorterTop();

    if(g_pMagicMgr)
    {
        DrawTexShow(g_pMagicMgr->m_pMagicSky);			// 空中魔法
    }
		
	IterateByXY(-2,-7,0,20,&CSceneManager::OverObjectIterator);

    DrawAlpha(); //绘制Alpha了的人物

    if(g_Config.GetWeather() || g_pWeather->GetParticleType() == PARTICLE_FLOWER9)
        g_pWeather->DrawAir(m_iTileStartX,m_iTileStartY,-m_iDrawOffX - TILE_WIDTH,-m_iDrawOffY - TILE_HEIGHT);


	if(!(g_Config.GetWeather() && g_pWeather->GetParticleType() != PARTICLE_NONE))
	{
		if (strnicmp(g_pGameMap->GetMapName(),"lxxd",4) == 0 || strnicmp(g_pGameMap->GetMapName(),"MAZE",4) == 0)
		{
			g_pWeather->SetDensity(-30);
			g_pWeather->DrawCloud(m_iTileStartX,m_iTileStartY,-m_iDrawOffX - TILE_WIDTH,-m_iDrawOffY - TILE_HEIGHT,-1);
		}		
	}	
}

bool CSceneManager::OverlayGoodNameIterator(SMapTile* pTile,int iX,int iY,int sx,int sy)
{
    if(g_pGfx->IsNoDraw())
        return false;

    //绘制物品
    CSimpleGoodNode* pGood = MapArray.GetSimpleGoodHead(iX,iY);
    if(pGood == NULL)
        return true;

    int x = sx + TILE_WIDTH / 2;
    int y = sy + TILE_HEIGHT / 2;
    int iFontH = FONTSIZE_DEFAULT + 2;

    if((!g_pInput->IsCtrl() && g_pInput->IsAlt()) || pGood && pGood->HasID(g_pControl->GetMouseOnID())) //使用Alt键，可以将物品名字全部显示出来
    {
        int iNodeNum = 0;
        CSimpleGoodNode* p = pGood;
        while(p)
        {
            DWORD dwColor = COLOR_DEFAULT;
            g_PickCfgMgr.GetItemColor(p->GetShowType(),dwColor);

            g_pGfx->DrawFillRect(x - strlen(p->GetName()) * FONTSIZE_DEFAULT/4,y + iNodeNum*iFontH - iFontH/2 - 5,strlen(p->GetName()) * FONTSIZE_DEFAULT/2,iFontH + 3,0x4F000000);
            g_pFont->DrawText(x,y+iNodeNum*iFontH-10,p->GetName(),0xFFFFFFFF,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_Center|DTF_BlackFrame);

            iNodeNum++;
            p = p->m_MapNext;
        }
    }
    else if(g_AICfgMgr.IsShowObjectName()) //显示物品名字
    {
        CSimpleGoodNode* pGood = MapArray.GetSimpleGoodHead(iX,iY);
        CSimpleGoodNode* pDrawGood = NULL; //把特殊的找出来
        CSimpleGoodNode* p = pGood;
        DWORD dwFocusType = 0;

        while(p)
        {
            DWORD dwType = p->GetShowType();

            if(dwType & ITEMSHOWTYPE_SHOW)
            {
                if((NULL == pDrawGood) ||
                    ((dwType & ITEMSHOWTYPE_EXPENSIVE) != 0 && (dwFocusType & ITEMSHOWTYPE_EXPENSIVE) == 0
                    && (dwType & ITEMSHOWTYPE_BEST) == 0)||
                    ((dwType & ITEMSHOWTYPE_BEST) != 0 && (dwFocusType & ITEMSHOWTYPE_BEST) == 0))
                {
                    pDrawGood = p;
                    dwFocusType = dwType;
                }
            }

            p = p->m_MapNext;
        }

        if(pDrawGood)
        {
            if(!g_AICfgMgr.IsShowBestItem() || (dwFocusType & ITEMSHOWTYPE_BEST) != 0)
            {
                DWORD dwColor = COLOR_DEFAULT;
                g_PickCfgMgr.GetItemColor(pGood->GetShowType(),dwColor);

                g_pFont->DrawText(x,y - 15,pGood->GetName(),dwColor,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_Center|DTF_BlackFrame);
            }
        }
    }

    // 显示物品的星
    CSimpleGoodNode* p = pGood;
    while(p)
    {
        p->AddStar();
        int iStar = p->GetStar();

        if(g_AICfgMgr.IsStarObject())
            iStar = iStar % 20;
        else
            iStar = iStar % 75;

        if(iStar < 16)
        {
            LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE,861+iStar/2,EP_MAGIC);
            if(pTex)
            {
                g_pGfx->SetRenderMode(RM_ADD1);
                g_pGfx->DrawTextureNL(x-15,y - 16,pTex);
                g_pGfx->SetRenderMode();
            }
        }
        p = p->m_MapNext;
    }
    return true;
}

#define HUMAN_OFF  40
#define HORSE_OFF  32

bool CSceneManager::OverlayCharNameIterator(SMapTile* pTile,int iX,int iY,int sx,int sy)
{
    if(g_pGfx->IsNoDraw())
        return false;


    if(iX == m_iPlayerX && iY == m_iPlayerY && g_pControl->GetMouseOnID() == SELF.GetID()) //绘制主角
    {
		DrawCharName(sx,sy,NULL);
    }
    else
    {
        /////////////////////////////////////////////////////////////////////////////////////
        //绘制其他玩家,在下面,先判断是否需要绘制
        CSimpleCharacterNode* pChar = MapArray.GetSimpleCharacterHead(iX,iY);
        while(pChar)
		{
			DrawCharName(sx,sy,pChar);
			pChar = pChar->m_MapNext;
		}

    }

    return true;
}

bool CSceneManager::DrawCharName(int sx,int sy,CSimpleCharacter* pChar)
{
	if(g_pGfx->IsNoDraw())
		return false;

	int x = sx + TILE_WIDTH / 2;
	int y = sy + TILE_HEIGHT / 2;
	int iFontH = FONTSIZE_DEFAULT + 2;

	UINT uID = 0;
	BYTE byNameColor = 0xFF;
	string strName,strTemp,strMaskName;
	VString VDrawStr;
	int iRaceNo = - 1;
	bool bGoldOX = false;
	bool bMonster = false;

	if(pChar == NULL) //绘制主角
	{
		if(SELF.GetReserveData(pubDisableDraw) == 1)
			return true;
		///////////////////////////////////////////////////////////////////
		//绘制玩家自己的逻辑
		byNameColor = SELF.GetNameColor();

		SELF.GetTopXY(x,y);
		y += HUMAN_OFF;
		if(SELF.IsOnHorse())
		{
			y -= HORSE_OFF;
		}
		//骑凤凰
		if(SELF.IsOnPhenix() && !SELF.GetFightOnLeopard())
		{
			y -= 50;
		}

		uID = SELF.GetID();
		strName = SELF.GetName();
		strMaskName = SELF.GetMaskName();
		VDrawStr = SELF.GetShowName();
		if (SELF.GetExtraState() & ES_GOLDENOXSUIT)
			bGoldOX = true;
	}
	else
	{
		if(pChar->GetReserveData(pubDisableDraw) == 1)
			return true;

		int iCharX,iCharY;
		pChar->GetXY(iCharX,iCharY);

		bMonster = pChar->IsMonster();

		if(abs(iCharY-m_iPlayerY) > 12 || abs(iCharX-m_iPlayerX) > 12)
		{
			return true;
		}

		uID = pChar->GetID();
		bool bNeedDraw = false;
		if (pChar->GetExtraState() & ES_GOLDENOXSUIT)
			bGoldOX = true;

		//始终显示的人物名字，采用简略形式，只有名字和封号码
		if((g_pInput->IsAlt() && g_pInput->IsCtrl()) || g_AICfgMgr.IsShowPlayerName())
		{
			if(pChar->IsHuman()) //只有人才绘制名字
			{
				bNeedDraw = true;
			}
		}

		iRaceNo = pChar->GetRaceNo();
		if(pChar->GetID() == g_pControl->GetMouseOnID())
		{
			if(pChar->IsNpc() && (iRaceNo == 45 || iRaceNo == 38)) //雕像柱子不显示名字
				return true;
			else if(pChar->IsMonster() && iRaceNo >= 200 && iRaceNo <= 203) //城墙等
				return true;

			bNeedDraw = true;
		}
		else if(pChar->IsMonster() && ((iRaceNo >= 251 && iRaceNo <= 253) || (iRaceNo >= 256 && iRaceNo <= 259)))//显示常胜玄兵的名字,行会怪物
			bNeedDraw = true;

		if(!bNeedDraw) return true; //不需要绘制就结束

		pChar->GetTopXY(x,y);
		if(pChar->IsHuman())
		{
			y += HUMAN_OFF;
			if(pChar->IsOnHorse())
			{
				y -= HORSE_OFF;
			}
			//骑乘凤凰
			if(pChar->IsOnPhenix() && !pChar->GetFightOnLeopard())
			{
				y -= 50;
			}
		}
		else
		{
			y += pChar->GetTexH() / 2;
		}

		if(pChar->IsMonster() && iRaceNo >= 56 && iRaceNo <= 57) //神威狱的门,比较特殊
		{
			g_pFont->DrawText(x,y,"将鼠标指向大门,使用快捷栏中灵符,",COLOR_DEFAULT,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_Center|DTF_BlackFrame);
			g_pFont->DrawText(x,y+iFontH,"就可以开启大门,获取丰厚奖励。",COLOR_DEFAULT,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_Center|DTF_BlackFrame);
			return true;
		}

		if(pChar->IsMonster() && !pChar->IsDead())	//机关门,比较特殊
		{
			if(iRaceNo == 335)
			{
				g_pFont->DrawText(x,y,"消灭地图两端的兽骑兵守门卫士门就会开启",COLOR_DEFAULT,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_Center|DTF_BlackFrame);
				return true;
			}
			else if(iRaceNo >= 333 && iRaceNo <= 336)
			{
				g_pFont->DrawText(x,y,"此门需用机关符印开启",COLOR_DEFAULT,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_Center|DTF_BlackFrame);
				return true;
			}
		}

		byNameColor = pChar->GetNameColor();

		strName = pChar->GetName();
		if(pChar->IsLeopard()) //豹子，有名字的豹子
		{
			int i1 = strName.find(")");
			int i2 = strName.find("-");
			int i3 = strName.find(":");

			if(i1>0 && i2>i1)
			{
				if(i3 < 0)
					strTemp = strName.substr(i2+1,string::npos);
				else if(i3>0 && i3>i2)
					strTemp = strName.substr(i2+1,i3-i2-1);

				strName = strName.substr(0,i2);
			}
		}

		if(pChar->IsHuman())
		{
			strMaskName = pChar->GetMaskName();
		}
		else if(pChar->IsLeopard() && pChar->GetMonsterArmLevel() > 0)
		{
			strMaskName = "万兽至尊";
		}
		VDrawStr = pChar->GetShowName();
	}

	if(y <= -200)
		return true;//如果return false会导致其它的怪名字也不绘制


	///////////////////////////////////////////////////////////////////////////////////////
	//根据前面的判断进行实际的绘制
	DWORD dwNameColor = g_pGameData->GetMirColor(byNameColor);

    //绘制小名
	y -= 4;
    if(!strTemp.empty())
    {
        g_pFont->DrawText(x,y - iFontH,strTemp.c_str(),COLOR_GREEN,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_Center|DTF_BlackFrame|DTF_Shadow);
		y += iFontH;
	}

	//绘制封号
	if(uID == g_pControl->GetMouseOnID() && !strMaskName.empty())
	{
		g_pFont->DrawText(x,y - iFontH,strMaskName.c_str(),COLOR_GREEN,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_Center|DTF_BlackFrame);
	}

	//绘制名字
	if (iRaceNo == 236)//阿修罗神B
		x += 385;
	if (bMonster && iRaceNo >= 256 && iRaceNo <= 259)
	{
		g_pFont->DrawText(x,y,strName.c_str(),COLOR_YELLOW,FONT_LISHU,18,DTF_Center|DTF_BlackFrame);
	}
	else
	{
		g_pFont->DrawText(x,y,strName.c_str(),dwNameColor,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_Center|DTF_BlackFrame);
	}

	y+=iFontH;

	for(int ii = 1;ii < VDrawStr.size();ii++)
	{
		string str = VDrawStr.at(ii);
		if(!str.empty())
		{
			g_pFont->DrawText(x,y,str.c_str(),dwNameColor,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_Center|DTF_BlackFrame);
			y += iFontH;
		}
	}

	if (bGoldOX)
		g_pFont->DrawText(x,y,"牛气冲天",dwNameColor,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_Center|DTF_BlackFrame);

	return true;
}

bool CSceneManager::OverlayNPCNameIterator(SMapTile* pTile,int iX,int iY,int sx,int sy)
{
    if(g_pGfx->IsNoDraw())
        return false;

    int x = sx + TILE_WIDTH / 2;
    int y = sy + TILE_HEIGHT / 2;
    int iFontH = FONTSIZE_DEFAULT + 2;

    CSimpleCharacter* pChar = MapArray.GetSimpleCharacterHead(iX,iY);
    if(pChar == NULL)
        return true;

	int iCharX,iCharY;
	pChar->GetXY(iCharX,iCharY);

	if(abs(iCharY-m_iPlayerY) > 12 || abs(iCharX-m_iPlayerX) > 12)
	{
		return true;
	}

    if(pChar->IsNpc())
    {
        pChar->GetTopXY(x,y);
        if(x < 0 && y < 0) //没有设置
            return true;


        int iRaceNo = pChar->GetRaceNo();

		//BYTE byHint = HIBYTE(pChar->GetLooks().Char.wShape);
		BYTE byHint = LOBYTE(pChar->GetLooks().Char.wReserve);
		if(byHint  > 0 && byHint < 6)
		{
			LPTexture pHint = g_pTexMgr->GetTex(PACKAGE_INTERFACE,17540 + byHint,EP_NPC);
			g_pGfx->DrawTextureNL(x - 10,y - 35,pHint);
		}

        if(iRaceNo == 38)
            return true;

       if((iRaceNo>= 100 && iRaceNo <= 105) || (iRaceNo >= 166 && iRaceNo <= 171))
        {
			x += 32;
			y -= (pChar->GetShowName().size() * 12 - 4);

            g_pFont->DrawText(x,y,pChar->GetName(),COLOR_DEFAULT,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_Center|DTF_BlackFrame|DTF_Shadow);

            for(int ii = 1;ii < pChar->GetShowName().size();ii++)
            {
                string str = pChar->GetShowName().at(ii);
                if(!str.empty())
                {
                    g_pFont->DrawText(x,y+iFontH*ii,str.c_str(),COLOR_GREEN,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_Center|DTF_BlackFrame|DTF_Shadow);
                }
            }
        }
        else if(g_AICfgMgr.IsShowNpcPriority())
        {
            g_pFont->DrawText(x,y - 5,pChar->GetName(),COLOR_YELLOW,FONT_LISHU,18,DTF_Center|DTF_BlackFrame);

            int iHalfNameLen = FONTSIZE_BIG * strlen(pChar->GetName()) / 4;

            SRect rc(x-iHalfNameLen,y-2,x+iHalfNameLen,y+20);

            if(rc.PtInRect(m_iMouseX,m_iMouseY))
            {
                g_pControl->SetMouseOnID(pChar->GetID());
            }
        }
    }
    return true;
}


void CSceneManager::DrawOverlay()
{
	if(g_pGfx->IsNoDraw())
        return;

    IterateByXY(-1,-1,0,0,&CSceneManager::OverlayGoodNameIterator);
    IterateByXY(-1,-1,0,0,&CSceneManager::OverlayCharNameIterator);
    IterateByXY(-1,-1,0,0,&CSceneManager::OverlayNPCNameIterator);
	IterateByXY(-1,-1,0,0,&CSceneManager::OverlayCharHPIterator);
}

void CSceneManager::DrawLianJiNumber()
{
	if (SELF.FindMagicPos(MAGICID_LIANJI) == -1)
	{
		return;
	}

	int iTopY = 250;

	//static DWORD CurNumber = 0;

	bool Effect = false;
	DWORD dwColor = 0xffffffff;
	DWORD tempColor = 0xfffffff;
	float fMinSize = 1.0f, fMaxSize = 1.3f;

	DWORD LianJiNumber = 0;

	DWORD dwHasPastTick = 0;
	//连击显示
	if (SELF.GetLianJiShowEndTime() > GetTickCount() && SELF.GetLianJiNumber() > 0)
	{
		//DWORD realNumber = SELF.GetLianJiNumber();

		//if (realNumber >= 100)
		//{
		//	Effect = true;
		//}
		//
		//if (CurNumber < realNumber && (g_pGfx->GetFrameCount()%4 == 0))
		//{
		//	++CurNumber;
		//}

		//DWORD LianJiNumber = CurNumber;

		LianJiNumber = SELF.GetLianJiNumber();

		if (LianJiNumber >= 50)
		{
			Effect = true;
		}

		DWORD LianJiShowEndTime = SELF.GetLianJiShowEndTime();

		bool bAnimation = false;
		//int fLastTime = 12000;

		int fLastTime = LianJiShowEndTime - SELF.GetLianJiZhuangJiTime();

		if (LianJiNumber == 9 || LianJiNumber == 29 || LianJiNumber == 49 || (LianJiNumber > 50 && (LianJiNumber%50 == 49)))
		{
			bAnimation = true;
		}

		/*if (SELF.GetCareer() == 0)
		{
			fLastTime = 15000;
		}*/

		std::vector<LPTexture> vLPTexture;//数字图片

		LPTexture pTex = NULL;
		while(LianJiNumber > 0)
		{
			int Tnumber = LianJiNumber%10; 

			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE ,23065+Tnumber,EP_CHARACTER);

			if (pTex)
			{
				vLPTexture.push_back(pTex);
			}

			LianJiNumber /= 10;
		}

		int Nwidth = 0;
		int NHeight = 0;

		DWORD dwTickCount = GetTickCount();
		DWORD dwLeaveTick = LianJiShowEndTime - dwTickCount;
		dwHasPastTick = fLastTime - dwLeaveTick;

		float fDura = (float)(dwHasPastTick % 300) / 300;
		
		float colorDura = 1.0f;
		if (dwLeaveTick < 5000)
		{
			colorDura = ((float)((dwLeaveTick)%5000)/5000);
		} 

		float fCurSize = fMinSize;

		if (dwHasPastTick < 300)
		{
			float fpast = (float)(dwHasPastTick % 300) / 300;
			fCurSize = fMaxSize * (1.0f-fpast) + fMinSize * fpast;
		} 
		else
		{
			if (bAnimation)
			{
				fCurSize = fMaxSize * (1.0f-fDura) + fMinSize * fDura;
			}
		}

		dwColor = ((DWORD)(0xFF*colorDura)<<24) + 0xFFFFFF;

		for (int i = 0 ; i != vLPTexture.size() ; ++i)
		{
			LPTexture pTex = vLPTexture.at(i);

			if (!pTex)
			{
				continue;
			}
			Nwidth = (int)(pTex->GetWidth0()*0.85f);
			NHeight = pTex->GetHeight0();

			
			if (Effect)
			{
				LPTexture EffectTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE ,23098,EP_CHARACTER);
				g_pGfx->DrawTextureFX(g_pGfx->GetWidth() - 172 - (int)(i*fMinSize*Nwidth),(int)(iTopY - 92),EffectTex,dwColor,NULL,&POS(fMinSize,fMinSize));

				g_pGfx->SetRenderMode(RM_ADD2);
				EffectTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE ,23078,EP_CHARACTER);

				BYTE byAlpha = (BYTE)((dwColor & 0xFF000000) >> 24);
				BYTE byR = (BYTE)(((dwColor & 0x00FF0000) >> 16) * byAlpha / 256);
				BYTE byG = (BYTE)(((dwColor & 0x0000FF00) >> 8) * byAlpha / 256);
				BYTE byB = (BYTE)( (dwColor & 0x000000FF) * byAlpha / 256);
				tempColor = ((DWORD)byAlpha << 24) | ((DWORD)byR << 16) | ((DWORD)byG << 8) | (DWORD)byB;		

				g_pGfx->DrawTextureFX(g_pGfx->GetWidth() - 172 - (int)(i*fMinSize*Nwidth),(int)(iTopY - 92),EffectTex,tempColor,NULL,&POS(fMinSize,fMinSize));
				g_pGfx->SetRenderMode();
			}

			g_pGfx->DrawTextureFX(g_pGfx->GetWidth() - 145 - (int)(i*fCurSize*Nwidth),(int)(iTopY + (1-fCurSize)*0.5*NHeight - 64),pTex,dwColor,NULL,&POS(fCurSize,fCurSize));
			
		}			
	}
	else
	{
		LianJiNumber = 0;
		dwHasPastTick = 0;
		SELF.SetLianJiNumber(0);

		if (SELF.GetLianJiShowEndTime() <= GetTickCount())
		{
			SELF.SetRealLianJiNumber(0);
		}
		
		LPTexture pTex0 = g_pTexMgr->GetTex(PACKAGE_INTERFACE ,23065,EP_CHARACTER);
		g_pGfx->DrawTextureFX(g_pGfx->GetWidth() - 145,(int)(iTopY - fMinSize*64),pTex0,0xffffffff,NULL,&POS(fMinSize,fMinSize));
	}



	if (Effect)
	{

		LPTexture EffectTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE ,23099,EP_CHARACTER);
		g_pGfx->DrawTextureNL_Trans(g_pGfx->GetWidth() - 145,iTopY + 30,EffectTex,1.0f,1.0f,0,dwColor);
		g_pGfx->SetRenderMode(RM_ADD2);
		EffectTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE ,23079,EP_CHARACTER);

		g_pGfx->DrawTextureNL_Trans(g_pGfx->GetWidth() - 145,iTopY + 30,EffectTex,1.0f,1.0f,0,tempColor);
		g_pGfx->SetRenderMode();
	}

	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE ,23076,EP_CHARACTER);
	g_pGfx->DrawTextureNL_Trans(g_pGfx->GetWidth() - 100,iTopY - 10,pTex,1.0f,1.0f,0,0xffffffff);

	if (dwHasPastTick > 0)
	{
		int interval = 40;//40ms,每两帧

		int curFrame = dwHasPastTick/interval;

		if (curFrame >= 0 && curFrame < 14)
		{
			g_pGfx->SetRenderMode(RM_ADD2);
			pTex = g_pTexMgr->GetTex(PACKAGE_magic3 ,3699+curFrame,EP_CHARACTER);
			g_pGfx->DrawTextureNL(g_pGfx->GetWidth() - 100 - 83,iTopY - 10 - 152,pTex);
			g_pGfx->SetRenderMode();
		}
	}
};

void CSceneManager::DrawTalk(CSimpleCharacter* pChar)         // 显示人物头上的话
{
    if(g_pGfx->IsNoDraw())
        return;

    DWORD  dwColor	= COLOR_DEFAULT;

    int iTopX,iTopY;
	DWORD dwCount = GetTickCount();

    if (!pChar) //主角
    {
        SELF.GetTopXY(iTopX,iTopY);

        if(SELF.GetTalkShowTime() == 0 || dwCount > SELF.GetTalkShowTime() + 2000) //停留两秒
            return;

        VString& VStr = SELF.GetTalk();
        int iFontSize = SELF.GetFontSize();
        dwColor = SELF.GetTalkColor();

        for(size_t i = 0;i < VStr.size() && i < 5;i++)
        {
            int y = iTopY - 30 - (iFontSize + 1) * i;
            g_pFont->DrawText(iTopX,y,VStr.at(VStr.size()-i-1).c_str(),dwColor,SELF.GetTalkFontType(),iFontSize,DTF_Center|DTF_BlackFrame);	
        }
    }
    else
    {
        pChar->GetTopXY(iTopX,iTopY);

        if (pChar->GetRaceNo() == 166 ||
            pChar->GetRaceNo() == 167 ||
            pChar->GetRaceNo() == 168 ||
            pChar->GetRaceNo() == 2)
        {
            if(pChar->GetPetLeaveWords().empty() || (dwCount - pChar->GetPetLeaveWordsTime()) <= 4000)
                return;

            if((dwCount - pChar->GetPetLeaveWordsTime()) > 6000)
                pChar->SetPetLeaveWordsTime(dwCount);

            VString  vDrawStr;
            CutByUnicode(pChar->GetPetLeaveWords().c_str(),vDrawStr,16);

            for(size_t i = 0; i < vDrawStr.size(); i++)
            {
                int y = (iTopY - 30) - i * (FONTSIZE_DEFAULT + 1);
                g_pFont->DrawText(iTopX,y,vDrawStr.at(vDrawStr.size()-i-1).c_str(),dwColor,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_Center,DTF_BlackFrame);
            }
            return;
        }

        if(pChar->GetTalkShowTime() == 0 || dwCount > pChar->GetTalkShowTime() + 2000)
            return;

        VString& VStr = pChar->GetTalk();
        int iFontSize = pChar->GetFontSize();
        dwColor = pChar->GetTalkColor();
        for(size_t i = 0;i < VStr.size() && i < 5;i++)
        {
            int y = iTopY - 30 - (iFontSize + 1) * i;
            g_pFont->DrawText(iTopX,y,VStr.at(VStr.size()-i-1).c_str(),dwColor,pChar->GetTalkFontType(),iFontSize,DTF_Center|DTF_BlackFrame);	
        }
    }
}

bool CSceneManager::OverlayCharHPIterator(SMapTile* pTile,int iX,int iY,int sx,int sy)        // 显示血条
{
    if(g_pGfx->IsNoDraw())
        return true;

    CSimpleCharacterNode* pChar = NULL;

    if(iX == m_iPlayerX && iY == m_iPlayerY) //绘制主角
    {
        if(SELF.IsDead())
		{
			return true;
		}
		else
		{
			DrawCharHP(sx,sy,NULL);
		}
    }
    else
    {
        pChar = MapArray.GetSimpleCharacterHead(iX,iY);
		while(pChar)
		{
			if(pChar == NULL || pChar->IsDead())
			{
				;
			}
			else
			{
				DrawCharHP(sx,sy,pChar);
			}

			pChar = pChar->m_MapNext;
		}
    }

    return true;
}

bool CSceneManager::DrawCharHP(int sx,int sy,CSimpleCharacter* pChar)
{
	int iTopX,iTopY,iTexW,iTexH;
	int iHP,iHPMax;
	int iShowHP = 36;
	DWORD dwColor = COLOR_RED;
	DWORD dwFontColor = 0;
	int iMonster = -1;

	if (pChar == NULL)
	{
		if(SELF.IsDead() || SELF.GetReserveData(pubDisableDraw) == 1)
			return true;

		iHP = SELF.GetHP();
		iHPMax = SELF.GetHPMax();
		dwFontColor = g_pGameData->GetMirColor(SELF.GetNameColor());
		SELF.GetTopXY(iTopX,iTopY);
		SELF.GetTexWH(iTexW,iTexH);

		if (SELF.IsOnHorse())
		{
			iTopY -= HORSE_OFF;
		}
		//骑凤凰
		if(SELF.IsOnPhenix() && !SELF.GetFightOnLeopard())
		{
			iTopY -= 50;
		}

		if ((SELF.GetExtraState() & ES_STEELPROTECT) || (SELF.GetStatus() & CS_PROTECTSKIN))//护身真气，金刚护体的血条
		{	
			DWORD dwTemp = SELF.GetReserveData(pubHushenHP);
			float fRate = CalRate(LOWORD(dwTemp),HIWORD(dwTemp));
			int x = iTopX - iShowHP/2;

			g_pGfx->DrawRect(x,iTopY - 1,iShowHP,4,COLOR_BLACK);
			g_pGfx->DrawFillRect(x+1,iTopY,(int)((iShowHP-2)*(1.0f-fRate)),2,COLOR_GREEN);
		}
	}
	else
	{
		int iCharX,iCharY;
		pChar->GetXY(iCharX,iCharY);

		if(abs(iCharY-m_iPlayerY) > 12 || abs(iCharX-m_iPlayerX) > 12)
		{
			return true;
		}

		if (pChar->IsDead() || pChar->GetReserveData(pubDisableDraw) == 1)
			return true;

		//if (pChar->IsHuman() && g_AICfgMgr.IsNoDrawChar())//其他人物统统不画
		//	return true;

		pChar->GetTopXY(iTopX,iTopY);
		pChar->GetTexWH(iTexW,iTexH);
		if(pChar->IsOnHorse())
		{
			iTopY -= HORSE_OFF;
		}
		//骑乘凤凰
		if(pChar->IsOnPhenix() && !pChar->GetFightOnLeopard())
		{
			iTopY -= 50;
		}

		iMonster = pChar->GetRaceNo();
		if (pChar->IsMonster())
		{
			switch(iMonster)
			{
			case 56://门1
			case 57://门2
				iTopY += 100;
				break;
			case 231: // 冰锥不绘血条
			case 390:
			case 391:
			case 392:
			case 393:
			case 394:
			case 395:
			case 396:
			case 397:
			case 398:
			case 400:
			case 401:
			case 402:
			case 403:
			case 404:
			case 405:
			case 406:
			case 407:
			case 408:
			case 409:
			case 410:
			case 411:
			case 412:
			case 413:
			case 414:
			case 415:
				return true;
				//是否为 神之召唤时(组合技能中召唤大BOSS)的判断:通天教主(198) 阿修罗神(235) 禁地魔王(192) 蛇妖王(228) 铁血魔王(221)
				//去掉血条效果
			case 192:
			case 198:
			case 228:
			case 235:
			case 221:
				{
					if(pChar->GetReserveData(pubGradualAppear) >= 20 && pChar->GetReserveData(pubGradualAppear) <= 29)
						return true;
				}
				break;

			default:
				break;

			}
		}
		else if (pChar->IsNpc())
			return true;

		iHP = pChar->GetHP();
		iHPMax = pChar->GetHPMax();
		iShowHP = pChar->GetShowHP();
		if(iShowHP < 36)
			iShowHP = 36;

		DWORD dwNameColor = g_pGameData->GetMirColor(pChar->GetNameColor());

		if(!pChar->IsHuman() || pChar->GetNameColor() == 0xFF)
			dwColor = COLOR_RED;
		else 
			dwColor = dwNameColor;

		dwFontColor = dwNameColor;
	}

	if(iTopY <= -200)
		return true;//如果return false会导致其它的怪血条也不绘制

	//画vip商行等级
	int iVipTradeLevel = 0;
	if (pChar)
	{
		iVipTradeLevel = pChar->GetVipTradeLevel();
	}
	else
	{
		iVipTradeLevel = SELF.GetVipTradeLevel();
	}

	if (iVipTradeLevel > 0)
	{
		LPTexture pTex =  g_pTexMgr->GetTex(PACKAGE_INTERFACE,17464 + iVipTradeLevel,EP_CHARACTER);
		g_pGfx->DrawTextureNL(iTopX - 45,iTopY - 6,pTex);
	}

	int i12GongTitle = 0;
	if (pChar)
		i12GongTitle = pChar->Get12GongTitle();
	else
		i12GongTitle = SELF.Get12GongTitle();

	if (i12GongTitle > 0)
	{
		LPTexture pTex =  g_pTexMgr->GetTex(PACKAGE_INTERFACE,23010 + i12GongTitle,EP_CHARACTER);
		g_pGfx->DrawTextureNL(iTopX - 42,iTopY - 27,pTex);
	}


	iTopX = iTopX - iShowHP/2;
	if (pChar == NULL || g_Config.GetFloodOn() || pChar->GetID() == g_pControl->GetMouseOnID())
	{
		float fRate = CalRate(iHP,iHPMax);		
		if (iMonster == 236)
			iTopX += 400;
		g_pGfx->DrawRect(iTopX,iTopY - 1,iShowHP,4,COLOR_BLACK);
		g_pGfx->DrawFillRect(iTopX + 1,iTopY,(int)((iShowHP-2)*fRate),2,dwColor);

		string str = StringUtil::format("%d/%d",iHP,iHPMax);
		g_pFont->DrawText(iTopX + iShowHP/2,iTopY+3,str.c_str(),dwFontColor,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_Center|DTF_BlackFrame);
	}

	//画摆摊的名字
	DrawBoothName(pChar);
	DrawTalk(pChar);


	//突显玩家自己或是身上有特殊道具标示(活动专用)
	if( (pChar == NULL && (g_AICfgMgr.IsShowArrow() || (SELF.GetStatus() & CS_STAMP))) ||
		(pChar && (pChar->GetStatus() & CS_STAMP)) || (pChar != NULL && pChar->GetLiveTokenNumber() > 0) ||
		(pChar == NULL && SELF.GetLiveTokenNumber() > 0))
	{
		int iTemp = pChar?pChar->GetReserveData(pubStampFrame):SELF.GetReserveData(pubStampFrame);
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_magic2 ,1171 + iTemp/4,EP_MAGIC);
		if(pTex)
			g_pGfx->DrawTextureNL(iTopX - 109,iTopY - 48,pTex);

		if((++iTemp) >= 44)
			iTemp = 0;

		pChar?pChar->SetReserveData(pubStampFrame,iTemp):SELF.SetReserveData(pubStampFrame,iTemp);
	}

	if ((pChar != NULL && pChar->GetLiveTokenNumber() > 0) ||
		(pChar == NULL && SELF.GetLiveTokenNumber() > 0))
	{

		int numLiveToken = pChar?pChar->GetLiveTokenNumber():SELF.GetLiveTokenNumber();
		
		LPTexture pTex = NULL;
		if (numLiveToken >= 100)//金色
		{
			int h,t;
			h = numLiveToken / 100;
			numLiveToken = numLiveToken % 100;
			t = numLiveToken / 10;
			numLiveToken = numLiveToken % 10;

			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE ,22216+h,EP_CHARACTER);
			if(pTex)
				g_pGfx->DrawTextureNL(iTopX - 28,iTopY - 68,pTex);

			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE ,22216+t,EP_CHARACTER);
			if(pTex)
				g_pGfx->DrawTextureNL(iTopX - 8,iTopY - 68,pTex);

			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE ,22216+numLiveToken,EP_CHARACTER);
			if(pTex)
				g_pGfx->DrawTextureNL(iTopX + 12,iTopY - 68,pTex);
			
		}
		else if (numLiveToken >= 10)//金色
		{
			int t;
			t = numLiveToken / 10;
			numLiveToken = numLiveToken % 10;

			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE ,22216+t,EP_CHARACTER);
			if(pTex)
				g_pGfx->DrawTextureNL(iTopX - 18,iTopY - 68,pTex);

			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE ,22216+numLiveToken,EP_CHARACTER);
			if(pTex)
				g_pGfx->DrawTextureNL(iTopX + 2,iTopY - 68,pTex);
		} 
		else if(numLiveToken >= 5)//红色
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE ,22204+numLiveToken,EP_CHARACTER);
			if(pTex)
				g_pGfx->DrawTextureNL(iTopX - 5,iTopY - 68,pTex);
		}
		else if (numLiveToken >= 1)//绿色
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE ,22228+numLiveToken,EP_CHARACTER);
			if(pTex)
				g_pGfx->DrawTextureNL(iTopX - 5,iTopY - 68,pTex);
		}
		/*if(pTex)
			g_pGfx->DrawTextureNL(iTopX - 5,iTopY - 68,pTex);*/
	}

	//if ((pChar != NULL && pChar->GetMagicSloganEndTime() > GetTickCount()) ||
	//	(pChar == NULL && SELF.GetMagicSloganEndTime() > GetTickCount()))
	//{
	//	//char temp[16] = {0};

	//	//if (pChar)
	//	//{
	//	//	memcpy(temp,pChar->GetMagicSlogan(),strlen(pChar->GetMagicSlogan()));
	//	//} 
	//	//else
	//	//{
	//	//	memcpy(temp,SELF.GetMagicSlogan(),strlen(SELF.GetMagicSlogan()));
	//	//}
	//	//char cleanedWord[64] = {0};
	//	//g_DirtyWords.ClearWords(temp,cleanedWord);

	//	//BYTE byFontSize = 32;
	//	//BYTE byFontSpacing = byFontSize/2;	//字间距,字体大小的一半
	//	//BYTE cFontColor = 56;
	//	//BYTE cBackColor = 151;	//描边颜色
	//	//BYTE byFontType = FONT_YAHEI;

	//	g_pFont->DrawText(iTopX + 17 ,iTopY - 40,pChar?pChar->GetMagicSlogan():SELF.GetMagicSlogan(),0xffdc0000,FONT_YAHEI,32,DTF_BlackFrame|DTF_Center,0,0xfff7e700);	

	//}

	////连击显示
	//if ((pChar != NULL && pChar->GetLianJiShowEndTime() > GetTickCount()) ||
	//	(pChar == NULL && SELF.GetLianJiShowEndTime() > GetTickCount()))
	//{
	//	DWORD LianJiNumber = 0;
	//	DWORD LianJiShowEndTime = 0;

	//	float fMinSize = 0.6f, fMaxSize = 1.0f;

	//	if (pChar)
	//	{
	//		LianJiNumber = pChar->GetLianJiNumber();
	//		LianJiShowEndTime = pChar->GetLianJiShowEndTime();
	//	} 
	//	else
	//	{
	//		LianJiNumber = SELF.GetLianJiNumber();
	//		LianJiShowEndTime = SELF.GetLianJiShowEndTime();
	//	}
	//	
	//	bool bAnimation = false;
	//	int fLastTime = 12000;
	//	
	//	if (LianJiNumber == 10 || LianJiNumber == 30 || LianJiNumber == 60 || LianJiNumber >= 100)
	//	{
	//		bAnimation = true;
	//		fLastTime = 15000;
	//	}

	//	std::vector<LPTexture> vLPTexture;//数字图片

	//	LPTexture pTex = NULL;
	//	while(LianJiNumber > 0)
	//	{
	//		int Tnumber = LianJiNumber%10; 

	//		if (bAnimation)
	//		{
	//			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE ,23065+Tnumber,EP_CHARACTER);
	//		} 
	//		else
	//		{
	//			pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE ,23050+Tnumber,EP_CHARACTER);
	//		}
	//			
	//		if (pTex)
	//		{
	//			vLPTexture.push_back(pTex);
	//		}
	//		
	//		LianJiNumber /= 10;
	//	}

	//	int Nwidth = 0;
	//	int NHeight = 0;

	//	DWORD dwTickCount = GetTickCount();
	//	DWORD dwLeaveTick = LianJiShowEndTime - dwTickCount;
	//	int iLeaveSecond = dwLeaveTick / 1000 + 1;

	//	float fDura = (float)(dwLeaveTick % 1000) / 1000;
	//	float colorDura = ((float)((dwLeaveTick)%fLastTime)/fLastTime);
	//	const float fCurSize = fMaxSize * (1.0f-fDura) + fMinSize * fDura;
	//	DWORD dwColor = ((DWORD)(0xFF*colorDura)<<24) + 0xFFFFFF;

	//	for (int i = vLPTexture.size() - 1; i >= 0 ; --i)
	//	{
	//		LPTexture pTex = vLPTexture.at(i);

	//		if (!pTex)
	//		{
	//			continue;
	//		}
	//		Nwidth = pTex->GetWidth();
	//		NHeight = pTex->GetHeight();
	//
	//		if (bAnimation)
	//		{
	//			//g_pGfx->DrawTextureNL_Trans(iTopX+Nwidth/2+iLeft/2,iY, pTex,fCurSize,fCurSize,0,dwColor);
	//			g_pGfx->DrawTextureFX(iTopX + 50 +(int)((vLPTexture.size() - 1 - i)*fCurSize*Nwidth),(int)(iTopY - 1*fCurSize*NHeight) + 20,pTex,dwColor,NULL,&POS(fCurSize,fCurSize));
	//		} 
	//		else
	//		{
	//			g_pGfx->DrawTextureNL(iTopX + 50 +(vLPTexture.size() - 1 - i)*Nwidth,iTopY - NHeight + 20,pTex,dwColor);

	//		}	
	//	
	//	}

	//	pTex = NULL;
	//	
	//	/*if (bAnimation)
	//	{
	//		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE ,23075,EP_CHARACTER);
	//		g_pGfx->DrawTextureFX(iTopX + (int)(vLPTexture.size()*fCurSize*Nwidth),(int)(iTopY - 1*fCurSize*NHeight),pTex,dwColor,NULL,&POS(fCurSize,fCurSize));

	//	} 
	//	else*/
	//	{
	//		pTex = g_pTexMgr->GetTex(PACKAGE_INTERFACE ,23076,EP_CHARACTER);
	//		g_pGfx->DrawTextureNL(iTopX +140,iTopY - 160,pTex,dwColor);
	//	}		
	//}

// 	if (pChar == NULL)
// 	{
// 		if (SELF.GetTrusteeshipFlag() > 0 && !(SELF.GetStatus() & CS_STAMP) && !g_AICfgMgr.IsShowArrow())
// 		{
// 			g_pGfx->DrawTextureNL(iTopX + 5,iTopY - 31,g_pTexMgr->GetTex(PACKAGE_INTERFACE ,11025));
// 		}
// 	}
// 	else if (pChar->GetTrusteeshipFlag() > 0)
// 	{
// 		g_pGfx->DrawTextureNL(iTopX + 5,iTopY - 31,g_pTexMgr->GetTex(PACKAGE_INTERFACE ,11025 - 1 + pChar->GetTrusteeshipFlag()));
// 	}

	return true;
}

void CSceneManager::DrawBoothName(CSimpleCharacter* pChar)
{
    if(g_pGfx->IsNoDraw())
        return;

    DWORD dwColor = 0xFFF3A934;
    string strBoothName;
    int iTopX,iTopY;

    if (NULL == pChar) //本人
    {
        if(!SELF.GetBoothState()) //没有摆摊
            return;

        strBoothName = g_BoothData.GetBoothName();
        SELF.GetTopXY(iTopX,iTopY);
    }
    else  //其他人
    {
        if(!pChar->GetBoothState())
            return;

        strBoothName = pChar->GetBoothInfo().strBoothName;
        pChar->GetTopXY(iTopX,iTopY);
   }

    if(strBoothName.empty()) //名字为空
        return;

	int iFontHeight = FONTSIZE_DEFAULT;
    VString  vDrawStr;
    CutByUnicode(strBoothName.c_str(),vDrawStr,26,false);

    for(size_t i = 0; i < vDrawStr.size(); i++)
    {
        int y = iTopY - (i+1)*(iFontHeight+1);
        g_pFont->DrawText(iTopX,y,vDrawStr[vDrawStr.size()-i-1].c_str(),dwColor,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_Center|DTF_BlackFrame);
    }
}

void CSceneManager::DrawSeniorBooth(int iX,int iY,DrawBoothInf* pBoothInf,BOOL bFirst) // 显示摊位
{
    if(g_pGfx->IsNoDraw())
        return;

    if(pBoothInf == NULL)
        return;

	LPTexture pFlagPoleTex = 0,pFlagTex = 0,pTex = 0;

	if(pBoothInf->iBoothType == 13 || pBoothInf->iBoothType == 14)
	{
		if (pBoothInf->iBoothType == 13)
			pFlagTex = g_pTexMgr->GetTex(PACKAGE_items,5098,EP_CHARACTER);
		else
			pFlagTex = g_pTexMgr->GetTex(PACKAGE_items,5099,EP_CHARACTER);
			
		pTex =  g_pTexMgr->GetTex(PACKAGE_items,5000+pBoothInf->iDir,EP_CHARACTER); //摊位

		int x = iX+pBoothInf->iFlagOffX - 13;
		int y = iY+pBoothInf->iFlagOffY - 27;

		if(bFirst && pBoothInf->bFlagFirst) //旗子先画
		{
			//画旗子		
			if (pFlagTex)
				g_pGfx->DrawTextureNL(x,y,pFlagTex,pBoothInf->dwFlagColor);
		}

		if(bFirst == pBoothInf->bBoothFirst)
		{
			if (pTex)
				g_pGfx->DrawTextureNL(iX+pBoothInf->iOffX,iY+pBoothInf->iOffY,pTex);
		}

		if(!bFirst && !pBoothInf->bFlagFirst) //旗子后画
		{
			//画旗子
			if (pFlagTex)
				g_pGfx->DrawTextureNL(x,y,pFlagTex,pBoothInf->dwFlagColor);
		}
	}
	else
	{
		pFlagPoleTex = g_pTexMgr->GetTex(PACKAGE_items,5060,EP_CHARACTER);
		pFlagTex = g_pTexMgr->GetTex(PACKAGE_items,5061,EP_CHARACTER);
		pTex =  g_pTexMgr->GetTex(PACKAGE_items,5000+pBoothInf->iBoothType*4+pBoothInf->iDir,EP_CHARACTER); //摊位

		int x = iX+pBoothInf->iFlagOffX;
		int y = iY+pBoothInf->iFlagOffY;

		if(pBoothInf->iFlagType > 0 && bFirst && pBoothInf->bFlagFirst) //旗子先画
		{
			//画旗子
			if (pFlagPoleTex)
				g_pGfx->DrawTextureNL(x,y,pFlagPoleTex);
			if (pFlagTex)
				g_pGfx->DrawTextureNL(x,y,pFlagTex,pBoothInf->dwFlagColor);
		}

		if(pBoothInf->iBoothType != 0 && bFirst == pBoothInf->bBoothFirst)
		{
			if (pTex)
				g_pGfx->DrawTextureNL(iX+pBoothInf->iOffX,iY+pBoothInf->iOffY,pTex);
		}

		if(pBoothInf->iFlagType > 0 && !bFirst && !pBoothInf->bFlagFirst) //旗子后画
		{
			//画旗子
			if (pFlagPoleTex)
				g_pGfx->DrawTextureNL(x,y,pFlagPoleTex);
			if (pFlagTex)
				g_pGfx->DrawTextureNL(x,y,pFlagTex,pBoothInf->dwFlagColor);
		}
	}    
}

//绘制野蛮冲撞
void CSceneManager::DrawWildCollide(int iX, int iY)
{
	if(m_DrawInf.iDrawAction != 23)
		m_DrawInf.iDrawAction = ACTION_ATTACK1;
	else 
		m_DrawInf.iDrawAction = ACTION_LFRUN;
    m_DrawInf.iFrame = 4;
    DWORD dwColor = m_DrawInf.dwColor;

	 int ax,ay,sx,sy;
	 GetDirStep(m_DrawInf.iDir,ax,ay);

    // 野蛮冲撞
    for(int ii = 0; ii <= 2; ii++)
    {
        sx = iX - 32 * (ii+1) * ax;
        sy = iY - 16 * (ii+1) * ay;

        DWORD dwTemp = 0xA0 - 0x1A * (ii+1);
        m_DrawInf.dwColor = (dwTemp<<24)|(dwColor & 0x00FFFFFF);

        DrawLooks(sx,sy,&m_DrawInf);
    }

    m_DrawInf.dwColor = dwColor;
}

//绘制突斩刀光
void CSceneManager::DrawSuddenKill(int iX, int iY, SAction* pAction, CSimpleCharacter* pChar)
{
	int iFrame = pAction->iFrameNow;
	iFrame = (int)((float)iFrame / pAction->iFrameCount * 6);

	bool bIsOnLepoard = false;

	int iSuddenKillFrame = 0;

	if (pChar == NULL)
	{
		bIsOnLepoard = SELF.IsOnLepoard();
		iSuddenKillFrame = SELF.GetReserveData(plySuddenKillFrame);
		SELF.AddReserveData(plySuddenKillFrame);
	}
	else
	{
		bIsOnLepoard = pChar->IsOnLepoard();
		iSuddenKillFrame = pChar->GetReserveData(plySuddenKillFrame);
		pChar->AddReserveData(plySuddenKillFrame);
	}

	//预读纹理
	if(pAction->iFrameNow == 0 && pAction->iDealTimesCurFrame == 0)
	{
		if(!bIsOnLepoard)
		{
			int iBegin = 5502 + m_DrawInf.iDir*6;
			int iEnd = 5502 + m_DrawInf.iDir*6 + 6;
			g_pTexMgr->PreLoad(PACKAGE_magic1, iBegin, iEnd,EP_SKILL);
		}
		else
		{
			int iBegin = 29220 + m_DrawInf.iDir*6;
			int iEnd = 29220 + m_DrawInf.iDir*6 + 6;
			g_pTexMgr->PreLoad(PACKAGE_magic2, iBegin, iEnd,EP_SKILL);
		}
	}
 
 	if(iSuddenKillFrame % 10 == 0)
    {
        bool bSelf = false;
        int iSelfX,iSelfY,iOtherX,iOtherY; 
        SELF.GetXY(iSelfX,iSelfY);
        if (pChar == NULL)
        {
            SELF.GetXY(iOtherX,iOtherY);
            bSelf = true;			
        }
        else
        {
            pChar->GetXY(iOtherX,iOtherY);			
        }

        //播放突斩音乐
        int iSoundIndex = MAGICID_SUDDEN_KILL*4 + 16;
        g_pAudio->PlayEx(EAT_MAGIC,iSoundIndex,g_pAudio->GetRand()++,iSelfX,iOtherX,iSelfY,iOtherY,bSelf,1);
    }

    LPTexture pTex = NULL;

	if(bIsOnLepoard)
	{
		pTex = g_pTexMgr->GetTex(PACKAGE_magic2, 29220 + m_DrawInf.iDir*6 + iFrame,EP_SKILL);		
	}
	else
	{
		pTex = g_pTexMgr->GetTex(PACKAGE_magic1, 5502 + m_DrawInf.iDir*6 + iFrame,EP_SKILL);		
	}

	if(pTex)
	{
		g_pGfx->SetRenderMode(RM_ADD1);
		g_pGfx->DrawTextureNL(iX, iY - 30,pTex);
		g_pGfx->SetRenderMode();
	}
    
}

void CSceneManager::DrawSavageCollide(int iX,int iY,BYTE byType)
{
	if (byType == 0 || (byType != 1 && byType != 2)) return;

	WORD wTex = (byType == 1)?(38865 + m_DrawInf.iDir):(38875 + m_DrawInf.iDir);
	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_magic1,wTex,EP_SKILL);

	if(pTex)
	{
		g_pGfx->SetRenderMode(RM_ADD1);
		g_pGfx->DrawTextureNL(iX, iY,pTex);
		g_pGfx->SetRenderMode();
	}
}

//绘制怪物灵魂墙
void CSceneManager::DrawSoulWall(int iX,int iY,CSimpleCharacter * pChar)
{
    if(!pChar)
        return;

    pChar->AddReserveData(pubSoulWallFrame);
    DWORD dwFrame = pChar->GetReserveData(pubSoulWallFrame);
    DWORD dwState = pChar->GetReserveData(pubSoulWallState);
    int iSelfX,iSelfY,iOtherX,iOtherY;
    SELF.GetXY(iSelfX,iSelfY);
    pChar->GetXY(iOtherX,iOtherY);

	int iFlyType = 0;
	if(dwState >= 5 && dwState <= 8)
	{
		iFlyType = 1;	//神
		dwState -= 4;
	}
	//else if(dwState >= 9 && dwState <= 12)
	//{
	//	iFlyType = 2;	//魔
	//	dwState -= 8;
	//}

    DWORD iMusicID =  MAGICID_SOUL_WALL*4 +16;
    switch(dwState)
    {
    case 1:			// 开始
        {
            iMusicID += 3;
            g_pAudio->PlayEx(EAT_MAGIC,iMusicID,g_pAudio->GetRand()++,iSelfX,iOtherX,iSelfY,iOtherY,false);
            if(dwFrame > 10)
            {
                dwFrame = 1;
				pChar->SetReserveData(pubSoulWallState,3 + 4 * iFlyType);               
                pChar->SetReserveData(pubSoulWallFrame,1);
            }
            break;
        }
    case 2:			// 结束
        {
            iMusicID += 2;
            g_pAudio->PlayEx(EAT_MAGIC,iMusicID,g_pAudio->GetRand()++,iSelfX,iOtherX,iSelfY,iOtherY,false);

            if(dwFrame >= 10)
            {
                pChar->SetReserveData(pubSoulWallState,0 + 4 * iFlyType);
				pChar->SetReserveData(pubSoulWallFrame,0);
                pChar->RemoveStatus(CS_SOULWALL);
            }
            break;
        }
    case 3:			// 正常
        {
            if(dwFrame > 10)
            {
                dwFrame = 1;
                pChar->SetReserveData(pubSoulWallFrame,1);
            }
            if(m_DrawInf.iAction == ACTION_ATTACK1 && m_DrawInf.iFrame == 5)
            {
                dwFrame = 1;
                pChar->SetReserveData(pubSoulWallState,4 + 4 * iFlyType);
				pChar->SetReserveData(pubSoulWallFrame,1);
                break;
            }
            break;
        }
    case 4:		// 被攻击
        {
            iMusicID = 15;
            g_pAudio->PlayEx(EAT_MAGIC,iMusicID,g_pAudio->GetRand()++,iSelfX,iOtherX,iSelfY,iOtherY,false);

            if(dwFrame > 16)
            {
                dwFrame = 1;
                pChar->SetReserveData(pubSoulWallState,3 + 4 * iFlyType);
				pChar->SetReserveData(pubSoulWallFrame,1);
            }
            if(dwFrame > 1)
                dwFrame /= 2;
            break;
        }
    }

    dwState = pChar->GetReserveData(pubSoulWallState);
    /*if(dwState > 0 && dwState < 5)
    {
        LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_magic1,4730 + dwState * 10 + dwFrame);
        if(pTex)
        {
            g_pGfx->SetRenderMode(RM_ADD1);
            if(pChar->GetReserveData(pubSoulWallColor) == 0)
                g_pGfx->DrawTextureNL(iX,iY-8,pTex);
            else
                g_pGfx->DrawTextureNL(iX,iY-8,pTex,pChar->GetReserveData(pubSoulWallColor));

            g_pGfx->SetRenderMode();
        }
    }*/

	LPTexture pTex = NULL;

	int iOffsetX = 0;
	int iOffsetY = -8;
	if(dwState >= 1 && dwState <= 4)
	{
		pTex = g_pTexMgr->GetTex(PACKAGE_magic1,4731 + dwState * 10 + dwFrame,EP_SKILL);
	}
	else if(dwState >= 5 && dwState <= 8)
	{
		pTex = g_pTexMgr->GetTex(PACKAGE_magic1,41750 + (dwState - 5) * 10 + dwFrame,EP_SKILL);
		iOffsetX = -135;
		iOffsetY = -115;
	}
	else if(dwState >= 9 && dwState <= 12)
	{
		iOffsetX = -135;
		iOffsetY = -115;

		pTex = g_pTexMgr->GetTex(PACKAGE_magic1,41830 + (dwState - 9) * 10 + dwFrame,EP_SKILL);
		{
			if(pChar->GetReserveData(pubSoulWallColor) == 0)
				g_pGfx->DrawTextureNL(iX + iOffsetX,iY + iOffsetY,pTex);
			else
				g_pGfx->DrawTextureNL(iX + iOffsetX,iY + iOffsetY,pTex,pChar->GetReserveData(pubSoulWallColor));
		}

		pTex = g_pTexMgr->GetTex(PACKAGE_magic1,41790 + (dwState - 9) * 10 + dwFrame,EP_SKILL);
	}        

	g_pGfx->SetRenderMode(RM_ADD1);

	if(pChar->GetReserveData(pubSoulWallColor) == 0)
		g_pGfx->DrawTextureNL(iX + iOffsetX,iY + iOffsetY,pTex);
	else
		g_pGfx->DrawTextureNL(iX + iOffsetX,iY + iOffsetY,pTex,pChar->GetReserveData(pubSoulWallColor));

	g_pGfx->SetRenderMode();
}

void CSceneManager::DrawFlagText(int x,int y,CSimpleCharacter * pChar)
{
    if(!pChar)
        return;

    char *p = (char*)pChar->GetName();
    int n = strlen(p);
    char *p1;
    int i = 0,j = 0;
    char text[10] = {0};
    int iX,iY;
    int iNpc = pChar->GetRaceNo();

	/////////////
	///////黄榜大大旗
    if(iNpc==173)
    {
        LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_npc1,34606,EP_NPC);
        if(pTex)
            g_pGfx->DrawTextureNL(x-153,y-273,pTex,0xFFFFFFFF);
    }
    else if(iNpc==175)	//华盖天罗旗
    {
        LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_npc1,35005,EP_NPC);
        if(pTex)
            g_pGfx->DrawTextureNL(x-98,y-298,pTex,0xFFFFFFFF);

        pTex = g_pTexMgr->GetTex(PACKAGE_npc1,35006,EP_NPC);
        if(pTex)
            g_pGfx->DrawTextureNL(x-98,y-298,pTex,0xFFFFFFFF);
    }

	int iFont = FONT_DEFAULT;
    if(iNpc == 174)
        iFont = FONT_GDI_HANGKAI;
    else
        iFont = FONT_GDI_KAITI;

    while(i < n)
    {
        p1 = CharNext(p);
        if(p1 == NULL || j >= 10)
            break;
        i += (int)(p1 - p);
        memcpy(text,p,p1-p);
        p=p1;

        if(iNpc==173)//黄榜大旗
        {
            if(n<=10)
            {
                iX = x + 26;
                iY = y-164+18*j;
            }
            else
            {
                if(j<5)
                {
                    iX = x + 18;
                    iY = y - 170+18*j;
                }
                else
                {
                    iX = x + 36;
                    iY = y - 162 + 18*(j%5);
                }
            }	

            g_pFont->DrawText_Trans(iX,iY,text,0xFFFFFF00,iFont,FONTSIZE_BIG,DTF_BlackFrame,1.0f,1.0f,0.5f);
        }		
        else if(iNpc==174)//大的黄榜大旗
        {
            if(j >= 4)
                break;

            g_pFont->DrawText_Trans(x + 29,y - 192 + 26 * j,text,0xFFFFFF00,iFont,FONTSIZE_BIG,DTF_BlackFrame,2.0f,1.5f,0.9f);
        }
        else if(iNpc==175)//华盖天罗旗
        {
            if(n<=10)
            {
                iX = x+117;
                iY = y-34+17*j;
            }
            else
            {
                if(j<5)
                {
                    iX = x+109;
                    iY = y-40+17*j;
                }
                else
                {
                    iX = x+127;
                    iY = y-32+17*(j%5);
                }
            }

            g_pFont->DrawText_Trans(iX-95,iY- 144,text,0xFFFFFF00,iFont,FONTSIZE_BIG,DTF_BlackFrame,1.0f,1.0f,0.5f);
        }		
        else if(iNpc==176)//玄天功德牌
        {
            if(n<=10)
            {
                iX = x+86;
                iY = y-34+17*j;
            }
            else
            {
                if(j<5)
                {
                    iX = x+80;
                    iY = y-40+17*j;
                }
                else
                {
                    iX = x+95;
                    iY = y-32+17*(j%5);
                }
            }
            g_pFont->DrawText_Trans(iX-62,iY-76,text,0xFFFFFF00,iFont,FONTSIZE_BIG,DTF_BlackFrame,1.0f,1.0f,0.5f);
        }
        j++;
    }
}

void CSceneManager::DrawFireShieldAttack(int x,int y,CSimpleCharacter * pChar)
{
	CCharacterAttr* pAttr = NULL;
	if(pChar)
		pAttr = pChar;
	else
		pAttr = &SELF;

	if (!pAttr)
		return;

	

	SAction& sAction = pAttr->GetAction();
	int iFrame = m_dwFrameCount - sAction.tFrameStart;
	int iToFrame = sAction.iFrameSpeed * sAction.iFrameCount;
	int iScaleStart = iToFrame / 2;

	if (iFrame < iScaleStart)
		return;


	float fSaveScale = m_DrawInf.fScale;
	DWORD dwSaveColor = m_DrawInf.dwColor;
	int iSaveFrame = m_DrawInf.iFrame;

	if (iFrame >= iToFrame)
	{
		iFrame = iToFrame;
		m_DrawInf.iFrame = m_DrawInf.iActionFrames - 1;
	}

	int ax,ay,cx,cy;
	int backdir = ModifyDir(m_DrawInf.iDir + 4,8);
	GetDirStep(backdir,ax,ay);
	cx = x + 64*ax*(iFrame - iScaleStart)/(iToFrame - iScaleStart);
	cy = y + 32*ay*(iFrame - iScaleStart)/(iToFrame - iScaleStart);
	
	m_DrawInf.fScale = 1.0f + 1.0f * (iFrame - iScaleStart)/(iToFrame - iScaleStart);
	DWORD dwTemp = 0xA0 - 0x1A;
	m_DrawInf.dwColor = (dwTemp<<24)|(dwSaveColor & 0x00FFFFFF);

	DrawLooks(cx,cy,&m_DrawInf);

	m_DrawInf.fScale = fSaveScale;
	m_DrawInf.dwColor = dwSaveColor;
	m_DrawInf.iFrame = iSaveFrame;
	
}

//八方分影斩
void CSceneManager::DrawShadowKill8(int x,int y,CSimpleCharacter * pChar)
{
	CCharacterAttr* pAttr = NULL;
	if(pChar)
		pAttr = pChar;
	else
		pAttr = &SELF;

	if (!pAttr || !pAttr->IsInShadowKill8())
		return;

	DWORD dwFrameStart = pAttr->GetReserveData(pubShadowKill8FrameStart);
	int iFrame = g_pGfx->GetFrameCount() - dwFrameStart;//此处的iFrame不一定是连续的
	
	int iStepFrame = pAttr->GetShadowStepFrame();
	int leftShadowNum = pAttr->GetLeftShadowNum();
	int rightShadowNum = pAttr->GetRightShadowNum();
	int maxShadowNum = pAttr->GetMaxShadowNum();
	

	int iSaveDrawAction = m_DrawInf.iDrawAction;
	int iSaveAction = m_DrawInf.iAction;
	int iSaveFrame = m_DrawInf.iFrame;
	DWORD dwSaveColor = m_DrawInf.dwColor;
	int iSaveDir = m_DrawInf.iDir;

	int iStartdir = pAttr->GetShadowKill8Dir() + 4;

	int ax,ay,cx,cy,tx,ty;
	GetDirStep(m_DrawInf.iDir,ax,ay);
	cx = x + 128*ax;
	cy = y + 64*ay;

	DWORD dwTemp = 0xA0 - 0x1A;
	m_DrawInf.dwColor = (dwTemp<<24)|(dwSaveColor & 0x00FFFFFF);	

	m_DrawInf.iAction = ACTION_RUN;

	int iCount = 0;
	
	int p1x,p1y,p2x,p2y,movedir;
	int leftdir,rightdir;
	for (int i = 0; i < maxShadowNum; ++i,++iCount)
	{
		int iStartFrame = iStepFrame*i;
		if (iFrame >= iStartFrame && iFrame < iStartFrame + iStepFrame)
		{
			//左边分身
			if (i < leftShadowNum)
			{
				if (i == 0)
				{
					p1x = x;
					p1y = y;
				}
				else
				{
					leftdir = ModifyDir(iStartdir + i,8);
					GetDirStep(leftdir,ax,ay);
					p1x = cx + 128*ax;
					p1y = cy + 64*ay;
				}


				leftdir = ModifyDir(iStartdir + i + 1,8);
				GetDirStep(leftdir,ax,ay);
				p2x = cx + 128*ax;
				p2y = cy + 64*ay;

				movedir = GetDir8Attack(p2x,p2y,p1x,p1y);

				tx = p1x + (p2x - p1x) * (iFrame - iStartFrame) / iStepFrame;
				ty = p1y + (p2y - p1y) * (iFrame - iStartFrame)/ iStepFrame;
				m_DrawInf.iDir = movedir;
				DrawWildCollide(tx,ty);
			}
			

			//右边边分身
			if (i < rightShadowNum)
			{			
				if (i == 0)
				{
					p1x = x;
					p1y = y;
				}
				else
				{
					rightdir = ModifyDir(iStartdir - i,8);
					GetDirStep(rightdir,ax,ay);
					p1x = cx + 128*ax;
					p1y = cy + 64*ay;
				}

				rightdir = ModifyDir(iStartdir - i - 1,8);
				GetDirStep(rightdir,ax,ay);
				p2x = cx + 128*ax;
				p2y = cy + 64*ay;

				movedir = GetDir8Attack(p2x,p2y,p1x,p1y);

				tx = p1x + (p2x - p1x) * (iFrame - iStartFrame) / iStepFrame;
				ty = p1y + (p2y - p1y) * (iFrame - iStartFrame)/ iStepFrame;
				m_DrawInf.iDir = movedir;
				DrawWildCollide(tx,ty);
			}

			break;
		}
	}	

	m_DrawInf.iDrawAction = iSaveDrawAction;
	m_DrawInf.iAction = iSaveAction;
	m_DrawInf.iFrame = iSaveFrame;
	
	for (int i = 1; i <= iCount; ++i)
	{
		if (i <= leftShadowNum)
		{
			int leftdir1 = ModifyDir(iStartdir + i,8);
			int leftdir2 = ModifyDir(leftdir1 + 4,8);
			GetDirStep(leftdir1,ax,ay);
			tx = cx + 128*ax;
			ty = cy + 64*ay;
			m_DrawInf.iDir = leftdir2;
			DrawLooks(tx,ty,&m_DrawInf);

			if (g_pMagicMgr)
				g_pMagicMgr->DrawSwearLightEx(tx,ty,m_DrawInf.iDir,pChar);
		}
		
		if (i <= rightShadowNum)
		{
			int rightdir1 = ModifyDir(iStartdir - i,8);
			int rightdir2 = ModifyDir(rightdir1 + 4,8);
			GetDirStep(rightdir1,ax,ay);
			tx = cx + 128*ax;
			ty = cy + 64*ay;
			m_DrawInf.iDir = rightdir2;
			DrawLooks(tx,ty,&m_DrawInf);

			if (g_pMagicMgr)
				g_pMagicMgr->DrawSwearLightEx(tx,ty,m_DrawInf.iDir,pChar);
		}		
	}

	m_DrawInf.dwColor = dwSaveColor;
	m_DrawInf.iDir = iSaveDir;
	m_DrawInf.iDrawAction = iSaveDrawAction;
	m_DrawInf.iAction = iSaveAction;
	m_DrawInf.iFrame = iSaveFrame;	

	//绘制本体刀光
	if (g_pMagicMgr)
		g_pMagicMgr->DrawSwearLightEx(x,y,m_DrawInf.iDir,pChar);
}


//画从头上冒出来的经验及其它
// void CSceneManager::DrawBubbleUp(CSimpleCharacter * pChar)
// {
//     if(pChar)
//     {
//         if(!pChar->IsHuman() || pChar->IsDemonMonster() || pChar->IsDead())
//             return;
//     }
//     else if(SELF.IsDead())
//         return;
// 
//     int iX,iY;
//     if(pChar)
//         pChar->GetTopXY(iX,iY);
//     else
//         SELF.GetTopXY(iX,iY);
// 
//     DWORD dwLastTime = 0;
//     char strExp[24]={0};
// 
//     for(int i = 0; i < 8; i++)
//     {
//         BubbleUpStruct &expBubbleUp = (pChar)?pChar->GetBubbleUp(i):SELF.GetBubbleUp(i);
// 
//         if(expBubbleUp.byType != 0)
//         {
//             dwLastTime = GetTickCount() - expBubbleUp.dwStartTime;
//             if(dwLastTime >= expBubbleUp.dwLastTime)
//             {
//                 expBubbleUp.byType = 0;
//             }
//             else
//             {
//                 if(expBubbleUp.byType == 1)
//                     sprintf(strExp,"+%d经验",expBubbleUp.dwValue);
//                 else if(expBubbleUp.byType == 2)
//                     sprintf(strExp,"黑暗属性临时升级至%d",expBubbleUp.dwValue);
// 
//                 DWORD dwColor = 0x00FFFF00 | (((int)(255 * (expBubbleUp.dwLastTime - dwLastTime)/(float)(expBubbleUp.dwLastTime))) << 24);
//                 g_pFont->DrawText(iX + 15 - strlen(strExp)*11/2,iY - 20 - dwLastTime/50,strExp,dwColor,FONTSIZE_MIDDLE);            
//             }
//         }
//     }
// }
//??


