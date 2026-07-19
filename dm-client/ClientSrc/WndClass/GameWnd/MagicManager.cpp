#include "MagicManager.h"
#include "GameData/MagicCtrlMgr.h"
#include "GameData/GameData.h"
#include "GameData/GameGlobal.h"
#include <math.h>
#include "SceneManager.h"
#include "BaseClass/Audio/Audio.h"
#include "BaseClass/Graphic/TexManager.h"
#include "BaseClass/Graphic/GraphicD3D.h"

//魔法飞行偏移量
class CMagicFlyingBeginOffset 
{
public :
	int id;					//飞行技能偏移
	POINT ptOffset[8];		//8方向上的出发偏移

	static CMagicFlyingBeginOffset *GetMagicOffset(int id);
	static POINT GetCenterOffset(int id,int dir);

};
CMagicFlyingBeginOffset g_MagicOffset[] = 
{
	//0      1       2      3		 4        5         6      7	
	{180,{{0,-48},{14,-14},{32,0},{32,16}, {0,64}, {-32,16},{-32,0},{-14,14}}},
	{154,{{0,-32},{14,-14},{32,0},{32,16}, {0,48}, {-32,16},{-32,0},{-14,14}}},
};
CMagicFlyingBeginOffset *CMagicFlyingBeginOffset::GetMagicOffset(int id)
{
	for(int i = 0 ;i<sizeof(g_MagicOffset) / sizeof(g_MagicOffset[0]);++i)
	{
		if(id == g_MagicOffset[i].id)
			return &g_MagicOffset[i];
	}
	return NULL;
}

//输入16方向
POINT CMagicFlyingBeginOffset::GetCenterOffset(int id,int dir)
{
	POINT pt = {0,0};
	CMagicFlyingBeginOffset *off = GetMagicOffset(id);
	if(off)
	{
		dir/=2;
		return off->ptOffset[dir];
	}
	return pt;
};

CMagicManager* g_pMagicMgr = NULL;

//魔法管理器
#define MAX_WINDOW_LIMIT 1280					//最大窗口限制尺度
#define MAX_SERVER_NOTIFY_X 13					//服务端通知x范围
#define MAX_SERVER_NOTIFY_Y 14					//服务端通知Y范围

CMagicManager::CMagicManager(void)
{
	m_pMagicFloor	= NULL;		//地表魔法暂存
	m_pMagicSky		= NULL;		//空中魔法暂存
	m_dwTickCount = 0;
}

CMagicManager::~CMagicManager(void)
{
}

tex_show_t* CMagicManager::CreateTexShow()
{
	tex_show_t * pshow = NEW_TEX_SHOW();
	if (pshow)
	{
		memset(pshow,0,sizeof(pshow));
		pshow->fScaleX = 1.f;
		pshow->fScaleY = 1.f;

		return pshow;
	}
	return NULL;
};

void CMagicManager::DealMagic()
{
	if(g_pSceneMgr == NULL)
		return;

	m_dwTickCount = GetTimeCount();

	m_iTileStartX = g_pSceneMgr->m_iTileStartX;
	m_iTileStartY = g_pSceneMgr->m_iTileStartY;
	m_iDrawOffX   = g_pSceneMgr->m_iDrawOffX;
	m_iDrawOffY   = g_pSceneMgr->m_iDrawOffY;
	m_iScrWidth   = g_pSceneMgr->m_iScrWidth;
	m_iScrHeight  = g_pSceneMgr->m_iScrHeight;

	m_pMagicFloor = NULL;							//地表魔法暂存
	m_pMagicSky= NULL;								//空中魔法暂存

	Magic_Show_s* show = g_pMagicCtrl->GetShowList();	

	while(show)
	{
		Magic_Show_s* p = show->pNext;
		if (show->show_node)
		{
			ShowMagic(show);	//可能删除；其next指针不可用。
		}
		show = p;
	}
}

