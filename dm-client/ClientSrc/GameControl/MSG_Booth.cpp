#include "GameControl.h"
#include "BaseClass\Misc\Net.h"
#include "GameData\GameData.h"
#include "Global\GlobalMsg.h"
#include "GameData/MsgBoxMgr.h"
#include "BaseClass/Control/Control.h"
#include "Global/StringUtil.h"
#include "Global/Timer.h"
#include "GameData/TalkMgr.h"
#include "GameData/DirtyWords.h"
#include "GameData/PetData.h"
#include "GameData/BoothData.h"
#include "GameData/WooolStoreData.h"
#include "Global/Interface/StreamInterface.h"
#include "GameClient/WidgetManager.h"
#include "GameData/OtherData.h"
#include "BaseClass/Control/GoodGrid.h"
#include "GameData/NpcData.h"

//0x80D7
//玩家摆摊请求的应答
void CGameControl::MSG_Booth_Request_Result(const char * msg,int iLen)
{
	BYTE iResult = msg[14];
	if (iResult == 0) //请求摆摊成功
	{
		int iX = Conv_WORD(msg + 6);
		int iY = Conv_WORD(msg +8);

		bool bBoothState = SELF.GetBoothState();
		SELF.SetBoothState(true);
		SELF.SetXY(iX,iY);
		SELF.SetRealXY(iX,iY);
		SELF.SetDir(msg[10]);

		BoothInfo_t& inf = SELF.GetBoothInfo();
		inf.iBoothType = Conv_WORD(msg+12);
		inf.iFlagType = msg[15];
		inf.dwFlagColor = Conv_DWORD(msg+16);
		if (inf.dwFlagColor == 0)
		{
			inf.dwFlagColor = 0xFFFFFFFF;
		}

		if(CGoodGrid::GetDropGoodFrom().DropGood.GetID() != 0 && CGoodGrid::GetDropGoodFrom().eFromWnd == Booth_Wnd)
		{
			//清空鼠标上的摆摊物品
			CGoodGrid::ClearGoodFrom();
			g_BoothData.GetDropPrice().clear();
			g_BoothData.FullFill();
			g_BoothData.ClearReadyGood();			
		}

		if(!bBoothState)
		{
			//开始摆摊
			g_TalkMgr.PushSysTalk("开始摆摊");
			g_pControl->Msg(MSG_CTRL_PACKAGEWND,OPER_CLOSE);
		}
		else
		{   //已经摆摊时更新成功
			if (g_BoothData.GetAddingState())
			{
				g_BoothData.AddGood();
				g_BoothData.SetAddingState(false);
			}
			if (g_BoothData.GetDeletingState())
			{
				g_BoothData.SetDeletingState(false);
			}
		}
	}
	else if (iResult == 0xFF)//取消摆摊成功
	{
		SELF.SetBoothState(false);
		g_TalkMgr.PushSysTalk("摆摊终止");
		g_pControl->PopupWindow(MSG_CTRL_BOOTH_WND,OPER_CLOSE);
	}
	else//失败
	{
		string str;

		switch(iResult)
		{
		case 1:
			str = "到20级才能摆摊。";
			break;
		case 2:
			str = "骑乘或骑战状态不能摆摊。";
			break;
		case 3:
			str = "没有摆摊物品不能摆摊。";
			break;
		case 4:
			str = "摊位中有无法出售的物品，不能摆摊。";
			break;
		case 5:
			str = "你周围没有位置摆摊。";
			break;
		case 6:
			str = "你离其他人的摊位太近了，不能摆摊。";
			break;
		case 7:
			str = "摊位格子已满，请整理拍卖行中正在寄售的物品和已经下架的物品，\n稍候再试";
			break;
		case 8:
			str = "此处禁止摆摊，请到指定摆摊区域摆摊";
			break;
		default:
			str = "摆摊不成功！";
			break;
		}
		g_MsgBoxMgr.PopSimpleAlert(str.c_str());
	}
}
//显示其他玩家的摊位
void CGameControl::MSG_Booth_Show_Other_Stall(const char * msg,int iLen)
{
//#ifdef __SUPPORT_BOOTH
//SC_BOOTH_SHOW_OTHER_STALL
	if (iLen < 70)
		return;

	DWORD nOtherPlayerID = Conv_DWORD(msg);
	WORD  iBoothType = Conv_WORD(msg+12);
	BYTE ucShow = msg[14];
	//ucShow 0，-1：仅更新摊位状态信息
	//1：更新详细信息，并弹出窗口显示
	//2：更新物品信息和摊位状态信息
	BYTE iBoothFlagType = msg[15];
	DWORD    dwBoothFlagColor = Conv_DWORD(msg+16);
	WORD     dwSize = Conv_WORD(msg+20);

	BYTE byGoodLen = BYTE(msg[11]);//单个物品的长度
	if (byGoodLen == 0)
	{
		byGoodLen = CGood::PKLength();
	}


	char szBoothName[128] = {0};//摊位名
	memcpy(szBoothName,msg+22,MAX_BOOTH_NAME_LEN);

	//更新其他人的摊位状态
	CSimpleCharacterNode* pChar = g_pGameData->FindSimpleCharacter(nOtherPlayerID);
	if (pChar)
	{
		int iX = *(WORD*)(msg + 6);
		int iY = *(WORD*)(msg +8);
		BYTE iDir = BYTE(msg[10]);

		pChar->SetXY(iX,iY);
		pChar->SetDir((char)iDir);
		pChar->SetBoothState(ucShow != 0xFF);  //表示摆摊，0xFF表示没摆摊
		BoothInfo_t& inf = pChar->GetBoothInfo();
		inf.iBoothType = iBoothType;
		inf.iFlagType = iBoothFlagType;
		inf.dwFlagColor = dwBoothFlagColor;
		if (inf.dwFlagColor == 0)
		{
			inf.dwFlagColor = 0xFFFFFFFF;
		}

		inf.strBoothName = szBoothName;
		//g_PetBoothData.SetLepoardBoothName(szBoothName);
	}

	if(ucShow == 0xFF) //取消了摊位
	{
		if(g_pControl->FindWindowByName("OtherBoothWnd") && g_OtherBoothData.GetOtherPlayerID() == nOtherPlayerID)
		{	
			g_pControl->PopupWindow(MSG_CTRL_OTHER_BOOTH,OPER_CLOSE);
			g_OtherBoothData.Clear();
		}
	}

	if(ucShow == 0 || ucShow == 0xFF)
		return;

	if(ucShow == 1){ //摊位详细信息
		g_OtherBoothData.SetOtherPlayerID(nOtherPlayerID);
	}

	if(ucShow == 1 || 
		(ucShow == 2 && (nOtherPlayerID == g_OtherBoothData.GetOtherPlayerID()) && g_pControl->FindWindowByName("OtherBoothWnd"))) 
	{
		//摊位上的物品
		//char chTemp[4096];
		const int iHeaderLen = 22 + MAX_BOOTH_NAME_LEN;
		//if(iLen - iHeaderLen > 4096)
		//	return;
		//memcpy(chTemp ,msg + iHeaderLen,iLen - iHeaderLen);

		//清除前,备份
		CGood BackupGoods[MAX_BOOTH_GOODS_NUM];
		if (ucShow == 2)
		{
			for (int i = 0; i != MAX_BOOTH_GOODS_NUM; ++i)
			{
				BackupGoods[i] = g_OtherBoothData.GetGoods(i);
			}
		}

		g_OtherBoothData.Clear();
		//g_OtherBoothData.ClearC2CGood();
		//如果我正想买的东西更新了要提示玩家
		CGood& ReadyBuyGood = g_OtherBoothData.GetReadyBuy();
		bool bFindReadyBuyGood = false;
		bool bReadyBuyGoodChanged = false;
		bool bBuyBySelf = true;	//判断是否被自己买走

		int i=0,j=0;
		for( i = 0,j = 0 ; i < iLen - iHeaderLen; i += byGoodLen,j++)
		{
			if(j >= MAX_BOOTH_GOODS_NUM)
				return;

			CGood &tmp = g_OtherBoothData.GetGoods(j);
			tmp.FromBuffer(msg + iHeaderLen + i,TRUE,byGoodLen);
			tmp.AddItemState(InBoothGrid);
			if(ReadyBuyGood.GetID() > 0 && tmp.GetID() == ReadyBuyGood.GetID())
			{
				bFindReadyBuyGood = true;
				if(tmp.GetPrice() != ReadyBuyGood.GetPrice() || tmp.GetPayType() != ReadyBuyGood.GetPayType())
				{
					bReadyBuyGoodChanged =true;
				}
			}
		}

		if(!bFindReadyBuyGood)
		{
			g_pControl->MsgToWnd("OtherBoothWnd",MSG_CTRL_OTHER_BOOTH,2);
		}

		if (ucShow == 1 && j>0)
		{    
			int iType = j/5 +5;
			g_pControl->PopupWindow(MSG_CTRL_OTHER_BOOTH,OPER_CREATE,iType);
		}

		if (!bFindReadyBuyGood || bReadyBuyGoodChanged)
		{
			g_OtherBoothData.GetReadyBuy().clear();
		}

		DWORD dwAlreadyBuyGoodID = g_OtherBoothData.GetAlreadyBuy().GetID();

		if (ucShow == 2)
		{
			for (int n = 0, p = 0; n < MAX_BOOTH_GOODS_NUM && p < MAX_BOOTH_GOODS_NUM;)
			{
				if (dwAlreadyBuyGoodID != 0 && dwAlreadyBuyGoodID == BackupGoods[n].GetID())
				{
					n++;
					continue;
				}

				CGood& newGood = g_OtherBoothData.GetGoods(p);

				if (newGood.GetID() != 0 || BackupGoods[n].GetID() != 0)
				{
					if (newGood.GetID() != BackupGoods[n].GetID()
						|| newGood.GetPrice() != BackupGoods[n].GetPrice()
						|| newGood.GetPayType() != BackupGoods[n].GetPayType())
					{
						bBuyBySelf = false;
						break;
					}
				}

				n++;
				p++;
			}

			if(!bBuyBySelf)
			{
				g_pControl->PopupWindow(MSG_CTRL_BOOTH_BUY_CONFIRM,OPER_CLOSE);
				g_pControl->PopupWindow(MSG_CTRL_OTHER_BOOTH,OPER_CLOSE);
				g_MsgBoxMgr.PopSimpleAlert("货架中的物品发生变动，请留意价格。");
			}
		}

		g_OtherBoothData.GetAlreadyBuy().clear();

		////BoothBuyConfirmWnd窗口开着，正在购买，此时如果要购买的物品有变动，提示玩家，BoothBuyConfirmWnd窗口没开时bFindReadyBuyGood及bReadyBuyGoodChanged可能都不准
		//if(g_pControl->FindWindowByName("BoothBuyConfirmWnd"))
		//{
		//	if(!bFindReadyBuyGood)
		//	{
		//		g_pControl->PopupWindow(MSG_CTRL_BOOTH_BUY_CONFIRM,OPER_CLOSE);
		//		g_MsgBoxMgr.PopSimpleAlert("你要购买的物品已经被卖家移除或被其它玩家买走。");
		//	}
		//	else
		//	{
		//		if(bReadyBuyGoodChanged)
		//		{
		//			g_pControl->PopupWindow(MSG_CTRL_BOOTH_BUY_CONFIRM,OPER_CLOSE);
		//			g_MsgBoxMgr.PopSimpleAlert("货架中的物品发生变动，请留意价格。");
		//		}
		//	}
		//}
	}
//#endif
}

