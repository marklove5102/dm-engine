#include "AIAutoPickMgr.h"
#include "AIMgr.h"
#include "AIConfigMgr.h"
#include "AIPickCfgMgr.h"
#include "AIAutoMgr.h"
#include "AutoKillMonsterMgr.h"

#include "GameControl/GameControl.h"
#include "GameData/GameData.h"
#include "GameData/MagicDefine.h"
#include "GameData/GameGlobal.h"
#include "GameData/TalkMgr.h"
#include "WndClass/GameWnd/GameManager.h"
#include "GameData/OtherData.h"

///////////////////////////////////////////////////////////////////////
//流程：(旧的)
//1、在发现有极品时，去捡第一个发现的极品，其余极品加入队列中
//2、继续自动捡队列中的极品，将由下列三种情况下触发：
//   2.1自动走路，走完的时候AutoMoveFinish
//   2.2蝙蝠飞，待服务器返回成功后BatFlyFinish
//   2.3就在脚下，直接捡，等物品回到包裹里了，就继续下一步AddObject
//   2.4玩家自己走路，同上
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
//宠物捡物流程：
//在物品消失，宠物出现，扔到东西时触发
//流程复杂，请注意
///////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
CAIAutoPickMgr g_AutoPickMgr;

CAIAutoPickMgr::CAIAutoPickMgr(void)
{
	m_bAutoFly = false;
	m_bAutoStep = false;
	m_dwNotMyGoodID = 0;
	m_bOverWeight = false;
	m_pPickItem = NULL;
	m_iLimitRange = 3;
	m_dwPetGoodID = 0;
	m_iPetPickX = -1;
	m_iPetPickY = -1;
	m_dwLastNotifyTime = 0;
	m_iAutoX = m_iAutoY = 0;
	m_dwLastWalkPickTime = m_dwLastWalkPickItemID = m_dwLastPickItem = m_dwLastMoveTime = 0;
}

CAIAutoPickMgr::~CAIAutoPickMgr(void)
{
	m_listItem.clear();
	m_listPet.clear();
}

#define   TIMELIMITATION_MOVE  620

////////////////////////////////////////////////////////////////////////
//从地图上已有的物品，更新极品列表
void CAIAutoPickMgr::LoadBestItems()
{
	//重新生成极品列表
	m_listItem.clear();

	if(!g_AICfgMgr.IsGoPickup())
		return;

	MiniGood& _goods = MapArray.GetMiniGood();
	MiniGood::iterator it;

	for(it=_goods.begin();it!=_goods.end();it++)
	{
		CSimpleGoodNode* pItem = it->second;

		DWORD dwType = g_PickCfgMgr.GetShowType(pItem->GetName());
		pItem->SetShowType(dwType);

		if(g_PickCfgMgr.IsGoPick(dwType))
		{
			DisplayItem(pItem);
			m_listItem.push_back(pItem->GetID());
		}
	}
	m_pPickItem = NULL;
	if(m_listItem.size()>0)
	{
		AutoPickup();
	}
}

