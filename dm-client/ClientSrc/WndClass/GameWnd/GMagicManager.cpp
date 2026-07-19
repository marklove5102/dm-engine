#include "GameControl/GameControl.h"
#include "GMagicManager.h"
#include "GameData/MagicCtrlMgr.h"
#include "GameData/GameData.h"
#include "GameData/GameGlobal.h"
#include <math.h>
#include "GSceneManager.h"
#include "Global/Interface/AudioInterface.h"
#include "GameAI/AIMgr.h"
#include "GameData/OtherData.h"
#include "BaseClass/Control/Control.h"
#include "GameData/NpcData.h"
#include "GameData/TalkMgr.h"

CMagicManager* g_pMagicMgr = NULL;

//魔法管理器
#define MAX_WINDOW_LIMIT 1024					//最大窗口限制尺度
#define MAX_SERVER_NOTIFY_X 13					//服务端通知x范围
#define MAX_SERVER_NOTIFY_Y 14					//服务端通知Y范围

CMagicManager::CMagicManager(void)
{
	m_pMagicFloor	= NULL;		//地表魔法暂存
	m_pMagicSky		= NULL;		//空中魔法暂存
	m_dwTickCount   = 0;
	g_pMagicMgr     = this;
}

CMagicManager::~CMagicManager(void)
{
}

tex_show_t* CMagicManager::CreateTexShow()
{
	tex_show_t * pshow = NEW_TEX_SHOW();
	if (pshow)
	{
		memset(pshow,0,sizeof(tex_show_t));
		pshow->fScaleX = 1.f;
		pshow->fScaleY = 1.f;

		return pshow;
	}
	return NULL;
}

void CMagicManager::DealMagic()
{
	if(g_pSceneMgr == NULL)
		return;

	m_dwTickCount = GetTickCount();

	m_iTileStartX = g_pSceneMgr->m_iTileStartX;
	m_iTileStartY = g_pSceneMgr->m_iTileStartY;
	m_iDrawOffX   = g_pSceneMgr->m_iDrawOffX;
	m_iDrawOffY   = g_pSceneMgr->m_iDrawOffY;
	m_iScrWidth   = g_pSceneMgr->m_iScrWidth;
	m_iScrHeight  = g_pSceneMgr->m_iScrHeight;

	//m_pMagicFloor = NULL;							//地表魔法暂存
	//m_pMagicSky= NULL;								//空中魔法暂存
	clear_tex_store(&m_pMagicFloor);
	clear_tex_store(&m_pMagicSky);

	Magic_Show_s* show = g_pMagicCtrl->GetShowList();	

	while(show)
	{
		Magic_Show_s* p = show->pNext;
		ShowSpecialMagic(show);
		show = p;
	}
}

bool CMagicManager::ShowSpecialMagic(Magic_Show_s *show)
{
	if (NULL == show )
		return false;

	if(!show->pUnit)
		return ShowMagic(show);

	switch(show->pUnit->id)
	{
	case MAGICID_ICE_SWORD_ATTACKED://寒冰剑
	case MAGICID_TONGTIAN_FLY_ATTACK://通天教主的全屏攻击
	case MAGICID_WXXG_MONSTER_SPIRIT_FLY:
	case MAGICID_TIGER_SPRAY:
		DealCycleFlower(show);
		break;
	case MAGICID_FALL_OBJECT://丟物品到地上
		DealDropGoodEffect(show);
		break;
	case MAGICID_SHAKE_SCREEN://屏幕抖动的魔法单元，Cycles表示抖动的时间,单位100毫秒
		g_AIMgr.StartShake(show->iCycles*100);
		FinishMagic(show);
		break;
	case MAGICID_CREATE_NEW_MAGIC://创建新的魔法，在用到这个魔法单元的魔法中用Cycles表示要创建的魔法ID
	case MAGICID_CREATE_NEW_MAGIC_FABAO:
		DealNewMagicUnit(show);
		break;
	case MAGICID_OPEN_NEW_WND:
		DealOpenNewWnd(show);
		break;            
	case MAGICID_SKYROCKET:  // 处理焰火粒子
		DealSkyrocket(show);
		break;
	case MAGICID_FESTAL_SKYROCKET: //处理节日焰火
		DealFestalSkyrocket(show);
		break;
	case MAGICID_STREW_SKYROCKET:  //处理背景焰火
		DealStrewSkyrocket(show);
		break;
	case MAGICID_SENCER_EFFECT://处理香燃烧的效果
		DrawCenserEffect(show);
		break;
	case MAGICID_SPARKLE:  //处理小火球后面的火花
		DealSparkle(show);
		break;
	case 2:   //小火球飞行魔法单元，同时创建小火球后面的火花
	case 7:   //火炎刀飞行魔法单元，同时创建后面的火花
		CreateSparkle(show);
		ShowMagic(show);
		break;
	case 101: //强化小火球飞行魔法单元，同时创建后面的火花
		DealSuperFireBall(show);
		break;
	case 104: //强化施毒术飞行魔法单元,主骷髅
	case 4644: //强化施毒术飞行魔法单元,骷髅1(白)
	case 4645: //强化施毒术飞行魔法单元,骷髅2(黑)
		DealSuperPoison(show);
		break;
	case MAGICID_GRADUAL_APPEAR://渐隐魔法单元,在用到这个魔法单元的魔法中用Cycles表示渐隐速度
		DealGradualAppear(show);
		break;
	case 318: //烟花筒播放特效
		DealYanHuaTong(show);
		ShowMagic(show);
		break;
	case MAGICID_TARGET_OWNER_CHANGE:
		DealExchangeID(show);
		break;
	case 4659:
	case 4756:
		DealSpirtMonsterMsg(show);
		ShowMagic(show);
		break;
	case 4694:
		DealMagicDulingbo(show);
		break;	
	case MAGICID_HIDE:
		DealMagicHide(show);
		break;	
	case MAGICID_SHOW:
		DealMagicShow(show);
		break;	
	case MAGICID_SET_MAGIC_STATE:
		SetMagicState(show);
		break;
	case MAGICID_HIDE_FABAO:
		SetFaBaoState(show);
		break;
	/*case MAGICID_TEXTONHEAD:
		TextOnHead(show);
		break;*/
	case MAGICID_SEND_MESSAGE:
		DealSendMessage(show);
		break;
	case 408:
		DealThrowStone(show);
		break;		
	case 49:		//蝙蝠
	case 81:		//遁地
	case 84:		//移形换影
	case 356:		//主体瞬移至魔法阵祭坛
	case 109:       //心魔移形换影
	case 616:		//八方分影斩fly
		DealOwnerFly(show);		//处理自己随同魔法一起移动的函数
		break;
	case MAGICID_HEIYANMO_REFRACTION:
		DealHeiYanMoRefraction(show);
		break;
	case 5063://飞升台中心的圈,站上了人就不画
		DealFlyTowerCycle(show);
		break;
	case MAGICID_POPUP_PAOPAO://冒泡
		DealPopupPaoPao(show);
		break;
	case 96://火毒攻心剑起手
		DealDaoFighterPoison(show);
		break;
	case MAGICID_ALTERNATIVE:
		DealAlternativeMagic(show);
		break;
	case 5251://三味真火云彩,角色向上漂
		DealSanWeiUp(show);
		break;
	case 50015:
		DealShadowKill8Start(show);
		break;
	case 611:
		DealFireShield(show);		
		break;
	case 5558:
		DealFaJueState(show);
		break;
	case MAGICID_LIANJI_ZHUANGJI:
		DealLianJiZhuangji(show);		
		break;
	default:
		return ShowMagic(show);//可能删除；其next指针不可用。
		break;
	}


	return true;

}


