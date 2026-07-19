#include "ChatPanelWnd.h"
#include "GameData/WooolStoreData.h"
#include "GameAI/AIConfigMgr.h"
#include "GameData/ConfigData.h"
#include "GameData/GameData.h"
#include "GameData/TaskData.h"
#include "GameData/TalkMgr.h"
#include "Baseclass/Control/ParserTip.h"
#include "BaseClass/Control/CtrlMenuWnd.h"
#include "GameControl/GameControl.h"


INIT_WND_POSX(CChatPanelWnd,POS_VARIABLE,POS_VARIABLE)

CChatPanelWnd::CChatPanelWnd(void)
{	
	m_bNoMove = true;
	m_bNoChangeLevel = true;
	m_bNeedSavePos  = false;
	m_bDisableEscape = true;

	m_iIndex = 22078;
	m_iPages = 1;

	m_iAlignType = XAL_BOTTOMLEFT;
	m_iOffX = 26;
	m_iOffY = -124;
	if (g_bNeedScale)
	{
		m_iOffY = (int)(g_ScaleRate.fy * m_iOffY);
	}

	m_pCommandBtn = NULL;
	m_pChangeChannelBtn = NULL;
	m_pChangeSizeBtn = NULL;

	m_pChannelPhyle = NULL;
	m_pChannelNormal = NULL;
	m_pChannelPrivate = NULL;
	m_pChannelPublic = NULL;
	m_pChannelTroop = NULL;
	m_pChannelGuild = NULL;

	m_vTalkFilter.push_back(stTalkFilter("ChannelNormal",TALKTYPE_NORMAL));
	m_vTalkFilter.push_back(stTalkFilter("ChannelPrivate",TALKTYPE_PRIVATE));
	m_vTalkFilter.push_back(stTalkFilter("ChannelPublic",TALKTYPE_PUBLIC));
	m_vTalkFilter.push_back(stTalkFilter("ChannelTroop",TALKTYPE_TROOP));
	m_vTalkFilter.push_back(stTalkFilter("ChannelGuild",TALKTYPE_GUILD));
	m_vTalkFilter.push_back(stTalkFilter("ChannelPhyle",TALKTYPE_PHYLE));
}

CChatPanelWnd::~CChatPanelWnd(void)
{
}