////////////////////////////////////////////////////////////////////////
//直接捡物品
void CAIAutoPickMgr::ItemPickup(int x,int y)
{
	if(SELF.IsDead())
		return;

	CSimpleGoodNode* pNode = MapArray.GetSimpleGoodHead(x,y);

	//没有物品，立即返回
	if(NULL == pNode)
		return;

// 	if (g_TrusteeshipData.IsTrusteeship() && !g_TrusteeshipData.IsCaptain())
// 		return;

	if(strcmp(pNode->GetName(),"金币") == 0) //自动拣取金币
	{
		g_pGameControl->SEND_Object_Pickup(x,y);
		return;
	}

	if(m_bOverWeight)
	{
		if (g_pGameData->IsAutoKillMonster())
		{
			int iItemX = -1,iItemY = -1;
			if (m_pPickItem)
			{
				m_pPickItem->GetXY(iItemX,iItemY);
			}

			bool bNeedDelItem = true;
			if (g_AICfgMgr.IsRejectBadItemWhenPickBestItem() && iItemX == x && iItemY == y)
			{
				int iSize = SELF.PackageGood().Size();
				for (int i = iSize - 1; i >= 0; i--)
				{
					DWORD dwType = g_PickCfgMgr.GetShowType(SELF.GetPackageGood(i).GetName());
					if (!g_PickCfgMgr.IsGoPick(dwType))
					{
						if(g_pGameControl->SEND_Package_Object_Reject(SELF.GetPackageGood(i)))
						{
							bNeedDelItem = false;
							m_bOverWeight = false;
						}

						break;
					}
				}
			}

			if(bNeedDelItem && EatForBestItem(x,y))	//为了捡极品自动喝药
			{
				bNeedDelItem = false;
				m_bOverWeight = false;
			}

			if(bNeedDelItem && m_pPickItem)
			{
				if (iItemX == x && iItemY == y)
				{
					if(strcmp(pNode->GetName(),"金币") != 0)//金币不删除
					{
						ItemDisappear(m_pPickItem->GetID());
					}

					m_pPickItem = NULL;
				}
			}
		}

		return;
	}

	//查找是否有需要捡的物品
	bool bExistAutoPickItem = false;

	if(g_AICfgMgr.IsAutoPickup())
	{
		while(pNode)
		{
			if(!m_bOverWeight && g_PickCfgMgr.IsAutoPick(pNode->GetShowType()))
			{
				bExistAutoPickItem = true;
				break;
			}
			pNode = pNode->m_MapNext;
		}
	}
	else if(g_AICfgMgr.IsGoPickup())
	{
		while(pNode)
		{
			if(!m_bOverWeight && g_PickCfgMgr.IsGoPick(pNode->GetShowType()))
			{
				bExistAutoPickItem = true;
				break;
			}
			pNode = pNode->m_MapNext;
		}
	}

	//不存在需要捡的物品
	if(!bExistAutoPickItem)
		return;

	pNode = MapArray.GetSimpleGoodHead(x,y);
	while(pNode)
	{
		g_pGameControl->SEND_Object_Pickup(x,y);
		if (!g_pGameData->HasStatus(29))
		{
			if((pNode->GetLooks() >= 168  && pNode->GetLooks() <= 171) || 
				pNode->GetLooks() == 731 || pNode->GetLooks() == 733   ||
				pNode->GetLooks() == 235 ||	pNode->GetLooks() == 237)
			{
				g_pGameControl->Send_Player_Prompt_Status(29,1);
			}
		}

		pNode = pNode->m_MapNext;
	}
}

////////////////////////////////////////////////////////////////////////
//丛林豹消失时清除
void CAIAutoPickMgr::PetPickupClear()
{
	m_dwPetGoodID = 0;
	m_iPetPickX = m_iPetPickY = -1;
}

void CAIAutoPickMgr::SetPetGood(DWORD id,int iItemX,int iItemY)
{
	m_dwPetGoodID = id;
	m_iPetPickX = iItemX;
	m_iPetPickY = iItemY;
}

////////////////////////////////////////////////////////////////////////
//物品是一个一个出现的
//新物品出现时需要调整行走的目的地
void CAIAutoPickMgr::ItemAppear(CSimpleGood* pItem)
{
	if(!g_AIAutoMgr.IsEnalbeWaiGua() || !pItem)
		return;


	DWORD dwType = g_PickCfgMgr.GetShowType(pItem->GetName());
	pItem->SetShowType(dwType);

	if (g_PickCfgMgr.IsGoPick(dwType))
	{
		DisplayItem(pItem);
		m_listItem.push_back(pItem->GetID());
		m_pPickItem = NULL;
		AutoPickup();
	}

	//吃精制豹粮
	if(strcmp(pItem->GetName(),"精制豹粮") == 0 || strcmp(pItem->GetName(),"极品豹粮") == 0 ||
		strcmp(pItem->GetName(),"精制兽粮") == 0 || strcmp(pItem->GetName(),"极品兽粮") == 0)
	{
		GotoEat(pItem);
		return;
	}

	//是宠物需要捡的物品
	if (g_PickCfgMgr.IsPetPick(dwType) || strcmp(pItem->GetName(),"金币") == 0)
	{
		m_listPet.push_back(pItem->GetID());
		m_dwPetGoodID = 0;
		PetPickup();
	}
}

