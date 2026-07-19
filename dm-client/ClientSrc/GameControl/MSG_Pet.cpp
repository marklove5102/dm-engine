#include "GameControl.h"
#include "GameData\MagicDefine.h"
#include "BaseClass/Control/Control.h"
#include "GameData/GameData.h"
#include "Global/Timer.h"
#include "GameData/TalkMgr.h"
#include "GameData/PetData.h"
#include "GameData/BoothData.h"
#include "GameData/WooolStoreData.h"
#include "GameAI/AIAutoEatMgr.h"
#include "GameAI/AIAutoMgr.h"
#include "GameAI/AIAutoPickMgr.h"
#include "GameAI/AutoKillMonsterMgr.h"
#include "GameClient/ReplayManager.h"

////////////////////////////////////////////////////////////////////////////////////////////
//宠物包裹
//保存到宠物包裹
void CGameControl::SEND_Pet_Object_Storage(CGood& Good)
{
	DWORD id = Good.GetID();

	SET_COMMAND(CS_PET_OBJECT_STORAGE,12);
	memcpy(temp,&id,4);
	SEND_GAME_SERVER();
}

//从宠物包裹取出东西
void CGameControl::SEND_Pet_Object_GetBack(CGood& Good)
{
	DWORD id = Good.GetID();

	SET_COMMAND(CS_PET_OBJECT_GETBACK,12);
	memcpy(temp,&id,4);
	SEND_GAME_SERVER();
}

//宠物包裹增加一个东西
void CGameControl::MSG_Pet_Object_Add(const char* msg, int iLen)
{
	//SC_PET_OBJECT_ADD
	//宠物包裹满就直接返回
	int iPos = SELF.PetGood().FindGoodPos(0);
	if(iPos < 0)
		return;

	//直接存放
	if(!SELF.GetPetGood(iPos).FromBuffer(msg + 12,false,iLen - 12))
		return;

	//1.907增加商城
	DWORD id = SELF.GetPetGood(iPos).GetID();
	SELF.GetPetGood(iPos).SetExternString(g_pGameData->GetExternString(id));
}

void CGameControl::MSG_Pet_Object_Storage_Result(const char* msg, int iLen)
{
	WORD wResult = *(WORD*)(msg + 10);

	//没有存物品
	CGood& tmp = SELF.GetPetTemp().Get(0);
	if(tmp.GetID() == 0) //没有存物品
		return;

	if(wResult > 0) //1：成功，0：失败
	{
		SELF.GetPetTemp().DeleteGood(tmp.GetID());
	}
	else //失败
	{
		SELF.PackageGood().BackToArray(tmp,tmp.GetPos());
		SELF.GetPetTemp().DeleteGood(tmp.GetID());
	}
}

void CGameControl::MSG_Pet_Object_GetBack_Result(const char* msg, int iLen)
{
	WORD wResult = *(WORD*)(msg + 10);

	//没有取物品
	CGood& tmp = SELF.GetPetTemp().Get(0);
	if(tmp.GetID() == 0)
		return;

	if(wResult > 0)  //成功
	{
		SELF.GetPetTemp().DeleteGood(tmp.GetID());
	}
	else //失败
	{
		SELF.PetGood().BackToArray(tmp,tmp.GetPos());
		SELF.GetPetTemp().DeleteGood(tmp.GetID());
	}
}

void CGameControl::MSG_Pet_Object_Info(const char* msg, int iLen)
{
	BYTE byGoodLen = BYTE(msg[7]);//单个物品的长度
	if (byGoodLen == 0)
	{
		byGoodLen = CGood::PKLength();
	}
	//清空原来的表
	SELF.PetGood().Clear();

	for(int i = 12,j = 0 ; i < iLen; i += byGoodLen,j++)
	{
		if(j >= MAX_PET_PACKAGE)
			return;

		if(!SELF.GetPetGood(j).FromBuffer(msg+i,false,byGoodLen))
			continue;

		//1.907增加商城
		DWORD id = SELF.GetPetGood(j).GetID();
		SELF.GetPetGood(j).SetExternString(g_pGameData->GetExternString(id));
	}
}