//算法描述：基于状态帧和多叉树结构的魔法系统
//1.起始条件：快捷键 + 服务端通知
//2.运行条件：魔法本地脚本控制：具体参见魔法编辑器使用指南。
//3.结束条件：服务端通知+本地控制。
//4.特殊启动：直接滤过n个节点;这样可以启动frozen的中间状态，避免人物来回行走的时候，
//	在本机上看到frozen的临时创建和启动；当然，如果是攻击型魔法；主动攻击者必须可以
//	看到整个过程。所以，根据状态空间创建的魔法，必须是状态型的魔法。
//5.关于异常结束的处理：对于飞行技能，如果已经判定造成上伤害，应当先设置属性
//6.编辑器应当在每个攻击型魔法的适当节点，设置攻击伤害触发，以防止异常结束，而略过属性更新
//7.飞行攻击魔法一定要创建击中特效触发节点。
bool CMagicManager::ShowMagic(Magic_Show_s *ms)	
{
	int  x_off =  128;								//图片基偏移，匹配人物中心
	int  y_off =  152;								//图片基偏移，匹配人物中心

	if (NULL == ms )
		return false;

	if (NULL == ms->ref  || (ms->ref->attr & EMP_MAGIC_EXIT) ||  ms->ref->ref_count == 0 || NULL == ms->show_node )		//impossible
	{
		FinishMagic(ms);
		return false;
	}

	Magic_Unit_s* pUnit = g_pMagicCtrl->GetMagicUnit(ms->show_node->unit_idx);

	int x,y; 
	int X,Y,ox,oy;
	int iDelayFinish = 0;
	int iSelfX,iSelfY,iOtherX,iOtherY;
	bool bSelf = (ms->ref->uOwnerID == SELF.GetID()?true:false);
	float fScale = 1.0f;

	X = Y = ox = oy =0;
	x = y = iSelfX = iSelfY =iOtherX =iOtherY =0;

	//服务器允许成功结束,启动子节点
	//状态魔法[都为循环魔法]，服务端返回
	//由于只对loop节点作用，可以利用该特性实现循环魔法的结束效果！
	if ((ms->ref->attr & EMP_MAGIC_SERVER_SUCCESS) && (ms->show_node->attr & EMP_MAGIC_LOOP))	
	{
		//ms->ref->attr &= ~EMP_MAGIC_SERVER_SUCCESS;		//for muti-loop node,this will not finish ?? so craze!
		iDelayFinish = 1;
	}

	//帧数情况
	int   iFrameNow   = ms->GetFrame(m_dwTickCount);

	//人物隐藏
	Magic_Root_s* pRoot = g_pMagicCtrl->GetMagicRoot(ms->ref->wMagicID);
	if(pRoot && (pRoot->attr & EMP_MAGIC_HIDE_MYSELF))
	{
		int DisableDraw = 0;
		if(5 == iFrameNow)
		{
			DisableDraw = 2;
		}		
		else if(12 == iFrameNow)
		{
			DisableDraw = 1;
		}
		if(DisableDraw > 0)
		{
			UINT uOwnerID = ms->ref->uOwnerID;
			if(uOwnerID == SELF.GetID())
			{
				SELF.SetReserveData(pubDisableDraw,DisableDraw - 1);
			}
			else 
			{
				CSimpleCharacterNode * p = g_pGameData->FindSimpleCharacter(uOwnerID);
				if(p) 
				{
					p->SetReserveData(pubDisableDraw,DisableDraw - 1);
				}
			}
		}
	}

	//持续伤害效果
	if(m_dwTickCount == ms->tFrameStart)
	{
		if(ms->ref->pTarget )
		{
			if(ms->show_node->attr & EMP_MAGIC_ATTACKED)
			{
				EffectAttacked(ms->ref->pTarget);
			}
		}
		iOtherX = ms->ref->wOriginTileX;
		iOtherY = ms->ref->wOriginTileY;
	}

	//任何单循环节点的攻击伤害应当及时处理;
	if(ms->ref->pTarget && (ms->show_node->attr & EMP_MAGIC_LOOP) && !(ms->show_node->attr & EMP_MAGIC_FLYING))
	{
		if( ms->show_node->attr & EMP_MAGIC_ATTACKED )
		{
			EffectAttacked(ms->ref->pTarget);
		}
	}

	//超过显示帧
	if (ms->IsEnd(m_dwTickCount))	//超过显示
	{
		//判断当前魔法是否在等待服务器返回消息
		if(ms->ref->attr & EMP_MAGIC_SERVER_WAIT)
		{
			if((m_dwTickCount - ms->tFrameStart > 5000)  || (!SELF.IsWaitServer()))		// 服务器太长的时间没响应，直接结束魔法
			{
				FinishMagic(ms);
				return false;
			}
			//如果允许循环,重新开始
			if((ms->show_node->attr & EMP_MAGIC_LOOP) != 0) 
				ms->tFrameStart = m_dwTickCount;
		}
		else
		{
			//如果不允许循环,结束;
			if((ms->show_node->attr & EMP_MAGIC_LOOP) == 0) 
			{
				iDelayFinish = 1;
			}
		}
	}

	//节点主属性
	if (ms->show_node->attr & EMP_MAGIC_MYSELF)
	{
		GetXYFromID(ms->ref->uOwnerID,X,Y,ox,oy);	
		if (X == -1 || Y == -1)
		{
			FinishMagic(ms);
			return false;
		}
		x = (X - m_iTileStartX) * TILE_WIDTH + ox - m_iDrawOffX;
		y = (Y - m_iTileStartY) * TILE_HEIGHT + oy - m_iDrawOffY ;

		if(ms->tFrameStart == m_dwTickCount)
		{
			iOtherX = X;
			iOtherY = Y;
		}

		//节点附加属性----特殊效果魔法：
		if(1 == iDelayFinish )
		{
			CreateMagic(ms);
			FinishMagic(ms);
			return false;
		}
	}
	else if (ms->show_node->attr & EMP_MAGIC_OBJECT)
	{
		//通常不支持hit,仅作为结束帧或者攻击后效果；
		GetXYFromID(ms->ref->uTargetID,X,Y,ox,oy);	
		if (X == -1 || Y == -1)
		{
			if(ms->show_node->attr & EMP_MAGIC_LOOP) 
			{
				iDelayFinish = 1;
			}
			x = (ms->ref->wTargetTileX - m_iTileStartX)*TILE_WIDTH - m_iDrawOffX + (int)ms->fx ;
			y = (ms->ref->wTargetTileY - m_iTileStartY)*TILE_HEIGHT - m_iDrawOffY + (int)ms->fy ;

			if(ms->tFrameStart == m_dwTickCount)
			{
				iOtherX = ms->ref->wTargetTileX;
				iOtherY = ms->ref->wTargetTileY;
			}
		}
		else
		{
			x = (X - m_iTileStartX)*TILE_WIDTH + ox - m_iDrawOffX;
			y = (Y - m_iTileStartY)*TILE_HEIGHT + oy - m_iDrawOffY;

			if(ms->show_node->attr & EMP_MAGIC_ADJUST_OBJECT)
			{
				CSimpleCharacterNode * pChar = g_pGameData->FindSimpleCharacter(ms->ref->uTargetID);
				if(pChar && pChar->GetRaceType() == CHARACTER_MONSTER)
				{
					if(pChar->GetTexH() < 100)
					{
						y += 80 - pChar->GetTexH() / 2;
					}
				}
			}

			if(ms->tFrameStart == m_dwTickCount)
			{
				iOtherX = X;
				iOtherY = Y;
			}
			//缩放比例
		}
	}
	else if (ms->show_node->attr & EMP_MAGIC_FLYING)
	{
		if(!DealMagicFlying(ms,iDelayFinish))
		{
			FinishMagic(ms);
			return false;
		}

		x = (ms->ref->wOriginTileX - m_iTileStartX)*TILE_WIDTH - m_iDrawOffY + (int)ms->fx ;
		y = (ms->ref->wOriginTileY - m_iTileStartY)*TILE_HEIGHT - m_iDrawOffY + (int)ms->fy ;

		POINT ptOffset = CMagicFlyingBeginOffset::GetCenterOffset(ms->ref->wMagicID,ms->iData3);
		x -= ptOffset.x;
		y -= ptOffset.y;

		//仅飞行魔法可以删除
		if( x<-MAX_WINDOW_LIMIT || y<-MAX_WINDOW_LIMIT
			|| x>=m_iScrWidth+MAX_WINDOW_LIMIT || y>=m_iScrHeight+MAX_WINDOW_LIMIT) // 魔法是在可显示范围之外
		{
			FinishMagic(ms);
			return false;
		}
	}
	else if (ms->show_node->attr & EMP_MAGIC_STATIC)
	{
		//对应于原魔法系统的攻击对象
		//通常不支持hit,仅作为结束帧或者攻击后效果；
		SELF.GetXY(X,Y);
		SELF.GetOffset(ox,oy);
		WORD wRefX,wRefY;

		int iattr2 = (ms->show_node->attr >> 16) & 0xFF;
		switch(iattr2)
		{
		case 3:
			wRefX = ms->ref->wOriginTileX;
			wRefY = ms->ref->wOriginTileY;
			break;
		default :
			wRefX = ms->ref->wTargetTileX;
			wRefY = ms->ref->wTargetTileY;
			break;
		}

		if(ms->tFrameStart == m_dwTickCount)
		{
			iOtherX = ms->ref->wTargetTileX;
			iOtherY = ms->ref->wTargetTileY;
		}

		x = (wRefX - m_iTileStartX)*TILE_WIDTH - m_iDrawOffX + (int)ms->fx ;
		y = (wRefY - m_iTileStartY)*TILE_HEIGHT - m_iDrawOffY + (int)ms->fy ;


		if(X-wRefX > MAX_SERVER_NOTIFY_X || X-wRefX < -MAX_SERVER_NOTIFY_Y|| Y-wRefY > MAX_SERVER_NOTIFY_Y|| Y-wRefY < -MAX_SERVER_NOTIFY_Y)
		{
			FinishMagic(ms);
			return false;
		}

		//足够的高度和宽度来删除
		if( x<-MAX_WINDOW_LIMIT || y<-MAX_WINDOW_LIMIT
			|| x>=m_iScrWidth+MAX_WINDOW_LIMIT || y>=m_iScrHeight+MAX_WINDOW_LIMIT) // 魔法是在可显示范围之外
		{
			FinishMagic(ms);
			return false;
		}
	}

	//音效
	if((ms->tFrameStart == m_dwTickCount) && (ms->show_node->attr & EMP_MAGIC_MUSIC_LOOP))
	{
		SELF.GetXY(iSelfX,iSelfY);
		if(iSelfX && iSelfY && iOtherX && iOtherY)
			g_pAudio->PlayEx(EAT_MAGIC,pUnit->sound,g_nRand++,iSelfX,iOtherX,iSelfY,iOtherY,bSelf,1);
		ms->nSoundRand = g_nRand;					//保存播放id;用于控制
	}

	//在某些特殊情况下；ms->iData3禁止切换
	int dirs = 0;
	if (ms->iData3> 0 && pUnit->dirs > 1)	//仅当图片序列存在多方向[8 | 16 = (7 | 15)]，才支持按方向取图
	{
		if((pUnit->dirs+1) < 16)				//判断是否为8方向的图
			dirs =(((ms->iData3 )>> 1) % (pUnit->dirs+1)); //8 
		else 
			dirs =(ms->iData3 % (pUnit->dirs+1));
	}
	//仅飞行魔法可以删除
	if( x<-MAX_WINDOW_LIMIT || y<-MAX_WINDOW_LIMIT|| x>= m_iScrWidth+MAX_WINDOW_LIMIT || y>=m_iScrHeight+MAX_WINDOW_LIMIT) // 魔法是在可显示范围之外
	{
		if(1 == iDelayFinish )
		{
			CreateMagic(ms);
			FinishMagic(ms);
			return false;
		}
	}
	x +=  pUnit->offx- x_off;
	y +=  pUnit->offy- y_off;

	//根据节点的属性来创建显示列表
	tex_show_t * tex = CreateTexShow();
	if (tex)
	{
		//TexID是本pUnit的开始值，而FrameIndex则对应原来的帧偏移；
		int outDir= 0,bMapping= 0;
		DirMapping(dirs,pUnit->dirs,outDir,bMapping);

		//预读图片
		int iBegin = pUnit->tex + outDir*pUnit->frames;
		g_TexMgr.PreLoad(PACKAGE_MAGIC1+pUnit->package,iBegin,iBegin+pUnit->framereal);

		tex->wTexID	= pUnit->tex + outDir*pUnit->frames+iFrameNow;
		tex->wPackage = pUnit->package;
		tex->wAdd	= pUnit->alpha;
		tex->x		= x;
		tex->y		= y;
		tex->dwAlpha	= 0xFFFFFFFF;

		//如果设置了比例参数
		if((fScale!=1.0) && (ms->show_node->attr & EMP_MAGIC_SCALE))
		{
			tex->iType = EST_SCALE_CENTER;
		}

		PushTexStore(ms ,tex,pUnit->offx - x_off);	//暂存!!!
	}

	if(1 == iDelayFinish ) 
	{	
		//首先判断是否为结束型触发伤害
		if(ms->ref->pTarget && (ms->show_node->attr & EMP_MAGIC_POST_ATTACKED) )
		{
			EffectAttacked(ms->ref->pTarget);
		}

		CreateMagic(ms);
		FinishMagic(ms);
		return false;
	}
	return true;
}

