#include "AIAutoMgr.h"
#include "GameData/GameData.h"
#include "GameData/OtherData.h"
#include "GameData/TalkMgr.h"
#include "Global/Interface/FontInterface.h"
#include "GameControl/GameControl.h"
#include "BaseClass/Misc/Input.h"
#include "BaseClass/Control/Control.h"
#include "GameMap/GameMap.h"
#include "WndClass/GameWnd/GameManager.h"
#include "GameData/GameGlobal.h"
#include "GameClient/SDOAInterface.h"
#include "GameData/LoginData.h"
#include "GameData/ConfigData.h"

#include "AIConfigMgr.h"
#include "AIAutoFightMgr.h"
#include "AIAutoPickMgr.h"
#include "AIAutoEatMgr.h"
#include "AIPickCfgMgr.h"
#include "AIPromptMgr.h"
#include "AIBossCfgMgr.h"
#include "AIMgr.h"
#include "AutoKillMonsterMgr.h"

CAIAutoMgr g_AIAutoMgr;

CAIAutoMgr::CAIAutoMgr(void)
{
	m_bEnableWaigua = false;
	m_dwLastSayTime = 0;
	m_iDrawInfoX = 0;m_iDrawInfoY = 0;
}

CAIAutoMgr::~CAIAutoMgr(void)
{
}


void CAIAutoMgr::LaunchWaigua()
{
	if(m_bEnableWaigua)
		return;

	if(strlen(SELF.GetName()) == 0)
		return;

	m_bEnableWaigua = true;

	//读取配置文件
	g_AICfgMgr.LoadConfig();  
	g_AutoEatMgr.LoadConfig();
	g_PickCfgMgr.LoadConfig();
	g_BossCfgMgr.LoadBossConfig();
	g_AutoKillMonsterMgr.Clear();
	g_AutoPickMgr.Clear();
	g_AIPromptMgr.Clear();
	g_AIPromptMgr.ReadBossDeathFile();
	g_AIPromptMgr.ReadPKFile();


	//自动关闭组队
	if(g_pGameData->IsAllowTroop() && g_AICfgMgr.IsCloseTeam())
		g_pGameControl->SEND_Group_Open_Team(false);
}

void CAIAutoMgr::DoLoop()
{
	////自己被队长微操，并且这个客户端不是队长的,什么也不做;如果是对长的,要处理被操控队员,自己及离线托管对队的AI,这里的self是指全局的,如果微操后则指向被微操的人,否则就是原来的自己
	//if (SELF.IsMicroControled() && !g_TrusteeshipData.IsCaptain())
	//{
	//	return;
	//}

// 	CCharacter *pOldSelf = g_pSelf;

	g_pSelf = &ORIGINALSELF;
	
	//没有附身,处理自己
// 	if (g_TrusteeshipData.GetCaptionSubstitutePos() < 0)
	{
		g_AutoEatMgr.AutoPlace(); //自动解包
		if(m_bEnableWaigua && GetTickCount() - g_OtherData.GetLastMovePosTime() > 1000)//跨GS后立即使用魔法等服务器会不响应,因此就一直waitserver,导致10秒内不能动
		{
			g_AutoPickMgr.DoLoop();
			g_AutoFightMgr.DoLoop();
			g_AIPromptMgr.DoLoop();	
			AutoMoGong(); //自动点魔宫
			AutoSay(); //自动说话
			g_AutoEatMgr.AddHP();
			g_AutoEatMgr.AddMP();
			//g_AutoEatMgr.AutoFeed();
		}
	}

// 	//离线托管玩家,微操对象,附身且没有微操其它对象时,都在队长的客户端执行相关AI, 非离线托管模式下,附身且微操时,附身对象的AI在原来的队员的客户端执行
// 	if (g_TrusteeshipData.IsTrusteeship() && g_TrusteeshipData.IsCaptain())
// 	{
// 		for (int i = 0; i < MAX_TRUSTEESHIP_NUM; i++)
// 		{
// 			TneupMember& member = g_TrusteeshipData.GetTneupMember(i);
// 			if (member.dwID != 0 && member.byPos == i && member.pCharacter &&
// 				(member.byOffLineMode || member.byPos == g_TrusteeshipData.GetMicroControlPos() || (member.byPos == g_TrusteeshipData.GetCaptionSubstitutePos() && g_TrusteeshipData.GetMicroControlPos() < 0))
// 				)
// 			{
// 				g_pSelf = member.pCharacter;
// 				g_AutoEatMgr.AutoPlace(); //自动解包
// 				g_AutoFightMgr.DoLoop();
// 				g_AutoPickMgr.DoLoop();
// 				g_AIPromptMgr.DoLoop();
// 				g_AutoEatMgr.AddHP();
// 				g_AutoEatMgr.AddMP();
// 			}
// 		}
// 	}
// 
// 	g_pSelf = pOldSelf;
}