////////////////////////////////////////////////////////////////////////
//物品消失
//如果是自己捡东西，则该消息在00C8消息[增加一个物品到包裹]之后收到
void CAIAutoPickMgr::ItemDisappear(DWORD id)
{
	if(!g_AIAutoMgr.IsEnalbeWaiGua())
		return;


	//物品消失了，走路也要停止
	if(m_pPickItem && m_pPickItem->GetID() == id)
	{
		m_pPickItem = NULL;
		if (SELF.GetMapPathFinder().GetDestX() == m_iAutoX && SELF.GetMapPathFinder().GetDestY() == m_iAutoY)
		{
			SELF.GetMapPathFinder().SetOnRoute(0);
		}
	}

	ListItem::iterator it = m_listItem.begin();
	while(it!=m_listItem.end())
	{
		DWORD itemid = *it;

		if(g_pGameData->FindSimpleGood(itemid) == NULL || itemid == id)
		{
			it = m_listItem.erase(it);
			continue;
		}
		it++;
	}

	//宠物捡物列表
	it = m_listPet.begin();
	while(it!=m_listPet.end())
	{
		DWORD itemid = *it;

		if (g_pGameData->FindSimpleGood(itemid) == NULL || itemid == id)
		{
			it = m_listPet.erase(it);
			continue;
		}
		it++;
	}

	//宠物在屏幕内
	if(SELF.GetMyPetID() != 0)
	{
		if(id == m_dwPetGoodID)
		{
			m_iPetPickX = m_iPetPickY = -1;
			m_dwPetGoodID = 0;
		}
		else//如果一个格子上有两个物品重叠，那么只会捡到一个，如果捡到的不是m_dwPetGoodID，那么就会傻掉不动，所以判断一下，如果格子相同也清除掉
		{
			CSimpleGood* pItem = g_pGameData->FindSimpleGood(id);
			if(pItem)
			{
				int iItemX = 0,iItemY = 0;
				pItem->GetXY(iItemX,iItemY);
				if(m_iPetPickX == iItemX && m_iPetPickY == iItemY)
				{
					m_iPetPickX = m_iPetPickY = -1;
					m_dwPetGoodID = 0;
				}
			}
		}

		if(m_listPet.size() > 0)
			PetPickup();
	}
}



////////////////////////////////////////////////////////////////////////
//系统消息
bool CAIAutoPickMgr::FilterRecv(const char* str)
{
	if(strcmp(str,"化身蝙蝠失败") == 0)
	{
		BatFlyFinish(0,0);
		return false;
	}

	if(m_pPickItem)
	{
		if(strcmp(str,"包裹格子或负重已满") == 0)
		{
			m_bOverWeight = true;
			bool bNeedDelItem = true;

			if(m_listItem.size()>0 && !SELF.IsDead())
			{
				if(EatForBestItem())	//为了捡极品自动喝药
				{
					bNeedDelItem = false;
					m_bOverWeight = false;
				}
			}

			if(bNeedDelItem && m_pPickItem && g_pGameData->IsAutoKillMonster())
			{
				int iSelfX = 0,iSelfY = 0;
				SELF.GetRealXY(iSelfX,iSelfY);
				CSimpleGoodNode* pNode = MapArray.GetSimpleGoodHead(iSelfX,iSelfY);
				while(pNode)
				{
					if(pNode->GetID() == m_pPickItem->GetID())
					{
						if (stricmp(m_pPickItem->GetName(),"金币") != 0)//金币不删除
						{
							ItemDisappear(m_pPickItem->GetID());
						}
						m_pPickItem = NULL;
						break;
					}
					pNode = pNode->m_MapNext;
				}
			}
		}
		else if(strcmp(str,"物品不属于你，一定时间范围内不能捡") == 0)
		{
			if (g_pGameData->IsAutoKillMonster())
			{
				ItemDisappear(m_pPickItem->GetID());
				m_pPickItem = NULL;
			}
			else
			{
				if(m_pPickItem->GetID() != m_dwNotMyGoodID)
				{
					m_dwNotMyGoodID = m_pPickItem->GetID();
					return false;
				}
				else
				{
					return true;   //过滤掉
				}
			}

		}
		else
		{
			return false;
		}
	}
	return false;
}