void CChatPanelWnd::OnCreate()
{
	m_pCommandBtn = new CCtrlButton();
	AddControl(m_pCommandBtn);
	m_pCommandBtn->CreateEx(this, 249, 1, 22070,22071,22072,0);	

	m_pChangeChannelBtn = new CCtrlButton();
	AddControl(m_pChangeChannelBtn);
	m_pChangeChannelBtn->CreateEx(this, 1, 1, 9300,9301,9302,0);

	m_pChangeSizeBtn = new CCtrlButton();
	AddControl(m_pChangeSizeBtn);
	m_pChangeSizeBtn->CreateEx(this, 246, 21, 22074,22075,22076,0);

	m_pChannelPhyle = new CCtrlRadio();
	AddControl(m_pChannelPhyle);
	m_pChannelPhyle->Create(this,2,22,22082,22080,0,0,40,17);
	m_pChannelPhyle->SetName("ChannelPhyle");
	m_pChannelPhyle->SetText("宗 族",0xffdc783c,0xffdc783c,0xffdc783c,0xffdc783c,FONTSIZE_SMALL,0,FONT_DEFAULT,-36);
	m_pChannelPhyle->SetTips("点击按钮停止在聊天框显示宗派聊天");

	m_pChannelNormal = new CCtrlRadio();
	AddControl(m_pChannelNormal);
	m_pChannelNormal->Create(this,2 + 41,22,22082,22080,0,0,40,17);
	m_pChannelNormal->SetName("ChannelNormal");
	m_pChannelNormal->SetText("普 通",0xffdc783c,0xffdc783c,0xffdc783c,0xffdc783c,FONTSIZE_SMALL,0,FONT_DEFAULT,-36);
	m_pChannelNormal->SetTips("点击按钮停止在聊天框显示普通聊天");

	m_pChannelPrivate = new CCtrlRadio();
	AddControl(m_pChannelPrivate);
	m_pChannelPrivate->Create(this,2 + 41 * 2,22,22082,22080,0,0,40,17);
	m_pChannelPrivate->SetName("ChannelPrivate");
	m_pChannelPrivate->SetText("私 聊",0xffdc783c,0xffdc783c,0xffdc783c,0xffdc783c,FONTSIZE_SMALL,0,FONT_DEFAULT,-36);
	m_pChannelPrivate->SetTips("点击按钮停止在聊天框显示私聊");

	m_pChannelPublic = new CCtrlRadio();
	AddControl(m_pChannelPublic);
	m_pChannelPublic->Create(this,2 + 41 * 3,22,22082,22080,0,0,40,17);
	m_pChannelPublic->SetName("ChannelPublic");
	m_pChannelPublic->SetText("喊 话",0xffdc783c,0xffdc783c,0xffdc783c,0xffdc783c,FONTSIZE_SMALL,0,FONT_DEFAULT,-36);
	m_pChannelPublic->SetTips("点击按钮停止在聊天框显示喊话");

	m_pChannelTroop = new CCtrlRadio();
	AddControl(m_pChannelTroop);
	m_pChannelTroop->Create(this,2 + 41 * 4,22,22082,22080,0,0,40,17);
	m_pChannelTroop->SetName("ChannelTroop");
	m_pChannelTroop->SetText("组 队",0xffdc783c,0xffdc783c,0xffdc783c,0xffdc783c,FONTSIZE_SMALL,0,FONT_DEFAULT,-36);
	m_pChannelTroop->SetTips("点击按钮停止在聊天框显示组队聊天");

	m_pChannelGuild = new CCtrlRadio();
	AddControl(m_pChannelGuild);
	m_pChannelGuild->Create(this,2 + 41 * 5,22,22082,22080,0,0,40,17);
	m_pChannelGuild->SetName("ChannelGuild");
	m_pChannelGuild->SetText("行 会",0xffdc783c,0xffdc783c,0xffdc783c,0xffdc783c,FONTSIZE_SMALL,0,FONT_DEFAULT,-36);
	m_pChannelGuild->SetTips("点击按钮停止在聊天框显示行会聊天");


	DWORD dwFilter = g_TalkMgr.GetFilter();
	for(int ii = 0;ii < (int)m_vTalkFilter.size();ii++)
	{
		CCtrlRadio* pRadio = this->FindRadioByName(m_vTalkFilter[ii].str.c_str());
		if(pRadio)
		{
			pRadio->SetChecked((dwFilter & m_vTalkFilter[ii].filter) != 0);
		}
	}

	g_TalkMgr.Refill();
}