void CMagicManager::GetDevByMagicDir(int iMagicID,int iDir,int& ix,int& iy)
{	
	switch(iMagicID)
	{
		case MAGICID_ZHONGLEI_EFFECT_HIT:
			GetDevAccordDir(iDir,ix,iy);
			break;
		default:
			ix = iy = 0;
			break;
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
	if (NULL == ms )
		return false;

	int  x_off =  128;								//图片基偏移，匹配人物中心
	int  y_off =  152;								//图片基偏移，匹配人物中心
	int iTileStartX = m_iTileStartX,iTileStartY = m_iTileStartY;
	int iDrawOffX = m_iDrawOffX,iDrawOffY = m_iDrawOffY;

	if(ms->attr & EMP_MAGIC_SCREEN)//有该属性的魔法相对于屏幕左上角坐标为零点,在屏幕上，不会随着地图移动
	{
		iTileStartX = iTileStartY = 0;
		iDrawOffX = iDrawOffY = 0;
		x_off = y_off = 0;
	}

	Magic_Unit_s* pUnit = ms->pUnit;

	if (NULL == ms->ref  || (ms->ref->attr & EMP_MAGIC_EXIT) ||  ms->ref->ref_count == 0 || NULL == ms->show_node || !pUnit)//impossible
	{
		FinishMagic(ms);
		return false;
	}


	int x,y; 
	int X,Y,ox,oy;
	int iDelayFinish = 0;
	bool bSelf = (ms->ref->uOwnerID == SELF.GetID()?true:false);
	float fScale = 1.0f;

	X = Y = ox = oy =0;
	x = y = 0;

	//服务器允许成功结束,启动子节点
	//状态魔法[都为循环魔法]，服务端返回
	//由于只对loop节点作用，可以利用该特性实现循环魔法的结束效果！
	if ((ms->attr & EMP_MAGIC_SERVER_SUCCESS) && (ms->attr & EMP_MAGIC_LOOP))	
	{
		iDelayFinish = 1;
	}

	//帧数情况
	int iFrameNow   = ms->GetFrame(m_dwTickCount);

	//持续伤害效果
	if(ms->ref->pTarget && (ms->attr & EMP_MAGIC_ATTACKED))
	{
		EffectAttacked(ms->ref->pTarget);
	}
	//超过显示帧
	if(ms->IsEnd(m_dwTickCount))	//超过显示
	{
		//判断当前魔法是否在等待服务器返回消息
		if(ms->attr & EMP_MAGIC_WAIT_SERVER)
		{
			if(m_dwTickCount - ms->dwStartTime > 5000)		// 服务器太长的时间没响应，直接结束魔法
			{
				FinishMagic(ms);
				return false;
			}
		}		
        else if(ms->attr & EMP_MAGIC_LOOP)//无限循环,重新开始 
		{
			if((ms->attr & EMP_MAGIC_END_TIME) && GetTickCount() - ms->dwStartTime > ms->iCycles * 100)//有时间限制,ms->iCycles表示时间,以100毫秒为单位
			{
				iDelayFinish = 1;
			}
			else
			{
				ms->tFrameStart = m_dwTickCount;
				iFrameNow = 0;
			}
			if(ms->iCurCycle < 65535)
				ms->iCurCycle ++;
		}
		else if(ms->iCycles > 0)//有限循环,重新开始 
		{
			ms->iCurCycle ++;
			if(ms->iCurCycle >= ms->iCycles)
			{
				ms->iCurCycle = 0;
				iDelayFinish = 1;
			}
			else
			{
				ms->tFrameStart = m_dwTickCount;
				iFrameNow = 0;
			}
		}
		else//如果不允许循环,结束;
		{
			iDelayFinish = 1;
		}

		//配了EMP_MAGIC_HIT_TARGET，没有EMP_MAGIC_HIT_MOUSE属性且目标不存在时不创建击中效果的子结点
		//例:78 怒斩天下-86-87-222 如果有击中目标时会在目标身上放击中效果 怒斩天下-86-87-222-329,否则直接结束
		//配了EMP_MAGIC_FLYING，没有目标时也想要在wTargetTileX，wTargetTileY处创建击中效果的子结点，不要配EMP_MAGIC_HIT_TARGET，或同时配上EMP_MAGIC_HIT_TARGET及EMP_MAGIC_HIT_MOUSE
		//如15 神圣战甲术-220 如果有目标则在目标身上爆炸,没有则在鼠标处爆炸
		if(iDelayFinish == 1 && (ms->attr & EMP_MAGIC_HIT_TARGET) && !(ms->attr & EMP_MAGIC_HIT_MOUSE))
		{
			int itx,ity,iTox,itoy;
			GetXYFromID(ms->ref->uTargetID,itx,ity,iTox,itoy);	
			if(itx == -1 || ity == -1)
			{
				FinishMagic(ms);
				return false;
			}
		}
	}
	
	//节点主属性
	if (ms->attr & EMP_MAGIC_MYSELF)
	{
		GetXYFromID(ms->ref->uOwnerID,X,Y,ox,oy);	
		if (X == -1 || Y == -1)
		{
			//if(m_dwTickCount - ms->tFrameStart > 1000)
			FinishMagic(ms);
			return false;
		}
		x = (X - iTileStartX) * TILE_WIDTH + ox - iDrawOffX;
		y = (Y - iTileStartY) * TILE_HEIGHT + oy - iDrawOffY ;
	}
	else if (ms->attr & EMP_MAGIC_OBJECT)
	{
		//通常不支持hit,仅作为结束帧或者攻击后效果；
		if(ms->ref->uTargetID == 0)
		{
			if (ms->ref->wTargetTileX == 0 || ms->ref->wTargetTileY == 0)
			{
				iDelayFinish = 1;
			}
			else
			{
				x = (ms->ref->wTargetTileX - iTileStartX)*TILE_WIDTH - iDrawOffX + TILE_WIDTH / 2 ;
				y = (ms->ref->wTargetTileY - iTileStartY)*TILE_HEIGHT - iDrawOffY + TILE_HEIGHT / 2 ;
			}
		}
		else
		{
			GetXYFromID(ms->ref->uTargetID,X,Y,ox,oy);	
			if (X == -1 || Y == -1)
			{
				iDelayFinish = 1;
				x = (ms->ref->wTargetTileX - iTileStartX)*TILE_WIDTH - iDrawOffX + TILE_WIDTH / 2 ;
				y = (ms->ref->wTargetTileY - iTileStartY)*TILE_HEIGHT - iDrawOffY + TILE_HEIGHT / 2 ;
			}
			else
			{
				x = (X - iTileStartX)*TILE_WIDTH + ox - iDrawOffX;
				y = (Y - iTileStartY)*TILE_HEIGHT + oy - iDrawOffY;

				if(ms->show_node->attr & EMP_MAGIC_ADJUST_OBJECT)
				{
					CSimpleCharacterNode * pChar = g_pGameData->FindSimpleCharacter(ms->ref->uTargetID);
					if(pChar && pChar->IsMonster())
		 			{
						if(pChar->GetTexH() < 100)
						{
							y += (80 - pChar->GetTexH()) / 2;
						}
					}
				}
			}
		}
	}
	else if (ms->attr & EMP_MAGIC_FLYING)
	{
		
		if(!DealMagicFlying(ms,iDelayFinish))
		{
			FinishMagic(ms);
			return false;
		}		

		x = (ms->ref->wOriginTileX - iTileStartX)*TILE_WIDTH - iDrawOffX + (int)ms->fx ;
		y = (ms->ref->wOriginTileY - iTileStartY)*TILE_HEIGHT - iDrawOffY + (int)ms->fy ;
	}
	else if (ms->attr & EMP_MAGIC_STATIC)
	{
		//对应于原魔法系统的攻击对象
		//通常不支持hit,仅作为结束帧或者攻击后效果；
        GetXYFromID(SELF.GetID(),X,Y,ox,oy);
        ox -= TILE_WIDTH / 2;
        oy -= TILE_HEIGHT / 2;

		WORD wRefX,wRefY;

		if(ms->attr & EMP_MAGIC_HIT_TARGET)//对于EMP_MAGIC_STATI魔法 配了EMP_MAGIC_HIT_TARGET这个属性以wOriginTileX作为魔法所在位置,否则以wTargetTileX作为魔法所在位置
		{
			wRefX = ms->ref->wOriginTileX;
			wRefY = ms->ref->wOriginTileY;
		}
		else
		{
			wRefX = ms->ref->wTargetTileX;
			wRefY = ms->ref->wTargetTileY;
		}

		if (wRefX == 0 || wRefY == 0)
		{
			iDelayFinish = 1;
		}
		else
		{
			int iDevX=0,iDevY=0,iDir = -1;	
			if(SELF.GetID() == ms->ref->uOwnerID)
			{
				iDir = SELF.GetDir();
			}
			else
			{
				CSimpleCharacterNode* pNode = g_pGameData->FindSimpleCharacter(ms->ref->uOwnerID);			
				if(pNode)	iDir = pNode->GetDir();
			}
			GetDevByMagicDir(ms->ref->wMagicID,iDir,iDevX,iDevY);		

			x = (wRefX - iTileStartX)*TILE_WIDTH - iDrawOffX + iDevX + TILE_WIDTH/2;
			y = (wRefY - iTileStartY)*TILE_HEIGHT - iDrawOffY + iDevY + TILE_HEIGHT/2;
		}
	}


	if(ms->wSound > 0 && (ms->iCurCycle == 0 || ms->show_node->attr & EMP_MAGIC_MUSIC_LOOP))
	{
		int iSelfX = 0,iSelfY = 0;
		Magic_Node_s *pNode = ms->show_node;

		int iOtherX = x / TILE_WIDTH + iTileStartX;
		int iOtherY = y / TILE_HEIGHT + iTileStartY;
		SELF.GetXY(iSelfX,iSelfY);

		if(ms->nSoundRand == 0)
		{
			ms->nSoundRand = g_pAudio->GetRand()++;					//保存播放id;用于控制
			g_pAudio->PlayEx(EAT_MAGIC,ms->wSound,ms->nSoundRand,iSelfX,iOtherX,iSelfY,iOtherY,bSelf,1,(ms->attr & EMP_MAGIC_MUSIC_LOOP) != 0);
		}
		else
		{
			g_pAudio->SetPan(EAT_MAGIC,ms->wSound,ms->nSoundRand,iSelfX,iOtherX,iSelfY,iOtherY);
		}
	}

	//必须放在if(ms->wSound > 0 && (ms->iCurCycle == 0 || ms->show_node->attr & EMP_MAGIC_MUSIC_LOOP))之后
	x +=  ms->iOffX -  x_off;
	y +=  ms->iOffY -  y_off;


	//根据节点的属性来创建显示列表
	bool bNeedDraw = true;
	if(ms->ref->uOwnerID == SELF.GetID())
	{
		if(SELF.GetReserveData(pubDisableDraw) == 1 && (ms->attr & EMP_MAGIC_NO_HIDE) == 0)
		{
			bNeedDraw = false;
		}
	}
	else
	{
		CSimpleCharacterNode * p = g_pGameData->FindSimpleCharacter(ms->ref->uOwnerID);
		if(p && p->GetReserveData(pubDisableDraw) == 1 && (ms->attr & EMP_MAGIC_NO_HIDE) == 0) 
		{
			bNeedDraw = false;
		}
	}

	//仅飞行魔法可以删除
	if( x < -MAX_WINDOW_LIMIT || y < -MAX_WINDOW_LIMIT || x>= m_iScrWidth + MAX_WINDOW_LIMIT || y >= m_iScrHeight + MAX_WINDOW_LIMIT) // 魔法是在可显示范围之外
	{
		if(ms->attr & EMP_MAGIC_FLYING)
		{
			FinishMagic(ms);
			return false;
		}

		//不是EMP_MAGIC_FLYING，也不是iDelayFinish的，不删除，只是不显示
		if(0 == iDelayFinish)
		{
			return false;
		}
	}	

	if (!iDelayFinish && bNeedDraw && iFrameNow < ms->iFrameReal)
	{

		tex_show_t * tex = CreateTexShow();
		if (tex)
		{
			//预读图片
			int iDir = ms->iDir>= 0?ms->iDir:0;
			int iBegin = pUnit->tex + iDir * ms->iFrameDir;

			if(ms->tFrameStart == m_dwTickCount)
			{
				if (ms->ref->wMagicID >= MAGICID_FIREBALL && ms->ref->wMagicID < MAGICID_GOLD_POWDER)
				{
					g_pTexMgr->PreLoad(PACKAGE_magic1+pUnit->package,iBegin,iBegin+pUnit->framereal,EP_SKILL);
				}
				else
				{
					g_pTexMgr->PreLoad(PACKAGE_magic1+pUnit->package,iBegin,iBegin+pUnit->framereal,EP_MAGIC);
				}
			}

			tex->wTexID	= iBegin + iFrameNow;
			tex->wPackage = pUnit->package;
			tex->wAdd	= ms->byAlpha;
			tex->x		= x;
			tex->y		= y;
			tex->dwColor	= ms->dwColor;

			//如果设置了比例参数
			if((fScale!=1.0) && (ms->show_node->attr & EMP_MAGIC_SCALE))
			{
				tex->iType = EST_SCALE_CENTER;
			}

			PushTexStore(ms,tex,0);	//暂存!!!
		}
	}

	if(1 == iDelayFinish ) 
	{	
		//首先判断是否为结束型触发伤害
		if(ms->ref->pTarget && (ms->show_node->attr & EMP_MAGIC_POST_ATTACKED) )
		{
			EffectAttacked(ms->ref->pTarget);
		}

		Magic_Show_s* pChild = CreateMagic(ms);
		ShowSpecialMagic(pChild);//创建完了子结点马上显示,否则中间会闪烁一下

		FinishMagic(ms);
		return false;
	}
	return true;
}

// 处理节点飞行
bool CMagicManager::DealMagicFlying(Magic_Show_s *ms,int &iDelayFinish)	
{
	int X,Y,ox,oy;
	GetXYFromID(ms->ref->uTargetID,X,Y,ox,oy);	

	//根据攻击方向来计算--需要计算是否爆炸、是否到达目的
	bool  bUseTargetTile = false;

	float fSpeed = (float)(ms->iFlySpeed) / 20;

	if(ms->show_node->attr & EMP_MAGIC_HIT_MOUSE)
		bUseTargetTile = true;

	//没有目标ID(dwTargetId == 0)，而且没有目标位置wTargetTileX==0&&wTargetTileY==0,的飞行魔法，直接根据方向往前飞
	if (ms->ref->uTargetID == 0 && ms->ref->wTargetTileX == 0 && ms->ref->wTargetTileY == 0)
	{
		float dx,dy;
		int iShowDir = ms->iDir;
		if(ms->pUnit && ms->pUnit->dirs > 0 && ms->pUnit->dirs < 16)//ms->iDir是按16方向得到的，如果pUnit->dirs小于16方向，把ms->iDir转化成pUnit对应的方向
		{
			iShowDir = iShowDir*16 / ms->pUnit->dirs % 16;			
		}
		
		GetDir16Step(iShowDir,dx,dy);
		

		//int dx,dy;
		//GetDirStep(ms->iDir,dx,dy);
		
		ms->fData1 = (float)TILE_WIDTH / fSpeed * dx;
		ms->fData2 = (float)TILE_HEIGHT / fSpeed * dy;
	}
	else 
		RefleshFlyObject(ms, fSpeed,bUseTargetTile);

	ms->fx += ms->fData1;
	ms->fy += ms->fData2;
	if (ms->pUnit->id == 101)//强化火球的加速追赶
	{
		ms->fx += (float)(ms->fData1 * ms->ref->wData[2]/5.0);
		ms->fy += (float)(ms->fData2 * ms->ref->wData[2]/5.0);
	}
	//if (ms->pUnit->id == 4644 || ms->pUnit->id == 4645)//强化施毒术的加速
	//{
	//	//偏移加速
	//	ms->ref->wData[1]++;
	//	ms->fx += ms->fData1*(0.50f+(float)ms->ref->wData[1]/8);
	//	ms->fy += ms->fData2*(0.50f+(float)ms->ref->wData[1]/8);
	//}

	if (ms->fData1 == 0.0 && ms->fData2 == 0.0)
	{
		iDelayFinish = 1;
	}
	else if(!ms->iData1 && WillBlast(ms))	
	{
		if((ms->show_node->attr & EMP_MAGIC_HIT_TARGET))
		{
			//创建儿子节点，结束自身节点			
			if(X != -1 && Y != -1)
				iDelayFinish = 1;
			else
			{
				if(bUseTargetTile)//无目标击中targettile且有EMP_MAGIC_HIT_MOUSE属性在此位置产生爆炸效果
					iDelayFinish = 1;
				else if((ms->show_node->attr & EMP_MAGIC_OFFSCREEN) == 0)//无目标击中targettile且没有EMP_MAGIC_HIT_MOUSE和EMP_MAGIC_OFFSCREEN属性不产生爆炸效果，直接结束魔法，如果有EMP_MAGIC_OFFSCREEN不爆炸，飞到屏幕外再结束（iDelayFinish＝0）
				{
					FinishMagic(ms);
					return false;
				}
				//else 一直飞到屏幕外结束
			}
		}
		else if(bUseTargetTile)
		{
			iDelayFinish = 1;
		}
		else if(ms->iData4 == 1)
		{
			CreateMagic(ms);
			FinishMagic(ms);
		}
		ms->iData1 = 1;
	}

	return true;
}

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
		iAX = (ms->ref->wTargetTileX - ms->ref->wOriginTileX) * TILE_WIDTH + TILE_WIDTH/2;
		iAY = (ms->ref->wTargetTileY - ms->ref->wOriginTileY) * TILE_HEIGHT + TILE_HEIGHT/2;
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
			if(IsHumanByID(ms->ref->uOwnerID) || (ms->attr & EMP_MAGIC_ADJUST_DIR))
			{
				int iDir = GetDir16(iAX,iAY,0,0);	 
				ms->iDir = iDir;
				if(ms->pUnit->dirs > 0 && ms->pUnit->dirs < 16)//ms->iDir是按16方向得到的，如果pUnit->dirs小于16方向，把ms->iDir转化成pUnit对应的方向
				{
					if(ms->pUnit->dirs == 8)
						ms->iDir = GetDir8(iAX,iAY,0,0);
					else
						ms->iDir = (iDir*ms->pUnit->dirs)/16 % ms->pUnit->dirs;
				}
			}
		}
	}
}


//判断飞行体是否要击中某个物标
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
}										

