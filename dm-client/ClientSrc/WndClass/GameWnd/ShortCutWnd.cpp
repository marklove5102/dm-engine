#include "ShortCutWnd.h"
#include "GameData/GameData.h"
#include "GameAI/AIMgr.h"
#include "GameControl/GameControl.h"
#include "Global/Interface/AudioInterface.h"
#include "GameAI/AIMgr.h"
#include "GameAI/AIConfigMgr.h"

INIT_WND_POSX(CShortCutWnd,POS_VARIABLE,POS_VARIABLE)

CShortCutWnd::CShortCutWnd()
{
	m_kSetShortCutKeyType = sm_dwWindowType;	

	m_iIndex = 474;
	
	m_iPages = 1;
	m_iAlignType = XAL_TOPLEFT;
	m_iOffX = 100;
	m_iOffY = 100;

	m_nKey = 0;

	switch (m_kSetShortCutKeyType)
	{
	case 1:
		{
			m_nConSkillID = 0;
			m_nPos = g_AIMgr.GetShortCutKeyMagicPos();
			m_nKey = SELF.GetMagic(m_nPos).GetShortCutKey();
			if(m_nKey != 0)
				m_nKey -= 0x30;
		}
		break;
	case 2:
		{
			m_nConSkillID = g_AIMgr.GetShortCutKeyConSkillID();
			ConSkillData* pConSkillData = g_AIMgr.FindConSkillByID(m_nConSkillID);
			if (pConSkillData && pConSkillData->dwKey != 0)
			{
				WORD wHighKey = HIWORD(pConSkillData->dwKey);
				WORD wLowKey = LOWORD(pConSkillData->dwKey);
				if (wHighKey == 0)
				{
					m_nKey = wLowKey - VK_F1 + 1;
				}
			}
		}
		break;
	case 3:
		break;
	}	
}

CShortCutWnd::~CShortCutWnd(void)
{

}