////////////////////////////////////////////////////////////////////////
//得到玩家能捡到的最近的极品
CSimpleGood* CAIAutoPickMgr::GetBestItem()
{
	if(m_listItem.size() == 0)
		return NULL;

	int iSelfX = 0,iSelfY = 0;
	SELF.GetRealXY(iSelfX,iSelfY);

	ListItem::iterator it = m_listItem.begin();
	CSimpleGood* pBestItem = NULL;
	int iMinDis = 20;

	ListItem::iterator it_min = m_listItem.end();

	while(it!=m_listItem.end())
	{
		DWORD itemid = *it;
		CSimpleGood* pItem = g_pGameData->FindSimpleGood(itemid);

		if(NULL == pItem)
		{
			it = m_listItem.erase(it);
			continue;
		}
		it++;

		int iItemX = 0,iItemY = 0;
		pItem->GetXY(iItemX,iItemY);

		if(IsHoldByOther(iItemX,iItemY))
			continue;

		//此算法优先左边和上面的物品，因为他们放在了列表的最前面
		//也是最先出现的
		int iDeltaX = abs(iItemX-iSelfX);
		int iDeltaY = abs(iItemY-iSelfY);
		int iDis = max(iDeltaX,iDeltaY); 
		if(iDis>1) //3x3格子以外
		{
			if(iDeltaX == 0)
				iDis = (iDeltaY+1) >> 1;
			else if(iDeltaY == 0)
				iDis = (iDeltaX+1) >> 1;
			else if(iDeltaX>iDeltaY)
				iDis = ((iDeltaY+1) >> 1)+((iDeltaX-iDeltaY) >> 1);
			else
				iDis = ((iDeltaX+1) >> 1)+((iDeltaY-iDeltaX) >> 1);
			iDis+=1; //步数+1
		}

		if(iDis<iMinDis)  //是极品
		{
			iMinDis = iDis;
			pBestItem = pItem;
			it_min = it;
			if(iDis == 0)
				break;
		}
	}
	return pBestItem;
}

CSimpleGood* CAIAutoPickMgr::GetNearestItem()
{
	if(m_listItem.size()>0)
	{
		DWORD itemid = m_listItem.front();
		CSimpleGood* pItem = g_pGameData->FindSimpleGood(itemid);
		if(pItem)
		{
			m_listItem.pop_front();
			m_listItem.push_back(itemid);
			return pItem;
		}
		else
		{
			m_listItem.pop_front();
			return NULL;
		}
		return NULL;
	}
	return NULL;
}

////////////////////////////////////////////////////////////////////////
//是否需要蝙蝠飞去捡
bool CAIAutoPickMgr::IsNeedFly(int iSelfX,int iSelfY,int iX,int iY)
{
	//太远了，飞不到
	if(abs(iX-iSelfX) > 8 || abs(iY-iSelfY) > 8)
		return false;

	if(!g_AICfgMgr.IsAllowFly())
		return false;

	return true;
}

bool CAIAutoPickMgr::IsHoldByOther(int iItemX,int iItemY)
{
	//判断是否有玩家或怪物已经占了这个位置
	CSimpleCharacterNode* pNode = NULL;
	pNode = MapArray.GetSimpleCharacterHead(iItemX,iItemY);
	while(pNode)
	{
		if(!pNode->IsDead())
		{
			return true;
		}
		pNode = pNode->m_MapNext;
	}
	return false;
}

void CAIAutoPickMgr::DisplayItem(CSimpleGood* pItem)
{
	if(!pItem)
		return;

	int iItemX = 0,iItemY = 0;
	pItem->GetXY(iItemX,iItemY);

	DWORD dwSelfID = SELF.GetID();
	int iSelfX = 0,iSelfY = 0;
	SELF.GetRealXY(iSelfX,iSelfY);

	const char* szDir = GetToDir(iSelfX,iSelfY,iItemX,iItemY);

	char temp[1024]={0};
	sprintf(temp,"发现物品: %s  方向: %s  坐标:(%d,%d)",pItem->GetName(),szDir,iItemX,iItemY);

	g_TalkMgr.PushShoutTalk(temp,strlen(temp),0xFCFF,dwSelfID,0xFF,FONT_DEFAULT,FONTSIZE_DEFAULT,TALKTYPE_SYSTEM);
}

////////////////////////////////////////////////////////////////////////
//自动捡物
void CAIAutoPickMgr::AutoPickup()
{
	if(!g_AICfgMgr.IsGoPickup() || SELF.IsDead())
		return;

	if(m_pPickItem)
		return;

	m_pPickItem = GetBestItem();

	if(m_pPickItem)
	{
		GoAndPickup();
	}
}