// 判断是否要爆炸/击中
bool CMagicManager::WillBlast(Magic_Show_s *ms)	
{
	int iX,iY;
	if(ms->ref->wOriginTileX == ms->ref->wTargetTileX && ms->ref->wOriginTileY == ms->ref->wTargetTileY)
	{
		return true;;
	}

	int ox,oy;
	GetXYFromID(ms->ref->uTargetID,iX,iY,ox,oy);
	if(iX == -1 || iY == -1)
	{
		iX = (ms->ref->wTargetTileX - ms->ref->wOriginTileX) * TILE_WIDTH - (int)ms->fx + TILE_WIDTH / 2;
		iY = (ms->ref->wTargetTileY - ms->ref->wOriginTileY) * TILE_HEIGHT - (int)ms->fy + TILE_HEIGHT / 2;
	}
	else
	{
		iX = (iX  - ms->ref->wOriginTileX) * TILE_WIDTH - (int)ms->fx + ox;
		iY = (iY  - ms->ref->wOriginTileY) * TILE_HEIGHT - (int)ms->fy + oy;
	}


	int iBlastDis = 500;
	switch(ms->ref->wMagicID)
	{
	case MAGICID_PROTECT_SYMBOL:
	case MAGICID_SHIELD_REFLECT:
	case MAGICID_GONGJIAN_HUANGSE_JIAQIANG:
	case MAGICID_GONGJIAN_HUANGSE_PUTONG:
	case MAGICID_GONGJIAN_LANSE_JIAQIANG:
	case MAGICID_GONGJIAN_LANSE_PUTONG:

		iBlastDis = 1200;
		break;
	//case MAGICID_ICE_ATTACK1:
	case MAGICID_FIRE_BALL_HORSE_ATTACK:
	case MAGICID_FIRE_BOSS_ATTACK1:
	case MAGICID_DAO_BLOOD_MONSTER_ATTACK:
	case MAGICID_FABAOATTACK_XILING_jin:
	case MAGICID_FABAOATTACK_XILING_mu:
	case MAGICID_FABAOATTACK_XILING_tu:
	case MAGICID_FABAOATTACK_XILING_shui:
	case MAGICID_FABAOATTACK_XILING_huo:
		iBlastDis = 8000;
		break;		
	case MAGICID_ENCHANTER_WILD:  
	case MAGICID_ESCAPE_QUICK:		
	case MAGICID_SHIFT_TRANSPORT:		
		iBlastDis = 150;
		break;
	case MAGICID_JIANGJUNBOSS_PT_FLYINGBALL:
	case MAGICID_JIANGJUNBOSS_BS_MAGIC:
		iBlastDis = 15000;
		break;
	case MAGICID_LEOPARD_ATTACK2:
	case MAGICID_QILIN_FLYINGBALL:
	case MAGICID_MEINVSNAKE_FLYINGBALL:
	case MAGICID_SHUYAO_FLYINGBALL:
		iBlastDis = 4000;
	default:
		break;
	}

	if(iX*iX/4+iY*iY <= iBlastDis)
		return true;

	return false;

}

void CMagicManager::PushTexStore(Magic_Show_s *ms,tex_show_t *tex,int offy)
{
	DWORD attr = ms->attr;
	
	if(*(ms->ref->ppShowList) != g_pMagicCtrl->GetShowList())
	{
		g_OtherData.SetFireTexShow(tex);
	}
	else if(attr & EMP_MAGIC_FLOOR)
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
			if(g_pGfx->IsNoDraw())//不绘制不要push,不然会越堆越多
				return;

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
			if(g_pGfx->IsNoDraw())//不绘制不要push,不然会越堆越多
				return;

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
}

// 结束魔法显示
void CMagicManager::FinishMagic(Magic_Show_s *ms)	
{
	g_pMagicCtrl->FinishMagic(ms);
}

Magic_Show_s* CMagicManager::CreateMagic(Magic_Show_s *ms)
{
	return g_pMagicCtrl->CreateMagic(ms);
}

//根据取得人物的ID取得它的砖块坐标
void CMagicManager::GetXYFromID(DWORD dwID,int &X,int &Y,int &ox,int &oy)
{
	if(dwID != 0)
	{
        CCharacterAttr * pAttr = NULL;

        if(dwID == SELF.GetID())
		{    
            pAttr = &SELF;
			SELF.GetXY(X,Y);
			SELF.GetOffset(ox,oy);
			int iDrawoffX = 0,iDrawOffY = 0;

			SELF.GetDrawOffset(iDrawoffX,iDrawOffY);
			ox += iDrawoffX;
			oy += iDrawOffY;
		}
		else 
		{
			CSimpleCharacterNode *p = g_pGameData->FindSimpleCharacter(dwID);
            if(p) 
			{
                pAttr = p;
                p->GetXY(X,Y);		
				p->GetOffset(ox,oy);

				int iDrawoffX = 0,iDrawOffY = 0;
				p->GetDrawOffset(iDrawoffX,iDrawOffY);
				ox += iDrawoffX;
				oy += iDrawOffY;
			}
			else
			{
				X = Y = -1;
				ox = oy = 0;
			}
		} 

        if(pAttr && pAttr->IsOnLepoard())
        {
            int ix = 0,iy = 0;
            GetDevAccordDir(pAttr->GetDir(),ix,iy);

			if(pAttr->IsOnPhenix() && !pAttr->GetFightOnLeopard())
			{
				iy -= 70;
			}

            ox += ix;
            oy += iy;
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
			return (pChar->IsHuman());
	}
	return false;
}

bool CMagicManager::AddMagicTarget(DWORD dwTargetID,int iType)
{
    if(dwTargetID <= 0)
        return false;

    Magic_Show_s* ms = g_pMagicCtrl->FindMagicByAll(4671,-1,dwTargetID,-1,EMP_MAGIC_STATE);

    return AddMagicTarget(ms,dwTargetID,iType);
}

bool CMagicManager::AddMagicTarget(DWORD dwTargetID,WORD wMagicID,DWORD dwOwnerID)
{
	if(dwTargetID <= 0 || (wMagicID == 0 && dwOwnerID == -1))
		return false;

	if(dwTargetID == SELF.GetID())
	{
		if(SELF.IsDead())
			return false;
	}
	else
	{
		CSimpleCharacterNode * pChar = g_pGameData->FindSimpleCharacter(dwTargetID);
		if(!pChar || pChar->IsDead()) 
			return false;
	}

	Magic_Show_s* ms = g_pMagicCtrl->FindMagicByAll(wMagicID,dwOwnerID,dwTargetID);//先找wMagicID,dwOwnerID,dwTargetID都符合的
	//下面的注释掉,不然打怪反弹何罪伤害时主角自己冒泡的时候被攻击的怪物无法做受伤动作/
	//if (!ms)//没找到再找wMagicID,dwOwnerID符合的,因为有的时候魔法的uTarget可能用来表示坐标
 //       ms = g_pMagicCtrl->FindMagicByAll(wMagicID,dwOwnerID);

	if (ms && (EMP_MAGIC_STATE & ms->attr) == 0)
		return AddMagicTarget(ms,dwTargetID);

	return false;
}

bool CMagicManager::AddMagicTarget(Magic_Show_s* ms,DWORD dwTargetID,int iType)
{
	if(!ms || !ms->ref || dwTargetID <= 0)
		return false;

	STargetData *pNewTarget = NEW_TARGETDATA();

    pNewTarget->iType = iType;
	pNewTarget->dwID = dwTargetID;
	pNewTarget->dwOID = ms->ref->uOwnerID;
	if(ms->ref->pTarget)
		pNewTarget->pNext = ms->ref->pTarget;

	ms->ref->pTarget = pNewTarget;
	return true;
}

void CMagicManager::DrawSwearLight(int iX,int iY,CSimpleCharacter* pChar)
{
	
	
	SAction* pAction = NULL;
	bool bFightOnLeopard = false;

	if(pChar)
	{
		if(pChar->GetFightOnLeopard())
			bFightOnLeopard = true;
		pAction = &pChar->GetAction();
		if(pAction == NULL)
			return;
	}
	else
	{
		pAction = &SELF.GetAction();
		if(SELF.GetFightOnLeopard())
			bFightOnLeopard = true;
	}

	if(pChar && pChar->GetRaceNo() == 49 && pAction->wAction == ACTION_ATTACK1)
	{
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_magic1,4600 + pChar->GetDir() * 10 + pAction->iFrameNow,EP_SKILL);
		g_pGfx->DrawTextureNL(iX - 480,iY - 524,pTex);		
		return;
	}

	//不需要绘制刀光的动作
	if(pAction->wAction != ACTION_ATTACK1 || ((pAction->uFlag & MAGIC_ACTION) == 0 && !bFightOnLeopard))
		return;

	

	int x,y;
	LPTexture pTex;	 
	int iSelfX,iSelfY,iOtherX,iOtherY; 
	bool bSelf = false;
	bool bMale = false;
	BYTE cDir = DIR_NO;
    

	SELF.GetXY(iSelfX,iSelfY);
	if (pChar == NULL)
	{
		SELF.GetXY(iOtherX,iOtherY);
		bSelf = true;
		cDir = SELF.GetDir();	
		if(SELF.IsMale())
            bMale = true;        
	}
	else
	{
		pChar->GetXY(iOtherX,iOtherY);

		int iRealX,iRealY;
		SELF.GetPlayerXY(iRealX,iRealY);
		if(abs(iOtherX-iRealX) > 12 || abs(iOtherY-iRealY) > 12)
		{
			return;
		}

		cDir = pChar->GetDir();
        if(pChar->IsMale())
            bMale = true;        
	}

	x = iX - 96;
	y = iY - 132;

	if(bFightOnLeopard)
	{
		int iOffsetX,iOffsetY;
		GetDevAccordDir(cDir,iOffsetX,iOffsetY);
		x += iOffsetX;
		y += iOffsetY;
	}

	if((pAction->uFlag & MAGIC_ACTION) == 0)		//骑战刀光
	{
		int iFrameNow = pAction->iFrameNow;

		if(iFrameNow == 0 || iFrameNow == 5)
		{
			return;
		}

		WORD wTexID	= 29270 + cDir * 6 + iFrameNow;

		pTex = g_pTexMgr->GetTex(PACKAGE_magic2,wTexID,EP_SKILL);

		if(pTex) 
		{
			g_pGfx->SetRenderMode(RM_ADD2);

			if(pAction->dwColor != 0)
				g_pGfx->DrawTextureNL(x - 130, y - 150, pTex,pAction->dwColor);
			else
				g_pGfx->DrawTextureNL(x - 130, y - 150, pTex);

			g_pGfx->SetRenderMode();
		}

		return;
	}

	Magic_Unit_s* pUnit = NULL;
	Magic_Node_s *pNode = NULL;

    int iMagicID = pAction->iData;
    if(bFightOnLeopard)
    {
        iMagicID += 3000;
    }

	Magic_Root_s * pRoot = g_pMagicCtrl->GetMagicRoot(iMagicID);
	if(pRoot && pRoot->m)
	{
		pNode = pRoot->m->child;
		if(pNode)
			pUnit = g_pMagicCtrl->GetMagicUnit(pNode->unit_idx);
	}

	if (pUnit)
	{
		int dirOut = 0,bMapping = 0;
		DirMapping(cDir,pUnit->dirs,dirOut,bMapping);

        int iFrameNow = pAction->iFrameNow;
		WORD wTexID	= pUnit->tex + dirOut*pUnit->frames+iFrameNow;

		pTex = g_pTexMgr->GetTex(PACKAGE_magic1+pUnit->package,wTexID,EP_SKILL);

		//预读
        int iBegin = pUnit->tex + dirOut*pUnit->frames;
        g_pTexMgr->PreLoad(PACKAGE_magic1+pUnit->package,iBegin,iBegin+pUnit->framereal,EP_SKILL);

		if(pTex) 
		{
			g_pGfx->SetRenderMode((RenderMode)pUnit->alpha);
			if(pAction->dwColor != 0)
				g_pGfx->DrawTextureNL(x + pNode->offx, y + pNode->offy, pTex,pAction->dwColor);
			else
				g_pGfx->DrawTextureNL(x + pNode->offx, y + pNode->offy, pTex);

			g_pGfx->SetRenderMode();
		}

        if(iMagicID == MAGICID_LEOPARD_ATTACK_FIRE) //骑战烈火剑法宠物身上的光
        {
            if(pNode->sibling)
                pNode = pNode->sibling;
            
            pUnit = g_pMagicCtrl->GetMagicUnit(pNode->unit_idx);

            int iBegin = pUnit->tex + dirOut*pUnit->frames;
            g_pTexMgr->PreLoad(PACKAGE_magic1+pUnit->package,iBegin,iBegin + pUnit->framereal,EP_SKILL);

            WORD wTexID	= pUnit->tex + dirOut*pUnit->frames+iFrameNow;
            pTex = g_pTexMgr->GetTex(PACKAGE_magic1+pUnit->package,wTexID,EP_SKILL);

            g_pGfx->SetRenderMode((RenderMode)pUnit->alpha);
            if(pAction->dwColor != 0)
                g_pGfx->DrawTextureNL(x + pNode->offx, y + pNode->offy, pTex,pAction->dwColor);
            else
                g_pGfx->DrawTextureNL(x + pNode->offx, y + pNode->offy, pTex);

            g_pGfx->SetRenderMode();
        }

		if (iFrameNow == 0 && pAction->iDealTimesCurFrame == 1)
		{
			if(pAction->iData % 1000 == MAGICID_ATTACK_KILL)//攻杀,骑战攻杀,大师攻杀_神,_魔,大师骑战攻杀_神,_魔
			{
				int iSound = pNode->sound;
				if( !bMale ) 
					iSound++;
     			g_pAudio->PlayEx(EAT_MAGIC,iSound,g_pAudio->GetRand()++,iSelfX,iOtherX,iSelfY,iOtherY,bSelf,1);
				return;
			}

			g_pAudio->PlayEx(EAT_MAGIC,pNode->sound,g_pAudio->GetRand()++,iSelfX,iOtherX,iSelfY,iOtherY,bSelf,1);
		}
		else if (iFrameNow == 4 && pAction->iDealTimesCurFrame == 1)
		{
			g_pAudio->PlayEx(EAT_MAGIC,176,g_pAudio->GetRand()++,iSelfX,iOtherX,iSelfY,iOtherY,bSelf,1);
		}
	}
}


void CMagicManager::DrawSwearLightEx(int iX,int iY,BYTE cDir,CSimpleCharacter* pChar)
{
	SAction* pAction = NULL;
	bool bFightOnLeopard = false;

	if(pChar)
	{		
		pAction = &pChar->GetAction();
		if(pAction == NULL)
			return;
	}
	else
	{
		pAction = &SELF.GetAction();
	
	}

	if(pAction->wAction != ACTION_ATTACK1)
		return;

	int x,y;	 
	int iOtherX,iOtherY;
	
	if (pChar == NULL)
	{
		SELF.GetXY(iOtherX,iOtherY);			
	}
	else
	{
		pChar->GetXY(iOtherX,iOtherY);

		int iRealX,iRealY;
		SELF.GetPlayerXY(iRealX,iRealY);
		if(abs(iOtherX-iRealX) > 12 || abs(iOtherY-iRealY) > 12)
		{
			return;
		}		
	}

	x = iX;
	y = iY;

	Magic_Node_s *pNode = NULL;
	Magic_Root_s * pRoot = g_pMagicCtrl->GetMagicRoot(MAGICID_SHADOWKILL8_DAOG);
	if(pRoot && pRoot->m)
	{
		pNode = pRoot->m->child;
		if(pNode)
			DrawMagicNode(pNode,x,y,cDir,pAction->iFrameNow,pAction->dwColor);
	}
}

