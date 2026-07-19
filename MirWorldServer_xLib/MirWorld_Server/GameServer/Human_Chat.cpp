#include "StdAfx.h"
#include "humanplayer.h"
#include "gameworld.h"
#include "humanplayermgr.h"
#include "groupobject.h"
#include "guildex.h"
#include "logicmap.h"
#include "gsclientobj.h"

// ==================== 聊天系统 ====================

VOID CHumanPlayer::SendWisper(const char* pszMsg, ...)
{
	char szBuff[248];
	va_list	vl;
	va_start(vl, pszMsg);
	_vsnprintf(szBuff, sizeof(szBuff) - 1, pszMsg, vl);
	va_end(vl);
	szBuff[120] = '\0';
	SendMsg(0, 0x67, 0xfffc, 0, 1, szBuff);
}

VOID CHumanPlayer::Cry(const char* pszMsg, ...)
{
	char szBuff[248];
	snprintf(szBuff, sizeof(szBuff), "(!)%s: ", GetName());
	va_list	vl;
	va_start(vl, pszMsg);
	size_t nLen = strlen(szBuff);
	_vsnprintf(szBuff + nLen, 247 - nLen, pszMsg, vl);
	va_end(vl);
	szBuff[120] = '\0';
	//	SendMsg( 0, 0x67, 0xfffc, 0, 1, szBuff );
	SendMsg(GetId(), 0x64, 0x9700, 0x38, 0x100, (LPVOID)szBuff);
	SendMapMsg(GetId(), 0x64, 0x9700, 0x38, 0x100, (LPVOID)szBuff);
}

VOID CHumanPlayer::SayGroup(const char* pszMsg, ...)
{
	char szBuff[248];
	va_list	vl;
	va_start(vl, pszMsg);
	_vsnprintf(szBuff, sizeof(szBuff) - 1, pszMsg, vl);
	va_end(vl);
	szBuff[120] = '\0';
	if (m_pGroupObject)
		m_pGroupObject->SendMsg(nullptr, GetId(), 0x68, 0xff2f, 0/*0x38*/, 1, szBuff);
	else
		SaySystem("您还没有加入编组, 无法使用编组聊天模式!");
}

VOID CHumanPlayer::ChangeChatChannel(e_chatchannel channel)
{
	int itemp = (int)m_ChatChannel;
	if (channel == CCH_MAX)
		itemp++;
	else
		itemp = (int)channel;
	if (itemp == CCH_GM)
	{
		if (!IsGameMaster()) itemp++;
	}
	if (itemp >= CCH_MAX) itemp = 0;
	m_ChatChannel = (e_chatchannel)itemp;
	if (m_ChatChannel == CCH_WISPER)
	{
		if (g_pChatChannelDesc[m_ChatChannel])
		{
			SaySystemAttrib(CC_GREEN, "[%s聊天频道 当前密谈对象 %s]", g_pChatChannelDesc[m_ChatChannel], m_szCurWisperTarget[0] == 0 ? "空" : m_szCurWisperTarget.data());
			if (m_szCurWisperTarget[0] == 0)
				SaySystemAttrib(CC_GREEN, "[在密谈频道成功使用 /角色名 密谈一次后, 该角色即被设置成密谈对象]");
		}
		else
			SaySystemAttrib(CC_GREEN, "[%s聊天频道 已关闭]", g_pChatChannelDesc[m_ChatChannel]);
	}
	else
		SaySystemAttrib(CC_GREEN, "[%s聊天频道%s]", g_pChatChannelDesc[m_ChatChannel], g_pChatChannelDesc[m_ChatChannel] ? "" : " 已关闭");
}

VOID CHumanPlayer::DisableChannel(e_chatchannel channel)
{
	if (channel >= CCH_MAX || channel < 0) channel = GetChatChannel();
	if (channel < 0 || channel >= CCH_MAX) return;
	if (IsChannelDisabled(channel))
		SaySystemAttrib(CC_GREEN, "%s 频道已经被关闭", g_pChatChannelDesc[channel]);
	else
	{
		m_bChatChannelDisabled[channel] = TRUE;
		SaySystemAttrib(CC_GREEN, "%s 频道被关闭", g_pChatChannelDesc[channel]);
	}
}