void CAIAutoMgr::ExitWaigua()
{
	m_bEnableWaigua = false;
}

void CAIAutoMgr::ExtraDraw()
{
	DrawSysInfo();
	DrawLockInfo();
	DrawMagicTime();

	g_AIPromptMgr.DrawPrompt();
	g_AIPromptMgr.ExtraDraw();
}

void CAIAutoMgr::DrawSysInfo()
{
	char str_buf[512] = {0};

	if(g_AICfgMgr.IsShowSysInfo())
	{
		//综合信息
		//char mouse_buf[128] ={0};
		char time_buf[128] = {0};
		_strtime(time_buf);
		UINT64  _iExp = SELF.GetExp();
		UINT64  _iLevelUpExp = SELF.GetLevelUpExp();
		int  _iPackageWeight = SELF.GetPackageWeight();
		int  _iPackageWeightMax = SELF.GetPackageWeightMax();
		int  _iWeight = SELF.GetWeight();
		int  _iWeightMax = SELF.GetWeightMax();
		int  _iMoney = SELF.GetGold();

		sprintf(str_buf,"经验:%I64d/%I64d 包裹:%d/%d 负重:%d/%d 金币:%d",_iExp,_iLevelUpExp,
			_iPackageWeight,_iPackageWeightMax,_iWeight,_iWeightMax,_iMoney);
		g_pFont->DrawText(m_iDrawInfoX,m_iDrawInfoY,str_buf,0xFF00FF00,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);

		int dwMouseX,dwMouseY;
		g_pGameMgr->GetMouseTile(dwMouseX,dwMouseY);

		sprintf(str_buf,"本地时间:%s 鼠标位置:%d %d",time_buf,dwMouseX,dwMouseY);
		g_pFont->DrawText(m_iDrawInfoX,m_iDrawInfoY + 14,str_buf,0xFF00FF00,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);
	}	

	//持久显示
	if(g_pInput->IsEnable() && (GetKeyState(VK_INSERT) & 0x0100) != 0)
	{
		int _iEquipmentX = 25;
		int _iEquipmentY = 100;
		int iSize = SELF.EquipGood().Size();

		for(int _iEquip=0;_iEquip < iSize;_iEquip++)
		{
			CGood temp = SELF.GetEquipGood(_iEquip);
			if(temp.GetID() == 0)
				continue;

			//持久
			string str;
			str = temp.GetName();
			for(int i = str.size();i<14;i++)
				str.append(" ");
			sprintf(str_buf,"%6d/%6d",temp.GetDura(),temp.GetDuraMax());
			str.append(str_buf);

			g_pFont->DrawText(_iEquipmentX,_iEquipmentY,str.c_str(),0xFF00FF00,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);

			_iEquipmentY += FONTSIZE_DEFAULT + 2;
		}
	}

   // if(SELF.IsOnLepoard())
   // {
   //     if(SELF.GetFightOnLeopard())
   //     {
			//g_pFont->DrawText(g_pGfx->GetWidth() - 119,g_pGfx->GetHeight() - 210,"骑兽:[骑战模式]",0xFFFFFF00,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);
   //     }
   //     else
   //     {
   //         g_pFont->DrawText(g_pGfx->GetWidth() - 119,g_pGfx->GetHeight() - 210,"骑兽:[骑乘模式]",0xFFFFFF00,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);
   //     }
   // }

}