void CMagicManager::DrawMagicNode(Magic_Node_s* pNode, int x, int y, BYTE cDir, int iFrameNow, DWORD dwColor)
{
	if (pNode)
	{
		Magic_Unit_s* pUnit = g_pMagicCtrl->GetMagicUnit(pNode->unit_idx);
		if (pUnit)
		{
			LPTexture pTex = NULL;

			int dirOut = 0,bMapping = 0;
			DirMapping(cDir,pUnit->dirs,dirOut,bMapping);

			WORD wTexID	= pUnit->tex + dirOut*pUnit->frames+iFrameNow;

			pTex = g_pTexMgr->GetTex(PACKAGE_magic1+pUnit->package,wTexID,EP_UI);

			//预读
			int iBegin = pUnit->tex + dirOut*pUnit->frames;
			g_pTexMgr->PreLoad(PACKAGE_magic1+pUnit->package,iBegin,iBegin+pUnit->framereal,EP_SKILL);

			if(pTex) 
			{
				g_pGfx->SetRenderMode((RenderMode)pUnit->alpha);
				if( dwColor != 0)
					g_pGfx->DrawTextureNL(x + pNode->offx, y + pNode->offy, pTex,dwColor);
				else
					g_pGfx->DrawTextureNL(x + pNode->offx, y + pNode->offy, pTex);

				g_pGfx->SetRenderMode();
			}
		}		
	}
}

//绘制人物身上的持续特效
void CMagicManager::DrawHumanEff(int iX,int iY,CSimpleCharacter* pChar)
{
	int x,y;
	WORD wStatus = 0;
	WORD byExtraState = 0;
	SAction* pAction = NULL;
	bool bOnHorse = false;
	bool bRideFight = false;
	LPTexture pTex;
	CCharacterAttr* pAttr = NULL;

	if(pChar)
	{
		wStatus = pChar->GetStatus();
		byExtraState = pChar->GetExtraState();
		pAction = &pChar->GetAction();
		bOnHorse = pChar->IsOnHorse();
		bRideFight = pChar->GetFightOnLeopard();
		pAttr = pChar;
	}
	else
	{
		wStatus = SELF.GetStatus();
		byExtraState = SELF.GetExtraState();
		pAction = &SELF.GetAction();
		bOnHorse = SELF.IsOnHorse();
		bRideFight = SELF.GetFightOnLeopard();
		pAttr = &SELF;
	}

	if(bOnHorse && !bRideFight) //在马上,结束
		return;

	int iFrame = pAction->iFrameNow; //得到当前绘制的帧

	// 画图位置
	x = iX - 96;
	y = iY - 132; 

	// 魔法师的魔法盾
	if(wStatus & CS_MAGICPROTECT)
	{
		int iCurFrame = (pAction->wAction == ACTION_ATTACKED)?(6 + (iFrame % 3)) : (iFrame % 6);
		pTex = g_pTexMgr->GetTex(PACKAGE_magic1 ,10 + iCurFrame,EP_SKILL);

		if( pTex ) 
		{
			g_pGfx->SetRenderMode(RM_ADD1);
			g_pGfx->DrawTextureNL(x,y,pTex);
			g_pGfx->SetRenderMode();
		}
		else
		{
			g_pTexMgr->PreLoad(PACKAGE_magic1,10,16,EP_MAGIC);
		}
	}

	if(byExtraState & ES_PAOPAO) //泡泡
	{
		pTex = g_pTexMgr->GetTex(PACKAGE_magic2 ,5400 + iFrame % 10,EP_MAGIC);

		if(pTex) 
		{
			g_pGfx->SetRenderMode(RM_ADD1);
			g_pGfx->DrawTextureNL(x,y,pTex);
			g_pGfx->SetRenderMode();
		}
	}

	// 战士的金刚护体受攻击特效,其它特效用魔法做了
	if(byExtraState & ES_STEELPROTECT)
	{
		DWORD dwFrame = 0;
		if(pAction->wAction == ACTION_ATTACKED)
		{
			pAttr->AddReserveData(pubSteelAttackedEff);
		}
		dwFrame = pAttr->GetReserveData(pubSteelAttackedEff);

		if(dwFrame > 0)//金刚护体受攻击
		{
			pTex = g_pTexMgr->GetTex(PACKAGE_magic1 ,5481 + dwFrame/4,EP_SKILL);
			if(pTex)
			{
				g_pGfx->SetRenderMode(RM_ADD1);
				g_pGfx->DrawTextureNL(x,y,pTex);
				g_pGfx->SetRenderMode();
			}
			else
			{
				g_pTexMgr->PreLoad(PACKAGE_magic2,5482,5491,EP_SKILL);
			}

			pAttr->AddReserveData(pubSteelAttackedEff);

			if(dwFrame >41)  //10帧
			{
				pAttr->SetReserveData(pubSteelAttackedEff,0);
			}
		}
	}


	if(byExtraState & ES_DODGEPROTECT) //风影盾
	{
		DWORD dwEffTime = pAttr->GetReserveData(pubLastEffDodgeTime);
		if(dwEffTime == 0)
		{
			dwEffTime = m_dwTickCount;
			bool bSelf = false;
			int iSelfX,iSelfY,iOtherX,iOtherY;
			SELF.GetXY(iSelfX,iSelfY);
			if (pChar)
				pChar->GetXY(iOtherX,iOtherY);
			else
			{
				iOtherX = iSelfX;
				iOtherY = iSelfY;
				bSelf = true;
			}			
			g_pAudio->PlayEx(EAT_MAGIC,296,g_pAudio->GetRand()++,iSelfX,iOtherX,iSelfY,iOtherY,bSelf);	
		}

		int iFrame = (m_dwTickCount - dwEffTime) / 60;
		if(iFrame >= 30 || dwEffTime == m_dwTickCount)
		{
			pAttr->SetReserveData(pubLastEffDodgeTime,m_dwTickCount);
		}

		pTex = g_pTexMgr->GetTex(PACKAGE_magic1,5366 + iFrame,EP_SKILL);
		if(pTex)
		{
			g_pGfx->SetRenderMode(RM_ADD1);
			g_pGfx->DrawTextureNL(x,y,pTex);
			g_pGfx->SetRenderMode();
		}
		else
		{
			g_pTexMgr->PreLoad(PACKAGE_magic1,5366,5366 + 30,EP_SKILL);
		}
	}

	//幻彩套装
	if(pAttr->GetReserveData(pubHuanCaiCloth) == 1)
	{
		LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_stateitem,6000,EP_MAGIC);
		if (pTex)
		{
			int sy = y;
			if(bOnHorse)
				sy-= 42;

			g_pGfx->SetRenderMode(RM_ADD1);
			g_pGfx->DrawTextureNL(x,sy,pTex);
			g_pGfx->SetRenderMode();
		}
	}
	
	////真元攻击
	//bool bKillOrAttack = false;
	//	DWORD dwStartTm = SELF.GetReserveData(plyZhenYuanStart);
	//	if(GetTickCount() - dwStartTm < SELF.GetReserveData(plyZhenYuanConTm))
	//		bKillOrAttack = true;		

	//bKillOrAttack |= ((byExtraState & ES_BRAST) != 0);
	//if(bKillOrAttack)
	//{
	//	LPTexture pTex = g_pTexMgr->GetTex(PACKAGE_magic2,25680,TRP_MOST_HIGH);
	//	if(pTex)
	//	{
	//		g_pGfx->SetRenderMode(RM_ADD1);
	//		g_pGfx->DrawTextureNL(x,y,pTex);
	//		g_pGfx->SetRenderMode();
	//	}
	//}
}

void CMagicManager::DealCycleFlower(Magic_Show_s* ms)
{
	if(!ms || !ms->ref || !ms->show_node)
		return;

	Magic_Unit_s* pUnit = ms->pUnit;

	if(!pUnit)
		return;

	int x,y,mx,my;
	int iFrameNow;

	WORD wMagicID = ms->ref->wMagicID;

	switch(pUnit->id)
	{
	case MAGICID_ICE_SWORD_ATTACKED:
	case MAGICID_TONGTIAN_FLY_ATTACK:
	case MAGICID_TIGER_SPRAY:
		ms->fData1 = 32.0f;   //每次移动的距离
		ms->fData2 = 16.0f;
		break;
    case MAGICID_WXXG_MONSTER_SPIRIT_FLY:
        ms->fData1 = 32.0f;   //每次移动的距离
        ms->fData2 = 16.0f;
        break;
	default:
		{
			CreateMagic(ms);
			FinishMagic(ms);
			return;
		}
		break;
	}

    if(ms->attr & EMP_MAGIC_MYSELF && wMagicID == MAGICID_WXXG_MONSTER_SPIRIT)
    {
        int X,Y,ox,oy;
        //SELF.GetXY(X,Y);
        //SELF.GetOffset(ox,oy);  
        GetXYFromID(SELF.GetID(),X,Y,ox,oy);
        mx = (X - m_iTileStartX) * TILE_WIDTH + ox - m_iDrawOffX;
        my = (Y - m_iTileStartY) * TILE_HEIGHT + oy - m_iDrawOffY;
        
    }
    else 
	if(ms->attr & EMP_MAGIC_MYSELF)
	{
		mx = (ms->ref->wOriginTileX - m_iTileStartX)*TILE_WIDTH - m_iDrawOffX + TILE_WIDTH / 2;
		my = (ms->ref->wOriginTileY - m_iTileStartY)*TILE_HEIGHT - m_iDrawOffY + TILE_HEIGHT / 2;
	}
	else
	{
		mx = (ms->ref->wTargetTileX - m_iTileStartX)*TILE_WIDTH - m_iDrawOffX + TILE_WIDTH / 2;
		my = (ms->ref->wTargetTileY - m_iTileStartY)*TILE_HEIGHT - m_iDrawOffY + TILE_HEIGHT / 2;
	}

	if(ms->dwStartTime == 0)
	{
		ms->dwStartTime = m_dwTickCount;
	}
	iFrameNow = (m_dwTickCount - ms->dwStartTime) / ms->iFrameSpeed;
	int iMu = 1;
	if(pUnit->reserved2 >= 16)//pUnit->reserved2大于16图片放置时是逆时针方向，pUnit->reserved2%16表示起始方向,即第一张图片的方向
		iMu = -1;

    int RandomArrary[16] ={3,4,2,5,1,5,3,4,1,5,3,2,2,4,3,5};
    
	for(int iDir = 0;iDir < 16;iDir++)
    {
        float fAngle = 0;

        if(wMagicID == MAGICID_WXXG_MONSTER_SPIRIT || wMagicID == MAGICID_TIANJUE_END_EFF)		
		{
            fAngle = (float)(((iDir - 3)%16)*22.5*PI*2) / 360;
        }
        else
        {
            fAngle = (float)(((12 + iDir)%16)*22.5*PI*2) / 360;
        }

		float fCosAngle = (float)cos(fAngle);
		float fSinAngle = (float)sin(fAngle);

        if(wMagicID == MAGICID_WXXG_MONSTER_SPIRIT || wMagicID == MAGICID_TIANJUE_END_EFF)
		{
            int iFrame = iFrameNow - RandomArrary[iDir];

            if(iFrame > 25)
            {

                iFrame = 25;
            }
            else if(iFrame < 0)
            {
                continue;
            }

            x = mx + (int)(ms->fData1 * fCosAngle*(27 - iFrame));
            y = my + (int)(ms->fData2 * fSinAngle*(27 - iFrame));
        }
		else
		{
            
			x = mx + (int)(ms->fData1 * fCosAngle*iFrameNow);
			y = my + (int)(ms->fData2 * fSinAngle*iFrameNow);
		}

		tex_show_t * tex = CreateTexShow();
		if (tex)
		{
			//预读图片
			int iBegin = pUnit->tex + (pUnit->reserved2 + iDir * iMu)%16 * ms->iFrameDir;//pUnit->reserved2大于16图片放置时是逆时针方向，pUnit->reserved2%16表示起始方向,即第一张图片的方向
			if(ms->tFrameStart == m_dwTickCount)
				g_pTexMgr->PreLoad(PACKAGE_magic1+pUnit->package,iBegin,iBegin + pUnit->framereal,EP_MAGIC);

			tex->wTexID	= iBegin + (iFrameNow % pUnit->framereal);
			tex->wPackage = pUnit->package;
			tex->wAdd	= pUnit->alpha;
			tex->x		= x + ms->iOffX;
			tex->y		= y + ms->iOffY;
			tex->dwColor	= ms->dwColor;

			PushTexStore(ms ,tex,0);
		}
	}

    if(wMagicID == MAGICID_WXXG_MONSTER_SPIRIT && ((iFrameNow > 30 && ms->iCycles == 1) || (iFrameNow > 35 && ms->iCycles == 0)))
    {
        CreateMagic(ms);
        FinishMagic(ms);
    }
	else 
	if(iFrameNow > 50)
	{
		CreateMagic(ms);
		FinishMagic(ms);
	}
}