////////////////////////////////////////////////////////////////////////
//bFly表明在能够飞的情况下是否要飞
void CAIAutoPickMgr::GoAndPickup()
{
	if(!m_pPickItem)
		return;

	if (g_AutoKillMonsterMgr.HasAutoKillMonster())
	{
		return;
	}


	if(m_bAutoFly || m_bAutoStep)  //在飞,或者在走
	{
		if ( GetTickCount() - m_dwLastMoveTime > 2000)//防止没有回应一直不动
		{
			m_bAutoFly = m_bAutoStep = false;
		}
		return;
	}


	m_dwLastMoveTime = GetTickCount();

// 	if (g_TrusteeshipData.IsTrusteeship() && !g_TrusteeshipData.IsCaptain())
// 		return;

	if(SELF.GetMapPathFinder().IsOnRoute() && SELF.GetMapPathFinder().GetDestX() == m_iAutoX && SELF.GetMapPathFinder().GetDestY() == m_iAutoY) //在自动寻路的走了
	{
		return;
	}

	//是否要停止攻击
	bool bNeedClearAttackID = true;

	int iSelfX,iSelfY;
	SELF.GetRealXY(iSelfX,iSelfY);
	int iAutoX,iAutoY;
	m_pPickItem->GetXY(iAutoX,iAutoY);

	if (iSelfX == iAutoX && iSelfY == iAutoY) 
	{
		if (GetTickCount() - m_dwLastPickItem > 1000)
		{
			ItemPickup(iSelfX,iSelfY); 
			bNeedClearAttackID = false;
			m_dwLastPickItem = GetTickCount();
		}
	}
	//一步以内就直接捡了
	else if(IsInOneStep(iSelfX,iSelfY,iAutoX,iAutoY))
	{
		if(g_AIMgr.IsCanMove()) //可以跳，直接跳
		{
			m_bAutoStep = true;

			if(m_bRun)
				g_pGameControl->SEND_Player_Run(iAutoX,iAutoY);
			else
				g_pGameControl->SEND_Player_Walk(iAutoX,iAutoY);

			ItemPickup(iAutoX,iAutoY);
			SELF.SetXY(iAutoX,iAutoY);
		}
	}
	else
	{
		int iDistance = max(abs(iAutoX-iSelfX),abs(iAutoY-iSelfY));

		WORD wMagicID = 0;

		if(IsNeedFly(iSelfX,iSelfY,iAutoX,iAutoY) && g_AIMgr.IsCanFly(wMagicID))
			//能蝙蝠飞就飞过去
		{
			m_bAutoFly = true;
			g_pGameControl->SEND_Player_Attack_Magic(0,iAutoX,iAutoY,wMagicID);
		}
		else if(m_iLimitRange > 0 && iDistance <= m_iLimitRange)
		{
			//一定时间以内不要两次走路去捡同一个物品,防止去捡走一步又消失掉,自动打怪走路又出现,导致一直死循环
			if (m_dwLastWalkPickItemID == m_pPickItem->GetID() && GetTickCount() - m_dwLastWalkPickTime < 5000)
			{
				return;
			}

			//走到新的地方
			if(g_pGameMgr->AI_AutoWalk(iAutoX,iAutoY))
			{
				m_iAutoX = iAutoX;
				m_iAutoY = iAutoY;
				g_OtherData.SetFindPathReason(2);
				m_dwLastWalkPickItemID = m_pPickItem->GetID();
				m_dwLastWalkPickTime = GetTickCount();
			}
			else
			{
				ItemDisappear(m_pPickItem->GetID());
			}
		}
		else //太远了
		{
			m_pPickItem = NULL;
			bNeedClearAttackID = false;
		}
	}

	if (bNeedClearAttackID)
	{
		SELF.SetReserveData(plyAttackLockID,0);
		SELF.SetReserveData(plyMagicLockID,0);
	}
}

////////////////////////////////////////////////////////////////////////
void CAIAutoPickMgr::AutoMoveFinish()
{
	if(m_pPickItem)
		m_pPickItem = NULL;
}

void CAIAutoPickMgr::AutoMoveFaild()
{
	if (m_pPickItem)
	{
		// 移动失败，将物品删除，不然自动打怪会卡住不动(当周围没怪的时候)
		ItemDisappear(m_pPickItem->GetID());
		m_pPickItem = NULL;
	}
}

////////////////////////////////////////////////////////////////////////
//蝙蝠飞返回成功后调用
void CAIAutoPickMgr::BatFlyFinish(int x,int y)
{
	if (m_bAutoFly)
	{
		m_bAutoFly = false;
		ItemPickup(x,y);
	}
}

