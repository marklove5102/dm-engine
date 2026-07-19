#include "AIMsgFilter.h"
#include "Global/StringUtil.h"
#include "BaseClass/Control/Control.h"
#include "Global/Interface/WeatherInterface.h"
#include "GameData/MagicCtrlMgr.h"
#include "GameData/GameData.h"
#include "GameData/LoginData.h"
#include "GameData/TalkMgr.h"
#include "GameControl/GameControl.h"
#include "GameData/ConfigData.h"
#include "WndClass/GameWnd/GameManager.h"
#include "GameMap/GameMap.h"
#include "GameMap/MinMap.h"
#include "GameAI/AIGoodMgr.h"

#include "AIAutoPickMgr.h"
#include "AIMgr.h"
#include "AIConfigMgr.h"
#include "AIBossCfgMgr.h"
#include "AIAutoMgr.h"
#include "AIAutoFightMgr.h"
#include "GameData/ItemCfgMgr.h"
#include "GameData/OtherData.h"
#include "GameData/TipsCfg.h"
#include "GameData/XmlCfg.h"
#include "BaseClass/Misc/Net.h"
#include "GameData/ProtocolCfg.h"
#include "BaseClass/Control/UiManager.h"

CAIMsgFilter g_MsgFilter;

bool CAIMsgFilter::FilterSend(const char* str)
{
	m_strMsg = str;
	StringUtil::trim(m_strMsg);

	if(FindHeader("@showfps"))
	{
		SendMessage(g_hWnd,WM_SHOWFPS,0,0);
		return true;
	}
	else if(FindHeader("@showpref"))
	{
		SendMessage(g_hWnd,WM_SHOWPERF,0,0);
		return true;
	}
	else if(FindHeader("@开启辅助工具"))
	{
		g_AIAutoMgr.LaunchWaigua();
		g_TalkMgr.PushSysTalk("辅助工具已开启");
		return true;
	}
	else if(FindHeader("@关闭辅助工具"))
	{
		g_AIAutoMgr.ExitWaigua();
		g_TalkMgr.PushSysTalk("辅助工具已关闭");
		return true;
	}
	else if(FindHeader("@cleartex"))
	{
		g_pStreamMgr->ClearDownloadingFilesList();
		g_pTexMgr->ClearAllTex();
		return true;
	}
	else if(FindHeader("@reloadcfg"))
	{
		g_ItemCfgMgr.ReLoad();
		g_pGameMap->LoadDynObjData();
		return true;
	}
	else if(FindHeader("@reloadtips"))
	{
		g_TipsCfg.ReLoadGoodsTipsCfg();
		g_TipsCfg.ReLoadSkillTipsCfg();
		g_TipsCfg.ReLoadOtherTipsCfg();
		g_XmlCfg.ReloadXmgCfg();
		return true;
	}
	else if(FindHeader("@reloadsoullevelupexpcfg"))
	{
		g_AIGoodMgr.LoadSoulLevelUpExp();
		return true;
	}
	else if(FindHeader("@reloadhelpcfg"))
	{
		g_AICfgMgr.LoadFireLianHuaHelpCfg();
		g_AICfgMgr.LoadQiShuHelpCfg();
		return true;
	}
	else if(FindHeader("@reloadmapjump"))
	{
		g_MapFinder.LoadMapJumpXML();
		return true;
	}
	else if (FindHeader("@reloaddynmap"))
	{
		g_pGameMap->LoadDynMinMap();
		return true;
	}
	else if (FindHeader("@reloadminmap"))
	{
		g_pMinMap->Load();
		return true;
	}
	else if (FindHeader("@reloaduixml"))
	{
		string path = StringUtil::format("ui\\ui.xml");
		g_pUiMgr->LoadFile(path.c_str());
		return true;
	}
	else if(FindHeader("@reloaddat"))
	{
		g_ActionInf.LoadDATFile();
		return true;
	}
	else if(FindHeader("@currentdir"))
	{
		char strPath[MAX_PATH]={0};
		GetCurrentDirectory(MAX_PATH,strPath);
		MessageBox(g_hWnd,strPath,g_AICfgMgr.GetConfigPath(),MB_OK);
		return true;
	}
	else if(FindHeader("@setfont"))
	{
		g_Config.SetShowTestFontText(!g_Config.IsShowTestFontText());
		g_pFont->ClearCache();

		int i = 0;
		int iShow = StringUtil::toInt(m_strMsg,i,' ');
		g_Config.SetShowTestFontText(iShow != 0?true:false);
		float fWeight = StringUtil::toFloat(m_strMsg,i,' ');
		float fContrast = StringUtil::toFloat(m_strMsg,i,' ');
		float fGamma = StringUtil::toFloat(m_strMsg,i,' ');
		int iMode = StringUtil::toInt(m_strMsg,i,' ');

		g_pFont->SetFontConfig(fWeight,fContrast,fGamma,iMode);
		return true;
	}
	else if(FindHeader("@netmsglen"))
	{
		char szTemp[128];
		DWORD dwCount = (GetTickCount() - g_dwProStartCount) / 1000;
		sprintf(szTemp,"Time:%us,Send:%u Byte,Receive:%u Byte,Speed_Send:%u Byte/s,Speed_Receive:%u Bytes/s,", dwCount ,g_pNet->GetTotalSendBytes(),g_pNet->GetTotalRecvBytes(),g_pNet->GetTotalSendBytes()/dwCount,g_pNet->GetTotalRecvBytes()/dwCount);
		g_TalkMgr.PushSysTalk(szTemp);
		return true;
	}
	else if(FindHeader("@downloadnetmsg"))
	{
		if(g_pDownLoadNet)
		{
			char szTemp[256];
			char szServerIp[32] = {0};
			g_pDownLoadNet->GetServer(szServerIp);
			DWORD dwCount = (GetTickCount() - g_dwProStartCount) / 1000;
			sprintf(szTemp,"ServerIp:%s,Time:%us,Send:%u Byte,Receive:%u Byte,Speed_Send:%u Byte/s,Speed_Receive:%u Bytes/s,DownloadedFiles:%d,SavingFiles:%d,ReceiveDataBufferSize:%d,DownloadingFiles:%d",szServerIp, dwCount ,g_pDownLoadNet->GetTotalSendBytes(),g_pDownLoadNet->GetTotalRecvBytes(),g_pDownLoadNet->GetTotalSendBytes()/dwCount,g_pDownLoadNet->GetTotalRecvBytes()/dwCount,g_pDownLoadNet->GetReceiveFiles(),g_pStreamMgr->GetSavingFiles(),g_pDownLoadNet->GetReceiveBuffSize(),g_pStreamMgr->GetDownloadingFiles());
			g_TalkMgr.PushSysTalk(szTemp);
		}
		return true;
	}
	else if(FindHeader("@testmagic"))
	{
		g_pMagicCtrl->LoadFile();
		if(m_strMsg.size() == 0)
			return true;

		DWORD dwTargetID = g_pControl->GetMouseOnID();
		int iX,iY;
		g_pGameMgr->GetMouseTile(iX,iY);
		int i = 0;
		int iMagicID = StringUtil::toInt(m_strMsg,i,' ');
		string strColor = StringUtil::toStr(m_strMsg,i);
		DWORD dwColor = 0xFFFFFFFF;
		if(strColor.length() > 0)
		{
			char   *stopstring;
			dwColor = strtoul(strColor.c_str(),&stopstring,16);
		}

		Magic_Show_s* ms = NULL;
		if(dwTargetID == 0)
			ms = g_pMagicCtrl->CreateMagic(iMagicID,EMP_MAGIC_NOTARGET|EMP_MAGIC_SERVER_ADVISE,SELF.GetID(),MAKELONG(iX,iY),-1,dwColor);
		else
			ms = g_pMagicCtrl->CreateMagic(iMagicID,EMP_MAGIC_SERVER_ADVISE,SELF.GetID(),dwTargetID,-1,dwColor);

		if(ms)
			ms->attr &= ~EMP_MAGIC_WAIT_SERVER;

		return true;
	}
#ifdef _DEBUG
	else if(FindHeader("@delmagic"))
	{
		g_pMagicCtrl->LoadFile();
		if(m_strMsg.size() == 0)
			return true;

		int i = 0;
		int iMagicID = StringUtil::toInt(m_strMsg,i,' ');

		g_pMagicCtrl->DelMagicTex(iMagicID);
		return true;
	}
#endif
	else if(FindHeader("@testitemmagic"))
	{
		g_pMagicCtrl->LoadFile();
		char temp_str[2048] = {0};
		strcpy(temp_str,str);

		char* szCommand = strtok(temp_str," ");
		if(NULL == szCommand)
			return true;

		char* szItemType = strtok(NULL, " ");
		if(NULL == szItemType || strlen(szItemType) == 0)
			return true;

		WORD wItemType = atoi(szItemType);
		if(wItemType == 0)
			return true;

		char* szColor = strtok(NULL, " ");
		DWORD dwColor = 0xFFFFFFFF;
		if(szColor)
		{
			char   *stopstring;
			dwColor = strtoul(szColor,&stopstring,16);
		}

		int iX  = 0,iY = 0;
		char* szX = strtok(NULL," ");
		if(szX == NULL)
			SELF.GetXY(iX,iY);
		else
		{
			iX = atoi(szX);
			char* szY = strtok(NULL," ");
			if(szY != NULL)
			{
				iY = atoi(szY);
			}
		}

		char temp[16] = {0};
		*(DWORD*)(temp) = SELF.GetID();
		*(WORD*)(temp + 4) = SC_ITEM_EFFECT_SWITH;
		*(WORD*)(temp + 10) = wItemType;
		*(WORD*)(temp + 6) = iX;
		*(WORD*)(temp + 8) = iY;
		*(DWORD*)(temp + 12) = dwColor;

		g_pGameControl->MSG_Item_Effect_Switch(temp,16);
		return true;
	}
	else if(FindHeader("@magic"))
	{
		g_Config.SetEnableMagic(!g_Config.IsEnableMagic());
		return true;
	}
	else if(FindHeader("@block"))
	{
		g_Config.SetShowBlock(!g_Config.IsShowBlock());
		return true;
	}
	else if(FindHeader("@talk"))
	{
		g_Config.SetShowTalk(!g_Config.IsShowTalk());
		return true;
	}
	else if (FindHeader("@testproto"))
	{
		int i = 0;
		std::string strID = StringUtil::toStr(m_strMsg,i,' ');
		DWORD dwID = 0;
		if(strID.length() > 0)
		{			
			dwID = DWORD(strtoul(strID.c_str(),0,16));
			g_pGameControl->SEND_TestProto(WORD(dwID));
		}
		return true;
	}
	else if(FindHeader("@reloadproto"))
	{
		g_ProtosCfg.ReLoadProtocolCfg();
		return true;
	}
	else if (FindHeader("@mergept"))
	{
		if (g_Login.GetLoginInExpType() == 1)
		{
			int i = 0;
			std::string strAccount = StringUtil::toStr(m_strMsg,i,' ');
			std::string strpassWd = StringUtil::toStr(m_strMsg,i,' ');
			g_pGameControl->SEND_MergePT_Req(strAccount.c_str(),strpassWd.c_str());
		}
		return true;
	}
#ifdef _DEBUG
	else if(FindHeader("@walkto"))
	{
		int _iDstX = 0,_iDstY = 0;
		if(sscanf(m_strMsg.c_str() ,"%d %d",&_iDstX,&_iDstY) >= 2)
		{
			if(g_pGameMgr)
			{
				g_OtherData.SetFindPathReason(2);
				g_pGameMgr->AI_AutoWalk(_iDstX,_iDstY);
			}
		}
		return true;
	}
	else if(FindHeader("@showallchar"))
	{
		string str;
		MiniChar& vChar = MapArray.GetMiniChar();
		MiniChar::iterator itr;
		for(itr = vChar.begin();itr != vChar.end();itr++)
		{
			CSimpleCharacterNode* pChar = itr->second;
			if(pChar == NULL) continue;

			switch(pChar->GetRaceType())
			{
			case CHARACTER_HUMAN:
				str = "[人]";
				break;
			case CHARACTER_NPC:
				str = "[NPC]";
				break;
			default:
				str = "[怪]";
				break;
			}
			int cx,cy;
			pChar->GetRealXY(cx,cy);
			str.append(StringUtil::format(" %s 位置:%d,%d 编号:%d ID:%u",pChar->GetName(),cx,cy,pChar->GetRaceNo(),pChar->GetID()));
			g_TalkMgr.PushSysTalk(str.c_str(),TALKCOLOR_BLUE);
		}
		return true;
	}
	else if(FindHeader("@enableweather"))
	{
		g_Config.SetWeather(1);
		int iParticle = atoi(m_strMsg.c_str());

		if((iParticle>= 0 && iParticle <= PARTICLE_FLOWER8))
		{
			g_pWeather->EnableParticle(iParticle);
		}
		return true;
	}
	else if(FindHeader("@reloadmagic"))
	{
		g_pMagicCtrl->FinishAllMagic();
		g_pMagicCtrl->LoadFile();
		return true;
	}
	else if(FindHeader("@lmagic"))
	{
		g_pMagicCtrl->LoadFile();
		if(m_strMsg.size() == 0)
			return true;

		SELF.PushSNextAction(ACTION_MAGIC,SELF.GetDir());

		DWORD dwTargetID = g_pControl->GetMouseOnID();
		int iX,iY;
		g_pGameMgr->GetMouseTile(iX,iY);
		int iMagicID = atoi(m_strMsg.c_str());
		if(dwTargetID == 0)
			g_pMagicCtrl->CreateMagic(iMagicID,EMP_MAGIC_NOTARGET,SELF.GetID(),MAKELONG(iX,iY));
		else
			g_pMagicCtrl->CreateMagic(iMagicID,0,SELF.GetID(),dwTargetID);

		return true;
	}
#endif

	if(g_AIAutoMgr.IsEnalbeWaiGua())
	{
		if(FindHeader("@fly"))
		{
			WORD wMagicID = 0;
			if(SELF.FindMagicPos(MAGICID_ENCHANTER_WILD) >= 0)
				wMagicID = MAGICID_ENCHANTER_WILD;
			else if(SELF.FindMagicPos(MAGICID_ESCAPE_QUICK) >= 0)
				wMagicID = MAGICID_ESCAPE_QUICK;
			else if(SELF.FindMagicPos(MAGICID_SHIFT_TRANSPORT) >= 0)
				wMagicID = MAGICID_SHIFT_TRANSPORT;

			if(wMagicID == 0)
				return true;

			int x,y;
			char * p = (char*) (str + 5);
			while(*p != 0)
			{
				if(*p == 0x20)
				{
					*p = 0;
					break;
				}
				p++;
			}
			x = atol(str + 5);
			y = atol(++p);
			g_pGameControl->SEND_Player_Attack_Magic(0,x,y,wMagicID);
			return true;
		}
		//设置在MINIMAP地图上要直接显示的坐标
		else if(FindHeader("@find")) 
		{
			int iFindPointX = 0,iFindPointY = 0;
			if(m_strMsg.length() == 0)
				return true;

			if(sscanf(m_strMsg.c_str() ,"%d %d",&iFindPointX,&iFindPointY) >= 2)
			{
				if(iFindPointX >= 0 && iFindPointX < g_pGameMap->GetWidth() && iFindPointY >= 0 && iFindPointY < g_pGameMap->GetHeight())
				{
					g_pMinMap->SetFindPoint(iFindPointX,iFindPointY);
				}
				else
				{
					char temp_str[256] = {0};
					sprintf(temp_str,"你要查找的坐标(%d,%d)超出了地图范围，请重新查找！",iFindPointX,iFindPointY);
					g_TalkMgr.PushSysTalk(temp_str);
				}
			}
			else
			{
				g_pMinMap->ClearFindPoint();
			}
			return true;
		}
	}

	return false;
}