void CAIAutoMgr::DrawLockInfo()
{
	char str_buf[512] = {0};

	//攻击目标
	CSimpleCharacter* pLockChar = NULL;
	pLockChar = g_pGameData->FindSimpleCharacter(SELF.GetReserveData(plyMagicLockID));
	if(pLockChar == NULL)
		pLockChar = g_pGameData->FindSimpleCharacter(SELF.GetReserveData(plyAttackLockID));

	if(pLockChar && !pLockChar->IsDead() && !pLockChar->IsNpc())
	{
		string strName = pLockChar->GetName();
		sprintf(str_buf,"攻击目标:%s %.2f%%",strName.c_str(),100 * CalRate(pLockChar->GetHP(),pLockChar->GetHPMax()));

		int x = (g_pGfx->GetWidth() > 800)?34:12;
		int y = (g_pGfx->GetHeight() > 600)?350:250;
		g_pFont->DrawText(x,y,str_buf,0xFF00FF00,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);

		int iLockX,iLockY;
		pLockChar->GetXY(iLockX,iLockY);
		int iSelfX,iSelfY;
		SELF.GetXY(iSelfX,iSelfY);

		sprintf(str_buf,"目标位置: %d %d %s",iLockX,iLockY,GetToDir(iSelfX,iSelfY,iLockX,iLockY));
		g_pFont->DrawText(x,y+20,str_buf,0xFFFF0000,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);
	}
}