void CGameControl::MSG_Pet_Package_Capacity(const char* msg, int iLen)
{
	UINT iOldPetPackage = MAX_PET_PACKAGE;
	MAX_PET_PACKAGE = Conv_WORD(msg + 10);

	////增加豹子摆摊摊位的格子数量
	//int iBoothLimit = Conv_WORD(msg + 8);
	//g_PetBoothData.SetBoothLimit(iBoothLimit);

	//宠物包裹尺寸为0表示没有宠物包裹
	if(MAX_PET_PACKAGE == 0)   
	{
		SELF.ResizePetPackageCapacity(iOldPetPackage);

		g_pControl->Msg(MSG_CTRL_PET_PACKAGE_WND,OPER_CLOSE);
	}
	else if(iOldPetPackage != MAX_PET_PACKAGE) //包裹大小变化
	{
		SELF.ResizePetPackageCapacity(iOldPetPackage);
		g_pControl->Msg(MSG_CTRL_PET_PACKAGE_WND,OPER_CLOSE);
	}

	if(g_AIAutoMgr.IsEnalbeWaiGua() && MAX_PET_PACKAGE > 0 && iOldPetPackage == 0)
	{
		g_AutoPickMgr.PetPackageCapacity(MAX_PET_PACKAGE);
	}
	//大小没有变化时不动包裹数据
}

void CGameControl::SEND_Pet_Object_Pickup(int iX,int iY)
{
	if(SELF.GetMyPetID() == 0)
		return;

	SET_COMMAND(CS_PET_OBJECT_PICKUP,12);
	ASSIGN_WORD(6,iX);
	ASSIGN_WORD(8,iY);

	SEND_GAME_SERVER();
}

void CGameControl::MSG_Pet_Object_Pickup_Fail(const char* msg,int iLen)
{
	DWORD id = *(DWORD*)(msg);
	WORD  wErr = *(WORD*)(msg + 10);

	string str;

	switch(wErr)
	{
	case 2:
		str = "该物品已被冻结，你不能捡取！";
		break;
	case 3:
		str = "物品不属于你，一定时间范围内不能捡";
		break;
	case 4:
		str = "金币数量达到上限";
		break;
	case 5:
		str = "包裹格子或负重已满";
		break;
	default:
		break;
	}
	if(g_AIAutoMgr.IsEnalbeWaiGua())
	{
		g_AutoPickMgr.PetItemPickupFail(id,wErr);
	}

	if(str.size() != 0)
	{
		g_TalkMgr.PushSysTalk(str.c_str());
	}
}

void CGameControl::SEND_Request_Server_Time()
{
	static DWORD dwLastRequestTime = 0;

	//防止向服务器请求过于频繁
	if(GetTickCount() < dwLastRequestTime + 3000)
		return;
	dwLastRequestTime = GetTickCount();

	SET_COMMAND(CS_REQUEST_SERVER_TIME,12);
	SEND_GAME_SERVER();
}

void CGameControl::MSG_Answer_Server_Time(const char* msg,int iLen)
{
	DWORD dwTime = *(DWORD*)(msg);
	if (dwTime != 0)
	{
		//g_dwServerTime是服务器时间同客户端时间之差
#ifdef ENABLE_REPLAY
		if(g_ReplayManager.IsInReplay())//为了使时间和录像当时的一致,这里通过改变g_dwServerTime来实现
		{
			g_dwServerTime = dwTime + g_ReplayManager.GetStartTime() - 2 * g_Timer.GetCurTime(); //dwTime - g_Timer.GetCurTime() + (g_ReplayManager.GetStartTime() - g_Timer.GetCurTime()); 			
		}
		else
#endif
		{
			g_dwServerTime = dwTime - g_Timer.GetCurTime(); 
		}
	}
}