//------------------------------------------------------------------------------
// Purpose : 处理节点飞行
// Input   :
// Output  :
//------------------------------------------------------------------------------
bool CMagicManager::DealMagicFlying(Magic_Show_s *ms,int &iDelayFinish)	
{
	int X,Y,ox,oy;
	GetXYFromID(ms->ref->uTargetID,X,Y,ox,oy);	

	//根据攻击方向来计算--需要计算是否爆炸、是否到达目的
	bool  bUseTargetTile = false;
	float fSpeed = 2.0f;

	//计算初始偏移
	if(ms->tFrameStart == m_dwTickCount)
	{
		POINT ptOffset = CMagicFlyingBeginOffset::GetCenterOffset(ms->ref->wMagicID,ms->iData3);
		ms->fx += ptOffset.x;
		ms->fy += ptOffset.y;
	}

	RefleshFlyObject(ms, fSpeed,bUseTargetTile);

	ms->fx += ms->fData1;
	ms->fy += ms->fData2;
	if (ms->fData1 == 0.0 && ms->fData2 == 0.0)
	{
		iDelayFinish = 1;
	}
	else if(!ms->iData1 && WillBlast(ms))	
	{
		if((ms->show_node->attr & EMP_MAGIC_HIT))
		{
			//创建儿子节点，结束自身节点			
			if(X != -1 && Y != -1)
			{
				iDelayFinish = 1;
			}
			else
			{	//当无物标、具有击中效果，且hit；需要判断EMP_MAGIC_OFFSCREEN
				if(!(ms->show_node->attr & EMP_MAGIC_OFFSCREEN))
				{
					iDelayFinish = 1;
				}
			}
		}
		else if(ms->iData4 == 1)
		{
			CreateMagic(ms);
		}
		ms->iData1 = 1;		//
	}
	return true;
};