void CMagicManager::DealDropGoodEffect(Magic_Show_s* ms)
{
	if(!ms || !ms->ref || !ms->show_node)
		return;

	Magic_Unit_s* pUnit = ms->pUnit;

	if(!pUnit)
		return;

	if(ms->tFrameStart > m_dwTickCount)
		return;

	int x,y,ax,ay;
	int iFrameNow = ms->GetFrame(m_dwTickCount);
	DWORD dwLooks = ms->ref->uTargetID;
	switch(iFrameNow)
	{
	case 0:
		ay = 0;
		break;
	case 1:
		ay = 15;
		break;
	case 2:
		ay = 25;
		break;
	case 3:
		ax = 4;
		ay = 35;
		break;
	case 4:
		ay = 43;
		break;
	case 5:
		ay = 50;
		break;
	case 6:
		ay = 43;
		break;
	case 7:
		ay = 35;
		break;
	case 8:
		ay = 25;
		break;
	case 9:
		ay = 15;
		break;
	default:
		break;		
	}

	if(ms->IsEnd(m_dwTickCount))
	{
		CSimpleGood * p = g_pGameData->FindSimpleGood(ms->ref->uOwnerID);
		if(p)
			p->SetLooks(dwLooks);

		FinishMagic(ms);
		return;
	}

	x = (ms->iData2 - m_iTileStartX)*TILE_WIDTH - m_iDrawOffX + TILE_WIDTH / 2;
	y = (ms->iData3 - m_iTileStartY)*TILE_HEIGHT - m_iDrawOffY + TILE_HEIGHT / 2 - ay;

	if(x <  0 || y < 0 || x > g_pGfx->GetWidth() || y > g_pGfx->GetHeight())
	{
		CSimpleGood * p = g_pGameData->FindSimpleGood(ms->ref->uOwnerID);
		if(p)
			p->SetLooks(ms->ref->uTargetID);

		FinishMagic(ms);
		return;
	}

	tex_show_t * tex = CreateTexShow();
	if (tex)
	{
		BYTE byAlpha = pUnit->alpha;
		if(dwLooks == 497)
		{
			x -= 100;
			y -= 120;
			byAlpha = RM_ADD2;
		}
		else if(dwLooks == 816)
		{
			x -= 44;
			y -= 40;
			byAlpha = RM_ADD2;
		}

		tex->wTexID	= ms->ref->uTargetID;
		tex->wPackage = PACKAGE_dnitems - PACKAGE_magic1;
		tex->wAdd	= byAlpha;
		tex->x		= x + ms->iOffX;
		tex->y		= y + ms->iOffY;
		tex->dwColor	= ms->dwColor;

		PushTexStore(ms ,tex,0);
	}

}

//节日焰火
void CMagicManager::DealFestalSkyrocket(Magic_Show_s* ms)
{
	//ms中：iData2 节日类型, iData3 祝贺焰火时间间隔, iData4 背景焰火持续时间, iCycles 祝贺焰火播放次数, tFrameStart 焰火开始时间;
	if(ms == NULL)
		return;
	
	if(ms->iData2 <=  0 || ms->iData2 > 3)
	{
		FinishMagic(ms);
		return;
	}

	int iTemp = (int)(GetTickCount() - ms->tFrameStart);
	if(iTemp < 0)
	{
		FinishMagic(ms);
		return;
	}
	else if(iTemp <= 20500)		// 计数焰火
	{
		if(iTemp / 2000 != ms->iData1)
		{
			ms->iData1 = iTemp / 2000;		
			Magic_Show_s* msSkyrocket = g_pMagicCtrl->CreateMagic(MAGICID_SKYROCKET, 0, SELF.GetID());
			if(msSkyrocket)
			{
				msSkyrocket->iFrameSpeed = 1;       // 时间间隔(秒)
				msSkyrocket->iData2	= 1;            // 类型
				msSkyrocket->iFrameCount = 10;      // 帧数
				msSkyrocket->dwLastTex = 2001 + (10 - ms->iData1) * 10;	// 图片起始位置
				msSkyrocket->tFrameStart = GetTickCount();		        // 开始时间
				int X,Y,ox,oy;
				//SELF.GetXY(X,Y);
				//SELF.GetOffset(ox,oy);
                GetXYFromID(SELF.GetID(),X,Y,ox,oy);
				msSkyrocket->iData3	= (X - m_iTileStartX) * TILE_WIDTH + ox - m_iDrawOffX - 96 - TILE_WIDTH / 2;
				msSkyrocket->iData4 = (Y - m_iTileStartY) * TILE_HEIGHT + oy - m_iDrawOffY - 256 - TILE_HEIGHT / 2;			
			}
		}
	}
	else if(iTemp <= 21000)
	{
		return;
	}
	else if(iTemp < 21000 + ms->iData3 * ms->iCycles)			// 祝贺焰火
	{
		iTemp = iTemp - 21000;
		if(iTemp / ms->iData3 != ms->iData1)
		{
			ms->iData1 = iTemp / ms->iData3;
			for(int ic = 0; ic < 4; ic++)
			{
				Magic_Show_s* msSkyrocket = g_pMagicCtrl->CreateMagic(MAGICID_SKYROCKET, 0, SELF.GetID());
				if(msSkyrocket)
				{
					msSkyrocket->iFrameSpeed = 10*20;           // 播放速度
					msSkyrocket->iData2	= 0;                    // 类型
					msSkyrocket->iFrameCount = 10;              // 帧数
					msSkyrocket->tFrameStart = GetTickCount();  // 开始时间
					int X,Y,ox,oy;
					//SELF.GetXY(X,Y);
					//SELF.GetOffset(ox,oy);
                    GetXYFromID(SELF.GetID(),X,Y,ox,oy);
					msSkyrocket->iData3	= (X - m_iTileStartX) * TILE_WIDTH + ox - m_iDrawOffX  - 396 + ic * 200 - TILE_WIDTH / 2;
					msSkyrocket->iData4 = (Y - m_iTileStartY) * TILE_HEIGHT + oy - m_iDrawOffY - 224 - TILE_HEIGHT / 2;	

					if(ic == 0)
						msSkyrocket->dwLastTex = 2301 + (ms->iData2 - 1) * 20; // 图片起始位置
					else if(ic == 1)
						msSkyrocket->dwLastTex = 2311 + (ms->iData2 - 1) * 20;
					else if(ic == 2)
						msSkyrocket->dwLastTex = 2201;
					else
						msSkyrocket->dwLastTex = 2211;	
				}
			}
		}
	}
	else    //背景焰火
	{
		if(ms->iData4)
		{
			Magic_Show_s* msStrew = g_pMagicCtrl->CreateMagic(MAGICID_STREW_SKYROCKET, 0, SELF.GetID());
			if(msStrew)
			{
				msStrew->iData2 = 0;                   // 类型
				msStrew->iData3 = 30;                  // 稠密程度
				msStrew->iData4 = ms->iData4;          // 持续时间
				msStrew->tFrameStart = GetTickCount(); //开始时间
			}
		}
		FinishMagic(ms);
		return;
	}
}

// 背景焰火
void CMagicManager::DealStrewSkyrocket(Magic_Show_s* ms)
{	
	//ms中，iData2 类型，iData3 稠密程度， iData4 持续时间，tFrameStart 开始时间
	if(ms == NULL)
		return;
	
	if((int)(GetTickCount() - ms->tFrameStart) > ms->iData4 || ms->iData3 < 0)
	{
		FinishMagic(ms);
		return;
	}

	if(rand() % 100 > ms->iData3)
		return;

	int iNum = ms->iData3 / 100;
	if(iNum == 0)
		iNum = 1;

	if(ms->iData2 < 6)
	{
		for(int ic = 0; ic < iNum; ic++)
		{
			Magic_Show_s* msSkyrocket = g_pMagicCtrl->CreateMagic(MAGICID_SKYROCKET, 0, SELF.GetID());
			if(msSkyrocket)
			{
				msSkyrocket->iFrameSpeed = (1 + rand() % 2)*20;      // 播放速度
				msSkyrocket->iData2	= 0;                             // 类型
				msSkyrocket->iFrameCount = 20;                       // 帧数
				msSkyrocket->tFrameStart = GetTickCount();           // 开始时间
				msSkyrocket->iData3	= rand() % g_pGfx->GetWidth();     // 坐标
				msSkyrocket->iData4	= rand() % g_pGfx->GetHeight();
				if(msSkyrocket->iData3 > (g_pGfx->GetWidth() / 2 - 50) && msSkyrocket->iData3 < (g_pGfx->GetWidth() / 2 + 50)
					&& msSkyrocket->iData4 > (g_pGfx->GetHeight() / 2 - 30) && msSkyrocket->iData4 < (g_pGfx->GetHeight() / 2 + 30))
				{
					msSkyrocket->iData3	= rand() % g_pGfx->GetWidth();
					msSkyrocket->iData4	= rand() % g_pGfx->GetHeight();
				}
				msSkyrocket->iData3	-= 128;
				msSkyrocket->iData4	-= 128;

				if(ms->iData2 <= 0)
					msSkyrocket->dwLastTex = (WORD)(2700 + (GetTickCount() % 5) * 20);// 图片起始位置
				else
					msSkyrocket->dwLastTex = (WORD)(2700 + (ms->iData2 - 1) * 20);
			}
		}
	}
	else
	{
		FinishMagic(ms);
	}		
}

//焰火粒子
void CMagicManager::DealSkyrocket(Magic_Show_s* ms)
{
	//ms中，iFrameSpeed 时间间隔或播放速度，iData2 类型，iData3和iData4 坐标，iFrameCount 帧数， dwLastTex 起始帧，tFrameStart 起始时间
	if(ms == NULL)
		return;
	
	int iFrameNow = 0;
	switch(ms->iData2)
	{
	case 0:
		{
			iFrameNow = ms->GetFrame(m_dwTickCount);
			if(iFrameNow >= ms->iFrameCount - 1)
			{
				FinishMagic(ms);
				return;
			}
			break;
		}
	case 1:
		{
			int iTemp = (int)(m_dwTickCount - ms->tFrameStart);
			if(iTemp >= ms->iFrameSpeed * 2000)
			{
				FinishMagic(ms);
				return;
			}
			iFrameNow =  ms->iFrameCount * iTemp / (ms->iFrameSpeed * 2000);
			break;
		}
	default:
		{
			FinishMagic(ms);
			return;
		}
	}

	if(ms->tFrameStart == m_dwTickCount)
		g_pTexMgr->PreLoad(PACKAGE_magic2,ms->dwLastTex, ms->dwLastTex + ms->iFrameCount - 1,EP_MAGIC);

	tex_show_t * tex = CreateTexShow();
	tex->wTexID	  = (WORD)(ms->dwLastTex + iFrameNow);
	tex->wPackage = 1;
	tex->wAdd	 = RM_ADD2;
	tex->x		 = ms->iData3;
	tex->y		 = ms->iData4;
	tex->dwColor = ms->dwColor;
	PushTexStore(ms, tex, 0);	
}
//画香燃烧的火焰
void CMagicManager::DrawCenserEffect(Magic_Show_s* ms)
{
	if(!ms || g_OtherData.GetCenserDurTime() <= 0)
		return;

	float fScale = (GetTickCount() - g_OtherData.GetCenserStartTime())/float(g_OtherData.GetCenserDurTime());
	if(fScale >= 1.0f)
	{
		CreateMagic(ms);
		FinishMagic(ms);
		return;
	}

	ms->iOffY = (int)(ms->iData2 + ms->iData3 * fScale) - TILE_HEIGHT/2;
	ShowMagic(ms);
}

//创建火球后面的火花
void CMagicManager::CreateSparkle(Magic_Show_s* ms)
{
	if(ms == NULL || ms->ref == NULL)
		return;

	Magic_Show_s* msSparkle = g_pMagicCtrl->CreateMagic(MAGICID_SPARKLE,EMP_MAGIC_NOOWNER|EMP_MAGIC_NOTARGET);
	if(msSparkle)
	{
		msSparkle->ref->wOriginTileX = ms->ref->wOriginTileX;
		msSparkle->ref->wOriginTileY = ms->ref->wOriginTileY;
        msSparkle->dwLastTex = 1701; //起始图片
		msSparkle->iFrameCount = 4;  //图片帧数
		msSparkle->fData1 = ms->fData1;
		msSparkle->fData2 = ms->fData2;  
		msSparkle->iData2 = rand() % 2;
		msSparkle->iData3 = 3 + rand() % 4; 
		msSparkle->fx = (float)((int)ms->fx) + (rand() % 4) * ms->fData1 / 4;
		msSparkle->fy = (float)((int)ms->fy) + (rand() % 4) * ms->fData2 / 4; 
		msSparkle->iFrameSpeed = msSparkle->iData3 * 20;
		msSparkle->tFrameStart = GetTickCount();
	}
}

//处理火球后面的火花
void CMagicManager::DealSparkle(Magic_Show_s* ms)
{
	if(ms == NULL || ms->ref == NULL)
		return;

	int iFrameNow = ms->GetFrame(m_dwTickCount);
	
	ms->fx += ms->fData1;
	ms->fy += ms->fData2;

	ms->fData1 /= 2;
	ms->fData2 /= 2;

	int x = (ms->ref->wOriginTileX - m_iTileStartX)*TILE_WIDTH - m_iDrawOffX + (int)ms->fx - 126;
	int y = (ms->ref->wOriginTileY - m_iTileStartY)*TILE_HEIGHT - m_iDrawOffY + (int)ms->fy - 152;

	tex_show_t * tex = CreateTexShow();
	tex->wTexID	  = (WORD)(ms->dwLastTex + ms->iData2 * ms->iFrameCount + iFrameNow);
	tex->wPackage = 0;
	tex->wAdd	 = RM_ADD1;
	tex->x		 = x + ms->ref->iOffX +  ms->iOffX;
	tex->y		 = y + ms->ref->iOffY +  ms->iOffY;
	tex->dwColor = ms->dwColor;
	PushTexStore(ms, tex, 0);

	if(iFrameNow >= ms->iFrameCount - 1)
	{
		FinishMagic(ms);
		return;
	}
}