bool CAIMsgFilter::FilterRecv(const char* str)
{
	m_strMsg = str;
	StringUtil::trim(m_strMsg);

	if(g_AutoPickMgr.FilterRecv(m_strMsg.c_str()))
		return true;

	int iPos = m_strMsg.find("攻击模式]");
	if(iPos >= 0)
	{
		if(FindHeader("[全体攻击模式]"))
			SELF.SetReserveData(plyAttackType,ATTACK_TYPE_ALL);
		else if(FindHeader("[和平攻击模式]"))
			SELF.SetReserveData(plyAttackType,ATTACK_TYPE_PEACE);
		else if(FindHeader("[编组攻击模式]"))
			SELF.SetReserveData(plyAttackType,ATTACK_TYPE_GROUP);
		else if(FindHeader("[行会攻击模式]"))
			SELF.SetReserveData(plyAttackType,ATTACK_TYPE_GUILD);
		else if(FindHeader("[师徒攻击模式]"))
			SELF.SetReserveData(plyAttackType,ATTACK_TYPE_ST);
		else if(FindHeader("[夫妻攻击模式]"))
			SELF.SetReserveData(plyAttackType,ATTACK_TYPE_WH);
		else if(FindHeader("[善&恶攻击模式]"))
			SELF.SetReserveData(plyAttackType,ATTACK_TYPE_SE);

		g_pControl->Msg(MSG_CTRL_UPDATE_ATTACKMODE,0);

		return false;
	}
	else if(FindHeader("抗魔法力增加"))
	{
		g_AIMgr.SetReserveTime(plyStartGhostTime,GetTickCount());
		g_AIMgr.SetReserveTime(plyDuraGhostTime,atoi(m_strMsg.c_str()));
		return false;
	}
	else if(FindHeader("防御力增加"))
	{
		g_AIMgr.SetReserveTime(plyStartArmorTime,GetTickCount());
		g_AIMgr.SetReserveTime(plyDuraArmorTime,atoi(m_strMsg.c_str()));
		return false;
	}
	else if(FindHeader("抗魔法力恢复正常"))
	{
		g_AIMgr.SetReserveTime(plyStartGhostTime,0);
		return false;
	}
	else if (FindHeader("防御力恢复正常"))
	{
		g_AIMgr.SetReserveTime(plyStartArmorTime,0);
		return false;
	}
	return false;
}

