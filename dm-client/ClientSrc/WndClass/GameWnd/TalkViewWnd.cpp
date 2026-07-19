#include "TalkViewWnd.h"
#include "Global/Global.h"
#include "GameData/GameData.h"
#include "GameControl/GameControl.h"
#include "BaseClass/Control/CtrlButton.h"
#include "GameData/MsgBoxMgr.h"
#include "GameData/TalkMgr.h"
#include "GameData/OtherData.h"
#include "GameData/TalkName.h"
#include "GameData/WooolStoreData.h"
#include "Global/Interface/StreamInterface.h"
#include "GameAI/AIBossCfgMgr.h"
#include "GameClient/SDOAInterface.h"
#include "GameAI/AIAutoMgr.h"
#include "GameClient/VoiceAdapter.h"
#include "GameData/TaskData.h"
#include "Global/Interface/AudioInterface.h"
#include "GameAI/AIGoodMgr.h"

INIT_WND_POSX(CTalkViewWnd,POS_VARIABLE,POS_VARIABLE)

const char *szSendTalkChannelHeader[ETC_NUM] = {"","!!","!","$","!~","*"};//不同频道加上的头

CTalkViewWnd::CTalkViewWnd(void)
{
	m_bNoMove = true;
	m_bNoChangeLevel = true;
	m_bNeedSavePage = false;
	m_bNeedSavePos  = false;
	m_bDisableEscape = true;

	m_pTalkViewer = NULL;
	m_pPanelEdit = NULL;
	//m_bScale = g_bNeedScale;
	m_bScaleWidthAndHeight = false;

	//if (g_pGameData && g_EutUiType == EUT_CLASSIC)
	//{
	//	m_iAlignType = XAL_BOTTOM;
	//	m_iWidth = g_pGfx->GetWidth();
	//	m_iHeight = (int)(185*g_ScaleRate.fy+0.5f);
	//}
	//else
	{
		m_iAlignType = XAL_BOTTOMLEFT;
		m_iWidth = 273;
		m_iHeight = 144 + 20;
		m_iOffX = 26;
		m_iOffY = -144;
		if (g_bNeedScale)
		{
			m_iOffY = (int)(g_ScaleRate.fy * m_iOffY);
		}
	}
}

CTalkViewWnd::~CTalkViewWnd(void)
{
}

void CTalkViewWnd::OnCreate()
{
	m_pTalkViewer = new CTalkViewer();
	AddControl(m_pTalkViewer);
	//聊天框
	m_pPanelEdit = new CGlyphEdit();
	AddControl(m_pPanelEdit);
	//if (g_EutUiType == EUT_FASHION)
	{	
		m_pTalkViewer->Create(this,0,1,270,142);
		m_pTalkViewer->AddScroll(0,17,10,111,22101,XAL_TOPLEFT);
		m_pTalkViewer->AddUpButton(0,0,22104,22105,22106,XAL_TOPLEFT);
		m_pTalkViewer->AddDownButton(0,144 - 16,22107,22108,22109,XAL_TOPLEFT);
		//m_pTalkViewer->AddSwitchButton(0,6,0,17557,17558,XAL_TOPRIGHT);
		m_pTalkViewer->SetTalkType(&g_TalkMgr.GetTalk());
		//m_pTalkViewer->SetBigTalkBack(13880);
		m_pTalkViewer->SetDrawOffXY(18,1);

		m_pPanelEdit->Create(this,FONTSIZE_DEFAULT,COLOR_DEFAULT,25,148,273 - 25 - 28,12);
		m_pPanelEdit->SetMaxLength(120);
		m_pPanelEdit->DisableFocus();
	}
	//else
	//{
	//	m_pTalkViewer->Create(this,175,93,464,70);
	//	m_pTalkViewer->AddScroll(0,17,10,48,17555,XAL_TOPRIGHT);
	//	m_pTalkViewer->AddSwitchButton(0,6,0,17557,17558,XAL_TOPRIGHT);
	//	m_pTalkViewer->SetTalkType(&g_TalkMgr.GetTalk());
	//	m_pTalkViewer->SetBigTalkBack(13880);
	//	m_pTalkViewer->SetDrawOffXY(1,1);	

	//	m_pPanelEdit->SetAlignType(XAL_BOTTOMLEFT);
	//	m_pPanelEdit->SetScaleWidthAndHeight(false);//输入框不要缩放,不然的话文字就看不清了
	//	m_pPanelEdit->Create(this,FONTSIZE_DEFAULT,COLOR_DEFAULT,196,-6,(int)(380*g_ScaleRate.fx + 0.50f),12);
	//	m_pPanelEdit->SetMaxLength(200);
	//	m_pPanelEdit->DisableFocus();
	//}


	CCtrlWindowX::OnCreate();
	//ResetControlPos();

}