//处理渐隐特效魔法
void CMagicManager::DealGradualAppear(Magic_Show_s* ms)  
{
	UINT uOwnerID = ms->ref->uOwnerID;
	if(uOwnerID == SELF.GetID())
	{
		SELF.SetReserveData(pubGradualAppear,ms->iCycles);
        SELF.SetReserveData(pubGradualTime,GetTickCount());
	}
	else 
	{
		CSimpleCharacterNode * p = g_pGameData->FindSimpleCharacter(uOwnerID);
		if(p) 
		{
			p->SetReserveData(pubGradualAppear,ms->iCycles);
            p->SetReserveData(pubGradualTime,GetTickCount());
		}		    
	}

	CreateMagic(ms);
	FinishMagic(ms);
}

void CMagicManager::DealNewMagicUnit(Magic_Show_s* ms)
{
	if(!ms)
		return;

	Magic_Ref_s * ref = ms->ref;
	if(!ref)
		return;

	DWORD dwOwnerID = ref->uOwnerID;
	DWORD dwTargetID = ref->uTargetID;
	if(dwOwnerID == 0)
		dwOwnerID = MAKELONG(ref->wOriginTileX,ref->wOriginTileY);
	if(dwTargetID == 0)
		dwTargetID = MAKELONG(ref->wTargetTileX,ref->wTargetTileY);

	if(ms->attr & EMP_MAGIC_HIT_TARGET)
	{
		DWORD dwTemp = dwTargetID;
		dwTargetID = dwOwnerID;
		dwOwnerID = dwTemp;
	}

	if (ms->byRev1 == 0)
	{
		Magic_Show_s* pNewMs = g_pMagicCtrl->CreateMagic(ms->iCycles,ref->attr,dwOwnerID,dwTargetID);
		FinishMagic(ms);
	} 
	else if(ms->pUnit->id == MAGICID_CREATE_NEW_MAGIC_FABAO)	//rev1(byte)----最远点帧数,rev2(int)----最远点格子数
	{
		int iAX,iAY;

		iAX = ms->ref->wTargetTileX - ms->ref->wOriginTileX;
		iAY = ms->ref->wTargetTileY - ms->ref->wOriginTileY;
		iAX = max(abs(iAX),abs(iAY));

		if(ms->GetFrame(m_dwTickCount) > ( ms->byRev1 * iAX / ms->iRev1))
		{
			Magic_Show_s* pNewMs = g_pMagicCtrl->CreateMagic(ms->iCycles,ref->attr,dwOwnerID,dwTargetID);
			FinishMagic(ms);
		}
		
	}
	
}

void  CMagicManager::DealOpenNewWnd(Magic_Show_s* ms)
{
    if(!ms)
        return;    

	if(ms->iCycles == MSG_CTRL_WUXING_PRIZE_WND)
	{
		if(g_NPC.GetWuXingResult().strPrize.size() != 0)
		{
			g_pControl->PopupWindow(ms->iCycles,OPER_CREATE,1);
		}
		else
		{
			g_pControl->PopupWindow(ms->iCycles,OPER_CREATE,2);
		}
	}

    if( ms->wSound > 0)
    {
        g_pAudio->Play(EAT_MAGIC,ms->wSound,g_pAudio->GetRand()++);       
    }

    FinishMagic(ms);
}

void CMagicManager::DealExchangeID(Magic_Show_s* ms)
{
	if(!ms)
		return;    
	
	UINT temp = ms->ref->uOwnerID;
	ms->ref->uOwnerID = ms->ref->uTargetID;
	ms->ref->uTargetID = temp;

	int iX = 0,iY = 0;
	if(ms->ref->uOwnerID == SELF.GetID())
	{
		SELF.GetXY(iX,iY);
		ms->ref->wOriginTileX = iX;
		ms->ref->wOriginTileY = iY;
	}
	else 
	{
		CSimpleCharacterNode * pChar = g_pGameData->FindSimpleCharacter(ms->ref->uOwnerID);
		if(pChar) 
		{
			pChar->GetXY(iX,iY);
			ms->ref->wOriginTileX = iX;
			ms->ref->wOriginTileY = iY;
		}		
	}

	if(ms->ref->uTargetID == SELF.GetID())
	{
		SELF.GetXY(iX,iY);
		ms->ref->wTargetTileX = iX;
		ms->ref->wTargetTileY = iY;
	}
	else 
	{
		CSimpleCharacterNode * pChar = g_pGameData->FindSimpleCharacter(ms->ref->uTargetID);
		if(pChar) 
		{
			pChar->GetXY(iX,iY);
			ms->ref->wTargetTileX = iX;
			ms->ref->wTargetTileY = iY;
		}		
	}

	ms->ref->iDir = (ms->ref->iDir + 8) % 16;//方向反过来

	CreateMagic(ms);
	FinishMagic(ms);
}
//处理强化小火球飞行
void CMagicManager::DealSuperFireBall(Magic_Show_s* ms)
{
	if (ms->tFrameStart == 0 && ms->ref->wData[2] == 0)
	{
		int dwTargetID = ms->ref->uTargetID;
		for (int i = 0;i < ms->ref->wData[1] - 1;i++)
		{
			if(dwTargetID == 0)
				dwTargetID = MAKELONG(ms->ref->wTargetTileX,ms->ref->wTargetTileY);
			Magic_Show_s* pNewMs = g_pMagicCtrl->CreateMagic(MAGICID_SUPER_FIREBALL_FLYING,ms->ref->attr,ms->ref->uOwnerID,dwTargetID);
			if (pNewMs)
			{
				pNewMs->ref->wData[2] = i+1;//第几个
				pNewMs->ref->wData[3] = 6 + i*6;//延时发出
			}
		}
	}

	if(ms->ref->wData[3] > 0)
	{
		ms->ref->wData[3] --;
		return;
	}

	if(rand()%2 == 0)
		CreateSparkle(ms);

	ShowMagic(ms);
}

//处理强化施毒术飞行
void  CMagicManager::DealSuperPoison(Magic_Show_s* ms)
{
	Magic_Show_s* pNewMs[4];
	pNewMs[0] = pNewMs[1] = pNewMs[2] = pNewMs[3] = NULL;
	if (ms->tFrameStart == 0 && ms->ref->wData[2] == 0)
	{		
		pNewMs[0] = g_pMagicCtrl->CreateMagic(MAGICID_SUPER_POISON_SKULL2,ms->ref->attr,ms->ref->uOwnerID,ms->ref->uTargetID);
		if (pNewMs[0])
			pNewMs[0]->ref->wData[2] = 1;
		switch(ms->ref->wData[1])//级别
		{
			//wData[2]记录层数决定延时,wData[3]决定位移偏量
		case 2:
			break;
		case 3:
			{
				pNewMs[1] = g_pMagicCtrl->CreateMagic(MAGICID_SUPER_POISON_SKULL1,ms->ref->attr,ms->ref->uOwnerID,ms->ref->uTargetID);
				if (pNewMs[1])
					pNewMs[1]->ref->wData[2] = 2;
			}
			break;
		case 4:
			{
				pNewMs[1] = g_pMagicCtrl->CreateMagic(MAGICID_SUPER_POISON_SKULL1,ms->ref->attr,ms->ref->uOwnerID,ms->ref->uTargetID);
				if (pNewMs[1])
				{
					pNewMs[1]->ref->wData[2] = 2;
					pNewMs[1]->ref->wData[3] = -1;
				}
				pNewMs[2] = g_pMagicCtrl->CreateMagic(MAGICID_SUPER_POISON_SKULL1,ms->ref->attr,ms->ref->uOwnerID,ms->ref->uTargetID);
				if (pNewMs[2])
				{
					pNewMs[2]->ref->wData[2] = 2;
					pNewMs[2]->ref->wData[3] = 1;
				}
			}
			break;
		case 5:
			{
				if (pNewMs[0])
					pNewMs[0]->ref->wData[3] = 2;
				pNewMs[1] = g_pMagicCtrl->CreateMagic(MAGICID_SUPER_POISON_SKULL1,ms->ref->attr,ms->ref->uOwnerID,ms->ref->uTargetID);
				if (pNewMs[1])
				{
					pNewMs[1]->ref->wData[2] = 2;
					pNewMs[1]->ref->wData[3] = 2;
				}
				pNewMs[2] = g_pMagicCtrl->CreateMagic(MAGICID_SUPER_POISON_SKULL1,ms->ref->attr,ms->ref->uOwnerID,ms->ref->uTargetID);
				if (pNewMs[2])
				{
					pNewMs[2]->ref->wData[2] = 2;
					pNewMs[2]->ref->wData[3] = 1;
				}
				pNewMs[3] = g_pMagicCtrl->CreateMagic(MAGICID_SUPER_POISON_SKULL2,ms->ref->attr,ms->ref->uOwnerID,ms->ref->uTargetID);
				if (pNewMs[3])
				{
					pNewMs[3]->ref->wData[2] = 1;
					pNewMs[3]->ref->wData[3] = 1;
				}
			}
			break;
		default:
			break;
		}
		for (int i=0;i < 4;i++)
		{
			if (!pNewMs[i])
				continue;
			pNewMs[i]->ref->uTargetID = ms->ref->uTargetID;
			pNewMs[i]->ref->wTargetTileX = ms->ref->wTargetTileX;
			pNewMs[i]->ref->wTargetTileY = ms->ref->wTargetTileY;
			if(pNewMs[i]->ref->wData[3] != 0)
			{
				int itx = -1,ity = -1,ox,oy;
				GetXYFromID(ms->ref->uTargetID,itx,ity,ox,oy);
				itx = itx * 64 + ox;
				ity = ity * 32 + oy;

				int iX , iY;
				iX = ms->ref->wOriginTileX * 64 - itx; 
				iY =  ms->ref->wOriginTileY * 32 - ity; 

				double atom = 3.1416/2;
				if(iX != 0)
					atom = atan((float)abs(iY)/abs(iX));
				int imulx  = (int)(20 * sin(atom));
				int imuly  = (int)(20 * cos(atom)/1.5); 
				int iTemp = pNewMs[i]->ref->wData[3];
				if ( iTemp != 1)
					iTemp = -1;
				pNewMs[i]->iOffX += iTemp *imulx;
				pNewMs[i]->iOffY += iTemp *imuly;  

				//if(ms->iDir == 7)
				//{
				//	pNewMs[i]->iOffX += (int) ( 0.30f * pNewMs[i]->iOffX);
				//	pNewMs[i]->iOffY += (int) ( 1.30f * pNewMs[i]->iOffY);
				//}
			}
		}
	}

	if (ms->ref->wData[2] == 0 || (m_dwTickCount - ms->dwStartTime) > (ms->ref->wData[2] * 150))
		ShowMagic(ms);
}
//处理烟花筒特效
void CMagicManager::DealYanHuaTong(Magic_Show_s* ms)
{
	if(NULL == ms || NULL == ms->ref)
		return;

	if(GetTickCount() - ms->dwStartTime > 3000)
	{
		// 每隔3秒 随机加播放烟花
		int iYanhuoType=1002;
		int iRand1 = iYanhuoType + (ms->dwStartTime%10);
		ms->dwStartTime = GetTickCount();
		g_pMagicCtrl->CreateMagic(iRand1, 0, 0, ms->ref->uOwnerID);
	}
}

void CMagicManager::DealThrowStone(Magic_Show_s* ms)
{
	int iFrameNow = ms->GetFrame(GetTickCount());
	
	if(iFrameNow >= 15)
	{
		CreateMagic(ms);
		FinishMagic(ms);
	}
	else
	{
		if(iFrameNow == 0)
		{
			int iX = 0, iY = 0;
			ms->fData1 = float((ms->ref->wOriginTileX - ms->ref->wTargetTileX)*64);
			ms->fData2 = float((ms->ref->wOriginTileY - 2 - ms->ref->wTargetTileY)*32);
			ms->fx = float(-(ms->fData1 * 0.95 * 0.45f)/5.0f);
			ms->fy = -200;
		}
		else if(iFrameNow < 5 && ms->iData1 != iFrameNow)
		{
			ms->fy -= (5 - iFrameNow) * 15.0f;
			ms->fx -= float((ms->fData1 * 0.95 * 0.45f)/5.0f);
		}
		else if(ms->iData1 != iFrameNow)
		{
			if(ms->fData2 / 32.0f == 0.0f) 
			{
				ms->fy += (int)(fabs((fabs(ms->fy) - ms->fData2) * (iFrameNow - 5) / 20.0f));
				ms->fx -= (int)(ms->fData1 / 17.4f);
			}
			else if(ms->fData2 / 32.0f > 0.0f)
			{
				ms->fy += (int)(fabs((fabs(ms->fy) - ms->fData2) * (iFrameNow - 5) / 20.0f));
				ms->fx -= (int)(ms->fData1 / 17.4f);
			}
			else if(ms->fData2 / 32.0f < 0.0f)
			{
				ms->fy += (int)(fabs(((ms->fy) + ms->fData2) * (iFrameNow - 5) / 20.0f));
				ms->fx -= (int)(ms->fData1 / 17.4f);
			}
		}
		
		ms->iData1 = iFrameNow;

		int x = (ms->ref->wOriginTileX - m_iTileStartX) * 64 - m_iDrawOffX + (int)ms->fx - 100;
		int y = (ms->ref->wOriginTileY - 2 - m_iTileStartY) * 32 - m_iDrawOffY + (int)ms->fy - 144;

		tex_show_t * tex = CreateTexShow();
		if (tex)
		{
			//预读图片
			if(ms->tFrameStart == m_dwTickCount)
				g_pTexMgr->PreLoad(PACKAGE_magic1,3201,3216,EP_MAGIC);

			tex->wTexID	= 3201 + iFrameNow;
			tex->wPackage = 0;
			tex->wAdd	= 0;
			tex->x		= x;
			tex->y		= y;
			tex->dwColor	= ms->dwColor;

			PushTexStore(ms,tex,0);
		}
	}
}

