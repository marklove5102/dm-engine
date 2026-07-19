#include "TalkMgr.h"
#include "GameData.h"
#include "DirtyWords.h"
#include "Global/Interface/GraphicInterface.h"
#include "GameData/ConfigData.h"

CTalkMgr g_TalkMgr;

CTalkMgr::CTalkMgr(void)
{
	m_dwFilter = TALKTYPE_ALL & ~(TALKTYPE_SERVER);
	//m_bShowSystemLeft = false;
}

CTalkMgr::~CTalkMgr(void)
{
	Clear();
}

void CTalkMgr::Clear()
{
	m_CurTalk.clear();

	ClearTalk(m_PanelTalk);
	//ClearTalk(m_SystemTalk);
	ClearTalk(m_ServerTalk);
	ClearTalk(m_MultiServerTalk);
	//ClearTalk(m_MultiSystemTalk);

	ClearTalk(m_BoothTalk);
	ClearTalk(m_BuyTalk);
	ClearTalk(m_PaiMaiTalk);
	ClearTalk(m_AssPrivateTalk);
	ClearTalk(m_VoiceTalk);
	ClearTalk(m_GuildTalk);
}

void CTalkMgr::Clear(DWORD dwType)
{
	if (dwType & TALKTYPE_BOOTH)
		ClearTalk(m_BoothTalk);
	else if (dwType & TALKTYPE_BUY)
		ClearTalk(m_BuyTalk);
	else if (dwType & TALKTYPE_PAIMAI)
		ClearTalk(m_PaiMaiTalk);
	else if(dwType & TALKTYPE_VOICE)
		ClearTalk(m_VoiceTalk);
	else if(dwType & TALKTYPE_ASSPRIVATE)
		ClearTalk(m_AssPrivateTalk);
	else if(dwType & TALKTYPE_MGUILD)
		ClearTalk(m_GuildTalk);
	//else if(dwType & TALKTYPE_SYSTEM)
	//	ClearTalk(m_SystemTalk);
	else if(dwType & TALKTYPE_SERVER)
	{
		ClearTalk(m_ServerTalk);
		ClearTalk(m_MultiServerTalk);
	}
}

void CTalkMgr::ModifyFilter(DWORD dwFilter,BOOL bAdd)
{
	if(bAdd)
		m_dwFilter |= dwFilter;
	else
		m_dwFilter &= ~dwFilter;

	//重新生成CurTalk
	ReCreateCurTalk();
}

void CTalkMgr::ReCreateCurTalk()
{
	m_CurTalk.clear();
	TalkType1::iterator itr;
	for(itr = m_PanelTalk.begin(); itr!= m_PanelTalk.end();itr++)
	{
		CMultiLine* pLine = *itr;
		if(pLine == NULL) continue;

		if(m_dwFilter & pLine->getType()) //通过的频道
		{
			for(int ii = 0; ii < pLine->getLineCount();ii++)
			{
				m_CurTalk.push_back(pLine->getLine(ii));
			}
		}
	}
}