void CTalkViewWnd::Draw()
{
	if (/*g_EutUiType == EUT_FASHION && */m_pTalkViewer->IsShow())
	{	
		g_pGfx->DrawTextureNL(m_iScreenX,m_iScreenY,g_pTexMgr->GetTex(PACKAGE_INTERFACE,22100,EP_UI));
	}

	CCtrlWindowX::Draw();

	//背景图片还没有下载下来,其它的东西不画
	if (m_iOriginalWidth <= 0 || m_iOriginalHeight <= 0 || m_iWidth <= 0 || m_iHeight <= 0)
	{
		return;
	}

}

//如果不在自己的子控制不接受消息
bool CTalkViewWnd::IsInControl(int iX,int iY)
{
	CControl * p = m_pControls;
	while(p) 
	{
		if (p->IsShow() && iX >= p->GetX() && iY >= p->GetY() && iX < p->GetX() + p->GetWidth() && iY < p->GetY() + p->GetHeight())
		{
			return true;
		}

		p = p->GetControlNext(); 
	}

	return false;
}

bool CTalkViewWnd::OnWheel(int iWheel)
{
	if(m_pTalkViewer && m_pTalkViewer->OnWheel(iWheel))
		return true;

	return CCtrlWindowX::OnWheel(iWheel);
}

void CTalkViewWnd::ChangeTalkAutoWheel()
{
	if (m_pTalkViewer)
	{
		m_pTalkViewer->SetAutoWheel(!m_pTalkViewer->IsAutoWheel());
	}
}

//void CTalkViewWnd::SetControlState()
//{
//	if (g_EutUiType == EUT_CLASSIC)
//	{
//		int iW = g_pGfx->GetWidth();
//		if (m_pTalkViewer)
//		{
//			if(m_pTalkViewer->IsBigTalkVier())
//				m_pTalkViewer->SwitchTalkViewer();
//
//			if (iW == 800 || g_bNeedScale)
//			{
//				m_pTalkViewer->SetOriginalWidth(465);
//				m_pTalkViewer->SetWidth(465);
//				m_pTalkViewer->SetBigTalkBack(13880);
//				m_pPanelEdit->SetOriginalWidth((int)(390*g_ScaleRate.fx + 0.50f));
//				m_pPanelEdit->SetWidth((int)(390*g_ScaleRate.fx + 0.50f));
//			}
//			else if (iW == 1024)
//			{
//				m_pTalkViewer->SetOriginalWidth(689);
//				m_pTalkViewer->SetWidth(689);
//				m_pTalkViewer->SetBigTalkBack(13881);
//				m_pPanelEdit->SetOriginalWidth(626);
//				m_pPanelEdit->SetWidth(626);
//			}
//			else if (iW == 1280)
//			{
//				m_pTalkViewer->SetOriginalWidth(945);
//				m_pTalkViewer->SetWidth(945);
//				m_pTalkViewer->SetBigTalkBack(14721);
//				m_pPanelEdit->SetOriginalWidth(873);
//				m_pPanelEdit->SetWidth(873);
//			}
//		}
//	}
//}