void CMagicManager::DealSpirtMonsterMsg(Magic_Show_s* ms)
{
	if (!ms) return;

	int iCurFrame = ms->GetFrame(GetTickCount());
	if(iCurFrame >= ms->pUnit->framereal - 3 && ms->iData3 == 0)
	{
		if (ms->pUnit->id == 4659)
		{
			//g_TalkMgr.PushSysTalk("成功捕捉到骑兽！");
			ms->iData3 = 1;
		}
		else if (ms->pUnit->id == 4756)
		{
			//g_TalkMgr.PushSysTalk("未能捕捉到骑兽！");
			ms->iData3 = 1;
		}
	}
}

void CMagicManager::DealMagicDulingbo(Magic_Show_s* ms)
{
	if(!ms)
		return; 

	ms->GetFrame(m_dwTickCount);

	if(ms->ref && ms->tFrameStart == m_dwTickCount && ms->iCurCycle ==0)
	{
		bool bHaveBackAction = false;

		CCharacterAttr *pChar = g_pGameData->FindCharacterByID(ms->ref->uTargetID);
		if (pChar)
		{
			SNextAction* pNextAction = pChar->GetNextAction();
			while(pNextAction)
			{
				if(pNextAction->uFlag & DELAY_ACTION)
				{
					pNextAction->uFlag &= ~DELAY_ACTION;
					bHaveBackAction = true;
				}
				pNextAction = pNextAction->pNext;
			}
		}

		if(ms->ref->uTargetID == SELF.GetID())
		{
			SNextAction* pNextAction = SELF.GetNextAction();
			while(pNextAction)
			{
				if(pNextAction->uFlag & DELAY_ACTION)
				{
					pNextAction->uFlag &= ~DELAY_ACTION;
					bHaveBackAction = true;
				}
				pNextAction = pNextAction->pNext;
			}
		}

		if(!bHaveBackAction)
		{
			CreateMagic(ms);
			FinishMagic(ms);
			return;
		}
	}

	ShowMagic(ms);
}

void CMagicManager::DealMagicShow(Magic_Show_s* ms)
{
	if(!ms || !ms->ref)
		return;

	UINT uID = ms->ref->uOwnerID;
	if(ms->attr & EMP_MAGIC_HIT_TARGET)
	{
		uID = ms->ref->uTargetID;
	}

	if(uID == SELF.GetID())
	{
		SELF.SetReserveData(pubDisableDraw,0);
	}
	else 
	{
		CSimpleCharacterNode * p = g_pGameData->FindSimpleCharacter(uID);
		if(p) 
		{
			p->SetReserveData(pubDisableDraw,0);
		}
	}

	CreateMagic(ms);
	FinishMagic(ms);
}


void CMagicManager::DealMagicHide(Magic_Show_s* ms)
{
	if(!ms || !ms->ref)
		return;

	UINT uID = ms->ref->uOwnerID;
	if(ms->attr & EMP_MAGIC_HIT_TARGET)
	{
		uID = ms->ref->uTargetID;
	}

	if(uID == SELF.GetID())
	{
		SELF.SetReserveData(pubDisableDraw,1);
	}
	else 
	{
		CSimpleCharacterNode * p = g_pGameData->FindSimpleCharacter(uID);
		if(p) 
			p->SetReserveData(pubDisableDraw,1);
	}

	CreateMagic(ms);
	FinishMagic(ms);
}

void CMagicManager::SetMagicState(Magic_Show_s* ms)
{
	if(!ms || !ms->ref)
		return;

	UINT uID = ms->ref->uOwnerID;
	if(ms->attr & EMP_MAGIC_HIT_TARGET)
	{
		uID = ms->ref->uTargetID;
	}

	if(uID == SELF.GetID())
	{
		SELF.SetUsingMagicType(ms->wSound);
	}
	else 
	{
		CSimpleCharacterNode * p = g_pGameData->FindSimpleCharacter(uID);
		if(p) 
			p->SetUsingMagicType(ms->wSound);
	}
	
	CreateMagic(ms);
	FinishMagic(ms);
}

void	CMagicManager::SetFaBaoState(Magic_Show_s* ms)
{
	if(!ms || !ms->ref)
		return;

	UINT uID = ms->ref->uOwnerID;

	if(uID == SELF.GetID())
	{
		SELF.HideFaBao(ms->byRev1);
	}
	else 
	{
		CSimpleCharacterNode * p = g_pGameData->FindSimpleCharacter(uID);
		if(p) 
			p->HideFaBao(ms->byRev1);
	}
	CreateMagic(ms);
	FinishMagic(ms);
}

void CMagicManager::DealShadowKill8Start(Magic_Show_s* ms)
{
	if(!ms || !ms->ref)
		return;

	UINT uID = ms->ref->uOwnerID;

	if(uID == SELF.GetID())
	{		
		SELF.SetUsingMagicType(1);
		SELF.SetInShadowKill8(true);
		SELF.SetReserveData(pubShadowKill8FrameStart,g_pGfx->GetFrameCount());
		//SELF.InitShadowNum(7);
		//SELF.SetShadowKill8Dir(2);

		SELF.InitAction(ACTION_STAND);
		SAction& sAction = SELF.GetAction();
		sAction.uFlag |= SHADOWKILL_ACTION;
	}
	else 
	{
		CSimpleCharacterNode * p = g_pGameData->FindSimpleCharacter(uID);
		if(p) 
		{
			p->SetUsingMagicType(1);
			p->SetInShadowKill8(true);
			p->SetReserveData(pubShadowKill8FrameStart,g_pGfx->GetFrameCount());

			p->InitAction(ACTION_STAND);
			SAction& sAction = p->GetAction();
			sAction.uFlag |= SHADOWKILL_ACTION;
		}
	}
	CreateMagic(ms);
	FinishMagic(ms);
}

//void	CMagicManager::TextOnHead(Magic_Show_s* ms)
//{
//	if(!ms || !ms->ref)
//		return;
//
//	UINT uID = ms->ref->uOwnerID;
//
//	DWORD TextEndTime = GetTickCount() + ms->byRev1 * 1000;
//	if(uID == SELF.GetID())
//	{
//		SELF.SetMagicSloganEndTime(TextEndTime);
//		//SELF.SetMagicSlogan(SELF.GetYIHUOINFO().strName);
//	}
//	else 
//	{
//		CSimpleCharacterNode * p = g_pGameData->FindSimpleCharacter(uID);
//		if(p) 
//		{
//			p->SetMagicSloganEndTime(TextEndTime);
//			//p->SetMagicSlogan(p->getyihuo);
//		}
//			
//	}
//
//	CreateMagic(ms);
//	FinishMagic(ms);
//}

void CMagicManager::DealSendMessage(Magic_Show_s* ms)
{
	if(!ms || !ms->ref)
		return;

	UINT uID = ms->ref->wMagicID;

	if(uID == MAGICID_LINGWU_CHENGYUN)
	{
		if (ms->ref->uOwnerID == SELF.GetID())
		{
			g_pGameControl->SEND_GoLingWu();
		}
		
		//SELF.SetUsingMagicType(ms->wSound);
	}
	else if(uID == MAGICID_JUEXING_ZHENHUO_LAN/* ||
						uID == MAGICID_JUEXING_ZHENHUO_HONG ||
						uID == MAGICID_JUEXING_ZHENHUO_JIN*/
			)
	{
		if (ms->ref->uOwnerID == SELF.GetID())
		{
			if (g_OtherData.GetSanWeiZhenHuoInfo().byFireLevel > 0)
				g_pControl->PopupWindow(MSG_CTRL_SANWEIFIRE_WND,OPER_RECREATE);
		}
	}
	
	CreateMagic(ms);
	FinishMagic(ms);
}

void CMagicManager::DealOwnerFly(Magic_Show_s* ms)
{
	if(!ms)
		return;

	if(ms->iData1 == 0)
	{
		ms->iData1 = 1;
		int iX,iY,ox,oy,iAX,iAY;
		GetXYFromID(ms->ref->uTargetID,iX,iY,ox,oy);
		if(iX == -1 || iY == -1 )							// 没有目标，使用坐标
		{
			iAX = (ms->ref->wTargetTileX - ms->ref->wOriginTileX) * TILE_WIDTH;
			iAY = (ms->ref->wTargetTileY - ms->ref->wOriginTileY) * TILE_HEIGHT;		
		}
		else												// 使用目标坐标
		{
			iAX = (iX - ms->ref->wOriginTileX) * TILE_WIDTH + ox; 
			iAY = (iY - ms->ref->wOriginTileY) * TILE_HEIGHT + oy; 		
		}

		//非法数据
		if (iAX == 0 && iAY == 0)
		{
			SELF.SetOffset(0,0);
			CreateMagic(ms);
			FinishMagic(ms);
			return;
		}

		ms->iOffX = 0;
		ms->iOffY = 0;
		ms->fx = 0;
		ms->fy = 0;
		
		float speed = 10.0f;
		if(ms->pUnit->id == 81 || ms->pUnit->id == 49)
		{
			speed = 16.0f;
		}
		else if (ms->pUnit->id == 616)
		{
			speed = 16.0f;
		}

		float s = sqrt((float)(iAX * iAX + iAY * iAY));
		float t = s / speed;
		ms->fData1 = iAX / t;
		ms->fData2 = iAY / t;
		ms->iData3 = (int)t;
		ms->iData4 = 0;
		

		int iDir = GetDir16(iAX,iAY,0,0);

		ms->iDir = iDir;
		if(ms->pUnit->dirs > 0 && ms->pUnit->dirs < 16)//ms->iDir是按16方向得到的，如果pUnit->dirs小于16方向，把ms->iDir转化成pUnit对应的方向
		{
			if(ms->pUnit->dirs == 8)
				ms->iDir = GetDir8(iAX,iAY,0,0);
			else
				ms->iDir = (iDir*ms->pUnit->dirs + 8)/16 % ms->pUnit->dirs;
		}		
	}

	ms->fx += ms->fData1;
	ms->fy += ms->fData2;
	
	ms->iOffX = (int)ms->fx;
	ms->iOffY = (int)ms->fy;

	int iDelayFinish = 0;

	ms->iData4++;
	if(ms->iData4 > ms->iData3)
	{
		iDelayFinish = 1;
	}
	else
	{
		iDelayFinish = 0;
	}

	int iNewX	= (ms->iOffX)/64 + ms->ref->wOriginTileX;
	int iNewY	= (ms->iOffY)/32 + ms->ref->wOriginTileY;

	
	int iox	= (ms->iOffX % 64);
	int ioy	= (ms->iOffY % 32);

	if(ms->ref->uOwnerID == SELF.GetID())
	{
		SELF.SetXY(iNewX,iNewY);
		if(iDelayFinish)
		{
			SELF.SetRealXY(iNewX,iNewY);
			SELF.SetOffset(0,0);
			SELF.InitAction(ACTION_STAND);
		}
		else
		{
			SELF.SetOffset(iox,ioy);
		}
	}
	else
	{
		CSimpleCharacterNode * p = g_pGameData->FindSimpleCharacter(ms->ref->uOwnerID);
		if(p) 
		{
			p->SetXY(iNewX,iNewY);
			if(iDelayFinish)
			{
				int iSelfX = 0,iSelfY = 0;
				p->SetRealXY(iNewX,iNewY);
				p->InitAction(ACTION_STAND);
				g_pGameData->AddSimpleCharacter(iNewX,iNewY,p->GetID());

				SELF.GetRealXY(iSelfX,iSelfY);
				
				p->SetOffset(0,0);
				if (ms->pUnit->id != 616)//八方分影斩
					p->SetDir(ms->iDir);
				
				if(abs(iNewX-iSelfX) > 12 || abs(iNewY-iSelfY) > 12)
				{
					MapArray.DeleteCharacter(p->GetID());
				}
			}
			else
			{
				p->SetOffset(iox,ioy);
			}
		}
	}

	if(ms->pUnit->id == 84)
	{
		ms->iOffX -= 128;
		ms->iOffY -= 128;
	}
	else if(ms->pUnit->id == 356)
	{
		ms->iOffX += 120;
		ms->iOffY += 40;
	}
	else if(ms->pUnit->id == 109)
	{
		ms->iOffX -= 128;
		ms->iOffY -= 128;
	}


	ShowMagic(ms);

	if(iDelayFinish == 1)
	{
		CreateMagic(ms);
		FinishMagic(ms);
	}
}

void CMagicManager::DealHeiYanMoRefraction(Magic_Show_s* ms)
{
	if (!ms || !ms->ref)
	{
		return;
	}

	if(ms->IsEnd(m_dwTickCount))
	{
		char *pData = ms->ref->pDynData;
		if(pData)
		{
			if(pData[0] < pData[1])
			{
				Magic_Show_s *pNew = NULL;
				pNew = g_pMagicCtrl->CreateMagic(MAGICID_HEIYANMO_REFRACTION,0,ms->ref->uTargetID,*((DWORD*)(pData + 2 + pData[0] * 4)) );

				if (pNew)
				{
					pData[0]++;

					pNew->ref->pDynData = ms->ref->pDynData;
					ms->ref->pDynData = NULL;
				}
			}
		}
	}

	ShowMagic(ms);
}


void CMagicManager::DealFlyTowerCycle(Magic_Show_s* ms)
{
	if (!ms || !ms->ref)
	{
		return;
	}
	
	if(SELF.GetX() == ms->ref->wTargetTileX && SELF.GetY() == ms->ref->wTargetTileY)
	{
		ShowMagic(ms);
		return;
	}

	CSimpleCharacterNode * p = MapArray.GetSimpleCharacterHead(ms->ref->wTargetTileX,ms->ref->wTargetTileY);	
	while(p)
	{
		if(!p->IsDead() && p->IsHuman())
		{
			ShowMagic(ms);
			return;
		}
		else
		{
			p = p->m_MapNext;
		}
	}
}