void CGameControl::MSG_Pet_State_Change(const char* msg,int iLen)
{
	if (iLen < 16) return;
// 	TneupMember *pSelf = g_TrusteeshipData.GetSelf();
// 
// 	if (!pSelf)
// 	{
// 		return;
// 	}
// 
// 	if (*(WORD*)(msg + 10) == 1)
// 	{
// 		pSelf->aPet[ePetSkull] = 0;		
// 		pSelf->aPet[ePetCorpus] = 0;
// 		pSelf->aPet[ePetSupernalBeast] = 0;
// 
// 		pSelf->aPet[ePetSuperSkull]--;
// 		if (pSelf->aPet[ePetSuperSkull] < 0)
// 			pSelf->aPet[ePetSuperSkull] = 0;
// 	}
 	if (*(WORD*)(msg + 10) == 1)
 	{
 		g_AutoKillMonsterMgr.OnPetStateChange();
 	}

	//if(iLen < 16)  return;

	//DWORD wAction = *(WORD*)(msg + 10);
	//DWORD dwPetID = *(DWORD*)(msg + 12);

	//if(wAction == 0)
	//{
	//	std::list<DWORD>& listPet = SELF.GetPetList();
	//	std::list<DWORD>::iterator it;
	//	for(it = listPet.begin(); it != listPet.end(); it++)
	//	{
	//		if(*it == dwPetID)
	//			return;
	//	}
	//	SELF.GetPetList().push_back(dwPetID);
	//}
	//else if(wAction == 1)
	//{
	//	std::list<DWORD>& listPet = SELF.GetPetList();
	//	std::list<DWORD>::iterator it;
	//	for(it = listPet.begin(); it != listPet.end(); it++)
	//	{
	//		if(*it == dwPetID)
	//		{
	//			listPet.erase(it);
	//			break;
	//		}
	//	}
	//}
}

//协议类型:
//1=请求可以领养的豹子数据, 协议头前4字节:起始序号(2字节)终止序号(2字节)
//2=请求自己寄养的豹子数据, 协议头前4字节未用
//3=请求领养, 协议头前4字节魔石流水号
//4=请求取回寄养的豹子, 协议头前4字节魔石流水号
//5=请求释放豹子经验, 要释放的经验数
//6=请求寄养自己放出来的豹子

void CGameControl::SEND_Pet_Adopt_Request(BYTE cRequestType,DWORD dwData,DWORD dwID)
{
	SET_COMMAND(CS_PET_ADOPT_REQUEST,12);
	ASSIGN_ID(dwData);//未用或者豹魔石流水号或者其他(4字节)
	ASSIGN_BYTE(6,cRequestType);//协议类型(1字节)
	ASSIGN_DWORD(7,dwID);
	SEND_GAME_SERVER();
}

//发送宠物信息请求，例如打开窗口时会请求释放出来宠物的血量请求数据
void CGameControl::SEND_Pet_Info_Request(BYTE cRequestType,DWORD dwData,DWORD dwID)
{
	SET_COMMAND(CS_PET_INFO_REQ,12);
	ASSIGN_ID(dwData);//未用或者豹魔石流水号或者其他(4字节)
	ASSIGN_BYTE(6,cRequestType);//协议类型(1字节)
	ASSIGN_DWORD(7,dwID);
	SEND_GAME_SERVER();
}

//协议体服务器端数据结构：
//typedef struct tagFosteragePetPacket
//{
//	DWORD  dwPetItemID;//豹魔石流水号
//	char   szPetName[15];//豹子名,（兽名魔咒命名的）
//	BYTE   byPetType;//豹子类型(0丛林豹，1踏云豹)
//	BYTE   byPetLevel;//豹子等级
//	BYTE   byStatus;//状态,1寄养中未领养，2领养中
//	time_t tLastEatTime;//最后喂食时间
//	char szJYPlayerName[15];//寄养者角色名
//	char szLYPlayerName[15];//领养者角色名
//	char szReserved[8];
//} FOSTERAGEPETPACKET;