//发送玩家摆摊请求
void CGameControl::SEND_Booth_Request_Stall()
{
	SET_COMMAND(CS_BOOTH_REQUEST_STALL,2048);
	strncpy(temp + iBLen,g_BoothData.GetBoothName(),52);
    iBLen += 52;

	BYTE nSize = 0;
	for (int i=0;i<MAX_BOOTH_GOODS_NUM;i++)
	{
		GoodPrice_t &price = g_BoothData.GetPrice(i);
		if (price.GetID() ==0)
			continue;

		ADD_DWORD(price.GetID());
		DWORD ulCount = price.GetYuanBao();
		BYTE ucType = 1;
		if (ulCount == 0)
		{
			ulCount = price.GetMoney();
			ucType = 0;
		}
		ADD_DWORD(ulCount);
		ADD_WORD(ucType);
		nSize++;
	}

	if (SELF.GetBoothState())
	{
		GoodPrice_t& readyPrice = g_BoothData.GetReadyPrice();
		if (readyPrice.GetID() > 0)
		{
			ADD_DWORD(readyPrice.GetID());
			DWORD ulCount = readyPrice.GetYuanBao();
			BYTE ucType = 1;
			if (ulCount == 0)
			{
				ulCount = readyPrice.GetMoney();
				ucType = 0;
			}
			ADD_DWORD(ulCount);
			ADD_WORD(ucType);
			nSize++;
		}
	}

	temp[6] = nSize;
	SEND_GAME_SERVER();

	if (nSize == 0)
	{
		SELF.SetBoothState(false);
		g_pControl->PopupWindow(MSG_CTRL_BOOTH_WND,OPER_CLOSE);
	}
}

