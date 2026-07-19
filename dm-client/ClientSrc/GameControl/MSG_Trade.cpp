#include "GameControl.h"
#include "GameData/GameData.h"
#include "GameData/LoginData.h"
#include "BaseClass/Control/Control.h"
#include "GameData/MagicDefine.h"
#include "GameData/MsgBoxMgr.h"
#include "GameAI/AIMgr.h"
#include "GameData/WooolStoreData.h"
#include "GameData/TalkMgr.h"
#include "GameData/OtherData.h"
#include "GameData/MacroDefine.h"
#include "GameData/TradeData.h"
#include "GameData/BoothData.h"
#include "GameClient/SDOAInterface.h"
#include "GameAI/AIConfigMgr.h"
#include <string>

void CGameControl::MSG_Trade_Tackin_Failed(const char * msg,int iLen)
{ 
	if(g_TradeData.GetTempGood().GetID()!=0)
	{
		SELF.PackageGood().BackToArray(g_TradeData.GetTempGood());
		g_TradeData.GetTempGood().SetID(0);
	}
}

// 收到的消息处理
void CGameControl::MSG_Trade_Failed(const char *msg,int iLen)
{
	g_TradeData.BackToPackage();  //物品还到包裹里面

	g_pControl->PopupWindow(MSG_CTRL_TRADEWND,OPER_CLOSE);
}

void CGameControl::MSG_Trade_Success(const char *msg,int iLen)
{
	// 交易成功, 清除交易栏中的金币和元宝
	g_TradeData.SetSelfGolds(0);
	g_TradeData.SetAnotherGolds(0);
	g_TradeData.SetSelfYuanBao(0);
	g_TradeData.SetAnotherYuanBao(0);

	for(int i = 0; i < MAX_TRADE_OBJECTS; i ++)
	{
		g_TradeData.GetSelfGood(i).SetID(0);
		g_TradeData.GetAnotherGood(i).SetID(0);
	}
	g_pControl->PopupWindow(MSG_CTRL_TRADEWND,OPER_CLOSE);
}

void CGameControl::MSG_Trade_Money_Info(const char * msg,int iLen)
{
	DWORD selfMoney = 0;
	DWORD packageMoney = 0;
	selfMoney = *((DWORD *)msg);
	packageMoney = *((DWORD *)(msg + 6));

	if(msg[10] == 1)		// 从包裹中拿金币或者元宝到交易栏中
	{
		g_TradeData.SetSelfYuanBao(selfMoney);
		SELF.SetYuanBao(packageMoney);
	}
	else
	{
		g_TradeData.SetSelfGolds(selfMoney);
		SELF.SetGold(packageMoney);
	}

}

void CGameControl::MSG_Trade_Player_Money_Info(const char * msg,int iLen)
{
	DWORD Money = 0;
	Money = *((DWORD *)msg);
	if(msg[10] == 1)		// 另一个玩家给包裹中放置元宝或者金币
		g_TradeData.SetAnotherYuanBao(Money);
	else
		g_TradeData.SetAnotherGolds(Money);	
}

void CGameControl::MSG_Trade_Disable(const char * msg,int iLen)
{
	g_TalkMgr.PushSysTalk("交易必须和对方面对面！");
}

void CGameControl::MSG_Trade_Enable(const char * msg,int iLen)
{
	char temp[256];
	memcpy(temp,msg + 12,iLen - 12);
	temp[iLen-12] = 0;
	g_TradeData.SetAnotherName(temp);
	g_TradeData.SetTradeFlag(false);
	g_pControl->PopupWindow(MSG_CTRL_TRADEWND,OPER_RECREATE);
}

void CGameControl::MSG_Trade_Player_Object_Info(const char *msg,int iLen)
{
	for(int j = 0; j < MAX_TRADE_OBJECTS; j++)
	{
		if(g_TradeData.GetAnotherGood(j).GetID() == 0)
		{
			g_TradeData.GetAnotherGood(j).FromBuffer(msg + 12,false,iLen - 12);
			g_TradeData.GetAnotherGood(j).AddItemState(InTradeGrid);
			return;
		}
	}
}

void CGameControl::MSG_Trade_Object_Confirm(const char *msg,int iLen)
{
	for(int i = 0 ; i < MAX_TRADE_OBJECTS; i++)
	{
		if(g_TradeData.GetSelfGood(i).GetID() == 0)
		{
			g_TradeData.GetSelfGood(i) = g_TradeData.GetTempGood();

			//从摊位上清除已准备交易的物品
			DWORD id = g_TradeData.GetTempGood().GetID();
			g_BoothData.CleanItem(id);

			////1.907增加商城
			g_pGameData->DelExternString(id);
			g_TradeData.GetTempGood().SetID(0);

			return;
		}
	}
}


// 要发送的消息处理
void CGameControl::SEND_Trade_End()
{
	SET_COMMAND(CS_TRADE_END,12);

	SEND_GAME_SERVER();

	g_TradeData.BackToPackage();
}

void CGameControl::SEND_Trade_Goin()
{
	SET_COMMAND(CS_TRADE_GOIN,12);

	SEND_GAME_SERVER();
}