void CGameControl::MSG_Pet_Adopt_Info(const char* strMsg2,int iLen)
{
	WORD wTotalNum = *(WORD*)(strMsg2);//服务器寄养豹子总数(2字节)
	WORD wGetNum = *(WORD*)(strMsg2+2);//本次发送的豹子信息总数(2字节)
	WORD wStartNum = *(WORD*)(strMsg2+6);//本次发送的信息起始序号(2字节)
	BYTE byType = *(BYTE*)(strMsg2+8);//本次协议类型(1字节,1可以领养的豹子数据,2自己寄养的豹子数据)

	int iPerPetLen = sizeof(DWORD) + sizeof(char)*53 + sizeof(BYTE)*3 + sizeof(__time32_t);
	int iTimeLen = sizeof(__time32_t);
	__time32_t t_time;
	_time32(&t_time);
	t_time += g_dwServerTime;
	PET_ADOPT petAdopt;


	if(byType == 1)
	{
		g_PetData.SetAdoptPetNum(wTotalNum);
		PET_ADOPT_MAP &PetAdopMap = g_PetData.GetPetAdoptMap();
		PetAdopMap.clear();
		int iSelfPetCount = 0;

		for(int i = 0; i < wGetNum && i*iPerPetLen <= iLen ; i++)
		{
			memset(&petAdopt,0,sizeof(petAdopt));

			petAdopt.dwPetItemID = *(DWORD*)(strMsg2+12+i*iPerPetLen);
			memcpy(petAdopt.szPetName,strMsg2+12+i*iPerPetLen+4,15);
			petAdopt.byPetType = *(BYTE*)(strMsg2+12+i*iPerPetLen+19);
			petAdopt.byPetLevel = *(BYTE*)(strMsg2+12+i*iPerPetLen+20);
			petAdopt.byStatus = *(BYTE*)(strMsg2+12+i*iPerPetLen+21);
			petAdopt.tLastEatTime = DWORD(*(__time32_t*)(strMsg2+12+i*iPerPetLen+22));
			memcpy(petAdopt.szJYPlayerName,strMsg2+12+i*iPerPetLen+22+iTimeLen,15);
			memcpy(petAdopt.szLYPlayerName,strMsg2+12+i*iPerPetLen+37+iTimeLen,15);
			memcpy(petAdopt.szReserved,strMsg2+12+i*iPerPetLen+52+iTimeLen,8);
			//petAdopt.iFeedDays =  (t_time-petAdopt.tLastEatTime )/ (24*3600);
			if(strcmp(petAdopt.szJYPlayerName,SELF.GetName()) == 0)//是自己寄养的排除在列表外
			{
				g_PetData.SetAdoptPetNum(g_PetData.GetAdoptPetNum() - 1);
				iSelfPetCount ++;
				continue;
			}

			PetAdopMap[wStartNum + i - iSelfPetCount] = petAdopt;
		} 

	}
	else if(byType == 2)
	{
		g_PetData.SetCarrayBackPetNum(wTotalNum);
		PET_ADOPT_MAP &PetAdopMap = g_PetData.GetPetCarryBackMap();
		PetAdopMap.clear();

		for(int i = 0; i < wGetNum && i*iPerPetLen <= iLen ; i++)
		{
			memset(&petAdopt,0,sizeof(petAdopt));

			petAdopt.dwPetItemID = *(DWORD*)(strMsg2+12+i*iPerPetLen);
			memcpy(petAdopt.szPetName,strMsg2+12+i*iPerPetLen+4,15);
			petAdopt.byPetType = *(BYTE*)(strMsg2+12+i*iPerPetLen+19);
			petAdopt.byPetLevel = *(BYTE*)(strMsg2+12+i*iPerPetLen+20);
			petAdopt.byStatus = *(BYTE*)(strMsg2+12+i*iPerPetLen+21);
			petAdopt.tLastEatTime = DWORD(*(__time32_t*)(strMsg2+12+i*iPerPetLen+22));
			memcpy(petAdopt.szJYPlayerName,strMsg2+12+i*iPerPetLen+22+iTimeLen,15);
			memcpy(petAdopt.szLYPlayerName,strMsg2+12+i*iPerPetLen+37+iTimeLen,15);
			memcpy(petAdopt.szReserved,strMsg2+12+i*iPerPetLen+52+iTimeLen,8);

			PetAdopMap[wStartNum + i] = petAdopt;
		} 
	}

}