VOID CHumanPlayer::EnableChannel(e_chatchannel channel)
{
	if (channel >= CCH_MAX || channel < 0) channel = GetChatChannel();
	if (channel < 0 || channel >= CCH_MAX) return;
	if (!IsChannelDisabled(channel))
		SaySystemAttrib(CC_GREEN, "%s 频道没有被关闭", g_pChatChannelDesc[channel]);
	else
	{
		m_bChatChannelDisabled[channel] = FALSE;
		SaySystemAttrib(CC_GREEN, "%s 频道被开通", g_pChatChannelDesc[channel]);
	}
}

BOOL CHumanPlayer::IsChannelDisabled(e_chatchannel channel)const
{
	if (channel >= CCH_MAX || channel < 0) channel = GetChatChannel();
	if (channel < 0 || channel >= CCH_MAX) return FALSE;
	return m_bChatChannelDisabled[channel];
}

BOOL CHumanPlayer::IsGameMaster()
{
	return IsSystemFlagSeted(SF_GAMEMASTER);
}

VOID CHumanPlayer::ChannelSay(e_chatchannel channel, const char* pszParam, const char* pszWords/*, ...*/)
{
	if (channel < 0 || channel >= CCH_MAX) channel = GetChatChannel();
	if (channel < 0 || channel >= CCH_MAX) return;
	DWORD dwWaitTime = CGameWorld::GetInstance()->GetChannelWaitTime(channel);
	if (!m_ChatChannelTimer[channel].IsTimeOut(dwWaitTime))
	{
		DWORD dwTime = GetTimeToTime(m_ChatChannelTimer[channel].GetSavedTime(), CFrameTime::GetFrameTime());
		SaySystem("%s频道 %u 秒后才能继续发言!", g_pChatChannelDesc[channel], (dwWaitTime + 999 - dwTime) / 1000);
		return;
	}
	m_ChatChannelTimer[channel].Savetime();

	char szBuff[248];
	o_strncpy(szBuff, pszWords, 120);
	switch (channel)
	{
	case CCH_NORMAL:
	{
		xListHelper<VISIBLE_OBJECT> obj(&this->m_xVisibleObjectList);
		char szText[512];
		snprintf(szText, sizeof(szText), "%s: %s", GetName(), szBuff);
		DWORD dwParam1 = IsSystemFlagSeted(SF_FONTCHANGED) ? GetSystemFlagParam(SF_FONTCHANGED) : 0;
		dwParam1 = (dwParam1 << 8) | m_btChatColor;
		for (VISIBLE_OBJECT* pVo = obj.first(); pVo != nullptr; pVo = obj.next())
		{
			if (pVo->pObject == nullptr || pVo->pObject->GetType() != OBJ_PLAYER)
				continue;
			CHumanPlayer* pObject = (CHumanPlayer*)pVo->pObject;
			if (pObject->IsChannelDisabled(CCH_NORMAL))
				continue;
			pObject->ChannelHearDirectly(CCH_NORMAL, GetId(), szText, dwParam1);
		}
		this->ChannelHearDirectly(CCH_NORMAL, GetId(), szText, dwParam1);
	}
	break;
	case CCH_WISPER:
	{
		char* pszName = (char*)pszParam;
		if (pszName == nullptr)pszName = this->m_szCurWisperTarget.data();
		if (pszName[0] == 0)
		{
			SaySystem("当前密谈对象为空, 无法密谈!");
			return;
		}
		CHumanPlayer* player = CHumanPlayerMgr::GetInstance()->FindbyName(pszName);
		if (player == nullptr)
			SaySystem("%s 目前不在线, 无法密谈!", pszName);
		else
		{
			if (player == this)
				player->SaySystem("你干吗要自言自语呢?");
			else
			{
				if (player->IsChannelDisabled(CCH_WISPER))
					SaySystem("对方关闭了密谈频道, 请稍候再试!");
				else
				{
					player->ChannelHear(CCH_WISPER, GetId(), "%s: %s", GetName(), szBuff);
					SetWisperTarget(pszName);
				}
			}
		}
		return;
	}
	break;
	case CCH_CRY:
	{
		if (m_pMap == nullptr) break;
		xListHelper<CMapObject> objlist;
		m_pMap->GetObjList(objlist);
		char szText[512];
		snprintf(szText, sizeof(szText), "(!)%s: %s", GetName(), szBuff);
		for (CMapObject* pObj = objlist.first(); pObj != nullptr; pObj = objlist.next())
		{
			if (pObj->GetType() == OBJ_PLAYER)
			{
				if (!((CHumanPlayer*)pObj)->IsChannelDisabled(CCH_CRY))
					((CHumanPlayer*)pObj)->ChannelHearDirectly(CCH_CRY, GetId(), szText);
			}
		}
	}
	break;
	case CCH_GM:
	{
	}
	break;
	case CCH_GROUP:
	{
		CGroupObject* pGroupObj = GetGroupObject();
		if (pGroupObj == nullptr)
			SaySystem("没有在编组内, 组队频道发言无效!");
		else
		{
			char szText[512];
			snprintf(szText, sizeof(szText), "_%s: %s", GetName(), szBuff);
			xAutoPtrArray<CHumanPlayer>& array = pGroupObj->GetMemberArray();
			CHumanPlayer* pPlayer = nullptr;
			for (UINT i = 0; i < array.GetCount(); i++)
			{
				pPlayer = array[i];
				if (pPlayer && !pPlayer->IsChannelDisabled(CCH_GROUP))
					pPlayer->ChannelHearDirectly(CCH_GROUP, GetId(), szText);
			}
		}
	}
	break;
	case CCH_GUILD:
	{
		if (m_pGuild)
		{
			if (m_pGuild->IsAllNoSay())
			{
				SaySystem("会长已经禁止全行会聊天!");
				break;
			}
			if (m_pGuild->IsNoSay(GetName()))
			{
				SaySystem("会长已经把你禁止行会聊天!");
				break;
			}
			char szText[512];
			snprintf(szText, sizeof(szText), "%s[%s]: %s", GetName(), m_pGuild->GetName(), szBuff);
			m_pGuild->SendWords(szText);
		}
		else
			SaySystem( "没有加入行会, 行会频道发言无效!" );
	}
	break;
	default:
		SaySystem("系统尚未启动!");
		break;
	}
}