//发送玩家取消摆摊请求
void CGameControl::SEND_Booth_Cancel_Stall()
{
	SET_COMMAND(CS_BOOTH_REQUEST_STALL,12);
	SEND_GAME_SERVER();
}
//发送摊位留言
void CGameControl::SEND_Message_Booth(const char * str,int iLen,bool bBuyPart)
{
	string buf;
	buf.assign(str,iLen);

	StringUtil::trim(buf);

	if(buf.size() == 0)
		return;

	SET_COMMAND(CS_MESSAGE_PRIVATE,4096);

	string strPlayerName = SELF.GetName();

	if(!g_DirtyWords.ParseNameIsGM(strPlayerName))
	{
		if(g_DirtyWords.ClearWords1(str))
			return;
		g_DirtyWords.ClearWords(str,temp + 12);
	}
	else
	{
		ADD_STR(buf.c_str());
	}

	iBLen += strlen(temp+12);
	if (bBuyPart)
		temp[6] = 3;
	else
		temp[6] = 2;

	SEND_GAME_SERVER();
}

//0x80D0 
void CGameControl::MSG_C2C_Booth_Info(const char * msg,int iLen)
{
}

void CGameControl::MSG_WoolStore_Token(const char * msg,int iLen)
{
	if(iLen <= 12 || iLen > 4096)
		return;

	WORD       wType   =  Conv_WORD(msg + 10) + 1;
	string     strWoolStoreToken;
	strWoolStoreToken.assign(msg + 12,iLen - 12);

	string URL;
	char url[1024] = {0};

// 	if(wType == WOOOLSTORE_REAL)
// 	{
// 		//实物商城的相关参数 易宝商城
// 		DWORD      dwWoolStoreTargetID = Conv_DWORD(msg);
// 		WORD       wWoolStoreKeyNo = Conv_WORD(msg+8);
// 
// 		URL = g_pStreamMgr->GetWebsite("RSLogon");
// 		if(URL.empty())
// 			URL = "http://gameshop.sdo.com/WooolMall.aspx";
// 
// 		sprintf(url,"%s?id=%d_%d&token=%s",URL.c_str(),dwWoolStoreTargetID,wWoolStoreKeyNo,strWoolStoreToken.c_str());
// 	}
 	//else 
		if(wType == WOOOLSTORE_PAY)
 	{
 		URL = g_pStreamMgr->GetWebsite("PTPay");
 		if(URL.empty())
 			sprintf(url,"http://ingame.pay.sdo.com/GameDeposit/GameLogin.aspx?gamenumid=%u&token=%s",WS_GAME_ID,strWoolStoreToken.c_str());
 		else
 			sprintf(url,"%s?gamenumid=%u&token=%s",URL.c_str(),WS_GAME_ID,strWoolStoreToken.c_str());
 	}
// 	else if(wType == WOOOLSTORE_PAIMAI || wType == WOOOLSTORE_PAIMAI2 || wType == 7)
// 	{
// 		if(g_WooolStoreMgr.GetWooolStorePage() <= 0)
// 			return;
// 
// 		//登陆拍卖行
// 		URL = g_pStreamMgr->GetWebsite("SnMallLogon");
// 		if(URL.empty())
// 			URL = "http://snmall.sdo.com/SnMall.Web/Logon.aspx";
// 
// 		sprintf(url,"%s?gameid=1&time=%u&token=%s",URL.c_str(),GetTickCount(),strWoolStoreToken.c_str());
// 	}
// 	else if(wType == WOOOLSTORE_JIFEN )
// 	{
// 		// 积分商城
// 		URL = g_pStreamMgr->GetWebsite("JiFenLogon");
// 		if(URL.empty())
// 			URL = "http://gameshop.sdo.com/WooolMall.aspx";
// 
// 		sprintf(url,"%s?game=1&NoPortal=1&token=%s",URL.c_str(),strWoolStoreToken.c_str());
// 	}
 	else if(wType == WOOOLSTORE_FENGHONG)
	{
		// 登录分红商城
		URL = g_pStreamMgr->GetWebsite("FengHongLogin");
		if(URL.empty())
			URL = "http://61.172.251.32:8081/ForSSO/GameLogin.aspx";

		sprintf(url,"%s?game=1&NoPortal=1&token=%s",URL.c_str(),strWoolStoreToken.c_str());
	}

	if(strlen(url) != 0)
	{
		g_WidgetMgr.Navigate(EWT_FIRST,url);
	}
}

//0x0710
void CGameControl::SEND_WoolStore_Guid(const char * str,int iType)
{
	TRY_BEGIN;

	SET_COMMAND(CS_WOOLSTORE_GUID,256);
	int  iLen = strlen(str);

// 	//收费的拍卖行 查看豹子摊位
// 	if(iType == WOOOLSTORE_PAIMAI)
// 	{
// 		switch(g_WooolStoreMgr.GetPaiMaiStoreType())
// 		{
// 		case 1:
// 			{
// 				iType = WOOOLSTORE_PAIMAI2;
// 				*((DWORD*)temp) = g_OtherBoothData.GetOtherPlayerID();//查看的豹子ID
// 
// 				//打开商城拍卖行 上传页面
// 				g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORE_WND,OPER_CREATE,3);
// 				break;
// 			}
// 		case 4:
// 			{
// 				g_WooolStoreMgr.SetPaiMaiStoreType(0);
// 				SEND_SUBMIT_PAIMAI_GOOD(str);
// 				return;
// 			}
// 		case 5:
// 			{
// 				string szRedirect = g_PetBoothData.GetRedirectPage();
// 
// 				if(szRedirect.size() + iLen < 244)
// 				{
// 					iType = 7; //6表示跳转地址
// 					strcpy(temp + 12 + iLen + 1,szRedirect.c_str());
// 					iLen += szRedirect.size() + 1;
// 				}
// 			}
// 		}
// 		g_WooolStoreMgr.SetPaiMaiStoreType(0);
// 	}

	ASSIGN_BYTE(10,iType-1);
	ASSIGN_WORD(6,iLen);
	strcpy(temp + 12,str);
	iBLen = CMD_SIZE + iLen;
	SEND_GAME_SERVER();

TRY_END
}