CMultiLine*  CTalkMgr::AddTalk(DWORD dwType,const char *buf,int iLen,DWORD dwColor,DWORD dwBack,int iCols,BYTE byFont,BYTE byFontSize,bool bIgnoreewLine,bool bNeedClearWords)
{
	if(!g_Config.IsShowTalk())
		return NULL;

	CMultiLine* pCurLine = new CMultiLine(dwColor,dwBack,dwType,iCols,byFont,byFontSize,bIgnoreewLine);
	pCurLine->handleBuffer(buf,iLen);
	CMultiLine* pCurLine2 = NULL;
	
	if(bNeedClearWords && ((dwType != TALKTYPE_SYSTEM && dwType != TALKTYPE_SERVER) || (0xFFFFFFFF == dwColor && dwBack == 0xFFEF6B00)))//传音号角的颜色
	{
		string strPlayerName;
		strPlayerName.assign(SELF.GetName());
		if(!g_DirtyWords.ParseNameIsGM(strPlayerName))
		{
			//脏话过滤
			if(g_DirtyWords.ClearWords1(pCurLine))
			{
				SAFE_DELETE(pCurLine);
				return NULL;
			}
			g_DirtyWords.ClearWords(pCurLine);
		}
	}

	TalkType* TypeTmp = NULL;
	TalkType1* Type1Tmp = NULL;

	if (dwType & TALKTYPE_BOOTH)
		TypeTmp = &m_BoothTalk;
	else if (dwType & TALKTYPE_BUY)
		TypeTmp = &m_BuyTalk;
	else if (dwType & TALKTYPE_PAIMAI)
		TypeTmp =  &m_PaiMaiTalk;
	else if(dwType & TALKTYPE_VOICE)
		TypeTmp =  &m_VoiceTalk;
	else if(dwType & TALKTYPE_ASSPRIVATE)
		TypeTmp =  &m_AssPrivateTalk;
	else if(dwType & TALKTYPE_MGUILD)
		TypeTmp =  &m_GuildTalk;
	else if(dwType & TALKTYPE_SERVER)
	{
		TypeTmp =  &m_ServerTalk;
		Type1Tmp =  &m_MultiServerTalk;
		pCurLine2 = pCurLine;
	}
	else
	{
		Type1Tmp =  &m_PanelTalk;
		pCurLine2 = pCurLine;
	}

	//超过行数就清除
	if (TypeTmp)
	{
		if(TypeTmp->size() > TALKCONTENT_BUFFER)
		{
			for(int i = 0;i< pCurLine->getLineCount();i++)
			{
				if(TypeTmp->size() > 0)
				{
					CStringLine* pTemp = TypeTmp->front();
					SAFE_DELETE(pTemp);
					TypeTmp->pop_front();
				}
			}

			for(int i = 0;i< 70;i++)//为了CTalkViewer的WheelToEnd能正确滚动，再删除70行
			{
				if(TypeTmp->size() > 0)
				{
					CStringLine* pTemp = TypeTmp->front();
					SAFE_DELETE(pTemp);
					TypeTmp->pop_front();
				}
				else
				{
					break;
				}
			}
		}

		for (int ii = 0; ii < pCurLine->getLineCount();ii++)
		{
			CStringLine* pLine = new CStringLine();
			pLine->assign(*pCurLine->getLine(ii));
			TypeTmp->push_back(pLine);
		}

		if(pCurLine2 != pCurLine)
			SAFE_DELETE(pCurLine);
	}
	
	if(Type1Tmp && pCurLine2)
	{
		int iMaxSize = TALKCONTENT_BUFFER;
		if(dwType & TALKTYPE_SERVER)
			iMaxSize = 10;
		//else if((dwType & TALKTYPE_SYSTEM) && m_bShowSystemLeft)
		//	iMaxSize = 5;

		if(Type1Tmp->size() > iMaxSize)
		{
			int iDelLines = 1;
			if(iMaxSize == TALKCONTENT_BUFFER)
				iDelLines = 50;//为了CTalkViewer的WheelToEnd能正确滚动，再删除50行

			for(int i = 0; i < iDelLines; i++)
			{
				CMultiLine* pTemp = Type1Tmp->front();
				if(pTemp)
				{
					for(int ii = 0; ii < pTemp->getLineCount();ii++)
					{
						if(m_CurTalk.size() > 0 && pTemp->getLine(ii) == m_CurTalk.front())
						{
							m_CurTalk.pop_front();
						}
					}
					SAFE_DELETE(pTemp);
				}

				Type1Tmp->pop_front();
			}
		}
		Type1Tmp->push_back(pCurLine2);
	}

	return pCurLine2;
}