BOOL CHumanPlayer::ChannelHear(e_chatchannel channel, DWORD dwParam, const char* pszWords, ...)
{
	if (channel < 0 || channel >= CCH_MAX) channel = GetChatChannel();
	if (channel < 0 || channel >= CCH_MAX) return FALSE;
	if (m_bChatChannelDisabled[channel])return FALSE; // 被关闭的频道听不到东西
	char szBuff[248];
	va_list	vl;
	va_start(vl, pszWords);
	_vsnprintf(szBuff, sizeof(szBuff) - 1, pszWords, vl);
	va_end(vl);
	szBuff[120] = '\0';
	return ChannelHearDirectly(channel, dwParam, szBuff);
}

BOOL CHumanPlayer::ChannelHearDirectly(e_chatchannel channel, DWORD dwParam, const char* pszWords, DWORD dwParam1, DWORD dwParam2)
{
	if (channel < 0 || channel >= CCH_MAX) channel = GetChatChannel();
	if (channel < 0 || channel >= CCH_MAX) return FALSE;
	WORD wMsg = g_ChatChannelMsg[channel];
	DWORD dwAttrib = g_ChatChannelAttrib[channel];
	WORD wFlag = (channel == CCH_NORMAL);
	if (wFlag)
		wFlag = (dwParam1 & 0xffff);
	SendMsg(dwParam, wMsg, LOWORD(dwAttrib), HIWORD(dwAttrib), wFlag, (LPVOID)pszWords);
	return TRUE;
}