//0x0713
//拍卖行 上传拍卖物品信息
void CGameControl::MSG_PAIMAI_GOOD_INFO(const char * strMsg, int iLen)
{
	//提交拍卖行物品
	int iType = *((WORD*)(strMsg +10)); // 0 豹子 1 玩家
	int iFlag = *((WORD*)(strMsg +6));

	if(iFlag == 1)
	{
		//该物品不能上传
		DWORD upid = g_PetBoothData.GetUploadingID();

		if(upid != 0)
		{
			for(int ii = 0;ii < MAX_UPLOAD_GOODS_NUM;ii++)
			{
				if(g_PetBoothData.GetC2CGood(ii).GetID() == upid)
				{
					char temp[256] = {0};
					sprintf(temp,"[%s]不能上架",g_PetBoothData.GetC2CGood(ii).GetName());

					g_TalkMgr.PushSysTalk(temp);
					break;
				}
			}
			g_PetBoothData.BackToPackage(upid);
			g_PetBoothData.SetUploadingID(0);
		}
		//继续上传
		SEND_SUBMIT_PAIMAI_GOOD(g_WooolStoreMgr.GetWooolStoreGUID());
		return;
	}
	else if(iFlag == 2)
	{
		g_TalkMgr.PushSysTalk("物品上传拍卖数量已超过上限");
		return;
	}
	else if(iFlag > 0)
	{
		return;
	}

	string         strPaiMaiStoreToken;
	strPaiMaiStoreToken.assign(strMsg + 12,iLen - 12);

	char url[1024] = {0};
	string URL = g_pStreamMgr->GetWebsite("SnMallUpload");
	if(URL.empty())
		URL = "http://snmall.sdo.com/SnMall.Web/Trade/NewUpload.aspx";

	sprintf(url,"%s?gameid=1&time=%u&token=%s",URL.c_str(),GetTickCount(),strPaiMaiStoreToken.c_str());

	g_WidgetMgr.SetShow(EWT_FIRST,false);
	g_WidgetMgr.Navigate(EWT_FIRST,url);
}

//卖方的交易成功 
//刷新自己的商城网页
void CGameControl::MSG_PAIMAI_SALE_RESULT(const char * strMsg,int iLen)
{
}

//0x0714
//提交拍卖后的结果
void CGameControl::MSG_PAIMAI_RESULT(const char * strMsg, int iLen)
{
	DWORD tempGoodID = *((DWORD*)strMsg);
	int  iType = *((WORD*)(strMsg + 6)); //0上架/1下架
	int  iResult = *((WORD*)(strMsg + 8)); //结果 >0 失败
	int  iPetOrMan = *( (WORD*)(strMsg + 10)); //0 豹子 1 玩家

	if(iType == 0 && g_PetBoothData.GetUploadingID() != tempGoodID)
		return;

	g_PetBoothData.SetUploadingID(0);

	//上架
	if(iType == 0)
	{
		if(iResult == 0)//上架成功
		{
			for(int i=0;i < MAX_UPLOAD_GOODS_NUM;i++)
			{
				CGood& tmp = g_PetBoothData.GetC2CGood(i);
				if(tmp.GetID() == tempGoodID )
				{
					string str = StringUtil::format("%s成功上架",tmp.GetName());
					g_TalkMgr.PushDataTalk(TALKTYPE_PAIMAI,str.c_str(),str.size(),0x38FF);
					tmp.SetID(0);
				}
			}
			g_PetBoothData.C2CFullFill();
		}
		else //上架失败，放回包裹中间去
		{
			g_PetBoothData.BackToPackage(tempGoodID);
		}

		//继续上传
		SEND_SUBMIT_PAIMAI_GOOD(g_WooolStoreMgr.GetWooolStoreGUID());
		return;
	}
}

//0x0713
//提交拍卖物品请求
void CGameControl::SEND_SUBMIT_PAIMAI_GOOD(const char* strGUID)
{
	//继续上传
	for(int ii = 0;ii < MAX_UPLOAD_GOODS_NUM;ii++)
	{
		DWORD id = g_PetBoothData.GetC2CGood(ii).GetID();
		if(id != 0)
		{
			g_PetBoothData.SetUploadingID(id);
			GoodPrice_t& price = g_PetBoothData.GetPrice(ii);

			SET_COMMAND(CS_SUBMIT_PAIMAI_GOOD,256);
			ASSIGN_ID(id);

			if(price.GetYuanBao() > 0)
			{
				ASSIGN_DWORD(6,price.GetYuanBao());
				temp[11] = 1;
			}
			else if(price.GetMoney() > 0)
			{
				ASSIGN_DWORD(6,price.GetMoney());
				temp[11] = 0;
			}
			ASSIGN_BYTE(10,2);
			ADD_STR(strGUID);
			SEND_GAME_SERVER();
			return; //结束
		}
	}
}

void CGameControl::MSG_Yuanbao_Info(const char * msg,int iLen)
{
	//  设置当前身上的元宝数量, 没有问题
	DWORD iOldYuanBao = SELF.GetYuanBao();
	SELF.SetYuanBao(*((DWORD*)msg));

	//如果玩家在实物商城页面 重新请求更新一次GUID 和 token
	if(SELF.GetYuanBao() > iOldYuanBao)
		g_pControl->MsgToWnd(MSG_CTRL_WOOOL_STORE_WND,MSG_CTRL_REQUIRE_GUID);

}

//绑定的元宝数量
void CGameControl::MSG_Bind_Yuanbao_Info(const char * msg,int iLen)
{
	SELF.SetBindYuanBao(*((DWORD*)msg));
}

void CGameControl::MSG_Credit_Info(const char * msg,int iLen)
{
	//DWORD dwOldCreditArrearage = SELF.GetCreditArrearage();
	//BYTE byOldCreditState = SELF.GetCreditState();

	SELF.SetCredit(Conv_DWORD(msg));
	SELF.SetCreditMax(Conv_DWORD(msg + 6));
	SELF.SetCreditFeeRate(float(Conv_WORD(msg + 10)) / 1000);
	SELF.SetCreditState(BYTE(msg[12]));
	SELF.SetCreditArrearage(Conv_DWORD(msg + 13));

	output_debug("Credit:%u;CreditMax:%u;CreditFeeRate:%f;CreditState:%d;CreditArrearage:%u\r\n",SELF.GetCredit(),SELF.GetCreditMax(),SELF.GetCreditFeeRate(),SELF.GetCreditState(),SELF.GetCreditArrearage());

	if(g_OtherData.GetCreditAlertState() == 0)
	{
		g_OtherData.SetCreditAlertState(1);
	}


	/*	//改成不显示最后还款日期了

	//新出现欠款时请求最后还款日期
	if (dwOldCreditArrearage != SELF.GetCreditArrearage())
	{
	string strUrl = g_pStreamMgr->GetWebsite("LastPayOffDate","");//信用最后还款日期
	if(strUrl.empty())
	strUrl = "http://192.168.118.24/cd/GetRepayDate.aspx";

	strUrl += "?creditID=";
	strUrl += g_Login.GetAccount();

	g_HttpMgr.InternetGet(strUrl.c_str(),INET_LASTPAYOFF_DATE);
	}
	*/

}