BOOL CTalkMgr::AddHeadTalk(UINT uID,const char *str,DWORD dwColor,BYTE byFont,BYTE byFontSize)
{
	if(str == NULL || uID == 0)
		return false;

	//处理头顶文字
	CSimpleCharacter* pChar = NULL;

	if(uID == SELF.GetID())
	{
		SELF.SetTalk(str,dwColor,byFont,byFontSize);
	}
	else
	{
		pChar = g_pGameData->FindSimpleCharacter(uID);
		if(pChar == NULL)
			return false;

		pChar->SetTalk(str,dwColor,byFont,byFontSize);
	}
	return TRUE;
}

void CTalkMgr::ClearTalk(TalkType1 &Talk)
{
	while(!Talk.empty())
	{
		CMultiLine* pLine = Talk.back();
		SAFE_DELETE(pLine);
		Talk.pop_back();
	}
}

void CTalkMgr::ClearTalk(TalkType& Talk)
{
	while(!Talk.empty())
	{
		CStringLine* pLine = Talk.back();
		SAFE_DELETE(pLine);
		Talk.pop_back();
	}
}

void CTalkMgr::Refill()
{
	TalkType1::iterator itr;
	for(itr = m_PanelTalk.begin();itr != m_PanelTalk.end();itr++)
	{
		CMultiLine* pLine = *itr;
		if(pLine == NULL) continue;

		string buf;
		int iLen = 0;
		for(int ii = 0;ii < pLine->getLineCount();ii++)
		{
			CStringLine* pStr = pLine->getLine(ii);
			if(pStr == NULL)
				continue;

			buf.append(pStr->getBuf());
			iLen += pStr->getBufLength();
		}
		pLine->clear();

		//int iWidth = g_pGfx->GetWidth();
		//if (g_EutUiType == EUT_FASHION)
		{
			pLine->setLimit(42);
		}
		//else if (iWidth == 800 || g_bNeedScale)
		//{
		//	pLine->setLimit((int)(74*g_ScaleRate.fx));
		//}
		//else if (iWidth == 1024)
		//{
		//	pLine->setLimit(112);
		//}
		//else if (iWidth == 1280)
		//{
		//	pLine->setLimit(154);
		//}

		pLine->handleBuffer(buf.c_str(),iLen);
	}
	ModifyFilter(0,TRUE);
}

CMultiLine*  CTalkMgr::PushDataTalk(DWORD dwType,const char *buf,int iLen,WORD wColor,BYTE byFont,BYTE byFontSize,bool bIgnoreewLine,bool bNeedClearWords)
{
	if(!buf || iLen == 0 || dwType == 0)
	{
		return NULL;
	}

	string strMsg;
	strMsg.assign(buf,iLen);

	//int iWidth = g_pGfx->GetWidth();
	int iCols = 42; //加入聊天频道

	//if (iWidth == 800 || g_bNeedScale)
	//{
	//	iCols =  (int)(74*g_ScaleRate.fx);
	//}
	//else if (iWidth == 1024)
	//{
	//	iCols = (int)(110*g_ScaleRate.fx);
	//}
	//else if (iWidth == 1280)
	//{
	//	iCols = 131;
	//}


	BYTE byColor = BYTE(wColor);
	BYTE byBack = wColor >> 8;

	DWORD dwColor = g_pGameData->GetMirColor(byColor);
	DWORD dwBack = g_pGameData->GetMirColor(byBack);

	if(dwType & TALKTYPE_ASSPRIVATE)
	{
		iCols = 63;
	}
	else if(dwType & TALKTYPE_SERVER)
	{
		iCols = 64;
		dwColor = 0xFFFFFF00;
		dwBack = 0;
		byFontSize = FONTSIZE_MIDDLE;		
	}
	else if(dwType & TALKTYPE_VOICE)
	{
		iCols = 34;
	}
	else if(dwType & TALKTYPE_PAIMAI)
	{
		iCols = 30;
	}
	else if(dwType & (TALKTYPE_BOOTH | TALKTYPE_BUY))
	{
		iCols = 32;
	}
	else//加入到聊天主面板里的消息
	{
		if((m_dwFilter & dwType) == 0)
			return NULL;

		//if (g_EutUiType == EUT_FASHION)
		//{
		//	iCols = 42;
		//}

		if(dwType & TALKTYPE_GUILD)
		{
			AddTalk(TALKTYPE_MGUILD,strMsg.c_str(),iLen,dwColor,dwBack,52,byFont,byFontSize,false,bNeedClearWords); //同时加入行会界面的聊天窗口
		}
	}


	CMultiLine* pCurLine = AddTalk(dwType,strMsg.c_str(),iLen,dwColor,dwBack,iCols,byFont,byFontSize,bIgnoreewLine,bNeedClearWords);
	if(!pCurLine)
		return NULL;

	if(m_dwFilter & dwType) //要求加入到聊天主面板里的频道
	{
		for(int ii = 0; ii < pCurLine->getLineCount();ii++)
		{
			m_CurTalk.push_back(pCurLine->getLine(ii));
		}
	}

	return pCurLine;
}