bool CShortCutWnd::Msg(DWORD dwMsg,DWORD dwData,CControl * pControl)
{
	switch ( dwMsg )
	{
	case MSG_CTRL_BUTTON_CLICK:
		{
			if(pControl == m_pKeyButton[0])
			{
				m_nKey = 1;
				m_pKeyButton[0]->SetChecked(true);
				m_pKeyButton[1]->SetChecked(false);
				m_pKeyButton[2]->SetChecked(false);
				m_pKeyButton[3]->SetChecked(false);
				m_pKeyButton[4]->SetChecked(false);
				m_pKeyButton[5]->SetChecked(false);
				m_pKeyButton[6]->SetChecked(false);
				m_pKeyButton[7]->SetChecked(false);
				m_pKeyButton[8]->SetChecked(false);
				//return true;
			}
			else if(pControl == m_pKeyButton[1])
			{
				m_nKey = 2;
				m_pKeyButton[0]->SetChecked(false);
				m_pKeyButton[1]->SetChecked(true);
				m_pKeyButton[2]->SetChecked(false);
				m_pKeyButton[3]->SetChecked(false);
				m_pKeyButton[4]->SetChecked(false);
				m_pKeyButton[5]->SetChecked(false);
				m_pKeyButton[6]->SetChecked(false);
				m_pKeyButton[7]->SetChecked(false);
				m_pKeyButton[8]->SetChecked(false);
				//return true;
			}
			else if(pControl == m_pKeyButton[2])
			{
				m_nKey = 3;
				m_pKeyButton[0]->SetChecked(false);
				m_pKeyButton[1]->SetChecked(false);
				m_pKeyButton[2]->SetChecked(true);
				m_pKeyButton[3]->SetChecked(false);
				m_pKeyButton[4]->SetChecked(false);
				m_pKeyButton[5]->SetChecked(false);
				m_pKeyButton[6]->SetChecked(false);
				m_pKeyButton[7]->SetChecked(false);
				m_pKeyButton[8]->SetChecked(false);
				//return true;
			}
			else if(pControl == m_pKeyButton[3])
			{
				m_nKey = 4;
				m_pKeyButton[0]->SetChecked(false);
				m_pKeyButton[1]->SetChecked(false);
				m_pKeyButton[2]->SetChecked(false);
				m_pKeyButton[3]->SetChecked(true);
				m_pKeyButton[4]->SetChecked(false);
				m_pKeyButton[5]->SetChecked(false);
				m_pKeyButton[6]->SetChecked(false);
				m_pKeyButton[7]->SetChecked(false);
				m_pKeyButton[8]->SetChecked(false);
				//return true;
			}
			else if(pControl == m_pKeyButton[4])
			{
				m_nKey = 5;
				m_pKeyButton[0]->SetChecked(false);
				m_pKeyButton[1]->SetChecked(false);
				m_pKeyButton[2]->SetChecked(false);
				m_pKeyButton[3]->SetChecked(false);
				m_pKeyButton[4]->SetChecked(true);
				m_pKeyButton[5]->SetChecked(false);
				m_pKeyButton[6]->SetChecked(false);
				m_pKeyButton[7]->SetChecked(false);
				m_pKeyButton[8]->SetChecked(false);
				//return true;
			}
			else if(pControl == m_pKeyButton[5])
			{
				m_nKey = 6;
				m_pKeyButton[0]->SetChecked(false);
				m_pKeyButton[1]->SetChecked(false);
				m_pKeyButton[2]->SetChecked(false);
				m_pKeyButton[3]->SetChecked(false);
				m_pKeyButton[4]->SetChecked(false);
				m_pKeyButton[5]->SetChecked(true);
				m_pKeyButton[6]->SetChecked(false);
				m_pKeyButton[7]->SetChecked(false);
				m_pKeyButton[8]->SetChecked(false);
				//return true;
			}
			else if(pControl == m_pKeyButton[6])
			{
				m_nKey = 7;
				m_pKeyButton[0]->SetChecked(false);
				m_pKeyButton[1]->SetChecked(false);
				m_pKeyButton[2]->SetChecked(false);
				m_pKeyButton[3]->SetChecked(false);
				m_pKeyButton[4]->SetChecked(false);
				m_pKeyButton[5]->SetChecked(false);
				m_pKeyButton[6]->SetChecked(true);
				m_pKeyButton[7]->SetChecked(false);
				m_pKeyButton[8]->SetChecked(false);
				//return true;				
			}
			else if(pControl == m_pKeyButton[7])
			{
				m_nKey = 8;
				m_pKeyButton[0]->SetChecked(false);
				m_pKeyButton[1]->SetChecked(false);
				m_pKeyButton[2]->SetChecked(false);
				m_pKeyButton[3]->SetChecked(false);
				m_pKeyButton[4]->SetChecked(false);
				m_pKeyButton[5]->SetChecked(false);
				m_pKeyButton[6]->SetChecked(false);
				m_pKeyButton[7]->SetChecked(true);
				m_pKeyButton[8]->SetChecked(false);
				//return true;
			}
			else if(pControl == m_pKeyButton[8])
			{
				m_nKey = 0;
				m_pKeyButton[0]->SetChecked(false);
				m_pKeyButton[1]->SetChecked(false);
				m_pKeyButton[2]->SetChecked(false);
				m_pKeyButton[3]->SetChecked(false);
				m_pKeyButton[4]->SetChecked(false);
				m_pKeyButton[5]->SetChecked(false);
				m_pKeyButton[6]->SetChecked(false);
				m_pKeyButton[7]->SetChecked(false);
				m_pKeyButton[8]->SetChecked(true);
				//return true;
			}
			else if(pControl == m_pOk)
			{
				switch (m_kSetShortCutKeyType)
				{
				case 1:
					{
						if(SELF.GetMagic(m_nPos).GetMagicID() == 0)
						{
							OnClickCloseButton();
							return true;
						}

						//主角那边设置组合技能
						WORD wMagicID = SELF.GetMagic(m_nPos).GetMagicID();

						if( m_nKey == 0 && SELF.GetMagic(m_nPos).GetShortCutKey()!= 0) // 去掉快捷键
						{
							g_pGameControl->SEND_ShortCut_Key_Change(wMagicID,0);
							SELF.GetMagic(m_nPos).SetShortCutKey(0);
						}
						else if(m_nKey != 0) // 设置快捷键
						{
							for(int i = 0 ; i < MAX_MAGIC_SKILL; i ++)
							{
								WORD tMagicID = SELF.GetMagic(i).GetMagicID();

								if(i == m_nPos || tMagicID == 0)
									continue;

								if(SELF.GetMagic(i).GetShortCutKey() == m_nKey + 0x30)
								{
									g_pGameControl->SEND_ShortCut_Key_Change(tMagicID,0);
									SELF.GetMagic(i).SetShortCutKey(0);
								}
							}

							//法决
							/*if (SELF.GetYiHuoShortCutKey() == m_nKey + 0x30)
							{
								g_pGameControl->SEND_YIHUO_NAME_SHORTCUT(0,SELF.GetYIHUOINFO().strName);
								SELF.SetYiHuoShortCutKey(0);
							}*/

							int retkey = m_nKey + VK_F1 - 1;

							ConSkillData* pConSkillData2 = g_AIMgr.FindConSkillByKey(MAKELONG(retkey,0));
							if (pConSkillData2 && pConSkillData2->dwKey != 0)
							{
								pConSkillData2->dwKey = 0;
								g_AICfgMgr.SaveShortCutConfig();
							}


							g_pGameControl->SEND_ShortCut_Key_Change(wMagicID,(char)0x30 + m_nKey);
							SELF.GetMagic(m_nPos).SetShortCutKey(m_nKey + 0x30);

							g_pControl->MsgToWnd(MSG_CTRL_SHORTCUTKEY_WND,MSG_CTRL_POPUP_ARROWTIP,wMagicID);
						}
					}
					break;
				case 2:
					{
						ConSkillData* pConSkillData = g_AIMgr.FindConSkillByID(m_nConSkillID);
						if (pConSkillData)
						{
							if (m_nKey == 0 && pConSkillData->dwKey != 0)
							{
								pConSkillData->dwKey = 0;
								g_AICfgMgr.SaveShortCutConfig();
							}
							else if(m_nKey != 0)
							{
								for(int i = 0 ; i < MAX_MAGIC_SKILL; i ++)
								{
									WORD tMagicID = SELF.GetMagic(i).GetMagicID();

									if(tMagicID == 0)
										continue;

									if(SELF.GetMagic(i).GetShortCutKey() == m_nKey + 0x30)
									{
										g_pGameControl->SEND_ShortCut_Key_Change(tMagicID,0);
										SELF.GetMagic(i).SetShortCutKey(0);
									}
								}

								//法决
								/*if (SELF.GetYiHuoShortCutKey() == m_nKey + 0x30)
								{
									g_pGameControl->SEND_YIHUO_NAME_SHORTCUT(0,SELF.GetYIHUOINFO().strName);
									SELF.SetYiHuoShortCutKey(0);
								}*/

								int retkey = m_nKey + VK_F1 - 1;

								ConSkillData* pConSkillData2 = g_AIMgr.FindConSkillByKey(MAKELONG(retkey,0));
								if (pConSkillData2 && pConSkillData2->dwKey != 0)
									pConSkillData2->dwKey = 0;

								pConSkillData->dwKey = MAKELONG(retkey,0);

								g_AICfgMgr.SaveShortCutConfig();
							}

							g_pControl->Msg(MSG_CTRL_UPDATE_SKILLWND,0,0);
						}
					}
					break;
				case 3:
					{
						//法决
						/*if (m_nKey == 0 && SELF.GetYiHuoShortCutKey() != 0)
						{
							g_pGameControl->SEND_YIHUO_NAME_SHORTCUT(0,SELF.GetYIHUOINFO().strName);
							SELF.SetYiHuoShortCutKey(0);
						}
						else */if(m_nKey != 0)
						{
							//将普通技能中的此快捷键清除
							for(int i = 0 ; i < MAX_MAGIC_SKILL; i ++)
							{
								WORD tMagicID = SELF.GetMagic(i).GetMagicID();

								if(tMagicID == 0)
									continue;

								if(SELF.GetMagic(i).GetShortCutKey() == m_nKey + 0x30)
								{
									g_pGameControl->SEND_ShortCut_Key_Change(tMagicID,0);
									SELF.GetMagic(i).SetShortCutKey(0);
								}
							}

							//将连续技能中的此快捷键清除
							int retkey = m_nKey + VK_F1 - 1;
							ConSkillData* pConSkillData2 = g_AIMgr.FindConSkillByKey(MAKELONG(retkey,0));
							if (pConSkillData2 && pConSkillData2->dwKey != 0)
							{
								pConSkillData2->dwKey = 0;
								g_AICfgMgr.SaveShortCutConfig();
							}

							//法决
							/*g_pGameControl->SEND_YIHUO_NAME_SHORTCUT(m_nKey + 0x30,SELF.GetYIHUOINFO().strName);
							SELF.SetYiHuoShortCutKey(m_nKey + 0x30);*/
						}
					}
					break;
				}				

				OnClickCloseButton();
			}
			else if(pControl == m_pCancel)
			{
				OnClickCloseButton();
			}
			else
				return CCtrlWindowX::Msg(dwMsg,dwData,pControl);

			g_pAudio->Play(EAT_OTHER,12,g_pAudio->GetRand()++);

			return true;
		}
		break;  
	}
	return CCtrlWindowX::Msg(dwMsg,dwData,pControl);
}