bool CTalkViewWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch(dwMsg)
	{
	case MSG_CTRL_CHANGE_TALKVIEWSIZE:
		{
			if (m_pTalkViewer->IsShow())
			{
				m_pTalkViewer->SetShow(false);
				m_pTalkViewer->SetEnable(false);
			}
			else
			{
				m_pTalkViewer->SetShow(true);
				m_pTalkViewer->SetEnable(true);
			}

			return true;
		}
		break;
	case MSG_CTRL_TALKVIEW_LCLICK:
		{
			if(!OnClickTalkView())
			{
				string strLine;
				if(m_pTalkViewer && m_pTalkViewer->getLineHeader(strLine))
				{
					m_pPanelEdit->SetText(strLine.c_str());
					m_pPanelEdit->SetFocus();
				}
			}

			break;
		}
	case MSG_CTRL_EDIT_ENTRY:
		{
			if(pControl == m_pPanelEdit)
			{
				CStringLine* pLine = m_pPanelEdit->getLine();
				if(pLine == NULL || pLine->getBufLength() == 0)
				{
					if(!g_pControl->SetTopWindowFocus())
					{
						this->SetFocus();
					}
					m_pPanelEdit->SetFakeFocus(false);
					return true;
				}

				char c = *pLine->c_str();

				if(c == '/')
				{
					const char* str = pLine->c_str();
					if(g_pSDOAInterface && pLine->getBufLength() > 1 && str[1] == '/')
					{
						if (pLine->getBufLength() == 2)
						{
							g_pSDOAInterface->ExecuteWidget("","help");
						}
						else
						{
							g_pSDOAInterface->ExecuteWidget("",str + 2);
						}
					}
					else
					{
						g_pGameControl->SEND_Message_Private(pLine);
					}
				}
#ifdef _CHAT
				else if (c == '*')
				{
					if (g_VoiceAdapter.IsInRoom())
					{
						string szTemp = pLine->c_str();
						szTemp = szTemp.substr(1,szTemp.length()-1);
						g_VoiceAdapter.SendTextMessage(szTemp.c_str(),szTemp.length() + 1,NULL);
					}
					else
					{
						g_TalkMgr.PushSysTalk("您当前没有加入语聊频道，无法使用语聊频道");
						return true;
					}
				}
#endif
				else if(g_AIAutoMgr.IsEnalbeWaiGua() && c =='%')
				{
					string szTemp = pLine->c_str();
					szTemp = szTemp.substr(1,szTemp.length()-1);
					g_BossCfgMgr.AddBlackName(const_cast<char*>(szTemp.c_str()));
					g_pControl->Msg(MSG_CTRL_RELATION_WND,12);
				}
				else if(strcmp(pLine->getBuf(),"@退出门派") == 0 && strlen(SELF.GetTitle()) > 0 && !g_GuildData.IsHeader())
				{
					g_MsgBoxMgr.PopSimpleComfirm("离开行会后你所有的行会职位和行会银两都会清空！\n你是否确定离开行会？",MSG_CTRL_LEAVE_GUILD,0);
				}
				else
				{
					eSendTalkChannel eSendTalkChannelType = g_pGameData->GetSendTalkChannelType();
					//g_pGameControl->SEND_Message_Send(pLine);
					//优先使用指令方式,如果没有指令则使用频道
					string strHeader = "";
					if (c == '!' || //'!':喊话,"!!":组队,"!~":行会
						c == '$' || //'$':宗族
						c == '@' //用户指令或gm指令等
						)
					{
						//strSendContent = pLine->getBuf();
					}
					else
					{
						if(eSendTalkChannelType == ETC_TEAM && g_pGameData->GetTroopMembers() == 0)
						{
							g_TalkMgr.PushSysTalk("您当前不在队伍里，无法使用队伍聊天频道");
							return true;
						}
						if(eSendTalkChannelType == ETC_PHYLE && g_OtherData.GetPhyleName().empty())
						{
							g_TalkMgr.PushSysTalk("您当前不属于任何一个宗族，无法使用宗族聊天频道");
							return true;
						}
						if(eSendTalkChannelType == ETC_GUILD && strlen(SELF.GetGuildTitle()) == 0)
						{
							g_TalkMgr.PushSysTalk("您当前不属于任何一个行会，无法使用行会聊天频道");
							return true;
						}

						if(eSendTalkChannelType == ETC_VOICE)
						{
							if (!g_VoiceAdapter.IsInRoom())
							{
								g_TalkMgr.PushSysTalk("您当前没有加入语聊频道，无法使用语聊频道");
								return true;
							}
							else
							{
								string szTemp = pLine->c_str();
								szTemp = szTemp.substr(1,szTemp.length()-1);
								g_VoiceAdapter.SendTextMessage(szTemp.c_str(),szTemp.length() + 1,NULL);
								return true;
							}
						}

						//strSendContent = szSendTalkChannelHeader[m_eSendTalkChannelType];
						//strSendContent += pLine->getBuf();
						strHeader = szSendTalkChannelHeader[eSendTalkChannelType];
					}

					//g_pGameControl->SEND_Message_Send(strSendContent.c_str(),strSendContent.length());
					g_pGameControl->SEND_Message_Send(pLine,strHeader.c_str());

					//第一次喊话任务冒泡
					char c2 = 0;
					if(pLine->getBufLength() > 1)
					{
						c2 = *(pLine->c_str() + 1);
					}

					if (g_pGameData->HasPaoPaoStatus(EP_First_FaYan_Step2) && !g_pGameData->HasPaoPaoStatus(EP_First_FaYan_Step3) && (eSendTalkChannelType == ETC_SHOUT || (c == '!' && c2 != 0 && c2 != '!' && c2 != '~') ))
					{
						this->Msg(MSG_REMOVE_ARROWTIP_CONTROL,EP_First_FaYan_Step1,NULL);
						this->Msg(MSG_REMOVE_ARROWTIP_CONTROL,EP_First_FaYan_Step2,NULL);

						AddArrowTip(EP_First_FaYan_Step3,149,171,XAL_TOPLEFT,false,XAL_BOTTOMRIGHT);
					}
				}

				m_pPanelEdit->Clear();
				if(!g_pControl->SetTopWindowFocus())
				{
					this->SetFocus();
				}
				m_pPanelEdit->SetFakeFocus(false);
			}

			return true;
		}
	case MSG_CTRL_SWITCH_TALKVIEWER:
		{
			if(m_pTalkViewer)
				m_pTalkViewer->SwitchTalkViewer();
			return true;
		}
	case MSG_CTRL_RELEASE_FAKEFOCUS:
		{
			if (m_pPanelEdit)
			{
				m_pPanelEdit->SetFakeFocus(false);
			}
			return true;
		}
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == NULL)
				break;

			g_pAudio->Play(EAT_OTHER, 2, g_pAudio->GetRand()++);
			break;
		}
	case MSG_CTRL_INSERT_TEXT:
		{
			if(!pControl || !m_pPanelEdit)
				return true;

			if(dwData == 3)
			{
				if(g_pControl->GetFocusCtrl() != this && !m_pPanelEdit->IsFakeFocus())
					return true;
				else
					dwData = 0;
			}

			char tmp[1024] = {0};

			if(dwData == 2)
			{
				sprintf(tmp,"%s",(char*)pControl);
			}
			else if(dwData == 1)
			{
				sprintf(tmp,"/%s",(char*)pControl);
			}
			else
			{
				strcpy(tmp,(char*)pControl);
			}
			m_pPanelEdit->SetText(tmp);

			if(dwData != 0)
				m_pPanelEdit->SetFocus();

			return true;
			break;
		}
	case MSG_CTRL_INSERT_OBJECTLINK:
		{
			CGood *pGood = (CGood *)pControl;

			if(!pGood || pGood->GetID() == 0)
				return false;

			if(m_pPanelEdit == NULL || g_AIGoodMgr.IsFitBelt(*pGood) || pGood->GetStdMode() == 31)
				return false;

			m_pPanelEdit->InsertObjectLink(new CObjectLink(*pGood));
			return true;
			break;
		}
	case MSG_INPUT_CHAR:
		{
			CControl *pFocusControl = g_pControl->GetFocusCtrl();
			if(pFocusControl && pFocusControl != m_pPanelEdit && pFocusControl->IsNeedKeyInput())
				return false;//让pFocusControl去处理

			WORD wKey = LOWORD(dwData);	
			bool bNeedCloseNameWnd = true;
			switch(wKey)
			{
			case '/':
				{
					if(strlen(m_pPanelEdit->GetText()) == 0)
					{
						m_pPanelEdit->SetFocus();//焦点有可能设置不成功
						m_pPanelEdit->SetFakeFocus(true);
						if(g_TalkName.GetSize() > 0)
							g_pControl->PopupWindow(MSG_CTRL_NAMEWND,OPER_CREATE,1);

						bNeedCloseNameWnd = false;
					}
				}
				break;
			case '#':
				{
					if(m_pPanelEdit->IsFakeFocus() && strlen(m_pPanelEdit->GetText()) == 0)
					{
						m_pPanelEdit->SetFocus();//焦点有可能设置不成功
						m_pPanelEdit->SetFakeFocus(true);
						if(g_TalkName.GetLWNameSize() > 0)
							g_pControl->PopupWindow(MSG_CTRL_NAMEWND,OPER_CREATE,2);

						bNeedCloseNameWnd = false;
					}
				}
				break;
			case ' ':
				{
					if(!m_pPanelEdit->IsFakeFocus())
					{
						// 						if (g_TrusteeshipData.GetMicroControlPos() >= 0)//取消微操
						// 						{
						// 							g_pGameControl->SEND_MicroControl_Select(0);
						// 						}
						// 						else
						{
							m_pPanelEdit->SetFocus();//焦点有可能设置不成功
							m_pPanelEdit->SetFakeFocus(true);
						}
						return true;
					}
				}
				break;
			}

			if(m_pPanelEdit->IsFakeFocus())
			{
				if(bNeedCloseNameWnd)
				{
					g_pControl->PopupWindow(MSG_CTRL_NAMEWND,OPER_CLOSE);
				}

				return m_pPanelEdit->Msg(dwMsg,dwData,pControl);
			}

			return false;

			break;
		}
	case MSG_INPUT_KEYDOWN:
		{
			WORD wKey = LOWORD(dwData);	
			if (!m_pPanelEdit)
			{
				return false;
			}

			switch(wKey)
			{
			case VK_RETURN:
				if(m_pPanelEdit->IsFakeFocus())
				{
					if(g_pControl->MsgToWnd(MSG_CTRL_NAMEWND,dwMsg,dwData,pControl))
						return true;

					m_pPanelEdit->OnEntry();
					m_pPanelEdit->SetFakeFocus(false);
				}
				else
				{
					m_pPanelEdit->SetFakeFocus(true);
				}
				return true;
				break;
			case VK_ESCAPE:
				if(m_pPanelEdit->IsFakeFocus())
				{
					m_pPanelEdit->OnEscape();
					return true;
				}
				break;
			case VK_UP:                                       
				{
					if(g_pInput->IsShift() || g_pInput->IsCtrl() || g_pInput->IsAlt())	
						break;
					if(g_pControl->MsgToWnd(MSG_CTRL_NAMEWND,dwMsg,dwData,pControl))
						break;

					if(m_pTalkViewer)
						m_pTalkViewer->Wheel(-1);
					return true;
				}
			case VK_DOWN:
				{
					if(g_pInput->IsShift() || g_pInput->IsCtrl() || g_pInput->IsAlt())	
						break;
					if(g_pControl->MsgToWnd(MSG_CTRL_NAMEWND,dwMsg,dwData,pControl))
						break;

					if(m_pTalkViewer)
						m_pTalkViewer->Wheel(1);
					return true;
				}
			case VK_PRIOR:
				{
					if(g_pInput->IsShift() || g_pInput->IsCtrl() || g_pInput->IsAlt())
						break;

					if(m_pTalkViewer && m_pPanelEdit->IsFakeFocus())//有焦点才滚动,否则别人查看PK等信息时会出现下面滚动
						m_pTalkViewer->WheelPage(-1);

					return true;
				}
				break;
			case VK_NEXT:
				{
					if(g_pInput->IsShift() || g_pInput->IsCtrl() || g_pInput->IsAlt())	
						break;

					if(m_pTalkViewer && m_pPanelEdit->IsFakeFocus())//有焦点才滚动,否则别人查看PK等信息时会出现下面滚动
						m_pTalkViewer->WheelPage(1);

					return true;
				}
			default:
				break;
			}

			if(m_pPanelEdit->IsFakeFocus())
			{
				return m_pPanelEdit->Msg(dwMsg,dwData,pControl);
			}

			return false;
		}

	default:
		break;

	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}