bool CGameControl::SEND_Trade_Object_Tackin(CGood & good)
{
	static DWORD dwLastTradeTime = GetTickCount();
	if (g_TradeData.GetTempGood().GetID() != 0 && GetTickCount() - dwLastTradeTime < 10000)
	{
		g_TalkMgr.PushSysTalk("正在等待服务器响应,请稍后再试.");
		return false;
	}

	dwLastTradeTime = GetTickCount();
	g_TradeData.GetTempGood() = good;

	SET_COMMAND(CS_TRADE_OBJECT_TAKCEIN,256);
	ASSIGN_ID(good.GetID());
	ASSIGN_BYTE(10,0x01);
	ADD_STR(good.GetName());
	SEND_GAME_SERVER();

	return true;
}

void CGameControl::SEND_Trade_Object_Tackout(CGood & good)
{
	SET_COMMAND(CS_TRADE_OBJECT_TAKCEOUT,256);
	ASSIGN_ID(good.GetID());
	ADD_STR(good.GetName());
	SEND_GAME_SERVER();
}

void CGameControl::SEND_Trade_Money_Tackin(DWORD money,bool bYuanBao)
{
	// 自己向包裹中放入了元宝或者金币
	SET_COMMAND(CS_TRADE_MONEY_TAKCEIN,12);

	if(bYuanBao)
	{
		temp[6] = 1;
	}
	ASSIGN_DWORD(0,money);
	SEND_GAME_SERVER();
}

void CGameControl::SEND_Trade_Ok()
{
	SET_COMMAND(CS_TRADE_OK,12);
	SEND_GAME_SERVER();
}

void CGameControl::MSG_Trade_Player_Object_Takeout(const char *msg,int iLen)
{
	for(int i = 0 ; i < MAX_TRADE_OBJECTS; i ++)
	{
		if(g_TradeData.GetAnotherGood(i).GetID() == *((DWORD*)(msg + 56)))
			g_TradeData.GetAnotherGood(i).SetID(0);
	}
}

void CGameControl::MSG_Credit_Req(const char * msg,int iLen)
{
	DWORD dwPayCount = Conv_DWORD(msg);//元宝总数
	DWORD dwID = Conv_DWORD(msg + 6);//购买物品编号
	BYTE byNum = (BYTE)(msg + 10);//购买数量

	if(dwPayCount <= 0 || byNum <= 0 || dwPayCount/byNum <= 0)
		return;

	CQuickItem qitem;
	qitem.iPrice = dwPayCount/byNum;
	char strTemp[12]={0};
	sprintf(strTemp,"%d",dwID);
	qitem.strItemID = strTemp;
	qitem.iType = 0;

	g_WooolStoreMgr.BuyQuickItem(qitem,false,byNum,false,false);
}

void CGameControl::MSG_Credit_Token(const char * msg,int iLen)
{
	BYTE byType = BYTE(msg[0]);


	//调用igw用信用购买
	if(byType == 1)//元宝不足
	{
		g_MsgBoxMgr.PopTagAlert(STRING_NOT_ENOUGH_YUANBAO);
	}
	else if(g_pSDOAInterface)
	{
		std::string strParam;
		strParam.assign(msg + 12,iLen - 12);
		strParam += "&gatetype=2";
		size_t iPos = strParam.find_last_of("?");
		if(iPos != std::string::npos)
		{
			strParam = strParam.substr(iPos + 1,strParam.length() - iPos - 1 );
		}

		if(SDOA_FALSE == g_pSDOAInterface->OpenWidgetEx("sdoCredit",strParam.c_str(),0))
		{
			g_MsgBoxMgr.PopTagAlert(STRING_NOT_ENOUGH_YUANBAO);
		}
	}
	else
	{
		g_MsgBoxMgr.PopTagAlert(STRING_NOT_ENOUGH_YUANBAO);
	}

}

// 接收到对应的数据
void CGameControl::MSG_PT_Store_Info(const char * msg,int iLen)
{
	g_WooolStoreMgr.WSMFacade(msg, iLen);
}

// 发送相关数据
void CGameControl::SEND_PT_Store_Info(int iProtocol, const char * szAppendStr, int iAppendLen,BOOL bSilent,BOOL bInputPetPkg)
{

	if(iProtocol==6)//查询赠品信息
	{
		static DWORD dwStoreLastRequestTime = 0;

		//防止向服务器请求过于频繁
		if(GetTickCount() < dwStoreLastRequestTime + 60000)
			return;

		dwStoreLastRequestTime = GetTickCount();
	}
	else if(iProtocol==5)//购买商品
	{

		//if(SELF.GetYuanBao() == 0)
		//{
		//	g_MsgBoxMgr.PopTagAlert(STRING_NOT_ENOUGH_YUANBAO);
		//	return;
		//}

		static DWORD dwStoreLastRequestTime2 = 0;

		//防止向服务器请求过于频繁
		if(GetTickCount() < dwStoreLastRequestTime2 + 5000)
		{
			g_TalkMgr.PushSysTalk("在传奇世界商城您每隔5秒才能使用元宝一次");

			return;
		}
		dwStoreLastRequestTime2 = GetTickCount();

	}

	SET_COMMAND(CS_PT_STORE_INFO,256);
	ASSIGN_DWORD(6,iProtocol);

	if (szAppendStr != NULL && iAppendLen > 0)
	{
		ADD_ARRAY(szAppendStr, iAppendLen);
	}

	if(bSilent)
	{
		ASSIGN_BYTE(7,1);
	}
	if(bInputPetPkg) 
		ASSIGN_BYTE(10,1);
	SEND_GAME_SERVER();
}