bool CAIAutoPickMgr::EatForBestItem(int iItemX,int iItemY)
{
	if (iItemX == -1 || iItemY == -1)
	{
		SELF.GetRealXY(iItemX,iItemY);
	}

	int iItemCount = 0;
	bool bExistBestItem = false;
	CSimpleGoodNode* pNode = MapArray.GetSimpleGoodHead(iItemX,iItemY);
	while(pNode)
	{
		if(g_PickCfgMgr.IsGoPick(pNode->GetShowType()))
			bExistBestItem = true;

		iItemCount++;
		pNode = pNode->m_MapNext;
	}

	if(bExistBestItem)
	{
		if(SELF.PackageGood().GetBlankNumber() > 0)
			return false;

		if(SELF.GetUsingTemp().GetGoodNumber() != 0) //已经在使用了
			return false;

		bool bIsEatWater = false;

		for(int i=0;i<iItemCount;i++)
		{
			int iPos = SELF.PackageGood().FindGoodByStdmode(0,0);
			if(iPos < 0)
				break;

			string szName = SELF.GetPackageGood(iPos).GetName();

			if(g_pGameControl->SEND_Use_Object(iPos)) //喝掉
			{
				char str[256]={0};
				sprintf(str,"捡极品自动喝掉： %s",szName.c_str());
				g_TalkMgr.PushSysTalk(str,TALKCOLOR_BLUE);
				bIsEatWater = true;
			}
		}


		if(bIsEatWater)
		{
			m_bOverWeight = false;
			ItemPickup(iItemX,iItemY);
		}

		return bIsEatWater;
	}

	return  false;
}

////////////////////////////////////////////////////////////////////////
//包裹中增加一个物品
void CAIAutoPickMgr::AddObject(CGood* pGood)
{
	if(NULL == pGood)
		return;

	//自动捡物时扔掉不需要的东西
	//if(addGood.GetID()>0 && g_AICfgMgr.IsAutoDrop(addGood.GetName()))
	//{
	//	SELF.GetPackageGood(iPos).SetID(0);
	//	g_pGameControl->SEND_Package_Object_Reject(addGood);
	//}

	if(SELF.GetMyPetID() == 0 && MAX_PET_PACKAGE > 0 &&
		pGood->GetID() == m_dwPetGoodID && m_listPet.size() > 0)
		//宠物在屏幕外，捡了一个宠物要去捡的东西(玩家，宠物？)
	{
		m_dwPetGoodID = 0;
		PetPickup();
	}
}

////////////////////////////////////////////////////////////////////////
//删除一个物品
void CAIAutoPickMgr::RemoveObject(DWORD id)
{
	if(m_bOverWeight)
	{
		m_pPickItem = NULL;
		m_bOverWeight = false;
		if(m_listItem.size()>0)
			AutoPickup();
	}
	if(m_listPet.size() > 0)
		PetPickup();
}

void CAIAutoPickMgr::EnableAutoGoPick(bool b)
{
	g_AICfgMgr.SetIsGoPickup(b);

	if(!b)
	{
		if(m_pPickItem)
		{
			m_pPickItem = NULL;
			//SELF.ClearPathLink();
			SELF.GetMapPathFinder().SetOnRoute(0);
		}
	}
	else
	{
		m_pPickItem = NULL;
		if(m_listItem.size()>0)
			AutoPickup();
	}
}

////////////////////////////////////////////////////////////////////////
void CAIAutoPickMgr::DoLoop()
{
	if(m_pPickItem)
	{
		int iItemX = 0,iItemY = 0;
		m_pPickItem->GetXY(iItemX,iItemY);
		if(!IsHoldByOther(iItemX,iItemY))
			GoAndPickup();
		else
		{
			m_pPickItem = NULL;
			if(m_listItem.size()>0)
				AutoPickup();
		}
	}
	else
	{
		if(m_listItem.size()>0)
			AutoPickup();
		else if (m_listPet.size() > 0)//如果这里不处理的话,骑上豹子后物品出现再下豹子就不会去捡之前出现的东西了,除非再出现一个新的东西
			PetPickup();
	}

	if(g_AICfgMgr.IsObjectPrompt())
	{
		if(GetTickCount() > (m_dwLastNotifyTime+g_AICfgMgr.GetObjectPromptTime()))
		{
			CSimpleGood* pItem = GetNearestItem();
			if(pItem)
			{
				DisplayItem(pItem);
			}
			m_dwLastNotifyTime = GetTickCount();
		}
	}
}