void CMagicManager::DealPopupPaoPao(Magic_Show_s* ms)
{
	if (!ms || !ms->ref)
	{
		return;
	}

	DWORD dwLastTime = GetTickCount() - ms->dwStartTime;
	if (ms->byRev1 ==1)
	{
		if(dwLastTime >= 2000)
		{
			FinishMagic(ms);
			return;
		}
	}
	else
	{
		if(dwLastTime >= 2000)
		{
			FinishMagic(ms);
			return;
		}
	}


	DWORD dwAlpha = 0xFF000000;
	char szTemp[128] = {0};

	if (ms->byRev1 >= 3 && ms->byRev1 <= 5)	//3:抵抗 4:反弹 5:破击
	{
		float fRate = (int)dwLastTime / 50.0f;
		if (fRate > 1.0f)
		{
			fRate = 1.0f;
		}

		float fScale = 0.30f + fRate * 0.20f;

		ms->fy = -120.0f;
		int iX = ms->ref->wOriginTileX,iY = ms->ref->wOriginTileY;				
		int x = 0,y = 0;
		DWORD dwColor = -1;//(0x00FFFFFF & ms->dwColor) | dwAlpha;

		if (ms->ref->uOwnerID == SELF.GetID())
		{
			SELF.GetPlayerXY(iX,iY);

			x = (iX - m_iTileStartX)*TILE_WIDTH + TILE_WIDTH / 2 - (int)(90 * fScale);//-图片宽度
			y = (iY - m_iTileStartY)*TILE_HEIGHT + (int)ms->fy;
		}
		else
		{
			x = (iX - m_iTileStartX)*TILE_WIDTH - m_iDrawOffX + ms->iOffX % TILE_WIDTH - (int)(90 * fScale);
			y = (iY - m_iTileStartY)*TILE_HEIGHT - m_iDrawOffY + (int)ms->fy - ms->iOffY;
		}


		tex_show_t * tex = CreateTexShow();
		if (tex)
		{
			tex->wTexID	= 44900 + ms->byRev1 - 3;
			tex->iType = EST_SCALE;
			tex->fScaleX = fScale;
			tex->fScaleY = fScale;
			tex->wPackage = 0;//PACKAGE_INTERFACE - PACKAGE_magic1;
			tex->x		= x;
			tex->y		= y;
			tex->dwColor = dwColor;
			PushTexStore(ms ,tex,0);
		}

	}
	else if (ms->byRev1 == 2)//加减血冒泡
	{
		bool bBaoJi = false;
		if (ms->iData1 < 0 && ms->iRev1 == 1)//暴击
		{
			bBaoJi = true;
		}

		float fRate = (int)dwLastTime / 500.0f;
		if (fRate > 1.0f)
		{
			fRate = 1.0f;
		}
		float fScale = 0.40f;// + (1 - fRate) * 0.70f;
		int iOffX = 0;
		ms->fy = -80 * fRate - 80.0f;
		
		if (bBaoJi)//暴击
		{
			fRate = (int)dwLastTime / 50.0f;
			if (fRate > 1.0f)
			{
				fRate = 1.0f;
			}
			fScale = 0.40f + fRate * 0.40f;
			iOffX = - 30;
			ms->fy = -120.0f;
		}

		int iX = ms->ref->wOriginTileX,iY = ms->ref->wOriginTileY;				
		int x = 0,y = 0;
		DWORD dwColor = -1;//(0x00FFFFFF & ms->dwColor) | dwAlpha;

		sprintf(szTemp,"%d",abs(ms->iData1));
		int iLen = 25;//"+" or "-"
		int iDigW[10] = {29,20,30,35,30,33,29,29,31,26};

		//先计算所有图加起来的宽度,以便居中
		char *p = szTemp;
		while(*p)
		{
			iLen += (int)(fScale * iDigW[*p - '0']);
			p ++;
		}

		if (ms->ref->uOwnerID == SELF.GetID())
		{
			SELF.GetPlayerXY(iX,iY);

			x = (iX - m_iTileStartX)*TILE_WIDTH - iLen / 2 + TILE_WIDTH /2 + iOffX;
			y = (iY - m_iTileStartY)*TILE_HEIGHT + (int)ms->fy;
		}
		else
		{
			x = (iX - m_iTileStartX)*TILE_WIDTH - m_iDrawOffX - iLen / 2 + ms->iOffX % TILE_WIDTH + iOffX;
			y = (iY - m_iTileStartY)*TILE_HEIGHT - m_iDrawOffY + (int)ms->fy - ms->iOffY;
		}


		tex_show_t * tex = CreateTexShow();
		if (tex)
		{
			tex->wTexID	= ms->iData1 > 0?9378:9379;
			tex->iType = EST_SCALE;
			tex->fScaleX = fScale;
			tex->fScaleY = fScale;
			tex->wPackage = PACKAGE_INTERFACE - PACKAGE_magic1;
			tex->x		= x;
			tex->y		= y;
			tex->dwColor = dwColor;
			PushTexStore(ms ,tex,0);
		}


		x += (int)(fScale * 25);
		p = szTemp;
		while(*p)
		{
			tex = CreateTexShow();
			if (tex)
			{
				tex->wTexID	= (ms->iData1 > 0?9390:9380) + *p - '0';
				tex->iType = EST_SCALE;
				tex->fScaleX = fScale;
				tex->fScaleY = fScale;
				tex->wPackage = PACKAGE_INTERFACE - PACKAGE_magic1;
				tex->x		= x;
				tex->y		= y;
				tex->dwColor	= dwColor;
				PushTexStore(ms ,tex,0);
			}

			x += (int)(fScale * iDigW[*p - '0']);
			p ++;
		}

	}
	else if (ms->byRev1 == 1)//资源冒泡
	{
		float fRate = (2000 - dwLastTime)/(float)(2000);
		if (fRate < 0.5)
		{
			dwAlpha = (((DWORD)(255 * (fRate + 0.5f))) << 24);
		}
		dwAlpha = max(dwAlpha,0x55FFFFFF);

		ms->fy = 150 * (fRate - 1) - 250;
		sprintf(szTemp,"%d",ms->iData1 % 10000);//>10000表示蓝方,否则表示红方
		int iLen = strlen(szTemp);
		int iDigW = 16;
		DWORD dwColor = 0x00FFFFFF | dwAlpha;

		int iOffX = iDigW * iLen / 2;
		int x = (ms->ref->wOriginTileX - m_iTileStartX)*TILE_WIDTH - m_iDrawOffX - (iDigW * iLen / 2) + 40;
		int y = (ms->ref->wOriginTileY - m_iTileStartY)*TILE_HEIGHT - m_iDrawOffY + (int)ms->fy;

		tex_show_t * tex = CreateTexShow();
		if (tex)
		{
			tex->wTexID	= 9371;
			tex->wPackage = PACKAGE_INTERFACE - PACKAGE_magic1;
			tex->x		= x;
			tex->y		= y;
			tex->dwColor = dwColor;
			PushTexStore(ms ,tex,0);
		}

		x += 18;
		char *p = szTemp;
		while(*p)
		{
			tex = CreateTexShow();
			if (tex)
			{
				if (ms->iData1 > 10000)
				{
					tex->wTexID	= 9344 + *p - '0';
				}
				else 
				{
					tex->wTexID	= 9332 + *p - '0';
				}

				tex->wPackage = PACKAGE_INTERFACE - PACKAGE_magic1;
				tex->x		= x;
				tex->y		= y;
				tex->dwColor	= dwColor;
				PushTexStore(ms ,tex,0);
			}


			x += iDigW;
			p ++;
		}
	}
}

void CMagicManager::DealDaoFighterPoison(Magic_Show_s* ms)
{
	if (!ms || !ms->ref)
	{
		return;
	}

	if(ms->IsEnd(m_dwTickCount))
	{
		if(!(ms->attr & EMP_MAGIC_WAIT_SERVER))
		{
			int iSelfX,iSelfY;
			SELF.GetXY(iSelfX,iSelfY);
			if( iSelfX < ms->ref->wTargetTileX ) //目标在主角右边
				g_pMagicCtrl->CreateMagic(MAGICID_DAO_FIGHTER_POISON_RIGHT,EMP_MAGIC_SERVER_ADVISE | EMP_MAGIC_NOTARGET | EMP_MAGIC_NOOWNER,MAKELONG(ms->ref->wTargetTileX,ms->ref->wTargetTileY),MAKELONG(ms->ref->wTargetTileX,ms->ref->wTargetTileY));
			else if(iSelfX > ms->ref->wTargetTileX )
				g_pMagicCtrl->CreateMagic(MAGICID_DAO_FIGHTER_POISON_LEFT,EMP_MAGIC_SERVER_ADVISE | EMP_MAGIC_NOTARGET | EMP_MAGIC_NOOWNER,MAKELONG(ms->ref->wTargetTileX,ms->ref->wTargetTileY),MAKELONG(ms->ref->wTargetTileX,ms->ref->wTargetTileY));
			else if(iSelfX == ms->ref->wTargetTileX )
				g_pMagicCtrl->CreateMagic(MAGICID_DAO_FIGHTER_POISON_MIDDLE,EMP_MAGIC_SERVER_ADVISE | EMP_MAGIC_NOTARGET | EMP_MAGIC_NOOWNER,MAKELONG(ms->ref->wTargetTileX,ms->ref->wTargetTileY),MAKELONG(ms->ref->wTargetTileX,ms->ref->wTargetTileY));
		}
	
		FinishMagic(ms);
		return;
	}

	ShowMagic(ms);

}
void CMagicManager::DealAlternativeMagic(Magic_Show_s* ms)
{
	if (!ms || !ms->ref)
	{
		return;
	}	

	if (ms->ref->wData[1] >= ms->wSound)
	{
		ShowMagic(ms);
	}
	else
	{
		FinishMagic(ms);
	}

}

void CMagicManager::DealSanWeiUp(Magic_Show_s* ms)
{
	if (!ms || !ms->ref)
	{
		return;
	}	

	CSimpleCharacter *pChar = g_pGameData->FindCharacterByID(ms->ref->uOwnerID);
	if (pChar)
	{
		int iFrameNow   = ms->GetFrame(m_dwTickCount);
		int iOffY = (m_dwTickCount - ms->dwStartTime) / 5;
		pChar->SetDrawOffset(0,-iOffY);
	}

	ShowMagic(ms);
}

void CMagicManager::DealFireShield(Magic_Show_s* ms)
{
	CCharacterAttr * pAttr = NULL;

	if(ms->ref->uOwnerID == SELF.GetID())
		pAttr = &SELF;				
	else 
		pAttr = g_pGameData->FindSimpleCharacter(ms->ref->uOwnerID);					

	if(pAttr && pAttr->IsOnHorse())
		ms->iOffY = -55;
	else
		ms->iOffY = -25;

	ShowMagic(ms);
}

void CMagicManager::DealLianJiZhuangji(Magic_Show_s* ms)
{
	if(!ms || !ms->ref)
		return;
	//记录撞击时间
	//连击数--创建时存储

	SELF.SetLianJiZhuangJiTime(GetTickCount());
	SELF.SetLianJiNumber(ms->ref->wData[0]);

	CreateMagic(ms);
	FinishMagic(ms);
}

void CMagicManager::DealFaJueState(Magic_Show_s* ms)
{
	if (!ms || !ms->ref)
	{
		return;
	}

	CSimpleCharacter *pChar = g_pGameData->FindCharacterByID(ms->ref->uOwnerID);
	if (pChar)
	{
		if (!pChar->IsUsingFaZhen())
		{
			//Magic_Show_s* ms = g_pMagicCtrl->FindMagicByAll(MAGICID_FABAO_LIANJIE,pChar->GetID());
			g_pMagicCtrl->FinishMagic(ms);
			return;
		}

		if (pChar->GetReserveData(pubDisableDraw) != 1)
		{
			int x = 0;
			int y = 0;
			pChar->GetTopXY(x,y);

			BYTE FaBao[3];
			FaBao[0] = ms->byRev1;
			FaBao[1] = (ms->wSound >> 8);
			FaBao[2] = (BYTE)(ms->wSound & 0xf);

			LPTexture pTexNor = NULL;
			LPTexture pTexAdd = NULL;

			DWORD CurFrameCount = g_pGfx->GetFrameCount();
			WORD FrameDelta = (WORD)((CurFrameCount / 4) % 16);

			for (int i=0; i < 3; ++i)
			{
				if (FaBao[i] > 0)
				{
					tex_show_t * texNor = CreateTexShow();
					tex_show_t * texAdd = CreateTexShow();

					if (texNor == NULL || texAdd == NULL)
					{
						continue;
					}

					if (FaBao[i] <= 11)
					{
						WORD frame = (FaBao[i] - 1) * 20 + FrameDelta;

						texNor->wTexID = 50283 + frame;
						texNor->wPackage = 0;

						texAdd->wTexID = 50503 + frame;
						texAdd->wPackage = 0;

					}
					else if (FaBao[i] == 38)
					{
						texNor->wTexID = 1670 + FrameDelta;
						texNor->wPackage = 2;

						texAdd->wTexID = 1920 + FrameDelta;
						texAdd->wPackage = 2;

					}

					if (i == 0)//主
					{
						texAdd->x		= x - 135;
						texAdd->y		= y - 110;				
					}
					else if ( i == 1)//左
					{
						texAdd->x		= x - 190;
						texAdd->y		= y + 30;	
					}
					else if ( i == 2)//右
					{
						texAdd->x		= x - 60;
						texAdd->y		= y + 30;	
					}
					texNor->wAdd	= 0;
					texAdd->wAdd	= 2;

					texNor->x = texAdd->x;
					texNor->y = texAdd->y;

					texNor->dwColor= 0xffffffff;
					texAdd->dwColor= 0xffffffff;

					PushTexStore(ms,texNor,0);
					PushTexStore(ms,texAdd,0);
				}
			}
		}
	}

	ShowMagic(ms);
}