//如果类型为1:表示有如下协议体(加密):
////豹子头像信息(单字节对齐):
//typedef struct tagPetInfoPacket
//{
//	DWORD  dwPetItemID;//豹魔石流水号
//	char   szPetName[15];//豹子名,（兽名魔咒命名的）
//	BYTE   byPetType;//豹子类型(0丛林豹，1踏云豹)
//	BYTE   byPetLevel;//豹子等级
//	WORD   wPetExp;//豹子经验
//	BYTE   byPetColor;//豹子颜色,0普通，1白，2黑
//	__time32_t tLastEatTime;//豹子最后喂食时间
//	BYTE   byFullTime;//豹子饱食最长时间(天)
//	DWORD  dwMaxSorbExp;//豹子吸收的经验上限
//	DWORD  dwSorbExp;//豹子吸收的经验
//	BYTE   byHPPercent;//血条百分比	
//DWORD  dwTodayReleasedExp;//玩家今天吸收了多少经验
//DWORD  dwMaxCheapExpPerDay;//低价经验，每天最多1000万
//DWORD  dwCheapExpPerFullDay;//每天饱食度，可以吸取的低价经验28万
//DWORD  dwExpensiveExpPerFullDay;//每天饱食度，可以吸取的高价经验20万 
//	char   szReserved[8];
//} PETINFOPACKET;
void CGameControl::MSG_SendOutPet_Info(const char* strMsg2,int iLen)
{

	PET_SENDOUT SendOutPet;

	SendOutPet.dwSorbExp = *(DWORD*)(strMsg2);//豹子吸收经验(4字节)
	SendOutPet.byHPPercent = *(BYTE*)(strMsg2+6);//豹子血条百分比(1字节)

	DWORD dwID = *(DWORD*)(strMsg2+12);
	BYTE byType = *(BYTE*)(strMsg2+7);//类型(1字节)
	BYTE byOnOrOff = *(BYTE*)(strMsg2+8);//开关(1字节,0关,1开)
	map<DWORD,PET_SENDOUT>::iterator it;
	int istatus = strMsg2[66];

	it = g_PetData.GetSendOutPetList().find(dwID);
	if(byType == 1)
	{
		int iTimeLen = sizeof(__time32_t);
		__time32_t t_time;
		_time32(&t_time);
		t_time += g_dwServerTime;		
		if(it != g_PetData.GetSendOutPetList().end())
		{
			if(istatus != 3 && istatus != 9)//摆摊
			{
				it->second.dwPetItemID = *(DWORD*)(strMsg2+12);
				memset(it->second.szPetName,0,15);
				memcpy(it->second.szPetName,strMsg2+16,15);
				it->second.byPetType = *(BYTE*)(strMsg2+31);
				it->second.byPetLevel = *(BYTE*)(strMsg2+32);
				it->second.wPetExp = *(WORD*)(strMsg2+33);
				it->second.byPetColor = *(BYTE*)(strMsg2+35);
				it->second.tLastEatTime = *(__time32_t*)(strMsg2+36);
				it->second.byFullTime = *(BYTE*)(strMsg2+36+iTimeLen);
				it->second.dwMaxSorbExp = *(DWORD*)(strMsg2+37+iTimeLen);
				it->second.dwSorbExp = *(DWORD*)(strMsg2+41+iTimeLen);
				it->second.byHPPercent = *(BYTE*)(strMsg2+45+iTimeLen);
				it->second.dwTodayReleasedExp = *(DWORD*)(strMsg2+46+iTimeLen);
				it->second.dwMaxCheapExpPerDay = *(DWORD*)(strMsg2+50+iTimeLen);
				it->second.dwCheapExpPerFullDay = *(DWORD*)(strMsg2+54+iTimeLen);
				it->second.dwExpensiveExpPerFullDay = *(DWORD*)(strMsg2+58+iTimeLen);
			}
			if(istatus != 100)	
				memcpy(it->second.szReserved,strMsg2+62+iTimeLen,8);

		}
		else
		{
			SendOutPet.dwPetItemID = *(DWORD*)(strMsg2+12);
			memset(SendOutPet.szPetName,0,15);
			memcpy(SendOutPet.szPetName,strMsg2+16,15);
			SendOutPet.byPetType = *(BYTE*)(strMsg2+31);
			SendOutPet.byPetLevel = *(BYTE*)(strMsg2+32);
			SendOutPet.wPetExp = *(WORD*)(strMsg2+33);
			SendOutPet.byPetColor = *(BYTE*)(strMsg2+35);
			SendOutPet.tLastEatTime = *(__time32_t*)(strMsg2+36);
			SendOutPet.byFullTime = *(BYTE*)(strMsg2+36+iTimeLen);
			SendOutPet.dwMaxSorbExp = *(DWORD*)(strMsg2+37+iTimeLen);
			SendOutPet.dwSorbExp = *(DWORD*)(strMsg2+41+iTimeLen);
			SendOutPet.byHPPercent = *(BYTE*)(strMsg2+45+iTimeLen);
			SendOutPet.dwTodayReleasedExp = *(DWORD*)(strMsg2+46+iTimeLen);
			SendOutPet.dwMaxCheapExpPerDay = *(DWORD*)(strMsg2+50+iTimeLen);
			SendOutPet.dwCheapExpPerFullDay = *(DWORD*)(strMsg2+54+iTimeLen);
			SendOutPet.dwExpensiveExpPerFullDay = *(DWORD*)(strMsg2+58+iTimeLen);
			memcpy(SendOutPet.szReserved,strMsg2+62+iTimeLen,8);
			//SendOutPet.szReserved[0] = 1; //释放出来跟随状态

			//这里要判断是否死亡
			__time32_t t_time;
			_time32(&t_time);
			t_time += g_dwServerTime;

			int iDay = (int)((t_time - SendOutPet.tLastEatTime)/(24 * 3600));
			if(iDay < 4)
			{
				typedef pair<DWORD,PET_SENDOUT> Map_Pair;
				g_PetData.GetSendOutPetList().insert(Map_Pair(SendOutPet.dwPetItemID,SendOutPet));
				g_pControl->MsgToWnd("PetWnd",MSG_CTRL_PET_WND,1);
				DWORD dwGoodID = 0;
				//先寻找极品
				for(int i = 0; i < MAX_PACKAGE_ELEMENT; i++)
				{
					CGood& tmp = SELF.GetPackageGood(i);
					if(tmp.GetID() == 0)
						continue;

					if(stricmp("极品兽粮",tmp.GetName()) == 0 || stricmp("极品豹粮",tmp.GetName()) == 0)
					{
						dwGoodID = tmp.GetID();
						
						break;
					}
				}

				//再寻找精品
				if (dwGoodID == 0)
				{
					for(int i = 0; i < MAX_PACKAGE_ELEMENT; i++)
					{
						CGood& tmp = SELF.GetPackageGood(i);
						if(tmp.GetID() == 0)
							continue;

						if(stricmp("精制兽粮",tmp.GetName()) == 0 || stricmp("精制豹粮",tmp.GetName()) == 0)
						{
							dwGoodID = tmp.GetID();
							
							break;
						}
					}
				}

				//寻找一般兽粮
				if (dwGoodID == 0)
				{
					for(int i = 0; i < MAX_PACKAGE_ELEMENT; i++)
					{
						CGood& tmp = SELF.GetPackageGood(i);
						if(tmp.GetID() == 0)
							continue;

						if(stricmp("兽粮",tmp.GetName()) == 0 || stricmp("豹粮",tmp.GetName()) == 0)
						{
							dwGoodID = tmp.GetID();
							
							break;
						}
					}
				}

				//寻找豹粮（捆）
				if (dwGoodID == 0)
				{
					for(int i = 0; i < MAX_PACKAGE_ELEMENT; i++)
					{
						CGood& tmp = SELF.GetPackageGood(i);

						if(tmp.GetID() == 0)
							continue;

						if((stricmp("兽粮(捆)",tmp.GetName()) == 0 || stricmp("豹粮(捆)",tmp.GetName()) == 0) && tmp.GetDura() > 0) 
						{
							dwGoodID = tmp.GetID();
							
							break;
						}
					}
				}

				//if(dwGoodID == 0)
				//{
				//	//快捷消费,自动喂豹粮
				//	if(iDay >= 2 && SendOutPet.szReserved[0] == 1)
				//	{
				//		g_WooolStoreMgr.SetQuickBuyItem("兽粮");
				//		CQuickBuyData &QuickBuyData = g_WooolStoreMgr.GetQuickBuyData();

				//		if(QuickBuyData.pItem)
				//		{
				//			char cTemp[128]={0};
				//			sprintf(cTemp,"你要购买兽粮并喂食你的灵兽吗？单价%d元宝",QuickBuyData.pItem->iPrice);
				//			QuickBuyData.iUseAfterBuyType = 6;
				//			QuickBuyData.strMsg = cTemp;
				//			QuickBuyData.iType = 1;
				//			g_pControl->PopupWindow(MSG_CTRL_QUICKBUY_WND,OPER_CREATE);						
				//		}
				//	}
				//}
				////else
				////{
				////	g_AutoEatMgr.SetastAutoFeedTime(0);
				////}
			}
		}
	}
	else if(byType == 0)//收回
	{
		if(it != g_PetData.GetSendOutPetList().end())
		{
			g_PetData.GetSendOutPetList().erase(it);
		}
	}else if(byType == 2)//马
	{
		g_PetData.GetMyHorse().wAppr = *(WORD*)(strMsg2);
		g_PetData.GetMyHorse().bStatus = byOnOrOff;
		g_PetData.GetMyHorse().byHPPercent = *(BYTE*)(strMsg2+6);
	}else if(byType == 3)//其它宠物
	{
		//修改血量
		if(it != g_PetData.GetSendOutPetList().end())
		{
			it->second.byHPPercent = *(BYTE*)(strMsg2+6);
		}
	}

	if(byOnOrOff == 0)
	{
		SendOutPet.dwPetItemID = 0;
	}

}