//出售物品种类(数量,1字节)+物品名(15字节)+物品编号(15字节)+looks(2字节)+原价(2字节)+银卡(2字节)+金卡(2字节)+限卖个数(2字节)+描述长度(2字节)+描述内容(变长)+...
//+赠送物品种类(数量,1字节)+赠送物品名(15字节)+赠送物品编号(15字节)+looks(2字节)+vip类型(1字节,1:银卡,2:金卡)+数量(2字节)+描述长度(2字节)+描述内容(变长)+....+商城公告长度(2字节)+商城公告(变长)+商城滚动长度(2字节)+商城滚动(变长)
void CGameControl::MSG_Vip_Store_Info(const char * msg,int iLen)
{
	if (iLen <= 12)
	{
		return;
	}

	CVipStoreData& vipData = g_WooolStoreMgr.GetVipStoreData();
	vipData.vPresentItems.clear();
	vipData.vSellItems.clear();
	vipData.tOverdueTime = Conv_DWORD(msg);

	BYTE bySellCount = (BYTE)msg[12];
	int iPos = 12 + 1;
	for (BYTE i = 0; i < bySellCount; i++)
	{
		CVipStoreItem item;
		if (iLen < iPos + 15 * 2 + 2 * 5)
		{
			return;
		}
		item.m_strName.assign(msg + iPos,15);
		iPos += 15;

		item.m_strItemID.assign(msg + iPos,15);
		iPos += 15;

		item.m_wItemLooks = Conv_WORD(msg + iPos);
		iPos += 2;

		item.m_wPrice = Conv_WORD(msg + iPos);
		iPos += 2;

		item.m_wSilverCardPrice = Conv_WORD(msg + iPos);
		iPos += 2;

		item.m_wGoldCardPrice = Conv_WORD(msg + iPos);
		iPos += 2;

		item.m_iItemCount = Conv_WORD(msg + iPos);
		item.m_iLimitRemain = item.m_iItemCount;
		iPos += 2;

		WORD wLen = Conv_WORD(msg + iPos);
		iPos += 2;

		if (iLen < iPos + wLen)
		{
			return;
		}
		item.m_strDesc.assign(msg + iPos,wLen);
		StringUtil::replace_all(item.m_strDesc,"\\","\n");
		iPos += wLen;

		vipData.vSellItems.push_back(item);
	}

	if (iLen < iPos + 1)
	{
		return;
	}
	BYTE byGifCount = (BYTE)msg[iPos];
	iPos += 1;

	for (BYTE i = 0; i < byGifCount; i++)
	{
		CVipStoreItem item;
		if (iLen < iPos + 15 * 2 + 2 * 2 + 1)
		{
			return;
		}
		item.m_strName.assign(msg + iPos,15);
		iPos += 15;

		item.m_strItemID.assign(msg + iPos,15);
		iPos += 15;

		item.m_wItemLooks = Conv_WORD(msg + iPos);
		iPos += 2;

		char cVipType = msg[iPos];
		iPos += 1;

		item.m_iItemCount = Conv_WORD(msg + iPos);
		item.m_iLimitRemain = item.m_iItemCount;
		iPos += 2;

		WORD wLen = Conv_WORD(msg + iPos);
		iPos += 2;

		if (iLen < iPos + wLen)
		{
			return;
		}
		item.m_strDesc.assign(msg + iPos,wLen);
		StringUtil::replace_all(item.m_strDesc,"\\","\n");
		iPos += wLen;

		vipData.vPresentItems.push_back(item);
	}	

	if (iLen < iPos + 2)
	{
		return;
	}
	WORD wMsgLen = Conv_WORD(msg + iPos);
	iPos += 2;
	if (iLen < iPos + wMsgLen)
	{
		return;
	}
	vipData.strServiceMsg.assign(msg + iPos,wMsgLen);
	iPos += wMsgLen;

	if (iLen < iPos + 2)
	{
		return;
	}
	WORD wTipsLen = Conv_WORD(msg + iPos);
	iPos += 2;
	if (iLen < iPos + wTipsLen)
	{
		return;
	}

	vipData.strTips.assign(msg + iPos,wTipsLen);
	__time32_t tNow;
	_time32(&tNow);
	tNow += g_dwServerTime;
	g_WooolStoreMgr.GetVipStoreData().tLastRequireTime = tNow;

	g_pControl->MsgToWnd(MSG_CTRL_VIPSTORE_WND,MSG_CTRL_VIPSTORE_WND,1);

}