void CMagicManager::RefleshFlyObject(Magic_Show_s *ms,float iSpeed,bool bUseTargetTile)//				
{
	if(iSpeed <= 0.0000001f)
		iSpeed = 1.0f;								// 飞行每个格子需要的帧数
	if(ms->iData4 && !bUseTargetTile)			// 没目标ID的飞行魔法，并且不强制使用target坐标；直接返回
		return;

	// 计算像素点间距
	int iX,iY,ox,oy,iAX,iAY;


	GetXYFromID(ms->ref->uTargetID,iX,iY,ox,oy);
	if(iX == -1 || iY == -1 )	//						// 没有目标，使用坐标
	{
		iAX = (ms->ref->wTargetTileX - ms->ref->wOriginTileX) * TILE_WIDTH ;
		iAY = (ms->ref->wTargetTileY - ms->ref->wOriginTileY) * TILE_HEIGHT;
		ms->iData4 = 1;        

		if(bUseTargetTile)	//终止于wTargetTileX，wTargetTileY位置
		{
			iAX -= (int)ms->fx;
			iAY -= (int)ms->fy;
		}
	}
	else												// 使用目标坐标
	{
		iAX = (iX - ms->ref->wOriginTileX) * TILE_WIDTH + ox - (int)ms->fx; 
		iAY = (iY - ms->ref->wOriginTileY) * TILE_HEIGHT + oy - (int)ms->fy; 
		ms->iData4 = 0;
	}


	// 转换到斜视角
	iAX /= 2;

	// 根据间距计算下一帧飞行的距离
	double atom = sqrt((double)(iAX * iAX + iAY * iAY)) * 1.5;
	if(atom == 0.0)
	{
		ms->fData1 = 0;
		ms->iData2 = 0;
		//ms->iMagicState = 2;	// 已经应该爆炸了
	}
	else
	{

		ms->fData1 = (float)((TILE_WIDTH / iSpeed) * iAX / atom);
		ms->fData2 = (float)((TILE_HEIGHT / iSpeed) * iAY / atom);

		if(ms->tFrameStart == m_dwTickCount)
		{
			if(IsHumanByID(ms->ref->uOwnerID))
				ms->iData3 = GetDir16(iAX,iAY,0,0);
		}
	}
};