void CShortCutWnd::OnCreate()
{
	if (m_kSetShortCutKeyType == 1)
	{
		g_pControl->MsgToWnd(MSG_CTRL_SKILLWND,MSG_REMOVE_ARROWTIP_CONTROL,EP_First_Skill_SkillWnd_PaoPao);

		if(!g_pGameData->HasPaoPaoStatus(EP_First_ShortcutKeyWnd_PaoPao) && g_pGameData->HasPaoPaoStatus(EP_First_Skill_SkillWnd_PaoPao))
		{
			AddArrowTip(EP_First_ShortcutKeyWnd_PaoPao,22 + 28 - 9,50 + 28 - 9,XAL_TOPLEFT,false,XAL_TOPLEFT);
		}
	}


	char* pStr[9] = {"F1","F2","F3","F4","F5","F6","F7","F8","空"};

	for(int i = 0; i < 4 ; i++)
	{
		m_pKeyButton[i] = new CCtrlRadio();
		AddControl(m_pKeyButton[i]);
		m_pKeyButton[i]->Create(this,22 + i * 39,52,1653,1654);
		m_pKeyButton[i]->SetText(pStr[i], COLOR_BTN_NORMAL, COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE,0,FONT_YAHEI);
		m_pKeyButton[i]->SetTextOff(4, -3);
	}

	for(int i = 4; i < 8 ; i++)
	{
		m_pKeyButton[i] = new CCtrlRadio();
		AddControl(m_pKeyButton[i]);
		m_pKeyButton[i]->Create(this, 22 + (i-4) * 39,104,1653,1654);
		m_pKeyButton[i]->SetText(pStr[i], COLOR_BTN_NORMAL, COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE,0,FONT_YAHEI);
		m_pKeyButton[i]->SetTextOff(4, -3);
	}

	m_pOk = new CCtrlButton();
	AddControl(m_pOk);
	m_pOk->CreateEx(this,14,195,90,91,92);
	m_pOk->SetText("确 定", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

	m_pCancel = new CCtrlButton();
	AddControl(m_pCancel);
	m_pCancel->CreateEx(this,98,195,90,91,92);
	m_pCancel->SetText("取 消", COLOR_BTN_NORMAL ,COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE, 0, FONT_YAHEI);

	m_pKeyButton[8] = new CCtrlRadio();
	AddControl(m_pKeyButton[8]);
	m_pKeyButton[8]->Create(this,82,150,1653,1654);
	m_pKeyButton[8]->SetText(pStr[8], COLOR_BTN_NORMAL, COLOR_BTN_MOUSEON, COLOR_BTN_PRESS, COLOR_BTN_DISABLE, FONTSIZE_MIDDLE,0,FONT_YAHEI);
	m_pKeyButton[8]->SetTextOff(4, -3);

	if(m_nKey>=1 && m_nKey <=8)
	{
		m_pKeyButton[m_nKey-1]->SetChecked(true);
	}

	SetCloseButton(157,1,80);
}