//0x0422 个人购买信息
//个人购买或赠送种类(数量,1字节) + 类型(1字节)+物品编号(15字节) + 已买或已领取个数(2字节)+类型(1字节)+物品编号(15字节) + 已买或已领取个数(2字节)....
void CGameControl::MSG_Vip_Store_Personal_Info(const char * msg,int iLen)
{
	if (iLen <= 12)
	{
		return;
	}

	CVipStoreData& vipData = g_WooolStoreMgr.GetVipStoreData();
	vector<CVipStoreItem> &vSellItems = vipData.vSellItems;
	vector<CVipStoreItem> &vPresentItems = vipData.vPresentItems;

	BYTE byCount = (BYTE)msg[12];
	int iPos = 12 + 1;
	for (BYTE i = 0; i < byCount; i++)
	{
		BYTE byType = (BYTE)msg[iPos];
		iPos += 1;
		if (iLen < iPos + 17)
		{
			return;
		}
		string strItemID;
		strItemID.assign(msg + iPos,15);
		iPos += 15;
		WORD wSoldCount = Conv_WORD(msg + iPos);
		iPos += 2;

		if (byType == 1)
		{
			for (size_t j = 0; j < vSellItems.size(); j++)
			{
				if (strcmp(vSellItems[j].m_strItemID.c_str(),strItemID.c_str()) == 0)
				{
					vSellItems[j].m_iLimitRemain = vSellItems[j].m_iItemCount - wSoldCount;
					if (vSellItems[j].m_iLimitRemain < 0)
					{
						vSellItems[j].m_iLimitRemain = 0;
					}
					break;
				}			
			}
		}
		else
		{
			for (size_t j = 0; j < vPresentItems.size(); j++)
			{
				if (strcmp(vPresentItems[j].m_strItemID.c_str(),strItemID.c_str()) == 0)
				{
					vPresentItems[j].m_iLimitRemain = vPresentItems[j].m_iItemCount - wSoldCount;
					if (vPresentItems[j].m_iLimitRemain < 0)
					{
						vPresentItems[j].m_iLimitRemain = 0;
					}
					break;
				}			
			}
		}

	}

	g_pControl->MsgToWnd(MSG_CTRL_VIPSTORE_WND,MSG_CTRL_VIPSTORE_WND,1);

}
/*
	点击租赁摊位npc,根据状态弹出不同窗口 or 商铺信息变更发来的同步消息
	4个字节的NPCID，
	2个字节的消息类型,
	2个字节就是状态信息（0，1，2, 3）
	4种状态:
		0: 无人租
		1: 别人已租
		2: 自己已租
		3: 商铺信息变更
	若是0,接下来的四个字节代表到期时间
	若是1或2，则接下来2个字节表示物品数量
	若是1,则接下来2个字节代表租赁所剩天数
	消息头之后,2个字节代表所有人的名字长度
	之后,所有人名字
	后面是物品信息
*/
void CGameControl::Msg_LeaseBooth_State(const char * msg, int iLen)
{
	if(strlen(msg)<=0){
		return;
	}
	DWORD nOtherPlayerID = Conv_DWORD(msg);
	WORD state = Conv_WORD(msg + 6);

	BYTE byGoodNum = BYTE(msg[8]);//物品个数
	BYTE byGoodLen = BYTE(msg[9]);//单个物品的长度
	if (byGoodLen == 0)
	{
		byGoodLen = CGood::PKLength();
	}
	
	if (state == 0)//无人租
	{
		g_OtherBoothData.Clear();
		g_OtherBoothData.SetOtherPlayerID(nOtherPlayerID);
		vector<S_LeaseBoothType> &VType = g_OtherBoothData.GetLeaseBoothTypeVector();
		VType.clear();

		WORD wTypeNum =  Conv_WORD(msg + 12);
		S_LeaseBoothType lbType;
		for (int i = 0; i < wTypeNum; i++)
		{
			lbType.wDays = Conv_WORD(msg + 12 + 2 + i*4);
			lbType.wPrice = Conv_WORD(msg + 12 + 4 + i*4);
			VType.push_back(lbType);
		}

		g_pControl->PopupWindow(MSG_CTRL_LEASEBOOTH_WND,OPER_RECREATE,0);
		return;
	}

	WORD nameLength = Conv_WORD(msg + 12);
	char boothName[256] = {0};
	memcpy(boothName,msg + 14,nameLength);	

	//char chTemp[4096];
	const int iHeaderLen = 12 + 2 + nameLength;

	//if(iLen - iHeaderLen > 4096)
	//	return;

	//memcpy(chTemp ,msg + iHeaderLen,iLen - iHeaderLen);	

	if (state == 1 || state == 2)//界面新打开
	{
		g_OtherBoothData.SetOtherPlayerID(nOtherPlayerID);
		g_OtherBoothData.SetBoothName(boothName);
		g_OtherBoothData.Clear();

		g_OtherBoothData.GetReadyBuy().clear();
		g_OtherBoothData.GetAlreadyBuy().clear();

		int i=0,j=0;
		for( i = 0,j = 0 ; i < iLen - iHeaderLen; i += byGoodLen,j++)
		{
			if(j >= MAX_BOOTH_GOODS_NUM)
				return;

			CGood &tmp = g_OtherBoothData.GetGoods(j);
			tmp.FromBuffer(msg + iHeaderLen + i,TRUE,byGoodLen);
			tmp.AddItemState(InBoothGrid);
		}
		if (state == 1)//别人租
		{
			WORD days = Conv_WORD(msg + 10);
			g_OtherBoothData.SetDaysRemain(days);
			g_pControl->PopupWindow(MSG_CTRL_LEASEOTHERBOOTH_WND,OPER_RECREATE);
		}
		else//自己租
		{
			WORD days = Conv_WORD(msg + 10);
			g_OtherBoothData.SetDaysRemain(days);
			g_pControl->PopupWindow(MSG_CTRL_LEASEBOOTH_WND,OPER_RECREATE,2);
		}
	}
	else if (state == 3 && nOtherPlayerID == g_OtherBoothData.GetOtherPlayerID())//商铺信息变更
	{
		CGood BackupGoods[MAX_BOOTH_GOODS_NUM];
		for (int i = 0; i != MAX_BOOTH_GOODS_NUM; ++i)
		{
			BackupGoods[i] = g_OtherBoothData.GetGoods(i);
		}
		g_OtherBoothData.Clear();

		if (g_pControl->FindWindowByName("LeaseOtherBoothWnd") || g_pControl->FindWindowByName("LeaseBoothWnd"))//购物者&拥有者
		{
			CGood& ReadyBuyGood = g_OtherBoothData.GetReadyBuy();

			bool bFindReadyBuyGood = false;
			bool bReadyBuyGoodChanged = false;
			bool bBuyBySelf = true;	//判断是否被自己买走

			int i=0,j=0;
			for( i = 0,j = 0 ; i < iLen - iHeaderLen; i += byGoodLen,j++)
			{
				if(j >= MAX_BOOTH_GOODS_NUM)
					return;

				CGood &tmp = g_OtherBoothData.GetGoods(j);
				tmp.FromBuffer(msg + iHeaderLen + i,TRUE,byGoodLen);
				tmp.AddItemState(InBoothGrid);
				if(ReadyBuyGood.GetID() > 0 && tmp.GetID() == ReadyBuyGood.GetID())
				{
					bFindReadyBuyGood = true;
					if(tmp.GetPrice() != ReadyBuyGood.GetPrice() || tmp.GetPayType() != ReadyBuyGood.GetPayType())
					{
						bReadyBuyGoodChanged =true;
					}
				}
			}			

			if(!bFindReadyBuyGood)
			{
				g_pControl->MsgToWnd("LeaseOtherBoothWnd",MSG_CTRL_OTHER_BOOTH,2);
			}

			DWORD dwAlreadyBuyGoodID = g_OtherBoothData.GetAlreadyBuy().GetID();

			for (int n = 0, p = 0; n < MAX_BOOTH_GOODS_NUM && p < MAX_BOOTH_GOODS_NUM;)
			{
				if (dwAlreadyBuyGoodID != 0 && dwAlreadyBuyGoodID == BackupGoods[n].GetID())
				{
					n++;
					continue;
				}

				CGood& newGood = g_OtherBoothData.GetGoods(p);

				if (newGood.GetID() != 0 || BackupGoods[n].GetID() != 0)
				{
					if (newGood.GetID() != BackupGoods[n].GetID()
						|| newGood.GetPrice() != BackupGoods[n].GetPrice()
						|| newGood.GetPayType() != BackupGoods[n].GetPayType())
					{
						bBuyBySelf = false;
						break;
					}
				}

				n++;
				p++;
			}
			
			if (g_pControl->FindWindowByName("LeaseOtherBoothWnd") && !bBuyBySelf)//购物者
			{
				g_pControl->PopupWindow(MSG_CTRL_BOOTH_BUY_CONFIRM,OPER_CLOSE);
				g_pControl->PopupWindow(MSG_CTRL_LEASEOTHERBOOTH_WND,OPER_CLOSE);
				g_MsgBoxMgr.PopSimpleAlert("货架中的物品发生变动，请留意价格。");
				/*if(g_pControl->FindWindowByName("BoothBuyConfirmWnd"))
				{
					if(ReadyBuyGood.GetID() > 0 && !bFindReadyBuyGood)
					{
						ReadyBuyGood.SetID(0);
						g_pControl->PopupWindow(MSG_CTRL_BOOTH_BUY_CONFIRM,OPER_CLOSE);
						g_MsgBoxMgr.PopSimpleAlert("你要购买的物品已经被卖家移除或被其它玩家买走。");
					}
					else
					{
						if(bReadyBuyGoodChanged)
						{
							ReadyBuyGood.SetID(0);
							g_pControl->PopupWindow(MSG_CTRL_BOOTH_BUY_CONFIRM,OPER_CLOSE);
							g_MsgBoxMgr.PopSimpleAlert("货架中的物品发生变动，请留意价格。");
						}
					}
				}*/
			}
			else
			{
				if (ReadyBuyGood.GetID() > 0 && !bFindReadyBuyGood)
				{
					//g_MsgBoxMgr.PopSimpleAlert("你要拿回的物品已经被被其它玩家买走。");
					if (CGoodGrid::GetDropGoodFrom().DropGood.GetID() == ReadyBuyGood.GetID())
					{
						CGoodGrid::GetDropGoodFrom().DropGood.SetID(0);
						ReadyBuyGood.SetID(0);
					}
				} 
			}
		}

		g_OtherBoothData.GetAlreadyBuy().clear();
	}
}
void CGameControl::Send_LeaseBooth_Request(int iType)
{
	SET_COMMAND(CS_LEASEBOOTH_REQUEST,12);
	ASSIGN_ID(g_OtherBoothData.GetOtherPlayerID());
	
	ASSIGN_WORD(6,iType);
	SEND_GAME_SERVER();
}
void CGameControl::Msg_LeaseBooth_Response(const char * msg, int iLen)
{
	DWORD nOtherPlayerID = Conv_DWORD(msg);
	WORD result = Conv_WORD(msg + 6);

	if (result == 1)
	{
		g_MsgBoxMgr.PopSimpleAlert("租赁商铺成功");
		g_pControl->PopupWindow(MSG_CTRL_LEASEBOOTH_WND,OPER_CLOSE);
	}
}