bool CChatPanelWnd::Msg(DWORD dwMsg, DWORD dwData, CControl *pControl)
{
	switch ( dwMsg )
	{	
	case MSG_CTRL_BUTTON_CLICK:
		{
			if (pControl == m_pCommandBtn)
			{
				g_pControl->PopupWindow("ComandWnd",OPER_UPDATE);
				return true;
			}
			else if (pControl == m_pChangeChannelBtn)
			{
				OnChangeChannel(pControl);
				return true;
			}
			else if (pControl == m_pChangeSizeBtn)
			{
				g_pControl->Msg(MSG_CTRL_CHANGE_TALKVIEWSIZE,0);				
				return true;
			}

			for(int ii = 0; ii < (int)m_vTalkFilter.size();ii++)
			{
				if(stricmp(pControl->GetName(),m_vTalkFilter[ii].str.c_str()) == 0)
				{
					CCtrlRadio* pRadio = dynamic_cast<CCtrlRadio*>(pControl);
					if(pRadio)
					{
						string strTips = pRadio->GetTips();
						if(pRadio->IsChecked())
							strTips.replace(8,4,"停止");
						else
							strTips.replace(8,4,"开始");


						pRadio->SetTips(strTips.c_str());

						g_TalkMgr.ModifyFilter(m_vTalkFilter[ii].filter,pRadio->IsChecked());
						//完成第一次发言任务
						if (ii == 2 && g_pGameData->HasPaoPaoStatus(EP_First_FaYan_Step3) && !g_pGameData->HasPaoPaoStatus(EP_First_FaYan))
						{
							g_pControl->MsgToWnd(MSG_CTRL_TALKVIEW_WND,MSG_REMOVE_ARROWTIP_CONTROL,EP_First_FaYan_Step3,NULL);
							g_pGameControl->Send_Player_Prompt_Status(EP_First_FaYan);//完成这个任务
						}

					}
					return true;
				}
			}
		}
		break;
	case MSG_CTRL_POP_MENU:
		{
			if(dwData >= POP_MENU_ITEM_CHANNEL_COMMON && dwData <= POP_MENU_ITEM_CHANNEL_VOICE)
			{
				CCtrlButton * pBtn = m_pChangeChannelBtn;
				if(pBtn)
				{
					eSendTalkChannel eSendTalkChannelType = (eSendTalkChannel)(dwData - POP_MENU_ITEM_CHANNEL_COMMON);
					g_pGameData->SetSendTalkChannelType(eSendTalkChannelType);
					pBtn->ReloadTex(9300 + eSendTalkChannelType*4,9301 + eSendTalkChannelType*4,9302 + eSendTalkChannelType*4);
				}

				if (dwData == POP_MENU_ITEM_CHANNEL_SHOUT && g_pGameData->HasPaoPaoStatus(EP_First_FaYan_Step1) && !g_pGameData->HasPaoPaoStatus(EP_First_FaYan_Step2))
				{
					g_pControl->MsgToWnd(MSG_CTRL_TALKVIEW_WND,MSG_REMOVE_ARROWTIP_CONTROL,EP_First_FaYan_Step1,NULL);
					g_pControl->PopupArrowTip(MSG_CTRL_TALKVIEW_WND,EP_First_FaYan_Step2,30,149,XAL_TOPLEFT,false,XAL_BOTTOMRIGHT);
				}
			}

			return true;
		}
		break;
	default:
		break;
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CChatPanelWnd::Draw(void)
{
	CCtrlWindowX::Draw();	
	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

	
}

void CChatPanelWnd::OnChangeChannel(CControl * pControl)
{
	CCtrlMenuWnd* pMenuWnd = g_pControl->GetMenuWnd();
	pMenuWnd->Clear();

	S_POP_MENU_DATA & PopMenuData = pMenuWnd->GetData();
	PopMenuData.wBackTex = 9324;
	PopMenuData.fItemHeight = 15.3f;
	PopMenuData.iLeftOffX = 27;
	PopMenuData.iRightOffX = 4;
	PopMenuData.iTopOffY = PopMenuData.iBottomOffY = 3;
	PopMenuData.fItemHeight = 17.0f;
	PopMenuData.iFontType = FONT_YAHEI;
	PopMenuData.dwBackColor = 0xFFFFFFFF;
	PopMenuData.dwItemTextColor = 0xffb46428;
	PopMenuData.dwHoverTextColor = 0xffffe650;
	PopMenuData.dwSelectColor = 0xff1e6464;
	PopMenuData.iSelBackColorLeftOffX = 25;


	pMenuWnd->AddMenuItem(POP_MENU_ITEM_CHANNEL_COMMON,"普通聊天频道");
	pMenuWnd->AddMenuItem(POP_MENU_ITEM_CHANNEL_TEAM,"组队频道");
	pMenuWnd->AddMenuItem(POP_MENU_ITEM_CHANNEL_SHOUT,"喊话频道");
	pMenuWnd->AddMenuItem(POP_MENU_ITEM_CHANNEL_PHYLE,"宗族频道");
	pMenuWnd->AddMenuItem(POP_MENU_ITEM_CHANNEL_GUILD,"行会频道");
	pMenuWnd->AddMenuItem(POP_MENU_ITEM_CHANNEL_VOICE,"语聊频道");

	//PopMenuData.iW = PopMenuData.iFontSize*12/2 + PopMenuData.iLeftOffX + PopMenuData.iRightOffX;
	//PopMenuData.iH = (int)( PopMenuData.fItemHeight*pMenuWnd->GetMenuItemVector().size()) + PopMenuData.iTopOffY + PopMenuData.iBottomOffY;
	PopMenuData.iW = 105;
	PopMenuData.iH = 104;

	PopMenuData.iX = pControl->GetScreenX()+1;
	PopMenuData.iY = pControl->GetScreenY() - PopMenuData.iH - 1;

	m_bClick = false;
	m_bRBClick = false;//否则MenuWnd获得不到焦点
	pMenuWnd->SetCaller(this);
	pMenuWnd->ShowMenu();
	g_pControl->GetTipWnd()->SetShow(false);
}