//------------------------------------------------------------------------------
// Purpose : 判断飞行体是否要击中某个物标
// Input   : 
// Output  :
//------------------------------------------------------------------------------
bool CMagicManager::WillHitOn(Magic_Show_s *ms,UINT uTargetID)
{
	int iX,iY;
	if(uTargetID)
	{
		int ox,oy;
		GetXYFromID(uTargetID,iX,iY,ox,oy);
		if(iX == -1 || iY == -1)
			return false;

		iX = (iX  - ms->ref->wOriginTileX) * TILE_WIDTH - (int)ms->fx + ox ;
		iY = (iY  - ms->ref->wOriginTileY) * TILE_HEIGHT - (int)ms->fy + oy ;
	}
	if(iX*iX/4+iY*iY <= 1600)
		return true;
	return false;
};										

//------------------------------------------------------------------------------
// 判断是否要爆炸/击中
bool CMagicManager::WillBlast(Magic_Show_s *ms)	
{
	int iX,iY;
	if(ms->ref->wOriginTileX == ms->ref->wTargetTileX && ms->ref->wOriginTileY == ms->ref->wTargetTileY)
	{
		return true;;
	}
	if(ms->ref->uTargetID)
	{
		int ox,oy;
		GetXYFromID(ms->ref->uTargetID,iX,iY,ox,oy);
		if(iX == -1 || iY == -1)
			return false;
		iX = (iX  - ms->ref->wOriginTileX) * TILE_WIDTH - (int)ms->fx + ox;
		iY = (iY  - ms->ref->wOriginTileY) * TILE_HEIGHT - (int)ms->fy + oy;
	}
	else
	{
		iX = (ms->ref->wTargetTileX - ms->ref->wOriginTileX) * TILE_WIDTH - (int)ms->fx + TILE_WIDTH / 2;
		iY = (ms->ref->wTargetTileY - ms->ref->wOriginTileY) * TILE_HEIGHT - (int)ms->fy + TILE_HEIGHT / 2;
	}
	if(iX*iX/4+iY*iY <= 1600)
		return true;
	return false;

};