/*
	寄卖商品
	0-3: npcID
	4-5:协议号:0x0467
	6:物品数量
	12开始:每10个字节一个商品
	4字节商品ID,4字节价格(元宝或金币),2字节标示元宝或金币: 1-元宝,0-金币
*/
void CGameControl::Send_LeaseBooth_Start(DWORD npcid,bool onlyOne)
{
	SET_COMMAND(CS_LEASEBOOTH_START,2048);
	
	ASSIGN_DWORD(0,npcid);//npcID

	BYTE nSize = 0;
	DWORD ulCount = 0;
	BYTE ucType = 0;
	if (onlyOne)//单一物品
	{
		nSize = 1;
		CGood& readyGood = g_OtherBoothData.GetReadyGood();
		if (readyGood.GetID() > 0)
		{
			ADD_DWORD(readyGood.GetID());
			ulCount = readyGood.GetPrice();
			if (readyGood.GetPayType())//元宝方式
			{
				ucType = 1;
			} 
			else//金币方式
			{
				ucType = 0;
			}

			ADD_DWORD(ulCount);
			ADD_WORD(ucType);
		}
	} 
	else//多物品
	{
		for (int i=0;i<MAX_BOOTH_GOODS_NUM;i++)
		{
			CGood &price = g_OtherBoothData.GetGoods(i);
			if (price.GetID() ==0)
				continue;

			ADD_DWORD(price.GetID());
			ulCount = price.GetPrice();
			if (price.GetPayType())//元宝方式
			{
				ucType = 1;
			} 
			else//金币方式
			{
				ucType = 0;
			}

			ADD_DWORD(ulCount);
			ADD_WORD(ucType);
			nSize++;
		}
	}
	
	temp[6] = nSize;
	SEND_GAME_SERVER();
}