void CAIAutoMgr::DrawMagicTime()
{
	char str_buf[512] = {0};

	if(g_AICfgMgr.IsShowMagicTime())
	{
		DWORD dwCount = GetTickCount();
		DWORD dwStart = g_AIMgr.GetReserveTime(plyStartArmorTime);
		DWORD dwDura  = g_AIMgr.GetReserveTime(plyDuraArmorTime);

		if(dwStart > 0 && dwStart + dwDura * 1000 > dwCount)
		{
			sprintf(str_buf,"防御力： %d 秒",dwDura - (dwCount - dwStart) / 1000);
			g_pFont->DrawText(10,(g_pGfx->GetHeight() > 600)?250:200,str_buf,0xFF00FF00,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);
		}

		dwStart = g_AIMgr.GetReserveTime(plyStartGhostTime);
		dwDura  = g_AIMgr.GetReserveTime(plyDuraGhostTime);

		if(dwStart > 0 && dwStart + dwDura * 1000 > dwCount)
		{
			sprintf(str_buf,"抗魔法力： %d 秒",dwDura - (dwCount - dwStart) / 1000);
			g_pFont->DrawText(10,(g_pGfx->GetHeight() > 600)?270:220,str_buf,0xFF00FF00,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);
		}


		DWORD dwStartTm = SELF.GetReserveData(plyZhenYuanStart);
		if(dwStartTm > 0 && GetTickCount() - dwStartTm < SELF.GetReserveData(plyZhenYuanConTm))
		{
			sprintf(str_buf,"真元攻击持续时间： %d 秒", (SELF.GetReserveData(plyZhenYuanConTm) + dwStartTm - GetTickCount()) / 1000 );
			g_pFont->DrawText(0,(g_pGfx->GetHeight() > 600)?230:180,str_buf,0xFF00FF00,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);
		}
	}
	//图腾效果
	//隐身图腾效果\反射图腾效果\生命图腾效果\魔力图腾效果\巫毒图腾效果\巫术图腾效果,以上几个用绿字表示		重力图腾效果\诅咒图腾效果,以上几个用红字表示
	DWORD dwState = SELF.GetTuTengState();
	int iLine = 0;
	if(dwState & ETTS_REFLECT)
	{
		g_pFont->DrawText(10,(g_pGfx->GetHeight() > 600)?290:240 +iLine*16,"反射效果",0xFF00FF00,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);
		iLine ++;
	}
	if(dwState & ETTS_LIFE)
	{
		g_pFont->DrawText(10,(g_pGfx->GetHeight() > 600)?290:240 +iLine*16,"生命效果",0xFF00FF00,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);
		iLine ++;
	}
	if(dwState & ETTS_GRAVITY)
	{
		g_pFont->DrawText(10,(g_pGfx->GetHeight() > 600)?290:240 +iLine*16,"重力效果",0xFF00FF00,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);
		iLine ++;
	}
	if(dwState & ETTS_CURSE)
	{
		g_pFont->DrawText(10,(g_pGfx->GetHeight() > 600)?290:240 +iLine*16,"诅咒效果",0xFF00FF00,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);
		iLine ++;
	}
	if(dwState & ETTS_ZOMBIE)
	{
		g_pFont->DrawText(10,(g_pGfx->GetHeight() > 600)?290:240 +iLine*16,"巫毒效果",0xFF00FF00,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);
		iLine ++;
	}
	if(dwState & ETTS_SORCERY)
	{
		g_pFont->DrawText(10,(g_pGfx->GetHeight() > 600)?290:240 +iLine*16,"巫术效果",0xFF00FF00,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);
		iLine ++;
	}
	if(dwState & ETTS_MAGIC)
	{
		g_pFont->DrawText(10,(g_pGfx->GetHeight() > 600)?290:240 +iLine*16,"魔力效果",0xFF00FF00,FONT_DEFAULT,FONTSIZE_DEFAULT,DTF_BlackFrame);
		iLine ++;
	}

}

void CAIAutoMgr::AutoSay()
{
	if(!g_AICfgMgr.IsAutoSay())
		return;

	string str = g_AICfgMgr.GetAutoSay();

	if(g_AICfgMgr.GetAutoSayTimer() == 0 || str.empty())
		return;

	DWORD dwCount = GetTickCount();

	if((dwCount-m_dwLastSayTime) > g_AICfgMgr.GetAutoSayTimer() * 1000)
	{
		if(str[0] == '/')
			g_pGameControl->SEND_Message_Private(str.c_str(),str.size());
		else
			g_pGameControl->SEND_Message_Send(str.c_str(),str.size());

		m_dwLastSayTime = dwCount;
	}
}