void CMagicManager::PushTexStore(Magic_Show_s *ms,tex_show_t *tex,int offy)
{
	DWORD attr = ms->show_node->attr;
	if(attr & EMP_MAGIC_FLOOR)
	{
		push_tex_store(&m_pMagicFloor,tex);
	}
	else if (attr & EMP_MAGIC_AIR)
	{
		push_tex_store(&m_pMagicSky,tex);
	}
	else 
	{
		UINT uOwnerID = ms->ref->uOwnerID;
		if(attr & EMP_MAGIC_OBJECT)
		{
			uOwnerID = ms->ref->uTargetID;
		}

		//根据owner ID,获得其指针
		if (attr & EMP_MAGIC_BACK)
		{
			if(SELF.GetID() == uOwnerID)
			{
				SELF.PushTexBack(tex);
			}
			else
			{
				CSimpleCharacterNode * p = g_pGameData->FindSimpleCharacter(uOwnerID);
				if(p) 
				{
					p->PushTexBack(tex);
				}
				else 
				{
					push_tex_store(&m_pMagicSky,tex);
				}
			}
		}
		else if (attr & EMP_MAGIC_FRONT)
		{
			if(SELF.GetID() == uOwnerID)
			{
				SELF.PushTexFront(tex);
			}
			else
			{
				CSimpleCharacterNode * p = g_pGameData->FindSimpleCharacter(uOwnerID);
				if(p) 
				{
					p->PushTexFront(tex);
				}
				else
				{
					push_tex_store(&m_pMagicSky,tex);
				}
			}
		}
		else 
		{
			push_tex_store(&m_pMagicSky,tex);
		}
	}//人物所属的tex
};