//寄卖回复
//7-8:成功失败
void CGameControl::Msg_LeaseBooth_Start(const char * msg, int iLen)
{
	DWORD nOtherPlayerID = Conv_DWORD(msg);
	WORD result = Conv_WORD(msg + 6);
	
	if (result == 1)//寄卖成功
	{
		if (g_pControl->FindWindowByName("LeaseBoothWnd"))
		{
			g_OtherBoothData.AddGood();
			g_OtherBoothData.GetReadyBuy().SetID(0);
		}	
	} 
}
/*
	从租赁商铺中拿下商品:买别人的,或者拿回自己的
	0-3:npcid
	4-5:协议号
	6-9:物品ID
	10:1:买别人的,0:从自己的拿回
*/
void CGameControl::Send_LeaseBooth_BuyOthers(DWORD npcid, DWORD goodid,BYTE buyOthers,const char * strName)
{
	SET_COMMAND(CS_LEASEBOOTH_BUYOTHERS,12);
	ASSIGN_ID(npcid);
	ASSIGN_DWORD(6,goodid);
	ASSIGN_BYTE(10,buyOthers);
	ADD_STR(strName);
	SEND_GAME_SERVER();
}
void CGameControl::Msg_LeaseBooth_BuyOthers(const char * msg, int iLen)
{
	/*
		失败需要把商品放回g_otherBoothDAta
	*/
	string str;

	switch(msg[0])
	{
	case 0x01:
		str = "物品被卖出。";
		break;
	case 0x02:
		str = "不能携带更多的物品了！";
		break;
	case 0x03:////////////////////////
		str = "你没有足够的钱来购买物品。";
		break;
	case 0x04://///////////////////////////
		str = "你没有足够的元宝来购买物品。";////////////////
		break;
	case 0x05:
		str = "对方不能容纳更多金币。";
		break;
	case 0x06:
		str = "对方不能容纳更多元宝。";
		break;
	default:
		str = "购买物品失败！";
		break;
	}

	g_pControl->MsgToWnd("LeaseOtherBoothWnd",MSG_CTRL_OTHER_BOOTH,2);

	g_MsgBoxMgr.PopSimpleAlert(str.c_str());
}
/*
	前4个字节是NPCID，
	5,6字节是协议号0470；
	7,8字节是表示取钱还是取东西，0：钱；1：东西
*/
void CGameControl::Send_LeaseBooth_TakeMoneyBack(WORD flag)
{
	SET_COMMAND(CS_LEASEBOOTH_TAKEBACKMONEY,12);
	ASSIGN_WORD(6,flag);
	SEND_GAME_SERVER();
}
void CGameControl::Msg_LeaseBooth_TakeMoneyBackResponse(const char * msg, int iLen)
{

}
void CGameControl::Send_LeaseBooth_LeaveMessage(DWORD npcid,WORD msgLen,const char * message)
{
	SET_COMMAND(CS_LEASEBOOTH_LEAVEMESSAGE,256);
	ASSIGN_ID(npcid);
	ASSIGN_WORD(6,msgLen);
	ADD_STR(message);
	SEND_GAME_SERVER();
}
/*
	租赁过期
	0-3 npcid
*/
void CGameControl::Msg_LeaseBooth_OverdueTime(const char * msg, int iLen)
{
	DWORD nOtherPlayerID = Conv_DWORD(msg);
	if (g_pControl->FindWindowByName("LeaseOtherBoothWnd") 
		|| g_pControl->FindWindowByName("LeaseBoothWnd") 
		&& nOtherPlayerID == g_OtherBoothData.GetOtherPlayerID())
	{
		g_MsgBoxMgr.PopSimpleAlert("当前打开的摊位已过期!");
		g_pControl->PopupWindow(MSG_CTRL_LEASEOTHERBOOTH_WND,OPER_CLOSE);
		g_pControl->PopupWindow(MSG_CTRL_LEASEBOOTH_WND,OPER_CLOSE);
	}
	
}
////商行协议//
/*
	协议号:0x0472
	0-3:物品iD
	6-7:页数
	8: 1-点页签,2-点下拉菜单
	9: 1-自己的,2-别人的收购信息
	10:阶数
	11:0-金币+元宝,1-金币,2-元宝
*/
void CGameControl::Send_Purchase_Request(DWORD goodsID,WORD wPage,BYTE byType,BYTE bySelf,BYTE level,BYTE tradeType)
{
	SET_COMMAND(CS_FIRM_SHOWPURCHASEITEM,12);
	ASSIGN_ID(goodsID);
	ASSIGN_WORD(6,wPage);
	ASSIGN_BYTE(8,byType);
	ASSIGN_BYTE(9,bySelf);
	ASSIGN_BYTE(10,level);
	ASSIGN_BYTE(11,tradeType+1);
	SEND_GAME_SERVER();
}
/*
*(BYTE*)(szHead+6) = byEnd;
*(BYTE*)(szHead+7) = byType;
*(WORD*)(szHead+8) = nPage;
*/
void CGameControl::Msg_GetFirmItemList(const char * msg, int iLen)
{
	_FirmData & info = g_NPC.GetFirmData();
	info.bEnd = msg[6] != 0?true:false;
	info.iCurPage = Conv_WORD(msg + 8);

	info.vecFirmItemList.clear();

	int start = 12;

	while(start < iLen)
	{
		_FirmData::_ItemListInfo entry;

		entry.dwRowID = Conv_DWORD(msg + start);
		start += 4;

		entry.strPlayerName.assign(msg + start,15);
		start += 15;

		entry.strItemName.assign(msg + start,25);
		start += 25;

		entry.dwItemIdx = Conv_DWORD(msg + start);
		start += 4;
		entry.nLevel = Conv_DWORD(msg + start);
		start += 4;
		entry.nUnitNum = Conv_DWORD(msg + start);
		start += 4;
		entry.nNumPerUnit = Conv_DWORD(msg + start);
		start += 4;
		entry.dwPrice = Conv_DWORD(msg + start);
		start += 4;
		entry.byTradeType = Conv_BYTE(msg + start);
		start += 1;

		info.vecFirmItemList.push_back(entry);
	}
}
void CGameControl::Msg_SelectFirmItem(const char * msg, int iLen)
{
	_FirmData& firmData = g_NPC.GetFirmData();
	
	firmData.vecItemNameID.clear();
	
	int start = 12;

	while(start < iLen)
	{
		_FirmData::_ItemNameID entry;

		entry.strItemName.assign(msg + start,25); 
		start += 25;

		entry.dwItemID = Conv_DWORD(msg + start);
		start += 4;
		
		entry.nLevel = Conv_DWORD(msg + start);
		start += 4;

		entry.bCanOverlap = (msg[start] == 1)?true:false;;
		start += 1;

		firmData.vecItemNameID.push_back(entry);
	}
	g_pControl->PopupWindow(MSG_CTRL_FIRM_WND,OPER_RECREATE);
}

void CGameControl::Msg_PurchaseItem(const char * msg, int iLen)
{	
	//byte byResult = *(BYTE*)(msg + 6);
	//if (byResult == 1)
	//{
		// 发布收购物品成功
		g_pControl->Msg(MSG_CTRL_FIRM_WND,10);
	//}
}

void CGameControl::Send_MyPurchaseInfo(DWORD itemIdx,DWORD nlevel,DWORD nUnitNum,
									   DWORD nNumPerUnit,DWORD dwPrice,BYTE byTradeType)
{
	SET_COMMAND(CS_FIRM_SENDPURCHASEITEMINFO,32);
	ASSIGN_ID(itemIdx);
	ASSIGN_DWORD(6,nlevel);
	ADD_DWORD(nUnitNum);
	ADD_DWORD(nNumPerUnit);
	ADD_DWORD(dwPrice);
	ADD_BYTE(byTradeType);
	SEND_GAME_SERVER();
}
void CGameControl::Send_SellItem(DWORD dwRowID,DWORD dwGoodsID)
{
	SET_COMMAND(CS_FIRM_SELLITEM,12);
	ASSIGN_ID(dwRowID);
	ASSIGN_DWORD(6,dwGoodsID);
	SEND_GAME_SERVER();
}
void CGameControl::Send_CancelPurchase(DWORD dwRowID)
{
	SET_COMMAND(CS_FIRM_CANCELPURCHASE,12);
	ASSIGN_ID(dwRowID);
	SEND_GAME_SERVER();
}
void CGameControl::Send_TakeBackGoods()
{
	SET_COMMAND(CS_FIRM_TAKEBACKGOODS,12);
	SEND_GAME_SERVER();
}

void CGameControl::Send_GetMoneyOver()
{
	SET_COMMAND(CS_FIRM_GETMONEYOVER,12);
	SEND_GAME_SERVER();
}