bool CAIMsgFilter::FindHeader(const string& header)
{
	if(m_strMsg.substr(0,header.size()) == header)
	{
		int pos = header.size();
		if(m_strMsg[pos] == ' ')
			pos += 1;

		m_strMsg = m_strMsg.substr(pos,m_strMsg.size()-pos);
		return true;
	}
	return false;
}

void CAIMsgFilter::GetNameOfMessage(const char * strMsg, string& name)
{
	char* pCh2 = const_cast<char*>(strstr(strMsg,"=> "));
	if(pCh2)
	{
		if(strlen(strMsg) >= 4 && strMsg[0] == '<'  && strMsg[3] == '>')
			name.assign(strMsg + 4,pCh2 - strMsg - 4);
		else
			name.assign(strMsg,pCh2 - strMsg);
	}
	else
	{
		pCh2 = const_cast<char*>(strstr(strMsg,": "));
		if(pCh2)
		{
			if(strncmp(strMsg,"(!)",3) == 0)
				name.assign(strMsg+3, pCh2 - strMsg - 3);
			else
				name.assign(strMsg,pCh2 - strMsg);
		}
	}
}

bool CAIMsgFilter::FilterBlack(const char* buf)
{
	string name;
	GetNameOfMessage(buf,name);
	if(name.empty())
		return false;

	if(g_AICfgMgr.IsBlackFilter() && g_BossCfgMgr.IsBlack(name.c_str()))
	{
		return true;
	}
	return false;
}