//喊话等消息处理
void CTalkMgr::PushSysTalk(const char* str,WORD color)
{
	PushDataTalk(TALKTYPE_SYSTEM,str,strlen(str),color);
}

void CTalkMgr::PushServerTalk(const char* str,WORD color)
{
	PushDataTalk(TALKTYPE_SERVER,str,strlen(str),color,FONT_DEFAULT,FONTSIZE_MIDDLE);
}

void CTalkMgr::PushTalk(DWORD nType,const char * str,WORD color)      //添加文字消息
{
	PushDataTalk(nType,str,strlen(str),color);
}

void CTalkMgr::PushShoutTalk(const char* buf,int iLen,WORD color,DWORD id,BYTE cTalkColor,BYTE byFont,BYTE byFontSize,DWORD dwType)
{
	CMultiLine* pCurLine = PushDataTalk(dwType,buf,iLen,color);

	if(pCurLine && (m_dwFilter & dwType))
	{
		DWORD dwTalkColor = g_pGameData->GetMirColor(cTalkColor);
		AddHeadTalk(id,pCurLine->c_str(),dwTalkColor,byFont,byFontSize);
	}
}

// void CTalkMgr::RemoveSysTalkToLeft()
// {
// 	ClearTalk(m_MultiSystemTalk);
// 	Clear(TALKTYPE_SYSTEM);//先清除m_SystemTalk,再从m_PanelTalk移动5行CMultiLine到m_MultiSystemTalk,再重新生新m_CurTalk
// 	int iSize = m_PanelTalk.size();
// 	int iLins = 5;
// 	if(iLins > iSize)
// 		iLins = iSize;
// 
// 	for(int i = iSize - iLins; i < iSize; iSize --)
// 	{
// 		CMultiLine* pMultiLine = m_PanelTalk.at(i);
// 		if(pMultiLine == NULL || (pMultiLine->getType() & TALKTYPE_SYSTEM) == 0)
// 			continue;
// 
// 		DWORD dwColor = pMultiLine->getColor();
// 		DWORD dwBack = pMultiLine->getBack();
// 
// 		string str = pMultiLine->c_str();
// 		AddTalk(TALKTYPE_SYSTEM,str.c_str(),str.length(),dwColor,dwBack,28,pMultiLine->getFont(),pMultiLine->getFontSize());
// 		//删除
// 		SAFE_DELETE(pMultiLine);
// 		m_PanelTalk.erase(m_PanelTalk.begin() + i);
// 	}
// 
// 	//重新生成CurTalk
// 	ReCreateCurTalk();
// }

// void CTalkMgr::RemoveSysTalkToPanel()
// {
// 	int iSize = m_MultiSystemTalk.size();
// 	for(int i = 0; i < iSize; i++)
// 	{
// 		CMultiLine* pMultiLine = m_MultiSystemTalk.at(i);
// 		if(pMultiLine == NULL)
// 			continue;
// 
// 		m_PanelTalk.push_back(pMultiLine);
// 	}
// 
// 	//删除
// 	m_MultiSystemTalk.clear();
// 
// 	//重新生成CurTalk
// 	ReCreateCurTalk();
// }