//0x0310, x[0]=0 系统发给玩家A请求失败的原因 x[1]　表示各种失败情况　x[1]＝0为成功　不发协议 　前四字节为玩家指针 
//0x0310,x[0]=1　玩家Ｂ发送回复玩家A请求交易的对话框 前四字节为玩家指针 x[1]0成功　其他 失败 文字要跟么
void CGameControl::MSG_Player_Trade(const char * strMsg2,int iLen)
{
	DWORD dwID = *((DWORD*)strMsg2);
	BYTE byType = *((BYTE*)(strMsg2 + 6));
	BYTE byResult = *((BYTE*)(strMsg2 + 7));
	if(byType == 1)//请求交易
	{
		if (!g_AICfgMgr.IsAcceptFrientAndTrade())
		{
			g_pGameControl->SEND_Player_Trade(1,dwID,1);
			return;
		}

		CSimpleCharacterNode * p = g_pGameData->FindSimpleCharacter(dwID);
		if(p)
		{	
			S_CommonConfirm CommonConfirm;

			CommonConfirm.iType = 3;
			CommonConfirm.dwLastTime = 30000;
			CommonConfirm.dwRequestPersonID = dwID;

			DWORD dwRelation = p->GetRelationType();
			if( (dwRelation & RT_WIFE) != 0)
				CommonConfirm.strMsg = "<color=red ＜妻＞>";
			else if( (dwRelation & RT_HUSBAND) != 0)
				CommonConfirm.strMsg = "<color=red ＜夫＞>";
			else if( (dwRelation & RT_PRENTICE) != 0)
				CommonConfirm.strMsg = "<color=red ＜徒＞>";
			else if( (dwRelation & RT_MASTER) != 0)
				CommonConfirm.strMsg = "<color=red ＜师＞>";
			else if( (dwRelation & RT_FRIEND) != 0)
				CommonConfirm.strMsg = "<color=red ＜友＞>";
			else 
				CommonConfirm.strMsg = "<color=red ＜生＞>";

			CommonConfirm.strMsg = CommonConfirm.strMsg + p->GetName() + "申请和你进行交易。";
			g_OtherData.GetCommonConfirmVector().push_back(CommonConfirm);

			g_pControl->PopupWindow(MSG_CTRL_COMMON_CONFIRM_WND,OPER_CREATE);
		}
	}
	else if(byType == 0)//回复请求交易
	{
		if(byResult != 0)//请求交易失败
		{
			BYTE byMsgLen = *((BYTE*)(strMsg2 + 8));
			char strTemp[128]={0};
			memcpy(strTemp,strMsg2+12,byMsgLen);

			g_TalkMgr.PushSysTalk(strTemp);
		}
	}

}

//0x0312, x[0]=1 
//玩家A向玩家Ｂ发送交易请求 前四字节玩家B指针+ 协议号（2字节）+　类型0 (1字节）
//回复玩家Ａ请求，前四字节Ａ指针+ 协议号（2字节）+　类型1 (1字节）+ 1字节表示是否同意，０表示同意，其它表示不同意
void CGameControl::SEND_Player_Trade(BYTE byType,DWORD dwID,BYTE byData)
{
	if (g_Login.GetLoginInExpType() == 1 && byData == 0)
	{
		g_pControl->PopupWindow(MSG_CTRL_BINDPTWND_WND,OPER_RECREATE);
		return;
	}

	if(byType == 0 && GetTickCount() - g_AIMgr.GetReserveTime(plyLastRequireTradeTime) < 5000)
	{
		g_TalkMgr.PushSysTalk("每5秒才能请求一次交易");
		return;
	}

	//如果不是和平攻击，先把自己的攻击模式改成和平攻击
	//if(byType == 0 && strcmp(g_pAssTools->GetFightMode(),"[和平攻击]") != 0)
	//{
	//	char strTemp[56]={0};
	//	sprintf(strTemp,"@AttackMode %d",POP_MENU_ITEM_ATTACK_TYPE_PEACE - POP_MENU_ITEM_ATTACK_TYPE_SE);
	//	g_pGameControl->SEND_Guild_Ally(strTemp);
	//}

	SET_COMMAND(SC_PLAYER_TRADE,12);
	ASSIGN_DWORD(0,dwID);
	ASSIGN_BYTE(6,byType);

	if(byType == 1)
		ASSIGN_BYTE(7,byData);

	int i = SEND_GAME_SERVER();

	if(i > 0 && byType == 0)
	{
		g_TalkMgr.PushSysTalk("已经发出交易请求，请耐心等待回应");
		g_AIMgr.SetReserveTime(plyLastRequireTradeTime,GetTickCount());	   
	}
}