BOOL CAIAutoMgr::DoAccelKey(WORD wKey)
{
	if(!m_bEnableWaigua)
		return FALSE;

	char temp[1024] = {0};

	if(g_pInput->IsAlt()) //ALT
	{
		switch (wKey)
		{
		case 'A':
			{
				switch(SELF.GetCareer())
				{
				case JOB_ZHANSHI:
					{
						bool b = !g_AICfgMgr.IsAutoWildCollide();
						g_AICfgMgr.SetAutoWildCollide(b);

						if(b)
							sprintf(temp,"[智能突斩开启]");
						else
							sprintf(temp,"[智能突斩关闭]");

						g_TalkMgr.PushSysTalk(temp,TALKCOLOR_GREEN);
						break;
					}
				case JOB_FASHI:
					{
						bool b = !g_AICfgMgr.IsAutoDispute();
						g_AICfgMgr.SetAutoDispute(b);

						if(b)
							sprintf(temp,"[自动抗拒开启]");
						else
							sprintf(temp,"[自动抗拒关闭]");

						g_TalkMgr.PushSysTalk(temp,TALKCOLOR_GREEN);
						break;
					}
			case JOB_DAO:
					{
						bool b = !g_AICfgMgr.IsAutoLionCall();
						g_AICfgMgr.SetAutoLionCall(b);

						if(b)
							sprintf(temp,"[自动狮子吼开启]");
						else
							sprintf(temp,"[自动狮子吼关闭]");

						g_TalkMgr.PushSysTalk(temp,TALKCOLOR_GREEN);
						break;
					}
				}
				return TRUE;
			}
		case 'M':
			{
				switch(SELF.GetCareer())
				{
				case JOB_ZHANSHI:
					{
						if(!g_AICfgMgr.IsAutoProtectSkin()) //没有开护身，先开护身
						{
							g_AICfgMgr.SetAutoProtectSkin(true);
							sprintf(temp,"[持续护身真气开启]");
						}
						else if(!g_AICfgMgr.IsAutoSteelProtect()) //然后开金刚
						{
							g_AICfgMgr.SetAutoSteelProtect(true);
							sprintf(temp,"[持续金刚护体开启]");
						}
						else //全部关闭
						{
							g_AICfgMgr.SetAutoProtectSkin(false);
							g_AICfgMgr.SetAutoSteelProtect(false);
							sprintf(temp,"[持续魔法保护关闭]");
						}
						g_TalkMgr.PushSysTalk(temp,TALKCOLOR_GREEN);
						break;
					}
				default: //其他情况没有处理
					break;
				}
				return TRUE;
			}
		case 'C':
			{
				CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(g_pControl->GetMouseOnID());
				if(pChar && pChar->IsHuman())
				{
					if( g_pGameData->GetTroopMembers() <= 0 ) //未组队，创建一个新的队伍
					{
						g_pGameControl->SEND_Group_Create(pChar->GetName()); 
					}
					else if(strcmp(SELF.GetName(),g_pGameData->GetMemberName(0)) == 0) //组队了，并且是队长
					{
						g_pGameControl->SEND_Group_Add(pChar->GetName());
					}
				}
				return TRUE;
			}
		case 'D':
			{
				bool b = !g_AICfgMgr.IsShowBestItem();
				g_AICfgMgr.SetShowBestItem(b);

				if(b)
					sprintf(temp,"[只显示极品名称]开启");
				else
					sprintf(temp,"[只显示极品名称]关闭");
				g_TalkMgr.PushSysTalk(temp,TALKCOLOR_GREEN);
				return TRUE;
			}
		//显示玩家名称开关
		case 'B':
			{
				bool b = !g_AICfgMgr.IsShowPlayerName();
				g_AICfgMgr.SetShowPlayerName(b);

				if(b)
					sprintf(temp,"[显示玩家名字]开启");
				else
					sprintf(temp,"[显示玩家名字]关闭");

				g_TalkMgr.PushSysTalk(temp,TALKCOLOR_GREEN);

				return TRUE;
			}
		case 'Y':
			{
				CSimpleCharacter* pChar = g_pGameData->FindSimpleCharacter(g_pControl->GetMouseOnID());

				if(pChar && pChar->IsHuman())
				{
					g_BossCfgMgr.AddBlackName((char*)pChar->GetName());
				}
				return TRUE;
			}
		case 'S':
			{
				bool b = !g_AICfgMgr.IsRunAttack();
				g_AICfgMgr.SetRunAttack(b);

				if(b)
					sprintf(temp,"[追杀模式]开启");
				else
					sprintf(temp,"[追杀模式]关闭");
				g_TalkMgr.PushSysTalk(temp,TALKCOLOR_GREEN);
				return TRUE;
			}
		case 191: //按Alt+?
			{
				if(g_pGameData->GetTroopMembers() > 0)
				{
					int iSelfX = 0,iSelfY = 0;
					SELF.GetXY(iSelfX,iSelfY);

					string str;
					str.assign("!!");
					sprintf(temp,"我在%s(%d,%d)处PK,快来帮忙！",g_pGameMap->GetMapTitle(),iSelfX,iSelfY);
					str.append(temp);

					g_pGameControl->SEND_Message_Send(str.c_str(),str.size());
				}
				return TRUE;
			}
		case 'F':
			{
				bool b = !g_AICfgMgr.IsAllowFly();
				g_AICfgMgr.SetAllowFly(b);

				if(b)
					sprintf(temp,"[允许瞬移抢物]开启");
				else
					sprintf(temp,"[允许瞬移抢物]关闭");

				g_TalkMgr.PushSysTalk(temp,TALKCOLOR_GREEN);
				return TRUE;
			}
		case 'E':
			{
				switch(SELF.GetCareer())
				{
				case JOB_ZHANSHI:
					{
						bool b = !g_AICfgMgr.IsDestroyShield();
						g_AICfgMgr.SetDestroyShield(b);

						if(b)
							sprintf(temp,"[持续破盾斩开启]");
						else
							sprintf(temp,"[持续破盾斩关闭]");

						g_TalkMgr.PushSysTalk(temp,TALKCOLOR_GREEN);
						break;
					}
				case JOB_FASHI:
					{
						bool b = !g_AICfgMgr.IsAutoMagicProtect();
						g_AICfgMgr.SetAutoMagicProtect(b);

						if(b)
							sprintf(temp,"[持续魔法盾开启]");
						else
							sprintf(temp,"[持续魔法盾关闭]");

						g_TalkMgr.PushSysTalk(temp,TALKCOLOR_GREEN);
						break;
					}
				case JOB_DAO:
					{
						bool b = !g_AICfgMgr.IsAutoProtectGhost();
						g_AICfgMgr.SetAutoProtectGhost(b);

						if(b)
							sprintf(temp,"[持续幽灵盾开启]");
						else
							sprintf(temp,"[持续幽灵盾关闭]");
						g_TalkMgr.PushSysTalk(temp,TALKCOLOR_GREEN);
						break;
					}
				}
				return TRUE;
			}
		case 'R':
			{
				switch(SELF.GetCareer())
				{
				case JOB_ZHANSHI:
					{
						bool b = !g_AICfgMgr.IsDestroyShell();
						g_AICfgMgr.SetDestroyShell(b);

						if(b)
							sprintf(temp,"[持续破击剑法开启]");
						else
							sprintf(temp,"[持续破击剑法关闭]");

						g_TalkMgr.PushSysTalk(temp,TALKCOLOR_GREEN);
						break;
					}
				case JOB_FASHI:
					{
						bool b = !g_AICfgMgr.IsAutoDodgeSkill();
						g_AICfgMgr.SetAutoDodgeSkill(b);

						if(b)
							sprintf(temp,"[持续风影盾开启]");
						else
							sprintf(temp,"[持续风影盾关闭]");

						g_TalkMgr.PushSysTalk(temp,TALKCOLOR_GREEN);
						break;
					}
				case JOB_DAO:
					{
						bool b = !g_AICfgMgr.IsAutoProtectArmor();
						g_AICfgMgr.SetAutoProtectArmor(b);

						if(b)
							sprintf(temp,"[持续神圣战甲术开启]");
						else
							sprintf(temp,"[持续神圣战甲术关闭]");

						g_TalkMgr.PushSysTalk(temp,TALKCOLOR_GREEN);
						break;
					}
				}
				return TRUE;
			}
		case 'G':
			{
				bool b = !g_AICfgMgr.IsAutoFindWay();
				g_AICfgMgr.SetAutoFindWay(b);
				if(b)
					sprintf(temp,"[智能寻路]开启");
				else
					sprintf(temp,"[智能寻路]关闭");
				g_TalkMgr.PushSysTalk(temp,TALKCOLOR_GREEN);
				return TRUE;
			}
		//case 'L':
		//	{
		//		bool b = !g_AICfgMgr.IsNoDrawChar();
		//		g_AICfgMgr.SetNoDrawChar(b);

		//		if(b)
		//			sprintf(temp,"[隐藏其他玩家]开启");
		//		else
		//			sprintf(temp,"[隐藏其他玩家]关闭");
		//		g_TalkMgr.PushSysTalk(temp,TALKCOLOR_GREEN);

		//		return TRUE;
		//	}
		case '1':
		    {
				if(ATTACK_TYPE_ALL != SELF.GetReserveData(plyAttackType))
				{
					char strTemp[56]={0};
					sprintf(strTemp,"@AttackMode %d",ATTACK_TYPE_ALL);
					g_pGameControl->SEND_Guild_Ally(strTemp);
				}
				return TRUE;
		    }
		case '2':
			{
				if(ATTACK_TYPE_PEACE != SELF.GetReserveData(plyAttackType))
				{
					char strTemp[56]={0};
					sprintf(strTemp,"@AttackMode %d",ATTACK_TYPE_PEACE);
					g_pGameControl->SEND_Guild_Ally(strTemp);
				}
				return TRUE;
			}
		case '3':
			{
				if(ATTACK_TYPE_GROUP != SELF.GetReserveData(plyAttackType))
				{
					char strTemp[56]={0};
					sprintf(strTemp,"@AttackMode %d",ATTACK_TYPE_GROUP);
					g_pGameControl->SEND_Guild_Ally(strTemp);
				}
				return TRUE;
			}
		case '4':
			{
				if(ATTACK_TYPE_GUILD != SELF.GetReserveData(plyAttackType))
				{
					char strTemp[56]={0};
					sprintf(strTemp,"@AttackMode %d",ATTACK_TYPE_GUILD);
					g_pGameControl->SEND_Guild_Ally(strTemp);
				}
				return TRUE;
			}
		case '5':
			{
				if(ATTACK_TYPE_ST != SELF.GetReserveData(plyAttackType))
				{
					char strTemp[56]={0};
					sprintf(strTemp,"@AttackMode %d",ATTACK_TYPE_ST);
					g_pGameControl->SEND_Guild_Ally(strTemp);
				}
				return TRUE;
			}
		case '6':
			{
				if(ATTACK_TYPE_WH != SELF.GetReserveData(plyAttackType))
				{
					char strTemp[56]={0};
					sprintf(strTemp,"@AttackMode %d",ATTACK_TYPE_WH);
					g_pGameControl->SEND_Guild_Ally(strTemp);
				}
				return TRUE;
			}
		case '7':
			{
				if(ATTACK_TYPE_SE != SELF.GetReserveData(plyAttackType))
				{
					char strTemp[56]={0};
					sprintf(strTemp,"@AttackMode %d",ATTACK_TYPE_SE);
					g_pGameControl->SEND_Guild_Ally(strTemp);
				}
				return TRUE;
			}
		}
	}
	else if(g_pInput->IsCtrl())
	{
		switch(wKey)
		{
		case 'B':
			{
				bool b = !g_AICfgMgr.IsRunNotStop();
				g_AICfgMgr.SetRunNotStop(b);
				if(b)
					sprintf(temp,"[跑不停开启]");
				else
					sprintf(temp,"[跑不停关闭]");
				g_TalkMgr.PushSysTalk(temp,TALKCOLOR_GREEN);
				return TRUE;
			}
		}
	}
	else
	{
		CControl* pCtrl = g_pControl->GetFocusCtrl();
		switch(wKey)
		{
		case VK_HOME: //Boss列表翻页
		case VK_END://Boss列表翻页
			{
				if(!g_pInput->IsEnable())
					break;

				if(g_pInput->IsKeyDown(VK_DELETE) && (pCtrl == NULL || !pCtrl->IsNeedKeyInput()))
				{
					if(wKey == VK_HOME)
						g_AIPromptMgr.PriorBossPage();
					else
						g_AIPromptMgr.NextBossPage();
					return TRUE;
				}
				break;
			}
		case VK_TAB:
			{
				if(!g_pInput->IsShift() && (pCtrl == NULL || !pCtrl->IsNeedKeyInput()))
				{
					bool b = !g_AICfgMgr.IsAutoMagicAttack();
					g_AICfgMgr.SetAutoMagicAttack(b);
					g_AIMgr.SetLastPressMagic(0);
					g_AIMgr.SetLastPressConSkill(0);

					if(b)
						sprintf(temp,"[连续魔法攻击]开启");
					else
						sprintf(temp,"[连续魔法攻击]关闭");

					g_TalkMgr.PushSysTalk(temp,TALKCOLOR_GREEN);
					return TRUE;
				}
			}
		}
	}
	return FALSE;
}