//协议头:未用(4字节)+0x88A2(2字节)+领养豹子数(1字节)+未用(5字节)
//协议体(加密):领养豹子1指针(4字节)+领养豹子1寄养者角色名(15字节)+
//领养豹子2指针(4字节)+领养豹子2寄养者角色名(15字节)+
//...
void CGameControl::MSG_MyAdoptPet_Info(const char* strMsg2,int iLen)
{
	BYTE byCount = *(BYTE*)(strMsg2+6);
	map<DWORD,PET_ADOPT> &MyAdopPetMap = g_PetData.GetMyAdoptMap();

	PET_ADOPT petAdopt;

	MyAdopPetMap.clear();

	for(int i = 0; i < byCount; i++)
	{
		memset(&petAdopt,0,sizeof(PET_ADOPT));

		petAdopt.dwPetItemID = *(DWORD*)(strMsg2+12+i*19);
		memcpy(petAdopt.szJYPlayerName,strMsg2+12+i*19+4,15);

		//通过搜索包裹里的豹魔石来获得名字，最后喂食时间等信息
		for(int i = 0; i < MAX_PACKAGE_ELEMENT; i++)
		{
			CGood& tmp = SELF.GetPackageGood(i);

			if(tmp.GetID() == petAdopt.dwPetItemID)
			{

				petAdopt.byPetLevel = tmp.GetDC();
				petAdopt.byPetType = tmp.GetSC();

				//自定义名字
				if(tmp.GetExternString() !="")
					sprintf(petAdopt.szPetName,"%s",tmp.GetExternString().c_str());
				else
				{
					std::string strPetName = g_PetData.GetPetName(petAdopt.byPetType);
					sprintf(petAdopt.szPetName,"%s",strPetName.c_str());				
				}


				//喂食时间
				petAdopt.tLastEatTime = tmp.GetRecordTime();

				break;
			}
		}

		MyAdopPetMap[petAdopt.dwPetItemID] = petAdopt;  

	}
}

