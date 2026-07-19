#include "EffectMsgFilter.h"
#include "GameData/ItemCfgMgr.h"
#include "GameData/GameData.h"
#include "GameData/MagicCtrlMgr.h"
#include "Global/Interface/AudioInterface.h"
#include "BaseClass/Control/Control.h"
#include "GameAI/AIMgr.h"
#include "GameData/OtherData.h"
#include "GameAI/AIAutoEatMgr.h"
#include "WndClass/GameWnd/GameManager.h"
#include "GameMap/GameMap.h"
#include "GameAI/AIConfigMgr.h"

CEffectMsgFilter g_EffectMsgFilter;

CEffectMsgFilter::CEffectMsgFilter(void)
{
}

CEffectMsgFilter::~CEffectMsgFilter(void)
{
}

bool CEffectMsgFilter::DoFilter(DWORD id,WORD wItemType,WORD iX,WORD iY,DWORD dwColor)
{
	int iSelfX,iSelfY;
	SELF.GetXY(iSelfX,iSelfY);

	char sItemType[256] = {0};
	sprintf(sItemType,"%d",wItemType);
	SItem2MagicEffect sItem2Magic;

	Magic_Show_s* pMagic = NULL;

	if(g_ItemCfgMgr.ParseItem2Magic("item2magic",sItemType,sItem2Magic))
	{
		if(sItem2Magic.iMagicID > 0) //标准的特效只要在配置文件里面配就可以了，如果想由代码启动之，就配0
		{
			DWORD dwOwnerID = id;
			DWORD dwTargetID = 0;
			DWORD dwMagicType  = EMP_MAGIC_SERVER_ADVISE;
			if(sItem2Magic.iUsedXY == 1)
			{
				dwMagicType |= EMP_MAGIC_NOOWNER;
				dwOwnerID = MAKELONG(iX,iY);
			}
			else if(sItem2Magic.iUsedXY == 2)
			{
				dwTargetID = MAKELONG(iX,iY);
			}

			pMagic = g_pMagicCtrl->CreateMagic(sItem2Magic.iMagicID,dwMagicType,dwOwnerID,dwTargetID,-1,0xFFFFFFFF,sItem2Magic.iFx,sItem2Magic.iFy);
		}
	}

	if(id == SELF.GetID())
		SELF.SetReserveData(plyJumpMap,0); //清除跨地图标记，如果在特效播放后出现跨地图则此变量为变为1

	if (sItem2Magic.iMagicID == 4338 && id == SELF.GetID())//回城神石
	{
		SELF.SetReserveData(plyMagicLockID,0);
		SELF.SetReserveData(plyAttackLockID,0);
	}

	switch(wItemType)
	{
	case 1:   //天山雪莲
		{
			SELF.SetReserveData(plyDrinkSuperRed,iX);
			break;
		}
	case 2:  //深海灵礁
		{
			SELF.SetReserveData(plyDrinkSuperBlue,iX);
			break;
		}
	case 9: //泡泡爆炸
		{
			BYTE cRange[4] = {0};
			memcpy(cRange,&id,4);

			int    iCenterX = iX;
			int    iCenterY = iY;

			//泡泡爆炸音乐
			g_pAudio->PlayEx(EAT_MAGIC,2008,g_pAudio->GetRand()++,iSelfX,iCenterX,iSelfY,iCenterY,false);

			for(int i = 3;i >= 0; i--)
			{
				int x = iCenterX , y = iCenterY;
				if(i == 3) iCenterY--;

				for(int j= (i == 3)?0:1; j <= cRange[i]; j++)
				{
					if(i == 3)  y--;
					else if(i == 2) x++;
					else if(i == 1) y++;
					else  x--;

					g_pMagicCtrl->CreateMagic(MAGICID_PAOPAO_BLAST,EMP_MAGIC_NOTARGET,SELF.GetID(),MAKELONG(x,y));
				}
			}
			break;
		}
	case 19:// 是指屏幕震动 iX=震动时间(毫秒)
		{
			if( iX>0 && iX <10)
			   g_AIMgr.StartShake(iX*1000);
			break;
		}
	case 20: // 关闭香火特效指令
		{
			g_OtherData.SetCenserStartTime(0);
			g_OtherData.SetCenserDurTime(0);
			g_OtherData.SetCenserStart(false);
			break;
		}
	case 26:  //注意:这个特效规定只能自己看得到! 香火特效
		{
			if(SELF.GetID() == id && iX > 10)//约定第6的字段
			{
				if(iY == 0)//iX为香的可烧时间也是最大值，开始时间为现在
				{
					g_OtherData.SetCenserStartTime(GetTickCount());
					g_OtherData.SetCenserDurTime( (iX-10)*1000 );//减去10秒钟香炉过渡消失的时间
				}
				else//iY表示最大值，香最大长度时的时间值,iX为还可以烧多少时间，开始时间为GetTickCount()-(iY-iX)*1000
				{
					g_OtherData.SetCenserStartTime(GetTickCount()-(iY-iX)*1000);
					g_OtherData.SetCenserDurTime( (iY-10)*1000 );//减去10秒钟香炉过渡消失的时间
				}
				g_OtherData.SetCenserStart(true);
		    }
			break;
		}
	//case 37://元神合一特效(男)
	//	{
	//		g_pMagicCtrl->CreateMagic(MAGICID_PLAYER_APPEAR_YUANSHEN,EMP_MAGIC_NOOWNER,MAKELONG(434,171));
	//	    break;
	//	}	
	//case 38://元神合一特效(女)
	//	{
	//		g_pMagicCtrl->CreateMagic(MAGICID_PLAYER_APPEAR_YUANSHEN2,EMP_MAGIC_NOOWNER,MAKELONG(434,171));
	//	    break;
	//	}	
	case 39:  //烟花筒
		{
			//2006-2-5 xms
			//1.先检查该id是否已经在放烟花筒特效(防止服务端给一个id发送多次特效指令,进行纠错)
			//2.如果没有在放,那么在队列中增加该id的特效
			if(g_pMagicCtrl->FindMagicByAttr(MAGICID_PLAYER_YANHUATONG,id,EMP_MAGIC_NOTARGET))
				return false;

			if(iX > 0 && iX < 500)
			{
				pMagic = g_pMagicCtrl->CreateMagic(MAGICID_PLAYER_YANHUATONG,EMP_MAGIC_NOTARGET,id);
				if(pMagic)
				{      
					pMagic->iCycles = (WORD)(iX/1.6);//烟花播放的时间(秒) 
				}
			}
			break;
		}
	//case 42: //元神消散的特效
	//	{
	//		CSimpleCharacterNode* pChar = g_pGameData->FindSimpleCharacter(id);
	//		if(pChar == NULL)
	//			return false;

	//		//if( id == MCHAR.GetID() )
	//		//	MCHAR.SetMerOnline(false);

	//		pMagic = g_pMagicCtrl->CreateMagic(MAGICID_PLAYER_QUIT_YUANSHEN,0,id,0);
	//		break;
	//	}
	//case 43: //元神瞬移的特效
	//	{
	//		if(pMagic)
	//		{
	//			pMagic->ref->wOriginTileX = 430;
	//			pMagic->ref->wOriginTileY = 172;
	//			pMagic->ref->wTargetTileX = 433;
	//			pMagic->ref->wTargetTileY = 169;
	//		}
	//		break;
	//	}
	case 47:  //放新豹子特效
		{
			SELF.SetReserveData(pubLastCloseId,id);
			break;
		}
	case 50: //豹子吼叫 怪物晕眩
		{
			CSimpleCharacterNode * pChar = g_pGameData->FindSimpleCharacter(id);
			if(NULL == pChar)
				break;
			if(iX > 0 && iX < 500)
			{
				pMagic = g_pMagicCtrl->CreateMagic(MAGICID_MONSTER_DIZZY,0,id,0);
				if(pMagic)
				{
					pMagic->show_node->cycle = (int)(iX/6)+2;
				}		
			}
			break;
		}
	case 51: //豹子吼叫 怪物晕眩
		{
			bool bPhenix = false;
			CSimpleCharacter * pChar = g_pGameData->FindSimpleCharacter(id);

			if(NULL == pChar && SELF.GetID() == id)
			{
				SELF.SetDir(BYTE(iY));
				bPhenix = SELF.IsOnPhenix();
			}
			else
			{
				pChar->SetDir(BYTE(iY));
				if((pChar->IsHuman() && pChar->IsOnPhenix()) || pChar->GetRaceNo() == 301 || pChar->GetRaceNo() == 302)
				{
					bPhenix = true;
				}
			}

			if(id > 0 && iX > 0 && iX < 500)
			{
				if(bPhenix)
				{
					pMagic = g_pMagicCtrl->CreateMagic(MAGICID_FENGHUANG_HOU,0,id,0,2 * iY);
				}
				else
				{
					pMagic = g_pMagicCtrl->CreateMagic(MAGICID_MONSTER_DIZZY2,0,id,0,2 * iY);	
				}
			}
			break;
		}
	//case 52:  //元神境界分期升级特效
	//	break;
	//case 55://借入借出元神
	//	break;
	case 56:
		{
			if(pMagic)
			{
				pMagic->ref->wTargetTileX = iX;
				pMagic->ref->wTargetTileY = iY;
			}
			break;
		}
	//case 61://俘获心魔
	//	break;
	//case 62://培元期心魔出现(只是调一下出场的渐变)
	//	break;
	//case 67:  //囚禁心魔特效
	//	break;
	//case 70:  //心魔宝宝必杀技能击中特效，真元攻击特效
	//	break;
	//case 74: //假心魔死亡时，除了假心魔死亡特效4626外,如果是在副本地图要求放一个从死亡怪物到龙珠的特效
	//	break;
	//case 75://天魔宝藏喷宝
	//	break;
	//case 77://在天魔宝藏地图中打心魔时从心魔飞到人身上的特效
	//	break;
	//case 79://设置最后一次使用辟邪丹的时间及最后一次使用的时间
	//	break;
	case 80:
		{
			CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(id);
			if(pChar)
			{
				BubbleUpStruct &expBubbleUp = pChar->NewBubbleUp();
				expBubbleUp.dwStartTime = GetTickCount();
				expBubbleUp.dwValue = iX;
				expBubbleUp.byType = 1;	
			}
		}
		break;
	case 82:
		{
			CGood *pGood  = SELF.PackageGood().FindGood(id);
			if(pGood)
			{
				//圣殿精华升宝石		
				GoodAddEffect &goodEffect = pGood->GetGoodAddEffect();
				goodEffect.dwStartID = MAKELONG(1070,PACKAGE_stateitem);
				goodEffect.iFrams = 15;
				goodEffect.RM = RM_ADD2;
				goodEffect.iOffX = -63;
				goodEffect.iOffY = -93;
				goodEffect.iSoundID = 3;
			}

			break;	
		}
	case 83:
		g_pControl->PopupWindow(MSG_CTRL_CHARWND,OPER_CREATE);
		if(iX == 2)
		{
			g_pControl->MsgToWnd(MSG_CTRL_CHARWND,MSG_CTRL_EQUIPMENT_EFFECT,3);
		}
		else
			g_pControl->MsgToWnd(MSG_CTRL_CHARWND,MSG_CTRL_EQUIPMENT_EFFECT,1);
		break;
	//case 85://内聚真元特效动画
	//	if(id == SELF.GetID())
	//	{
	//		g_OtherData.SetSecondKillStatus(true);
	//		g_pControl->PopupWindow(MSG_CTRL_CHARWND,OPER_RECREATE);		
	//		g_pControl->MsgToWnd(MSG_CTRL_CHARWND,MSG_CTRL_EQUIPMENT_EFFECT,13);//装备栏播放真元特效
	//		g_pAudio->Play(EAT_OTHER,914,g_pAudio->GetRand()++);	
	//	}		
	//	break;
	//case 86://结丹特效动画
	//	if(id == SELF.GetID())
	//	{
	//		g_pControl->PopupWindow(MSG_CTRL_CHARWND,OPER_RECREATE);
	//		g_pControl->MsgToWnd(MSG_CTRL_CHARWND,MSG_CTRL_EQUIPMENT_EFFECT,12);//装备栏播放结丹特效
	//		g_pAudio->Play(EAT_OTHER,915,g_pAudio->GetRand()++);
	//	}			
	//	break;
	//case 87://化婴特效动画
	//	if(id == SELF.GetID())
	//	{
	//		g_pControl->PopupWindow(MSG_CTRL_CHARWND,OPER_RECREATE);
	//		g_pControl->MsgToWnd(MSG_CTRL_CHARWND,MSG_CTRL_EQUIPMENT_EFFECT,11);//装备栏播放化婴特效
	//		g_pAudio->Play(EAT_OTHER,916,g_pAudio->GetRand()++);		
	//	}	

	//	break;
	//case 88:
	//	g_pMagicCtrl->CreateMagic(MAGICID_PLAYER_APPEAR_YUANSHEN2_MALE,EMP_MAGIC_NOOWNER,MAKELONG(25,14));
	//	break;
	//case 89:
	//	g_pMagicCtrl->CreateMagic(MAGICID_PLAYER_APPEAR_YUANSHEN2_FEMALE,EMP_MAGIC_NOOWNER,MAKELONG(25,14));
	//	break;
	case 91:
		{			
		}
		break;
    case 92:    //五行玄关召唤宝宝消失特效
        {
            if( iX > 0 && iX < g_pGameMap->GetWidth() && iY > 0 && iY < g_pGameMap->GetHeight())
            {
                g_pMagicCtrl->CreateMagic(MAGICID_SEAL_MONSTER_DISAPPEAR,EMP_MAGIC_NOTARGET,0,MAKELONG(iX,iY));//固定坐标                
            }
        }
        break;
    case 93:
        {
            g_pMagicCtrl->CreateMagic(MAGICID_WXXG_MONSTER_SPIRIT,0,id,id);
        }
        break;
	case 94:
		{
			CCharacterAttr* p = g_pGameData->FindCharacterByID(id);
			int x,y;
			if(p)
			{
				p->GetXY(x,y);
				g_pMagicCtrl->CreateMagic(4684,EMP_MAGIC_NOOWNER,MAKELONG(x,y));
			}			
		}
		break;
	case 95:
		{
			Magic_Show_s* pShow = g_pMagicCtrl->FindMagicByAll(MAGICID_DULINGBO_ATTACK);
			if(pShow)
			{
				//pShow->attr = pShow->attr & (~EMP_MAGIC_WAIT_SERVER);
				g_pMagicCtrl->FinishMagic(pShow);
			}
		}
        break;
    case 96:
        {
			CCharacterAttr* p = g_pGameData->FindCharacterByID(id);
			int x,y;
			if(p)
			{
				p->GetXY(x,y);
				g_pMagicCtrl->CreateMagic(127,EMP_MAGIC_NOOWNER,MAKELONG(x,y));
			}
        }
        break;
    case 97:
        break;
	case 98:
		{
			if(strnicmp(g_pGameMap->GetMapName(),"MAZE",4) == 0)
			{
				g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE1);
				g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE2);
				g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE3);
				g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE4);
				g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE9);
				g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE10);
				g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE11);
				g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE12);

				Magic_Show_s* ms = NULL;
				ms = g_pMagicCtrl->FindMagicByID(MAGICID_MIGONG_BGIDGE5);
				if ((iX & 0x0002) != 0 && !ms)
					g_pMagicCtrl->CreateMagic(MAGICID_MIGONG_BGIDGE5,EMP_MAGIC_NOOWNER|EMP_MAGIC_NOTARGET,0,MAKELONG(51,18));//固定坐标
				else if((iX & 0x0002) == 0 && ms)
					g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE5);

				ms = g_pMagicCtrl->FindMagicByID(MAGICID_MIGONG_BGIDGE6);
				if ((iX & 0x0008) != 0 && !ms)
					g_pMagicCtrl->CreateMagic(MAGICID_MIGONG_BGIDGE6,EMP_MAGIC_NOOWNER|EMP_MAGIC_NOTARGET,0,MAKELONG(65,40));//固定坐标
				else if((iX & 0x0008) == 0 && ms)
					g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE6);

				ms = g_pMagicCtrl->FindMagicByID(MAGICID_MIGONG_BGIDGE7);
				if ((iX & 0x0020) != 0 && !ms)
					g_pMagicCtrl->CreateMagic(MAGICID_MIGONG_BGIDGE7,EMP_MAGIC_NOOWNER|EMP_MAGIC_NOTARGET,0,MAKELONG(51,54));//固定坐标
				else if((iX & 0x0020) == 0 && ms)
					g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE7);

				ms = g_pMagicCtrl->FindMagicByID(MAGICID_MIGONG_BGIDGE8);
				if ((iX & 0x0080) != 0 && !ms)
					g_pMagicCtrl->CreateMagic(MAGICID_MIGONG_BGIDGE8,EMP_MAGIC_NOOWNER|EMP_MAGIC_NOTARGET,0,MAKELONG(35,40));//固定坐标
				else if((iX & 0x0080) == 0 && ms)
					g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE8);

				if (iY < 49 && iY >= 0)
				{
					_MazeMap& MazeMap = g_OtherData.GetMazeMap();
					//if(((iX>>8) & 0x04) != 0 && ((iX>>8) & 0x01) == 0)
					//{
					//	//大宝箱，清除原有数据
					//	g_OtherData.GetMazeMap().clear();
					//}
					MazeMap.maze[iY].byType = (iX>>8);
					MazeMap.maze[iY].byMap = (iX&0xFF);
					if ((iX>>8) & 0x01)//地图
					{
						//在当前地图上
						MazeMap.byPlayerIndex = (BYTE)iY;
					}

					if ((iX>>8) & 0x04)//大宝箱地图
					{
                        g_pMagicCtrl->CreateMagic(MGGICID_MAZE_BAGUA,EMP_MAGIC_NOOWNER|EMP_MAGIC_NOTARGET,0,MAKELONG(51,37));//固定坐标，地表八挂
						Magic_Show_s* pShow = g_pMagicCtrl->FindMagicByAll(MGGICID_MAZE_SHIBEI);
						if (pShow == NULL)
						{
							g_pMagicCtrl->CreateMagic(MAGICID_MAZE_SHIBEI_DEADNODELAY,EMP_MAGIC_NOOWNER|EMP_MAGIC_NOTARGET,0,MAKELONG(51,37));//固定坐标，地表怪物死亡)
						}
					}
				}       
			}
		}
		break;
	case 99:
		{
			if(strnicmp(g_pGameMap->GetMapName(),"MAZE",4) == 0)
			{
				if( iY == 0) //迷仙阵天桥渐隐
				{
					switch (iX)
					{
					case 0:
						g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE1);
						g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE5);
						g_pMagicCtrl->CreateMagic(MAGICID_MIGONG_BGIDGE9,EMP_MAGIC_NOOWNER|EMP_MAGIC_NOTARGET,0,MAKELONG(51,18));//固定坐标 
						break;
					case 1:
						g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE2);
						g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE6);
						g_pMagicCtrl->CreateMagic(MAGICID_MIGONG_BGIDGE10,EMP_MAGIC_NOOWNER|EMP_MAGIC_NOTARGET,0,MAKELONG(65,40));//固定坐标 
						break;
					case 2:
						g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE3);
						g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE7);
						g_pMagicCtrl->CreateMagic(MAGICID_MIGONG_BGIDGE11,EMP_MAGIC_NOOWNER|EMP_MAGIC_NOTARGET,0,MAKELONG(51,54));//固定坐标 
						break;
					case 3:
						g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE4);
						g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE8);
						g_pMagicCtrl->CreateMagic(MAGICID_MIGONG_BGIDGE12,EMP_MAGIC_NOOWNER|EMP_MAGIC_NOTARGET,0,MAKELONG(35,40));//固定坐标 
						break;		
					}
				}
				else //迷仙阵天桥渐出
				{
					switch (iX)
					{
					case 0:
						g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE5);
						g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE9);
						g_pMagicCtrl->CreateMagic(MAGICID_MIGONG_BGIDGE1,EMP_MAGIC_NOOWNER|EMP_MAGIC_NOTARGET,0,MAKELONG(51,18));//固定坐标 
						break;
					case 1:
						g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE6);
						g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE10);
						g_pMagicCtrl->CreateMagic(MAGICID_MIGONG_BGIDGE2,EMP_MAGIC_NOOWNER|EMP_MAGIC_NOTARGET,0,MAKELONG(65,40));//固定坐标 
						break;
					case 2:
						g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE7);
						g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE11);
						g_pMagicCtrl->CreateMagic(MAGICID_MIGONG_BGIDGE3,EMP_MAGIC_NOOWNER|EMP_MAGIC_NOTARGET,0,MAKELONG(51,54));//固定坐标 
						break;
					case 3:
						g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE8);
						g_pMagicCtrl->FinishMagicByID(MAGICID_MIGONG_BGIDGE12);
						g_pMagicCtrl->CreateMagic(MAGICID_MIGONG_BGIDGE4,EMP_MAGIC_NOOWNER|EMP_MAGIC_NOTARGET,0,MAKELONG(35,40));//固定坐标 
						break;		
					}
				}
			}				
		}
		break;
	case 100:
		break;
	case 101:
		g_pMagicCtrl->CreateMagic(MAGICID_THUNDER_HIT,EMP_MAGIC_NOOWNER|EMP_MAGIC_NOTARGET,0,MAKELONG(iX,iY));//固定坐标，引怪雷击 
		break;
	case 102:
		g_pMagicCtrl->CreateMagic(MAGICID_THUNDER_HIT_BAOXIANG,EMP_MAGIC_NOOWNER|EMP_MAGIC_NOTARGET,0,MAKELONG(51,37));//固定坐标，大宝箱地图专用雷击
		break;
	case 104:
		if(!g_pMagicCtrl->FindMagicByAll(MAGICID_SHIELD_REFLECT,-1,id))
		{
			g_pMagicCtrl->CreateMagic(MAGICID_SHIELD_REFLECT,0,(MAKELONG(iY,iX)),id);
		}		
		break;
	case 108:
		{
			g_OtherData.SetExploreID(id);
			CSimpleCharacterNode * pChar = g_pGameData->FindSimpleCharacter(id);
			if(pChar != NULL)
			{
				SNextAction *pNext = pChar->PushSNextAction(ACTION_APPEAR);
				if (pNext)
				{
					pNext->iData = 6;		//淡出后删掉凤凰
				}

				pChar->SetReserveData(pubGradualAppear,20);
				pChar->SetReserveData(pubGradualTime,GetTickCount());
			}
		}
		break;
	case 109://冰boss冰剑特效
		{
			id = MAKELONG(iX,iY);
			pMagic = g_pMagicCtrl->CreateMagic(MAGICID_ICE_SWORD,EMP_MAGIC_SERVER_ADVISE|EMP_MAGIC_NOOWNER|EMP_MAGIC_NOTARGET,id,id,-1,dwColor);
			SMapTile *pTile = NULL;//g_pGameMap->GetTile(iX,iY);

			for (int i = 1; i <= 5; i++)
			{
				pTile = g_pGameMap->GetTile(iX + i,iY + i);
				if(!pTile || (pTile->byAttr & AT_BLOCK) != 0)
					break;

				id = MAKELONG(iX + i,iY + i);
				pMagic = g_pMagicCtrl->CreateMagic(MAGICID_ICE_SWORD2,EMP_MAGIC_SERVER_ADVISE|EMP_MAGIC_NOOWNER|EMP_MAGIC_NOTARGET,id,id,-1,dwColor);
			}
			for (int i = 1; i <= 5; i++)
			{
				pTile = g_pGameMap->GetTile(iX + i,iY - i);
				if(!pTile || (pTile->byAttr & AT_BLOCK) != 0)
					break;

				id = MAKELONG(iX + i,iY - i);
				pMagic = g_pMagicCtrl->CreateMagic(MAGICID_ICE_SWORD2,EMP_MAGIC_SERVER_ADVISE|EMP_MAGIC_NOOWNER|EMP_MAGIC_NOTARGET,id,id,-1,dwColor);
			}
			for (int i = 1; i <= 5; i++)
			{
				pTile = g_pGameMap->GetTile(iX - i,iY + i);
				if(!pTile || (pTile->byAttr & AT_BLOCK) != 0)
					break;

				id = MAKELONG(iX - i,iY + i);
				pMagic = g_pMagicCtrl->CreateMagic(MAGICID_ICE_SWORD2,EMP_MAGIC_SERVER_ADVISE|EMP_MAGIC_NOOWNER|EMP_MAGIC_NOTARGET,id,id,-1,dwColor);
			}
			for (int i = 1; i <= 5; i++)
			{
				pTile = g_pGameMap->GetTile(iX - i,iY - i);
				if(!pTile || (pTile->byAttr & AT_BLOCK) != 0)
					break;

				id = MAKELONG(iX - i,iY - i);
				pMagic = g_pMagicCtrl->CreateMagic(MAGICID_ICE_SWORD2,EMP_MAGIC_SERVER_ADVISE|EMP_MAGIC_NOOWNER|EMP_MAGIC_NOTARGET,id,id,-1,dwColor);
			}

		}
		break;
	case 121:
		if (pMagic)
		{
			pMagic->byRev1 = 1;
			pMagic->iData1 = id;
		}
		break;
	case 122:   //托管队员喝了天山雪莲
		{
// 			TneupMember *pMember = g_TrusteeshipData.FindMemberByID(id);
// 			if (pMember)
// 			{
// 				pMember->bDrinkSuperRed = (iX > 0?true:false);
// 			}
			break;
		}
	case 123:  //托管队员喝了深海灵礁
		{
// 			TneupMember *pMember = g_TrusteeshipData.FindMemberByID(id);
// 			if (pMember)
// 			{
// 				pMember->bDrinkSuperBlue = (iX > 0?true:false);
// 			}
			break;
		}
	case 125://抵抗
	case 126://反弹
	case 127://破击
		{
			if (g_AICfgMgr.IsShowHpChangeEffect())
			{
				CCharacterAttr* pChar = g_pGameData->FindCharacterByID(id);
				if (pChar)
				{
					Magic_Show_s *ms = g_pMagicCtrl->CreateMagic(MAGICID_POPUP_PAOPAO,0,id,0);
					if (ms)
					{
						ms->byRev1 = 3 + wItemType - 125;
						int iTexW,iTexH;
						int iTexOffX = 0,iTexOffY = 0;
						pChar->GetTexWH(iTexW,iTexH);
						pChar->SetTexOffXY(iTexOffX,iTexOffY);
						ms->iOffX = iTexW / 2 + iTexOffX - 40;
						ms->iOffY = iTexH + iTexOffY - 132;//怪物绘制非透明部分起始位置y - 132 + iTexOffY + iTexH = 怪物脚底y,所有 怪物绘制非透明部分起始位置y = 怪物脚底y - (iTexOffY + iTexH - 132)
					}
				}
			}

		}
		break;
	case 130://"爆"字
		{
			DWORD dwMagicType  = EMP_MAGIC_SERVER_ADVISE;
			dwMagicType |= EMP_MAGIC_NOTARGET;
			DWORD dwTargetID = MAKELONG(iX,iY);

			g_pMagicCtrl->CreateMagic(MAGICID_BAOZI_XINLINGBAOPO,dwMagicType,0,dwTargetID);
		}
		break;
	case 132://心灵爆破宠物爆炸
		{
			DWORD dwMagicType  = EMP_MAGIC_SERVER_ADVISE;
			dwMagicType |= EMP_MAGIC_NOTARGET;
			DWORD dwTargetID = MAKELONG(iX,iY);

			g_pMagicCtrl->CreateMagic(MAGICID_BAOZHA_XINLINGBAOPO,dwMagicType,0,dwTargetID);
		}
		break;
	case 133://心灵爆破敌人击中
		{
			DWORD dwMagicType  = EMP_MAGIC_SERVER_ADVISE;
			dwMagicType |= EMP_MAGIC_NOTARGET;
			DWORD dwTargetID = MAKELONG(iX,iY);

			g_pMagicCtrl->CreateMagic(MAGICID_JIZHONG_XINLINGBAOPAO,dwMagicType,0,dwTargetID);
		}
		break;
	case 134://消失特效
		{
			//圣武雕像变身特效
			CSimpleCharacterNode * pChar = g_pGameData->FindSimpleCharacter(id);
			//雕像
			switch (pChar->GetRaceNo())
			{
			case 410:
			case 411:
			case 412:
			case 413:
			case 414:
			case 415:
			case 418:
				{
					int x,y;
					pChar->GetXY(x,y);
					g_pMagicCtrl->CreateMagic(MAGICID_DIAOXIANG_XIANSHEN,EMP_MAGIC_NOOWNER|EMP_MAGIC_NOTARGET,0,MAKELONG(x,y));
				}
				break;
			}
		}
		break;
	case 135://被将军大招攻击者身上特效
		{
			//DWORD dwTargetID = MAKELONG(iX,iY);
			g_pMagicCtrl->CreateMagic(MAGICID_ATTACKED_BY_TONGTIAN,0,id);
		}
		break;
	}
	return true;
}