void CAIAutoMgr::DealWithHurted(int hp,int hpmax)
{
	class StoneGoodIterator : public GoodIterator
	{
	public:
		StoneGoodIterator(bool b){ bForever = b; }
		virtual bool DoIterator(CGood& tmp)
		{
			if(bForever)
			{
				if(tmp.GetStdMode() == 3 && tmp.GetShape() == 15 && tmp.GetDC() != 0)
					return true;
			}
			else
			{
				if(tmp.GetStdMode() == 3 && tmp.GetShape() == 17 && tmp.GetDC() != 0)
					return true;
			}
			return false;
		}
	private:
		bool bForever;
	};

	if(g_AICfgMgr.GetAutoAction() != 0)
	{
		if(g_AICfgMgr.GetAutoActionLimit() >= 0 && 
			g_AICfgMgr.GetAutoActionLimit() < hpmax && 
			hp < g_AICfgMgr.GetAutoActionLimit())
		{
			int npos = -1;
			switch(g_AICfgMgr.GetAutoAction())
			{
			case 1:
				npos = SELF.PackageGood().FindGoodByName("随机卷轴");
				break;
			case 2:
				npos = SELF.PackageGood().FindGoodByName("地牢卷轴");
				break;
			case 3:
				npos = SELF.PackageGood().FindGoodByName("回城卷轴");
				break;
			case 4://回城石
				{
					StoneGoodIterator itr(true);
					npos = SELF.PackageGood().FindGoodByIterator(itr);
				}
				break;
			//case 5:
			//	npos = SELF.PackageGood().FindGoodByName("随机神石");
			//	break;
			case 5: //回城神石
				{
					StoneGoodIterator itr(false);
					npos = SELF.PackageGood().FindGoodByIterator(itr);
				}
				break;
			}
			if(npos >= 0)
				g_pGameControl->SEND_Use_Object(npos);
		}
	}

	if(g_AICfgMgr.IsAutoQuit())
	{
		//符合了自动小退的条件
		int iQuitLimit = g_AICfgMgr.GetAutoQuitLimit();

		if(iQuitLimit > 0 && iQuitLimit < hpmax && hp < iQuitLimit)
		{
			g_pControl->Msg(MSG_CTRL_QUIT_WND,OPER_CUSTOM+1);
		}
	}
}

void CAIAutoMgr::AutoMoGong()
{
	if(g_AICfgMgr.IsAutoMogong()) //自动点魔宫
	{
		if(strcmp(g_pGameMap->GetMapName(),"MC014") == 0)
		{
			int selfX,selfY;
			SELF.GetRealXY(selfX,selfY);
			if(abs(selfX - 76) < 3 && abs(selfY - 72) < 3)
			{
				CSimpleCharacterNode * p = MapArray.FindSimpleCharacter(76,72);
				while (p != NULL)
				{
					if(p->IsNpc())
					{
						g_pGameControl->SEND_Exchange_Goin(p->GetID());
						break;
					}
					p = p->m_MapNext;
				}
			}
		}
	}
}