//void CTalkViewWnd::ResetControlPos()
//{
//	m_bScale = g_bNeedScale;
//	if(g_EutUiType == EUT_CLASSIC)
//	{
//		m_iOriginalWidth = g_pGfx->GetWidth();
//		m_iWidth = m_iOriginalWidth;
//		if (g_bNeedScale)
//		{
//			m_iOriginalHeight = (int)(185*g_ScaleRate.fy+0.5f);
//		}
//		else
//		{
//			m_iOriginalHeight = 185;
//		}
//		m_iHeight = m_iOriginalHeight;
//	}
//
//	SetControlState();
//	CControlContainer::ResetControlPos();
//}

bool CTalkViewWnd::IsFakeFocus()
{
	if (m_pPanelEdit)
	{
		return m_pPanelEdit->IsFakeFocus();
	}

	return false;
}

bool CTalkViewWnd::OnClickTalkView()
{
	if(!m_pTalkViewer)
		return false;

	string strCommand = m_pTalkViewer->GetCommand();
	if(strCommand.length() <= 5)
		return false;

	//if( strncmp(strCommand.c_str(),"@@c2c",5) == 0)
	//{
	//	if(g_PetBoothData.IsPaimaiClosed()) //判断是否关闭了拍卖行
	//		return false;

	//	g_WooolStoreMgr.SetWooolStorePage(WOOOLSTORE_PAIMAI);
	//	g_WooolStoreMgr.SetPaiMaiStoreType(5);

	//	string str = strCommand.substr(5,strCommand.length() - 5);
	//	g_PetBoothData.SetRedirectPage(str.c_str());
	//	g_pControl->PopupWindow(MSG_CTRL_WOOOL_STORE_WND,OPER_RECREATE,3);
	//}
	//else 
	if(strncmp(strCommand.c_str(),"@@buy",5) == 0)
	{
		CQuickItem qitem;
		string str = strCommand.substr(5,strCommand.length() - 5);
		int pos = str.find_first_of('|');
		if(pos > 0)
		{
			qitem.strName = str.substr(0,pos);
			qitem.strItemID = str.substr(pos+1,str.size() - pos - 1);
		}
		else
		{
			qitem.strItemID = str;
		}

		g_WooolStoreMgr.BuyQuickItem(qitem,!qitem.strName.empty());
	}

	return true;
}