bool CAIAutoPickMgr::IsInOneStep(int iSelfX,int iSelfY,int iItemX,int iItemY)
{
	if(SELF.IsOnHorse())
		return false;

	char cDir = 0;
	int iDeltaX = iItemX - iSelfX;
	int iDeltaY = iItemY - iSelfY;
	int iADeltaX = abs(iDeltaX);
	int iADeltaY = abs(iDeltaY);

	if(iADeltaX>2 || iADeltaY>2 || (iADeltaX== 1 && iADeltaY == 2) || (iADeltaY == 1 && iADeltaX == 2))
		return false;

	if(SELF.IsBianShen() && (iADeltaX == 2 || iADeltaY == 2)) //变身时超过两格走去捡
		return false;

	m_cDir = GetDir8(iDeltaX,iDeltaY,0,0);
	if(iADeltaX == 2 || iADeltaY == 2)
	{
		if(IsHoldByOther((iSelfX+iItemX)/2,(iSelfY+iItemY)/2))
			return false;

		if (!g_pGameMgr->IsCanGo(iSelfX,iSelfY,m_cDir,2))
		{
			return false;
		}

		m_bRun = true;
	}
	else
		m_bRun = false;

	return true;
}

void CAIAutoPickMgr::MoveFailed(int x,int y)
{
	if(m_bAutoFly)
		m_bAutoFly = false;

	if(m_bAutoStep)
		m_bAutoStep = false;
}

void CAIAutoPickMgr::MoveSuccess()
{
	if(m_bAutoStep)
	{
		m_bAutoStep = false;

		if(m_pPickItem)
			m_pPickItem = NULL;
	}
}

void CAIAutoPickMgr::Clear()
{
	m_listItem.clear();
	m_listPet.clear();
	m_bAutoStep = false;
	m_bAutoFly = false;
	m_dwNotMyGoodID = 0;
	m_bOverWeight = false;
	m_pPickItem = NULL;
	m_iLimitRange = 3;

	m_dwPetGoodID = 0;
	m_iPetPickX = -1;
	m_iPetPickY = -1;
}

////////////////////////////////////////////////////////////////////////////////////////
//宠物捡物
CSimpleGood* CAIAutoPickMgr::GetPetItem()
{
	if(m_listPet.size() == 0)
		return NULL;

	DWORD dwMyPetID = SELF.GetMyPetID();
	if(0 == dwMyPetID)
		return NULL;

	CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(dwMyPetID);
	if(NULL == pChar)
		return NULL;

	//宠物的级别
	int    iMyPetLevel = SELF.GetMyPetLevel();
	__time32_t tMyPetTime  = SELF.GetMyPetTime();//豹子饿的时间的前一天
	bool   bIsOldPet   = SELF.IsMyOldPet();

	if((bIsOldPet && iMyPetLevel < 1) || tMyPetTime == 0) //如果是没有升级过的豹子，要判断等级
		return NULL;

	//计算饱或饿的状态
	__time32_t t_time;
	_time32(&t_time);
	t_time += g_dwServerTime;
	int iDays = (int)((t_time - tMyPetTime) / (24*3600));

	if(iDays > 3)//饿了3天说明豹子死了
		return NULL;

	bool   bNotPickItem = false;
	if(SELF.IsMyOldPet()) //如果是没有升级过的豹子，要判断等级
	{
		if(iMyPetLevel == 1 || (iMyPetLevel > 1 && iDays >= 2))
			bNotPickItem = true;
	}
	else
	{
		if(iDays >= 2)
			bNotPickItem = true;
	}
	//----------------

	int iPetX = 0,iPetY = 0;
	pChar->GetRealXY(iPetX,iPetY);

	ListItem::iterator it = m_listPet.begin();
	CSimpleGood* pBestItem = NULL;
	int iMinDis = 20;

	ListItem::iterator it_min = m_listPet.end();

	while(it!=m_listPet.end())
	{
		DWORD itemid = *it;
		CSimpleGood* pItem = g_pGameData->FindSimpleGood(itemid);

		if(NULL == pItem)
		{
			it = m_listPet.erase(it);
			continue;
		}
		it++;

		if(bNotPickItem && stricmp(pItem->GetName(),"金币") != 0)
			continue;

		int iItemX = 0,iItemY = 0;
		pItem->GetXY(iItemX,iItemY);

		int iDis = max(abs(iItemX-iPetX),abs(iItemY-iPetY)); 

		if(iDis<iMinDis)  //贵重物品
		{
			iMinDis = iDis;
			pBestItem = pItem;
			it_min = it;
			if(iDis == 0)
				break;
		}
	}
	return pBestItem;
}

