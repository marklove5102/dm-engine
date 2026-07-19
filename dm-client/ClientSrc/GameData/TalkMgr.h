#pragma once

#include "ObjectLink/MultiLine.h"
#include "Global/Interface/FontInterface.h"

class CTalkMgr
{
public:
	CTalkMgr(void);
	~CTalkMgr(void);

	//聊天消息
	void PushTalk(DWORD nType,const char * str,WORD color);      //添加简单文字消息
	void PushSysTalk(const char* str,WORD wColor = 0x38FF);  //添加红字消息
	void PushServerTalk(const char* str,WORD wColor = 0x38FF);  //添加系统广播消息
	void PushShoutTalk(const char* buffer,int iLen,WORD color,DWORD id,BYTE cTalkColor,BYTE byFont = FONT_DEFAULT,BYTE byFontSize = FONTSIZE_DEFAULT,DWORD dwType = TALKTYPE_NORMAL);
	CMultiLine* PushDataTalk(DWORD dwType,const char *buf,int iLen,WORD wColor,BYTE byFont = FONT_SONGTI,BYTE byFontSize = FONTSIZE_DEFAULT,bool bIgnoreewLine = true,bool bNeedClearWords = true);//bIgnoreewLine = true,忽略回车换行

	//
	void SetPanelTalkSize(UINT uSize) { m_uPanelTalkSize = uSize; }
	void SetLeftTopTalkSize(UINT uSize) { m_uLeftTopTalkSize = uSize; }


	void  ModifyFilter(DWORD dwFilter,BOOL bAdd);
	DWORD GetFilter(){  return m_dwFilter; }

	TalkType1& GetMultiServerTalk() { return m_MultiServerTalk;}

	TalkType& GetTalk() { return m_CurTalk; }
	TalkType& GetBoothTalk(){ return m_BoothTalk;}
	TalkType& GetPaiMaiTalk(){ return m_PaiMaiTalk;}
	TalkType& GetVoiceTalk(){ return m_VoiceTalk;}
	TalkType& GetAssPrivateTalk(){ return m_AssPrivateTalk;}
	TalkType& GetBuyTalk(){return m_BuyTalk;}
	TalkType& GetGuildTalk(){return m_GuildTalk;}
	TalkType& GetServerTalk(){return m_ServerTalk;}

	void  Clear();
	void  Clear(DWORD dwType);
	void  Refill();

protected:
	CMultiLine* AddTalk(DWORD dwType, const char *buf,int iLen,DWORD dwColor,DWORD dwBack,int iCols = 0,BYTE byFont = FONT_DEFAULT,BYTE byFontSize = FONTSIZE_DEFAULT,bool bIgnoreewLine = false,bool bNeedClearWords = true);
	BOOL AddHeadTalk(UINT uID,const char *str,DWORD dwColor = 0xC0591340,BYTE byFont = FONT_SONGTI,BYTE byFontSize = FONTSIZE_DEFAULT);
	void ClearTalk(TalkType1& Talk);
	void ClearTalk(TalkType& Talk);
	void ReCreateCurTalk();

	TalkType1 m_PanelTalk;
	TalkType1 m_MultiServerTalk;

	TalkType m_BoothTalk;
	TalkType m_BuyTalk;
	TalkType m_PaiMaiTalk;
	TalkType m_AssPrivateTalk;
	TalkType m_VoiceTalk;
	TalkType m_GuildTalk;
	TalkType m_ServerTalk;//系统公告

	TalkType  m_CurTalk;

	UINT	  m_uPanelTalkSize;		// 面板上的聊天纪录大小
	UINT	  m_uLeftTopTalkSize;	// 左上角聊天纪录大小

	DWORD     m_dwFilter;           // 系统过滤器

};

extern CTalkMgr g_TalkMgr;