void CGameControl::MSG_PetGoodInfo(const char* strMsg2,int iLen)
{
	DWORD dwPlayerID = Conv_DWORD(strMsg2);

	if(dwPlayerID == SELF.GetID() || strMsg2[8] == 1)	//玩家自己的灵兽石或者摆摊的灵兽石
	{
		WORD wPetItemLen = *( (WORD*)(strMsg2 + 6));
		WORD wNum = *(WORD*)(strMsg2+10);

		DWORD dwItemID = 0;
		int iPosition = 12;
		CGoodPetInfo stGoodPetInfo;

		map<DWORD,CGoodPetInfo>& mapGoodPetInfo = g_PetData.GetGoodPetInfoMap();
		for(int i = 0; i < wNum && iPosition < iLen; i++)
		{
			stGoodPetInfo.clear();
			stGoodPetInfo.SetValue(strMsg2+iPosition, wPetItemLen);
			dwItemID = *((DWORD*)(strMsg2+iPosition ));
			iPosition += wPetItemLen;

			mapGoodPetInfo.erase(dwItemID);
			mapGoodPetInfo.insert(map<DWORD,CGoodPetInfo>::value_type(dwItemID, stGoodPetInfo));
		}
	}
	else
	{
		CGoodPetInfo stGoodPetInfo;
		map<DWORD,CGoodPetInfo>& mapGoodPetInfo = g_PetData.GetGoodPetInfoMap();

		stGoodPetInfo.clear();
		stGoodPetInfo.SetValue(strMsg2 + 12, Conv_WORD(strMsg2 + 6));		

		mapGoodPetInfo.erase(dwPlayerID);
		mapGoodPetInfo.insert(map<DWORD,CGoodPetInfo>::value_type(dwPlayerID, stGoodPetInfo));
	}
}

//从宠物界面给指定宠物喂食
void CGameControl::SEND_Feed_Pet_From_Interface(DWORD dwPetID,DWORD dwFoodID)
{
	SET_COMMAND(SC_PET_FEED_FROM_INTERFACE,12);
	ASSIGN_ID(dwFoodID);
	ASSIGN_DWORD(6,dwPetID);
	SEND_GAME_SERVER();
}

void CGameControl::MSG_PET_ADOPT_END(const char * msg,int iLen)
{
	DWORD dwPetID = *(DWORD*)(msg);
    g_PetData.GetMyAdoptMap().erase(dwPetID);
}

void CGameControl::Send_Pet_Explore(DWORD id,byte type)		//type = 0 探宝, type = 1 召回, type = 2 领奖
{
	SET_COMMAND(CS_PET_EXPLORE,12);
	ASSIGN_ID(id);
	ASSIGN_BYTE(6,type);
	SEND_GAME_SERVER();
}