void CAIAutoPickMgr::PetPickup()
{
	if(!g_AICfgMgr.IsPetPickup())
		return;

	if(SELF.GetMyPetID() == 0)   //宠物还没有放出等
		return;

	if(m_dwPetGoodID != 0)     //已经在捡一个物品了
		return;

	CSimpleGood* pPetPickItem = GetPetItem();
	if(NULL == pPetPickItem)
	{
		return;
	}

	int iItemX = 0,iItemY = 0;
	pPetPickItem->GetXY(iItemX,iItemY);

	if(m_iPetPickX == iItemX && m_iPetPickY == iItemY)
		return;

	g_pGameControl->SEND_Pet_Object_Pickup(iItemX,iItemY);

	m_dwPetGoodID = pPetPickItem->GetID();
	m_iPetPickX = iItemX;
	m_iPetPickY = iItemY;
}

void CAIAutoPickMgr::PetItemPickupFail(DWORD id,WORD wErr)
{
	//将此物品从宠物捡物列表中删除
	ListItem::iterator it = m_listPet.begin();
	while(it!=m_listPet.end())
	{
		DWORD itemid = *it;

		if (g_pGameData->FindSimpleGood(itemid) == NULL || itemid == id)
		{
			it = m_listPet.erase(it);
			continue;
		}
		it++;
	}

	if(id == m_dwPetGoodID)
	{
		m_iPetPickX = m_iPetPickY = -1;
		m_dwPetGoodID = 0;

		if(wErr == 1 || wErr == 2 || wErr == 4 || wErr == 5) //物品不可捡，马上换目标
		{
			if(m_listPet.size() > 0)  //有新的物品就捡新的物品
				PetPickup();
			else
			{
				int iSelfX = 0,iSelfY = 0;
				SELF.GetRealXY(iSelfX,iSelfY);
				g_pGameControl->SEND_Pet_Object_Pickup(iSelfX,iSelfY);
			}
		}
		else if(wErr == 3)
		{
			CSimpleGood* pItem = g_pGameData->FindSimpleGood(id);
			if(pItem == NULL || !g_PickCfgMgr.IsBest(pItem->GetShowType())) //不是极品
			{
				if(m_listPet.size() > 0)
					PetPickup();
				else
				{
					int iSelfX = 0,iSelfY = 0;
					SELF.GetRealXY(iSelfX,iSelfY);
					g_pGameControl->SEND_Pet_Object_Pickup(iSelfX,iSelfY);
				}
			}
			//如果是极品就不动
		}
		else if(wErr == 6) //无此功能
		{
			if(m_listPet.size() > 0)  //有新的物品就捡新的物品
				PetPickup();
			else
			{
				int iSelfX = 0,iSelfY = 0;
				SELF.GetRealXY(iSelfX,iSelfY);
				g_pGameControl->SEND_Pet_Object_Pickup(iSelfX,iSelfY);
			}
		}
	}
}

void CAIAutoPickMgr::PetPackageCapacity(int iSize)
{
	if(m_listPet.size() > 0)
		PetPickup();
}

void CAIAutoPickMgr::GotoEat(CSimpleGood* pItem)
{
	if(pItem == NULL)
		return;

	if(SELF.GetMyPetID() == 0)   //宠物还没有放出等
		return;

	int iItemX = 0,iItemY = 0;
	pItem->GetXY(iItemX,iItemY);

	if(m_iPetPickX == iItemX && m_iPetPickY == iItemY)
		return;

	g_pGameControl->SEND_Pet_Object_Pickup(iItemX,iItemY);

	m_dwPetGoodID = pItem->GetID();
	m_iPetPickX = iItemX;
	m_iPetPickY = iItemY;
}

bool CAIAutoPickMgr::NeedPickUpGood()
{
	if(m_pPickItem && !g_AutoKillMonsterMgr.HasAutoKillMonster())
	{
		return true;
	}

	return false;
}