//------------------------------------------------------------------------------
// 结束魔法显示
void CMagicManager::FinishMagic(Magic_Show_s *ms)	
{
	g_pMagicCtrl->FinishMagic(ms);
};

Magic_Show_s* CMagicManager::CreateMagic(Magic_Show_s *ms)
{
	return g_pMagicCtrl->CreateMagic(ms);
}

//根据取得人物的ID取得它的砖块坐标
void CMagicManager::GetXYFromID(DWORD dwID,int &X,int &Y,int &ox,int &oy)
{
	if(dwID != 0)
	{
		if(dwID == SELF.GetID())
		{
			SELF.GetXY(X,Y);
			SELF.GetOffset(ox,oy);
		}
		else 
		{
			CSimpleCharacterNode * p = g_pGameData->FindSimpleCharacter(dwID);
			if(p) 
			{
				p->GetXY(X,Y);		
				p->GetOffset(ox,oy);
			}
			else
			{
				X = Y = -1;
				ox = oy = 0;
			}
		}
	}
	else
	{
		X = Y = -1;
		ox = oy = 0;
	}
	ox += TILE_WIDTH / 2;
	oy += TILE_HEIGHT / 2;
}

bool CMagicManager::IsHumanByID(UINT uID)
{
	if(uID == SELF.GetID())
	{
		return true;
	}
	else
	{
		CSimpleCharacterNode* pChar = g_pGameData->FindSimpleCharacter(uID);
		if(pChar)
		{
			return (pChar->GetRaceType() == CHARACTER_HUMAN);
		}
	}
	return false;
}

void CMagicManager::DrawSwearLight(int iX,int iY,CSimpleCharacterNode* pChar)
{
	//不需要绘制刀光的动作
	SAction* pAction = NULL;
	if(pChar)
	{
		pAction = &pChar->GetAction();
		if(pAction == NULL)
			return;
	}
	else
	{
		pAction = &SELF.GetAction();

		iX -= TILE_WIDTH / 2;
		iY -= TILE_HEIGHT / 2 + 12;
	}

	if(pAction->wAction != ACTION_ATTACK1 || (pAction->uFlag & MAGIC_ACTION) == 0)
		return;

	int x,y;
	LPTexture pTex;	 
	int iSelfX,iSelfY,iOtherX,iOtherY; 
	bool bSelf = false;
	BYTE cDir = DIR_NO;

	SELF.GetXY(iSelfX,iSelfY);
	if (pChar == NULL)
	{
		SELF.GetXY(iOtherX,iOtherY);
		bSelf = true;
		cDir = SELF.GetDir();
	}
	else
	{
		pChar->GetXY(iOtherX,iOtherY);
		cDir = pChar->GetDir();
	}

	x = iX - 96;
	y = iY - 132;

	Magic_Unit_s* pUnit = g_pMagicCtrl->GetMagicUnitByRoot(pAction->iData);

	if (pUnit)
	{
		int dirOut = 0,bMapping = 0;
		DirMapping(cDir,pUnit->dirs,dirOut,bMapping);

		int iFrameNow = pAction->GetFrame(m_dwTickCount);
		WORD wTexID	= pUnit->tex + dirOut*pUnit->frames+iFrameNow;

		pTex = g_TexMgr.GetTex(PACKAGE_MAGIC1+pUnit->package,wTexID);

		//预读
		int iBegin = pUnit->tex + dirOut*pUnit->frames;
		g_TexMgr.PreLoad(PACKAGE_MAGIC1+pUnit->package,iBegin,iBegin+pUnit->framereal);

		if(pTex) 
		{
			g_Gfx.SetRenderMode((RenderMode)pUnit->alpha);
			g_Gfx.DrawTextureNL(x,y,pTex);
			g_Gfx.SetRenderMode();
		}

		if (m_dwTickCount == pAction->tFrameStart)
		{
			g_pAudio->PlayEx(EAT_MAGIC,pUnit->sound,g_nRand++,iSelfX,iOtherX,iSelfY,iOtherY,bSelf,1);
		}
	}
}